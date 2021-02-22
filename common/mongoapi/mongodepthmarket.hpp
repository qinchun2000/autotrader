/*
 * mongodepthmarket.hpp
 *
 *  Created on: 2017-8-4
 *      Author: qinchun
 */

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <json/json.h>
#include <boost/regex.hpp>
#include "dateutil.hpp"
#include "depthmarket.hpp"
#include "mongomaincontract.hpp"
#include "mongoinstrument.hpp"



#ifndef MONGODEPTHMARKET_HPP_
#define MONGODEPTHMARKET_HPP_

using namespace std;
using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;




	class MongoDepthMarket
		{

		public :

			MongoDepthMarket();

			void Insert(DepthMarket depthmarket);
			void DeleteOne(DepthMarket depthmarket);
			void DeleteAll();
			void Check_DepthMarket(DepthMarket depthmarket);
			DepthMarket Find_LastUpdateTime();
			DepthMarket Findby_Instrumentid(const char *pinstrumentid);
			int Count();
			MainContractInfo Creat_Maincontract(const char *productid );

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGODEPTHMARKET_HPP_ */
