#include "mysqlma.hpp"



MysqlMA::MysqlMA()
{
	std::string database="ma";

	if (!IsAliveConn())
	{
		if(!Connect())
			{

				return;
			}
	}

	this->Set_Database(database);

}


void MysqlMA::CreateTable(const char *tablename)
{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
							"Time DATETIME NOT NULL PRIMARY KEY, "
							"MA59 DOUBLE(15,3),"
							"MA49 DOUBLE(15,3),"
							"MA19 DOUBLE(15,3),"
							"MA17 DOUBLE(15,3),"
							"MA9 DOUBLE(15,3),"
							"MA8 DOUBLE(15,3),"
							"MA3 DOUBLE(15,3)) default character set utf8 ",tablename);
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}


void MysqlMA::Insert(const char *pinstrumentid,MAData data)
{
//	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s(Time, MA59,MA49,MA19,MA9,MA17,MA8,MA3) "
			"VALUES ('%s', '%.3f','%.3f','%.3f','%.3f','%.3f','%.3f','%.3f')",
			pinstrumentid,data.Time,data.MA59,data.MA49,data.MA19,data.MA9,data.MA17,data.MA8,data.MA3);

	str_sql=buf;
	this->Execute(str_sql);
//	conn->commit();
}


std::string MysqlMA::GetLastDateTime(const char *tablename)
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




bool MysqlMA::Exist_DateTime(const char *tablename,std::string datetime)
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


MAData  MysqlMA::Find_LastMAData(const char *tablename)
{

	MAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time desc limit 1 ",tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());
					 data.MA59=getValueDouble("MA59");
					 data.MA49=getValueDouble("MA49");
					 data.MA19 =getValueDouble("MA19");
					 data.MA9 =getValueDouble("MA9");
					 data.MA17 =getValueDouble("MA17");
					 data.MA8 =getValueDouble("MA8");
					 data.MA3 =getValueDouble("MA3");
				}
		}
	this->Empty();
	  return data;

}


MAData  MysqlMA::Find_MADatabyTime(const char *tablename,std::string datetime)
{

	MAData data;
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
					 data.MA59=getValueDouble("MA59");
					 data.MA49=getValueDouble("MA49");
					 data.MA19 =getValueDouble("MA19");
					 data.MA9 =getValueDouble("MA9");
					 data.MA17 =getValueDouble("MA17");
					 data.MA8 =getValueDouble("MA8");
					 data.MA3 =getValueDouble("MA3");

				}
		}
	this->Empty();
	  return data;

}

//
//MAData MysqlMA::Check_DatabyDate(const char* pinstrument,const char* date)
//{
//	MAData madata;
//	CreateTable(pinstrument);
//	std::string ma_lastdate =GetLastDateTime(pinstrument);
//	if (strcmp(date,ma_lastdate.c_str())==0)
//	{
//		madata=Find_MADatabyTime(pinstrument,date);
//	}
//	else
//	{
////		printf("lastdate 不同 ------>tradingday: %s    ma_date:%s    depth: %s \n  ",
////					tradingday.c_str(),ma_lastdate.c_str(),depthmarket.TradingDay);
//		MysqlDayLine mysqldayline;
//		madata=mysqldayline.Collect_TotalMAData(pinstrument,date);
//		Insert(pinstrument,madata);
//	}
//return madata;
//}
