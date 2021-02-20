#include "mysqlhisatr.hpp"



MysqlHisATR::MysqlHisATR()
{

	std::string database="hisatr";


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


}

void MysqlHisATR::CreateTable(const char* tablename)
{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
							"Time DATETIME NOT NULL PRIMARY KEY, "
							"TR DOUBLE,"
							"ATR DOUBLE) default character set utf8",tablename);
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}

void MysqlHisATR::Insert(const char* tablename,ATRData data)
{

	char buf[2048];
	std::string str_sql;

//	DateUtil dateutil;
//	std::string time =dateutil.ConvertCurrentTimetoSqlTime();


	sprintf(buf,"INSERT INTO %s (Time, TR, ATR) VALUES ('%s', '%.3f','%.3f' )",
				tablename,data.Time,data.TR,data.ATR);
	str_sql=buf;

	this->Execute(str_sql);


}



std::string MysqlHisATR::GetLastDateTime(const char *tablename)
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






bool MysqlHisATR::Exist_DateTime(const char *tablename,std::string datetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s WHERE Time='%s' ",tablename,datetime.c_str());

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



ATRData  MysqlHisATR::Find_DatabyDateTime(const char *tablename,std::string datetime)
{

	ATRData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time='%s' ",tablename,datetime.c_str());
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();

					 strcpy(data.Time,getValueString("Time").c_str());

					 data.TR=getValueDouble("TR");
					 data.ATR=getValueDouble("ATR");


				}
		}
	this->Empty();
	  return data;

}

ATRData  MysqlHisATR::FindPreDatabyDateTime(const char *tablename,const char* datetime)
{

	ATRData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time<'%s' order by Time Desc Limit 1 ",tablename,datetime);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();

					 strcpy(data.Time,getValueString("Time").c_str());

					 data.TR=getValueDouble("TR");
					 data.ATR=getValueDouble("ATR");


				}
		}
	this->Empty();
	  return data;

}

void MysqlHisATR::Find_ListsByTime(vector<ATRData> &temp,const char *tablename,const char * datetime,int number)
{

	ATRData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time Desc Limit %d",tablename,datetime,number);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
			 strcpy(data.Time,getValueString("Time").c_str());

			 data.TR=getValueDouble("TR");
			 data.ATR=getValueDouble("ATR");
			temp.push_back(data);
			this->getNext();
			}
	}
	this->Empty();

}




int MysqlHisATR::Count_ListsByTime_N(const char *tablename,const char * datetime,int number)
{



	int count=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time Desc Limit %d",tablename,datetime,number);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				ATRData data;
			 strcpy(data.Time,getValueString("Time").c_str());

			 data.TR=getValueDouble("TR");
			 data.ATR=getValueDouble("ATR");

			 if (data.TR > data.ATR)
			 {
				 count++;
			 }
			this->getNext();
			}
	}
	this->Empty();
	return count;

}


int MysqlHisATR::Count2x_ListsByTime_N(const char *tablename,const char * datetime,int number)
{



	int count=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time Desc Limit %d",tablename,datetime,number);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				ATRData data;
			 strcpy(data.Time,getValueString("Time").c_str());

			 data.TR=getValueDouble("TR");
			 data.ATR=getValueDouble("ATR");

			 if (data.TR > data.ATR*2 )
			 {
				 count++;
			 }
			this->getNext();
			}
	}
	this->Empty();
	return count;

}


double MysqlHisATR::Indicator_ListsByTime_N(const char *tablename,const char * datetime,int number)
{


	double total_tr=0;
	double total_atr=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time Desc Limit %d",tablename,datetime,number);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				ATRData data;
			 strcpy(data.Time,getValueString("Time").c_str());

			 data.TR=getValueDouble("TR");
			 data.ATR=getValueDouble("ATR");

			 total_tr += data.TR;
			 total_atr += data.ATR;
			this->getNext();
			}
	}
	this->Empty();
	return total_tr/total_atr;

}
