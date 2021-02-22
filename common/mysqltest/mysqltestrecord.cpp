#include "mysqltestrecord.hpp"

MysqlTestRecord::MysqlTestRecord(){
	std::string database="testrecord";

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
}


void MysqlTestRecord::CreateTable(const char* tablename)
{
	char buf[2048];
	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"ID CHAR(40) NOT NULL PRIMARY KEY, "
						"OpenDateTime DATETIME, "
						"InstrumentID CHAR(20),"
						"Section CHAR(20),"
						"Direction INT(11),"

						"AverageUpDownrate DOUBLE,"
						"pATR DOUBLE,"
						"PreSettlePrice DOUBLE,"
						"DayOpenPrice DOUBLE,"
						"HalfStop BOOL,"

						"OpenPrice DOUBLE,"
						"StopPrice DOUBLE,"
						"WinPrice DOUBLE,"
						"ClosePrice DOUBLE,"
						"Highest DOUBLE,"

			"Open_VolOpiRate DOUBLE,"
			"Open_AddOPIRate DOUBLE,"
			"Close_VolOpiRate DOUBLE,"
			"Close_AddOPIRate DOUBLE,"

						"Lowest DOUBLE,"
						"Volume INT(11),"
						"CloseVolume INT(11),"
						"Status INT(11),"
						"OpenOPI INT(11),"
						"CloseOPI INT(11),"

						"Sedimentary DOUBLE,"
						"AddOPIRate DOUBLE,"
						"Profit DOUBLE,"
						"Commission DOUBLE,"
						"Asset DOUBLE,"

						"Margin DOUBLE,"
						"UseMargin DOUBLE,"
						"UseMarginRate DOUBLE,"
						"OpenSignal CHAR(30),"
						"CloseSignal CHAR(30),"


						"OpenDate CHAR(30),"
						"CloseDate CHAR(30),"
						"FiveDayLineType CHAR(10),"
						"CloseDateTime DATETIME) default character set utf8 ",tablename);
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);
}


int MysqlTestRecord::CountTables()
{

	int count=0;
	char buf[2048];

	sprintf(buf,"SELECT CONCAT('drop table ',table_name,';') FROM information_schema.`TABLES` WHERE table_schema='testrecord' ");
	std::string str_sql;
	str_sql=buf;


	if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */


			count = this->getRowCount();
		}
	this->Empty();
	return count;
}

void MysqlTestRecord::DropAllTables()
{

	char buf[2048];

		sprintf(buf,"SELECT CONCAT('drop table ',table_name,';') FROM information_schema.`TABLES` WHERE table_schema='testrecord' ");
		std::string str_sql;
		str_sql=buf;


		if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();
				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
					{

						int index=1;
						std::string sql=getValueString(index);

						printf("sql--> %s  \n",sql.c_str());
						this->Execute(sql);

						this->getNext();
					}
			}
		this->Empty();

}

void MysqlTestRecord::DropPrefixTables(const char* prefix)
{

	char buf[2048];

		sprintf(buf,"SELECT CONCAT('drop table ',table_name,';') FROM information_schema.`TABLES` WHERE table_schema='testrecord' and table_name like '%s%%'",prefix);
		std::string str_sql;
		str_sql=buf;


		if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();
				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
					{

						int index=1;
						std::string sql=getValueString(index);

						printf("sql--> %s  \n",sql.c_str());
						this->Execute(sql);

						this->getNext();
					}
			}
		this->Empty();

}

