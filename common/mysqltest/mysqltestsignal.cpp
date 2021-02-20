#include "mysqltestsignal.hpp"

MysqlTestSignal::MysqlTestSignal()
{
	std::string database="testrecord";
	std::string tablename="day_signal";

	if(this->IsCloseConn())	{
		if(!Connect())	{
			return;
		}
	}
	else{
		if (IsAliveConn()){
		}
		else{
			if(!Connect()){
				return;
			}
		}
	}


	this->Set_Database(database);
	this->Set_Table(tablename);
}


void MysqlTestSignal::CreateTable()
{

	char buf[2048];
	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
					"SingalName  CHAR(40) NOT NULL PRIMARY KEY, "
					"InstrumentID CHAR(20),"
					"ProductID CHAR(20),"
					"Section CHAR(20),"
					"Direction INT(11),"
					"Offset INT(11),"
					"Price DOUBLE,"
					"StopPrice DOUBLE,"
					"StopRange DOUBLE,"

					"Sedimentary DOUBLE,"
					"OpenPrice DOUBLE,"
					"PreSettlePrice DOUBLE,"
					"CloseOpenRate DOUBLE,"
					"PreSettleOpenRate DOUBLE,"


					"Volume INT(11),"
					"SignalCount INT(11),"
					"SingalActive BOOL,"
					"SignalActiveCount INT(11),"
					"TradingDay CHAR(20),"
					"SingalStartTime DATETIME,"
					"SingalEndTime DATETIME) default character set utf8 ",this->getTablename().c_str());
	std::string str_sql;
	str_sql=buf;
	this->Execute(str_sql);

}

void MysqlTestSignal::Insert(Signal data){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"INSERT INTO %s (SingalName,InstrumentID,ProductID,Section,Direction,Offset,Price,"
			"StopPrice,StopRange,Volume,SignalCount,SingalActive,SignalActiveCount,"
			"TradingDay,SingalStartTime,SingalEndTime,Sedimentary,OpenPrice,"
			"PreSettlePrice,CloseOpenRate,PreSettleOpenRate) "
			"VALUES ('%s','%s','%s','%s','%d','%d','%.3f',"
			"'%.3f','%.3f','%d','%d','%d','%d',"
			"'%s','%s','%s','%.3f','%.3f',"
			"'%.3f','%.2f','%.2f')",this->getTablename().c_str(),
			data.SingalName,data.InstrumentID,data.ProductID,data.Section,data.Direction,data.Direction,data.Price,
			data.StopPrice,data.StopRange,data.Volume,data.SignalCount,data.SingalActive,data.SignalActiveCount,
			data.TradingDay,data.SingalStartTime,data.SingalEndTime,data.Sedimentary,data.OpenPrice,
			data.PreSettlePrice,data.CloseOpenRate,data.PreSettleOpenRate);

	str_sql=buf;
	this->Execute(str_sql);
}

void MysqlTestSignal::Update(Signal data){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET SignalCount='%d',Price='%.3f',StopPrice='%.3f',StopRange='%.3f',CloseOpenRate='%.3f',"
			"PreSettleOpenRate='%.3f',SingalActive='%d',SignalActiveCount='%d',SingalEndTime='%s' "
			"WHERE SingalName='%s' ",this->getTablename().c_str(),
			data.SignalCount,data.Price,data.StopPrice,data.StopRange,data.CloseOpenRate,
			data.PreSettleOpenRate,	data.SingalActive,data.SignalActiveCount,data.SingalEndTime,
			data.SingalName);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}

bool  MysqlTestSignal::Exist_DataBySignalName(const char* signalname)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s'  ",this->getTablename().c_str(),signalname);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSignal::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSignal::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

bool  MysqlTestSignal::Exist_ActiveDataBySignalName(const char* signalname)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s' AND  SingalActive='%d'  ",this->getTablename().c_str(),signalname,true);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSignal::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSignal::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}
bool  MysqlTestSignal::Exist_Long_ActiveDataBySignalName(const char* signalname)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s' AND  SingalActive='%d'  AND Direction='%d' ",
			this->getTablename().c_str(),signalname,true,0);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSignal::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSignal::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

