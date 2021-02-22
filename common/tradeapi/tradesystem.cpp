#include "tradesystem.hpp"


TradeSystem::TradeSystem() {

	DateUtil dateutil;
	double Q_BarTime_2 = dateutil.GetCurrentBarTime();

	if (Q_BarTime_2 >= 0.160000 )	{
			_TaskFlag16 =true;
	}
	else	{
			_TaskFlag16 =false;
	}

	if (Q_BarTime_2 >= 0.200000 )	{
			_TaskFlag =true;
	}
	else	{
			_TaskFlag =false;
	}

}


void TradeSystem::SetCThostFtdcTraderApi(CThostFtdcTraderApi *ptraderapi){

	this->_pTraderApi=ptraderapi;

}


CThostFtdcTraderApi * TradeSystem::GetCThostFtdcTraderApi(){

	return this->_pTraderApi;

}

void TradeSystem::SetCTraderSpi(CTraderSpi *ptraderspi){

	this->_pTraderSpi=ptraderspi;

}


CTraderSpi * TradeSystem::GetCTraderSpi(){

	return this->_pTraderSpi;

}

void TradeSystem::SetTrader(TrendTrader *ptrader){
	this->_pTrader=ptrader;
}
TrendTrader * TradeSystem::GetTrader(){
	return this->_pTrader;
}
void TradeSystem::SetSecAssess(SecAssess *psecassess){
	this->_psecassess=psecassess;
}
SecAssess * TradeSystem::GetSecAssess(){
	return this->_psecassess;
}

void TradeSystem::SetStrategyName(const char* strategyname){
	this->_strategyName=strategyname;

}

std::string TradeSystem::GetStrategyName(){
	return this->_strategyName;
}


void TradeSystem::SetStrategy(Strategy *pstrategy){

	this->_pstrategy=pstrategy;

}


Strategy * TradeSystem::GetStrategy(){

	return this->_pstrategy;

}
void TradeSystem::SetAvalible(double ava)
{
	this->_avalible=ava;
}
double  TradeSystem::GetAvalible()
{
	return this->_avalible;
}

void TradeSystem::DropSqlTable(){
	printf("TradeSystem:开始启动程序  --->>>>DropSqlTable \n");
	Config config(this->GetStrategyName().c_str());
	MysqlDepthMarket mysqldepthmarket_d;
	if(mysqldepthmarket_d.Exist_Table() && mysqldepthmarket_d.Count_ColumnsbyTableName()<20){
		printf("	DropSqlTable:drop数据表  MysqlDepthMarket !!!\n");
		mysqldepthmarket_d.DropTable();
	}

//	MysqlCommission mysqlcommission;
//	if(mysqlcommission.Exist_Table()){
//		mysqlcommission.DropTable();
//		printf("	DropSqlTable:清空数据表  MysqlCommission !!!\n");
//	}

	MysqlSection mysqlsection;
	if(mysqlsection.Exist_Table()){
		mysqlsection.DropTable();
		printf("	DropSqlTable:清空数据表  MysqlSection !!!\n");
	}

	MysqlSecAssess mysqlsecassess_d(config.GetUserID().c_str());
	if(mysqlsecassess_d.Exist_Table() && mysqlsecassess_d.Count_ColumnsbyTableName()<11){
		mysqlsecassess_d.DropTable();
		printf("	DropSqlTable:清空数据表  MysqlSecAssess !!!\n");
	}

	MysqlProduct mysqlproduct_d;
	if(mysqlproduct_d.Exist_Table()){
		mysqlproduct_d.DropTable();
		printf("	DropSqlTable:drop数据表  MysqlProduct !!!\n");
	}
		//
		//	MysqlInstrument mysqlinstrument_d;
		//	if(mysqlinstrument_d.Exist_Table()){
		//	mysqlinstrument_d.DropTable();
		//	}
		//
//	MysqlMaincontract msmt;
//	if(msmt.Exist_Table()){
//		msmt.DropTable();
//		printf("	DropSqlTable:drop数据表  MysqlMaincontract !!!\n");
//	}

		//
		//	MysqlGlobalView mysqlglobalview_d;
		//		if(mysqlglobalview_d.Exist_Table()){
		//			mysqlglobalview_d.DropTable();
		//
		//			printf("InitialSqlTable:drop数据表  MysqlGlobalView \n");
		//		}


	//	Config config(this->GetStrategyName().c_str());



	//	MysqlOrder mysqlopen(config.GetTable_Open().c_str(),config.GetUserID().c_str());
	//	if(mysqlopen.Exist_Table()){
	//		mysqlopen.DropTable();
	//
	//		printf("InitialSqlTable:drop数据表  MysqlOrder ---> open table!!!  \n");
	//	}
	//	else{
	//		mysqlopen.CreateTable();
	//		printf("InitialSqlTable:新建数据表  MysqlOrder ---> open table!!!  \n");
	//	}
	//
	//	MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),config.GetUserID().c_str());
	//	if(mysqlcloseorder.Exist_Table()){
	//		mysqlcloseorder.DropTable();
	//
	//		printf("InitialSqlTable:drop数据表  MysqlOrder ---> close table!!!  \n");
	//	}else{
	//		mysqlcloseorder.CreateTable();
	//		printf("InitialSqlTable:新建数据表  MysqlOrder ---> close table!!!  \n");
	//	}

	//MysqlAsset mysqlasset_d(config.GetUserID().c_str());
	//	if(mysqlasset_d.Exist_Table()){
	//		mysqlasset_d.DropTable();
	//
	//		printf("InitialSqlTable:drop数据表  MysqlAsset \n");
	//	}
	MysqlTradingDay mysqltradingday(config.GetUserID().c_str());
	if(mysqltradingday.Exist_Table() && mysqltradingday.Count_ColumnsbyTableName()<17){
		printf("InitialSqlTable:drop数据表  MysqlTradingDay \n");
		mysqltradingday.DropTable();
	}

	//		MysqlTradingDay mysqltradingday_d(config.GetUserID().c_str());
	//		if(mysqltradingday_d.Exist_Table()){
	//			mysqltradingday_d.DropTable();
	//
	//			printf("	InitialSqlTable:drop数据表  MysqlTradingDay \n");
	//		}

	//		MysqlUser mysqluser_d;
	//		if(mysqluser_d.Exist_Table()){
	//			mysqluser_d.DropTable();
	//
	//			printf("InitialSqlTable:drop数据表  MysqlUser \n");
	//		}
}
void TradeSystem::InitialSqlTable(){

	printf("TradeSystem:开始启动程序  --->>>>InitialSqlTable \n");
	Config config(this->GetStrategyName().c_str());

	MysqlHold mysqlhold(config.GetUserID().c_str());
	if(mysqlhold.Exist_Table()){
		mysqlhold.DropTable();

		printf("InitialSqlTable:drop数据表  MysqlHold ---> table!!!  \n");
	}

//		MysqlMaincontract msmt;
//		if(msmt.Exist_Table()){
//			msmt.DropTable();
//			printf("	DropSqlTable:drop数据表  MysqlMaincontract !!!\n");
//		}
//
//		MysqlProduct mysqlproduct_d;
//		if(mysqlproduct_d.Exist_Table()){
//			mysqlproduct_d.DropTable();
//			printf("	DropSqlTable:drop数据表  MysqlProduct !!!\n");
//		}

//		MysqlInstrument mysqlinstrument_d;
//		if(mysqlinstrument_d.Exist_Table()){
//		mysqlinstrument_d.DropTable();
//		}


	//临时代码
//	MysqlGlobalView mysqlglobalview(config.GetUserID().c_str());
//	if(mysqlglobalview.Exist_Table()){
//		printf("	DropSqlTable:drop数据表  mysqlglobalview !!!\n");
//		mysqlglobalview.DropTable();
//	}
//
//	if(!mysqlglobalview.Exist_Table()){
//		mysqlglobalview.CreateTable();
//	}

///////////////////////////////////////////////////////////////////////////
	MysqlUser mysqluser;
	if(!mysqluser.Exist_Table()){
		mysqluser.CreateTable();
		printf("	InitialSqlTable:创建数据表  MysqlUser \n");
	}

	MysqlExchange mysqlexchange;
	if(!mysqlexchange.Exist_Table()){
		mysqlexchange.CreateTable();
		printf("	InitialSqlTable:创建数据表  MysqlExchange \n");
	}

	MysqlInstrument mysqlinstrument;
	if(!mysqlinstrument.Exist_Table()){
		mysqlinstrument.CreateTable();
		printf("	InitialSqlTable:创建数据表  MysqlInstrument \n");
	}

	MysqlProduct mysqlproduct;
	if(!mysqlproduct.Exist_Table()){
		mysqlproduct.CreateTable();
		printf("	InitialSqlTable:创建数据表  MysqlProduct \n");
	}

	MysqlDepthMarket mysqldepthmarket;
	if(!mysqldepthmarket.Exist_Table()){
		mysqldepthmarket.CreateTable();
		printf("	InitialSqlTable:创建数据表  MysqlDepthMarket \n");
	}

	MysqlMaincontract mysqlmaincontract;
	if(!mysqlmaincontract.Exist_Table()){
		mysqlmaincontract.CreateTable();
		printf("	InitialSqlTable:创建数据表  MysqlMaincontract \n");
	}

	MysqlTradingDay mysqltradingday(config.GetUserID().c_str());



	if(!mysqltradingday.Exist_Table()){
		mysqltradingday.CreateTable();

		printf("	InitialSqlTable:创建数据表  MysqlTradingDay \n");
	}

	ReadCommissionData();

	InitialSectionTable();

	InitialSecAssessTable(config.GetUserID().c_str());
}


void TradeSystem::InitialFlag(){

	DateUtil dateutil;
	 // 初始化线程同步变量
	double Q_BarTime_2 = this->GetCurrentTime();

	if (Q_BarTime_2 >= 0.160000 )	{
			_TaskFlag16 =true;
	}
	else	{
			_TaskFlag16 =false;
	}

	if (Q_BarTime_2 >= 0.200000 )	{
			_TaskFlag =true;
	}
	else	{
			_TaskFlag =false;
	}

}
double TradeSystem::GetCurrentTime(){
	DateUtil dateutil;
	double ret = dateutil.GetCurrentBarTime()+ (1/10e5)*this->GetTrader()->GetDifSec();
	return ret;

}

void TradeSystem::LocalLogin(){

	this->GetTrader()->CTP_Set();
	printf("LocalLogin:CTP 初始化设置完成!!!    \n");

	this->GetTrader()->FrontConnect();
	printf("LocalLogin:CTP FrontConnect完成!!!    \n");

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0){
		printf("TradeSystem::LocalLogin:中粮期货CTP 认证 Begin!!!    \n");
		this->GetTrader()->ReqAuthenticate();
		printf("TradeSystem::LocalLogin:中粮期货CTP 认证 End!!!    \n");
	}
	if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){
		printf("TradeSystem::LocalLogin:中粮期货CTP 认证 Begin!!!    \n");
		this->GetTrader()->ReqAuthenticate();
		printf("TradeSystem::LocalLogin:中粮期货CTP 认证 End!!!    \n");
	}
	else if(strcmp(this->GetStrategyName().c_str(),"ZLQH_TEST")==0){
		printf("TradeSystem::LocalLogin:中粮期货CTP TEST 认证 Begin!!!    \n");
		this->GetTrader()->ReqAuthenticate();
		printf("TradeSystem::LocalLogin:中粮期货CTP TEST 认证 End!!!    \n");
	}
	else{
//		printf("TradeSystem::LocalLogin:SimNowCTP TEST 认证 Begin!!!    \n");
//		this->GetTrader()->ReqAuthenticate();
//		printf("TradeSystem::LocalLogin:SimNowCTP TEST 认证 End!!!    \n");
	}

	this->GetTrader()->FirstLogin();
	printf("LocalLogin:CTP FirstLogin完成!!!    \n");
	DateUtil dateutil;
	printf("Trade:开始启动程序  登录成功--->>>>TradingDay %s \n",this->GetTrader()->GetCThostFtdcTraderApi()->GetTradingDay());
	this->GetTrader()->UpdateTradingDay();
	this->GetTrader()->UpdateTradingDaySqlTime();
	this->GetTrader()->UpdateDifSec();
	this->GetTrader()->SetDifSec(this->GetTrader()->GetDifSec());
	this->GetStrategy()->UpdateTradingDay();
	this->GetSecAssess()->SetTradingDay(this->GetStrategy()->GetTradingDay().c_str());

		if(this->GetCTraderSpi()->GetFlagLogin())	{
			printf("Trade:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime :%s\n", this->GetCTraderSpi()->GetTest().c_str(),
					this->GetTrader()->GetTradingDay().c_str(),this->GetTrader()->GetTradingDaySqlTime().c_str());
			//初始化逻辑判断信号
			InitialFlag();
			dateutil.CheckWaitSecondsbySHFELoginMarketTime(this->GetTrader()->GetTradingDay().c_str());

		}
}

void TradeSystem::UpdateFlag(){
	DateUtil dateutil;
	bool _flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
	bool _flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
	bool _flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
	bool _flag_holiday = dateutil.TodayIsHoliday();
}
//可登录时间段  比正常交易时间段要广
bool TradeSystem::IsLoginAvalibleTime(){
	DateUtil dateutil;
	this->_flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
	return this->_flag_loginmarket;
}
//交易时段 所有合约的交易时段合集
bool TradeSystem::IsTradeAvalibleTime(){
	DateUtil dateutil;
	this->_flag_open =dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
	return this->_flag_open;
}
//是否节假日
bool TradeSystem::IsHoliday(){
	DateUtil dateutil;
	this->_flag_holiday = dateutil.TodayIsHoliday();
	return this->_flag_holiday;
}
//是否周末
bool TradeSystem::IsWeekEnd(){
	DateUtil dateutil;
	return dateutil.Check_wday();
}
bool TradeSystem::IsLoginOK(){

	 return this->GetCTraderSpi()->GetFlagLogin();
}

