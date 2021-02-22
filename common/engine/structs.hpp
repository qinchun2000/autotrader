#ifndef _STRUCTS_H
#define _STRUCTS_H

#include<string>
#include<iostream>
#include<vector>
#include<mutex>
#include<map>

//#include"utils.hpp"
/*************************************/
#define PRICETYPE_LIMITPRICE "pricetypelimit"
#define PRICETYPE_MARKETPRICE "pricetypemarket"
#define PRICETYPE_FAK "FAK"
#define PRICETYPE_FOK "FOK"
#define DIRECTION_NONE "directionnone"
#define DIRECTION_LONG "directionlong"
#define DIRECTION_SHORT "directionshort"
#define DIRECTION_UNKNOWN "unknow"
#define DIRECTION_NET "directionnet"
#define OFFSET_NONE "offsetnone"
#define OFFSET_OPEN "offsetopen"
#define OFFSET_CLOSE "offsetclose"
#define OFFSET_CLOSETODAY "offsetclosetoday"
#define OFFSET_CLOSEYESTERDAY "offsetcloseyesterday"
#define OFFSET_UNKNOWN "offsetunknow"
#define EXCHANGE_SHFE "SHFE"
#define STATUS_ALLTRADED "orderalltraded"
#define STATUS_CANCELLED "ordercanceld"
#define STATUS_WAITING "orderwaiting"

enum Mode
{
	RealMode = 0,
	BacktestMode = 1
};

struct SubscribeReq
{
	std::string symbol;
	std::string exchange;
	std::string productClass;
	std::string currency;
	std::string expiry;
	double strikePrice;
	std::string optionType;
};

struct OrderReq
{
	std::string symbol;
	std::string exchange;
	double price;
	double volume;
	std::string priceType;
	std::string direction;
	std::string offset;
	std::string productClass;
	std::string currency;
	std::string expiry;
	double strikePrice;
	std::string optionType;
	std::string strateyName;
};

struct CancelOrderReq
{
	std::string symbol;
	std::string exchange;
	std::string orderID;
	std::string frontID;
	std::string sessionID;
};

struct Portfolio_Result_Data
{

	double	maxCapital;
	double	drawdown;
	double	Winning;	//平仓全部盈利
	double	Losing;		//平仓全部亏损
	double  totalwinning; //平仓净盈亏  winning-losing
	double  lastdayTotalwinning;
	double  delta;
	int totalResult;

	double holdingwinning;//持仓盈亏
	double holdingposition;
	double holdingprice;

	double holding_and_totalwinning;	//totalwinning+holdingwinning

	double portfolio_winning;			//所有策略 合约的盈利
};

//class JSData
//{
//public:
//	JSData(std::string type) :m_datatype(type)
//	{}
//	std::string GetDataType()
//	{
//		return m_datatype;
//	}
//private:
//	std::string m_datatype;
//};
//
//class TickData :public JSData
//{
//public:
//	int getminute()
//	{
//		std::string timecopy = time;
//		std::vector<std::string>v;
//		char* pch = strtok(const_cast<char*>(timecopy.c_str()), ":.");
//		while (pch != NULL)
//		{
//			v.push_back(pch);
//			pch = strtok(NULL, ":.");
//		}
//		if (v.size() == 4)
//		{
//			return atoi(v[1].c_str());//返回分钟
//		}
//		else
//		{
//			std::cout << "无法返回分钟，这个时间戳有问题！" << std::endl;
//		}
//	}
//	int gethour()
//	{
//		std::string timecopy = time;
//		std::vector<std::string>v;
//		char* pch = strtok(const_cast<char*>(timecopy.c_str()), ":.");
//		while (pch != NULL)
//		{
//			v.push_back(pch);
//			pch = strtok(NULL, ":.");
//		}
//		if (v.size() == 4)
//		{
//			return atoi(v[0].c_str());//返回小时
//		}
//		else
//		{
//			std::cout << "无法返回小时，这个时间戳有问题！" << std::endl;
//		}
//	}
//	TickData() :JSData("tick")
//	{};
//	std::string symbol;
//	std::string exchange;
//	std::string gatewayname;
//	//成交数据
//	double lastprice;//最新成交价
//	double volume;//总成交量
//	double openInterest;//持仓量
//	long long unixdatetime;	//时间戳在提取mongodb数据时候会除以1000
//	std::string date;//日期
//	std::string time;//时间
//	//常规行情
//	double openPrice;//今日开
//	double highPrice;//今日高
//	double lowPrice;//今日低
//	double preClosePrice;//昨收
//
//	double upperLimit;//涨停
//	double lowerLimit;//跌停
//	//五档行情
//	double bidprice1;
//	double bidprice2;
//	double bidprice3;
//	double bidprice4;
//	double bidprice5;
//
//	double askprice1;
//	double askprice2;
//	double askprice3;
//	double askprice4;
//	double askprice5;
//
//	double bidvolume1;
//	double bidvolume2;
//	double bidvolume3;
//	double bidvolume4;
//	double bidvolume5;
//
//	double askvolume1;
//	double askvolume2;
//	double askvolume3;
//	double askvolume4;
//	double askvolume5;
//};

