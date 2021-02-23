#include "dateutil.hpp"

DateUtil::DateUtil(){

}

int DateUtil::GetRandomNumber()
{
	int RandomNumber;
	srand((unsigned)time(NULL));//time()用系统时间初始化种。为rand()生成不同的随机种子。
	RandomNumber = rand() % 100 + 1;//生成1~100随机数
	return RandomNumber;
}

void DateUtil::timeraddMS(struct timeval *a, long int ms){
    a->tv_usec += ms * 1000;
    if(a->tv_usec >= 1000000)    {
        a->tv_sec += a->tv_usec / 1000000;
        a->tv_usec %= 1000000;
    }
}

// 格式"20170906"
std::string DateUtil::GetTodayString(){
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
		str_today.append(char_month);
		str_today.append(char_day);

		return str_today;
}
std::string DateUtil::GetCurrentYearString(){
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

std::string DateUtil::GetCurrentMonthString(){
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
//		str_today.append(char_year);
		str_today.append(char_month);
//		str_today.append(char_day);

		return str_today;
}
// 格式"20170906"
std::string DateUtil::GetDateStringByTimet(time_t * tt ){

		tm* t= localtime(tt);
		char char_year[10];
		char char_month[10];
		char char_day[10];

		sprintf(char_year,"%04d",t->tm_year+1900);
		sprintf(char_month,"%02d",t->tm_mon+1);
		sprintf(char_day,"%02d", t->tm_mday);
//		printf("GetTodayString <<<<<<---- char year:%s  month:%s   day:%s :\n",char_year,char_month,char_day);
		std::string str_today;
		str_today.append(char_year);
		str_today.append(char_month);
		str_today.append(char_day);

		return str_today;
}


int DateUtil::ConvertCurrentDatetoNumber(){

	time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);
		char char_year[10];
		char char_month[10];
		char char_day[10];

		sprintf(char_year,"%04d",t->tm_year+1900);
		sprintf(char_month,"%02d",t->tm_mon+1);
		sprintf(char_day,"%02d", t->tm_mday);
//		printf("GetTodayString <<<<<<---- char year:%s  month:%s   day:%s :\n",char_year,char_month,char_day);


		int day = atoi(char_day);
		return day;
}

bool DateUtil::IsCurrentFridayisMonthEnd(){

	int alldays = this->getDaysByYearAndMonth();
	int today = this->ConvertCurrentDatetoNumber();
	bool ret = false;

//	printf("CurrentFridayisMonthEnd <<<<<<---- alldays:%d  today:%d  \n",alldays,today);
	if (Check_Friday() && today>alldays-7 ){
		printf("CurrentFridayisMonthEnd  最后一个月末的周五<<<<<<---- alldays:%d  today:%d  \n",alldays,today);
		ret = true;
	}
	else {
		printf("CurrentFridayisMonthEnd 非周五 <<<<<<---- alldays:%d  today:%d  \n",alldays,today);

	}

	return  ret;
}
// 格式"20170906"
std::string DateUtil::GetCurrentDayString(){

		return GetTodayString();
}


// 格式"2017-09-06 18:32:00"
std::string DateUtil::GetDateTimeString(){
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);

		char char_today[30];

		 sprintf(char_today ,"%d-%02d-%02d %02d:%02d:%02d",t->tm_year + 1900,t->tm_mon + 1,
					  t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		 std::string date = char_today;
		return date;
}

// 格式"2017-09-06 18:32:00"
std::string DateUtil::GetUTimeString(){
//		time_t tt = time(NULL);//这句返回的只是一个时间cuo
//		tm* t= localtime(&tt);
//
//		char char_today[30];
//
//		 sprintf(char_today ,"%d-%02d-%02d %02d:%02d:%02d",t->tm_year + 1900,t->tm_mon + 1,
//					  t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,t->);
//		 std::string date = char_today;

struct timeval tv;
struct timezone tz;
gettimeofday(&tv,&tz);
cout << "second : \t" << tv.tv_sec << endl; //秒
cout << "millisecond : \t" << tv.tv_sec*1000 + tv.tv_usec/1000 << endl; // 毫秒
char buf[20];
sprintf(buf,"%ld.%ld",tv.tv_sec ,tv.tv_usec);
std::string utime=buf;
return utime;
}

// 格式"18:32:00"
std::string DateUtil::GetTimeString(){
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);

		char char_today[30];

		 sprintf(char_today ,"%02d:%02d:%02d",t->tm_hour, t->tm_min, t->tm_sec);
		 std::string date = char_today;
		return date;
}
// 格式"2017-09-06 18:32:00"
std::string DateUtil::GetCurrent_Date_TimeString(){

	return GetDateTimeString();
}
// 格式"2017-09-06 18:32:00"
std::string DateUtil::GetCurrentSqlTimeString(){
	return GetDateTimeString();
}


// 格式"20170906183200"
std::string DateUtil::GetDateTimeing()
{
		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);

		char char_today[30];

		 sprintf(char_today ,"%d%02d%02d%02d%02d%02d",t->tm_year + 1900,t->tm_mon + 1,
					  t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		 std::string date = char_today;
		return date;
}
time_t DateUtil::GetCurrentTimet()
{
	return time(NULL);
}

time_t DateUtil::GetCurrentDayTimet()
{
time_t time_today;
std::string str_today = this->GetCurrentDayString();
std::string str_today_sqltime = this->ConvertDatetoSqlTime(str_today.c_str());
time_today=this->ConvertSqlTimetoTimet(str_today_sqltime.c_str());
return time_today;
}
// 2019-04-14 00:00:00
std::string DateUtil::GetCurrentDaySqlTime()
{

std::string str_today = this->GetCurrentDayString();
std::string str_today_sqltime = this->ConvertDatetoSqlTime(str_today.c_str());

return str_today_sqltime;
}


double DateUtil::GetCurrentBarTime()
{
	time_t tt = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&tt);
	double Q_BarTime_2	= (1/10e1)*t->tm_hour + (1/10e3)*t->tm_min + (1/10e5)*t->tm_sec;	//时间格式0.145100 = 14：51：00
	return Q_BarTime_2;
}

int DateUtil::GetCurrentIntBarTime()
{
	time_t tt = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&tt);
	int Q_BarTime_2	= 10000*t->tm_hour + 100*t->tm_min + t->tm_sec;	//时间格式145100 = 14：51：00
	return Q_BarTime_2;
}
//时间格式14：51：00 ---> 0.145100
double DateUtil::ConvertMinuteTimetoBarTime(const char* mtime)
{
	std::string str_time=mtime;

	int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());

	double Q_BarTime_2	= (1/10e1)*hours + (1/10e3)*minutes + (1/10e5)*seconds;	//时间格式0.145100 = 14：51：00
	return Q_BarTime_2;
}
int DateUtil::GetDiffTime(const char* time,int sec)
{
	char char_times[30];
	strcpy(char_times,time);
	std::string str_time=char_times;
	int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());
	double Q_BarTime	= (1/10e1)*hours + (1/10e3)*minutes + (1/10e5)*seconds;
	int c_time = (int)(fabs(Q_BarTime-GetCurrentSHFEBarTime(sec))*10e5);
	return c_time;
}

int DateUtil::CoverMinuteTimetoIntBarTime(const char* mtime)
{
	std::string str_time=mtime;

	int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());

	int Q_BarTime_2	= hours*10000 + minutes*100 + seconds;	//时间格式0.145100 = 14：51：00
	return Q_BarTime_2;
}
double DateUtil::GetCurrentSHFEBarTime(int difsec)
{
	time_t tt = time(NULL)+difsec;//这句返回的只是一个时间
	tm* t= localtime(&tt);

	double Q_BarTime_2	= (1/10e1)*t->tm_hour + (1/10e3)*t->tm_min + (1/10e5)*t->tm_sec ;	//时间格式0.145100 = 14：51：00
	return Q_BarTime_2;
}
std::string DateUtil::GetLastMinuteBarTime(const char * minutetime)
{
	std::string str0=minutetime;

	int hours = atoi(str0.substr(0,2).c_str());
	int minutes = atoi(str0.substr(3,2).c_str());
	int seconds = atoi(str0.substr(6,2).c_str());

	//时间字符串
	int last_minutes;
	int last_hours;
	int last_seconds=0;


	if (minutes ==0)
	{
		last_minutes =59;
		if(hours>=1)
		{
			last_hours = hours-1;
		}
		else
		{
			last_hours = 23;
		}


	}
	else
	{
		last_minutes =minutes-1;
		last_hours = hours;


	}

	char buf_last[20];
	sprintf(buf_last,"%02d:%02d:%02d",last_hours,last_minutes,last_seconds);
	std::string Q_BarTime_last=buf_last;

//	printf ("3--->>> %s  \n",Q_BarTime_last.c_str());
	return Q_BarTime_last;
}
// 返回格式"20170907082401"
std::string DateUtil::ConvertTimetoString(time_t t)
{
	std::string str_date;
	tm* local; //本地时间

	char buf[32]= {0};

	local = localtime(&t); //转为本地时间
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", local);
//	printf ("ConvertTimetoString ---> %ld     -----> %s \n",t,buf);
	str_date=buf;

	return str_date;
}

