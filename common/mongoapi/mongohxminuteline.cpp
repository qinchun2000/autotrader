#include "mongohxminuteline.hpp"


	MongoHxMinuteLine::MongoHxMinuteLine()
		{
//			printf("MongoHxMinuteLine 构造函数 入口 ...\n");
			mongocxx::instance inst{};
			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"minuteline");

			mongocxx::uri _uri(_mongodb_url);
			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];

		}

	void MongoHxMinuteLine::Check_HttpMinuteLine(const char * pinstrumentid,const char *  date)
		{
			std::string url_head ="http://webftcn.hermes.hexun.com/shf/historyminute?code=";
			std::string url_tail ="&date=";

			std::string cffeurl_head ="http://webcffex.hermes.hexun.com/cffex/historyminute?code=";
			std::string cffeurl_tail = "&date=";

			char ProductID[31];
			std::string str_ProductID;
			std::string str_ExchangeID;
			std::string str_InstrumentID=pinstrumentid;
			std::string str_req;

			std::string pattern("(^[a-zA-Z]*[^0-9]+)"); // ([a-z]+):([0-9]+) also works here

			boost::regex reg(pattern);    //查找字符串里的字母
			boost::cmatch mat;
			if(boost::regex_search( pinstrumentid, mat, reg))
			{
		//			cout << "searched:" << mat[0] << endl;
		//						printf("----->  %s 匹配主力合约的关键字--mat-[%d]--->%s \n", pinstrumentID[i], i,mat[0].str().c_str());
			}
			std::string url ;
			strcpy(ProductID,mat[0].str().c_str());
			str_ProductID = mat[0].str();

			ProductClassInfo productclassinfo;
			MongoProductClass mongoproductclass;
			productclassinfo = mongoproductclass.Findby_ProductID(ProductID);
			str_ExchangeID = productclassinfo.ExchangeID;
			if (str_ProductID.compare("ru")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}
			else if(str_ProductID.compare("au")==0 || str_ProductID.compare("ag")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append("2");
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}
			else if(str_ExchangeID.compare("SHFE")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append("3");
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}
			else if(str_ExchangeID.compare("CZCE")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID.substr(0,2));
				str_req.append("1");
				str_req.append(str_InstrumentID.substr(2,3));
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);

			}
			else if(str_ExchangeID.compare("CFFEX")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID);
				url.append(cffeurl_head);
				url.append(str_req);
				url.append(cffeurl_tail);
				printf("cffeurl_tail ------> %s \n",cffeurl_tail.c_str());

			}
			else
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
			}



			printf ("req ------->  %s \n",str_req.c_str());
			printf ("url ------->  %s    date: %s\n",url.c_str(),date);

			std::string response_data;
			HttpBase httpbase;
			httpbase.get_url_response(url,response_data);
			printf ("response_data ------->  %s \n",response_data.c_str());
