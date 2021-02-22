  
#ifndef _MYSQLMINDATA_HPP
#define _MYSQLMINDATA_HPP

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
#include "mysqlbase.hpp"
#include "mysqldata.hpp"


using namespace std;
using namespace sql;



class MysqlMinData:public MysqlBase
{ 
  
public:
	MysqlMinData();
	int Count(const char *tablename);
	bool Exist_MinuteTime(const char *tablename,const char* minutetime);
	void Insert(const char *tablename,Mysql_MinData data);
	void Update(const char* tablename,Mysql_MinData data);
	void CreateTable(const char *tablename);
	std::string GetLast_MinuteTime(const char *tablename);
	Mysql_MinData Find_LastDatabyMinuteTime(const char *tablename);
	Mysql_MinData Find_MaxVolume(const char *tablename);
	Mysql_MinData Find_DatabyMinuteTime(const char *tablename,const char* mintime);
	ATRData Get_ATRDataByPreLists(const char *tablename,int n);
	void Find_ListsbyMinuteTime(vector<Mysql_MinData> &temp,const char *tablename,const char* minutetime);
	void Find_ListsNbyMinuteTime(vector<Mysql_MinData> &temp,const char *tablename,const char* minutetime,int n);
	void Find_LastListsbyN(vector<Mysql_MinData> &temp,const char *tablename,int n);
	void Find_AllLists(vector<Mysql_MinData> &temp,const char *tablename);

private:

//	std::string _DATABASE;// "test"
//	std::string _DATABASE;// "test"
//	std::string _TABLENAME;// "test"
};
  
  
#endif     

