#include "codeutil.hpp"

CodeUtil::CodeUtil()
{

}
inline std::string CodeUtil::GetProductID(const char * pinstrumentid)
{
		std::string ProductID;
		char r[30];
		strcpy(r,"(^[a-zA-Z]*[^0-9]+)");
		char *regex = r;
		regex_t comment;
		size_t nmatch;
		regmatch_t regmatch[20];
		regcomp(&comment, regex, REG_EXTENDED|REG_NEWLINE);
		int j = regexec(&comment,pinstrumentid,sizeof(regmatch)/sizeof(regmatch_t),regmatch,0);
		if(j == 0)	{
			 ProductID.assign(pinstrumentid+regmatch[0].rm_so, regmatch[0].rm_eo - regmatch[0].rm_so);
//			 printf("GetProductID----->   匹配主力合约的关键字--mat-[%s]---> \n", ProductID.c_str());
		}

		regfree(&comment);
		return ProductID;
}


std::string CodeUtil::GetNumber(const char * pinstrumentid)
{
		std::string ProductID;
		char r[15];
		strcpy(r,"([0-9]+)");
		char *regex = r;
		regex_t comment;
		size_t nmatch;
		regmatch_t regmatch[20];
		regcomp(&comment, regex, REG_EXTENDED|REG_NEWLINE);
		int j = regexec(&comment,pinstrumentid,sizeof(regmatch)/sizeof(regmatch_t),regmatch,0);
		if(j == 0)
		{
			 ProductID.assign(pinstrumentid+regmatch[0].rm_so, regmatch[0].rm_eo - regmatch[0].rm_so);
//			 printf("GetProductID----->   匹配主力合约的关键字--mat-[%s]---> \n", ProductID.c_str());
		}

		regfree(&comment);
		return ProductID;
}
std::string CodeUtil::GetInstrumentid_NumberPartString(const char * pinstrumentid)
{

		std::string Number;
		char r[15];
		strcpy(r,"([0-9]+)");
		char *regex = r;
		regex_t comment;
		size_t nmatch;
		regmatch_t regmatch[20];
		regcomp(&comment, regex, REG_EXTENDED|REG_NEWLINE);
		int j = regexec(&comment,pinstrumentid,sizeof(regmatch)/sizeof(regmatch_t),regmatch,0);
		if(j == 0)
		{
			Number.assign(pinstrumentid+regmatch[0].rm_so, regmatch[0].rm_eo - regmatch[0].rm_so);
//			 printf("GetProductID----->   匹配主力合约的关键字--mat-[%s]---> \n", ProductID.c_str());
		}

		regfree(&comment);
		return Number;
}
std::string  CodeUtil::GetLastYearStringbyInstrumentID(const char * pinstrumentid)
{
	int num;
	std::string str_lastyear;
	std::string str_ym;
	str_ym = this->GetNumber(pinstrumentid);
	std::string str_longym;

	// CZCE
	if (str_ym.length()==3){
		char last_current[10];
		char last_instrumentid[10];
		strcpy(last_current,this->GetCurrentYearString().substr(3,1).c_str());
		strcpy(last_instrumentid,str_ym.substr(0,1).c_str());
		if(strcmp(last_current,last_instrumentid)==0){
			int current_year = atoi(this->GetCurrentYearString().c_str());
			int last_year = current_year -1;

			char tmp[10];
			sprintf(tmp,"%04d",last_year);
			str_lastyear=tmp;
//			 printf("CodeUtil::GetLastYearStringbyInstrumentID----->CZCE %s  lastyear:%s %d  \n", pinstrumentid,str_lastyear.c_str(),last_year);
			return str_lastyear;
		}
		else{
			int current_year = atoi(this->GetCurrentYearString().c_str());
			char tmp[10];
			sprintf(tmp,"%04d",current_year);
			str_lastyear=tmp;
//			printf("CodeUtil::GetLastYearStringbyInstrumentID----->CZCE  年份自动加1 %s  lastyear:%s %d  \n", pinstrumentid,str_lastyear.c_str(),current_year);
			return str_lastyear;
		}

	}
	else if(str_ym.length()==4){  // DCE SHFE
		str_longym=this->GetCurrentYearString().substr(0,2);
		str_longym.append(str_ym.substr(0,2));
		int current_year = atoi(str_longym.c_str());
		int last_year = current_year -1;

		char tmp[10];
		sprintf(tmp,"%04d",last_year);
		str_lastyear=tmp;

//		 printf("CodeUtil::GetLastYearStringbyInstrumentID----->%s  lastyear:%s %d  \n", pinstrumentid,str_lastyear.c_str(),last_year);
		return str_lastyear;
	}
	else{
		 printf("CodeUtil::GetLastYearStringbyInstrumentID----->ERROR   匹配主力合约的关键字年份错误!!! [%s]--->[%s] \n", pinstrumentid,str_longym.c_str());
		 return 0;
	}


}
int CodeUtil::GetYearbyInstrumentID(const char * pinstrumentid)
{
	int num;
	std::string str_ym;
	str_ym = this->GetNumber(pinstrumentid);
	std::string str_longym;
	if (str_ym.length()==3){
		char last_current[10];
		char last_instrumentid[10];
		strcpy(last_current,this->GetCurrentYearString().substr(3,1).c_str());
		strcpy(last_instrumentid,str_ym.substr(0,1).c_str());
		if(strcmp(last_current,last_instrumentid)==0){
			str_longym=this->GetCurrentYearString().substr(0,3);
			str_longym.append(str_ym.substr(0,1));
			num=atoi(str_longym.c_str());
			 printf("CodeUtil::GetYearMonthbyInstrumentID----->CZCE %s  %s %d  \n", pinstrumentid,str_longym.c_str(),num);
			return num;
		}
		else{
			num=atoi(this->GetCurrentYearString().c_str())+1;
			printf("CodeUtil::GetYearMonthbyInstrumentID----->CZCE  年份自动加1 %s  %s %d  \n", pinstrumentid,str_longym.c_str(),num);
			return num;
		}

	}
	else if(str_ym.length()==4){
		str_longym=this->GetCurrentYearString().substr(0,2);
		str_longym.append(str_ym.substr(0,2));
		num=atoi(str_longym.c_str());
		 printf("CodeUtil::GetYearMonthbyInstrumentID----->%s  %s %d  \n", pinstrumentid,str_longym.c_str(),num);
		return num;
	}
	else{
		 printf("CodeUtil::GetYearMonthbyInstrumentID----->ERROR   匹配主力合约的关键字年份错误!!! [%s]--->[%s] \n", pinstrumentid,str_longym.c_str());
		 return 0;
	}


}
// 返回格式:190901 , 200101
int CodeUtil::GetYearMonthbyInstrumentID(const char * pinstrumentid)
{
	int num;
	std::string str_ym;
	str_ym = this->GetNumber(pinstrumentid);
	std::string str_longym;
	if (str_ym.length()==3){
		char last_current[10];
		char last_instrumentid[10];
		strcpy(last_current,this->GetCurrentYearString().substr(3,1).c_str());
		strcpy(last_instrumentid,str_ym.substr(0,1).c_str());
		if(strcmp(last_current,last_instrumentid)==0){
			str_longym=this->GetCurrentYearString().substr(0,3);
			str_longym.append(str_ym);
			num=atoi(str_longym.c_str());
//			 printf("CodeUtil::GetYearMonthbyInstrumentID----->CZCE %s  %s num:%d  \n", pinstrumentid,str_longym.c_str(),num);
			return num;
		}
		else{

			int tmp=atoi(this->GetCurrentYearString().c_str())+1;
			char char_nextyear[10];

			sprintf(char_nextyear,"%04d",tmp);

			str_longym=char_nextyear;
			str_longym.append(str_ym.substr(1,2));
			num=atoi(str_longym.c_str());
//			printf("CodeUtil::GetYearMonthbyInstrumentID----->CZCE  年份自动加1 %s  %s num:%d  \n", pinstrumentid,str_longym.c_str(),num);
			return num;
		}

	}
	else if(str_ym.length()==4){
		str_longym=this->GetCurrentYearString().substr(0,2);
		str_longym.append(str_ym);
		num=atoi(str_longym.c_str());
//		 printf("CodeUtil::GetYearMonthbyInstrumentID----->%s  %s num:%d  \n", pinstrumentid,str_longym.c_str(),num);
		return num;
	}
	else{
		 printf("CodeUtil::GetYearMonthbyInstrumentID----->ERROR   匹配主力合约的关键字年份错误!!! [%s]--->[%s] \n", pinstrumentid,str_longym.c_str());
		 return 0;
	}


}