//			Parse_MinuteLineData(response_data,str_InstrumentID);



		}

	void MongoHxMinuteLine::Insert(const char * pinstrumentid,const char *  date,std::string response)
		{
//			printf("MongoHxDayLine Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];

			Json::Reader  reader;
			Json::Value   value;

			if(reader.parse(response,value))
			{
			  if(!value["Data"].isNull())
			  {
		//		  char data[response_data.length()];
		//		  strcpy (data,value["Data"].asString().c_str());
		//		  string str_data = value["Data"].asString();

				  double priceweight;
				  int sz = value["Data"].size();
				  std::string LastClose;
				  std::string High;
				  std::string Low;
				  std::string str_PriceWeight;
				  std::string OpenTime;
				  std::string CloseTime;

//				  printf ("Data length----->  %d \n",sz);
				  for (int i = 0; i < sz; ++i)
				  {

					if(i==1)
						{
							char buf[20];
							sprintf(buf, "%.3f", value["Data"][i].asDouble());
							LastClose = buf;
//							printf ("LastClose -----> %s \n",LastClose.c_str());
						}
					else if(i==2)
						{
							char buf[20];
							sprintf(buf, "%.3f", value["Data"][i].asDouble());
							High = buf;
//							printf ("High -----> %s \n",High.c_str());
						}
					else if(i==3)
						{
							char buf[20];
							sprintf(buf, "%.3f", value["Data"][i].asDouble());
							Low = buf;
//							printf ("Low -----> %s \n",Low.c_str());
						}
					else if(i==4)
						{
							priceweight = value["Data"][i].asDouble();
							char buf[20];
							sprintf(buf, "%.3f", value["Data"][i].asDouble());
							str_PriceWeight = buf;

//							printf ("PriceWeight -----> %f \n",priceweight);
						}
					else if(i==5)
						{
							char buf[20];
							sprintf(buf, "%.f", value["Data"][i].asDouble());
							OpenTime = buf;
//							printf ("OpenTime -----> %s \n",OpenTime.c_str());
						}
					else if(i==6)
						{
							char buf[20];
							sprintf(buf, "%.f", value["Data"][i].asDouble());
							CloseTime = buf;
//							printf ("CloseTime -----> %s \n",CloseTime.c_str());
						}

				  }

				  bsoncxx::builder::stream::document builder{};
				  auto in_array = builder << "LastClose" << LastClose
						  << "High" << High
						  << "Low" << Low
						  << "PriceWeight" << str_PriceWeight
						  << "OpenTime" << OpenTime
						  << "CloseTime" << CloseTime
						  << "MinData" << bsoncxx::builder::stream::open_array;

				  for (int i = 0; i < 1; ++i)
				  {
		//		          std::cout << "Data " << i + 1 << ": ";
					  int szi = value["Data"][i].size();
		//			  printf ("Data[i] length----->  %d \n",szi);
					  for (int j = 0; j < szi; ++j)
					  {
							  int szj = value["Data"][i][j].size();

							  std::string MinuteTime;
							  std::string Price;
							  std::string Amount;
							  std::string Volume;
							  std::string AvePrice;
							  std::string OpenInterest;

							  for (int k = 0; k < szj; ++k)
							  {
		//		        			  printf ("parse----->  %d \n",k);
								  if (k==0)
									  {
										  char buf[20];
										  sprintf(buf, "%.f", value["Data"][i][j][k].asDouble());
										  MinuteTime = buf;
//										  printf ("MinuteTime ---->  %s  \n",MinuteTime.c_str());

									  }
								  else if (k==1)
									  {
										  char buf[20];
										  sprintf(buf, "%.3f", value["Data"][i][j][k].asDouble()/priceweight);
										  Price = buf;
//										  printf ("Price ---->  %s \n",Price.c_str());
									  }
								  else if (k==2)
									  {
										  char buf[20];
										  sprintf(buf, "%.f", value["Data"][i][j][k].asDouble());
										  Amount = buf;
//										  printf ("Amount ---->  %s \n",Amount.c_str());
									  }
								  else if (k==3)
									  {
										  char buf[20];
										  sprintf(buf, "%.f", value["Data"][i][j][k].asDouble());
										  Volume = buf;
//										  printf ("Volume ---->  %s \n",Volume.c_str());
									  }
								  else if (k==4)
									  {
										  char buf[20];
										  sprintf(buf, "%.3f", value["Data"][i][j][k].asDouble()/priceweight);
										  AvePrice = buf;
//										  printf ("AvePrice ---->  %s \n",AvePrice.c_str());
									  }
								  else if (k==5)
									  {
										  char buf[20];
										  sprintf(buf, "%.f", value["Data"][i][j][k].asDouble());
										  OpenInterest = buf;
//										  printf ("OpenInterest ---->  %s \n",OpenInterest.c_str());
									  }

							  }

							  in_array = in_array << bsoncxx::builder::stream::open_document << "MinuteTime" << MinuteTime
									  << "Price" << Price
									  << "Amount" << Amount
									  << "Volume" << Volume
									  << "AvePrice" << AvePrice
									  << "OpenInterest" << OpenInterest
									  << bsoncxx::builder::stream::close_document;

					  }

					  auto after_array = in_array << bsoncxx::builder::stream::close_array;

					  bsoncxx::document::value doc = after_array << bsoncxx::builder::stream::finalize;

//					  std::cout <<" to_json "<< bsoncxx::to_json(doc) << std::endl;


					  std::string str_date =date;
					  if (str_date.compare(CloseTime.substr(0,8))==0)
					  {
						  collection.insert_one(doc.view());
						  printf("---->  不存在分时数据 : %s  日期: %s   数据日期:%s  写入分钟数据! ...\n",pinstrumentid,str_date.c_str(),CloseTime.substr(0,8).c_str());
					  }
					  else
					  {
//						  printf("数据写入日期不匹配 ---->  合约: %s  写入日期: %s   数据日期:%s  写入分钟数据! ...\n",pinstrumentid,str_date.c_str(),CloseTime.c_str());
					  }

				  }
			  }
			  else
			  {
				  printf (" Data is null \n");
			  }
			}




//			printf("MongoHxMinuteLine Insert finished! ...\n");


		}
	int MongoHxMinuteLine::Count(const char * pinstrumentid)
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
	bool MongoHxMinuteLine::Findby_CloseTime(const char * pinstrumentid,const char * closetime)
		{

			int ret =-1;
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];


			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "CloseTime" << closetime<< finalize);
			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
			//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 主力合约数据存在，-----无需插入数据！\n");
				Json::Reader  reader;
				Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
				  {
					  if(!value["CloseTime"].isNull())
					  {
						  return 1;
					  }
				  }
			}
			else
			{
//					printf("-----> 数据不存在！\n");
				return 0;
			}

		printf("MongoHxMinuteLine Findby_CloseTime  end! %d ...\n",ret);
			return ret;
		}

	bool MongoHxMinuteLine::Exist_CloseTime(const char * pinstrumentid,const char * closetime)
			{
				bool ret=false;

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "CloseTime" << closetime<< finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 主力合约数据存在，-----无需插入数据！\n");


							  ret=true;


				}
				else
				{
	//					printf("-----> 数据不存在！\n");
					ret=false;
				}


				return ret;
			}
	std::string  MongoHxMinuteLine::Get_LastCloseTime(const char * pinstrumentid)
			{

				std::string lastdate;
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "CloseTime" << -1 ;   //
				opts.sort(order_builder.view()).limit(1);

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(bsoncxx::builder::stream::document{} << finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoMainContract check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 主力合约数据存在，-----无需插入数据！\n");
					Json::Reader  reader;
					Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["CloseTime"].isNull())
						  {
							  lastdate=value["CloseTime"].asString();
						  }
					  }
				}
				else
				{
	//					printf("-----> 数据不存在！\n");
					lastdate= "00000000150000";
				}

