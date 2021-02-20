#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>

// CTP头文件
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"


#ifndef _MYSQLDATA_HPP_
#define _MYSQLDATA_HPP_

//#define DEBUG

enum e_OrderStatus{
	sendout=0,
	part_ok=1,
	all_ok=2,
	part_out=3,
	discard=4,
	ordererror=-1,
	cancle_send=20,
	cancle_accept=21,
	cancle_ok=22,
	cancle_error=-21,
	unknow=-999};

class User
{
public:

	char UserID[20];
	char BrokerID[20];
	char InvestorID[20];
	char Password[20];
	char StrategyName[50];
	bool Actived;
};

class  TradingDayData
	{

		public :
			char TradingDateTime[30];
			char TradingDay[30];   ///交易日
		   	int FrontID;
		   	int SessionID;
		   	char MaxOrderRef[13];
		   	char LoginTime[9];

			char DCETime[9];
			char CZCETime[9];
			char SHFETime[9];
			char INETime[9];
			int DifSeconds;

			bool SettleInfoConfirmStatus;
			bool DataReadyStatus;
			char ConfirmDate[30];
			char ConfirmTime[30];
			char LocalCreateTime[20];   	///
		   	char LocalUpdateTime[20];   	///


	};

class MAData
{
public:
	char Time[20];
	double MA59;
	double MA50;
	double MA49;
	double MA20;
	double MA19;
	double MA17;
	double MA10;
	double MA9;
	double MA8;
	double MA3;
	double MA50_X;

};

class EMAData
{
public:
	char Time[20];
	double EMA50;
	double EMA9;
	double EMA4;
	double EMA49;
	double EMA19;
	double EMA8;
	double EMA3;
	double EMA50_x;
	double EMA9_x;
	double EMA4_x;

};
class HisMAData
{
public:
	char Time[20];
	double MA50;
	double MA20;
	double MA10;
	double MA50Angel;
	char TrendStatus[10];
	char LongStartTime[20];
	char ShortStartTime[20];
	int LongCount;
	int ShortCount;

};

class HisEMAData
{
public:
	char Time[20];
	double EMA50;
	double EMA20;
	double EMA9;
	double EMA4;
	double EMA50Angel;
	char TrendStatus[10];
	char LongStartTime[20];
	char ShortStartTime[20];
	int LongCount;
	int ShortCount;

};
class  CycleData
{

	public :

		char InstrumentID[40];   ///合约代码
		char Date[20];   	///交易日
		char Time[20];   	///交易时间
		char HighDate[20];   	///最高价对应的日期
		char LowDate[20];   	///最低价对应的日期

		double High20;   ///
		double Low20;  //

		double HighN;   ///
		double LowN;  //

		double Highest;
		double Lowest;

		int HighCount ;
		int LowCount;


};

class ATRData
{
public :

	char Time[20];
	double TR;
	double ATR;   // 20日


};

class  OrderData
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[40];
			char TradeID[21];

			int CombOffsetFlag;
			int CombHedgeFlag;
			int Direction;
			int FrontID;
			int SessionID;
			int ErrorId;
			char OrderSysID[21];
			char OrderRef[20];
			char ExchangeID[10];
			char SignalName[30];
			char CancelSignalName[30];

			double LimitPrice;
			double Price;
			double StopPrice;
			double StopRange;
			double WinPrice;
			int VolumeTotalOriginal;
			int Volume;
			int Status;


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
			double CurrMargin;  //占用保证金

			char LocalDate[20];
			char TradingDay[20];
			char TradeDate[20];
			char TradeTime[20];
//			char OrderLocalID[20];
//			char ClearingPartID[20];
//			char ClientID[20];
//			char BusinessUnit[20];
//			long SequenceNo;
			char LocalUpdateTime[20];

		private :

	};

class  RecordData
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[40];
			int Direction;
			char ExchangeID[10];
			char OpenSignal[30];
			char CloseSignal[30];
			double OpenPrice;
			double ClosePrice;
			int Volume;
			double Margin;
			int Unit;
			double Profit;
			double Commission;
			double Asset;
			char WinLoseStatus[10];
			char OpenDateTime[20];
			char CloseDateTime[20];
			char LocalUpdateTime[20];

		private :

	};

class  HoldData
{

	public :
		///合约代码
		//TThostFtdcInstrumentIDType	InstrumentID;
		char InstrumentID[40];
		char ExchangeID[9];

