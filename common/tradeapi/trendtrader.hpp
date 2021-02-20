#ifndef _TRENDTRADER_HPP
#define _TRENDTRADER_HPP

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


class TrendTrader:public Trader
{

public:

	TrendTrader();
	TrendTrader(const char* strategyname);
    virtual ~TrendTrader();

    int GetMaxActionNumber();

    void TradeOpenCheck(const char* opensignal);
    void TradeOpen_Test(const char* opensignal);
    void TradeOpen_Limit_Check(const char* opensignal);
    void TradeOpen_Section_Check(const char* opensignal);
    void TradeOpen_Global_Section_Check(Strategy * strategy,const char* opensignal);
    void TradeOpen_Global_SecAssess(Strategy * strategy,SecAssess *secassess,const char* opensignal);

    int Valid_PreOrderInsertCheck(const char* pinstrumentid,
    		MainContractData *maincontractdata,
    		const char* signalname);
    int Valid_OrderInsertCheck(const char* pinstrumentid,
    		MainContractData *maincontractdata,const char* signalname,DepthMarket *depthmarket);

    int Valid_SectionConflictOrderInsertCheck(const char* pinstrumentid,
    		SectionAssessData *sectionassessdata,const char* signalname);
    int Valid_CloseFlagOrderInsertCheck(const char* pinstrumentid,MainContractData *maincontractdata);

    void ReqOpen(MainContractData*item,Strategy * strategy,
    		SectionAssessData *sectionassessdata,const char* opensignal,const char* pinstrumentid);

    void ClosebySignal(const char * postfix);
    void QueueingPriceChange_CancelCheck();
    void QueueingClosePriceChange_CancelCheck();

    //资金管理   仓位管理
    void CheckLocalOrderList(const char* tablename);

    void AccountSelfCheck();
    void DayTrade_AccountSelfCheck();

    void AccountCheck();
    double GetProfitbyInvestPositionData(InvestPositionData *item);
    void FixAccountCheck(double avalible);
    int UpdateInvestPosition();
    int MarketClose_UpdateInvestPosition();
    void UpdateAccount();

    void TotalHoldingCheck();
    void SectionHoldingCheck();

    void HoldPositionDetailCheck();
    void CheckInvestbySqlHoldPosition();

    void CheckSqlHoldPositionbyPosDetail();
    void CheckPosDetailbySqlHoldPosition();

    HoldData CollectHoldDatabyInvest(InvestPositionData *item);
    bool UpdateHoldDatabyInvest(InvestPositionData *item,HoldData &holddata);
    void SyncHoldInvestMap();
    void UpdateHoldMapbyPosDetail();
    void DayTrade_CheckSqlHoldPositionbyPosDetail();

    HoldData  CollectDynamicStopPriceByOpenDate(const char* pinstrumentid,
    		const char* opendate_sqltime,double openprice,int direction);

    HoldData CollectStopPriceByOpenDate(const char* pinstrumentid,
    		const char* opendate_sqltime,double openprice,int direction);
    HoldData  DayTrade_CollectStopPriceByOpenDate(const char* pinstrumentid,
    		const char* opendate,const char* tradeid,double openprice,int direction);

    HoldData  DayTrade_CollectATRStopPriceByOpenDate(const char* pinstrumentid,const char* opendate,double openprice,int direction);
    HoldData  DayTrade_CollectDayLine_ATRStopPriceByOpenDate(const char* pinstrumentid,const char* opendate,double openprice,int direction);
private:


    int _MAX_ACTION_NUMBER=3;  //每次开仓尝试次数 尝试3次失败后,不再开单
};

#endif /* _TRENDTRADER_HPP */
