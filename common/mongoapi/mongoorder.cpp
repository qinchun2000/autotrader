#include "mongoorder.hpp"


	MongoOrder::MongoOrder()
		{
			mongocxx::instance inst{};
			strcpy(_mongodb_url,"mongodb://127.0.0.1:27017");
			strcpy(_mongodb_dbname,"account");
			strcpy(_mongodb_collenction,"localorder");


			mongocxx::uri _uri(_mongodb_url);
			mongocxx::client _client(_uri);
			_dbname = _client[_mongodb_dbname];
			_collection = _dbname[_mongodb_collenction];

		}

	void MongoOrder::Insert(OrderInfo orderinfo)
		{
//			printf("MongoExchangeInfo Insert begin! ...\n");

			printf("_mongodb_url  %s\n",_mongodb_url);
			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);
			//打开"baseinfo"数据库
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			document doc{};
			doc<< "InstrumentID" << orderinfo.InstrumentID
				<< "LimitPrice" << orderinfo.LimitPrice
				<< "StopPrice" << orderinfo.StopPrice
				<< "TradingDay" << orderinfo.TradingDay
				<< "TradeDate" << orderinfo.TradeDate
				<< "TradeTime" << orderinfo.TradeTime
				<< "Direction" << orderinfo.Direction
				<< "Volume" << orderinfo.Volume
				<< "TraderID" << orderinfo.TraderID
				<< "OrderRef" << orderinfo.OrderRef
				<< "SequenceNo" << orderinfo.SequenceNo
				<< "LocalReqOrderTime" << orderinfo.LocalReqOrderTime;

            collection.insert_one(doc.view());

//			_collection.insert_one(view);
//			printf("MongoOrder Insert finished! ...\n");
		}

	void MongoOrder::Check_Order(OrderInfo orderinfo)
			{
//				printf("_mongodb_url  %s\n",_mongodb_url);
				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);
				//打开"baseinfo"数据库
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];
				DateUtil dateutil;
				std::string str_localupdate = dateutil.GetTodayString();

				document doc{};
				doc<< "InstrumentID" << orderinfo.InstrumentID
						<< "ExchangeID" << orderinfo.ExchangeID
						<< "OpenSignalName" << orderinfo.OpenSignalName
					<< "LimitPrice" << orderinfo.LimitPrice
					<< "Status" << orderinfo.Status
					<< "StopPrice" << orderinfo.StopPrice
					<< "TradingDay" << orderinfo.TradingDay
					<< "Direction" << orderinfo.Direction
					<< "Volume" << orderinfo.Volume
					<< "CombOffsetFlag" << orderinfo.CombOffsetFlag
					<< "CombHedgeFlag" << orderinfo.CombHedgeFlag
					<< "OrderRef" << orderinfo.OrderRef
					<< "SequenceNo" << orderinfo.SequenceNo
					<< "CancelFlag" << orderinfo.CancelFlag
					<< "CancelCount" << orderinfo.CancelCount
					<< "LocalReqOrderTime" << orderinfo.LocalReqOrderTime
					<< "LocalDate" << str_localupdate;

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						  << "OrderRef" << orderinfo.OrderRef
						  << "TradingDay" << orderinfo.TradingDay<< finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				  printf("MongoOrder 未成交委托单  数据存在 ... %s\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");
				}
				else
				{
					printf("MongoOrder 未成交委托单-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoOrder check end! ...\n");
			}

	void MongoOrder::Check_RspOrder(OrderInfo orderinfo)
				{
	//				printf("_mongodb_url  %s\n",_mongodb_url);
					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);
					//打开"baseinfo"数据库
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];
					DateUtil dateutil;
					std::string str_localupdate = dateutil.GetTodayString();

					document doc{};
					doc<< "InstrumentID" << orderinfo.InstrumentID
						<< "LimitPrice" << orderinfo.LimitPrice
						<< "Status" << orderinfo.Status
						<< "StopPrice" << orderinfo.StopPrice
						<< "TradingDay" << orderinfo.TradingDay
						<< "Direction" << orderinfo.Direction
						<< "Volume" << orderinfo.Volume
						<< "CombOffsetFlag" << orderinfo.CombOffsetFlag
						<< "CombHedgeFlag" << orderinfo.CombHedgeFlag
						<< "OrderRef" << orderinfo.OrderRef
						<< "LocalDate" << str_localupdate;

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							  << "OrderRef" << orderinfo.OrderRef
							  << "TradingDay" << orderinfo.TradingDay<< finalize);
					if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					  printf("MongoOrder 未成交委托单  数据存在 ... %s\n",bsoncxx::to_json(*maybe_result).c_str() );
	//				  printf("-----> 数据存在，-----无需插入数据！\n");
					}
					else
					{
						printf("MongoOrder 未成交委托单-----> 数据不存在，插入新数据！\n");
						 collection.insert_one(doc.view());
					}



	//				printf("MongoOrder check end! ...\n");
				}
	void MongoOrder::Check_InvestPositionDetail(OrderInfo orderinfo)
			{

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];
				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

				document doc{};


				doc<< "InstrumentID" << orderinfo.InstrumentID
					<< "TradeID" << orderinfo.TradeID
					<< "Status" << orderinfo.Status
					<< "TradingDay" << orderinfo.TradingDay
					<< "TradeDate" << orderinfo.TradeDate
					<< "Direction" << orderinfo.Direction
					<< "Volume" << orderinfo.Volume
					<< "CombOffsetFlag" << orderinfo.CombOffsetFlag
					<< "CombHedgeFlag" << orderinfo.CombHedgeFlag
					<< "OpenPrice" << orderinfo.OpenPrice
					<< "StopPrice" << orderinfo.StopPrice
					<< "LocalUpdateTime" << str_localupdate;

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						  << "TradeID" << orderinfo.TradeID	  << finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoOrder check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");
				}
				else
				{
//					printf("-----> 数据不存在，插入新数据！\n");
					 collection.insert_one(doc.view());
				}



