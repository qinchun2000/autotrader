#include "tech.hpp"

Tech::Tech() {
//	SetATR_Weight(2.0);   //ATR 作为止损价差的基数,止损价差=atr* 权重倍数
}
Tech::~Tech() {

}
void Tech::SetRedisSignal(RedisSignal *redis)
{
	this->_predissignal=redis;
}

RedisSignal* Tech::GetRedisSignal()
{
	return this->_predissignal;
}
void Tech::SetRedisDepthMarket(RedisDepthMarket *rdepthmarket)
{
	this->_predisdepthmarket=rdepthmarket;
//	printf("SetRedisDepthMarket   ---host    %s \n",this->_predisdepthmarket->GetHost().c_str());
}

RedisDepthMarket* Tech::GetRedisDepthMarket()
{

//	printf("GetRedisDepthMarket   ---host    %s \n",this->_predisdepthmarket->GetHost().c_str());
	return this->_predisdepthmarket;
}

void Tech::SetRedisSection(RedisSection *redis)
{
	this->_predissection=redis;
}

RedisSection * Tech::GetRedisSection()
{
	return this->_predissection;
}


void Tech::SetStrategyName(const char* strategyname)
{
	this->_strategyName=strategyname;
}

std::string Tech::GetStrategyName()
{
	return this->_strategyName;
}


void Tech::SetTradingDay(const char* date)
{
	this->_tradingday_date=date;
}

//TradingDayData  Tech::GetTradingDayData()
//{
////	MysqlUser mysqluser;
////	User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
////	MysqlTradingDay mysqltradingday(userinfo.UserID);
//	TradingDayData tradingdaydata;
////	tradingdaydata= mysqltradingday.Find_LastTradingDay();
//	return tradingdaydata;
//}
std::string Tech::GetTradingDay()
{
	return this->_tradingday_date;
}
void Tech::UpdateTradingDay()
{
	DateUtil dateutil;
	double current_bartime =dateutil.GetCurrentBarTime();

	if (this->_tradingday_date.length()==0)	{
		MysqlUser mysqluser;
		User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
		MysqlTradingDay mysqltradingday(userinfo.UserID);
		TradingDayData tradingdaydata;
		tradingdaydata= mysqltradingday.Find_LastTradingDay();

		std::string str_tradingdate;
		if(strcmp(tradingdaydata.TradingDay,this->_tradingday_date.c_str())!=0)		{
			printf("Tech::UpdateTradingDay ------> 交易日为空,更新交易日 [%s]---------->[%s] \n",
					_tradingday_date.c_str(),tradingdaydata.TradingDay);
//			_tradingday_date = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

			SetTradingDay(tradingdaydata.TradingDay);
		}

	}

	std::string tradingday ;
	tradingday = this->_tradingday_date;
	tradingday.append("160000");

	time_t tradingclose=dateutil.ConvertStringtoTimet(tradingday.c_str());
	time_t tt = time(NULL);//这句返回的只是一个时间cuo

	if (tt>tradingclose)	{

		MysqlUser mysqluser;
		User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
		MysqlTradingDay mysqltradingday(userinfo.UserID);
		TradingDayData tradingdaydata;
		tradingdaydata= mysqltradingday.Find_LastTradingDay();

		std::string str_tradingdate;
		if(strcmp(tradingdaydata.TradingDay,this->_tradingday_date.c_str())!=0)		{
			printf("Tech::UpdateTradingDay ------> 当前时间>16:00 更新交易日 [%s]---------->[%s] \n",
					_tradingday_date.c_str(),tradingdaydata.TradingDay);
//			_tradingday_date = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
			SetTradingDay(tradingdaydata.TradingDay);
		}

	}

	printf("Tech::UpdateTradingDay End !!!\n");
}

