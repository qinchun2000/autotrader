#include "mongoexchangeinfo.hpp"


	MongoExchangeInfo::MongoExchangeInfo()
		{
//			printf("MongoExchangeInfo 构造函数 入口 ...\n");
			mongocxx::instance inst{};
			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"baseinfo");
			strcpy(_mongodb_collenction,"exchangeinfo");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];



		}

	void MongoExchangeInfo::Insert(ExchangeInfo exchangeinfo)
		{
//			printf("MongoExchangeInfo Insert begin! ...\n");

			printf("_mongodb_url  %s\n",_mongodb_url);
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			document doc{};
			doc<< "ExchangeID" << exchangeinfo.ExchangeID
				<< "ExchangeName" << exchangeinfo.ExchangeName
				<< "ExchangeProperty" << exchangeinfo.ExchangeProperty
				<< "ExchangeP" << exchangeinfo.ExchangeP;

            collection.insert_one(doc.view());

//			_collection.insert_one(view);
//			printf("MongoExchangeInfo Insert finished! ...\n");
		}

	void MongoExchangeInfo::Check_ExchangeInfo(ExchangeInfo exchangeinfo)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				document doc{};

	//			doc << "_id" << 7
				doc<< "ExchangeID" << exchangeinfo.ExchangeID
				<< "ExchangeName" << exchangeinfo.ExchangeName
				<< "ExchangeProperty" << exchangeinfo.ExchangeProperty;


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "ExchangeName" << exchangeinfo.ExchangeName<< finalize);
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


	int MongoExchangeInfo::Count()
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
