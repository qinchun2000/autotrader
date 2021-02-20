#ifndef _MYSQLINSTRUMENTID_HPP
#define _MYSQLINSTRUMENTID_HPP

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

class MysqlInstrument: public MysqlBase {

public:
	MysqlInstrument();

	void CreateTable();
	void Insert(InstrumentData data);
	std::string GetLastLocalUpdateTime();
	InstrumentData FindLastExpireDateInstrument();
	bool Exist_DatabyInstrumentID(const char *pinstrumentid);
	InstrumentData Find_DatabyInstrumentID(const char *pinstrumentid);
	void Find_AllList(vector<InstrumentData> &temp);
	void Find_ListsbyProductID(vector<InstrumentData> &temp,
			const char * productid);

private:

};

#endif

