/*
 * mongomaincontract.cpp
 *
 *  Created on: 2017-8-8
 *      Author: vnpy
 */

#include "mongomaincontract.hpp"


MongoMainContract::MongoMainContract()
		{
//			printf("MongoMainContract 构造函数 入口 ...\n");
			mongocxx::instance inst{};

			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"baseinfo");
			strcpy(_mongodb_collenction,"maincontract");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];
		}

void MongoMainContract::Delete(MainContractInfo maincontract)
	{
//			printf("MongoMainContract Delete begin! ...\n");

		mongocxx::uri uri(_mongodb_url);

		mongocxx::client client(uri);
		//打开"baseinfo"数据库
		mongocxx::database dbname = client[_mongodb_dbname];
		mongocxx::collection collection = dbname[_mongodb_collenction];


		document doc{};
		doc<< "InstrumentID" <<maincontract.InstrumentID
		<< "ExchangeID" << maincontract.ExchangeID;


//		collection.delete_one(document{}<<" InstrumentID "<<maincontract.InstrumentID<<
//		" ExchangeID "<< maincontract.ExchangeID<<finalize);

        collection.delete_one(doc.view());

		printf("MongoMainContract Delete finished! ...\n");
	}

void MongoMainContract::DeleteAll()
				{
		//			printf("MongoMainContract Delete begin! ...\n");
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];


					collection.delete_many(document{} << finalize);
					printf("MongoMainContract DeleteAll finished! ...\n");
				}

	void MongoMainContract::Insert(MainContractInfo maincontract)
		{
//			printf("MongoMainContract Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];


			document doc{};
			doc<< "ProductID" << maincontract.ProductID
				<<"InstrumentName" << maincontract.InstrumentName
				<<"InstrumentID" << maincontract.InstrumentID
				<< "ExchangeID" << maincontract.ExchangeID
				<< "VolumeMultiple" << maincontract.VolumeMultiple
				<< "PriceTick" << maincontract.PriceTick
				<< "UpperLimitPrice" << maincontract.UpperLimitPrice
				<< "LowerLimitPrice" << maincontract.LowerLimitPrice
				<< "LongMarginRatio" << maincontract.LongMarginRatio
				<< "ShortMarginRatio" << maincontract.ShortMarginRatio
				<< "OpenInterest" << maincontract.OpenInterest;

	        collection.insert_one(doc.view());

			printf("MongoMainContract Insert finished! ...\n");
		}

	void MongoMainContract::Check_MainContract(MainContractInfo maincontract)
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

				doc<< "ProductID" << maincontract.ProductID
						<<"InstrumentName" << maincontract.InstrumentName
						<<"InstrumentID" << maincontract.InstrumentID
						<<"TradingDay" << maincontract.TradingDay
						<<"ActionDay" << maincontract.ActionDay

						<<"UpdateTime" << maincontract.UpdateTime
							<< "ExchangeID" << maincontract.ExchangeID
							<< "Section" << maincontract.Section
							<< "DayNight" << maincontract.DayNight
							<< "VolumeMultiple" << maincontract.VolumeMultiple

							<< "PriceTick" << maincontract.PriceTick
							<< "UpperLimitPrice" << maincontract.UpperLimitPrice
							<< "LowerLimitPrice" << maincontract.LowerLimitPrice
							<< "LongMarginRatio" << maincontract.LongMarginRatio
							<< "ShortMarginRatio" << maincontract.ShortMarginRatio

							<< "MaxOpenInterest" << maincontract.MaxOpenInterest
							<< "OpenInterest" << maincontract.OpenInterest
							<< "Volume" << maincontract.Volume
							<< "Sedimentary" << maincontract.Sedimentary
							<< "LastPrice" << maincontract.LastPrice

							<< "InstrumentID_next" << maincontract.InstrumentID_next
							<< "OpenInterest_next" << maincontract.OpenInterest_next
							<< "UpperLimitPrice_next" << maincontract.UpperLimitPrice_next
							<< "LowerLimitPrice_next" << maincontract.LowerLimitPrice_next
							<< "Sedimentary_next" << maincontract.Sedimentary_next

							<< "LastPrice_next" << maincontract.LastPrice_next
							<< "Volume_next" << maincontract.Volume_next
							<< "InstrumentID_trade" << maincontract.InstrumentID_trade
							<<"LocalUpdateTime" << str_localupdate;




				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << maincontract.InstrumentID<< finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 主力合约数据存在！\n");
					document filter_del{};

					filter_del<< "ProductID" << maincontract.ProductID
								<< finalize;
					collection.delete_many(filter_del.view());
	//				printf("-----> 数据存在，删除后--->插入新数据！\n");
					collection.insert_one(doc.view());
				}
				else
				{
//					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoMainContract check end! ...\n");
			}

	int MongoMainContract::Count()
			{
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				int i =0;
				auto cursor = collection.find(document{} << finalize);
				for (auto&& doc : cursor)
				{
					i++;
				}


				return i;

			}
