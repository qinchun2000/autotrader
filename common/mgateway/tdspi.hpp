/*
 * tdspi.hpp
 *
 *  Created on: 2020年2月22日
 *      Author: qc
 */

#ifndef TDSPI_HPP_
#define TDSPI_HPP_

#include <string>
#include <string.h>
#include <map>
#include <iostream>
#include <algorithm>


//#include <unistd.h>
//#include <sys/types.h>
#include <sys/stat.h>

#include "ThostFtdcUserApiDataType.h"
#include "ctpgateway.hpp"
#include "structs.hpp"
#include "codeutil.hpp"

class CTPGateway;
class CTPTD :public CThostFtdcTraderSpi
{
public:
	CTPTD(CTPGateway *CTPGateway, std::string gatewayname);
	virtual ~CTPTD();
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);

	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	///成交通知
//	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);
	// 是否收到成功的响应

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	bool IsFlowControl(int iResult);

	void connect(std::string userID, std::string password, std::string brokerID, std::string address);

	void login();
	void ReqAuthenticate();
	void logout();
	void qryAccount();
	void qryPosition();
	std::string sendOrder(OrderReq & req);
	void cancelOrder(CancelOrderReq & req);
	void close();

	std::string GetUTimeString();

	void SetBrokerID(const char* id);
	std::string GetBrokerID();
	void SetUserID(const char* id);
	std::string GetUserID();
	void SetProductInfo(const char* info);
	std::string GetProductInfo();

private:
	CThostFtdcTraderApi* m_tdapi=nullptr;
	CTPGateway *m_ctpgateway=nullptr;
	std::string m_gatewayname;
	std::atomic_int m_reqID;  //请求编号

	std::mutex m_orderRefmtx;//报单编号
	int m_orderRef;

	std::atomic_bool m_connectionStatus;
	std::atomic_bool m_loginStatus;
	std::atomic_bool m_AuthenticateStatus;
	std::string m_userID;   //账号密码 经纪商地址例如9999 1080 服务器地址tcp://xxx.xxx.xxx.xxx
	std::string m_password;
	std::string m_brokerID;
	std::string m_address;
	std::string m_productifno;
	std::string m_authcode;
	std::string m_appid;

	int m_frontID;//前置机编号
	int m_sessionID;//会话编号

	//
	std::mutex m_positionbuffermtx;//缓存持仓
	std::map<std::string, std::shared_ptr<Event_Position>>m_posBufferMap;
	std::map<std::string, std::string>m_symbolExchangeMap;	   //只读不写，不需要加锁
	std::map<std::string, int>m_symbolSizeMap;
};


#endif /* TDSPI_HPP_ */
