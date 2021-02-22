/*
 * mongohxdayline.hpp
 *
 *  Created on: 2017-8-22
 *      Author: qinchun
 */
#include <unistd.h>
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
#include <bsoncxx/builder/basic/document.hpp>
#include <json/json.h>
#include "depthmarket.hpp"


#ifndef MONGOAVERAGEPRICE_HPP_
#define MONGOAVERAGEPRICE_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class MongoAveragePrice
		{

		public :

	MongoAveragePrice();

			void Insert(AveragePriceData averagepricedata);
			void Check_AveragePriceData(AveragePriceData averagepricedata);
			double Find_MaxOpenInterest(const char *pinstrumentid );
			bool Existby_Instrumentid(const char *pinstrumentid ,const char*date);
			std::string GetLastDate(const char *pinstrumentid );
			int Count(const char * pinstrumentid);

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;
			mongocxx::uri * _uri=nullptr;
			mongocxx::client *_client=nullptr;


		};


#endif /* MONGOAVERAGEPRICE_HPP_ */
