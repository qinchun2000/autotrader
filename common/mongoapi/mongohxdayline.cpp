#include "mongohxdayline.hpp"


	MongoHxDayLine::MongoHxDayLine()
		{
//			printf("MongoHxDayLine 构造函数 入口 ...\n");
			mongocxx::instance inst{};

			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"dayline");

//			mongocxx::uri _uri(_mongodb_url);
//			mongocxx::client _client(_uri);
//
//			//打开"baseinfo"数据库
//			mongocxx::database _dbname = _client[_mongodb_dbname];

		}

	void MongoHxDayLine::Insert(HxDayLineData hxdaylinedata)
		{
//			printf("MongoHxDayLine Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);


			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];

			mongocxx::collection collection = dbname[hxdaylinedata.InstrumentID];


			bsoncxx::builder::stream::document doc{};

			doc<< "Time" << hxdaylinedata.Time
				<< "Open" << hxdaylinedata.Open
				<< "High" << hxdaylinedata.High
				<< "Low" << hxdaylinedata.Low
				<< "Close" << hxdaylinedata.Close
				<< "LastClose" << hxdaylinedata.LastClose
				<< "Amount" << hxdaylinedata.Amount
				<< "Volume" << hxdaylinedata.Volume;


			collection.insert_one(doc.view());


			printf("MongoHxDayLine Insert finished! ...\n");
		}
	HxDayLineData MongoHxDayLine::Find_HxDayLineData(const char* pinstrumentid,const char * date)
			{
				HxDayLineData hxdaylinedata;
				printf("MongoHxDayLine Find_HxDayLineData begin! ...\n");
				mongocxx::uri _uri(_mongodb_url);
				mongocxx::client _client(_uri);
				mongocxx::database _dbname = _client[_mongodb_dbname];
				mongocxx::collection collection = _dbname[pinstrumentid];



				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(bsoncxx::builder::stream::document{} << "Time" <<  date << finalize);

				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
//				  printf("MongoHxDayLine Find_HxDayLineData ... %s \n",bsoncxx::to_json(*maybe_result).c_str() );

				  Json::Reader  reader;
				  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
				  {
					  strcpy(hxdaylinedata.InstrumentID,value["InstrumentID"].asString().c_str());
					  strcpy(hxdaylinedata.Time,value["Time"].asString().c_str());
					  hxdaylinedata.uTIme = value["uTime"].asUInt();
					  hxdaylinedata.LastClose = atof(value["LastClose"].asString().c_str());
					  hxdaylinedata.Open = atof(value["Open"].asString().c_str());
					  hxdaylinedata.High = atof(value["High"].asString().c_str());
					  hxdaylinedata.Low = atof(value["Low"].asString().c_str());
					  hxdaylinedata.Close = atof(value["Close"].asString().c_str());
					  hxdaylinedata.Amount = atof(value["Amount"].asString().c_str());
					  hxdaylinedata.Volume = atof(value["Volume"].asString().c_str());

				  }
				}
				else
				{
					printf("-----> 数据不存在，插入新数据！\n");

				}

				printf("MongoHxDayLine Find_HxDayLineData end! ...\n");
			}

	void MongoHxDayLine::Check_HxDayLineData(HxDayLineData hxdaylinedata)
		{

			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[hxdaylinedata.InstrumentID];

			 char LastClose[20];
			sprintf(LastClose, "%.3f", hxdaylinedata.LastClose);

			char Open[20];
			sprintf(Open, "%.3f", hxdaylinedata.Open);

			char High[20];
			sprintf(High, "%.3f", hxdaylinedata.High);

			char Low[20];
			sprintf(Low, "%.3f", hxdaylinedata.Low);

			char Close[20];
			sprintf(Close, "%.3f", hxdaylinedata.Close);

			char Amount[20];
			sprintf(Amount, "%f", hxdaylinedata.Amount);

			char Volume[20];
			sprintf(Volume, "%f", hxdaylinedata.Volume);



			bsoncxx::builder::stream::document doc{};

			doc<< "Time" << hxdaylinedata.Time
					 << "uTime" << hxdaylinedata.uTIme
					  << "Open" << Open
					<< "High" << High
					<< "Low" << Low
					<< "Close" << Close
					<< "LastClose" << LastClose
					<< "Amount" << Amount
					<< "Volume" << Volume;

			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(bsoncxx::builder::stream::document{} << "Time" <<  hxdaylinedata.Time << finalize);

			if(maybe_result) {
//			  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
			//  printf("MongoHxDayLine check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );

			  HxDayLineData find_hxdaylinedata;
			  Json::Reader  reader;
			  Json::Value   value;

			  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
			  {
				  strcpy(find_hxdaylinedata.InstrumentID,value["InstrumentID"].asString().c_str());
//				  strcpy(find_hxdaylinedata.Time,value["Time"].asString().c_str());

				  find_hxdaylinedata.uTIme = value["uTime"].asUInt();

				  find_hxdaylinedata.LastClose = atof(value["LastClose"].asString().c_str());
				  find_hxdaylinedata.Open = atof(value["Open"].asString().c_str());
				  find_hxdaylinedata.High = atof(value["High"].asString().c_str());
				  find_hxdaylinedata.Low = atof(value["Low"].asString().c_str());
				  find_hxdaylinedata.Close = atof(value["Close"].asString().c_str());
				  find_hxdaylinedata.Amount = atof(value["Amount"].asString().c_str());
				  find_hxdaylinedata.Volume = atof(value["Volume"].asString().c_str());



				  if (find_hxdaylinedata.Open != hxdaylinedata.Open
						  ||find_hxdaylinedata.Close != hxdaylinedata.Close
						  || find_hxdaylinedata.High != hxdaylinedata.High
						  || find_hxdaylinedata.Low != hxdaylinedata.Low
						  || find_hxdaylinedata.Volume != hxdaylinedata.Volume)
				  {


					  printf("-----> 数据存在，但是数据不同，需要更新！  %f \n",find_hxdaylinedata.Close);
					  collection.update_one(bsoncxx::builder::stream::document{} << "Time" << hxdaylinedata.Time << finalize,
							  bsoncxx::builder::stream::document{} << "$set" << open_document
//							  << "uTIme" << hxdaylinedata.uTIme
											  << "Open" << Open
											<< "High" << High
											<< "Low" << Low
											<< "Close" << Close
											<< "LastClose" << LastClose
											<< "Amount" << Amount
											<< "Volume" << Volume
											<< close_document << finalize);
				  }
				  else
				  {
//					  printf("-----> 数据存在，-----无需插入数据！\n");
				  }
			  }
			}
			else
			{
				printf("-----> 数据不存在，插入新数据！\n");
				 collection.insert_one(doc.view());
			}



//			printf("MongoHxDayLine Check_HxDayLineData end! ...\n");
		}

	void MongoHxDayLine::Check_AllHxMinData(const char *pinstrumentid )
		{
			HxDayLineData hxdaylinedata;
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];
			MongoHxMinuteLine mongohxminuteline;

			mongocxx::options::find opts;
			bsoncxx::builder::stream::document order_builder;
			order_builder << "Time" << -1 ;   //
			opts.sort(order_builder.view()).limit(80);

			auto cursor = collection.find(bsoncxx::builder::stream::document{} << finalize,opts);
			for (auto&& doc : cursor)
			{
//						std::cout << bsoncxx::to_json(doc) << std::endl;

				Json::Reader  reader;
				Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["Time"].isNull())
					  {

//							  std::cout<<value["Time"].asString()<<std::endl;
					  DateUtil dateutil;
					  std::string date = value["Time"].asString().substr(0,8);
					  std::string CloseTime ;
					  CloseTime.append(date);
					  CloseTime.append("150000");

					  std::string lastclosetime;
					  lastclosetime=mongohxminuteline.Get_LastCloseTime(pinstrumentid);
					  time_t t_closetime =dateutil.ConvertStringtoTime (CloseTime.c_str());
					  time_t t_lastclosetime =dateutil.ConvertStringtoTime (CloseTime.c_str());

					  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
						time_t local_t;
						local_t = dateutil.ConvertStringtoTime(str_localupdate.c_str());

					  if(t_closetime<local_t && t_closetime>t_lastclosetime)
						{
							 tm tm_;
							 time_t t_;

							  strptime(CloseTime.c_str(), "%Y%m%d%H%M%S", &tm_); //将字符串转换为tm时间
							  tm_.tm_isdst = -1;
							  t_  = mktime(&tm_); //将tm时间转换为秒时间


							  time_t tt = time(NULL);//这句返回的只是一个时间
							  tm* lastyear= localtime(&tt);
							  lastyear->tm_year  = lastyear->tm_year -1;

							  time_t  lastyear_t =mktime(lastyear);


//									  printf ("%s ----closetime %ld   lastyear %ld   localtime  %ld    \n",pinstrumentid,t_ ,lastyear_t,tt);
							 // 只下载一年内的分钟数据，如果请求的时当日未收盘的盘中分钟数据，则忽略
							  if (t_ < tt && t_ > lastyear_t)
							  {
								  printf (" ------->  不存在合约当日分钟数据，插入: %s 数据！！ \n",CloseTime.c_str());
//										  MongoHxMinuteLine mongohxminuteline;
								  mongohxminuteline.Check_HxMinuteLineData(pinstrumentid,CloseTime.substr(0,8).c_str());
							  }


						}



					  }
				 }


			}