void MysqlTestRecord::Insert(const char* tablename,TestRecordData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"INSERT INTO %s (ID,OpenDateTime,InstrumentID,Section,Direction,PreSettlePrice,DayOpenPrice,"
			"OpenPrice,StopPrice,Highest,Lowest,Volume,"
			"Status,OpenSignal,AverageUpDownrate,pATR,WinPrice,"
			"OpenOPI,Open_VolOpiRate,Open_AddOPIRate,Close_VolOpiRate,Close_AddOPIRate,"
			"OpenDate,Margin,UseMargin,UseMarginRate,CloseVolume,"
			"HalfStop,Commission,Sedimentary,FiveDayLineType) "
			"VALUES ('%s','%s','%s','%s','%d','%.3f','%.3f',"
			"'%.3f','%.3f','%.3f','%.3f','%d', "
			"'%d','%s','%.4f','%.3f','%.3f',"
			"'%d','%.4f','%.4f','%.4f','%.4f',"
			"'%s','%f','%.2f','%.2f','%d',"
			"'%d','%.2f','%.2f','%s')",tablename,
			data.ID,data.OpenDateTime,data.InstrumentID,data.Section,data.Direction,data.PreSettlePrice,data.DayOpenPrice,
			data.OpenPrice,data.StopPrice,data.Highest,data.Lowest,data.Volume,
			data.Status,data.OpenSignal,data.AverageUpDownrate,data.pATR,data.WinPrice,
			data.OpenOPI,data.Open_VolOpiRate,data.Open_AddOPIRate,data.Close_VolOpiRate,data.Close_AddOPIRate,
			data.OpenDate,data.Margin,data.UseMargin,data.UseMarginRate,0,
			false,data.Commission,data.Sedimentary,data.FiveDayLineType);

	str_sql=buf;

	this->Execute(str_sql);


}



void MysqlTestRecord::InsertTotalTable(const char* tablename,TestRecordData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"INSERT INTO %s (OpenDateTime,InstrumentID,Direction,CloseDateTime,Status,"
			"Profit) "
			"VALUES ('%s','%s','%d','%s','%d','%.3f')",tablename,
			data.OpenDateTime,data.InstrumentID,data.Direction,data.CloseDateTime,data.Status,
			data.Profit);

	str_sql=buf;

	this->Execute(str_sql);


}

void MysqlTestRecord::Update(const char* tablename,TestRecordData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET CloseDateTime='%s',ClosePrice='%.3f',CloseVolume='%d',Status='%d',Profit='%.2f',"
			"Asset ='%.2f' ,CloseSignal ='%s' ,tr.HalfStop='%d'"
			"WHERE OpenDateTime='%s' ",tablename,
			data.CloseDateTime,data.ClosePrice,data.CloseVolume,data.Status,data.Profit,
			data.Asset,data.CloseSignal,data.HalfStop,data.OpenDateTime);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateRef(const char* tablename,TestRecordData data)
{


	char buf[2048];
	std::string str_sql;



//	sprintf(buf,"UPDATE %s SET EMA4='%.3f',EMA9='%.3f',EMA50='%.3f',pATR='%.3f',EMA50_X ='%.3f' ,EMA9_X ='%.3f' "
//			"WHERE OpenDateTime='%s' ",tablename,
//			data.EMA4,data.EMA9,data.EMA50,data.pATR,data.EMA50_X,data.EMA9_X,data.OpenDateTime);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}


void MysqlTestRecord::UpdatebyInstrument(const char* tablename,const char* pinstrumentid,TestRecordData data)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET CloseDateTime='%s',ClosePrice='%.3f',CloseVolume='%d',CloseOPI='%d',AddOPIRate='%.3f',"
			" Status='%d',Profit='%.2f',Asset ='%.2f' ,CloseSignal ='%s',HalfStop='%d' ,Commission='%.3f' "
			"WHERE OpenDateTime='%s' AND InstrumentID='%s' ",tablename,
			data.CloseDateTime,data.ClosePrice,data.CloseVolume,data.CloseOPI,data.AddOPIRate,
			data.Status,data.Profit,data.Asset,data.CloseSignal,data.HalfStop,data.Commission,data.OpenDateTime,pinstrumentid);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void MysqlTestRecord::UpdatebyCloseInstrument(const char* tablename,const char* pinstrumentid,TestRecordData *data)
{
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"UPDATE %s SET CloseDateTime='%s',ClosePrice='%.3f',CloseVolume='%d',CloseOPI='%d',AddOPIRate='%.3f',"
			" Status='%d',Profit='%.2f',CloseSignal ='%s',HalfStop='%d' ,Commission='%.3f' "
			"WHERE OpenDateTime='%s' AND InstrumentID='%s' ",tablename,
			data->CloseDateTime,data->ClosePrice,data->CloseVolume,data->CloseOPI,data->AddOPIRate,
			data->Status,data->Profit,data->CloseSignal,data->HalfStop,data->Commission,data->OpenDateTime,pinstrumentid);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void MysqlTestRecord::UpdateByStatus(const char* tablename,TestRecordData data,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET CloseDateTime='%s',ClosePrice='%.3f',CloseOPI='%d',Status='%d',Profit='%.3f' "
			"WHERE Status='%d' ",tablename,
			data.CloseDateTime,data.ClosePrice,data.CloseOPI,data.Status,data.Profit,status);
