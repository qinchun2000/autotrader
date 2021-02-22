#include "mongoaverageprice.hpp"


	MongoAveragePrice::MongoAveragePrice()
		{
//			printf("MongoAveragePrice 构造函数 入口 ...\n");
			mongocxx::instance inst{};

			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"averageprice");
//			 _uri(_mongodb_url);
//
//			 _client(_uri);
//			//打开"baseinfo"数据库
//			_dbname = _client[_mongodb_dbname];
//			_collection = _dbname[_mongodb_collenction];



		}

	void MongoAveragePrice::Insert(AveragePriceData averagepricedata)
		{
//			printf("MongoAveragePrice Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[averagepricedata.InstrumentID];

			document doc{};

			doc<< "Date" << averagepricedata.Date
				<< "OpenInterest" << averagepricedata.OpenInterest
				<< "StartOpenInterest" << averagepricedata.StartOpenInterest
				<< "AveragePrice" << averagepricedata.AveragePrice;

			collection.insert_one(doc.view());


			printf("MongoAveragePrice Insert finished! ...\n");
		}

	void MongoAveragePrice::Check_AveragePriceData(AveragePriceData averagepricedata)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[averagepricedata.InstrumentID];

				char AveragePrice[20];
				sprintf(AveragePrice, "%.3f", averagepricedata.AveragePrice);


				document doc{};


				doc<< "Date" << averagepricedata.Date
						<< "OpenInterest" << averagepricedata.OpenInterest
						<< "StartOpenInterest" << averagepricedata.StartOpenInterest
						<< "AveragePrice" << AveragePrice;

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "Date" << averagepricedata.Date<< finalize);

				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoHxDayLine check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");
				}
				else if(averagepricedata.OpenInterest !=0 && averagepricedata.StartOpenInterest!=0 && averagepricedata.AveragePrice !=0)
				{
					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoAveragePrice check end! ...\n");
			}

	double MongoAveragePrice::Find_MaxOpenInterest(const char *pinstrumentid )
				{
					double maxopeninterest;
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[pinstrumentid];


//					bsoncxx::builder::stream::document filter_builder;
//					        filter_builder << "ProductID" << productid ;
					// Sort query results.

					        // @begin: cpp-query-sort
					mongocxx::options::find opts;
					bsoncxx::builder::stream::document order_builder;
					order_builder << "OpenInterest" << -1 ;
					opts.sort(order_builder.view()).limit(1);

					auto cursor = collection.find(document{} << finalize, opts);
					for (auto&& doc : cursor)
					{
//					            std::cout << "Find_MaxOpenInterest ---->" <<bsoncxx::to_json(doc) << std::endl;

						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(doc),value))
						  {
							  if(!value["OpenInterest"].isNull())
							  {
								  double opi = value["OpenInterest"].asDouble();

									if (opi > maxopeninterest)
									{
										maxopeninterest = opi;
									}


							  //r.ProductID = value["ProductID"].asString().c_str();
							  }
						 }
					// @end: cpp-query-sort

					}


//					printf("Find_MaxOpenInterest check end! ...\n");
					return maxopeninterest;
				}


	bool MongoAveragePrice::Existby_Instrumentid(const char *pinstrumentid ,const char*date)
		{
			int ret=0;
			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];

			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "Date" << date << finalize);

			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
			//  printf("MongoHxDayLine check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//					  printf("-----> 历史周期价差表 找到！\n");
			  Json::Reader  reader;
			  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
				  {
					  if(!value["Date"].isNull())
					  {
						  usleep(100);

					  ret= 1;


					  }
				 }

			}
			else
			{
//						printf("-----> 数据不存在！\n");
				ret= 0;
			}


//					printf("Findby_Instrumentid check end! ...\n");
			return ret;
		}

	std::string MongoAveragePrice::GetLastDate(const char *pinstrumentid )
			{
				std::string lastdate;
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "Date" << -1 ;   //
				opts.sort(order_builder.view()).limit(1);

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(bsoncxx::builder::stream::document{} << finalize,opts);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["Date"].isNull())
						  {


						  lastdate=value["Date"].asString();


						  }
					 }

				}
				else
				{
	//						printf("-----> 数据不存在！\n");
					lastdate= "00000000";
				}


	//					printf("Findby_Instrumentid check end! ...\n");
				return lastdate;
			}
	int MongoAveragePrice::Count(const char * pinstrumentid)
					{
						mongocxx::uri uri(_mongodb_url);
						mongocxx::client client(uri);
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[pinstrumentid];

						int i =0;
						auto cursor = collection.find(document{} << finalize);
						for (auto&& doc : cursor)
						{
							i++;
						}


						return i;

					}
