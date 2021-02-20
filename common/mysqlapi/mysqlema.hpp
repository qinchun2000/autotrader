
#ifndef _MYSQLEMA_HPP
#define _MYSQLEMA_HPP

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
#include "mysqlhisema.hpp"

using namespace std;
using namespace sql;



class MysqlEMA:public MysqlBase
{

public:
	MysqlEMA();

	void CreateTable(const char *tablename);
	void Insert(const char *pinstrumentid,EMAData data);

	std::string GetLastDateTime(const char *tablename);
	bool Exist_DateTime(const char *tablename,std::string datetime);
	EMAData  Find_LastEMAData(const char *tablename);
	EMAData  Find_EMADatabyTime(const char *tablename,std::string datetime);
	void Find_ListsbyDate(vector<EMAData> &temp,const char *tablename,const char* date,int count);
	double  EMA(const char* pinstrumentid,const char* sqltime, int N);

private:

};


#endif

