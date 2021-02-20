#ifndef _TRADESYSTEM_HPP
#define _TRADESYSTEM_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
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

// 线程控制相关
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "httpbase.hpp"
#include "tradespi.hpp"

#include "mysqlaveprice.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlorder.hpp"
#include "mysqldata.hpp"
#include "mysqlposdetail.hpp"
#include "mysqlinvest.hpp"
#include "mysqlhold.hpp"
#include "mysqlutil.hpp"
#include "mysqluser.hpp"
#include "mysqlsection.hpp"
#include "mysqlsecassess.hpp"

#include "redismd.hpp"
#include "redisstatus.hpp"
#include "redissignal.hpp"
#include "redissection.hpp"
#include "dateutil.hpp"

#include "logutil.hpp"
#include "lock.hpp"
#include <boost/regex.hpp>
#include <json/json.h>
#include "config.hpp"
#include "strategytrend.hpp"
#include "staday.hpp"
#include "strategyspread.hpp"
#include "trendtrader.hpp"

#include "logbase.hpp"
#include "threadbase.hpp"
#include "secassess.hpp"
#include "zeromqutil.hpp"


using namespace std;
using namespace log4cplus;


class TradeSystem:public LogBase
{

public:

	TradeSystem();

	void SetCThostFtdcTraderApi(CThostFtdcTraderApi *ptraderapi);
	CThostFtdcTraderApi * GetCThostFtdcTraderApi();
	void SetCTraderSpi(CTraderSpi *ptraderspi);
	CTraderSpi * GetCTraderSpi();
	void SetTrader(TrendTrader *ptrader);
	TrendTrader * GetTrader();
	void SetSecAssess(SecAssess *psecassess);
	SecAssess * GetSecAssess();


	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();

	void SetStrategy(Strategy *pstrategy);
	Strategy * GetStrategy();
	void SetAvalible(double ava);
	double  GetAvalible();

	void DropSqlTable();
	void InitialSqlTable();
	void InitialFlag();
	double GetCurrentTime();
	void LocalLogin();
	void UpdateFlag();
	bool IsLoginAvalibleTime();
	bool IsTradeAvalibleTime();
	bool IsHoliday();
	bool IsWeekEnd();
	bool IsLoginOK();

	bool ConnectRestore();
	bool OpenMarketLoginOK();
	void ReadCommissionData();
	void InitialSectionTable();
	void InitialSecAssessTable(const char* userid);

	void Run();
	void CheckAllHttpDayLine();

	std::string  CheckFlowPath();
	void SetLogProperty(const char* userid);
	void TradeInitial();
	void Fun_ConnectError();
	void Fun_ReadyLogin();
	void Fun_TradeLimit();
	void Fun_Trade_FlagOrderInsertCheck();
	void Fun_Trade_HoldCheck();
	void Fun_Trade_OpenCheck();

	void ThreadRun_DayTrade(const char* staname);
	void Run_DayTrade(const char* staname);
	void Run_CalendarSpreadTrade_zlqh();
	void Run_Trade_zlqh(const char* staname);
	void Run_TrendTrade(const char*systemname);

	void Run_Trade(const char* staname);

	void AutoTask_AfterNoonClosebyUser();
	void AutoTask_BeforMorningbyUser();
	void AutoTask_BeforNightbyUser();
	void AutoTaskEveryDay();
	void AutoTask();

private:
	std::string _strategyName;
//	std::string _log_pathfilename;
	CThostFtdcTraderApi* _pTraderApi=NULL;
	CTraderSpi* _pTraderSpi=NULL;
	TrendTrader * _pTrader=NULL;
	Strategy *_pstrategy=NULL;
	SecAssess *_psecassess=NULL;


	RedisDepthMarket redisdepthmarket;
	RedisSignal redissignal;
	RedisSection redissection;
	double _avalible=0.0;
	bool _TaskSendMail_Morning_break=false;
	bool _TaskSendMail_Morning=false;
	bool _TaskSendMail_Afternoon=false;
	bool _TaskSendMail_Night=false;

	bool _TaskFlag16 =false;
	bool _TaskFlag=false;
	bool _TaskFlag_EveryDay=false;

	bool _flag_loginmarket=false;  //早盘开盘前10分钟 夜盘开盘前15分钟  包含
	bool _flag_open=false; //早盘休市 时间,即金融板块开盘时间  是包含关系
	bool _flag_openmarket=false;
	bool _flag_holiday=false;
};

#endif /* _TRADESYSTEM_HPP */
