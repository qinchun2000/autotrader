/*
 * tdspi.cpp
 *
 *  Created on: 2020年2月22日
 *      Author: qc
 */

#include "tdspi.hpp"

CTPTD::CTPTD(CTPGateway *CTPGateway, std::string gatewayname)//验证无问题
{
	this->m_ctpgateway = CTPGateway;
	this->m_gatewayname = gatewayname;
	this->m_connectionStatus = false;
	this->m_loginStatus = false;
	this->m_AuthenticateStatus=false;
	this->m_reqID=0;  //请求编号
	this->m_orderRef=1000; //报单编号

	this->m_frontID=0;
	this->m_sessionID=0;
}

CTPTD::~CTPTD()//验证无问题
{

}

void CTPTD::OnFrontConnected()//验证无问题
{
	m_connectionStatus = true;
	std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
	e->gatewayname = m_gatewayname;
	e->msg = "交易服务器连接成功";
	m_ctpgateway->onLog(e);

	ReqAuthenticate();


	std::cout<<"onfront--->>>>>>>>>>>>"<<this->GetUTimeString()<<std::endl;
}

void CTPTD::OnFrontDisconnected(int nReason)//验证无问题
{
	m_connectionStatus = false;
	m_loginStatus = false;
	m_ctpgateway->ctptdconnected = false;
	std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
	e->gatewayname = m_gatewayname;
	e->msg = "交易服务器连接断开";
	m_ctpgateway->onLog(e);
}
///客户端认证响应
void CTPTD::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField,
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID,
		bool bIsLast)
{

	char buf[1050];
	printf("OnRspAuthenticate():被执行...\n");


	// 进程返回结果检查
	if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
	  // typedef int TThostFtdcErrorIDType;
	  // typedef char TThostFtdcErrorMsgType[81];
	  char ErrorMsg[243];
//	  gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
//	  printf("OnRspAuthenticate():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
//
//	  sprintf(buf,"OnRspAuthenticate():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
//	  logutil.WriteLog(buf);
//
//	  this->SetFlagAuthenticate(false);
//
//	  if(pRspInfo->ErrorID==63){
//
//		  sprintf(buf,"退出程序>>>>>>>OnRspAuthenticate():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
//		 		  logutil.WriteLog(buf);
//		//
//		exit(-1);
//		}
//		else{
//
//		}
//	}
//	else{
//		this->SetFlagAuthenticate(true);
	}

	// 如果有返回结果读取返回信息
	if ( pRspAuthenticateField != NULL ) {

		// 读取返回信息,并做编码转化
		///经纪公司代码 TThostFtdcBrokerIDType char[11]
//		char BrokerID[33];
//		gbk2utf8(pRspAuthenticateField->BrokerID,BrokerID,sizeof(BrokerID));
//		printf("OnRspAuthenticate():后台响应  BrokerID=%s  \n",BrokerID);
//		//投资者代码 TThostFtdcInvestorIDType char[13]
//		char UserID[39];
//		gbk2utf8(pRspAuthenticateField->UserID,UserID,sizeof(UserID));
//		printf("OnRspAuthenticate():后台响应  UserID=%s  \n",UserID);
//
//		char UserProductInfo[1024];
//		gbk2utf8(pRspAuthenticateField->UserProductInfo,UserProductInfo,sizeof(UserProductInfo));
//		printf("OnRspAuthenticate():后台响应  UserProductInfo=%s  \n",UserProductInfo);
//		sprintf(buf,"OnRspAuthenticate():后台响应  UserProductInfo=%s \n",UserProductInfo);
//		logutil.WriteLog(buf);
//
//		// 以下代码切换回模拟盘 需要注释///////////////////////////////////////////////////
//		char AppID[1024];
//		gbk2utf8(pRspAuthenticateField->AppID,AppID,sizeof(AppID));
		printf("OnRspAuthenticate():后台响应  AppID=%s  \n",pRspAuthenticateField->AppID);

		//char AppType[1024];
		//gbk2utf8(pRspAuthenticateField->AppType,AppID,sizeof(AppID));
		printf("OnRspAuthenticate():后台响应  AppType=%d  \n",pRspAuthenticateField->AppType-'0');
		// 以上代码切换回模拟盘 需要注释///////////////////////////////////////////////////
		this->m_AuthenticateStatus=true;
	}

	// 如果响应函数已经返回最后一条信息
	if(bIsLast) {
	  // 通知主过程，响应函数将结束
		login();
//	 this->PostSemAuthenticate();
//	  logutil.WriteLog("OnRspAuthenticate():被执行完成...");
	}

}
void CTPTD::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	if (pRspUserLogin == nullptr)
	{
		return;
	}
	if (!IsErrorRspInfo(pRspInfo))
	{
		m_frontID = pRspUserLogin->FrontID;
		m_sessionID = pRspUserLogin->SessionID;
		m_loginStatus = true;
		m_ctpgateway->ctptdconnected = true;
		std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
		e->gatewayname = m_gatewayname;
		e->msg = "交易服务器登录完成";
		m_ctpgateway->onLog(e);
		//确认结算信息
		CThostFtdcSettlementInfoConfirmField myreq;
		strncpy(myreq.BrokerID, m_brokerID.c_str(), sizeof(myreq.BrokerID)-1 );
		strncpy(myreq.InvestorID, m_userID.c_str(), sizeof(myreq.InvestorID)-1);
		m_reqID++;
		m_tdapi->ReqSettlementInfoConfirm(&myreq, m_reqID);

		std::cout<<"login ok  -->"<<myreq.InvestorID<<std::endl;
	}
	else
	{
		//返回错误信息
		std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
		e->gatewayname = m_gatewayname;
		e->errorID = pRspInfo->ErrorID;
		e->errorMsg = pRspInfo->ErrorMsg;
		m_ctpgateway->onError(e);
	}
}