std::string CodeUtil::GetSectionByInstrumentid(const char * pinstrumentid)
{
		std::string section;
		char productid[31];

		strcpy(productid,this->GetProductID(pinstrumentid).c_str());
		section = this->GetSectionByProductid(productid);
		return section;

}
std::string CodeUtil::GetSectionByProductid(const char * productid)
{
		std::string section;

		char buf[20];

		if (!strcmp(productid,"ZC")||!strcmp(productid,"FG")||!strcmp(productid,"jm")||
			!strcmp(productid,"j")||!strcmp(productid,"i")||!strcmp(productid,"rb")||
			!strcmp(productid,"hc")||!strcmp(productid,"SF")||!strcmp(productid,"SM")||!strcmp(productid,"ss"))  {
			  strcpy(buf,"Black");
		}
		else if (!strcmp(productid,"ru")||!strcmp(productid,"fu")||!strcmp(productid,"bu")||
			  !strcmp(productid,"l")||!strcmp(productid,"pp")||!strcmp(productid,"eg")||
			  !strcmp(productid,"v")||!strcmp(productid,"TA")||!strcmp(productid,"MA")
			  ||!strcmp(productid,"eb")||!strcmp(productid,"UR")||!strcmp(productid,"SA"))	  {
		  strcpy(buf,"Oil");
		}
		else if (!strcmp(productid,"y")||!strcmp(productid,"p")||!strcmp(productid,"OI"))	  {
		  strcpy(buf,"CookOil");
		}
		else if (!strcmp(productid,"cs")||!strcmp(productid,"c"))    {
		  strcpy(buf,"Corn");
		}
		else if (!strcmp(productid,"a")||!strcmp(productid,"b")||!strcmp(productid,"m")||!strcmp(productid,"RM"))    {
		  strcpy(buf,"Feed");
		}
		else if (!strcmp(productid,"SR")||!strcmp(productid,"sp")||!strcmp(productid,"jd")||
			  !strcmp(productid,"AP")||!strcmp(productid,"CY")||!strcmp(productid,"CF")||!strcmp(productid,"CJ"))    {
		  strcpy(buf,"Single");
		}
		else if (!strcmp(productid,"au")||!strcmp(productid,"ag"))    {
		  strcpy(buf,"Precious");
		}
		else if (!strcmp(productid,"cu")||!strcmp(productid,"al")||!strcmp(productid,"ni")||
			  !strcmp(productid,"sn")||!strcmp(productid,"pb")||!strcmp(productid,"zn"))  {
		  strcpy(buf,"Metal");
		}
		else if (!strcmp(productid,"TF")||!strcmp(productid,"T"))  {// 5年国债
		  strcpy(buf,"Finance");
		}
		else if (!strcmp(productid,"IC")||!strcmp(productid,"IF")||!strcmp(productid,"IH")) { // 中证500
		  strcpy(buf,"Stock");
		}
		else if (!strcmp(productid,"sc")) { //原油   待调整
		  strcpy(buf,"Nothing");
		}
		else  {
		  strcpy(buf,"Nothing");
		}
		section =buf;
		return section;
}

