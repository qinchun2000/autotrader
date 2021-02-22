
#ifndef _MYSQLHOLD_HPP
#define _MYSQLHOLD_HPP

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
#include <json/json.h>

#include "mysqlbase.hpp"
using namespace std;
using namespace sql;





class MysqlHold:public MysqlBase
{

public:
	MysqlHold(const char* userid);


  void CreateTable();
  void Insert(HoldData data);
  void InsertbyPositionDetail(HoldData data);

  void Update(HoldData data);
  void UpdateHighest_StopPrice(HoldData data);
  void UpdateLowest_StopPrice(HoldData data);

  bool Exist_HoldPositionbyInstrumentid(const char* pinstrumentid,const char *opendate,int direction);
  HoldData  Find_DatabyInstrumentid(const char* pinstrumentid,const char *opendate,int direction);
  void  Delete_DatabyInstrumentid(const char* pinstrumentid,const char *opendate,int direction);

  void Find_AllList(vector<HoldData> &temp);
  void Find_AllListOrderbyDate(vector<HoldData> &temp);
  void Find_AllTradingDayList(vector<HoldData> &temp,const char * tradingday);

  int CollectPositions_byInstrumentID(const char * pinstrumentid,int direction);
  int Count_AllListsbySection(const char* section);
  int CountbyAll();

private:


};


#endif

