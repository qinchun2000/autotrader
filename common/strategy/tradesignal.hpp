#ifndef _TRADESIGNAL_HPP
#define _TRADESIGNAL_HPP

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

#include "httpbase.hpp"

#include "mysqlaveprice.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlorder.hpp"
#include "mysqldata.hpp"
#include "mysqlposdetail.hpp"
#include "mysqlinvest.hpp"
#include "mysqlutil.hpp"
#include "mysqlhold.hpp"
#include "mysqlhisatr.hpp"
#include "mysqlma.hpp"
#include "mysqlema.hpp"
#include "mysqltestreport.hpp"

#include "redismd.hpp"
#include "redisorder.hpp"
#include "redissection.hpp"
#include "depthmarket.hpp"
#include "dateutil.hpp"

#include "logutil.hpp"
#include "lock.hpp"
#include <boost/regex.hpp>
#include <json/json.h>
#include "cping.hpp"


#include "config.hpp"
#include "strategy.hpp"


using namespace std;
using namespace log4cplus;



class TradeSignal:public Strategy
{

public:

	TradeSignal();





	void Check_MASignal();
	void Check_CycleSignal();
	void Check_EMACycleSignal();
	void Check_EMACrossSignal();

	ReferenceData GetKeyPrice(const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int n);


	EMAData  GetEMAData(const char* pinstrumentid,const char* tradingdate,const char* exchangeid,double lastprice);
	HisEMAData GetPreEMAData(const char* pinstrumentid,const char* tradingdate);
	HisEMAData GetHisEMAData_Angle(const char* pinstrumentid,const char* tradingdate);

	void Stop_SignalCheck();


	void Reverse_SignalCheck();
	void TrendReverse_SignalCheck();
	void MAReverse_SignalCheck();
	void EMAReverse_SignalCheck();

	void EndTimeClose_SignalCheck();

	void WinPrice_SignalCheck();
	void RiskMangement_SignalCheck();



private :



};

#endif /* _TRADESIGNAL_HPP */
