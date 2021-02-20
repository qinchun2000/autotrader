#include "mysqltestasset.hpp"

MysqlTestAsset::MysqlTestAsset()
{
	std::string database="testasset";

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

void MysqlTestAsset::CreateTable(const char* tablename)
{


	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"ID CHAR(30) NOT NULL PRIMARY KEY, "
						"OpenDateTime DATETIME,"
						"CloseDateTime DATETIME,"
						"InstrumentID CHAR(20),"
						"Direction INT,"
						"Status INT,"
						"Sedimentary DOUBLE,"
						"Profit DOUBLE,"
						"Commission DOUBLE,"
						"UseMarginRate DOUBLE,"
						"HighestAsset DOUBLE,"
						"BackDownRate DOUBLE,"
						"SameTimeHold DOUBLE,"
						"MaxMargin DOUBLE,"
						"MaxMarginRate DOUBLE,"
						"LastAsset DOUBLE) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);



}



void MysqlTestAsset::Insert(const char* tablename,TestAssetData data)
{


	char buf[2048];
	std::string str_sql;

//	sprintf(buf,"INSERT INTO %s (Time,StartTime,EndTime) VALUES ('%s','%s','%s' )",tablename,
//			data.Time,data.StartTime,data.EndTime);

	sprintf(buf,"INSERT INTO %s (ID,OpenDateTime,InstrumentID,Direction,CloseDateTime,"
				"Status,Profit,Commission,UseMarginRate,HighestAsset,"
				"BackDownRate,Sedimentary) "
				"VALUES ('%s','%s','%s','%d','%s',"
				"'%d','%.2f','%.2f','%.2f','%.2f',"
				"'%.2f','%.2f')",tablename,
				data.ID,data.OpenDateTime,data.InstrumentID,data.Direction,data.CloseDateTime,
				data.Status,data.Profit,data.Commission,data.UseMarginRate,data.HighestAsset,
				data.BackDownRate,data.Sedimentary);

	str_sql=buf;

	this->Execute(str_sql);


}

void MysqlTestAsset::Update(const char* tablename,TestAssetData data)
{


	char buf[2048];
	std::string str_sql;



//	sprintf(buf,"UPDATE %s SET LastAsset='%.2f' ,HighestAsset='%.2f',BackDownRate='%.2f' ,SameTimeHold='%d',MaxMarginRate='%.2f' "
//			"WHERE ID='%s' ",tablename,
//			data.LastAsset,data.HighestAsset,data.BackDownRate,data.SameTimeHold,data.MaxMarginRate,data.ID);

	sprintf(buf,"UPDATE %s SET LastAsset='%.2f' ,HighestAsset='%.2f',BackDownRate='%.2f' "
				"WHERE ID='%s' ",tablename,
				data.LastAsset,data.HighestAsset,data.BackDownRate,data.ID);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestAsset::UpdateMaxMargin(const char* tablename,TestAssetData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET SameTimeHold='%d' ,MaxMarginRate='%.2f' "
			"WHERE ID='%s' ",tablename,
			data.SameTimeHold,data.MaxMarginRate,data.ID);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void  MysqlTestAsset::Delete_byID(const char* tablename,const char* id)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"DELETE FROM %s  WHERE ID='%s' ",tablename,id);


	str_sql=buf;
	this->Execute(str_sql);
}

double  MysqlTestAsset::Get_MaxbyHighestAsset(const char* tablename)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT max(HighestAsset) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("max(HighestAsset)");

		}


	}

	this->Empty();

	return max;
}



double  MysqlTestAsset::Get_MaxbyBackDownRate(const char* tablename)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT max(BackDownRate) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("max(BackDownRate)");

		}


	}

	this->Empty();

	return max;
}
double  MysqlTestAsset::Get_MaxbySameTimeHold(const char* tablename)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT max(SameTimeHold) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("max(SameTimeHold)");

		}


	}

	this->Empty();

	return max;
}
double  MysqlTestAsset::Get_MaxbyMarginRate(const char* tablename)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT max(MaxMarginRate) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("max(MaxMarginRate)");

		}


	}

	this->Empty();

	return max;
}

double  MysqlTestAsset::Get_SumbyProfit(const char* tablename)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT sum(Profit) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("sum(Profit)");

		}


	}

	this->Empty();

	return max;
}

double  MysqlTestAsset::Get_SumbyCommission(const char* tablename)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT sum(Commission) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("sum(Commission)");

		}


	}

	this->Empty();

	return max;
}

