#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <regex.h>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <float.h>

#include "mysqldata.hpp"
#include "mysqldayline.hpp"
#include "mysqltestrecord.hpp"
#include "mysqlcycle.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqlhismindata.hpp"
#include "mysqlutil.hpp"
#include "mysqlhisatr.hpp"
#include "mysqldatetime.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqltestreport.hpp"
#include "mysqltestasset.hpp"
#include "mysqlhissection.hpp"
#include "mysqlminutetime.hpp"
#include "mysqltestsignal.hpp"
#include "mysqltestsecassess.hpp"
#include  "tradetest.hpp"
#include "redissignal.hpp"

#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef _REALTRADETEST_HPP
#define _REALTRADETEST_HPP

class RealTradeTest:public TradeTest
{

public:
	RealTradeTest();

	RealTradeTest(const char* pstrategy,const char* year);
	void InitialAssessTable();
	void AllTimeIndex_TrendTradeTest();
	void AllTimeIndex_DayTradeTest();
	void OpenSignal_MinuteTime_DynamicCycle(const char *current_mt);
	void OpenSignal_MinuteTime_SameUpDown(const char *current_mt);
	void InitialSignal( Signal *signal,MinuteLineData *mindata);
	void Operate();

	void BuyOpenSignal(const char* section,const char * pinstrumentid,const char* opensignal);
	void SellOpenSignal(const char* section,const char * pinstrumentid,const char* opensignal);
	void Dynamic_Hold(const char* pinstrumentid);
	void EndTimeClose(const char* pinstrumentid);
	void Create_SectionTestReportXlsFile(std::string pathfile);
	void Create_TestReportXlsFile(std::string pathfile);
	void Create_TestRecordXlsFile(std::string pathfile);

private:


};
#endif /* _REALTRADETEST_HPP */
