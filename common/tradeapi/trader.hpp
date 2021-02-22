#ifndef _TRADER_HPP
#define _TRADER_HPP

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
#include "mysqlcommission.hpp"
#include "redismd.hpp"
#include "redisstatus.hpp"
//#include "redisorder.hpp"
#include "redissection.hpp"
#include "depthmarket.hpp"
#include "dateutil.hpp"
//#include "smtputil.hpp"
#include "logutil.hpp"
#include "lock.hpp"
#include <boost/regex.hpp>
#include <json/json.h>

#include "config.hpp"

#include "tradebase.hpp"
#include "debuglog.hpp"

using namespace std;
using namespace log4cplus;


class Trader:public TradeBase
{

public:

	Trader();
    virtual ~Trader();


    void InitialTradeApi();
    void InitialTradeApibyUser();
    void InitialTradeApi_MD();
    void ReqInit();
    void ReqAuthenticate();
    void ReqQryTradingAccount();
    void ReqQryInvestor();
    void ReqQryInvestorPositionDetail();
    void ReqQueryMaxOrderVolume();
    void ReqQryNotice();
    void ReqQrySettlementInfoConfirm (); //查询是否结算信息确认
    void ReqQrySettlementInfo (); // 查询结算信息
    void ReqQrySettlementInfoDefault ();
    void ReqSettlementInfoConfirm ();  // 请求结算信息确认
    void ReqQryExchange();
    void ReqQryInvestorProductGroupMargin();
    void ReqQryOrder();
    void ReqQryOrder(const char * pinstrumentid);
    void ReqQryTrade();
    void ReqQryInvestorPosition();

    void CheckInstrument();
    void CheckDepMarket();


    void CheckSqlInstrument();
    void CheckSqlProduct();
    void CheckSqlDepMarket();
    void CheckSqlDepMarket_EndTime();

    void CheckSettleInfoConfirm();
    bool CheckSettleInfoConfirmbyTrade();
    void CheckSqlMaincontracont();
    void CheckContract(vector<std::string> &instrumentlist);
    void CheckSqlMaincontracontbyOpenInterest();
    bool GetNextContractbyVolume(MainContractData *maincontractdata,ProductData *item);
    bool GetNextContractbyOpeninterest(MainContractData *maincontractdata,ProductData *item);
    void CheckSqlMaincontracontbyMaxOpenInterest();
    void SaveMainContractData(DepthMarketData *main,DepthMarketData *next,ProductData *item);
    void CreatMainContractData(MysqlDayLine *mysqldayline,MainContractData *maincontractdata,
    		InstrumentData *insopi, DepthMarketData *maxopi,ProductData *item,double margin);
    void CreatMainContractDatabyMaxOPI(MainContractData *maincontractdata,
    		InstrumentData *insopi,DepthMarketData *maxopi,ProductData *item);
    void UpdateSqlDepMarket();
    void ReqQrySingleInstrument(const char* pinstrumentid);

    bool CheckTradingDay();
    void CloseOrder(Signal signal,
    			double openprice,
    			double stopprice,
    			const char*exchangeid,
    			const char*opendate,int maxcount);

};

#endif /* _BASEINFO_HPP */