// 返回格式"2017-09-07 08:24:01"
std::string DateUtil::ConvertTimetoSqlString(time_t t)
{
	std::string str_date;
	tm* local; //本地时间

	char buf[32]= {0};

	local = localtime(&t); //转为本地时间
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
//	printf ("ConvertTimetoString ---> %ld     -----> %s \n",t,buf);
	str_date=buf;

	return str_date;
}

// 返回格式"20170907082401"
std::string DateUtil::ConvertCurrentTimetoString()
{
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t= localtime(&tt);
	char s_year[10];
	char s_month[10];
	char s_day[10];
	char s_hours[10];
	char s_minutes[10];
	char s_seconds[10];

	sprintf(s_year,"%04d",t->tm_year+1900);
	sprintf(s_month,"%02d",t->tm_mon+1);
	sprintf(s_day,"%02d", t->tm_mday);
	sprintf(s_hours,"%02d",t->tm_hour);
	sprintf(s_minutes,"%02d",t->tm_min);
	sprintf(s_seconds,"%02d", t->tm_sec);

	std::string str_date;
	str_date.append(s_year);
	str_date.append(s_month);
	str_date.append(s_day);
	str_date.append(s_hours);
	str_date.append(s_minutes);
	str_date.append(s_seconds);
	return str_date;
}


// 返回格式"2017-09-07 08:24:01"
std::string DateUtil::ConvertCurrentTimetoSqlTime()
{
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t= localtime(&tt);
	char s_year[10];
	char s_month[10];
	char s_day[10];
	char s_hours[10];
	char s_minutes[10];
	char s_seconds[10];

	sprintf(s_year,"%04d",t->tm_year+1900);
	sprintf(s_month,"%02d",t->tm_mon+1);
	sprintf(s_day,"%02d", t->tm_mday);
	sprintf(s_hours,"%02d",t->tm_hour);
	sprintf(s_minutes,"%02d",t->tm_min);
	sprintf(s_seconds,"%02d", t->tm_sec);


	std::string str_date;
	str_date.append(s_year);
	str_date.append("-");
	str_date.append(s_month);
	str_date.append("-");
	str_date.append(s_day);
	str_date.append(" ");
	str_date.append(s_hours);
	str_date.append(":");
	str_date.append(s_minutes);
	str_date.append(":");
	str_date.append(s_seconds);
	return str_date;
}
std::string DateUtil::ConvertSHFETimetoString(int difsec)
{
	time_t tt = time(NULL)+difsec;//这句返回的只是一个时间cuo
	tm* t= localtime(&tt);
	char s_year[10];
	char s_month[10];
	char s_day[10];
	char s_hours[10];
	char s_minutes[10];
	char s_seconds[10];

	sprintf(s_year,"%04d",t->tm_year+1900);
	sprintf(s_month,"%02d",t->tm_mon+1);
	sprintf(s_day,"%02d", t->tm_mday);
	sprintf(s_hours,"%02d",t->tm_hour);
	sprintf(s_minutes,"%02d",t->tm_min);
	sprintf(s_seconds,"%02d", t->tm_sec);

	std::string str_date;
	str_date.append(s_year);
	str_date.append(s_month);
	str_date.append(s_day);
	str_date.append(s_hours);
	str_date.append(s_minutes);
	str_date.append(s_seconds);
	return str_date;
}
std::string DateUtil::ConvertSHFETimetoSqlTime(int difsec)
{
	time_t tt = time(NULL)+difsec;//这句返回的只是一个时间cuo
	tm* t= localtime(&tt);
	char s_year[10];
	char s_month[10];
	char s_day[10];
	char s_hours[10];
	char s_minutes[10];
	char s_seconds[10];

	sprintf(s_year,"%04d",t->tm_year+1900);
	sprintf(s_month,"%02d",t->tm_mon+1);
	sprintf(s_day,"%02d", t->tm_mday);
	sprintf(s_hours,"%02d",t->tm_hour);
	sprintf(s_minutes,"%02d",t->tm_min);
	sprintf(s_seconds,"%02d", t->tm_sec);

	std::string str_date;
	str_date.append(s_year);
	str_date.append("-");
	str_date.append(s_month);
	str_date.append("-");
	str_date.append(s_day);
	str_date.append(" ");
	str_date.append(s_hours);
	str_date.append(":");
	str_date.append(s_minutes);
	str_date.append(":");
	str_date.append(s_seconds);
	return str_date;
}

//20180201 ----> 2018-02-01 00:00:00
std::string DateUtil::ConvertDatetoSqlTime(const char* date)
{
std::string tmp =date;
std::string sqltime;
char buf[20];
sprintf(buf,"%s-%s-%s 00:00:00",tmp.substr(0,4).c_str(),tmp.substr(4,2).c_str(),tmp.substr(6,2).c_str());
sqltime =buf;
return sqltime;
}
std::string DateUtil::ConvertSqlTimetoDate(const char * datetime)
{
	std::string sqltime=datetime;
	std::string str_date;
	char buf[20];
	sprintf(buf,"%s%s%s",sqltime.substr(0,4).c_str(),sqltime.substr(5,2).c_str(),sqltime.substr(8,2).c_str());

	str_date=buf;

//	printf("ConvertSqlTimetoDate : sqltime[%s]   date[%s]\n",datetime,str_date.c_str());
	  return str_date;
}
int DateUtil::CollectDifSecBySHFETime(const char* rsp_shfe_time)
 {
	char char_times[10];
	std::string shfetime;

	std::string str_localupdate = ConvertCurrentTimetoString();


	std::string str_time=rsp_shfe_time;
	int DifSec;
	if(str_time.length()>=8)
	{
		shfetime=GetTodayString();
		shfetime.append(str_time.substr(0,2));
		shfetime.append(str_time.substr(3,2));
		shfetime.append(str_time.substr(6,2));


		time_t shfe_t;
		time_t local_t;
		shfe_t=ConvertStringtoTimet(shfetime.c_str());
		local_t = ConvertStringtoTimet(str_localupdate.c_str());
		DifSec=shfe_t-local_t;
		printf("CollectDifSecBySHFETime---- 比对 string  交易所时间:%s  本地时间:%s :\n",shfetime.c_str(),str_localupdate.c_str());
		printf("CollectDifSecBySHFETime---- 比对 time_t  交易所:%ld   本地:%ld :\n",shfe_t,local_t);
	}
	else
	{
		DifSec=0;
	}



	return DifSec;
 }