//			printf("MongoHxMinuteLine Findby_CloseTime  end! %d ...\n",ret);
				return lastdate;
			}

	int MongoHxMinuteLine::CollectLastOpenInterest_CloseTime(const char * pinstrumentid,const char * closetime)
			{

			int LastOpenInterest;

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];

			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "CloseTime" << closetime<< finalize);
			if(maybe_result) {

				Json::Reader  reader;
				Json::Value   value;

			  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
			  {
				  if(!value["MinData"].isNull())
				  {
					  int sz = value["MinData"].size();
						//		  printf ("Data length----->  %d \n",sz);

					  char buf[40];
					  sprintf(buf, "%s", value["MinData"][sz-1]["OpenInterest"].asString().c_str());
					  LastOpenInterest = atof(buf);
//					  printf ("buf----->  %s \n",buf);
//					  printf ("Data length----->  %d  %d \n",sz,LastOpenInterest);

				  }
			  }
			}
			else
			{
//					printf("-----> 数据不存在！\n");

			}
//		printf("MongoHxMinuteLine CollectLastOpenInterest_CloseTime  ----> %d   end! ...\n",LastOpenInterest);
			return LastOpenInterest;
			}

	int MongoHxMinuteLine::CollectFirstOpenInterest_CloseTime(const char * pinstrumentid,const char * closetime)
			{
				int StartOpenInterest;

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "CloseTime" << closetime<< finalize);
				if(maybe_result) {

					Json::Reader  reader;
					Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
				  {
					  if(!value["MinData"].isNull())
					  {
						  int sz = value["MinData"].size();



							  int i=0;

							  char buf[40];
							  sprintf(buf, "%s", value["MinData"][i]["OpenInterest"].asString().c_str());
							  StartOpenInterest = atof(buf);
//							  printf ("buf----->  %s \n",buf);
//							  printf ("Data length----->  %d  %d \n",sz,StartOpenInterest);

					  }
				  }
				}
				else
				{
	//					printf("-----> 数据不存在！\n");

				}
//			printf("MongoHxMinuteLine CollectFirstOpenInterest_CloseTime  ----> %d   end! ...\n",StartOpenInterest);
				return StartOpenInterest;
			}

	double MongoHxMinuteLine::CollectAveragePrice_CloseTime(const char * pinstrumentid,const char * closetime)
			{
				double totalprice=0;
				double aveprice=0;
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "CloseTime" << closetime<< finalize);
				if(maybe_result) {

					Json::Reader  reader;
					Json::Value   root;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),root))
				  {
					  if(!root["MinData"].isNull())
					  {
						  int sz = root["MinData"].size();
						  printf ("Data length----->  %d \n",sz);

						  for (int i = 0; i < sz; ++i)
						  {
							  char buf[40];
							  sprintf(buf, "%s", root["MinData"][i]["Price"].asString().c_str());

							  //				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
//							  printf ("buf----->  %s \n",buf);
								  double price = atof(buf);
								  totalprice +=price;


						  }
						  aveprice = totalprice/sz;
					  }
				  }
				}
				else
				{
	//					printf("-----> 数据不存在！\n");

				}
