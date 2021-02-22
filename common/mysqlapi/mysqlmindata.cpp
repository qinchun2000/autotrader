#include "mysqlmindata.hpp"
 

MysqlMinData::MysqlMinData()
{

	if (IsAliveConn())
	{


	}else
	{

		if(this->IsCloseConn())
		{
			if(!Connect())
					{

						return;
					}
		}



	}



}


void MysqlMinData::CreateTable(const char *tablename)
{
	if(!this->Exist_Table(tablename))
	{
		char buf[500];
		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
					"MinuteTime DATETIME NOT NULL PRIMARY KEY, "
					"OpenPrice DOUBLE,"
					"ClosePrice DOUBLE,"
					"HighestPrice DOUBLE,"
					"LowestPrice DOUBLE,"
					"Volume INT(11),"
					"OpenInterest DOUBLE,"
					"Amount DOUBLE,"
					"LastTickTime CHAR(20) ) default character set utf8 ",tablename);
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}



void MysqlMinData::Insert(const char *tablename,Mysql_MinData data)
{

	char buf[500];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s (MinuteTime, OpenPrice,ClosePrice,HighestPrice,LowestPrice,"
			"Amount,Volume,OpenInterest,LastTickTime) "
			"VALUES ('%s','%.3f','%.3f','%.3f','%.3f',"
			"'%.3f','%d','%.f','%s')",tablename,
			data.MinuteTime,data.OpenPrice,data.ClosePrice,data.HighestPrice,data.LowestPrice,
			data.Amount,data.Volume,data.OpenInterest,data.LastTickTime);


	str_sql=buf;

	this->Execute(str_sql);

}

void MysqlMinData::Update(const char* tablename,Mysql_MinData data)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"UPDATE %s SET ClosePrice='%.3f',HighestPrice='%.3f',LowestPrice='%.3f',Amount ='%.2f' ,Volume ='%d',"
			"OpenInterest='%.f' ,LastTickTime='%s' "
			"WHERE MinuteTime='%s' ",tablename,
			data.ClosePrice,data.HighestPrice,data.LowestPrice,	data.Amount,data.Volume,
			data.OpenInterest,data.LastTickTime,data.MinuteTime);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

bool MysqlMinData::Exist_MinuteTime(const char *tablename,const char* minutetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;




	sprintf(buf,"SELECT MinuteTime FROM %s WHERE MinuteTime='%s' ",tablename,minutetime);
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



std::string MysqlMinData::GetLast_MinuteTime(const char *tablename)
{

	std::string  info;
	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT MinuteTime FROM %s ORDER BY MinuteTime desc limit 1 ",tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();
					 info =  getValueString("MinuteTime");
				}
		}
	this->Empty();
	  return info;

}

Mysql_MinData MysqlMinData::Find_LastDatabyMinuteTime(const char *tablename)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	Mysql_MinData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Order by MinuteTime Desc Limit 1 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());

			 data.OpenPrice=getValueDouble("OpenPrice");
			 data.ClosePrice=getValueDouble("ClosePrice");
			 data.HighestPrice=getValueDouble("HighestPrice");
			 data.LowestPrice=getValueDouble("LowestPrice");

			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.OpenInterest=getValueInt("OpenInterest");

			 strcpy(data.LastTickTime,getValueString("LastTickTime").c_str());

			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

Mysql_MinData MysqlMinData::Find_MaxVolume(const char *tablename)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	Mysql_MinData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf," select * from %s where Volume=(select MAX(Volume) from  %s ) ",tablename,tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());

			 data.OpenPrice=getValueDouble("OpenPrice");
			 data.ClosePrice=getValueDouble("ClosePrice");
			 data.HighestPrice=getValueDouble("HighestPrice");
			 data.LowestPrice=getValueDouble("LowestPrice");

			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.OpenInterest=getValueInt("OpenInterest");

			 strcpy(data.LastTickTime,getValueString("LastTickTime").c_str());

			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}