		int Direction;
		int Volume;
		double OpenPriceAverage;
		double Highest;
		double Lowest;
		double StopPrice;
		double StopRange;
		double WinPrice;
		double ClosePriceAverage;
		int HedgeFlag;
		double Margin;
		double PreSettlementPrice;
		double SettlementPrice;

		double HighProfit;
		char TradingDay[20];
		char OpenDate[20];
		char LocalUpdateTime[20];


	private :

};

class  InvestPositionData
{

	public :
		///合约代码
		//TThostFtdcInstrumentIDType	InstrumentID;
		char InstrumentID[40];
		char ExchangeID[10];
		int YdPosition;
		int Position;
		int TodayPosition;
		int PosiDirection;
		int OpenVolume;
		int CloseVolume;
		double OpenPriceAverage;


		double OpenAmount;
		double CloseAmount;

		int PositionDate;
		double ExchangeMargin;
		double PositionProfit;
		double CloseProfit;
		double Commission;

		int SettlementID;
		int HedgeFlag;
		double UseMargin;
		double PreSettlementPrice;
		double SettlementPrice;

		char TradingDay[10];
		char LocalUpdateTime[20];

	private :

};
class InvestPositionBuffer//用来缓存持仓
{
public:
	std::string symbol;//合约

	//多头
	double longposition = 0;
	double longtodayposition = 0;
	double longydposition = 0;
	//空头
	double shortposition = 0;
	double shorttodayposition = 0;
	double shortydposition = 0;
};

class  SubscribeData
{
	public :
		 char InstrumentID[40];
		 bool Status;
		 char UserID[20];
		 char Processor[20];
		 char  LocalUpdateTime[20];
	private :

};



struct  InstrumentData
{
public :
	 char InstrumentID[41];
	  ///交易所代码 TThostFtdcExchangeIDType char[9]
	  char ExchangeID[27];
	  ///合约名称 TThostFtdcInstrumentNameType char[21]
	  char InstrumentName[41];
	  ///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
	  char ExchangeInstID[41];
	  ///产品代码 TThostFtdcInstrumentIDType char[31]
	  char ProductID[31];
	  ///产品类型 TThostFtdcProductClassType char
	  //// THOST_FTDC_PC_Futures '1' 期货
	  //// THOST_FTDC_PC_Options '2' 期权
	  //// THOST_FTDC_PC_Combination '3' 组合
	  //// THOST_FTDC_PC_Spot '4' 即期
	  //// THOST_FTDC_PC_EFP '5' 期转现
	  int ProductClass ;
	  ///交割年份 TThostFtdcYearType int
	  int DeliveryYear ;
	  ///交割月 TThostFtdcMonthType int
	  int DeliveryMonth ;
	  ///市价单最大下单量 TThostFtdcVolumeType int
	  int MaxMarketOrderVolume ;
	  ///市价单最小下单量 TThostFtdcVolumeType int
	  int MinMarketOrderVolume ;
	  ///限价单最大下单量 TThostFtdcVolumeType int
	  int MaxLimitOrderVolume ;
	  ///限价单最小下单量 TThostFtdcVolumeType int
	  int MinLimitOrderVolume ;
	  ///合约数量乘数 TThostFtdcVolumeMultipleType int
	  int VolumeMultiple ;
	  ///最小变动价位 TThostFtdcPriceType double
	  double PriceTick ;
	  ///创建日 TThostFtdcDateType char[9]
	  char CreateDate[27];
	  ///上市日 TThostFtdcDateType char[9]
	  char OpenDate[27];
	  ///到期日 TThostFtdcDateType char[9]
	  char ExpireDate[27];
	  ///开始交割日 TThostFtdcDateType char[9]
	  char StartDelivDate[27];
	  ///结束交割日 TThostFtdcDateType char[9]
	  char EndDelivDate[27];

