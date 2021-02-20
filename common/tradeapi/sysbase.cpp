#include "sysbase.hpp"


SysBase::SysBase() {
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
SysBase::~SysBase() {
	if (m_pTrader!=nullptr){
		delete m_pTrader;
	}
	if(m_pdatawrapper!=nullptr){
		delete m_pdatawrapper;
	}
}


RedisSignal* SysBase::GetRedisSignal()
{
	return &redissignal;
}


RedisDepthMarket* SysBase::GetRedisDepthMarket()
{

//	printf("GetRedisDepthMarket   ---host    %s \n",this->_predisdepthmarket->GetHost().c_str());
	return &redisdepthmarket;
}



RedisSection * SysBase::GetRedisSection()
{
	return &redissection;
}


void SysBase::SetTrader(DayTrader *ptrader){
	this->m_pTrader=ptrader;

//	m_tdapi=ptrader->GetCThostFtdcTraderApi();
}
DayTrader * SysBase::GetTrader(){
	return this->m_pTrader;
}


void SysBase::SetStrategyName(const char* strategyname){
	this->_strategyName=strategyname;
}

std::string SysBase::GetStrategyName(){
	return this->_strategyName;
}


void SysBase::SetDataWrapper(DataWrapper *point){
	this->m_pdatawrapper=point;
}
DataWrapper* SysBase::GetDataWrapper(){
	return this->m_pdatawrapper;
}

void SysBase::SetAvalible(double ava)
{
	this->_avalible=ava;
}
double  SysBase::GetAvalible()
{
	return this->_avalible;
}
void SysBase::SetUserid(const char* userid)
{
	this->m_userid=userid;
}
std::string SysBase::GetUserid()
{
	return this->m_userid;
}
void SysBase::SetBaseAccount(bool flag)
{
	this->m_baseaccount=flag;
}
bool SysBase::GetBaseAccount()
{
	return this->m_baseaccount;
}
void SysBase::InitialFlag(){

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
double SysBase::GetCurrentTime(){
	DateUtil dateutil;
	double ret = dateutil.GetCurrentBarTime()+ (1/10e5)*this->GetTrader()->GetDifSec();
	return ret;

}

//可登录时间段  比正常交易时间段要广
bool SysBase::IsLoginAvalibleTime(){
	DateUtil dateutil;
	this->_flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
	return this->_flag_loginmarket;
}
//交易时段 所有合约的交易时段合集
bool SysBase::IsTradeAvalibleTime(){
	DateUtil dateutil;
	this->_flag_open =dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
	return this->_flag_open;
}
//是否节假日
bool SysBase::IsHoliday(){
	DateUtil dateutil;
	this->_flag_holiday = dateutil.TodayIsHoliday();
	return this->_flag_holiday;
}
//是否周末
bool SysBase::IsWeekEnd(){
	DateUtil dateutil;
	return dateutil.Check_wday();
}
bool SysBase::IsLoginOK(){

	 return this->GetTrader()->GetCTraderSpi()->GetFlagLogin();
}



void SysBase::CheckAllHttpDayLine()
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
std::string  SysBase::CheckFlowPath(){

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
void SysBase::SetLogProperty(const char* userid){

	std::string system_log_name="system";
	system_log_name.append("_");
	system_log_name.append(userid);

	std::string system_logfilename="system";
	system_logfilename.append("_");
	system_logfilename.append(userid);
	system_logfilename.append(".log");

	this->SetLogName(system_log_name.c_str());
	this->SetLogPathFileName(system_logfilename.c_str());
	LogUtil * plog= new LogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->SetLogUtil(plog);
}

void SysBase::ReadCommissionData()
{
	MysqlCommission mysqlcommission;

//	if(mysqlcommission.Exist_Table()){
//			mysqlcommission.DropTable();
//			printf("ReadCommissionData:  vm correct   ------> 清空数据表  MysqlCommission \n");
//		}

//	if(mysqlcommission.Exist_Table()&& mysqlcommission.Count_ColumnsbyTableName()<9){
//		mysqlcommission.DropTable();
//		printf("ReadCommissionData:Cloumns 不足  ------> 清空数据表  MysqlCommission \n");
//	}
//
//	if(mysqlcommission.Exist_Table()){
//
//		int c=mysqlcommission.Count_AllLists();
//		int n=52;
//		if(c<n){
//			mysqlcommission.DropTable();
//			printf("ReadCommissionData:数据不足%d  < %d   ------> 清空数据表  MysqlCommission \n",c,n);
//		}
//
//	}

	if(!mysqlcommission.Exist_Table()){
		mysqlcommission.CreateTable();
	}

	if(mysqlcommission.Count_AllLists()==0){

		printf("ThreadTrade:ReadCommissionData  不存在佣金表,写入佣金表!!! \n");
		Config config(this->GetStrategyName().c_str());
		std::string path=config.GetConfigPath();
		std::string filename ="commission.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("ThreadTrade:ReadCommissionData ---->File not found!\n");
		}
		else	{
			printf("ThreadTrade:ReadCommissionData --->File found!\n");
		}

		while (CSVRead.CSVReadNextRow())	{

			std::string strProductID;
			std::string strExchangeID;
			std::string strMode;
			std::string strOpenAccount;
			std::string strOpenNumber;
			std::string strCloseAccount;
			std::string strCloseNumber;
			std::string strVolumeMultiple;
			std::string strMargin;

			CSVRead.CSVRead("productid", strProductID);
			CSVRead.CSVRead("exchangeid", strExchangeID);
			CSVRead.CSVRead("mode", strMode);

			CSVRead.CSVRead("open_account", strOpenAccount);
			CSVRead.CSVRead("open_number", strOpenNumber);
			CSVRead.CSVRead("close_account", strCloseAccount);
			CSVRead.CSVRead("close_number", strCloseNumber);
			CSVRead.CSVRead("volumemultiple", strVolumeMultiple);
			CSVRead.CSVRead("margin", strMargin);

			double openaccount = atof(strOpenAccount.c_str());
			double opennumber = atof(strOpenNumber.c_str());
			double closeaccount = atof(strCloseAccount.c_str());
			double closenumber = atof(strCloseNumber.c_str());
			double margin = atof(strMargin.c_str());
			int volumemultiple = atoi(strVolumeMultiple.c_str());

			Commission commdata;
			strcpy(commdata.ProductID,strProductID.c_str());
			strcpy(commdata.ExchangeID,strExchangeID.c_str());
			strcpy(commdata.Mode,strMode.c_str());

			commdata.OpenAccount=openaccount/10000;
			commdata.OpenNumber=opennumber;
			commdata.CloseAccount=closeaccount/10000;
			commdata.CloseNumber=closenumber;
			commdata.Margin = margin;
			commdata.VolumeMultiple=volumemultiple;

			printf("ThreadTrade:ReadCommissionData 原始数据：%s %s %s  %f %.2f %f %.2f  margin:%f volumemultiple:%d\n",strProductID.c_str(),strExchangeID.c_str(),strMode.c_str(),
					openaccount,opennumber,closeaccount,closenumber,margin,volumemultiple);


			if(!mysqlcommission.Exist_DatabyProductID(strProductID.c_str()))	{
				mysqlcommission.Insert(commdata);
				printf("ThreadTrade:ReadCommissionData 插入数据 ----> %s %s %s   %f %.2f %f %.2f %f %d\n",
						commdata.ProductID,commdata.ExchangeID,commdata.Mode,commdata.OpenAccount,commdata.OpenNumber,
						commdata.CloseAccount,commdata.CloseNumber,commdata.Margin,commdata.VolumeMultiple);
			}

		}

	}
	else	{
		printf("ThreadTrade:ReadCommissionData  存在佣金表\n");
	}

}
//section table 用于scetion view
void SysBase::InitialSectionTable()
{
	MysqlSection mysqlsection;

//	if(mysqlsection.Exist_Table()){
//		mysqlsection.DropTable();
//		printf("InitialSectionTable:清空数据表  MysqlSection \n");
//	}

	if(!mysqlsection.Exist_Table()){
		mysqlsection.CreateTable();
	}

	if(mysqlsection.Count_AllLists()==0){

		printf("ThreadTrade::InitialSectionTable  不存在评估,写入初始评估表!!! \n");
		Config config(this->GetStrategyName().c_str());
		std::string path=config.GetConfigPath();
		std::string filename ="assess.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("ThreadTrade::InitialSectionTable ---->File not found!\n");
		}
		else	{
			printf("ThreadTrade::InitialSectionTable --->File found!\n");
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
			printf("ThreadTrade::InitialSectionTable 原始数据：%s %d %d %d %d  \n",strSection.c_str(),totalpruductnumber,
					threshholdnumber,productrisknumber,active);

			if(!mysqlsection.Exist_DataBySection(strSection.c_str()))	{
				mysqlsection.Insert(data);
				printf("ThreadTrade::InitialSectionTable 插入数据 ----> %s %d %d %d %d\n",data.Section,
						data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber,data.Active);
			}
		}

	}
	else	{
		printf("ThreadTrade::InitialSectionTable  存在Section Table !!! \n");
	}

}
// secassess 用于对版块的开仓仓位进行控制
void SysBase::InitialSecAssessTable(const char* userid)
{
	MysqlSecAssess mysqlsecassess(userid);
//	if(mysqlsecassess.Exist_Table() && mysqlsecassess.Count_ColumnsbyTableName()<14){
//		mysqlsecassess.DropTable();
//		printf("	InitialSecAssessTable:清空数据表  MysqlSecAssess !!!\n");
//	}

	if(!mysqlsecassess.Exist_Table()){
		mysqlsecassess.CreateTable();
	}

	if(mysqlsecassess.Count_AllLists()==0){

		printf("ThreadTrade:InitialAssessTable  不存在评估,写入初始评估表!!! \n");
		Config config(this->GetStrategyName().c_str());
		std::string path=config.GetConfigPath();
		std::string filename ="assess.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("ThreadTrade:InitialAssessTable ---->File not found!\n");
		}
		else	{
//			printf("ThreadTrade:InitialAssessTable --->File found!\n");
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
//			printf("RealTradeTest:InitialAssessTable 原始数据：%s %d %d %d  %d \n",strSection.c_str(),totalpruductnumber,
//					threshholdnumber,productrisknumber,priority);

			if(!mysqlsecassess.Exist_DataBySection(strSection.c_str()))	{
				mysqlsecassess.Insert(data);
//				printf("RealTradeTest:InitialAssessTable 插入数据 ----> %s %d %d %d %d\n",data.Section,
//						data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber,data.Priority);
			}
		}

	}
	else	{
//		printf("RealTradeTest:InitialAssessTable  存在开仓评估表!!! \n");
	}

}
void SysBase::UpdateHoldlist()
{
	this->GetTrader()->ReqQryTradingAccount();
	sleep(1);
	 this->GetTrader()->ReqQryInvestorPositionDetail();
	 sleep(1);
	 this->GetTrader()->ReqQryInvestorPosition();
	 this->GetTrader()->SyncHoldInvestMap();
//	 this->GetTrader()->DayTrade_CheckSqlHoldPositionbyPosDetail();
//	 sleep(1);
//	 this->GetTrader()->CheckPosDetailbySqlHoldPosition();
//	 sleep(1);
}
void SysBase::AutoTask_AfterNoonClosebyUser(){

	DateUtil dateutil;


	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());

	double start = 0.153000;
	double end = 0.154000;
	double Q_BarTime_2 = this->GetCurrentTime();
	 if (Q_BarTime_2 >=start && Q_BarTime_2 < end  && _TaskFlag16 ==true  && !IsWeekEnd() && IsLoginOK())		{

		 this->GetLogUtil()->WriteLog("AutoTask_AfterNoonClosebyUser:  自动交易程序---> 启动每日15:30 定时任务开始！");

		 _TaskFlag16 =false;
		this->GetTrader()->ReqQryTradingAccount();
		sleep(5);
		if(this->GetBaseAccount())	{
			this->GetTrader()->CheckSqlDepMarket_EndTime();
		}

		this->GetLogUtil()->WriteLog("AutoTask_AfterNoonClosebyUser:  自动交易程序---> 完成每日15:30 定时任务结束！");
	}

	 if (Q_BarTime_2 < start && _TaskFlag16 ==false && !IsWeekEnd() )	{
	 _TaskFlag16 =true;
	}

}