void CTPTD::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	if (pUserLogout == nullptr)
	{
		return;
	}
	//登出回报
	if (!IsErrorRspInfo(pRspInfo))
	{
		m_loginStatus = false;
		m_ctpgateway->ctptdconnected = false;
		std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
		e->gatewayname = m_gatewayname;
		e->msg = "交易服务器登出完成";
		m_ctpgateway->onLog(e);
	}
	else
	{
		//返回错误信息
		std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
		e->gatewayname = m_gatewayname;
		e->errorID = pRspInfo->ErrorID;
		e->errorMsg = pRspInfo->ErrorMsg;
		m_ctpgateway->onError(e);
	}
}

void CTPTD::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	//错误发单柜台
	if (pInputOrder == nullptr)
	{
		return;
	}
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->gatewayname = m_gatewayname;
	e->errorID = pRspInfo->ErrorID;
	e->errorMsg = pRspInfo->ErrorMsg;
	m_ctpgateway->onError(e);
}

void CTPTD::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	//撤单错误柜台
	if (pInputOrderAction == nullptr)
	{
		return;
	}
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->gatewayname = m_gatewayname;
	e->errorID = pRspInfo->ErrorID;
	e->errorMsg = pRspInfo->ErrorMsg;
	m_ctpgateway->onError(e);
}

void CTPTD::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	//结算单确认
	if (pSettlementInfoConfirm == nullptr)
	{
		return;
	}
	std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
	e->gatewayname = m_gatewayname;
	e->msg = "结算单确认完成";
	m_ctpgateway->onLog(e);

	std::cout<<"结算单确认完成!!! "<<this->GetUTimeString()<<std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(5));

	std::cout<<"waiting ok!!! "<<this->GetUTimeString()<<std::endl;

	m_reqID++;
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	while (true)
	{
		if (m_connectionStatus == false)
		{
			return;
		}
		if (m_tdapi != nullptr)
		{
			int iResult = m_tdapi->ReqQryInstrument(&req, m_reqID);
			if (!IsFlowControl(iResult))
			{
				std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
				e->gatewayname = m_gatewayname;
				e->msg = "查询合约请求发送";
				m_ctpgateway->onLog(e);
				break;
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
				e->gatewayname = m_gatewayname;
				e->msg = "查询合约请求流控限制";
				m_ctpgateway->onLog(e);
			}
		}
	}
}

