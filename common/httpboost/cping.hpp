#ifndef _PING_H
#define _PING_H
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define PACKET_SIZE     4096
#define SEND_DATA_LEN   56
#define ERROR           -1
#define SUCCESS         1
#define MAX_WAIT_TIME  3
#define MAX_NO_PACKETS  4

using namespace std;

class Cping
{
public:
    Cping(const char * ip, int timeout);
    Cping(const Cping& orig);
    virtual ~Cping();

private:
    std::string m_strIp;            //用来保存用户输入的IP
    std::string m_copy_Ip;          //这个也是用来保存用户的输入IP，相当于一个备份

    int m_nSend;                   //用来记录发送的数据包数量
    int m_nRecv;                   //用来记录接收到的数据包数量
    struct sockaddr_in m_dest_addr;     //用来保存目标IP的套接字地址
    struct sockaddr_in m_from_addr;     //用来保存来自目标IP的套接字地址
    char m_sendpacket[PACKET_SIZE];     //用于保存发送的ICMP包
    char m_recvpacket[PACKET_SIZE];     //用于保存接收的ICMP包
    struct timeval m_tvrecv;            //收到数据包的时间
    struct timeval m_begin_tvrecv;      //开始发送数据包的时间
    struct timeval m_end_tvrecv;        //最后一次收到的数据包的时间
    double m_dTotalResponseTimes;       //发送到接收所有数据包的时间
    int m_nSocketfd=0;
//套接字

    int m_nMaxTimeWait;         //每次接收数据包的最长等待时间
public:
    bool ping(int times);
    //创建关闭socket
    bool CreateSocket();
    bool CloseSocket();
    //收发一个IP包
    void send_packet(void);
    void recv_packet(void);
    //数据打包和拆包
    int pack(int pack_no);
    int unpack(char *buf, int len);
    //时间的计算以及信息打印
    void tv_sub(struct timeval *out, struct timeval *in);
    void statistics(int sig);
    //ICMP校验和计算函数
    static unsigned short cal_chksum(unsigned short *addr, int len);
};

#endif
