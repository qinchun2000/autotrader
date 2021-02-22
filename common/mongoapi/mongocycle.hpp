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
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <json/json.h>
#include "depthmarket.hpp"




#ifndef _MONGOCYCLE_HPP_
#define _MONGOCYCLE_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;



	class MongoCycle
		{

		public :

			MongoCycle();

			void Insert(CycleData cycledata);
			void Check_CycleData(CycleData cycledata);
			int Check_Highcount(const char* pinstrumentid,const char * date,double high20);
			int Check_Lowcount(const char* pinstrumentid,const char * date,double Low20);
			void Check_HighLowCount(const char* pinstrumentid,const char * date,CycleData  cycledata);
			CycleData Findby_Instrumentid(const char *pinstrumentid ,const char*date);
			int Existby_Instrumentid(const char *pinstrumentid ,const char*date);
			std::string Find_LastDate(char * pinstrumentid);

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* _MONGOCYCLE_HPP_ */
