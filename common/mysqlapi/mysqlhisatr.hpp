
#ifndef _MYSQLHISATR_HPP
#define _MYSQLHISATR_HPP

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

#include <json/json.h>
#include "mysqldata.hpp"
#include "mysqlbase.hpp"


using namespace std;
using namespace sql;


class MysqlHisATR:public MysqlBase
{

public:
	MysqlHisATR();


	void CreateTable(const char *pinstrumentid);
	void Insert(const char *pinstrumentid,ATRData data);
	std::string GetLastDateTime(const char *tablename);


	bool Exist_DateTime(const char *tablename,std::string datetime);
	ATRData  Find_DatabyDateTime(const char *tablename,std::string datetime);
	ATRData FindPreDatabyDateTime(const char *tablename,const char * datetime);
	void Find_ListsByTime(vector<ATRData> &temp,const char *tablename,const char * datetime,int number);
	int Count_ListsByTime_N(const char *tablename,const char * datetime,int number);
	int Count2x_ListsByTime_N(const char *tablename,const char * datetime,int number);
	double Indicator_ListsByTime_N(const char *tablename,const char * datetime,int number);



private:


};


#endif