//class BarData :public JSData
//{
//public:
//	int getminute()
//	{
//		std::string timecopy = time;
//		std::vector<std::string>v;
//		char* pch = strtok(const_cast<char*>(timecopy.c_str()), ":.");
//		while (pch != NULL)
//		{
//			v.push_back(pch);
//			pch = strtok(NULL, ":.");
//		}
//		if (v.size() == 4||v.size()==3)
//		{
//			return atoi(v[1].c_str());//返回分钟
//		}
//		else
//		{
//			std::cout << "无法返回分钟，这个时间戳有问题！" << std::endl;
//		}
//	}
//	int gethour()
//	{
//		std::string timecopy = time;
//		std::vector<std::string>v;
//		char* pch = strtok(const_cast<char*>(timecopy.c_str()), ":.");
//		while (pch != NULL)
//		{
//			v.push_back(pch);
//			pch = strtok(NULL, ":.");
//		}
//		if (v.size() == 4||v.size()==3)
//		{
//			return atoi(v[0].c_str());//返回小时
//		}
//		else
//		{
//			std::cout << "无法返回小时，这个时间戳有问题！" << std::endl;
//		}
//	}
//	BarData() :JSData("bar")
//	{};
//	std::string symbol;
//	std::string exchange;
//	double open = 0;
//	double high = 0;
//	double low = 999999;
//	double close = 0;
//
//	std::string date;
//	std::string time;
//	long long unixdatetime;	//时间戳在提取mongodb数据时候会除以1000
//
//	double openPrice = 0;//今日开
//	double highPrice = 0;//今日高
//	double lowPrice = 999999;//今日低
//	double preClosePrice = 0;//昨收
//
//	double upperLimit = 0;//涨停
//	double lowerLimit = 0;//跌停
//
//	double volume = 0;
//	double openInterest = 0;
//};

//class DailyBar :public JSData
//{
//public:
//	int getminute()
//	{
//		std::string timecopy = time;
//		std::vector<std::string>v;
//		char* pch = strtok(const_cast<char*>(timecopy.c_str()), ":.");
//		while (pch != NULL)
//		{
//			v.push_back(pch);
//			pch = strtok(NULL, ":.");
//		}
//		if (v.size() == 4)
//		{
//			return atoi(v[1].c_str());//返回分钟
//		}
//		else
//		{
//			std::cout << "无法返回分钟，这个时间戳有问题！" << std::endl;
//		}
//	}
//	int gethour()
//	{
//		std::string timecopy = time;
//		std::vector<std::string>v;
//		char* pch = strtok(const_cast<char*>(timecopy.c_str()), ":.");
//		while (pch != NULL)
//		{
//			v.push_back(pch);
//			pch = strtok(NULL, ":.");
//		}
//		if (v.size() == 4)
//		{
//			return atoi(v[0].c_str());//返回小时
//		}
//		else
//		{
//			std::cout << "无法返回小时，这个时间戳有问题！" << std::endl;
//		}
//	}
//	DailyBar() :JSData("dailybar")
//	{};
//	std::string symbol;
//	std::string exchange;
//	double open;
//	double high;
//	double low;
//	double close;
//
//	std::string date;
//	std::string time;
//	long long unixdatetime;//毫秒级UNIX时间戳
//
//	double volume;
//	double openInterest;
//};

