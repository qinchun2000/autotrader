#ifndef _MYSQLDEPTHMARKET_HPP
#define _MYSQLDEPTHMARKET_HPP

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

class MysqlDepthMarket: public MysqlBase {

public:
	MysqlDepthMarket();
	MysqlDepthMarket(const char* tablename);

	void CreateTable();
	void Insert(DepthMarketData data);

	std::string GetLastLocalUpdateTime();
	std::string GetOldestTradingDay();
	bool Exist_DatabyInstrumentID(const char *pinstrumentid);
	DepthMarketData Find_DatabyInstrumentID(const char *pinstrumentid);

	void Find_AllList(vector<DepthMarketData> &temp);
	void Find_ListsbyProductID(vector<DepthMarketData> &temp,
			const char * productid);
	void Find_ListsbyProductIDVolume(vector<DepthMarketData> &temp,
			const char * productid);
	DepthMarketData Find_DatabyMaxOPI(const char *productid);
	DepthMarketData Find_DatabyMaxVolume(const char *productid);
	void Find_MainNextListsbyProductID(vector<DepthMarketData> &temp,
			const char * productid);
	void Find_MainNextItembyProductID(DepthMarketData &a, DepthMarketData &b,
			const char * productid);

private:

};

#endif

