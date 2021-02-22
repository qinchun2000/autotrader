#ifndef _MYSQLUSER_HPP
#define _MYSQLUSER_HPP

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

class MysqlUser: public MysqlBase {

public:
	MysqlUser();

	void CreateTable();
	void Insert(User data);
	void Update(User data);
	void UpdatebyStrategyName(User data) ;
	bool Exist_ActiveDataByUserID(const char *user);
	bool Exist_DatabyUserID(const char *user);
	bool Exist_ActiveDataByStrategyName(const char *strategyname);
	bool Exist_ActiveData();

	bool Exist_DataByStrategyName(const char *strategyname);

	User Find_DatabyUserID(const char *user);
	User Find_ActiveDatabyStrategyName(const char* strategyname);
	User Find_DatabyStrategyName(const char* strategyname);
	User Find_ActiveData();
	void Find_AllList(vector<User> &temp);
	void Find_AllActiveListbyStrategyName(vector<User> &temp);

private:

};

#endif

