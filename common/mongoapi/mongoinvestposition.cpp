#include "mongoinvestposition.hpp"


	MongoInvestPosition::MongoInvestPosition()
		{
//			printf("MongoExchangeInfo 构造函数 入口 ...\n");
			mongocxx::instance inst{};
			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"account");
			strcpy(_mongodb_collenction,"investposition");

//			printf("_mongodb_url  %s\n",_mongodb_url);

			mongocxx::uri _uri(_mongodb_url);

			mongocxx::client _client(_uri);
			//打开"baseinfo"数据库
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];



		}

	void MongoInvestPosition::Insert(InvestPositionInfo investpositioninfo)
		{
//			printf("MongoExchangeInfo Insert begin! ...\n");

			printf("_mongodb_url  %s\n",_mongodb_url);
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

			document doc{};
			doc<< "InstrumentID" << investpositioninfo.InstrumentID
				<< "OpenPriceAverage" << investpositioninfo.OpenPriceAverage
				<< "YdPosition" << investpositioninfo.YdPosition
				<< "Position" << investpositioninfo.Position
				<< "TodayPosition" << investpositioninfo.TodayPosition
				<< "HedgeFlag" << investpositioninfo.HedgeFlag
				<< "TradingDay" << investpositioninfo.TradingDay
				<< "PositionProfit" << investpositioninfo.PositionProfit
				<< "CloseProfit" << investpositioninfo.CloseProfit
				<< "CloseVolume" << investpositioninfo.CloseVolume
				<< "Commission" << investpositioninfo.Commission
				<< "UseMargin" << investpositioninfo.UseMargin
				<< "PreSettlementPrice" << investpositioninfo.PreSettlementPrice
				<< "SettlementPrice" << investpositioninfo.SettlementPrice
				<< "PosiDirection" << investpositioninfo.PosiDirection
				<< "LocalUpdateTime" << str_localupdate;

            collection.insert_one(doc.view());

//			_collection.insert_one(view);
//			printf("MongoExchangeInfo Insert finished! ...\n");
		}

	void MongoInvestPosition::Check_InvestPosition(InvestPositionInfo investpositioninfo)
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

	//			doc << "_id" << 7
				doc<< "InstrumentID" << investpositioninfo.InstrumentID
					<< "OpenPriceAverage" << investpositioninfo.OpenPriceAverage
					<< "YdPosition" << investpositioninfo.YdPosition
					<< "Position" << investpositioninfo.Position
					<< "PosiDirection" << investpositioninfo.PosiDirection
					<< "TodayPosition" << investpositioninfo.TodayPosition
					<< "HedgeFlag" << investpositioninfo.HedgeFlag
					<< "TradingDay" << investpositioninfo.TradingDay
					<< "PositionDate" << investpositioninfo.PositionDate
					<< "PositionProfit" << investpositioninfo.PositionProfit
					<< "CloseProfit" << investpositioninfo.CloseProfit
					<< "OpenAmount" << investpositioninfo.OpenAmount
					<< "OpenVolume" << investpositioninfo.OpenVolume
					<< "CloseAmount" << investpositioninfo.CloseAmount
					<< "CloseVolume" << investpositioninfo.CloseVolume
					<< "Commission" << investpositioninfo.Commission
					<< "UseMargin" << investpositioninfo.UseMargin
					<< "PreSettlementPrice" << investpositioninfo.PreSettlementPrice
					<< "SettlementPrice" << investpositioninfo.SettlementPrice
					<< "LocalUpdateTime" << str_localupdate;


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << investpositioninfo.InstrumentID<< finalize);
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





	void MongoInvestPosition::Find_TradeInstrumentIDList(vector<InvestPositionInfo> &temp)
			{

				InvestPositionInfo investpositioninfo;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				mongocxx::cursor cursor = collection.find(document{} << finalize);



				for(auto doc : cursor)
				{
//								  std::cout << bsoncxx::to_json(doc) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {

							  strcpy(investpositioninfo.InstrumentID,value["InstrumentID"].asString().c_str());
							  strcpy(investpositioninfo.TradingDay,value["TradingDay"].asString().c_str());

							  strcpy(investpositioninfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
							  investpositioninfo.PositionDate = value["PositionDate"].asInt();
							  investpositioninfo.Position = value["Position"].asInt();
							  investpositioninfo.YdPosition = value["YdPosition"].asInt();
							  investpositioninfo.TodayPosition = value["TodayPosition"].asInt();
							  investpositioninfo.PosiDirection = value["PosiDirection"].asInt();
							  investpositioninfo.UseMargin = value["UseMargin"].asDouble();
							  investpositioninfo.OpenPriceAverage = value["OpenPriceAverage"].asDouble();
							  investpositioninfo.PreSettlementPrice = value["PreSettlementPrice"].asDouble();
							  investpositioninfo.SettlementPrice = value["SettlementPrice"].asDouble();
							  investpositioninfo.HedgeFlag = value["HedgeFlag"].asInt();
							  investpositioninfo.PositionProfit = value["PositionProfit"].asDouble();
							  investpositioninfo.CloseProfit = value["CloseProfit"].asDouble();
							  investpositioninfo.OpenVolume = value["OpenVolume"].asInt();
							  investpositioninfo.CloseVolume = value["CloseVolume"].asInt();
							  investpositioninfo.OpenAmount = value["OpenAmount"].asDouble();
							  investpositioninfo.CloseAmount = value["CloseAmount"].asDouble();
							  investpositioninfo.Commission = value["Commission"].asDouble();
							  temp.push_back(investpositioninfo);



//							  printf("MongoInvestPosition Find_TradeInstrumentIDList[%d]! ...\n",tradeinsidlist.length);
						  }
					 }

				}

	//						printf("MongoInvestPosition Find_TradeList  end! ...\n");




			}
	int  MongoInvestPosition::Find_PositionbyInstrumentid(const char *pinstrumentid)
			{
				int position;
				position=0;

	//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
						  collection.find_one(document{} << "InstrumentID" << pinstrumentid << finalize);
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

							  position = value["Position"].asInt();

							  }
						 }
				}
				else
				{
//					printf("-----> 合约无持仓！\n");

				}