/*********************************事件宏定义**********************************************/
#define EVENT_QUIT "equit"//退出事件
#define EVENT_TIMER "etimer"//循环1秒钟事件，用来不断的查持仓，防止CTP流控
#define EVENT_TICK "etick"
#define EVENT_TRADE "etrade"
#define EVENT_ORDER "eorder"
#define EVENT_POSITION "ePosition"
#define EVENT_ACCOUNT "eAccount"
#define EVENT_CONTRACT "eContract"
#define EVENT_ERROR "eError"
#define EVENT_LOG "elog"
#define EVENT_LOADSTRATEGY "eloadstrategy"
#define EVENT_UPDATESTRATEGY "eupdatestrategy"
#define EVENT_UPDATEPORTFOLIO "eupdateportfolio"
//回测事件
#define EVENT_BACKTEST_TICK "ebacktesttick"
#define EVENT_BACKTEST_BAR "ebacktestbar"

///////////////////////////////////////


/**********************************事件类型*****************************************/
class  Event //结构中定义事件
{
public:
	Event(std::string type) :m_eventtype(type)
	{}
	std::string GetEventType()
	{
		return m_eventtype;
	}
private:
	std::string m_eventtype;
};

class  Event_Exit :public Event
{
public:
	Event_Exit() :Event(EVENT_QUIT)
	{}
};

class   Event_Timer :public Event
{
public:
	Event_Timer() :Event(EVENT_TIMER)
	{}
};

class   Event_Tick :public Event
{
public:
	Event_Tick() :Event(EVENT_TICK)
	{}
	//代码
	std::string symbol;
	std::string exchange;
	std::string gatewayname;
	//成交数据
	double lastprice=0.0;//最新成交价
	double volume=0.0;//总成交量
	double openInterest=0.0;//持仓量
	std::string date;//日期
	std::string time;//时间
	//常规行情
	double openPrice=0.0;//今日开
	double highPrice=0.0;//今日高
	double lowPrice=0.0;//今日低
	double preClosePrice=0.0;//昨收

	double upperLimit=0.0;//涨停
	double lowerLimit=0.0;//跌停
	//五档行情
	double bidprice1=0.0;
	double bidprice2=0.0;
	double bidprice3=0.0;
	double bidprice4=0.0;
	double bidprice5=0.0;

	double askprice1=0.0;
	double askprice2=0.0;
	double askprice3=0.0;
	double askprice4=0.0;
	double askprice5=0.0;

	double bidvolume1=0.0;
	double bidvolume2=0.0;
	double bidvolume3=0.0;
	double bidvolume4=0.0;
	double bidvolume5=0.0;

	double askvolume1=0.0;
	double askvolume2=0.0;
	double askvolume3=0.0;
	double askvolume4=0.0;
	double askvolume5=0.0;

};

class   Event_Trade :public Event
{
public:
	Event_Trade() :Event(EVENT_TRADE)
	{}
	//代码编号
	std::string symbol;
	std::string exchange;
	std::string tradeID;   //交易编号
	std::string orderID;  //订单编号
	std::string gatewayname;
	//成交相关
	std::string direction;//方向
	std::string offset; //成交开平仓
	double price=0.0;//成交价格
	double volume=0.0;//成交量
	std::string tradeTime;//成交时间
};