void SysBase::AutoTask_BeforMorningbyUser(){
	DateUtil dateutil;
	printf("AutoTask_BeforMorningbyUser 111111\n");
	double Q_BarTime_2 =  this->GetCurrentTime();

//	const double starttime = 0.085500;
	const double starttime = 0.000100;
//	if (Q_BarTime_2 >= starttime && Q_BarTime_2 < 0.090100 && m_taskflag_morning ==true  && !IsWeekEnd() && IsLoginOK())
	if (Q_BarTime_2 >= starttime && Q_BarTime_2 < 0.090100 && m_task ==0  && !IsWeekEnd() && IsLoginOK())
	{
		this->GetLogUtil()->WriteLog("AutoTask_BeforMorningbyUser:  自动交易程序---> 启动08:55 每日定时任务！");
		printf("AutoTask_BeforMorningbyUser 22222222\n");


		m_taskflag_morning =false;
		m_task++;
		//每日定时任务
		 printf("AutoTask_BeforMorningbyUser:date*************** %s \n",
				 dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
		 sleep(1);
		 this->GetTrader()->ReqQryTradingAccount();
		 this->GetTrader()->UpdateTradingDay();
		 this->GetTrader()->UpdateTradingDaySqlTime();
		 this->GetTrader()->UpdateDifSec();
		 sleep(1);
		 this->GetTrader()->CheckSettleInfoConfirmbyTrade();

		this->GetDataWrapper()->SetTradingDay(this->GetTrader()->GetTradingDay().c_str());
		this->GetDataWrapper()->SetDifSec(this->GetTrader()->GetDifSec());

		if(this->GetBaseAccount())	{
			this->GetLogUtil()->WriteLog("AutoTask_BeforNightbyUser:  自动交易程序---> 主力合约追踪及盘前自检邮件发送完成！");
		}

		sleep(1);
		this->GetTrader()->ReqQryInvestorPositionDetail();
		sleep(1);
		this->GetTrader()->ReqQryInvestorPosition();
		this->GetTrader()->SyncHoldInvestMap();
	}


	if (Q_BarTime_2 < starttime && m_taskflag_morning ==false  && !IsWeekEnd() )	{
		m_taskflag_morning =true;
		printf("AutoTask_BeforMorningbyUser 3333333333\n");
	}

	printf("AutoTask_BeforMorningbyUser <<<<<<<<<<\n");
}

void SysBase::AutoTask_BeforNightbyUser(){
	DateUtil dateutil;

	MysqlUtil  mysqlutil(this->GetStrategyName().c_str());
	double Q_BarTime_2 =  this->GetCurrentTime();

	if (Q_BarTime_2 >= 0.205500 && Q_BarTime_2 < 0.235900 && _TaskFlag ==true  && !IsWeekEnd() && IsLoginOK())
		{
			this->GetLogUtil()->WriteLog("AutoTask_BeforNightbyUser:  自动交易程序---> 启动21:01每日定时任务！");

			if(this->GetBaseAccount())	{
				//每个新的交易日清空一次内存数据库
				 this->GetTrader()->GetRedisSignal()->FlushDB();
				 this->GetTrader()->GetRedisSection()->FlushDB();
				 this->GetTrader()->GetRedisDepthMarket()->FlushDB();
			}

			_TaskFlag =false;
			//每日定时任务
			 printf("AutoTask_BeforNightbyUser:date *************************************************************************** %s \n",
					 dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
			 sleep(1);
			 this->GetTrader()->ReqQryTradingAccount();
			 this->GetTrader()->UpdateTradingDay();
			 this->GetTrader()->UpdateTradingDaySqlTime();
			 this->GetTrader()->UpdateDifSec();
			 sleep(1);
			 this->GetTrader()->CheckSettleInfoConfirmbyTrade();

			 this->GetDataWrapper()->SetTradingDay(this->GetTrader()->GetTradingDay().c_str());
			this->GetDataWrapper()->SetDifSec(this->GetTrader()->GetDifSec());

			if(this->GetBaseAccount())	{
				printf("AutoTask_BeforNightbyUser: ********   历史数据基础操作-----Begin!!! \n");

				 this->GetTrader()->CheckSqlMaincontracontbyOpenInterest();

				this->GetLogUtil()->WriteLog("AutoTask_BeforNightbyUser:  自动交易程序---> 主力合约追踪及盘前自检邮件发送完成！");
			}
			UpdateHoldlist();//更新持仓表
		}

	if (Q_BarTime_2 < 0.205500 && _TaskFlag ==false  && !IsWeekEnd() )	{
			_TaskFlag =true;
		}
}
void SysBase::AutoTaskEveryDay(){
	DateUtil dateutil;
	char logbuf[1024];
//	Config config(this->GetStrategyName().c_str());


	double Q_BarTime_2 =  this->GetCurrentTime();

	bool start0 = Q_BarTime_2 > 0.073000 && Q_BarTime_2 < 0.073500 ;
	bool start1 = Q_BarTime_2 > 0.133000 && Q_BarTime_2 < 0.133500 ;
	bool start2 = Q_BarTime_2 > 0.193000 && Q_BarTime_2 < 0.193500 ;
	bool start3 = Q_BarTime_2 > 0.013000 && Q_BarTime_2 < 0.013500 ;

	 if ((start0 || start1 || start2 || start3 )&& _TaskFlag_EveryDay  )	{
		 sprintf(logbuf,"SysBase::AutoTaskEveryDay:  自动交易程序---> 完成每日定时任务！   %s ",
				 dateutil.GetCurrentSqlTimeString().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
		this->GetTrader()->GetMysqlAsset()->Ping();
		this->GetTrader()->GetMysqlInvestPosition()->Ping();

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