bool TradeSystem::ConnectRestore(){
	bool frontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();
	bool frontdisconnect_was =! this->GetCTraderSpi()->GetFlagFrontDisConnect() ;
	bool login_not = ! this->GetCTraderSpi()->GetFlagLogin();
	bool holiday_not =! this->IsHoliday();
	bool ret = IsLoginAvalibleTime() && frontconnect_ok && frontdisconnect_was && login_not && holiday_not;
	 return ret;
}
bool TradeSystem::OpenMarketLoginOK(){
//	printf("TradeSystem::OpenMarketLoginOK  --> begin \n");
	bool frontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();
//	printf("TradeSystem::OpenMarketLoginOK  --> frontconnect_ok  --->%d  \n",frontconnect_ok);
	bool frontdisconnect_was =! this->GetCTraderSpi()->GetFlagFrontDisConnect() ;
//	printf("TradeSystem::OpenMarketLoginOK  --> frontdisconnect_was  --->%d  \n",frontdisconnect_was);
	bool login_ok = this->GetCTraderSpi()->GetFlagLogin();
//	printf("TradeSystem::OpenMarketLoginOK  --> login_ok  --->%d  \n",login_ok);
	bool holiday_not =! this->IsHoliday();
//	printf("TradeSystem::OpenMarketLoginOK  --> holiday_not  --->%d  \n",holiday_not);
//	printf("TradeSystem::OpenMarketLoginOK  --> IsTradeAvalibleTime  --->%d  \n",IsTradeAvalibleTime());
	bool ret = IsTradeAvalibleTime() && frontconnect_ok && frontdisconnect_was && login_ok && holiday_not;

//	printf("TradeSystem::OpenMarketLoginOK  --> end  --->%d  \n",ret);
	 return ret;
}

void TradeSystem::ReadCommissionData()
{
	MysqlCommission mysqlcommission;
	if(mysqlcommission.Exist_Table()){

		int c=mysqlcommission.Count_AllLists();
		int n=52;
		if(c<n){
			mysqlcommission.DropTable();
			printf("ReadCommissionData:数据不足%d  < %d   ------> 清空数据表  MysqlCommission \n",c,n);
		}

	}

	if(!mysqlcommission.Exist_Table()){
		mysqlcommission.CreateTable();
	}

	if(mysqlcommission.Count_AllLists()==0){

		printf("TradeSystem:ReadCommissionData  不存在佣金表,写入佣金表!!! \n");
		Config config(this->GetStrategyName().c_str());
		std::string path=config.GetConfigPath();
		std::string filename ="commission.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("TradeSystem:ReadCommissionData ---->File not found!\n");
		}
		else	{
			printf("TradeSystem:ReadCommissionData --->File found!\n");
		}

		while (CSVRead.CSVReadNextRow())	{

			std::string strProductID;
			std::string strExchangeID;
			std::string strMode;
			std::string strOpenAccount;
			std::string strOpenNumber;
			std::string strCloseAccount;
			std::string strCloseNumber;
			std::string strMargin;

			CSVRead.CSVRead("productid", strProductID);
			CSVRead.CSVRead("exchangeid", strExchangeID);
			CSVRead.CSVRead("mode", strMode);

			CSVRead.CSVRead("open_account", strOpenAccount);
			CSVRead.CSVRead("open_number", strOpenNumber);
			CSVRead.CSVRead("close_account", strCloseAccount);
			CSVRead.CSVRead("close_number", strCloseNumber);
			CSVRead.CSVRead("margin", strMargin);

			double openaccount = atof(strOpenAccount.c_str());
			double opennumber = atof(strOpenNumber.c_str());
			double closeaccount = atof(strCloseAccount.c_str());
			double closenumber = atof(strCloseNumber.c_str());
			double margin = atof(strMargin.c_str());

			Commission commdata;
			strcpy(commdata.ProductID,strProductID.c_str());
			strcpy(commdata.ExchangeID,strExchangeID.c_str());
			strcpy(commdata.Mode,strMode.c_str());

			commdata.OpenAccount=openaccount/10000;
			commdata.OpenNumber=opennumber;
			commdata.CloseAccount=closeaccount/10000;
			commdata.CloseNumber=closenumber;
			commdata.Margin = margin;

			printf("TradeSystem:ReadCommissionData 原始数据：%s %s %s  %f %.2f %f %.2f  margin:%f \n",strProductID.c_str(),strExchangeID.c_str(),strMode.c_str(),
					openaccount,opennumber,closeaccount,closenumber,margin);


			if(!mysqlcommission.Exist_DatabyProductID(strProductID.c_str()))	{
				mysqlcommission.Insert(commdata);
				printf("TradeSystem:ReadCommissionData 插入数据 ----> %s %s %s   %f %.2f %f %.2f %f \n",commdata.ProductID,commdata.ExchangeID,commdata.Mode,
						commdata.OpenAccount,commdata.OpenNumber,commdata.CloseAccount,commdata.CloseNumber,commdata.Margin);
			}

		}

	}
	else	{
		printf("TradeSystem:ReadCommissionData  存在佣金表\n");
	}

}
//section table 用于scetion view
void TradeSystem::InitialSectionTable()
{
	MysqlSection mysqlsection;

	if(mysqlsection.Exist_Table()){
		mysqlsection.DropTable();
		printf("InitialSectionTable:清空数据表  MysqlSection \n");
	}

	if(!mysqlsection.Exist_Table()){
		mysqlsection.CreateTable();
	}

	if(mysqlsection.Count_AllLists()==0){

		printf("TradeSystem::InitialSectionTable  不存在评估,写入初始评估表!!! \n");
		Config config(this->GetStrategyName().c_str());
		std::string path=config.GetConfigPath();
		std::string filename ="assess.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("TradeSystem::InitialSectionTable ---->File not found!\n");
		}
		else	{
			printf("TradeSystem::InitialSectionTable --->File found!\n");
		}

		while (CSVRead.CSVReadNextRow())	{

			std::string strSection;
			std::string strTotalProductNumber;
			std::string strThreshHoldNumber;
			std::string strProductRiskNumber;
			std::string strActive;
			CSVRead.CSVRead("Section", strSection);
			CSVRead.CSVRead("TotalProductNumber", strTotalProductNumber);
			CSVRead.CSVRead("ThreshHoldNumber", strThreshHoldNumber);
			CSVRead.CSVRead("ProductRiskNumber", strProductRiskNumber);
			CSVRead.CSVRead("Active", strActive);

			int totalpruductnumber = atoi(strTotalProductNumber.c_str());
			int productrisknumber = atoi(strProductRiskNumber.c_str());
			int threshholdnumber = atoi(strThreshHoldNumber.c_str());
			int active = atoi(strActive.c_str());

			SectionAssessData data;
			strcpy(data.Section,strSection.c_str());

			data.TotalProductNumber=totalpruductnumber;
			data.ProductRiskNumber=productrisknumber;
			data.ThreshHoldNumber = threshholdnumber;
			data.ProductHoldNumber=0;
			data.PD_Long_SignalNumber=0;
			data.PD_Short_SignalNumber=0;
			data.InsHoldNumber=0;

			if(active==1){
				data.Active =true;
			}
			else {
				data.Active =false;
			}

			DateUtil dateutil;
			strcpy(data.UpdateTime,dateutil.ConvertCurrentTimetoSqlTime().c_str());
			printf("TradeSystem::InitialSectionTable 原始数据：%s %d %d %d %d  \n",strSection.c_str(),totalpruductnumber,
					threshholdnumber,productrisknumber,active);

			if(!mysqlsection.Exist_DataBySection(strSection.c_str()))	{
				mysqlsection.Insert(data);
				printf("TradeSystem::InitialSectionTable 插入数据 ----> %s %d %d %d %d\n",data.Section,
						data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber,data.Active);
			}
		}

	}
	else	{
		printf("TradeSystem::InitialSectionTable  存在Section Table !!! \n");
	}

}
// secassess 用于对版块的开仓仓位进行控制
void TradeSystem::InitialSecAssessTable(const char* userid)
{
	MysqlSecAssess mysqlsecassess(userid);
	if(mysqlsecassess.Exist_Table() && mysqlsecassess.Count_ColumnsbyTableName()<14){
		mysqlsecassess.DropTable();
		printf("	InitialSecAssessTable:清空数据表  MysqlSecAssess !!!\n");
	}

	if(!mysqlsecassess.Exist_Table()){
		mysqlsecassess.CreateTable();
	}

	if(mysqlsecassess.Count_AllLists()==0){

		printf("TradeSystem:InitialAssessTable  不存在评估,写入初始评估表!!! \n");
		Config config(this->GetStrategyName().c_str());
		std::string path=config.GetConfigPath();
		std::string filename ="assess.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("TradeSystem:InitialAssessTable ---->File not found!\n");
		}
		else	{
			printf("TradeSystem:InitialAssessTable --->File found!\n");
		}

		while (CSVRead.CSVReadNextRow())	{

			std::string strSection;
			std::string strTotalProductNumber;
			std::string strThreshHoldNumber;
			std::string strProductRiskNumber;
			std::string strPriority;
			CSVRead.CSVRead("Section", strSection);
			CSVRead.CSVRead("TotalProductNumber", strTotalProductNumber);
			CSVRead.CSVRead("ThreshHoldNumber", strThreshHoldNumber);
			CSVRead.CSVRead("ProductRiskNumber", strProductRiskNumber);
			CSVRead.CSVRead("Priority", strPriority);
			int totalpruductnumber = atoi(strTotalProductNumber.c_str());
			int productrisknumber = atoi(strProductRiskNumber.c_str());
			int threshholdnumber = atoi(strThreshHoldNumber.c_str());
			int priority = atoi(strPriority.c_str());

			SectionAssessData data;
			strcpy(data.Section,strSection.c_str());

			data.TotalProductNumber=totalpruductnumber;
			data.ProductRiskNumber=productrisknumber;
			data.ThreshHoldNumber = threshholdnumber;
			data.Priority = priority;
			data.ProductHoldNumber=0;
			data.PD_Long_SignalNumber=0;
			data.PD_Short_SignalNumber=0;
			data.InsHoldNumber=0;
			DateUtil dateutil;
			strcpy(data.UpdateTime,dateutil.ConvertCurrentTimetoSqlTime().c_str());
			printf("RealTradeTest:InitialAssessTable 原始数据：%s %d %d %d  %d \n",strSection.c_str(),totalpruductnumber,
					threshholdnumber,productrisknumber,priority);

			if(!mysqlsecassess.Exist_DataBySection(strSection.c_str()))	{
				mysqlsecassess.Insert(data);
				printf("RealTradeTest:InitialAssessTable 插入数据 ----> %s %d %d %d %d\n",data.Section,
						data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber,data.Priority);
			}
		}

	}
	else	{
		printf("RealTradeTest:InitialAssessTable  存在开仓评估表!!! \n");
	}

}
void TradeSystem::Run(){

	std::string staname="TrendTrade";

	this->SetStrategyName(staname.c_str());

	Config config(this->GetStrategyName().c_str());

	std::string logfilename=this->GetStrategyName();
	logfilename.append("_");
	logfilename.append(config.GetUserID().c_str());
	logfilename.append(".log");

	this->SetLogPathFileName(logfilename.c_str());

	DateUtil dateutil;
	Logger _tradelogger;
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	logutil.WriteLog("TradeSystem:-----------------开始自动交易程序!!!!!!");

	printf("TradeSystem:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());

	redissignal.FlushDB();
	redissection.FlushDB();
	redisdepthmarket.FlushDB();

	//初始化Mysql数据库table
	InitialSqlTable();


	printf("TradeSystem:开始启动程序  --->>>>00000 \n");
	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./traderflow/");
	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);


	printf("TradeSystem:开始启动程序  --->>>>TrendTrade \n");
	TrendTrader trader;

	trader.SetStrategyName(this->GetStrategyName().c_str());
	trader.SetUserLoginField();

	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());



	trader.SetLogName(this->GetStrategyName().c_str());
	trader.SetLogPathFileName(logfilename.c_str());

	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);

	printf("TradeSystem:开始启动程序  --->>>>initial  userid: %s  strategy: %s \n",
			trader.GetPointUserLoginField()->UserID,trader.GetStrategyName().c_str());
	this->SetTrader(&trader);


	StrategyTrend strategy(staname.c_str());
	strategy.SetRedisDepthMarket(&redisdepthmarket);
	strategy.SetRedisSignal(&redissignal);
	strategy.SetRedisSection(&redissection);

	strategy.SetStrategyName(staname.c_str());
	std::string stalogfilename="sta";
	stalogfilename.append("_");
	stalogfilename.append(config.GetUserID().c_str());
	stalogfilename.append(".log");

	strategy.SetLogName(this->GetStrategyName().c_str());
	strategy.SetLogPathFileName(logfilename.c_str());
	this->SetStrategy(&strategy);



	this->LocalLogin();
	trader.UpdateTradingDay();
	trader.UpdateTradingDaySqlTime();
	trader.UpdateDifSec();

	strategy.SetDifSec(this->GetTrader()->GetDifSec());
	strategy.UpdateTradingDay();


	if(this->GetCTraderSpi()->GetFlagLogin())
	{
		printf("TradeSystem:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime  :%s \n", this->GetCTraderSpi()->GetTest().c_str(),
				trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
		//初始化逻辑判断信号
		InitialFlag();


	}


	sleep(1);

	// 初始化TradeApi
	trader.InitialTradeApi();
	logutil.WriteLog("TradeSystem:  自动交易程序启动后 初始工作完成！");

	trader.AccountSelfCheck();
	strategy.SetAvalible(trader.GetAvalible());


	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());
	mysqlutil.CheckHttpDayLine(this->GetStrategyName().c_str());
