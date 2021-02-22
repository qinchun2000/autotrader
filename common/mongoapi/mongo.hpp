/*
 * mongo.hpp
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



#include <string>
#include <math.h>
#include <vector>
#include <regex.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <float.h>

#include "tradespi.hpp"
#include "httpbase.hpp"


#include "mysqldata.hpp"
#include "mysqldayline.hpp"
#include "mysqlhismindata.hpp"
#include "mysqlcycle.hpp"
#include "mysqlma.hpp"
#include "mysqlhisma.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlaveprice.hpp"

#ifndef _MONGO_HPP_
#define _MONGO_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;



	class Mongo
		{

		public :

			Mongo();
			void CheckAveragePriceData();
			void CheckHttpDayLine();
			void CheckHttpMinuteLine();
			void CheckCycleData();
			void CheckHisMAData();
			void CheckTradingDayMAData();
			void CheckTradingDayCycleData();


		private :




		};


#endif /* _MONGO_HPP_ */
