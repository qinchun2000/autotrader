
#ifndef _MYSQLMDLOGIN_HPP
#define _MYSQLMDLOGIN_HPP

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



using namespace std;
using namespace sql;




class MysqlMdLogin:public MysqlBase
{

public:
	MysqlMdLogin();



	void CreateTable();
	bool Exist_DateTime(const char* datetime);
	void Insert(TradingDayData data);

	TradingDayData  Find_LastTradingDay();




private:

	std::string _Tablename;// "test"

};


#endif

