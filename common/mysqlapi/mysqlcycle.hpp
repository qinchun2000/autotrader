
#ifndef _MYSQLCYCLE_HPP
#define _MYSQLCYCLE_HPP

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


class MysqlCycle:public MysqlBase
{

public:
	MysqlCycle();


	void CreateTable(const char *pinstrumentid);
	void Insert(const char *pinstrumentid,CycleData data);
	std::string GetLastDateTime(const char *tablename);


	bool Exist_DateTime(const char *tablename,std::string datetime);
	CycleData  Find_DatabyDateTime(const char *tablename,std::string datetime);
	void Find_ListsByTime(vector<CycleData> &temp,const char *tablename,const char * datetime,int number);
	void Find_HighListsByTime(vector<CycleData> &temp,const char *tablename,const char * datetime,int number);
	void Find_LowListsByTime(vector<CycleData> &temp,const char *tablename,const char * datetime,int number);
	CycleData GetHighestbyHighCount(const char *tablename,const char* datetime,int number);
	CycleData GetLowestbyLowCount(const char *tablename,const char * datetime,int number);

	int Check_Highcount(const char* tablename,const char * date,double high20);
	int Check_Lowcount(const char* tablename,const char * date,double Low20);




private:


};


#endif

