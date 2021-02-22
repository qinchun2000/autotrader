#include "testbase.hpp"

TestBase::TestBase(){
//	SetTable_Whole_DateTime("alltime");

	printf("TestBase --> 构造函数   \n");
	 m_pmysqlmaincontract = new MysqlMaincontract();
	 m_pmysqltestrecord = new MysqlTestRecord();
	 m_mysqldayline= new MysqlDayLine() ;
	m_pmysqlhismindata = new MysqlHisMinData();

}
TestBase::~TestBase(){
	printf("TestBase --> 析构构造函数   \n");
	//	SetStrategy(pstrategy);
	if(m_pmysqlmaincontract!=nullptr)	{
		printf("TestBase --> delete    m_pmysqlmaincontract\n");
		delete m_pmysqlmaincontract;
	}
	if(m_pmysqltestrecord!=nullptr)	{
		printf("TestBase --> delete    m_pmysqltestrecord\n");
		delete m_pmysqltestrecord;
	}
	if(m_mysqldayline!=nullptr)	{
		printf("TestBase --> delete    m_mysqldayline\n");
		delete m_mysqldayline;
	}
	if(m_pmysqlhismindata!=nullptr)	{
		printf("TestBase --> delete    m_pmysqlhismindata\n");
		delete m_pmysqlhismindata;
	}

	printf("TestBase --> 析构构造函数   结束\n");
}
TestBase::TestBase(const char* pstrategy,const char* year){
	printf("TestBase --> 带参数构造函数   \n");
//	SetStrategy(pstrategy);
//	SetYear(year);
//	SetTable_Year_DateTime(year);
//	SetTable_Whole_DateTime("alltime");
//
//	SetTable_Report();
//
//	SetTable_Record_Prefix();
//	MysqlDateTime mysqldatetime;
//
//	if(!mysqldatetime.Exist_Table(GetTable_Year_DateTime().c_str()))		{
//		this->CheckDateTimeSequence();
//	}
}

void TestBase::SetInitialMoney(double money){
	this->_InitialMoney=money;
}
double TestBase::GetInitialMoney(){
	return this->_InitialMoney;
}

void TestBase::SetRiskRate(double risk){
	this->_RiskRate=risk;
}

double TestBase::GetRiskRate(){
	return this->_RiskRate;
}

void TestBase::SetLimitSedimentary(double money){
	this->_limitsedimientary=money;
}
double TestBase::GetLimitSedimentary(){
	return this->_limitsedimientary;
}

void TestBase::SetSingleMarginRate(double rate){
	this->_singlemarginrate=rate;

}

double TestBase::GetSingleMarginRate(){
	return this->_singlemarginrate;

}
void TestBase::SetOpenTimeMode(int mode){
	this->_opentime_mode=mode;

}

int TestBase::GetOpenTimeMode(){
	return this->_opentime_mode;

}

void TestBase::SetCloseTimeMode(int mode)
{
	this->_closetime_mode=mode;

}

int TestBase::GetCloseTimeMode(){
	return this->_closetime_mode;

}
void TestBase::SetTable_Report(){
	_table_report =this->GetStrategy();
	_table_report.append(this->GetYear());
}
void TestBase::SetTable_Record_Prefix(){
	_table_record_prefix=this->GetYear();
	_table_record_prefix.append("_");
	_table_record_prefix.append(this->GetStrategy());
}

std::string  TestBase::GetTable_Record_Prefix(){
	return this->_table_record_prefix;
}

void TestBase::SetTable_Year_DateTime(const char * year){
	_table_year_datetime="y";
	_table_year_datetime.append(year);
}

std::string TestBase::GetTable_Year_DateTime(){
	return this->_table_year_datetime;

}

void TestBase::SetTable_Whole_DateTime(const char * prefix){
	_table_whole_datetime=prefix;

}

std::string TestBase::GetTable_Whole_DateTime(){
	return this->_table_whole_datetime;
}

void TestBase::SetTable_HisMindata(const char * instrumentid,const char*date){
	this->_table_hismindata=instrumentid;
	this->_table_hismindata.append("_");
	this->_table_hismindata.append(date);
}
std::string TestBase::GetTable_HisMindata(){
	return this->_table_hismindata;
}
void TestBase::SetDataBase_HisMindata(const char * productid){
	this->_database_hismindata = "min_";
	this->_database_hismindata.append(productid);
}
std::string TestBase::GetDataBase_HisMindata(){
	return this->_database_hismindata;
}

void TestBase::SetStrategy(const char * pstrategy){
	this->_strategy=pstrategy;
}
std::string TestBase::GetStrategy(){
	return this->_strategy;
}

void TestBase::SetYear(const char * pyear){
	this->_year=pyear;
}
std::string TestBase::GetYear(){
	return this->_year;
}

void TestBase::SetCusor_DaySqlTime(const char * time){
	this->_cusor_day_sqltime=time;
}
std::string TestBase::GetCusor_DaySqlTime(){
	return this->_cusor_day_sqltime;
}

void TestBase::SetCusor_MinuteTime(const char * time){
	this->_cusor_minutetime=time;
}
std::string TestBase::GetCusor_MinuteTime(){
	return this->_cusor_minutetime;
}

