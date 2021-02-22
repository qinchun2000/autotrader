#ifndef _MYSQLSIGNAL_HPP
#define _MYSQLSIGNAL_HPP

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
//#include "depthmarket.hpp"
#include "mysqldata.hpp"
#include <json/json.h>
//#include "mongoproductclass.hpp"

#include "mysqlbase.hpp"

using namespace std;
using namespace sql;

class MysqlSignal: public MysqlBase {

public:
	MysqlSignal(const char* userid);

	void CreateTable();
	void Insert(SignalAssessData data);
	void Update(SignalAssessData data);
	bool Exist_DatabyTradingDay(const char* date);
	void Find_AllList(vector<SignalAssessData> &temp);
	void Find_AllListbyTradingDay(vector<SignalAssessData> &temp,const char * tradingday);
	SignalAssessData Find_LastDatabyTradingDay(const char* tradingday);
	SignalAssessData Find_MaxSignalNumberbyTradingDay(const char* tradingday);

private:

};

#endif