int  DateUtil::getDaysByYearAndMonth()
{

	time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t= localtime(&tt);
		char char_year[10];
		char char_month[10];
		char char_day[10];

		sprintf(char_year,"%04d",t->tm_year+1900);
		sprintf(char_month,"%02d",t->tm_mon+1);
		sprintf(char_day,"%02d", t->tm_mday);
//		printf("GetTodayString <<<<<<---- char year:%s  month:%s   day:%s :\n",char_year,char_month,char_day);
//		std::string str_today;
//		str_today.append(char_year);
//		str_today.append(char_month);
//		str_today.append(char_day);
	int year=atoi(char_year);
	int month=atoi(char_month);

   int result;
   if (2==month)	   {
	   int a=year%4;
	   int b=year%100;
	   int c=year%400;
	   if( (a==0 && b !=0)||c==0){
			result=29;
	   }else{
			result=28;
	   }
   }
   else if (month==4||month==6||month==9||month==11)	   {
	   result=30;
   }
   else{
	   result=31;
   }
   return result;
}
bool DateUtil::Check_FridayNithtbySqlTime(const char* sqltime)
{
	time_t t_sqltime = this->ConvertSqlTimetoTimet(sqltime);//这句返回的只是一个时间
	tm* t= localtime(&t_sqltime);
	std::string tmp = sqltime;
	char night_begin[20];
	sprintf(night_begin,"%s-%s-%s 21:00:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_night_begin = this->ConvertSqlTimetoTimet(night_begin);


	char night_end[20];
	sprintf(night_end,"%s-%s-%s 23:59:59",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_night_end = this->ConvertSqlTimetoTimet(night_end);

//	printf("  %s-->%ld   %s-->%ld     %s-->%ld   \n",sqltime,t_sqltime,night_begin,t_night_begin,night_end,t_night_end);

	if (t->tm_wday ==5 && t_sqltime>t_night_begin && t_sqltime<t_night_end)	{
		//星期五
		return true;
	}
	else	{
		return false;
	}
}

bool DateUtil::Check_Friday()
{

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);


		if (t->tm_wday ==5)	{
			//星期五
			return true;
		}

		else	{
			return false;
		}
}
bool DateUtil::Check_wday()
{

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);


	if (t->tm_wday ==6){
		//星期六
		return true;
	}
	else if (t->tm_wday ==0)	{
		//星期天
		return true;
	}
	else{
		return false;
	}
}
bool DateUtil::Check_Sunday()
{

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);


	if (t->tm_wday ==6)		{
		//星期六
		return false;
	}
	else if (t->tm_wday ==0)	{
		//星期天
		return true;
	}
	else	{
		return false;
	}
}
time_t DateUtil::Collect_ExpireTradeDate(const char* expiredate)
{
	int countday =18;    // 到期日 倒推15天  作为最后可交易日
	time_t tt_lasttrade;
	std::string expire_datetime;

	std::string tmp;
	tmp = expiredate;   //xxxxxxxx   20170901


	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tmp.substr(0,4).c_str(),tmp.substr(4,2).c_str(),
			tmp.substr(6,2).c_str());
	expire_datetime=time;   //2017-09-01 00:00:00


	time_t tm_expire;

	tm_expire=ConvertSqlTimetoTimet(expire_datetime.c_str());
	tt_lasttrade = tm_expire -60*60*24*countday;
	return tt_lasttrade;
}
// "20191216  --> 20191217   20191220  --> 20191223"
string DateUtil::GetNextTradingDay()
{
	string str_tradingday;

	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(0);

	bool Timemore_night = (Q_BarTime_2>=0.200000 && Q_BarTime_2<=0.235959);
	bool Timemore_day = (Q_BarTime_2>=0.000000 && Q_BarTime_2<0.200000);
	string str_currentday= this->GetCurrentDayString();

	time_t  tt_currentday = this->GetCurrentDayTimet();
	time_t  tt_tradingday;
	tm* tm_currentday= localtime(&tt_currentday);
	if (tm_currentday->tm_wday ==5)	{

		if(Timemore_night){
			tt_tradingday=mktime(tm_currentday)+ 60*60*24*4;
			str_tradingday = this->ConvertTimetoString(tt_tradingday);
//			printf("	周五  夜盘%s   ---> 下一交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
		}
		else{
			tt_tradingday=mktime(tm_currentday)+ 60*60*24*3;
			str_tradingday = this->ConvertTimetoString(tt_tradingday);
//			printf("	周五  日盘%s   ---> 下一交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
		}
	}
	else if(tm_currentday->tm_wday ==6){
		tt_tradingday=mktime(tm_currentday)+ 60*60*24*3;
		str_tradingday = this->ConvertTimetoString(tt_tradingday);
//		printf("	周六  %s   ---> 下一交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
	}
	else if(tm_currentday->tm_wday ==0){
		tt_tradingday=mktime(tm_currentday)+ 60*60*24*2;
		str_tradingday = this->ConvertTimetoString(tt_tradingday);
//		printf("	周日  %s   ---> 下一交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
	}
	else{
		if(Timemore_night){
			tt_tradingday=mktime(tm_currentday)+ 60*60*24*2;
			str_tradingday = this->ConvertTimetoString(tt_tradingday);
//			printf("	周一-----周四  夜盘%s   ---> 下一交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
		}
		else{
			tt_tradingday=mktime(tm_currentday)+ 60*60*24;
			str_tradingday = this->ConvertTimetoString(tt_tradingday);
//			printf("	周一-----周四  日盘%s   ---> 下一交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
		}
	}

	return str_tradingday;
}
// "20191216  --> 20191217   20191220  --> 20191223"
string DateUtil::GetTradingDaybyCurrentDay()
{
	string str_tradingday;

	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(0);

	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	string str_currentday= this->GetCurrentDayString();

	time_t  tt_currentday = this->GetCurrentDayTimet();
	time_t  tt_tradingday;
	tm* tm_currentday= localtime(&tt_currentday);
	if (tm_currentday->tm_wday ==5)	{

		if(Timemore_night){
			tt_tradingday=mktime(tm_currentday)+ 60*60*24*3;
			str_tradingday = this->ConvertTimetoString(tt_tradingday);
			printf("周五  夜盘%s   ---> 交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
		}
		else{
			str_tradingday=str_currentday;
		}
	}
	else if(tm_currentday->tm_wday ==6){
		tt_tradingday=mktime(tm_currentday)+ 60*60*24*2;
		str_tradingday = this->ConvertTimetoString(tt_tradingday);
		printf("周六  %s   ---> 交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
	}
	else if(tm_currentday->tm_wday ==0){
		tt_tradingday=mktime(tm_currentday)+ 60*60*24;
		str_tradingday = this->ConvertTimetoString(tt_tradingday);
		printf("周日  %s   ---> 交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
	}
	else{
		if(Timemore_night){
			tt_tradingday=mktime(tm_currentday)+ 60*60*24;
			str_tradingday = this->ConvertTimetoString(tt_tradingday);
			printf("周一-----周四  夜盘%s   ---> 交易日:%s   \n",str_currentday.c_str(),str_tradingday.c_str());
		}
		else{
			str_tradingday=str_currentday;
		}
	}

	return str_tradingday;
}

// 返回上一个交易日的收盘后的半小时时间，即15:30 分，因为IC IF IH 15:15 收盘，15:30 作为每日固定数据更新时间
time_t DateUtil::GetLastCloseTime()
{

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);
	tm* last_tradeday_tm;
	time_t  lastclosetime_t;

	last_tradeday_tm = localtime(&current_t);
	last_tradeday_tm->tm_hour =15;
	last_tradeday_tm->tm_min =00;
	last_tradeday_tm->tm_sec =0;
	last_tradeday_tm->tm_isdst = -1;


		if (t->tm_wday ==6)	{
			//星期六
			lastclosetime_t =mktime(last_tradeday_tm)- 60*60*24;
		}
		else if (t->tm_wday ==0)	{
			//星期天
			lastclosetime_t =mktime(last_tradeday_tm)- 60*60*24*2;
		}
		else if (t->tm_wday ==1)	{
			//星期一
			if (current_t<mktime(last_tradeday_tm))	{
				lastclosetime_t =mktime(last_tradeday_tm)- 60*60*24*3;
			}
			else	{
				lastclosetime_t =mktime(last_tradeday_tm);
			}
		}
		else	{
// 下午15:30  之前  为昨日收盘时间
		if (current_t<mktime(last_tradeday_tm))			{
			lastclosetime_t =mktime(last_tradeday_tm)- 60*60*24;

		}
		else {  // 下午15:30 之后,为当日收盘时间

			lastclosetime_t =mktime(last_tradeday_tm);
		}
	}
	return lastclosetime_t;
}

time_t DateUtil::ConvertStringtoTimet(const char * datetime)
{
	 tm tm_update;
	 time_t t_update;
	  strptime(datetime, "%Y%m%d%H%M%S", &tm_update); //将字符串转换为tm时间

	  tm_update.tm_isdst = -1;
	  t_update  = mktime(&tm_update); //将tm时间转换为秒时间
//	  printf ("ConvertStringtoTimet--char :%s --- struct tm %d-%d-%d %d:%d:%d ----> time_t: %ld \n" ,datetime,tm_update.tm_year+1900,tm_update.tm_mon+1,tm_update.tm_mday,
//					  tm_update.tm_hour,tm_update.tm_min,tm_update.tm_sec,t_update);
	  return t_update;
}

time_t DateUtil::ConvertSqlTimetoTimet(const char * datetime)
{
	 tm tm_update;
	 time_t t_update;
	  strptime(datetime, "%Y-%m-%d %H:%M:%S", &tm_update); //将字符串转换为tm时间

	  tm_update.tm_isdst = -1;
	  t_update  = mktime(&tm_update); //将tm时间转换为秒时间
//	  printf ("ConvertStringtoTimet--char :%s --- struct tm %d-%d-%d %d:%d:%d ----> time_t: %ld \n" ,datetime,tm_update.tm_year+1900,tm_update.tm_mon+1,tm_update.tm_mday,
//					  tm_update.tm_hour,tm_update.tm_min,tm_update.tm_sec,t_update);
	  return t_update;
}

time_t DateUtil::ConvertSqlTimetoCloseTimet(const char * datetime)
{
	 tm tm_update;
	 time_t t_update;
	  strptime(datetime, "%Y-%m-%d %H:%M:%S", &tm_update); //将字符串转换为tm时间

	  tm_update.tm_hour =15;
	  tm_update.tm_min =30;
	  tm_update.tm_sec =0;
	  tm_update.tm_isdst = -1;
	  t_update  = mktime(&tm_update); //将tm时间转换为秒时间
//	  printf ("ConvertStringtoTimet--char :%s --- struct tm %d-%d-%d %d:%d:%d ----> time_t: %ld \n" ,datetime,tm_update.tm_year+1900,tm_update.tm_mon+1,tm_update.tm_mday,
//					  tm_update.tm_hour,tm_update.tm_min,tm_update.tm_sec,t_update);
	  return t_update;
}
int DateUtil::SetSystemTime(char *dt)
{
    char t_string[] = "2012-04-28 22:30:00";
    struct tm time_tm;
    struct timeval time_tv;
    time_t timep;
    int ret = 0;

    sscanf(t_string, "%d-%d-%d %d:%d:%d", &time_tm.tm_year, &time_tm.tm_mon, &time_tm.tm_mday, &time_tm.tm_hour, &time_tm.tm_min, &time_tm.tm_sec);
    time_tm.tm_year -= 1900;
    time_tm.tm_mon -= 1;
    time_tm.tm_wday = 0;
    time_tm.tm_yday = 0;
    time_tm.tm_isdst = 0;

    timep = mktime(&time_tm);
//    time_tv.tv_sec = timep;
//    time_tv.tv_usec = 0;
//
//    ret = settimeofday(&time_tv, NULL);
//    if(ret != 0)
//    {
//        fprintf(stderr, "settimeofday failed\n");
//        return -1;
//    }
    return 0;
}
bool DateUtil::CheckOpenMarketTime()
{
	bool ret;

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);
	double Q_BarTime_2 = this->GetCurrentBarTime();


	bool Timemore_morning1 = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1016);
	bool Timemore_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1501);
	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning1 || Timemore_morning2 || Timemore_afternoon || Timemore_night || Timemore_night1);


		if (t->tm_wday ==6)	{
			//星期六
			if(Timemore_night1)	{
				ret=  true;
			}
			else	{
				ret= false;
			}
		}
		else if (t->tm_wday ==0)	{
			//星期天
			ret=  false;
		}
		else	{
			if(Timemore )	{
					ret =true;
			}
			else{
				ret=false;
			}
		}
	return ret;
}

