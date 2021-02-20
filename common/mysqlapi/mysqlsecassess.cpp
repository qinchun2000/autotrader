#include "mysqlsecassess.hpp"

MysqlSecAssess::MysqlSecAssess(const char* userid)
{
	std::string database="account";
	std::string tablename="secassess_";
	tablename.append(userid);

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


void MysqlSecAssess::CreateTable()
{

	if(!this->Exist_Table(this->getTablename().c_str()))	{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
				"Section  CHAR(40) NOT NULL PRIMARY KEY, "
				"TotalProductNumber INT,"   // 该板块品种总数
				"ThreshHoldNumber INT,"  // 该板块信号数 超过该值 则可以在该板块开仓
				"ProductRiskNumber INT,"  // 该板块可以开仓的品种数

				"ProductHoldNumber INT,"  // 该板块目前有持仓的品种数

				"PD_Long_SignalNumber INT,"  // 该板块当日出现多头信号的品种数
				"PD_Short_SignalNumber INT,"  // 该板块当日出现空头信号的品种数
				"PD_Long_InstrumentId  CHAR(40),"  // 当多头信号为2个以上时 此字段将优选合约赋值
				"PD_Short_InstrumentId  CHAR(40),"// 当空头信号为2个以上时 此字段将优选合约赋值
				"PD_Long_Inst_UpdownRate  DOUBLE,"  // 当多头信号为2个以上时 此字段将优选合约对应的涨幅
				"PD_Short_Inst_UpdownRate  DOUBLE,"// 当空头信号为2个以上时 此字段将优选合约对应的跌幅
				"InsHoldNumber INT,"  //该板块目前合约总持仓手数
				"Priority INT,"  //板块默认优先级
				"UpdateTime DATETIME) ENGINE=MEMORY  default character set utf8 ",this->getTablename().c_str());
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);
	}

}


void MysqlSecAssess::Insert(SectionAssessData data){
	//	printf("insert begin\n");

		char buf[2048];
		std::string str_sql;
		DateUtil dateutil;
		std::string time =dateutil.ConvertCurrentTimetoSqlTime();

		sprintf(buf,"INSERT INTO %s (Section,TotalProductNumber,ThreshHoldNumber,ProductRiskNumber,ProductHoldNumber,PD_Long_SignalNumber,"
					"PD_Short_SignalNumber,InsHoldNumber,Priority,UpdateTime) "
					"VALUES ('%s','%d','%d','%d','%d','%d',"
					"'%d','%d','%d','%s')",this->getTablename().c_str(),
					data.Section,data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber,data.ProductHoldNumber,data.PD_Long_SignalNumber,
					data.PD_Short_SignalNumber,	data.InsHoldNumber,data.Priority,data.UpdateTime);
		str_sql=buf;
		this->Execute(str_sql);
}


void MysqlSecAssess::Update(SectionAssessData data){
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET ProductHoldNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			data.ProductHoldNumber,data.Section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);



}
void MysqlSecAssess::UpdateProductHoldNumber(int holdnumber,const char* section){
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET ProductHoldNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			holdnumber,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}

