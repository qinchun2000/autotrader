/*
 * mongohxminute.hpp
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
#include "mongoproductclass.hpp"
#include "httpbase.hpp"
#include <boost/regex.hpp>
#include "dateutil.hpp"



#ifndef MONGOHXMINUTELINE_HPP_
#define MONGOHXMINUTELINE_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class MongoHxMinuteLine
		{

		public :

			MongoHxMinuteLine();

			void Check_HttpMinuteLine(const char * pinstrumentid,const char *  date);
			void Insert(const char * pinstrumentid,const char *  date,std::string response);
			int Count(const char * pinstrumentid);
			bool Findby_CloseTime(const char * pinstrumentid,const char * closetime);
			bool Exist_CloseTime(const char * pinstrumentid,const char * closetime);
			std::string  Get_LastCloseTime(const char * pinstrumentid);
			int CollectLastOpenInterest_CloseTime(const char * pinstrumentid,const char * closetime);
			int CollectFirstOpenInterest_CloseTime(const char * pinstrumentid,const char * closetime);
			double CollectAveragePrice_CloseTime(const char * pinstrumentid,const char * closetime);
			AveragePriceData CollectAveragePriceData_CloseTime(const char * pinstrumentid,const char * closetime);
			void Check_HxMinuteLineData(const char * pinstrumentid,const char *  date);


		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOHXMINUTELINE_HPP_ */
