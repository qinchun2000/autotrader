
#ifndef _MYSQLSEDIMENTARY_HPP
#define _MYSQLSEDIMENTARY_HPP

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
//#include "depthmarket.hpp"
#include "mysqldata.hpp"
#include <json/json.h>
//#include "mongoproductclass.hpp"

#include "mysqlbase.hpp"

using namespace std;
using namespace sql;



class MysqlSedimentary:public MysqlBase
{

public:
	MysqlSedimentary();


  std::string GetLastDateTime(const char *tablename);
  void CreateTable(const char*tablename);
  void Insert(const char* tablename,SedimentaryData data);


private:


};


#endif