	  ///合约生命周期状态 TThostFtdcInstLifePhaseType char
	  //// THOST_FTDC_IP_NotStart '0' 未上市
	  //// THOST_FTDC_IP_Started '1' 上市
	  //// THOST_FTDC_IP_Pause '2' 停牌
	  //// THOST_FTDC_IP_Expired '3' 到期
	  int InstLifePhase ;
	  ///当前是否交易 TThostFtdcBoolType int
	  int IsTrading ;
	  ///持仓类型 TThostFtdcPositionTypeType char
	  //// THOST_FTDC_PT_Net '1' 净持仓
	  //// THOST_FTDC_PT_Gross '2' 综合持仓
	  char PositionType ;
	  ///持仓日期类型 TThostFtdcPositionDateTypeType char
	  //// THOST_FTDC_PDT_UseHistory '1' 使用历史持仓
	  //// THOST_FTDC_PDT_NoUseHistory '2' 不使用历史持仓
	  char PositionDateType ;
	  ///多头保证金率 TThostFtdcRatioType double
	  double LongMarginRatio;
	  ///空头保证金率 TThostFtdcRatioType double
	  double ShortMarginRatio ;
	  ///是否使用大额单边保证金算法 TThostFtdcMaxMarginSideAlgorithmType char
	  //// THOST_FTDC_MMSA_NO '0' 不使用大额单边保证金算法
	  //// THOST_FTDC_MMSA_YES '1' 使用大额单边保证金算法
	  char MaxMarginSideAlgorithm ;
	  char UpdateTime[27];
	  char LocalUpdateTime[27];
};


class  DepthMarketData
	{

		public :

		   ///交易日
		   	//TThostFtdcDateType	TradingDay;
		   	char TradingDay[27];
		   	///合约代码
		   	//TThostFtdcInstrumentIDType	InstrumentID;
		   	char InstrumentID[40];
		   	///交易所代码
		   	//TThostFtdcExchangeIDType	ExchangeID;
		   	char ExchangeID[27];
		   	///合约在交易所的代码
		   	//TThostFtdcExchangeInstIDType	ExchangeInstID;
		   	char ExchangeInstID[27];
		   	///产品代码 TThostFtdcInstrumentIDType char[31]
		   	char ProductID[10];
		   	///最新价
		   	//TThostFtdcPriceType	LastPrice;
		   	double LastPrice ;
		   	///上次结算价
		   	//TThostFtdcPriceType	PreSettlementPrice;
		   	double PreSettlementPrice ;
		   	///昨收盘
		   	//TThostFtdcPriceType	PreClosePrice;
		   	double PreClosePrice ;
		   	///昨持仓量
		   	//TThostFtdcLargeVolumeType	PreOpenInterest;
		   	double PreOpenInterest;
		   	///今开盘
		   	//TThostFtdcPriceType	OpenPrice;
		   	double OpenPrice;
		   	///最高价
		   	//TThostFtdcPriceType	HighestPrice;
		   	double HighestPrice;
		   	///最低价
		   	//TThostFtdcPriceType	LowestPrice;
		   	double LowestPrice;
		   	///数量
		   	//TThostFtdcVolumeType	Volume;
		   	int Volume;
		   	///成交金额
		   	//TThostFtdcMoneyType	Turnover;
		   	double Turnover;
		   	///持仓量
		   	//TThostFtdcLargeVolumeType	OpenInterest;
		   	int OpenInterest ;

		   	///今收盘
		   	//TThostFtdcPriceType	ClosePrice;
		   	double ClosePrice;
		   	///本次结算价
		   	//TThostFtdcPriceType	SettlementPrice;
		   	double SettlementPrice;
		   	///涨停板价
		   	//TThostFtdcPriceType	UpperLimitPrice;
		   	double UpperLimitPrice;
		   	///跌停板价
		   	//TThostFtdcPriceType	LowerLimitPrice;
		   	double LowerLimitPrice;
		   	///昨虚实度
		   	//TThostFtdcRatioType	PreDelta;
		   	double PreDelta;
		   	///今虚实度
		   	//TThostFtdcRatioType	CurrDelta;
		   	double CurrDelta;
		   	///最后修改时间
		   	//TThostFtdcTimeType	UpdateTime;
		   	char UpdateTime[27];
		   	int UpdateMillisec;


//		   	///申买价一
		   	double	BidPrice1;
//		   	///申买量一
		   	double	BidVolume1;
//		   	///申卖价一
		   	double	AskPrice1;
//		   	///申卖量一
		   	double	AskVolume1;
//		   	///申买价二
		   	double	BidPrice2;
//		   	///申买量二
		   	double	BidVolume2;
//		   	///申卖价二
		   	double	AskPrice2;
//		   	///申卖量二
		   	double	AskVolume2;
//		   	///申买价三
		   	double	BidPrice3;
//		   	///申买量三
		   	double	BidVolume3;
//		   	///申卖价三
		   	double	AskPrice3;
//		   	///申卖量三
		   	double	AskVolume3;
//		   	///申买价四
		   	double	BidPrice4;
//		   	///申买量四
		   	double	BidVolume4;
//		   	///申卖价四
		   	TThostFtdcPriceType	AskPrice4;
//		   	///申卖量四
		   	double	AskVolume4;
//		   	///申买价五
		   	double	BidPrice5;
//		   	///申买量五
		   	TThostFtdcVolumeType	BidVolume5;
//		   	///申卖价五
		   	double	AskPrice5;
//		   	///申卖量五
		   	double	AskVolume5;
//		   	///当日均价
//		   	TThostFtdcPriceType	AveragePrice;

		   	double AveragePrice;
		   	///业务日期
		   	//TThostFtdcDateType	ActionDay;
		   	char ActionDay[27];

			char LocalUpdateTime[27];

			double LongMarginRatio;
			double ShortMarginRatio;
			double Margin;
			///合约数量乘数 TThostFtdcVolumeMultipleType int
			int VolumeMultiple ;
			///最小变动价位 TThostFtdcPriceType double
			double PriceTick ;
			char Section[30];

			char DayNight[30];


			double OpenPreSettleRate;
			double LastOpenRate;
			double UpdownRate;
			double Sedimentary;
			double LastSedimentary;
			double SedimentaryIORate;
			double AddOpenInterestRate;


	};


