
#ifndef _MYSQLCOMMISSION_HPP
#define _MYSQLCOMMISSION_HPP

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

class Commission
{
public:
	char ProductID[10];
	char ExchangeID[10];
	char Mode[10];

	double OpenAccount;
	double OpenNumber;
	double CloseAccount;
	double CloseNumber;
//
	double TodayCloseAccount;
	double TodayCloseNumber;
	double ChangeAccount;
	double ChangeNumber;
	double Margin;
	int VolumeMultiple;
};


class MysqlCommission:public MysqlBase
{

public:
	MysqlCommission();

  void CreateTable();
  void Insert(Commission data);
  bool Exist_DatabyProductID(const char *productid);
  Commission Find_DatabyProductID(const char *productid);
  void Find_AllList(vector<Commission> &temp);

private:

};


#endif

