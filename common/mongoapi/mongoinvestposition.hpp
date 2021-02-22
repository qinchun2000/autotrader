/*
 * mongoinvestposition.hpp
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
#include <json/json.h>
#include "dateutil.hpp"



#ifndef MONGOINVESTPOSTION_HPP_
#define MONGOINVESTPOSTION_HPP_
#define MAXIPInfo_SIZE 20
using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;





	class  InvestPositionInfo
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[9];
			int YdPosition;
			int Position;
			int TodayPosition;
			int OpenVolume;
			int CloseVolume;
			double OpenPriceAverage;
			double StoplessPrice;
			double WinPrice;
			double ClosePriceAverage;
			double OpenAmount;
			double CloseAmount;
			int PosiDirection;
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



	class MongoInvestPosition
		{

		public :

			MongoInvestPosition();



			void Find_TradeInstrumentIDList(vector<InvestPositionInfo> &temp);
			InvestPositionInfo  Find_byInstrumentid(const char *pinstrumentid);
			void Insert(InvestPositionInfo investpositioninfo);
			void Check_InvestPosition(InvestPositionInfo investpositioninfo);
			int  Find_PositionbyInstrumentid(const char *pinstrumentid);
			int Count();
			void DeleteAll();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOINVESTPOSTION_HPP_ */
