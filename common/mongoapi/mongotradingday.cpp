#include "mongotradingday.hpp"


	MongoTradingDay::MongoTradingDay()
	{
//			printf("MongoTradingDay 构造函数 入口 ...\n");
		mongocxx::instance inst{};

		strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
		strcpy(_mongodb_dbname,"baseinfo");
		strcpy(_mongodb_collenction,"tradingday");

		mongocxx::uri _uri(_mongodb_url);
		mongocxx::client _client(_uri);
		//打开"baseinfo"数据库
		_dbname = _client[_mongodb_dbname];
		_collection = _dbname[_mongodb_collenction];

	}

	void MongoTradingDay::Insert(TradingDayInfo tradingdayinfo)
		{
//			printf("MongoTradingDay Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];


			document doc{};
			doc<< "TradingDay" << tradingdayinfo.TradingDay
				<<"Available" << tradingdayinfo.Available

				<< "CurrMargin" << tradingdayinfo.CurrMargin
				<< "LocalUpdateTime" << tradingdayinfo.LocalUpdateTime;

			collection.insert_one(doc.view());

			printf("MongoTradingDay Insert finished! ...\n");
		}


	void MongoTradingDay::Check_TradingDay(TradingDayInfo tradingdayinfo)
		{

			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

			document doc{};
					doc<< "TradingDay" << tradingdayinfo.TradingDay
						<< "MaxOrderRef" << tradingdayinfo.MaxOrderRef
						<< "FrontID" << tradingdayinfo.FrontID
						<< "SessionID" << tradingdayinfo.SessionID
						<< "DCETime" << tradingdayinfo.DCETime
						<< "CZCETime" << tradingdayinfo.CZCETime
						<< "SHFETime" << tradingdayinfo.SHFETime
						<< "INETime" << tradingdayinfo.INETime
						<< "LoginTime" << tradingdayinfo.LoginTime
						<< "LocalCreateTime" << tradingdayinfo.LocalCreateTime
						<< "LocalUpdateTime" << str_localupdate;

			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "TradingDay" << tradingdayinfo.TradingDay<< finalize);
			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
			//  printf("MongoTradingDay check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 存在当天TradingDay！\n");
			}
			else
			{
//					printf("-----> 数据不存在，插入新数据！\n");
				 collection.insert_one(doc.view());
			}
//				printf("MongoTradingDay Check_TradingDay end! ...\n");
		}

	void MongoTradingDay::Update_TradingDay(TradingDayInfo tradingdayinfo)
			{
	//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


				document doc{};
									doc<< "TradingDay" << tradingdayinfo.TradingDay
										<< "PreBalance" << tradingdayinfo.PreBalance
										<< "Available" << tradingdayinfo.Available
										<< "CurrMargin" << tradingdayinfo.CurrMargin
										<< "LocalUpdateTime" << str_localupdate;


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "TradingDay" << tradingdayinfo.TradingDay<< finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoTradingDay check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 存在当天TradingDay！\n");

					 Json::Reader  reader;
					  Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
						  {
							  if(!value["TradingDay"].isNull())
							  {

							  strcpy(tradingdayinfo.MaxOrderRef,value["MaxOrderRef"].asString().c_str());
							  strcpy(tradingdayinfo.DCETime,value["DCETime"].asString().c_str());
							  strcpy(tradingdayinfo.CZCETime,value["CZCETime"].asString().c_str());
							  strcpy(tradingdayinfo.SHFETime,value["SHFETime"].asString().c_str());
							  strcpy(tradingdayinfo.INETime,value["INETime"].asString().c_str());
							  strcpy(tradingdayinfo.LoginTime,value["LoginTime"].asString().c_str());
							  tradingdayinfo.FrontID =value["FrontID"].asInt();
							  tradingdayinfo.SessionID =value["SessionID"].asInt();



							  }
						 }

					collection.update_one(document{} << "TradingDay" << tradingdayinfo.TradingDay << finalize,
					                      document{} << "$set" << open_document <<
										  "PreBalance" << tradingdayinfo.PreBalance <<
										  "Available" << tradingdayinfo.Available <<
										  "CurrMargin" << tradingdayinfo.CurrMargin <<
										  "LocalUpdateTime" << str_localupdate << close_document << finalize);
				}
				else
				{
	//					printf("-----> 数据不存在，插入新数据！\n");
//					 collection.insert_one(doc.view());
				}



	//				printf("MongoTradingDay check end! ...\n");
			}

	void MongoTradingDay::Update_MaxOrderRef(const char *tradingday)
		{
			char MaxOrderRef[13];
			char OrderRef[13];

			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);

			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "TradingDay" << tradingday<< finalize);
			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
			//  printf("MongoTradingDay check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 存在当天TradingDay！\n");

				 Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["TradingDay"].isNull())
						  {

						  strcpy(MaxOrderRef,value["MaxOrderRef"].asString().c_str());

						  int iNextOrderRef = atoi(MaxOrderRef);
						  iNextOrderRef++;

						  sprintf(OrderRef, "%d", iNextOrderRef);

						  }
					 }

				collection.update_one(document{} << "TradingDay" << tradingday << finalize,
									  document{} << "$set" << open_document <<
									  "MaxOrderRef" << OrderRef <<
									  "LocalUpdateTime" << str_localupdate << close_document << finalize);
			}
			else
			{
//					printf("-----> 数据不存在，插入新数据！\n");
//				 collection.insert_one(doc.view());
			}



//				printf("MongoTradingDay check end! ...\n");
		}

	TradingDayInfo MongoTradingDay::Find_LastTradingDay()
		{
			TradingDayInfo tradingdayinfo;
			std::string str_date;
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			mongocxx::options::find opts;
			bsoncxx::builder::stream::document order_builder;
			order_builder << "TradingDay" << -1 ;   //此处取时间最新的日期，保持永远是最近的一个交易日
			opts.sort(order_builder.view()).limit(1);

			auto cursor = collection.find(document{} << finalize, opts);
			for (auto&& doc : cursor)
			{
//						std::cout << "Find_LastTradingDay ---->" <<bsoncxx::to_json(doc) << std::endl;

				Json::Reader  reader;
				Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["TradingDay"].isNull())
					  {
						  strcpy(tradingdayinfo.TradingDay , value["TradingDay"].asString().c_str());
						  strcpy(tradingdayinfo.MaxOrderRef , value["MaxOrderRef"].asString().c_str());
						  tradingdayinfo.FrontID = value["FrontID"].asInt();
						  tradingdayinfo.SessionID = value["SessionID"].asInt();
						  tradingdayinfo.PreBalance = value["PreBalance"].asDouble();
						  tradingdayinfo.Available = value["Available"].asDouble();
						  tradingdayinfo.CurrMargin = value["CurrMargin"].asDouble();

//						  printf("MongoTradingDay Find_LastTradingDay ---->%f --> %f \n",value["CurrMargin"].asDouble(),tradingdayinfo.CurrMargin);

					  }
				 }
			// @end: cpp-query-sort

			}

//							printf("MongoTradingDay Find_LastTradingDay end! ...\n");
			return tradingdayinfo;


		}
