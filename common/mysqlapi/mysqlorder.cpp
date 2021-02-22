#include "mysqlorder.hpp"

MysqlOrder::MysqlOrder(const char*flag,const char * userid)
{

	std::string database="account";

	std::string tablename=flag;
	tablename.append("_");
	tablename.append(userid);

	if (IsAliveConn())
	{

	}else
	{
		if(!Connect())
		{

			return;
		}
	}

	this->Set_Database(database);
	this->Set_Table(tablename);

}


void MysqlOrder::CreateTable(const char* tablename)
{

		char buf[2048];
		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
					"InstrumentID CHAR(50),"
					"Status INT(11),"
					"Direction INT(11),"
					"TradeID CHAR(21),"
					"CombOffsetFlag INT(11),"
					"CombHedgeFlag INT(11),"
					"FrontID INT(11),"
					"SessionID INT(11),"
					"ErrorId INT(11),"
					"OrderSysID CHAR(21),"
					"OrderRef CHAR(21),"
					"ExchangeID CHAR(21),"
					"SignalName CHAR(50),"
					"CancelSignalName CHAR(50),"
					"LimitPrice DOUBLE,"
					"Price DOUBLE,"
					"StopPrice DOUBLE,"
					"StopRange DOUBLE,"
					"WinPrice DOUBLE,"
					"VolumeTotalOriginal INT(11),"
					"Volume INT(11),"
					"OrderSubmitStatus INT(11),"
					"OrderStatus INT(11),"
					"TradingDay CHAR(20),"
					"TradeDate CHAR(20),"
					"TradeTime CHAR(20),"
					"LocalUpdateTime DATETIME)  default character set utf8 ",tablename);
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);

}

void MysqlOrder::CreateTable()
{

	if(!this->Exist_Table(this->getTablename().c_str()))	{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
					"InstrumentID CHAR(50),"
					"Status INT(11),"
					"Direction INT(11),"
					"TradeID CHAR(21),"
					"CombOffsetFlag INT(11),"
					"CombHedgeFlag INT(11),"
					"FrontID INT(11),"
					"SessionID INT(11),"
					"ErrorId INT(11),"
					"OrderSysID CHAR(21),"
					"OrderRef CHAR(21),"
					"ExchangeID CHAR(21),"
					"SignalName CHAR(50),"
					"CancelSignalName CHAR(50),"
					"LimitPrice DOUBLE,"
					"Price DOUBLE,"
					"StopPrice DOUBLE,"
					"StopRange DOUBLE,"
					"WinPrice DOUBLE,"
					"VolumeTotalOriginal INT(11),"
					"Volume INT(11),"
					"OrderSubmitStatus INT(11),"
					"OrderStatus INT(11),"
					"TradingDay CHAR(20),"
					"TradeDate CHAR(20),"
					"TradeTime CHAR(20),"
					"LocalUpdateTime DATETIME)  default character set utf8 ",this->getTablename().c_str());


		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}
void MysqlOrder::Insert(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"INSERT INTO %s (InstrumentID,Status,CombOffsetFlag,CombHedgeFlag,Direction,"
				"OrderRef,ExchangeID,SignalName,LimitPrice,StopPrice, "
				"VolumeTotalOriginal,TradingDay,StopRange,WinPrice,LocalUpdateTime)"
				"VALUES ('%s','%d', '%d','%d','%d',"
				"'%s','%s','%s','%.3f','%.3f',"
				"'%d','%s','%.3f','%.3f','%s')",this->getTablename().c_str(),
				data.InstrumentID,data.Status,data.CombOffsetFlag,data.CombHedgeFlag,data.Direction,
				data.OrderRef,data.ExchangeID,data.SignalName,data.LimitPrice,data.StopPrice,
				data.VolumeTotalOriginal,data.TradingDay,data.StopRange,data.WinPrice,time.c_str());

	str_sql=buf;

	this->Execute(str_sql);


}