//	char** MongoMainContract::Find_All()
//					{
//
//
//		//				printf("_mongodb_url  %s\n",_mongodb_url);
//						mongocxx::uri uri(_mongodb_url);
//
//						mongocxx::client client(uri);
//						//打开"baseinfo"数据库
//						mongocxx::database dbname = client[_mongodb_dbname];
//						mongocxx::collection collection = dbname[_mongodb_collenction];
//
//
//						mongocxx::cursor cursor = collection.find(document{} << finalize);
//						int i=0;
//						for(auto doc : cursor)
//						{
////						  std::cout << bsoncxx::to_json(doc) << "\n";
//
//						  Json::Reader  reader;
//						  Json::Value   value;
//
//							  if(reader.parse(bsoncxx::to_json(doc),value))
//							  {
//								  if(!value["InstrumentID"].isNull())
//								  {
//
//
////								  std::cout<<value["InstrumentID"].asString()<<std::endl;
//								  char *char_instrumentid= new char[10];
//
//								  strcpy(char_instrumentid,value["InstrumentID"].asString().c_str());
//								  pinstrumentID[i]=char_instrumentid;
//								  i++;
//
//
//								  }
//							 }
//
//						}
//
//
//
////						printf("MongoMainContract Find_All  end! ...\n");
//
//
//
//						return pinstrumentID;
//					}