bool  MysqlTestSignal::Exist_Short_ActiveDataBySignalName(const char* signalname)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s' AND  SingalActive='%d'  AND Direction='%d' ",
			this->getTablename().c_str(),signalname,true,1);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSignal::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSignal::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

bool  MysqlTestSignal::Exist_Long_UnActiveDataBySignalNameTradingDay(const char* signalname,const char* tradingday)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE SingalName='%s' AND  SingalActive='%d'  AND TradingDay='%s'    AND Direction='%d' ",
			this->getTablename().c_str(),signalname,false,tradingday,0);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSignal::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSignal::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

bool  MysqlTestSignal::Exist_Short_UnActiveDataBySignalNameTradingDay(const char* signalname,const char* tradingday)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE SingalName='%s' AND  SingalActive='%d' AND TradingDay='%s'    AND Direction='%d' ",
			this->getTablename().c_str(),signalname,false,tradingday,1);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSignal::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSignal::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

Signal  MysqlTestSignal::Find_DataBySignalName(const char* signalname)
{
//	printf(" Find_DataBySignalName begin  \n ");
	Signal data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s'  ",this->getTablename().c_str(),signalname);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
//		printf(" count %d \n ",this->getRowCount());

		if(this->getRowCount()>0){
			this->getFirst();
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


		}
	}

	this->Empty();
	return data;
}


Signal  MysqlTestSignal::Find_ActiveDataBySignalNameDirection(const char* signalname,int direction)
{
//	printf(" Find_DataBySignalName begin  \n ");
	Signal data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s'  AND  SingalActive='%d'  AND Direction='%d'  ",
			this->getTablename().c_str(),signalname,true,direction);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
//		printf(" count %d \n ",this->getRowCount());

		if(this->getRowCount()>0){
			this->getFirst();
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


		}
	}

	this->Empty();
	return data;
}
Signal  MysqlTestSignal::Find_UnActiveDataBySignalNameDirectionTradingDay(const char* signalname,int direction,const char* tradingday)
{
//	printf(" Find_DataBySignalName begin  \n ");
	Signal data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  SingalName='%s'  AND  SingalActive='%d'  AND Direction='%d' AND TradingDay='%s'  ",
			this->getTablename().c_str(),signalname,false,direction,tradingday);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
//		printf(" count %d \n ",this->getRowCount());

		if(this->getRowCount()>0){
			this->getFirst();
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


		}
	}

	this->Empty();
	return data;
}
void MysqlTestSignal::Find_AllLists(vector<Signal> &temp)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}
void MysqlTestSignal::Find_AllListsbyTradingDay(vector<Signal> &temp,const char* tradingday)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s'",this->getTablename().c_str(),tradingday);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

void MysqlTestSignal::Find_AllListsActivebyTradingDay(vector<Signal> &temp,const char* tradingday)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND SingalActive ='%d' ",this->getTablename().c_str(),tradingday,true);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}
void MysqlTestSignal::Find_AllListsbySection(vector<Signal> &temp,const char* section)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE Section='%s'  Order by Sedimentary Desc Limit 1 ",this->getTablename().c_str(),section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}


void MysqlTestSignal::Find_AllListsActivebySection(vector<Signal> &temp,const char* section)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE Section='%s'  AND SingalActive='%d' Order by Sedimentary Desc Limit 1 ",
			this->getTablename().c_str(),section,true);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

void MysqlTestSignal::Find_Long_AllListsActivebySection(vector<Signal> &temp,const char* section)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE Section='%s'  AND SingalActive='%d' And Direction ='%d' Order by Sedimentary Desc Limit 1 ",
			this->getTablename().c_str(),section,true,0);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

void MysqlTestSignal::Find_Short_AllListsActivebySection(vector<Signal> &temp,const char* section)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE Section='%s'  AND SingalActive='%d' And Direction ='%d' Order by Sedimentary Desc Limit 1 ",
			this->getTablename().c_str(),section,true,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

int  MysqlTestSignal::Count_CurrentDay_Long_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section)
{

	int count=0;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT  count(SingalName) FROM %s  WHERE TradingDay='%s' AND  Section='%s'  AND SingalActive='%d' And Direction ='%d' "
				,this->getTablename().c_str(),tradingday,section,true,0);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
				count = getValueInt("count(SingalName)");
		}
	}

	this->Empty();

	return count;
}

