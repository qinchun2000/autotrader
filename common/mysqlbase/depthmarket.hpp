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

#ifndef _DEPTHMARKET_HPP_
#define _DEPTHMARKET_HPP_

class DepthMarket {

public:

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
	char ProductID[31];
	///最新价
	//TThostFtdcPriceType	LastPrice;
	double LastPrice;
	///上次结算价
	//TThostFtdcPriceType	PreSettlementPrice;
	double PreSettlementPrice;
	///昨收盘
	//TThostFtdcPriceType	PreClosePrice;
	double PreClosePrice;
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
	double OpenInterest;

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
	double BidPrice1;
//		   	///申买量一
	double BidVolume1;
//		   	///申卖价一
	double AskPrice1;
//		   	///申卖量一
	double AskVolume1;
//		   	///申买价二
	double BidPrice2;
//		   	///申买量二
	double BidVolume2;
//		   	///申卖价二
	double AskPrice2;
//		   	///申卖量二
	double AskVolume2;
//		   	///申买价三
	double BidPrice3;
//		   	///申买量三
	double BidVolume3;
//		   	///申卖价三
	double AskPrice3;
//		   	///申卖量三
	double AskVolume3;
//		   	///申买价四
	double BidPrice4;
//		   	///申买量四
	double BidVolume4;
//		   	///申卖价四
	TThostFtdcPriceType AskPrice4;
//		   	///申卖量四
	double AskVolume4;
//		   	///申买价五
	double BidPrice5;
//		   	///申买量五
	TThostFtdcVolumeType BidVolume5;
//		   	///申卖价五
	double AskPrice5;
//		   	///申卖量五
	double AskVolume5;
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
	int VolumeMultiple;
	///最小变动价位 TThostFtdcPriceType double
	double PriceTick;
	char Section[30];

	char DayNight[30];

	double OpenPreSettleRate;
	double LastOpenRate;
	double UpdownRate;
	double Sedimentary;
	double LastSedimentary;
//			double SedimentaryIO;
	double SedimentaryIORate;
	double AddOpenInterestRate;

};

class MinuteData {

public:

	///合约代码
	//TThostFtdcInstrumentIDType	InstrumentID;
	char InstrumentID[9];

	///交易日
	char DateTime[20];

	///交易日
	char Date[20];

	///交易日
	char MinuteTime[20];

	///最新价
	double OpenPrice;

	double HighestPrice;

	double LowestPrice;

	double ClosePrice;

	double AvePrice;
	//TThostFtdcVolumeType	Volume;
	double Volume;

	double Amount;
	///持仓量
	//TThostFtdcLargeVolumeType	OpenInterest;
	double OpenInterest;

};

class AveragePriceData {

public:

	char InstrumentID[9];   ///合约代码
	char Date[20];   	///交易日
	int OpenInterest;   ///每日分钟线 最后一分钟持仓量， 即每日持仓量
	int StartOpenInterest;  // 分钟线 第一分钟持仓量
	double AveragePrice;

};

class TradingDayInfo {

public:

	char TradingDay[27];   ///交易日
	double PreBalance;    // 上次结算准备金
	double Available;   ///可用金额
	double CurrMargin;  // CurrMargin
	double Commission;
	int FrontID;
	int SessionID;
	char MaxOrderRef[13];
	char LoginTime[9];
	char DCETime[9];
	char CZCETime[9];
	char SHFETime[9];
	char INETime[9];
	char LocalCreateTime[20];   	///
	char LocalUpdateTime[20];   	///

};

#endif /* _DEPTHMARKET_HPP_ */
