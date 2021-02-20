#include "mysqlcycle.hpp"



MysqlCycle::MysqlCycle()
{

	std::string database="cycle";


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

void MysqlCycle::CreateTable(const char* tablename)
{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
							"Time DATETIME NOT NULL PRIMARY KEY, "
							"High20 DOUBLE(15,3),"
							"HighCount INT(11), "
							"HighDate DATETIME, "
							"Low20 DOUBLE(15,3), "
							"LowCount INT(11), "
							"LowDate DATETIME) default character set utf8",tablename);
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}

void MysqlCycle::Insert(const char* tablename,CycleData data)
{

	char buf[2048];
	std::string str_sql;

//	DateUtil dateutil;
//	std::string time =dateutil.ConvertCurrentTimetoSqlTime();


	sprintf(buf,"INSERT INTO %s(Time, High20,HighCount,HighDate,Low20,LowCount,LowDate) "
				"VALUES ('%s', '%.3f','%d','%s','%.3f','%d','%s')",
				tablename,data.Time,data.High20,data.HighCount,data.HighDate,data.Low20,data.LowCount,data.LowDate);
	str_sql=buf;

	this->Execute(str_sql);


}



std::string MysqlCycle::GetLastDateTime(const char *tablename)
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





CycleData  MysqlCycle::GetHighestbyHighCount(const char *tablename,const char* datetime,int number)
{

	CycleData data;
	data.High20=0;

	vector<CycleData>  list;
	int maxcount;

	this->Find_HighListsByTime(list,tablename,datetime,number);
	int i=0;
	for (auto &item:list)
	{
		if(i==0)
		{
			maxcount=item.HighCount;
			data.High20=item.High20;
			data.HighCount = item.HighCount;
			strcpy(data.HighDate,item.HighDate);
		}

		if (item.High20 > data.High20)
		{
			maxcount=item.HighCount;

			data.High20=item.High20;
			data.HighCount = item.HighCount;
			strcpy(data.HighDate,item.HighDate);

		}
		i++;
	}

	vector<CycleData>().swap(list);

	  return data;

}


CycleData MysqlCycle::GetLowestbyLowCount(const char *tablename,const char * datetime,int number)
{

	CycleData data;
	data.Low20=0;



	vector<CycleData>  list;


	this->Find_LowListsByTime(list,tablename,datetime,number);

//		printf("getlowest ---> count %ld \n",list.size());
	int i=0;
	for (auto &item:list)
	{


		if(i==0)
		{

			data.Low20=item.Low20;
			data.LowCount = item.LowCount;
			strcpy(data.LowDate,item.LowDate);
		}

		if (item.Low20 < data.Low20)
		{
//				printf("-----> lowcount %d  low20: %.3f \n",item.LowCount,item.Low20);


			data.Low20=item.Low20;
			data.LowCount = item.LowCount;
			strcpy(data.LowDate,item.LowDate);
		}

		i++;
	}

	vector<CycleData>().swap(list);

	  return data;

}

bool MysqlCycle::Exist_DateTime(const char *tablename,std::string datetime)
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



CycleData  MysqlCycle::Find_DatabyDateTime(const char *tablename,std::string datetime)
{

	CycleData data;
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
					 strcpy(data.HighDate,getValueString("HighDate").c_str());
					 strcpy(data.LowDate,getValueString("LowDate").c_str());
					 data.High20=getValueDouble("High20");
					 data.Low20=getValueDouble("Low20");
					 data.HighCount =getValueInt("HighCount");
					 data.LowCount =getValueInt("LowCount");

				}
		}
	this->Empty();
	  return data;

}



void MysqlCycle::Find_ListsByTime(vector<CycleData> &temp,const char *tablename,const char * datetime,int number)
{

	CycleData data;


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
			 strcpy(data.HighDate,getValueString("HighDate").c_str());
			 strcpy(data.LowDate,getValueString("LowDate").c_str());
			 data.High20=getValueDouble("High20");
			 data.Low20=getValueDouble("Low20");
			 data.HighCount =getValueInt("HighCount");
			 data.LowCount =getValueInt("LowCount");
				temp.push_back(data);
				this->getNext();
			}
	}
	this->Empty();

}



void MysqlCycle::Find_LowListsByTime(vector<CycleData> &temp,const char *tablename,const char * datetime,int number)
{

	CycleData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time Desc Limit %d",tablename,datetime,number);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		int N;
		double low=-1.0;
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
			 strcpy(data.Time,getValueString("Time").c_str());
			 strcpy(data.HighDate,getValueString("HighDate").c_str());
			 strcpy(data.LowDate,getValueString("LowDate").c_str());
			 data.High20=getValueDouble("High20");
			 data.Low20=getValueDouble("Low20");
			 data.HighCount =getValueInt("HighCount");
			 data.LowCount =getValueInt("LowCount");

			 N = data.LowCount;

			 if (N>=2 && data.Low20 != low)
			 {
				 low = data.Low20;
				 temp.push_back(data);
			 }

				this->getNext();
			}
	}
	this->Empty();

}



void MysqlCycle::Find_HighListsByTime(vector<CycleData> &temp,const char *tablename,const char * datetime,int number)
{

	CycleData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time Desc Limit %d",tablename,datetime,number);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		int N;
		double high=0.0;
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
			 strcpy(data.Time,getValueString("Time").c_str());
			 strcpy(data.HighDate,getValueString("HighDate").c_str());
			 strcpy(data.LowDate,getValueString("LowDate").c_str());
			 data.High20=getValueDouble("High20");
			 data.Low20=getValueDouble("Low20");
			 data.HighCount =getValueInt("HighCount");
			 data.LowCount =getValueInt("LowCount");

			 N = data.HighCount;

			 if (N>=2 && data.High20 != high)
			 {
				 high = data.High20;
				 temp.push_back(data);
			 }

				this->getNext();
			}
	}
	this->Empty();

}
int MysqlCycle::Check_Highcount(const char* tablename,const char * date,double high20)
{
	int highcount=1;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC  ",tablename,date);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */

		for (int i=0; i<this->getRowCount();i++)
		{
			  std::string str_date;
			  str_date= getValueString("Time");
			  double last_high20  = getValueDouble("High20");

			  if (last_high20 == high20)
			  {
				  highcount++;

			  }
			  else
			  {
//									  printf("%s---------->last_high: %f    high20: %f  \n",str_date.c_str(),last_high20,high20);
				  break;
			  }
			  this->getNext();
		}
	}
	this->Empty();
	return highcount;


//					printf("MongoCycle Check_Highcount end! ...\n");
}

int MysqlCycle::Check_Lowcount(const char* tablename,const char * date,double Low20)
{
	int lowcount=1;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC  ",tablename,date);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */

			for (int i=0; i<this->getRowCount();i++)
			{

					  std::string str_date;
					  str_date= getValueString("Time");
					  double last_low20  = getValueDouble("Low20");

					  if (last_low20 == Low20)
					  {
						  lowcount++;

					  }
					  else
					  {
			//										  printf("%s---------->last_high: %f    high20: %f  \n",str_date.c_str(),last_low20,Low20);
						  break;
					  }
					  this->getNext();

				}
		}


	this->Empty();
	return lowcount;


//						printf("MongoCycle Check_Lowcount end! ...\n");
}
