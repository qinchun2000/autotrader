
#ifndef _MYSQLDATETIME_HPP
#define _MYSQLDATETIME_HPP

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

class DateTime
{
public:
	char Time[20];
	char StartTime[20];
	char EndTime[20];
};


class MysqlDateTime:public MysqlBase
{

public:
	MysqlDateTime();


  void CreateTable(const char* tablename);
  void Insert(const char* tablename,DateTime data);


  bool Exist_DateTime(const char* tablename,const char *datetime);
  DateTime  Find_PreDateTime(const char* tablename,const char* datetime);
  DateTime  Find_LastDateTime(const char* tablename);
  void Find_AllLists(vector<DateTime> &temp,const char* tablename);
  void Find_AllListsbyStartSqlTime(vector<DateTime> &temp,const char* tablename,const char* start_sqltime);


private:


};


#endif

