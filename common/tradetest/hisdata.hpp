#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <regex.h>

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <float.h>

#include "mysqldata.hpp"
#include "mysqldayline.hpp"
#include "mysqltestrecord.hpp"
#include "mysqlcycle.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqlhismindata.hpp"
#include "mysqlutil.hpp"
#include "mysqlhisatr.hpp"
#include "mysqldatetime.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqltestreport.hpp"
#include "mysqlhissection.hpp"
#include "mysqlminutetime.hpp"
#include "ticktoklineutil.hpp"
#include "tradetest.hpp"

#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef _HISDATA_HPP
#define _HISDATA_HPP

class HisData
{

public:
	HisData();
	void SetDatabase(const char* database);
	std::string GetDatabase();

	void DropEmpty_MinuteTable(const char* year);
	void CheckAllCsvData(const char* year);

	void CheckInstrument_Minuteline(const char* pinstrumentid);
	void CheckInstrument_HisMinuteline(const char* pinstrumentid);
	void DropInstrument_MinutelineTable(const char* pinstrumentid);

	void ConvertDayTicks_Minuteline(const char* path,const char*filename,const char* pinstrumentid,const char* sqltime);

	std::string GetPath(const char*pinstrumentid,const char* time);
	std::string GetFileName(const char* pinstrumentid ,const char* sqltime);

private:

	std::string _database_hisminute;
	std::string _table_hisminute;

};
#endif /* _HISDATA_HPP */
