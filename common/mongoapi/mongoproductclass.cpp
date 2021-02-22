#include "mongoproductclass.hpp"


MongoProductClass::MongoProductClass()
		{
//			printf("MongoMainContract 构造函数 入口 ...\n");
			mongocxx::instance inst{};

			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"baseinfo");
			strcpy(_mongodb_collenction,"productclass");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];
		}

	void MongoProductClass::DeleteAll()
			{
	//			printf("MongoDepthMarket Delete begin! ...\n");
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				collection.delete_many(document{} << finalize);
				printf("MongoProductClass DeleteAll finished! ...\n");
			}

	void MongoProductClass::Insert(ProductClassInfo productclassinfo)
		{
//			printf("MongoMainContract Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];


			document doc{};
			doc<< "ProductID" << productclassinfo.ProductID
			<< "ExchangeID" << productclassinfo.ExchangeID
			<< "Section" << productclassinfo.Section;

	        collection.insert_one(doc.view());

			printf("MongoProductClass Insert finished! ...\n");
		}

	void MongoProductClass::Check_ProductClass(ProductClassInfo productclassinfo)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				document doc{};

				doc<< "ProductID" << productclassinfo.ProductID
							<< "ExchangeID" << productclassinfo.ExchangeID
							<< "Section" << productclassinfo.Section
							<< "DayNight" << productclassinfo.DayNight;




				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "ProductID" << productclassinfo.ProductID<< finalize);
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



//				printf("MongoProductClass check end! ...\n");
			}

	ProductClassInfo MongoProductClass::Findby_ProductID(char * productid)
					{
						ProductClassInfo productclassinfo;
//						printf("MongoProductClass---_mongodb_url  %s\n",_mongodb_url);

						mongocxx::uri uri(_mongodb_url);
						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];


						bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
						  collection.find_one(document{} << "ProductID" << productid<< finalize);
						if(maybe_result) {

//						  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
						  Json::Reader  reader;
						  Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
						  {
							  if(!value["ProductID"].isNull())
							  {


							  strcpy(productclassinfo.Section,value["Section"].asString().c_str());
							  strcpy(productclassinfo.ProductID,value["ProductID"].asString().c_str());
							  strcpy(productclassinfo.DayNight,value["DayNight"].asString().c_str());
							  strcpy(productclassinfo.ExchangeID,value["ExchangeID"].asString().c_str());

							  }
						  }
						  else
						  {
							  printf("-----> json 读取失败！ \n");
						  }

						}
						else
						{
							printf("-----> 数据不存在，插入新数据！\n");

						}

//						printf("MongoProductClass Findby_ProductID end! ...\n");
						return productclassinfo;

					}

//	char** MongoProductClass::Find_All()
//				{
//
//
//	//				printf("_mongodb_url  %s\n",_mongodb_url);
//					mongocxx::uri uri(_mongodb_url);
//
//					mongocxx::client client(uri);
//					//打开"baseinfo"数据库
//					mongocxx::database dbname = client[_mongodb_dbname];
//					mongocxx::collection collection = dbname[_mongodb_collenction];
//
//
//					mongocxx::cursor cursor = collection.find(document{} << finalize);
//					int i=0;
//					for(auto doc : cursor)
//					{
////					  std::cout << bsoncxx::to_json(doc) << "\n";
//
//					  Json::Reader  reader;
//					  Json::Value   value;
//
//						  if(reader.parse(bsoncxx::to_json(doc),value))
//						  {
//							  if(!value["ProductID"].isNull())
//							  {
//
//
//
//							  char *char_productid = new char[10];
//
//							  strcpy(char_productid,value["ProductID"].asString().c_str());
//							  pProductID[i]=char_productid;
////							  printf("Debug:MongoProductClass---------> productid [%d] :   %s  \n",i,pProductID[i]);
//							  i++;
//
//
//							  }
//						 }
//
//					}
//
//
//
////					printf("Find_All Find_All end! ...\n");
//
//
//
//					return pProductID;
//				}

	// 待修改，用于返回字符串数组
	void MongoProductClass::FindAll_ProductClass(vector<ProductClassInfo> &temp)
					{
						ProductClassInfo info;
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
								  if(!value["ProductID"].isNull())
								  {

								    strcpy(info.ProductID,value["ProductID"].asString().c_str());
								   	strcpy(info.ExchangeID,value["ExchangeID"].asString().c_str());
									if(strcmp(info.ExchangeID,"INE")!=0)
										{
											 temp.push_back(info);
											 printf("FindAll_ProductClass ----[%s]\n",info.ProductID);
										}


								  }
							 }

						}



						printf("Find_All Find_All end! ...\n");




					}


	int MongoProductClass::Count()
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
