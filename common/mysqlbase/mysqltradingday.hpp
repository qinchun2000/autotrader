#ifndef _MYSQLTRADINGDAY_HPP
#define _MYSQLTRADINGDAY_HPP

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
//#include "mongoproductclass.hpp"

using namespace std;
using namespace sql;

class MysqlTradingDay: public MysqlBase {

public:
	MysqlTradingDay(const char * table);

	void CreateTable();
	void Insert(TradingDayData data);
	void Update(TradingDayData data);
	void UpdateMaxOrderRefbyDate(const char *maxorderref, const char* datetime);
	void UpdateDifSecbyDate(int difsec, const char* date);
	void UpdateSettleInfoConfirmStatusbyDate(TradingDayData *data);
	void UpdateDataReadyStatusbyDate(TradingDayData *data);
	void ResetSettleInfoConfirmStatusbyDate(bool status, const char* date);
	bool Exist_DateTime(const char* datetime);
	bool Exist_TradingDay(const char* date);
	TradingDayData Find_DatabyDate(const char *datetime);
	TradingDayData Find_DatabyTradingDay(const char *date);
	TradingDayData Find_LastTradingDay();

private:

};

#endif