//
//	printf("UPDATE %s SET CloseDateTime='%s',ClosePrice='%.3f',Status='%d',Profit='%.3f' "
//				"WHERE Status='%d' \n",tablename,
//				data.CloseDateTime,data.ClosePrice,data.Status,data.Profit,status);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateLowestByStatus(const char* tablename,double lowest,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Lowest='%.3f' WHERE Status='%d' ",tablename, lowest, status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateHighestByStatus(const char* tablename,double highest,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Highest='%.3f' WHERE Status='%d' ",tablename, highest, status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateLowestByInstrumentStatus(const char* tablename,const char* pinstrumentid,double lowest,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Lowest='%.3f'  "
			" WHERE InstrumentID = '%s' AND Status='%d' ",tablename,lowest,  pinstrumentid,status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void MysqlTestRecord::UpdateHighestByInstrumentStatus(const char* tablename,const char* pinstrumentid,double highest,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Highest='%.3f' WHERE InstrumentID = '%s' AND Status='%d' ",tablename, highest, pinstrumentid,status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateProfit(const char* tablename,TestRecordData data)
{


	char buf[2048];
	std::string str_sql;

	if(data.Direction ==0)
	{
		sprintf(buf,"UPDATE %s SET  Highest='%.3f',StopPrice='%.3f' ,Profit='%.2f'  "
					" WHERE InstrumentID = '%s' AND Status='%d' ",tablename,data.Highest, data.StopPrice,data.Profit,data.InstrumentID,data.Status);
	}
	else if(data.Direction ==1)
	{
		sprintf(buf,"UPDATE %s SET  Lowest='%.3f',StopPrice='%.3f',Profit='%.2f'  "
					" WHERE InstrumentID = '%s' AND Status='%d' ",tablename,data.Lowest, data.StopPrice,data.Profit,data.InstrumentID,data.Status);
	}





	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}


void MysqlTestRecord::UpdateProfitbyOpenDate(const char* tablename,TestRecordData data)
{

	char buf[2048];
	std::string str_sql;

	if(data.Direction ==0)	{
		sprintf(buf,"UPDATE %s SET  Highest='%.3f',StopPrice='%.3f' ,Profit='%.2f'  "
					" WHERE InstrumentID = '%s' AND Status='%d' ",tablename,data.Highest, data.StopPrice,data.Profit,data.InstrumentID,data.Status);
	}
	else if(data.Direction ==1)	{
		sprintf(buf,"UPDATE %s SET  Lowest='%.3f',StopPrice='%.3f',Profit='%.2f'  "
					" WHERE InstrumentID = '%s' AND Status='%d' ",tablename,data.Lowest, data.StopPrice,data.Profit,data.InstrumentID,data.Status);
	}

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void MysqlTestRecord::UpdateLowStopPriceByInstrumentStatus(const char* tablename,const char* pinstrumentid,double lowest,double stopprice,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Lowest='%.3f',StopPrice='%.3f' WHERE InstrumentID = '%s' AND Status='%d' ",tablename,lowest,stopprice,  pinstrumentid,status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateHighStopPriceByInstrumentStatus(const char* tablename,const char* pinstrumentid,double highest,double stopprice,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Highest='%.3f',StopPrice='%.3f' WHERE InstrumentID = '%s' AND Status='%d' ",tablename, highest,stopprice, pinstrumentid,status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateLowStopWinPriceByInstrumentStatus(const char* tablename,const char* pinstrumentid,double lowest,double stopprice,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Lowest='%.3f',WinPrice='%.3f' WHERE InstrumentID = '%s' AND Status='%d' ",tablename,lowest,stopprice,  pinstrumentid,status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlTestRecord::UpdateHighStopWinPriceByInstrumentStatus(const char* tablename,const char* pinstrumentid,double highest,double stopprice,int status)
{


	char buf[2048];
	std::string str_sql;



	sprintf(buf,"UPDATE %s SET  Highest='%.3f',WinPrice='%.3f' WHERE InstrumentID = '%s' AND Status='%d' ",tablename, highest,stopprice, pinstrumentid,status);


	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
bool  MysqlTestRecord::Exist_RecordByOpenDateTime(const char* tablename,const char *opendatetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  OpenDateTime='%s'  ",tablename,opendatetime);

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


bool  MysqlTestRecord::Exist_RecordByStatus(const char* tablename,int status)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Status='%d'  ",tablename,status);

//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestRecord::Exist_RecordByStatus getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestRecord::Exist_RecordByStatus [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}


bool  MysqlTestRecord::Exist_RecordByInstrumentStatus(const char* tablename,const char*pinstrumentid,int status)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  InstrumentID='%s' AND Status='%d'  ",tablename,pinstrumentid,status);

//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestRecord::Exist_RecordByStatus getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestRecord::Exist_RecordByStatus [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}


bool  MysqlTestRecord::Exist_RecordByOpenDateInstrument(const char* tablename,const char*opendate,const char*pinstrumentid)
{
	bool ret=false;

	char buf[2048];
	std::string str_sql;

	std::string id =opendate;
	id.append("_");
	id.append(pinstrumentid);
	sprintf(buf,"SELECT * FROM %s WHERE  ID='%s' ",tablename,id.c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
//		printf("MysqlTestRecord::Exist_RecordByOpenDateInstrument getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)	{
//			printf("MysqlTestRecord::Exist_RecordByOpenDateInstrument [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}
	}

	this->Empty();

	return ret;
}

int  MysqlTestRecord::Count_RecordByStatus(const char* tablename,int status)
{

	int  count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Status='%d'  ",tablename,status);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		count=this->getRowCount();
	}

	this->Empty();

	return count;
}
int  MysqlTestRecord::Count_RecordBySectionStatus(const char* tablename,const char*section,int status)
{

	int  count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Section='%s' AND  Status='%d'  ",tablename,section,status);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		count=this->getRowCount();
	}

	this->Empty();

	return count;
}

int  MysqlTestRecord::Count_RecordByTradingDaySection(const char* tablename,const char*section,const char* tradingday)
{

	int  count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Section='%s' AND  OpenDate='%s'  ",tablename,section,tradingday);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		count=this->getRowCount();
	}

	this->Empty();

	return count;
}


int  MysqlTestRecord::Count_RecordBySection(const char* tablename,const char*section)
{

	int count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(ID) FROM %s WHERE  Section='%s' AND  Status='1' ",tablename,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
				count = getValueInt("count(ID)");
		}
	}

	this->Empty();

	return count;
}


int  MysqlTestRecord::Count_WinRecordBySection(const char* tablename,const char*section)
{

	int count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(ID) FROM %s WHERE  Section='%s' AND  Status='1' AND Profit>'0' ",tablename,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
				count = getValueInt("count(ID)");
		}
	}

	this->Empty();

	return count;
}


