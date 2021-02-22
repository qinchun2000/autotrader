/*
 * mongoinvestpositiondetail.hpp
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
#include "mongoorder.hpp"





#ifndef MONGOINVESTPOSTIONDETAIL_HPP_
#define MONGOINVESTPOSTIONDETAIL_HPP_
#define MAXINVESTDETAIL_SIZE 30
using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;



	class  InvestPositionDetailInfo
	{

		public :
			///合约代码
			//TThostFtdcInstrumentIDType	InstrumentID;
			char InstrumentID[9];
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
			char TradingDay[20];
			char OpenDate[20];
			char TradeID[20];



		private :

	};

	class TradeDetailInstrumentInfo
		{
			public:
				std::string InstrumentID;
				std::string TradeID;

		};


	class MongoInvestPositionDetail
		{

		public :

			MongoInvestPositionDetail();

			void Insert(InvestPositionDetailInfo investpositiondetailinfo);
			void Check_InvestPositionDetail(InvestPositionDetailInfo investpositiondetailinfo);

			double CalOpenAveragePrice_byInstrumentID(char * pinstrumentid);
			void Find_TradeDetailList(vector<TradeDetailInstrumentInfo> &temp);
			InvestPositionDetailInfo  Find_byInstrumentIDTradeID(const char * pinstrumentid,const char *tradeid);
			bool  Exist_byInstrumentIDTradeID(const char * pinstrumentid,const char *tradeid);
			void Check_LocalOrder();
			int Count();
			void DeleteAll();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOINVESTPOSTIONDETAIL_HPP_ */