bool DateUtil::CheckMDOpenTime()
{
	bool ret;

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);
	double Q_BarTime_2 = this->GetCurrentBarTime();


	bool Timemore_morning1 = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1016);
	bool Timemore_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1501);
	bool Timemore_night = (Q_BarTime_2>=0.20490000 && Q_BarTime_2<=0.240000);   //夜盘行情订阅时间多扩展一些
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning1 || Timemore_morning2 || Timemore_afternoon || Timemore_night || Timemore_night1);
	bool Timemore_Monday=( Timemore_morning1 || Timemore_morning2 || Timemore_afternoon || Timemore_night );
	bool Timemore_Saturday= Timemore_night1 ;
		if (t->tm_wday ==6)	{
			//星期六
			if(Timemore_Saturday)	{
				ret=  true;
			}
			else			{
				ret= false;
			}

		}
		else if (t->tm_wday ==0)	{
			//星期天
			ret=  false;

		}
		else if (t->tm_wday ==1)	{
			//星期一
			if(Timemore_Monday)	{
				ret=  true;
			}
			else			{
				ret= false;
			}
		}
		else	{
			if(Timemore )	{
				ret =true;
			}
			else{
				ret=false;
			}

		}
	return ret;

}

time_t DateUtil::CheckHisDayLine(const char *tradingday,int difsec)
{
	time_t current_t = time(NULL);//这句返回的只是一个时间
		tm* t= localtime(&current_t);

		// 交易日
	std::string tradingdate;
		tradingdate = tradingday;   //xxxxxxxx   20170901

	std::string tradingdatetime;
		char time[20];
		sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
				tradingdate.substr(6,2).c_str());
		tradingdatetime=time;   //2017-09-01 00:00:00

		time_t tm_yesterday;

		time_t tm_tradingday;

		tm_tradingday=ConvertSqlTimetoTimet(tradingdatetime.c_str());

		double Q_BarTime_2 = GetCurrentSHFEBarTime(difsec);


		bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
		bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
		bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
		bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
		bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);
		time_t tm_dayline;

		if (t->tm_wday ==5 && Timemore_night)   // 周五夜盘
		{
			tm_dayline =tm_tradingday -60*60*24*3;
		}
		else if (t->tm_wday ==6 && Timemore_night1)  // 周六凌晨- 夜盘
		{
			//星期六
			tm_dayline =tm_tradingday -60*60*24*3;
		}
		else if (t->tm_wday ==0)
		{
			//星期天
			tm_dayline =tm_tradingday -60*60*24*3;
		}
		else if(t->tm_wday ==1 && ( Timemore_morning  || Timemore_afternoon) )   // 周一白天
		{

			tm_dayline =tm_tradingday -60*60*24*3;
		}
		else
		{
			tm_dayline =tm_tradingday -60*60*24;
		}


	return  tm_dayline;

}

time_t DateUtil::CheckHisLastDayLinebyDate_ExcludeHoliday(const char *tradingday,
		int DifSec,const char* path,const char* filename)
{
	vector<std::string>  list;
	this->ReadHoliday_CSV(list,path,filename);

	time_t tm_enddayline;
	tm_enddayline =  this->CheckHisLastDayLine(list,tradingday,DifSec);

	vector<std::string>().swap(list);
	return tm_enddayline;
}


time_t DateUtil::CheckHisLastDayLine(vector<std::string> &temp,const char *tradingday,int difsec)
{
	time_t current_t = time(NULL);//这句返回的只是一个时间
		tm* t= localtime(&current_t);

		// 交易日
	std::string tradingdate;
		tradingdate = tradingday;   //xxxxxxxx   20170901

	std::string tradingdatetime;
		char time[20];
		sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
				tradingdate.substr(6,2).c_str());
		tradingdatetime=time;   //2017-09-01 00:00:00

		time_t tm_yesterday;

		time_t tm_tradingday;
		time_t tm_today;

		tm_tradingday=ConvertSqlTimetoTimet(tradingdatetime.c_str());

		std::string currentday_sqltime= this->ConvertDatetoSqlTime(this->GetCurrentDayString().c_str());
		tm_today=this->ConvertSqlTimetoTimet(currentday_sqltime.c_str());
		double Q_BarTime_2 = GetCurrentSHFEBarTime(difsec);


		bool Timemore_morning = (Q_BarTime_2>=0.0600 && Q_BarTime_2<0.1131);
		bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
		bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
		bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
		bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);
		time_t tm_dayline;

		if (t->tm_wday ==5 && Timemore_night)  { // 周五夜盘

			tm_dayline =tm_tradingday -60*60*24*3;
//			printf("DateUtil::CheckHisLastDayLine   -----星期五夜盘  \n");
		}
		else if (t->tm_wday ==6 )		{
			//星期六
			if(Timemore_night1) {  // 周六凌晨- 夜盘
				tm_dayline =tm_tradingday -60*60*24*3;
			}
			else {
				tm_dayline =tm_tradingday -60*60*24*3;
			}

//			printf("DateUtil::CheckHisLastDayLine   -----星期六 凌晨夜盘  \n");
		}
		else if (t->tm_wday ==0)		{
			//星期天
			tm_dayline =tm_tradingday -60*60*24*3;
//			printf("DateUtil::CheckHisLastDayLine   -----星期天  \n");
		}
		else if(t->tm_wday ==1 ){   // 周一白天

			if(Q_BarTime_2>=0.0000 && Q_BarTime_2<0.1500){
				tm_dayline =tm_tradingday -60*60*24*3;
			}
			else{
				tm_dayline =tm_today;
			}
//			printf("DateUtil::CheckHisLastDayLine   -----星期一  \n");

		}
		else if(t->tm_wday >=2 &&  t->tm_wday <=5 )		{
//			printf("DateUtil::CheckHisLastDayLine   -----星期二---星期五  \n");
			tm_dayline =tm_tradingday -60*60*24;

			if(Q_BarTime_2>=0.0000 && Q_BarTime_2<0.1500){
				tm_dayline =tm_tradingday -60*60*24;
			}
			else{
				tm_dayline =tm_today;
			}
		}
		else		{
			tm_dayline =tm_tradingday -60*60*24;
		}

