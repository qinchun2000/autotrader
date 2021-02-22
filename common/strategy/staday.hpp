#ifndef _STADAY_HPP
#define _STADAY_HPP

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
#include "mysqlhold.hpp"
#include "mysqltestreport.hpp"
#include "mysqlmindata.hpp"
#include "strategy.hpp"


using namespace std;
using namespace log4cplus;



class StaDay:public Strategy
{

public:

	StaDay();
	void SetMinTableName(const char* table);
	std::string GetMinTableName();
	void SetMysqlMindata(MysqlMinData * pmysqlmindata);
	MysqlMinData * GetMysqlMindata();

	void DayTrade_Open_BigVolume();
	void  DayTrade_Open_CycleN();
	void  Check_MinuteVolumeOpiChange();
	void  Check_VolumeOpiChange(const char* pinstrumentid);
	int Test(DepthMarket *pdepthmarket,InstrumentData *pinsdata);
	void Hold_MA50Reverse_SignalCheck();
	void DayTrade_VolumeReverse_SignalCheck();
	void DayTrade_EndTimeClose_SignalCheck();
	void DayTrade_WinPrice_SignalCheck();
	void DayTrade_WinProfit_SignalCheck();
	void DayTrade_DynamicStop_SignalCheck();
	void DayTrade_Stop_SignalCheck();
	void DayTrade_RiskMangement_SignalCheck();

private :
	MysqlMinData _mysqlmindata;

	MysqlMinData * _pmysqlmindata=NULL;
	std::string _min_tablename;
};

#endif /* _STADAY_HPP */
