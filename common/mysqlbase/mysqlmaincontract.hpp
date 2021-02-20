#ifndef _MYSQLMAINCONTRACT_HPP
#define _MYSQLMAINCONTRACT_HPP

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
#include "mysqlinvest.hpp"
#include "mysqluser.hpp"
#include "mysqlbase.hpp"

#include <json/json.h>
//#include "mongoproductclass.hpp"

using namespace std;
using namespace sql;

class MysqlMaincontract: public MysqlBase {

public:
	MysqlMaincontract();

	void CreateTable();
	void Insert(MainContractData data);

	void UpdateErrorDaylineCount(int count, const char* instrumentid);
	void UpdateErrorDaylineCountNext(int count, const char* instrumentid);

	std::string GetLastLocalUpdateTime();
	std::string GetOldestTradingDay();
	bool Exist_DatabyInstrumentID(const char *pinstrumentid);
	bool Exist_DatabyInstrumentIDNext(const char *pinstrumentid);
	bool Exist_DatabyProductID(const char *productid);
	MainContractData Find_DatabyInstrumentID(const char *pinstrumentid);
	MainContractData Find_DatabyInstrumentIDNext(const char *pinstrumentid);
	MainContractData Find_DatabyProductID(const char *productid);
	void Find_AllList(vector<MainContractData> &temp);
	void Find_TradeSectionList(vector<MainContractData> &temp);
	void Find_CycleNList(vector<MainContractData> &temp);
	void Find_CycleNList_Ndays(vector<MainContractData> &temp, int n);
	void Find_Night_CycleNList_Ndays(vector<MainContractData> &temp, int n);

	void Find_ListsbySection(vector<MainContractData> &temp,
			const char* sectionname);
	void Find_AllListorderbySed(vector<MainContractData> &temp);
	void Find_AllListorderbySedN20(vector<MainContractData> &temp);
	void Find_LimitListbySed_Ndays(
			vector<MainContractData> &temp, int n) ;
	void Find_SpreadListorderbySed_Ndays(vector<MainContractData> &temp, int n);
	void Find_Night_SpreadListorderbySed_Ndays(vector<MainContractData> &temp,
			int n);

	void GetTradeInstrumentLists(vector<std::string> &temp);
	void GetTradeInstrumentListsbyStrategy(vector<std::string> &temp,
			const char * strategy);
	void GetAllInstrumentListsbyStrategy(vector<std::string> &temp,
			const char * strategy);
	void GetTradeInstrumentListsbyMain(vector<std::string> &temp);
	void GetNextInstrumentLists(vector<std::string> &temp);
	void GetAllInstrumentLists(vector<std::string> &temp);
	int CountbyMainDayLine();
	int CountbyNextDayLine();
	void Find_DayLineWrongLists(vector<MainContractData> &temp, int n);
	void Find_NextDayLineWrongLists(vector<MainContractData> &temp, int n);

private:

};

#endif

