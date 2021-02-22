
#ifndef _MYSQLTESTASSET_HPP
#define _MYSQLTESTASSET_HPP

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

class TestAssetData
{
public:
	char ID[20];
	char InstrumentID[20];
	char OpenDateTime[20];
	char CloseDateTime[20];
	int Direction;
	int Status;

	double HighestAsset;
	double BackDownRate;

	int SameTimeHold;
	double UseMarginRate;
	double MaxMargin;
	double MaxMarginRate;

	double Profit;
	double Commission;
	double LastAsset;
	double Sedimentary;

};


class MysqlTestAsset:public MysqlBase
{

public:
	MysqlTestAsset();

  void CreateTable(const char* tablename);
  void Insert(const char* tablename,TestAssetData data);
  void Update(const char* tablename,TestAssetData data);
  void UpdateMaxMargin(const char* tablename,TestAssetData data);
  void Delete_byID(const char* tablename,const char* id);

  double  Get_MaxbyHighestAsset(const char* tablename);
  double  Get_MaxbyBackDownRate(const char* tablename);
  double  Get_MaxbySameTimeHold(const char* tablename);
  double  Get_MaxbyMarginRate(const char* tablename);
  double  Get_SumbyProfit(const char* tablename);
  double  Get_SumbyCommission(const char* tablename);
  int  CountbyAll(const char* tablename);
  int  CountbyWinProfit(const char* tablename);

  bool  Exist_DatabyID(const char* tablename,const char *id);
  TestAssetData  Find_DatabyID(const char* tablename,const char* id);
  void Find_AllLists(vector<TestAssetData> &temp,const char* tablename);
  void Find_AllListsbyCloseTime(vector<TestAssetData> &temp,const char* tablename);
  void Find_SameTimeHoldListbyTime(vector<TestAssetData> &temp,const char* tablename,
  		const char* starttime ,const char* endtime);

  int  CountbySqlTime(const char* tablename,const char* sqltime);
  double  Get_SumUseMarginRatebySqlTime(const char* tablename,const char* sqltime);

private:


};


#endif