void MysqlOrder::UpdatebyFailReq(OrderData data)
{
//	printf("UpdatebyFailReq begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Status='%d',LocalUpdateTime='%s' "
				"WHERE InstrumentID='%s' AND  OrderRef='%s' AND TradingDay='%s' ",
				this->getTablename().c_str(),data.Status,time.c_str(),
				data.InstrumentID,data.OrderRef,data.TradingDay);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

void MysqlOrder::NewOrderData_byRspQryOrder(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"INSERT INTO %s (InstrumentID,Status,CombOffsetFlag,CombHedgeFlag,Direction,"
				"OrderRef,ExchangeID,LimitPrice,VolumeTotalOriginal,TradingDay, "
				"FrontID,SessionID,OrderSubmitStatus,OrderStatus,Volume,"
				"LocalUpdateTime)"
				"VALUES ('%s','%d', '%d','%d','%d',"
				"'%s','%s','%.3f','%d','%s',"
				"'%d','%d','%d','%d','%d',"
				"'%s')",this->getTablename().c_str(),
				data.InstrumentID,data.Status,data.CombOffsetFlag,data.CombHedgeFlag,data.Direction,
				data.OrderRef,data.ExchangeID,data.LimitPrice,data.VolumeTotalOriginal,data.TradingDay,
				data.FrontID,data.SessionID,data.OrderSubmitStatus,data.OrderStatus,data.Volume,
				time.c_str());

	str_sql=buf;

	this->Execute(str_sql);


}

void MysqlOrder::NewInvestPositionDetail(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"INSERT INTO %s (InstrumentID,Status,CombOffsetFlag,CombHedgeFlag,Direction,"
				"OrderRef,ExchangeID,TradeID,Price,StopPrice, "
				"VolumeTotalOriginal,Volume,TradingDay,LocalUpdateTime)"
				"VALUES ('%s','%d', '%d','%d','%d',"
				"'%s','%s','%s','%.3f','%.3f',"
				"'%d','%d','%s','%s')",this->getTablename().c_str(),
				data.InstrumentID,data.Status,data.CombOffsetFlag,data.CombHedgeFlag,data.Direction,
				data.OrderRef,data.ExchangeID,data.TradeID,data.Price,data.StopPrice,
				data.VolumeTotalOriginal,data.Volume,data.TradingDay,time.c_str());

	str_sql=buf;

	this->Execute(str_sql);


}



bool  MysqlOrder::Exist_OrderbyInstrumentid(const char* pinstrumentid,const char *tradeid)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradeID='%s'  ",this->getTablename().c_str(),pinstrumentid,tradeid);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		if(this->getRowCount()>0)		{
			ret=true;
		}


	}
	this->Empty();
	return ret;
}


bool  MysqlOrder::Exist_OrderbyInsOrderRef(const char* pinstrumentid,const char *orderref,const char*tradingday)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s'  AND  OrderRef='%s' AND TradingDay='%s'  ",this->getTablename().c_str(),
			pinstrumentid,orderref,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		if(this->getRowCount()>0)		{
			ret=true;
		}


	}
	this->Empty();
	return ret;
}



bool  MysqlOrder::Exist_OrderbyInstrumentidStatus(const char* pinstrumentid,const char* tradingday,int status)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradingDay='%s' AND Status='%d'  ",this->getTablename().c_str(),
			pinstrumentid,tradingday,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		if(this->getRowCount()>0)		{
			ret=true;
		}


	}
	this->Empty();
	return ret;
}


bool  MysqlOrder::Exist_CloseOrderbySection(const char* section,const char* tradingday)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  TradingDay='%s' AND Status='%d'  ",this->getTablename().c_str(),
			tradingday,2);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			char instrumentid[30];
			strcpy(instrumentid,getValueString("InstrumentID").c_str());

			char productid[31];

			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(instrumentid).c_str());
			std::string str_section;
			str_section  = codeutil.GetSectionByProductid(productid);
            if(strcmp(str_section.c_str(),section)==0){
            	ret=true;
            	return ret;
            }
            else {
            	this->getNext();
            }

		}


	}
	this->Empty();
	return ret;
}


