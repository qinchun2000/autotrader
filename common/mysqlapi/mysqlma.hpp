
#ifndef _MYSQLMA_HPP
#define _MYSQLMA_HPP

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
#include "depthmarket.hpp"
#include "mysqldata.hpp"
#include <json/json.h>
#include "mysqldayline.hpp"
#include "mysqlbase.hpp"

using namespace std;
using namespace sql;



class MysqlMA:public MysqlBase
{

public:
	MysqlMA();

	void CreateTable(const char *tablename);
	void Insert(const char *pinstrumentid,MAData data);

	std::string GetLastDateTime(const char *tablename);
	bool Exist_DateTime(const char *tablename,std::string datetime);
	MAData  Find_LastMAData(const char *tablename);
	MAData  Find_MADatabyTime(const char *tablename,std::string datetime);

//	MAData Check_DatabyDate(const char* pinstrument,const char* date);



private:

};


#endif

