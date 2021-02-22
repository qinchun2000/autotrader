#include "mongomindata.hpp"


	MongoMinData::MongoMinData()
		{
//			printf("MongoDepthMarket 构造函数 入口 ...\n");
			mongocxx::instance inst{};
					////chenzw:123 -->用户：密码  参考博客用户管理
//			char _mongodb_url[] = "tcp://180.168.146.187:10001";
//
//			char _mongodb_dbname[] ="baseinfo";
//			char _mongodb_collenction[] ="exchangeinfo";


			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"ctpmindata");
//			strcpy(_mongodb_collenction,"depthmarket");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
//			_collection = _dbname[_mongodb_collenction];



		}

	void MongoMinData::Insert(MinuteData minutedata)
		{
//			printf("MongoMinData Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[minutedata.InstrumentID];


			document doc{};

//			doc << "_id" << 7
			doc<< "InstrumentID" << minutedata.InstrumentID
			<< "Date" << minutedata.Date
			<< "MinuteTime" << minutedata.MinuteTime
			<< "OpenPrice" << minutedata.OpenPrice
			<< "HighestPrice" << minutedata.HighestPrice
			<< "LowestPrice" << minutedata.LowestPrice
			<< "ClosePrice" << minutedata.ClosePrice
			<< "OpenInterest" << minutedata.OpenInterest
			<< "Volume" << minutedata.Volume;




//		bsoncxx::document::view view = doc_value.view();

//			printf("MongoExchangeInfo Insert ready! ...  \n");
//			 document doc{};
//			doc << "_id" << 6
//			<< "No" << 2
//			<< "info" << "test";
			collection.insert_one(doc.view());


			printf("MongoMinData Insert finished! ...\n");
		}

	void MongoMinData::Check_MinuteData(MinuteData minutedata)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[minutedata.InstrumentID];


				document doc{};

				doc<< "InstrumentID" << minutedata.InstrumentID
				<< "Date" << minutedata.Date
				<< "MinuteTime" << minutedata.MinuteTime
				<< "OpenPrice" << minutedata.OpenPrice
				<< "HighestPrice" << minutedata.HighestPrice
				<< "LowestPrice" << minutedata.LowestPrice
				<< "ClosePrice" << minutedata.ClosePrice
				<< "OpenInterest" << minutedata.OpenInterest
				<< "Volume" << minutedata.Volume;

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << minutedata.InstrumentID
						  <<"Date" << minutedata.Date
						  <<"MinuteTime" << minutedata.MinuteTime<< finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
				  printf("-----> 数据存在，-----无需插入数据！\n");
				}
				else
				{
//					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoMinData check end! ...\n");
			}

	DepthMarket MongoMinData::Find_Maincontract(const char *productid )
				{
					DepthMarket depthmarket;

		//				printf("_mongodb_url  %s\n",_mongodb_url);
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];


					bsoncxx::builder::stream::document filter_builder;
					        filter_builder << "ProductID" << productid ;
					// Sort query results.

					        // @begin: cpp-query-sort
					        mongocxx::options::find opts;
					        bsoncxx::builder::stream::document order_builder;
					        order_builder << "OpenInterest" << -1 ;
					        opts.sort(order_builder.view()).limit(1);

					        auto cursor = collection.find(filter_builder.view(), opts);
					        for (auto&& doc : cursor)
					        {
					            std::cout << bsoncxx::to_json(doc) << std::endl;

					            Json::Reader  reader;
					          					  Json::Value   value;

								  if(reader.parse(bsoncxx::to_json(doc),value))
								  {
									  if(!value["InstrumentID"].isNull())
									  {
									  std::cout<<value["OpenInterest"].asDouble()<<std::endl;
									  std::cout<<value["InstrumentID"].asString()<<std::endl;
									  std::cout<<value["ProductID"].asString()<<std::endl;

									  strcpy(depthmarket.InstrumentID,value["InstrumentID"].asString().c_str());
									  strcpy(depthmarket.ProductID,value["ProductID"].asString().c_str());

									 // r.InstrumentID= value["InstrumentID"].asString().c_str();
									depthmarket.OpenInterest = value["OpenInterest"].asDouble();
									depthmarket.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
									depthmarket.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
									depthmarket.Volume = value["Volume"].asDouble();
									  //r.ProductID = value["ProductID"].asString().c_str();
									  }
								 }
					        // @end: cpp-query-sort

					        }


					printf("MongoDepthMarket check end! ...\n");
					return depthmarket;
				}