class MainContractData
	{

		public:
			///交易日
			   	//TThostFtdcDateType	TradingDay;
			char TradingDay[27];
			///业务日期
			//TThostFtdcDateType	ActionDay;
			char ActionDay[27];
			char UpdateTime[10];
			//
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[40];
			///产品代码 TThostFtdcInstrumentIDType char[31]
			char ProductID[30];

			char Section[30];

			char DayNight[30];

			  ///合约名称 TThostFtdcInstrumentNameType char[21]
			char InstrumentName[40];
			 ///交易所代码
			//TThostFtdcExchangeIDType	ExchangeID;
			char ExchangeID[27];
			///合约数量乘数 TThostFtdcVolumeMultipleType int
			int VolumeMultiple ;
			///最小变动价位 TThostFtdcPriceType double
			double PriceTick ;
			///涨停板价
			//TThostFtdcPriceType	UpperLimitPrice;
			double UpperLimitPrice;
			///跌停板价
			//TThostFtdcPriceType	LowerLimitPrice;
			double LowerLimitPrice;
			int MaxOpenInterest;   // 合约历史最高持仓量
			int OpenInterest ;
			double LongMarginRatio;
			double ShortMarginRatio;
			int Volume ;  // 成交量
			double LastPrice;
			double Sedimentary;
			int DaylineCount;
			int ErrorDaylineCount;

			//次月主力合约
			char InstrumentID_next[40];
			char InstrumentName_next[40];
			int OpenInterest_next ;
			double UpperLimitPrice_next;
			double LowerLimitPrice_next;
			int Volume_next ;
			double LastPrice_next;
			double Sedimentary_next;
			int DaylineCount_next;
			int ErrorDaylineCount_next;

			// 当前交易的合约ID
			char InstrumentID_trade[40];
			char LocalUpdateTime[30];


	};

class ProductData
	{
	public:
		char ProductID[30];
		char ExchangeID[30];
		char Section[20];
		char DayNight[20];


	};

class  ExchangeData
	{

		public :
			char ExchangeID[9];
			char ExchangeName[61];
			int ExchangeProperty;
			int  ExchangeP;

		private :

	};


class AssetData
{
public :

	char TradingDay[20];
	char AccountID[20];
	double PreBalance;    // 上次结算准备金
	double Withdraw;  //出金
	double Deposit;  //入金
	double Available;   ///可用金额
	double CurrMargin;  // CurrMargin
	double Commission ;
	double StaticEquity;
	double DynamicEquity;
	double HighestProfit;
	double LowestProfit;
	double Profit;
	double HoldProfit;
	double PositionProfit;
	double CloseProfit;
	double TotalCaptial;
	char LocalUpdatetime[20];

};

class SedimentaryData
{
public :

	char DateTime[20];
	double TotalSedimentary;

	double Sedimentary_Black;
	double Sedimentary_Oil;
	double Sedimentary_Metal;
	double Sedimentary_Stock;
	double Sedimentary_Corn;
	double Sedimentary_Single;
	double Sedimentary_Finance;
	double Sedimentary_Feed;
	double Sedimentary_Precious;
	double Sedimentary_CookOil;

};

class MarketMinuteData
{
public :
	char MinuteTime[20];
	double MaxUpRate;
	char MaxUpInstrumentID[20];
	double MinDownRate;
	char MinDownInstrumentID[20];

};
class MinuteLineData
{
public :
	char MinuteTime[20];
	double price;
	double Amount;
	int Volume;
	double AvePrice;
	int OpenInterest;
};
class  Mysql_MinData
{