void MysqlOrder::NewOrderData_byRtnOrder(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"INSERT INTO %s (InstrumentID,Status,CombOffsetFlag,CombHedgeFlag,Direction,"
					"OrderRef,ExchangeID,LimitPrice,VolumeTotalOriginal,TradingDay, "
					"FrontID,SessionID,OrderSubmitStatus,OrderStatus,Volume,"
					"LocalUpdateTime)"
					"VALUES ('%s','%d', '%d','%d','%d',"
					"'%s','%s','%.3f','%d','%s',"
					"'%d','%d','%d','%d','%d',"
					"'%s')",this->getTablename().c_str(),
					data.InstrumentID,data.Status,data.CombOffsetFlag,data.CombHedgeFlag,data.Direction,
					data.OrderRef,data.ExchangeID,data.LimitPrice,data.VolumeTotalOriginal,data.TradingDay,
					data.FrontID,data.SessionID,data.OrderSubmitStatus,data.OrderStatus,data.Volume,
					time.c_str());

	str_sql=buf;

	this->Execute(str_sql);


}


void MysqlOrder::UpdateOrderData_byRtnOrder(OrderData data)
{
//	printf("insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Status='%d',FrontID='%d',SessionID='%d',OrderSubmitStatus='%d',"
				"OrderStatus='%d',ExchangeID='%s',Volume='%d',LocalUpdateTime='%s' "
				"WHERE InstrumentID='%s' AND  OrderRef='%s' AND TradingDay='%s' ",
				this->getTablename().c_str(),data.Status,data.FrontID,data.SessionID,data.OrderSubmitStatus,
				data.OrderStatus,data.ExchangeID,data.Volume,time.c_str(),
				data.InstrumentID,data.OrderRef,data.TradingDay);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}


void MysqlOrder::UpdateOrderData_byOnRspOrderInsert(OrderData data)
{
//	printf("insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Status='%d',ErrorId='%d',LocalUpdateTime='%s' "
					"WHERE InstrumentID='%s' AND  OrderRef='%s' AND TradingDay='%s' ",
					this->getTablename().c_str(),data.Status,data.ErrorId,time.c_str(),
					data.InstrumentID,data.OrderRef,data.TradingDay);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}


void MysqlOrder::UpdateOrderActionData(OrderData data)
{
//	printf("insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Status='%d',CancelSignalName='%s' ,LocalUpdateTime='%s' "
				"WHERE InstrumentID='%s' AND  OrderRef='%s' AND TradingDay='%s' ",
				this->getTablename().c_str(),data.Status,	data.CancelSignalName,time.c_str(),data.InstrumentID,data.OrderRef,data.TradingDay);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

void MysqlOrder::NewOrderData_byRtnTrade(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"INSERT INTO %s (InstrumentID,Status,CombOffsetFlag,CombHedgeFlag,Direction,"
				"OrderRef,ExchangeID,Price,TradingDay,OrderSysID, "
				"TradeID,TradeDate,TradeTime,Volume,"
				"LocalUpdateTime)"
				"VALUES ('%s','%d', '%d','%d','%d',"
				"'%s','%s','%.3f','%s','%s',"
				"'%s','%s','%s','%d',"
				"'%s')",this->getTablename().c_str(),
				data.InstrumentID,data.Status,data.CombOffsetFlag,data.CombHedgeFlag,data.Direction,
				data.OrderRef,data.ExchangeID,data.Price,data.TradingDay,data.OrderSysID,
				data.TradeID,data.TradeDate,data.TradeTime,data.Volume,
				time.c_str());

	str_sql=buf;

	this->Execute(str_sql);


}

