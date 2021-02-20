
#ifndef _MYSQLDAYLINE_HPP
#define _MYSQLDAYLINE_HPP

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
#include <boost/regex.hpp>
//#include "depthmarket.hpp"
#include <json/json.h>

#include "mysqldata.hpp"
#include "mysqlcycle.hpp"
#include "mysqlhisma.hpp"
#include "mysqlbase.hpp"
//#include "mongoproductclass.hpp"

using namespace std;
using namespace sql;

class HighLowest
{
public:
	double highest;
	double lowest;
};

class MysqlDayLine:public MysqlBase
{

public:
	MysqlDayLine();

	void CreateTable(const char *pinstrumentid);
	void Insert(const char *pinstrumentid,HxDayLineData data);
	void Update(const char* tablename,HxDayLineData data);
	void UpdateAllPrice(const char* tablename,HxDayLineData data);

	void InsertDepthmarket(const char* tablename,HxDayLineData data);
	void UpdateDepthmarket(const char* tablename,HxDayLineData data);
	void UpdateAllPriceDepthmarket(const char* tablename,HxDayLineData data);

	std::string GetLastDateTime(const char *tablename);
	bool Exist_DateTime(const char *tablename,std::string datetime);
	bool Exist_DatabySqlTime(const char *tablename,const char * sqltime);


	void Find_UpdateDateList(vector<HxDayLineData> &temp,const char *tablename);
	HxDayLineData Find_DataBySqlTime(const char *tablename,const char* sqltime);
	HxDayLineData Find_PreDataBySqlTime(const char *tablename,const char* sqltime);
	HxDayLineData Find_PreNDataBySqlTime(const char *tablename,const char* sqltime,int n);
	HxDayLineData Find_LastDayLine(const char *tablename);
	HxDayLineData Find_LastDayLineOffset(const char *tablename,int offset);
	void CopyData(HxDayLineData *data,const char *tablename);

	void Find_AllList(vector<HxDayLineData> &temp,const char *tablename);
	void Find_ListsLimit(vector<HxDayLineData> &temp,const char *tablename,int count);
	void Find_ListsbyDate(vector<HxDayLineData> &temp,const char *tablename,const char* date,int count);
	void Find_ListsbyOpenDate(vector<HxDayLineData> &temp,const char *tablename,const char* opendate);


	void Find_FiveDayLineDataLists(vector<FiveDayLineData> &temp,const char *tablename,int count);
	void Find_FiveDayLineDataListsbyDate(vector<FiveDayLineData> &temp,const char *tablename,const char* date,int count);

	HighLowest Collect_CycleDatabyOpenDate(const char *tablename,const char * date);
	void Update_CycleData(const char *tablename);
	CycleData Collect_CycleData(const char *tablename,const char * date);

	CycleData Collect_CycleNData(const char *tablename,const char * date,int n);

	double Collect_TotalMA_NData(const char *tablename,const char * date,int n);

	double EMA(std::vector<double> &X, int N);
//	double  EMAA(std::vector<double> &X, int N);

	double Collect_EMA_NData(const char *tablename,const char * date,int n);
	EMAData Collect_EMAData(const char *tablename,const char * date);
	double Get_EMADatabyMinPrice(const char *tablename,const char * date,int n,double minprice);

	double Get_MADatabyMinPrice(const char *tablename,const char * date,int n,double minprice);
	double Get_OpenClosePercent(const char *tablename,const char * date,int n);

	double Get_ATRDatabyMinPrice(const char *tablename,const char * date,int n,double today_tr);
	double Get_TRndaysbySqlTime(const char *tablename,const char * date,int n);

	double Get_HisMAData(const char *tablename,const char * date,int n);
	double Get_PreHisMAData(const char *tablename,const char * date,int n);
	double Get_PreHisMADatabyOffset(const char *tablename,const char * date,int n,int offset);

	double Get_HisEMAData(const char *tablename,const char * date,int n);

	double Collect_TotalHisMA_NData(const char *tablename,const char * date,int n);
	MAData Collect_TotalMAData(const char *tablename,const char * date);
	HisMAData Collect_TotalHisMAData(const char *tablename,const char * date);
	ATRData Collect_HisATR_NData(const char *tablename,const char * date,int n);
	ATRData Collect_PreHisATR_NData(const char *tablename,const char * date,int n);
	void Update_HisData(const char *tablename);

	double Collect_HisEMA_NData(const char *tablename,const char * date,int n);
	HisEMAData Collect_HisEMAData(const char *tablename,const char * date);
	int  Count_HoldDays(const char* tablename,const char* opendate,const char *holddate);
	int  CountbyAll(const char* tablename);
	void CorrectDayLineByInstrument(const char * pinstrumentid);

private:

	std::string _instrumentid;
	std::string _productid;
	std::string _start_time;
	std::string _end_time;


};


#endif