//					printf("MongoInvestPosition Find_PositionbyInstrumentid  end! ...\n");

				return position;
			}

	InvestPositionInfo  MongoInvestPosition::Find_byInstrumentid(const char *pinstrumentid)
				{

					InvestPositionInfo investpositioninfo;
		//				printf("_mongodb_url  %s\n",_mongodb_url);
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
							  collection.find_one(document{} << "InstrumentID" << pinstrumentid << finalize);
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

									  strcpy(investpositioninfo.InstrumentID,value["InstrumentID"].asString().c_str());
									  strcpy(investpositioninfo.TradingDay,value["TradingDay"].asString().c_str());

									  strcpy(investpositioninfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
									  investpositioninfo.PositionDate = value["PositionDate"].asInt();
									  investpositioninfo.Position = value["Position"].asInt();
									  investpositioninfo.YdPosition = value["YdPosition"].asInt();
									  investpositioninfo.TodayPosition = value["TodayPosition"].asInt();
									  investpositioninfo.PosiDirection = value["PosiDirection"].asInt();
									  investpositioninfo.UseMargin = value["UseMargin"].asDouble();
									  investpositioninfo.OpenPriceAverage = value["OpenPriceAverage"].asDouble();
									  investpositioninfo.PreSettlementPrice = value["PreSettlementPrice"].asDouble();
									  investpositioninfo.SettlementPrice = value["SettlementPrice"].asDouble();
									  investpositioninfo.HedgeFlag = value["HedgeFlag"].asInt();
									  investpositioninfo.PositionProfit = value["PositionProfit"].asDouble();
									  investpositioninfo.CloseProfit = value["CloseProfit"].asDouble();
									  investpositioninfo.OpenVolume = value["OpenVolume"].asInt();
									  investpositioninfo.CloseVolume = value["CloseVolume"].asInt();
									  investpositioninfo.OpenAmount = value["OpenAmount"].asDouble();
									  investpositioninfo.CloseAmount = value["CloseAmount"].asDouble();
									  investpositioninfo.Commission = value["Commission"].asDouble();

								  }
							 }
					}
					else
					{
	//					printf("-----> 合约无持仓！\n");

					}
	//					printf("MongoInvestPosition Find_PositionbyInstrumentid  end! ...\n");

					return investpositioninfo;
				}
	int MongoInvestPosition::Count()
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


	void MongoInvestPosition::DeleteAll()
					{
			//			printf("MongoInvestPosition Delete begin! ...\n");
						mongocxx::uri uri(_mongodb_url);
						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];


						collection.delete_many(document{} << finalize);
						printf("MongoInvestPosition DeleteAll finished! ...\n");
					}