//				printf("MongoOrder Check_InvestPositionDetail end! ...\n");
			}

	void MongoOrder::Update_Order(OrderInfo orderinfo)
			{
			char logbuf[100];
			LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);

				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


				document doc{};
				doc<< "InstrumentID" << orderinfo.InstrumentID
						<< "StopPrice" << orderinfo.StopPrice
						<< "TradingDay" << orderinfo.TradingDay
						<< "TradeDate" << orderinfo.TradeDate
						<< "TradeTime" << orderinfo.TradeTime
						<< "Direction" << orderinfo.Direction
						<< "Volume" << orderinfo.Volume
						<< "TraderID" << orderinfo.TraderID
						<< "OrderRef" << orderinfo.OrderRef
						<< "SequenceNo" << orderinfo.SequenceNo;



				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						  << "OrderRef" << orderinfo.OrderRef
						  << "CombOffsetFlag" << orderinfo.CombOffsetFlag
						  	  << "TradingDay" << orderinfo.TradingDay  << finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";

					 Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["InstrumentID"].isNull()&&!value["OrderRef"].isNull())
						  {
							  OrderInfo info;

							  info.OrderSubmitStatus = value["OrderSubmitStatus"].asInt();
							  info.OrderStatus = value["OrderStatus"].asInt();

							  if(info.OrderSubmitStatus != orderinfo.OrderSubmitStatus || info.OrderStatus !=orderinfo.OrderStatus)

							  {
								  collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
										  << "OrderRef" << orderinfo.OrderRef
										  << "CombOffsetFlag" << orderinfo.CombOffsetFlag
										<< "TradingDay" << orderinfo.TradingDay << finalize,
										  document{} << "$set" << open_document <<
										   "Status" << orderinfo.Status <<
										  "OrderSubmitStatus" << orderinfo.OrderSubmitStatus <<
										  "OrderStatus" << orderinfo.OrderStatus <<
										  "OrderSysID" << orderinfo.OrderSysID<<
										  "FrontID" << orderinfo.FrontID<<
										  "SessionID" << orderinfo.SessionID<<
										  "ExchangeID" << orderinfo.ExchangeID<<
										  "InsertDate" << orderinfo.InsertDate<<
										  "InsertTime" << orderinfo.InsertTime<<
										  "LocalInsertTime" << str_localupdate<<
										  "LocalUpdateTime" << str_localupdate << close_document << finalize);

								  sprintf(logbuf,"MongoOrder Update_Order  ...%s TradingDay  %s ",
										  orderinfo.InstrumentID,orderinfo.TradingDay);
								  logutil.PrintLog(logbuf);
							  }
						  }
					 }



				}
				else
				{

					sprintf(logbuf,"MongoOrder Update_Order  ...%s TradingDay 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
					logutil.PrintLog(logbuf);
//					printf("-----> 数据不存在，插入新数据！\n");
//					 collection.insert_one(doc.view());
				}

	//				printf("MongoOrder Update_Order end! ...\n");
			}

	void MongoOrder::Update_CloseOrder(OrderInfo orderinfo)
				{
				char logbuf[100];
				LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);

					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					DateUtil dateutil;
					std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							  << "CloseOrderRef" << orderinfo.CloseOrderRef
							  << "CloseTradingDay" <<  orderinfo.CloseTradingDay << finalize);
					if(maybe_result) {
		//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";


						collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
								 << "CloseOrderRef" << orderinfo.CloseOrderRef
									  << "CloseTradingDay" <<  orderinfo.CloseTradingDay << finalize,
											  document{} << "$set" << open_document <<
											   "Status" <<  orderinfo.Status <<
												  "CloseOrderSubmitStatus" << orderinfo.OrderSubmitStatus <<
												  "CloseOrderStatus" << orderinfo.OrderStatus <<
											  "CombOffsetFlag" << orderinfo.CombOffsetFlag <<
											  "CloseOrderSysID" << orderinfo.CloseOrderSysID<<
											  "CloseFrontID" << orderinfo.CloseFrontID<<
											  "CloseSessionID" << orderinfo.CloseSessionID<<
											  "LocalUpdateTime" << str_localupdate << close_document << finalize);
					}
					else
					{

						sprintf(logbuf,"MongoOrder Update_CloseOrder  ...%s TradeID 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
						logutil.PrintLog(logbuf);
	//					printf("-----> 数据不存在，插入新数据！\n");
	//					 collection.insert_one(doc.view());
					}

		//				printf("MongoOrder Update_Order end! ...\n");
				}

	void MongoOrder::Check_CloseOrder(OrderInfo orderinfo,const char *tradeid)
				{
				char logbuf[100];
				LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);

					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					DateUtil dateutil;
					std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							  << "TradeID" << tradeid << finalize);
					if(maybe_result) {
		//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";


						collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
											<< "TradeID" << tradeid << finalize,
											  document{} << "$set" << open_document <<
											   "Status" <<  orderinfo.Status <<
											   "CombOffsetFlag" << orderinfo.CombOffsetFlag<<
											   "CloseSignalName" <<  orderinfo.CloseSignalName <<
											  "CloseOrderRef" << orderinfo.CloseOrderRef<<
											  "CloseTradingDay" << orderinfo.CloseTradingDay<<
											  "LocalUpdateTime" << str_localupdate << close_document << finalize);
					}
					else
					{

						sprintf(logbuf,"MongoOrder Check_CloseOrder  ...%s TradeID 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
						logutil.PrintLog(logbuf);
	//					printf("-----> 数据不存在，插入新数据！\n");
	//					 collection.insert_one(doc.view());
					}

		//				printf("MongoOrder Update_Order end! ...\n");
				}


	void MongoOrder::Update_ActionOrder(OrderInfo orderinfo)
		{
		char logbuf[100];
		LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);

			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
					  << "OrderRef" << orderinfo.OrderRef
					  << "TradingDay" <<  orderinfo.TradingDay << finalize);
			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";

				sprintf(logbuf,"MongoOrder Update_ActionOrder  ...%s Price: %.2f ",orderinfo.InstrumentID,orderinfo.LimitPrice);
				logutil.PrintLog(logbuf);

				 Json::Reader  reader;
				  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
				  {
					  if(!value["InstrumentID"].isNull())
					  {

							  collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
								 << "OrderRef" << orderinfo.OrderRef
										  << "TradingDay" <<  orderinfo.TradingDay << finalize,
											  document{} << "$set" << open_document <<
											   "Status" <<  orderinfo.Status <<
												"LocalUpdateTime" << str_localupdate << close_document << finalize);

					  }
				 }



			}
			else
			{

				sprintf(logbuf,"MongoOrder Update_ActionOrder  ...%s TradeID 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
				logutil.PrintLog(logbuf);
//					printf("-----> 数据不存在，插入新数据！\n");
//					 collection.insert_one(doc.view());
			}

//				printf("MongoOrder Update_Order end! ...\n");
		}
	void MongoOrder::Update_ActionCloseOrder(OrderInfo orderinfo)
			{
			char logbuf[100];
			LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);

				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						  << "CloseOrderRef" << orderinfo.CloseOrderRef
						  << "TradingDay" <<  orderinfo.TradingDay << finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";

					sprintf(logbuf,"MongoOrder Update_ActionCloseOrder  ...%s Price: %.2f ",orderinfo.InstrumentID,orderinfo.LimitPrice);
					logutil.PrintLog(logbuf);

					 Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {

								  collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
									 << "CloseOrderRef" << orderinfo.CloseOrderRef
											  << "TradingDay" <<  orderinfo.TradingDay << finalize,
												  document{} << "$set" << open_document <<
												   "Status" <<  orderinfo.Status <<
													"LocalUpdateTime" << str_localupdate << close_document << finalize);

						  }
					 }



				}
				else
				{

					sprintf(logbuf,"MongoOrder Update_ActionOrder  ...%s TradeID 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
					logutil.PrintLog(logbuf);
	//					printf("-----> 数据不存在，插入新数据！\n");
	//					 collection.insert_one(doc.view());
				}

	//				printf("MongoOrder Update_Order end! ...\n");
			}
	void MongoOrder::Check_ActionOrder(OrderInfo orderinfo)
		{
		char logbuf[100];
		LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);

			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
					  << "OrderRef" << orderinfo.OrderRef
					  << "TradingDay" <<  orderinfo.TradingDay << finalize);
			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				sprintf(logbuf,"MongoOrder Check_ActionOrder  ...%s Price: %.2f ",orderinfo.InstrumentID,orderinfo.LimitPrice);
				logutil.PrintLog(logbuf);

				 Json::Reader  reader;
				  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
				  {
					  if(!value["InstrumentID"].isNull())
					  {
							  collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
								 << "OrderRef" << orderinfo.OrderRef
										  << "TradingDay" <<  orderinfo.TradingDay << finalize,
											  document{} << "$set" << open_document <<
											   "Status" <<  orderinfo.Status <<
											   "OpenCancelSignalName" <<  orderinfo.OpenCancelSignalName <<
											  "LocalUpdateTime" << str_localupdate << close_document << finalize);

					  }
				 }



			}
			else
			{

				sprintf(logbuf,"MongoOrder Check_ActionOrder  ...%s TradeID 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
				logutil.PrintLog(logbuf);
//					printf("-----> 数据不存在，插入新数据！\n");
//					 collection.insert_one(doc.view());
			}

//				printf("MongoOrder Update_Order end! ...\n");
		}

	void MongoOrder::Check_ActionCloseOrder(OrderInfo orderinfo)
			{
			char logbuf[100];
			LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);

				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						  << "CloseOrderRef" << orderinfo.CloseOrderRef
						  << "TradingDay" <<  orderinfo.TradingDay << finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
					sprintf(logbuf,"MongoOrder Check_ActionCloseOrder  ...%s Price: %.2f ",orderinfo.InstrumentID,orderinfo.LimitPrice);
					logutil.PrintLog(logbuf);

					 Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {
								  collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
									 << "CloseOrderRef" << orderinfo.CloseOrderRef
											  << "TradingDay" <<  orderinfo.TradingDay << finalize,
												  document{} << "$set" << open_document <<
												   "Status" <<  orderinfo.Status <<
												   "CloseCancelSignalName" <<  orderinfo.CloseCancelSignalName <<
												  "LocalUpdateTime" << str_localupdate << close_document << finalize);

						  }
					 }



				}
				else
				{

					sprintf(logbuf,"MongoOrder Check_ActionOrder  ...%s TradeID 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
					logutil.PrintLog(logbuf);
	//					printf("-----> 数据不存在，插入新数据！\n");
	//					 collection.insert_one(doc.view());
				}

	//				printf("MongoOrder Update_Order end! ...\n");
			}
	void MongoOrder::Update_ErrorOrder(OrderInfo orderinfo)
		{
			char logbuf[100];
			LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

			mongocxx::uri uri(_mongodb_url);

			mongocxx::client client(uri);

			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];

			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


			document doc{};
			doc<< "InstrumentID" << orderinfo.InstrumentID
					<< "StopPrice" << orderinfo.StopPrice
					<< "TradingDay" << orderinfo.TradingDay
					<< "Direction" << orderinfo.Direction
					<< "Volume" << orderinfo.Volume
					<< "OrderRef" << orderinfo.OrderRef;



			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
					  << "OrderRef" << orderinfo.OrderRef
						  << "TradingDay" << orderinfo.TradingDay<< finalize);
			if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";


				collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						<< "TradingDay" << orderinfo.TradingDay << finalize,
									  document{} << "$set" << open_document <<
									   "Status" << orderinfo.Status <<
									  "ErrorId" << orderinfo.ErrorId <<
									  "FrontID" << orderinfo.FrontID<<
									  "SessionID" << orderinfo.SessionID<<
									  "ExchangeID" << orderinfo.ExchangeID<<
									  "LocalUpdateTime" << str_localupdate << close_document << finalize);
			}
			else
			{

				sprintf(logbuf,"MongoOrder Update_ErrorOrder  ...%s TradingDay 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
				logutil.PrintLog(logbuf);
//					printf("-----> 数据不存在，插入新数据！\n");
//					 collection.insert_one(doc.view());
			}

//				printf("MongoOrder Update_Order end! ...\n");
		}

	void MongoOrder::Update_ErrorCloseOrder(OrderInfo orderinfo)
			{
				char logbuf[100];
				LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);

				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


				document doc{};
				doc<< "InstrumentID" << orderinfo.InstrumentID
						<< "StopPrice" << orderinfo.StopPrice
						<< "TradingDay" << orderinfo.TradingDay
						<< "Direction" << orderinfo.Direction
						<< "Volume" << orderinfo.Volume
						<< "OrderRef" << orderinfo.OrderRef;



				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
						  << "OrderRef" << orderinfo.OrderRef
							  << "TradingDay" << orderinfo.TradingDay<< finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";


					collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							<< "TradingDay" << orderinfo.TradingDay << finalize,
										  document{} << "$set" << open_document <<
										   "Status" << orderinfo.Status <<
										  "ErrorId" << orderinfo.ErrorId <<
										  "FrontID" << orderinfo.FrontID<<
										  "SessionID" << orderinfo.SessionID<<
										  "ExchangeID" << orderinfo.ExchangeID<<
										  "LocalUpdateTime" << str_localupdate << close_document << finalize);
				}
				else
				{

					sprintf(logbuf,"MongoOrder Update_ErrorOrder  ...%s TradingDay 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
					logutil.PrintLog(logbuf);
	//					printf("-----> 数据不存在，插入新数据！\n");
	//					 collection.insert_one(doc.view());
				}

	//				printf("MongoOrder Update_Order end! ...\n");
			}
	void MongoOrder::Update_Trade(OrderInfo orderinfo)
				{
				char logbuf[100];
				LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

				mongocxx::uri uri(_mongodb_url);

				mongocxx::client client(uri);

				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
				  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							  << "TradingDay" << orderinfo.TradingDay
							  << "OrderRef" << orderinfo.OrderRef << finalize);
				if(maybe_result) {
	//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";

					sprintf(logbuf,"MongoOrder Update_Trade  ...%s Price: %.2f ",orderinfo.InstrumentID,orderinfo.Price);
					logutil.PrintLog(logbuf);

					collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							<< "OrderRef" << orderinfo.OrderRef
							<< "TradingDay" << orderinfo.TradingDay << finalize,
										  document{} << "$set" << open_document <<
										  "Status" << orderinfo.Status<<
										  "Price" << orderinfo.Price<<
										  "OpenPrice" << orderinfo.Price<<
										  "TraderID" << orderinfo.TraderID<<
										  "TradeID" << orderinfo.TradeID<<
										  "TradeDate" << orderinfo.TradeDate<<
										  "TradeTime" << orderinfo.TradeTime<<
										  "LocalTradeTime" << str_localupdate<<
										  "LocalUpdateTime" << str_localupdate << close_document << finalize);
				}
				else
				{

					sprintf(logbuf,"MongoOrder Update_Trade  ...%s TradingDay 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
					logutil.PrintLog(logbuf);
//					printf("-----> 数据不存在，插入新数据！\n");
//					 collection.insert_one(doc.view());
				}

		//				printf("MongoOrder Update_Order end! ...\n");
		}

	void MongoOrder::Update_CloseTrade(OrderInfo orderinfo)
					{
					char logbuf[100];
					LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");

					mongocxx::uri uri(_mongodb_url);

					mongocxx::client client(uri);

					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					DateUtil dateutil;
					std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
					  collection.find_one(document{} << "InstrumentID" << orderinfo.InstrumentID
							  << "CloseOrderRef" << orderinfo.CloseOrderRef
							  << "CloseTradingDay" <<  orderinfo.CloseTradingDay  << finalize);
					if(maybe_result) {
		//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";

						sprintf(logbuf,"MongoOrder Update_CloseTrade  ...%s Price: %.2f ",orderinfo.InstrumentID,orderinfo.Price);
						logutil.PrintLog(logbuf);

						collection.update_one(document{} << "InstrumentID" << orderinfo.InstrumentID
								 << "CloseOrderRef" << orderinfo.CloseOrderRef
									  << "CloseTradingDay" <<  orderinfo.CloseTradingDay  << finalize,
											  document{} << "$set" << open_document <<
											  "Status" << orderinfo.Status<<
											  "ClosePrice" << orderinfo.ClosePrice<<
											  "CloseTradeID" << orderinfo.CloseTradeID<<
											  "CloseTradeDate" << orderinfo.CloseTradeDate<<
											  "CloseTradeTime" << orderinfo.CloseTradeTime<<
											  "LocalUpdateTime" << str_localupdate << close_document << finalize);
					}
					else
					{

						sprintf(logbuf,"MongoOrder Update_Trade  ...%s TradingDay 不存在 %s ",orderinfo.InstrumentID,orderinfo.TradingDay);
						logutil.PrintLog(logbuf);
	//					printf("-----> 数据不存在，插入新数据！\n");
	//					 collection.insert_one(doc.view());
					}

			//				printf("MongoOrder Update_Order end! ...\n");
			}
	bool MongoOrder::Exist_OrderbyInstrumentID(const char * pinstrumentid,const char *tradingday)
		{
				bool ret;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


			bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
			  collection.find_one(document{} << "InstrumentID" << pinstrumentid
						  << "TradingDay" << tradingday<< finalize);
			if(maybe_result) {

				ret= true;
			}
			else
			{

				 ret =false;
			}


			return ret;
		}

	// 获取所有提交成功 未成交的合约,如何超时则用于撤单或者改单
	void  MongoOrder::Find_OrderInsertTimeoutList(vector<OrderInfo> &temp,const char *tradingday,int timeout_sec)
		{

			char logbuf[100];
			LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");
			OrderInfo orderinfo;

			mongocxx::uri uri(_mongodb_url);
			mongocxx::client client(uri);
			mongocxx::database dbname = client[_mongodb_dbname];
			mongocxx::collection collection = dbname[_mongodb_collenction];


			auto cursor = collection.find(document{} << "Status" << 1
							<< "TradingDay" << tradingday
							<< "LocalDate" << tradingday << finalize);

			for(auto doc : cursor)
			{
//								  std::cout << bsoncxx::to_json(doc) << "\n";

			  Json::Reader  reader;
			  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull()&& !value["LocalDate"].isNull())
					  {
						  	DateUtil dateutil;
							time_t localreqordertime ;


							time_t localcurrenttime = time(NULL);//这句返回的只是一个时间
							localreqordertime =dateutil.ConvertStringtoTime(value["LocalReqOrderTime"].asString().c_str());
							  printf("MongoInvestPosition localreqordertime: %ld localcurrenttime:  %ld ---> %ld ...\n",
									  localreqordertime,localcurrenttime,localcurrenttime - localreqordertime);

							if ((localcurrenttime - localreqordertime ) > timeout_sec)
							{

								  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
								  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
								  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
								  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
								  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
								  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
								  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
								  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
								  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
								  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
								  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());

								  orderinfo.Direction = value["Direction"].asInt();
								  orderinfo.FrontID = value["FrontID"].asInt();
								  orderinfo.SessionID = value["SessionID"].asInt();
								  orderinfo.Volume = value["Volume"].asInt();
								  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
								  orderinfo.Price = value["Price"].asDouble();
								  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
								  orderinfo.StopPrice = value["StopPrice"].asDouble();

								 temp.push_back(orderinfo);
								 sprintf(logbuf,"MongoInvestPosition localreqordertime: %ld localcurrenttime:  %ld ---> %ld ...\n",
													  localreqordertime,localcurrenttime,localcurrenttime - localreqordertime);
								 logutil.PrintLog(logbuf);
							}


					  }
				 }

			}