void CTPTD::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	if (pTradingAccount == nullptr)
	{
		return;
	}
	std::shared_ptr<Event_Account>e = std::make_shared<Event_Account>();
	e->gatewayname = "CTP";
	e->accountid = pTradingAccount->AccountID;
	e->preBalance = pTradingAccount->PreBalance;
	e->available = pTradingAccount->Available;
	e->commission = pTradingAccount->Commission;
	e->margin = pTradingAccount->CurrMargin;
	e->closeProfit = pTradingAccount->CloseProfit;
	e->positionProfit = pTradingAccount->PositionProfit;

	e->balance = (pTradingAccount->PreBalance - pTradingAccount->PreCredit - pTradingAccount->PreMortgage +
		pTradingAccount->Mortgage - pTradingAccount->Withdraw + pTradingAccount->Deposit +
		pTradingAccount->CloseProfit + pTradingAccount->PositionProfit + pTradingAccount->CashIn -
		pTradingAccount->Commission);
	m_ctpgateway->onAccount(e);
	std::cout<<"OnRspQryTradingAccount--->>>>>>>>>>>>"<<this->GetUTimeString()<<std::endl;
}

void CTPTD::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//持仓查询
	if (pInvestorPosition == nullptr)
	{
		//空指针
		return;
	}
	std::string symboldirection = pInvestorPosition->InstrumentID;//合约名
	symboldirection += pInvestorPosition->PosiDirection;
	std::unique_lock<std::mutex>lck(m_positionbuffermtx);
	if (m_posBufferMap.find(symboldirection) == m_posBufferMap.end())//如果找不到这个合约
	{
		std::shared_ptr<Event_Position>e = std::make_shared<Event_Position>();
		m_posBufferMap.insert(std::pair<std::string, std::shared_ptr<Event_Position>>(symboldirection, e));//创建缓存
	}
	m_posBufferMap[symboldirection]->gatewayname = "CTP";
	m_posBufferMap[symboldirection]->symbol = pInvestorPosition->InstrumentID;
	if (pInvestorPosition->PosiDirection == '2')
	{
		m_posBufferMap[symboldirection]->direction = DIRECTION_LONG;
	}
	else if (pInvestorPosition->PosiDirection == '3')
	{
		m_posBufferMap[symboldirection]->direction = DIRECTION_SHORT;
	}

	std::string exchange = m_symbolExchangeMap[pInvestorPosition->InstrumentID];
	if (exchange == EXCHANGE_SHFE)
	{
		if (pInvestorPosition->YdPosition)
		{
			m_posBufferMap[symboldirection]->ydPosition = pInvestorPosition->Position;
			m_posBufferMap[symboldirection]->ydPositionCost = pInvestorPosition->PositionCost;
		}
		else
		{
			m_posBufferMap[symboldirection]->todayPosition = pInvestorPosition->Position;
			m_posBufferMap[symboldirection]->todayPositionCost = pInvestorPosition->PositionCost;
		}

		m_posBufferMap[symboldirection]->position = m_posBufferMap[symboldirection]->ydPosition + m_posBufferMap[symboldirection]->todayPosition;

		if (m_posBufferMap[symboldirection]->todayPosition || m_posBufferMap[symboldirection]->ydPosition)
		{
			m_posBufferMap[symboldirection]->price = (m_posBufferMap[symboldirection]->ydPositionCost + m_posBufferMap[symboldirection]->todayPositionCost) / ((m_posBufferMap[symboldirection]->todayPosition + m_posBufferMap[symboldirection]->ydPosition)*m_symbolSizeMap[pInvestorPosition->InstrumentID]);
		}
		else
		{
			m_posBufferMap[symboldirection]->price = 0;
		}
	}
	else
	{
		m_posBufferMap[symboldirection]->position = pInvestorPosition->Position;
		m_posBufferMap[symboldirection]->ydPosition = 0;

		if (m_posBufferMap[symboldirection]->position)
		{
			m_posBufferMap[symboldirection]->price = pInvestorPosition->PositionCost / (m_posBufferMap[symboldirection]->position*m_symbolSizeMap[pInvestorPosition->InstrumentID]);
		}
		else
		{
			m_posBufferMap[symboldirection]->price = 0;
		}
	}
	if (bIsLast == true)
	{
		//如果是最后一笔数据
		for (std::map<std::string, std::shared_ptr<Event_Position>>::iterator it = m_posBufferMap.begin(); it != m_posBufferMap.end(); it++)
		{
			//循环推入position
//			m_ctpgateway->onPosition(it->second);
		}
	}

	std::cout<<"OnRspQryInvestorPosition--->>>>>>>>>>>>"<<this->GetUTimeString()<<std::endl;
}

