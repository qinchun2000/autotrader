#include "mysqlhismindata.hpp"

MysqlHisMinData::MysqlHisMinData()
{
	if (!IsAliveConn())	{
		if(this->IsCloseConn())	{
			if(!Connect())	{
				return;
			}
		}
	}

}


void MysqlHisMinData::CreateTable(const char *tablename)
{

	if(!this->Exist_Table(tablename))	{
		char buf[500];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
							"MinuteTime DATETIME NOT NULL PRIMARY KEY, "
							"price DOUBLE(15,3),"
							"Amount DOUBLE(15,3), "
							"Volume INT(11), "
							"AvePrice DOUBLE(15,3), "
							"OpenInterest INT(11))",tablename);

		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}


void MysqlHisMinData::Insert(const char *tablename,MinuteLineData data)
{
	char buf[2048];
	std::string str_sql;


	sprintf(buf,"INSERT INTO %s (MinuteTime, price,Amount,Volume,AvePrice,OpenInterest) "
			"VALUES ('%s','%.3f','%.3f','%d','%.3f','%d')",tablename,
			data.MinuteTime,data.price,data.Amount,data.Volume,data.AvePrice,data.OpenInterest);
	str_sql=buf;

	this->Execute(str_sql);


}
void MysqlHisMinData::Update(const char* tablename,MinuteLineData data)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"UPDATE %s SET price='%.3f',Amount='%.3f',Volume='%d',AvePrice='%.3f',OpenInterest='%d' "
			"  WHERE MinuteTime='%s' ",tablename,data.price,data.Amount,data.Volume,data.AvePrice,data.OpenInterest,data.MinuteTime);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlHisMinData::DeletebyMinuteTime(const char* tablename,const char* sqltime)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"DELETE  FROM %s WHERE MinuteTime='%s' ",tablename,sqltime);

	str_sql=buf;
	this->Execute(str_sql);

}
int  MysqlHisMinData::CountbyAll(const char* tablename)
{

	int count=0;

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

bool MysqlHisMinData::Exist_MinuteTime(const char *tablename,std::string minutetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;

	char tem[20];
	sprintf(tem,"%s-%s-%s %s:%s:%s",minutetime.substr(0,4).c_str(),minutetime.substr(4,2).c_str(),minutetime.substr(6,2).c_str(),
			minutetime.substr(8,2).c_str(),minutetime.substr(10,2).c_str(),minutetime.substr(12,2).c_str());
	sprintf(buf,"SELECT MinuteTime FROM %s WHERE MinuteTime='%s' ",tablename,tem);

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



bool MysqlHisMinData::Exist_DataBySqlTime(const char *tablename,const char* minutetime)
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



void MysqlHisMinData::Find_ListsNbyMinuteTime(vector<MinuteLineData> &temp,const char *tablename,const char* minutetime,int n)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  WHERE MinuteTime<='%s' ORDER BY MinuteTime DESC limit %d ",tablename,minutetime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			MinuteLineData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.price=getValueDouble("price");
				 data.Amount=getValueDouble("Amount");
				 data.AvePrice=getValueDouble("AvePrice");
				 data.OpenInterest=getValueInt("OpenInterest");

				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
void MysqlHisMinData::Find_PriceListsN(vector<double> &temp,const char *tablename,int n)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  ORDER BY MinuteTime DESC limit %d ",tablename,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

				double price=getValueDouble("price");


				temp.push_back(price);
//				cout<<"   "<<price<<endl;
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
void MysqlHisMinData::Find_PriceListsNbyMinuteTime(vector<double> &temp,const char *tablename,const char* minutetime,int n)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  WHERE MinuteTime<='%s' ORDER BY MinuteTime DESC limit %d ",tablename,minutetime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

				double price=getValueDouble("price");


				temp.push_back(price);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
void MysqlHisMinData::Find_ListsbyMinuteTime(vector<MinuteLineData> &temp,const char *tablename,const char* minutetime)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  WHERE MinuteTime<='%s' ORDER BY MinuteTime ",tablename,minutetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			MinuteLineData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.price=getValueDouble("price");
				 data.Amount=getValueDouble("Amount");
				 data.AvePrice=getValueDouble("AvePrice");
				 data.OpenInterest=getValueInt("OpenInterest");

				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
void MysqlHisMinData::Find_ListsbyStartEnd(vector<MinuteLineData> &temp,const char *tablename,const char* starttime,const char* endtime)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  WHERE MinuteTime>'%s' AND MinuteTime<'%s' ORDER BY MinuteTime ASC ",tablename,starttime,endtime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			MinuteLineData data;
				strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
				data.Volume=getValueInt("Volume");
				data.price=getValueDouble("price");
				 data.Amount=getValueDouble("Amount");
				 data.AvePrice=getValueDouble("AvePrice");
				 data.OpenInterest=getValueInt("OpenInterest");

				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
void MysqlHisMinData::Find_AllList(vector<MinuteLineData> &temp,const char *tablename)
{
//	printf ("MysqlHisMinData::Find_AllList begin \n");
	MinuteLineData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY MinuteTime ASC ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql)){
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");

			  temp.push_back(data);
			  this->getNext();
			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();

}

//void MysqlHisMinData::Find_AllDateList(vector<std::string> &temp,const char *tablename)
//{
////	printf ("MysqlHisMinData::Find_AllList begin \n");
//	MinuteLineData data;
//	char buf[2048];
//	std::string str_sql;
//
//	sprintf(buf,"SELECT * FROM %s ORDER BY MinuteTime ASC ",tablename);
//
//	str_sql=buf;
//	if(this->ExecuteQuery(str_sql))
//	{
//		this->getFirst();
//		/* retrieve the row count in the result set */
//		for (int i=0; i<this->getRowCount();i++)
//			{
//
//			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
//			 data.price=getValueDouble("price");
//			 data.Amount=getValueDouble("Amount");
//			 data.Volume=getValueInt("Volume");
//			 data.AvePrice=getValueDouble("AvePrice");
//			 data.OpenInterest=getValueInt("OpenInterest");
//
//			  temp.push_back(data);
//			  this->getNext();
//			}
//	}
//
////	printf ("MysqlHisMinData::Find_AllList end \n");
//	this->Empty();
//
//}

MinuteLineData MysqlHisMinData::Find_DatabyMinuteTime(const char *tablename,const char* mintime)
{
//	printf ("MysqlHisMinData::Find_DatabyMinuteTime begin \n");
	MinuteLineData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where  MinuteTime='%s' ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");



			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

MinuteLineData MysqlHisMinData::Find_ExistDatabyMinuteTime(const char *tablename,const char* mintime)
{
//	printf ("MysqlHisMinData::Find_DatabyMinuteTime begin \n");
	MinuteLineData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where  MinuteTime<='%s' ORDER BY MinuteTime DESC LIMIT 1  ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");



			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

MinuteLineData MysqlHisMinData::Find_PreDatabyMinuteTime(const char *tablename,const char* mintime)
{
//	printf ("MysqlHisMinData::Find_DatabyMinuteTime begin \n");
	MinuteLineData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where  MinuteTime<'%s'  Order by MInuteTime Desc Limit 1 ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)			{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");

			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}
MinuteLineData MysqlHisMinData::Find_PreNDatabyMinuteTime(const char *tablename,const char* mintime,const int n)
{
//	printf ("MysqlHisMinData::Find_DatabyMinuteTime begin \n");
	MinuteLineData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where  MinuteTime<'%s'  Order by MInuteTime asc Limit %d ",tablename,mintime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
//		for (int i=0; i<this->getRowCount();i++){

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");



//			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

MinuteLineData MysqlHisMinData::Find_LastData(const char *tablename)
{
//	printf ("MysqlHisMinData::Find_DatabyMinuteTime begin \n");
	MinuteLineData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY MinuteTime DESC limit 1 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		/* retrieve the row count in the result set */

		if(this->getRowCount()>0){

			this->getFirst();

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");
			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

int MysqlHisMinData::Count_MinuteTables(const char *pinstrumentid)
{

	int count=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"show tables like '%s_%%'  ",pinstrumentid);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		count = this->getRowCount();
	}
	this->Empty();
	return count;
}

MinuteLineData MysqlHisMinData::Find_MaxVolume(const char *tablename)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	MinuteLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf," select * from %s where Volume=(select MAX(Volume) from  %s ) ",tablename,tablename);

//	printf("sql:%s\n",buf);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");


			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

MinuteLineData MysqlHisMinData::Find_MaxVolumebyMinuteTime(const char *tablename,const char* mintime)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	MinuteLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf," select * from %s where Volume=(select MAX(Volume) from  %s where  MinuteTime<'%s' ) and   MinuteTime<'%s' ",
			tablename,tablename,mintime,mintime);

//	printf("sql:%s\n",buf);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");


			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

MinuteLineData MysqlHisMinData::Find_MaxVolumeEndbyMinuteTime(const char *tablename,const char* mintime)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	MinuteLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf," select * from %s where Volume=(select MAX(Volume) from  %s where  MinuteTime<='%s' ) and   MinuteTime<='%s' ",
			tablename,tablename,mintime,mintime);

//	printf("sql:%s\n",buf);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{

			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");


			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}
