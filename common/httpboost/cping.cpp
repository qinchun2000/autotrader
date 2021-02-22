#include "cping.hpp"

//构造函数对参数的初始化
Cping::Cping(const char * ip, int timeout)
{
    m_strIp = ip;
    m_copy_Ip = ip;

    m_nSend = 0;
    m_nRecv = 0;

    m_dTotalResponseTimes = 0;

    if(timeout > MAX_WAIT_TIME)
    {
        m_nMaxTimeWait = MAX_WAIT_TIME;
    }
    else
    {
        m_nMaxTimeWait = timeout;
    }
}

Cping::~Cping()
{
    if(!CloseSocket())
    {
        cout<<"CloseSocket failed!"<<endl;
    }
}

/*ping函数描述了ping的框架
 *1.创建一个套接字并获取（转换）目标IP地址
 *2.发送报文
 *3.接收报文
 *4.结束打印信息
*/
bool Cping::ping(int times)
{
    //signal(SIGINT,statistics);
    if(!CreateSocket())
    {
        printf("CreateSocket failed!!!\n");
        return false;
    }
    printf("PING %s(%s): %d bytes data in ICMP packets.\n", m_strIp.c_str(),
                m_copy_Ip.c_str(), SEND_DATA_LEN);
    while(times--)
    {
        send_packet();
        recv_packet();
        sleep(1);   //每秒发送一个报文
    }
    statistics(SIGINT);
    return true;
}

//创建socket，并完成m_dest_addr结构体的赋值
bool Cping::CreateSocket()
{

    char buf[2048];                         //gethostbyname_r函数临时的缓冲区，用来存储过程中的各种信息
    int errnop = 0;                         //gethostbyname_r函数存储错误码
    unsigned long inaddr;                   //用来保存网络字节序的二进制地址
    struct hostent hostinfo,*dest_phost;    //用于gethostbyname_r存放IP信息
    struct protoent *protocol;              //指向协议protoent的一个结构体

    //1.创建一个socket
    //1.1通过协议名称icmp获取协议编号
    if ((protocol = getprotobyname("icmp")) == NULL)
    {
        printf("CreateSocket: getprotobyname failed:%d\n",errno);

        return false;
    }
    //1.2创建原始套接字时，需要有root权限，防止应用程序绕过内建安全机制
    if(-1 == (m_nSocketfd = socket(AF_INET,SOCK_RAW,protocol->p_proto)))
    {
        printf("CreateSocket: create socket failed:%d\n",errno);

        return false;
    }

    //回收root权限,设置当前用户权限
    unsigned int rid = getuid();
    setuid(rid);

    //2.设置m_dest_addr结构体
    m_dest_addr.sin_family = AF_INET;

    bzero(&(m_dest_addr.sin_zero),8);

    //2.1检测用户输入的参数是主机名还是IP地址（点分十进制地址），并转化为网络字节序的二进制地址
    if((inaddr=inet_addr(m_strIp.c_str())) == INADDR_NONE)
    {
        if(gethostbyname_r(m_strIp.c_str(),&hostinfo,buf,sizeof(buf),&dest_phost,&errnop))
        {
            printf("CreateSocket: gethostbyname error %s failed:%d\n",m_strIp.c_str(),errnop);

            return false;
        }
        else//输入的是主机名
        {
            m_dest_addr.sin_addr = *((struct in_addr *)dest_phost->h_addr);//？会不会有问题直接赋值
        }
    }
    else//输入的是IP地址
    {
        m_dest_addr.sin_addr.s_addr = inaddr;
    }
    //2.2将用户输入的IP地址保存下来，用于后续包的校验
    m_copy_Ip = inet_ntoa(m_dest_addr.sin_addr);
    /*
    if(inet_ntop(AF_INET,(void*)m_copy_Ip.c_str(),(char*)&(m_dest_addr.sin_addr.s_addr),sizeof(m_copy_Ip)))
    {
        printf("CreateSocket: inet_ntop error!,errno = %d\n",errno);

        exit(1);
    }
    */
    return true;
}

//关闭套接字
bool Cping::CloseSocket()
{

    bool flag = false;
    if(m_nSocketfd)
    {
        close(m_nSocketfd);
        flag = true;
    }
    return flag;

}

//发送一个IP包
void Cping::send_packet(void)
{
    int packetsize;      //包的大小
    packetsize = pack(m_nSend);

    if((sendto(m_nSocketfd,m_sendpacket,packetsize,0,(const struct sockaddr*)&m_dest_addr,sizeof(m_dest_addr))) < 0)
    {
        printf("send_packet: send error :%d\n",errno);
    }
    m_nSend++;
  //发送一个IP包以后，发送次数加1
}

