#include "mysqlminutetime.hpp"

MysqlMinuteTime::MysqlMinuteTime()
{
	std::string database="trademinute";

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


void MysqlMinuteTime::CreateTable(const char* tablename)
{


	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"MinuteTime DATETIME NOT NULL PRIMARY KEY, "
						"StartMinuteTime DATETIME,"
						"EndMinuteTime DATETIME) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);



}



void MysqlMinuteTime::Insert(const char* tablename,MinuteTimeData data)
{


	char buf[2048];
	std::string str_sql;



//	sprintf(buf,"INSERT INTO %s (Time,StartTime,EndTime) VALUES ('%s','%s','%s' )",tablename,
//			data.Time,data.StartTime,data.EndTime);

	sprintf(buf,"INSERT INTO %s (MinuteTime) VALUES ('%s' )",tablename,
				data.MinuteTime);

	str_sql=buf;

	this->Execute(str_sql);


}




bool  MysqlMinuteTime::Exist_MinuteTime(const char* tablename,const char *minutetime)
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





MinuteTimeData  MysqlMinuteTime::Find_LastMinuteTime(const char* tablename)
{

	MinuteTimeData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s  ORDER BY MinuteTime DESC LIMIT 1  ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();


			strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			strcpy(data.StartMinuteTime,getValueString("StartMinuteTime").c_str());
			strcpy(data.EndMinuteTime,getValueString("EndMinuteTime").c_str());
		}


	}

	this->Empty();

	return data;
}

void MysqlMinuteTime::Find_AllLists(vector<MinuteTimeData> &temp,const char* tablename)
{


	MinuteTimeData data;


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
				strcpy(data.StartMinuteTime,getValueString("StartMinuteTime").c_str());
				strcpy(data.EndMinuteTime,getValueString("EndMinuteTime").c_str());


				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

int  MysqlMinuteTime::CountbyAll(const char* tablename)
{

	int count;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(MinuteTime) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				count = getValueInt("count(MinuteTime)");

		}


	}

	this->Empty();

	return count;
}


