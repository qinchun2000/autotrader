
#ifndef _MYSQLRECORD_HPP
#define _MYSQLRECORD_HPP

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



class MysqlRecord:public MysqlBase
{

public:
	MysqlRecord();


  void CreateTable();
  void Insert(RecordData data);
  bool  Exist_RecordByOpenDateTime(const char* pinstrumentid,const char *opendatetime);
  void Find_AllLists(vector<RecordData> &temp);





private:


};


#endif