Mysql_MinData MysqlMinData::Find_DatabyMinuteTime(const char *tablename,const char* mintime)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	Mysql_MinData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where  MinuteTime='%s' ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)	{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());

			 data.OpenPrice=getValueDouble("OpenPrice");
			 data.ClosePrice=getValueDouble("ClosePrice");
			 data.HighestPrice=getValueDouble("HighestPrice");
			 data.LowestPrice=getValueDouble("LowestPrice");

			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.OpenInterest=getValueInt("OpenInterest");

			 strcpy(data.LastTickTime,getValueString("LastTickTime").c_str());

			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}


ATRData MysqlMinData::Get_ATRDataByPreLists(const char *tablename,int n)
		{


			ATRData atrdata;
//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);
			double total_tr;
			double tr;
			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s  ORDER BY MinuteTime DESC limit %d ",tablename,n);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();

				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					Mysql_MinData current_minute;

					current_minute.HighestPrice =  getValueDouble("HighestPrice");
					current_minute.LowestPrice =  getValueDouble("LowestPrice");
					current_minute.ClosePrice=  getValueDouble("ClosePrice");
					strcpy(current_minute.MinuteTime,getValueString("MinuteTime").c_str());

					double hl = abs(current_minute.HighestPrice -current_minute.LowestPrice);
					double ll= abs(current_minute.ClosePrice - current_minute.LowestPrice);
					double lh = abs(current_minute.ClosePrice - current_minute.HighestPrice);

					tr = max(hl,max(ll,lh));

					if (i==0)
					{
						atrdata.TR =tr;
						strcpy(atrdata.Time,current_minute.MinuteTime);
					}
//					printf("MysqlDayLine max %.2f    today.LastClose %.2f   today.High %.2f today.Low %.2f \n",tr,today.LastClose,today.High,today.Low );
					total_tr +=tr;
					this->getNext();

				}
			}


			atrdata.ATR = total_tr/(double)n;
//			printf("Get_ATRDatabyMinPrice Collect_HisATR_NData %.2f    %.2f \n",tr, atrdata.TR);
			this->Empty();
			return atrdata;

		}
void MysqlMinData::Find_ListsbyMinuteTime(vector<Mysql_MinData> &temp,const char *tablename,const char* minutetime)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  WHERE MinuteTime<'%s'  ",tablename,minutetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
				Mysql_MinData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.OpenInterest=getValueDouble("OpenInterest");
				data.OpenPrice = getValueDouble("OpenPrice");
				data.ClosePrice = getValueDouble("ClosePrice");
				data.HighestPrice = getValueDouble("HighestPrice");
				data.LowestPrice = getValueDouble("LowestPrice");
				temp.push_back(data);
				this->getNext();
			}
	}

	this->Empty();

}
void MysqlMinData::Find_ListsNbyMinuteTime(vector<Mysql_MinData> &temp,const char *tablename,const char* minutetime,int n)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  WHERE MinuteTime<'%s' ORDER BY MinuteTime DESC limit %d ",tablename,minutetime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				Mysql_MinData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.OpenInterest=getValueDouble("OpenInterest");
				data.OpenPrice = getValueDouble("OpenPrice");
				data.ClosePrice = getValueDouble("ClosePrice");
				data.HighestPrice = getValueDouble("HighestPrice");
				data.LowestPrice = getValueDouble("LowestPrice");
				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
void MysqlMinData::Find_LastListsbyN(vector<Mysql_MinData> &temp,const char *tablename,int n)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  ORDER BY MinuteTime DESC limit %d ",tablename,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				Mysql_MinData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.OpenInterest=getValueDouble("OpenInterest");
				data.OpenPrice = getValueDouble("OpenPrice");
				data.ClosePrice = getValueDouble("ClosePrice");
				data.HighestPrice = getValueDouble("HighestPrice");
				data.LowestPrice = getValueDouble("LowestPrice");
				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}


void MysqlMinData::Find_AllLists(vector<Mysql_MinData> &temp,const char *tablename)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				Mysql_MinData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.OpenInterest=getValueDouble("OpenInterest");
				data.OpenPrice = getValueDouble("OpenPrice");
				data.ClosePrice = getValueDouble("ClosePrice");
				data.HighestPrice = getValueDouble("HighestPrice");
				data.LowestPrice = getValueDouble("LowestPrice");
				temp.push_back(data);
				this->getNext();
			}
	}
//	reverse(temp.begin(),temp.end());
	this->Empty();

}

