#ifndef _MYSQLGLOBALVIEW_HPP
#define _MYSQLGLOBALVIEW_HPP

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

class MysqlGlobalView: public MysqlBase {

public:
	MysqlGlobalView(const char* userid);

	void CreateTable();
	void Insert(GlobalViewData data);
	void Insert(GlobalViewData *data);
	void Update(GlobalViewData data);
	bool Exist_DataByTime(const char* time);
	bool Exist_DataByTradingDay(const char* tradingday) ;
	GlobalViewData Find_LastData();
	GlobalViewData Find_LastDatabyTradingDay(const char * tradingday);
	void Find_ListsbyTradingDay(vector<GlobalViewData> &temp,
			const char * tradingday);
	void Find_ListsbyTradingDayReason(vector<GlobalViewData> &temp,
			const char * tradingday,int reason);

private:

};

#endif   //_MYSQLGLOBALVIEW_HPP