//			 printf("MongoOrder Find_OrderInsertTimeoutList! ...\n");

		}

	// 获取所有提交成功 未成交的合约,用于撤单或者改单
		void  MongoOrder::Find_OrderQueueingList(vector<OrderInfo> &temp,const char *tradingday)
			{

				char logbuf[100];
				LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");
				OrderInfo orderinfo;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				auto cursor = collection.find(document{} << "Status" << 1
								<< "TradingDay" << tradingday	 << finalize);

				for(auto doc : cursor)
				{
//				  std::cout << bsoncxx::to_json(doc) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
					  {
							  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
							  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
							  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
							  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
							  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
							  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
							  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
							  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
							  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
							  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
							  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());

							  orderinfo.Direction = value["Direction"].asInt();
							  orderinfo.FrontID = value["FrontID"].asInt();
							  orderinfo.SessionID = value["SessionID"].asInt();
							  orderinfo.Volume = value["Volume"].asInt();
							  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
							  orderinfo.Price = value["Price"].asDouble();
							  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
							  orderinfo.StopPrice = value["StopPrice"].asDouble();

								 temp.push_back(orderinfo);
					  }
				 }

				}

	//			 printf("MongoOrder Find_OrderQueueingList! ...\n");

			}

		void  MongoOrder::Find_OrderQueueingStatusList(vector<OrderInfo> &temp,const char *tradingday,int status,int offset)
				{

					char logbuf[100];
					LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");
					OrderInfo orderinfo;

					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					auto cursor = collection.find(document{} << "Status" << status
							<<"CombOffsetFlag" << offset
									<< "TradingDay" << tradingday	 << finalize);

					for(auto doc : cursor)
					{
	//				  std::cout << bsoncxx::to_json(doc) << "\n";

					  Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
						  {
								  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
								  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
								  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
								  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
								  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
								  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
								  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
								  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
								  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
								  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
								  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());

								  orderinfo.Direction = value["Direction"].asInt();
								  orderinfo.FrontID = value["FrontID"].asInt();
								  orderinfo.SessionID = value["SessionID"].asInt();
								  orderinfo.Volume = value["Volume"].asInt();
								  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
								  orderinfo.Price = value["Price"].asDouble();
								  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
								  orderinfo.StopPrice = value["StopPrice"].asDouble();

									 temp.push_back(orderinfo);
						  }
					 }

					}

		//			 printf("MongoOrder Find_OrderQueueingList! ...\n");

				}
		bool  MongoOrder::Exist_QueueingOrder(const char* pinstrumentid,const char *tradingday)
				{
					bool ret=false;
					char logbuf[100];
					LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");


					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					auto cursor = collection.find(document{} << "Status"  << 1
								<< "TradingDay" << tradingday << finalize);

					for(auto doc : cursor)
					{
	//				  std::cout << bsoncxx::to_json(doc) << "\n";

					  Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
						  {
							ret=true;
//							printf("MongoOrder Exist_QueueingOrder 存在! ...\n");
						  }
					 }

					}

		//			 printf("MongoOrder Find_OrderQueueingList! ...\n");
					return ret;
				}

		int  MongoOrder::Count_ActionOrder(const char* pinstrumentid,const char *tradingday)
						{
							int ret=0;
							char logbuf[100];
							LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");


							mongocxx::uri uri(_mongodb_url);
							mongocxx::client client(uri);
							mongocxx::database dbname = client[_mongodb_dbname];
							mongocxx::collection collection = dbname[_mongodb_collenction];

							auto cursor = collection.find(document{} << "Status" << 22
											<< "TradingDay" << tradingday
											<< "InstrumentID" << pinstrumentid << finalize);

							for(auto doc : cursor)
							{
			//				  std::cout << bsoncxx::to_json(doc) << "\n";

							  Json::Reader  reader;
							  Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(doc),value))
							  {
								  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
								  {
									ret++;
								  }
							 }

							}

				//			 printf("MongoOrder Count_ActionOrder! ...\n");
							return ret;
						}
