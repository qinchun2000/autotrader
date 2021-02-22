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
#include "mysqlhissection.hpp"
#include "mysqlminutetime.hpp"
#include "mysqlcommission.hpp"
#include "mysqltestasset.hpp"


#ifndef _TESTBASE_HPP
#define _TESTBASE_HPP

struct HighLowAnalyst{
	std::string MinuteTime;
	int Direction;
	double last_highlow;
	double HighLowPrice;
	double HLOpenRate;

};


class TestBase
{

public:
	TestBase();

	virtual ~TestBase();
	TestBase(const char* pstrategy,const char* year);
	void SetInitialMoney(double money);
	double GetInitialMoney();
	void SetRiskRate(double risk);
	double GetRiskRate();
	void SetLimitSedimentary(double money);
	double GetLimitSedimentary();

	void SetSingleMarginRate(double rate);
	double GetSingleMarginRate();

	void SetOpenTimeMode(int mode);
	int GetOpenTimeMode();
	void SetCloseTimeMode(int mode);
	int GetCloseTimeMode();

	void SetTable_Report();
	void SetTable_Record_Prefix();
	std::string  GetTable_Record_Prefix();

	void SetTable_Year_DateTime(const char * year);
	std::string GetTable_Year_DateTime();

	void SetTable_Whole_DateTime(const char *prefix);
	std::string GetTable_Whole_DateTime();

	void SetTable_HisMindata(const char * instrumentid,const char*date);
	std::string GetTable_HisMindata();
	void SetDataBase_HisMindata(const char * db);
	std::string GetDataBase_HisMindata();

	void SetStrategy(const char * pstrategy);
	std::string GetStrategy();

	void SetYear(const char * pyear);
	std::string GetYear();

	void SetProductID(const char * pproductid);
	std::string  GetProductID();

	void SetInstrumentID(const char * pinstrumentid);
	std::string  GetInstrumentID();

	void SetMainInstrumentID(const char * pinstrumentid);
	std::string  GetMainInstrumentID();

	void SetTableRecord(const char * pproductid);
	std::string  GetTableRecord();

	std::string  GetTableReport();

	void SetCusor_DaySqlTime(const char * time);
	std::string GetCusor_DaySqlTime();

	void SetCusor_MinuteTime(const char * time);
	std::string GetCusor_MinuteTime();


	void SetOpenFlag(bool flag);
	bool GetOpenFlag();


	void SetCloseFlag(bool flag);
	bool GetCloseFlag();
	void SetVolumeMultiple(int  vm);
	int GetVolumeMultiple();

	void SetMaxUpDownLimit(double  rate);
	double GetMaxUpDownLimit();

	void SetMargin(double  margin);
	double GetMargin();

	 void SetStopRatio(double  rate);
	double GetStopRatio();
	void SetTodayHigh(double high);
	double  GetTodayHigh();
	void SetTodayLow(double low);
	double  GetTodayLow();

	void SetStopRange(double range);
	double  GetStopRange();

	void SetK(double k);
	double  GetK();

	void SetN(int n);
	int  GetN();

	void SetActiveCount(int n);
	int  GetActiveCount();

	void SetWinRatio(double range);
	double  GetWinRatio();

//	MysqlMaincontract *GetMysqlMaincontract();
//	MysqlTestRecord * GetMysqlTestRecord();
//	MysqlDayLine *GetMysqlDayLine();
//	MysqlHisMinData *GetMysqlHisMinData();

	void  CheckDateTimeSequence();

public:
	MysqlMaincontract * m_pmysqlmaincontract=nullptr;
	MysqlTestRecord * m_pmysqltestrecord=nullptr;
	MysqlDayLine * m_mysqldayline=nullptr;
	MysqlHisMinData * m_pmysqlhismindata=nullptr;
private:


	int _opentime_mode=0;   // 0, 盘中开仓   1,尾盘开仓
	int _closetime_mode=0; //0,盘中平仓 1,尾盘平仓
	std::string _year;
	std::string _strategy;

	std::string _table_record_prefix;
	std::string _path;
	std::string _testrecord_filename;
	std::string _testreport_filename;

	std::string _productid;
	std::string _instrumentid;
	std::string _main_instrumentid;
	std::string _table_record;

	std::string _table_year_datetime;
	std::string _table_whole_datetime;

	std::string _table_report;
	std::string _table_hismindata;
	std::string _database_hismindata;

	std::string _cusor_day_sqltime;
	std::string _cusor_minutetime;

	int _volumemutiple=0;
	double _margin=0.0;
	double _maxupdownlimit=0.05;
	double _todayhigh_minline=0.0;
	double _todaylow_minline=0.0;
	bool _openflag=false;
	bool _closeflag=false;

	double _ratio=0;
	double _stop_range=0;
	double _wintatio=0;
	double _k=0; //波动系数  近期波动变大时,K 变小
	int _n=0;
	int _activecount=0;
	double _singlemarginrate=0; //单品种开仓占用保证金比例


	double _InitialMoney=0;
	double	_RiskRate=0;
	double _limitsedimientary=0;  // 沉淀资金最低限制
};
#endif /* _TESTBASE_HPP */