//		ConvertTimetoSqlString
		reverse(temp.begin(),temp.end());

		for (auto &day:temp)		{
			std::string str_find =ConvertTimetoSqlString(tm_dayline);
			if(strcmp(day.c_str(),str_find.c_str())==0)		{
				tm_dayline =tm_dayline -60*60*24;
				//printf("CheckHisLastDayLine  : finded ------>  %s    \n",str_find.c_str());
			}


		}
	return  tm_dayline;

}

bool DateUtil::CheckHoliday_SqlTime(const char *tradingday_sqltime)
{
	bool ret=false;

	vector<std::string>  list;
	ReadHoliday_CSV(list,m_path.c_str(),m_file.c_str());

	for (auto &day:list)	{

		if(strcmp(day.c_str(),tradingday_sqltime)==0)	{
			ret=true;
			printf("	CheckHoliday_SqlTime  : 假期列表找到对应假日 ------>  %s     %s \n",day.c_str(),tradingday_sqltime);
		}
	}

	vector<std::string>().swap(list);
	return  ret;

}
bool DateUtil::CheckSHFEOpenMarketTime(int difsec)
{
	bool ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);


	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1501);
	bool Timemore_night = (Q_BarTime_2>=0.205500 && Q_BarTime_2<=0.235959);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	if (t->tm_wday ==6)		{
			//星期六
			if(Timemore_night1)	{
				ret=  true;
			}
			else{
				ret= false;
			}

		}
		else if (t->tm_wday ==0){
			//星期天
			ret=  false;
		}
		else		{
			if(Timemore )	{
					ret =true;
				}
				else	{
					ret=false;
				}
		}
	return ret;

}
bool DateUtil::CheckSHFEEndCloseMarketTime(int difsec)
{
	bool ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);

	bool Timemore_afterend = (Q_BarTime_2>=0.1455 && Q_BarTime_2<0.1500);
	if (t->tm_wday ==6)		{
		ret= false;
	}
	else if (t->tm_wday ==0)	{
		//星期天
		ret=  false;

	}
	else		{
		if(Timemore_afterend )		{
				ret =true;
			}
			else	{
				ret=false;
			}

	}
	return ret;

}
bool DateUtil::isFridayNight(int difsec)
{
	bool ret=false;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);

	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);

	if (t->tm_wday ==6)	{
		//星期六
		if(Timemore_night1)	{
			ret=  true;
		}
		else	{
			ret= false;
		}
	}
	else if (t->tm_wday ==5){
		//星期天
		if( Timemore_night )	{
			ret =true;
		}
		else{
			ret=false;
		}
	}

	return ret;

}

bool DateUtil::isNightMorningBreakTime(int difsec)
{
	bool ret=false;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);


	bool Open_morning = (Q_BarTime_2>=0.090500 && Q_BarTime_2<0.093200);
//	bool Open_morning1 = (Q_BarTime_2>=0.0901 && Q_BarTime_2<0.1015);
//	bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
//	bool Open_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1455);


	bool Open_night = (Q_BarTime_2>=0.210500 && Q_BarTime_2<=0.235959);
	bool Open_night1 = (Q_BarTime_2>=0.000000 && Q_BarTime_2<0.023000);

	if (t->tm_wday ==6)	{
		//星期六

		ret= false;

	}
	else if (t->tm_wday ==0){
			//星期天

		ret=false;

	}
	else if (t->tm_wday ==1){
		//
		if( Open_morning ||Open_night )	{
			ret =true;
		}
		else{
			ret=false;
		}
	}

	else if (t->tm_wday ==2  || t->tm_wday ==3 || t->tm_wday ==4 ){

		if(  Open_night1 || Open_morning ||Open_night)	{
			ret =true;
		}
		else{
			ret=false;
		}
	}
	else if (t->tm_wday ==5){
		//
		if( Open_morning )	{
			ret =true;
		}
		else{
			ret=false;
		}
	}
	return ret;

}
bool DateUtil::CheckSHFENightTime(int difsec)
{
	bool ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);


	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	bool Night= Timemore_night || Timemore_night1;

	if (t->tm_wday ==6)	{
		//星期六
		if(Timemore_night1)	{
			ret=  true;
		}
		else	{
			ret= false;
		}
	}
	else if (t->tm_wday ==0){
		//星期天
		ret=  false;
	}
	else if (t->tm_wday ==1){
		//星期天
		if( Timemore_night )	{
			ret =true;
		}
		else{
			ret=false;
		}
	}
	else{
		if( Night )	{
			ret =true;
		}
		else{
			ret=false;
		}

	}
	return ret;

}

bool DateUtil::CheckSHFEDayTime(int difsec)
{
	bool ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);


	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	bool Day=  Timemore_morning  || Timemore_afternoon ;

	if (t->tm_wday ==6)	{

		ret= false;
	}
	else if (t->tm_wday ==0){
		//星期天
		ret=  false;
	}
	else	{
		if( Day )	{
			ret =true;
		}
		else	{
			ret=false;
		}

	}
	return ret;

}

//早盘开盘前10分钟 夜盘开盘前15分钟  包含
bool DateUtil::CheckSHFELoginMarketTime(int difsec)
{
	bool ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);


	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
	bool Timemore_night = (Q_BarTime_2>=0.20450000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	if (t->tm_wday ==6)
		{
			//星期六
			if(Timemore_night1)
			{
				ret=  true;
			}
			else
			{
				ret= false;
			}



		}
		else if (t->tm_wday ==0)
		{
			//星期天

			ret=  false;

		}
		else
		{
			if(Timemore )
				{
					ret =true;
				}
				else
				{
					ret=false;
				}

		}
	return ret;

}