class   Event_Order :public Event
{
public:
	Event_Order() :Event(EVENT_ORDER)
	{}
	//编号相关
	std::string symbol;
	std::string exchange;
	std::string orderID;//订单编号
	std::string gatewayname;
	//报单相关
	std::string direction;//方向
	std::string offset;//开平方向
	double price=0.0; //报单价格
	double totalVolume=0.0;//报单总量
	double tradedVolume=0.0;//成交数量
	std::string status;//报单状态

	std::string orderTime;//发单时间
	std::string cancelTime;//撤单时间

	int frontID=0;//前置机编号
	int sessionID=0;//连接编号

};

class   Event_Contract :public Event
{
public:
	Event_Contract() :Event(EVENT_CONTRACT)
	{}
	std::string symbol;
	std::string exchange;
	std::string name;                   //合约中文名
	std::string gatewayname;
	std::string productClass;           //合约类型
	int size=0;                           //合约大小
	double priceTick=0.0;					//合约最小价格Tick

	double strikePrice=0.0;					//行权价
	std::string underlyingSymbol;       //标的物合约代码
	std::string optionType;				//期权类型
};

class   Event_Position :public Event
{
public:
	Event_Position() :Event(EVENT_POSITION)
	{
		position = 0;
		todayPosition = 0;
		ydPosition = 0;
		todayPositionCost = 0;
		ydPositionCost = 0;
		price = 0;
		frozen = 0;
	}
	std::string symbol;
	std::string direction;
	std::string gatewayname;
	double position;
	double todayPosition;
	double ydPosition;
	double todayPositionCost;
	double ydPositionCost;
	double price;
	double frozen;
};

class   Event_Account :public Event
{
public:
	Event_Account() :Event(EVENT_ACCOUNT)
	{}
	std::string gatewayname;
	std::string accountid;
	double preBalance=0.0;//昨日账户结算净值
	double balance=0.0;//账户净值
	double available=0.0;//可用资金
	double commission=0.0;//今日手续费
	double margin=0.0;//保证金占用
	double closeProfit=0.0;//平仓盈亏
	double positionProfit=0.0;//持仓盈亏
};

class   Event_Error :public Event
{
public:
	Event_Error() :Event(EVENT_ERROR)
	{}
	std::string errorID;//错误代码
	std::string errorMsg;//错误信息
	std::string additionalInfo;//附加信息
	std::string gatewayname;
//	std::string errorTime = Utils::getCurrentSystemTime();
};

class   Event_Log :public Event
{
public:
	Event_Log() :Event(EVENT_LOG)
	{}
	std::string msg;//log信息
	std::string gatewayname;
//	std::string logTime = Utils::getCurrentSystemTime();
};

class   Event_UpdateStrategy :public Event
{
public:
	Event_UpdateStrategy() :Event(EVENT_UPDATESTRATEGY)
	{}
	std::string strategyname;
	std::map<std::string, std::string>parammap;
	std::map<std::string, std::string>varmap;
};

class   Event_UpdatePortfolio :public Event
{
public:
	Event_UpdatePortfolio() :Event(EVENT_UPDATEPORTFOLIO)
	{}
	std::string strategyname;
	std::string symbol;
	time_t datetime{0};
	Portfolio_Result_Data Portfoliodata;
	std::vector<int>strategyrows;
};

class   Event_LoadStrategy :public Event
{
public:
	Event_LoadStrategy() :Event(EVENT_LOADSTRATEGY)
	{}
	std::string strategyname;
	std::map<std::string, std::string>parammap;
	std::map<std::string, std::string>varmap;
};

//class   Event_Backtest_Tick :public Event
//{
//public:
//	Event_Backtest_Tick() :Event(EVENT_BACKTEST_TICK)
//	{}
//	TickData tick;
//};
//
//class   Event_Backtest_Bar :public Event
//{
//public:
//	Event_Backtest_Bar() :Event(EVENT_BACKTEST_BAR)
//	{}
//	BarData bar;
//};
#endif
