#include "mysqlhold.hpp"



MysqlHold::MysqlHold(const char* userid)
{
	std::string database="account";
	std::string tablename="holdposition_";
	tablename.append(userid);

	if(this->IsCloseConn())	{
		if(!Connect())	{
			return;
		}
	}
	else{
		if (IsAliveConn()){
		}
		else{
			if(!Connect()){
				return;
			}
		}
	}

		this->Set_Database(database);
		this->Set_Table(tablename);

}


void MysqlHold::CreateTable()
{

	if(!this->Exist_Table(this->getTablename().c_str()))
	{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"InstrumentIDDate CHAR(50) NOT NULL PRIMARY KEY,"
						"InstrumentID CHAR(40),"
						"ExchangeID CHAR(10),"
						"Direction INT(11),"
						"Volume INT(11),"
						"OpenPriceAverage DOUBLE,"
						"StopPrice DOUBLE,"
						"WinPrice DOUBLE,"
						"StopRange DOUBLE,"
						"Highest DOUBLE,"
						"Lowest DOUBLE,"
						"Margin DOUBLE,"
						"HighProfit DOUBLE,"
						"TradingDay CHAR(20),"
						"OpenDate CHAR(20),"
						"LocalUpdateTime DATETIME)  ENGINE=MEMORY  default character set utf8 ",this->getTablename().c_str());
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}


void MysqlHold::Insert(HoldData data)
{
	//	printf("insert begin\n");

		char buf[2048];
		std::string str_sql;

		DateUtil dateutil;
		std::string time =dateutil.ConvertCurrentTimetoSqlTime();

		sprintf(buf,"INSERT INTO %s (InstrumentIDDate,InstrumentID,ExchangeID,Direction,Volume,"
				"OpenPriceAverage,StopPrice,Margin,TradingDay,OpenDate,"
				"Highest,Lowest,StopRange,WinPrice, LocalUpdateTime)"
				"VALUES ('%s_%s','%s','%s','%d','%d',"
				"'%.3f','%.3f','%.3f','%s','%s',"
				"'%.3f','%.3f','%.3f','%.3f','%s')",this->getTablename().c_str(),
				data.InstrumentID,data.OpenDate,data.InstrumentID,data.ExchangeID,data.Direction,data.Volume,
				data.OpenPriceAverage,data.StopPrice,data.Margin,data.TradingDay,data.OpenDate,
				data.Highest,data.Lowest,data.StopRange,data.WinPrice,time.c_str());

		str_sql=buf;

		this->Execute(str_sql);
}

void MysqlHold::InsertbyPositionDetail(HoldData data)
{
	//	printf("insert begin\n");

		char buf[2048];
		std::string str_sql;

		DateUtil dateutil;
		std::string time =dateutil.ConvertCurrentTimetoSqlTime();

		sprintf(buf,"INSERT INTO %s (InstrumentIDDate,InstrumentID,ExchangeID,Direction,Volume,"
				"OpenPriceAverage,StopPrice,Margin,TradingDay,OpenDate,"
				"Highest,Lowest,LocalUpdateTime)"
				"VALUES ('%s_%s','%s','%s','%d','%d',"
				"'%.3f','%.3f','%.3f','%s','%s',"
				"'%.3f','%.3f','%s')",this->getTablename().c_str(),
				data.InstrumentID,data.OpenDate,data.InstrumentID,data.ExchangeID,data.Direction,data.Volume,
				data.OpenPriceAverage,data.StopPrice,data.Margin,data.TradingDay,data.OpenDate,
				data.Highest,data.Lowest,time.c_str());

		str_sql=buf;

		this->Execute(str_sql);
}
void MysqlHold::Update(HoldData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
			std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Volume='%d',OpenPriceAverage='%.3f',StopPrice='%.3f',LocalUpdateTime='%s'  "
			"WHERE InstrumentIDDate='%s_%s' AND  Direction='%d'  AND OpenDate='%s'  ",this->getTablename().c_str(),
			data.Volume,data.OpenPriceAverage,data.StopPrice,time.c_str(),data.InstrumentID,data.OpenDate,data.Direction,data.OpenDate);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlHold::UpdateHighest_StopPrice(HoldData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
			std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Highest='%.3f',StopPrice='%.3f',HighProfit='%.2f',LocalUpdateTime='%s'   "
			"WHERE InstrumentIDDate='%s_%s' AND  Direction='%d'  AND OpenDate='%s'  ",this->getTablename().c_str(),
			data.Highest,data.StopPrice,data.HighProfit,time.c_str(),data.InstrumentID,data.OpenDate,data.Direction,data.OpenDate);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);
//	this->Empty();
}

void MysqlHold::UpdateLowest_StopPrice(HoldData data)
{


	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
			std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET Lowest='%.3f',StopPrice='%.3f',HighProfit='%.2f',LocalUpdateTime='%s'  "
			"WHERE InstrumentIDDate='%s_%s' AND  Direction='%d'  AND OpenDate='%s'  ",this->getTablename().c_str(),
			data.Lowest,data.StopPrice,data.HighProfit,time.c_str(),data.InstrumentID,data.OpenDate,data.Direction,data.OpenDate);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);
//	this->Empty();
}

bool  MysqlHold::Exist_HoldPositionbyInstrumentid(const char* pinstrumentid,const char *opendate,int direction)
{

	bool ret=false;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentIDDate='%s_%s' AND Direction='%d' ",
			this->getTablename().c_str(),pinstrumentid,opendate,direction);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		if(this->getRowCount()>0)
		{
			ret=true;
		}


	}
	this->Empty();

	return ret;
}