//			printf("MongoHxMinuteLine CollectAveragePrice_CloseTime  ----> %f   end! ...\n",aveprice);
				return aveprice;
			}


	AveragePriceData MongoHxMinuteLine::CollectAveragePriceData_CloseTime(const char * pinstrumentid,const char * closetime)
				{

					AveragePriceData averagepricedata;
					std::string date =closetime;
					strcpy(averagepricedata.InstrumentID,pinstrumentid);
					strcpy(averagepricedata.Date,date.substr(0,8).c_str());

					double totalprice=0;
					double aveprice=0;
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[pinstrumentid];


					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "CloseTime" << closetime<< finalize);
					if(maybe_result) {

						Json::Reader  reader;
						Json::Value   root;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),root))
					  {
						  if(!root["MinData"].isNull())
						  {
							  int sz = root["MinData"].size();
//							  printf ("CollectAveragePriceData_CloseTime Data length----->  %d \n",sz);

							  for (int i = 0; i < sz; ++i)
							  {
								  char buf[40];
								  sprintf(buf, "%s", root["MinData"][i]["Price"].asString().c_str());

//								  				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
	//							  printf ("buf----->  %s \n",buf);
									  double price = atof(buf);
									  totalprice +=price;

									  if (i==0)
									  {
										  char buf_start[40];
										  sprintf(buf_start, "%s", root["MinData"][i]["OpenInterest"].asString().c_str());
										  averagepricedata.StartOpenInterest = atof(buf_start);
									  }

									  if (i==sz-1)
									  {
										  char buf_last[40];
										  sprintf(buf, "%s", root["MinData"][sz-1]["OpenInterest"].asString().c_str());
										  averagepricedata.OpenInterest = atof(buf_last);
									  }

							  }
							  aveprice = totalprice/sz;
							  averagepricedata.AveragePrice = aveprice;



						  }
					  }
					}
					else
					{
							printf("-----> %s  对应的 数据不存在！\n",closetime);

					}
	//			printf("MongoHxMinuteLine CollectAveragePrice_CloseTime  ----> %f   end! ...\n",aveprice);
					return averagepricedata;
				}

	void MongoHxMinuteLine::Check_HxMinuteLineData(const char * pinstrumentid,const char *  date)
		{

			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];


			std::string url_head ="http://webftcn.hermes.hexun.com/shf/historyminute?code=";
			std::string url_tail ="&date=";

			std::string cffeurl_head ="http://webcffex.hermes.hexun.com/cffex/historyminute?code=";
			std::string cffeurl_tail = "&date=";

			char ProductID[31];
			std::string str_ProductID;
			std::string str_ExchangeID;
			std::string str_InstrumentID=pinstrumentid;
			std::string str_req;

			std::string pattern("(^[a-zA-Z]*[^0-9]+)"); // ([a-z]+):([0-9]+) also works here

			boost::regex reg(pattern);    //查找字符串里的字母
			boost::cmatch mat;
			if(boost::regex_search( pinstrumentid, mat, reg))
			{
		//			cout << "searched:" << mat[0] << endl;
		//						printf("----->  %s 匹配主力合约的关键字--mat-[%d]--->%s \n", pinstrumentID[i], i,mat[0].str().c_str());
			}
			std::string url ;
			strcpy(ProductID,mat[0].str().c_str());
			str_ProductID = mat[0].str();

			ProductClassInfo productclassinfo;
			MongoProductClass mongoproductclass;
			productclassinfo = mongoproductclass.Findby_ProductID(ProductID);
			str_ExchangeID = productclassinfo.ExchangeID;
			if (str_ProductID.compare("ru")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}
			else if(str_ProductID.compare("au")==0 || str_ProductID.compare("ag")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append("2");
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}
			else if(str_ExchangeID.compare("SHFE")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append("3");
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}
			else if(str_ExchangeID.compare("CZCE")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID.substr(0,2));
				str_req.append("1");
				str_req.append(str_InstrumentID.substr(2,3));
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);

			}
			else if(str_ExchangeID.compare("CFFEX")==0)
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID);
				url.append(cffeurl_head);
				url.append(str_req);
				url.append(cffeurl_tail);
				url.append(date);
//				printf("cffeurl_tail ------> %s \n",cffeurl_tail.c_str());

			}
			else
			{
				str_req.append(str_ExchangeID);
				str_req.append(str_InstrumentID);
				url.append(url_head);
				url.append(str_req);
				url.append(url_tail);
				url.append(date);
			}



//			printf ("-----分时历史数据检测 ------->  %s \n",str_req.c_str());
//			printf ("date: %s url ------->  %s    \n",date,url.c_str());

			std::string response_data;
			HttpBase httpbase;
			int ret =httpbase.get_url_response(url,response_data);
//			printf ("response_data ------->  %s \n",response_data.c_str());
			if (ret ==0)
			{
				this->Insert(pinstrumentid,date,response_data.substr(1,-3));
			}



		}


