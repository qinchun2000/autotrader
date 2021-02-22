/*
 * mysqlprostatis.hpp
 *
 *  Created on: 2020年1月19日
 *      Author: qc
 */

#ifndef MYSQLPROSTATIS_HPP_
#define MYSQLPROSTATIS_HPP_


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

class ProductStatisticsData
{
public:
	char ProductID[20];
	char InstrumentID[40];
	char Time[20];
	int Year;
	double Sedimentary;   // 总沉淀资金

};


class MysqlProStatis:public MysqlBase
{

public:
	MysqlProStatis();


  void CreateTable(const char* tablename);
  void Insert(const char* tablename,ProductStatisticsData data);


  bool Exist_DatabyMinuteTime(const char* tablename,const char *minutetime);
  ProductStatisticsData  Find_DatabyProductID(const char* tablename,const char* productid);
  void Find_AllLists(vector<ProductStatisticsData> &temp,const char* tablename);
  ProductStatisticsData Find_MaxSedimentary(const char *tablename);
  ProductStatisticsData Find_MinSedimentary(const char *tablename);
  double GetAverageSedimentary(const char * tablename);

private:


};


#endif /* MYSQLPROSTATIS_HPP_ */
