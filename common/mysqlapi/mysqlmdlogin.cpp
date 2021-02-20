#include "mysqlmdlogin.hpp"



MysqlMdLogin::MysqlMdLogin()
{


	std::string database="baseinfo";
	std::string tablename="mdlogin";

	if (IsAliveConn())
		{
	//		printf("MysqlMdLogin IsAliveConn !!!! %s \n",database.c_str());

		}else
		{

	//		printf("MysqlMdLogin IsAliveConn not connect !!!! %s \n",database.c_str());
			if(!Connect())
			{

				return;
			}

		}

	this->Set_Database(database);
	this->Set_Table(tablename);
//	printf("connet to database [%s]OK\n",_DATABASE.c_str());

}

void MysqlMdLogin::CreateTable()
{

	if(!this->Exist_Table(this->getTablename().c_str()))
	{
		char buf[2048];
		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
//					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
				"TradingDateTime DATETIME NOT NULL  PRIMARY KEY,"
				"TradingDay CHAR(10),"
				"FrontID INT(11),"
				"SessionID INT(11),"
				"MaxOrderRef CHAR(13),"
				"LoginTime CHAR(9),"

				"DCETime CHAR(9),"
				"CZCETime CHAR(9),"
				"SHFETime CHAR(9),"
				"INETime CHAR(9),"
				"DifSeconds INT(11),"

				"LocalCreateTime DATETIME,"
				"LocalUpdatetime DATETIME) default character set utf8 ",this->getTablename().c_str()) ;


		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}

bool MysqlMdLogin::Exist_DateTime(const char* datetime)
{
//	printf("Exist_MinuteTime begin\n");
	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT TradingDateTime FROM %s WHERE TradingDateTime='%s' ",this->getTablename().c_str(),datetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			ret=true;
		}


	}

	/* retrieve the row count in the result set */

this->Empty();
	    return ret;
}

void MysqlMdLogin::Insert(TradingDayData data)
{
	printf("MysqlMdLogin Insert begin\n");
   std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();


	sprintf(buf,"INSERT INTO %s (TradingDateTime,TradingDay,FrontID,SessionID,MaxOrderRef,LoginTime,"
			"DCETime,CZCETime,SHFETime,INETime,DifSeconds,"
			"LocalCreateTime,LocalUpdatetime) "
			"VALUES ('%s','%s', '%d','%d','%s','%s',"
			"'%s','%s','%s','%s','%d',"
			"'%s','%s') ",tablename.c_str(),
			data.TradingDateTime,data.TradingDay,data.FrontID,data.SessionID,data.MaxOrderRef,data.LoginTime,
			data.DCETime,data.CZCETime,data.SHFETime,data.INETime,data.DifSeconds,
			time.c_str(),time.c_str());

	str_sql=buf;

	this->Execute(str_sql);

	printf("MysqlMdLogin Insert end \n");
}


TradingDayData  MysqlMdLogin::Find_LastTradingDay()
{
//	printf("Find_LastTradingDay begin\n");


		TradingDayData info;
	    char buf[2048];
		std::string str_sql;
		sprintf(buf,"SELECT * FROM %s  ORDER BY TradingDateTime DESC LIMIT 1 ",this->getTablename().c_str());

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
		{

			this->getFirst();
			/* retrieve the row count in the result set */
			if (this->getRowCount()==1)
			{

				 strcpy(info.TradingDateTime,getValueString("TradingDateTime").c_str());
				 strcpy(info.TradingDay,getValueString("TradingDay").c_str());

				 info.FrontID = getValueInt("FrontID");
				 info.SessionID = getValueInt("SessionID");
				 strcpy(info.MaxOrderRef,getValueString("MaxOrderRef").c_str());
				 strcpy(info.LoginTime,getValueString("LoginTime").c_str());

				 strcpy(info.CZCETime,getValueString("CZCETime").c_str());
				 strcpy(info.DCETime,getValueString("DCETime").c_str());
				 strcpy(info.SHFETime,getValueString("SHFETime").c_str());
				 strcpy(info.INETime,getValueString("INETime").c_str());
				 info.DifSeconds = getValueInt("DifSeconds");

				 strcpy(info.LocalCreateTime,getValueString("LocalCreateTime").c_str());
				 strcpy(info.LocalUpdateTime,getValueString("LocalUpdateTime").c_str());

			}
		}
		this->Empty();
		  return info;

}
