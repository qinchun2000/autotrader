/*
 * mongohxdayline.hpp
 *
 *  Created on: 2017-8-22
 *      Author: qinchun
 */

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include "httpbase.hpp"
#include <boost/regex.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <json/json.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>


#include "mongoproductclass.hpp"
#include "mongohxminuteline.hpp"
#include "mongocycle.hpp"
#include "mongoaverageprice.hpp"
#include "depthmarket.hpp"
#include "dateutil.hpp"



#ifndef MONGOHXDAYLINE_HPP_
#define MONGOHXDAYLINE_HPP_

using namespace mongocxx;
using namespace std;


using mongocxx::database;

using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class MongoHxDayLine
		{

		public :

			MongoHxDayLine();

			void Insert(HxDayLineData hxdaylinedata);
			HxDayLineData Find_HxDayLineData(const char* pinstrumentid,const char * date);
			void Check_HxDayLineData(HxDayLineData hxdaylinedata);
			void Check_AllHxMinData(const char *pinstrumentid );
			double Find_OpenHighPrice(const char *pinstrumentid ,const char * opendate);
			double Find_OpenLowPrice(const char *pinstrumentid ,const char * opendate);
			void Update_CycleData(const char *pinstrumentid );
			void Update_AveragePriceData(const char *pinstrumentid );
			void Find_UpdateDateList(vector<HxDayLineData> &temp,const char *pinstrumentid);
			void UpdateNewDay_CycleData(const char *pinstrumentid ,const char *date);
			void CheckHttpDayLine(char *pinstrumentid);
			void ParseKLineData(std::string response_data,std::string instrumentid);
			CycleData Collect_CycleData(MongoCycle mongocycle,const char *pinstrumentid,const char * date);
			AveragePriceData Collect_AveragePriceData(const char *pinstrumentid,const char * date);


		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::uri _uri;
			mongocxx::client _client;
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOHXDAYLINE_HPP_ */