	public :
		///交易日
		char MinuteTime[20];
		///最新价
		double OpenPrice;
		double HighestPrice;
		double LowestPrice ;
		double ClosePrice ;
		double AvePrice;
		//TThostFtdcVolumeType	Volume;
		int Volume;
		double Amount;
		///持仓量
		//TThostFtdcLargeVolumeType	OpenInterest;
		double OpenInterest ;
		char LastTickTime[20];
};

class  RMinData
{
	public :
	char InstrumentID[40];
	char MinuteTime[20];
	double OpenPrice;
	double HighestPrice;
	double LowestPrice ;
	double ClosePrice ;
	double AvePrice;
	int Volume;
	double Amount;
	int OpenInterest ;
	char LastTickTime[20];
};
class TypeMinuteLineData
{
public :
	char MinuteTime[20];
	int Number;
	double price;
	double Amount;
	int Volume;
	double AvePrice;
	int OpenInterest;
};
class  SqlAveragePriceData
	{

		public :

			char Time[20];   	///交易日
		   	int OpenInterest;   ///每日分钟线 最后一分钟持仓量， 即每日持仓量
		   	int StartOpenInterest;  // 分钟线 第一分钟持仓量
		   	double AveragePrice ;


	};

class TestRecordData
{
public:
	char ID[60];
	char OpenDateTime[20];
	char OpenDate[20];
	char InstrumentID[40];
	char Section[20];
	int Direction;
	int Status;   // 0  : 开仓   1: 平仓
	double AverageUpDownrate;
	double pATR;
	int OpenOPI;
	int CloseOPI;
	double AddOPIRate;
	double PreSettlePrice;
	double DayOpenPrice;
	double EMA50;
	double EMA20;
	double EMA9;
	double EMA4;
	double EMA50_X;
	double EMA20_X;
	double EMA9_X;
	double EMA4_X;

	double Open_LastOpenRate;  //开仓时 最新价和开盘价的涨跌幅
	double Open_VolOpiRate;	//开仓时
	double Open_AddOPIRate;	//开仓时
	double Close_VolOpiRate;
	double Close_AddOPIRate;
	double Open_LARate;   // 开盘时  LastOpenRate   AddOPIRate 比值;

	bool HalfStop;
	double OpenPrice ;
	char OpenSignal[30];
	int Volume;
	double ClosePrice;
	int CloseVolume;
	double StopPrice;
	double WinPrice;
	double Highest;
	double Lowest;
	char CloseDateTime[20];
	char CloseDate[20];
	double Sedimentary;
	double Profit;
	double Commission;  //佣金
	double Margin;   //保证金比例
	double UseMargin;  //占用保证金
	double UseMarginRate;   //占用保证金比例
	double Asset;
	char CloseSignal[30];
	char FiveDayLineType[10];

};


class  PositionDetailData
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[40];
			char ExchangeID[10];
			double MarginRateByMoney;
			double MarginRateByVolume;
			double OpenPrice;
			int Volume;
			double CloseProfitByTrade;
			double CloseProfitByDate;
			double PositionProfitByTrade;
			double PositionProfitByDate;
			double Margin;
			double ExchMargin;
			int Direction;
			double CloseAmount;
			int CloseVolume;
			char TradingDay[20];
			char OpenDate[20];
			char TradeID[20];
			char  LocalUpdateTime[20];
		private :

	};
class  CycleNRefData{
	public :
		char InstrumentID[40];   ///合约代码
		char Time[20];   	///交易日

		double Open;   ///开盘价
		double Highest;
		char Highest_Time[20];   	///交易日

		double Lowest;
		char Lowest_Time[20];   	///交易日
		double Close;
		double LastClose ;

		int Start_OpenInterest;
		int End_OpenInterest;
		double TotalVolume;   //成交量
		double Amount ;  ///成交金额

};
class  MinuteStaticData{
	public :
		char InstrumentID[40];   ///合约代码
		char Time[20];   	///交易日

