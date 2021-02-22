/*
 * mongomaincontract.hpp
 *
 *  Created on: 2017-8-8
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
#include <json/json.h>
#include "dateutil.hpp"
#include "mysqlcycle.hpp"
#include <xlslib/xlslib.h>

using namespace xlslib_core;

#ifndef MONGOMAINCONTRACT_HPP_
#define MONGOMAINCONTRACT_HPP_

using namespace mongocxx;

using mongocxx::database;
using mongocxx::collection;
using mongocxx::cursor;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

//extern char *pinstrumentID[60];

	class MainContractInfo
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
			char InstrumentID[9];
			///产品代码 TThostFtdcInstrumentIDType char[31]
			char ProductID[20];

			char Section[30];

			char DayNight[30];

			  ///合约名称 TThostFtdcInstrumentNameType char[21]
			char InstrumentName[27];
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
			double MaxOpenInterest;   // 合约历史最高持仓量
			double OpenInterest ;
			double LongMarginRatio;
			double ShortMarginRatio;
			double Volume ;  // 成交量
			double LastPrice;
			double Sedimentary;

			//次月主力合约
			char InstrumentID_next[9];
			char InstrumentName_next[27];
			double OpenInterest_next ;
			double UpperLimitPrice_next;
			double LowerLimitPrice_next;
			double Volume_next ;
			double LastPrice_next;
			double Sedimentary_next;

			// 当前交易的合约ID
			char InstrumentID_trade[9];
			char LocalUpdateTime[30];


	};


	class MongoMainContract
	{
	public :

		MongoMainContract();
				void Delete(MainContractInfo maincontract);
				void DeleteAll();
				void Insert(MainContractInfo maincontract);
				void Check_MainContract(MainContractInfo maincontract);
				int Count();
//				char** Find_All();
//				char** Find_InstrumentTradeAll();
				void Find_AllTrade(vector<MainContractInfo> &temp);
				MainContractInfo Findby_InstrumentID(char * pinstrumentid);
				MainContractInfo Findby_TradeInstrumentID(char * pinstrumentid);
				MainContractInfo Findby_ID(char * pinstrumentid);   // 如果instrumentid 找不到,则继续找instrumentid_next
				std::string Find_LastActionDay();
				std::string Find_LastTradingDay();
				std::string Find_LastLocalUpdatetime();
				void Write_xls(std::string pathfile);
			private :

				char  _mongodb_url[30];
				char  _mongodb_dbname[20];
				char  _mongodb_collenction[20];
				mongocxx::database _dbname;
				mongocxx::collection _collection;
	};


#endif /* MONGOMAINCONTRACT_HPP_ */