//			printf("MongoHxDayLine::CheckHXMinData  end! ...\n");

		}


	double MongoHxDayLine::Find_OpenHighPrice(const char *pinstrumentid ,const char * opendate)
		{
			double High;
			High =0;
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];

			mongocxx::options::find opts;
			bsoncxx::builder::stream::document order_builder;
			order_builder << "Time" << -1 ;   //
			opts.sort(order_builder.view());

			auto cursor = collection.find(document{} << "Time" << open_document << "$gte" << opendate
							  << close_document << finalize,opts);
			for (auto&& doc : cursor)
			{
//						std::cout << "Find_OpenHighPrice highcount ---->" <<bsoncxx::to_json(doc) << std::endl;

				Json::Reader  reader;
				Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["Time"].isNull())
					  {


						  double h = atof(value["High"].asString().c_str());
						  if (h > High || High==0)
						  {
							  High =h;

						  }


					  }

				  }

			}

			return High;
		}

	double MongoHxDayLine::Find_OpenLowPrice(const char *pinstrumentid ,const char * opendate)
			{
				double Low;
				Low =0;
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "Time" << -1 ;   //
				opts.sort(order_builder.view());

				auto cursor = collection.find(document{} << "Time" << open_document << "$gte" << opendate
								  << close_document << finalize,opts);
				for (auto&& doc : cursor)
				{
	//						std::cout << "Find_OpenLowPrice ---->" <<bsoncxx::to_json(doc) << std::endl;

					Json::Reader  reader;
					Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["Time"].isNull())
						  {

							  double l = atof(value["Low"].asString().c_str());

							  if (l <  Low || Low==0 )
							  {
								  Low =l;

							  }


						  }

					  }

				}

				return Low;
			}
	void MongoHxDayLine::Update_CycleData(const char *pinstrumentid )
		{
			HxDayLineData hxdaylinedata;
			int i=0;
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];
			MongoCycle mongocycle;

					auto cursor = collection.find(bsoncxx::builder::stream::document{} << finalize);
					for (auto&& doc : cursor)
					{
//						std::cout << bsoncxx::to_json(doc) << std::endl;
						usleep(10);
						Json::Reader  reader;
						Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(doc),value) && i>20)
						  {
							  if(!value["Time"].isNull())
							  {

//							  std::cout<<value["Time"].asString().c_str()<<std::endl;
//							  char date[20];
							  std::string str_date;
							  str_date= value["Time"].asString();

							  this->Collect_CycleData(mongocycle,pinstrumentid,str_date.c_str());

//							  int ret = mongocycle.Existby_Instrumentid(pinstrumentid,str_date.c_str());
//							  if(ret==1)
//								{
////									printf (" ------->  存在合约当日cycle数据！！！！！ \n");
//								}
//							  else if(ret==0)
//								{
//									printf (" ------->  不存在合约当日cycle数据，插入: %s 数据！！ \n",str_date.c_str());
//
//									this->Collect_CycleData(mongocycle,pinstrumentid,str_date.c_str());
//								}

							  }
						 }
						  i++;