int  MysqlTestRecord::Count_LoseRecordBySection(const char* tablename,const char*section)
{

	int count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(ID) FROM %s WHERE  Section='%s' AND  Status='1' AND Profit<='0' ",tablename,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
				count = getValueInt("count(ID)");
		}
	}

	this->Empty();

	return count;
}

double  MysqlTestRecord::SumProfit_RecordBySection(const char* tablename,const char*section)
{

	double totalprofit=0.0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT sum(profit) FROM %s WHERE  Section='%s' AND  Status='1' ",tablename,section);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0){
			this->getFirst();
			totalprofit = getValueDouble("sum(profit)");
		}
	}

	this->Empty();

	return totalprofit;
}



TestRecordData  MysqlTestRecord::Find_RecordByStatus(const char* tablename,int status)
{
//	printf(" Find_RecordByStatus begin  \n ");
	TestRecordData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Status='%d'  ",tablename,status);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf(" count %d \n ",this->getRowCount());

		if(this->getRowCount()>0)
		{
			this->getFirst();
			strcpy(data.ID,getValueString("ID").c_str());

			strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.AverageUpDownrate = getValueDouble("AverageUpDownrate");
			data.pATR = getValueDouble("pATR");
			data.PreSettlePrice = getValueDouble("PreSettlePrice");
			 data.DayOpenPrice = getValueDouble("DayOpenPrice");


			 data.OpenPrice = getValueDouble("OpenPrice");
			 data.ClosePrice = getValueDouble("ClosePrice");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.WinPrice = getValueDouble("WinPrice");

			 data.Highest = getValueDouble("Highest");

			 data.OpenOPI = getValueInt("OpenOPI");
			 data.CloseOPI = getValueInt("CloseOPI");
			 data.AddOPIRate = getValueDouble("AddOPIRate");
			 data.Sedimentary = getValueDouble("Sedimentary");
//			 printf("Find_RecordByStatus --> Highest %.3f\n",data.Highest);

			 data.Lowest = getValueDouble("Lowest");
			 data.Volume = getValueInt("Volume");
			 data.CloseVolume = getValueInt("CloseVolume");
			data.Status = getValueInt("Status");

			data.Margin = getValueDouble("Margin");
			data.UseMargin = getValueDouble("UseMargin");
			data.UseMarginRate = getValueDouble("UseMarginRate");

			strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());
			strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
			strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());
			strcpy(data.FiveDayLineType,getValueString("FiveDayLineType").c_str());
			 data.HalfStop = getValueInt("HalfStop");
			 data.Commission = getValueDouble("Commission");
		}


	}

	this->Empty();

	return data;
}
TestRecordData  MysqlTestRecord::Find_RecordByInstrumentStatus(const char* tablename,const char*pinstrumentid,int status)
{
//	printf(" Find_RecordByStatus begin  \n ");
	TestRecordData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Instrumentid='%s' AND Status='%d'  ",tablename,pinstrumentid,status);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf(" count %d \n ",this->getRowCount());

		if(this->getRowCount()>0)
		{
			this->getFirst();
			strcpy(data.ID,getValueString("ID").c_str());
			strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.AverageUpDownrate = getValueDouble("AverageUpDownrate");
			data.pATR = getValueDouble("pATR");
			data.PreSettlePrice = getValueDouble("PreSettlePrice");
			 data.DayOpenPrice = getValueDouble("DayOpenPrice");

			 data.OpenPrice = getValueDouble("OpenPrice");
			 data.ClosePrice = getValueDouble("ClosePrice");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.Highest = getValueDouble("Highest");
			 data.WinPrice = getValueDouble("WinPrice");
//			 printf("Find_RecordByStatus --> Highest %.3f\n",data.Highest);
			 data.Profit = getValueDouble("Profit");

			 data.Sedimentary = getValueDouble("Sedimentary");
			 data.OpenOPI = getValueInt("OpenOPI");
			 data.CloseOPI = getValueInt("CloseOPI");
			 data.AddOPIRate = getValueDouble("AddOPIRate");
			 data.Lowest = getValueDouble("Lowest");
			 data.Volume = getValueInt("Volume");
			 data.CloseVolume = getValueInt("CloseVolume");
			data.Status = getValueInt("Status");
			data.Margin = getValueDouble("Margin");
			data.UseMargin = getValueDouble("UseMargin");
			data.UseMarginRate = getValueDouble("UseMarginRate");

			strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());
			strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
			strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());
			strcpy(data.FiveDayLineType,getValueString("FiveDayLineType").c_str());
			data.HalfStop = getValueInt("HalfStop");
			 data.Commission = getValueDouble("Commission");
		}


	}

	this->Empty();

	return data;
}