//早盘开盘前10分钟 夜盘开盘前15分钟  包含
int DateUtil::CheckWaitSecondsbySHFELoginMarketTime(const char* tradingday)
{
	int ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentBarTime();


	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
	bool Timemore_night = (Q_BarTime_2>=0.20450000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	if (t->tm_wday ==6)		{
		//星期六
		if(Timemore_night1)			{
			ret=  true;
		}
		else			{
			ret= false;
		}
	}
	else if (t->tm_wday ==0)	{
		//星期天

		std::string opentime =tradingday;
		opentime.append("090000");
		time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
		time_t t_current = this->GetCurrentTimet();

		printf("CheckWaitSecondsbySHFELoginMarketTime   %ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
		return t_opentime-t_current;
	}
	else	{
		if(Q_BarTime_2<0.09000  )	{
			//早盘开盘前
			std::string opentime =tradingday;
			opentime.append("090000");
			time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
			time_t t_current = this->GetCurrentTimet();

			printf("CheckWaitSecondsbySHFELoginMarketTime   早盘开盘前:%ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
			return t_opentime-t_current;

		}
		else if(Q_BarTime_2>0.1130 && Q_BarTime_2<0.1330  )	{
			//下午盘开盘前
			std::string opentime =tradingday;
			opentime.append("133000");
			time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
			time_t t_current = this->GetCurrentTimet();

			printf("CheckWaitSecondsbySHFELoginMarketTime   下午盘开盘前:%ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
			return t_opentime-t_current;

		}
		else if(Q_BarTime_2>0.153000 && Q_BarTime_2<0.2100  )	{
			//夜盘开盘前
			std::string opentime =tradingday;
			opentime.append("210000");
			time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
			time_t t_current = this->GetCurrentTimet();

			printf("CheckWaitSecondsbySHFELoginMarketTime   夜盘开盘前:%ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
			return t_opentime-t_current;

		}
		else		{

		}

	}
	return ret;

}

//早盘开盘前10分钟 夜盘开盘前15分钟  包含
int DateUtil::CheckWaitSecondsbyOpenTime()
{
	int ret;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentBarTime();


	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1131);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);
	bool Timemore_night = (Q_BarTime_2>=0.20450000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	if (t->tm_wday ==6)		{
		//星期六
		if(Timemore_night1)			{
			ret=  0;
		}
		else			{
			ret= 3600;
		}
	}
	else if (t->tm_wday ==0)	{	//星期天


		return 3600;
	}
	else	{
		if(Q_BarTime_2<0.085000  && Q_BarTime_2>0.023500)	{
			//早盘开盘前
			std::string opentime =this->GetCurrentDayString();
			opentime.append("085000");
			time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
			time_t t_current = this->GetCurrentTimet();

			printf("CheckWaitSecondsbySHFELoginMarketTime   早盘开盘前:%ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
			return t_opentime-t_current;

		}
		else if(Q_BarTime_2>0.113000 && Q_BarTime_2<0.132000  )	{
			//下午盘开盘前
			std::string opentime =this->GetCurrentDayString();
			opentime.append("132000");
			time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
			time_t t_current = this->GetCurrentTimet();

			printf("CheckWaitSecondsbySHFELoginMarketTime   下午盘开盘前:%ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
			return t_opentime-t_current;

		}
		else if(Q_BarTime_2>0.153000 && Q_BarTime_2<0.205000  )	{
			//夜盘开盘前
			std::string opentime =this->GetCurrentDayString();
			opentime.append("205000");
			time_t t_opentime =this->ConvertStringtoTimet (opentime.c_str());
			time_t t_current = this->GetCurrentTimet();

			printf("CheckWaitSecondsbySHFELoginMarketTime   夜盘开盘前:%ld  - %ld   = %ld \n",t_opentime,t_current,t_opentime-t_current);
			return t_opentime-t_current;

		}
		else		{
			ret=0;
		}

	}
	return ret;

}
bool DateUtil::CheckOpenTime(int difsec,const char *exchangeid,const char* productid)
{
	bool ret;
	time_t tt = time(NULL)+difsec;//这句返回的只是一个时间
	tm* t= localtime(&tt);

	double Q_BarTime_2	= (1/10e1)*t->tm_hour + (1/10e3)*t->tm_min + (1/10e5)*t->tm_sec ;


	bool flag_wday = this->Check_wday();

	bool Timemore_morning = (Q_BarTime_2>=0.0900 && Q_BarTime_2<0.1015);
	bool Timemore_morning1 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1500);
	bool Timemore_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);

	bool au_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool au_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);

	bool cu_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool cu_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0100);


	bool ru_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.2300);
	bool dce_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.2330);
	bool czce_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.2330);

	bool night=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	bool cffex_tf_morning = (Q_BarTime_2>=0.0915 && Q_BarTime_2<0.1130);
	bool cffex_tf_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);

	bool cffex_if_morning = (Q_BarTime_2>=0.0930 && Q_BarTime_2<0.1130);
	bool cffex_if_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1500);

	if(strcmp(exchangeid,"SHFE")==0)	{

		if(strcmp(productid,"ru")==0 || strcmp(productid,"rb")==0  ||
				strcmp(productid,"hc")==0 || strcmp(productid,"bu")==0)	{

			if (t->tm_wday ==6|| t->tm_wday ==0)	{
				ret= false;
			}
			else		{
				if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon ||ru_night)		{
					ret=  true;
				}
				else	{
					ret= false;
				}
			}

		}
		else if(strcmp(productid,"au")==0 || strcmp(productid,"ag")==0)	{

			if (t->tm_wday ==6)		{
				//星期六
				if(Timemore_night1)	{
					ret=  true;
				}
				else{
					ret= false;
				}
			}
			else if (t->tm_wday ==0)	{
				//星期天

				ret=  false;

			}
			else if (t->tm_wday ==1){
				//星期一

				if( Timemore_morning  || Timemore_morning1 || Timemore_afternoon || au_night)	{
					ret=  true;
				}
				else{
					ret= false;
				}
			}
			else	{
				if( Timemore_morning  ||Timemore_morning1 || Timemore_afternoon || au_night || au_night1){
					ret =true;
				}
				else		{
					ret=false;
				}
			}
		}
		else	{

			if (t->tm_wday ==6)	{
				//星期六
				if(Timemore_night1)	{
					ret=  true;
				}
				else	{
					ret= false;
				}
			}
			else if (t->tm_wday ==0){
				//星期天
				ret=  false;
			}
			else if (t->tm_wday ==1)	{
				//星期一

				if( Timemore_morning  ||Timemore_morning1 || Timemore_afternoon || cu_night)	{
					ret=  true;
				}
				else{
					ret= false;
				}


			}
			else{
				if( Timemore_morning ||Timemore_morning1 || Timemore_afternoon || cu_night || cu_night1)		{
					ret =true;
				}
				else		{
					ret=false;
				}

			}
		}

	}
	else if(strcmp(exchangeid,"DCE")==0){
		if( strcmp(productid,"jd")==0	|| strcmp(productid,"c")==0|| strcmp(productid,"cs")==0 )	{

					if (t->tm_wday ==6|| t->tm_wday ==0)	{
						ret= false;
					}
					else		{
						if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon)	{
							ret=  true;
//							printf("CheckOpenTime  -----111111 \n");
						}
						else	{
							ret= false;
//							printf("CheckOpenTime  -----22222222 \n");
						}
					}

				}
				else{
					if (t->tm_wday ==6|| t->tm_wday ==0)		{
						ret= false;
					}
					else	{
						if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon ||dce_night)		{
							ret=  true;
						}
						else	{
							ret= false;
						}
					}
				}
	}
	else if(strcmp(exchangeid,"CZCE")==0)	{
		if(strcmp(productid,"SM")==0 || strcmp(productid,"SF")==0|| strcmp(productid,"AP")==0)	{

			if (t->tm_wday ==6|| t->tm_wday ==0)	{
				ret= false;
			}
			else	{
				if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon)		{
					ret=  true;
				}
				else	{
					ret= false;
				}
			}

		}
		else{

			if (t->tm_wday ==6|| t->tm_wday ==0)	{
				ret= false;
			}
			else	{
				if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon ||czce_night)		{
					ret=  true;
				}
				else{
					ret= false;
				}
			}
		}
	}
	else if(strcmp(exchangeid,"CFFEX")==0){
		if(strcmp(productid,"IC")==0 || strcmp(productid,"IF")==0 || strcmp(productid,"IH")==0)	{
			if (t->tm_wday ==6|| t->tm_wday ==0)	{
				ret= false;
			}
			else	{
				if(cffex_if_morning || cffex_if_afternoon)		{
					ret=  true;
				}
				else	{
					ret= false;
				}
			}
		}
		else	{

			if (t->tm_wday ==6|| t->tm_wday ==0)	{
				ret= false;
			}
			else	{
				if(cffex_tf_morning || cffex_tf_afternoon)	{
					ret=  true;
				}
				else	{
					ret= false;
				}
			}
		}
	}
	else	{
		printf(">>>  DateUtil::CheckOpenTime 交易所[%s]不存在,请确认输入的EXCHANGEID \n",exchangeid);
		ret=false;
	}

	return ret;

}

bool DateUtil::CheckMDOpenTimebyInstrumentID(double Q_BarTime_2,const char *exchangeid,const char* productid)
{
	bool ret;
	time_t tt = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&tt);
