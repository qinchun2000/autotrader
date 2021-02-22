#ifndef _MYSQLEXCHANGE_HPP
#define _MYSQLEXCHANGE_HPP

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

#include "codeutil.hpp"
#include "dateutil.hpp"
#include <boost/regex.hpp>
#include "mysqldata.hpp"
#include "mysqlbase.hpp"

#include <json/json.h>
//#include "mongoproductclass.hpp"

using namespace std;
using namespace sql;

class MysqlExchange: public MysqlBase {

public:
	MysqlExchange();

	void CreateTable();
	void Insert(ExchangeData data);
	bool Exist_DatabyExchangeID(const char *exchangeid);
	ExchangeData Find_DatabyExchangeID(const char *exchangeid);
	void Find_AllList(vector<ExchangeData> &temp);

private:

};

#endif