int  MysqlTestSignal::Count_CurrentDay_Short_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section)
{

	int count=0;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT  count(SingalName) FROM %s  WHERE TradingDay='%s' AND  Section='%s'  AND SingalActive='%d' And Direction ='%d' "
				,this->getTablename().c_str(),tradingday,section,true,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
				count = getValueInt("count(SingalName)");
		}
	}

	this->Empty();

	return count;
}

void MysqlTestSignal::Find_CurrentDay_Long_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND  Section='%s'  AND SingalActive='%d' And Direction ='%d' "
			,this->getTablename().c_str(),tradingday,section,true,0);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}
void MysqlTestSignal::Find_CurrentDay_Short_AllListsActivebySection(vector<Signal> &temp,const char* tradingday,const char* section)
{
//		printf("Find_AllList begin\n");
	Signal data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND  Section='%s'  AND SingalActive='%d' And Direction ='%d' "
			,this->getTablename().c_str(),tradingday,section,true,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{

			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");


			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

double MysqlTestSignal::Sum_ActiveUpRatebySection(const char*tradingday,const char * section)
{
	double sum=0;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  sum(PreSettleOpenRate) FROM %s  WHERE TradingDay='%s' AND Section='%s'   AND SingalActive='%d' ",
			this->getTablename().c_str(),tradingday,section,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0){
			this->getFirst();
				sum = getValueInt("sum(PreSettleOpenRate)");
		}

	}
	this->Empty();
	return sum;
}

Signal MysqlTestSignal::Find_BigSedSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  Order by Sedimentary Desc Limit 1",
			this->getTablename().c_str(),tradingday,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}


Signal MysqlTestSignal::Find_Long_BigSedSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  And Direction ='%d'  Order by Sedimentary Desc Limit 1",
			this->getTablename().c_str(),tradingday,section,0);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}
Signal MysqlTestSignal::Find_Short_BigSedSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  And Direction ='%d'  Order by Sedimentary Desc Limit 1",
			this->getTablename().c_str(),tradingday,section,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}


Signal MysqlTestSignal::Find_Long_BigUpRateSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  And Direction ='%d'  Order by PreSettleOpenRate Desc Limit 1",
			this->getTablename().c_str(),tradingday,section,0);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}
Signal MysqlTestSignal::Find_Short_BigUpRateSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  And Direction ='%d'  Order by PreSettleOpenRate ASC Limit 1",
			this->getTablename().c_str(),tradingday,section,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}
Signal MysqlTestSignal::Find_Up_CloseOpenRateSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  Order by CloseOpenRate Desc Limit 1",
			this->getTablename().c_str(),tradingday,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}

Signal MysqlTestSignal::Find_Down_CloseOpenRateSignalbySection(const char*tradingday,const char * section)
{
	Signal data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE TradingDay='%s' AND Section='%s'  Order by CloseOpenRate ASC Limit 1",
			this->getTablename().c_str(),tradingday,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		if(this->getRowCount()>0)
		{
			strcpy(data.SingalName,getValueString("SingalName").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.ProductID,getValueString("ProductID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.Offset = getValueInt("Offset");
			data.Price = getValueDouble("Price");
			data.StopPrice = getValueDouble("StopPrice");
			data.StopRange = getValueDouble("StopRange");
			data.Volume = getValueInt("Volume");
			data.SignalCount = getValueInt("SignalCount");
			data.SingalActive = getValueInt("SingalActive");
			data.SignalActiveCount = getValueInt("SignalActiveCount");

			strcpy(data.TradingDay,getValueString("TradingDay").c_str());
			strcpy(data.SingalStartTime,getValueString("SingalStartTime").c_str());
			strcpy(data.SingalEndTime,getValueString("SingalEndTime").c_str());

			data.Sedimentary = getValueDouble("Sedimentary");
			data.OpenPrice=getValueDouble("OpenPrice");
			data.PreSettlePrice=getValueDouble("PreSettlePrice");
			data.CloseOpenRate = getValueDouble("CloseOpenRate");
			data.PreSettleOpenRate=getValueDouble("PreSettleOpenRate");

		}


	}
	this->Empty();
	return data;
}