//
//	double Q_BarTime_2	= (1/10e1)*t->tm_hour + (1/10e3)*t->tm_min + (1/10e5)*t->tm_sec ;


	bool flag_wday = this->Check_wday();

	bool Timemore_morning = (Q_BarTime_2>=0.0900 && Q_BarTime_2<=0.1015);
	bool Timemore_morning1 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<=0.1130);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<=0.1500);
	bool Timemore_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<=0.0235);

	bool au_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool au_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);

	bool cu_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool cu_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0100);


	bool ru_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.2300);
	bool dce_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.2330);
	bool czce_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.2330);

	bool night=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	bool cffex_tf_morning = (Q_BarTime_2>=0.0915 && Q_BarTime_2<0.1130);
	bool cffex_tf_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1516);

	bool cffex_if_morning = (Q_BarTime_2>=0.0930 && Q_BarTime_2<0.1130);
	bool cffex_if_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1500);

	if(strcmp(exchangeid,"SHFE")==0)
	{

		if(strcmp(productid,"ru")==0 )
		{

			if (t->tm_wday ==6|| t->tm_wday ==0)
			{
				ret= false;
			}
			else
			{
				if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon ||ru_night)
				{
					ret=  true;
				}
				else
				{
					ret= false;
				}
			}

		}
		else if(strcmp(productid,"au")==0 || strcmp(productid,"ag")==0)
		{

			if (t->tm_wday ==6)
					{
						//星期六
						if(Timemore_night1)
						{
							ret=  true;
						}
						else
						{
							ret= false;
						}



					}
					else if (t->tm_wday ==0)
					{
						//星期天

						ret=  false;

					}
					else if (t->tm_wday ==1)
					{
						//星期一

						if( Timemore_morning  || Timemore_morning1 || Timemore_afternoon || au_night)
						{
							ret=  true;
						}
						else
						{
							ret= false;
						}


					}
					else
					{
						if( Timemore_morning  ||Timemore_morning1 || Timemore_afternoon || au_night || au_night1)
							{
								ret =true;
							}
							else
							{
								ret=false;
							}

					}

		}
		else
		{

			if (t->tm_wday ==6)
			{
				//星期六
				if(Timemore_night1)
				{
					ret=  true;
				}
				else
				{
					ret= false;
				}



			}
			else if (t->tm_wday ==0)
			{
				//星期天

				ret=  false;

			}
			else if (t->tm_wday ==1)
			{
				//星期一

				if( Timemore_morning  ||Timemore_morning1 || Timemore_afternoon || cu_night)
				{
					ret=  true;
				}
				else
				{
					ret= false;
				}


			}
			else
			{
				if( Timemore_morning ||Timemore_morning1 || Timemore_afternoon || cu_night || cu_night1)
					{
						ret =true;
					}
					else
					{
						ret=false;
					}

			}
		}

	}
	else if(strcmp(exchangeid,"DCE")==0)
	{
		if(strcmp(productid,"pp")==0 || strcmp(productid,"l")==0|| strcmp(productid,"jd")==0
				|| strcmp(productid,"c")==0|| strcmp(productid,"cs")==0 || strcmp(productid,"v")==0)
				{

					if (t->tm_wday ==6|| t->tm_wday ==0)
					{
						ret= false;
					}
					else
					{
						if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon)
						{
							ret=  true;
//							printf("CheckOpenTime  -----111111 \n");
						}
						else
						{
							ret= false;
//							printf("CheckOpenTime  -----22222222 \n");
						}
					}

				}
				else
				{
					if (t->tm_wday ==6|| t->tm_wday ==0)
					{
						ret= false;
					}
					else
					{
						if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon ||dce_night)
						{
							ret=  true;
						}
						else
						{
							ret= false;
						}
					}

				}


	}
	else if(strcmp(exchangeid,"CZCE")==0)
	{
		if(strcmp(productid,"SM")==0 || strcmp(productid,"SF")==0)
		{

			if (t->tm_wday ==6|| t->tm_wday ==0)
			{
				ret= false;
			}
			else
			{
				if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon)
				{
					ret=  true;
				}
				else
				{
					ret= false;
				}
			}

		}
		else
		{

			if (t->tm_wday ==6|| t->tm_wday ==0)
				{
					ret= false;
				}
				else
				{
					if(Timemore_morning || Timemore_morning1 ||Timemore_afternoon ||czce_night)
					{
						ret=  true;
					}
					else
					{
						ret= false;
					}
				}

		}
	}
	else if(strcmp(exchangeid,"CFFEX")==0)
	{
		if(strcmp(productid,"IC")==0 || strcmp(productid,"IF")==0 || strcmp(productid,"IH")==0)
		{
			if (t->tm_wday ==6|| t->tm_wday ==0)
					{
						ret= false;
					}
					else
					{
						if(cffex_if_morning || cffex_if_afternoon)
						{
							ret=  true;
						}
						else
						{
							ret= false;
						}
					}



		}
		else
		{

			if (t->tm_wday ==6|| t->tm_wday ==0)
			{
				ret= false;
			}
			else
			{
				if(cffex_tf_morning || cffex_tf_afternoon)
				{
					ret=  true;
				}
				else
				{
					ret= false;
				}
			}


		}

	}
	else
	{
		printf("交易所[%s]不存在,请确认输入的EXCHANGEID \n",exchangeid);
		ret=false;
	}




	return ret;

}
//早盘休市 时间,即金融板块开盘时间  是包含关系
bool DateUtil::CheckCloseAll(int difsec)
{
	bool ret;
	time_t tt = time(NULL)+difsec;//这句返回的只是一个时间
	tm* t= localtime(&tt);

	double Q_BarTime_2	= (1/10e1)*t->tm_hour + (1/10e3)*t->tm_min + (1/10e5)*t->tm_sec ;


	bool flag_wday = this->Check_wday();

	bool Timemore_morning = (Q_BarTime_2>=0.0900 && Q_BarTime_2<0.1130);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1515);


	bool au_night = (Q_BarTime_2>=0.2100 && Q_BarTime_2<=0.240000);
	bool au_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);

	if (t->tm_wday ==6)
	{
		//星期六
		if(au_night1)
		{
			ret=  true;
		}
		else
		{
			ret= false;
		}



	}
	else if (t->tm_wday ==0)
	{
		//星期天

		ret=  false;

	}
	else if (t->tm_wday ==1)
	{
		//星期一

		if( Timemore_morning  || Timemore_afternoon || au_night)
		{
			ret=  true;
		}
		else
		{
			ret= false;
		}


	}
	else
	{
		if( Timemore_morning  || Timemore_afternoon || au_night || au_night1)
			{
				ret =true;
			}
			else
			{
				ret=false;
			}

	}
	return ret;
}
bool DateUtil::CheckOrderInsertTime()
{
	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentBarTime();


	//早盘开盘后5分钟内不开仓
	bool Open_morning1 = (Q_BarTime_2>=0.0905 && Q_BarTime_2<0.1016);
	bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
	bool Open_afternoon = (Q_BarTime_2>=0.1305 && Q_BarTime_2<0.1501);
	//夜盘五分钟内不开仓
	bool Open_night = (Q_BarTime_2>=0.21050000 && Q_BarTime_2<=0.240000);
	bool Open_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool OpenTime=( Open_morning1 || Open_morning2 || Open_afternoon || Open_night || Open_night1);

	if(OpenTime && !flag_wday )
	{
		ret =true;
	}
	else
	{
		ret=false;
	}
	return ret;

}
bool DateUtil::CheckSHFEOrderInsertTime(int difsec)
{
	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);

	//早盘开盘后5分钟内不开仓
	bool Open_morning1 = (Q_BarTime_2>=0.0905 && Q_BarTime_2<0.1010);
	bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
	//下午盘开盘后5分钟内不开仓
	bool Open_afternoon = (Q_BarTime_2>=0.1335 && Q_BarTime_2<0.1500);
	//夜盘五分钟内不开仓
	bool Open_night = (Q_BarTime_2>=0.21050000 && Q_BarTime_2<=0.240000);
	bool Open_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);
	bool OpenTime=( Open_morning1 || Open_morning2 || Open_afternoon || Open_night || Open_night1);

	if(OpenTime && !flag_wday )	{
		ret =true;
	}
	else	{
		ret=false;
	}
	return ret;

}

bool DateUtil::CheckDayTradeSHFEOrderInsertTime(int difsec)
{
	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);

	bool Open_morning1 = (Q_BarTime_2>=0.0901 && Q_BarTime_2<0.1015);
	bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
	bool Open_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1455);


	bool Open_night = (Q_BarTime_2>=0.21010000 && Q_BarTime_2<=0.240000);
	bool Open_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
	bool OpenTime=( Open_morning1 || Open_morning2 || Open_afternoon || Open_night || Open_night1);

	if(OpenTime && !flag_wday )
	{
		ret =true;
	}
	else
	{
		ret=false;
	}
	return ret;

}

