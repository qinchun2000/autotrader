/*
 * mongotradingday.hpp
 *
 *  Created on: 2017-8-22
 *      Author: qinchun
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
#include "depthmarket.hpp"
#include "dateutil.hpp"



#ifndef _MONGOTRADINGDAY_HPP_
#define _MONGOTRADINGDAY_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;



	class MongoTradingDay
		{

		public :

			MongoTradingDay();

			void Insert(TradingDayInfo tradingdayinfo);
			void Check_TradingDay(TradingDayInfo tradingdayinfo);
			void Update_TradingDay(TradingDayInfo tradingdayinfo);
			void Update_MaxOrderRef(const char *tradingday);
			TradingDayInfo Find_LastTradingDay();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* _MONGOTRADINGDAY_HPP_ */
