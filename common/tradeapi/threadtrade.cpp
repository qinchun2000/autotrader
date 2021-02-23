#include "threadtrade.hpp"


ThreadTrade::ThreadTrade(const char* strategyname) {

	this->SetStrategyName(strategyname);
	Config config(this->GetStrategyName().c_str());
	this->SetUserid(config.GetUserID().c_str());
	this->SetLogProperty(config.GetUserID().c_str());
}

void ThreadTrade::DropSqlTable(){
	printf("ThreadTrade:开始启动程序  --->>>>DropSqlTable \n");
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

	MysqlSecAssess mysqlsecassess_d(this->GetUserid().c_str());
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

	//	MysqlOrder mysqlopen(config.GetTable_Open().c_str(),this->GetUserid().c_str());
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
	//	MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetUserid().c_str());
	//	if(mysqlcloseorder.Exist_Table()){
	//		mysqlcloseorder.DropTable();
	//
	//		printf("InitialSqlTable:drop数据表  MysqlOrder ---> close table!!!  \n");
	//	}else{
	//		mysqlcloseorder.CreateTable();
	//		printf("InitialSqlTable:新建数据表  MysqlOrder ---> close table!!!  \n");
	//	}

	//MysqlAsset mysqlasset_d(this->GetUserid().c_str());
	//	if(mysqlasset_d.Exist_Table()){
	//		mysqlasset_d.DropTable();
	//
	//		printf("InitialSqlTable:drop数据表  MysqlAsset \n");
	//	}
	MysqlTradingDay mysqltradingday(this->GetUserid().c_str());
	if(mysqltradingday.Exist_Table() && mysqltradingday.Count_ColumnsbyTableName()<17){
		printf("InitialSqlTable:drop数据表  MysqlTradingDay \n");
		mysqltradingday.DropTable();
	}

	//		MysqlTradingDay mysqltradingday_d(this->GetUserid().c_str());
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
void ThreadTrade::InitialSqlTable(){

	printf("ThreadTrade:开始启动程序  --->>>>InitialSqlTable \n");
	Config config(this->GetStrategyName().c_str());

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
//	MysqlGlobalView mysqlglobalview(this->GetUserid().c_str());
//	if(mysqlglobalview.Exist_Table()){
//		printf("	DropSqlTable:drop数据表  mysqlglobalview !!!\n");
//		mysqlglobalview.DropTable();
//	}
//
//	if(!mysqlglobalview.Exist_Table()){
//		mysqlglobalview.CreateTable();
//	}




///////////////////////////////////////////////////////////////////////////


	if(this->GetBaseAccount())	{
		ReadCommissionData();
		InitialSectionTable();

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
	}


	MysqlTradingDay mysqltradingday(this->GetUserid().c_str());
	if(!mysqltradingday.Exist_Table()){
		mysqltradingday.CreateTable();

		printf("	InitialSqlTable:创建数据表  MysqlTradingDay \n");
	}





	InitialSecAssessTable(this->GetUserid().c_str());
}

void ThreadTrade::MdConnect(){
		CodeUtil codeutil;
		std::string flowpath=codeutil.GetCurrentDirectory();
		flowpath.append("/mdflow/");

		Market market(this->GetDataWrapper(),this->GetStrategyName().c_str(),flowpath.c_str());
		market.AutoConnect();
}



void ThreadTrade::LocalLogin(){

//	printf("LocalLogin:CTP 初始化设置begin !!!    \n");
	this->GetTrader()->CTP_AutoInitial();
//	printf("LocalLogin:CTP 初始化设置完成!!!    \n");

	this->GetTrader()->FrontConnect();
//	printf("LocalLogin:CTP FrontConnect完成!!!    \n");

	this->GetTrader()->ReqAuthenticate();
	this->GetTrader()->FirstLogin();
//	printf("LocalLogin:CTP FirstLogin完成!!!    \n");
	DateUtil dateutil;
	printf("LocalLogin:开始启动程序  登录成功--->>>>TradingDay  \n");
	this->GetTrader()->UpdateTradingDay();
	this->GetTrader()->UpdateTradingDaySqlTime();

	this->GetTrader()->UpdateDifSec();
	this->GetTrader()->SetDifSec(this->GetTrader()->GetDifSec());

//	this->GetSecAssess()->SetTradingDay(this->GetTrader()->GetCThostFtdcTraderApi()->GetTradingDay());


	if(this->GetTrader()->GetCTraderSpi()->GetFlagLogin())	{
		printf("LocalLogin:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime :%s\n", this->GetTrader()->GetCTraderSpi()->GetTest().c_str(),
				this->GetTrader()->GetTradingDay().c_str(),this->GetTrader()->GetTradingDaySqlTime().c_str());
		//初始化逻辑判断信号
		InitialFlag();
		dateutil.CheckWaitSecondsbySHFELoginMarketTime(this->GetTrader()->GetTradingDay().c_str());

	}
}
void ThreadTrade::LocalLogin_MD(){

	this->GetTrader()->CTP_AutoInitial_MD();
	printf("LocalLogin:CTP 初始化设置完成!!!    \n");

	this->GetTrader()->FrontConnect();
	printf("LocalLogin:CTP FrontConnect完成!!!    \n");

	this->GetTrader()->ReqAuthenticate();
	this->GetTrader()->FirstLogin();
	printf("LocalLogin:CTP FirstLogin完成!!!    \n");
	DateUtil dateutil;
	printf("LocalLogin:开始启动程序  登录成功--->>>>TradingDay  \n");
	this->GetTrader()->UpdateTradingDay();
	this->GetTrader()->UpdateTradingDaySqlTime();

	this->GetTrader()->UpdateDifSec();
	this->GetTrader()->SetDifSec(this->GetTrader()->GetDifSec());

//	this->GetSecAssess()->SetTradingDay(this->GetTrader()->GetCThostFtdcTraderApi()->GetTradingDay());


	if(this->GetTrader()->GetCTraderSpi()->GetFlagLogin())	{
		printf("LocalLogin:登录成功,spi----> %s  TradingDay : %s TradingDaySqlTime :%s\n", this->GetTrader()->GetCTraderSpi()->GetTest().c_str(),
				this->GetTrader()->GetTradingDay().c_str(),this->GetTrader()->GetTradingDaySqlTime().c_str());
		//初始化逻辑判断信号
		InitialFlag();
		dateutil.CheckWaitSecondsbySHFELoginMarketTime(this->GetTrader()->GetTradingDay().c_str());

	}
}
void ThreadTrade::UpdateFlag(){
	DateUtil dateutil;
	bool _flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
	bool _flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());
	bool _flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());
	bool _flag_holiday = dateutil.TodayIsHoliday();
}



