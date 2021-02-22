/*
 * mongoorder.hpp
 *
 *  Created on: 2017-8-4
 *      Author: vnpy
 */

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/builder/stream/document.hpp>


#ifndef MONGOORDER_HPP_
#define MONGOORDER_HPP_

#include "dateutil.hpp"
#include "logutil.hpp"
#include <json/json.h>
#include "mongoinvestpositiondetail.hpp"
#include <vector>

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class  OrderInfo
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[9];
			char TraderID[21];
			char TradeID[21];
			double Avalible;
			int CombOffsetFlag;
			int CombHedgeFlag;
			int Direction;
			int FrontID;
			int SessionID;
			int ErrorId;
			char OrderSysID[21];
			char OrderRef[20];
			char ExchangeID[27];
			char OpenSignalName[30];
			char OpenCancelSignalName[30];
			char CloseSignalName[30];
			char CloseCancelSignalName[30];
			double LimitPrice;
			double BidPrice1;   //申买价1
			double AskPrice1;   //申卖价1

			double Price;
			double OpenPrice;
			double OpenPrice1;
			double OpenPrice2;
			double OpenPrice3;

			double AverageOpenPrice;

			int Volume;
			int Volume1;
			int Volume2;
			int Volume3;
			int Status;
			double StopPrice;
			bool WinReady;   // 启动止盈标志 true: 启动  false: 未启动
			double WinPlanPrice;   // 计划止盈价格
			///报单提交状态 TThostFtdcOrderSubmitStatusType char
			//// THOST_FTDC_OSS_InsertSubmitted '0' 已经提交
			//// THOST_FTDC_OSS_CancelSubmitted '1' 撤单已经提交
			//// THOST_FTDC_OSS_ModifySubmitted '2' 修改已经提交
			//// THOST_FTDC_OSS_Accepted '3' 已经接受
			//// THOST_FTDC_OSS_InsertRejected '4' 报单已经被拒绝
			//// THOST_FTDC_OSS_CancelRejected '5' 撤单已经被拒绝
			//// THOST_FTDC_OSS_ModifyRejected '6' 改单已经被拒绝
			char OrderSubmitStatus;

			///报单状态 TThostFtdcOrderStatusType char
			//// THOST_FTDC_OST_AllTraded '0' 全部成交
			//// THOST_FTDC_OST_PartTradedQueueing '1' 部分成交还在队列中
			//// THOST_FTDC_OST_PartTradedNotQueueing '2' 部分成交不在队列中
			//// THOST_FTDC_OST_NoTradeQueueing '3' 未成交还在队列中
			//// THOST_FTDC_OST_NoTradeNotQueueing '4' 未成交不在队列中
			//// THOST_FTDC_OST_Canceled '5' 撤单
			//// THOST_FTDC_OST_Unknown 'a' 未知
			//// THOST_FTDC_OST_NotTouched 'b' 尚未触发
			//// THOST_FTDC_OST_Touched 'c' 已触发
			char OrderStatus;

			double Margin;
			double ExchMargin;
			double CurrMargin;   //占用保证金
			double Profit;
			bool WinLoseFlag;
			int OperatorType;   // 1 : 趋势单  2 : 日内单
			int HoldDays;

			char LocalDate[20];
			char TradingDay[20];
			char TradeDate[20];
			char TradeTime[20];
			char InsertDate[20];
			char InsertTime[20];


			char OrderLocalID[20];
			char ClearingPartID[20];
			char ClientID[20];
			char BusinessUnit[20];
			long SequenceNo;
			char LocalReqOrderTime[20];
			char LocalInsertTime[20];
			char LocalTradeTime[20];
			char RspOrderTime[20];
			char OrderOpenTime[20];
			char OrderCloseTime[20];
			char LocalUpdateTime[20];
			int Reaction_Count;

			int CloseFrontID;
			int CloseSessionID;
			int CloseErrorId;
			char CloseOrderSysID[21];
			char CloseOrderSubmitStatus;
			char CloseOrderStatus;

			char CloseTradingDay[20];
			char CloseOrderRef[20];
			char CloseTradeID[21];
			char CloseTradeDate[20];
			char CloseTradeTime[20];
			double CloseLimitPrice;
			double ClosePrice;
			int CloseVolume;
			int Close_ReactionCount;


			bool CancelFlag;
			int CancelCount;
		private :

	};



	class MongoOrder
		{

		public :

		MongoOrder();

			void Insert(OrderInfo orderinfo);
			void Check_Order(OrderInfo orderinfo);
			void Check_RspOrder(OrderInfo orderinfo);
			void Check_InvestPositionDetail(OrderInfo orderinfo);
			void Update_Order(OrderInfo orderinfo);
			void Update_ErrorOrder(OrderInfo orderinfo);
			void Update_ErrorCloseOrder(OrderInfo orderinfo);
			void Check_CloseOrder(OrderInfo orderinfo,const char *tradeid);
			void Check_ActionOrder(OrderInfo orderinfo);
			void Check_ActionCloseOrder(OrderInfo orderinfo);
			void Update_ActionOrder(OrderInfo orderinfo);
			void Update_ActionCloseOrder(OrderInfo orderinfo);
			void Update_CloseOrder(OrderInfo orderinfo);
			void Update_Trade(OrderInfo orderinfo);
			void Update_CloseTrade(OrderInfo orderinfo);
			bool Exist_OrderbyInstrumentID(const char * pinstrumentid,const char *tradingday);

			void Find_OrderInsertTimeoutList(vector<OrderInfo> &temp,const char *tradingday,int timeout_sec);
			void Find_OrderQueueingList(vector<OrderInfo> &temp,const char *tradingday);
			void Find_OrderQueueingStatusList(vector<OrderInfo> &temp,const char *tradingday,int status,int offset);
			bool Exist_QueueingOrder(const char* pinstrumentid,const char *tradingday);
			int  Count_ActionOrder(const char* pinstrumentid,const char *tradingday);
			int  Count_Trade(const char* pinstrumentid,const char *tradingday);
			int  Count_OrderbyStatus(const char* pinstrumentid,const char *tradingday,int status);
			void Find_OrderHoldList(vector<OrderInfo> &temp);
			void Find_CloseOrderList(vector<OrderInfo> &temp,const char * tradingday);
			void  Find_TodayPositionTradeList(vector<OrderInfo> &temp,const char * tradingday);

			OrderInfo  Find_byInstOrderRef(const char *pinstrumentid,const char *tradingday,const char *orderref);
			OrderInfo  Find_byInstrumentid(const char *pinstrumentid);
			OrderInfo  Find_TradebyInstrumentid(const char *pinstrumentid,const char *tradeid);
			int Count();
			int OrderInsertTimeOutCount();
			void DeleteAll();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOORDER_HPP_ */