//	char** MongoMainContract::Find_InstrumentTradeAll()
//			{
//
//
////				printf("_mongodb_url  %s\n",_mongodb_url);
//				mongocxx::uri uri(_mongodb_url);
//
//				mongocxx::client client(uri);
//				//打开"baseinfo"数据库
//				mongocxx::database dbname = client[_mongodb_dbname];
//				mongocxx::collection collection = dbname[_mongodb_collenction];
//
//
//				mongocxx::cursor cursor = collection.find(document{} << finalize);
//				int i=0;
//				for(auto doc : cursor)
//				{
////						  std::cout << bsoncxx::to_json(doc) << "\n";
//
//				  Json::Reader  reader;
//				  Json::Value   value;
//
//					  if(reader.parse(bsoncxx::to_json(doc),value))
//					  {
//						  if(!value["InstrumentID_trade"].isNull())
//						  {
//
//
////								  std::cout<<value["InstrumentID"].asString()<<std::endl;
//						  char *char_instrumentid= new char[10];
//
//						  strcpy(char_instrumentid,value["InstrumentID_trade"].asString().c_str());
//						  pinstrumentID[i]=char_instrumentid;
//						  i++;
//
//
//						  }
//					 }
//
//				}
//
//
//
////						printf("MongoMainContract Find_All  end! ...\n");
//
//
//
//				return pinstrumentID;
//			}

	void MongoMainContract::Find_AllTrade(vector<MainContractInfo> &temp)
		{

			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];


			mongocxx::cursor cursor = collection.find(document{} << finalize);

			for(auto doc : cursor)
			{
//						  std::cout << bsoncxx::to_json(doc) << "\n";

			  Json::Reader  reader;
			  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["InstrumentID_trade"].isNull())
					  {

						  MainContractInfo info;
						  strcpy(info.InstrumentID_trade,value["InstrumentID_trade"].asString().c_str());
						  strcpy(info.InstrumentID,value["InstrumentID"].asString().c_str());
						  strcpy(info.InstrumentID_next,value["InstrumentID_next"].asString().c_str());
						  strcpy(info.ExchangeID,value["ExchangeID"].asString().c_str());
						  strcpy(info.ProductID,value["ProductID"].asString().c_str());
						  strcpy(info.Section,value["Section"].asString().c_str());
						  strcpy(info.DayNight,value["DayNight"].asString().c_str());

						  info.Sedimentary = value["Sedimentary"].asDouble();
						  info.PriceTick =value["PriceTick"].asDouble();
						  info.UpperLimitPrice =value["UpperLimitPrice"].asDouble();
						  info.LowerLimitPrice =value["LowerLimitPrice"].asDouble();
						  info.VolumeMultiple  =value["VolumeMultiple"].asInt();

							if(strcmp(info.ExchangeID,"INE")!=0)
							{
								 temp.push_back(info);
							}



					  }
				 }

			}


		}
	MainContractInfo MongoMainContract::Findby_InstrumentID(char * pinstrumentid)
				{

					MainContractInfo maincontractinfo;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];


					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << pinstrumentid<< finalize);
					if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 主力合约数据存在，-----无需插入数据！\n");
						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
						  {
							  if(!value["InstrumentID"].isNull())
							  {
								strcpy(maincontractinfo.InstrumentID,value["InstrumentID"].asString().c_str());
								strcpy(maincontractinfo.ProductID,value["ProductID"].asString().c_str());
								strcpy(maincontractinfo.ExchangeID,value["ExchangeID"].asString().c_str());
								strcpy(maincontractinfo.Section,value["Section"].asString().c_str());
								strcpy(maincontractinfo.DayNight,value["DayNight"].asString().c_str());

								strcpy(maincontractinfo.TradingDay,value["TradingDay"].asString().c_str());
								strcpy(maincontractinfo.ActionDay,value["ActionDay"].asString().c_str());
								strcpy(maincontractinfo.UpdateTime,value["UpdateTime"].asString().c_str());
								strcpy(maincontractinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());

								maincontractinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
								maincontractinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
								maincontractinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
								maincontractinfo.PriceTick = value["PriceTick"].asDouble();

								maincontractinfo.OpenInterest = value["OpenInterest"].asDouble();
								maincontractinfo.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
								maincontractinfo.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
								maincontractinfo.Volume = value["Volume"].asDouble();
								maincontractinfo.LastPrice = value["LastPrice"].asDouble();
								maincontractinfo.Sedimentary = value["Sedimentary"].asDouble();
								maincontractinfo.LastPrice_next = value["LastPrice_next"].asDouble();
								maincontractinfo.Sedimentary_next = value["Sedimentary_next"].asDouble();
								maincontractinfo.Volume_next = value["Volume_next"].asDouble();
							  }
						  }
					}
					else
					{
	//					printf("-----> 数据不存在！\n");

					}