bool ThreadTrade::ConnectRestore(){
	bool frontconnect_ok = this->GetTrader()->GetCTraderSpi()->GetFlagFrontConnectFalg();
	bool frontdisconnect_was =! this->GetTrader()->GetCTraderSpi()->GetFlagFrontDisConnect() ;
	bool login_not = ! this->GetTrader()->GetCTraderSpi()->GetFlagLogin();
	bool holiday_not =! this->IsHoliday();
	bool ret = IsLoginAvalibleTime() && frontconnect_ok && frontdisconnect_was && login_not && holiday_not;
	 return ret;
}
bool ThreadTrade::OpenMarketLoginOK(){
//	printf("ThreadTrade::OpenMarketLoginOK  --> begin \n");
	bool frontconnect_ok = this->GetTrader()->GetCTraderSpi()->GetFlagFrontConnectFalg();
//	printf("ThreadTrade::OpenMarketLoginOK  --> frontconnect_ok  --->%d  \n",frontconnect_ok);
	bool frontdisconnect_was =! this->GetTrader()->GetCTraderSpi()->GetFlagFrontDisConnect() ;
//	printf("ThreadTrade::OpenMarketLoginOK  --> frontdisconnect_was  --->%d  \n",frontdisconnect_was);
	bool login_ok = this->GetTrader()->GetCTraderSpi()->GetFlagLogin();
//	printf("ThreadTrade::OpenMarketLoginOK  --> login_ok  --->%d  \n",login_ok);
	bool holiday_not =! this->IsHoliday();
//	printf("ThreadTrade::OpenMarketLoginOK  --> holiday_not  --->%d  \n",holiday_not);
//	printf("ThreadTrade::OpenMarketLoginOK  --> IsTradeAvalibleTime  --->%d  \n",IsTradeAvalibleTime());
	bool ret = IsTradeAvalibleTime() && frontconnect_ok && frontdisconnect_was && login_ok && holiday_not;

//	printf("ThreadTrade::OpenMarketLoginOK  --> end  --->%d  \n",ret);
	 return ret;
}


