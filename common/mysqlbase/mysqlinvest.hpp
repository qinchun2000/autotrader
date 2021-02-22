#ifndef _MYSQLINVEST_HPP
#define _MYSQLINVEST_HPP

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

class MysqlInvestPosition: public MysqlBase {

public:
	MysqlInvestPosition(const char* userid);

	void CreateTable();
	void Insert(InvestPositionData data);
	bool Exist_DatabyInstrumentID(const char* pinstrumentid);
	int Count_DetailList(const char* pinstrumentid, const char *tradingday);
	int Count_OpenNumber(const char *tradingday);
	int Count_OperatedNumber(const char *tradingday);

	void Find_AllLists(vector<InvestPositionData> &temp);
	HoldData CollectHoldPosition(const char * pinstrumentid,
			const char *opendate, int direction);
	int GetPositionbyIns(const char * pinstrumentid);
	int GetPositionbyInsDirection(const char * pinstrumentid,const int direction);

private:

};

#endif