void MysqlSecAssess::UpdateSignalNumber(SectionAssessData data){
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,"UPDATE %s SET PD_Long_SignalNumber='%d',PD_Short_SignalNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			data.PD_Long_SignalNumber,data.PD_Short_SignalNumber,data.Section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void MysqlSecAssess::UpdateLongSectionRecommend(const char* section,int signalnumber,const char*instrumentid,double updownrate){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Long_SignalNumber='%d',PD_Long_InstrumentId='%s',PD_Long_Inst_UpdownRate='%.3f'  "
			"WHERE Section='%s' ",this->getTablename().c_str(),	signalnumber,instrumentid,updownrate,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
void MysqlSecAssess::UpdateShortSectionRecommend(const char* section,int signalnumber,const char*instrumentid,double updownrate){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Short_SignalNumber='%d',PD_Short_InstrumentId='%s',PD_Short_Inst_UpdownRate='%.3f'  "
			"WHERE Section='%s' ",this->getTablename().c_str(),	signalnumber,instrumentid,updownrate,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
void MysqlSecAssess::UpdateLongProductDaySignalNumber(int signalnumber,const char* section){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Long_SignalNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			signalnumber,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
void MysqlSecAssess::UpdateLongInstrumentId(int signalnumber,const char* section,const char* instrument){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Long_InstrumentId='%s' WHERE Section='%s' ",this->getTablename().c_str(),
			instrument,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}

void MysqlSecAssess::UpdateShortProductDaySignalNumber(int signalnumber,const char* section){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Short_SignalNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			signalnumber,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
void MysqlSecAssess::UpdateShortInstrumentId(int signalnumber,const char* section,const char* instrument){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Short_InstrumentId='%s' WHERE Section='%s' ",this->getTablename().c_str(),
			instrument,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
bool  MysqlSecAssess::Exist_DataBySection(const char* section)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Section='%s'  ",this->getTablename().c_str(),section);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
//		printf("MysqlTestSecAssess::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)	{
//			printf("MysqlTestSecAssess::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

SectionAssessData  MysqlSecAssess::Find_DataBySection(const char* section)
{
//	printf(" Find_DataBySignalName begin  \n ");
	SectionAssessData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Section='%s'  ",this->getTablename().c_str(),section);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
//		printf(" count %d \n ",this->getRowCount());

		if(this->getRowCount()>0){
			this->getFirst();
			strcpy(data.Section,getValueString("Section").c_str());

			data.TotalProductNumber = getValueInt("TotalProductNumber");
			data.ThreshHoldNumber =  getValueInt("ThreshHoldNumber");
			data.ProductRiskNumber = getValueInt("ProductRiskNumber");
			data.ProductHoldNumber = getValueInt("ProductHoldNumber");
			data.PD_Long_SignalNumber = getValueInt("PD_Long_SignalNumber");
			data.PD_Short_SignalNumber = getValueInt("PD_Short_SignalNumber");
			strcpy(data.PD_Long_InstrumentId,getValueString("PD_Long_InstrumentId").c_str());
			strcpy(data.PD_Short_InstrumentId,getValueString("PD_Short_InstrumentId").c_str());
			data.PD_Long_Inst_UpdownRate = getValueDouble("PD_Long_Inst_UpdownRate");
			data.PD_Short_Inst_UpdownRate = getValueDouble("PD_Short_Inst_UpdownRate");
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			data.Priority = getValueInt("Priority");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());
		}
	}

	this->Empty();
	return data;
}

void MysqlSecAssess::Find_AllLists(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			SectionAssessData data;
			strcpy(data.Section,getValueString("Section").c_str());
			data.TotalProductNumber = getValueInt("TotalProductNumber");
			data.ThreshHoldNumber =  getValueInt("ThreshHoldNumber");
			data.ProductRiskNumber = getValueInt("ProductRiskNumber");
			data.ProductHoldNumber = getValueInt("ProductHoldNumber");
			data.PD_Long_SignalNumber = getValueInt("PD_Long_SignalNumber");
			data.PD_Short_SignalNumber = getValueInt("PD_Short_SignalNumber");
			strcpy(data.PD_Long_InstrumentId,getValueString("PD_Long_InstrumentId").c_str());
			strcpy(data.PD_Short_InstrumentId,getValueString("PD_Short_InstrumentId").c_str());
			data.PD_Long_Inst_UpdownRate = getValueDouble("PD_Long_Inst_UpdownRate");
			data.PD_Short_Inst_UpdownRate = getValueDouble("PD_Short_Inst_UpdownRate");
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			data.Priority = getValueInt("Priority");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

void MysqlSecAssess::Find_OpenLists(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");
	SectionAssessData data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  where ProductRiskNumber >0  ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			strcpy(data.Section,getValueString("Section").c_str());
			data.TotalProductNumber = getValueInt("TotalProductNumber");
			data.ThreshHoldNumber =  getValueInt("ThreshHoldNumber");
			data.ProductRiskNumber = getValueInt("ProductRiskNumber");
			data.ProductHoldNumber = getValueInt("ProductHoldNumber");
			data.PD_Long_SignalNumber = getValueInt("PD_Long_SignalNumber");
			data.PD_Short_SignalNumber = getValueInt("PD_Short_SignalNumber");
			strcpy(data.PD_Long_InstrumentId,getValueString("PD_Long_InstrumentId").c_str());
			strcpy(data.PD_Short_InstrumentId,getValueString("PD_Short_InstrumentId").c_str());
			data.PD_Long_Inst_UpdownRate = getValueDouble("PD_Long_Inst_UpdownRate");
			data.PD_Short_Inst_UpdownRate = getValueDouble("PD_Short_Inst_UpdownRate");
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			data.Priority = getValueInt("Priority");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}
void MysqlSecAssess::Find_Long_AllListsBySingalNumber(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");
	SectionAssessData data;
	char buf[1024];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE PD_Long_SignalNumber>0 AND PD_Short_SignalNumber=0 AND "
			"ProductRiskNumber>ProductHoldNumber ORDER BY PD_Long_SignalNumber DESC ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			strcpy(data.Section,getValueString("Section").c_str());
			data.TotalProductNumber = getValueInt("TotalProductNumber");
			data.ThreshHoldNumber =  getValueInt("ThreshHoldNumber");
			data.ProductRiskNumber = getValueInt("ProductRiskNumber");
			data.ProductHoldNumber = getValueInt("ProductHoldNumber");
			data.PD_Long_SignalNumber = getValueInt("PD_Long_SignalNumber");
			data.PD_Short_SignalNumber = getValueInt("PD_Short_SignalNumber");
			strcpy(data.PD_Long_InstrumentId,getValueString("PD_Long_InstrumentId").c_str());
			strcpy(data.PD_Short_InstrumentId,getValueString("PD_Short_InstrumentId").c_str());
			data.PD_Long_Inst_UpdownRate = getValueDouble("PD_Long_Inst_UpdownRate");
			data.PD_Short_Inst_UpdownRate = getValueDouble("PD_Short_Inst_UpdownRate");
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			data.Priority = getValueInt("Priority");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

void MysqlSecAssess::Find_Short_AllListsBySingalNumber(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");

	char buf[1024];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE PD_Short_SignalNumber>0 AND PD_Long_SignalNumber=0 "
			"AND ProductRiskNumber>ProductHoldNumber ORDER BY PD_Short_SignalNumber DESC ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			SectionAssessData data;
			strcpy(data.Section,getValueString("Section").c_str());
			data.TotalProductNumber = getValueInt("TotalProductNumber");
			data.ThreshHoldNumber =  getValueInt("ThreshHoldNumber");
			data.ProductRiskNumber = getValueInt("ProductRiskNumber");
			data.ProductHoldNumber = getValueInt("ProductHoldNumber");
			data.PD_Long_SignalNumber = getValueInt("PD_Long_SignalNumber");
			data.PD_Short_SignalNumber = getValueInt("PD_Short_SignalNumber");
			strcpy(data.PD_Long_InstrumentId,getValueString("PD_Long_InstrumentId").c_str());
			strcpy(data.PD_Short_InstrumentId,getValueString("PD_Short_InstrumentId").c_str());
			data.PD_Long_Inst_UpdownRate = getValueDouble("PD_Long_Inst_UpdownRate");
			data.PD_Short_Inst_UpdownRate = getValueDouble("PD_Short_Inst_UpdownRate");
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			data.Priority = getValueInt("Priority");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

int  MysqlSecAssess::SumLongSignalNumber()
{
	int sum=0;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT SUM(PD_Long_SignalNumber) FROM %s  WHERE PD_Long_SignalNumber >0 AND PD_Short_SignalNumber=0 ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
			sum = getValueInt("SUM(PD_Long_SignalNumber)");
		}
	}

	this->Empty();

	return sum;
}

int  MysqlSecAssess::SumShortSignalNumber()
{
	int sum=0;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT SUM(PD_Short_SignalNumber) FROM %s WHERE PD_Long_SignalNumber =0 AND PD_Short_SignalNumber>0 ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
			sum = getValueInt("SUM(PD_Short_SignalNumber)");
		}
	}

	this->Empty();

	return sum;
}

int  MysqlSecAssess::CountLongSignalNumber()
{
	int count=0;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT COUNT(PD_Long_SignalNumber) FROM %s  WHERE PD_Long_SignalNumber >0 AND PD_Short_SignalNumber=0 ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
			count = getValueInt("COUNT(PD_Long_SignalNumber)");
		}
	}

	this->Empty();

	return count;
}