//						  printf (" i------[%d]-> ！！！！！ \n",i);

					}


//			printf("MongoHxDayLine::Update_CycleData  end! ...\n");

		}

	void MongoHxDayLine::Update_AveragePriceData(const char *pinstrumentid )
				{

					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[pinstrumentid];

					mongocxx::options::find opts;
					bsoncxx::builder::stream::document order_builder;
					order_builder << "Time" << -1 ;   //
					opts.sort(order_builder.view()).limit(60);

					MongoAveragePrice mongoaverageprice;
					MongoHxMinuteLine mongohxminuteline;

					auto cursor = collection.find(bsoncxx::builder::stream::document{} << finalize,opts);
					for (auto&& doc : cursor)
					{
		//							std::cout << bsoncxx::to_json(doc) << std::endl;
	//					usleep(500);
						Json::Reader  reader;
						Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value) )
					  {
						  if(!value["Time"].isNull())
						  {
									  std::cout<<value["Time"].asString().c_str()<<std::endl;
		//							  char date[20];
						  std::string str_date;
						  str_date= value["Time"].asString().substr(0,8);

						  std:string closetime;
							closetime.append(str_date);
							  //				printf("MongoHxDayLine Collect_AveragePriceData  --->   %s \n",closetime.c_str());
							std::string tail ="150000";
							closetime.append(tail);
							DateUtil dateutil;

							std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
							time_t local_t;

							local_t = dateutil.ConvertStringtoTime(str_localupdate.c_str());

							time_t t_closetime =dateutil.ConvertStringtoTime (closetime.c_str());
							std::string lastdate;

							lastdate=mongoaverageprice.GetLastDate(pinstrumentid);
							lastdate.append(tail);

							time_t t_lastdate;
							t_lastdate=dateutil.ConvertStringtoTime(lastdate.c_str());

							if(t_closetime<local_t && t_closetime>t_lastdate)
							{


	//							  int ret = mongoaverageprice.Existby_Instrumentid(pinstrumentid,str_date.c_str());

	//							if(ret==1)
	//								{
	//	//										printf (" ------->  存在合约当日Averageprice数据！！！！！ \n");
	//								}
	//							  else if(ret==0)
	//								{
											printf (" ------->  不存在合约当日Averageprice 数据，插入: %s 数据！！ \n",str_date.c_str());
	//										this->Collect_AveragePriceData(pinstrumentid,str_date.c_str());

											std::string time;
											time.append(str_date);
							//				printf("MongoHxDayLine Collect_AveragePriceData  --->   %s \n",closetime.c_str());
											std::string tail ="150000";
											time.append(tail);
							//				printf("MongoHxDayLine Collect_AveragePriceData  --->   %s \n",closetime.c_str());
											AveragePriceData averagepricedata;
							//				strcpy(averagepricedata.InstrumentID,pinstrumentid);
							//				strcpy(averagepricedata.Date,date);

							//				averagepricedata.AveragePrice = mongohxminuteline.CollectAveragePrice_CloseTime(pinstrumentid,closetime.c_str());
							//				averagepricedata.StartOpenInterest = mongohxminuteline.CollectFirstOpenInterest_CloseTime(pinstrumentid,closetime.c_str());
							//				averagepricedata.OpenInterest = mongohxminuteline.CollectLastOpenInterest_CloseTime(pinstrumentid,closetime.c_str());
											averagepricedata = mongohxminuteline.CollectAveragePriceData_CloseTime(pinstrumentid,time.c_str());

											printf (" ------->  averagepricedata: %s 数据收集完毕！！ \n",str_date.c_str());
											mongoaverageprice.Check_AveragePriceData(averagepricedata);
	//								}
							}
	//						else
	//						{
	//							printf (" ------->  交易日未收盘数据，不更新: %s 数据！！ \n",str_date.c_str());
	//
	//						}



						 }
						 }

	//						  printf (" i------[%d]-> ！！！！！ \n",i);

				}


		//			printf("MongoHxDayLine::Update_CycleData  end! ...\n");

				}

	void MongoHxDayLine::Find_UpdateDateList(vector<HxDayLineData> &temp,const char *pinstrumentid)
			{
				HxDayLineData info;
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[pinstrumentid];

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "Time" << -1 ;   //
				opts.sort(order_builder.view()).limit(30);


				auto cursor = collection.find(bsoncxx::builder::stream::document{} << finalize,opts);
				for (auto&& doc : cursor)
				{
	//							std::cout << bsoncxx::to_json(doc) << std::endl;
//					usleep(500);
					Json::Reader  reader;
					Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value) )
				  {
					  if(!value["Time"].isNull())
					  {
						  strcpy(info.Time,value["Time"].asCString());
						  temp.push_back(info);
					  }
					 }


			}


