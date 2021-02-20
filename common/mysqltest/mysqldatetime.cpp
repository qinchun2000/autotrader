#include "mysqldatetime.hpp"

MysqlDateTime::MysqlDateTime()
{
	std::string database="tradedate";

	if(this->IsCloseConn())	{

		if(!Connect())		{
			return;
		}

	}
	else	{
		if (IsAliveConn())		{

		}
		else		{

			if(!Connect())		{

				return;
			}
		}
	}


	this->Set_Database(database);



}


void MysqlDateTime::CreateTable(const char* tablename)
{


	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"Time DATETIME NOT NULL PRIMARY KEY, "
						"StartTime DATETIME,"
						"EndTime DATETIME) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);



}



void MysqlDateTime::Insert(const char* tablename,DateTime data)
{


	char buf[2048];
	std::string str_sql;



//	sprintf(buf,"INSERT INTO %s (Time,StartTime,EndTime) VALUES ('%s','%s','%s' )",tablename,
//			data.Time,data.StartTime,data.EndTime);

	sprintf(buf,"INSERT INTO %s (Time) VALUES ('%s' )",tablename,
				data.Time);

	str_sql=buf;

	this->Execute(str_sql);


}




bool  MysqlDateTime::Exist_DateTime(const char* tablename,const char *datetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Time='%s'  ",tablename,datetime);

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





DateTime  MysqlDateTime::Find_LastDateTime(const char* tablename)
{

	DateTime data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s  ORDER BY Time DESC LIMIT 1  ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();


			strcpy(data.Time,getValueString("Time").c_str());
			strcpy(data.StartTime,getValueString("StartTime").c_str());
			strcpy(data.EndTime,getValueString("EndTime").c_str());
		}


	}

	this->Empty();

	return data;
}

DateTime  MysqlDateTime::Find_PreDateTime(const char* tablename,const char* datetime)
{

	DateTime data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s  Where Time <'%s' ORDER BY Time DESC LIMIT 1  ",tablename,datetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();


			strcpy(data.Time,getValueString("Time").c_str());
			strcpy(data.StartTime,getValueString("StartTime").c_str());
			strcpy(data.EndTime,getValueString("EndTime").c_str());
		}


	}

	this->Empty();

	return data;
}

void MysqlDateTime::Find_AllLists(vector<DateTime> &temp,const char* tablename)
{


	DateTime data;


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
				strcpy(data.Time,getValueString("Time").c_str());
				strcpy(data.StartTime,getValueString("StartTime").c_str());
				strcpy(data.EndTime,getValueString("EndTime").c_str());


				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}



void MysqlDateTime::Find_AllListsbyStartSqlTime(vector<DateTime> &temp,const char* tablename,const char* start_sqltime)
{
	DateTime data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE Time>'%s' ", tablename,start_sqltime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				strcpy(data.Time,getValueString("Time").c_str());
				strcpy(data.StartTime,getValueString("StartTime").c_str());
				strcpy(data.EndTime,getValueString("EndTime").c_str());


				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}