//监测是否有某合约的当天交易日成交单
		int  MongoOrder::Count_Trade(const char* pinstrumentid,const char *tradingday)
				{
					int ret=0;
					char logbuf[100];
					LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");
					OrderInfo orderinfo;

					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					auto cursor = collection.find(document{} << "Status" << 2
									<< "TradingDay" << tradingday
									<< "InstrumentID" << pinstrumentid<< finalize);

					for(auto doc : cursor)
					{
	//				  std::cout << bsoncxx::to_json(doc) << "\n";

					  Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
						  {
							ret++;
						  }
					 }

					}

		//			 printf("MongoOrder Find_OrderQueueingList! ...\n");
					return ret;
				}

		int  MongoOrder::Count_OrderbyStatus(const char* pinstrumentid,const char *tradingday,int status)
						{
//							printf("MongoOrder Count_OrderbyStatus! ...\n");
							int ret=0;
							char logbuf[100];
							LogUtil logutil("mongodblog","/home/vnpy/autotrader/baseinfo/mongodb.log");
//							logutil.PrintLog("MongoOrder Count_OrderbyStatus begin! ...\n");

							mongocxx::uri uri(_mongodb_url);
							mongocxx::client client(uri);
							mongocxx::database dbname = client[_mongodb_dbname];
							mongocxx::collection collection = dbname[_mongodb_collenction];

							auto cursor = collection.find(document{} << "Status" << status
											<< "TradingDay" << tradingday
											<< "InstrumentID" << pinstrumentid << finalize);

							for(auto doc : cursor)
							{
//							  std::cout << bsoncxx::to_json(doc) << "\n";

							  Json::Reader  reader;
							  Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(doc),value))
							  {
								  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
								  {
									ret++;
								  }
							 }

							}