		int MinuteCount;
		int LongCount;
		int ShortCount;
};
class  HxDayLineData{
	public :
		char InstrumentID[40];   ///合约代码
		char Time[20];   	///交易日
		long int uTIme;
		double Open;   ///开盘价
		double High;
		double Low ;
		double Close;
		double LastClose ;
		double PreSettlementPrice;
		double SettlementPrice;
		int OpenInterest;
		double Volume;   //成交量
		double Amount ;  ///成交金额

};
class  FiveDayLineData{
	public :
		char InstrumentID[40];   ///合约代码
		char Time[20];   	///交易日
		int Number;
		double Open;   ///开盘价
		double High;
		double Low ;
		double Close;
		double LastClose ;
		double PreSettlementPrice;
		double SettlementPrice;
		int OpenInterest;
		double Volume;   //成交量
		double Amount ;  ///成交金额

};
class SectionAssessData{

public:
	char Section[40];
	int Priority;
	bool Active;
	int TotalProductNumber;
	int ThreshHoldNumber;
	int ProductRiskNumber;
	int ProductHoldNumber;
	int PD_Long_SignalNumber;
	int PD_Short_SignalNumber;
	int InsHoldNumber;
	char PD_Long_InstrumentId[40];
	char PD_Short_InstrumentId[40];
	double PD_Long_Inst_UpdownRate;
	double PD_Short_Inst_UpdownRate;

	double Sedimentary;
	double SedimentaryIO;
	double SedimentaryIORate;
	double AverageUpDownRate;
	double AbsUpDownRate;

	int Result_Direction;
	char Result_InstrumentId[40];
	char UpdateTime[20];

};
class  SignalAssessData{

	public :
		char Time[30];
		char TradingDay[30];   ///交易日
		int TotalSignalNumber;
		int TotalSectionNumber;

		int Total_SingleDirection_SignalNumber;
		int Total_SingleDirection_SectionNumber;
		int SingleLong_SectionNumber;
		int SingleShort_SectionNumber;
		int Single_Long_SignalNumber;
		int Single_Short_SignalNumber;

		int High_Long_SignalNumber;
		int High_Short_SignalNumber;
		char High_Long_Section[40];
		char High_Short_Section[40];
		char High_Long_InstrumentID[40];
		char High_Short_InstrumentID[40];

		int Long_SignalNumber;
		int Short_SignalNumber;

		char RecommandInstrumentID[40];   ///最终推荐信号
		int RecommandDirection;   ///最终推荐信号方向
		char LocalUpdateTime[20];
};


class GlobalViewData{

public:
	char Time[20];
	char TradingDay[20];
	int Status;  // 0 多头 1  空头 2 震荡
	double TotalCapital;  //整体市场沉淀资金总和
	double TotalSedimentaryIO;
	double TotalSedimentaryIORate;
	double TotalSedimentaryIn;
	double TotalSedimentaryOut;
	double TotalSedimentaryInRate;
	double TotalSedimentaryOutRate;

	double MaxSedimentaryIO;
	char MaxSedimentaryIOIns[40];
	double MinSedimentaryIO;
	char MinSedimentaryIOIns[40];

	double MaxSedimentaryIORate;
	char MaxSedimentaryIORateIns[40];
	double MinSedimentaryIORate;
	char MinSedimentaryIORateIns[40];

	double MaxSedimentaryIORate_Section;  //资金流入流出最大幅度板块的资金量
	double MaxSedimentaryIO_Section;  //资金流入流出最大板块的资金量
	char MaxSedIO_SectionName[40];  //资金流入流出最大板块的资金量
	char MaxSedIORate_SectionName[40];  //资金流入流出最大板块的资金量

	double MinSedimentaryIORate_Section; //资金流入流出最小幅度板块的资金量
	double MinSedimentaryIO_Section;  //资金流入流出最大板块的资金量
	char MinSedIO_SectionName[40]; //资金流入流入最小板块名
	char MinSedIORate_SectionName[40]; //资金流入流入最小板块名

	int CurrentCount_InstrumentID; //当前参与计算全局市场的合约总数
	int Up_Count;
	int Down_Count;
	double LongWeight;
	double ShortWeight;

	double Total_Up;
	double Total_Down;
	double Average_Up;
	double Average_Down;


	double MaxUpDown;    //最高涨跌幅
	double MinUpDown;   //最低涨跌幅
	char MaxUpDownIns[40];  //涨跌幅最高合约
	char MinUpDownIns[40];  //涨跌幅最低合约
	char AbsMaxUpDownIns[40];
	double AbsMaxUpDown;
	int StatusChangeReason;
	// 0 - 第一笔初始记录
	// 1 - 多头，震荡，空头状态发生变化
	//2- 资金流入流出最大合约发生变化
	//3- 涨跌幅绝对值最大合约发生变化

};
#endif /* _MYSQLDATA_HPP_ */