TestRecordData  MysqlTestRecord::Find_LastRecordByStatus(const char* tablename,int status)
{

	TestRecordData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Status='%d' ORDER BY OpenDateTime DESC LIMIT 1  ",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

			strcpy(data.ID,getValueString("ID").c_str());
			strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
			strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
			strcpy(data.Section,getValueString("Section").c_str());
			data.Direction = getValueInt("Direction");
			data.AverageUpDownrate = getValueDouble("AverageUpDownrate");
			data.pATR = getValueDouble("pATR");
			data.PreSettlePrice = getValueDouble("PreSettlePrice");
			 data.DayOpenPrice = getValueDouble("DayOpenPrice");


			 data.OpenPrice = getValueDouble("OpenPrice");
			 data.ClosePrice = getValueDouble("ClosePrice");
			 data.StopPrice = getValueDouble("StopPrice");
			 data.WinPrice = getValueDouble("WinPrice");
			 data.Highest = getValueDouble("Highest");
			 data.Lowest = getValueDouble("Lowest");
			 data.Volume = getValueInt("Volume");
			 data.CloseVolume = getValueInt("CloseVolume");
			data.Status = getValueInt("Status");
			data.Profit = getValueDouble("Profit");
			data.Asset = getValueDouble("Asset");
			data.Sedimentary = getValueDouble("Sedimentary");
			 data.OpenOPI = getValueInt("OpenOPI");
			 data.CloseOPI = getValueInt("CloseOPI");
			 data.AddOPIRate = getValueDouble("AddOPIRate");
			 data.Margin = getValueDouble("Margin");
			 data.UseMargin = getValueDouble("UseMargin");
			data.UseMarginRate = getValueDouble("UseMarginRate");

			strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());
			strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
			strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());
			strcpy(data.FiveDayLineType,getValueString("FiveDayLineType").c_str());
			data.HalfStop = getValueInt("HalfStop");
			 data.Commission = getValueDouble("Commission");
		}


	}

	this->Empty();

	return data;
}

