
#ifndef _MYSQLTESTSIGNAL_HPP
#define _MYSQLTESTSIGNAL_HPP

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



class MysqlTestSignal:public MysqlBase
{

public:
	MysqlTestSignal();
	void CreateTable();
	void Insert(Signal data);
	void Update(Signal data);

	bool  Exist_DataBySignalName(const char* signalname);
	bool  Exist_ActiveDataBySignalName(const char* signalname);
	bool  Exist_Long_ActiveDataBySignalName(const char* signalname);
	bool  Exist_Short_ActiveDataBySignalName(const char* signalname);

	bool  Exist_Long_UnActiveDataBySignalNameTradingDay(const char* signalname,const char* tradingday);
	bool  Exist_Short_UnActiveDataBySignalNameTradingDay(const char* signalname,const char* tradingday);

	Signal  Find_DataBySignalName(const char* signalname);
	Signal  Find_ActiveDataBySignalNameDirection(const char* signalname,int direction);
	Signal  Find_UnActiveDataBySignalNameDirectionTradingDay(const char* signalname,int direction,const char* tradingday);

	void Find_AllLists(vector<Signal> &temp);
	void Find_AllListsbyTradingDay(vector<Signal> &temp,const char* tradingday);
	void Find_AllListsActivebyTradingDay(vector<Signal> &temp,const char* tradingday);
	void Find_AllListsbySection(vector<Signal> &temp,const char* section);
	void Find_AllListsActivebySection(vector<Signal> &temp,const char* section);
	void Find_Long_AllListsActivebySection(vector<Signal> &temp,const char* section);
	void Find_Short_AllListsActivebySection(vector<Signal> &temp,const char* section);

	int  Count_CurrentDay_Long_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section);
	int  Count_CurrentDay_Short_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section);

	void Find_CurrentDay_Long_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section);
	void Find_CurrentDay_Short_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section);

	double Sum_ActiveUpRatebySection(const char*tradingday,const char * section);

	Signal Find_BigSedSignalbySection(const char*tradingday,const char * section);
	Signal Find_Long_BigSedSignalbySection(const char*tradingday,const char * section);
	Signal Find_Short_BigSedSignalbySection(const char*tradingday,const char * section);

	Signal Find_Long_BigUpRateSignalbySection(const char*tradingday,const char * section);
	Signal Find_Short_BigUpRateSignalbySection(const char*tradingday,const char * section);

	Signal Find_Up_CloseOpenRateSignalbySection(const char*tradingday,const char * section);
	Signal Find_Down_CloseOpenRateSignalbySection(const char*tradingday,const char * section);

private:


};


#endif