std::string CodeUtil::GetDayNightByProductid(const char * productid)
{
		std::string daynight;

		char buf[200];

		if (!strcmp(productid,"RM") || !strcmp(productid,"OI") ||
			  !strcmp(productid,"TA")||!strcmp(productid,"MA")||
			  !strcmp(productid,"CF")||!strcmp(productid,"CY")  ||
			  !strcmp(productid,"SR")||!strcmp(productid,"ZC") ||
			  !strcmp(productid,"FG")||!strcmp(productid,"SA"))	  {

		  strcpy(buf,"DayNight");
		}
		else if (!strcmp(productid,"AP")||!strcmp(productid,"SF") ||
				!strcmp(productid,"SM")||!strcmp(productid,"CJ")||!strcmp(productid,"UR"))	  {

		  strcpy(buf,"Day");
		}
		else if (!strcmp(productid,"a") ||!strcmp(productid,"b") ||
			  !strcmp(productid,"c")||!strcmp(productid,"cs") || //
			  !strcmp(productid,"m") ||!strcmp(productid,"eg")||
			  !strcmp(productid,"p")||!strcmp(productid,"y")||
			  !strcmp(productid,"jm") || !strcmp(productid,"j") ||
			  !strcmp(productid,"i")||!strcmp(productid,"v") ||
			  !strcmp(productid,"pp") ||!strcmp(productid,"l") ||!strcmp(productid,"eb"))    {

		  strcpy(buf,"DayNight");
		}
		else if (!strcmp(productid,"jd"))	  {

		  strcpy(buf,"Day");
		}
		else if (!strcmp(productid,"rb")||!strcmp(productid,"hc")||
			  !strcmp(productid,"ru")||!strcmp(productid,"fu")||!strcmp(productid,"bu")||
			  !strcmp(productid,"au")||!strcmp(productid,"ag")||
			  !strcmp(productid,"zn")||!strcmp(productid,"pb")||!strcmp(productid,"sn")||
			  !strcmp(productid,"ni")||!strcmp(productid,"cu")||!strcmp(productid,"al")||
			  !strcmp(productid,"sp")||!strcmp(productid,"ss"))	  {
		  strcpy(buf,"DayNight");
		}
		else if (!strcmp(productid,"sc")) { //原油   待调整
		  strcpy(buf,"Day");
		}
		else if (!strcmp(productid,"TF")||!strcmp(productid,"T")||!strcmp(productid,"TS")||  // 5年国债
			  !strcmp(productid,"IC")||!strcmp(productid,"IF")||!strcmp(productid,"IH"))	  {
		  strcpy(buf,"Day");
		}
		else  {
		  strcpy(buf,"No");
		}
		daynight=buf;
		return daynight;
}

