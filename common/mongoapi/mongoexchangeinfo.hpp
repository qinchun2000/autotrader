/*
 * mongoexchangeinfo.hpp
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





#ifndef MONGOEXCHANGEINFO_HPP_
#define MONGOEXCHANGEINFO_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class  ExchangeInfo
	{

		public :
			char ExchangeID[9];
			char ExchangeName[61];
			char ExchangeProperty;
			int  ExchangeP;

		private :

	};

	class MongoExchangeInfo
		{

		public :

			MongoExchangeInfo();

			void Insert(ExchangeInfo exchangeinfo);
			void Check_ExchangeInfo(ExchangeInfo exchangeinfo);
			int Count();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOEXCHANGEINFO_HPP_ */
