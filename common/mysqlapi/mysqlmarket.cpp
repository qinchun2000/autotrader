#include "mysqlmarket.hpp"


MysqlMarket::MysqlMarket()
{

	std::string database="market";
	std::string tablename="allmarket";

	if (IsAliveConn())
		{
	//		printf("MysqlExchange IsAliveConn !!!! %s \n",database.c_str());

		}else
		{

	//		printf("MysqlExchange IsAliveConn not connect !!!! %s \n",database.c_str());
			if(!Connect())
			{

				return;
			}

		}

		this->Set_Database(database);
		this->Set_Table(tablename);


//	printf("connet to database [%s]OK\n",_DATABASE.c_str());

}

void MysqlMarket::CreateTable()
{

	if(!this->Exist_Table(this->getTablename().c_str()))
	{
		char buf[2048];



		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
							"MinuteTime DATETIME NOT NULL PRIMARY KEY, "
							"MaxUpRate DOUBLE,"
							"MaxUpInstrumentID CHAR(20),"
							"MinDownRate DOUBLE,"
							"MinDownInstrumentID CHAR(20)) default character set utf8 ",this->getTablename().c_str());

		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}


void MysqlMarket::Insert(MarketMinuteData data)
{
	printf("MysqlMarket Insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();


	sprintf(buf,"INSERT INTO %s (MinuteTime,MaxUpRate,MaxUpInstrumentID,MinDownRate,MinDownInstrumentID) "
			"VALUES ('%s','%.3f', '%s','%.3f','%s') ",this->getTablename().c_str(),
			data.MinuteTime,data.MaxUpRate,data.MaxUpInstrumentID,data.MinDownRate,data.MinDownInstrumentID);

	str_sql=buf;

	this->Execute(str_sql);

	printf("MysqlMarket Insert end \n");
}
