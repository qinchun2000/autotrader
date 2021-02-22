#include "mongocycle.hpp"


	MongoCycle::MongoCycle()
		{
//			printf("MongoDepthMarket 构造函数 入口 ...\n");
			mongocxx::instance inst{};
					////chenzw:123 -->用户：密码  参考博客用户管理
//			char _mongodb_url[] = "tcp://180.168.146.187:10001";
//
//			char _mongodb_dbname[] ="baseinfo";
//			char _mongodb_collenction[] ="exchangeinfo";


			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"cycle");
//			strcpy(_mongodb_collenction,"depthmarket");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
//			_collection = _dbname[_mongodb_collenction];



		}

	void MongoCycle::Insert(CycleData cycledata)
		{
//			printf("MongoAveragePrice Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[cycledata.InstrumentID];

			document doc{};

			doc<< "Date" << cycledata.Date
								<< "High20" << cycledata.High20
								<< "HighCount" << cycledata.HighCount
								<< "HighDate" << cycledata.HighDate
								<< "Low20" << cycledata.Low20
								<< "LowCount" << cycledata.LowCount
								<< "LowDate" << cycledata.LowDate;

			collection.insert_one(doc.view());


			printf("MongoCycle Insert finished! ...\n");
		}

	void MongoCycle::Check_CycleData(CycleData cycledata)
			{

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[cycledata.InstrumentID];


				document doc{};


				doc<< "Date" << cycledata.Date
						<< "High20" << cycledata.High20
						<< "HighCount" << cycledata.HighCount
						<< "HighDate" << cycledata.HighDate
						<< "Low20" << cycledata.Low20
						<< "LowCount" << cycledata.LowCount
						<< "LowDate" << cycledata.LowDate;

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "Date" << cycledata.Date<< finalize);

				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoCycle check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
				  printf("-----> 数据存在，-----无需插入数据！\n");
				}
				else
				{
					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}

				printf("MongoCycle Check_CycleData end! ...\n");
			}

	int MongoCycle::Check_Highcount(const char* pinstrumentid,const char * date,double high20)
				{
					int highcount=1;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[pinstrumentid];

					mongocxx::options::find opts;
					bsoncxx::builder::stream::document order_builder;
					order_builder << "Date" << -1 ;   //
					opts.sort(order_builder.view());

					auto cursor = collection.find(document{} << "Date" << open_document << "$lt" << date
									  << close_document << finalize,opts);
					for (auto&& doc : cursor)
					{
//						std::cout << "Check_CycleData highcount ---->" <<bsoncxx::to_json(doc) << std::endl;

						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(doc),value))
						  {
							  if(!value["Date"].isNull())
							  {
								  std::string str_date;
								  str_date= value["Date"].asString();
								  double last_high20 = value["High20"].asDouble();
								  if (last_high20 == high20)
								  {
									  highcount++;

								  }
								  else
								  {
//									  printf("%s---------->last_high: %f    high20: %f  \n",str_date.c_str(),last_high20,high20);
									  break;
								  }

							  }

						  }

					}

					return highcount;


//					printf("MongoCycle Check_Highcount end! ...\n");
				}

	int MongoCycle::Check_Lowcount(const char* pinstrumentid,const char * date,double Low20)
					{
						int lowcount=1;
						mongocxx::uri uri(_mongodb_url);

						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[pinstrumentid];

						mongocxx::options::find opts;
						bsoncxx::builder::stream::document order_builder;
						order_builder << "Date" << -1 ;   //
						opts.sort(order_builder.view());

						auto cursor = collection.find(document{} << "Date" << open_document << "$lt" << date
										  << close_document << finalize,opts);
						for (auto&& doc : cursor)
						{
//							std::cout << "Check_CycleData highcount ---->" <<bsoncxx::to_json(doc) << std::endl;

							Json::Reader  reader;
							Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(doc),value))
							  {
								  if(!value["Date"].isNull())
								  {
									  std::string str_date;
									  str_date= value["Date"].asString();
									  double last_low20 = value["Low20"].asDouble();
									  if (last_low20 == Low20)
									  {
										  lowcount++;

									  }
									  else
									  {
//										  printf("%s---------->last_high: %f    high20: %f  \n",str_date.c_str(),last_low20,Low20);
										  break;
									  }

								  }

							  }

						}

						return lowcount;


