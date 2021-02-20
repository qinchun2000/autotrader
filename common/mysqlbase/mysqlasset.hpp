#ifndef _MYSQLASSET_HPP
#define _MYSQLASSET_HPP

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

class MysqlAsset: public MysqlBase {

public:
	MysqlAsset();
	MysqlAsset(const char* userid);

	void CreateTable();
	void Insert(AssetData data);
	void Update(AssetData data);
	void UpdateTotalCapital(double totalcapital, const char *tradingday);

	bool Exist_DatabyDate(const char *date);
	AssetData Find_DatabyDate(const char *date);
	AssetData Find_LastDayAsset();
	void Find_AllList(vector<AssetData> &temp);

private:

};

#endif

