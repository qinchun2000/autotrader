
#ifndef _MYSQLORDER_HPP
#define _MYSQLORDER_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <string>
#include <memory>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"

#include "codeutil.hpp"
#include "dateutil.hpp"
#include <boost/regex.hpp>
#include "depthmarket.hpp"
#include "mysqldata.hpp"
#include <json/json.h>
#include "mysqlbase.hpp"

using namespace std;
using namespace sql;



class MysqlOrder:public MysqlBase
{

public:
	MysqlOrder(const char*flag,const char * userid);



  void CreateTable(const char*tablename);
  void CreateTable();
  void Insert(OrderData data);
  void UpdatebyFailReq(OrderData data);

  void NewInvestPositionDetail(OrderData data);
  bool Exist_OrderbyInstrumentid(const char* pinstrumentid,const char *tradeid);
  bool Exist_OrderbyInsOrderRef(const char* pinstrumentid,
		  const char *orderref,const char*tradingday);
  bool Exist_OrderbyInstrumentidStatus(const char* pinstrumentid,
		  const char* tradingday,int status);
  bool Exist_CloseOrderbySection(const char* section,const char* tradingday);

  void NewOrderData_byRspQryOrder(OrderData data);
  void NewOrderData_byRtnOrder(OrderData data);
  void UpdateOrderData_byRtnOrder(OrderData data);

  void NewOrderData_byRtnTrade(OrderData data);
  void UpdateOrderData_byRtnTrade(OrderData data);
  void UpdateOrderData_byErrRtnOrder(OrderData data);
  void UpdateOrderActionData(OrderData data);
  void UpdateOrderData_byOnRspOrderInsert(OrderData data);

  int  Count_OrderListbyStatus(const char* pinstrumentid,const char *tradingday,int status);
  void Find_OrderListsbyStatus(vector<OrderData> &temp,const char *tradingday,int status);
  void Find_AllOrderListsbyStatus(vector<OrderData> &temp,int status);
  void Find_AllOrderListsbyDateTimeStatus(vector<OrderData> &temp,const char* instrumentid,const char* datetime,int status);
  void Find_QueueOrderLists(vector<OrderData> &temp,const char *tradingday);

  int Get_DoneOrderListsVolume(const char* pinstrumentid,int direction,const char *tradingday);
  int Get_QueueOrderListsVolume(const char* pinstrumentid,int direction,const char *tradingday);
  int Get_QueueOrderActionListsVolume(const char* pinstrumentid,int direction,const char *tradingday);

  int Count_QueueOrderProduct(const char *tradingday);
  int Count_QueueActionOrderProduct(const char *tradingday);

  void Find_AllLists(vector<OrderData> &temp);
  void Find_ListsByDate(vector<OrderData> &temp,const char * day);

  int  Get_VolumebyStatus(const char* pinstrumentid,const char *tradingday,int status);
  int  Get_TotalVolumebyDate(const char* pinstrumentid,const char *tradingday);

  OrderData  Find_OrderbyInsTradingday(const char* pinstrumentid,const char*tradingday);
  OrderData  Find_OrderbyInsOrderRef(const char* pinstrumentid,const char *orderref,const char*tradingday);
  OrderData  Find_OrderbyInsTradeID(const char* pinstrumentid,const char *tradeid,const char*tradingday);
  int  Exist_CloseOrderbyInsTradingDay(const char* pinstrumentid,const char *tradingday);
  OrderData  Find_CloseOrderbyInsTradingDay(const char* pinstrumentid,const char*tradingday);
  std::string GetLastLocalUpdateTime();



private:


};


#endif