//						printf("MongoMainContract Findby_InstrumentID  end! ...\n");



					return maincontractinfo;
				}

	MainContractInfo MongoMainContract::Findby_TradeInstrumentID(char * pinstrumentid)
						{

							MainContractInfo maincontractinfo;
							mongocxx::uri uri(_mongodb_url);

							mongocxx::client client(uri);
							//打开"baseinfo"数据库
							mongocxx::database dbname = client[_mongodb_dbname];
							mongocxx::collection collection = dbname[_mongodb_collenction];


							bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
							  collection.find_one(document{} << "InstrumentID_trade" << pinstrumentid<< finalize);
							if(maybe_result) {
			//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
							//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
			//				  printf("-----> 主力合约数据存在，-----无需插入数据！\n");
								Json::Reader  reader;
								Json::Value   value;

								  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
								  {
									  if(!value["InstrumentID_trade"].isNull())
									  {
										strcpy(maincontractinfo.InstrumentID,value["InstrumentID"].asString().c_str());
										strcpy(maincontractinfo.InstrumentID_trade,value["InstrumentID_trade"].asString().c_str());
										strcpy(maincontractinfo.ProductID,value["ProductID"].asString().c_str());
										strcpy(maincontractinfo.ExchangeID,value["ExchangeID"].asString().c_str());
										strcpy(maincontractinfo.Section,value["Section"].asString().c_str());
										strcpy(maincontractinfo.DayNight,value["DayNight"].asString().c_str());

										strcpy(maincontractinfo.TradingDay,value["TradingDay"].asString().c_str());
										strcpy(maincontractinfo.ActionDay,value["ActionDay"].asString().c_str());
										strcpy(maincontractinfo.UpdateTime,value["UpdateTime"].asString().c_str());
										strcpy(maincontractinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());

										maincontractinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
										maincontractinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
										maincontractinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
										maincontractinfo.PriceTick = value["PriceTick"].asDouble();

										maincontractinfo.OpenInterest = value["OpenInterest"].asDouble();
										maincontractinfo.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
										maincontractinfo.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
										maincontractinfo.Volume = value["Volume"].asDouble();
										maincontractinfo.LastPrice = value["LastPrice"].asDouble();
										maincontractinfo.Sedimentary = value["Sedimentary"].asDouble();
										maincontractinfo.LastPrice_next = value["LastPrice_next"].asDouble();
										maincontractinfo.Sedimentary_next = value["Sedimentary_next"].asDouble();
										maincontractinfo.Volume_next = value["Volume_next"].asDouble();
									  }
								  }
							}
							else
							{
			//					printf("-----> 数据不存在！\n");

							}




	//						printf("MongoMainContract Findby_InstrumentID  end! ...\n");



							return maincontractinfo;
						}

	MainContractInfo MongoMainContract::Findby_ID(char * pinstrumentid)
				{

					MainContractInfo maincontractinfo;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);

					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];


					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << pinstrumentid<< finalize);
					if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 主力合约数据存在，-----无需插入数据！\n");
						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
						  {
							  if(!value["InstrumentID"].isNull())
							  {
								strcpy(maincontractinfo.InstrumentID,value["InstrumentID"].asString().c_str());
								strcpy(maincontractinfo.InstrumentID_trade,value["InstrumentID_trade"].asString().c_str());
								strcpy(maincontractinfo.ProductID,value["ProductID"].asString().c_str());
								strcpy(maincontractinfo.ExchangeID,value["ExchangeID"].asString().c_str());
								strcpy(maincontractinfo.Section,value["Section"].asString().c_str());
								strcpy(maincontractinfo.DayNight,value["DayNight"].asString().c_str());

								strcpy(maincontractinfo.TradingDay,value["TradingDay"].asString().c_str());
								strcpy(maincontractinfo.ActionDay,value["ActionDay"].asString().c_str());
								strcpy(maincontractinfo.UpdateTime,value["UpdateTime"].asString().c_str());
								strcpy(maincontractinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());

								maincontractinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
								maincontractinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
								maincontractinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
								maincontractinfo.PriceTick = value["PriceTick"].asDouble();

								maincontractinfo.OpenInterest = value["OpenInterest"].asDouble();
								maincontractinfo.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
								maincontractinfo.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
								maincontractinfo.Volume = value["Volume"].asDouble();
								maincontractinfo.LastPrice = value["LastPrice"].asDouble();
								maincontractinfo.Sedimentary = value["Sedimentary"].asDouble();
								maincontractinfo.LastPrice_next = value["LastPrice_next"].asDouble();
								maincontractinfo.Sedimentary_next = value["Sedimentary_next"].asDouble();
								maincontractinfo.Volume_next = value["Volume_next"].asDouble();
							  }
						  }
					}
					else
					{
						bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result_next =
						collection.find_one(document{} << "InstrumentID_next" << pinstrumentid<< finalize);

						if(maybe_result_next) {


							Json::Reader  reader;
							Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(*maybe_result_next),value))
							  {
								  if(!value["InstrumentID_next"].isNull())
								  {
									strcpy(maincontractinfo.InstrumentID,value["InstrumentID"].asString().c_str());
									strcpy(maincontractinfo.InstrumentID_next,value["InstrumentID_next"].asString().c_str());
									strcpy(maincontractinfo.ProductID,value["ProductID"].asString().c_str());
									strcpy(maincontractinfo.ExchangeID,value["ExchangeID"].asString().c_str());
									strcpy(maincontractinfo.Section,value["Section"].asString().c_str());
									strcpy(maincontractinfo.DayNight,value["DayNight"].asString().c_str());

									strcpy(maincontractinfo.TradingDay,value["TradingDay"].asString().c_str());
									strcpy(maincontractinfo.ActionDay,value["ActionDay"].asString().c_str());
									strcpy(maincontractinfo.UpdateTime,value["UpdateTime"].asString().c_str());
									strcpy(maincontractinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());

									maincontractinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
									maincontractinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
									maincontractinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
									maincontractinfo.PriceTick = value["PriceTick"].asDouble();

									maincontractinfo.OpenInterest = value["OpenInterest"].asDouble();
									maincontractinfo.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
									maincontractinfo.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
									maincontractinfo.Volume = value["Volume"].asDouble();
									maincontractinfo.LastPrice = value["LastPrice"].asDouble();
								  }
							  }
						}
						else
						{


						}


					}