//	mysqlutil.CheckHttpMinuteLine();

	printf("ready while ---------------------------------------------------------------->  \n");

	logutil.WriteLog("TradeSystem:  自动交易程序启动后  准备进入定期循环任务！");


	while(1)
		{
			UpdateFlag();

			if (ConnectRestore())
			{
				logutil.WriteLog("TradeSystem:  断开后重连 ----------开始！");
				LocalLogin();
				trader.UpdateTradingDay();
				trader.UpdateTradingDaySqlTime();
				trader.UpdateDifSec();
				strategy.SetDifSec(this->GetTrader()->GetDifSec());
				strategy.UpdateTradingDay();
				logutil.WriteLog("TradeSystem:  断开后重连  ----------登录完成！ ");

				trader.ReqSettlementInfoConfirm ();
				logutil.WriteLog("TradeSystem:  断开后重连  ----------结算单确认完成！");


				trader.ReqQryTradingAccount();
				logutil.WriteLog("TradeSystem:  断开后重连  ----------账户检测完成！");

			}
			else if (OpenMarketLoginOK())
			{

				printf("date ***************************************************************************   %s \n",
				dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());


				strategy.SectionMarketView();
				strategy.Check_WholeMarketCapital();
				strategy.Check_UpDownRate();
	//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));
				if (spi.GetFlagOrderInsert())
				{

					logutil.WriteLog("TradeSystem:  自动交易程序---> 委托报单成交后,更新持仓列表开始！");


					trader.ReqQryTrade();

					trader.ReqQryInvestorPositionDetail();
					trader.ReqQryInvestorPosition();

					trader.CheckSqlHoldPositionbyPosDetail();
//					trader.CheckPosDetailbySqlHoldPosition();

					trader.CheckInvestbySqlHoldPosition();


					spi.SetFlagOrderInsert(false);
					logutil.WriteLog("TradeSystem:  自动交易程序---> 委托报单成交后,更新持仓列表结束！");
				}

				trader.AccountCheck();
				strategy.SetAvalible(trader.GetAvalible());
				strategy.UpdateTradingDay();


				//合约到期
				strategy.Close_ExpireMonthClose_SignalCheck();
				trader.ExpireMonthClose_Check();

				//浮动止盈止损
				strategy.Close_DynamicStop_SignalCheck();
				trader.DynamicStopPriceCheck();

				// 趋势反转时止损
				strategy.Hold_MA50Reverse_SignalCheck();
				trader.MA_ReversePriceCheck();


				strategy.Open_MACrossSignal();
				trader.TradeOpenCheck();  //新开仓合约检查

				strategy.QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
				trader.QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测

				strategy.QueueingClosePriceChange_SignalCheck();
				trader.QueueingClosePriceChange_CancelCheck();



			}


			AutoTask_AfterNoonClosebyUser();
			AutoTask_BeforNightbyUser();

			AutoTask();

		}

	return;

}
//void TradeSystem::Run_DayTrade(){
//
//
//	std::string staname="DayTrade";
//	this->SetStrategyName(staname.c_str());
//
//	Config config(this->GetStrategyName().c_str());
//	std::string logfilename=staname.c_str();
//	logfilename.append("_");
//	logfilename.append(config.GetUserID().c_str());
//	logfilename.append(".log");
//	this->SetLogPathFileName(logfilename.c_str());
//	DateUtil dateutil;
//
//	char logbuf[1024];
//	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
//
//	logutil.WriteLog("DayTradeSystem:-----------------开始自动交易程序!!!!!!");
//
//	printf("DayTradeSystem:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
//
//
//	redisdepthmarket.FlushDB();
//	redissignal.FlushDB();
//	redissection.FlushDB();
//
//
//	printf("DayTradeSystem:开始启动程序  --->>>>redis 清空 ok \n");
//	//初始化Mysql数据库table
//	InitialSqlTable();
//	printf("DayTradeSystem:开始启动程序  --->>>>InitialSqlTable ok \n");
//	// 产生一个CThostFtdcTraderApi实例
//	CThostFtdcTraderApi* pTraderApi;
//	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./daytraderflow/");
//	// 产生一个事件处理的实例
//	CTraderSpi spi(pTraderApi);
//	this->SetCThostFtdcTraderApi(pTraderApi);
//	this->SetCTraderSpi(&spi);
//
//	printf("DayTradeSystem:开始启动程序  --->>>>00000 \n");
//
//	TrendTrader trader;
//
//	trader.SetStrategyName(this->GetStrategyName().c_str());
//	trader.SetUserLoginField();
//	std::string spilogfilename="traderspi";
//	spilogfilename.append("_");
//	spilogfilename.append(config.GetUserID().c_str());
//	spilogfilename.append(".log");
//	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
//	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());
//
//	MysqlAsset mysqlasset(config.GetUserID().c_str());
//
//	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
//	mysqlinvest.CreateTable();
//
//
//	trader.SetLogName(this->GetStrategyName().c_str());
//	trader.SetLogPathFileName(logfilename.c_str());
//
//	trader.SetCThostFtdcTraderApi(pTraderApi);
//	trader.SetCTraderSpi(&spi);
//	trader.SetMysqlAsset(&mysqlasset);
//	trader.SetMysqlInvestPosition(&mysqlinvest);
//	trader.SetRedisDepthMarket(&redisdepthmarket);
//	trader.SetRedisSignal(&redissignal);
//	trader.SetRedisSection(&redissection);
//	trader.SetRiskRate(0.02);
//	this->SetTrader(&trader);
//
//
//
//	printf("DayTradeSystem:开始启动程序  --->>>>Strategy \n");
//	StaDay strategy;
//	strategy.SetRedisDepthMarket(&redisdepthmarket);
//	strategy.SetRedisSignal(&redissignal);
//	strategy.SetRedisSection(&redissection);
//	strategy.SetRiskRate(0.2);   //每次开仓的亏损额 占总资金比例
//	strategy.SetATR_Weight(0.5);   //用于定义止损时,atr的倍数
//	strategy.SetStrategyName(this->GetStrategyName().c_str());
//	std::string stalogfilename="sta";
//	stalogfilename.append("_");
//	stalogfilename.append(config.GetUserID().c_str());
//	stalogfilename.append(".log");
//
//	strategy.SetLogName(this->GetStrategyName().c_str());
//	strategy.SetLogPathFileName(logfilename.c_str());
//	this->SetDayStrategy(&strategy);
//
//	printf("DayTradeSystem:开始启动程序  --->>>>LocalLogin \n");
//
//	this->LocalLogin();
//	trader.UpdateTradingDay();
//	trader.UpdateTradingDaySqlTime();
//	trader.UpdateDifSec();
//
//	strategy.SetDifSec(this->GetTrader()->GetDifSec());
//	strategy.UpdateTradingDay();
//
//	if(this->GetCTraderSpi()->GetFlagLogin())	{
//		printf("TradeSystem:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime :%s\n", this->GetCTraderSpi()->GetTest().c_str(),
//				trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
//		//初始化逻辑判断信号
//		InitialFlag();
//
//
//	}
////	sleep(1);
//
//	// 初始化TradeApi
//	trader.InitialTradeApi();   //包含主力合约更新
//	logutil.WriteLog("DayTradeSystem:  自动交易程序启动后 初始工作完成！");
//
//	trader.DayTrade_AccountSelfCheck();
//	strategy.SetAvalible(trader.GetAvalible());
//
//	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());
//	mysqlutil.CheckHttpDayLine(this->GetStrategyName().c_str());
////	mysqlutil.CheckHttpMinuteLine();
////	mysqlutil.CheckAveragePriceData();
//
//	printf("DayTradeSystem:ready while ---------------------------------------------------------------->  \n");
//	logutil.WriteLog("DayTradeSystem:  自动交易程序启动后  准备进入定期循环任务！");
//
//	while(1)		{
//			UpdateFlag();
//
//			if (ConnectRestore())			{
//				logutil.WriteLog("DayTradeSystem:  断开后重连 ----------开始！");
////				LocalLogin();
//
//				this->GetTrader()->UserLogin();
//
//				logutil.WriteLog("DayTradeSystem:  断开后重连 ----------完成！");
//				trader.UpdateTradingDay();
//				trader.UpdateTradingDaySqlTime();
//				trader.UpdateDifSec();
//				strategy.SetDifSec(this->GetTrader()->GetDifSec());
//				strategy.UpdateTradingDay();
//
//				sprintf(logbuf,"DayTradeSystem:  断开后重连  ----------登录完成！  TradingDay : %s TradingDaySqlTime :%s ",
//						trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
//				logutil.WriteLog(logbuf);
//
//				trader.ReqSettlementInfoConfirm ();
//				logutil.WriteLog("DayTradeSystem:  断开后重连  ----------结算单确认完成！");
//
//
//				trader.ReqQryTradingAccount();
//				logutil.WriteLog("DayTradeSystem:  断开后重连  ----------账户检测完成！");
//
//			}
//			else if (OpenMarketLoginOK())			{
//
//				printf("DayTradeSystem:date ***************************************************************************   [%s] \n",
//				dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
//
//	//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));
//				if (spi.GetFlagOrderInsert())	{
//
//					logutil.WriteLog("DayTradeSystem:  自动交易程序---> 委托报单成交后,更新持仓列表开始！");
//
//					//查询后台
//					trader.ReqQryTrade();
//					trader.ReqQryInvestorPositionDetail();
//					trader.ReqQryInvestorPosition();
//
//					//更新本地数据
//					trader.DayTrade_CheckSqlHoldPositionbyPosDetail();
////					trader.CheckPosDetailbySqlHoldPosition();
//					trader.CheckInvestbySqlHoldPosition();
//
//					spi.SetFlagOrderInsert(false);
//					logutil.WriteLog("DayTradeSystem:  自动交易程序---> 委托报单成交后,更新持仓列表结束！");
//				}
//
//				trader.AccountCheck();
//				strategy.SetAvalible(trader.GetAvalible());
//
////				strategy.SectionMarketView();
////				trader.TotalHoldingCheck();
////				trader.HoldPositionDetailCheck();
//
//				if  (trader.GetTotalHolding()>0)  	{//如果存在持仓,则启动平仓策略 进行轮询
//
//				//尾盘平仓策略
//				strategy.DayTrade_EndTimeClose_SignalCheck();
//				trader.ClosebySignal("_daytrade_endtime");
//
//				//固定止损
////				strategy.DayTrade_Stop_SignalCheck();
////				trader.StopPriceCheck();
//
//				//浮动止盈止损
//				strategy.DayTrade_DynamicStop_SignalCheck();
//				trader.ClosebySignal("_daytrade_dynamicstop");
//
////				//浮盈回撤止损
////				strategy.DayTrade_WinProfit_SignalCheck();
////				trader.ClosebySignal("_daytrade_profitclose");
//
//				//止盈价止损
//				strategy.DayTrade_WinPrice_SignalCheck();
//				trader.ClosebySignal("_daytrade_winclose");
//
//				//成交量活跃度逆转
////				strategy.DayTrade_VolumeReverse_SignalCheck();
////				trader.ClosebySignal("_daytrade_volumereverse");
//
//				//平仓如果存在跳价,则进行撤单操作
//				strategy.QueueingClosePriceChange_SignalCheck();
//				trader.QueueingClosePriceChange_CancelCheck();
//				}
//
//
//				//开仓策略
//				strategy.SectionMarketView();
//				strategy.MarketMonitor();
//
//				strategy.DayTrade_Open_CycleN();
//				trader.DayTrade_OpenCheck();  //新开仓合约检查
//
//				// 开仓如果存在跳价  则进行撤单操作
//				strategy.QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
//				trader.QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
//
//			}
//
//
//			AutoTask_AfterNoonClosebyUser();
//			AutoTask_BeforNightbyUser();
//			AutoTask();
//
//		}
//
//	return;
//
//}
void TradeSystem::CheckAllHttpDayLine()
{
	DateUtil dateutil;
	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	if(mysqlmaincontract.Exist_Table()){
		mysqlmaincontract.GetAllInstrumentListsbyStrategy (list,this->GetStrategyName().c_str());
		printf("CheckAllHttpDayLine----->主力合约&远月合约 品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

		for (auto &item:list)	{
	//		printf ("日线数据更新检测 ------->  %s \n",item.c_str());
//			CheckHttpDayLineByInstrument(item.c_str());
		}

	}
	else{
		printf("CheckAllHttpDayLine----->不存在maincontract 主力合约表!!! 主力合约&远月合约 品种数[%ld] ----> %s\n",
				list.size(),dateutil.GetDateTimeString().c_str());
	}

	vector<std::string>().swap(list);
}
std::string  TradeSystem::CheckFlowPath(){

	CodeUtil codeutil;
	std::string flowpath=codeutil.GetCurrentDirectory();
	flowpath.append("/");
	flowpath.append(this->GetStrategyName().c_str());
	flowpath.append("Flow/");

	if(access(flowpath.c_str(),0)==-1){//access函数是查看文件是不是存在
		printf("CheckFlowPath:开始启动程序  --->>>>不存在文件夹 %s \n",flowpath.c_str());
		 if (mkdir(flowpath.c_str(),0777)){//如果不存在就用mkdir函数来创建
			 printf("CheckFlowPath:开始启动程序  --->>>>创建文件夹 %s Error!!!  \n",flowpath.c_str());
		}
		 else{
			 printf("CheckFlowPath:开始启动程序  --->>>>创建文件夹 %s 完成!  \n",flowpath.c_str());
		 }
	}
	else{
//		printf("Run_Trade_zlqh:开始启动程序  --->>>>存在文件夹 %s \n",flowpath.c_str());
	}
	return flowpath;
}
void TradeSystem::SetLogProperty(const char* userid){

	std::string system_log_name="system";
	system_log_name.append("_");
	system_log_name.append(userid);

	std::string system_logfilename="system";
	system_logfilename.append("_");
	system_logfilename.append(userid);
	system_logfilename.append(".log");

	this->SetLogName(system_log_name.c_str());
	this->SetLogPathFileName(system_logfilename.c_str());
}
void TradeSystem::TradeInitial(){
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	DateUtil dateutil;
	printf("TradeInitial:   ------>Begin \n");



	// 初始化TradeApi
	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
			this->GetTrader()->InitialTradeApi();   //包含主力合约更新
			double avalible =this->GetTrader()->GetInitialMoney();

			this->SetAvalible(avalible);
			this->GetTrader()->FixAccountCheck(this->GetAvalible());
			this->GetStrategy()->SetAvalible(this->GetAvalible());

	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
			strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){

		printf("TradeInitial:   InitialTradeApi------>Begin \n");
		this->GetTrader()->InitialTradeApi();   //包含主力合约更新
		printf("TradeInitial:   InitialTradeApi------>End \n");
		double avalible =this->GetTrader()->GetInitialMoney();
		this->SetAvalible(avalible);
		this->GetTrader()->FixAccountCheck(this->GetAvalible());
		this->GetStrategy()->SetAvalible(this->GetAvalible());
		
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0){
		this->GetTrader()->InitialTradeApibyUser();
		double avalible =this->GetTrader()->GetInitialMoney();
		this->SetAvalible(avalible);
		this->GetTrader()->FixAccountCheck(this->GetAvalible());
		this->GetStrategy()->SetAvalible(this->GetAvalible());


	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0){
		this->GetTrader()->InitialTradeApibyUser();
		double avalible =this->GetTrader()->GetInitialMoney();
		this->SetAvalible(avalible);
		this->GetTrader()->FixAccountCheck(this->GetAvalible());
		this->GetStrategy()->SetAvalible(this->GetAvalible());

	}
	else{
		this->GetTrader()->InitialTradeApibyUser();

		double avalible =this->GetTrader()->GetInitialMoney();
		this->SetAvalible(avalible);
		this->GetTrader()->FixAccountCheck(this->GetAvalible());
		this->GetStrategy()->SetAvalible(this->GetAvalible());
	}
	printf("TradeInitial   ------>End \n");

	this->GetStrategy()->SetContractLists();
	printf("Trade: 策略交易合约列表 >>>>>>>SetContractLists   完成!!!   \n");
	logutil.WriteLog("TradeInitial:  自动交易程序启动后 初始工作完成！");
}
void TradeSystem::Fun_ConnectError(){
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	DateUtil dateutil;
	int waitseconds = dateutil.CheckWaitSecondsbyOpenTime();
	if(waitseconds>=1800){
		printf("Trade:  网络链接断开!!!! ---->等待[1800]秒后重新链接！\n");
		sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[1800]秒后重新链接！  %s",dateutil.GetCurrentSqlTimeString().c_str());
		logutil.WriteLog(logbuf);
		sleep(1800);
	}
	else if(waitseconds>0){
		printf("Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		logutil.WriteLog(logbuf);
		sleep(waitseconds);
	}
	else{
		waitseconds=10;
		printf("Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		logutil.WriteLog(logbuf);
		sleep(waitseconds);
	}
}
void TradeSystem::Fun_ReadyLogin(){
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	/* 重新登录后，重置请求ID*/
	this->GetTrader()->SetRequestID(0);
	this->GetTrader()->UserLogin();

	if (this->GetCTraderSpi()->GetFlagLogin()){

		logutil.WriteLog("Trade:  断开后重login ----------完成！");
		this->GetTrader()->UpdateTradingDay();
		this->GetTrader()->UpdateTradingDaySqlTime();
		this->GetTrader()->UpdateDifSec();
		this->GetStrategy()->SetDifSec(this->GetTrader()->GetDifSec());
		this->GetStrategy()->UpdateTradingDay();
		this->GetSecAssess()->SetTradingDay(this->GetStrategy()->GetTradingDay().c_str());

		sprintf(logbuf,"Trade:  断开后重login  ----------登录完成！  TradingDay : %s TradingDaySqlTime :%s ",
				this->GetTrader()->GetTradingDay().c_str(),this->GetTrader()->GetTradingDaySqlTime().c_str());
		logutil.WriteLog(logbuf);
		sleep(5);
//					trader.ReqQryTradingAccount();
//					logutil.WriteLog("Run_CalendarSpreadTrade:  断开后重login成功  ----------ReqQryTradingAccount 账户检测完成！");

	}
	else{
		logutil.WriteLog("Trade:  断开后重login --------尝试登录失败!!! ");
		sleep(3);
	}
}
void TradeSystem::Fun_TradeLimit(){
	DateUtil dateutil;
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	sprintf(logbuf,"Trade:限制流量 请重新登录   [%s] \n",	dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
	logutil.WriteLog(logbuf);
	this->GetTrader()->UserLogout();

	if (!this->GetCTraderSpi()->GetFlagLogin()){
		this->GetTrader()->SetFlagTradeLimit(false);
		sprintf(logbuf,"Trade:限制流量 登出完成!!!   [%s] \n",
				dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
		logutil.WriteLog(logbuf);
	}
	sleep(5);
}
void TradeSystem::Fun_Trade_FlagOrderInsertCheck(){
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	if (this->GetCTraderSpi()->GetFlagOrderInsert())	{

		logutil.WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托报单成交后,更新持仓列表开始！");

		//查询后台
		this->GetTrader()->ReqQryTrade();
		this->GetTrader()->ReqQryInvestorPositionDetail();
		//detial 在前,因为需要detail的数据 进行校验
		this->GetTrader()->ReqQryInvestorPosition();

		//更新本地数据
		this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();
//					this->GetTrader()->CheckPosDetailbySqlHoldPosition();
		this->GetTrader()->CheckInvestbySqlHoldPosition();
		this->GetTrader()->SectionHoldingCheck();

		this->GetCTraderSpi()->SetFlagOrderInsert(false);
		logutil.WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托报单成交后,更新持仓列表结束！");
	}

	if (this->GetCTraderSpi()->GetFlagCloseOrderInsert()){
		logutil.WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托平仓报单成交后,更新持仓列表开始！");

		//查询后台
		this->GetTrader()->ReqQryTrade();
		this->GetTrader()->ReqQryInvestorPositionDetail();
		this->GetTrader()->ReqQryInvestorPosition();

		//更新本地数据
		this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();
//					this->GetTrader()->CheckPosDetailbySqlHoldPosition();
		this->GetTrader()->CheckInvestbySqlHoldPosition();
		this->GetTrader()->SectionHoldingCheck();

		this->GetCTraderSpi()->SetFlagCloseOrderInsert(false);
		logutil.WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托平仓报单成交后,更新持仓列表结束！");
	}

}
void TradeSystem::Fun_Trade_HoldCheck(){
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	printf("\n");
	//持仓策略
	if  (this->GetTrader()->GetTotalHolding()>0)  	{//如果存在持仓,则启动平仓策略 进行轮询

		//尾盘平仓策略
		this->GetStrategy()->DayTrade_EndTimeClose_SignalCheck();
		this->GetTrader()->ClosebySignal("_daytrade_endtime");

		//固定止损
	//				strategy.DayTrade_Stop_SignalCheck();
	//				this->GetTrader()->StopPriceCheck();

		//浮动止盈止损
		this->GetStrategy()->Close_DynamicStop_SignalCheck();
		this->GetTrader()->ClosebySignal("_dynamicstop");


//		if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")!=0 )	{
//			//板块趋势反向 平仓
//			this->GetStrategy()->Close_SectionTrendStop_SignalCheck();
//			this->GetTrader()->ClosebySignal("_sectiontrendstop");
//		}

		//涨跌停平仓
		this->GetStrategy()->Close_LimitPrice_SignalCheck();
		this->GetTrader()->ClosebySignal("_limitwin");

//		//多头持仓 资金流出风控平仓
//		this->GetStrategy()->Close_SedimentaryOut_SignalCheck();
//		this->GetTrader()->ClosebySignal("_longsed_out");
	//	//浮盈回撤止损
	//	this->GetStrategy()->DayTrade_WinProfit_SignalCheck();
	//	this->GetTrader()->ClosebySignal("_daytrade_profitclose");

		//止盈价平仓
		this->GetStrategy()->Close_WinPrice_SignalCheck();
		this->GetTrader()->ClosebySignal("_daytrade_winclose");

		//成交量活跃度逆转
	//	this->GetStrategy()->DayTrade_VolumeReverse_SignalCheck();
	//	this->GetTrader()->ClosebySignal("_daytrade_volumereverse");

		//平仓如果存在跳价,则进行撤单操作
		this->GetStrategy()->QueueingClosePriceChange_SignalCheck();
		this->GetTrader()->QueueingClosePriceChange_CancelCheck();
	}
}
void TradeSystem::Fun_Trade_OpenCheck(){
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	DateUtil dateutil;

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0 )	{
//		this->GetStrategy()->Open_Simple();
//		this->GetStrategy()->Open_CalendarSpreadSignal();
		this->GetStrategy()->Open_BigVolumOneMinSignal();
//		this->GetStrategy()->MysqlSectionAssess("_bigvolume");
//		this->GetSecAssess()->SectionAssess(this->GetStrategy(),"_spread");  // 对开仓信号 按照板块进行整理
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0){
//		this->GetStrategy()->Open_BigVolumOneMinSignal();
		this->GetStrategy()->Open_AbsMaxUpDownRate();
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
					strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){
		this->GetStrategy()->Open_BigVolumOneMinSignal();
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0){
		this->GetStrategy()->Open_AbsMaxUpDownRate();
	}
	else{
		this->GetStrategy()->Open_SpreadSignal_Average();
		this->GetSecAssess()->SectionAssess(this->GetStrategy(),"_spread");  // 对开仓信号 按照板块进行整理
	}


	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
				strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){

		if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetTrader()->GetDifSec())){
//			this->GetTrader()->TradeOpen_Global_Section_Check(this->GetStrategy(),"_spread");
//			this->GetTrader()->TradeOpen_Section_Check("_bigvolume");
			this->GetTrader()->TradeOpen_Limit_Check("_bigvolume");

			// 开仓如果存在跳价  则进行撤单操作
			this->GetStrategy()->QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
			this->GetTrader()->QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
		}

	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0 ){
		if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetTrader()->GetDifSec())){
//			this->GetTrader()->TradeOpen_Global_Section_Check(this->GetStrategy(),"_spread");
//			this->GetTrader()->TradeOpen_Section_Check("_bigvolume");
			this->GetTrader()->TradeOpen_Limit_Check("_bigvolume");


			// 开仓如果存在跳价  则进行撤单操作
			this->GetStrategy()->QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
			this->GetTrader()->QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
		}
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0 ){
			if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetTrader()->GetDifSec())){
	//			this->GetTrader()->TradeOpen_Global_Section_Check(this->GetStrategy(),"_spread");
	//			this->GetTrader()->TradeOpen_Section_Check("_bigvolume");
				this->GetTrader()->TradeOpen_Limit_Check("_absmaxupdown");


				// 开仓如果存在跳价  则进行撤单操作
				this->GetStrategy()->QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
				this->GetTrader()->QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
			}
		}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0 ){
				if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetTrader()->GetDifSec())){
					this->GetTrader()->TradeOpen_Limit_Check("_absmaxupdown");

					// 开仓如果存在跳价  则进行撤单操作
					this->GetStrategy()->QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
					this->GetTrader()->QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
				}
			}
	else{
		if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetTrader()->GetDifSec())){
			this->GetTrader()->TradeOpen_Global_SecAssess(this->GetStrategy(),this->GetSecAssess(),"_spread");

			// 开仓如果存在跳价  则进行撤单操作
			this->GetStrategy()->QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
			this->GetTrader()->QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
		}
	}

}
void TradeSystem::ThreadRun_DayTrade(const char* staname){

	this->SetStrategyName(staname);
	Config config(this->GetStrategyName().c_str());
	DateUtil dateutil;
	dateutil.IsCurrentFridayisMonthEnd();

	char logbuf[1024];
	std::string system_log_name="system";
	system_log_name.append("_");
	system_log_name.append(config.GetUserID().c_str());


	std::string system_logfilename="system";
	system_logfilename.append("_");
	system_logfilename.append(config.GetUserID().c_str());
	system_logfilename.append(".log");

	this->SetLogName(system_log_name.c_str());
	this->SetLogPathFileName(system_logfilename.c_str());
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	sprintf(logbuf,"Trade:-----------------开始自动交易程序!!!!!!   %s" ,dateutil.GetCurrentSqlTimeString().c_str());
	logutil.WriteLog(logbuf);
	printf("Trade:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0 ||
			strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0	||
						strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
	//	redisdepthmarket.FlushDB();
		redissignal.FlushDB();
		redissection.FlushDB();
		printf("Trade:开始启动程序  --->>>>redis 清空 ok \n");
	}

	//初始化Mysql数据库table
//	DropSqlTable();
//	printf("Trade:开始启动程序  --->>>>DropSqlTable ok \n");
	InitialSqlTable();
	printf("Trade:开始启动程序  --->>>>InitialSqlTable ok \n");

	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	CodeUtil codeutil;
	std::string flowpath=codeutil.GetCurrentDirectory();
	flowpath.append("/");
	flowpath.append(this->GetStrategyName().c_str());
	flowpath.append("Flow/");
	if(access(flowpath.c_str(),0)==-1){//access函数是查看文件是不是存在
		printf("Trade:开始启动程序  --->>>>不存在文件夹 %s \n",flowpath.c_str());
		 if (mkdir(flowpath.c_str(),0777)){//如果不存在就用mkdir函数来创建
			 printf("Trade:开始启动程序  --->>>>创建文件夹 %s Error!!!  \n",flowpath.c_str());
		}
		 else{
			 printf("Trade:开始启动程序  --->>>>创建文件夹 %s 完成!  \n",flowpath.c_str());
		 }
	}
	else{
//		printf("Trade:开始启动程序  --->>>>存在文件夹 %s \n",flowpath.c_str());
	}
	printf("Trade:开始启动程序  --->>>>创建ctp进程 %s  \n",flowpath.c_str());
	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());

	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	spi.SetStrategyName(this->GetStrategyName().c_str());
	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);

	printf("Trade:开始启动程序  --->>>>00000 \n");

	TrendTrader trader;

	trader.SetStrategyName(this->GetStrategyName().c_str());
	trader.SetUserLoginField();
	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());

	MysqlAsset mysqlasset(config.GetUserID().c_str());
	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
	mysqlinvest.CreateTable();

	std::string trader_logname="trader";
	trader_logname.append("_");
	trader_logname.append(config.GetUserID().c_str());

	std::string trader_logfilename="trader";
	trader_logfilename.append("_");
	trader_logfilename.append(config.GetUserID().c_str());
	trader_logfilename.append(".log");

	trader.SetLogName(trader_logname.c_str());
	trader.SetLogPathFileName(trader_logfilename.c_str());
	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetMysqlAsset(&mysqlasset);
	trader.SetMysqlInvestPosition(&mysqlinvest);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);

	this->SetTrader(&trader);

	printf("Trade:开始启动程序  --->>>>Strategy \n");

	RedisDepthMarket redisdepthmarket_sta;
	RedisSignal redissignal_sta;
	RedisSection redissection_sta;

	Strategy strategy(this->GetStrategyName().c_str());
	strategy.SetRedisDepthMarket(&redisdepthmarket_sta);
	strategy.SetRedisSignal(&redissignal_sta);
	strategy.SetRedisSection(&redissection_sta);

	std::string stalog_name="sta";
	stalog_name.append("_");
	stalog_name.append(config.GetUserID().c_str());


	std::string stalogfilename="sta";
	stalogfilename.append("_");
	stalogfilename.append(config.GetUserID().c_str());
	stalogfilename.append(".log");
	strategy.SetLogName(stalog_name.c_str());
	strategy.SetLogPathFileName(stalogfilename.c_str());
	this->SetStrategy(&strategy);


	printf("Trade:开始启动程序  --->>>>LocalLogin \n");
	this->LocalLogin();
	trader.UpdateTradingDay();
	trader.UpdateTradingDaySqlTime();
	trader.UpdateDifSec();

	strategy.SetDifSec(this->GetTrader()->GetDifSec());
	strategy.UpdateTradingDay();

	if(this->GetCTraderSpi()->GetFlagLogin())	{
		printf("Trade:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime :%s\n", this->GetCTraderSpi()->GetTest().c_str(),
				trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
		//初始化逻辑判断信号
		InitialFlag();
		dateutil.CheckWaitSecondsbySHFELoginMarketTime(trader.GetTradingDay().c_str());

	}
//	sleep(1);
	double avalible;
	// 初始化TradeApi
	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
			trader.InitialTradeApi();   //包含主力合约更新
			avalible =47000.0;
			trader.FixAccountCheck(avalible);
			strategy.SetAvalible(trader.GetAvalible());
			printf("CheckAllHttpDayLine   --->>>> %s\n",dateutil.GetCurrentSqlTimeString().c_str());

	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0){
		trader.InitialTradeApi();   //包含主力合约更新

		avalible =47000.0;
		trader.FixAccountCheck(avalible);
		strategy.SetAvalible(trader.GetAvalible());



	}
	else if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){
		trader.InitialTradeApi();   //包含主力合约更新

		avalible =47000.0;
		trader.FixAccountCheck(avalible);
		strategy.SetAvalible(trader.GetAvalible());


	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0){
		trader.InitialTradeApibyUser();
		avalible =100000.0;
		trader.FixAccountCheck(avalible);
		strategy.SetAvalible(trader.GetAvalible());

	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0){
		trader.InitialTradeApibyUser();
		avalible =47000.0;
		trader.FixAccountCheck(avalible);
		strategy.SetAvalible(trader.GetAvalible());

	}
	else{
		trader.InitialTradeApibyUser();

		avalible =47000.0;
		trader.FixAccountCheck(avalible);
		strategy.SetAvalible(trader.GetAvalible());
	}
	logutil.WriteLog("Trade:  自动交易程序启动后 初始工作完成！");

	strategy.start();