int  MysqlSecAssess::CountShortSignalNumber()
{
	int count=0;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT COUNT(PD_Short_SignalNumber) FROM %s WHERE PD_Long_SignalNumber =0 AND PD_Short_SignalNumber>0 ",
			this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		if(this->getRowCount()>0)		{
			this->getFirst();
			count = getValueInt("COUNT(PD_Short_SignalNumber)");
		}
	}

	this->Empty();

	return count;
}
SectionAssessData MysqlSecAssess::Find_MaxShortSignalSection()
{
	SectionAssessData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE PD_Long_SignalNumber =0 AND PD_Short_SignalNumber>0  "
			"ORDER BY PD_Short_SignalNumber desc limit 1 ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		if(this->getRowCount()>0)		{

			strcpy(data.Section,getValueString("Section").c_str());
			data.TotalProductNumber = getValueInt("TotalProductNumber");
			data.ThreshHoldNumber =  getValueInt("ThreshHoldNumber");
			data.ProductRiskNumber = getValueInt("ProductRiskNumber");
			data.ProductHoldNumber = getValueInt("ProductHoldNumber");
			data.PD_Long_SignalNumber = getValueInt("PD_Long_SignalNumber");
			data.PD_Short_SignalNumber = getValueInt("PD_Short_SignalNumber");
			strcpy(data.PD_Long_InstrumentId,getValueString("PD_Long_InstrumentId").c_str());
			strcpy(data.PD_Short_InstrumentId,getValueString("PD_Short_InstrumentId").c_str());
			data.PD_Long_Inst_UpdownRate = getValueDouble("PD_Long_Inst_UpdownRate");
			data.PD_Short_Inst_UpdownRate = getValueDouble("PD_Short_Inst_UpdownRate");
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			data.Priority = getValueInt("Priority");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());
		}


	}
	this->Empty();
	return data;
}
