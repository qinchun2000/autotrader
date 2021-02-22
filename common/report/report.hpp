

#ifndef _REPORT_HPP
#define _REPORT_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <memory.h>
#include <string.h>
#include <string>
#include <math.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

#include "trendtrader.hpp"
#include "tradespi.hpp"
#include "httpbase.hpp"

#include "mysqlaveprice.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlorder.hpp"
#include "mysqldata.hpp"
#include "mysqlposdetail.hpp"
#include "mysqlinvest.hpp"
#include "mysqlutil.hpp"
#include "mysqlhold.hpp"
#include "mysqltestreport.hpp"
#include "mysqlcommission.hpp"
#include "mysqlsection.hpp"
#include "mysqlglobalview.hpp"
#include "mysqlsignal.hpp"

#include "redismd.hpp"
#include "redisstatus.hpp"
//#include "redisorder.hpp"
#include "redissection.hpp"
#include "depthmarket.hpp"
#include "dateutil.hpp"
#include "smtputil.hpp"
#include "logutil.hpp"
#include "lock.hpp"
#include <boost/regex.hpp>
#include <json/json.h>
#include "cping.hpp"
#include <xlslib/xlslib.h>
#include "config.hpp"
#include "logbase.hpp"
#include "strategy.hpp"
#include "threadbase.hpp"
#include "datawrapper.hpp"

using namespace std;
using namespace log4cplus;
using namespace xlslib_core;


class Report:public LogBase
{

public:

	Report(DataWrapper * datawrapper,const char* staname);
	~Report();
	void run();

	void WriteGlobalViewReport(std::string * content);
	void WriteSectionViewReport(std::string * content);
	void WriteSectionSedmentaryViewReport(std::string * content);
	void WriteSignalViewReport(std::string * content);
	void WriteHoldOrderViewReport(std::string  &content,std::string &title);
	void WriteCloseOrderViewReport(std::string  &content,std::string &title);
	void WriteIpTailPart(std::string * content);
	void WriteReportStaticsPart(int n,std::string * content);

	void SendEndReport();
	void SendEndTimeReport();
	void SendWeekEndReport();
	void SendMonthEndReport();
	void SendNDaysReport(const char* startday);
	void SendLastNightReport();
	void SendSelfCheckReport();
	void SendMainContractCheckReport();
	void Create_MaincontractXlsFile(std::string pathfile);
	void Create_TestReportXlsFile(std::string pathfile);
private:
//	LogUtil * m_reportlog=nullptr;
	std::string m_maincontract_path;
	std::string m_userid;
	std::string m_tradingday;
	std::string m_strategyname;


	SmtpUtil * m_smtputil=nullptr;
	RedisDepthMarket m_redisdepthmarket;
	RedisSection m_redissection;

//	TrendTrader *_pTrader=nullptr;
//	Strategy *_pstrategy=nullptr;
	DataWrapper * m_pdatawrapper=nullptr;
};

#endif /* _REPORT_HPP */