void MysqlOrder::UpdateOrderData_byRtnTrade(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Status='%d',TradeID='%s',TradeDate='%s',TradeTime='%s',"
				"Price='%.3f', Volume='%d',LocalUpdateTime='%s' "
				"WHERE InstrumentID='%s' AND  OrderRef='%s' AND TradingDay='%s' ",this->getTablename().c_str(),
				data.Status,data.TradeID,data.TradeDate,data.TradeTime,
				data.Price,data.Volume,time.c_str(),
				data.InstrumentID,data.OrderRef,data.TradingDay);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}


void MysqlOrder::UpdateOrderData_byErrRtnOrder(OrderData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Status='%d',ErrorId='%d',LocalUpdateTime='%s' "
				"WHERE InstrumentID='%s' AND  OrderRef='%s' AND TradingDay='%s' ",
				this->getTablename().c_str(),data.Status,data.ErrorId,time.c_str(),
				data.InstrumentID,data.OrderRef,data.TradingDay);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}



int  MysqlOrder::Count_OrderListbyStatus(const char* pinstrumentid,const char *tradingday,int status)
{

	int count=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Status='%d' AND  InstrumentID='%s' AND TradingDay='%s'  ",this->getTablename().c_str(),
			status,pinstrumentid,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		count=this->getRowCount();

	}
	this->Empty();
	return count;
}



void MysqlOrder::Find_OrderListsbyStatus(vector<OrderData> &temp,const char *tradingday,int status)
{
	OrderData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE TradingDay='%s'  AND Status='%d' ",this->getTablename().c_str(),tradingday,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 	 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
				 strcpy(data.SignalName,getValueString("SignalName").c_str());
				 data.LimitPrice = getValueDouble("LimitPrice");
				 data.Price = getValueDouble("Price");

				 data.StopPrice = getValueDouble("StopPrice");
				 data.WinPrice = getValueDouble("WinPrice");
				 data.StopRange = getValueDouble("StopRange");

				 data.Volume = getValueInt("Volume");
				 data.Direction = getValueInt("Direction");
				 data.FrontID = getValueInt("FrontID");
				 data.SessionID = getValueInt("SessionID");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}


void MysqlOrder::Find_AllOrderListsbyStatus(vector<OrderData> &temp,int status)
{
	OrderData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE Status='%d' ",this->getTablename().c_str(),status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 	 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
				 strcpy(data.SignalName,getValueString("SignalName").c_str());
				 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
				 data.LimitPrice = getValueDouble("LimitPrice");
				 data.Price = getValueDouble("Price");

				 data.StopPrice = getValueDouble("StopPrice");
				 data.WinPrice = getValueDouble("WinPrice");
				 data.StopRange = getValueDouble("StopRange");

				 data.Volume = getValueInt("Volume");
				 data.Direction = getValueInt("Direction");
				 data.FrontID = getValueInt("FrontID");
				 data.SessionID = getValueInt("SessionID");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}


void MysqlOrder::Find_AllOrderListsbyDateTimeStatus(vector<OrderData> &temp,const char* instrumentid,const char* datetime,int status)

