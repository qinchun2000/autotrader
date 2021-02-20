#ifndef TRADESPI_HPP_
#define TRADESPI_HPP_
// 标准C库文件
#include <cstdint>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <boost/regex.hpp>
#include <atomic>
#include <map>

// CTP头文件
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

// 线程控制相关
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// 定时器相关
#include <signal.h>
#include <sys/time.h>


// 字符串编码转化
#include "code_convert.h"

//#include "depthmarket.hpp"
#include "lock.hpp"
#include "redisstatus.hpp"
#include "redismd.hpp"
#include "logutil.hpp"
#include "mysqldata.hpp"
#include "mysqlexchange.hpp"
#include "mysqltradingday.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlorder.hpp"
#include "mysqldayline.hpp"

#include "mysqlposdetail.hpp"
#include "mysqlinvest.hpp"
#include "mysqltradingday.hpp"
#include "mysqlasset.hpp"
#include "logbase.hpp"
#include <json/json.h>

#include "sembase.hpp"
#include "datawrapper.hpp"

using namespace boost;
using namespace std;

class CTraderSpi :public SemBase,public LogBase,public CThostFtdcTraderSpi{
	private:
       // 指向CThostFtdcMduserApi实例的指针
       CThostFtdcTraderApi *m_pUserApi=nullptr;
       DataWrapper * m_datawrapper=nullptr;
       CThostFtdcReqUserLoginField _userLoginField;
       std::string m_userid;


       MysqlDepthMarket *m_pmysqldept=nullptr;
       MysqlInstrument *m_pmysqlinstrument=nullptr;

       pthread_mutex_t _mutex_connect=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_disconnect=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_nreason=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_authenticate=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_login=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_logout=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_orderinsert=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_close_orderinsert=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_tradingday=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
       pthread_mutex_t _mutex_connectcount=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

       std::atomic_bool m_connectstatus{false};
       std::atomic_bool m_loginstatus{false};
       std::atomic_bool m_tradetatus{false};

       std::atomic_bool m_mdstatus{false};

       bool _FrontDisConnectFalg=false;
       bool _FrontConnectFalg=false;
       int _nReason=0;
       bool _flag_logout=false;
       bool _flag_authenticate=false;
       bool _OrderInsertFlag=false;
       bool _CloseOrderInsertFlag=false;
       bool _flag_login=false;
       std::string _tradingday;
       int _ConnectCount=0;
	   std::string test="test";
//	   std::string _logpathfilename;
	   std::string _strategyName;

	   std::mutex m_trademutex;
	   std::map<std::string,std::shared_ptr<OrderData>> m_trademap;

	   std::mutex m_closetrademutex;
	   std::map<std::string,std::shared_ptr<OrderData>> m_closetrademap;

    public:
	   CTraderSpi(DataWrapper * pdatawrapper,CThostFtdcTraderApi *pUserApi);
       virtual ~CTraderSpi() ;

       void SetMysqlDepthMarket(MysqlDepthMarket * psql);
       void SetMDStatus(bool flag);

    	void SetFlagFrontDisConnect(bool flag);
		 bool GetFlagFrontDisConnect();
		 void SetFlagFrontConnectFalg(bool flag);
		 bool GetFlagFrontConnectFalg();

		 void SetFlagOrderInsert(bool flag);
		 bool GetFlagOrderInsert();
		 void SetFlagCloseOrderInsert(bool flag);
		 bool GetFlagCloseOrderInsert();

		 void SetFlagAuthenticate(bool flag);
		 bool GetFlagAuthenticate();
    	 void SetFlagLogin(bool flag);
    	 bool GetFlagLogin();
    	 void SetFlagLogout(bool flag);
		 bool GetFlagLogout();

		 bool GetLoginStatus();

		  void SetConnectErrorCode(int code);
		  int GetConnectErrorCode();
		  void SetConnectCount(int count);
		  int GetConnectCount();

		  int  TryLockMutex();
		  int  UnLockMutex();
//		  bool EmptyInvestPositionMap();
//		  bool InsertInvestPositionMap(InvestPositionBuffer &data);
//		  std::map<std::string,std::shared_ptr<InvestPositionBuffer>> * GetInvestPositionMap();

//		  bool InsertOpenOrderMap(OrderData &data);
//		  bool UpdateOpenOrderMap(OrderData &data);
//		  bool ExistinOrderMap(const char* instrumentid);
//
//		  bool InsertCloseOrderMap(OrderData &data);
//		  bool UpdateCloseOrderMap(OrderData &data);
//		  bool ExistinCloseOrderMap(const char* instrumentid);

		  bool ExistinTradeMap(const char* instrumentid);
		  bool ExistinCloseTradeMap(const char* instrumentid);

