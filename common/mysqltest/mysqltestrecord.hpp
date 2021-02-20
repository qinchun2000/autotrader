
#ifndef _MYSQLTESTRECORD_HPP
#define _MYSQLTESTRECORD_HPP

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

#include "mysqldata.hpp"
#include "mysqlbase.hpp"

#include <json/json.h>


using namespace std;
using namespace sql;



class MysqlTestRecord:public MysqlBase
{

public:
	MysqlTestRecord();


  void CreateTable(const char* tablename);
  int CountTables();
  void DropAllTables();
  void DropPrefixTables(const char* prefix);

  void Insert(const char* tablename,TestRecordData data);
  void InsertTotalTable(const char* tablename,TestRecordData data);
  void Update(const char* tablename,TestRecordData data);

  void UpdateRef(const char* tablename,TestRecordData data);
  void UpdatebyInstrument(const char* tablename,const char* pinstrumentid,TestRecordData data);
  void UpdatebyCloseInstrument(const char* tablename,const char* pinstrumentid,TestRecordData *data);

  void UpdateByStatus(const char* tablename,TestRecordData data,int status);
  void UpdateLowestByStatus(const char* tablename,double lowest,int status);
  void UpdateHighestByStatus(const char* tablename,double highest,int status);
  void UpdateLowestByInstrumentStatus(const char* tablename,const char* pinstrumentid,double lowest,int status);
  void UpdateHighestByInstrumentStatus(const char* tablename,const char* pinstrumentid,double highest,int status);
  void UpdateProfit(const char* tablename,TestRecordData data);
  void UpdateProfitbyOpenDate(const char* tablename,TestRecordData data);

  void UpdateLowStopPriceByInstrumentStatus(const char* tablename,
		  const char* pinstrumentid,double lowest,double stopprice,int status);
  void UpdateHighStopPriceByInstrumentStatus(const char* tablename,
		  const char* pinstrumentid,double highest,double stopprice,int status);

  void UpdateLowStopWinPriceByInstrumentStatus(const char* tablename,
  		  const char* pinstrumentid,double lowest,double stopprice,int status);
    void UpdateHighStopWinPriceByInstrumentStatus(const char* tablename,
  		  const char* pinstrumentid,double highest,double stopprice,int status);


  bool Exist_RecordByOpenDateTime(const char* tablename,const char *opendatetime);
  bool Exist_RecordByStatus(const char* tablename,int status);
  bool Exist_RecordByInstrumentStatus(const char* tablename,const char*pinstrumentid,int status);
  bool  Exist_RecordByOpenDateInstrument(const char* tablename,const char*opendate,const char*pinstrumentid);

  int Count_RecordByStatus(const char* tablename,int status);
  int Count_RecordBySectionStatus(const char* tablename,const char*section,int status);
  int Count_RecordByTradingDaySection(const char* tablename,const char*section,const char* tradingday);

  int  Count_RecordBySection(const char* tablename,const char*section);
  int  Count_WinRecordBySection(const char* tablename,const char*section);
  int  Count_LoseRecordBySection(const char* tablename,const char*section);
  double  SumProfit_RecordBySection(const char* tablename,const char*section);

  TestRecordData  Find_LastRecordByStatus(const char* tablename,int status);
  TestRecordData  Find_RecordByStatus(const char* tablename,int status);
  TestRecordData  Find_RecordByInstrumentStatus(const char* tablename,const char*pinstrumentid,int status);
  void Find_AllLists(vector<TestRecordData> &temp,const char* tablename);
  void Find_AllListsbyStatus(vector<TestRecordData> &temp,const char* tablename);
  void Find_HoldLists(vector<TestRecordData> &temp,const char* tablename);



private:


};


#endif

