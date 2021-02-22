#include "mongoinvestpositiondetail.hpp"


	MongoInvestPositionDetail::MongoInvestPositionDetail()
		{
//			printf("MongoExchangeInfo 构造函数 入口 ...\n");
			mongocxx::instance inst{};
			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"account");
			strcpy(_mongodb_collenction,"investpositiondetail");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];



		}

	void MongoInvestPositionDetail::Insert(InvestPositionDetailInfo investposiitiondetailinfo)
		{
//			printf("MongoExchangeInfo Insert begin! ...\n");

			printf("_mongodb_url  %s\n",_mongodb_url);
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			document doc{};
			doc<< "InstrumentID" << investposiitiondetailinfo.InstrumentID
				<< "OpenPrice" << investposiitiondetailinfo.OpenPrice
				<< "OpenDate" << investposiitiondetailinfo.OpenDate
				<< "Volume" << investposiitiondetailinfo.Volume
				<< "MarginRateByMoney" << investposiitiondetailinfo.MarginRateByMoney
				<< "MarginRateByVolume" << investposiitiondetailinfo.MarginRateByVolume
				<< "Margin" << investposiitiondetailinfo.Margin
				<< "CloseProfitByTrade" << investposiitiondetailinfo.CloseProfitByTrade
				<< "Direction" << investposiitiondetailinfo.Direction;

            collection.insert_one(doc.view());

//			_collection.insert_one(view);
//			printf("MongoExchangeInfo Insert finished! ...\n");
		}

	void MongoInvestPositionDetail::Check_InvestPositionDetail(InvestPositionDetailInfo investposiitiondetailinfo)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				document doc{};


				doc<< "InstrumentID" << investposiitiondetailinfo.InstrumentID
								<< "OpenPrice" << investposiitiondetailinfo.OpenPrice
								<< "OpenDate" << investposiitiondetailinfo.OpenDate
								<< "TradingDay" << investposiitiondetailinfo.TradingDay
								<< "TradeID" << investposiitiondetailinfo.TradeID
								<< "Volume" << investposiitiondetailinfo.Volume
								<< "MarginRateByMoney" << investposiitiondetailinfo.MarginRateByMoney
								<< "MarginRateByVolume" << investposiitiondetailinfo.MarginRateByVolume
								<< "Margin" << investposiitiondetailinfo.Margin
								<< "CloseAmount" << investposiitiondetailinfo.CloseAmount
								<< "CloseProfitByTrade" << investposiitiondetailinfo.CloseProfitByTrade
								<< "CloseProfitByDate" << investposiitiondetailinfo.CloseProfitByDate
								<< "PositionProfitByTrade" << investposiitiondetailinfo.PositionProfitByTrade
								<< "PositionProfitByDate" << investposiitiondetailinfo.PositionProfitByDate
								<< "Direction" << investposiitiondetailinfo.Direction;


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << investposiitiondetailinfo.InstrumentID
						  << "TradeID" << investposiitiondetailinfo.TradeID<< finalize);
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

	 void MongoInvestPositionDetail::Find_TradeDetailList(vector<TradeDetailInstrumentInfo> &temp)
			{
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				mongocxx::cursor cursor = collection.find(document{} << finalize);
				TradeDetailInstrumentInfo info;

				for(auto doc : cursor)
				{
	//						  std::cout << bsoncxx::to_json(doc) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {


	//								  std::cout<<value["InstrumentID"].asString()<<std::endl;
//							  char *char_instrumentid= new char[10];

							  info.InstrumentID=value["InstrumentID"].asString().c_str();
							  info.TradeID=value["TradeID"].asString().c_str();

							  temp.push_back(info);



						  }
					 }

				}



	//						printf("MongoInvestPosition Find_TradeList  end! ...\n");




			}

	InvestPositionDetailInfo  MongoInvestPositionDetail::Find_byInstrumentIDTradeID(const char * pinstrumentid,const char *tradeid)
				{

					InvestPositionDetailInfo info;

					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
									  collection.find_one(document{} << "InstrumentID" << pinstrumentid
											  << "TradeID" << tradeid<< finalize);
					if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 数据存在，-----无需插入数据！\n");

						  Json::Reader  reader;
						  Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
							  {
								  if(!value["InstrumentID"].isNull())
								  {
									  strcpy(info.InstrumentID,value["InstrumentID"].asString().c_str());
									  strcpy(info.OpenDate,value["OpenDate"].asString().c_str());
									  info.OpenPrice = value["OpenPrice"].asDouble();
									  info.Direction = value["Direction"].asInt();
									  info.Volume = value["Volume"].asInt();
									  info.Margin = value["Margin"].asDouble();
									  info.CloseAmount = value["CloseAmount"].asDouble();
									  info.CloseProfitByTrade = value["CloseProfitByTrade"].asDouble();
									  info.CloseProfitByDate = value["CloseProfitByDate"].asDouble();
									  info.PositionProfitByTrade = value["PositionProfitByTrade"].asDouble();
									  info.PositionProfitByDate = value["PositionProfitByDate"].asDouble();

								  }

							  }

					}
					else
					{

					}




		//						printf("MongoInvestPosition Find_TradeList  end! ...\n");



					return info;
				}

	bool  MongoInvestPositionDetail::Exist_byInstrumentIDTradeID(const char * pinstrumentid,const char *tradeid)
			{

				bool ret;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
								  collection.find_one(document{} << "InstrumentID" << pinstrumentid
										  << "TradeID" << tradeid<< finalize);
				if(maybe_result)
				{

					ret=true;
				}
				else
				{
					ret=false;
				}


				return ret;
			}
	void MongoInvestPositionDetail::Check_LocalOrder()
		{
			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			MongoOrder mongoorder;
			mongocxx::cursor cursor = collection.find(document{} << finalize);

			for(auto doc : cursor)
			{
//						  std::cout << bsoncxx::to_json(doc) << "\n";

			  Json::Reader  reader;
			  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["InstrumentID"].isNull())
					  {

						  OrderInfo orderinfo;

						  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
						  strcpy(orderinfo.TraderID,value["TradeID"].asString().c_str());
						  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
						  orderinfo.Volume = value["Volume"].asDouble();
						  orderinfo.Direction = value["Direction"].asDouble();



					  }
				 }

			}