//	strategy.join();

	printf("Trade:ready while ---------------------------------------------------------------->  \n");
	logutil.WriteLog("Trade:  自动交易程序启动后  准备进入定期循环任务！");


	while(1)		{
//			UpdateFlag();


//			DateUtil dateutil;
			bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
			bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
			bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
			bool flag_holiday = dateutil.TodayIsHoliday();

			bool flagfrontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
			bool front_tradelimit = trader.GetFlagTradeLimit(); //网络限流
			bool flagauthenticate_ok = true; //终端授权成功
			bool flaglogin_ok = this->GetCTraderSpi()->GetFlagLogin();    //登录成功
			bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码


			bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
			bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode &&flag_openmarket; // 网络连接成功,未登录
			bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode&&flag_openmarket; // 网络连接成功,未登录
			bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&!front_tradelimit; //网络连接成功,完成登录
			bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&& !flag_holiday && flag_openmarket&&!front_tradelimit;
			bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&front_tradelimit;

			if(connect_error){
				printf("Trade:   网络链接断开!!!! \n");

				int waitseconds = dateutil.CheckWaitSecondsbyOpenTime();
				if(waitseconds>=1800){
					printf("Trade:  网络链接断开!!!! ---->等待[1800]秒后重新链接！\n");
					sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[1800]秒后重新链接！  %s",dateutil.GetCurrentSqlTimeString().c_str());
					logutil.WriteLog(logbuf);
					sleep(1800);
				}
				else if(waitseconds>0){
					printf("Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
					sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
					logutil.WriteLog(logbuf);
					sleep(waitseconds);
				}
				else{
					waitseconds=10;
					printf("Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
					sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
					logutil.WriteLog(logbuf);
					sleep(waitseconds);
				}


			}
			else if(ready_login){
				printf("Trade:   网络链接成功,未登录,等待重新login!!!! \n");
				logutil.WriteLog("Trade:  网络链接成功,未登录,等待重新login!!!! ");
				Fun_ReadyLogin();
			}
			else if(login_tradelimit){
				printf("Trade:限制流量 请重新登录 ***************************************************************************   [%s] \n",
								dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

				sprintf(logbuf,"Trade:限制流量 请重新登录   [%s] \n",	dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
				logutil.WriteLog(logbuf);
				trader.UserLogout();

				if (!this->GetCTraderSpi()->GetFlagLogin()){
					this->GetTrader()->SetFlagTradeLimit(false);
					sprintf(logbuf,"Trade:限制流量 登出完成!!!   [%s] \n",
							dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
					logutil.WriteLog(logbuf);
				}
				sleep(5);
			}
			else if (market_ok)			{
//				else if (1)			{

				printf("Trade[%s]:date ***************************************************************************   [%s] \n",
					config.GetUserID().c_str(),dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

	//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));
				if (spi.GetFlagOrderInsert())	{

					logutil.WriteLog("Trade:  自动交易程序---> 委托报单成交后,更新持仓列表开始！");

					//查询后台
					trader.ReqQryTrade();
					trader.ReqQryInvestorPositionDetail();
					trader.ReqQryInvestorPosition();

					//更新本地数据
					trader.DayTrade_CheckSqlHoldPositionbyPosDetail();
//					trader.CheckPosDetailbySqlHoldPosition();
					trader.CheckInvestbySqlHoldPosition();
					trader.SectionHoldingCheck();

					spi.SetFlagOrderInsert(false);
					logutil.WriteLog("Trade:  自动交易程序---> 委托报单成交后,更新持仓列表结束！");
				}

				trader.FixAccountCheck(avalible);
				trader.SectionHoldingCheck();

//				strategy.SetAvalible(trader.GetAvalible());
				strategy.SetAvalible(avalible);

//				strategy.SectionMarketView();
//				trader.TotalHoldingCheck();
//				trader.HoldPositionDetailCheck();

				//开仓策略
//				strategy.GlobalMarketView();
//				strategy.SectionMarketView();


				printf("\n");
				//持仓策略
				if  (trader.GetTotalHolding()>0)  	{//如果存在持仓,则启动平仓策略 进行轮询

				//尾盘平仓策略
				strategy.DayTrade_EndTimeClose_SignalCheck();
				trader.ClosebySignal("_daytrade_endtime");

				//固定止损
//				strategy.DayTrade_Stop_SignalCheck();
//				trader.StopPriceCheck();

				//浮动止盈止损
				strategy.Close_DynamicStop_SignalCheck();
				trader.ClosebySignal("_dynamicstop");

				//板块趋势反向 平仓
				strategy.Close_SectionTrendStop_SignalCheck();
				trader.ClosebySignal("_sectiontrendstop");

				//
				strategy.Close_LimitPrice_SignalCheck();
				trader.ClosebySignal("_limitwin");

//				//浮盈回撤止损
//				strategy.DayTrade_WinProfit_SignalCheck();
//				trader.ClosebySignal("_daytrade_profitclose");

				//止盈价止损
//				strategy.DayTrade_WinPrice_SignalCheck();
//				trader.ClosebySignal("_daytrade_winclose");

				//成交量活跃度逆转
//				strategy.DayTrade_VolumeReverse_SignalCheck();
//				trader.ClosebySignal("_daytrade_volumereverse");

				//平仓如果存在跳价,则进行撤单操作
				strategy.QueueingClosePriceChange_SignalCheck();
				trader.QueueingClosePriceChange_CancelCheck();
				}

				printf("\n");

				if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0){

					strategy.Open_CalendarSpreadATRSignal();
					strategy.MysqlSectionAssess("_spread_atr");  // 对开仓信号 按照板块进行整理
	//				trader.DayTrade_OpenCheck();  //新开仓合约检查

					if (!dateutil.CheckSHFEEndCloseMarketTime(trader.GetDifSec())){
						trader.TradeOpen_Global_Section_Check(&strategy,"_spread_atr");  //新开仓合约检查

						// 开仓如果存在跳价  则进行撤单操作
						strategy.QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
						trader.QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
					}
				}
				else{
//					strategy.Open_CalendarSpreadSignal();
//					strategy.SectionAssess("_spread");  // 对开仓信号 按照板块进行整理

					if (!dateutil.CheckSHFEEndCloseMarketTime(trader.GetDifSec())){
						trader.TradeOpen_Global_Section_Check(&strategy,"_spread");  //新开仓合约检查

//						trader.TradeOpen_Global_SecAssess(&strategy,"_spread");  //新开仓合约检查

						// 开仓如果存在跳价  则进行撤单操作
						strategy.QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
						trader.QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
					}
				}

			}

			if (flaglogin_ok){
				AutoTask_AfterNoonClosebyUser();
				AutoTask_BeforNightbyUser();

			}
			AutoTask();
			AutoTaskEveryDay();


		}

	return;

}
void TradeSystem::Run_DayTrade(const char* staname){

	this->SetStrategyName(staname);
	Config config(this->GetStrategyName().c_str());
	this->SetLogProperty(config.GetUserID().c_str());

	DateUtil dateutil;
	dateutil.IsCurrentFridayisMonthEnd();

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	printf("Trade:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
	sprintf(logbuf,"Trade:-----------------开始自动交易程序!!!!!!   %s" ,dateutil.GetCurrentSqlTimeString().c_str());
	logutil.WriteLog(logbuf);

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0 ||
			strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
			strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
	//	redisdepthmarket.FlushDB();
		redissignal.FlushDB();
		redissection.FlushDB();
		printf("Trade:开始启动程序  --->>>>redis 清空 ok \n");
	}

	//初始化Mysql数据库table
//	DropSqlTable();

	MysqlSecAssess mysqlsecassess_d(config.GetUserID().c_str());
	if(mysqlsecassess_d.Exist_Table() ){
		mysqlsecassess_d.DropTable();
		printf("	DropSqlTable:清空数据表  MysqlSecAssess !!!\n");
	}

//	printf("Trade:开始启动程序  --->>>>DropSqlTable ok \n");
	InitialSqlTable();
	printf("Trade:开始启动程序  --->>>>InitialSqlTable ok \n");

	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	CodeUtil codeutil;
	std::string flowpath=this->CheckFlowPath();
	printf("Trade:开始启动程序  --->>>>创建ctp进程 %s  \n",flowpath.c_str());
	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());

	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	spi.SetStrategyName(this->GetStrategyName().c_str());
	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);

	printf("Trade:开始启动程序  --->>>>00000 \n");

	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());

	MysqlAsset mysqlasset(config.GetUserID().c_str());
	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
	mysqlinvest.CreateTable();

	TrendTrader trader(staname);
	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetMysqlAsset(&mysqlasset);
	trader.SetMysqlInvestPosition(&mysqlinvest);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);

	this->SetTrader(&trader);

	printf("Trade:开始启动程序  --->>>>SecAssess \n");
	SecAssess secassess;
	secassess.SetSignalLabel("_spread");
	secassess.SetStrategyName(this->GetStrategyName().c_str());
	secassess.SetSectionAcessessLists(config.GetUserID().c_str());
	this->SetSecAssess(&secassess);

	Strategy strategy(this->GetStrategyName().c_str());
	strategy.SetRedisDepthMarket(&redisdepthmarket);
	strategy.SetRedisSignal(&redissignal);
	strategy.SetRedisSection(&redissection);


	this->SetStrategy(&strategy);


	ZeroMqUtil zeromqutil(this->GetStrategyName().c_str());
	zeromqutil.start();
//	zeromqutil.join();

	printf("Trade:开始启动程序  --->>>>LocalLogin--->>>>ApiVersion %s \n",trader.GetCThostFtdcTraderApi()->GetApiVersion());
	this->LocalLogin();
//	sleep(1);
	this->TradeInitial();

	printf("Trade:ready while ---------------------------------------------------------------->  \n");
	logutil.WriteLog("Trade:  自动交易程序启动后  准备进入定期循环任务！");

	while(1)		{
//			UpdateFlag();
//			DateUtil dateutil;
			bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
			bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
			bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
			bool flag_holiday = dateutil.TodayIsHoliday();

			bool flagfrontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
			bool front_tradelimit = trader.GetFlagTradeLimit(); //网络限流

			//终端授权成功
			bool flagauthenticate_ok = false;
			if(	strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
						strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
				flagauthenticate_ok = this->GetCTraderSpi()->GetFlagAuthenticate();
			}
			else{
				flagauthenticate_ok = true;
			}

			bool flaglogin_ok = this->GetCTraderSpi()->GetFlagLogin();    //登录成功
			bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码

			bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
			bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode &&flag_openmarket; // 网络连接成功,未认证,未登录
			bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode&&flag_openmarket; // 网络连接成功,认证成功,未登录
			bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&!front_tradelimit; //网络连接成功,完成登录
			bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&& !flag_holiday && flag_openmarket&&!front_tradelimit;//可以进行交易
			bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&front_tradelimit; //登录成功,交易限制

			if(connect_error){
				printf("Trade:   网络链接断开!!!! \n");
				this->Fun_ConnectError();
			}
			else if(ready_login){
				printf("Trade:   网络链接成功,未登录,等待重新login!!!! \n");
				logutil.WriteLog("Trade:  网络链接成功,未登录,等待重新login!!!! ");
				this->Fun_ReadyLogin();
			}
			else if(login_tradelimit){
				printf("Trade:限制流量 请重新登录 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   [%s] \n",
								dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
				this->Fun_TradeLimit();
			}
			else if (market_ok)			{
//				else if (1)			{
				printf("\nTrade[%s]:date ***************************************************************************   [%s] \n",
					config.GetUserID().c_str(),dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

	//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));


				// 有开仓合约后,需要更新持仓表 用于浮动止损 止盈
				this->Fun_Trade_FlagOrderInsertCheck();
				//账号查询
				trader.FixAccountCheck(this->GetAvalible());
				trader.SectionHoldingCheck();
//				this->GetStrategy()->SetAvalible(trader.GetAvalible());
				this->GetStrategy()->SetAvalible(this->GetAvalible());

				//行情汇总更新
				this->GetStrategy()->SectionMarketView();
				this->GetStrategy()->GlobalMarketView_AbsMaxUpDown();
				printf("\n");

				//持仓策略
				this->Fun_Trade_HoldCheck();
				printf("\n");
				//开仓监测
				this->Fun_Trade_OpenCheck();
			}

			if (flaglogin_ok){
				this->AutoTask_BeforMorningbyUser();
				this->AutoTask_AfterNoonClosebyUser();
				this->AutoTask_BeforNightbyUser();
			}
			this->AutoTask();
			this->AutoTaskEveryDay();
	}

	return;
}
void TradeSystem::Run_Trade(const char* staname){

	this->SetStrategyName(staname);
	Config config(this->GetStrategyName().c_str());
	this->SetLogProperty(config.GetUserID().c_str());
	DateUtil dateutil;
	dateutil.IsCurrentFridayisMonthEnd();

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	printf("Trade:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
	sprintf(logbuf,"Trade:-----------------开始自动交易程序!!!!!!   %s" ,dateutil.GetCurrentSqlTimeString().c_str());
	logutil.WriteLog(logbuf);

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0 ||
			strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
			strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
	//	redisdepthmarket.FlushDB();
		redissignal.FlushDB();
		redissection.FlushDB();
		printf("Trade:开始启动程序  --->>>>redis 清空 ok \n");
	}

	//初始化Mysql数据库table
//	DropSqlTable();

	MysqlSecAssess mysqlsecassess_d(config.GetUserID().c_str());
	if(mysqlsecassess_d.Exist_Table() ){
		mysqlsecassess_d.DropTable();
		printf("	DropSqlTable:清空数据表  MysqlSecAssess !!!\n");
	}

//	printf("Trade:开始启动程序  --->>>>DropSqlTable ok \n");
	InitialSqlTable();
	printf("Trade:开始启动程序  --->>>>InitialSqlTable ok \n");

	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	CodeUtil codeutil;
	std::string flowpath=this->CheckFlowPath();
	printf("Trade:开始启动程序  --->>>>创建ctp进程 %s  \n",flowpath.c_str());
	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());

	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	spi.SetStrategyName(this->GetStrategyName().c_str());
	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);

	printf("Trade:开始启动程序  --->>>>00000 \n");

	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());

	MysqlAsset mysqlasset(config.GetUserID().c_str());
	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
	mysqlinvest.CreateTable();

	TrendTrader trader(staname);
	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetMysqlAsset(&mysqlasset);
	trader.SetMysqlInvestPosition(&mysqlinvest);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);

	this->SetTrader(&trader);

	printf("Trade:开始启动程序  --->>>>SecAssess \n");
	SecAssess secassess;
	secassess.SetSignalLabel("_spread");
	secassess.SetStrategyName(this->GetStrategyName().c_str());
	secassess.SetSectionAcessessLists(config.GetUserID().c_str());
	this->SetSecAssess(&secassess);

	Strategy strategy(this->GetStrategyName().c_str());
	strategy.SetRedisDepthMarket(&redisdepthmarket);
	strategy.SetRedisSignal(&redissignal);
	strategy.SetRedisSection(&redissection);


	this->SetStrategy(&strategy);



	ZeroMqUtil zeromqutil(this->GetStrategyName().c_str());
	zeromqutil.start();