bool DateUtil::CheckInsOrderInsertTime(const char *pinstrumentid)
{
	char ProductID[31];
	std::string pattern("(^[a-zA-Z]*[^0-9]+)"); // ([a-z]+):([0-9]+) also works here

	boost::regex reg(pattern);    //查找字符串里的字母
	boost::cmatch mat;
	if(boost::regex_search( pinstrumentid, mat, reg))
	{
//			cout << "searched:" << mat[0] << endl;
//			printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", pinstrumentID,mat[0].str().c_str());
	}
	strcpy(ProductID,mat[0].str().c_str());

	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentBarTime();

	if(strcmp(ProductID,"IF")==0 || strcmp(ProductID,"IH")==0 || strcmp(ProductID,"IC")==0 || strcmp(ProductID,"TF")==0  ||strcmp(ProductID,"T")==0)
	{
		//早盘开盘后5分钟内不开仓
			bool Open_morning = (Q_BarTime_2>=0.0920 && Q_BarTime_2<0.1131);

			bool Open_afternoon = (Q_BarTime_2>=0.1305 && Q_BarTime_2<0.1501);

			bool OpenTime=( Open_morning  || Open_afternoon );

			if(OpenTime && !flag_wday )
			{
				ret =true;
			}
			else
			{
				ret=false;
			}
	}
	else
	{
		//早盘开盘后5分钟内不开仓
			bool Open_morning1 = (Q_BarTime_2>=0.0905 && Q_BarTime_2<0.1016);
			bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
			bool Open_afternoon = (Q_BarTime_2>=0.1335 && Q_BarTime_2<0.1501);
			//夜盘五分钟内不开仓
			bool Open_night = (Q_BarTime_2>=0.21050000 && Q_BarTime_2<=0.240000);
			bool Open_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
			bool OpenTime=( Open_morning1 || Open_morning2 || Open_afternoon || Open_night || Open_night1);

			if(OpenTime && !flag_wday )
			{
				ret =true;
			}
			else
			{
				ret=false;
			}

	}

	return ret;
}

bool DateUtil::CheckSHFEInsOrderInsertTime(const char *pinstrumentid,int difsec)
{
	char ProductID[31];
	std::string pattern("(^[a-zA-Z]*[^0-9]+)"); // ([a-z]+):([0-9]+) also works here

	boost::regex reg(pattern);    //查找字符串里的字母
	boost::cmatch mat;
	if(boost::regex_search( pinstrumentid, mat, reg))
	{
//			cout << "searched:" << mat[0] << endl;
//			printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", pinstrumentID,mat[0].str().c_str());
	}
	strcpy(ProductID,mat[0].str().c_str());

	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);
//	printf("----->  %s CheckSHFEInsOrderInsertTime--mat-[]--->%f \n", pinstrumentid,Q_BarTime_2);

	if(strcmp(ProductID,"IF")==0 || strcmp(ProductID,"IH")==0 || strcmp(ProductID,"IC")==0 || strcmp(ProductID,"TF")==0  ||strcmp(ProductID,"T")==0)
	{
		//早盘开盘后5分钟内不开仓
			bool Open_morning = (Q_BarTime_2>=0.0920 && Q_BarTime_2<0.1131);

			bool Open_afternoon = (Q_BarTime_2>=0.1305 && Q_BarTime_2<0.1501);

			bool OpenTime=( Open_morning  || Open_afternoon );

			if(OpenTime && !flag_wday )			{
				ret =true;
			}
			else			{
				ret=false;
			}
	}
	else	{
		//早盘开盘后5分钟内不开仓
			bool Open_morning1 = (Q_BarTime_2>=0.0905 && Q_BarTime_2<0.1016);
			bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
			bool Open_afternoon = (Q_BarTime_2>=0.1335 && Q_BarTime_2<0.1501);
			//夜盘五分钟内不开仓
			bool Open_night = (Q_BarTime_2>=0.21050000 && Q_BarTime_2<=0.240000);
			bool Open_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
			bool OpenTime=( Open_morning1 || Open_morning2 || Open_afternoon || Open_night || Open_night1);

			if(OpenTime && !flag_wday )			{
				ret =true;
			}
			else			{
				ret=false;
			}

	}

	return ret;
}
bool DateUtil::CheckDayTradeSHFEInsOrderInsertTime(const char *pinstrumentid,int difsec)
{
	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	bool ret=false;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);
//	printf("----->  %s CheckSHFEInsOrderInsertTime--mat-[]--->%f \n", pinstrumentid,Q_BarTime_2);

	if(strcmp(productid,"IF")==0 || strcmp(productid,"IH")==0 || strcmp(productid,"IC")==0 ||
			strcmp(productid,"TF")==0  ||strcmp(productid,"T")==0)	{
		//早盘开盘后5分钟内不开仓
		bool Open_morning = (Q_BarTime_2>=0.0920 && Q_BarTime_2<0.1131);
		bool Open_afternoon = (Q_BarTime_2>=0.1305 && Q_BarTime_2<0.1501);
		bool OpenTime=( Open_morning  || Open_afternoon );

		if(OpenTime && !flag_wday )		{
			ret =true;
		}
		else	{
			ret=false;
		}
	}
	else{
		//早盘开盘后1分钟内不开仓
		bool Open_morning1 = (Q_BarTime_2>=0.0901 && Q_BarTime_2<0.1016);
		bool Open_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
		bool Open_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1455);
		//夜盘1分钟内不开仓
		bool Open_night = (Q_BarTime_2>=0.210100 && Q_BarTime_2<=0.240000);
		bool Open_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0235);
		bool OpenTime=( Open_morning1 || Open_morning2 || Open_afternoon || Open_night || Open_night1);

		if(OpenTime && !flag_wday )			{
			ret =true;
		}
		else	{
			ret=false;
		}

	}

	return ret;
}
bool DateUtil::CheckCloseMarketTime()
{
	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentBarTime();

	bool Timemore_afternoon = (Q_BarTime_2>=0.1455 && Q_BarTime_2<0.1500);

	if(Timemore_afternoon && !flag_wday )
	{
		ret =true;
	}
	else
	{
		ret=false;
	}
	return ret;

}

bool DateUtil::CheckSHFEEndMarketTime(int difsec)
{
	bool ret;

	bool flag_wday = this->Check_wday();
	double Q_BarTime_2 = this->GetCurrentSHFEBarTime(difsec);

	bool Timemore_afternoon = (Q_BarTime_2>=0.1455 && Q_BarTime_2<0.1500);

	if(Timemore_afternoon && !flag_wday )	{
		ret =true;
	}
	else	{
		ret=false;
	}
	return ret;

}


void DateUtil::ReadHoliday_CSV(vector<std::string> &temp,const char* path,const char*filename)
{

	CSVFile CSVRead;

	if (!CSVRead.Open(true, path, filename))	{
		printf("ReadHoliday_CSV  ---->File not found!\n");
	}
	else	{
//		printf("File found!\n");
	}

	while (CSVRead.CSVReadNextRow())	{

	    std::string strDate;
		CSVRead.CSVRead("日期", strDate);
		 char time[20];
		sprintf(time,"%s 00:00:00",strDate.c_str());
		temp.push_back(time);

	}// while loop end
}
bool DateUtil::IsOpen(int difsec,const char *exchangeid,const char* productid)
{
	bool ret=false;
	bool opentime =CheckOpenTime(difsec,exchangeid,productid);
	std::string today_sqltime= this->ConvertDatetoSqlTime(this->GetTodayString().c_str());

	bool holiday = CheckHoliday_SqlTime(today_sqltime.c_str()) ;
	if (opentime && !holiday)
	{
		ret=true;
	}
	return ret;
}

bool DateUtil::TodayIsHoliday()
{


	std::string today_sqltime= this->ConvertDatetoSqlTime(this->GetTodayString().c_str());
	bool ret = CheckHoliday_SqlTime(today_sqltime.c_str()) ;

	return ret;
}

bool DateUtil::IsHolidaybyNextTradingDay()
{


	std::string next_sqltime= this->ConvertDatetoSqlTime(this->GetNextTradingDay().c_str());
	bool ret = CheckHoliday_SqlTime(next_sqltime.c_str()) ;

	return ret;
}
