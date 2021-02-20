#ifndef _SYSBASE_HPP
#define _SYSBASE_HPP

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
#include <sys/stat.h>

// 线程控制相关
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

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

#include "trendtrader.hpp"
#include "daytrader.hpp"

#include "logbase.hpp"
#include "threadbase.hpp"
#include "secassess.hpp"
#include "datawrapper.hpp"


using namespace std;
using namespace log4cplus;

class SysBase:public LogBase
{

public:

	SysBase();
	virtual ~SysBase();

	RedisSignal* GetRedisSignal();
	RedisDepthMarket* GetRedisDepthMarket();
	RedisSection* GetRedisSection();

	void SetTrader(DayTrader *ptrader);
	DayTrader * GetTrader();
	void SetSecAssess(SecAssess *psecassess);
	SecAssess * GetSecAssess();

	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();

//	void setStrategy(Strategy *pstrategy);
//	Strategy * getStrategy();

	void SetDataWrapper(DataWrapper *point);
	DataWrapper* GetDataWrapper();

	void SetAvalible(double ava);
	double  GetAvalible();
	void SetUserid(const char* userid);
	std::string GetUserid();
	void SetBaseAccount(bool flag);
	bool GetBaseAccount();

	void InitialFlag();
	double GetCurrentTime();
	bool IsLoginAvalibleTime();
	bool IsTradeAvalibleTime();
	bool IsHoliday();
	bool IsWeekEnd();
	bool IsLoginOK();
	void CheckAllHttpDayLine();

	std::string  CheckFlowPath();
	void SetLogProperty(const char* userid);
	void ReadCommissionData();
	void InitialSectionTable();
	void InitialSecAssessTable(const char* userid);
	void UpdateHoldlist();
	void AutoTask_AfterNoonClosebyUser();
	void AutoTask_BeforMorningbyUser();
	void AutoTask_BeforNightbyUser();
	void AutoTaskEveryDay();

private:

	std::string _strategyName;
	std::string m_userid;
	//	std::string _log_pathfilename;
//	CThostFtdcTraderApi* m_tdapi=nullptr;
//	CTraderSpi* _pTraderSpi=nullptr;
	DayTrader * m_pTrader=nullptr;
//	StrategyBase *m_pstrategy=nullptr;
//	SecAssess *_psecassess=nullptr;

	DataWrapper * m_pdatawrapper=nullptr;

	bool m_baseaccount=false;


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
	bool m_taskflag_morning=false;
	int  m_task =0;
	bool _TaskFlag_EveryDay=false;

	bool _flag_loginmarket=false;  //早盘开盘前10分钟 夜盘开盘前15分钟  包含
	bool _flag_open=false; //早盘休市 时间,即金融板块开盘时间  是包含关系
	bool _flag_openmarket=false;
	bool _flag_holiday=false;

};
#endif
