#include "mongoinstrument.hpp"


	MongoInstrument::MongoInstrument()
		{
//			printf("MongoInstrument 构造函数 入口 ...\n");
//			mongocxx::instance inst{};
					////chenzw:123 -->用户：密码  参考博客用户管理
//			char _mongodb_url[] = "tcp://180.168.146.187:10001";
//
//			char _mongodb_dbname[] ="baseinfo";
//			char _mongodb_collenction[] ="exchangeinfo";


			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"baseinfo");
			strcpy(_mongodb_collenction,"instrumentinfo");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];



		}

	void MongoInstrument::DeleteAll()
				{
		//			printf("MongoDepthMarket Delete begin! ...\n");
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];


					collection.delete_many(document{} << finalize);
					printf("MongoInstrument DeleteAll finished! ...\n");
				}

	void MongoInstrument::Insert(InstrumentInfo instrumentinfo)
		{
//			printf("MongoInstrument Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			document doc{};
//			doc << "_id" << 7
			doc<< "InstrumentID" << instrumentinfo.InstrumentID
			<< "ExchangeID" << instrumentinfo.ExchangeID
			<< "InstrumentName" << instrumentinfo.InstrumentName
			<< "ExchangeInstID" << instrumentinfo.ExchangeInstID
			<< "ProductID" << instrumentinfo.ProductID
			<< "ProductClass" << instrumentinfo.ProductClass
			<< "DeliveryYear" << instrumentinfo.DeliveryYear
			<< "DeliveryMonth" << instrumentinfo.DeliveryMonth
			<< "VolumeMultiple" << instrumentinfo.VolumeMultiple
			<< "PriceTick" << instrumentinfo.PriceTick

			<< "CreateDate" << instrumentinfo.CreateDate
			<< "OpenDate" << instrumentinfo.OpenDate
			<< "ExpireDate" << instrumentinfo.ExpireDate
			<< "StartDelivDate" << instrumentinfo.StartDelivDate
			<< "EndDelivDate" << instrumentinfo.EndDelivDate
			<< "InstLifePhase" << instrumentinfo.InstLifePhase
			<< "LongMarginRatio" << instrumentinfo.LongMarginRatio
			<< "ShortMarginRatio" << instrumentinfo.ShortMarginRatio;







//		bsoncxx::document::view view = doc_value.view();

//			printf("MongoExchangeInfo Insert ready! ...  \n");
//			 document doc{};
//					                doc << "_id" << 6
//					                                << "No" << 2
//					                                << "info" << "test";
					                collection.insert_one(doc.view());

//			_collection.insert_one(view);
			printf("MongoInstrument Insert finished! ...\n");
		}

	void MongoInstrument::Check_Instrument(InstrumentInfo instrumentinfo)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				time_t tt = time(NULL);//这句返回的只是一个时间cuo
				tm* t= localtime(&tt);
				char s_year[10];
				char s_month[10];
				char s_day[10];
				char s_hours[10];
				char s_minutes[10];
				char s_seconds[10];

				sprintf(s_year,"%04d",t->tm_year+1900);
				sprintf(s_month,"%02d",t->tm_mon+1);
				sprintf(s_day,"%02d", t->tm_mday);
				sprintf(s_hours,"%02d",t->tm_hour);
				sprintf(s_minutes,"%02d",t->tm_min);
				sprintf(s_seconds,"%02d", t->tm_sec);

				std::string str_update;
				str_update.append(s_year);
				str_update.append(s_month);
				str_update.append(s_day);
				str_update.append(s_hours);
				str_update.append(s_minutes);
				str_update.append(s_seconds);

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

				document doc{};

	//			doc << "_id" << 7
				doc<< "InstrumentID" << instrumentinfo.InstrumentID
							<< "ExchangeID" << instrumentinfo.ExchangeID
							<< "InstrumentName" << instrumentinfo.InstrumentName
							<< "ExchangeInstID" << instrumentinfo.ExchangeInstID
							<< "ProductID" << instrumentinfo.ProductID
							<< "ProductClass" << instrumentinfo.ProductClass
							<< "DeliveryYear" << instrumentinfo.DeliveryYear
							<< "DeliveryMonth" << instrumentinfo.DeliveryMonth
							<< "VolumeMultiple" << instrumentinfo.VolumeMultiple
							<< "PriceTick" << instrumentinfo.PriceTick
							<< "CreateDate" << instrumentinfo.CreateDate
							<< "OpenDate" << instrumentinfo.OpenDate
							<< "ExpireDate" << instrumentinfo.ExpireDate
							<< "StartDelivDate" << instrumentinfo.StartDelivDate
							<< "EndDelivDate" << instrumentinfo.EndDelivDate
							<< "InstLifePhase" << instrumentinfo.InstLifePhase
							<< "LongMarginRatio" << instrumentinfo.LongMarginRatio
							<< "ShortMarginRatio" << instrumentinfo.ShortMarginRatio
							<< "UpdateTime" <<str_update.c_str()
				<< "LocalUpdateTime" << str_localupdate.c_str();


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << instrumentinfo.InstrumentID<< finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");

					document filter_del{};

					filter_del<< "InstrumentID" << instrumentinfo.InstrumentID
								<< finalize;
					collection.delete_many(filter_del.view());
					printf("-----> 数据存在，删除后--->插入新数据！\n");
					collection.insert_one(doc.view());
				}
				else
				{
					printf("MongoInstrument::Check_Instrument-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoInstrument check end! ...\n");
			}


	InstrumentInfo MongoInstrument::Find_InstrumentID(InstrumentInfo instrumentinfo)
				{

		InstrumentInfo r;
	//				printf("_mongodb_url  %s\n",_mongodb_url);
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << instrumentinfo.InstrumentID<< finalize);


					if(maybe_result) {
					  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
					  printf("-----> 数据存在，-----无需插入数据！\n");

					  Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {
						  std::cout<<value["VolumeMultiple"].asInt()<<std::endl;
						  std::cout<<value["InstrumentID"].asString()<<std::endl;
						  std::cout<<value["ProductID"].asString()<<std::endl;

						  strcpy(r.InstrumentID,value["InstrumentID"].asString().c_str());
						  strcpy(r.ProductID,value["ProductID"].asString().c_str());
						  strcpy(r.InstrumentName,value["InstrumentName"].asString().c_str());
						  strcpy(r.ExchangeID,value["ExchangeID"].asString().c_str());


						 // r.InstrumentID= value["InstrumentID"].asString().c_str();
						  r.VolumeMultiple = value["VolumeMultiple"].asInt();
						  r.PriceTick = value["PriceTick"].asDouble();
						  r.LongMarginRatio = value["LongMarginRatio"].asDouble();
						  r.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
						  //r.ProductID = value["ProductID"].asString().c_str();
						  }
					  }
					  else
					  {
						  printf("-----> json 读取失败！ \n");
					  }

					}
					else
					{
						printf("-----> 数据不存在！\n");

					}

					return r;

					printf("MongoInstrument Find end! ...\n");
				}

	InstrumentInfo MongoInstrument::Findby_InstrumentID(char * pinstrumentid)
					{

						InstrumentInfo instrumentinfo;
		//				printf("_mongodb_url  %s\n",_mongodb_url);
						mongocxx::uri uri(_mongodb_url);

						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];

						bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
						  collection.find_one(document{} << "InstrumentID" << pinstrumentid<< finalize);


						if(maybe_result) {
//						  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
						//  printf("MongoInstrument check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//						  printf("MongoInstrument-----> Findby_InstrumentID，-----\n");

						  Json::Reader  reader;
						  Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
						  {
							  if(!value["InstrumentID"].isNull())
							  {
//							  std::cout<<value["VolumeMultiple"].asInt()<<std::endl;
//							  std::cout<<value["InstrumentID"].asString()<<std::endl;
//							  std::cout<<value["ProductID"].asString()<<std::endl;

							  strcpy(instrumentinfo.InstrumentID,value["InstrumentID"].asString().c_str());
							  strcpy(instrumentinfo.ProductID,value["ProductID"].asString().c_str());
							  strcpy(instrumentinfo.InstrumentName,value["InstrumentName"].asString().c_str());
							  strcpy(instrumentinfo.ExchangeID,value["ExchangeID"].asString().c_str());


							 // r.InstrumentID= value["InstrumentID"].asString().c_str();
							  instrumentinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
							  instrumentinfo.PriceTick = value["PriceTick"].asDouble();
							  instrumentinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
							  instrumentinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
							  //r.ProductID = value["ProductID"].asString().c_str();
							  }
						  }
						  else
						  {
							  printf("-----> json 读取失败！ \n");
						  }

						}
						else
						{
							printf("Findby_InstrumentID-----> 数据不存在！\n");

						}
