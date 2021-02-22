#include "mysqltestsecassess.hpp"

MysqlTestSecAssess::MysqlTestSecAssess()
{
	std::string database="testrecord";
	std::string tablename="realcycle_assess";

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


void MysqlTestSecAssess::CreateTable(){

	char buf[2048];
	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
					"Section  CHAR(40) NOT NULL PRIMARY KEY, "
					"TotalProductNumber INT,"   // 该板块品种总数
					"ThreshHoldNumber INT,"  // 该板块信号数 超过该值 则可以在该板块开仓
					"ProductRiskNumber INT,"  // 该板块可以开仓的品种数
					"ProductHoldNumber INT,"  // 该板块目前有持仓的品种数
					"PD_Long_SignalNumber INT,"  // 该板块当日出现多头信号的品种数
					"PD_Short_SignalNumber INT,"  // 该板块当日出现空头信号的品种数
					"InsHoldNumber INT,"  //该板块目前合约总持仓手数
					"UpdateTime DATETIME) default character set utf8 ",this->getTablename().c_str());
	std::string str_sql;
	str_sql=buf;
	this->Execute(str_sql);
}
void MysqlTestSecAssess::Insert(SectionAssessData data){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"INSERT INTO %s (Section,TotalProductNumber,ThreshHoldNumber,ProductRiskNumber,ProductHoldNumber,PD_Long_SignalNumber,"
			"PD_Short_SignalNumber,InsHoldNumber,UpdateTime) "
			"VALUES ('%s','%d','%d','%d','%d','%d','%d',"
			"'%d','%s')",this->getTablename().c_str(),
			data.Section,data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber,data.ProductHoldNumber,data.PD_Long_SignalNumber,data.PD_Short_SignalNumber,
			data.InsHoldNumber,data.UpdateTime);

	str_sql=buf;
	this->Execute(str_sql);
}

void MysqlTestSecAssess::Update(SectionAssessData data){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET ProductHoldNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			data.ProductHoldNumber,data.Section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}


void MysqlTestSecAssess::UpdateLongProductDaySignalNumber(int signalnumber,const char* section){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Long_SignalNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			signalnumber,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
void MysqlTestSecAssess::UpdateShortProductDaySignalNumber(int signalnumber,const char* section){

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"UPDATE %s SET PD_Short_SignalNumber='%d' WHERE Section='%s' ",this->getTablename().c_str(),
			signalnumber,section);
	str_sql=buf;
	this->ExecuteUpdate(str_sql);
}
bool  MysqlTestSecAssess::Exist_DataBySection(const char* section)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT * FROM %s WHERE  Section='%s'  ",this->getTablename().c_str(),section);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
//		printf("MysqlTestSecAssess::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if(this->getRowCount()>0)
		{
//			printf("MysqlTestSecAssess::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			 ret=true;
		}


	}

	this->Empty();

	return ret;
}

SectionAssessData  MysqlTestSecAssess::Find_DataBySection(const char* section)
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
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());
		}
	}

	this->Empty();
	return data;
}

void MysqlTestSecAssess::Find_AllLists(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");
	SectionAssessData data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s ",this->getTablename().c_str());

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
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}


void MysqlTestSecAssess::Find_Long_AllListsBySingalNumber(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");
	SectionAssessData data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE PD_Long_SignalNumber>0 ORDER BY PD_Long_SignalNumber DESC ",this->getTablename().c_str());

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
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}

void MysqlTestSecAssess::Find_Short_AllListsBySingalNumber(vector<SectionAssessData> &temp)
{
//		printf("Find_AllList begin\n");
	SectionAssessData data;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s WHERE PD_Short_SignalNumber>0 ORDER BY PD_Short_SignalNumber DESC ",this->getTablename().c_str());

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
			data.InsHoldNumber = getValueInt("InsHoldNumber");
			strcpy(data.UpdateTime,getValueString("UpdateTime").c_str());

			  temp.push_back(data);
			  this->getNext();
		}
	}

	this->Empty();
}