int MysqlHisMinData::GetStartOpenInterest(const char *tablename)
{

	int opi=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY MinuteTime ASC limit 1 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
			 opi =  getValueInt("OpenInterest");
		}
	}
	this->Empty();
	return opi;
}


int MysqlHisMinData::GetLastOpenInterest(const char *tablename)
{

	int opi=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY MinuteTime DESC limit 1 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();
			 opi =  getValueInt("OpenInterest");
		}
	}
	this->Empty();
	return opi;
}

int MysqlHisMinData::GetLast3MinVolumeSum(const char *tablename,const char*mintime)
{

	int sumvol=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where MinuteTime<'%s' ORDER BY MinuteTime DESC limit 3 ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			sumvol+=getValueInt("Volume");
			this->getNext();
		}
	}
	this->Empty();
	return sumvol;
}
int MysqlHisMinData::GetVolumeSum(const char *tablename)
{

	int sum_volume=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT sum(Volume) FROM %s   ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
		this->getFirst();
		/* retrieve the row count in the result set */

		sum_volume=getValueInt("sum(Volume)");

		}
	}
	this->Empty();


	return sum_volume;
}
int MysqlHisMinData::GetLastMinVolumeSum(const char *tablename,const char*mintime)
{

	int sumvol=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where MinuteTime<'%s' ORDER BY MinuteTime DESC limit 1 ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			sumvol+=getValueInt("Volume");
			this->getNext();
		}
	}
	this->Empty();
	return sumvol;
}