//						printf("MongoMainContract Findby_ID  end! ...\n");

					return maincontractinfo;
				}
	std::string MongoMainContract::Find_LastActionDay()
					{

						std::string str_lastactionday;
						mongocxx::uri uri(_mongodb_url);

						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];

						mongocxx::options::find opts;
						bsoncxx::builder::stream::document order_builder;
						order_builder << "ActionDay" << 1 ;   //此处取时间最久远的那个数据，要确保最久远的数据都是最新更新的
						opts.sort(order_builder.view()).limit(1);

						auto cursor = collection.find(document{} << finalize, opts);
						for (auto&& doc : cursor)
						{
	//						std::cout << "Find_LastUpdateTime ---->" <<bsoncxx::to_json(doc) << std::endl;

							Json::Reader  reader;
							Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(doc),value))
							  {
								  if(!value["ActionDay"].isNull())
								  {



									  str_lastactionday = value["ActionDay"].asString();
	//								  printf("MongoInstrument Find_LastUpdateTime ---->%s ...\n",str_lastupdatetime.c_str());

								  }
							 }
						// @end: cpp-query-sort

						}

	//							printf("MongoInstrument Find_LastUpdateTime end! ...\n");
						return str_lastactionday;


					}

	std::string MongoMainContract::Find_LastTradingDay()
				{

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



								  str_date = value["TradingDay"].asString();
//								  printf("MongoMainContract Find_LastTradingDay ---->%s ...\n",str_lastupdatetime.c_str());

							  }
						 }
					// @end: cpp-query-sort

					}