//接收一个IP包
void Cping::recv_packet(void)
{

    int fromlen,packetsize,n;
    while(m_nRecv < m_nSend)
    {
        struct timeval timeout;
        fd_set readfd;                 //定义一个文件集合
        FD_ZERO(&readfd);
//将该集合清空
        FD_SET(m_nSocketfd,&readfd);
//将套接字描述符添加到readfd集合中
        int maxfd = m_nSocketfd + 1;    //select监听的最大文件描述符
        timeout.tv_sec = m_nMaxTimeWait;
//设置select的超时等待时间
        timeout.tv_usec = 0;

        n = select(maxfd,&readfd,NULL,NULL,&timeout);//用select实现非阻塞I/O

        switch(n)
        {
            case 0:
                printf("recv_packet: select time out :%d\n",errno);
                break;
            case -1:
                printf("recv_packet: select error :%d\n",errno);
                break;
            default:
                if(FD_ISSET(m_nSocketfd,&readfd)) //通过检测m_nSocketfd是否还在集合readfd中，判断m_nSocketfd是否有可读数据
                {
                    //recevfrom接收数据同时获取数据发送者的源地址，保存在m_from_addr中
                    if((packetsize=recvfrom(m_nSocketfd,m_recvpacket,sizeof(m_recvpacket),0,(struct sockaddr *)&m_from_addr
                    ,(socklen_t*)&fromlen)) < 0)
                    {
                        printf("packetsize = %d\n",packetsize);
                        printf("recv_packet: recv error :%d\n",errno);
                        return ;
                    }
                    //保存最后一个接收到包的时间
                    gettimeofday(&m_tvrecv,NULL);
                    m_end_tvrecv.tv_usec = m_tvrecv.tv_usec;
                    m_end_tvrecv.tv_sec = m_tvrecv.tv_sec;
                    //拆包
                    if(unpack(m_recvpacket,packetsize) == -1)
                    {
                        continue;
                    }
                    m_nRecv++;
                }//end if
                break;
        }//end switch
    }//end whlie

}//end send_packet

//首先要构建一个ICMP数据包，对ICMP结构体成员变量赋值
int Cping::pack(int pack_number)
{
    int packsize;
    struct icmp *pIcmp;
    struct timeval *pTime;

    pIcmp = (struct icmp*)m_sendpacket;

    //类型和代码分别为ICMP_ECHO,0代表请求回送
    pIcmp->icmp_type = ICMP_ECHO;                    //ICMP报文类型，type和code共同决定
    pIcmp->icmp_code = 0;
    pIcmp->icmp_cksum = 0;                           //校验和
    pIcmp->icmp_seq = pack_number;                   //包序列号
    pIcmp->icmp_id = getpid();                       //取进程号作为标志
    packsize = 8 + SEND_DATA_LEN;                    //ICMP报文的大小包括8个字节的报头，和报文数据
    pTime = (struct timeval *)pIcmp->icmp_data;      //数据段存放发送时间
    gettimeofday(pTime, NULL);
    if(m_nSend == 0)                                 //第一次发送报文的时间
    {
        m_begin_tvrecv.tv_usec = pTime->tv_usec;
        m_begin_tvrecv.tv_sec = pTime->tv_sec;
    }
   pIcmp->icmp_cksum = cal_chksum((unsigned short *)pIcmp,packsize);

    return packsize;                                //返回ICMP报文大小
}

//获取到IP数据包以后，要去除IP报头，完成ICMP报文的校验以及时间的提取
int Cping::unpack(char *buf, int len)
{
    int i,iphdrlen;
    struct icmp *pIcmp;
    struct timeval *tvsend;
    struct ip* recv_ip = (struct ip*)buf;
    double rtt;

    iphdrlen = recv_ip->ip_hl << 2;                    //求ip报头长度,即ip报头的长度标志乘4
    pIcmp = (struct icmp*)(buf + iphdrlen);         //pIcmp指向的是ICMP头部，因此要跳过IP头部数据
    len -= iphdrlen;                        //ICMP报头及ICMP数据报的总长度
    if (len < 8) //小于ICMP报头长度则不合理
    {
       printf( "ICMP packets\'s length is less than 8");

        return -1;
    }
    //对ICMP报文进行数据校验，提取时间
    if((pIcmp->icmp_type == ICMP_ECHOREPLY) && (m_copy_Ip == inet_ntoa(m_from_addr.sin_addr)) && (pIcmp->icmp_id = getpid()) )
    {
        tvsend = (struct timeval *)pIcmp->icmp_data;

        tv_sub(&m_tvrecv,tvsend);

        rtt = m_tvrecv.tv_sec * 1000 + (double)m_tvrecv.tv_usec / 1000; //以毫秒为单位计算rtt

        //打印报文相关信息
       printf("%d byte from %s : icmp_seq=%u ttl=%d time=%.3fms\n",
                len,
                inet_ntoa(m_from_addr.sin_addr),
                pIcmp->icmp_seq,
                recv_ip->ip_ttl,
                rtt);
    }
    else
    {
        printf("throw away the old package %d\tbyte from %s\t: icmp_seq=%u\tttl=%d\trtt=%.3f\tms",
        len,
        inet_ntoa(m_from_addr.sin_addr),
        pIcmp->icmp_seq,
        recv_ip->ip_ttl,
        rtt);

        return -1;
    }
    return 1;
}


//时间的计算以及信息打印
void Cping::tv_sub(struct timeval *out, struct timeval *in)
{
    if((out->tv_usec -= in->tv_usec) < 0)
    {
        --out->tv_sec;
        out->tv_usec += 10000000;
    }
    out->tv_sec -= in->tv_sec;

}
//打印收发信息
void Cping::statistics(int sig)
{
    tv_sub(&m_end_tvrecv,&m_begin_tvrecv);
    m_dTotalResponseTimes = m_end_tvrecv.tv_sec * 1000 + (double)m_end_tvrecv.tv_usec / 1000;
    printf("------statistics------\n");
    printf("%d packets transmitted, %d received , %d%% lost,time:%.3lfms\n", m_nSend, m_nRecv,
            (m_nSend - m_nRecv) / m_nSend * 100,m_dTotalResponseTimes);
    close(m_nSocketfd);
}


//ICMP校验和计算函数，这个校验和函数是通用的
unsigned short Cping::cal_chksum(unsigned short *addr, int len)
{

    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;
    //采用32bit加法
    while(nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if( nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    //把高16位加到低16位上去
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;

}