//							logutil.PrintLog("MongoOrder Count_OrderbyStatus end! ...\n");
							return ret;
						}

	 //获取所有持仓的合约,可用于实时检测止损
	 void MongoOrder::Find_OrderHoldList(vector<OrderInfo> &temp)
			{
				 OrderInfo orderinfo;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				mongocxx::options::find opts;
					bsoncxx::builder::stream::document order_builder;
					order_builder << "TradeDate" << 1 ;   //
					opts.sort(order_builder.view());

				auto cursor = collection.find(document{} << "Status" << 2
						<<"CombOffsetFlag" << 0 << finalize,opts);

				for(auto doc : cursor)
				{
	//								  std::cout << bsoncxx::to_json(doc) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

				  if(reader.parse(bsoncxx::to_json(doc),value))
				  {
					  if(!value["InstrumentID"].isNull())
					  {

						  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
						  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
						  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
						  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
						  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
						  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
						  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
						  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
						  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
						  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
						  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());

						  orderinfo.Direction = value["Direction"].asInt();
						  orderinfo.FrontID = value["FrontID"].asInt();
						  orderinfo.SessionID = value["SessionID"].asInt();
						  orderinfo.Volume = value["Volume"].asInt();
						  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
						  orderinfo.Price = value["Price"].asDouble();
						  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
						  orderinfo.StopPrice = value["StopPrice"].asDouble();
						  temp.push_back(orderinfo);

					  }
				 }

				}

			}

	 //获取所有持仓的合约,可用于实时检测止损
	 	 void MongoOrder::Find_CloseOrderList(vector<OrderInfo> &temp,const char * tradingday)
	 			{


	 				mongocxx::uri uri(_mongodb_url);
	 				mongocxx::client client(uri);
	 				mongocxx::database dbname = client[_mongodb_dbname];
	 				mongocxx::collection collection = dbname[_mongodb_collenction];

	 				mongocxx::options::find opts;
	 					bsoncxx::builder::stream::document order_builder;
	 					order_builder << "CloseTradeDate" << 1 ;   //
	 					opts.sort(order_builder.view());

	 				auto cursor = collection.find(document{} << "Status" << 12
	 						<<"CombOffsetFlag" << 1
							<<"CloseTradeDate" << tradingday << finalize,opts);

	 				for(auto doc : cursor)
	 				{
	 	//								  std::cout << bsoncxx::to_json(doc) << "\n";

	 				  Json::Reader  reader;
	 				  Json::Value   value;

	 				  if(reader.parse(bsoncxx::to_json(doc),value))
	 				  {
	 					  if(!value["InstrumentID"].isNull())
	 					  {
	 						 OrderInfo orderinfo;
	 						  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
	 						  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
	 						  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
	 						  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
	 						  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
	 						  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
	 						  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
	 						  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
	 						  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
	 						  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
	 						  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());
	 						 strcpy(orderinfo.CloseTradeDate,value["CloseTradeDate"].asString().c_str());

	 						  orderinfo.Direction = value["Direction"].asInt();
	 						  orderinfo.FrontID = value["FrontID"].asInt();
	 						  orderinfo.SessionID = value["SessionID"].asInt();
	 						  orderinfo.Volume = value["Volume"].asInt();
	 						  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
	 						  orderinfo.Price = value["Price"].asDouble();
	 						  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
	 						 orderinfo.ClosePrice = value["ClosePrice"].asDouble();
	 						  orderinfo.StopPrice = value["StopPrice"].asDouble();
	 						  temp.push_back(orderinfo);

	 					  }
	 				 }

	 				}

	 				auto cursor1 = collection.find(document{} << "Status" << 12
							<<"CombOffsetFlag" << 3
							<<"CloseTradeDate" << tradingday << finalize,opts);

					for(auto doc : cursor1)
					{
		//								  std::cout << bsoncxx::to_json(doc) << "\n";

					  Json::Reader  reader;
					  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {
							  OrderInfo orderinfo;
							  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
							  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
							  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
							  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
							  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
							  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
							  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
							  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
							  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
							  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
							  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());
							 strcpy(orderinfo.CloseTradeDate,value["CloseTradeDate"].asString().c_str());

							  orderinfo.Direction = value["Direction"].asInt();
							  orderinfo.FrontID = value["FrontID"].asInt();
							  orderinfo.SessionID = value["SessionID"].asInt();
							  orderinfo.Volume = value["Volume"].asInt();
							  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
							  orderinfo.Price = value["Price"].asDouble();
							  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
							 orderinfo.ClosePrice = value["ClosePrice"].asDouble();
							  orderinfo.StopPrice = value["StopPrice"].asDouble();
							  temp.push_back(orderinfo);

						  }
					 }

					}

	 			}
	//获取所有当天开仓的合约,可用于尾盘平仓检查
	void  MongoOrder::Find_TodayPositionTradeList(vector<OrderInfo> &temp,const char * tradingday)
			{
				OrderInfo orderinfo;

				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);
				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];


				auto cursor = collection.find(document{} << "Status" << 2
						<<"CombOffsetFlag" << 0
						<<"TradeDate" << tradingday << finalize);

				for(auto doc : cursor)
				{
	//								  std::cout << bsoncxx::to_json(doc) << "\n";

				  Json::Reader  reader;
				  Json::Value   value;

					  if(reader.parse(bsoncxx::to_json(doc),value))
					  {
						  if(!value["InstrumentID"].isNull())
						  {

							  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
							  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
							  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
							  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
							  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
							  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
							  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
							  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
							  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
							  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
							  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());

							  orderinfo.Direction = value["Direction"].asInt();
							  orderinfo.FrontID = value["FrontID"].asInt();
							  orderinfo.SessionID = value["SessionID"].asInt();
							  orderinfo.Volume = value["Volume"].asInt();
							  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
							  orderinfo.Price = value["Price"].asDouble();
							  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
							  orderinfo.StopPrice = value["StopPrice"].asDouble();
							  temp.push_back(orderinfo);
	//							  printf("MongoOrder Find_TradeList[%d]! ...\n",tradeinsidlist.length);
						  }
					 }

				}


			}
	OrderInfo  MongoOrder::Find_byInstOrderRef(const char *pinstrumentid,const char *tradingday,const char *orderref)
			{

				OrderInfo orderinfo;
				mongocxx::uri uri(_mongodb_url);
				mongocxx::client client(uri);

				mongocxx::database dbname = client[_mongodb_dbname];
				mongocxx::collection collection = dbname[_mongodb_collenction];

				bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
						  collection.find_one(document{} << "InstrumentID" << pinstrumentid
								  << "TradingDay" << tradingday
								  << "OrderRef" << orderref << finalize);
				if(maybe_result) {
//				  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
				//  printf("MongoExchangeInfo check ... %\n",bsoncxx::to_json(*maybe_result).c_str() );
//				  printf("-----> 数据存在，-----无需插入数据！\n");

					 Json::Reader  reader;
					  Json::Value   value;

						  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
						  {
							  if(!value["InstrumentID"].isNull()&& !value["OrderRef"].isNull())
							  {
								  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
								  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
								  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
								  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
								  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
								  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
								  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
								  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
								  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());

								  orderinfo.FrontID = value["FrontID"].asInt();
								  orderinfo.SessionID = value["SessionID"].asInt();
								  orderinfo.Volume = value["Volume"].asInt();
								  orderinfo.Direction = value["Direction"].asInt();
								  orderinfo.Price = value["Price"].asDouble();
								  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
								  orderinfo.StopPrice = value["StopPrice"].asDouble();

							  }
						 }
				}
				else
				{
//					printf("-----> 合约无持仓！\n");

				}