bool CodeUtil::IsNightByProductid(const char * productid)
{
	std::string daynight=this->GetDayNightByProductid(productid);
	bool ret=false;
	if (strcmp(daynight.c_str(),"Day")==0  )	  	{
	//		printf("day   --> %s   \n ",product);
		ret=false;
	}
	else if (strcmp(daynight.c_str(),"DayNight")==0 )  	{
	//		printf("night   --> %s   \n ",product);
		ret=true;
	}
	else{
		ret=false;
	}

	return ret;
}


std::string CodeUtil::GetWorkDirectory()
{
//	printf("config   GetWorkDirectory begin  \n");
	int PATH_MAX=1024;
    char dir[PATH_MAX] ;
    char syspath[PATH_MAX] ;
    memset(dir,0,PATH_MAX*sizeof(int));
    memset(syspath,0,PATH_MAX*sizeof(int));
//    int n = readlink("/proc/self/exe", dir, PATH_MAX);

    std::string s2;
	if(!getcwd(dir, sizeof(dir)))
	{
		s2="";
		return  s2;
	}
//	printf("config   GetWorkDirectory 000  \n");
    const char *ptr;
    ptr = strrchr(dir, '/');

    int length = strlen(dir) - strlen(ptr);
    std::string s1(dir);
//    printf("config   GetWorkDirectory 1111  \n");
    s2 = s1.substr(0, length+1);
    memcpy(syspath, (char*)s2.data(), PATH_MAX);

    ptr = strrchr(syspath, '/');
    length = strlen(syspath) - strlen(ptr);
    s2 = s2.substr(0, length);

    cout << "dir     : " << dir << endl;
    cout << "syspath : " << syspath << endl;
    cout << "s2      : " << s2 << endl;

    return s2;
}