//						printf("MongoInstrument Find end! ...\n");
						return instrumentinfo;


					}

	bool MongoInstrument::Exist_InstrumentID(char * pinstrumentid)
				{
					bool ret;
					ret=false;
					InstrumentInfo instrumentinfo;
	//				printf("_mongodb_url  %s\n",_mongodb_url);
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << pinstrumentid<< finalize);


					if(maybe_result) {
//						  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					//  printf("MongoInstrument check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//						  printf("MongoInstrument-----> Findby_InstrumentID，-----\n");

					  Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {
//							  std::cout<<value["VolumeMultiple"].asInt()<<std::endl;
//							  std::cout<<value["InstrumentID"].asString()<<std::endl;
//							  std::cout<<value["ProductID"].asString()<<std::endl;

						  strcpy(instrumentinfo.InstrumentID,value["InstrumentID"].asString().c_str());
						  strcpy(instrumentinfo.ProductID,value["ProductID"].asString().c_str());
						  strcpy(instrumentinfo.InstrumentName,value["InstrumentName"].asString().c_str());
						  strcpy(instrumentinfo.ExchangeID,value["ExchangeID"].asString().c_str());


						 // r.InstrumentID= value["InstrumentID"].asString().c_str();
						  instrumentinfo.VolumeMultiple = value["VolumeMultiple"].asInt();
						  instrumentinfo.PriceTick = value["PriceTick"].asDouble();
						  instrumentinfo.LongMarginRatio = value["LongMarginRatio"].asDouble();
						  instrumentinfo.ShortMarginRatio = value["ShortMarginRatio"].asDouble();
						  //r.ProductID = value["ProductID"].asString().c_str();
						  ret=true;
						  }
					  }
					  else
					  {
						  printf("-----> json 读取失败！ \n");
						  ret =false;
					  }

					}
					else
					{
						printf("Findby_InstrumentID-----> 数据不存在！\n");
						ret =false;

					}