//	zeromqutil.join();

	printf("Trade:开始启动程序  --->>>>LocalLogin--->>>>ApiVersion %s \n",trader.GetCThostFtdcTraderApi()->GetApiVersion());
	this->LocalLogin();
//	sleep(1);
	this->TradeInitial();

	printf("Trade:ready while ---------------------------------------------------------------->  \n");
	logutil.WriteLog("Trade:  自动交易程序启动后  准备进入定期循环任务！");

	while(1)		{
//			UpdateFlag();
//			DateUtil dateutil;
			bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
			bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
			bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
			bool flag_holiday = dateutil.TodayIsHoliday();

			bool flagfrontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
			bool front_tradelimit = trader.GetFlagTradeLimit(); //网络限流

			//终端授权成功
			bool flagauthenticate_ok = false;
			if(	strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
						strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
				flagauthenticate_ok = this->GetCTraderSpi()->GetFlagAuthenticate();
			}
			else{
				flagauthenticate_ok = true;
			}

			bool flaglogin_ok = this->GetCTraderSpi()->GetFlagLogin();    //登录成功
			bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码

			bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
			bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode &&flag_openmarket; // 网络连接成功,未认证,未登录
			bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode&&flag_openmarket; // 网络连接成功,认证成功,未登录
			bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&!front_tradelimit; //网络连接成功,完成登录
			bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&& !flag_holiday && flag_openmarket&&!front_tradelimit;//可以进行交易
			bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&front_tradelimit; //登录成功,交易限制

			if(connect_error){
				printf("Trade:   网络链接断开!!!! \n");
				this->Fun_ConnectError();
			}
			else if(ready_login){
				printf("Trade:   网络链接成功,未登录,等待重新login!!!! \n");
				logutil.WriteLog("Trade:  网络链接成功,未登录,等待重新login!!!! ");
				this->Fun_ReadyLogin();
			}
			else if(login_tradelimit){
				printf("Trade:限制流量 请重新登录 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   [%s] \n",
								dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
				this->Fun_TradeLimit();
			}
			else if (market_ok)			{
//				else if (1)			{
				printf("\nTrade[%s]:date ***************************************************************************   [%s] \n",
					config.GetUserID().c_str(),dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

	//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));


				// 有开仓合约后,需要更新持仓表 用于浮动止损 止盈
				this->Fun_Trade_FlagOrderInsertCheck();
				//账号查询
				trader.FixAccountCheck(this->GetAvalible());
				trader.SectionHoldingCheck();
//				this->GetStrategy()->SetAvalible(trader.GetAvalible());
				this->GetStrategy()->SetAvalible(this->GetAvalible());

				//行情汇总更新
				this->GetStrategy()->SectionMarketView();
				this->GetStrategy()->GlobalMarketView_AbsMaxUpDown();
				printf("\n");

				//持仓策略
				this->Fun_Trade_HoldCheck();
				printf("\n");
				//开仓监测
				this->Fun_Trade_OpenCheck();
			}

			if (flaglogin_ok){
				this->AutoTask_BeforMorningbyUser();
				this->AutoTask_AfterNoonClosebyUser();
				this->AutoTask_BeforNightbyUser();
			}
			this->AutoTask();
			this->AutoTaskEveryDay();
	}

	return;
}
void TradeSystem::Run_CalendarSpreadTrade_zlqh(){

	std::string staname="CalendarSpreadTrade_zlqh";
	this->SetStrategyName(staname.c_str());
	Config config(this->GetStrategyName().c_str());
	DateUtil dateutil;
	char logbuf[1024];

	std::string system_log_name="system";
	system_log_name.append("_");
	system_log_name.append(config.GetUserID().c_str());


	std::string system_logfilename="system";
	system_logfilename.append("_");
	system_logfilename.append(config.GetUserID().c_str());
	system_logfilename.append(".log");

	this->SetLogName(system_log_name.c_str());
	this->SetLogPathFileName(system_logfilename.c_str());
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	logutil.WriteLog("Run_CalendarSpreadTrade_zlqh:-----------------开始自动交易程序!!!!!!");

	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
//	redisdepthmarket.FlushDB();
	redissignal.FlushDB();
	redissection.FlushDB();
	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>redis 清空 ok \n");

	//初始化Mysql数据库table
//	DropSqlTable();
//	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>DropSqlTable ok \n");

	InitialSqlTable();
	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>InitialSqlTable ok \n");
	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	CodeUtil codeutil;
	std::string flowpath=codeutil.GetCurrentDirectory();
	flowpath.append("/");
	flowpath.append(staname.c_str());
	flowpath.append("Flow/");

	if(access(flowpath.c_str(),0)==-1){//access函数是查看文件是不是存在
		printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>不存在文件夹 %s \n",flowpath.c_str());
		 if (mkdir(flowpath.c_str(),0777)){//如果不存在就用mkdir函数来创建
			 printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>创建文件夹 %s Error!!!  \n",flowpath.c_str());
		}
		 else{
			 printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>创建文件夹 %s 完成!  \n",flowpath.c_str());
		 }
	}
	else{
//		printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>存在文件夹 %s \n",flowpath.c_str());
	}
	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());


	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	spi.SetStrategyName(this->GetStrategyName().c_str());
	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);

	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>00000 \n");

	TrendTrader trader;

	trader.SetStrategyName(this->GetStrategyName().c_str());
	trader.SetUserLoginField();
	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());


	MysqlAsset mysqlasset(config.GetUserID().c_str());

	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
	if(!mysqlinvest.Exist_Table()){
		mysqlinvest.CreateTable();
	}

	std::string trader_logname="trader";
	trader_logname.append("_");
	trader_logname.append(config.GetUserID().c_str());

	std::string trader_logfilename="trader";
	trader_logfilename.append("_");
	trader_logfilename.append(config.GetUserID().c_str());
	trader_logfilename.append(".log");

	trader.SetLogName(trader_logname.c_str());
	trader.SetLogPathFileName(trader_logfilename.c_str());

	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetMysqlAsset(&mysqlasset);
	trader.SetMysqlInvestPosition(&mysqlinvest);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);
	this->SetTrader(&trader);

	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>Strategy \n");
	Strategy strategy(staname.c_str());
	strategy.SetRedisDepthMarket(&redisdepthmarket);
	strategy.SetRedisSignal(&redissignal);
	strategy.SetRedisSection(&redissection);