{
	OrderData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE InstrumentID = '%s' AND LocalUpdateTime < '%s' AND  Status='%d' Order by LocalUpdateTime desc Limit 1",
			this->getTablename().c_str(),instrumentid,datetime,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 	 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
				 strcpy(data.SignalName,getValueString("SignalName").c_str());
				 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
				 data.LimitPrice = getValueDouble("LimitPrice");
				 data.Price = getValueDouble("Price");
				 data.StopPrice = getValueDouble("StopPrice");
				 data.WinPrice = getValueDouble("WinPrice");
				 data.StopRange = getValueDouble("StopRange");
				 data.Volume = getValueInt("Volume");
				 data.Direction = getValueInt("Direction");
				 data.FrontID = getValueInt("FrontID");
				 data.SessionID = getValueInt("SessionID");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

void MysqlOrder::Find_QueueOrderLists(vector<OrderData> &temp,const char *tradingday)
{
	OrderData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE TradingDay='%s'  AND   Status=1 ",this->getTablename().c_str(),tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 	 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
				 strcpy(data.SignalName,getValueString("SignalName").c_str());
				 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
				 data.LimitPrice = getValueDouble("LimitPrice");
				 data.Price = getValueDouble("Price");
				 data.StopPrice = getValueDouble("StopPrice");
				 data.WinPrice = getValueDouble("WinPrice");
				 data.StopRange = getValueDouble("StopRange");
				 data.Volume = getValueInt("Volume");
				 data.Direction = getValueInt("Direction");
				 data.FrontID = getValueInt("FrontID");
				 data.SessionID = getValueInt("SessionID");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}
int MysqlOrder::Count_QueueOrderProduct(const char *tradingday)
{

	int count=0;

	char buf[2018];
	std::string str_sql;

	sprintf(buf,"SELECT count(ID) FROM %s WHERE TradingDay='%s'  AND  ( Status=0 OR Status=1) ",
			this->getTablename().c_str(),tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
				count = getValueInt("count(ID)");
		}
	}

	this->Empty();

	return count;
}
int MysqlOrder::Count_QueueActionOrderProduct(const char *tradingday)
{

	int count=0;

	char buf[2018];
	std::string str_sql;

	sprintf(buf,"SELECT count(ID) FROM %s WHERE TradingDay='%s'  AND  (  Status=20 OR Status=21) ",
			this->getTablename().c_str(),tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
				count = getValueInt("count(ID)");
		}
	}

	this->Empty();

	return count;
}
int MysqlOrder::Get_DoneOrderListsVolume(const char* pinstrumentid,int direction,const char *tradingday)
{

	int totalvolume=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE InstrumentID='%s' AND Direction='%d' AND TradingDay='%s'  AND  ( Status=2 OR Status=3) ",
			this->getTablename().c_str(),pinstrumentid,direction,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
				/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)		{
			OrderData data;
			 data.VolumeTotalOriginal = getValueInt("VolumeTotalOriginal");
			 totalvolume+=data.VolumeTotalOriginal;
			this->getNext();
		}
	}
	this->Empty();
	return totalvolume;
}
int MysqlOrder::Get_QueueOrderListsVolume(const char* pinstrumentid,int direction,const char *tradingday)
{

	int totalvolume=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE InstrumentID='%s' AND Direction='%d' AND TradingDay='%s'  AND  ( Status=0 OR Status=1) ",
			this->getTablename().c_str(),pinstrumentid,direction,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
				/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
		{
			OrderData data;
			 data.VolumeTotalOriginal = getValueInt("VolumeTotalOriginal");
			 totalvolume+=data.VolumeTotalOriginal;
			this->getNext();
		}

	}
	this->Empty();
	return totalvolume;
}

// 计算撤单队列中存在的撤单合约的撤单仓量
int MysqlOrder::Get_QueueOrderActionListsVolume(const char* pinstrumentid,int direction,const char *tradingday)
{

	int totalvolume=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE InstrumentID='%s' AND Direction='%d' AND TradingDay='%s'  AND  ( Status=20 OR Status=21) ",
			this->getTablename().c_str(),pinstrumentid,direction,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
				/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
		{
			OrderData data;
			 data.VolumeTotalOriginal = getValueInt("VolumeTotalOriginal");
			 totalvolume+=data.VolumeTotalOriginal;
			this->getNext();
		}

	}
	this->Empty();
	return totalvolume;
}