std::string CodeUtil::GetCurrentDirectory()
{
//	printf("config   GetWorkDirectory begin  \n");
//	int PATH_MAX=1024;
    char dir[1024] ={0};
    char syspath[1024]={0} ;
  //  memset(dir,0,PATH_MAX*sizeof(int));
   // memset(syspath,0,PATH_MAX*sizeof(int));
//    int n = readlink("/proc/self/exe", dir, PATH_MAX);

    std::string s2;
	if(!getcwd(dir, sizeof(dir)))
	{
		s2="";
		return  s2;
	}
//	printf("config   GetWorkDirectory 000  \n");
//    const char *ptr;
//    ptr = strrchr(dir, '/');
//
//    int length = strlen(dir) - strlen(ptr);
//    std::string s1(dir);
////    printf("config   GetWorkDirectory 1111  \n");
//    s2 = s1.substr(0, length+1);
//    memcpy(syspath, (char*)s2.data(), PATH_MAX);
//
//    ptr = strrchr(syspath, '/');
//    length = strlen(syspath) - strlen(ptr);
//    s2 = s2.substr(0, length);

//    cout << "dir     : " << dir << endl;
//    cout << "syspath : " << syspath << endl;
//    cout << "s2      : " << s2 << endl;

    return dir;
}

void CodeUtil::codeConvert(char * from,char * to,char * src,char * dst,size_t dstSize){
        size_t sl,dl;
        sl = strlen(src);
        dl = dstSize;
        char * pIn = src;
    char * pOut = dst;
        memset(dst,'\0',dstSize);
        iconv_t conv = iconv_open(to,from);
        iconv(conv, &pIn, &sl, &pOut, &dl);
        iconv_close(conv);
}

void CodeUtil::gbk2utf8(char * src,char * dst,size_t dstSize){
        char * gbk = (char*)"GBK";
        char * utf8 = (char*)"UTF-8";
        codeConvert(gbk,utf8,src,dst,dstSize);
}

//int utf82gbk(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
//    return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
//}
void CodeUtil::utf82gbk(char * src,char * dst,size_t dstSize){
        char * gbk = (char*)"GBK";
        char * utf8 = (char*)"UTF-8";
        codeConvert(utf8,gbk,src,dst,dstSize);
}




bool CodeUtil::GetHostInfo(std::string& hostName, std::string& Ip) {
	char name[256];
	gethostname(name, sizeof(name));
	hostName = name;

	struct hostent* host = gethostbyname(name);
	char ipStr[32];
	const char* ret = inet_ntop(host->h_addrtype, host->h_addr_list[0], ipStr, sizeof(ipStr));
	if (NULL==ret) {
		std::cout << "hostname transform to ip failed";
		return false;
	}
	Ip = ipStr;
	return true;
}

std::string CodeUtil::GetCurrentYearString(){
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);
		char char_year[10];
		char char_month[10];
		char char_day[10];

		sprintf(char_year,"%04d",t->tm_year+1900);
		sprintf(char_month,"%02d",t->tm_mon+1);
		sprintf(char_day,"%02d", t->tm_mday);
//		printf("GetTodayString <<<<<<---- char year:%s  month:%s   day:%s :\n",char_year,char_month,char_day);
		std::string str_today;
		str_today.append(char_year);
//		str_today.append(char_month);
//		str_today.append(char_day);

		return str_today;
}
int CodeUtil::GetCurrentYearNumber(){
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);

		int year = t->tm_year+1900;
		return year;
}
int CodeUtil::GetCurrentMonthNumber(){
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);

		int month = t->tm_mon+1;
		return month;
}
int CodeUtil::GetCurrentDayNumber(){
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);

		int day = t->tm_mday;
		return day;
}