MinuteLineData MysqlHisMinData::GetLastMinbyMinuteTime(const char *tablename,const char*mintime)
{

	MinuteLineData data;
	memset(&data,0,sizeof(data));

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where MinuteTime<'%s' ORDER BY MinuteTime DESC limit 1 ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
			 data.price=getValueDouble("price");
			 data.Amount=getValueDouble("Amount");
			 data.Volume=getValueInt("Volume");
			 data.AvePrice=getValueDouble("AvePrice");
			 data.OpenInterest=getValueInt("OpenInterest");
			this->getNext();
		}
	}
	this->Empty();
	return data;
}

double MysqlHisMinData::GetMAbyMinuteTime(const char *tablename,const int n,const char*mintime)
{


	double sum_price=0.0;
	double ma=0.0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT sum(price) FROM %s Where MinuteTime<='%s' ORDER BY MinuteTime limit %d  ",tablename,mintime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
		this->getFirst();
			/* retrieve the row count in the result set */
			sum_price=getValueDouble("sum(price)");
		}
	}
	this->Empty();

	if(n>0){
		ma=sum_price/(double)n;
	}
	else{
		ma=0.0;
	}
	return ma;
}
double MysqlHisMinData::GetSettlePrice(const char *tablename,const int vm)
{

	int sum_volume=0;
	double sum_amount=0.0;
	double averageprice=0.0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT sum(Volume),sum(Amount) FROM %s   ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
		this->getFirst();
		/* retrieve the row count in the result set */

		sum_volume=getValueInt("sum(Volume)");
		sum_amount=getValueDouble("sum(Amount)");
		}
	}
	this->Empty();

	if(sum_volume>0){
		averageprice=sum_amount/((double)sum_volume*(double)vm);
	}
	else{
		averageprice=0.0;
	}
	return averageprice;
}
double MysqlHisMinData::GetAveragePricebyMinuteTime(const char *tablename,const int vm,const char*mintime)
{

	int sum_volume=0;
	double sum_amount=0.0;
	double averageprice=0.0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT sum(Volume),sum(Amount) FROM %s Where MinuteTime<'%s' ORDER BY MinuteTime  ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
		this->getFirst();
		/* retrieve the row count in the result set */

		sum_volume=getValueInt("sum(Volume)");
		sum_amount=getValueDouble("sum(Amount)");
		}
	}
	this->Empty();

	if(sum_volume>0){
		averageprice=sum_amount/((double)sum_volume*(double)vm);
	}
	else{
		averageprice=0.0;
	}
	return averageprice;
}
double MysqlHisMinData::GetAverageVolumeNbyMinuteTime(const char *tablename,const int n,const char*mintime)
{

	int sum_volume=0;

	double average=0.0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT sum(Volume) FROM %s Where MinuteTime<='%s' ORDER BY MinuteTime DESC LIMIT %d ",tablename,mintime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
		this->getFirst();
		/* retrieve the row count in the result set */

		sum_volume=getValueInt("sum(Volume)");

		}
	}
	this->Empty();

	if(sum_volume>0){
		average=(double)sum_volume/(double)n;
	}
	else{
		average=0.0;
	}
	return average;
}
double MysqlHisMinData::GetAveragePrice(const char *tablename)
{

	double total_price=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()>0)
			{
				this->getFirst();
				for (int i=0; i<this->getRowCount();i++)
				{
					 double price=getValueDouble("price");
					 total_price+=price;
					  this->getNext();
				}

			}
		}

	double averageprice;

	if(this->getRowCount()>0)
	{
		averageprice = total_price/(double)this->getRowCount();
	}
	else
	{
		averageprice=0;
	}

	this->Empty();
	return averageprice;
}
CycleNRefData  MysqlHisMinData::GetCycleNRefDatabyMinteTime(const char *tablename,const char* mintime)
{
	CycleNRefData ret;
	memset(&ret,0,sizeof(ret));

	double total_price=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where MinuteTime<'%s' ORDER BY MinuteTime ",tablename,mintime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))		{
			if(this->getRowCount()>0)		{
				this->getFirst();

				for (int i=0; i<this->getRowCount();i++)	{

					MinuteLineData data;
					 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
					 data.price=getValueDouble("price");
					 data.Amount=getValueDouble("Amount");
					 data.Volume=getValueInt("Volume");
					 data.AvePrice=getValueDouble("AvePrice");
					 data.OpenInterest=getValueInt("OpenInterest");

					if(i==0){
						ret.Highest = data.price;
						ret.Lowest = data.price;
						strcpy(ret.Highest_Time,data.MinuteTime);
						strcpy(ret.Lowest_Time,data.MinuteTime);
					}
					else{
						if(data.price>ret.Highest){
							ret.Highest = data.price;
							strcpy(ret.Highest_Time,data.MinuteTime);
						}
						if(data.price<ret.Lowest){
							ret.Lowest = data.price;
							strcpy(ret.Lowest_Time,data.MinuteTime);
						}
					}

				  this->getNext();
				}

			}
		}

	this->Empty();
	return ret;
}
MinuteStaticData MysqlHisMinData::GetMinuteStaticDatabyMinteTime(const char *tablename,int vm,const char* mintime)
{
	MinuteStaticData ret;
		memset(&ret,0,sizeof(ret));

		double total_price=0;

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s  Where MinuteTime<'%s' ORDER BY MinuteTime ",tablename,mintime);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))		{
				if(this->getRowCount()>0)		{
					ret.MinuteCount = this->getRowCount();
					this->getFirst();

					for (int i=0; i<this->getRowCount();i++)	{
						MinuteLineData data;
						 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
						 data.price=getValueDouble("price");
						 data.Amount=getValueDouble("Amount");
						 data.Volume=getValueInt("Volume");
						 data.AvePrice=getValueDouble("AvePrice");
						 data.OpenInterest=getValueInt("OpenInterest");
						double averageprice = this->GetAveragePricebyMinuteTime(tablename,vm,data.MinuteTime);

						if(data.price>averageprice){
							ret.LongCount++;
						}
						else if (data.price<averageprice){
							ret.ShortCount++;
						}



					  this->getNext();
					}

				}
			}

		this->Empty();
		return ret;
}
CycleNRefData  MysqlHisMinData::GetCycleNRefDatabyNMinteTime(const char *tablename,const char* mintime, int n)
{
	CycleNRefData ret;
	memset(&ret,0,sizeof(ret));

	double total_price=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  Where MinuteTime<'%s' ORDER BY MinuteTime Limit %d",tablename,mintime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))		{
			if(this->getRowCount()>0)		{
				this->getFirst();

				for (int i=0; i<this->getRowCount();i++)	{

					MinuteLineData data;
					 strcpy(data.MinuteTime,getValueString("MinuteTime").c_str());
					 data.price=getValueDouble("price");
					 data.Amount=getValueDouble("Amount");
					 data.Volume=getValueInt("Volume");
					 data.AvePrice=getValueDouble("AvePrice");
					 data.OpenInterest=getValueInt("OpenInterest");

					if(i==0){
						ret.Open = data.price;
						ret.Highest = data.price;
						ret.Lowest = data.price;
						strcpy(ret.Highest_Time,data.MinuteTime);
						strcpy(ret.Lowest_Time,data.MinuteTime);
					}
					else{
						if(data.price>ret.Highest){
							ret.Highest = data.price;
							strcpy(ret.Highest_Time,data.MinuteTime);
						}
						if(data.price<ret.Lowest){
							ret.Lowest = data.price;
							strcpy(ret.Lowest_Time,data.MinuteTime);
						}
					}

				  this->getNext();
				}

			}
		}

	this->Empty();
	return ret;
}
double MysqlHisMinData::GetAveragePriceByTradingDay(const char *instrumentid,const char *tradingday)
{
	double averageprice;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT AVG(price) FROM %s_%s  ",instrumentid,tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)	{
			this->getFirst();
			for (int i=0; i<this->getRowCount();i++)	{
				averageprice=getValueDouble("AVG(price)");
			}

		}
		else{
			averageprice=0;
		}
	}


	this->Empty();
	return averageprice;
}

