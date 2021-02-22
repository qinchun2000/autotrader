/*
 * mongotrade.hpp
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
#include "dateutil.hpp"




#ifndef MONGOTRADE_HPP_
#define MONGOTRADE_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class  TradeInfo
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[9];
			int OffsetFlag;
			int Direction;
			double Price;
			int Volume;
			double Margin;
			double ExchMargin;
			char TradingDay[20];
			char TradeDate[20];
			char TradeTime[20];
			char TradeID[20];
			char OrderRef[20];
			char OrderLocalID[20];
			char ClearingPartID[20];
			char ClientID[20];
			char BusinessUnit[20];
			long SequenceNo;
			int SettlementID;
			char LocalUpdateTime[20];



		private :

	};

	class MongoTrade
		{

		public :

		MongoTrade();

			void Insert(TradeInfo tradeinfo);
			void Check_Trade(TradeInfo tradeinfo);
			int Count();
			void DeleteAll();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOTRADE_HPP_ */