void CTPTD::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)//验证无问题
{
	if (pInstrument == nullptr)
	{
		//空指针
		return;
	}
	//合约查询
	std::shared_ptr<Event_Contract>e = std::make_shared<Event_Contract>();
	e->gatewayname = m_gatewayname;
	e->symbol = std::string(pInstrument->InstrumentID);
	e->exchange = pInstrument->ExchangeID;
	e->name = pInstrument->InstrumentName;
	e->optionType = pInstrument->OptionsType;
	e->priceTick = pInstrument->PriceTick;
	e->productClass = pInstrument->ProductClass;
	e->size = pInstrument->VolumeMultiple;
	e->strikePrice = pInstrument->StrikePrice;
	e->underlyingSymbol = pInstrument->UnderlyingInstrID;
	m_symbolExchangeMap.insert(std::pair<std::string, std::string>(e->symbol, e->exchange));
	m_symbolSizeMap.insert(std::pair<std::string, int>(e->symbol, e->size));
	m_ctpgateway->onContract(e);

//	std::cout<<"instrument--->"<<e->symbol<<std::endl;
	if (bIsLast)
	{
		std::cout<<"instrument  fin!!!--->"<<this->GetUTimeString()<<std::endl;

		std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
		e->gatewayname = m_gatewayname;
		e->msg = "合约查询完成";
		m_ctpgateway->onLog(e);
		m_ctpgateway->initQuery();


	}
}

void CTPTD::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	if (pOrder == nullptr)
	{
		//空指针
		return;
	}
	const char *neworderref = pOrder->OrderRef;
	m_orderRefmtx.lock();
	m_orderRef = std::max(atoi(neworderref), m_orderRef);
	m_orderRefmtx.unlock();
	std::shared_ptr<Event_Order>e = std::make_shared<Event_Order>();
	e->symbol = pOrder->InstrumentID;
	e->exchange = pOrder->ExchangeID;
	e->orderID = pOrder->OrderRef;
	e->gatewayname = "CTP";
	if (pOrder->Direction == '0')
	{
		//多
		e->direction = DIRECTION_LONG;
	}
	else if (pOrder->Direction == '1')
	{
		//空
		e->direction = DIRECTION_SHORT;
	}
	else
	{
		//未知
		e->direction = DIRECTION_UNKNOWN;
	}

	if (pOrder->CombOffsetFlag[0] == '0')
	{
		//开
		e->offset = OFFSET_OPEN;
	}
	else if (pOrder->CombOffsetFlag[0] == '1')
	{
		e->offset = OFFSET_CLOSE;
	}
	else if (pOrder->CombOffsetFlag[0] == '3')
	{
		e->offset = OFFSET_CLOSETODAY;
	}
	else if (pOrder->CombOffsetFlag[0] == '4')
	{
		e->offset = OFFSET_CLOSEYESTERDAY;
	}
	else
	{
		e->offset = OFFSET_UNKNOWN;
	}

	if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)
	{
		//全成交
		e->status = STATUS_ALLTRADED;
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)
	{
		//部分成交
		e->status = THOST_FTDC_OST_PartTradedQueueing;
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)
	{
		//未成交
		e->status = THOST_FTDC_OST_NoTradeQueueing;
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
	{
		//撤销
		e->status = STATUS_CANCELLED;
	}
	else
	{
		//未知
		e->status = THOST_FTDC_OST_Unknown;
	}

	e->price = pOrder->LimitPrice;
	e->totalVolume = pOrder->VolumeTotalOriginal;
	e->tradedVolume = pOrder->VolumeTraded;
	e->orderTime = pOrder->InsertTime;
	e->cancelTime = pOrder->CancelTime;
	e->frontID = pOrder->FrontID;
	e->sessionID = pOrder->SessionID;

	//维护订单状态
	if (e->status == STATUS_ALLTRADED || e->status == STATUS_CANCELLED)
	{
		std::unique_lock<std::mutex>lck(m_ctpgateway->m_ordermapmtx);
		if (m_ctpgateway->m_ordermap.find(e->orderID) != m_ctpgateway->m_ordermap.end())
		{
			m_ctpgateway->m_ordermap.erase(e->orderID);
		}
	}
	else
	{
		std::unique_lock<std::mutex>lck(m_ctpgateway->m_ordermapmtx);
		if (m_ctpgateway->m_ordermap.find(e->orderID) != m_ctpgateway->m_ordermap.end())
		{
			m_ctpgateway->m_ordermap[e->orderID] = e;

		}
		else
		{
			m_ctpgateway->m_ordermap.insert(std::pair<std::string, std::shared_ptr<Event_Order>>(e->orderID, e));
		}
	}

	m_ctpgateway->onOrder(e);
}