//	strategy.SetStrategyName(this->GetStrategyName().c_str());
//	std::string stalog_name="sta";
//	stalog_name.append("_");
//	stalog_name.append(config.GetUserID().c_str());
//
//
//	std::string stalogfilename="sta";
//	stalogfilename.append("_");
//	stalogfilename.append(config.GetUserID().c_str());
//	stalogfilename.append(".log");
//	strategy.SetLogName(stalog_name.c_str());
//	strategy.SetLogPathFileName(stalogfilename.c_str());
	this->SetStrategy(&strategy);

	printf("Run_CalendarSpreadTrade_zlqh:开始启动程序  --->>>>LocalLogin \n");
	printf("Trade:开始启动程序  登录成功--->>>>ApiVersion %s \n",trader.GetCThostFtdcTraderApi()->GetApiVersion());
	this->LocalLogin();
	trader.UpdateTradingDay();
	trader.UpdateTradingDaySqlTime();
	trader.UpdateDifSec();

	strategy.SetDifSec(this->GetTrader()->GetDifSec());
	strategy.UpdateTradingDay();

	if(this->GetCTraderSpi()->GetFlagLogin())	{
		printf("Run_CalendarSpreadTrade_zlqh:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime :%s\n", this->GetCTraderSpi()->GetTest().c_str(),
				trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
		//初始化逻辑判断信号
		InitialFlag();


	}
	sleep(1);

//	 初始化TradeApi
	trader.InitialTradeApi();   //包含主力合约更新
	logutil.WriteLog("Run_CalendarSpreadTrade_zlqh:  自动交易程序启动后 初始工作完成！");

	double avalible =47000.0;
	trader.FixAccountCheck(avalible);
	strategy.SetAvalible(trader.GetAvalible());


	printf("Run_CalendarSpreadTrade_zlqh:ready while ---------------------------------------------------------------->  \n");
	logutil.WriteLog("Run_CalendarSpreadTrade_zlqh:  自动交易程序启动后  准备进入定期循环任务！");


	while(1)		{
//		UpdateFlag();


		DateUtil dateutil;
		bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
		bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
		bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
		bool flag_holiday = dateutil.TodayIsHoliday();

		bool flagfrontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
		//			bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ; //网络断开连接
		bool front_tradelimit = trader.GetFlagTradeLimit(); //网络限流
		bool flagauthenticate_ok = this->GetCTraderSpi()->GetFlagAuthenticate();
		bool flaglogin_ok = this->GetCTraderSpi()->GetFlagLogin();    //未登录
		bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码


		bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
		bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok && noerrorcode &&flag_openmarket; // 网络连接成功,未登录
		bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok && noerrorcode&&flag_openmarket; // 网络连接成功,未登录
		bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode   &&flag_openmarket; //网络连接成功,完成登录
		bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode  && !flag_holiday && flag_openmarket;
		bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode   &&flag_openmarket &&front_tradelimit;

		if(connect_error){
			printf("CalendarSpreadTrade:   网络链接断开!!!! \n");

			int waitseconds = dateutil.CheckWaitSecondsbyOpenTime();
			if(waitseconds>=3600){
				printf("CalendarSpreadTrade:  网络链接断开!!!! ---->等待[3600]秒后重新链接！\n");
				sprintf(logbuf,"CalendarSpreadTrade:  网络链接断开!!!! ---->等待[3600]秒后重新链接！  %s",dateutil.GetCurrentSqlTimeString().c_str());
				logutil.WriteLog(logbuf);
				sleep(3600);
			}
			else if(waitseconds>0){
				printf("CalendarSpreadTrade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				sprintf(logbuf,"CalendarSpreadTrade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				logutil.WriteLog(logbuf);
				sleep(waitseconds);
			}
			else{
				waitseconds=10;
				printf("CalendarSpreadTrade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				sprintf(logbuf,"CalendarSpreadTrade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				logutil.WriteLog(logbuf);
				sleep(waitseconds);
			}

		}
		else if(ready_authenticate){
			sleep(5);
			printf("CalendarSpreadTrade:   网络链接成功,未认证,中粮期货CTP 认证 Begin!!!    \n");
			logutil.WriteLog("CalendarSpreadTrade:  网络链接成功,未认证,等待重新认证!!!! ");
			trader.ReqAuthenticate();
			sleep(5);
		}
		else if(login_tradelimit){
			printf("CalendarSpreadTrade:限制流量 请重新登录 ***************************************************************************   [%s] \n",
							dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

			sprintf(logbuf,"CalendarSpreadTrade:限制流量 请重新登录   [%s] \n",	dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
			logutil.WriteLog(logbuf);
			trader.UserLogout();
			sleep(5);
		}
		else if(ready_login){
			printf("CalendarSpreadTrade:   网络链接成功,未登录,等待重新登录!!!! \n");
			logutil.WriteLog("CalendarSpreadTrade:  网络链接成功,未登录,等待重新登录!!!!");

			/* 重新登录后，重置请求ID*/
			trader.SetRequestID(0);
			trader.UserLogin();

			if (this->GetCTraderSpi()->GetFlagLogin()){
				logutil.WriteLog("CalendarSpreadTrade:  断开后重连 ----------完成！");
				trader.UpdateTradingDay();
				trader.UpdateTradingDaySqlTime();
				trader.UpdateDifSec();
				strategy.SetDifSec(trader.GetDifSec());
				strategy.UpdateTradingDay();

				sprintf(logbuf,"CalendarSpreadTrade:  断开后重连  ----------登录完成！  TradingDay : %s TradingDaySqlTime :%s ",
						trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
				logutil.WriteLog(logbuf);
				sleep(5);
				trader.ReqQryTradingAccount();
				logutil.WriteLog("Run_CalendarSpreadTrade_zlqh:  断开后重连  ----------账户检测完成！");

			}
			else{
				logutil.WriteLog("CalendarSpreadTrade:  断开后重连 --------尝试登录失败!!! ");
				sleep(3);
			}

		}
		else if (market_ok)			{
//				else if (1)			{
			printf("\nCalendarSpreadTrade[%s]:date ***************************************************************************   [%s] \n",
					config.GetUserID().c_str(),dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));
			if (spi.GetFlagOrderInsert())	{

				logutil.WriteLog("CalendarSpreadTrade:  自动交易程序---> 委托报单成交后,更新持仓列表开始！");

				//查询后台
				trader.ReqQryTrade();
				trader.ReqQryInvestorPositionDetail();
				trader.ReqQryInvestorPosition();

				//更新本地数据
				trader.DayTrade_CheckSqlHoldPositionbyPosDetail();
//					trader.CheckPosDetailbySqlHoldPosition();
				trader.CheckInvestbySqlHoldPosition();
				trader.SectionHoldingCheck();

				spi.SetFlagOrderInsert(false);
				logutil.WriteLog("CalendarSpreadTrade:  自动交易程序---> 委托报单成交后,更新持仓列表结束！");
			}

			trader.FixAccountCheck(avalible);
			trader.SectionHoldingCheck();
//				strategy.SetAvalible(trader.GetAvalible());
			strategy.SetAvalible(avalible);

//				strategy.SectionMarketView();
//				trader.TotalHoldingCheck();
//				trader.HoldPositionDetailCheck();

			//开仓策略
			strategy.GlobalMarketView();
			strategy.SectionMarketView();

			//持仓策略
			if  (trader.GetTotalHolding()>0)  	{//如果存在持仓,则启动平仓策略 进行轮询

			//尾盘平仓策略
			strategy.DayTrade_EndTimeClose_SignalCheck();
			trader.ClosebySignal("_daytrade_endtime");

			//固定止损
//				strategy.DayTrade_Stop_SignalCheck();
//				trader.StopPriceCheck();

			//浮动止盈止损
			strategy.Close_DynamicStop_SignalCheck();
			trader.ClosebySignal("_dynamicstop");

			//板块趋势反向 平仓
			strategy.Close_SectionTrendStop_SignalCheck();
			trader.ClosebySignal("_sectiontrendstop");

			//涨跌停平仓
			strategy.Close_LimitPrice_SignalCheck();
			trader.ClosebySignal("_limitwin");

			//多头持仓 资金流出风控平仓
			strategy.Close_SedimentaryOut_SignalCheck();
			trader.ClosebySignal("_longsed_out");

//				//浮盈回撤止损
//				strategy.DayTrade_WinProfit_SignalCheck();
//				trader.ClosebySignal("_daytrade_profitclose");

			//止盈价止损
//				strategy.DayTrade_WinPrice_SignalCheck();
//				trader.ClosebySignal("_daytrade_winclose");

			//成交量活跃度逆转
//				strategy.DayTrade_VolumeReverse_SignalCheck();
//				trader.ClosebySignal("_daytrade_volumereverse");

			//平仓如果存在跳价,则进行撤单操作
			strategy.QueueingClosePriceChange_SignalCheck();
			trader.QueueingClosePriceChange_CancelCheck();
			}

			strategy.Open_CalendarSpreadSignal_Average();
			strategy.MysqlSectionAssess("_spread");  // 对开仓信号 按照板块进行整理
//				trader.TradeOpen_Section_Check();  //新开仓合约检查

			if (!dateutil.CheckSHFEEndCloseMarketTime(trader.GetDifSec())){
				trader.TradeOpen_Global_Section_Check(&strategy,"_spread");  //新开仓合约检查

				// 开仓如果存在跳价  则进行撤单操作
				strategy.QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
				trader.QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
			}
		}

		if (flaglogin_ok){
			AutoTask_AfterNoonClosebyUser();
			AutoTask_BeforNightbyUser();

		}
		AutoTask();
		AutoTaskEveryDay();
	}

	return;

}
void TradeSystem::Run_Trade_zlqh(const char* staname){

	this->SetStrategyName(staname);
	Config config(this->GetStrategyName().c_str());
	DateUtil dateutil;
	char logbuf[1024];
	this->SetLogProperty(config.GetUserID().c_str());

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	logutil.WriteLog("Run_Trade_zlqh:-----------------开始自动交易程序!!!!!!");

	printf("Run_Trade_zlqh:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
//	redisdepthmarket.FlushDB();
	redissignal.FlushDB();
	redissection.FlushDB();
	printf("Run_Trade_zlqh:开始启动程序  --->>>>redis 清空 ok \n");

	//初始化Mysql数据库table
//	DropSqlTable();
//	printf("Run_Trade_zlqh:开始启动程序  --->>>>DropSqlTable ok \n");

	InitialSqlTable();
	printf("Run_Trade_zlqh:开始启动程序  --->>>>InitialSqlTable ok \n");
	CodeUtil codeutil;

	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	std::string flowpath=this->CheckFlowPath();
	printf("Run_Trade_zlqh:开始启动程序  --->>>>创建ctp进程 %s  \n",flowpath.c_str());
	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());

	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	spi.SetStrategyName(this->GetStrategyName().c_str());

	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);

	printf("Run_Trade_zlqh:开始启动程序  --->>>>00000 \n");
	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());


	MysqlAsset mysqlasset(config.GetUserID().c_str());

	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
	if(!mysqlinvest.Exist_Table()){
		mysqlinvest.CreateTable();
	}

	TrendTrader trader(staname);
	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetMysqlAsset(&mysqlasset);
	trader.SetMysqlInvestPosition(&mysqlinvest);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);

	this->SetTrader(&trader);

	SecAssess secassess;
	secassess.SetSignalLabel("_spread");
	secassess.SetStrategyName(this->GetStrategyName().c_str());
	secassess.SetSectionAcessessLists(config.GetUserID().c_str());
	this->SetSecAssess(&secassess);


	printf("Run_Trade_zlqh:开始启动程序  --->>>>Strategy \n");
	Strategy strategy(staname);
	strategy.SetRedisDepthMarket(&redisdepthmarket);
	strategy.SetRedisSignal(&redissignal);
	strategy.SetRedisSection(&redissection);


	this->SetStrategy(&strategy);

	printf("Run_Trade_zlqh:开始启动程序  --->>>>LocalLogin \n");
	printf("Trade:开始启动程序  登录成功--->>>>ApiVersion %s \n",trader.GetCThostFtdcTraderApi()->GetApiVersion());
	this->LocalLogin();

	this->TradeInitial();


	printf("Run_Trade_zlqh:ready while ---------------------------------------------------------------->  \n");
	logutil.WriteLog("Run_Trade_zlqh:  自动交易程序启动后  准备进入定期循环任务！");


	while(1)		{
//		UpdateFlag();


		DateUtil dateutil;
		bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
		bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
		bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
		bool flag_holiday = dateutil.TodayIsHoliday();

		bool flagfrontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
		//			bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ; //网络断开连接
		bool front_tradelimit = trader.GetFlagTradeLimit(); //网络限流
		bool flagauthenticate_ok = this->GetCTraderSpi()->GetFlagAuthenticate();
		bool flaglogin_ok = this->GetCTraderSpi()->GetFlagLogin();    //未登录
		bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码


		bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
		bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok && noerrorcode &&flag_openmarket; // 网络连接成功,未登录
		bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok && noerrorcode&&flag_openmarket; // 网络连接成功,未登录
		bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode   &&flag_openmarket; //网络连接成功,完成登录
		bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode  && !flag_holiday && flag_openmarket;
		bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode   &&flag_openmarket &&front_tradelimit;

		if(connect_error){
			printf("Run_Trade_zlqh:   网络链接断开!!!! \n");
			this->Fun_ConnectError();
		}
		else if(ready_authenticate){
			sleep(5);
			printf("Run_Trade_zlqh:   网络链接成功,未认证,中粮期货CTP 认证 Begin!!!    \n");
			logutil.WriteLog("Run_Trade_zlqh:  网络链接成功,未认证,等待重新认证!!!! ");
			trader.ReqAuthenticate();
			sleep(5);
		}
		else if(login_tradelimit){
			printf("Run_Trade_zlqh:限制流量 请重新登录 ***************************************************************************   [%s] \n",
							dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
			this->Fun_TradeLimit();
		}
		else if(ready_login){
			printf("Run_Trade_zlqh:   网络链接成功,未登录,等待重新登录!!!! \n");
			logutil.WriteLog("Run_Trade_zlqh:  网络链接成功,未登录,等待重新登录!!!!");

			this->Fun_ReadyLogin();
		}
		else if (market_ok)			{
//				else if (1)			{
			printf("\nRun_Trade_zlqh[%s]:date ***************************************************************************   [%s] \n",
					config.GetUserID().c_str(),dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));
			this->Fun_Trade_FlagOrderInsertCheck();

			//账号查询
			this->GetTrader()->FixAccountCheck(this->GetAvalible());
			this->GetTrader()->SectionHoldingCheck();
//				this->GetStrategy()->SetAvalible(trader.GetAvalible());
			this->GetStrategy()->SetAvalible(this->GetAvalible());

			//行情汇总更新
			this->GetStrategy()->SectionMarketView();
			this->GetStrategy()->GlobalMarketView();

			this->Fun_Trade_HoldCheck();

			this->Fun_Trade_OpenCheck();
		}

		if (flaglogin_ok){
			AutoTask_AfterNoonClosebyUser();
			AutoTask_BeforNightbyUser();

		}
		AutoTask();
		AutoTaskEveryDay();
	}

	return;

}
void TradeSystem::Run_TrendTrade(const char*systemname){

	std::string staname=systemname;

	this->SetStrategyName(staname.c_str());

	Config config(this->GetStrategyName().c_str());
	DateUtil dateutil;

	MysqlAsset mysqlasset_d(config.GetUserID().c_str());
		if(mysqlasset_d.Exist_Table()){
			mysqlasset_d.DropTable();

			printf("TrendTradeSystem:清空数据表  MysqlAsset \n");
		}

	MysqlSecAssess mysqlsecassess_d(config.GetUserID().c_str());
	if(!mysqlsecassess_d.Exist_Table()){
		mysqlsecassess_d.DropTable();

		printf("TrendTradeSystem:清空数据表  MysqlSecAssess \n");
	}

	std::string logfilename=this->GetStrategyName();
	logfilename.append("_");
	logfilename.append(config.GetUserID().c_str());
	logfilename.append(".log");
	char logbuf[1024];
	this->SetLogPathFileName(logfilename.c_str());
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	logutil.WriteLog("TrendTradeSystem:-----------------开始自动交易程序!!!!!!");

	printf("TrendTradeSystem:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());

	redissignal.FlushDB();
	redissection.FlushDB();
	redisdepthmarket.FlushDB();

	//初始化Mysql数据库table
	InitialSqlTable();

	printf("TrendTradeSystem:开始启动程序  --->>>>00000 \n");
	// 产生一个CThostFtdcTraderApi实例
	CThostFtdcTraderApi* pTraderApi;
	CodeUtil codeutil;
	std::string flowpath=codeutil.GetCurrentDirectory();
	flowpath.append("/");
	flowpath.append(staname.c_str());
	flowpath.append("Flow/");

	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());
	// 产生一个事件处理的实例
	CTraderSpi spi(pTraderApi);
	spi.SetStrategyName(this->GetStrategyName().c_str());
	this->SetCThostFtdcTraderApi(pTraderApi);
	this->SetCTraderSpi(&spi);



	printf("TrendTradeSystem:开始启动程序  --->>>>TrendTrade \n");
	TrendTrader trader;


	trader.SetStrategyName(this->GetStrategyName().c_str());
	trader.SetUserLoginField();
	std::string spilogfilename="traderspi";
	spilogfilename.append("_");
	spilogfilename.append(config.GetUserID().c_str());
	spilogfilename.append(".log");
	this->GetCTraderSpi()->SetLogName(config.GetUserID().c_str());
	this->GetCTraderSpi()->SetLogPathFileName(spilogfilename.c_str());



	MysqlAsset mysqlasset(config.GetUserID().c_str());

	MysqlInvestPosition mysqlinvest(config.GetUserID().c_str());
	mysqlinvest.CreateTable();


	trader.SetLogName(this->GetStrategyName().c_str());
	trader.SetLogPathFileName(logfilename.c_str());

	trader.SetCThostFtdcTraderApi(pTraderApi);
	trader.SetCTraderSpi(&spi);
	trader.SetMysqlAsset(&mysqlasset);
	trader.SetMysqlInvestPosition(&mysqlinvest);
	trader.SetRedisDepthMarket(&redisdepthmarket);
	trader.SetRedisSignal(&redissignal);
	trader.SetRedisSection(&redissection);


	printf("TrendTradeSystem:开始启动程序  --->>>>initial  userid: %s  strategy: %s \n",
			trader.GetPointUserLoginField()->UserID,trader.GetStrategyName().c_str());
	this->SetTrader(&trader);

	Strategy strategy(staname.c_str());
	strategy.SetRedisDepthMarket(&redisdepthmarket);
	strategy.SetRedisSignal(&redissignal);
	strategy.SetRedisSection(&redissection);

	strategy.SetStrategyName(staname.c_str());
	std::string stalogfilename="sta";
	stalogfilename.append("_");
	stalogfilename.append(config.GetUserID().c_str());
	stalogfilename.append(".log");

	strategy.SetLogName(this->GetStrategyName().c_str());
	strategy.SetLogPathFileName(logfilename.c_str());
	this->SetStrategy(&strategy);

	this->LocalLogin();
	trader.UpdateTradingDay();
	trader.UpdateTradingDaySqlTime();
	trader.UpdateDifSec();

	strategy.SetDifSec(this->GetTrader()->GetDifSec());
	strategy.UpdateTradingDay();


	if(this->GetCTraderSpi()->GetFlagLogin())
	{
		printf("TrendTradeSystem:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime  :%s \n", this->GetCTraderSpi()->GetTest().c_str(),
				trader.GetTradingDay().c_str(),trader.GetTradingDaySqlTime().c_str());
		//初始化逻辑判断信号
		InitialFlag();
	}

	sleep(1);

	// 初始化TradeApi
	trader.InitialTradeApibyUser();
	logutil.WriteLog("TrendTradeSystem:  自动交易程序启动后 初始工作完成！");

	trader.AccountSelfCheck();
	strategy.SetAvalible(trader.GetAvalible());

	printf("TrendTradeSystem:ready while ---------------------------------------------------------------->  \n");

	logutil.WriteLog("TrendTradeSystem:  自动交易程序启动后  准备进入定期循环任务！");


	while(1)
		{
//			UpdateFlag();
			bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
			bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
			bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
			bool flag_holiday = dateutil.TodayIsHoliday();

			bool flagfrontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
			bool front_tradelimit = trader.GetFlagTradeLimit(); //网络限流
			bool flagauthenticate_ok = true;
			bool flaglogin_ok = this->GetCTraderSpi()->GetFlagLogin();    //未登录
			bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码


			bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
			bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode &&flag_openmarket; // 网络连接成功,未登录
			bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode&&flag_openmarket; // 网络连接成功,未登录
			bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&!front_tradelimit; //网络连接成功,完成登录
			bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&& !flag_holiday && flag_openmarket&&!front_tradelimit;
			bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&front_tradelimit;

			if(connect_error){
				printf("TrendTradeSystem:   网络链接断开!!!! \n");
				this->Fun_ConnectError();
			}
			else if(ready_login){
				printf("TrendTradeSystem:   网络链接成功,未登录,等待重新login!!!! \n");
				logutil.WriteLog("TrendTradeSystem:  网络链接成功,未登录,等待重新login!!!! ");
				this->Fun_ReadyLogin();
			}
			else if(login_tradelimit){
				printf("TrendTradeSystem:限制流量 请重新登录 ***************************************************************************   [%s] \n",
								dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
				sprintf(logbuf,"TrendTradeSystem:限制流量 请重新登录   [%s] \n",	dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());
				logutil.WriteLog(logbuf);
				this->Fun_TradeLimit();
			}
			else if (market_ok)			{

				printf("TrendTradeSystem:date ***************************************************************************   %s \n",
				dateutil.ConvertSHFETimetoSqlTime(trader.GetDifSec()).c_str());

	//	    printf("pinstrumentID --> %s  %ld   %ld \n",pinstrumentID[0],sizeof(pinstrumentID[0]),sizeof(pinstrumentID));
				this->Fun_Trade_FlagOrderInsertCheck();

				trader.AccountCheck();
				strategy.SetAvalible(trader.GetAvalible());
				strategy.UpdateTradingDay();

				//开仓策略
				strategy.MarketMonitor();
				strategy.SectionMarketView();

				if  (trader.GetTotalHolding()>0)  //如果存在持仓,则启动平仓策略 进行轮询
				{
					//合约到期
					strategy.Close_ExpireMonthClose_SignalCheck();
					trader.ClosebySignal("_expire");

					//浮动止盈止损
					strategy.Close_DynamicStop_SignalCheck();
					trader.ClosebySignal("_dynamicstop");

					// 趋势反转时止损
//					if(strcmp(this->GetStrategyName().c_str(),"TrendTrade")==0){
//						strategy.Hold_MA50Reverse_SignalCheck();
//						trader.ClosebySignal("_ma50reverse");  //新开仓合约检查
//					}

					// 平仓队列撤单追价再平
					strategy.QueueingClosePriceChange_SignalCheck();
					trader.QueueingClosePriceChange_CancelCheck();

				}


				if (strcmp(this->GetStrategyName().c_str(),"TrendTradeCycleN")==0){
					strategy.Open_CycleNSignal();
					trader.TradeOpenCheck("_cyclenopen");  //新开仓合约检查
				}
				else if(strcmp(this->GetStrategyName().c_str(),"TrendTrade")==0){

					strategy.Open_CycleN_NewSignal();
					trader.TradeOpenCheck("_cyclennewopen");  //新开仓合约检查
				}
				else{

				}

//				开仓队列撤单追价开仓监测
				strategy.QueueingOpenPriceChange_SignalCheck();  //未成交委托单队列中 出现跳价信号检测
				trader.QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
			}


			if (flaglogin_ok){
				AutoTask_AfterNoonClosebyUser();
				AutoTask_BeforNightbyUser();

			}
			AutoTask();
			AutoTaskEveryDay();

		}

	return;

}

void TradeSystem::AutoTask_AfterNoonClosebyUser(){

	DateUtil dateutil;

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());

	double start = 0.153000;
	double end = 0.154000;
	double Q_BarTime_2 = this->GetCurrentTime();
	 if (Q_BarTime_2 >=start && Q_BarTime_2 < end  && _TaskFlag16 ==true  && !IsWeekEnd() && IsLoginOK())		{

		 logutil.WriteLog("AutoTask_AfterNoonClosebyUser:  自动交易程序---> 启动每日15:30 定时任务开始！");

		 _TaskFlag16 =false;
		this->GetTrader()->ReqQryTradingAccount();
		sleep(5);
		if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0  ||
				strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
				strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{

//			this->GetTrader()->CheckSqlInstrument();

			//更新日线历史数据
			mysqlutil.CheckAllHttpDayLine(this->GetStrategyName().c_str());
			this->GetTrader()->CheckSqlDepMarket_EndTime();

			// 查询合约信息表  和 行情表，整理出处理合约表
//				 this->GetTrader()->CheckSqlMaincontracont();
//			 this->GetTrader()->CheckSqlMaincontracontbyMaxOpenInterest();
		}

		logutil.WriteLog("AutoTask_AfterNoonClosebyUser:  自动交易程序---> 完成每日15:30 定时任务结束！");
	}

	 if (Q_BarTime_2 < start && _TaskFlag16 ==false && !IsWeekEnd() )	{
	 _TaskFlag16 =true;
	}

}

void TradeSystem::AutoTask_BeforMorningbyUser(){
	DateUtil dateutil;

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());


	double Q_BarTime_2 =  this->GetCurrentTime();


	if (Q_BarTime_2 >= 0.085500 && Q_BarTime_2 < 0.090100 && _TaskFlag ==true  && !IsWeekEnd() && IsLoginOK())	{
			logutil.WriteLog("AutoTask_BeforMorningbyUser:  自动交易程序---> 启动09:01每日定时任务！");

			if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
				//每个新的交易日清空一次内存数据库
				 this->GetTrader()->GetRedisSignal()->FlushDB();
				 this->GetTrader()->GetRedisSection()->FlushDB();
				 this->GetTrader()->GetRedisDepthMarket()->FlushDB();
			}
			else{
				if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
						strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
					//每个新的交易日清空一次内存数据库
					 this->GetTrader()->GetRedisSignal()->FlushDB();
					 this->GetTrader()->GetRedisSection()->FlushDB();
					 this->GetTrader()->GetRedisDepthMarket()->FlushDB();
				}
			}

			_TaskFlag =false;
			//每日定时任务
			 printf("AutoTask_BeforMorningbyUser:date *************************************************************************** %s \n",
					 dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
			 sleep(6);
			 this->GetTrader()->ReqQryTradingAccount();
			 this->GetTrader()->UpdateTradingDay();
			 this->GetTrader()->UpdateTradingDaySqlTime();
			 this->GetTrader()->UpdateDifSec();
			 sleep(5);
			 this->GetTrader()->CheckSettleInfoConfirmbyTrade();

			this->GetStrategy()->UpdateTradingDay();
			this->GetStrategy()->SetDifSec(this->GetTrader()->GetDifSec());

			if(strcmp(this->GetStrategyName().c_str(),"TrendTrade")==0)	{


			}
			else if(strcmp(this->GetStrategyName().c_str(),"TrendTradeCycleN")==0)	{


			}
			else if(strcmp(this->GetStrategyName().c_str(),"DayTrade")==0)	{

				 printf("AutoTask_BeforMorningbyUser:DayTrade ********   历史数据基础操作-----\n");

//				mysqlutil.CheckHttpDayLine();

				mysqlutil.CheckAllHttpDayLine(this->GetStrategyName().c_str());
//				mysqlutil.CheckAllHttpMinuteLine(this->GetStrategyName().c_str());

				//			 mysqlutil.CheckHttpMinuteLine();
				//			 mysqlutil.CheckCycleData();
				//			 mysqlutil.CheckHisMAData();
				//			 mysqlutil.CheckAveragePriceData();
				//			 mysqlutil.CheckTradingDayCycleData();
				//			 mysqlutil.CheckTradingDayMAData();

				// 查询合约信息表  和 行情表，整理出处理合约表
//				 this->GetTrader()->CheckSqlMaincontracont();
				 this->GetTrader()->CheckSqlMaincontracontbyOpenInterest();
				 printf("AutoTask_BeforMorningbyUser:DayTrade ********   历史数据基础操作-----0000\n");

				printf("AutoTask_BeforMorningbyUser:DayTrade ********   历史数据基础操作-----1111111\n");
				logutil.WriteLog("AutoTask_BeforMorningbyUser:  自动交易程序---> 主力合约追踪及盘前自检邮件发送完成！");

			}
			else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{

				//更新持仓表
				 this->GetTrader()->ReqQryInvestorPositionDetail();
				 sleep(1);
				 this->GetTrader()->ReqQryInvestorPosition();
				 sleep(1);
				 this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();

				 sleep(1);
				 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
				 sleep(1);


			}
			else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
					strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{

				printf("AutoTask_BeforMorningbyUser:CalendarSpreadTrade_zlqh ********   历史数据基础操作-----Begin!!! \n");


				mysqlutil.CheckAllHttpDayLine(this->GetStrategyName().c_str());

				printf("AutoTask_BeforMorningbyUser:CalendarSpreadTrade_zlqh ********   历史数据基础操作-----End!!! \n");
				//更新持仓表
				 this->GetTrader()->ReqQryInvestorPositionDetail();
				 sleep(1);
				 this->GetTrader()->ReqQryInvestorPosition();
				 sleep(1);
				 this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();
				 sleep(1);
				 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
				 sleep(1);

			}
			else{
				//更新持仓表
				 this->GetTrader()->ReqQryInvestorPositionDetail();
				 sleep(1);
				 this->GetTrader()->ReqQryInvestorPosition();
				 sleep(1);
				 this->GetTrader()->CheckSqlHoldPositionbyPosDetail();
				 sleep(1);
				 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
				 sleep(1);
			}


		}

	if (Q_BarTime_2 < 0.205500 && _TaskFlag ==false  && !IsWeekEnd() )	{
			_TaskFlag =true;
		}
}