int  MysqlTestAsset::CountbyAll(const char* tablename)
{

	int count;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(ID) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				count = getValueInt("count(ID)");

		}


	}

	this->Empty();

	return count;
}
int  MysqlTestAsset::CountbyWinProfit(const char* tablename)
{

	int count;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(ID) FROM %s WHERE Profit>0 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				count = getValueInt("count(ID)");

		}


	}

	this->Empty();

	return count;
}
bool  MysqlTestAsset::Exist_DatabyID(const char* tablename,const char *id)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  ID='%s'  ",tablename,id);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

			char buf[100];
			strcpy(buf,getValueString("ID").c_str());

			char buf1[100];
			strcpy(buf1,id);

			if (strcmp(buf,buf1)==0)
			{
				 ret=true;
			}



		}


	}

	this->Empty();

	return ret;
}


TestAssetData  MysqlTestAsset::Find_DatabyID(const char* tablename,const char* id)
{

	TestAssetData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE ID ='%s' ",tablename,id);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();


			char buf[100];
			strcpy(buf,getValueString("ID").c_str());

			char buf1[100];
			strcpy(buf1,id);

			if (strcmp(buf,buf1)==0)
			{

				strcpy(data.ID,getValueString("ID").c_str());
				data.UseMarginRate = getValueDouble("UseMarginRate");
				data.Profit = getValueDouble("Profit");
				data.LastAsset = getValueDouble("LastAsset");
				data.Sedimentary = getValueDouble("Sedimentary");

			}


		}


	}

	this->Empty();

	return data;
}

void MysqlTestAsset::Find_AllLists(vector<TestAssetData> &temp,const char* tablename)
{


	TestAssetData data;


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
			strcpy(data.ID,getValueString("ID").c_str());

			data.Profit = getValueDouble("Profit");
			data.LastAsset = getValueDouble("LastAsset");
			data.Sedimentary = getValueDouble("Sedimentary");

			  temp.push_back(data);
			  this->getNext();
			}
	}
	this->Empty();

}

void MysqlTestAsset::Find_AllListsbyCloseTime(vector<TestAssetData> &temp,const char* tablename)
{


	TestAssetData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ORDER BY CloseDateTime ASC ", tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
			strcpy(data.ID,getValueString("ID").c_str());
			strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
			strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());

			data.Profit = getValueDouble("Profit");
			data.UseMarginRate = getValueDouble("UseMarginRate");
			data.LastAsset = getValueDouble("LastAsset");
			data.HighestAsset = getValueDouble("HighestAsset");
			data.BackDownRate = getValueDouble("BackDownRate");
			data.Sedimentary = getValueDouble("Sedimentary");
			  temp.push_back(data);
			  this->getNext();
			}
	}
	this->Empty();

}


void  MysqlTestAsset::Find_SameTimeHoldListbyTime(vector<TestAssetData> &temp,const char* tablename,
		const char* starttime ,const char* endtime)
{

	TestAssetData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s  Where OpenDateTime >'%s' AND OpenDateTime <'%s' order by OpenDateTime asc ",tablename,starttime,endtime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();



			for (int i=0; i<this->getRowCount();i++)
			{
				strcpy(data.ID,getValueString("ID").c_str());
				strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
				strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());

				data.Profit = getValueDouble("Profit");
				data.UseMarginRate = getValueDouble("UseMarginRate");
				data.LastAsset = getValueDouble("LastAsset");
				data.HighestAsset = getValueDouble("HighestAsset");
				data.BackDownRate = getValueDouble("BackDownRate");
				data.Sedimentary = getValueDouble("Sedimentary");
				temp.push_back(data);
				this->getNext();
			}
		}


	}

	this->Empty();


}


int  MysqlTestAsset::CountbySqlTime(const char* tablename,const char* sqltime)
{

	int count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(ID) FROM %s WHERE OpenDateTime <'%s' AND CloseDateTime >'%s'  ",tablename,sqltime,sqltime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				count = getValueInt("count(ID)");

		}


	}

	this->Empty();

	return count;
}


double  MysqlTestAsset::Get_SumUseMarginRatebySqlTime(const char* tablename,const char* sqltime)
{

	double max;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT sum(UseMarginRate) FROM %s WHERE OpenDateTime <'%s' AND CloseDateTime >'%s' ",tablename,sqltime,sqltime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				max = getValueDouble("sum(UseMarginRate)");

		}


	}

	this->Empty();

	return max;
}