void CTPTD::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo == nullptr)
	{
		//空指针
		return;
	}
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->errorMsg = pRspInfo->ErrorMsg;
	e->errorID = pRspInfo->ErrorID;
	e->gatewayname = m_gatewayname;
	m_ctpgateway->onError(e);
}

//void CTPTD::OnRtnTrade(CThostFtdcTradeField *pTrade)
//{
//	if (pTrade == nullptr)
//	{
//		//空指针
//		return;
//	}
//	std::shared_ptr<Event_Trade>e = std::make_shared<Event_Trade>();
//	e->gatewayname = m_gatewayname;
//	e->symbol = pTrade->InstrumentID;
//	e->exchange = pTrade->ExchangeID;
//	e->tradeID = pTrade->TradeID;
//	e->orderID = pTrade->OrderRef;
//	if (pTrade->Direction == '0')
//	{
//		//多
//		e->direction = DIRECTION_LONG;
//	}
//	else if (pTrade->Direction == '1')
//	{
//		//空
//		e->direction = DIRECTION_SHORT;
//	}
//	else
//	{
//		//未知
//		e->direction = DIRECTION_UNKNOWN;
//	}
//
//	if (pTrade->OffsetFlag == '0')
//	{
//		//开
//		e->offset = OFFSET_OPEN;
//	}
//	else if (pTrade->OffsetFlag == '1')
//	{
//		e->offset = OFFSET_CLOSE;
//	}
//	else if (pTrade->OffsetFlag == '3')
//	{
//		e->offset = OFFSET_CLOSETODAY;
//	}
//	else if (pTrade->OffsetFlag == '4')
//	{
//		e->offset = OFFSET_CLOSEYESTERDAY;
//	}
//	else
//	{
//		e->offset = OFFSET_UNKNOWN;
//	}
//	e->price = pTrade->Price;
//	e->volume = pTrade->Volume;
//	e->tradeTime = Utils::getCurrentSystemDate() + pTrade->TradeTime;
//	m_ctpgateway->onTrade(e);
//}

void CTPTD::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	if (pInputOrder == nullptr)
	{
		//空指针
		return;
	}
	//发单错误回报(交易所)
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->errorID = pRspInfo->ErrorID;
	e->errorMsg = pRspInfo->ErrorMsg;
	m_ctpgateway->onError(e);
};

void CTPTD::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	if (pOrderAction == nullptr)
	{
		//空指针
		return;
	}
	//撤单错误回报(交易所)
	std::shared_ptr<Event_Error>e = std::make_shared<Event_Error>();
	e->errorID = pRspInfo->ErrorID;
	e->errorMsg = pRspInfo->ErrorMsg;
	m_ctpgateway->onError(e);
}

bool CTPTD::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	return bResult;
}

bool CTPTD::IsFlowControl(int iResult)
{
	return ((iResult == -2) || (iResult == -3));
}

void CTPTD::connect(std::string userID, std::string password, std::string brokerID, std::string address)
{
	m_userID = userID;
	m_password = password;
	m_brokerID = brokerID;
	m_address = address;
	if (m_connectionStatus == false)
	{

		CodeUtil codeutil;
		std::string flowpath=codeutil.GetCurrentDirectory();
		flowpath.append("/");
		flowpath.append("Test");
		flowpath.append("Flow/");

		if(access(flowpath.c_str(),0)==-1){//access函数是查看文件是不是存在
			printf("CheckFlowPath:开始启动程序  --->>>>不存在文件夹 %s \n",flowpath.c_str());
			 if (mkdir(flowpath.c_str(),0777)){//如果不存在就用mkdir函数来创建
				 printf("CheckFlowPath:开始启动程序  --->>>>创建文件夹 %s Error!!!  \n",flowpath.c_str());
			}
			 else{
				 printf("CheckFlowPath:开始启动程序  --->>>>创建文件夹 %s 完成!  \n",flowpath.c_str());
			 }
		}
		else{
	//		printf("Run_Trade_zlqh:开始启动程序  --->>>>存在文件夹 %s \n",flowpath.c_str());
		}

		m_tdapi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());
		m_tdapi->RegisterSpi(this);
		m_tdapi->RegisterFront((char*)m_address.c_str());
		m_tdapi->SubscribePublicTopic(THOST_TERT_QUICK);
		m_tdapi->SubscribePrivateTopic(THOST_TERT_QUICK);
		m_tdapi->Init();
	}
	else
	{
		if (m_loginStatus == false)
		{
			login();
		}
	}

}