void MysqlTestRecord::Find_AllLists(vector<TestRecordData> &temp,const char* tablename)
{


	TestRecordData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ", tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
				strcpy(data.ID,getValueString("ID").c_str());
				strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
				strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				strcpy(data.Section,getValueString("Section").c_str());
			 	data.Direction = getValueInt("Direction");
			 	data.AverageUpDownrate = getValueDouble("AverageUpDownrate");
			 	data.pATR = getValueDouble("pATR");
			 	data.PreSettlePrice = getValueDouble("PreSettlePrice");
				 data.DayOpenPrice = getValueDouble("DayOpenPrice");


			 	 data.OpenPrice = getValueDouble("OpenPrice");
			 	 data.ClosePrice = getValueDouble("ClosePrice");
			 	data.StopPrice = getValueDouble("StopPrice");
			 	data.WinPrice = getValueDouble("WinPrice");
			 	 data.Volume = getValueInt("Volume");
			 	data.CloseVolume = getValueInt("CloseVolume");
			 	data.Status = getValueInt("Status");

			 	 data.Highest = getValueDouble("Highest");
				 data.Lowest = getValueDouble("Lowest");

				 data.Open_VolOpiRate = getValueDouble("Open_VolOpiRate");
				 data.Open_AddOPIRate = getValueDouble("Open_AddOPIRate");
				 data.Close_VolOpiRate = getValueDouble("Close_VolOpiRate");
				 data.Close_AddOPIRate = getValueDouble("Close_AddOPIRate");

			 	data.Profit = getValueDouble("Profit");
			 	data.Commission = getValueDouble("Commission");
			 	data.Asset = getValueDouble("Asset");
			 	data.Sedimentary = getValueDouble("Sedimentary");
			 	 data.OpenOPI = getValueInt("OpenOPI");
				 data.CloseOPI = getValueInt("CloseOPI");
				 data.AddOPIRate = getValueDouble("AddOPIRate");
				 data.Margin = getValueDouble("Margin");
				 data.UseMargin = getValueDouble("UseMargin");
				data.UseMarginRate = getValueDouble("UseMarginRate");

				strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());

				strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
				strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());
				strcpy(data.FiveDayLineType,getValueString("FiveDayLineType").c_str());
				data.HalfStop = getValueInt("HalfStop");
				 data.Commission = getValueDouble("Commission");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

