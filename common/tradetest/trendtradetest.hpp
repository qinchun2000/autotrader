#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <map>
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
#include  "tradetest.hpp"

#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef _TRENDTRADETEST_HPP
#define _TRENDTRADETEST_HPP

class TrendTradeTest:public TradeTest
{

public:
	TrendTradeTest();


	TrendTradeTest(const char* pstrategy,const char* year);

	void AllMainContract_TrendTradeTest(const char* year);
	void SingleIns_Trade(vector<std::string> &array);
	void InitialSingleTest();

	void Cycle_Open_MinuteLine();

	void OneDay_MinuteLine_Cycle_Open_();

	void OneDay_MinuteLine_PreDayPrice_Open_();
	void OneDay_MinuteLine_PreDayPrice_Hold(const char* pinstrumentid);

	void OneDay_MinuteLine_EMA_Open_();
	void OneDay_MinuteLine_EMA_Hold(const char* pinstrumentid);

	//变N 突破开仓
	void OneDay_MinuteLine_DynamicCycle_Open_();

	void OneDay_MinuteLine_DynamicLastATRCycle_Open_();

	//MA 10 20 50 顺势开仓
	void OneDay_MinuteLine_MA_Open_();
	//MA 4 9 18 顺势开仓
	void OneDay_MinuteLine_MA4918_Open_();

	void OneDay_MinuteLine_MA4950_Open_();

	void OneDay_MinuteLine_MA102050_Open_();

	void OneDay_MinuteLine_DynamicMACycle_Open_();


	//动态持仓,浮动止盈止损,调仓
	void OneDay_MinuteLine_Dynamic_Hold(const char* pinstrumentid);

	int LineType(vector<HxDayLineData> list);
	void BigOneMinVolume_Open_MinuteLine();
	void BigVolume_Hold_MinuteLine(const char* pinstrumentid);

	void MA_Open_MinuteLine();
	void MA_Hold_MinuteLine(const char* pinstrumentid);

	void EMA_Open_MinuteLine();
	void EMA_Hold_MinuteLine(const char* pinstrumentid);


	void EMA_Hold_Emergence_MinuteLine(const char *pinstrumentid,
			const char *datetime,
			SqlAveragePriceData predata,
			HxDayLineData today);

	void Hold_OMA_MinuteLine(const char *pinstrumentid,
			const char *datetime,
			SqlAveragePriceData predata,
			HxDayLineData today);
	void UpdateOpenReference_EMA(const char *pinstrumentid,const char* mintime,EMAData emadata);

	int  Strategy_MA10_20_50_Pre();
	int  Strategy_MA4_9_50_Pre();

	void Create_TestReportXlsFile(std::string pathfile);
	void Create_TestRecordXlsFile(std::string pathfile);



private:


};
#endif /* _TRENDTRADETEST_HPP */