void CTPTD::login()
{
	if (m_userID.empty() == false && m_password.empty() == false && m_brokerID.empty() == false)
	{
		CThostFtdcReqUserLoginField myreq;
		strncpy(myreq.BrokerID, m_brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
		strncpy(myreq.UserID, m_userID.c_str(), sizeof(myreq.UserID) - 1);
		strncpy(myreq.Password, m_password.c_str(), sizeof(myreq.Password) - 1);

		strcpy(myreq.UserProductInfo,"ANYUNSIMNO");

		m_reqID++;
		m_tdapi->ReqUserLogin(&myreq, m_reqID);
	}
}
void CTPTD::ReqAuthenticate()
{

	printf("Trader::ReqAuthenticate   ---->Begin!!!   \n");

	CThostFtdcReqAuthenticateField authenticatefield ;
	memset(&authenticatefield,0,sizeof(authenticatefield));

	strcpy(authenticatefield.BrokerID, "9999");
	strcpy(authenticatefield.UserID,  "081759");

//	gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
	char info[11];
	char code[17];
	CodeUtil codeutil;
	std::string userproductinfo;
	std::string authcode;
	std::string appid;


	 userproductinfo = "ANYUNSIMNO";
	 authcode ="0000000000000000";
	 appid="simnow_client_test";

	strcpy(authenticatefield.UserProductInfo, userproductinfo.c_str()); // 11 字节 限定
	strcpy(authenticatefield.AuthCode, authcode.c_str());  // 17字节限定
	strcpy(authenticatefield.AppID,appid.c_str());


	printf("Trader::ReqAuthenticate   上报信息----> BrokerID:%s  \n",authenticatefield.BrokerID);
	printf("Trader::ReqAuthenticate   上报信息----> UserID:%s  \n",authenticatefield.UserID);
	printf("Trader::ReqAuthenticate   上报信息----> UserProductInfo:%s  \n",authenticatefield.UserProductInfo);
	printf("Trader::ReqAuthenticate   上报信息----> AuthCode:%s  \n",authenticatefield.AuthCode);
	printf("Trader::ReqAuthenticate   上报信息----> AppID:%s  \n",authenticatefield.AppID);

	m_reqID++;
	sleep(1);

	int rt_pTraderApi =m_tdapi->ReqAuthenticate(&authenticatefield,m_reqID);

}

std::string CTPTD::sendOrder(OrderReq & req)
{
	std::unique_lock<std::mutex>lck(m_orderRefmtx);
	m_orderRef += 1;
	m_reqID += 1;
	CThostFtdcInputOrderField myreq;
	memset(&myreq, 0, sizeof(myreq));

	strncpy(myreq.BrokerID, m_brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
	strncpy(myreq.InvestorID, m_userID.c_str(), sizeof(myreq.InvestorID) - 1);
	strncpy(myreq.OrderRef, std::to_string(m_orderRef).c_str(), sizeof(myreq.OrderRef) - 1);
	strncpy(myreq.InstrumentID, req.symbol.c_str(), sizeof(myreq.InstrumentID) - 1);
	strncpy(myreq.UserID, m_userID.c_str(), sizeof(myreq.UserID) - 1);

	myreq.LimitPrice = req.price;
	myreq.VolumeTotalOriginal = req.volume;
	if (req.priceType == PRICETYPE_LIMITPRICE)
	{
		myreq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	}
	if (req.priceType == PRICETYPE_MARKETPRICE)
	{
		myreq.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
	}

	if (req.direction == DIRECTION_LONG)
	{
		myreq.Direction = THOST_FTDC_D_Buy;
	}
	if (req.direction == DIRECTION_SHORT)
	{
		myreq.Direction = THOST_FTDC_D_Sell;
	}

	if (req.offset == OFFSET_OPEN)
	{
		myreq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	}
	if (req.offset == OFFSET_CLOSE)
	{
		myreq.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
	}
	if (req.offset == OFFSET_CLOSETODAY)
	{
		myreq.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}
	if (req.offset == OFFSET_CLOSEYESTERDAY)
	{
		myreq.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
	}

	myreq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	myreq.ContingentCondition = THOST_FTDC_CC_Immediately;
	myreq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	myreq.IsAutoSuspend = 0;
	myreq.TimeCondition = THOST_FTDC_TC_GFD;
	myreq.VolumeCondition = THOST_FTDC_VC_AV;
	myreq.MinVolume = 1;

	if (req.priceType == PRICETYPE_FAK)
	{
		myreq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		myreq.TimeCondition = THOST_FTDC_TC_IOC;
		myreq.VolumeCondition = THOST_FTDC_VC_AV;
	}
	if (req.priceType == PRICETYPE_FOK)
	{
		myreq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		myreq.TimeCondition = THOST_FTDC_TC_IOC;
		myreq.VolumeCondition = THOST_FTDC_VC_CV;
	}

	int i = m_tdapi->ReqOrderInsert(&myreq, m_reqID);
	return std::to_string(m_orderRef);
}

void CTPTD::cancelOrder(CancelOrderReq & req)
{
	m_reqID += 1;
	CThostFtdcInputOrderActionField myreq = CThostFtdcInputOrderActionField();
	memset(&myreq, 0, sizeof(myreq));

	strncpy(myreq.BrokerID, m_brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
	strncpy(myreq.InvestorID, m_userID.c_str(), sizeof(myreq.InvestorID) - 1);
	strncpy(myreq.InstrumentID, req.symbol.c_str(), sizeof(myreq.InstrumentID) - 1);
	strncpy(myreq.ExchangeID, req.exchange.c_str(), sizeof(myreq.ExchangeID) - 1);
	strncpy(myreq.OrderRef, req.orderID.c_str(), sizeof(myreq.OrderRef) - 1);
	myreq.FrontID = m_frontID;
	myreq.SessionID = m_sessionID;

	myreq.ActionFlag = THOST_FTDC_AF_Delete;

	m_tdapi->ReqOrderAction(&myreq, m_reqID);
}

void CTPTD::qryPosition()//验证无问题
{
	m_reqID++;
	CThostFtdcQryInvestorPositionField req = CThostFtdcQryInvestorPositionField();
	memset(&req, 0, sizeof(req));
	strncpy(req.BrokerID, m_brokerID.c_str(), sizeof(req.BrokerID) - 1);
	strncpy(req.InvestorID, m_userID.c_str(), sizeof(req.InvestorID) - 1);
	if (m_tdapi != nullptr)
	{
		m_tdapi->ReqQryInvestorPosition(&req, m_reqID);
	}
}

void CTPTD::qryAccount()//验证无问题
{
	m_reqID++;
	CThostFtdcQryTradingAccountField req = CThostFtdcQryTradingAccountField();
	memset(&req, 0, sizeof(req));
	strncpy(req.BrokerID, m_brokerID.c_str(), sizeof(req.BrokerID) - 1);
	strncpy(req.InvestorID, m_userID.c_str(), sizeof(req.InvestorID) - 1);
	if (m_tdapi != nullptr)
	{
		m_tdapi->ReqQryTradingAccount(&req, m_reqID);
	}
}

void CTPTD::logout()
{
	CThostFtdcUserLogoutField myreq;
	strncpy(myreq.BrokerID, m_brokerID.c_str(), sizeof(myreq.BrokerID) - 1);
	strncpy(myreq.UserID, m_userID.c_str(), sizeof(myreq.UserID) - 1);
	m_reqID++;
	m_tdapi->ReqUserLogout(&myreq, m_reqID);
}


void CTPTD::close()//验证无问题
{
	if (m_tdapi != NULL)
	{
		logout();
		m_tdapi->RegisterSpi(NULL);
		m_tdapi->Release();
		m_tdapi = NULL;
		m_connectionStatus = false;
	}
}


inline std::string CTPTD::GetUTimeString(){
//		time_t tt = time(NULL);//这句返回的只是一个时间cuo
//		tm* t= localtime(&tt);
//
//		char char_today[30];
//
//		 sprintf(char_today ,"%d-%02d-%02d %02d:%02d:%02d",t->tm_year + 1900,t->tm_mon + 1,
//					  t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec,t->);
//		 std::string date = char_today;

struct timeval tv;
struct timezone tz;
gettimeofday(&tv,&tz);
//cout << "second : \t" << tv.tv_sec << endl; //秒
//cout << "millisecond : \t" << tv.tv_sec*1000 + tv.tv_usec/1000 << endl; // 毫秒
char buf[1024];
sprintf(buf,"%ld.%ld",tv.tv_sec ,tv.tv_usec);
std::string utime=buf;
return utime;
}

