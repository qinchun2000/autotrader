#include "mysqlaveprice.hpp"



MysqlAvePrice::MysqlAvePrice()
{

	std::string database="averageprice";

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

void MysqlAvePrice::CreateTable(const char* tablename)
{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
							"Time DATETIME NOT NULL PRIMARY KEY, "
							"OpenInterest INT(11), "
							"StartOpenInterest INT(11), "
							"AveragePrice DOUBLE(15,3))",tablename);

		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}


void MysqlAvePrice::Insert(const char *pinstrumentid,SqlAveragePriceData data)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s (Time, OpenInterest,StartOpenInterest,AveragePrice) "
				"VALUES ('%s', '%d','%d','%.3f')",pinstrumentid,
				data.Time,data.OpenInterest,data.StartOpenInterest,data.AveragePrice);

	str_sql=buf;

	this->Execute(str_sql);


}



std::string MysqlAvePrice::GetLastDateTime(const char *tablename)
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


bool MysqlAvePrice::Exist_DateTime(const char *tablename,std::string datetime)
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


SqlAveragePriceData  MysqlAvePrice::Find_DateTime(const char *tablename,std::string datetime)
{

	SqlAveragePriceData data;
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
					 data.AveragePrice=getValueDouble("AveragePrice");
					 data.StartOpenInterest =getValueInt("StartOpenInterest");
					 data.OpenInterest =getValueInt("OpenInterest");

				}
		}
	this->Empty();
	  return data;

}


SqlAveragePriceData  MysqlAvePrice::Find_YesterdayDatabyDateTime(const char *tablename,std::string datetime)
{

	SqlAveragePriceData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC Limit 1",tablename,datetime.c_str());
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();

					 strcpy(data.Time,getValueString("Time").c_str());
					 data.AveragePrice=getValueDouble("AveragePrice");
					 data.StartOpenInterest =getValueInt("StartOpenInterest");
					 data.OpenInterest =getValueInt("OpenInterest");

				}
		}
	this->Empty();
	  return data;

}

int MysqlAvePrice::Get_MaxOpenInterest(const char* tablename)
	{
//	printf("MysqlAvePrice Get_MaxOpenInterest  begin ...\n");
		int max =0;

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT max(OpenInterest) FROM %s ",tablename);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
				{

					if(this->getRowCount()==1)
						{
							this->getFirst();
							max = getValueInt("max(OpenInterest)");
						}
				}

		this->Empty();
		return max;


//		printf("MysqlAvePrice Get_MaxOpenInterest max ---> [%d]  ...\n",max);
	}

int MysqlAvePrice::Check_Highcount(const char* tablename,const char * date,double high20)
	{
		int highcount=1;


		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC  ",tablename,date);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			for (int i=0; i<this->getRowCount();i++)
			{
				 std::string str_date;
				  str_date=getValueString("Time");
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

		/* retrieve the row count in the result set */


		this->Empty();
		return highcount;


//					printf("MongoCycle Check_Highcount end! ...\n");
	}

	int MysqlAvePrice::Check_Lowcount(const char* tablename,const char * date,double Low20)
	{
		int lowcount=1;

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC  ",tablename,date);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
				{
					this->getFirst();
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
