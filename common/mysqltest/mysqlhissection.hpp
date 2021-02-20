
#ifndef _MYSQLHISSECTION_HPP
#define _MYSQLHISSECTION_HPP

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

class HisSectionData
{
public:

	char MinuteTime[20];
	char MaxInstrumentID[10];
	char MinInstrumentID[10];
	int UpCount;
	int DownCount;
	int ZeroCount;
	double Maxrate;
	double Minrate;
	double TotalUprate;
	double TotalDownrate;
	double AverageUpDownrate;
	double CountUprate;
	double CountDownrate;
	double Sedimentary;   // 板块总沉淀资金
	double SedimentaryIO;  //板块流入流出资金
	double SedimentaryIORate;  // 板块流入流出资金比例
};


class MysqlHisSection:public MysqlBase
{

public:
	MysqlHisSection();


  void CreateTable(const char* tablename);
  void Insert(const char* tablename,HisSectionData data);


  bool Exist_DatabyMinuteTime(const char* tablename,const char *minutetime);
  HisSectionData  Find_DatabyMinuteTime(const char* tablename,const char* minutetime);
  void Find_AllLists(vector<HisSectionData> &temp,const char* tablename);



private:


};


#endif

