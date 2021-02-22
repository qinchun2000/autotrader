
#ifndef _MYSQLSECASSES_HPP
#define _MYSQLSECASSES_HPP

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
#include <json/json.h>

#include "mysqlbase.hpp"
using namespace std;
using namespace sql;


class MysqlSecAssess:public MysqlBase
{

public:
	MysqlSecAssess(const char* userid);

	void CreateTable();
	void Insert(SectionAssessData data);
	void Update(SectionAssessData data);
	void UpdateProductHoldNumber(int holdnumber,const char* section);


	void UpdateSignalNumber(SectionAssessData data);
	void UpdateLongSectionRecommend(const char* section,int signalnumber,const char*instrumentid,double updownrate);
	void UpdateShortSectionRecommend(const char* section,int signalnumber,const char*instrumentid,double updownrate);

	void UpdateLongProductDaySignalNumber(int signalnumber,const char* section);
	void UpdateShortProductDaySignalNumber(int signalnumber,const char* section);

	void UpdateLongInstrumentId(int signalnumber,const char* section,const char* instrument);
	void UpdateShortInstrumentId(int signalnumber,const char* section,const char* instrument);

	bool  Exist_DataBySection(const char* section);
	SectionAssessData  Find_DataBySection(const char* section);
	void Find_AllLists(vector<SectionAssessData> &temp);
	void Find_OpenLists(vector<SectionAssessData> &temp);
	void Find_Long_AllListsBySingalNumber(vector<SectionAssessData> &temp);
	void Find_Short_AllListsBySingalNumber(vector<SectionAssessData> &temp);

	int  SumLongSignalNumber();
	int  SumShortSignalNumber();
	int  CountLongSignalNumber();
	int  CountShortSignalNumber();
	SectionAssessData Find_MaxShortSignalSection();

private:


};


#endif //_MYSQLSECASSES_HPP

