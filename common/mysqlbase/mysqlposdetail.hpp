#ifndef _MYSQLPOSDETAIL_HPP
#define _MYSQLPOSDETAIL_HPP

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

class MysqlPosDetail: public MysqlBase {

public:
	MysqlPosDetail(const char * userid);

	void CreateTable();
	void Insert(PositionDetailData data);
	bool Exist_PositionbyInstrumentid(const char* pinstrumentid,
			const char *tradeid, const char *opendate);
	bool Exist_DatabyInstrumentid(const char* pinstrumentid);
	bool Exist_HoldbyInstrumentid(HoldData holddata);

	int Count_DetailList(const char* pinstrumentid, const char *tradingday);
	void Find_AllList(vector<PositionDetailData> &temp);
	void Find_CloseLists(vector<PositionDetailData> &temp);
	HoldData CollectHoldPositionbyOpenDate(const char * pinstrumentid,
			const char *opendate, int direction);
	double CollectOpenAveragePrice(const char * pinstrumentid);
	void Find_InstrumentIDLists(vector<string> &lists_instrumentid);
	HoldData CollectHoldPosition(const char * pinstrumentid, int direction);

private:

};

#endif

