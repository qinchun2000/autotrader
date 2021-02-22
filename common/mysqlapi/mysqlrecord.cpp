#include "mysqlrecord.hpp"

MysqlRecord::MysqlRecord()
{
	std::string database="position";
	std::string tablename="record";

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


void MysqlRecord::CreateTable()
{

	if(!this->Exist_Table(this->getTablename().c_str()))
	{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
							"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
							"InstrumentID CHAR(10),"
							"Direction INT(11),"
							"ExchangeID CHAR(21),"
							"OpenSignal CHAR(30),"
							"CloseSignal CHAR(30),"
							"OpenPrice DOUBLE,"
							"ClosePrice DOUBLE,"
							"Volume INT(11),"
							"Unit INT(11),"
							"Margin DOUBLE,"
							"Profit DOUBLE,"
							"Commission DOUBLE,"
							"OpenDateTime DATETIME,"
							"CloseDateTime DATETIME,"
							"WinLoseStatus CHAR(10),"
							"Asset DOUBLE,"
							"LocalUpdateTime DATETIME) default character set utf8 ",this->getTablename().c_str());
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}

void MysqlRecord::Insert(RecordData data)
{

   std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"INSERT INTO %s (InstrumentID,Direction,ExchangeID,OpenSignal,CloseSignal,"
			"OpenPrice,ClosePrice,Volume,Margin,Profit, "
			"Commission,OpenDateTime,CloseDateTime,WinLoseStatus, Asset,"
			"Unit,LocalUpdateTime)"
			"VALUES ('%s','%d', '%s','%s','%s',"
			"'%.3f','%.3f','%d','%.3f','%.3f',"
			"'%.3f','%s','%s','%s','%.3f',"
			"'%d','%s')",tablename.c_str(),
			data.InstrumentID,data.Direction,data.ExchangeID,data.OpenSignal,data.CloseSignal,
			data.OpenPrice,data.ClosePrice,data.Volume,data.Margin,data.Profit,
			data.Commission,data.OpenDateTime,data.CloseDateTime,data.WinLoseStatus,data.Asset,
			data.Unit,time.c_str());

	str_sql=buf;

	this->Execute(str_sql);


}


bool  MysqlRecord::Exist_RecordByOpenDateTime(const char* pinstrumentid,const char *opendatetime)
{
	std::string tablename = this->getTablename();
	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND OpenDateTime='%s'  ",tablename.c_str(),pinstrumentid,opendatetime);

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



void MysqlRecord::Find_AllLists(vector<RecordData> &temp)
{


	RecordData data;


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
			 	strcpy(data.InstrumentID, getValueString("InstrumentID").c_str());
			 	data.Direction = getValueInt("Direction");
			 	strcpy(data.ExchangeID,getValueString("ExchangeID").c_str());
			 	strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
			 	strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());

			 	 data.OpenPrice = getValueDouble("OpenPrice");
			 	 data.ClosePrice = getValueDouble("ClosePrice");
			 	 data.Volume = getValueInt("Volume");
			 	 data.Unit = getValueInt("Unit");
			 	data.Margin = getValueDouble("Margin");

			 	data.Profit = getValueDouble("Profit");
			 	data.Commission = getValueDouble("Commission");
				strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
				strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());
				strcpy(data.WinLoseStatus,getValueString("WinLoseStatus").c_str());

				data.Asset = getValueDouble("Asset");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}




