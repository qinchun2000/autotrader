#include "mongodepthmarket.hpp"


	MongoDepthMarket::MongoDepthMarket()
		{
//			printf("MongoDepthMarket 构造函数 入口 ...\n");
			mongocxx::instance inst{};
					////chenzw:123 -->用户：密码  参考博客用户管理
//			char _mongodb_url[] = "tcp://180.168.146.187:10001";
//
//			char _mongodb_dbname[] ="baseinfo";
//			char _mongodb_collenction[] ="exchangeinfo";


			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"baseinfo");
			strcpy(_mongodb_collenction,"depthmarket");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];



		}

	void MongoDepthMarket::Insert(DepthMarket depthmarket)
		{
//			printf("MongoDepthMarket Insert begin! ...\n");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];


			document doc{};

//			doc << "_id" << 7
			doc<< "InstrumentID" << depthmarket.InstrumentID
			<< "ExchangeID" << depthmarket.ExchangeID
			<< "ProductID" << depthmarket.ProductID
			<< "OpenInterest" << depthmarket.OpenInterest
			<< "Volume" << depthmarket.Volume
			<< "ActionDay" << depthmarket.ActionDay;



//		bsoncxx::document::view view = doc_value.view();

//			printf("MongoExchangeInfo Insert ready! ...  \n");
//			 document doc{};
//			doc << "_id" << 6
//			<< "No" << 2
//			<< "info" << "test";
			collection.insert_one(doc.view());


			printf("MongoDepthMarket Insert finished! ...\n");
		}

	void MongoDepthMarket::DeleteOne(DepthMarket depthmarket)
			{
	//			printf("MongoDepthMarket Delete begin! ...\n");
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				document doc{};
				doc<< "InstrumentID" << depthmarket.InstrumentID
				<< "ExchangeID" << depthmarket.ExchangeID;

				collection.delete_one(doc.view());
				printf("MongoDepthMarket Delete finished! ...\n");
			}

	void MongoDepthMarket::DeleteAll()
				{
		//			printf("MongoDepthMarket Delete begin! ...\n");
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];


					collection.delete_many(document{} << finalize);
					printf("MongoDepthMarket DeleteAll finished! ...\n");
				}
// 如果存在行情数据 则删除后插入，如果不存在 则直接插入
	void MongoDepthMarket::Check_DepthMarket(DepthMarket depthmarket)
			{

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				DateUtil dateutil;
				std::string str_update = dateutil.ConvertCurrentTimetoString();

				document doc{};

				doc<< "InstrumentID" << depthmarket.InstrumentID
							<< "ExchangeID" << depthmarket.ExchangeID
							<< "ProductID" << depthmarket.ProductID
							<< "OpenInterest" << depthmarket.OpenInterest
							<< "Volume" << depthmarket.Volume
							<< "LowerLimitPrice" << depthmarket.LowerLimitPrice
							<< "UpperLimitPrice" << depthmarket.UpperLimitPrice
							<< "TradingDay" << depthmarket.TradingDay
							<< "Turnover" << depthmarket.Turnover
							<< "UpdateTime" << depthmarket.UpdateTime
							<< "LastPrice" << depthmarket.LastPrice
							<< "SettlementPrice" << depthmarket.SettlementPrice
							<< "PreSettlementPrice" << depthmarket.PreSettlementPrice
							<< "ActionDay" << depthmarket.ActionDay
							<< "LocalUpdateTime" << str_update.c_str();


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << depthmarket.InstrumentID<< finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");
				document filter_del{};

				filter_del<< "InstrumentID" << depthmarket.InstrumentID
							<< finalize;
				collection.delete_many(filter_del.view());
				printf("-----> 数据存在，删除后--->插入新数据！\n");
				collection.insert_one(doc.view());
				}
				else
				{
					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoDepthMarket Check_DepthMarket end! ...\n");
			}

	DepthMarket MongoDepthMarket::Find_LastUpdateTime()
			{

				DepthMarket depthmarket;
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				mongocxx::options::find opts;
				bsoncxx::builder::stream::document order_builder;
				order_builder << "LocalUpdateTime" << -1 ;
				opts.sort(order_builder.view()).limit(1);

				auto cursor = collection.find(document{} << finalize, opts);
				for (auto&& doc : cursor)
				{
//					std::cout << "Find_LastUpdateTime ---->" <<bsoncxx::to_json(doc) << std::endl;

					Json::Reader  reader;
					Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {
								strcpy(depthmarket.InstrumentID,value["InstrumentID"].asString().c_str());
								strcpy(depthmarket.ProductID,value["ProductID"].asString().c_str());


								strcpy(depthmarket.TradingDay,value["TradingDay"].asString().c_str());
								strcpy(depthmarket.ActionDay,value["ActionDay"].asString().c_str());
								strcpy(depthmarket.UpdateTime,value["UpdateTime"].asString().c_str());
								strcpy(depthmarket.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());

								depthmarket.OpenInterest = value["OpenInterest"].asDouble();
								depthmarket.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
								depthmarket.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
								depthmarket.Volume = value["Volume"].asDouble();
								depthmarket.LastPrice = value["LastPrice"].asDouble();

//							  printf("MongoInstrument Find_LastUpdateTime ---->%s ...\n",depthmarket.LocalUpdateTime);

						  }
					 }
				// @end: cpp-query-sort

				}

