#ifndef _MYSQLSUBSCRIBE_HPP
#define _MYSQLSUBSCRIBE_HPP

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

class MysqlSubscribe: public MysqlBase {

public:
	MysqlSubscribe(const char* userid);

	void CreateTable();
	void Insert(SubscribeData data);
	void Update(SubscribeData data);
	void DeleteAllbyProcessor(const char* processor);
	bool Exist_DatabyInstrumentID(const char *pinstrumentid);
	void Find_AllList(vector<SubscribeData> &temp);
	void Find_AllListbyProcessor(vector<SubscribeData> &temp,
			const char* processor);
	void Find_AllListbyUser(vector<SubscribeData> &temp, const char* user);

private:

};

#endif