void MysqlOrder::Find_AllLists(vector<OrderData> &temp)
{
	OrderData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 	 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
				 strcpy(data.SignalName,getValueString("SignalName").c_str());
				 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
				 data.LimitPrice = getValueDouble("LimitPrice");
				 data.Price = getValueDouble("Price");
				 data.StopPrice = getValueDouble("StopPrice");
				 data.WinPrice = getValueDouble("WinPrice");
				 data.StopRange = getValueDouble("StopRange");
				 data.Volume = getValueInt("Volume");
				 data.Direction = getValueInt("Direction");
				 data.FrontID = getValueInt("FrontID");
				 data.SessionID = getValueInt("SessionID");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

void MysqlOrder::Find_ListsByDate(vector<OrderData> &temp,const char * day)
{
	OrderData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  Where TradingDay > '%s' AND Status='%d' Order by TradingDay ASC ",
			this->getTablename().c_str(),day,2);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)		{

			 	 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
				 strcpy(data.TradeTime,getValueString("TradeTime").c_str());
				 strcpy(data.TradeDate,getValueString("TradeDate").c_str());
				 strcpy(data.SignalName,getValueString("SignalName").c_str());
				 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
				 data.LimitPrice = getValueDouble("LimitPrice");
				 data.Price = getValueDouble("Price");
				 data.StopPrice = getValueDouble("StopPrice");
				 data.WinPrice = getValueDouble("WinPrice");
				 data.StopRange = getValueDouble("StopRange");
				 data.Volume = getValueInt("Volume");
				 data.Direction = getValueInt("Direction");
				 data.FrontID = getValueInt("FrontID");
				 data.SessionID = getValueInt("SessionID");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

int  MysqlOrder::Get_VolumebyStatus(const char* pinstrumentid,const char *tradingday,int status)
{

	int totalvolume=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Status='%d' AND  InstrumentID='%s' AND TradingDay='%s'  ",this->getTablename().c_str(),status,pinstrumentid,tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
				/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
		{
			OrderData data;
			 data.Volume = getValueInt("Volume");
			 totalvolume+=data.Volume;
			this->getNext();
		}

	}
	this->Empty();
	return totalvolume;
}





int  MysqlOrder::Get_TotalVolumebyDate(const char* pinstrumentid,const char *tradingday)
{

	int totalvolume=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradingDay='%s'  ",this->getTablename().c_str(),pinstrumentid,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
				/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
		{
			OrderData data;
			 data.Volume = getValueInt("Volume");
			 totalvolume+=data.Volume;
			this->getNext();
		}

	}
	this->Empty();
	return totalvolume;
}




OrderData  MysqlOrder::Find_OrderbyInsOrderRef(const char* pinstrumentid,const char *orderref,const char*tradingday)
{

	OrderData data;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s'  AND  OrderRef='%s' AND TradingDay='%s'  ",this->getTablename().c_str(),
			pinstrumentid,orderref,tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

				/* retrieve the row count in the result set */
		if (this->getRowCount()==1)	{
			this->getFirst();
			 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
			 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
			 strcpy(data.SignalName,getValueString("SignalName").c_str());
			 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
			 data.LimitPrice = getValueDouble("LimitPrice");
			 data.Price = getValueDouble("Price");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.WinPrice = getValueDouble("WinPrice");
			 data.StopRange = getValueDouble("StopRange");
			 data.Volume = getValueInt("Volume");
			 data.Direction = getValueInt("Direction");
			 data.FrontID = getValueInt("FrontID");
			 data.SessionID = getValueInt("SessionID");
			 data.OrderSubmitStatus = getValueInt("OrderSubmitStatus");
			 data.OrderStatus = getValueInt("OrderStatus");
			 data.Status = getValueInt("Status");
		}

	}
	this->Empty();
	return data;
}
OrderData  MysqlOrder::Find_OrderbyInsTradingday(const char* pinstrumentid,const char*tradingday)
{

	OrderData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradingDay='%s'  AND  ( Status=2 OR Status=3)  ",
			this->getTablename().c_str(),pinstrumentid,tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

				/* retrieve the row count in the result set */
		if (this->getRowCount()==1)		{
			this->getFirst();
			 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
			 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
			 strcpy(data.SignalName,getValueString("SignalName").c_str());
			 strcpy(data.OrderRef,getValueString("OrderRef").c_str());


			 strcpy(data.TradeTime,getValueString("TradeTime").c_str());

			 data.LimitPrice = getValueDouble("LimitPrice");
			 data.Price = getValueDouble("Price");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.WinPrice = getValueDouble("WinPrice");
			 data.StopRange = getValueDouble("StopRange");
			 data.Volume = getValueInt("Volume");
			 data.Direction = getValueInt("Direction");
			 data.FrontID = getValueInt("FrontID");
			 data.SessionID = getValueInt("SessionID");
			 data.OrderSubmitStatus = getValueInt("OrderSubmitStatus");
			 data.OrderStatus = getValueInt("OrderStatus");
			 data.Status = getValueInt("Status");
		}

	}
	this->Empty();
	return data;
}
OrderData  MysqlOrder::Find_OrderbyInsTradeID(const char* pinstrumentid,const char *tradeid,const char*tradingday)
{

	OrderData data;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s'  AND  TradeID='%s' AND TradingDay='%s'  ",this->getTablename().c_str(),
			pinstrumentid,tradeid,tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

				/* retrieve the row count in the result set */
		if (this->getRowCount()==1)
		{
			this->getFirst();
			 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
			 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
			 strcpy(data.SignalName,getValueString("SignalName").c_str());
			 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
			 data.LimitPrice = getValueDouble("LimitPrice");
			 data.Price = getValueDouble("Price");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.WinPrice = getValueDouble("WinPrice");
			 data.StopRange = getValueDouble("StopRange");
			 data.Volume = getValueInt("Volume");
			 data.Direction = getValueInt("Direction");
			 data.FrontID = getValueInt("FrontID");
			 data.SessionID = getValueInt("SessionID");
			 data.OrderSubmitStatus = getValueInt("OrderSubmitStatus");
			 data.OrderStatus = getValueInt("OrderStatus");
			 data.Status = getValueInt("Status");
		}

	}
	this->Empty();
	return data;
}