//						printf("MongoInvestPosition Find_TradeList  end! ...\n");




		}

	double MongoInvestPositionDetail::CalOpenAveragePrice_byInstrumentID(char * pinstrumentid)
				{
					int totalvolume;
					double totalprice,averageprice;

					totalvolume =0;
					totalprice=0;
					averageprice=0;

		//				printf("_mongodb_url  %s\n",_mongodb_url);
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];
					mongocxx::cursor cursor = collection.find(document{} << "InstrumentID" << pinstrumentid << finalize);


					for(auto doc : cursor)
					{
		//						  std::cout << bsoncxx::to_json(doc) << "\n";

					  Json::Reader  reader;
					  Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(doc),value))
						  {
							  if(!value["InstrumentID"].isNull())
							  {
								  int volume = value["Volume"].asInt();
								  totalvolume = totalvolume+ volume;

								  double price = value["OpenPrice"].asDouble();
								  totalprice =totalprice+ price *volume;





							  }
						 }

					}



		//						printf("MongoInvestPosition Find_TradeList  end! ...\n");
					if (totalvolume !=0)
					{
						averageprice = totalprice/(double)totalvolume;
					}


					return averageprice;
				}
	int MongoInvestPositionDetail::Count()
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

	void MongoInvestPositionDetail::DeleteAll()
					{
			//			printf("MongoInvestPositionDetail Delete begin! ...\n");
						mongocxx::uri uri(_mongodb_url);
						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];


						collection.delete_many(document{} << finalize);
						printf("MongoInvestPositionDetail DeleteAll finished! ...\n");
					}