//					printf("MongoInvestPosition Find_PositionbyInstrumentid  end! ...\n");

				return orderinfo;
			}

	OrderInfo  MongoOrder::Find_byInstrumentid(const char *pinstrumentid)
				{

					OrderInfo orderinfo;
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);

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
									  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
									  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
									  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
									  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
									  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
									  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
									  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
									  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
									  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());

									  orderinfo.FrontID = value["FrontID"].asInt();
									  orderinfo.SessionID = value["SessionID"].asInt();
									  orderinfo.Volume = value["Volume"].asInt();
									  orderinfo.Price = value["Price"].asDouble();
									  orderinfo.LimitPrice = value["LimitPrice"].asDouble();
									  orderinfo.StopPrice = value["StopPrice"].asDouble();

								  }
							 }
					}
					else
					{
	//					printf("-----> 合约无持仓！\n");

					}
	//					printf("MongoInvestPosition Find_PositionbyInstrumentid  end! ...\n");

					return orderinfo;
				}
	OrderInfo  MongoOrder::Find_TradebyInstrumentid(const char *pinstrumentid,const char *tradeid)
				{

					OrderInfo orderinfo;
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);

					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					bsoncxx::stdx::optional< bsoncxx::document::value > maybe_result =
							  collection.find_one(document{} << "InstrumentID" << pinstrumentid
									  << "TradeID" << tradeid << finalize);
					if(maybe_result) {
//					  std::cout << bsoncxx::to_json(*maybe_result) << "\n";
//					  printf("MongoOrder Find_TradebyInstrumentid ... %\n",bsoncxx::to_json(*maybe_result).c_str() );


						 Json::Reader  reader;
						  Json::Value   value;

							  if(reader.parse(bsoncxx::to_json(*maybe_result),value))
							  {
								  if(!value["InstrumentID"].isNull())
								  {
									  strcpy(orderinfo.InstrumentID,value["InstrumentID"].asString().c_str());
									  strcpy(orderinfo.TradingDay,value["TradingDay"].asString().c_str());
									  strcpy(orderinfo.TraderID,value["TraderID"].asString().c_str());
									  strcpy(orderinfo.TradeID,value["TradeID"].asString().c_str());
									  strcpy(orderinfo.LocalUpdateTime,value["LocalUpdateTime"].asString().c_str());
									  strcpy(orderinfo.InsertDate,value["InsertDate"].asString().c_str());
									  strcpy(orderinfo.InsertTime,value["InsertTime"].asString().c_str());
									  strcpy(orderinfo.OrderRef,value["OrderRef"].asString().c_str());
									  strcpy(orderinfo.ExchangeID,value["ExchangeID"].asString().c_str());
									  strcpy(orderinfo.OrderSysID,value["OrderSysID"].asString().c_str());
									  strcpy(orderinfo.TradeDate,value["TradeDate"].asString().c_str());

									  orderinfo.Direction = value["Direction"].asInt();
									  orderinfo.FrontID = value["FrontID"].asInt();
									  orderinfo.SessionID = value["SessionID"].asInt();
									  orderinfo.Volume = value["Volume"].asInt();
									  orderinfo.Price = value["Price"].asDouble();
									  orderinfo.OpenPrice = value["OpenPrice"].asDouble();
									  orderinfo.StopPrice = value["StopPrice"].asDouble();

								  }
							 }
					}
					else
					{
						printf("MongoOrder----->Find_TradebyInstrumentid  合约不存在！\n");

					}
	//					printf("MongoOrder Find_TradebyInstrumentid  end! ...\n");

					return orderinfo;
				}


	int MongoOrder::Count()
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

	int MongoOrder::OrderInsertTimeOutCount()
				{
					mongocxx::uri uri(_mongodb_url);
					mongocxx::client client(uri);
					mongocxx::database dbname = client[_mongodb_dbname];
					mongocxx::collection collection = dbname[_mongodb_collenction];

					int i =0;
					auto cursor = collection.find(document{} << "Status" << 1 << finalize);
					for (auto&& doc : cursor)
					{
						i++;
					}


					return i;

				}

	void MongoOrder::DeleteAll()
					{
			//			printf("MongoTrade Delete begin! ...\n");
						mongocxx::uri uri(_mongodb_url);
						mongocxx::client client(uri);
						//打开"baseinfo"数据库
						mongocxx::database dbname = client[_mongodb_dbname];
						mongocxx::collection collection = dbname[_mongodb_collenction];


						collection.delete_many(document{} << finalize);
						printf("MongoOrder DeleteAll finished! ...\n");
					}
