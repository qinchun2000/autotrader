#ifndef _STRATEGY_HPP
#define _STRATEGY_HPP

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
#include "mysqlsection.hpp"
#include "mysqlsecassess.hpp"
#include "mysqlglobalview.hpp"

#include "redismd.hpp"
#include "redissignal.hpp"
#include "redissection.hpp"


#include "mysqldayline.hpp"
#include "mysqlmindata.hpp"
#include "mysqlutil.hpp"
#include "mysqlhold.hpp"
#include "mysqlcommission.hpp"
#include "tech.hpp"
#include "dateutil.hpp"
#include "logutil.hpp"
#include <float.h>//
#include "config.hpp"
#include "assetbase.hpp"
#include "logbase.hpp"
#include "strategybase.hpp"


using namespace std;
using namespace log4cplus;



class Strategy:public StrategyBase
{

public:
	Strategy();
	Strategy(const char* strategyname);
	void  Open();
	void  Close();
	void Check_WholeMarketCapital();
	void Check_UpDownRate();



	int  GetPositionByAvalible(MysqlDayLine *mysqldayline,double avalibe,
			const char* pinstrumentid,DepthMarket depthmarket,double today_tr);
	int  GetPosition(const char* pinstrumentid,DepthMarket depthmarket,double today_tr);
	int  GetMinPosition(const char* pinstrumentid,DepthMarket depthmarket,double today_tr);
	double  GetATRStopPrice(const char* pinstrumentid,int direction,double openprice);

	int CheckDifPriceByNextDayline(MysqlDayLine * pmysqldayline,
				const char*maincontract,
				const char*nextcontract,
				int n,
				double dif);

	void DayTrade_OpenSignalCheck(HoldData item,
			std::string signalname,
			const char* tradingday,
			int signalcount,
			int direction,
			int offset,
			double lastprice);

	void DayTrade_EndTimeClose_SignalCheck();


	void Close_WinPrice_SignalCheck();
	void Close_DynamicStop_SignalCheck();
	void Close_LimitPrice_SignalCheck();

	void Close_SedimentaryOut_SignalCheck();
	void Close_ExpireMonthClose_SignalCheck();

	void Close_SectionTrendStop_SignalCheck();

	void run();
	void Open_BigVolumOneMinSignal();
	void Open_CalendarSpreadSignal();
	void Open_CalendarSpreadSignal_Average();
	void Open_AbsMaxUpDownRate();
	void Open_SpreadSignal_Average();
	void Open_Simple();
	void Open_CalendarSpread10Signal();
	void Open_CalendarSpreadATRSignal();

	void Open_MACrossSignal();
	void Open_CycleNSignal();
	void Open_CycleN_NewSignal();
	void Hold_MA50Reverse_SignalCheck();
private :




};

#endif /* _STRATEGY_HPP */
