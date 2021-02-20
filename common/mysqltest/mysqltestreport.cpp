#include "mysqltestreport.hpp"

MysqlTestReport::MysqlTestReport()
{
	std::string database="testreport";

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

void MysqlTestReport::CreateTable(const char* tablename)
{


	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"Product CHAR(20) NOT NULL PRIMARY KEY, "
						"Strategy CHAR(20),"
						"Section CHAR(20),"
						"StartTestTime CHAR(20),"
						"EndTestTime CHAR(20),"
						"TradeCount INT,"
						"WinCount INT,"

						"LongCount INT,"
						"ShortCount INT,"
						"WinCount_Long INT,"
						"WinCount_Short INT,"

						"InitialMoney DOUBLE,"
						"TotalWinProfit DOUBLE,"
						"TotalLoseProfit DOUBLE,"
						"AverageWinProfit DOUBLE,"
						"AverageLoseProfit DOUBLE,"
						"Asset DOUBLE,"
						"WinRate DOUBLE,"
						"WinLoseRate DOUBLE,"
						"Expectation DOUBLE,"
						"Commission DOUBLE,"
						"ErrorMinuteData INT) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);



}



void MysqlTestReport::Insert(const char* tablename,TestReportData data)
{


	char buf[2048];
	std::string str_sql;

//	sprintf(buf,"INSERT INTO %s (Time,StartTime,EndTime) VALUES ('%s','%s','%s' )",tablename,
//			data.Time,data.StartTime,data.EndTime);

	sprintf(buf,"INSERT INTO %s (Product,Strategy,StartTestTime,EndTestTime,TradeCount,"
			"WinCount,InitialMoney,TotalWinProfit,TotalLoseProfit,AverageWinProfit,"
			"AverageLoseProfit,Asset,WinRate,WinLoseRate,Expectation,"
			"Section,Commission,LongCount,ShortCount,WinCount_Long,WinCount_Short ) "
			"VALUES ('%s','%s','%s','%s','%d',"
			"'%d','%.2f','%.2f','%.2f','%.2f',"
			"'%.2f','%.2f','%.2f','%.2f','%.2f',"
			"'%s','%.2f','%d','%d','%d','%d')",tablename,
				data.Product,data.Strategy,data.StartTestTime,data.EndTestTime,data.TradeCount,
				data.WinCount,data.InitialMoney,data.TotalWinProfit,data.TotalLoseProfit,data.AverageWinProfit,
				data.AverageLoseProfit,data.Asset,data.WinRate,data.WinLoseRate,data.Expectation,
				data.Section,data.Commission,data.LongCount,data.ShortCount,data.WinCount_Long,data.WinCount_Short);

	str_sql=buf;

	this->Execute(str_sql);


}

void MysqlTestReport::Update(const char* tablename,TestReportData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET Strategy='%s',StartTestTime='%s',EndTestTime='%s',TradeCount='%d',Commission='%.3f',"
			"WinCount ='%d' ,InitialMoney ='%.2f' ,TotalWinProfit='%.2f' ,TotalLoseProfit='%.2f' ,AverageWinProfit='%.2f' ,"
			"AverageLoseProfit='%.2f' ,Asset='%.2f' ,WinRate='%.2f' ,WinLoseRate='%.2f' ,Expectation='%.2f' ,Section='%s',"
			"LongCount='%d',ShortCount='%d',WinCount_Long='%d',WinCount_Short='%d'  "
			"WHERE Product='%s' ",tablename,
			data.Strategy,data.StartTestTime,data.EndTestTime,data.TradeCount,data.Commission,
			data.WinCount,data.InitialMoney,data.TotalWinProfit,data.TotalLoseProfit,data.AverageWinProfit,
			data.AverageLoseProfit,data.Asset,data.WinRate,data.WinLoseRate,data.Expectation,data.Section,
			data.LongCount,data.ShortCount,data.WinCount_Long,data.WinCount_Short,data.Product);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestReport::InsertErrorData(const char* tablename,TestReportData data)
{


	char buf[2048];
	std::string str_sql;

//	sprintf(buf,"INSERT INTO %s (Time,StartTime,EndTime) VALUES ('%s','%s','%s' )",tablename,
//			data.Time,data.StartTime,data.EndTime);

	sprintf(buf,"INSERT INTO %s (Product,ErrorMinuteData) "
			"VALUES ('%s','%d' )",tablename,
				data.Product,data.ErrorMinuteData);

	str_sql=buf;

	this->Execute(str_sql);


}
void MysqlTestReport::UpdateErrorData(const char* tablename,TestReportData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET ErrorMinuteData='%d' WHERE Product='%s' ",tablename,
			data.ErrorMinuteData,data.Product);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}



bool  MysqlTestReport::Exist_DatabyProductID(const char* tablename,const char *productid)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Product='%s'  ",tablename,productid);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

			char buf[100];
			strcpy(buf,getValueString("Product").c_str());

			char buf1[100];
			strcpy(buf1,productid);

			if (strcmp(buf,buf1)==0)
			{
				 ret=true;
			}



		}


	}

	this->Empty();

	return ret;
}