void ThreadTrade::TradeInitial_noSta(){
	char logbuf[1024];
	DateUtil dateutil;
	printf("TradeInitial_noSta:   ------>Begin \n");

	// 初始化TradeApi
	if(this->GetBaseAccount())	{
		this->GetTrader()->InitialTradeApi();   //包含主力合约更新
//		MysqlUtil  mysqlutil(this->GetStrategyName().c_str());
//		mysqlutil.CheckAllHttpDayLine(this->GetStrategyName().c_str());
//		printf("TradeInitial_noSta:CheckAllHttpDayLine   --->>>>%s\n",dateutil.GetCurrentSqlTimeString().c_str());
	}
	else{
		this->GetTrader()->InitialTradeApibyUser();
	}

	double avalible =this->GetTrader()->GetInitialMoney();
	this->SetAvalible(avalible);

	printf("TradeInitial_noSta   ------>End \n");
	this->GetLogUtil()->WriteLog("TradeInitial_noSta:  自动交易程序启动后 初始工作完成！");
}


void ThreadTrade::TradeInitial_MD(){
	char logbuf[1024];

	DateUtil dateutil;
	printf("TradeInitial_MD:   ------>Begin \n");
	// 初始化TradeApi
	this->GetTrader()->InitialTradeApi_MD();   //包含主力合约更新
	printf("TradeInitial_MD   ------>End \n");
	this->GetLogUtil()->WriteLog("TradeInitial_noSta:  自动交易程序启动后 初始工作完成！");
}
void ThreadTrade::Fun_ConnectError(){
	char logbuf[1024];

	DateUtil dateutil;
	int waitseconds = dateutil.CheckWaitSecondsbyOpenTime();
	if(waitseconds>=1800){
		printf("Trade:  网络链接断开!!!! ---->等待[1800]秒后重新链接！\n");
		sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[1800]秒后重新链接！  %s",dateutil.GetCurrentSqlTimeString().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
		sleep(1800);
	}
	else if(waitseconds>0){
		printf("Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
		sleep(waitseconds);
	}
	else{
		waitseconds=10;
		printf("Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		sprintf(logbuf,"Trade:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
		sleep(waitseconds);
	}
}
void ThreadTrade::Fun_ReadyLogin(){
	char logbuf[1024];

	/* 重新登录后，重置请求ID*/
	this->GetTrader()->SetRequestID(0);
	this->GetTrader()->UserLogin();

	if (this->GetTrader()->GetCTraderSpi()->GetFlagLogin()){

		this->GetLogUtil()->WriteLog("Trade:  断开后重login ----------完成！");
		this->GetTrader()->UpdateTradingDay();
		this->GetTrader()->UpdateTradingDaySqlTime();
		this->GetTrader()->UpdateDifSec();

//		this->GetSecAssess()->SetTradingDay(this->GetTrader()->GetTradingDay().c_str());

		sprintf(logbuf,"Trade:  断开后重login  ----------登录完成！  TradingDay : %s TradingDaySqlTime :%s ",
				this->GetTrader()->GetTradingDay().c_str(),this->GetTrader()->GetTradingDaySqlTime().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
		sleep(5);
//					trader.ReqQryTradingAccount();
//					this->GetLogUtil()->WriteLog("Run_CalendarSpreadTrade:  断开后重login成功  ----------ReqQryTradingAccount 账户检测完成！");

	}
	else{
		this->GetLogUtil()->WriteLog("Trade:  断开后重login --------尝试登录失败!!! ");
		sleep(3);
	}
}
void ThreadTrade::Fun_TradeLimit(){
	DateUtil dateutil;
	char logbuf[1024];

	sprintf(logbuf,"Trade:限制流量 请重新登录   [%s] \n",	dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
	this->GetLogUtil()->WriteLog(logbuf);
	this->GetTrader()->UserLogout();

	if (!this->GetTrader()->GetCTraderSpi()->GetFlagLogin()){
		this->GetTrader()->SetFlagTradeLimit(false);
		sprintf(logbuf,"Trade:限制流量 登出完成!!!   [%s] \n",
				dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
		this->GetLogUtil()->WriteLog(logbuf);
	}
	sleep(5);
}
void ThreadTrade::Fun_Trade_FlagOrderInsertCheck(){
	char logbuf[1024];

	if (this->GetTrader()->GetCTraderSpi()->GetFlagOrderInsert()||this->GetTrader()->GetCTraderSpi()->GetFlagCloseOrderInsert())	{

		this->GetLogUtil()->WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托报单成交后,更新持仓列表开始！");
		//查询后台
//		this->GetTrader()->ReqQryTrade();
		sleep(1);
		this->GetTrader()->ReqQryTradingAccount();
		sleep(1);
		this->GetTrader()->ReqQryInvestorPositionDetail();
		//detial 在前,因为需要detail的数据 进行校验
		sleep(1);
		this->GetTrader()->ReqQryInvestorPosition();
		//更新本地数据
		this->GetTrader()->UpdateInvestPosition();
		this->GetTrader()->SyncHoldInvestMap();

		if(this->GetTrader()->GetCTraderSpi()->GetFlagOrderInsert()){
			this->GetTrader()->GetCTraderSpi()->SetFlagOrderInsert(false);
		}
		else if(this->GetTrader()->GetCTraderSpi()->GetFlagCloseOrderInsert()){
			this->GetTrader()->GetCTraderSpi()->SetFlagCloseOrderInsert(false);
		}

		this->GetLogUtil()->WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托报单成交后,更新持仓列表结束！");
	}

	if(this->GetTrader()->GetFlagInvestUpdate()){
		sleep(1);
		this->GetTrader()->ReqQryInvestorPositionDetail();
		//detial 在前,因为需要detail的数据 进行校验
		sleep(1);
		this->GetTrader()->ReqQryInvestorPosition();
		//更新本地数据
		this->GetTrader()->UpdateInvestPosition();
		this->GetTrader()->SyncHoldInvestMap();

		this->GetTrader()->SetFlagInvestUpdate(false);
		this->GetLogUtil()->WriteLog("Fun_Trade_FlagOrderInsertCheck:  自动交易程序---> 委托报单成交后,GetFlagInvestUpdate 更新持仓列表！");
	}


}


void ThreadTrade::Fun_Trade_HoldCheck_noSta(){
	char logbuf[1024];

	printf("\n");

	int holdcount=this->GetDataWrapper()->GetHoldingPosition();
	 std::cout<<"ThreadTrade::------->总持仓合约品种:"<<holdcount<<std::endl;
	if  (holdcount>0)  	{//如果存在持仓,则启动平仓策略 进行轮询

		//尾盘平仓策略
		this->GetTrader()->CloseSignalCheck("daytrade_endtime");
		//浮动止盈止损
		this->GetTrader()->CloseSignalCheck("dynamicstop");
//		if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")!=0 )	{
//			//板块趋势反向 平仓
//			this->GetTrader()->CloseSignalCheck("_sectiontrendstop");
//		}
		//涨跌停平仓
		this->GetTrader()->CloseSignalCheck("limitwin");
//		//多头持仓 资金流出风控平仓
//		this->GetTrader()->CloseSignalCheck("_longsed_out");
	//	//浮盈回撤止损
	//	this->GetTrader()->CloseSignalCheck("_daytrade_profitclose");
		//止盈价平仓
		this->GetTrader()->CloseSignalCheck("daytrade_winclose");
		//成交量活跃度逆转
	//	this->GetTrader()->CloseSignalCheck("_daytrade_volumereverse");

		//平仓如果存在跳价,则进行撤单操作
		this->GetTrader()->QueueingClosePriceChange_CancelCheck();
	}


}
void ThreadTrade::Fun_Trade_OpenCheck_noSta(){

	DateUtil dateutil;
	if (dateutil.CheckSHFEEndCloseMarketTime(this->GetTrader()->GetDifSec())){
		return;
	}

	if(strcmp(this->GetStrategyName().c_str(),"statest")==0 ){
			this->GetTrader()->TradeOpen_Test(this->GetStrategyName().c_str());
	}
	else if(strcmp(this->GetStrategyName().c_str(),"bigvolume_zlqh")==0 ){
//		this->GetTrader()->OpenSignalCheck(this->GetStrategyName().c_str());
		}
	else{
		this->GetTrader()->OpenSignalCheck(this->GetStrategyName().c_str());
	}

	// 开仓如果存在跳价  则进行撤单操作
	this->GetTrader()->QueueingPriceChange_CancelCheck();      //跳价信号 撤单监测
}

void ThreadTrade::Run(){

	this->SetBaseAccount(true);
	DateUtil dateutil;
	char logbuf[1024];

#ifdef DEBUG
	printf("Trade:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
#endif
	sprintf(logbuf,"Trade:-----------------开始自动交易程序!!!!!!   %s" ,dateutil.GetCurrentSqlTimeString().c_str());
	this->GetLogUtil()->WriteLog(logbuf);

	if(this->GetBaseAccount())	{

	//	redisdepthmarket.FlushDB();
		this->GetRedisSignal()->FlushDB();
		this->GetRedisSection()->FlushDB();

		printf("Trade:开始启动程序  --->>>>redis 清空 ok \n");
	}

	//初始化Mysql数据库table
//	DropSqlTable();
//	//临时代码
//	MysqlHold mysqlhold(this->GetUserid().c_str());
//	if(mysqlhold.Exist_Table()){
//		mysqlhold.DropTable();
//		printf("---------------->drop数据表  MysqlHold ---> table!!!  \n");
//	}
//
//	MysqlPosDetail mysqlposdetail(this->GetUserid().c_str());
//	if(mysqlposdetail.Exist_Table()){
//		mysqlposdetail.DropTable();
//		printf("---------------->drop数据表  MysqlPosDetail ---> table!!!  \n");
//	}
//
//	MysqlInstrument mysqlins;
//	if(mysqlins.Exist_Table()  ){
//		mysqlins.DropTable();
//		printf("---------------->drop数据表  MysqlInstrument ---> table!!!  \n");
//	}

//	MysqlMaincontract mysqlmaincontract;
//	if(mysqlmaincontract.Exist_Table() && this->GetBaseAccount() ){
////		mysqlmaincontract.DropTable();
//		printf("---------------->drop数据表  MysqlMaincontract ---> table!!!  \n");
//	}
//
//	MysqlSecAssess mysqlsecassess_d(this->GetUserid().c_str());
//	if(mysqlsecassess_d.Exist_Table() ){
//		mysqlsecassess_d.DropTable();
//		printf("	DropSqlTable:清空数据表  MysqlSecAssess !!!\n");
//	}
//
//		printf("	DropSqlTable:drop数据表  111 !!!\n");
	MysqlInvestPosition mysqlinvestposition(this->GetUserid().c_str());
//		printf("	DropSqlTable:drop数据表  222 !!!\n");
	if(mysqlinvestposition.Exist_Table()){
		printf("	DropSqlTable:drop数据表  mysqlinvestposition !!!\n");
#ifdef DEBUG
		Msg_Debug("	DropSqlTable:drop数据表  mysqlinvestposition !!!\n");
#endif
		mysqlinvestposition.DropTable();
	}

	if(!mysqlinvestposition.Exist_Table()){
		mysqlinvestposition.CreateTable();
	}


//	printf("Trade:开始启动程序  --->>>>DropSqlTable ok \n");
	InitialSqlTable();
#ifdef DEBUG
	printf("Trade:开始启动程序  --->>>>InitialSqlTable ok \n");
#endif

	MysqlAsset mysqlasset(this->GetUserid().c_str());
#ifdef DEBUG
	printf("Trade:开始启动程序  --->>>> mysqlasset init fin! \n");
#endif
	DataWrapper datawrapper;
	this->SetDataWrapper(&datawrapper);
	datawrapper.SetUserID(this->GetUserid().c_str());
	datawrapper.InitCommisionMap();


#ifdef DEBUG
	printf("Trade:开始启动程序  --->>>> datawrapper init fin! \n");
#endif
	DayTrader *ptrader = new DayTrader(&datawrapper,this->GetStrategyName().c_str());
#ifdef DEBUG
	printf("Trade:开始启动程序  --->>>>day trader begin ! \n");
#endif
	this->SetTrader(ptrader);
	this->GetTrader()->SetMysqlAsset(&mysqlasset);
	this->GetTrader()->SetMysqlInvestPosition(&mysqlinvestposition);
	this->GetTrader()->SetRedisDepthMarket(this->GetRedisDepthMarket());
	this->GetTrader()->SetRedisSignal(this->GetRedisSignal());
	this->GetTrader()->SetRedisSection(this->GetRedisSection());


	StrategyBase *pstrategy;
	if(strcmp(this->GetStrategyName().c_str(),"turtle")==0 ){
		pstrategy= new Turtle (&datawrapper,this->GetStrategyName().c_str());
	}
	else{

#ifdef DEBUG
		Msg_Debug("Trade:开始启动程序  --->>>>未指定策略!!!! 请指定策略!!! \n");
#endif
		exit(1);
	}

#ifdef DEBUG
	Msg_Info("Trade:开始启动程序  --->>>>LocalLogin--->>>>ApiVersion %s \n",this->GetTrader()->GetCThostFtdcTraderApi()->GetApiVersion());
#endif

	this->LocalLogin();
	this->TradeInitial_noSta();

	//完成主力合约计算后,contract lists 写入map
	datawrapper.start();

	pstrategy->start();
	printf("Trade:ready while ---------------------------------------------------------------->  \n");
	this->GetLogUtil()->WriteLog("Trade:  自动交易程序启动后  准备进入定期循环任务！");


	while(1)		{
		sleep(3);

		printf("ttttttttttttttttt ---> 111111111 \n");
			bool flag_loginmarket = dateutil.CheckSHFELoginMarketTime(this->GetTrader()->GetDifSec());
			bool flag_open = dateutil.CheckCloseAll(this->GetTrader()->GetDifSec());  //09:00 开始
			bool flag_openmarket = dateutil.CheckSHFEOpenMarketTime(this->GetTrader()->GetDifSec());  //08:50 开始
			bool flag_holiday = dateutil.TodayIsHoliday();

			bool flagfrontconnect_ok = this->GetTrader()->GetCTraderSpi()->GetFlagFrontConnectFalg();  //网络连接成功
			bool front_tradelimit = this->GetTrader()->GetFlagTradeLimit(); //网络限流
			printf("ttttttttttttttttt ---> 111111111 aaa\n");
			bool flag_predatastatus =datawrapper.GetPredataStatus();
			bool flagauthenticate_ok = this->GetTrader()->GetCTraderSpi()->GetFlagAuthenticate();  //终端授权成功

			bool flaglogin_ok = this->GetTrader()->GetCTraderSpi()->GetFlagLogin();    //登录成功
			bool noerrorcode = this->GetTrader()->GetCTraderSpi()->GetConnectErrorCode()==0;    //网络连接无错误码

			bool connect_error= !flagfrontconnect_ok &&!flaglogin_ok &&!noerrorcode;   //网络连接断开
			bool ready_authenticate = flagfrontconnect_ok && !flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode &&flag_openmarket; // 网络连接成功,未认证,未登录
			bool ready_login = flagfrontconnect_ok && flagauthenticate_ok &&!flaglogin_ok
					&& noerrorcode&&flag_openmarket; // 网络连接成功,认证成功,未登录
			bool login_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&!front_tradelimit; //网络连接成功,完成登录
//			bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
//					&& !flag_holiday && flag_openmarket&&!front_tradelimit;//可以进行交易

			bool market_ok = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&& !flag_holiday &&!front_tradelimit && flag_predatastatus;//可以进行交易

			bool login_tradelimit = flagfrontconnect_ok && flagauthenticate_ok &&flaglogin_ok && noerrorcode
					&&flag_openmarket &&front_tradelimit; //登录成功,交易限制
			printf("ttttttttttttttttt ---> 22222222222222 \n");
			if(connect_error){
				printf("Trade:   网络链接断开!!!! \n");
				this->Fun_ConnectError();
			}
			else if(ready_login){
				printf("Trade:   网络链接成功,未登录,等待重新login!!!! \n");
				this->GetLogUtil()->WriteLog("Trade:  网络链接成功,未登录,等待重新login!!!! ");
				this->Fun_ReadyLogin();
			}
			else if(login_tradelimit){
				printf("Trade:限制流量 请重新登录 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   [%s] \n",
								dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());
				this->Fun_TradeLimit();
			}
			else if (market_ok)			{
//				else if (1)			{
				printf("\nTrade[%s]:date ***************************************************************************   [%s] \n",
						this->GetUserid().c_str(),dateutil.ConvertSHFETimetoSqlTime(this->GetTrader()->GetDifSec()).c_str());

				// 有开仓合约后,需要更新持仓表 用于浮动止损 止盈
				if(flag_open && flaglogin_ok){
					this->Fun_Trade_FlagOrderInsertCheck();
				}

				this->GetTrader()->UpdateInvestPosition();
				this->GetTrader()->SyncHoldInvestMap();
				//
//				//持仓策略
				this->Fun_Trade_HoldCheck_noSta();
//				printf("\n");
//				//开仓监测
				this->Fun_Trade_OpenCheck_noSta();

				usleep(500000); //500ms
			}
			printf("ttttttttttttttttt ---> 333333333 \n");
			if (flaglogin_ok){
				this->AutoTask_BeforMorningbyUser();
				this->AutoTask_AfterNoonClosebyUser();
				this->AutoTask_BeforNightbyUser();
			}
			printf("ttttttttttttttttt ---> 44444444444444 \n");
			this->AutoTaskEveryDay();
	}

	return;
}
void ThreadTrade::mdRun(){


	this->SetBaseAccount(true);
	DateUtil dateutil;
	char logbuf[1024];

	printf("Trade:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
	sprintf(logbuf,"Trade:-----------------开始自动交易程序!!!!!!   %s" ,dateutil.GetCurrentSqlTimeString().c_str());
	this->GetLogUtil()->WriteLog(logbuf);


	if(this->GetBaseAccount())	{
//		redisdepthmarket.FlushDB();
		this->GetRedisSignal()->FlushDB();
		this->GetRedisSection()->FlushDB();
		printf("Trade:开始启动程序  --->>>>redis 清空 ok \n");
	}

	//初始化Mysql数据库table
//	DropSqlTable();
	//临时代码
	MysqlInvestPosition mysqlinvestposition(this->GetUserid().c_str());
	if(mysqlinvestposition.Exist_Table()){
		printf("	DropSqlTable:drop数据表  mysqlinvestposition !!!\n");
		mysqlinvestposition.DropTable();
	}

	if(!mysqlinvestposition.Exist_Table()){
		mysqlinvestposition.CreateTable();
	}
//	printf("Trade:开始启动程序  --->>>>DropSqlTable ok \n");
	InitialSqlTable();
	printf("Trade:开始启动程序  --->>>>InitialSqlTable ok \n");

	MysqlAsset mysqlasset(this->GetUserid().c_str());
	DataWrapper datawrapper;
	this->SetDataWrapper(&datawrapper);
	datawrapper.SetUserID(this->GetUserid().c_str());
	datawrapper.InitCommisionMap();

	DayTrader *ptrader = new DayTrader(&datawrapper,this->GetStrategyName().c_str());
	this->SetTrader(ptrader);
	this->GetTrader()->SetMysqlAsset(&mysqlasset);
	this->GetTrader()->SetMysqlInvestPosition(&mysqlinvestposition);
	this->GetTrader()->SetRedisDepthMarket(this->GetRedisDepthMarket());
	this->GetTrader()->SetRedisSignal(this->GetRedisSignal());
	this->GetTrader()->SetRedisSection(this->GetRedisSection());

//	 std::thread t(this->connect());
	printf("Trade:开始启动程序  --->>>>LocalLogin--->>>>ApiVersion %s \n",this->GetTrader()->GetCThostFtdcTraderApi()->GetApiVersion());
	this->LocalLogin_MD();
	//	sleep(1);
	this->TradeInitial_MD();



	//完成主力合约计算后,contract lists 写入map
	datawrapper.start();
	MdConnect();

	sleep(20);
	this->GetTrader()->Close();
	datawrapper.join();

	return;
}