void MysqlTestRecord::Find_AllListsbyStatus(vector<TestRecordData> &temp,const char* tablename)
{


	TestRecordData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s Where Status='%d' ", tablename,1);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
			strcpy(data.ID,getValueString("ID").c_str());
				strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
				strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				strcpy(data.Section,getValueString("Section").c_str());
			 	data.Direction = getValueInt("Direction");
			 	data.AverageUpDownrate = getValueDouble("AverageUpDownrate");
			 	data.pATR = getValueDouble("pATR");
			 	data.PreSettlePrice = getValueDouble("PreSettlePrice");
				 data.DayOpenPrice = getValueDouble("DayOpenPrice");


			 	 data.OpenPrice = getValueDouble("OpenPrice");
			 	 data.ClosePrice = getValueDouble("ClosePrice");
			 	data.StopPrice = getValueDouble("StopPrice");
			 	data.WinPrice = getValueDouble("WinPrice");
			 	 data.Volume = getValueInt("Volume");
			 	data.CloseVolume = getValueInt("CloseVolume");
			 	data.Status = getValueInt("Status");

			 	 data.Highest = getValueDouble("Highest");
				 data.Lowest = getValueDouble("Lowest");

			 	data.Profit = getValueDouble("Profit");
			 	data.Commission = getValueDouble("Commission");
			 	data.Asset = getValueDouble("Asset");
			 	data.Sedimentary = getValueDouble("Sedimentary");
			 	 data.OpenOPI = getValueInt("OpenOPI");
				 data.CloseOPI = getValueInt("CloseOPI");
				 data.AddOPIRate = getValueDouble("AddOPIRate");
				 data.Margin = getValueDouble("Margin");
				 data.UseMargin = getValueDouble("UseMargin");
				data.UseMarginRate = getValueDouble("UseMarginRate");

				strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());

				strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
				strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());
				strcpy(data.FiveDayLineType,getValueString("FiveDayLineType").c_str());
				data.HalfStop = getValueInt("HalfStop");
				 data.Commission = getValueDouble("Commission");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}

void MysqlTestRecord::Find_HoldLists(vector<TestRecordData> &temp,const char* tablename)
{


	TestRecordData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE  Status='0' ", tablename);

//	printf("  Find_HoldLists    sql:%s\n",buf);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
//			printf("Find_HoldLists   %d     %d   \n",i,this->getRowCount());
			strcpy(data.ID,getValueString("ID").c_str());
				strcpy(data.OpenDateTime,getValueString("OpenDateTime").c_str());
				strcpy(data.InstrumentID,getValueString("InstrumentID").c_str());
				strcpy(data.Section,getValueString("Section").c_str());
			 	data.Direction = getValueInt("Direction");
			 	data.AverageUpDownrate = getValueDouble("AverageUpDownrate");
			 	data.pATR = getValueDouble("pATR");
			 	 data.PreSettlePrice = getValueDouble("PreSettlePrice");
			 	 data.DayOpenPrice = getValueDouble("DayOpenPrice");



			 	 data.OpenPrice = getValueDouble("OpenPrice");
			 	 data.ClosePrice = getValueDouble("ClosePrice");
			 	data.StopPrice = getValueDouble("StopPrice");
			 	data.WinPrice = getValueDouble("WinPrice");
			 	 data.Volume = getValueInt("Volume");
			 	data.CloseVolume = getValueInt("CloseVolume");
			 	data.Status = getValueInt("Status");

			 	 data.Highest = getValueDouble("Highest");
				 data.Lowest = getValueDouble("Lowest");

			 	data.Profit = getValueDouble("Profit");
			 	data.Commission = getValueDouble("Commission");
			 	data.Asset = getValueDouble("Asset");
			 	data.Sedimentary = getValueDouble("Sedimentary");
			 	 data.OpenOPI = getValueInt("OpenOPI");
				 data.CloseOPI = getValueInt("CloseOPI");
				 data.AddOPIRate = getValueDouble("AddOPIRate");
				 data.Margin = getValueDouble("Margin");
				 data.UseMargin = getValueDouble("UseMargin");
				data.UseMarginRate = getValueDouble("UseMarginRate");

				strcpy(data.CloseDateTime,getValueString("CloseDateTime").c_str());

				strcpy(data.OpenSignal,getValueString("OpenSignal").c_str());
				strcpy(data.CloseSignal,getValueString("CloseSignal").c_str());
				strcpy(data.FiveDayLineType,getValueString("FiveDayLineType").c_str());
				data.HalfStop = getValueInt("HalfStop");
				 data.Commission = getValueDouble("Commission");
				  temp.push_back(data);
				  this->getNext();
			}
	}
	this->Empty();

}