//						printf("MongoCycle Check_Lowcount end! ...\n");
					}

	void   MongoCycle::Check_HighLowCount(const char* pinstrumentid,const char * date,CycleData  cycledata)
				{
					int highcount=1;
					int lowcount=1;
					CycleData  insertdata;
					strcpy(insertdata.InstrumentID,pinstrumentid);
					strcpy(insertdata.Date,cycledata.Date);
					strcpy( insertdata.HighDate,cycledata.HighDate);
					strcpy(insertdata.LowDate,cycledata.LowDate);
					insertdata.High20 = cycledata.High20;
					insertdata.Low20 = cycledata.Low20;
					insertdata.HighCount =highcount;
					insertdata.LowCount = lowcount;

					bool highflag=false;
					bool lowflag =false;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[pinstrumentid];



					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
								  collection.find_one(document{} << "Date" << date << finalize);

								if(maybe_result) {

//										  printf("%s-----> %s 存在cycle数据，不再插入新数据！\n",pinstrumentid,date);

								}
								else
								{


									mongocxx::options::find opts;
									bsoncxx::builder::stream::document order_builder;
									order_builder << "Date" << -1 ;   //
									opts.sort(order_builder.view());

									auto cursor = collection.find(document{} << "Date" << open_document << "$lt" << date
													  << close_document << finalize,opts);
									for (auto&& doc : cursor)
									{
				//						std::cout << "Check_CycleData highcount ---->" <<bsoncxx::to_json(doc) << std::endl;

										Json::Reader  reader;
										Json::Value   value;

										  if(reader.parse(bsoncxx::to_json(doc),value))
										  {
											  if(!value["Date"].isNull())
											  {
												  std::string str_date;
												  str_date= value["Date"].asString();
												  double last_high20 = value["High20"].asDouble();
												  double last_low20 = value["Low20"].asDouble();
												  if (last_high20 == cycledata.High20 && highflag==false)
												  {
													  highcount++;
													  insertdata.HighCount =highcount;
				//									  printf("%s---------->last_high: %f    high20: %f ----> %d  \n",
				//											  str_date.c_str(),last_high20,cycledata.High20,highcount);
												  }
												  else
												  {
													  highflag=true;

												  }

												  if (last_low20 == cycledata.Low20 && lowflag==false)
												  {
													  lowcount++;
													  insertdata.LowCount = lowcount;
				//									  printf("%s---------->last_low: %f    low20: %f ----> %d  \n",
				//														  str_date.c_str(),last_low20,cycledata.Low20,lowcount);
												  }
												  else
												  {
													  lowflag=true;
				//									  printf("%s---------->last_high: %f    high20: %f  \n",str_date.c_str(),last_high20,high20);

												  }

												  if(highflag && lowflag)
												  {

				//									  printf("break --->%s---------->highcount: %d    lowcount: %d    \n",
				//													  str_date.c_str(),highcount,lowcount);

														 break;
												  }

											  }

										  }

									}


									document doc{};


									doc<< "Date" << insertdata.Date
											<< "High20" << insertdata.High20
											<< "HighCount" << insertdata.HighCount
											<< "HighDate" << insertdata.HighDate
											<< "Low20" << insertdata.Low20
											<< "LowCount" << insertdata.LowCount
											<< "LowDate" << insertdata.LowDate;

									 printf("%s-----> %s 数据不存在！\n",pinstrumentid,date);
									 collection.insert_one(doc.view());

								}



//					printf("MongoCycle Check_Highcount end! ...\n");
				}
	CycleData MongoCycle::Findby_Instrumentid(const char *pinstrumentid ,const char*date)
				{
					CycleData  cycledata;
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

							  strcpy(cycledata.Date,value["Date"].asString().c_str());
							  cycledata.High20 =value["High20"].asDouble();
							  cycledata.Low20 =value["Low20"].asDouble();
							  cycledata.HighCount =value["HighCount"].asInt();
							  cycledata.LowCount =value["LowCount"].asInt();


							  }
						 }

					}
					else
					{
//						printf("-----> 数据不存在！\n");
						cycledata.High20 = 0;
						cycledata.Low20 = 0;
					}


//					printf("Findby_Instrumentid check end! ...\n");
					return cycledata;
				}

	int MongoCycle::Existby_Instrumentid(const char *pinstrumentid ,const char*date)
		{
			printf("MongoCycle Existby_Instrumentid begin! ...\n");
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
					  printf("%s-----> %s 历史周期价差表 找到！\n",pinstrumentid,date);

					  ret= 1;



			}
			else
			{
				 printf("%s-----> %s 数据不存在！\n",pinstrumentid,date);

				ret= 0;
			}



			return ret;
		}

	std::string MongoCycle::Find_LastDate(char * pinstrumentid)
				{

					std::string str_date;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[pinstrumentid];

					mongocxx::options::find opts;
					bsoncxx::builder::stream::document order_builder;
					order_builder << "Date" << -1 ;   //此处取时间最近的那个数据，要确保cycle 是当天交易日的最新cycle数据
					opts.sort(order_builder.view()).limit(1);

					auto cursor = collection.find(document{} << finalize, opts);
					for (auto&& doc : cursor)
					{
	//						std::cout << "Find_LastUpdateTime ---->" <<bsoncxx::to_json(doc) << std::endl;

						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(doc),value))
						  {
							  if(!value["Date"].isNull())
							  {



								  str_date = value["Date"].asString();
	//								  printf("MongoInstrument Find_LastUpdateTime ---->%s ...\n",str_lastupdatetime.c_str());

							  }
						 }
					// @end: cpp-query-sort

					}

	//							printf("MongoInstrument Find_LastUpdateTime end! ...\n");
					return str_date;


				}