void TestBase::SetOpenFlag(bool flag){
	this->_openflag=flag;
}
bool TestBase::GetOpenFlag(){
	return this->_openflag;
}
void TestBase::SetCloseFlag(bool flag){
	this->_closeflag=flag;
}
bool TestBase::GetCloseFlag(){
	return this->_closeflag;
}

void TestBase::SetVolumeMultiple(int  vm){
 this->_volumemutiple=vm;
}

int TestBase::GetVolumeMultiple(){
	return this->_volumemutiple;
}

void TestBase::SetMaxUpDownLimit(double  rate){
 this->_maxupdownlimit=rate;
}

double TestBase::GetMaxUpDownLimit(){
	return this->_maxupdownlimit;
}

void TestBase::SetMargin(double  margin){
 this->_margin=margin;
}

double TestBase::GetMargin(){
	return this->_margin;
}


void TestBase::SetStopRatio(double  rate){
 this->_ratio=rate;
}

double TestBase::GetStopRatio(){
	return this->_ratio;
}


void TestBase::SetProductID(const char * pproductid){
	this->_productid=pproductid;
}
std::string  TestBase::GetProductID(){
	return this->_productid;
}
void TestBase::SetInstrumentID(const char * pinstrumentid){
	this->_instrumentid =pinstrumentid;
}
std::string  TestBase::GetInstrumentID(){
	return this->_instrumentid;
}

void TestBase::SetMainInstrumentID(const char * pinstrumentid){
	this->_instrumentid =pinstrumentid;
}
std::string  TestBase::GetMainInstrumentID(){
	return this->_instrumentid;
}
void TestBase::SetTableRecord(const char * pproductid){
	_table_record=_year;
	_table_record.append("_");
	_table_record.append(_strategy);
	_table_record.append(pproductid);
}

std::string  TestBase::GetTableRecord(){
	return this->_table_record;
}
std::string  TestBase::GetTableReport(){
	return this->_table_report;
}

void TestBase::SetTodayHigh(double high){
	this->_todayhigh_minline =high;
}

double  TestBase::GetTodayHigh(){
	return this->_todayhigh_minline;
}
void TestBase::SetTodayLow(double low){
	this->_todaylow_minline =low;
}

double  TestBase::GetTodayLow(){
	return this->_todaylow_minline;
}

void TestBase::SetStopRange(double range){
this->_stop_range =range;
}

double  TestBase::GetStopRange(){
	return this->_stop_range;
}
void TestBase::SetK(double k){
this->_k =k;
}

double  TestBase::GetK(){
	return this->_k;
}
void TestBase::SetN(int n){
this->_n =n;
}

int  TestBase::GetN(){
	return this->_n;
}

void TestBase::SetActiveCount(int n){
this->_activecount =n;
}

int  TestBase::GetActiveCount(){
	return this->_activecount;
}

void TestBase::SetWinRatio(double range){
this->_wintatio =range;
}

double  TestBase::GetWinRatio(){
	return this->_wintatio;
}
//MysqlMaincontract *TestBase::GetMysqlMaincontract(){
//	return &this->_mysqlmaincontract;
//}
//MysqlTestRecord * TestBase::GetMysqlTestRecord(){
//	return &this->_mysqltestrecord;
//}
//MysqlDayLine *TestBase::GetMysqlDayLine(){
//	return &this->_mysqldayline;
//}
//MysqlHisMinData *TestBase::GetMysqlHisMinData(){
//	return &this->_mysqlhismindata;
//}
void  TestBase::CheckDateTimeSequence()
{
	int count =13;
	MysqlUtil mysqlutil("test");

	std::string rb[13]={"rb1601","rb1605","rb1610",
			"rb1701","rb1705","rb1710",
			"rb1801","rb1805","rb1810",
			"rb1901","rb1905","rb1910",
			"rb2001"};
	vector<std::string> rb_list;

	for (int i=0;i<count;i++){
		rb_list.push_back(rb[i].c_str());
		mysqlutil.CheckAllHisDataByInstrument(rb[i].c_str());
	}

	for (auto &item:rb_list){
		MysqlDayLine mysqldayline;
		vector<HxDayLineData>  dayline_list;
		mysqldayline.Find_AllList(dayline_list,item.c_str());

		double endday_close;
		std::string endday;
		int i=0;
		for (auto &dayline:dayline_list){
				MysqlDateTime mysqldatetime;
				DateTime datetime;
				std::string temp =dayline.Time;

				std::string year;
				std::string tablename ;
				tablename="y";
				tablename.append( temp.substr(0,4));
				strcpy(datetime.Time,dayline.Time);
				mysqldatetime.CreateTable(tablename.c_str());

				if (!mysqldatetime.Exist_DateTime(tablename.c_str(),dayline.Time))	{
					printf("CheckDateTimeSequence:插入新日期   --------------%s  \n",dayline.Time);
					mysqldatetime.Insert(tablename.c_str(),datetime);
				}
		}
		vector<HxDayLineData>().swap(dayline_list);
	}

//	CheckMainContract("Black",rb_list);

	vector<std::string>().swap(rb_list);
}