HoldData  MysqlHold::Find_DatabyInstrumentid(const char* pinstrumentid,const char *opendate,int direction)
{

	HoldData data;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentIDDate='%s_%s' AND Direction='%d' ",
				this->getTablename().c_str(),pinstrumentid,opendate,direction);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

				/* retrieve the row count in the result set */
		if (this->getRowCount()==1)
		{
			this->getFirst();
			 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			 strcpy(data.OpenDate,getValueString("OpenDate").c_str());
			 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
			 data.OpenPriceAverage=getValueDouble("OpenPriceAverage");
			 data.StopPrice=getValueDouble("StopPrice");
			 data.Highest=getValueDouble("Highest");
			 data.Lowest=getValueDouble("Lowest");

			 data.StopRange=getValueDouble("StopRange");
			 data.WinPrice=getValueDouble("WinPrice");

			 data.Margin=getValueDouble("Margin");
			 data.Volume=getValueInt("Volume");
			 data.Direction=getValueInt("Direction");

			 data.HighProfit=getValueDouble("HighProfit");
		}

	}
	this->Empty();
	return data;
}

void  MysqlHold::Delete_DatabyInstrumentid(const char* pinstrumentid,const char *opendate,int direction)
{



	char buf[500];
	std::string str_sql;

	sprintf(buf,"DELETE FROM %s WHERE  InstrumentIDDate='%s_%s' AND Direction='%d' ",
				this->getTablename().c_str(),pinstrumentid,opendate,direction);


	str_sql=buf;
	this->Execute(str_sql);


}

void MysqlHold::Find_AllList(vector<HoldData> &temp)
{
	HoldData data;
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
				 strcpy(data.OpenDate,getValueString("OpenDate").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 data.OpenPriceAverage=getValueDouble("OpenPriceAverage");
				 data.StopPrice=getValueDouble("StopPrice");
				 data.Highest=getValueDouble("Highest");
				 data.Lowest=getValueDouble("Lowest");

				 data.StopRange=getValueDouble("StopRange");
				 data.WinPrice=getValueDouble("WinPrice");

				 data.Margin=getValueDouble("Margin");
				 data.Volume=getValueInt("Volume");
				 data.Direction=getValueInt("Direction");
				 data.HighProfit=getValueDouble("HighProfit");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}


void MysqlHold::Find_AllListOrderbyDate(vector<HoldData> &temp)
{
	HoldData data;
	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT  * FROM %s ORDER BY OpenDate ASC ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{


				 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.OpenDate,getValueString("OpenDate").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 data.OpenPriceAverage=getValueDouble("OpenPriceAverage");
				 data.StopPrice=getValueDouble("StopPrice");
				 data.Highest=getValueDouble("Highest");
				 data.Lowest=getValueDouble("Lowest");

				 data.StopRange=getValueDouble("StopRange");
				 data.WinPrice=getValueDouble("WinPrice");

				 data.Margin=getValueDouble("Margin");
				 data.Volume=getValueInt("Volume");
				 data.Direction=getValueInt("Direction");
				 data.HighProfit=getValueDouble("HighProfit");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}


void MysqlHold::Find_AllTradingDayList(vector<HoldData> &temp,const char * tradingday)
{
	HoldData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE OpenDate='%s'",this->getTablename().c_str(),tradingday);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{


				 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 strcpy(data.OpenDate,getValueString("OpenDate").c_str());
				 strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
				 data.OpenPriceAverage=getValueDouble("OpenPriceAverage");
				 data.StopPrice=getValueDouble("StopPrice");
				 data.Highest=getValueDouble("Highest");
				 data.Lowest=getValueDouble("Lowest");

				 data.StopRange=getValueDouble("StopRange");
				 data.WinPrice=getValueDouble("WinPrice");

				 data.Margin=getValueDouble("Margin");
				 data.Volume=getValueInt("Volume");
				 data.Direction=getValueInt("Direction");
				 data.HighProfit=getValueDouble("HighProfit");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}



int MysqlHold::CollectPositions_byInstrumentID(const char * pinstrumentid,int direction)
{


	int position=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE InstrumentID='%s' AND Direction='%d'",this->getTablename().c_str(),pinstrumentid,direction);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)
				{

				 position+=getValueInt("Volume");
				 this->getNext();
				}
	}
	this->Empty();
	return position;

}

int MysqlHold::Count_AllListsbySection(const char* section)
{
	int ret=0;

	HoldData data;
	char buf[2048];
	std::string str_sql;

	CodeUtil codeutil;


	sprintf(buf,"SELECT  * FROM %s ORDER BY OpenDate ASC ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				 strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				 char productid[31];
				strcpy(productid,codeutil.GetProductID(data.InstrumentID).c_str());

				std::string sec =codeutil.GetSectionByProductid(productid);
				if(strcmp(sec.c_str(),section)==0)
				{
					ret++;
//					printf("Count_AllListsbySection   相同 %s   %s \n",sec.c_str(),data.InstrumentID);
				}

				  this->getNext();
			}
	}
	this->Empty();

	return ret;

}



int  MysqlHold::CountbyAll()
{
	int count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(InstrumentIDDate) FROM %s ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
				count = getValueInt("count(InstrumentIDDate)");
		}
	}

	this->Empty();

	return count;
}