void TradeSystem::AutoTask_BeforNightbyUser(){
	DateUtil dateutil;

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());


	double Q_BarTime_2 =  this->GetCurrentTime();


	if (Q_BarTime_2 >= 0.205500 && Q_BarTime_2 < 0.210100 && _TaskFlag ==true  && !IsWeekEnd() && IsLoginOK())
		{
			logutil.WriteLog("AutoTask_BeforNightbyUser:  自动交易程序---> 启动21:01每日定时任务！");

			if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
				//每个新的交易日清空一次内存数据库
				 this->GetTrader()->GetRedisSignal()->FlushDB();
				 this->GetTrader()->GetRedisSection()->FlushDB();
				 this->GetTrader()->GetRedisDepthMarket()->FlushDB();
			}
			else{
				if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
						strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
					//每个新的交易日清空一次内存数据库
					 this->GetTrader()->GetRedisSignal()->FlushDB();
					 this->GetTrader()->GetRedisSection()->FlushDB();
					 this->GetTrader()->GetRedisDepthMarket()->FlushDB();
				}
			}

			_TaskFlag =false;
			//每日定时任务
			 printf("AutoTask_BeforNightbyUser:date *************************************************************************** %s \n",
					 dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
			 sleep(6);
			 this->GetTrader()->ReqQryTradingAccount();
			 this->GetTrader()->UpdateTradingDay();
			 this->GetTrader()->UpdateTradingDaySqlTime();
			 this->GetTrader()->UpdateDifSec();
			 sleep(5);
			 this->GetTrader()->CheckSettleInfoConfirmbyTrade();

			this->GetStrategy()->UpdateTradingDay();
			this->GetStrategy()->SetDifSec(this->GetTrader()->GetDifSec());

			if(strcmp(this->GetStrategyName().c_str(),"TrendTrade")==0)	{


			}
			else if(strcmp(this->GetStrategyName().c_str(),"TrendTradeCycleN")==0)	{


			}
			else if(strcmp(this->GetStrategyName().c_str(),"DayTrade")==0)	{

				 printf("AutoTask_BeforNightbyUser:DayTrade ********   历史数据基础操作-----\n");



				mysqlutil.CheckAllHttpDayLine(this->GetStrategyName().c_str());


				// 查询合约信息表  和 行情表，整理出处理合约表

				 this->GetTrader()->CheckSqlMaincontracontbyOpenInterest();
				 printf("AutoTask_BeforNightbyUser:DayTrade ********   历史数据基础操作-----0000\n");

				printf("AutoTask_BeforNightbyUser:DayTrade ********   历史数据基础操作-----1111111\n");
				logutil.WriteLog("AutoTask_BeforNightbyUser:  自动交易程序---> 主力合约追踪及盘前自检邮件发送完成！");

			}
			else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{


				//更新持仓表
				 this->GetTrader()->ReqQryInvestorPositionDetail();
				 sleep(1);
				 this->GetTrader()->ReqQryInvestorPosition();
				 sleep(1);
				 this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();

				 sleep(1);
				 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
				 sleep(1);


			}
			else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
					strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{

				printf("AutoTask_BeforNightbyUser:CalendarSpreadTrade_zlqh ********   历史数据基础操作-----Begin!!! \n");


				mysqlutil.CheckAllHttpDayLine(this->GetStrategyName().c_str());


				printf("AutoTask_BeforNightbyUser:CalendarSpreadTrade_zlqh ********   历史数据基础操作-----End!!! \n");
				//更新持仓表
				 this->GetTrader()->ReqQryInvestorPositionDetail();
				 sleep(1);
				 this->GetTrader()->ReqQryInvestorPosition();
				 sleep(1);
				 this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();

				 sleep(1);
				 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
				 sleep(1);

			}
			else{
				//更新持仓表
				 this->GetTrader()->ReqQryInvestorPositionDetail();
				 sleep(1);
				 this->GetTrader()->ReqQryInvestorPosition();
				 sleep(1);
				 this->GetTrader()->CheckSqlHoldPositionbyPosDetail();
				 sleep(1);
				 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
				 sleep(1);
			}


		}

	if (Q_BarTime_2 < 0.205500 && _TaskFlag ==false  && !IsWeekEnd() )	{
			_TaskFlag =true;
		}
}
void TradeSystem::AutoTaskEveryDay(){
	DateUtil dateutil;
	char logbuf[1024];
//	Config config(this->GetStrategyName().c_str());
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	double Q_BarTime_2 =  this->GetCurrentTime();

	bool start0 = Q_BarTime_2 > 0.073000 && Q_BarTime_2 < 0.073500 ;
	bool start1 = Q_BarTime_2 > 0.133000 && Q_BarTime_2 < 0.133500 ;
	bool start2 = Q_BarTime_2 > 0.193000 && Q_BarTime_2 < 0.193500 ;
	bool start3 = Q_BarTime_2 > 0.013000 && Q_BarTime_2 < 0.013500 ;

	 if ((start0 || start1 || start2 || start3 )&& _TaskFlag_EveryDay  )	{
		 sprintf(logbuf,"TradeSystem::AutoTaskEveryDay:  自动交易程序---> 完成每日定时任务！   %s ",
				 dateutil.GetCurrentSqlTimeString().c_str());
		logutil.WriteLog(logbuf);
		this->GetTrader()->GetMysqlAsset()->Ping();
		this->GetTrader()->GetMysqlInvestPosition()->Ping();

		this->GetStrategy()->GetMysqlDepthMarket()->Ping();
		this->GetStrategy()->GetMysqlInstrument()->Ping();
		this->GetStrategy()->GetMysqlMaincontract()->Ping();
		this->_TaskFlag_EveryDay =false;
	}

	 bool s1=Q_BarTime_2 < 0.073000 && Q_BarTime_2 > 0.072500;
	 bool s2=Q_BarTime_2 < 0.133000 && Q_BarTime_2 > 0.132500;
	 bool s3=Q_BarTime_2 < 0.193000 && Q_BarTime_2 > 0.192500;
	 bool s4=Q_BarTime_2 < 0.013000 && Q_BarTime_2 > 0.012500;

	 if ((s1 || s2 || s3 || s4) && !_TaskFlag_EveryDay   )		{
		 this->_TaskFlag_EveryDay =true;
	}
}


