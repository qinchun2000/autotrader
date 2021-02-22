#include "mysqlhisma.hpp"



MysqlHisMA::MysqlHisMA()
{
	std::string database="hisma";

	if (!IsAliveConn())
	{
		if(!Connect())
			{

				return;
			}
	}

	this->Set_Database(database);

}


void MysqlHisMA::CreateTable(const char *tablename)
{
		char buf[2048];
		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
						"Time DATETIME NOT NULL PRIMARY KEY, "
						"MA50 DOUBLE(15,3),"
						"MA20 DOUBLE(15,3),"
						"MA10 DOUBLE(15,3),"
						"MA50Angel DOUBLE(15,3),"
						"TrendStatus CHAR(10),"
						"LongCount INT(11),"
						"ShortCount INT(11),"
						"LongStartTime DATETIME,"
						"ShortStartTime DATETIME) default character set utf8 ",tablename);

		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}


void MysqlHisMA::Insert(const char *pinstrumentid,HisMAData data)
{


	char buf[1024];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s(Time, MA50,MA20,MA10) "
				"VALUES ('%s', '%.3f','%.3f','%.3f')",
				pinstrumentid,data.Time,data.MA50,data.MA20,data.MA10);

	str_sql=buf;
	this->Execute(str_sql);

}


std::string MysqlHisMA::GetLastDateTime(const char *tablename)
{

	std::string  info;
	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT Time FROM %s ORDER BY Time desc limit 1 ",tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();
					 info =  getValueString("Time");
				}
		}
	this->Empty();
	  return info;

}


bool MysqlHisMA::Exist_DateTime(const char *tablename,std::string datetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s WHERE Time='%s' ",tablename,datetime.c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		if(this->getRowCount()==1)
		{
			this->getFirst();
			 if(strcmp(getValueString("Time").c_str(),datetime.c_str())==0)
			 {
				 ret=true;
			 }

		}


	}
	this->Empty();
	return ret;
}


HisMAData  MysqlHisMA::Find_LastHisMADatabyOffset(const char *tablename,int offset)
{

	HisMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time desc limit 1 OFFSET %d",tablename,offset);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.MA50=getValueDouble("MA50");
					 data.MA20=getValueDouble("MA20");
					 data.MA10 =getValueDouble("MA10");
				}
		}
	this->Empty();
	  return data;

}

HisMAData  MysqlHisMA::Find_HisMADatabyDateTime_Offset(const char *tablename,const char* datetime,int offset)
{

	HisMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time desc limit 1 OFFSET %d",tablename,datetime,offset);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.MA50=getValueDouble("MA50");
					 data.MA20=getValueDouble("MA20");
					 data.MA10 =getValueDouble("MA10");
				}
		}
	this->Empty();
	  return data;

}

