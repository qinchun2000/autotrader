#ifndef _THREADTRADE_HPP
#define _THREADTRADE_HPP

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
#include "strategybase.hpp"

#include "statest.hpp"
#include "trendtrader.hpp"
#include "daytrader.hpp"
#include "turtle.hpp"

#include "logbase.hpp"
#include "threadbase.hpp"
#include "secassess.hpp"
#include "sysbase.hpp"
#include "datawrapper.hpp"
#include "eventengine.hpp"
#include "ctpgateway.hpp"
#include "gatewaymanager.hpp"
#include "ctamanager.hpp"
#include "market.hpp"
#include "debuglog.hpp"

using namespace std;
using namespace log4cplus;

#define DEBUG

class ThreadTrade:public SysBase
{

public:

	ThreadTrade(const char* strategyname);
	void DropSqlTable();
	void InitialSqlTable();
	void MdConnect();
	void LocalLogin();
	void LocalLogin_MD();
	void UpdateFlag();
	bool ConnectRestore();
	bool OpenMarketLoginOK();

	void TradeInitial_noSta();
	void TradeInitial_MD();
	void Fun_ConnectError();
	void Fun_ReadyLogin();
	void Fun_TradeLimit();
	void Fun_Trade_FlagOrderInsertCheck();
	void Fun_Trade_HoldCheck_noSta();
	void Fun_Trade_OpenCheck_noSta();

	void Run();
	void mdRun();

private:


};

#endif /* _THREADTRADE_HPP */