int  MysqlOrder::Exist_CloseOrderbyInsTradingDay(const char* pinstrumentid,const char *tradingday)
{

	int ret=0;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradingDay='%s'  AND  Status='%d' ",
			this->getTablename().c_str(),pinstrumentid,tradingday,2);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		if(this->getRowCount()>0)		{
			ret=this->getRowCount();
		}


	}
	this->Empty();
	return ret;
}

OrderData  MysqlOrder::Find_CloseOrderbyInsTradingDay(const char* pinstrumentid,const char*tradingday)
{

	OrderData data;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradingDay= '%s'  AND  Status='%d'"
			" Order by TradingDay ASC  LIMIT 1 ",
			this->getTablename().c_str(),pinstrumentid,tradingday,2);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

				/* retrieve the row count in the result set */
		if (this->getRowCount()==1)		{
			this->getFirst();
			 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			 strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			 strcpy(data.TradeTime,getValueString("TradeTime").c_str());
			 strcpy(data.TradeDate,getValueString("TradeDate").c_str());
			 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
			 strcpy(data.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());
			 strcpy(data.SignalName,getValueString("SignalName").c_str());
			 strcpy(data.OrderRef,getValueString("OrderRef").c_str());
			 data.LimitPrice = getValueDouble("LimitPrice");
			 data.Price = getValueDouble("Price");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.WinPrice = getValueDouble("WinPrice");
			 data.StopRange = getValueDouble("StopRange");
			 data.Volume = getValueInt("Volume");
			 data.Direction = getValueInt("Direction");
			 data.FrontID = getValueInt("FrontID");
			 data.SessionID = getValueInt("SessionID");
			 data.OrderSubmitStatus = getValueInt("OrderSubmitStatus");
			 data.OrderStatus = getValueInt("OrderStatus");
			 data.Status = getValueInt("Status");
		}

	}
	this->Empty();
	return data;
}

std::string MysqlOrder::GetLastLocalUpdateTime()
{

	std::string  info;
	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s ORDER BY LocalUpdateTime desc limit 1 ",this->getTablename().c_str());
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();
					 info =  getValueString("LocalUpdateTime");
				}
		}
	this->Empty();
	  return info;

}