//						printf("MongoInstrument Find end! ...\n");
					return ret;


				}
	std::string MongoInstrument::Find_LastUpdateTime()
				{

					std::string str_lastupdatetime;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					mongocxx::options::find opts;
					bsoncxx::builder::stream::document order_builder;
					order_builder << "UpdateTime" << -1 ;
					opts.sort(order_builder.view()).limit(1);

					auto cursor = collection.find(document{} << finalize, opts);
					for (auto&& doc : cursor)
					{
//						std::cout << "Find_LastUpdateTime ---->" <<bsoncxx::to_json(doc) << std::endl;

						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(doc),value))
						  {
							  if(!value["UpdateTime"].isNull())
							  {



								  str_lastupdatetime = value["UpdateTime"].asString();
//								  printf("MongoInstrument Find_LastUpdateTime ---->%s ...\n",str_lastupdatetime.c_str());

							  }
						 }
					// @end: cpp-query-sort

					}

//							printf("MongoInstrument Find_LastUpdateTime end! ...\n");
					return str_lastupdatetime;


				}

	int MongoInstrument::Count()
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

	// 轮休instrument 生成productclass 新表
	void MongoInstrument::Check_ProductClass()
					{


		//				printf("_mongodb_url  %s\n",_mongodb_url);
						mongocxx::uri uri(_mongodb_url);

						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];


						mongocxx::cursor cursor = collection.find(document{} << finalize);
						for(auto doc : cursor) {
//						  std::cout << bsoncxx::to_json(doc) << "\n";

						  Json::Reader  reader;
						  Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(doc),value))
							  {
								  if(!value["ProductID"].isNull())
								  {

//								  std::cout<<value["ExchangeID"].asString()<<std::endl;
//								  std::cout<<value["ProductID"].asString()<<std::endl;

								  ProductClassInfo productclassinfo;
								  MongoProductClass mongoproductclass;

								  strcpy(productclassinfo.ProductID,value["ProductID"].asString().c_str());
								  strcpy(productclassinfo.ExchangeID,value["ExchangeID"].asString().c_str());

								  if (!strcmp(productclassinfo.ProductID,"ZC"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"FG"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"jm"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"j"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"i"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"rb"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"hc"))
										  {
											  strcpy(productclassinfo.Section,"Black");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"TA"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"MA"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"v"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"pp"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"sc"))  //原油   待调整
											  {
												  strcpy(productclassinfo.Section,"Oil");
												  strcpy(productclassinfo.DayNight,"Day");
											  }
								  else if (!strcmp(productclassinfo.ProductID,"l"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"ru"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"bu"))
										  {
											  strcpy(productclassinfo.Section,"Oil");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"RM"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"OI"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"CF"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"y"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"p"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"m"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"jd"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"cs"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"c"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"a"))
										  {
											  strcpy(productclassinfo.Section,"Agriculture");
											  strcpy(productclassinfo.DayNight,"DayNight");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"SR"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"au"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"ag"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"zn"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"pb"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"sn"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"ni"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"cu"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"al"))
									  {
										  strcpy(productclassinfo.Section,"Other");
										  strcpy(productclassinfo.DayNight,"DayNight");
									  }
								  else if (!strcmp(productclassinfo.ProductID,"SF"))  // 硅铁
										  {
											  strcpy(productclassinfo.Section,"Other");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"SM"))  // 锰硅
										  {
											  strcpy(productclassinfo.Section,"Other");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"TF"))  // 5年国债
										  {
											  strcpy(productclassinfo.Section,"Finance");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"T"))  // 10年国债
										  {
											  strcpy(productclassinfo.Section,"Finance");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"IC"))  // 中证500
										  {
											  strcpy(productclassinfo.Section,"Finance");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"IF"))  // 沪深300
										  {
											  strcpy(productclassinfo.Section,"Finance");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else if (!strcmp(productclassinfo.ProductID,"IH"))  // 上证500
										  {
											  strcpy(productclassinfo.Section,"Finance");
											  strcpy(productclassinfo.DayNight,"Day");
										  }
								  else
									  {
										  strcpy(productclassinfo.Section,"Nothing");
										  strcpy(productclassinfo.DayNight,"No");
									  }

								  mongoproductclass.Check_ProductClass(productclassinfo);
								  }
							 }

						}





//						printf("MongoInstrument Find end! ...\n");
					}
