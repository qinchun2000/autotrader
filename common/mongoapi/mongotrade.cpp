#include "mongotrade.hpp"


	MongoTrade::MongoTrade()
		{
//			printf("MongoTrade 构造函数 入口 ...\n");
			mongocxx::instance inst{};
			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"account");
			strcpy(_mongodb_collenction,"trade");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];



		}

	void MongoTrade::Insert(TradeInfo tradeinfo)
		{
//			printf("MongoExchangeInfo Insert begin! ...\n");

			printf("_mongodb_url  %s\n",_mongodb_url);
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			document doc{};
			doc<< "InstrumentID" << tradeinfo.InstrumentID
				<< "Price" << tradeinfo.Price
				<< "TradeDate" << tradeinfo.TradeDate
				<< "TradeTime" << tradeinfo.TradeTime
				<< "Direction" << tradeinfo.Direction
				<< "OffsetFlag" << tradeinfo.OffsetFlag
				<< "TradeID" << tradeinfo.TradeID
				<< "OrderRef" << tradeinfo.OrderRef
				<< "SequenceNo" << tradeinfo.SequenceNo;

            collection.insert_one(doc.view());

//			_collection.insert_one(view);
//			printf("MongoExchangeInfo Insert finished! ...\n");
		}

	void MongoTrade::Check_Trade(TradeInfo tradeinfo)
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


				doc<< "InstrumentID" << tradeinfo.InstrumentID
								<< "Price" << tradeinfo.Price
								<< "Volume" << tradeinfo.Volume
								<< "TradingDay" << tradeinfo.TradingDay
								<< "TradeDate" << tradeinfo.TradeDate
								<< "TradeTime" << tradeinfo.TradeTime
								<< "Direction" << tradeinfo.Direction
								<< "OffsetFlag" << tradeinfo.OffsetFlag
								<< "TradeID" << tradeinfo.TradeID
								<< "SettlementID" << tradeinfo.SettlementID
								<< "ClientID" << tradeinfo.ClientID
								<< "OrderRef" << tradeinfo.OrderRef
								<< "SequenceNo" << tradeinfo.SequenceNo
								<< "LocalUpdateTime" << str_localupdate ;


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << tradeinfo.InstrumentID
						  << "TradeDate" << tradeinfo.TradeDate
						  << "TradeID" << tradeinfo.TradeID<< finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");
				}
				else
				{
//					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoExchangeInfo check end! ...\n");
			}


	int MongoTrade::Count()
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

	void MongoTrade::DeleteAll()
					{
			//			printf("MongoTrade Delete begin! ...\n");
						mongocxx::uri uri(_mongodb_url);
						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];


						collection.delete_many(document{} << finalize);
						printf("MongoTrade DeleteAll finished! ...\n");
					}
