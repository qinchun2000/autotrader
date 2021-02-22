
#ifndef _MYSQLTESTREPORT_HPP
#define _MYSQLTESTREPORT_HPP

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

class TestReportData
{
public:
	char Product[20];
	char ExchangeID[10];
	char Section[20];
	char Strategy[20];
	char StartTestTime[20];
	char EndTestTime[20];

	int TradeCount;
	int WinCount;

	int LongCount;
	int ShortCount;
	int WinCount_Long;
	int WinCount_Short;

	double InitialMoney;
	double TotalWinProfit;
	double TotalLoseProfit;
	double AverageWinProfit;
	double AverageLoseProfit;
	double Asset;
	double HighestAsset;    //历史最高收益
	double BackDownRate;  //最大资金回撤
	double WinRate;   // 胜率
	double WinLoseRate;   // 盈亏比
	double Expectation;
	double Commission;
	int ErrorMinuteData;
};


class MysqlTestReport:public MysqlBase
{

public:
	MysqlTestReport();


  void CreateTable(const char* tablename);
  void Insert(const char* tablename,TestReportData data);
  void Update(const char* tablename,TestReportData data);

  void InsertErrorData(const char* tablename,TestReportData data);
  void UpdateErrorData(const char* tablename,TestReportData data);

  bool  Exist_DatabyProductID(const char* tablename,const char *productid);
  TestReportData  Find_DatabyProductID(const char* tablename,const char* productid);
  void Find_AllLists(vector<TestReportData> &temp,const char* tablename);
  void Find_ListsOrderbyExp(vector<TestReportData> &temp,const char* tablename);


private:


};


#endif

