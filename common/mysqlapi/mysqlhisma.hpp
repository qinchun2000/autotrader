
#ifndef _MYSQLHISMA_HPP
#define _MYSQLHISMA_HPP

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

#include <boost/regex.hpp>
#include <json/json.h>


#include "codeutil.hpp"
#include "dateutil.hpp"
#include "mysqldata.hpp"
#include "mysqlbase.hpp"

using namespace std;
using namespace sql;



class MysqlHisMA:public MysqlBase
{

public:
	MysqlHisMA();

	void CreateTable(const char *tablename);
	void Insert(const char *pinstrumentid,HisMAData data);
	std::string GetLastDateTime(const char *tablename);
	bool Exist_DateTime(const char *tablename,std::string datetime);
	HisMAData  Find_LastHisMADatabyOffset(const char *tablename,int offset);
	HisMAData  Find_HisMADatabyDateTime_Offset(const char *tablename,const char* datetime,int offset);



private:


};


#endif

