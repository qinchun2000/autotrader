
#ifndef _MYSQLTESTASSESS_HPP
#define _MYSQLTESTASSESS_HPP

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
#include "redissignal.hpp"


#include <json/json.h>


using namespace std;
using namespace sql;



class MysqlTestSecAssess:public MysqlBase
{

public:
	MysqlTestSecAssess();
	void CreateTable();
	void Insert(SectionAssessData data);
	void Update(SectionAssessData data);

	void UpdateLongProductDaySignalNumber(int signalnumber,const char* section);
	void UpdateShortProductDaySignalNumber(int signalnumber,const char* section);

	bool  Exist_DataBySection(const char* section);
	SectionAssessData  Find_DataBySection(const char* section);
	void Find_AllLists(vector<SectionAssessData> &temp);
	void Find_Long_AllListsBySingalNumber(vector<SectionAssessData> &temp);
	void Find_Short_AllListsBySingalNumber(vector<SectionAssessData> &temp);

private:


};


#endif