//void Tech::UpdateTradingDay()
//{
//	DateUtil dateutil;
//	double current_bartime =dateutil.GetCurrentBarTime();
//	bool IsNight = (current_bartime>=0.200000 && current_bartime<=0.235959);
//
//	if (this->_tradingday_date.length()==0)	{
//			MysqlUser mysqluser;
//			User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
//			MysqlTradingDay mysqltradingday(userinfo.UserID);
//			TradingDayData tradingdaydata;
//			tradingdaydata= mysqltradingday.Find_LastTradingDay();
//
//
//			std::string str_tradingdate;
//			if(strcmp(tradingdaydata.TradingDay,this->_tradingday_date.c_str())!=0)		{
//				_tradingday_date = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
//			}
//
//		}
//
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(_tradingday_date.c_str());
//	time_t tradingday_tt = dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());
//
//	std::string currentday_sqltime = dateutil.ConvertDatetoSqlTime(dateutil.GetTodayString().c_str());
//	time_t current_tt = dateutil.ConvertSqlTimetoTimet(currentday_sqltime.c_str());
//
//	if (IsNight)	{
//
//		if (current_tt <=tradingday_tt )   //交易日未更新
//		{
//			MysqlUser mysqluser;
//			User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
//			MysqlTradingDay mysqltradingday(userinfo.UserID);
//			TradingDayData tradingdaydata;
//			tradingdaydata= mysqltradingday.Find_LastTradingDay();
//
//
//			std::string str_tradingdate;
//			if(strcmp(tradingdaydata.TradingDay,this->_tradingday_date.c_str())!=0)		{
//				_tradingday_date = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
//			}
//
//
//		}
//	}
//	else	{
//
//		if (current_tt > tradingday_tt )   //交易日未更新
//			{
//				MysqlUser mysqluser;
//				User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
//				MysqlTradingDay mysqltradingday(userinfo.UserID);
//				TradingDayData tradingdaydata;
//				tradingdaydata= mysqltradingday.Find_LastTradingDay();
//
//
//				std::string str_tradingdate;
//				if(strcmp(tradingdaydata.TradingDay,this->_tradingday_date.c_str())!=0)		{
//					_tradingday_date = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
//				}
//
//
//			}
//
//
//	}
//
//}
void Tech::SetTradingDaySqlTime(const char* date)
{
	this->_tradingday_sqltime=date;
}
std::string Tech::GetTradingDaySqlTime()
{
//	printf("GetTradingDaySqlTime   --> begin \n");
//	if (this->_tradingday_sqltime.length()==0)
	{
		MysqlUser mysqluser;
		User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
		MysqlTradingDay mysqltradingday(userinfo.UserID);
		TradingDayData tradingdaydata;
		tradingdaydata= mysqltradingday.Find_LastTradingDay();

		std::string str_tradingdate;
		str_tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

		std::string tradingday;
		char c_time[20];
		sprintf(c_time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
				str_tradingdate.substr(6,2).c_str());

		if(strcmp(c_time,this->_tradingday_sqltime.c_str())!=0)
		{

			this->_tradingday_sqltime=c_time;   //2017-09-01 00:00:00
//			printf("GetTradingDaySqlTime   -->  %s  \n",this->_tradingday_sqltime.c_str());
		}
	}
	return this->_tradingday_sqltime;
}

void Tech::UpdateTradingDaySqlTime()
{
//	printf("GetTradingDaySqlTime   --> begin \n");
//	if (this->_tradingday_sqltime.length()==0)
	{
		MysqlUser mysqluser;
		User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
		MysqlTradingDay mysqltradingday(userinfo.UserID);
		TradingDayData tradingdaydata;
		tradingdaydata= mysqltradingday.Find_LastTradingDay();

		std::string str_tradingdate;
		str_tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

		std::string tradingday;
		char c_time[20];
		sprintf(c_time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
				str_tradingdate.substr(6,2).c_str());

		if(strcmp(c_time,this->_tradingday_sqltime.c_str())!=0)
		{

			this->_tradingday_sqltime=c_time;   //2017-09-01 00:00:00
//			printf("GetTradingDaySqlTime   -->  %s  \n",this->_tradingday_sqltime.c_str());
		}
	}

}

void Tech::SetDifSec(int sec)
{
	this->_difsec=sec;
}

int Tech::GetDifSec()
{
	return this->_difsec;
}
void Tech::UpdateDifSec()
{
	MysqlUser mysqluser;
	User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
	MysqlTradingDay mysqltradingday(userinfo.UserID);

	mysqltradingday.CreateTable();

	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	this->_difsec=tradingdaydata.DifSeconds;
}
int Tech::GetTradingDayDifSec()
{
	UpdateDifSec();

	return GetDifSec();
}
//void Tech::SetATR_Weight(double weight)
//{
//	this->_atr_weight=weight;
//}
//
//double Tech::GetATR_Weight()
//{
//	return this->_atr_weight;
//}

