
#ifndef _MYSQLAVEPRICE_HPP
#define _MYSQLAVEPRICE_HPP

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


class MysqlAvePrice:public MysqlBase
{

public:
	MysqlAvePrice();

	void CreateTable(const char *tablename);
	void Insert(const char *pinstrumentid,SqlAveragePriceData data);

	std::string GetLastDateTime(const char *tablename);
	bool Exist_DateTime(const char *tablename,std::string datetime);
	SqlAveragePriceData  Find_DateTime(const char *tablename,std::string datetime);
	SqlAveragePriceData  Find_YesterdayDatabyDateTime(const char *tablename,std::string datetime);

	int Check_Highcount(const char* tablename,const char * date,double high20);
	int Check_Lowcount(const char* tablename,const char * date,double Low20);
	int Get_MaxOpenInterest(const char* tablename);




private:


};


#endif

