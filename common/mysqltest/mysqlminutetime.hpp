
#ifndef _MYSQLMINUTETIME_HPP
#define _MYSQLMINUTETIME_HPP

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

class MinuteTimeData
{
public:
	char MinuteTime[20];
	char StartMinuteTime[20];
	char EndMinuteTime[20];
};


class MysqlMinuteTime:public MysqlBase
{

public:
	MysqlMinuteTime();


  void CreateTable(const char* tablename);
  void Insert(const char* tablename,MinuteTimeData data);


  bool Exist_MinuteTime(const char* tablename,const char *datetime);
  MinuteTimeData  Find_LastMinuteTime(const char* tablename);
  void Find_AllLists(vector<MinuteTimeData> &temp,const char* tablename);
  int  CountbyAll(const char* tablename);


private:


};


#endif

