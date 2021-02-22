
#ifndef _MYSQLHISMINDATA_HPP
#define _MYSQLHISMINDATA_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <string>
#include <memory>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"

#include "codeutil.hpp"
#include "dateutil.hpp"
#include "csvfile.hpp"
#include "ticktoklineutil.hpp"

#include <boost/regex.hpp>
#include "depthmarket.hpp"
#include <json/json.h>
#include "mysqldata.hpp"
#include "mysqlcycle.hpp"
#include "mysqlhisma.hpp"
#include "mysqlbase.hpp"

using namespace std;
using namespace sql;



class MysqlHisMinData:public MysqlBase
{

public:
	MysqlHisMinData();

	void CreateTable(const char *tablename);
	void Insert(const char *pinstrumentid,MinuteLineData data);
	void Update(const char* tablename,MinuteLineData data);
	void DeletebyMinuteTime(const char* tablename,const char* sqltime);
	std::string GetHxUrlbyid(const char * pinstrumentid,const char *exchangeid,const char *  date);
	void Check_HxMinuteLineData(const char * pinstrumentid,const char *exchangeid,const char *  date);
	void Check_HxMinuteLineDatabyMinutelineCount(const char * pinstrumentid,const char *exchangeid,const char *  date,int min_count);

	void ParseMinuteLineData(const char * pinstrumentid,const char *date,std::string response);
	int ParseMinuteLineArryLists(const char * pinstrumentid,const char *date,std::string response,vector<MinuteLineData> &temp);
	int GetMinuteLineDataLength(const char * pinstrumentid,const char *date,std::string response);

	bool Exist_MinuteTime(const char *tablename,std::string minutetime);
	bool Exist_DataBySqlTime(const char *tablename,const char* minutetime);
	int  CountbyAll(const char* tablename);


	void Find_ListsbyStartEnd(vector<MinuteLineData> &temp,const char *tablename,const char* starttime,const char* endtime);
	void Find_ListsNbyMinuteTime(vector<MinuteLineData> &temp,const char *tablename,const char* minutetime,int n);
	void Find_PriceListsN(vector<double> &temp,const char *tablename,int n);
	void Find_PriceListsNbyMinuteTime(vector<double> &temp,const char *tablename,const char* minutetime,int n);
	void Find_ListsbyMinuteTime(vector<MinuteLineData> &temp,const char *tablename,const char* minutetime);
	void Find_AllList(vector<MinuteLineData> &temp,const char *tablename);
//	void Find_AllDateList(vector<std::string > &temp,const char *tablename);
	MinuteLineData Find_DatabyMinuteTime(const char *tablename,const char* mintime);
	MinuteLineData Find_ExistDatabyMinuteTime(const char *tablename,const char* mintime);
	MinuteLineData Find_PreDatabyMinuteTime(const char *tablename,const char* mintime);
	MinuteLineData Find_PreNDatabyMinuteTime(const char *tablename,const char* mintime,const int n);
	MinuteLineData Find_LastData(const char *tablename);

	int Count_MinuteTables(const char *pinstrumentid);
	MinuteLineData Find_MaxVolume(const char *tablename);
	MinuteLineData Find_MaxVolumebyMinuteTime(const char *tablename,const char* mintime);
	MinuteLineData Find_MaxVolumeEndbyMinuteTime(const char *tablename,const char* mintime);
	int GetStartOpenInterest(const char *tablename);
	int GetLastOpenInterest(const char *tablename);

	int GetVolumeSum(const char *tablename);
	int GetLast3MinVolumeSum(const char *tablename,const char*mintime);
	int GetLastMinVolumeSum(const char *tablename,const char*mintime);
	double GetMAbyMinuteTime(const char *tablename,const int n,const char*mintime);


	MinuteLineData GetLastMinbyMinuteTime(const char *tablename,const char*mintime);

	double GetSettlePrice(const char *tablename,const int vm);
	double GetAveragePricebyMinuteTime(const char *tablename,const int vm,const char*mintime);
	double GetAverageVolumeNbyMinuteTime(const char *tablename,const int n,const char*mintime);
	double GetAveragePrice(const char *tablename);
	MinuteStaticData GetMinuteStaticDatabyMinteTime(const char *tablename,int vm,const char* mintime);
	CycleNRefData  GetCycleNRefDatabyMinteTime(const char *tablename,const char* mintime);
	CycleNRefData  GetCycleNRefDatabyNMinteTime(const char *tablename,const char* mintime, int n);
	double GetAveragePriceByTradingDay(const char *instrumentid,const char *tradingday);


private:


};


#endif

