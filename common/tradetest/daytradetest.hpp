#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <map>
#include<queue>
#include <string>
#include <math.h>
#include <vector>
#include <regex.h>
#include<numeric>

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
#include "mysqlhissection.hpp"
#include "mysqlminutetime.hpp"
#include "mysqltestasset.hpp"
#include "mysqlprostatis.hpp"
#include  "tradetest.hpp"

#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef _DAYTRADETEST_HPP
#define _DAYTRADETEST_HPP



class DayTradeTest:public TradeTest
{

public:
	DayTradeTest();
	DayTradeTest(const char* pstrategy,const char* year);
	~DayTradeTest();
	void AllMainContract_DayTradeTest(const char* year);
	void SingleIns_Trade(vector<std::string> &array);
	void SingleIns_Trade_Test(vector<std::string> &array);
	void SingleIns_Statistics(vector<std::string> &array);

	void BigVolume_Open_MinuteLine();
	void MA_Open_MinuteLine();
	void CycleN_Open_MinuteLine();




	void GKGZ_Open_MinuteLine();
	void CCI_Test();
	void MinuteCycleN_Open_MinuteLine();
	void MinuteCycleN_AverageBack_Open_MinuteLine();

	void CycleNVolumeOpi_Open_MinuteLine();
	void VolumeOPI_Open_MinuteLine();

	void BigBigVolume_Open_MinuteLine();
	static bool  Min_LongSortbyNumber( TypeMinuteLineData min1, TypeMinuteLineData min2);
	static bool  Min_ShosrtSortbyNumber( TypeMinuteLineData min1, TypeMinuteLineData min2);
	static bool  LongSortbyNumber( FiveDayLineData stu1, FiveDayLineData stu2);
	static bool  ShortSortbyNumber( FiveDayLineData stu1, FiveDayLineData stu2);

	string type_FiveDay(vector<FiveDayLineData> fivedaydata_list,MinuteLineData *mindata,int direction);
	string type_FiveMinOPI(MinuteLineData *mindata,int direction);
	void Statistics_Ins_MinuteLine();

	void BigOneMinVolume_Open_MinuteLine();
	void BigVolume_Hold_MinuteLine(const char* pinstrumentid);
	void EMA52060_Open_MinuteLine();

	double zb_ema(HxDayLineData *preday,const char* mintime,int x,int y);

	double  EMA(std::vector<double> &X, int N);
	void BigBigVolume_AverageBack_Open_MinuteLine();

	void PivotPoint_Open_MinuteLine();

	void MinuteOpen(const char*section,
			const char *pinstrumentid,
			const char *datetime,
			SqlAveragePriceData predata,
			double openprice,
			CycleData highdata,
			CycleData lowdata,
			CycleData cycledata);
	double zb_AttackVolume(MysqlHisMinData *mysqlhismindata,MinuteLineData *mindata,double volopirate);

	double zb_AddOpiRate(MinuteLineData *mindata, int startopi);

	double zb_3Min_VolOpiRate(MysqlHisMinData *mysqlhismindata,MinuteLineData *mindata);

	void Create_TestReportXlsFile(std::string pathfile);

	void Create_TestRecordXlsFile(std::string pathfile);
	void Create_StatisticsXlsFile(std::string pathfile);
private:
	std::string strategy;
	std::string tablename_test;

};
#endif /* _DAYTRADETEST_HPP */
