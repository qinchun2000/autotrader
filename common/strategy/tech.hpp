#ifndef _TECH_HPP
#define _TECH_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

#include "mysqlinstrument.hpp"
#include "mysqltradingday.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlorder.hpp"
#include "mysqldata.hpp"
#include "mysqlhisatr.hpp"
#include "redismd.hpp"
#include "redissignal.hpp"
#include "redissection.hpp"


#include "mysqldayline.hpp"
#include "mysqlutil.hpp"
#include "mysqlhold.hpp"
#include "mysqluser.hpp"
//#include "mysqlma.hpp"
//#include "mysqlema.hpp"
//#include "mysqltestreport.hpp"




#include "dateutil.hpp"
#include "logutil.hpp"


//#include "lock.hpp"
//#include <boost/regex.hpp>
//#include <json/json.h>

#include <float.h>
//
//#include "config.hpp"


using namespace std;
using namespace log4cplus;



class Tech
{

public:

	Tech();
	virtual ~Tech();

	void SetRedisSignal(RedisSignal *redis);
	RedisSignal* GetRedisSignal();
	void SetRedisDepthMarket(RedisDepthMarket* rdepthmarket);
	RedisDepthMarket* GetRedisDepthMarket();
	void SetRedisSection(RedisSection *redis);
	RedisSection* GetRedisSection();


	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();

	void SetTradingDay(const char* date);
//	TradingDayData  GetTradingDayData();
	std::string GetTradingDay();
	void UpdateTradingDay();

	void SetTradingDaySqlTime(const char* date);
	std::string GetTradingDaySqlTime();
	void UpdateTradingDaySqlTime();

	void SetDifSec(int sec);
	int GetDifSec();
	void UpdateDifSec();
	int GetTradingDayDifSec();

//	void SetATR_Weight(double weight);
//	double GetATR_Weight();

	MAData  GetPreMADatabyTradingDay(const char* pinstrumentid,const char* exchangeid);
	MAData  GetMADatabyTradingDay(const char* pinstrumentid,const char* exchangeid,double lastprice);
	double  GetATRbyTradingDay(const char* pinstrumentid);
	double  GetATRbyOpenDate(const char* pinstrumentid,const char*opendate_sqltim);

	double GetCurrentDayTRbyDepth(DepthMarket *pdepthmarket);
	bool ValidCheck_DayLineNdays(MysqlDayLine *mysqldayline,const char* pinstrumentid,
			const char* tradingdate,const char* exchangeid,int N);
	bool ValidCheck_DepthMarket(DepthMarket *depthmarket);

private :

	std::string _tradingday_date;
	std::string _tradingday_sqltime;
	double _difsec=0;

//	double _atr_weight;

	RedisDepthMarket *_predisdepthmarket=NULL;
	RedisSection * _predissection=NULL;
	RedisSignal *_predissignal=NULL;
	std::string _strategyName;
};

#endif /* _TECH_HPP */