MAData  Tech::GetPreMADatabyTradingDay(const char* pinstrumentid,const char* exchangeid)
{
	MAData madata;
	MysqlDayLine mysqldayline;

	madata.MA50 =0;
	madata.MA20=0;
	madata.MA10=0;

	DateUtil dateutil;
	std::string str_tradingday = GetTradingDaySqlTime().c_str();
	int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);


	madata.MA50 = mysqldayline.Get_HisMAData(pinstrumentid,str_tradingday.c_str(),50);
	madata.MA20 = mysqldayline.Get_HisMAData(pinstrumentid,str_tradingday.c_str(),20);
	madata.MA10 = mysqldayline.Get_HisMAData(pinstrumentid,str_tradingday.c_str(),10);

	double pre_ma50_offset = mysqldayline.Get_PreHisMADatabyOffset(pinstrumentid,str_tradingday.c_str(),50,1);
	madata.MA50_X= atan((madata.MA50/pre_ma50_offset-1)*100)*180/M_PI;

	return madata;
}

MAData  Tech::GetMADatabyTradingDay(const char* pinstrumentid,const char* exchangeid,double lastprice)
{
	MAData madata;
	MysqlDayLine mysqldayline;

	madata.MA50 =0;
	madata.MA20=0;
	madata.MA10=0;

	DateUtil dateutil;
	std::string str_tradingday = GetTradingDaySqlTime().c_str();
	int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);


	madata.MA50 = mysqldayline.Get_MADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),50,lastprice);
	madata.MA20 = mysqldayline.Get_MADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),20,lastprice);
	madata.MA10 = mysqldayline.Get_MADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),10,lastprice);

	double pre_ma50_offset = mysqldayline.Get_PreHisMADatabyOffset(pinstrumentid,str_tradingday.c_str(),50,0);
	madata.MA50_X= atan((madata.MA50/pre_ma50_offset-1)*100)*180/M_PI;

	return madata;
}
double  Tech::GetATRbyTradingDay(const char* pinstrumentid)
{
	MysqlDayLine mysqldayline;

	std::string tradingday=GetTradingDaySqlTime().c_str();

	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

	HxDayLineData lastday=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday.c_str());
	double tr=max(lastday.Close,max(depthmarket.LowestPrice,depthmarket.HighestPrice));
	double today_atr=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday.c_str(),20,tr);

	return today_atr;

}
double  Tech::GetATRbyOpenDate(const char* pinstrumentid,const char*opendate_sqltim)
{
	MysqlDayLine mysqldayline;

	ATRData data=mysqldayline.Collect_HisATR_NData(pinstrumentid,opendate_sqltim,20);


	return data.ATR;

}
double Tech::GetCurrentDayTRbyDepth(DepthMarket *pdepthmarket)
{
	double tr=0.0;
	double tr_lowclose = abs(pdepthmarket->LowestPrice-pdepthmarket->PreClosePrice);
	double tr_highclose =abs(pdepthmarket->HighestPrice-pdepthmarket->PreClosePrice);
	double tr_highlow = abs(pdepthmarket->HighestPrice-pdepthmarket->LowestPrice);
	tr=max(tr_highlow,max(tr_lowclose,tr_highclose));
	return tr;
}
bool Tech::ValidCheck_DayLineNdays(MysqlDayLine *mysqldayline,const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int N){

//	printf("ValidCheck_DayLine [%s]   交易日[%s]     exchange: %s \n",pinstrumentid,tradingdate,exchangeid);
	bool ret=false;

//	MysqlUtil mysqlutil(this->GetStrategyName().c_str());
	DateUtil dateutil;
	std::string str_sqltradingday = dateutil.ConvertDatetoSqlTime(tradingdate);

	bool valid5;
	int count;

	if(mysqldayline->Exist_Table(pinstrumentid))	{
		count=mysqldayline->Count_AllListsbyTableName(pinstrumentid);
		if(count==0)		{
			printf("ValidCheck_DayLineNdays [%s]   日线无数据,需要更新     exchange: %s \n",pinstrumentid,exchangeid);

			count =mysqldayline->Count_AllListsbyTableName(pinstrumentid);
		}
	}
	else	{
		printf("ValidCheck_DayLineNdays [%s]   不存在日线数据表     exchange: %s \n",pinstrumentid,exchangeid);
		mysqldayline->CreateTable(pinstrumentid);

		count =mysqldayline->Count_AllListsbyTableName(pinstrumentid);
	}

	std::string dayline_lastdate =mysqldayline->GetLastDateTime(pinstrumentid);
	time_t tm_lastdate=dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());
	std::string path = "/root/autotrader/config/";
	std::string filename = "holiday.csv";
	time_t tm_enddayline =  dateutil.CheckHisLastDayLinebyDate_ExcludeHoliday(
			tradingdate,this->GetDifSec(),path.c_str(),filename.c_str());

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(str_sqltradingday.c_str());

	//				printf("	dayline  ------>tm_yesterday [%s]: %ld    tm_dayline [%s]: %ld     \n  ",
	//												dayline_lastdate.c_str(),tm_yesterday,tradingday.c_str(),tm_dayline);

	if (tm_lastdate ==tm_enddayline)	{
		valid5 =true;
	}
	else	{
		printf("Tech::ValidCheck_DayLineNdays  [%s]-----> 补齐数据 尝试   \n",pinstrumentid);

		std::string dayline_lastdate_new =mysqldayline->GetLastDateTime(pinstrumentid);
		time_t tm_lastdate_new=dateutil.ConvertSqlTimetoTimet(dayline_lastdate_new.c_str());
		if (tm_lastdate_new ==tm_enddayline){
				valid5 =true;
		}
		else if(tm_lastdate_new >tm_enddayline){
			valid5 =true;
			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
//			printf("ValidCheck_DayLineNdays[%s]-->	dayline 数据大于截止日期 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
//								str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);
		}
		else{
			valid5=false;
			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
			printf("Tech::ValidCheck_DayLineNdays[%s]-->	dayline 数据缺失 截止日期不相同 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
					str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);

		}
	}


	bool valid0 = count > N;

	bool b_holiday = dateutil.CheckHoliday_SqlTime(str_sqltradingday.c_str());   //判断当前交易日是否是假日

	ret = valid0 && valid5 && !b_holiday;

	if (!ret)	{
		if (b_holiday)		{
			printf("	当前交易日未假日: %s  --> 休市日 %s \n",pinstrumentid,str_sqltradingday.c_str());
		}

		if (!valid0)			{
				printf("	行情数据不合规: %s  --> 日线数据长度[%d]不足 %d \n",pinstrumentid,count,N);

		}

		if (!valid5)			{
			printf("	历史数据不合规: %s  --> 日线数据缺失 最后更新日期[%s]  \n",pinstrumentid,dayline_lastdate.c_str());
		}
	}


	return ret;
}
bool Tech::ValidCheck_DepthMarket(DepthMarket *depthmarket){

	bool ret=false;
	DateUtil dateutil;
	std::string str_actionday=depthmarket->ActionDay;
	double qbartime = dateutil.GetCurrentBarTime();
	if(qbartime >0.200000 &&str_actionday!=dateutil.GetTodayString())	 {
		str_actionday=dateutil.GetTodayString();
	}

	char buf[30];
	sprintf (buf,"%s-%s-%s %s",str_actionday.substr(0,4).c_str(),str_actionday.substr(4,2).c_str(),
			str_actionday.substr(6,2).c_str(),depthmarket->UpdateTime);
	std::string str_updatetime = buf;
	std::string str_current = dateutil.GetCurrentSqlTimeString();
	time_t t_updatetime = dateutil.ConvertSqlTimetoTimet(str_updatetime.c_str());
	time_t t_currenttime = dateutil.ConvertSqlTimetoTimet(str_current.c_str());

	bool valid_updatetime = abs(t_updatetime-t_currenttime)<60;
	if(!valid_updatetime){
		printf("	ValidCheck_DepthMarket 行情数据时间延迟大于10秒: %s  --> Updatime: %s [%ld]   currenttime: %s [%ld]  \n",
				depthmarket->InstrumentID,str_updatetime.c_str(),t_updatetime,str_current.c_str(),t_currenttime);

		return false;
	}

	bool valid_upperlimitprice = depthmarket->UpperLimitPrice >1.0 ;   // 涨停价
	if(!valid_upperlimitprice){
		printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 涨停价数据不合规 [%.3f]  \n",
				depthmarket->InstrumentID,depthmarket->UpperLimitPrice);

		return false;
	}

	bool valid_lowerlimitprice = fabs(depthmarket->LowerLimitPrice) >DBL_EPSILON ;  //跌停价
	if(!valid_lowerlimitprice){
		printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 跌停价数据不合规 [%.3f]  \n",
				depthmarket->InstrumentID,depthmarket->LowerLimitPrice);
		return false;
	}

	bool valid_lastprice_base  = depthmarket->LastPrice >1.0 ;
	bool valid_lastprice_normal = depthmarket->LastPrice >= depthmarket->LowerLimitPrice  &&
			depthmarket->LastPrice <= depthmarket->UpperLimitPrice;

	bool valid_lastprice= valid_lastprice_base && valid_lastprice_normal;

	if(!valid_lastprice){
		printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> lastprice [%.3f] 价格不合规  \n",
				depthmarket->InstrumentID,depthmarket->LastPrice);
		return false;
	}
	bool valid1 = depthmarket->OpenPrice >1.0 ;
	bool valid2 = depthmarket->PreSettlementPrice >1.0 ;
	bool valid3 = depthmarket->PreClosePrice >1.0 ;
	bool valid6 = depthmarket->AveragePrice >1.0 ;

	bool valid_ask_base = depthmarket->AskPrice1 >1.0 ;
	bool valid_ask_normal= depthmarket->AskPrice1 <=depthmarket->UpperLimitPrice &&
			depthmarket->AskPrice1 >=depthmarket->LowerLimitPrice;
	bool valid_uplimit_ask =  depthmarket->AskPrice1 ==0.0 ||  depthmarket->AskPrice1>1000000000000.0;

	bool valid_bid_base = depthmarket->BidPrice1 >1.0 ;
	bool valid_bid_normal= depthmarket->BidPrice1 <=depthmarket->UpperLimitPrice &&
				depthmarket->BidPrice1 >=depthmarket->LowerLimitPrice;
	bool valid_lowlimit_bid =  depthmarket->BidPrice1 ==0.0 ||  depthmarket->BidPrice1>1000000000000.0;

	bool valid_ask = valid_ask_base&& valid_ask_normal;
	bool valid_bid = valid_bid_base&& valid_bid_normal;

	bool valid_uplimit = valid_uplimit_ask && valid_bid;   //涨停
	bool valid_lowlimit =valid_lowlimit_bid &&valid_ask;  //跌停

	if(!valid_ask){

		//再查看是否处于涨停状态,如果处于涨停状态，则按照正常情况通过
		if(!valid_uplimit){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> AskPrice1 申卖价一 [%.3f] 不合规!!!!   \n",
					depthmarket->InstrumentID,depthmarket->AskPrice1);
			return false;
		}
	}

	if(!valid_bid){

		//再查看是否处于跌停状态,如果处于跌停状态，则按照正常情况通过
		if(!valid_lowlimit){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> BidPrice1 申买价一 [%.3f] 不合规!!!! \n",
					depthmarket->InstrumentID,depthmarket->BidPrice1);
			return false;
		}

	}

	bool valid9 = depthmarket->VolumeMultiple >=1  &&  depthmarket->VolumeMultiple <=20000 ;
	bool valid_margin = depthmarket->Margin >0.01 && depthmarket->Margin < 0.3 ;

	if(valid1 && valid2  && valid3 && valid6   && valid9 && valid_margin){
		ret = true;
	}
	else{
		if (!valid1){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 开盘价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->OpenPrice);
		}
		else if (!valid2){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 昨结算价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->PreSettlementPrice);
		}
		else if (!valid3){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 昨收盘价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->PreClosePrice);
		}
		else if (!valid6){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 均价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->AveragePrice);
		}
		else if (!valid9){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 交易单位[%d] 不合规,超出范围[1-20000]   \n",depthmarket->InstrumentID,
					depthmarket->VolumeMultiple);
		}
		else if (!valid_margin){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> Margin[%.2f] 不合规,超出范围[0.01-0.3]   \n",depthmarket->InstrumentID,
					depthmarket->Margin);
		}
		else {

		}

	}
	return ret;
}