    	 std::string GetTest();
		void SetStrategyName(const char* strategyname);
		std::string GetStrategyName();
    	 CThostFtdcReqUserLoginField *GetPointUserLoginField();
    	 void SetTradingDay(const char * day);
    	 std::string GetTradingDay();
    	 std::string GetTradingDay(const char * userid);
    	 void UpdateMysqlTradingDay(const char* userid,const char*tradingday);
    	 bool ExchangeIDCheck(const char * exchangeid,const char* productid);
    	 int  GetStatus(char OrderSubmitStatus,char OrderStatus);
    	 void UpdateOrderData(OrderData &pOrder) ;
    	 void SaveOrderData_Mysql(OrderData  &pOrder);
    	 void SaveSqlInevstPosition(CThostFtdcInvestorPositionField *pInvestorPosition);
    	 void Savesql_DepthMarket(MysqlDepthMarket * sqldepth,const DepthMarketData &depthmarket);

    	 ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
		virtual void OnFrontConnected() ;
    	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    	///@param nReason 错误原因
    	///        0x1001 网络读失败
    	///        0x1002 网络写失败
    	///        0x2001 接收心跳超时
    	///        0x2002 发送心跳失败
    	///        0x2003 收到错误报文
    	virtual void OnFrontDisconnected(int nReason);

    	///心跳超时警告。当长时间未收到报文时，该方法被调用。
    	///@param nTimeLapse 距离上次接收报文的时间
//    	virtual void OnHeartBeatWarning(int nTimeLapse);

    	///客户端认证响应
    	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


    	///登录请求响应
    	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///登出请求响应
    	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///用户口令更新请求响应
//    	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///资金账户口令更新请求响应
//    	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///报单录入请求响应
    	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///预埋单录入请求响应
//    	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///预埋撤单录入请求响应
//    	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///报单操作请求响应
    	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///查询最大报单数量响应
    	virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///投资者结算结果确认响应
    	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///删除预埋单响应
//    	virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///删除预埋撤单响应
//    	virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询报单响应
    	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询成交响应
    	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询投资者持仓响应
    	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询资金账户响应
    	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询投资者响应
    	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询交易编码响应
//    	virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询合约保证金率响应
//    	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询合约手续费率响应
//    	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询交易所响应
    	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询合约响应
    	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询行情响应
    	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询投资者结算结果响应
    	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询转帐银行响应
//    	virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询投资者持仓明细响应
    	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询客户通知响应
    	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询结算信息确认响应
    	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询投资者持仓明细响应
//    	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///查询保证金监管系统经纪公司资金账户密钥响应
//    	virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询仓单折抵信息响应
//    	virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询投资者品种/跨品种保证金响应
    	virtual void OnRspQryInvestorProductGroupMargin(CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询交易所保证金率响应
//    	virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField *pExchangeMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询交易所调整保证金率响应
//    	virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询转帐流水响应
//    	virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询银期签约关系响应
//    	virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///错误应答
    	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///报单通知
    	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) ;

    	///成交通知
    	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) ;

    	///报单录入错误回报
    	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) ;

    	///报单操作错误回报
    	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) ;

    	///合约交易状态通知
    	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

    	///交易通知
//    	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;

    	///提示条件单校验错误
//    	virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) ;

    	///请求查询签约银行响应
//    	virtual void OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询预埋单响应
//    	virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询预埋撤单响应
//    	virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询交易通知响应
//    	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询经纪公司交易参数响应
//    	virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///请求查询经纪公司交易算法响应
//    	virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///银行发起银行资金转期货通知
//    	virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    	///银行发起期货资金转银行通知
//    	virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer) ;

    	///银行发起冲正银行转期货通知
//    	virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    	///银行发起冲正期货转银行通知
//    	virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal) ;

    	///期货发起银行资金转期货通知
//    	virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    	///期货发起期货资金转银行通知
//    	virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer) ;

    	///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
//    	virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    	///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
//    	virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal) ;

    	///期货发起查询银行余额通知
//    	virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) ;

    	///期货发起银行资金转期货错误回报
//    	virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) ;

    	///期货发起期货资金转银行错误回报
//    	virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo) ;

    	///系统运行时期货端手工发起冲正银行转期货错误回报
//    	virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) ;

    	///系统运行时期货端手工发起冲正期货转银行错误回报
//    	virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) ;

    	///期货发起查询银行余额错误回报
//    	virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo) ;

    	///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
//    	virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    	///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
//    	virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal) ;

    	///期货发起银行资金转期货应答
//    	virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///期货发起期货资金转银行应答
//    	virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///期货发起查询银行余额应答
//    	virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

    	///银行发起银期开户通知
//    	virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount) ;

    	///银行发起银期销户通知
//    	virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount) ;

    	///银行发起变更银行账号通知
//    	virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount) ;

    	void OpenOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay);
    	void OpenActionOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay);
    	void CloseOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay);
    	void CloseActionOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay);


};


//// the types of the class factories
//extern "C" {
//		CThostFtdcTraderSpi* create(void);
//        void destroy(CThostFtdcTraderSpi *p);
//
//        typedef CThostFtdcTraderSpi* create_t(void);  // create factory
//        typedef void destory_t(CThostFtdcTraderSpi*); // destory
//}
#endif /* TRADESPI_HPP_ */
