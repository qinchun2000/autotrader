#ifndef _TRADE_HPP
#define _TRADE_HPP

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
#include "mytdspi.hpp"

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
#include "tech.hpp"
#include "assetbase.hpp"
#include "logbase.hpp"
#include "datawrapper.hpp"

using namespace std;
using namespace log4cplus;


class TradeBase:public Tech,public AssetBase,public LogBase
{

public:

	TradeBase();
    virtual ~TradeBase();
	void SetDataWrapper(DataWrapper *point);
	DataWrapper* GetDataWrapper();

	void SetMysqlAsset(MysqlAsset *mysqlasset);
	MysqlAsset* GetMysqlAsset();

	void SetMysqlInvestPosition(MysqlInvestPosition *mysqlinvestp);
	MysqlInvestPosition* GetMysqlInvestPosition();

	void SetUserLoginField();
	CThostFtdcReqUserLoginField GetUserLoginField();
	CThostFtdcReqUserLoginField *GetPointUserLoginField();
	std::string GetBrokerID();
	std::string GetUserID();

	void SetUserLogoutField();
	void SetUserLoginField(const char* name);
	CThostFtdcUserLogoutField *GetPointUserLogoutField();

	void SetCThostFtdcTraderApi(CThostFtdcTraderApi *ptraderapi);
	CThostFtdcTraderApi * GetCThostFtdcTraderApi();
	void SetCTraderSpi(CTraderSpi *ptraderspi);
	CTraderSpi * GetCTraderSpi();

	void SetFlagTradeLimit(bool flag);
	bool GetFlagTradeLimit();
	void SetFlagInvestUpdate(bool flag);
	bool GetFlagInvestUpdate();

	void SetRequestID(int requestid);
	int UpdateRequestID();
	int GetRequestID();

	void SetMaxOrderRef(int maxorderref);
	int UpdateMaxOrderRef();
	int GetMaxOrderRef();
	std::string AutoIncretMaxOrderRef();

	void SetNextOrderRef(int nref);
	int UpdateNextOrderRef();
	int GetNextOrderRef();
	char GetCombOffsetFlagbyCloseOrder(DepthMarket *depthmarket,HoldData *item);


//	bool EmptyMainContractMap();
//	bool InsertMainContractMap(MainContractData &data);
//	std::map<std::string,MainContractData> * GetMainContractMap();

	void CTP_Initial();
	std::string  CheckFlowPath();
	void CTP_AutoInitial();
	void CTP_AutoInitial_MD();
	void CTP_Set();
	void FrontConnect();
	void UserLogin();
//	void Login();
	void FirstLogin();

	void UserLogout();
	void ReleaseTradeApi();
	void Close();

	void ReqOrderInsertbySignal(Signal *psignal,const char* exchangeid,double price,int volume);


	void ReqSqlCloseOrderInsert(const char *signalname,
			const char *pinstrumentid,
			const char *exchangeid,
			char directiontype,
			char CombOffsetFlag,
			double price,
			int volume,
			double stopprice);

	void ReqSqlOrderAction(const char *signalname,const char *pinstrumentid,const char *exchangeid,const char*orderref,
				int frontid,int sessionid);
	void ReqSqlCloseOrderAction(const char *signalname,const char *pinstrumentid,const char *exchangeid,const char*orderref,
	            		int frontid,int sessionid);

private:
	CThostFtdcTraderApi* m_pTraderApi=nullptr;
	CTraderSpi* m_pTraderSpi=nullptr;
	MyTDSpi * m_mytdspi=nullptr;

	std::string m_log_path;
	std::string m_log_filename;
	// 登录请求结构体
	CThostFtdcReqUserLoginField m_userLoginField;
	//// 用户请求结构体
	CThostFtdcUserLogoutField m_userLogoutField;

	int m_requestID;
	int m_MaxOrderRef;
	int m_iNextOrderRef;
	bool m_flagtradelimit{false};
	bool m_flaginvestupdate{false};

//	MysqlAsset _mysqlasset;
	MysqlAsset *m_pmysqlasset=nullptr;
	MysqlInvestPosition *m_pmysqlinvestposition =nullptr;
	DataWrapper * m_pdatawrapper=nullptr;
};

#endif /* _TRADE_HPP */