//			printf("MongoHxDayLine::Find_UpdateDateList  end! ...\n");

	}

	void MongoHxDayLine::UpdateNewDay_CycleData(const char *pinstrumentid ,const char *date)
	{

		MongoCycle mongocycle;
		int ret = mongocycle.Existby_Instrumentid(pinstrumentid,date);
		if(ret==1)
		{
		//									printf (" ------->  存在合约当日cycle数据！！！！！ \n");
		}
		else if(ret==0)
		{
			printf (" ------->  不存在合约当日cycle数据，插入: %s 数据！！ \n",date);
			this->Collect_CycleData(mongocycle,pinstrumentid,date);
		}

//			printf("MongoHxDayLine::UpdateNewDay_CycleData  end! ...\n");

	}
	void MongoHxDayLine::CheckHttpDayLine(char *pinstrumentid)
	{
		time_t tt = time(NULL);//这句返回的只是一个时间
		tm* t= localtime(&tt);

		char s_year[10];
		char s_lastyear[10];
		char s_month[10];
		char s_day[10];

		sprintf(s_lastyear,"%04d",t->tm_year + 1900-1);
		sprintf(s_year,"%04d",t->tm_year + 1900);
		sprintf(s_month,"%02d",t->tm_mon + 1);
		sprintf(s_day,"%02d", t->tm_mday);

		std::string url_head ="http://webftcn.hermes.hexun.com/shf/kline?code=";
		std::string url_tail1 ="&start=";
		std::string url_tail2 ="000000&number=1000&type=5";
		std::string url_tail;
		url_tail.append(url_tail1);
		url_tail.append(s_lastyear);
		url_tail.append(s_month);
		url_tail.append(s_day);
		url_tail.append(url_tail2);

		std::string cffeurl_head ="http://webcffex.hermes.hexun.com/cffex/kline?code=";
		std::string cffeurl_tail1 = "&start=";
		std::string cffeurl_tail2 = "091500&number=-1000&type=5";
		std::string cffeurl_tail ;


		cffeurl_tail.append(cffeurl_tail1);
		cffeurl_tail.append(s_year);
		cffeurl_tail.append(s_month);
		cffeurl_tail.append(s_day);
		cffeurl_tail.append(cffeurl_tail2);


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
		}
		else if(str_ProductID.compare("au")==0 || str_ProductID.compare("ag")==0)
		{
			str_req.append(str_ExchangeID);
			str_req.append("2");
			str_req.append(str_InstrumentID);
			url.append(url_head);
			url.append(str_req);
			url.append(url_tail);
		}
		else if(str_ExchangeID.compare("SHFE")==0)
		{
			str_req.append(str_ExchangeID);
			str_req.append("3");
			str_req.append(str_InstrumentID);
			url.append(url_head);
			url.append(str_req);
			url.append(url_tail);
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

		}
		else if(str_ExchangeID.compare("CFFEX")==0)
		{
			str_req.append(str_ExchangeID);
			str_req.append(str_InstrumentID);
			url.append(cffeurl_head);
			url.append(str_req);
			url.append(cffeurl_tail);

		}
		else
		{
			str_req.append(str_ExchangeID);
			str_req.append(str_InstrumentID);
			url.append(url_head);
			url.append(str_req);
			url.append(url_tail);
		}



		printf ("------日线历史数据更新检测 ------->  %s \n",str_req.c_str());
	//					printf ("url ------->  %s \n",url.c_str());

		std::string response_data;
		HttpBase httpbase;
		int ret =httpbase.get_url_response(url,response_data);
		//			printf ("response_data ------->  %s \n",response_data.c_str());
		if (ret ==0)
		{
			this->ParseKLineData(response_data,str_InstrumentID);
		}

	}

	void MongoHxDayLine::ParseKLineData(std::string response_data,std::string instrumentid)
	{
		std::string response;
		response = response_data.substr(1,response_data.length()-3);
	//	printf ("response ------->  %s \n",response.c_str());

		Json::Reader  reader;
		Json::Value   value;

		if(reader.parse(response,value))
		{
		  if(!value["Data"].isNull())
		  {
	//		  char data[response_data.length()];
	//		  strcpy (data,value["Data"].asString().c_str());
	//		  std::string str_data = value["Data"].asString();

			  double priceweight;
			  int sz = value["Data"].size();
	//		  printf ("Data length----->  %d \n",sz);
			  for (int i = 0; i < sz; ++i)
			  {
				if(i==4)
				{
					priceweight = value["Data"][i].asDouble();
	//				printf ("priceweight -----> %f \n",priceweight);
				}
			  }



			  for (int i = 0; i < sz; ++i)
			  {
	//		          std::cout << "Data " << i + 1 << ": ";
				  int szi = value["Data"][i].size();
	//			  printf ("Data[i] length----->  %d \n",szi);
				  for (int j = 0; j < szi; ++j)
				  {
						  int szj = value["Data"][i][j].size();
						  HxDayLineData hxdaylinedata;
						  strcpy(hxdaylinedata.InstrumentID,instrumentid.c_str());
						  for (int k = 0; k < szj; ++k)
						  {
	//		        			  printf ("parse----->  %d \n",k);
							  if (k==0)
							  {
								  char buf[20];
								  sprintf(buf, "%f", value["Data"][i][j][k].asDouble());
								  std::string time = buf;
	//							  printf ("Time ---->  %s  --> %s\n",buf,time.substr(0,14).c_str());
								  strcpy(hxdaylinedata.Time,time.substr(0,14).c_str());   // 有时日期数据后面会多3个0，此时固定长度比较合适

								  tm tm_;
								  time_t t_;

								  strptime(hxdaylinedata.Time, "%Y%m%d%H%M%S", &tm_); //将字符串转换为tm时间
								  tm_.tm_isdst = -1;
								  t_  = mktime(&tm_); //将tm时间转换为秒时间


								  hxdaylinedata.uTIme =t_;
//								  printf ("Time ---->  %s  --> %s     ---->  long : %ld \n",buf,time.substr(0,14).c_str(),t_);
							  }
							  else if (k==1)
							  {

								  hxdaylinedata.LastClose = value["Data"][i][j][k].asDouble()/priceweight;
	//							  printf ("LastClose ---->  %.2f \n",hxdaylinedata.LastClose);
							  }
							  else if (k==2)
								  {
									  hxdaylinedata.Open = value["Data"][i][j][k].asDouble()/priceweight;
	//								  printf ("Open ---->  %.2f \n",hxdaylinedata.Open);
								  }
							  else if (k==3)
								  {
									  hxdaylinedata.Close = value["Data"][i][j][k].asDouble()/priceweight;
	//								  printf ("Close ---->  %.2f \n",hxdaylinedata.Close);
								  }
							  else if (k==4)
								  {
									  hxdaylinedata.High = value["Data"][i][j][k].asDouble()/priceweight;
	//								  printf ("High ---->  %.2f \n",hxdaylinedata.High);
								  }
							  else if (k==5)
								  {
									  hxdaylinedata.Low = value["Data"][i][j][k].asDouble()/priceweight;
	//								  printf ("Low ---->  %.2f \n",hxdaylinedata.Low);
								  }
							  else if (k==6)
								  {
									  hxdaylinedata.Volume = value["Data"][i][j][k].asDouble();
	//								  printf ("Volume ---->  %.2f \n",hxdaylinedata.Volume);
								  }
							  else if (k==7)
								  {
									  hxdaylinedata.Amount = value["Data"][i][j][k].asDouble();
	//								  printf ("Amount ---->  %.2f \n",hxdaylinedata.Amount);
								  }
						  }
//						  MongoHxDayLine mongohxdayline;
//						  mongohxdayline.
						  this->Check_HxDayLineData(hxdaylinedata);

	//					  printf ("parse----->  %d \n",j);


				  }

			  }

		  }
		  else
		  {
			  printf (" Data is null \n");
		  }
		}

	}

	CycleData MongoHxDayLine::Collect_CycleData(MongoCycle mongocycle,const char *pinstrumentid,const char * date)
		{

			CycleData cycledata;
			strcpy(cycledata.InstrumentID,pinstrumentid);
			strcpy(cycledata.Date,date);

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[pinstrumentid];

			mongocxx::options::find opts;
			bsoncxx::builder::stream::document order_builder;
			order_builder << "uTime" << -1 ;   //
			opts.sort(order_builder.view()).limit(20);

		  tm tm_;
		  time_t t_;

		  strptime(date, "%Y%m%d%H%M%S", &tm_); //将字符串转换为tm时间
		  tm_.tm_isdst = -1;
		  t_  = mktime(&tm_); //将tm时间转换为秒时间



//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);


		  int i=0;
			auto cursor = collection.find(document{} << "uTime" << open_document << "$lt" << t_
				  << close_document << finalize,opts);
			for (auto&& doc : cursor)
			{
//				std::cout << "Collect_CycleData ---->" <<bsoncxx::to_json(doc) << std::endl;

				Json::Reader  reader;
				Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["Time"].isNull())
					  {

						  std::string str_date;
						  str_date= value["Time"].asString();
						  double open = atof(value["Open"].asString().c_str());;
						  double close = atof(value["Close"].asString().c_str());;

						  double today_max;
						  double today_min;

						  if (i==0)
						  {
							  cycledata.High20=open;
							  cycledata.Low20=open;
							  strcpy(cycledata.HighDate,str_date.c_str());
							  strcpy(cycledata.LowDate,str_date.c_str());
							  today_max=open;
							  today_min=open;
						  }


						  if (open >close )
						  {
							  today_max =open;
							  if(today_max > cycledata.High20)
							  {
								  cycledata.High20 = today_max;
								  strcpy(cycledata.HighDate,str_date.c_str());
							  }
						  }
						  else
						  {
							  today_max =close;
							  if(today_max > cycledata.High20)
							  {
								  cycledata.High20 = today_max;
								  strcpy(cycledata.HighDate,str_date.c_str());
							  }
						  }

						  if (open <close )
						  {
							  today_min =open;
							  if(today_min < cycledata.Low20)
							  {
								  cycledata.Low20 = today_min;
								  strcpy(cycledata.LowDate,str_date.c_str());
							  }
						  }
						  else
						  {
							  today_min =close;
							  if(today_min < cycledata.Low20)
							  {
								  cycledata.Low20 = today_min;
								  strcpy(cycledata.LowDate,str_date.c_str());
							  }
						  }


//						  printf("MongoHxDayLine Collect_CycleData ---->%s    high %f low %f...\n",str_date.c_str(),cycledata.High20 ,cycledata.Low20);

					  }
				 }
				  i++;
			// @end: cpp-query-sort

			}