TestReportData  MysqlTestReport::Find_DatabyProductID(const char* tablename,const char* productid)
{

	TestReportData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE Product ='%s' ",tablename,productid);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();


			char buf[100];
			strcpy(buf,getValueString("Product").c_str());

			char buf1[100];
			strcpy(buf1,productid);

			if (strcmp(buf,buf1)==0)
			{

				strcpy(data.Product,getValueString("Product").c_str());
				strcpy(data.Section,getValueString("Section").c_str());
				strcpy(data.Strategy,getValueString("Strategy").c_str());
				strcpy(data.StartTestTime,getValueString("StartTestTime").c_str());
				strcpy(data.EndTestTime,getValueString("EndTestTime").c_str());
				data.TradeCount = getValueInt("TradeCount");

				data.WinCount = getValueInt("WinCount");
				data.InitialMoney = getValueDouble("InitialMoney");
				data.TotalWinProfit = getValueDouble("TotalWinProfit");
				data.TotalLoseProfit = getValueDouble("TotalLoseProfit");
				data.AverageWinProfit = getValueDouble("AverageWinProfit");

				data.AverageLoseProfit = getValueDouble("AverageLoseProfit");
				data.Asset = getValueDouble("Asset");
				data.WinRate = getValueDouble("WinRate");
				data.WinLoseRate = getValueDouble("WinLoseRate");
				data.Expectation = getValueDouble("Expectation");
				data.Commission = getValueDouble("Commission");
				data.ErrorMinuteData = getValueInt("ErrorMinuteData");

				data.LongCount = getValueInt("LongCount");
				data.ShortCount = getValueInt("ShortCount");
				data.WinCount_Long = getValueInt("WinCount_Long");
				data.WinCount_Short = getValueInt("WinCount_Short");

			}


		}


	}

	this->Empty();

	return data;
}

void MysqlTestReport::Find_AllLists(vector<TestReportData> &temp,const char* tablename)
{


	TestReportData data;


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
				strcpy(data.Product,getValueString("Product").c_str());
				strcpy(data.Section,getValueString("Section").c_str());
				strcpy(data.Strategy,getValueString("Strategy").c_str());
				strcpy(data.StartTestTime,getValueString("StartTestTime").c_str());
				strcpy(data.EndTestTime,getValueString("EndTestTime").c_str());
				data.TradeCount = getValueInt("TradeCount");

				data.WinCount = getValueInt("WinCount");
				data.InitialMoney = getValueDouble("InitialMoney");
				data.TotalWinProfit = getValueDouble("TotalWinProfit");
				data.TotalLoseProfit = getValueDouble("TotalLoseProfit");
				data.AverageWinProfit = getValueDouble("AverageWinProfit");

				data.AverageLoseProfit = getValueDouble("AverageLoseProfit");
				data.Asset = getValueDouble("Asset");
				data.WinRate = getValueDouble("WinRate");
				data.WinLoseRate = getValueDouble("WinLoseRate");
				data.Expectation = getValueDouble("Expectation");
				data.Commission = getValueDouble("Commission");
				data.ErrorMinuteData = getValueInt("ErrorMinuteData");
				data.LongCount = getValueInt("LongCount");
				data.ShortCount = getValueInt("ShortCount");
				data.WinCount_Long = getValueInt("WinCount_Long");
				data.WinCount_Short = getValueInt("WinCount_Short");

				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}


void MysqlTestReport::Find_ListsOrderbyExp(vector<TestReportData> &temp,const char* tablename)
{


	TestReportData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s Order by Expectation desc ", tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				strcpy(data.Product,getValueString("Product").c_str());
				strcpy(data.Section,getValueString("Section").c_str());
				strcpy(data.Strategy,getValueString("Strategy").c_str());
				strcpy(data.StartTestTime,getValueString("StartTestTime").c_str());
				strcpy(data.EndTestTime,getValueString("EndTestTime").c_str());
				data.TradeCount = getValueInt("TradeCount");

				data.WinCount = getValueInt("WinCount");
				data.InitialMoney = getValueDouble("InitialMoney");
				data.TotalWinProfit = getValueDouble("TotalWinProfit");
				data.TotalLoseProfit = getValueDouble("TotalLoseProfit");
				data.AverageWinProfit = getValueDouble("AverageWinProfit");

				data.AverageLoseProfit = getValueDouble("AverageLoseProfit");
				data.Asset = getValueDouble("Asset");
				data.WinRate = getValueDouble("WinRate");
				data.WinLoseRate = getValueDouble("WinLoseRate");
				data.Expectation = getValueDouble("Expectation");
				data.Commission = getValueDouble("Commission");
				data.ErrorMinuteData = getValueInt("ErrorMinuteData");

				data.LongCount = getValueInt("LongCount");
				data.ShortCount = getValueInt("ShortCount");
				data.WinCount_Long = getValueInt("WinCount_Long");
				data.WinCount_Short = getValueInt("WinCount_Short");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

