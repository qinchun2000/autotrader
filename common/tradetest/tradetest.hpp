#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <regex.h>
#include <numeric>

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
#include "mysqlcommission.hpp"
#include "mysqltestasset.hpp"
#include "testbase.hpp"

#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef _TRADETEST_HPP
#define _TRADETEST_HPP

class TradeTest:public TestBase
{

public:
	TradeTest();


	TradeTest(const char* pstrategy,const char* year);


	void HisDataCheck(const char* year);
	void HisCsvDataCheck(const char* year);


	void GetInstrumentList(vector<std::string> &instrument_list ,const char* year,MainContractData item);
	HxDayLineData Get_CurrentMaincontractbyTime(vector<std::string> &array,const char* sqltime);

	void MainContract_MinuteLineHold(const char *pinstrumentid,
			const char *datetime,
			SqlAveragePriceData predata,
			HxDayLineData today);

	void EndDay(const char* pinstrumentid,double endday_close,const char* endday);
	void EndDayClose(const char* pinstrumentid,HxDayLineData lastdata);

	int GetPositionbyATR(const char * pinstrumentid);
	int GetMinPosition(const char * pinstrumentid,double price);

	double GetSedimentary(const char * pinstrumentid,int opi,double price);
	void UpdateStopPrice(TestRecordData *tr,MinuteLineData *mindata);
	void UpdateStopWinPrice(TestRecordData *tr,MinuteLineData *mindata);

	void BuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal);
	void SellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal);
	void BuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data);
	void SellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data);

	void bvDayBuyOpen(MinuteLineData mindata,const char * pinstrumentid,
			const char* opensignal,TestRecordData *data,double openprice);
	void bvDaySellOpen(MinuteLineData mindata,const char * pinstrumentid,
			const char* opensignal,TestRecordData *data,double openprice);
	void bvClose(TestRecordData tr,MinuteLineData mindata,const char* closesignal,double closeprice);

	void DayBuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal);
	void DaySellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal);
	void DayBuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data);
	void DaySellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data);
	void Buy(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data);
	void Sell(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data);
	void Close(TestRecordData tr,MinuteLineData mindata,const char* closesignal);
	void CloseAdjust(TestRecordData tr,MinuteLineData mindata,const char* closesignal,int closevolume);
	void HalfCloseAdjust(TestRecordData tr,MinuteLineData mindata,const char* closesignal,int closevolume);

	void SingleTestReport(const char* tablename,const char* table_report);
	void TestReport();


	HxDayLineData data_PreDay();
	CycleData data_PreDynamicCycleData(const char* daytime);
	double  zb_Sedimentary(MinuteLineData *mindata);
	double  zb_PreSettlePrice(const char* time);
	int  zb_PreVolume(const char* time);
	int  zb_PreOpi(const char* time);
	double type_FiveDays(vector<HxDayLineData>  &fivedays_list);
	double  type_FiveDaysATR(vector<HxDayLineData>  &fivedays_list,double atr5);
	double  zb_CCI(vector<HxDayLineData>  &list,double current_tp);
	int process_GetPosition();
	void update_HighLow(MinuteLineData *mindata);
	bool condition_OpenTimebyMinTimeData(MinuteTimeData *mindata,int seconds);
	bool condition_EndTimebyMinTimeData(MinuteTimeData *mindata);
	bool condition_OpenTime(MinuteLineData *mindata,int mode);
	bool condition_EndTime(MinuteLineData *mindata);
	double  zb_TR(double h,double l,double preclose);
	double  zb_TR(double preclose);

	void StaticTestRecord();
	void StaticTestRecord(int maxhold);
	TestAssetData MaxHoldRecord();

	void CheckMinuteTimeSequence();
	void CheckTestInsLists(vector<std::string> &list,const char* year);
	void CheckTradeDaySequence(const char* year);
	void CheckWholeTradeDaySequence();

	void CheckErrorData(const char* productid);
	void CheckErrorData(const char* pinstrumentid,const char* date);

private:


};
#endif /* _TRENDTRADETEST_HPP */