//			MongoCycle mcycle;

			mongocycle.Check_HighLowCount(pinstrumentid,date,cycledata);
//			cycledata.HighCount = mcycle.Check_Highcount(pinstrumentid,date,cycledata.High20);
//			cycledata.LowCount = mcycle.Check_Lowcount(pinstrumentid,date,cycledata.Low20);
//			mongocycle.Check_CycleData(cycledata);
//			mcycle.Insert(cycledata);
//			printf("MongoHxDayLine Collect_CycleData end! ...\n");
			return cycledata;


		}


	AveragePriceData MongoHxDayLine::Collect_AveragePriceData(const char *pinstrumentid,const char *date)
			{

				std:string closetime;
				closetime.append(date);
//				printf("MongoHxDayLine Collect_AveragePriceData  --->   %s \n",closetime.c_str());
				std::string tail ="150000";
				closetime.append(tail);
//				printf("MongoHxDayLine Collect_AveragePriceData  --->   %s \n",closetime.c_str());
				AveragePriceData averagepricedata;
//				strcpy(averagepricedata.InstrumentID,pinstrumentid);
//				strcpy(averagepricedata.Date,date);
				MongoHxMinuteLine mongohxminuteline;
//				averagepricedata.AveragePrice = mongohxminuteline.CollectAveragePrice_CloseTime(pinstrumentid,closetime.c_str());
//				averagepricedata.StartOpenInterest = mongohxminuteline.CollectFirstOpenInterest_CloseTime(pinstrumentid,closetime.c_str());
//				averagepricedata.OpenInterest = mongohxminuteline.CollectLastOpenInterest_CloseTime(pinstrumentid,closetime.c_str());
				averagepricedata = mongohxminuteline.CollectAveragePriceData_CloseTime(pinstrumentid,closetime.c_str());

				MongoAveragePrice mongoaverageprice;
				mongoaverageprice.Check_AveragePriceData(averagepricedata);
				return averagepricedata;
			}
