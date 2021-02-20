
#ifndef _MYSQLHISEMA_HPP
#define _MYSQLHISEMA_HPP

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
#include "mysqldayline.hpp"

using namespace std;
using namespace sql;



class MysqlHisEMA:public MysqlBase
{

public:
	MysqlHisEMA();

	void CreateTable(const char *tablename);
	void Insert(const char *pinstrumentid,HisEMAData data);

	std::string GetLastDateTime(const char *tablename);
	bool Exist_DateTime(const char *tablename,std::string datetime);
	HisEMAData  Find_LastHisEMADatabyOffset(const char *tablename,int offset);
	HisEMAData  Find_HisEMADatabyDateTime(const char *tablename,const char* datetime);
	HisEMAData  Find_HisEMADatabyDateTime_Offset(const char *tablename,const char* datetime,int offset);
	HisEMAData  Find_PreHisEMADatabyDateTime(const char *tablename,const char* datetime);
	void Find_ListsbyDate(vector<HisEMAData> &temp,const char *tablename,const char* date,int count);
	double  EMA(const char* pinstrumentid,const char* sqltime, int N);

private:


};


#endif

