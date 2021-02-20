#include "mysqlhissection.hpp"

MysqlHisSection::MysqlHisSection()
{
	std::string database="hissection";

	if(this->IsCloseConn())
	{

		if(!Connect())
		{

			return;
		}

	}
	else
	{
		if (IsAliveConn())
		{

		}
		else
		{

			if(!Connect())
			{

				return;
			}
		}
	}


	this->Set_Database(database);



}


void MysqlHisSection::CreateTable(const char* tablename)
{


	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"MinuteTime DATETIME NOT NULL PRIMARY KEY, "
						"UpCount INT,"
						"DownCount INT,"
						"ZeroCount INT,"
						"MaxInstrumentID CHAR(10),"
						"MinInstrumentID CHAR(10),"
						"Maxrate DOUBLE,"
						"Minrate DOUBLE,"
						"AverageUpDownrate DOUBLE,"
						"CountUprate DOUBLE,"
						"CountDownrate DOUBLE) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);



}



void MysqlHisSection::Insert(const char* tablename,HisSectionData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"INSERT INTO %s (MinuteTime,UpCount,DownCount,ZeroCount,CountUprate,"
			"CountDownrate,AverageUpDownrate,Maxrate,MaxInstrumentID,Minrate,"
			"MinInstrumentID) "
			"VALUES ('%s','%d','%d','%d','%.2f',"
			"'%.2f' ,'%.4f','%.4f','%s','%.4f','%s')",tablename,
				data.MinuteTime,data.UpCount,data.DownCount,data.ZeroCount,data.CountUprate,
				data.CountDownrate,data.AverageUpDownrate,data.Maxrate,data.MaxInstrumentID,data.Minrate,data.MinInstrumentID);

	str_sql=buf;

	this->Execute(str_sql);


}




bool  MysqlHisSection::Exist_DatabyMinuteTime(const char* tablename,const char *minutetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  MinuteTime='%s'  ",tablename,minutetime);

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





HisSectionData  MysqlHisSection::Find_DatabyMinuteTime(const char* tablename,const char* minutetime)
{

	HisSectionData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE MinuteTime='%s'  ",tablename,minutetime);

//	printf("SELECT * FROM %s WHERE MinuteTime='%s'  \n ",tablename,minutetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();


			strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			strcpy(data.MaxInstrumentID,getValueString("MaxInstrumentID").c_str());
			strcpy(data.MinInstrumentID,getValueString("MinInstrumentID").c_str());
			data.UpCount=getValueInt("UpCount");
			data.DownCount=getValueInt("DownCount");
			data.ZeroCount=getValueInt("ZeroCount");
			data.CountUprate=getValueDouble("CountUprate");
			data.AverageUpDownrate=getValueDouble("AverageUpDownrate");
			data.Maxrate=getValueDouble("Maxrate");
			data.Minrate =getValueDouble("Minrate");
//			printf("     ---> %.2f    \n",data.CountUprate);
			data.CountDownrate=getValueDouble("CountDownrate");
		}


	}

	this->Empty();

	return data;
}

void MysqlHisSection::Find_AllLists(vector<HisSectionData> &temp,const char* tablename)
{


	HisSectionData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ", tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				strcpy(data.MaxInstrumentID,getValueString("MaxInstrumentID").c_str());
				strcpy(data.MinInstrumentID,getValueString("MinInstrumentID").c_str());
				data.UpCount=getValueInt("UpCount");
				data.DownCount=getValueInt("DownCount");
				data.ZeroCount=getValueInt("ZeroCount");
				data.CountUprate=getValueDouble("CountUprate");
				data.AverageUpDownrate=getValueDouble("AverageUpDownrate");
				data.Maxrate=getValueDouble("Maxrate");
				data.Minrate =getValueDouble("Minrate");
	//			printf("     ---> %.2f    \n",data.CountUprate);
				data.CountDownrate=getValueDouble("CountDownrate");


				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}