//						printf("MongoDepthMarket Find_LastUpdateTime end! ...\n");
				return depthmarket;


			}

	int MongoDepthMarket::Count()
			{


				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				int i=0;
				auto cursor = collection.find(document{} << finalize);
				for (auto&& doc : cursor)
				{

					i++;
				}

						printf("MongoDepthMarket Find_LastUpdateTime end! ...  count %d\n",i);
				return i;


			}

	DepthMarket MongoDepthMarket::Findby_Instrumentid(const char *pinstrumentid)
			{
				DepthMarket  info;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << pinstrumentid << finalize);

				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoHxDayLine check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//					  printf("-----> 历史周期价差表 找到！\n");
				  Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {

						  strcpy(info.InstrumentID,value["InstrumentID"].asString().c_str());

						  info.SettlementPrice =value["SettlementPrice"].asDouble();
						  info.PreSettlementPrice = value["PreSettlementPrice"].asDouble();
						  info.LastPrice = value["LastPrice"].asDouble();



						  }
					 }

				}
				else
				{
//						printf("-----> 数据不存在！\n");

				}


//					printf("Findby_Instrumentid check end! ...\n");
				return info;
			}


	MainContractInfo MongoDepthMarket::Creat_Maincontract(const char *productid )
			{

				MainContractInfo maincontractinfo;
				int arraylength =5;
				int datetime;
				int main_yearmonth[arraylength];
				char instrumentid[10];

				memset(instrumentid,0,sizeof(instrumentid));

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
				order_builder << "OpenInterest" << -1 ;   //降序进行数据的排列
				opts.sort(order_builder.view()).limit(arraylength);

				int i=0;
				auto cursor = collection.find(filter_builder.view(), opts);
				for (auto&& doc : cursor)
				{
//					std::cout << bsoncxx::to_json(doc) << std::endl;
					DepthMarket depthmarket;

					Json::Reader  reader;
					Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {

							strcpy(instrumentid,value["InstrumentID"].asString().c_str());
//							printf("Find_Maincontract ---[%d]--->%s \n", i,instrumentid);

							char yearmonth[10];
							string pattern("([0-9]+)"); // ([a-z]+):([0-9]+) also works here

							boost::regex reg(pattern);    //查找字符串里的数字
							boost::cmatch mat;
							if(boost::regex_search(instrumentid, mat, reg))
							{
//										        cout << "searched:" << mat[0] << endl;
//								printf("匹配主力合约的年份和月份--mat-[%d]--->%s \n", i,mat[0].str().c_str());
							}
							strcpy(yearmonth,mat[0].str().c_str());
							main_yearmonth[i]=atoi(yearmonth);

							if (i==0)
							{
//							printf("----------->找到主力合约，准备给主力合约赋值 --- \n");
							strcpy(maincontractinfo.InstrumentID,instrumentid);

							datetime = main_yearmonth[i];
							strcpy(maincontractinfo.ProductID,value["ProductID"].asString().c_str());

							MongoProductClass mongoproductclass;
							ProductClassInfo productclassinfo;
							productclassinfo=mongoproductclass.Findby_ProductID(maincontractinfo.ProductID);
							strcpy(maincontractinfo.Section,productclassinfo.Section);
							strcpy(maincontractinfo.DayNight,productclassinfo.DayNight);

							strcpy(maincontractinfo.TradingDay,value["TradingDay"].asString().c_str());
							strcpy(maincontractinfo.ActionDay,value["ActionDay"].asString().c_str());
							strcpy(maincontractinfo.UpdateTime,value["UpdateTime"].asString().c_str());

							maincontractinfo.OpenInterest = value["OpenInterest"].asDouble();
							maincontractinfo.UpperLimitPrice = value["UpperLimitPrice"].asDouble();
							maincontractinfo.LowerLimitPrice = value["LowerLimitPrice"].asDouble();
							maincontractinfo.Volume = value["Volume"].asDouble();
							maincontractinfo.LastPrice = value["LastPrice"].asDouble();

							InstrumentInfo instrumentinfo;
							MongoInstrument mongoInstrument;
							instrumentinfo = mongoInstrument.Findby_InstrumentID(maincontractinfo.InstrumentID);

							strcpy(maincontractinfo.ExchangeID,instrumentinfo.ExchangeID);
							strcpy(maincontractinfo.InstrumentName,instrumentinfo.InstrumentName);

							maincontractinfo.VolumeMultiple = instrumentinfo.VolumeMultiple;
							maincontractinfo.PriceTick = instrumentinfo.PriceTick;
							maincontractinfo.LongMarginRatio = instrumentinfo.LongMarginRatio;
							maincontractinfo.ShortMarginRatio = instrumentinfo.ShortMarginRatio;


							}
							else
							{
//								printf ("匹配次月合约中-------> main_yearmonth ---->%d  %d\n",main_yearmonth[i],datetime);
								if(main_yearmonth[i]>datetime &&  main_yearmonth[i]-datetime <100 )
								{

//												printf ("-------> main_yearmonth ---->%d  %d \n",main_yearmonth[i],datetime);
									strcpy(maincontractinfo.InstrumentID_next,instrumentid);
//												printf("Find_Maincontract ---[%d]-InstrumentID_next-->%s \n", i,instrumentid);
									maincontractinfo.OpenInterest_next = value["OpenInterest"].asDouble();
									maincontractinfo.LastPrice_next = value["LastPrice"].asDouble();
									maincontractinfo.UpperLimitPrice_next = value["UpperLimitPrice"].asDouble();
									maincontractinfo.LowerLimitPrice_next = value["LowerLimitPrice"].asDouble();
									maincontractinfo.Volume_next = value["Volume"].asDouble();
//									printf("break----------->找到符合的次月合约 ---[%d]-InstrumentID_next-->%s \n", i,maincontractinfo.InstrumentID);

									break;
								}
								else
								{
									strcpy(maincontractinfo.InstrumentID_next,maincontractinfo.InstrumentID);
//									printf("----------->没找到符合的次月合约，用主力代替 ---[%d]-InstrumentID_next-->%s \n", i,maincontractinfo.InstrumentID_next);
									maincontractinfo.OpenInterest_next = maincontractinfo.OpenInterest;
									maincontractinfo.LastPrice_next = maincontractinfo.LastPrice;
									maincontractinfo.UpperLimitPrice_next = maincontractinfo.UpperLimitPrice;
									maincontractinfo.LowerLimitPrice_next = maincontractinfo.LowerLimitPrice;
									maincontractinfo.Volume_next = maincontractinfo.Volume;
//									printf("----------->没找到符合的次月合约，------End！！！ --- \n");
								}
							}



						  }
					 }
					  else
					  {
						  printf("MongoDepthMarket::Creat_Maincontract----------->json数据为空，请查找 --- \n");

					  }
				// @end: cpp-query-sort
					  i++;
				} // for 循环


//				printf("MongoDepthMarket Find_Maincontract end! ...\n");
				return maincontractinfo;
			}
