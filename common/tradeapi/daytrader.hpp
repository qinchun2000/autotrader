/*
 * daytrader.hpp
 *
 *  Created on: 2020年3月16日
 *      Author: qc
 */

#ifndef DAYTRADER_HPP_
#define DAYTRADER_HPP_

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
#include "mysqlutil.hpp"
#include "mysqlmindata.hpp"
#include "mysqlsecassess.hpp"

#include "redismd.hpp"
#include "redisstatus.hpp"
//#include "redisorder.hpp"
#include "redissection.hpp"
#include "depthmarket.hpp"
#include "dateutil.hpp"

#include "logutil.hpp"
#include "lock.hpp"
#include <boost/regex.hpp>
#include <json/json.h>

#include "config.hpp"
#include "strategy.hpp"
#include "trader.hpp"
#include "secassess.hpp"


using namespace std;
using namespace log4cplus;


class DayTrader:public Trader
{

public:

	DayTrader();
	DayTrader(DataWrapper * datawrapper,const char* strategyname);
    virtual ~DayTrader();

    int GetMaxActionNumber();
    void TradeOpen_Test(const char* opensignal);

    void OpenSignalCheck(const char* opensignal);
    void CloseSignalCheck(const char * postfix);

    void QueueingPriceChange_CancelCheck();
    void QueueingClosePriceChange_CancelCheck();

    int UpdateInvestPosition();

    HoldData CollectHoldDatabyInvest(InvestPositionData *item);
    bool UpdateHoldDatabyInvest(InvestPositionData *item,HoldData &holddata);
    void SyncHoldInvestMap();


private:


    int _MAX_ACTION_NUMBER=3;  //每次开仓尝试次数 尝试3次失败后,不再开单
};




#endif /* DAYTRADER_HPP_ */