//							printf("MongoMainContract Find_LastTradingDay end! ...\n");
					return str_date;


				}

	std::string MongoMainContract::Find_LastLocalUpdatetime()
			{

				std::string str_date;
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "LocalUpdateTime" << 1 ;   //此处取时间最久远的那个数据，要确保最久远的数据都是最新更新的
				opts.sort(order_builder.view()).limit(1);

				auto cursor = collection.find(document{} << finalize, opts);
				for (auto&& doc : cursor)
				{
//						std::cout << "Find_LastUpdateTime ---->" <<bsoncxx::to_json(doc) << std::endl;

					Json::Reader  reader;
					Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["LocalUpdateTime"].isNull())
						  {



							  str_date = value["LocalUpdateTime"].asString();
//								  printf("MongoInstrument Find_LastUpdateTime ---->%s ...\n",str_lastupdatetime.c_str());

						  }
					 }
				// @end: cpp-query-sort

				}

//							printf("MongoInstrument Find_LastUpdateTime end! ...\n");
				return str_date;


			}


	void MongoMainContract::Write_xls(std::string pathfile)
			{

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				workbook wb;
				xf_t* xf = wb.xformat();
				xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
				xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
				xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
				xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

				worksheet* ws;
				ws = wb.sheet("maincontract");

				ws->colwidth(1,256*10);
				ws->colwidth(2,256*10);
				ws->colwidth(3,256*10);
				ws->colwidth(4,256*18);
				ws->colwidth(5,256*10);
				ws->colwidth(6,256*10);
				ws->colwidth(7,256*15);
				ws->colwidth(8,256*15);
				ws->colwidth(9,256*10);
				ws->colwidth(10,256*12);
				ws->colwidth(11,256*10);
				ws->colwidth(12,256*15);
				ws->colwidth(13,256*10);
				ws->colwidth(14,256*15);
				ws->colwidth(15,256*15);
				ws->colwidth(16,256*10);
				ws->colwidth(17,256*10);
				ws->colwidth(18,256*15);

				ws->label(1,1,"序号",xf);
				ws->label(1,2,"主力合约ID",xf);
				ws->label(1,3,"交易日期",xf);
				ws->label(1,4,"主力合约名称",xf);
				ws->label(1,5,"沉淀资金",xf);
				ws->label(1,6,"主力持仓量",xf);
				ws->label(1,7,"主力成交量",xf);
				ws->label(1,8,"主力最高持仓量",xf);
				ws->label(1,9,"交易所",xf);
				ws->label(1,10,"板块",xf);
				ws->label(1,11,"夜盘",xf);
				ws->label(1,12,"次月主力合约",xf);
				ws->label(1,13,"次月主力沉淀资金",xf);
				ws->label(1,14,"次月主力持仓量",xf);
				ws->label(1,15,"次月主力成交量",xf);
				ws->label(1,16,"更新日期",xf);
				ws->label(1,17,"交易合约",xf);
				ws->label(1,18,"周期价差更新日期",xf);

//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);

				range * _range;

				_range = ws->rangegroup(1,1,1,18);
				_range->cellcolor(CLR_GOLD);

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "Sedimentary" << -1 ;
				opts.sort(order_builder.view());

				mongocxx::cursor cursor = collection.find(document{} << finalize,opts);
				int i=0;
				for(auto doc : cursor)
				{
//				  std::cout << bsoncxx::to_json(doc) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID_trade"].isNull())
						  {

							  MainContractInfo maincontractinfo;
//								  std::cout<<value["InstrumentID"].asString()<<std::endl;


							strcpy(maincontractinfo.InstrumentID,value["InstrumentID"].asString().c_str());
							strcpy(maincontractinfo.InstrumentName,value["InstrumentName"].asString().c_str());
							strcpy(maincontractinfo.ProductID,value["ProductID"].asString().c_str());
							strcpy(maincontractinfo.ExchangeID,value["ExchangeID"].asString().c_str());
							strcpy(maincontractinfo.Section,value["Section"].asString().c_str());
							strcpy(maincontractinfo.DayNight,value["DayNight"].asString().c_str());

							strcpy(maincontractinfo.TradingDay,value["TradingDay"].asString().c_str());
							strcpy(maincontractinfo.ActionDay,value["ActionDay"].asString().c_str());
							strcpy(maincontractinfo.UpdateTime,value["UpdateTime"].asString().c_str());
							strcpy(maincontractinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());

							maincontractinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
							maincontractinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
							maincontractinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
							maincontractinfo.PriceTick = value["PriceTick"].asDouble();

							maincontractinfo.Sedimentary = value["Sedimentary"].asDouble();
							maincontractinfo.Sedimentary_next = value["Sedimentary_next"].asDouble();
							maincontractinfo.OpenInterest = value["OpenInterest"].asDouble();
							maincontractinfo.MaxOpenInterest = value["MaxOpenInterest"].asDouble();
							maincontractinfo.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
							maincontractinfo.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
							maincontractinfo.Volume = value["Volume"].asDouble();
							maincontractinfo.LastPrice = value["LastPrice"].asDouble();
							maincontractinfo.LastPrice_next = value["LastPrice_next"].asDouble();

							strcpy(maincontractinfo.InstrumentID_next,value["InstrumentID_next"].asString().c_str());
							maincontractinfo.OpenInterest_next = value["OpenInterest_next"].asDouble();
							maincontractinfo.Volume_next = value["Volume_next"].asDouble();
							strcpy(maincontractinfo.InstrumentID_trade,value["InstrumentID_trade"].asString().c_str());

							  int row=i+2;
							  char number[10];
							  sprintf(number,"%d",i+1);
							  ws->label(row,1,number,xf);
							  ws->label(row,2,maincontractinfo.InstrumentID,xf);
							  ws->label(row,3,maincontractinfo.TradingDay,xf);
							  ws->label(row,4,maincontractinfo.InstrumentName,xf);

							  char sedimentary[20];
							  sprintf(sedimentary,"%.1f",maincontractinfo.Sedimentary);
							  ws->label(row,5,sedimentary,xf);

							  char OpenInterest[20];
							  sprintf(OpenInterest,"%.f",maincontractinfo.OpenInterest);
							  ws->label(row,6,OpenInterest,xf);

							  char Volume[20];
							  sprintf(Volume,"%.f",maincontractinfo.Volume);
							  ws->label(row,7,Volume,xf);

							  char MaxOpenInterest[20];
							  sprintf(MaxOpenInterest,"%.f",maincontractinfo.MaxOpenInterest);
							  ws->label(row,8,MaxOpenInterest,xf);


							  ws->label(row,9,maincontractinfo.ExchangeID,xf);
							  ws->label(row,10,maincontractinfo.Section,xf);
							  ws->label(row,11,maincontractinfo.DayNight,xf);
							  ws->label(row,12,maincontractinfo.InstrumentID_next,xf);

							  char Sedimentary_next[20];
							  sprintf(Sedimentary_next,"%.1f",maincontractinfo.Sedimentary_next);
							  ws->label(row,13,Sedimentary_next,xf);

							  char OpenInterest_next[20];
							  sprintf(OpenInterest_next,"%.f",maincontractinfo.OpenInterest_next);
							  ws->label(row,14,OpenInterest_next,xf);

							  char Volume_next[20];
							  sprintf(Volume_next,"%.f",maincontractinfo.Volume_next);
							  ws->label(row,15,Volume_next,xf);




							  ws->label(row,16,maincontractinfo.UpdateTime,xf);
							  ws->label(row,17,maincontractinfo.InstrumentID_trade,xf);

							  MysqlCycle mycycle;
							  std::string s_cycledate=mycycle.GetLastDateTime(maincontractinfo.InstrumentID_trade);
							  ws->label(row,18,s_cycledate.substr(0,10),xf);

						  }
					 }
					  i++;

				}

				wb.Dump(pathfile);

	//						printf("MongoMainContract Find_All  end! ...\n");




		}
