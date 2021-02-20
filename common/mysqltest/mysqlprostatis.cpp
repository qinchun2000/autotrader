#include "mysqlprostatis.hpp"

MysqlProStatis::MysqlProStatis()
{
	std::string database="hisprostatis";

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


void MysqlProStatis::CreateTable(const char* tablename)
{


	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("

						"Time DATETIME NOT NULL PRIMARY KEY, "
						"ProductID CHAR(20) , "
						"InstrumentID CHAR(40),"
						"Year INT,"
						"Sedimentary DOUBLE) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);



}



void MysqlProStatis::Insert(const char* tablename,ProductStatisticsData data)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s (Time,ProductID,InstrumentID,Year,Sedimentary) "
			"VALUES ('%s','%s','%s','%d','%.2f')",tablename,
				data.Time,data.ProductID,data.InstrumentID,data.Year,data.Sedimentary);

	str_sql=buf;

	this->Execute(str_sql);


}




bool  MysqlProStatis::Exist_DatabyMinuteTime(const char* tablename,const char *time)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Time='%s'  ",tablename,time);

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





ProductStatisticsData  MysqlProStatis::Find_DatabyProductID(const char* tablename,const char* productid)
{
	ProductStatisticsData data;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE ProductID='%s'  ",tablename,productid);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
			strcpy(data.Time,getValueString("Time").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			data.Year=getValueInt("Year");
			data.Sedimentary=getValueDouble("Sedimentary");
		}
	}

	this->Empty();

	return data;
}

void MysqlProStatis::Find_AllLists(vector<ProductStatisticsData> &temp,const char* tablename)
{


	ProductStatisticsData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ", tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			strcpy(data.Time,getValueString("Time").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			data.Year=getValueInt("Year");
			data.Sedimentary=getValueDouble("Sedimentary");


			  temp.push_back(data);
			  this->getNext();
		}
	}
	this->Empty();

}

ProductStatisticsData MysqlProStatis::Find_MaxSedimentary(const char *tablename)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	ProductStatisticsData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf," select * from %s where Sedimentary=(select MAX(Sedimentary) from  %s ) ",tablename,tablename);

//	printf("sql:%s\n",buf);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{
			strcpy(data.Time,getValueString("Time").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			data.Year=getValueInt("Year");
			data.Sedimentary=getValueDouble("Sedimentary");
			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

ProductStatisticsData MysqlProStatis::Find_MinSedimentary(const char *tablename)
{
//	printf ("MysqlMinData::MysqlMinData begin \n");
	ProductStatisticsData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf," select * from %s where Sedimentary=(select MIN(Sedimentary) from  %s ) ",tablename,tablename);

//	printf("sql:%s\n",buf);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		if(this->getRowCount()>0)				{
			strcpy(data.Time,getValueString("Time").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			data.Year=getValueInt("Year");
			data.Sedimentary=getValueDouble("Sedimentary");
			}
	}

//	printf ("MysqlHisMinData::Find_AllList end \n");
	this->Empty();
	return data;
}

double MysqlProStatis::GetAverageSedimentary(const char * tablename)
{
	double averageprice;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT AVG(Sedimentary) FROM %s  ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)	{
			this->getFirst();
			averageprice=getValueDouble("AVG(Sedimentary)");
		}
		else{
			averageprice=0;
		}
	}


	this->Empty();
	return averageprice;
}
