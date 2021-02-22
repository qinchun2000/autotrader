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
#include "trendtradetest.hpp"

#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef _SECTIONTRENDTRADETEST_HPP
#define _SECTIONTRENDTRADETEST_HPP

class SectionTrendTradeTest:public TrendTradeTest
{

public:

	SectionTrendTradeTest(const char* pstrategy,const char* year);
	void SetSectionYear(const char* year);
	std::string GetSectionYear();


	void Check_SectionContractMinuteTime(const char* section);
	void Check_SectionUpDownRate(const char* section);

	HxDayLineData Collect_MainContractbyDateTime(vector<std::string> &array,const char* datetime);
	void Collect_MainContractsbySection(vector<std::string> &array,const char * section,const char* datetime);



	void TrendTradebySection(const char* section);
	void SectionSingleIns_Trade(const char* section,vector<std::string> &array);

	void SectionMainContract_MinuteLineOpen(const char*section,
			const char *pinstrumentid,
			const char *datetime,
			SqlAveragePriceData predata,
			double openprice,
			CycleData highdata,
			CycleData lowdata,
			CycleData cycledata);

	void  Create_TestRecordXlsFilebySection(std::string pathfile,const char*section);

private:
	std::string _section_year;
	std::string _section_strategy;


};
#endif /* _SECTIONTRENDTRADETEST_HPP */
