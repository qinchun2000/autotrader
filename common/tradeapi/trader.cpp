#include "trader.hpp"


Trader::Trader() {
}

Trader::~Trader() {
}


void Trader::InitialTradeApi()
{
		Config config(this->GetStrategyName().c_str());
		MysqlOrder mysqlorder(config.GetTable_OrderList().c_str(),this->GetPointUserLoginField()->UserID);
		std::string tablename=config.GetTable_OrderList();
		tablename.append("_");
		tablename.append(this->GetPointUserLoginField()->UserID);
		mysqlorder.CreateTable();
		mysqlorder.DeleteAllbyTableName(tablename.c_str());

		MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
		mysqlopenorder.CreateTable();

		MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
		mysqlcloseorder.CreateTable();

		MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
		mysqlhold.CreateTable();

//		trader.ReqQryInvestor();
		printf("InitialTradeApi   --->>>>:ReqQryTradingAccount\n");
		this->ReqQryTradingAccount();

//		this->ReqQryNotice();

//		this->ReqQueryMaxOrderVolume();

		printf("InitialTradeApi   --->>>>:CheckSettleInfoConfirm\n");
		this->CheckSettleInfoConfirm();

		printf("InitialTradeApi  --->>>>:ReqQryExchange\n");
		this->ReqQryExchange();

		////////////////////////////////////////////////////////////////////////////////////////////////
		///请求查询合约
		///////////////////////////////////////////////////////////////////////////////////////////////

		printf("InitialTradeApi   --->>>>:CheckSqlInstrument\n");
		this->CheckSqlInstrument();

		printf("InitialTradeApi   --->>>>:CheckSqlProduct\n");
		this->CheckSqlProduct();
		printf("InitialTradeApi   --->>>>:CheckSqlDepMarket\n");
		this->CheckSqlDepMarket();

		DateUtil dateutil;
		double Q_BarTime_2 = dateutil.GetCurrentBarTime();
		 if (Q_BarTime_2 >=0.1500 && Q_BarTime_2 < 0.1600 &&  !dateutil.Check_wday())		{
			 printf("InitialTradeApi   --->>>>:CheckSqlDepMarket_EndTime\n");
			 this->CheckSqlDepMarket_EndTime();
		 }

		printf("InitialTradeApi   --->>>>:CheckSqlMaincontracont\n");
		this->CheckSqlMaincontracontbyOpenInterest(); // 主力合约切换迅速

//		trader.ReqQryTrade();
		//detial 在前,因为需要detail的数据 进行校验
		printf("InitialTradeApi   --->>>>:ReqQryInvestorPositionDetail\n");
		this->ReqQryInvestorPositionDetail();

		printf("InitialTradeApi   --->>>>:ReqQryInvestorPosition\n");
		this->ReqQryInvestorPosition();
//		trader.ReqQryOrder();
		printf("InitialTradeApi   --->>>>:ReqQryOrder\n");
		this->ReqQryOrder();
		this->ReqQryTrade();

		printf("InitialTradeApi   --->>>>:CheckTradingDay\n");
		// 当日交易日的检测,有待完善
		this->CheckTradingDay();
}

void Trader::InitialTradeApibyUser()
{

		Config config(this->GetStrategyName().c_str());
		MysqlOrder mysqlorder(config.GetTable_OrderList().c_str(),this->GetPointUserLoginField()->UserID);
		std::string tablename=config.GetTable_OrderList();
		tablename.append("_");
		tablename.append(this->GetPointUserLoginField()->UserID);
		mysqlorder.CreateTable();
		mysqlorder.DeleteAllbyTableName(tablename.c_str());

		MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
		mysqlopenorder.CreateTable();

		MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
		mysqlcloseorder.CreateTable();

		MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
		mysqlhold.CreateTable();


		printf("InitialTradeApibyUser   --->>>>:ReqQryTradingAccount\n");
		this->ReqQryTradingAccount();


		printf("InitialTradeApibyUser   --->>>>:CheckSettleInfoConfirm\n");
		this->CheckSettleInfoConfirm();

		printf("InitialTradeApibyUser   --->>>>:ReqQryInvestorPositionDetail\n");
		this->ReqQryInvestorPositionDetail();

		//detial 在前,因为需要detail的数据 进行校验
		printf("InitialTradeApibyUser   --->>>>:ReqQryInvestorPosition\n");
		this->ReqQryInvestorPosition();


		printf("InitialTradeApibyUser   --->>>>:ReqQryOrder\n");
		this->ReqQryOrder();

		printf("InitialTradeApibyUser   --->>>>:CheckTradingDay\n");
		// 当日交易日的检测,有待完善
//		this->CheckTradingDay();


}

void Trader::InitialTradeApi_MD()
{
//		printf("InitialTradeApi_MD  --->>>>:ReqQryExchange\n");
//		this->ReqQryExchange();
		////////////////////////////////////////////////////////////////////////////////////////////////
		///请求查询合约
		///////////////////////////////////////////////////////////////////////////////////////////////

		printf("InitialTradeApi_MD   --->>>>:CheckSInstrument\n");
		this->CheckInstrument();

//		printf("InitialTradeApi_MD   --->>>>:CheckSqlProduct\n");
//		this->CheckSqlProduct();

		printf("InitialTradeApi_MD   --->>>>:CheckDepMarket\n");
		this->CheckDepMarket();

//		DateUtil dateutil;
//		double Q_BarTime_2 = dateutil.GetCurrentBarTime();
//		 if (Q_BarTime_2 >=0.1500 && Q_BarTime_2 < 0.1600 &&  !dateutil.Check_wday())		{
//			 printf("InitialTradeApi_MD   --->>>>:CheckSqlDepMarket_EndTime\n");
//			 this->CheckSqlDepMarket_EndTime();
//		 }
//		printf("InitialTradeApi_MD   --->>>>:CheckSqlMaincontracont\n");
//		this->CheckSqlMaincontracontbyOpenInterest(); // 主力合约切换迅速

//		printf("InitialTradeApi_MD   --->>>>:CheckTradingDay\n");
		// 当日交易日的检测,有待完善
//		this->CheckTradingDay();
}
void Trader::ReqInit()
{
//	sem_init(&sem,0,0);

	this->GetCTraderSpi()->ResetSemInit();

	printf("Init  Ready   --->>>>\n");
	int i=0;

	while (!this->GetCTraderSpi()->GetFlagFrontConnectFalg())	{
			printf("Init 未完成   --->>>>\n");

			if(i>0)	{

			}

			this->GetCThostFtdcTraderApi()->Init();
			i++;
			// 等待服务器发出登录消息

			printf(" Init ---- sem_wait(&sem)----->>>>   \n");

			int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemInit());

			if (ret==0)	{
				printf("Debug: ReqInit successed！  :\n");

			}
			else{
				printf("Debug: ReqInit failed！  :\n");
			}

		}

	printf("<<<<<----------Init 完成 ！！！！！\n");

}

void Trader::ReqAuthenticate()
{
	char logbuf[300];


	printf("Trader::ReqAuthenticate   ---->Begin!!!   \n");
	int rest = this->GetCTraderSpi()->ResetSemAuthenticate();

	if(rest)  {//初始化信号量失败

		perror("Trader::ReqAuthenticate Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}

	CThostFtdcReqAuthenticateField authenticatefield ;
	memset(&authenticatefield,0,sizeof(authenticatefield));

	strcpy(authenticatefield.BrokerID, this->GetPointUserLoginField()->BrokerID);
	strcpy(authenticatefield.UserID,  this->GetPointUserLoginField()->UserID);

//	gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
	char info[11];
	char code[17];
	CodeUtil codeutil;
	std::string userproductinfo;
	std::string authcode;
	std::string appid;

	Config config(this->GetStrategyName().c_str());

	strcpy(authenticatefield.UserProductInfo, config.GetProductInfo().c_str()); // 11 字节 限定
	strcpy(authenticatefield.AuthCode,  config.GetAuthCode().c_str());  // 17字节限定


	if((strlen(authenticatefield.BrokerID)==0)){
		sprintf(logbuf,"Trader::ReqAuthenticate   上报信息为空----> BrokerID:%s  \n",authenticatefield.BrokerID);

		Msg_Debug("%s",logbuf);
		this->GetLogUtil()->WriteLog(logbuf);
		exit(-1);
	}

	if((strlen(authenticatefield.UserID)==0)){
		sprintf(logbuf,"Trader::ReqAuthenticate   上报信息为空----> UserID:%s  \n",authenticatefield.UserID);
		this->GetLogUtil()->WriteLog(logbuf);
		exit(-1);
	}

	if((strlen(authenticatefield.UserProductInfo)==0)){
		sprintf(logbuf,"Trader::ReqAuthenticate   上报信息为空----> UserProductInfo:%s  \n",authenticatefield.UserProductInfo);
		this->GetLogUtil()->WriteLog(logbuf);
		exit(-1);
	}


	if((strlen(authenticatefield.AuthCode)==0)){
		sprintf(logbuf,"Trader::ReqAuthenticate   上报信息为空----> AuthCode:%s  \n",authenticatefield.AuthCode);
		this->GetLogUtil()->WriteLog(logbuf);
		exit(-1);
	}

//	printf("Trader::ReqAuthenticate   上报信息----> BrokerID:%s  \n",authenticatefield.BrokerID);
//	printf("Trader::ReqAuthenticate   上报信息----> UserID:%s  \n",authenticatefield.UserID);
//	printf("Trader::ReqAuthenticate   上报信息----> UserProductInfo:%s  信息长度:%ld \n",
//			authenticatefield.UserProductInfo,strlen(authenticatefield.UserProductInfo));
//	printf("Trader::ReqAuthenticate   上报信息----> AuthCode:%s  信息长度:%ld\n",
//			authenticatefield.AuthCode,strlen(authenticatefield.AuthCode));

	// 以下代码切换回模拟盘 需要注释///////////////////////////////////////////////////

	strcpy(authenticatefield.AppID, config.GetAppID().c_str());
	if((strlen(authenticatefield.AppID)==0)){
		sprintf(logbuf,"Trader::ReqAuthenticate   上报信息为空----> AppID:%s  \n",authenticatefield.AppID);
		this->GetLogUtil()->WriteLog(logbuf);
		exit(-1);
	}
//	printf("Trader::ReqAuthenticate   上报信息----> AppID:%s  信息长度:%ld \n",
//			authenticatefield.AppID,strlen(authenticatefield.AppID));

	sleep(1);
	DateUtil dateutil;
	int rt_pTraderApi =this->GetCThostFtdcTraderApi()->ReqAuthenticate(&authenticatefield,this->UpdateRequestID());
	if (rt_pTraderApi==0){
//		printf("Trader::ReqAuthenticate successed！  \n");

		sprintf(logbuf,"Trader::ReqAuthenticate successed！  \n");
		this->GetLogUtil()->WriteLog(logbuf);

//		printf("Trader::ReqAuthenticate  sem_wait(&sem_authenticate)   Begin------->>>>  %s \n",dateutil.GetCurrentSqlTimeString().c_str());
		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemAuthenticate());
//		printf("Trader::ReqAuthenticate  sem_wait(&sem_authenticate)   <<<<---------End  %s \n",dateutil.GetCurrentSqlTimeString().c_str());
	}
	else	{


		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (rt_pTraderApi==-1){
			sprintf(logbuf,"Trader::ReqAuthenticate failed！ [-1] 网络连接失败!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if(rt_pTraderApi==-2){
			sprintf(logbuf,"Trader::ReqAuthenticate failed！ [-2] 未处理请求超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if(rt_pTraderApi==-3){
			sprintf(logbuf,"Trader::ReqAuthenticate failed！ [-3] 每秒发送请求数超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else{
			sprintf(logbuf,"Trader::ReqAuthenticate failed！ [%d] 未知故障!!! \n",rt_pTraderApi);
			this->GetLogUtil()->WriteLog(logbuf);
		}

	}


}

void Trader::ReqQryTradingAccount()
{
	char logbuf[1024];


	printf("Trader::ReqQryTradingAccount  Begin !!!\n");
	int rest = this->GetCTraderSpi()->ResetSemTradingAccount();

	 if(rest) {//初始化信号量失败

		perror("Trader::ReqQryTradingAccount ---->Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}

//RMB(CNY)-人民币 USD-美圆 HKD-港元
	CThostFtdcQryTradingAccountField tradingaccoutfield ;
	memset(&tradingaccoutfield,0,sizeof(tradingaccoutfield));
	strcpy(tradingaccoutfield.BrokerID, this->GetPointUserLoginField()->BrokerID);
	strcpy(tradingaccoutfield.InvestorID,  this->GetPointUserLoginField()->UserID);
	strcpy(tradingaccoutfield.CurrencyID, "");

//	sleep(1);
	int requestid=this->UpdateRequestID();
//	this->GetDataWrapper()->EmptyAssetMap();
	int rt_pTraderApi =this->GetCThostFtdcTraderApi()->ReqQryTradingAccount(&tradingaccoutfield,requestid);
	if (rt_pTraderApi==0)	{
//		printf("ReqQryTradingAccount: ReqQryTradingAccount successed！  :\n");
		sprintf(logbuf,"Debug: ReqQryTradingAccount successed！ ---->requestid: %d\n",requestid);
		this->GetLogUtil()->WriteLog(logbuf);
		DateUtil dateutil;
//		printf("ReqQryTradingAccount:  sem_wait(&sem_tradingaccount)   begin-------->>>> %s \n",dateutil.GetCurrentSqlTimeString().c_str());

		this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemTradingAccount());

		std::shared_ptr<AssetData> data_asset = this->GetDataWrapper()->FindAssetData( this->GetPointUserLoginField()->UserID);
		sprintf(logbuf,"   userid:%s  交易日:%s 动态权益:%.2f   持仓浮盈:%2f   平仓浮盈:%.2f \n",data_asset->AccountID,
				data_asset->TradingDay,data_asset->DynamicEquity,data_asset->PositionProfit,data_asset->CloseProfit);
		this->GetLogUtil()->WriteLog(logbuf);
		sprintf(logbuf,"ReqQryTradingAccount:  sem_wait(&sem_tradingaccount)   <<<<---------End %s \n",dateutil.GetCurrentSqlTimeString().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
	}
	else		{
		printf("ReqQryTradingAccount: ReqQryTradingAccount failed！  :\n");
		sprintf(logbuf,"Debug: ReqQryTradingAccount failed！  :\n");
		this->GetLogUtil()->WriteLog(logbuf);

		if (rt_pTraderApi==-1){
			printf("Trader::ReqQryTradingAccount failed！ [-1] 网络连接失败!!! \n");
			sprintf(logbuf,"Trader::ReqQryTradingAccount failed！ [-1] 网络连接失败!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if(rt_pTraderApi==-2){
			printf("Trader::ReqQryTradingAccount failed！ [-2] 未处理请求超过许可数!!! \n");
			sprintf(logbuf,"Trader::ReqQryTradingAccount failed！ [-2] 未处理请求超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
			this->SetFlagTradeLimit(true);
		}
		else if(rt_pTraderApi==-3){
			printf("Trader::ReqQryTradingAccount failed！ [-3] 每秒发送请求数超过许可数!!! \n");
			sprintf(logbuf,"Trader::ReqQryTradingAccount failed！ [-3] 每秒发送请求数超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else{
			printf("Trader::ReqQryTradingAccount failed！ [%d] 未知故障!!! \n",rt_pTraderApi);
			sprintf(logbuf,"Trader::ReqQryTradingAccount failed！ [%d] 未知故障!!! \n",rt_pTraderApi);
			this->GetLogUtil()->WriteLog(logbuf);
		}

	}
	printf("Trader::ReqQryTradingAccount  End !!!\n");
}
void Trader::ReqQryInvestor()
{
	int rest = this->GetCTraderSpi()->ResetSemInvestor();

	 if(rest)//初始化信号量失败
	{
	perror("Semaphore initialization failed\n");
	exit(EXIT_FAILURE);
	}

	CThostFtdcQryInvestorField investorfield ;
	memset(&investorfield,0,sizeof(investorfield));
	strcpy(investorfield.BrokerID, this->GetPointUserLoginField()->BrokerID);
	strcpy(investorfield.InvestorID,  this->GetPointUserLoginField()->UserID);

	sleep(1);
	int rt_pTraderApi=this->GetCThostFtdcTraderApi()->ReqQryInvestor (&investorfield,this->UpdateRequestID());

	if (rt_pTraderApi==0)	{
		printf("Debug: ReqQryInvestor successed！  :\n");
		printf("sem_wait(&sem_investor)    --->>>>begin \n");

		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemInvestor());
		printf("sem_wait(&sem_investor)  <<<<<<---- end \n");

	}
	else	{
		printf("Debug: ReqQryInvestor failed！  :\n");
	}


}
void Trader::ReqQryInvestorPosition()
{
	char logbuf[1024];

	///请求查询投资者持仓
	int rest = this->GetCTraderSpi()->ResetSemInvest();
	 if(rest)	{
		perror("Semaphore initialization failed\n");    //初始化信号量失败
		exit(EXIT_FAILURE);
	}

	CThostFtdcQryInvestorPositionField pQryInvestorPosition;
	// 确保没有初始化的数据不会被访问
	memset(&pQryInvestorPosition,0,sizeof(pQryInvestorPosition));
	strcpy(pQryInvestorPosition.BrokerID,this->GetBrokerID().c_str());
	strcpy(pQryInvestorPosition.InvestorID, this->GetUserID().c_str());

	printf("Debug: ReqQryInvestorPosition successed！  清空持仓汇总表!!! \n");

	int result = this->GetCThostFtdcTraderApi()->ReqQryInvestorPosition(&pQryInvestorPosition, this->UpdateRequestID()) ;

	if (result==0)		{

//		printf("ReqQryInvestorPosition:请求持仓汇总表---- sem_wait(&sem_investorpostion)  begin  ---->>>>  \n");
		sprintf(logbuf,"ReqQryInvestorPosition:请求持仓汇总表---- sem_wait(&sem_investorpostion)  begin  ---->>>>  ");
		this->GetLogUtil()->WriteLog(logbuf);
		int ret=this->GetCTraderSpi()->LongTimeWaitSem(this->GetCTraderSpi()->GetSemInvest());

		vector<InvestPositionData> list;
		this->GetDataWrapper()->GetInvestList(list);
		for(auto &item:list){
			sprintf(logbuf,"	合约:%s    开仓价:%.2f   position:%d 方向:%d  openvolume:%d closevolume:%d",
					item.InstrumentID,item.OpenPriceAverage,item.Position,item.PosiDirection,item.OpenVolume,item.CloseVolume);
			this->GetLogUtil()->WriteLog(logbuf);
		}
		vector<InvestPositionData>().swap(list);
		sprintf(logbuf,"ReqQryInvestorPosition:请求持仓汇总表---- sem_wait(&sem_investorpostion)  <<<<----end!!!   \n");
		this->GetLogUtil()->WriteLog(logbuf);
	}
	else		{
//		this->GetCThostFtdcTraderApi()->ReqQryInvestorPosition(&pQryInvestorPosition, this->UpdateRequestID()) ;
		printf("Debug: ReqQryInvestorPosition failed！  :\n");
		sprintf(logbuf,"Debug: ReqQryInvestorPosition failed！  :\n");
		this->GetLogUtil()->WriteLog(logbuf);

		if (result==-1){
			sprintf(logbuf,"Trader::ReqQryInvestorPosition failed！ [-1] 网络连接失败!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if(result==-2){
			sprintf(logbuf,"Trader::ReqQryInvestorPosition failed！ [-2] 未处理请求超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
			this->SetFlagTradeLimit(true);
		}
		else if(result==-3){
			sprintf(logbuf,"Trader::ReqQryInvestorPosition failed！ [-3] 每秒发送请求数超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else{
			sprintf(logbuf,"Trader::ReqQryInvestorPosition failed！ [%d] 未知故障!!! \n",result);
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}

}

void Trader::ReqQryInvestorPositionDetail()
{
	char logbuf[1024];

	///请求查询投资者持仓
	int rest = this->GetCTraderSpi()->ResetSemInvestPosDetail();

	 if(rest)//初始化信号量失败
		{
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}

	CThostFtdcQryInvestorPositionDetailField pQryInvestorPositionDetial;
	// 确保没有初始化的数据不会被访问
	memset(&pQryInvestorPositionDetial,0,sizeof(pQryInvestorPositionDetial));
	strcpy(pQryInvestorPositionDetial.BrokerID,this->GetBrokerID().c_str());
	strcpy(pQryInvestorPositionDetial.InvestorID, this->GetUserID().c_str());

//	sleep(1);

//	this->GetDataWrapper()->EmptyPositionDetailMap();

//	MysqlPosDetail mysqlposdetail(this->GetPointUserLoginField()->UserID);
//	mysqlposdetail.CreateTable();
//	mysqlposdetail.DeleteAll();
	printf("ReqQryInvestorPositionDetail: ReqQryInvestorPosition successed！ 清空持仓明细表 !! \n");


	int result = this->GetCThostFtdcTraderApi()->ReqQryInvestorPositionDetail(&pQryInvestorPositionDetial, this->UpdateRequestID()) ;

	if (result==0){
		//
		printf("ReqQryInvestorPositionDetail:请求持仓明细表 ---- sem_wait(&sem_investorpostiondetail)   begin ----->>>>\n");
		sprintf(logbuf,"ReqQryInvestorPositionDetail:请求持仓明细表 ---- sem_wait(&sem_investorpostiondetail)   begin ----->>>>");
		this->GetLogUtil()->WriteLog(logbuf);
		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemInvestPosDetail());
		printf("ReqQryInvestorPositionDetail:请求持仓明细表 ---- sem_wait(&sem_investorpostiondetail)   <<<<<-----  end  !!! \n");

		vector<PositionDetailData> detaillist;
		this->GetDataWrapper()->GetPositionDetailList(detaillist);

		for (auto &item:detaillist)	{
			sprintf(logbuf,"	持仓明细: ----->  %s ---> 方向:%d  手数:%d  开仓价:%.2f  closevolue:%d 保证金:%.2f TradeID:%s  opendate:%s",
					item.InstrumentID,item.Direction,item.Volume,item.OpenPrice,item.CloseVolume,item.Margin,item.TradeID,item.OpenDate);
			this->GetLogUtil()->WriteLog(logbuf);
		}


		vector<PositionDetailData>().swap(detaillist);

		sprintf(logbuf,"ReqQryInvestorPositionDetail:请求持仓明细表 ---- sem_wait(&sem_investorpostiondetail)    <<<<<-----  end\n");
		this->GetLogUtil()->WriteLog(logbuf);
	}
	else	{
		printf("ReqQryInvestorPositionDetail: ReqQryInvestorPosition failed！  :\n");
		if (result==-1){
			sprintf(logbuf,"Trader::ReqQryInvestorPositionDetail failed！ [-1] 网络连接失败!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if(result==-2){
			sprintf(logbuf,"Trader::ReqQryInvestorPositionDetail failed！ [-2] 未处理请求超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
			this->SetFlagTradeLimit(true);
		}
		else if(result==-3){
			sprintf(logbuf,"Trader::ReqQryInvestorPositionDetail failed！ [-3] 每秒发送请求数超过许可数!!! \n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else{
			sprintf(logbuf,"Trader::ReqQryInvestorPositionDetail failed！ [%d] 未知故障!!! \n",result);
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}

}
void Trader::ReqQryNotice()
{
	int rest = this->GetCTraderSpi()->ResetSemNotice();

	 if(rest) {//初始化信号量失败
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}

	CThostFtdcQryNoticeField  noticefield ;
	memset(&noticefield,0,sizeof(noticefield));
	strcpy(noticefield.BrokerID,this->GetBrokerID().c_str());


	sleep(1);
	int rt_pTraderApi =this->GetCThostFtdcTraderApi()->ReqQryNotice(&noticefield, this->UpdateRequestID());
	if (rt_pTraderApi==0)		{
		printf("Debug: ReqQryNotice successed！  :\n");
		printf(" sem_wait(&sem_notice)   --->>>> \n");

		int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemNotice());
		printf("sem_wait(&sem_notice)   <<<<<<---- \n");
	}
	else	{
		printf("Debug: ReqQryNotice failed！  :\n");
	}

}

void Trader::ReqQueryMaxOrderVolume()
{
	int rest = this->GetCTraderSpi()->ResetSemMaxOrder();
	////////////////////////////////////////////////////////////////////////////////////////////////
	///查询最大报单数量请求
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQueryMaxOrderVolumeField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserID().c_str());
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(requestData.InstrumentID,"rb1801");
	///买卖方向 TThostFtdcDirectionType char
	//// THOST_FTDC_D_Buy '0' 买
	//// THOST_FTDC_D_Sell '1' 卖
	requestData.Direction = THOST_FTDC_D_Sell;
	///开平标志 TThostFtdcOffsetFlagType char
	//// THOST_FTDC_OF_Open '0' 开仓
	//// THOST_FTDC_OF_Close '1' 平仓
	//// THOST_FTDC_OF_ForceClose '2' 强平
	//// THOST_FTDC_OF_CloseToday '3' 平今
	//// THOST_FTDC_OF_CloseYesterday '4' 平昨
	//// THOST_FTDC_OF_ForceOff '5' 强减
	//// THOST_FTDC_OF_LocalForceClose '6' 本地强平
	requestData.OffsetFlag = THOST_FTDC_OF_Open;
	///投机套保标志 TThostFtdcHedgeFlagType char
	//// THOST_FTDC_HF_Speculation '1' 投机
	//// THOST_FTDC_HF_Arbitrage '2' 套利
	//// THOST_FTDC_HF_Hedge '3' 套保
	requestData.HedgeFlag = THOST_FTDC_HF_Arbitrage;
	///最大允许报单数量 TThostFtdcVolumeType int
	requestData.MaxVolume = 0;
	sleep(1);

	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqQueryMaxOrderVolume(&requestData,this->UpdateRequestID());

	if (result==0)	{
		printf("Debug: ReqQueryMaxOrderVolume successed！  :\n");
		printf(" sem_wait(&sem_querymaxordervolume)   --->>>> begin \n");

		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemMaxOrder());
		printf("sem_wait(&sem_querymaxordervolume)  <<<<<<----end \n");

	}
	else	{
		printf("Debug: ReqQueryMaxOrderVolume failed！  :\n");
	}

}
// 请求查询确认结算单
void Trader::ReqQrySettlementInfoConfirm ()
{
	char logbuf[1024];

	int rest = this->GetCTraderSpi()->ResetSemQrySettleInfoConfirm();

	 if(rest)//初始化信号量失败
		{
		perror("Trader::ReqQrySettlementInfoConfirm   Semaphore initialization failed\n");
		this->GetLogUtil()->WriteLog("Trader::ReqQrySettlementInfoConfirm --->Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}

	////////////////////////////////////////////////////////////////////////////////////////////////
	///投资者结算结果确认
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	 CThostFtdcQrySettlementInfoConfirmField  settlementinfoconfirmfield;
	// 确保没有初始化的数据不会被访问
	memset(&settlementinfoconfirmfield,0,sizeof(settlementinfoconfirmfield));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(settlementinfoconfirmfield.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(settlementinfoconfirmfield.InvestorID,this->GetUserID().c_str());
	///确认日期 TThostFtdcDateType char[9]
//	strcpy(settlementinfoconfirmfield.ConfirmDate,"");
//	///确认时间 TThostFtdcTimeType char[9]
//	strcpy(settlementinfoconfirmfield.ConfirmTime,"");

	sleep(1);
	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqQrySettlementInfoConfirm(&settlementinfoconfirmfield,this->UpdateRequestID());
	if (result==0)
	{
		printf("Debug: ReqQrySettlementInfoConfirm successed！  %s \n",this->GetUserID().c_str());
		sprintf(logbuf,"Debug: ReqQrySettlementInfoConfirm successed！  %s \n",this->GetUserID().c_str());
		this->GetLogUtil()->WriteLog(logbuf);

		printf(" sem_wait(&sem_qrysettleinfoconfirm)   --->>>>\n");
		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemQrySettleInfoConfirm());
		printf("sem_wait(&sem_qrysettleinfoconfirm)   <<<<<<----\n");
	}
	else
	{
		printf("Debug: ReqQrySettlementInfoConfirm failed！  %s \n",this->GetUserID().c_str());
		sprintf(logbuf,"Debug: ReqQrySettlementInfoConfirm failed！  %s \n",this->GetUserID().c_str());
		this->GetLogUtil()->WriteLog(logbuf);

		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (result==-1){
			printf("Trader::ReqQrySettlementInfoConfirm failed！ [-1] 网络连接失败!!! \n");
		}
		else if(result==-2){
			printf("Trader::ReqQrySettlementInfoConfirm failed！ [-2] 未处理请求超过许可数!!! \n");
			this->SetFlagTradeLimit(true);
		}
		else if(result==-3){
			printf("Trader::ReqQrySettlementInfoConfirm failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("Trader::ReqQrySettlementInfoConfirm failed！ [%d] 未知故障!!! \n",result);
		}

	}


}

// 请求查询确认结算单
void Trader::ReqQrySettlementInfo ()
{

	char logbuf[1024];

	sprintf(logbuf,"Trader::ReqQrySettlementInfo   %s \n",this->GetTradingDay().c_str());
	int rest = this->GetCTraderSpi()->ResetSemQrySettleInfo();

	//初始化信号量失败
	if(rest)		{
		perror("Semaphore initialization failed\n");
		this->GetLogUtil()->WriteLog("Trader::ReqQrySettlementInfo --->Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	///投资者结算结果确认
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQrySettlementInfoField settlementinfofield;
	// 确保没有初始化的数据不会被访问
	memset(&settlementinfofield,0,sizeof(settlementinfofield));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(settlementinfofield.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(settlementinfofield.InvestorID,this->GetUserID().c_str());

	///确认日期 TThostFtdcDateType char[9]
//	strcpy(settlementinfofield.TradingDay,this->GetTradingDay().c_str());
	DateUtil dateutil;

	strcpy(settlementinfofield.TradingDay,dateutil.GetCurrentDayString().c_str());

	sleep(3);
	// 调用API,并等待响应函数返回
	int requestid=this->UpdateRequestID();
	int result = this->GetCThostFtdcTraderApi()->ReqQrySettlementInfo(&settlementinfofield,requestid);
	if (result==0)	{
		printf("Trader::ReqQrySettlementInfo 发送请求成功！  %s  %s \n",this->GetUserID().c_str(),settlementinfofield.TradingDay);
		sprintf(logbuf,"Trader::ReqQrySettlementInfo 发送请求成功！  %s   ---->requestid: %d  结算日期:%s \n",
				this->GetUserID().c_str(),requestid,settlementinfofield.TradingDay);
		this->GetLogUtil()->WriteLog(logbuf);

		printf(" sem_wait(&sem_qrysettleinfo)   --->>>> %s \n",dateutil.GetCurrentSqlTimeString().c_str());

		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemQrySettleInfo());
		printf("sem_wait(&sem_qrysettleinfo)   <<<<<<----%s \n",dateutil.GetCurrentSqlTimeString().c_str());

	}
	else	{
		printf("Trader::ReqQrySettlementInfo 发送请求失败！  %s  %s \n",this->GetUserID().c_str(),settlementinfofield.TradingDay);
		sprintf(logbuf,"Trader::ReqQrySettlementInfo 发送请求失败！  %s   ---->requestid: %d  结算日期:%s \n",
				this->GetUserID().c_str(),requestid,settlementinfofield.TradingDay);
		this->GetLogUtil()->WriteLog(logbuf);

		if (result==-1){
			printf("Trader::ReqQrySettlementInfo failed！ [-1] 网络连接失败!!! \n");
		}
		else if(result==-2){
			printf("Trader::ReqQrySettlementInfo failed！ [-2] 未处理请求超过许可数!!! \n");
			this->SetFlagTradeLimit(true);
		}
		else if(result==-3){
			printf("Trader::ReqQrySettlementInfo failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("Trader::ReqQrySettlementInfo failed！ [%d] 未知故障!!! \n",result);
		}

	}


}

// 请求查询确认结算单
void Trader::ReqQrySettlementInfoDefault ()
{

	char logbuf[1024];

	sprintf(logbuf,"Trader::ReqQrySettlementInfoDefault   %s \n",this->GetTradingDay().c_str());
	int rest = this->GetCTraderSpi()->ResetSemQrySettleInfo();

	//初始化信号量失败
	if(rest)		{
		perror("Semaphore initialization failed\n");
		this->GetLogUtil()->WriteLog("Trader::ReqQrySettlementInfoDefault --->Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	///投资者结算结果确认
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQrySettlementInfoField settlementinfofield;
	// 确保没有初始化的数据不会被访问
	memset(&settlementinfofield,0,sizeof(settlementinfofield));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(settlementinfofield.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(settlementinfofield.InvestorID,this->GetUserID().c_str());

	///确认日期 TThostFtdcDateType char[9]
//	strcpy(settlementinfofield.TradingDay,this->GetTradingDay().c_str());
	DateUtil dateutil;

	strcpy(settlementinfofield.TradingDay,"");

	sleep(3);
	// 调用API,并等待响应函数返回
	int requestid=this->UpdateRequestID();
	int result = this->GetCThostFtdcTraderApi()->ReqQrySettlementInfo(&settlementinfofield,requestid);
	if (result==0)	{
		printf("Trader::ReqQrySettlementInfoDefault 发送请求成功！  %s  %s \n",this->GetUserID().c_str(),settlementinfofield.TradingDay);
		sprintf(logbuf,"Trader::ReqQrySettlementInfoDefault 发送请求成功！  %s   ---->requestid: %d  结算日期:%s \n",
				this->GetUserID().c_str(),requestid,settlementinfofield.TradingDay);
		this->GetLogUtil()->WriteLog(logbuf);

		printf(" sem_wait(&sem_qrysettleinfo)   --->>>>\n");

		int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemQrySettleInfo());
		printf("sem_wait(&sem_qrysettleinfo)   <<<<<<----\n");

	}
	else	{
		printf("Trader::ReqQrySettlementInfoDefault 发送请求失败！  %s  %s \n",this->GetUserID().c_str(),settlementinfofield.TradingDay);
		sprintf(logbuf,"Trader::ReqQrySettlementInfoDefault 发送请求失败！  %s   ---->requestid: %d  结算日期:%s \n",
				this->GetUserID().c_str(),requestid,settlementinfofield.TradingDay);
		this->GetLogUtil()->WriteLog(logbuf);

		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (result==-1){
			printf("Trader::ReqQrySettlementInfoDefault failed！ [-1] 网络连接失败!!! \n");
		}
		else if(result==-2){
			printf("Trader::ReqQrySettlementInfoDefault failed！ [-2] 未处理请求超过许可数!!! \n");
			this->SetFlagTradeLimit(true);
		}
		else if(result==-3){
			printf("Trader::ReqQrySettlementInfoDefault failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("Trader::ReqQrySettlementInfoDefault failed！ [%d] 未知故障!!! \n",result);
		}
	}


}
// 请求确认结算单
void Trader::ReqSettlementInfoConfirm ()
{
	char logbuf[1024];


	int rest = this->GetCTraderSpi()->ResetSemSettleInfoConfirm();

	 if(rest)//初始化信号量失败
		{
		perror("Semaphore initialization failed\n");
		this->GetLogUtil()->WriteLog("Trader::ReqSettlementInfoConfirm --->Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}

	////////////////////////////////////////////////////////////////////////////////////////////////
	///投资者结算结果确认
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcSettlementInfoConfirmField settlementinfoconfirmfield;
	// 确保没有初始化的数据不会被访问
	memset(&settlementinfoconfirmfield,0,sizeof(settlementinfoconfirmfield));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(settlementinfoconfirmfield.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(settlementinfoconfirmfield.InvestorID,this->GetUserID().c_str());
	///确认日期 TThostFtdcDateType char[9]
	strcpy(settlementinfoconfirmfield.ConfirmDate,"");
	///确认时间 TThostFtdcTimeType char[9]
	strcpy(settlementinfoconfirmfield.ConfirmTime,"");

	sleep(1);
	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqSettlementInfoConfirm(&settlementinfoconfirmfield,this->UpdateRequestID());
	if (result==0)		{
		printf("Debug: ReqSettlementInfoConfirm successed！  :\n");
		sprintf(logbuf,"Debug: ReqSettlementInfoConfirm successed！  :\n");
		this->GetLogUtil()->WriteLog(logbuf);

		printf(" sem_wait(&sem_settleinfoconfirm)   --->>>> \n");
			int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemSettleInfoConfirm());
			printf("sem_wait(&sem_settleinfoconfirm)    <<<<<<----\n");

	}
	else	{
		printf("Debug: ReqSettlementInfoConfirm failed！  :\n");
		sprintf(logbuf,"Debug: ReqSettlementInfoConfirm failed！  :\n");
		this->GetLogUtil()->WriteLog(logbuf);

		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (result==-1){
			printf("Trader::ReqSettlementInfoConfirm failed！ [-1] 网络连接失败!!! \n");
		}
		else if(result==-2){
			printf("Trader::ReqSettlementInfoConfirm failed！ [-2] 未处理请求超过许可数!!! \n");
			this->SetFlagTradeLimit(true);
		}
		else if(result==-3){
			printf("Trader::ReqSettlementInfoConfirm failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("Trader::ReqSettlementInfoConfirm failed！ [%d] 未知故障!!! \n",result);
		}

	}

}

void Trader::ReqQryExchange()
{
	MysqlExchange mysqlexchange;
	int count = mysqlexchange.Count_AllLists();

	if (count ==0)	{
		int rest = this->GetCTraderSpi()->ResetSemExchange();

		 if(rest)//初始化信号量失败
			{
			perror("Semaphore initialization failed\n");
			exit(EXIT_FAILURE);
			}

		////////////////////////////////////////////////////////////////////////////////////////////////
		///请求查询交易所
		///////////////////////////////////////////////////////////////////////////////////////////////
		// 定义调用API的数据结构
		CThostFtdcQryExchangeField exchangefield;
		// 确保没有初始化的数据不会被访问
		memset(&exchangefield,0,sizeof(exchangefield));
		// 为调用结构题设置参数信息
		///交易所代码 TThostFtdcExchangeIDType char[9]
		strcpy(exchangefield.ExchangeID,"");

		sleep(1);
		// 调用API,并等待响应函数返回
		int result = this->GetCThostFtdcTraderApi()->ReqQryExchange(&exchangefield,this->UpdateRequestID());
		if (result==0)	{
			printf("Debug: ReqQryExchange successed！  :\n");
			printf(" sem_wait(&sem_exchange)   --->>>>\n");

			int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemExchange());
			printf("sem_wait(&sem_exchange)  <<<<<<----\n");
		}
		else	{
			printf("Debug: ReqQryExchange failed！  :\n");
		}

	}

}
void Trader::ReqQryInvestorProductGroupMargin()
{
	int rest = this->GetCTraderSpi()->ResetSemProductGroup();

	 if(rest)//初始化信号量失败
		{
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}

	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询投资者品种/跨品种保证金
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQryInvestorProductGroupMarginField productgroupField;
	// 确保没有初始化的数据不会被访问
	memset(&productgroupField,0,sizeof(productgroupField));
	// 为调用结构题设置参数信息
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(productgroupField.BrokerID,this->GetBrokerID().c_str());
	///交易所代码 TThostFtdcExchangeIDType char[9]
	strcpy(productgroupField.InvestorID,this->GetUserID().c_str());

	///产品代码 TThostFtdcInstrumentIDType char[31]
	strcpy(productgroupField.ProductGroupID,"");

	sleep(1);
	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqQryInvestorProductGroupMargin(&productgroupField,this->UpdateRequestID());
	if (result==0)
		{
			printf("Debug: ReqQryInvestorProductGroupMargin successed！  :\n");

		}
		else
		{
			printf("Debug: ReqQryInvestorProductGroupMargin failed！  :\n");
		}
	printf("sem_wait(&sem_productgroup)    --->>>>\n");


	int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemProductGroup());
	printf("sem_wait(&sem_productgroup)  <<<<<<---- \n");
}


void Trader::ReqQryOrder()
{
	///请求查询投资者报单
	int rest = this->GetCTraderSpi()->ResetSemOrder();

	 if(rest)//初始化信号量失败
		{
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}



	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询报单
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQryOrderField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserID().c_str());
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(requestData.InstrumentID,"");
	///交易所代码 TThostFtdcExchangeIDType char[9]
	strcpy(requestData.ExchangeID,"");
	///报单编号 TThostFtdcOrderSysIDType char[21]
	strcpy(requestData.OrderSysID,"");
	///开始时间 TThostFtdcTimeType char[9]
	strcpy(requestData.InsertTimeStart,"");
	///结束时间 TThostFtdcTimeType char[9]
	strcpy(requestData.InsertTimeEnd,"");

	sleep(1);

	int result = this->GetCThostFtdcTraderApi()->ReqQryOrder (&requestData, this->UpdateRequestID()) ;

	if (result==0)			{
		printf("Debug: ReqQryOrder successed！  :\n");

	}
	else			{
		printf("Debug: ReqQryOrder failed！  :\n");
	}
	printf(" sem_wait(&sem_order)   --->>>>\n");

int ret = this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemOrder());
	printf("sem_wait(&sem_order)   <<<<<<----\n");
}


void Trader::ReqQryOrder(const char * pinstrumentid)
{
	///请求查询投资者报单
	int rest = this->GetCTraderSpi()->ResetSemOrder();

	 if(rest)//初始化信号量失败
		{
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}



	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询报单
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQryOrderField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserID().c_str());
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(requestData.InstrumentID,pinstrumentid);
	///交易所代码 TThostFtdcExchangeIDType char[9]
	strcpy(requestData.ExchangeID,"");
	///报单编号 TThostFtdcOrderSysIDType char[21]
	strcpy(requestData.OrderSysID,"");
	///开始时间 TThostFtdcTimeType char[9]
	strcpy(requestData.InsertTimeStart,"");
	///结束时间 TThostFtdcTimeType char[9]
	strcpy(requestData.InsertTimeEnd,"");

	sleep(1);

	int result = this->GetCThostFtdcTraderApi()->ReqQryOrder (&requestData, this->UpdateRequestID()) ;

	if (result==0)
			{
				printf("Debug: ReqQryOrder successed！  :\n");

			}
			else
			{
				printf("Debug: ReqQryOrder failed！  :\n");
			}
	printf("<<<<<<---- sem_wait(&sem_order)   :\n");
////	sem_wait(&sem_investorpostion);
//
//	struct timeval now;
//	struct timespec outtime;
//
//	gettimeofday(&now, NULL);
//	cout << now.tv_sec << " " << (now.tv_usec) << "\n";
//	DateUtil dateutil;
//	dateutil.timeraddMS(&now, 15000);
//	outtime.tv_sec = now.tv_sec;
//	outtime.tv_nsec = now.tv_usec * 1000;
//	int ret;
//	ret = sem_timedwait(&sem_order,&outtime);

	int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemOrder());
	printf("sem_wait(&sem_order)   --->>>>:\n");
}
void Trader::ReqQryTrade()
{
	///请求查询投资者持仓
	int rest = this->GetCTraderSpi()->ResetSemTrade();

	 if(rest)//初始化信号量失败
		{
		perror("Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}

	CThostFtdcQryTradeField pQryTrade;
	// 确保没有初始化的数据不会被访问
	memset(&pQryTrade,0,sizeof(pQryTrade));
	strcpy(pQryTrade.BrokerID,this->GetBrokerID().c_str());
	strcpy(pQryTrade.InvestorID, this->GetUserID().c_str());

	sleep(1);

	int result = this->GetCThostFtdcTraderApi()->ReqQryTrade (&pQryTrade, this->UpdateRequestID()) ;

	if (result==0)
			{
				printf("Debug: ReqQryTrade successed！  :\n");

			}
			else
			{
				printf("Debug: ReqQryTrade failed！  :\n");
			}
	printf("<<<<<<---- sem_wait(&sem_trade)   :\n");
////	sem_wait(&sem_investorpostion);
//
//	struct timeval now;
//	struct timespec outtime;
//
//	gettimeofday(&now, NULL);
//	cout << now.tv_sec << " " << (now.tv_usec) << "\n";
//	DateUtil dateutil;
//	dateutil.timeraddMS(&now, 15000);
//	outtime.tv_sec = now.tv_sec;
//	outtime.tv_nsec = now.tv_usec * 1000;
//	int ret;
//	ret = sem_timedwait(&sem_trade,&outtime);
int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemTrade());
	printf("sem_wait(&sem_trade)   --->>>>:\n");
}
void Trader::CheckInstrument()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询合约
	///////////////////////////////////////////////////////////////////////////////////////////////
	int res=this->GetCTraderSpi()->ResetSemInstrument();

	if(res){//初始化信号量失败

	perror("Trader::CheckSqlInstrument Semaphore initialization failed\n");
	exit(EXIT_FAILURE);
	}
	// 定义调用API的数据结构
	CThostFtdcQryInstrumentField instrumentField;
	// 确保没有初始化的数据不会被访问
	memset(&instrumentField,0,sizeof(instrumentField));
	// 为调用结构题设置参数信息
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(instrumentField.InstrumentID,"");
	///交易所代码 TThostFtdcExchangeIDType char[9]
	strcpy(instrumentField.ExchangeID,"");
	///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
	strcpy(instrumentField.ExchangeInstID,"");
	///产品代码 TThostFtdcInstrumentIDType char[31]
	strcpy(instrumentField.ProductID,"");

	// 调用API,并等待响应函数返回
	int result_insf = this->GetCThostFtdcTraderApi()->ReqQryInstrument(&instrumentField,this->UpdateRequestID());

	if (result_insf==0)			{
//			printf("Debug: ReqQryInstrument successed！  :\n");
		printf("Debug: ReqQryInstrument successed！ \n");
		printf("sem_wait(&sem_instrument)  --->>>> \n");
		int ret=this->GetCTraderSpi()->LongTimeWaitSem(this->GetCTraderSpi()->GetSemInstrument());
		printf("sem_wait(&sem_instrument)  <<<<<<---- \n");
	}
	else					{
		printf("Debug: ReqQryInstrument failed！  :\n");

		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (result_insf==-1){
			printf("Trader::ReqQryInstrument failed！ [-1] 网络连接失败!!! \n");
		}
		else if(result_insf==-2){
			printf("Trader::ReqQryInstrument failed！ [-2] 未处理请求超过许可数!!! \n");
			this->SetFlagTradeLimit(true);
		}
		else if(result_insf==-3){
			printf("Trader::ReqQryInstrument failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("Trader::ReqQryInstrument failed！ [%d] 未知故障!!! \n",result_insf);
		}

	}

}
void Trader::CheckDepMarket()
{

	char logbuf[1024];
	int res=this->GetCTraderSpi()->ResetSemDepthMarket();

	if(res){//初始化信号量失败

	perror("CheckDepMarket Semaphore initialization failed\n");
	exit(EXIT_FAILURE);
	}

	sprintf(logbuf,"<<<<<<-CheckDepMarket--- \n");
	this->GetLogUtil()->WriteLog(logbuf);
	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询合约tick行情截面
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcQryDepthMarketDataField qryDepthMarketDataField;

	// 确保没有初始化的数据不会被访问
	memset(&qryDepthMarketDataField,0,sizeof(qryDepthMarketDataField));
	// 为调用结构题设置参数信息
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(qryDepthMarketDataField.InstrumentID,"");
	///交易所代码 TThostFtdcExchangeIDType char[9]
	//strcpy(qryDepthMarketDataField.ExchangeID,"");
	///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
	//strcpy(qryDepthMarketDataField.ExchangeInstID,"");
	///产品代码 TThostFtdcInstrumentIDType char[31]
//		strcpy(qryDepthMarketDataField.ProductID,"");
	sleep(3);
	// 调用API,并等待响应函数返回
	int result_depthmarket = this->GetCThostFtdcTraderApi()->ReqQryDepthMarketData(&qryDepthMarketDataField,this->UpdateRequestID());

	if (result_depthmarket==0)	{

		sprintf(logbuf,"Debug: ReqQryDepthMarketData successed！  :\n");
		this->GetLogUtil()->WriteLog(logbuf);

		printf("sem_wait(&sem_depthmarketdata)   --->>>>\n");
		int ret=this->GetCTraderSpi()->LongTimeWaitSem(this->GetCTraderSpi()->GetSemDepthMarket());
		printf("sem_wait(&sem_depthmarketdata)   <<<<<<---- \n");
	}
	else			{
		sprintf(logbuf,"Debug: ReqQryDepthMarketData failed！  :\n");
		this->GetLogUtil()->WriteLog(logbuf);
	}

}

void Trader::CheckSqlInstrument()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询合约
	///////////////////////////////////////////////////////////////////////////////////////////////
	int res=this->GetCTraderSpi()->ResetSemInstrument();

	if(res){//初始化信号量失败

	perror("Trader::CheckSqlInstrument Semaphore initialization failed\n");
	exit(EXIT_FAILURE);
	}

	// 清空基础合约数据
	MysqlInstrument mysqlinstrument;
	int count =mysqlinstrument.Count_AllLists();
bool checkinstrument=false;

	if(count>0){
		std::string update = mysqlinstrument.GetLastLocalUpdateTime();
		InstrumentData insdata=mysqlinstrument.FindLastExpireDateInstrument();


		printf("<<<<<<CheckSqlInstrument---- 最近过期合约%s   %s \n",insdata.InstrumentID,insdata.ExpireDate);
		DateUtil dateutil;
	//	time_t lastclosetime_t = dateutil.GetLastCloseTime();
	//	time_t t_update =dateutil.ConvertSqlTimetoTimet (update.c_str());

		std::string tradingday_sql =dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());
		time_t t_tradingday = dateutil.ConvertSqlTimetoTimet(tradingday_sql.c_str());

		std::string expiredate_sqltime =  dateutil.ConvertDatetoSqlTime(insdata.ExpireDate);
		time_t t_expiredate = dateutil.ConvertSqlTimetoTimet(expiredate_sqltime.c_str()) ;
		if (t_expiredate < t_tradingday  ){
			checkinstrument=true;
			printf("<<<<<<CheckSqlInstrument  合约更新中!!!-----> 到期日期[%s]:%ld   交易日[%s]:%ld :\n",
					expiredate_sqltime.c_str(),t_expiredate,tradingday_sql.c_str(),t_tradingday);
		}
		else{
			printf("<<<<<<CheckSqlInstrument  !!!本次不更新Instrument表!!! -----> 到期日期[%s]:%ld   交易日[%s]:%ld :\n",
					expiredate_sqltime.c_str(),t_expiredate,tradingday_sql.c_str(),t_tradingday);
		}
	}
	else{
		checkinstrument=true;
		printf("<<<<<<CheckSqlInstrument  合约更新中!!!----->  数据表instrument 为空!!! \n");
	}


	if (checkinstrument )	{

		// 清空基础合约数据

		mysqlinstrument.DeleteAll();

		// 定义调用API的数据结构
		CThostFtdcQryInstrumentField instrumentField;
		// 确保没有初始化的数据不会被访问
		memset(&instrumentField,0,sizeof(instrumentField));
		// 为调用结构题设置参数信息
		///合约代码 TThostFtdcInstrumentIDType char[31]
		strcpy(instrumentField.InstrumentID,"");
		///交易所代码 TThostFtdcExchangeIDType char[9]
		strcpy(instrumentField.ExchangeID,"");
		///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
		strcpy(instrumentField.ExchangeInstID,"");
		///产品代码 TThostFtdcInstrumentIDType char[31]
		strcpy(instrumentField.ProductID,"");


		sleep(1);
		// 调用API,并等待响应函数返回
		int result_insf = this->GetCThostFtdcTraderApi()->ReqQryInstrument(&instrumentField,this->UpdateRequestID());

		if (result_insf==0)			{
//			printf("Debug: ReqQryInstrument successed！  :\n");
			printf("Debug: ReqQryInstrument successed！ \n");
			printf("sem_wait(&sem_instrument)  --->>>> \n");
			int ret=this->GetCTraderSpi()->LongTimeWaitSem(this->GetCTraderSpi()->GetSemInstrument());
			printf("sem_wait(&sem_instrument)  <<<<<<---- \n");
		}
		else					{
			printf("Debug: ReqQryInstrument failed！  :\n");

			//-1，表示网络连接失败；
			//-2，表示未处理请求超过许可数；
			//-3，表示每秒发送请求数超过许可数。

			if (result_insf==-1){
				printf("Trader::ReqQryInstrument failed！ [-1] 网络连接失败!!! \n");
			}
			else if(result_insf==-2){
				printf("Trader::ReqQryInstrument failed！ [-2] 未处理请求超过许可数!!! \n");
				this->SetFlagTradeLimit(true);
			}
			else if(result_insf==-3){
				printf("Trader::ReqQryInstrument failed！ [-3] 每秒发送请求数超过许可数!!! \n");
			}
			else{
				printf("Trader::ReqQryInstrument failed！ [%d] 未知故障!!! \n",result_insf);
			}

		}

	}
	else{
//		printf("<<<<<<CheckSqlInstrument  !!!不更新!!! \n");
		this->CheckInstrument();
	}
}
void Trader::CheckSqlProduct()
{
	MysqlProduct mysqlproduct;
	MysqlInstrument mysqlinstrument;
	vector<InstrumentData>  list;

	// 临时代码配合2019-03-29 日大连交易所部分合约增加夜盘
//	if(mysqlproduct.Exist_Table()){
//		int c = mysqlproduct.Count_AllLists();
//		int n=133;
//		if(!mysqlproduct.Exist_DatabyProductID("ss")){
//			printf("CheckSqlProduct----->清空表--->合约数[%d] 少于 %d \n",c,n);
//			mysqlproduct.DropTable();
//		}
//
//	}

	if(!mysqlproduct.Exist_Table()){
			mysqlproduct.CreateTable();
	}

	CodeUtil codeutil;
	int count = mysqlproduct.Count_AllLists();

	if(count==0)	{
		mysqlinstrument.Find_AllList(list);
		printf("CheckSqlProduct----->更新中--->合约数[%ld] ---->\n",list.size());

		for (auto &item:list)		{
			if(!mysqlproduct.Exist_DatabyProductID(item.ProductID))			{
				ProductData productclassinfo;
				strcpy(productclassinfo.ProductID,item.ProductID);
				strcpy(productclassinfo.ExchangeID,item.ExchangeID);

				strcpy(productclassinfo.Section,codeutil.GetSectionByProductid(item.ProductID).c_str());
				strcpy(productclassinfo.DayNight,codeutil.GetDayNightByProductid(item.ProductID).c_str());
//				printf("	CheckSqlProduct----->更新中--->%s  %s   %s   %s ---->\n",productclassinfo.ProductID,
//						productclassinfo.ExchangeID,productclassinfo.Section,productclassinfo.DayNight);
				mysqlproduct.Insert(productclassinfo);
			}
		}
		vector<InstrumentData>().swap(list);
	}
}

void Trader::CheckSqlDepMarket()
{
//	TradingDayData tradingdaydata=this->GetTradingDayData();
	char logbuf[1024];

	//清空深度行情数据
	MysqlDepthMarket mysqldepthmarket;

	int res=this->GetCTraderSpi()->ResetSemDepthMarket();

	if(res){//初始化信号量失败

	perror("CheckSqlInstrument Semaphore initialization failed\n");
	exit(EXIT_FAILURE);
	}

	bool check_depth=false;


	int count = mysqldepthmarket.Count_AllLists();
	if(count<400){
		printf("<<<<<<-CheckSqlDepMarket---合约信息不足400   [%d]更新数据!!!   \n",count);
		mysqldepthmarket.DeleteAll();
		count=0;
		check_depth=true;
	}
	else{

			std::string tradingday = mysqldepthmarket.GetOldestTradingDay();

			if(strcmp(this->GetTradingDay().c_str(),tradingday.c_str())!=0)	{

				printf("<<<<<<-CheckSqlDepMarket---交易日不同,更新数据!!!  maincontract交易日:%s   交易所交易日:%s \n",
						tradingday.c_str(),this->GetTradingDay().c_str());
				mysqldepthmarket.DeleteAll();
				check_depth=true;
			}
			else {

				std::string update = mysqldepthmarket.GetLastLocalUpdateTime();
				printf("<<<<<<-CheckSqlDepMarket--- 比对行情信息更新日期  LocalUpdateTime:%s  \n",update.c_str());
				DateUtil dateutil;
				time_t lastclosetime_t = dateutil.GetLastCloseTime();
				time_t t_localupdate =dateutil.ConvertSqlTimetoTimet (update.c_str());

				if (t_localupdate < lastclosetime_t  )	{
					printf("<<<<<<-CheckSqlDepMarket--- 行情数据未更新,更新数据!!! time_t  t_update:%s [%ld]   < lastclosetime_t:%s [%ld] :\n",
					dateutil.ConvertTimetoSqlString(t_localupdate).c_str(),t_localupdate,dateutil.ConvertTimetoSqlString(lastclosetime_t).c_str(),lastclosetime_t);
					mysqldepthmarket.DeleteAll();
					check_depth=true;
				}
				else{
					printf("<<<<<<-CheckSqlDepMarket---合约条目数[%d]------>>>>>本次不更新mysqldepthmarket数据表!!!   \n",count);
				}

			}
	}

	if (check_depth)	{

		sprintf(logbuf,"<<<<<<-CheckSqlDepMarket--- \n");
		this->GetLogUtil()->WriteLog(logbuf);
		////////////////////////////////////////////////////////////////////////////////////////////////
		///请求查询合约tick行情截面
		///////////////////////////////////////////////////////////////////////////////////////////////
		// 定义调用API的数据结构
		CThostFtdcQryDepthMarketDataField qryDepthMarketDataField;

		// 确保没有初始化的数据不会被访问
		memset(&qryDepthMarketDataField,0,sizeof(qryDepthMarketDataField));
		// 为调用结构题设置参数信息
		///合约代码 TThostFtdcInstrumentIDType char[31]
		strcpy(qryDepthMarketDataField.InstrumentID,"");
		///交易所代码 TThostFtdcExchangeIDType char[9]
		//strcpy(qryDepthMarketDataField.ExchangeID,"");
		///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
		//strcpy(qryDepthMarketDataField.ExchangeInstID,"");
		///产品代码 TThostFtdcInstrumentIDType char[31]
//		strcpy(qryDepthMarketDataField.ProductID,"");
		sleep(3);
		// 调用API,并等待响应函数返回
		int result_depthmarket = this->GetCThostFtdcTraderApi()->ReqQryDepthMarketData(&qryDepthMarketDataField,this->UpdateRequestID());

		if (result_depthmarket==0)	{

			sprintf(logbuf,"Debug: ReqQryDepthMarketData successed！  :\n");
			this->GetLogUtil()->WriteLog(logbuf);

			printf("sem_wait(&sem_depthmarketdata)   --->>>>\n");
			int ret=this->GetCTraderSpi()->LongTimeWaitSem(this->GetCTraderSpi()->GetSemDepthMarket());
			printf("sem_wait(&sem_depthmarketdata)   <<<<<<---- \n");
		}
		else			{
			sprintf(logbuf,"Debug: ReqQryDepthMarketData failed！  :\n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}
	else{
		this->CheckDepMarket();
	}
}
//收盘后获取行情，用于产生当天的日线数据
void Trader::CheckSqlDepMarket_EndTime()
{
	char logbuf[1024];

	//清空深度行情数据
	MysqlDepthMarket mysqldepthmarket("depthmarket_endtime");

	if(!mysqldepthmarket.Exist_Table()){
		mysqldepthmarket.CreateTable();
		printf("	CheckSqlDepMarket_EndTime:创建数据表  MysqlDepthMarket-->depthmarket_endtime  \n");
	}
	bool check_depth=false;

	int count = mysqldepthmarket.Count_AllLists();
	if(count==0){
		printf("<<<<<<-CheckSqlDepMarket_EndTime---收盘后   [%d]更新depthmarket_endtime数据表!!!   \n",count);
		mysqldepthmarket.DeleteAll();
		check_depth=true;
	}
	else{
		std::string update = mysqldepthmarket.GetLastLocalUpdateTime();
		printf("<<<<<<-CheckSqlDepMarket_EndTime--- 比对行情信息更新日期  LocalUpdateTime:%s  \n",update.c_str());
		DateUtil dateutil;
		time_t lastclosetime_t = dateutil.GetLastCloseTime();
		time_t t_localupdate =dateutil.ConvertSqlTimetoTimet (update.c_str());

		if (t_localupdate < lastclosetime_t  )	{
			printf("<<<<<<-CheckSqlDepMarket_EndTime--- 行情数据未更新,更新数据!!! time_t  t_update:%s [%ld]   < lastclosetime_t:%s [%ld] :\n",
			dateutil.ConvertTimetoSqlString(t_localupdate).c_str(),t_localupdate,dateutil.ConvertTimetoSqlString(lastclosetime_t).c_str(),lastclosetime_t);
			mysqldepthmarket.DeleteAll();
			check_depth=true;
		}
		else{
			printf("<<<<<<-CheckSqlDepMarket_EndTime---合约条目数[%d]------>>>>>本次不更新mysqldepthmarket数据表!!!   \n",count);
		}
	}
	if (check_depth)	{

		int res=this->GetCTraderSpi()->ResetSemDepthMarket();

		if(res){//初始化信号量失败

		perror("CheckSqlInstrument Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}
	//		printf("<<<<<<-CheckSqlDepMarket_EndTime--- 更新行情数据 time_t  t_update:%ld  < lastclosetime_t:%ld :\n",t_localupdate,lastclosetime_t);

		////////////////////////////////////////////////////////////////////////////////////////////////
		///请求查询合约tick行情截面
		///////////////////////////////////////////////////////////////////////////////////////////////
		// 定义调用API的数据结构
		CThostFtdcQryDepthMarketDataField qryDepthMarketDataField;

		// 确保没有初始化的数据不会被访问
		memset(&qryDepthMarketDataField,0,sizeof(qryDepthMarketDataField));
		// 为调用结构题设置参数信息
		///合约代码 TThostFtdcInstrumentIDType char[31]
		strcpy(qryDepthMarketDataField.InstrumentID,"");
		///交易所代码 TThostFtdcExchangeIDType char[9]
		//strcpy(qryDepthMarketDataField.ExchangeID,"");
		///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
		//strcpy(qryDepthMarketDataField.ExchangeInstID,"");
		///产品代码 TThostFtdcInstrumentIDType char[31]
	//		strcpy(qryDepthMarketDataField.ProductID,"");
		sleep(3);
		// 调用API,并等待响应函数返回
		int result_depthmarket = this->GetCThostFtdcTraderApi()->ReqQryDepthMarketData(&qryDepthMarketDataField,this->UpdateRequestID());

		if (result_depthmarket==0)	{
			sprintf(logbuf,"Debug: ReqQryDepthMarketData successed！  :\n");
			this->GetLogUtil()->WriteLog(logbuf);

			printf("sem_wait(&sem_depthmarketdata)   --->>>>\n");
			int ret=this->GetCTraderSpi()->LongTimeWaitSem(this->GetCTraderSpi()->GetSemDepthMarket());
			printf("sem_wait(&sem_depthmarketdata)   <<<<<<---- \n");
		}
		else			{
			sprintf(logbuf,"Debug: ReqQryDepthMarketData failed！  :\n");
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}



}
void Trader::UpdateSqlDepMarket()
{
	//清空深度行情数据
	MysqlDepthMarket mysqldepthmarket;

	int res=this->GetCTraderSpi()->ResetSemDepthMarket();

	if(res)//初始化信号量失败
		{
		perror("CheckSqlInstrument Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
		}

	{
//		mysqldepthmarket.DeleteAll();

		////////////////////////////////////////////////////////////////////////////////////////////////
		///请求查询合约tick行情截面
		///////////////////////////////////////////////////////////////////////////////////////////////
		// 定义调用API的数据结构
		CThostFtdcQryDepthMarketDataField qryDepthMarketDataField;

		// 确保没有初始化的数据不会被访问
		memset(&qryDepthMarketDataField,0,sizeof(qryDepthMarketDataField));
		// 为调用结构题设置参数信息
		///合约代码 TThostFtdcInstrumentIDType char[31]
		strcpy(qryDepthMarketDataField.InstrumentID,"");
		///交易所代码 TThostFtdcExchangeIDType char[9]
		//strcpy(qryDepthMarketDataField.ExchangeID,"");
		///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
		//strcpy(qryDepthMarketDataField.ExchangeInstID,"");
		///产品代码 TThostFtdcInstrumentIDType char[31]
		//strcpy(qryDepthMarketDataField.ProductID,"");

		sleep(1);
		// 调用API,并等待响应函数返回
		int result_depthmarket = this->GetCThostFtdcTraderApi()->ReqQryDepthMarketData(&qryDepthMarketDataField,this->UpdateRequestID());

		if (result_depthmarket==0)
			{
				printf("Debug: ReqQryDepthMarketData successed！  :\n");

			}
			else
			{
				printf("Debug: ReqQryDepthMarketData failed！  :\n");
			}

		printf("<<<<<<---- sem_wait(&sem_depthmarketdata)   :\n");
////		sem_wait(&sem_depthmarketdata);
//
//
//		struct timeval now;
//		struct timespec outtime;
//
//		gettimeofday(&now, NULL);
//		cout << now.tv_sec << " " << (now.tv_usec) << "\n";
//		DateUtil dateutil;
//		dateutil.timeraddMS(&now, 120000);   // 1000 ms   = 1s
//		outtime.tv_sec = now.tv_sec;
//		outtime.tv_nsec = now.tv_usec * 1000;
//		int ret;
//		ret = sem_timedwait(&sem_depthmarketdata,&outtime);
		int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemDepthMarket());
		printf("sem_wait(&sem_depthmarketdata)   --->>>>:\n");
	}



}
void Trader::ReqQrySingleInstrument(const char* pinstrumentid)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	///请求查询单个合约
	///////////////////////////////////////////////////////////////////////////////////////////////
	int res=this->GetCTraderSpi()->ResetSemInstrument();

	if(res)//初始化信号量失败
	    {
	    perror("Semaphore initialization failed\n");
	    exit(EXIT_FAILURE);
	    }

		// 定义调用API的数据结构
		CThostFtdcQryInstrumentField instrumentField;
		// 确保没有初始化的数据不会被访问
		memset(&instrumentField,0,sizeof(instrumentField));
		// 为调用结构题设置参数信息
		///合约代码 TThostFtdcInstrumentIDType char[31]
		strcpy(instrumentField.InstrumentID,pinstrumentid);
		///交易所代码 TThostFtdcExchangeIDType char[9]
		strcpy(instrumentField.ExchangeID,"");
		///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
		strcpy(instrumentField.ExchangeInstID,"");
		///产品代码 TThostFtdcInstrumentIDType char[31]
		strcpy(instrumentField.ProductID,"");

		sleep(1);
		// 调用API,并等待响应函数返回
		int result_insf = this->GetCThostFtdcTraderApi()->ReqQryInstrument(&instrumentField,this->UpdateRequestID());

		if (result_insf==0)
				{
					printf("Debug: ReqQryInstrument successed！  :\n");

				}
				else
				{
					printf("Debug: ReqQryInstrument failed！  :\n");
				}

		printf("<<<<<<---- sem_wait(&sem_instrument)   :\n");
////			sem_wait(&sem_instrument);
//
//		struct timeval now;
//		struct timespec outtime;
//
//		gettimeofday(&now, NULL);
//		cout << now.tv_sec << " " << (now.tv_usec) << "\n";
//		DateUtil dateutil;
//		dateutil.timeraddMS(&now, 120000);   // 1000 ms   = 1s
//		outtime.tv_sec = now.tv_sec;
//		outtime.tv_nsec = now.tv_usec * 1000;
//		int ret;
//		ret = sem_timedwait(&sem_instrument,&outtime);
int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemInstrument());
		printf("sem_wait(&sem_instrument)   --->>>>:\n");

}

void Trader::CheckSettleInfoConfirm()
{
	char logbuf[1024];


	this->ReqSettlementInfoConfirm ();
//	this->ReqQrySettlementInfoDefault();

	 printf("Trader::CheckSettleInfoConfirm   --->>>>:ReqQrySettlementInfoConfirm\n");
	 sprintf(logbuf,"Trader::CheckSettleInfoConfirm   --->>>>:ReqQrySettlementInfoConfirm\n");
	 this->GetLogUtil()->WriteLog(logbuf);
	 this->ReqQrySettlementInfoConfirm();
//////////////////////////

//		printf("Trader::CheckSettleInfoConfirm:   结算信息未确认,请求结算信息--->>>>:ReqQrySettlementInfoDefault\n");
//				this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 结算信息未确认,请求结算信息！ReqQrySettlementInfoDefault");

//
//	//			printf("Trader::CheckSettleInfoConfirm:   结算信息未确认,请求结算信息--->>>>:ReqQrySettlementInfo\n");
//	//			this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 结算信息未确认,请求结算信息！ReqQrySettlementInfo");
//	//			this->ReqQrySettlementInfo();
//				 sleep(1);
//				this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 请求结算信息确认！");
//				printf("Trader::CheckSettleInfoConfirm:   自动交易程序--->结算信息未确认,请求结算信息确认--->>>>:ReqSettlementInfoConfirm\n");
//				this->ReqSettlementInfoConfirm ();

/////////////////////////////
	 sleep(1);
	MysqlTradingDay mysqltradingday(this->GetUserID().c_str());

	if(mysqltradingday.Exist_Table()){

		if(mysqltradingday.Exist_TradingDay(this->GetTradingDay().c_str())){

			TradingDayData last;
			last= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());

			if(!last.SettleInfoConfirmStatus || strcmp(last.TradingDay,last.ConfirmDate)!=0){

				printf("Trader::CheckSettleInfoConfirm:   结算信息未确认,请求结算信息--->>>>:ReqQrySettlementInfoDefault\n");
				this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 结算信息未确认,请求结算信息！ReqQrySettlementInfoDefault");
				this->ReqQrySettlementInfoDefault();

	//			printf("Trader::CheckSettleInfoConfirm:   结算信息未确认,请求结算信息--->>>>:ReqQrySettlementInfo\n");
	//			this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 结算信息未确认,请求结算信息！ReqQrySettlementInfo");
	//			this->ReqQrySettlementInfo();
				 sleep(1);
				this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 请求结算信息确认！");
				printf("Trader::CheckSettleInfoConfirm:   自动交易程序--->结算信息未确认,请求结算信息确认--->>>>:ReqSettlementInfoConfirm\n");
				this->ReqSettlementInfoConfirm ();


				this->ReqQrySettlementInfoConfirm();

				this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 自动完成结算信息确认！");
			}
			else{
				printf("Trader::CheckSettleInfoConfirm   --->>>>自动交易程序---> 结算信息已经确认,无须再次发起确认！\n");
				this->GetLogUtil()->WriteLog("Trader::CheckSettleInfoConfirm:  自动交易程序---> 结算信息已经确认,无须再次发起确认！");
			}
		}
	}

}
bool Trader::CheckSettleInfoConfirmbyTrade()
{
	char logbuf[1024];


	bool ret=false;
	MysqlTradingDay mysqltradingday(this->GetUserID().c_str());

/////////////////////
//	printf("TrendTrader::CheckSettleInfoConfirmbyTrade:   请求结算信息--->>>>:ReqQrySettlementInfoDefault\n");
//	this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 请求结算信息ReqQrySettlementInfoDefault！");
//	this->ReqQrySettlementInfoDefault();
//	sleep(1);
//
//	this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 请求结算信息确认！");
//	printf("TrendTrader::CheckSettleInfoConfirmbyTrade:   交易前确认--->结算信息未确认,请求结算信息确认--->>>>:ReqSettlementInfoConfirm\n");
//	this->ReqSettlementInfoConfirm ();
//	sleep(1);
///////////////////////
	if(mysqltradingday.Exist_Table() ){

		if(mysqltradingday.Exist_TradingDay(this->GetTradingDay().c_str())){

			TradingDayData last;
			last= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());

			if(!last.SettleInfoConfirmStatus || strcmp(last.TradingDay,last.ConfirmDate)!=0){

				printf("TrendTrader::CheckSettleInfoConfirmbyTrade:   请求结算信息--->>>>:ReqQrySettlementInfoDefault\n");
				this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 请求结算信息ReqQrySettlementInfoDefault！");
				this->ReqQrySettlementInfoDefault();
				sleep(1);

				this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 请求结算信息确认！");
				printf("TrendTrader::CheckSettleInfoConfirmbyTrade:   交易前确认--->结算信息未确认,请求结算信息确认--->>>>:ReqSettlementInfoConfirm\n");
				this->ReqSettlementInfoConfirm ();
				sleep(1);

				 printf("TrendTrader::CheckSettleInfoConfirmbyTrade   --->>>>交易前确认:ReqQrySettlementInfoConfirm\n");
				 sprintf(logbuf,"TrendTrader::CheckSettleInfoConfirmbyTrade   --->>>>交易前确认:ReqQrySettlementInfoConfirm\n");
				 this->GetLogUtil()->WriteLog(logbuf);
				 this->ReqQrySettlementInfoConfirm();
				 sleep(1);

				TradingDayData last_1;
				last_1= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());
				if(!last_1.SettleInfoConfirmStatus || strcmp(last_1.TradingDay,last_1.ConfirmDate)!=0){

					this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 结算信息未确认,请继续关注！");

				}
				else{
					ret=true;
					this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 自动完成结算信息确认！");
				}

			}
			else{
				printf("TrendTrader::CheckSettleInfoConfirmbyTrade   --->>>>交易前确认---> 结算信息已经确认,无须再次发起确认！\n");
	//			this->GetLogUtil()->WriteLog("TrendTrader::CheckSettleInfoConfirmbyTrade:  交易前确认---> 结算信息已经确认,无须再次发起确认！");

				ret =true;
			}
		}


	}

	return ret;

}
void Trader::CheckSqlMaincontracont(){

//	TradingDayData tradingdaydata=this->GetTradingDayData();
	MysqlDepthMarket mysqldepthmarket;
	int depthcount=mysqldepthmarket.Count_AllLists();
	if(depthcount==0){
		printf("<<<<<<-CheckSqlMaincontracont--- depthmarket 数据表为空 再次请求一次! \n");
		CheckSqlDepMarket();
		depthcount=mysqldepthmarket.Count_AllLists();
		printf("<<<<<<-CheckSqlMaincontracont--- depthmarket 数据表为空 再次请求后 --->%d ! \n",depthcount);
	}

	MysqlMaincontract mysqlmaincontract;
	std::string update = mysqlmaincontract.GetLastLocalUpdateTime();
	printf("<<<<<<-CheckSqlMaincontracont--- 比对行情信息更新日期  LocalUpdateTime:%s  \n",update.c_str());
	DateUtil dateutil;
	time_t lastclosetime_t = dateutil.GetLastCloseTime();
	time_t t_localupdate =dateutil.ConvertSqlTimetoTimet (update.c_str());

	std::string tradingday = mysqlmaincontract.GetOldestTradingDay();

	if(strcmp(this->GetTradingDay().c_str(),tradingday.c_str())!=0)	{
		printf("<<<<<<-CheckSqlMaincontracont---交易日不同  maincontract交易日:%s   交易所交易日:%s \n",
				tradingday.c_str(),this->GetTradingDay().c_str());
		mysqlmaincontract.DeleteAll();
	}

	if (t_localupdate < lastclosetime_t)	{
		printf("<<<<<<-CheckSqlMaincontracont--- 主力合约更新日期为上一交易日之前  time_t  %s -->t_update:%ld   %s-->lastclosetime_t:%ld :\n",
				dateutil.ConvertTimetoSqlString(t_localupdate).c_str(),t_localupdate,dateutil.ConvertTimetoSqlString(lastclosetime_t).c_str(),lastclosetime_t);
		mysqlmaincontract.DeleteAll();
	}
	int count = mysqlmaincontract.Count_AllLists();
	printf("<<<<<<-CheckSqlMaincontracont   0000000\n");
	if( (t_localupdate < lastclosetime_t ||strcmp(this->GetTradingDay().c_str(),tradingday.c_str())!=0 || count==0 ) && (depthcount!=0) )	{

		MysqlProduct mysqlproduct;
		vector<ProductData>  list;

		mysqlproduct.Find_AllList(list);
		printf("CheckSqlMaincontracont----->更新主力合约 合约品种[%ld] ---->\n",list.size());

		MysqlDepthMarket mysqldepthmarket;
		MysqlInstrument mysqlinstrument;

		MysqlDayLine mysqldayline;
		for (auto &item:list)		{
			InstrumentData insopi;
			InstrumentData insvol;
			DepthMarketData maxopi;
			DepthMarketData maxvol;
			MainContractData maincontractdata;
			int main_ym;

			printf("CheckSqlMaincontracont----->合约品种[%s] ---->\n",item.ProductID);
			maxopi = mysqldepthmarket.Find_DatabyMaxOPI(item.ProductID);
			maxvol = mysqldepthmarket.Find_DatabyMaxVolume(item.ProductID);

			insopi = mysqlinstrument.Find_DatabyInstrumentID(maxopi.InstrumentID);
			insvol = mysqlinstrument.Find_DatabyInstrumentID(maxvol.InstrumentID);


			HxDayLineData daylinedata_opi =mysqldayline.Find_LastDayLine(maxopi.InstrumentID);
			HxDayLineData daylinedata_vol =mysqldayline.Find_LastDayLine(maxvol.InstrumentID);

			double sedimentary_opi ;
			sedimentary_opi = (maxopi.OpenInterest*insopi.VolumeMultiple*insopi.LongMarginRatio*maxopi.LastPrice)/(100000000.0);
			double sedimentary_vol;
			sedimentary_vol = (maxvol.OpenInterest*insopi.VolumeMultiple*insopi.LongMarginRatio*maxvol.LastPrice)/(100000000.0);

			if (strcmp(maxopi.InstrumentID,maxvol.InstrumentID)==0)			{

//					printf ("------maxopi[%d] =   maxvol[%d]   相同------[%s] ----沉淀资金[%.3f]\n",maxopi.OpenInterest,maxvol.Volume,maxopi.InstrumentID,sedimentary_opi);

					printf ("CheckSqlMaincontracont: maxopi[%s]  opi[%d] vol[%d]   相同------沉淀资金[%.3f]\n",maxopi.InstrumentID,maxopi.OpenInterest,maxopi.Volume,sedimentary_opi);
//					printf ("maxvol[%s]  opi[%d] vol[%d]   相同------沉淀资金[%.3f]\n",maxvol.InstrumentID,maxvol.OpenInterest,maxvol.Volume,sedimentary_vol);

					char str_ym[10];
					string pattern("([0-9]+)"); // ([a-z]+):([0-9]+) also works here

					boost::regex reg(pattern);    //查找字符串里的数字
					boost::cmatch mat;
					if(boost::regex_search(maxopi.InstrumentID, mat, reg))
					{
//										        cout << "searched:" << mat[0] << endl;
//								printf("匹配主力合约的年份和月份--mat-[%d]--->%s \n", i,mat[0].str().c_str());
					}
					strcpy(str_ym,mat[0].str().c_str());
					main_ym=atoi(str_ym);

//					CreatMainContractData(&maincontractdata,&insopi,&maxopi,&item);

			}
			else{

					printf ("CheckSqlMaincontracont: maxopi[%s]  opi[%d] vol[%d]   不同------沉淀资金[%.3f]\n",maxopi.InstrumentID,maxopi.OpenInterest,maxopi.Volume,sedimentary_opi);
					printf ("CheckSqlMaincontracont: maxvol[%s]  opi[%d] vol[%d]   不同------沉淀资金[%.3f]\n",maxvol.InstrumentID,maxvol.OpenInterest,maxvol.Volume,sedimentary_vol);

					char str_ym_opi[10];
					string pattern("([0-9]+)"); // ([a-z]+):([0-9]+) also works here

					boost::regex reg(pattern);    //查找字符串里的数字
					boost::cmatch mat_opi;
					if(boost::regex_search(maxopi.InstrumentID, mat_opi, reg))					{
//										        cout << "searched:" << mat[0] << endl;
//								printf("匹配主力合约的年份和月份--mat-[%d]--->%s \n", i,mat[0].str().c_str());
					}
					strcpy(str_ym_opi,mat_opi[0].str().c_str());
					int num_opi=atoi(str_ym_opi);

					char str_ym_vol[10];
//					boost::regex reg(pattern);    //查找字符串里的数字
					boost::cmatch mat_vol;
					if(boost::regex_search(maxvol.InstrumentID, mat_vol, reg))					{
	//										        cout << "searched:" << mat[0] << endl;
	//								printf("匹配主力合约的年份和月份--mat-[%d]--->%s \n", i,mat[0].str().c_str());
					}
					strcpy(str_ym_vol,mat_vol[0].str().c_str());
					int num_vol=atoi(str_ym_vol);

					if(num_opi>num_vol)		{
						printf ("CheckSqlMaincontracont: 主力合约选择远期合约----[%s]  maxopi 合约为远期合约------> \n",maxopi.InstrumentID);
						main_ym=num_opi;

//						CreatMainContractData(&maincontractdata,&insopi,&maxopi,&item);
					}
					else if(num_opi<num_vol)	{
						printf ("CheckSqlMaincontracont: 主力合约选择远期合约----[%s]  maxvol 合约为远期合约------> \n",maxvol.InstrumentID);
						main_ym=num_vol;
//						CreatMainContractData(&maincontractdata,&insvol,&maxvol,&item);
					}

			}


			vector<DepthMarketData>  depthlists;
			mysqldepthmarket.Find_ListsbyProductID(depthlists,item.ProductID);
			bool flag_find=false;
			for (auto &depthdata:depthlists){

				char str_ym[10];
				string pattern("([0-9]+)"); // ([a-z]+):([0-9]+) also works here

				boost::regex reg(pattern);    //查找字符串里的数字
				boost::cmatch mat;
				if(boost::regex_search(depthdata.InstrumentID, mat, reg))				{
//										        cout << "searched:" << mat[0] << endl;
//								printf("匹配主力合约的年份和月份--mat-[%d]--->%s \n", i,mat[0].str().c_str());
				}
				strcpy(str_ym,mat[0].str().c_str());
				int num=atoi(str_ym);
				if(num>main_ym&& !flag_find)	{
					flag_find=true;
					printf ("CheckSqlMaincontracont: ----[%s]  合约为次月主力合约------> \n",depthdata.InstrumentID);

					InstrumentData ins = mysqlinstrument.Find_DatabyInstrumentID(depthdata.InstrumentID);

					strcpy(maincontractdata.InstrumentID_next,depthdata.InstrumentID);
					maincontractdata.OpenInterest_next = depthdata.OpenInterest;
					maincontractdata.Volume_next = depthdata.Volume;
					maincontractdata.UpperLimitPrice_next =depthdata.UpperLimitPrice;
					maincontractdata.LowerLimitPrice_next =depthdata.LowerLimitPrice;
					maincontractdata.LastPrice_next =  depthdata.LastPrice;

					int openinterest =depthdata.OpenInterest;
					int volumemultiple =ins.VolumeMultiple;
					int pricetick =ins.PriceTick;
					double longmarginratio = ins.LongMarginRatio;
					double lastprice = depthdata.LastPrice;


					double sedimentary =(double)openinterest*(double)volumemultiple*longmarginratio*lastprice/100000000.0;
					maincontractdata.Sedimentary_next  = sedimentary;
				}


			}
			vector<DepthMarketData>().swap(depthlists);

			bool v = strcmp(item.ExchangeID,"INE")!=0  &&  strcmp(item.ExchangeID,"CFFEX")!=0;
			if(v && maincontractdata.Sedimentary>2 && !mysqlmaincontract.Exist_DatabyProductID(item.ProductID) && strcmp(maincontractdata.ProductID,item.ProductID)==0)
			{
				printf("CheckSqlMaincontracont: 不存在主力合约[%s] [%s]--- 插入新数据  %s  \n",maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,item.ProductID);

				MysqlAvePrice mysqlaveprice;
				double maxopi;
				maxopi=mysqlaveprice.Get_MaxOpenInterest(maincontractdata.InstrumentID);
				printf("CheckSqlMaincontracont: 主力合约 %s  的历史最高持仓量 maxopi -->  [%.f] \n",maincontractdata.InstrumentID,maxopi);
				if (maxopi>=1)				{
					maincontractdata.MaxOpenInterest = maxopi;
				}
				else				{
					maincontractdata.MaxOpenInterest=0;
				}

				if (maincontractdata.OpenInterest < maxopi/2 || maincontractdata.Sedimentary_next > maincontractdata.Sedimentary*0.7  )				{
					strcpy(maincontractdata.InstrumentID_trade ,maincontractdata.InstrumentID_next);
				}
				else				{
					strcpy(maincontractdata.InstrumentID_trade , maincontractdata.InstrumentID);

				}

				mysqlmaincontract.Insert(maincontractdata);
			}
			else if( strcmp(maincontractdata.ProductID,item.ProductID)!=0)			{
				printf("CheckSqlMaincontracont: 主力合约计算错误 [%s] -->[%s]--- 无法插入新数据  %s  \n",maincontractdata.ProductID,maincontractdata.InstrumentID,item.ProductID);
			}
			else if(maincontractdata.Sedimentary<=2)			{
				printf("CheckSqlMaincontracont: 合约沉淀资金过小 [%s]--- 暂不插入列表  %s  \n",maincontractdata.InstrumentID,item.ProductID);
			}
		}
		vector<ProductData>().swap(list);


	}
	else{
		if(depthcount==0){
			printf("ERROR: CheckSqlMaincontracont ----->depthmarket table is empty !\n");
		}
	}

	printf("<<<<<<-CheckSqlMaincontracont   end\n");
}
void Trader::CheckContract(vector<std::string> &instrumentlist){

	char logbuf[1024];

	MysqlDepthMarket mysqldepthmarket;
	MysqlInstrument mysqlinstrument;
	MysqlCommission mysqlcommission;


	int depthcount=mysqldepthmarket.Count_AllLists();
	if(depthcount==0){
		printf("	Trader::CheckContract--- depthmarket 数据表为空 再次请求一次! \n");
		CheckSqlDepMarket();
		depthcount=mysqldepthmarket.Count_AllLists();
		printf("	Trader::CheckContract--- depthmarket 数据表为空 再次请求后 --->%d ! \n",depthcount);
	}
	vector<DepthMarketData> depthlist;

	mysqldepthmarket.Find_AllList(depthlist);
	for (auto &item:depthlist){

		Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);

		InstrumentData	instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
		double sedimentary = (item.OpenInterest*instrumentdata.VolumeMultiple*commission.Margin*item.LastPrice)/(100000000.0);
		if(sedimentary>0.5){
			instrumentlist.push_back(item.InstrumentID);
			printf("Trader::CheckContract----->合约品种[%s] ---->sed:[%.2f]\n",item.InstrumentID,sedimentary);
		}
	}

vector<DepthMarketData>().swap(depthlist);
	printf("Trader::CheckContract   end\n");
}
void Trader::CheckSqlMaincontracontbyOpenInterest(){

	char logbuf[1024];
	bool check_flag=false;
	MysqlDepthMarket mysqldepthmarket;
	MysqlInstrument mysqlinstrument;
	MysqlCommission mysqlcommission;
	MysqlMaincontract mysqlmaincontract;
	MysqlProduct mysqlproduct;
	MysqlDayLine mysqldayline;

	int depthcount=mysqldepthmarket.Count_AllLists();
	if(depthcount==0){
		printf("	Trader::CheckSqlMaincontracontbyOpenInterest--- depthmarket 数据表为空 再次请求一次! \n");
		CheckSqlDepMarket();
		depthcount=mysqldepthmarket.Count_AllLists();
		printf("	Trader::CheckSqlMaincontracontbyOpenInterest--- depthmarket 数据表为空 再次请求后 --->%d ! \n",depthcount);
	}

	int count = mysqlmaincontract.Count_AllLists();
	if(count<30){
		mysqlmaincontract.DeleteAll();
		check_flag=true;
	}
	else{

		std::string update = mysqlmaincontract.GetLastLocalUpdateTime();
//		printf("Trader::CheckSqlMaincontracontbyOpenInterest--- 比对行情信息更新日期  LocalUpdateTime:%s  \n",update.c_str());
		DateUtil dateutil;
		time_t lastclosetime_t = dateutil.GetLastCloseTime();
		time_t t_localupdate =dateutil.ConvertSqlTimetoTimet (update.c_str());

		std::string tradingday = mysqlmaincontract.GetOldestTradingDay();

		if(strcmp(this->GetTradingDay().c_str(),tradingday.c_str())!=0)	{
			printf("	Trader::CheckSqlMaincontracontbyOpenInterest---交易日不同,更新主力合约表>>>>>>>>  maincontract交易日:%s   交易所交易日:%s \n",
					tradingday.c_str(),this->GetTradingDay().c_str());
			mysqlmaincontract.DeleteAll();
			check_flag=true;
		}
		else {

			if (t_localupdate < lastclosetime_t)	{
				printf("	Trader::CheckSqlMaincontracontbyOpenInterest--- 主力合约更新日期为上一交易日之前,更新主力合约表>>>>>>>>  time_t  %s -->t_update:%ld   %s-->lastclosetime_t:%ld :\n",
						dateutil.ConvertTimetoSqlString(t_localupdate).c_str(),t_localupdate,dateutil.ConvertTimetoSqlString(lastclosetime_t).c_str(),lastclosetime_t);
				mysqlmaincontract.DeleteAll();
				check_flag=true;
			}
			else {
				printf("	Trader::CheckSqlMaincontracontbyOpenInterest--- 本次不更新主力合约表!!!   LocalUpdateTime:%s  \n",update.c_str());
			}
		}

	}

//	printf("Trader::CheckSqlMaincontracontbyOpenInterest   0000000\n");
	if( !check_flag || depthcount==0 )	{
		if(depthcount==0){
			printf("Trader::CheckSqlMaincontracontbyOpenInterest  ERROR ----->depthmarket table is empty !\n");
		}

		if(!check_flag){
			printf("Trader::CheckSqlMaincontracontbyOpenInterest  直接返回 ----->无需更新主力合约表 !\n");
		}
		return;
	}

	vector<ProductData>  list;
	mysqlproduct.Find_AllList(list);
	printf("	Trader::CheckSqlMaincontracontbyOpenInterest----->更新主力合约 合约品种数[%ld] ---->\n",list.size());

	for (auto &item:list)		{
		InstrumentData instrumentdata_maxopi;
		DepthMarketData depthmarketdata_maxopi;
		MainContractData maincontractdata;
//			int main_ym;
		Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);

//			printf("Trader::CheckSqlMaincontracontbyOpenInterest----->合约品种[%s] ---->\n",item.ProductID);
		depthmarketdata_maxopi = mysqldepthmarket.Find_DatabyMaxOPI(item.ProductID);

		instrumentdata_maxopi = mysqlinstrument.Find_DatabyInstrumentID(depthmarketdata_maxopi.InstrumentID);
//			printf("Trader::CheckSqlMaincontracontbyOpenInterest----->合约交易单位[%d] ---->\n",instrumentdata_maxopi.VolumeMultiple);
//		printf("Trader::CheckSqlMaincontracontbyOpenInterest----->合约品种[%s] ---->持仓量最大合约[%s] 合约交易单位[%d] \n",
//							item.ProductID,depthmarketdata_maxopi.InstrumentID,instrumentdata_maxopi.VolumeMultiple);
//		sprintf(logbuf,"Trader::CheckSqlMaincontracontbyOpenInterest----->合约品种[%s] ---->持仓量最大合约[%s] 合约交易单位[%d]",
//										item.ProductID,depthmarketdata_maxopi.InstrumentID,instrumentdata_maxopi.VolumeMultiple);
//		this->GetLogUtil()->WriteLog(logbuf);
		if(depthmarketdata_maxopi.InstrumentID!=NULL){
//				printf("Trader::CheckSqlMaincontracontbyOpenInterest----->字符非空[%s] ---->\n",depthmarketdata_maxopi.InstrumentID);
		}
		else	{
			printf("	Trader::CheckSqlMaincontracontbyOpenInterest----->[%s] ---->Error:合约为空字符！！！ \n",
					depthmarketdata_maxopi.InstrumentID);
			sprintf(logbuf,"Trader::CheckSqlMaincontracontbyOpenInterest----->[%s] ---->Error:合约为空字符！！！ \n",
									depthmarketdata_maxopi.InstrumentID);
			this->GetLogUtil()->WriteLog(logbuf);
		}

		this->CreatMainContractData(&mysqldayline,&maincontractdata,&instrumentdata_maxopi,&depthmarketdata_maxopi,&item,commission.Margin);

		sprintf (logbuf,"	Trader::CheckSqlMaincontracontbyOpenInterest: 持仓量最大合约[%s]  持仓量[%d] 成交量[%d] ------>沉淀资金[%.3f]",
			depthmarketdata_maxopi.InstrumentID,depthmarketdata_maxopi.OpenInterest,depthmarketdata_maxopi.Volume,maincontractdata.Sedimentary);
		this->GetLogUtil()->WriteLog(logbuf);

		bool flag_findnext=this->GetNextContractbyOpeninterest(&maincontractdata,&item);
		if(!flag_findnext){
			flag_findnext=this->GetNextContractbyVolume(&maincontractdata,&item);
			printf("	Trader::CheckSqlMaincontracontbyOpenInterest: 主力合约[%s]  远月合约[%s] 完成计算!--->无法通过持仓量获取远月主力合约,改为成交量获取  %s  \n",
							maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,item.ProductID);
			sprintf(logbuf,"	Trader::CheckSqlMaincontracontbyOpenInterest: 主力合约[%s]  远月合约[%s] 完成计算!--->无法通过持仓量获取远月主力合约,改为成交量获取  %s  \n",
									maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,item.ProductID);
			this->GetLogUtil()->WriteLog(logbuf);
		}

		if(flag_findnext){

//			printf ("	Trader:: %.2f %.2f \n",maincontractdata.LastPrice_next,maincontractdata.Sedimentary_next);
			double sedimentary_next ;
			if(maincontractdata.LastPrice_next<10000000000){

			}
			else{
				HxDayLineData lastdata=mysqldayline.Find_LastDayLine(maincontractdata.InstrumentID_next);
				maincontractdata.LastPrice_next = lastdata.Close;
				sedimentary_next = (maincontractdata.OpenInterest_next*maincontractdata.VolumeMultiple*commission.Margin*lastdata.Close)/(100000000.0);
				printf ("	Trader::CheckSqlMaincontracontbyOpenInterest: 远月合约[%s]  持仓量[%d] 交易单位[%d] 保证金[%.2f%%] 最后交易日收盘价[%.2f]------>沉淀资金[%.3f]\n",
						maincontractdata.InstrumentID_next,maincontractdata.OpenInterest_next,
						maincontractdata.VolumeMultiple,commission.Margin*100,lastdata.Close,sedimentary_next);
				maincontractdata.Sedimentary_next=sedimentary_next;
//				printf ("	Trader:dayline %.2f %.2f \n",maincontractdata.LastPrice_next,maincontractdata.Sedimentary_next);
			}


		}
		else{
			printf("	Trader::CheckSqlMaincontracontbyOpenInterest: [%s] ---无法获取远月主力合约!!!!!  \n",item.ProductID);
			sprintf(logbuf,"	Trader::CheckSqlMaincontracontbyOpenInterest: [%s] ---无法获取远月主力合约!!!!!  \n",item.ProductID);
			this->GetLogUtil()->WriteLog(logbuf);
		}

		bool v = strcmp(item.ExchangeID,"INE")!=0  &&  strcmp(item.ExchangeID,"CFFEX")!=0;
		if(v && maincontractdata.Sedimentary>1 && !mysqlmaincontract.Exist_DatabyProductID(item.ProductID)
				&& strcmp(maincontractdata.ProductID,item.ProductID)==0 && flag_findnext)	{

			int daylinedata =mysqldayline.CountbyAll(maincontractdata.InstrumentID);

			maincontractdata.DaylineCount = daylinedata;

			if ( maincontractdata.Sedimentary_next > maincontractdata.Sedimentary*0.7  )	{
				strcpy(maincontractdata.InstrumentID_trade ,maincontractdata.InstrumentID_next);
			}
			else	{
				strcpy(maincontractdata.InstrumentID_trade , maincontractdata.InstrumentID);
			}


			int daylinedata_next =mysqldayline.CountbyAll(maincontractdata.InstrumentID_next);
			maincontractdata.DaylineCount_next = daylinedata_next;
			maincontractdata.ErrorDaylineCount=0;
			maincontractdata.ErrorDaylineCount_next=0;

			if(maincontractdata.LastPrice <10000000000.0 && maincontractdata.LastPrice >1.0){
				mysqlmaincontract.Insert(maincontractdata);
//					printf("Trader::CheckSqlMaincontracontbyOpenInterest: 插入数据  %s  ----> %.2f !!!  \n",
//							maincontractdata.InstrumentID,maincontractdata.Sedimentary);
				printf("	Trader::CheckSqlMaincontracontbyOpenInterest: 品种:%s 主力合约[%s]  远月合约[%s]   沉淀资金:%.2f ---数据不存在 插入新数据!\n",
						item.ProductID,maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,maincontractdata.Sedimentary);
				sprintf(logbuf,"	Trader::CheckSqlMaincontracontbyOpenInterest: 品种:%s 主力合约[%s]  远月合约[%s]   沉淀资金:%.2f ---数据不存在 插入新数据!\n",
											item.ProductID,maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,maincontractdata.Sedimentary);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else{
				sprintf(logbuf,"	Trader::CheckSqlMaincontracontbyOpenInterest: 主力合约价格超出最大值，无法插入数据!!!  \n");
				this->GetLogUtil()->WriteLog(logbuf);
			}

		}
		else {

			if( strcmp(maincontractdata.ProductID,item.ProductID)!=0)			{
				printf("	Trader::CheckSqlMaincontracontbyOpenInterest: 主力合约计算错误 [%s] -->[%s]--- 无法插入新数据  %s  \n",maincontractdata.ProductID,maincontractdata.InstrumentID,item.ProductID);

			}

			if(maincontractdata.Sedimentary<=2)			{
				printf("	Trader::CheckSqlMaincontracontbyOpenInterest: 合约沉淀资金过小 [%s]--- 暂不插入列表  %s  \n",maincontractdata.InstrumentID,item.ProductID);
			}

			if(!flag_findnext){
				printf("	Trader::CheckSqlMaincontracontbyOpenInterest: [%s] ---无法获取远月主力合约!!!!!  \n",item.ProductID);
			}
		}
	}
	vector<ProductData>().swap(list);
	printf("Trader::CheckSqlMaincontracontbyOpenInterest   end\n");
}

bool Trader::GetNextContractbyVolume(MainContractData *maincontractdata,ProductData *item){

	 MysqlDepthMarket mysqldepthmarket("depthmarket_endtime");
	 MysqlInstrument mysqlinstrument;
	 MysqlCommission mysqlcommission;
	 CodeUtil codeutil;
	vector<DepthMarketData>  depthlists;
	mysqldepthmarket.Find_ListsbyProductIDVolume(depthlists,item->ProductID);
	bool flag_find=false;
	int main_ym=codeutil.GetYearMonthbyInstrumentID(maincontractdata->InstrumentID);

	if(depthlists.size()>=2){
	for (auto &depthdata:depthlists){

			int num=codeutil.GetYearMonthbyInstrumentID(depthdata.InstrumentID);

//			printf ("Trader::GetNextContractbyVolume: 次月主力合约匹配中---->[%s]   主力合约年月:%d  待匹配合约年月:%d \n",depthdata.InstrumentID,main_ym,num);

			if(num>main_ym&& !flag_find)	{
				flag_find=true;
				printf ("	Trader::GetNextContractbyVolume: ----[%s]  合约为次月主力合约------> \n",depthdata.InstrumentID);

				InstrumentData ins = mysqlinstrument.Find_DatabyInstrumentID(depthdata.InstrumentID);
				Commission commission=mysqlcommission.Find_DatabyProductID(depthdata.ProductID);

				strcpy(maincontractdata->InstrumentID_next,depthdata.InstrumentID);
				maincontractdata->OpenInterest_next = depthdata.OpenInterest;
				maincontractdata->Volume_next = depthdata.Volume;
				maincontractdata->UpperLimitPrice_next =depthdata.UpperLimitPrice;
				maincontractdata->LowerLimitPrice_next =depthdata.LowerLimitPrice;
				maincontractdata->LastPrice_next =  depthdata.LastPrice;

				int openinterest =depthdata.OpenInterest;
				int volumemultiple =ins.VolumeMultiple;
				int pricetick =ins.PriceTick;
				double longmarginratio = ins.LongMarginRatio;
				double lastprice = depthdata.LastPrice;


				double sedimentary =(double)openinterest*(double)commission.Margin*lastprice/100000000.0;
				maincontractdata->Sedimentary_next  = sedimentary;
			}
		}
		vector<DepthMarketData>().swap(depthlists);
	}
	else{
		flag_find=false;
	}
	return flag_find;

}
bool Trader::GetNextContractbyOpeninterest(MainContractData *maincontractdata,ProductData *item){

	 MysqlDepthMarket mysqldepthmarket;
	 MysqlInstrument mysqlinstrument;
	 MysqlCommission mysqlcommission;
	 CodeUtil codeutil;
	vector<DepthMarketData>  depthlists;
	mysqldepthmarket.Find_ListsbyProductID(depthlists,item->ProductID);
	bool flag_find=false;
	int main_ym=codeutil.GetYearMonthbyInstrumentID(maincontractdata->InstrumentID);

	for (auto &depthdata:depthlists){

		int num=codeutil.GetYearMonthbyInstrumentID(depthdata.InstrumentID);

//		printf ("Trader::GetNextContractbyOpeninterest: 次月主力合约匹配中---->[%s]   主力合约年月:%d  待匹配合约年月:%d \n",depthdata.InstrumentID,main_ym,num);

		if(num>main_ym&& !flag_find)	{
			flag_find=true;
			printf ("	Trader::GetNextContractbyOpeninterest: ----[%s]  合约为次月主力合约------> \n",depthdata.InstrumentID);

			InstrumentData ins = mysqlinstrument.Find_DatabyInstrumentID(depthdata.InstrumentID);
			Commission commission=mysqlcommission.Find_DatabyProductID(depthdata.ProductID);

			strcpy(maincontractdata->InstrumentID_next,depthdata.InstrumentID);
			maincontractdata->OpenInterest_next = depthdata.OpenInterest;
			maincontractdata->Volume_next = depthdata.Volume;
			maincontractdata->UpperLimitPrice_next =depthdata.UpperLimitPrice;
			maincontractdata->LowerLimitPrice_next =depthdata.LowerLimitPrice;
			maincontractdata->LastPrice_next =  depthdata.LastPrice;

			int openinterest =depthdata.OpenInterest;
			int volumemultiple =ins.VolumeMultiple;
			int pricetick =ins.PriceTick;
			double longmarginratio = ins.LongMarginRatio;
			double lastprice = depthdata.LastPrice;


			double sedimentary =(double)openinterest*(double)commission.Margin*lastprice/100000000.0;
			maincontractdata->Sedimentary_next  = sedimentary;
		}
	}
	vector<DepthMarketData>().swap(depthlists);
	return flag_find;
}
void Trader::CheckSqlMaincontracontbyMaxOpenInterest(){

	CodeUtil codeutil;
	MysqlDepthMarket mysqldepthmarket;
	int depthcount=mysqldepthmarket.Count_AllLists();
	if(depthcount==0){
		printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest--- depthmarket 数据表为空 再次请求一次! \n");
		CheckSqlDepMarket();
		depthcount=mysqldepthmarket.Count_AllLists();
		printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest--- depthmarket 数据表为空 再次请求后 --->%d ! \n",depthcount);
	}

	MysqlMaincontract mysqlmaincontract;
	std::string update = mysqlmaincontract.GetLastLocalUpdateTime();
//	printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest--- 比对行情信息更新日期  LocalUpdateTime:%s  \n",update.c_str());
	DateUtil dateutil;
	time_t lastclosetime_t = dateutil.GetLastCloseTime();
	time_t t_localupdate =dateutil.ConvertSqlTimetoTimet (update.c_str());

	std::string tradingday = mysqlmaincontract.GetOldestTradingDay();

	if(strcmp(this->GetTradingDay().c_str(),tradingday.c_str())!=0)	{
		printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest---交易日不同  maincontract交易日:%s   交易所交易日:%s \n",
				tradingday.c_str(),this->GetTradingDay().c_str());
		mysqlmaincontract.DeleteAll();
	}
	else {

		if (t_localupdate < lastclosetime_t)	{
			printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest--- 主力合约更新日期为上一交易日之前  time_t  %s -->t_update:%ld   %s-->lastclosetime_t:%ld :\n",
					dateutil.ConvertTimetoSqlString(t_localupdate).c_str(),t_localupdate,dateutil.ConvertTimetoSqlString(lastclosetime_t).c_str(),lastclosetime_t);
			mysqlmaincontract.DeleteAll();
		}
		else {
			printf("Trader::CheckSqlMaincontracontbyOpenInterest--- 本次不更新主力合约表!!!   LocalUpdateTime:%s  \n",update.c_str());
		}
	}

	int count = mysqlmaincontract.Count_AllLists();

//	printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest   0000000\n");
	if( (t_localupdate < lastclosetime_t ||strcmp(this->GetTradingDay().c_str(),tradingday.c_str())!=0 || count<30  ) && (depthcount!=0) )	{

		MysqlProduct mysqlproduct;
		vector<ProductData>  list;
		mysqlproduct.Find_AllList(list);
		printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest----->更新主力合约 合约品种[%ld] ---->\n",list.size());

		MysqlInstrument mysqlinstrument;

		MysqlDayLine mysqldayline;
		for (auto &item:list)		{
			InstrumentData instrumentdata_a;
			InstrumentData instrumentdata_b;
			DepthMarketData depthmarketdata_a;
			DepthMarketData depthmarketdata_b;
			MainContractData maincontractdata;

			mysqldepthmarket.Find_MainNextItembyProductID(depthmarketdata_a,depthmarketdata_b,item.ProductID);

			int a_yearmonth=codeutil.GetYearMonthbyInstrumentID(depthmarketdata_a.InstrumentID);
			int b_yearmonth=codeutil.GetYearMonthbyInstrumentID(depthmarketdata_b.InstrumentID);

			instrumentdata_a = mysqlinstrument.Find_DatabyInstrumentID(depthmarketdata_a.InstrumentID);
			instrumentdata_b = mysqlinstrument.Find_DatabyInstrumentID(depthmarketdata_b.InstrumentID);

			if(a_yearmonth<b_yearmonth){
				printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest----->主力合约[%s] opi[%d] ,远月合约[%s] opi[%d]---->\n",
						depthmarketdata_a.InstrumentID,depthmarketdata_a.OpenInterest,depthmarketdata_b.InstrumentID,depthmarketdata_b.OpenInterest);
				SaveMainContractData(&depthmarketdata_a,&depthmarketdata_b,&item);
			}
			else{
				printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest---合约过渡期-->主力合约[%s] opi[%d] ,远月合约[%s] opi[%d]---->\n",
					depthmarketdata_b.InstrumentID,depthmarketdata_b.OpenInterest,depthmarketdata_a.InstrumentID,depthmarketdata_a.OpenInterest);

				SaveMainContractData(&depthmarketdata_b,&depthmarketdata_a,&item);
			}
		}
	}
	else{
		if(depthcount==0){
			printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest  ERROR ----->depthmarket table is empty !\n");
		}
	}

//	printf("Trader::CheckSqlMaincontracontbyMaxOpenInterest   end\n");
}
void Trader::SaveMainContractData(DepthMarketData *main,DepthMarketData *next,ProductData *item){

	MysqlDepthMarket mysqldepthmarket;
	MysqlInstrument mysqlinstrument;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;
	MysqlCommission mysqlcommission;


	InstrumentData instrumentdata_main;
	InstrumentData instrumentdata_next;

	instrumentdata_main = mysqlinstrument.Find_DatabyInstrumentID(main->InstrumentID);
	instrumentdata_next = mysqlinstrument.Find_DatabyInstrumentID(next->InstrumentID);
	MainContractData maincontractdata;

//	printf("Trader::SaveMainContractData----->合约品种[%s] ---->\n",item->ProductID);
//	printf("Trader::SaveMainContractData----->合约交易单位[%d] ---->\n",instrumentdata_main.VolumeMultiple);

	if(main->InstrumentID!=NULL){
//		printf("Trader::SaveMainContractData----->字符非空[%s] ---->\n",instrumentdata_main.InstrumentID);
	}
	else	{
		printf("Trader::SaveMainContractData----->[%s] ---->空字符！！！ \n",main->InstrumentID);
	}

	HxDayLineData daylinedata_main =mysqldayline.Find_LastDayLine(main->InstrumentID);

//					printf ("------maxopi[%d] =   maxvol[%d]   相同------[%s] ----沉淀资金[%.3f]\n",maxopi.OpenInterest,maxvol.Volume,maxopi.InstrumentID,sedimentary_opi);

//	printf ("Trader::SaveMainContractData: maxopi[%s]  opi[%d] vol[%d]  最大------沉淀资金[%.3f]\n",main->InstrumentID,
//			main->OpenInterest,main->Volume,sedimentary_opi);
//					printf ("maxvol[%s]  opi[%d] vol[%d]   相同------沉淀资金[%.3f]\n",maxvol.InstrumentID,maxvol.OpenInterest,maxvol.Volume,sedimentary_vol);
	Commission commission=mysqlcommission.Find_DatabyProductID(item->ProductID);

	CreatMainContractData(&mysqldayline,&maincontractdata,&instrumentdata_main,main,item,commission.Margin);

	strcpy(maincontractdata.InstrumentID_next,next->InstrumentID);
	maincontractdata.OpenInterest_next = next->OpenInterest;
	maincontractdata.Volume_next = next->Volume;
	maincontractdata.UpperLimitPrice_next =next->UpperLimitPrice;
	maincontractdata.LowerLimitPrice_next =next->LowerLimitPrice;
	maincontractdata.LastPrice_next =  next->LastPrice;

	int openinterest =next->OpenInterest;
	int volumemultiple =next->VolumeMultiple;
	int pricetick =instrumentdata_next.PriceTick;
	double longmarginratio = instrumentdata_next.LongMarginRatio;
	double lastprice = next->LastPrice;


	double sedimentary =(double)openinterest*(double)volumemultiple*longmarginratio*lastprice/100000000.0;
	maincontractdata.Sedimentary_next  = sedimentary;


	bool v = strcmp(item->ExchangeID,"INE")!=0  &&  strcmp(item->ExchangeID,"CFFEX")!=0;
	if(v && maincontractdata.Sedimentary>1 && !mysqlmaincontract.Exist_DatabyProductID(item->ProductID)
			&& strcmp(maincontractdata.ProductID,item->ProductID)==0)	{



		int daylinedata =mysqldayline.CountbyAll(maincontractdata.InstrumentID);

		maincontractdata.DaylineCount = daylinedata;

		if ( maincontractdata.Sedimentary_next > maincontractdata.Sedimentary*0.7  )				{
			strcpy(maincontractdata.InstrumentID_trade ,maincontractdata.InstrumentID_next);
		}
		else	{
			strcpy(maincontractdata.InstrumentID_trade , maincontractdata.InstrumentID);

		}



		int daylinedata_next =mysqldayline.CountbyAll(maincontractdata.InstrumentID_next);
		maincontractdata.DaylineCount_next = daylinedata_next;
		maincontractdata.ErrorDaylineCount=0;
		maincontractdata.ErrorDaylineCount_next=0;

		CodeUtil codeutil;
		std::string productid_main = codeutil.GetProductID(maincontractdata.InstrumentID);

		bool valid_productidcode = strcmp(maincontractdata.ProductID,productid_main.c_str())==0;
		bool valid_price = maincontractdata.LastPrice <10000000000.0 && maincontractdata.LastPrice >1.0;
		if(valid_productidcode && valid_price){

			printf("Trader::SaveMainContractData: 不存在主力合约[%s]  远月合约[%s]--- 插入新数据  %s  \n",
							maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,item->ProductID);

			mysqlmaincontract.Insert(maincontractdata);
//			printf("Trader::SaveMainContractData: 插入数据  %s  ----> %.2f !!!  \n",
//					maincontractdata.InstrumentID,maincontractdata.Sedimentary);
		}
		else{
			if(!valid_price){
				printf("Trader::SaveMainContractData: 不存在主力合约[%s]  远月合约[%s]--- 主力合约价格超出最大值，无法插入数据!!!  %s  \n",
							maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,item->ProductID);
			}
			if(!valid_productidcode){
				printf("Trader::SaveMainContractData: 主力合约[%s]  远月合约[%s]  --- 主力合约产品代码错误，无法插入数据!!!  %s  \n",
						maincontractdata.InstrumentID,maincontractdata.InstrumentID_next,item->ProductID);
			}

		}
	}
}
void Trader::CreatMainContractData(MysqlDayLine *mysqldayline,MainContractData *maincontractdata,
		InstrumentData *insopi,DepthMarketData *maxopi,ProductData *item,double margin){


	strcpy(maincontractdata->InstrumentID,maxopi->InstrumentID);
	strcpy(maincontractdata->InstrumentName,insopi->InstrumentName);
	maincontractdata->PriceTick =insopi->PriceTick;
	maincontractdata->VolumeMultiple=insopi->VolumeMultiple;
	maincontractdata->LongMarginRatio = insopi->LongMarginRatio;
	maincontractdata->ShortMarginRatio = insopi->ShortMarginRatio;

	strcpy(maincontractdata->ExchangeID,item->ExchangeID);
	strcpy(maincontractdata->ProductID,item->ProductID);
	strcpy(maincontractdata->Section,item->Section);
	strcpy(maincontractdata->DayNight,item->DayNight);

	strcpy(maincontractdata->TradingDay,maxopi->TradingDay);
	strcpy(maincontractdata->ActionDay,maxopi->ActionDay);
	strcpy(maincontractdata->UpdateTime,maxopi->UpdateTime);
	maincontractdata->OpenInterest = maxopi->OpenInterest;
	maincontractdata->Volume = maxopi->Volume;
	maincontractdata->UpperLimitPrice =maxopi->UpperLimitPrice;
	maincontractdata->LowerLimitPrice =maxopi->LowerLimitPrice;

//	maincontractdata->
	double sedimentary_opi ;
	if(maxopi->LastPrice<10000000000){
		maincontractdata->LastPrice = maxopi->LastPrice;
		sedimentary_opi = (maxopi->OpenInterest*insopi->VolumeMultiple*margin*maxopi->LastPrice)/(100000000.0);
		printf ("	Trader::CheckSqlMaincontracontbyOpenInterest: 持仓量最大合约[%s]  持仓量[%d] 成交量[%d] 交易单位[%d] 保证金[%.2f%%] 价格[%.2f]------>沉淀资金[%.3f]\n",
				maxopi->InstrumentID,maxopi->OpenInterest,maxopi->Volume,
				insopi->VolumeMultiple,margin*100,maxopi->LastPrice,sedimentary_opi);
	}
	else{
		HxDayLineData lastdata=mysqldayline->Find_LastDayLine(maxopi->InstrumentID);
		maincontractdata->LastPrice = lastdata.Close;
		sedimentary_opi = (maxopi->OpenInterest*insopi->VolumeMultiple*margin*lastdata.Close)/(100000000.0);
		printf ("	Trader::CheckSqlMaincontracontbyOpenInterest: 持仓量最大合约[%s]  持仓量[%d] 成交量[%d] 交易单位[%d] 保证金[%.2f%%] 最后交易日收盘价[%.2f]------>沉淀资金[%.3f]\n",
				maxopi->InstrumentID,maxopi->OpenInterest,maxopi->Volume,
				insopi->VolumeMultiple,margin*100,lastdata.Close,sedimentary_opi);
	}

	maincontractdata->Sedimentary  = sedimentary_opi;
}

void Trader::CreatMainContractDatabyMaxOPI(MainContractData *maincontractdata,
	InstrumentData *insopi,DepthMarketData *maxopi,ProductData *item){

	strcpy(maincontractdata->InstrumentID,maxopi->InstrumentID);
	strcpy(maincontractdata->InstrumentName,insopi->InstrumentName);
	maincontractdata->PriceTick =insopi->PriceTick;
	maincontractdata->VolumeMultiple=insopi->VolumeMultiple;
	maincontractdata->LongMarginRatio = insopi->LongMarginRatio;
	maincontractdata->ShortMarginRatio = insopi->ShortMarginRatio;


	strcpy(maincontractdata->ExchangeID,item->ExchangeID);
	strcpy(maincontractdata->ProductID,item->ProductID);
	strcpy(maincontractdata->Section,item->Section);
	strcpy(maincontractdata->DayNight,item->DayNight);

	strcpy(maincontractdata->TradingDay,maxopi->TradingDay);
	strcpy(maincontractdata->ActionDay,maxopi->ActionDay);
	strcpy(maincontractdata->UpdateTime,maxopi->UpdateTime);
	maincontractdata->OpenInterest = maxopi->OpenInterest;
	maincontractdata->Volume = maxopi->Volume;
	maincontractdata->UpperLimitPrice =maxopi->UpperLimitPrice;
	maincontractdata->LowerLimitPrice =maxopi->LowerLimitPrice;
	maincontractdata->LastPrice = maxopi->LastPrice;

	int openinterest =maxopi->OpenInterest;
	int volumemultiple =insopi->VolumeMultiple;
	int pricetick =insopi->PriceTick;
	double longmarginratio = insopi->LongMarginRatio;
	double lastprice = maxopi->LastPrice;


	double sedimentary =(double)openinterest*(double)volumemultiple*longmarginratio*lastprice/100000000.0;
	maincontractdata->Sedimentary  = sedimentary;


}
//功能 DataReadyStatus 状态重置
bool Trader::CheckTradingDay()
{



	bool ret;
//	TradingDayData tradingdaydata=this->GetTradingDayData();

//printf(" CheckTradingDay  \n");
	std::string tradingday ;
	tradingday = this->GetTradingDay().c_str();
	tradingday.append("160000");
	DateUtil dateutil;

	time_t tradingclose=dateutil.ConvertStringtoTimet(tradingday.c_str());
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	printf("Trader::CheckTradingDay   -----> [%s] \n",this->GetTradingDay().c_str());

//	 double Q_BarTime =  dateutil.GetCurrentBarTime();
//
//	bool day_time = Q_BarTime > 0.000000 && Q_BarTime < 0.160000 ;
//	bool night_time = Q_BarTime >= 0.160000 && Q_BarTime <= 0.235959 ;

	if (tt<tradingclose)	{

		MysqlUser mysqluser;
		User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
		MysqlTradingDay mysqltradingday(userinfo.UserID);
		TradingDayData tradingdaydata;
		tradingdaydata= mysqltradingday.Find_LastTradingDay();

		tradingdaydata.DataReadyStatus =true;
		mysqltradingday.UpdateDataReadyStatusbyDate(&tradingdaydata);

		this->GetLogUtil()->WriteLog("Trader::CheckTradingDay-----DataReadyStatus 完成主力合约 远月合约的计算,可以开始订阅合约!");
		ret = true;
	}
	else{
			printf("Trader::CheckTradingDay  ------>ReqQryTradingAccount \n");
			ret = false;
			this->ReqQryTradingAccount();

			this->GetLogUtil()->WriteLog("Trader::CheckTradingDay-----当前日期大于交易日收盘时间,重新查询!");

			this->UpdateTradingDay();
			this->UpdateTradingDaySqlTime();
			printf("Trader::CheckTradingDay   UpdateTradingDay 更新交易日后-----> [%s] \n",this->GetTradingDay().c_str());
	}


	printf("Trader::CheckTradingDay  End!!!  \n");
	return ret;
}

void Trader::CloseOrder(Signal signal,
		double openprice,
		double stopprice,
		const char*exchangeid,
		const char*opendate,int maxcount)
{
	int MAX_ERROR_COUNT = 4;
	std::string tradingdate;
	tradingdate = signal.TradingDay;
	tradingdate.append("000000");


	Config config(this->GetStrategyName().c_str());
	MysqlOrder mysqlorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);

	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
	int longposition =mysqlhold.CollectPositions_byInstrumentID(signal.InstrumentID,0);
	int shortposition =mysqlhold.CollectPositions_byInstrumentID(signal.InstrumentID,1);

	int longreqvolume = mysqlorder.Get_QueueOrderListsVolume(signal.InstrumentID,0, signal.TradingDay);
	int shortreqvolume = mysqlorder.Get_QueueOrderListsVolume(signal.InstrumentID,1, signal.TradingDay);

	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);


	MysqlDepthMarket mysqldepthmarket;
	DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(signal.InstrumentID);


	bool long_valid1 =  depthmarket.BidPrice1 >data.LowerLimitPrice;
	bool long_valid2 = depthmarket.BidPrice1 < data.UpperLimitPrice;
	bool longclose_valid = long_valid1 && long_valid2 &&depthmarket.BidPrice1!=0;

	bool short_valid1 =  depthmarket.AskPrice1 >data.LowerLimitPrice;
	bool short_valid2 = depthmarket.AskPrice1 < data.UpperLimitPrice;
	bool shortclose_valid = long_valid1 && long_valid2 && depthmarket.AskPrice1!=0;



	int send_count = mysqlorder.Count_OrderListbyStatus(signal.InstrumentID,tradingdate.substr(0,8).c_str(),0);   //发送了, 后台未受理
//					printf("信号激活 --> %s----发单数[%d]  \n",pinstrumentID,send_count);
	int queue_count = mysqlorder.Count_OrderListbyStatus(signal.InstrumentID,tradingdate.substr(0,8).c_str(),1);  //交易队列中
//					printf("信号激活 --> %s----排队[%d]  \n",pinstrumentID,queue_count);
	int done_count = mysqlorder.Count_OrderListbyStatus(signal.InstrumentID,tradingdate.substr(0,8).c_str(),2);  // 已成交
//					printf("信号激活 --> %s----成交数[%d]  \n",pinstrumentID,done_count);
	int action_count = mysqlorder.Count_OrderListbyStatus(signal.InstrumentID,tradingdate.substr(0,8).c_str(),22); //有撤单
//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);

	int error_count = mysqlorder.Count_OrderListbyStatus(signal.InstrumentID,tradingdate.substr(0,8).c_str(),-21); //有撤单
	//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);

	bool count_flag =  send_count==0 && queue_count==0 ;

	if (signal.SingalActive  &&  !signal.OrderSend )	{


		if(longclose_valid)		{
			printf("	委托平仓多单 价格合法性检查通过--> %s   longposition %d   longreqvolume %d \n",signal.SingalName,longposition,longreqvolume );
		}

		if (signal.Direction==0 )	{
			printf("	委托平仓多单 [%s] 信号激活--> %s\n",signal.SingalName, signal.InstrumentID);
		}
		else if(signal.Direction==1)		{
			printf("	委托平仓空单 [%s] 信号激活--> %s\n",signal.SingalName, signal.InstrumentID);
		}
		else	{
			printf("	委托平仓空单 [%s] 方向不明--> %s\n",signal.SingalName, signal.InstrumentID);
		}

		if(count_flag)	{
			printf("	委托平仓单 1111111111-->\n");
		}
		else	{
			printf("	委托平仓单 22222222-->  send_count: %d  queue_count: %d  done_count: %d \n",send_count,queue_count,done_count);
		}

		// 多头出现止损
		if(signal.Direction==0  && longclose_valid && longposition-longreqvolume >0 && count_flag)		{
			printf("	多头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
					openprice,stopprice,depthmarket.LastPrice);
			printf("	委托平仓多单 --> %s\n", signal.InstrumentID);
			char directiontype='1';   // 0 : 买    1:卖
			char CombOffsetFlag;
			int v =longposition-longreqvolume;

			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)			{
				if(strcmp(opendate, signal.TradingDay)==0)				{
					CombOffsetFlag=THOST_FTDC_OF_CloseToday;

				}
				else				{
					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
				}
			}
			else			{
				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
			}
			this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
					CombOffsetFlag,	depthmarket.BidPrice1,v,stopprice);
			signal.OrderSend=true;
			this->GetRedisSignal()->SetInsSignal(signal,signal.SingalName);
		}

		//空头出现止损
		else if(signal.Direction==1 && shortclose_valid && shortposition- shortreqvolume >0  && count_flag)		{
			printf("	空头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
					openprice,stopprice,depthmarket.LastPrice);
			printf("	委托平仓空单 --> %s\n", signal.InstrumentID);
			char directiontype='0';   // 0 : 买    1:卖
			char CombOffsetFlag;   // 0 : 开仓  1: 平仓

			int v= shortposition- shortreqvolume;
			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)			{
				if(strcmp(opendate, signal.TradingDay)==0)				{
					CombOffsetFlag=THOST_FTDC_OF_CloseToday;

				}
				else				{
					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
				}
			}
			else			{
				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
			}
			this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
					CombOffsetFlag,depthmarket.AskPrice1,v,stopprice);
			signal.OrderSend=true;
			this->GetRedisSignal()->SetInsSignal(signal,signal.SingalName);
		}
	}
	else if(signal.SingalActive && signal.OrderSend)	{
		// 多头出现亏损
		if(signal.Direction==0 && count_flag && longposition-longreqvolume >0 &&
				action_count<=maxcount && error_count<=MAX_ERROR_COUNT)		{

			printf("	多头合约再次平仓[%s] -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
											openprice,stopprice,depthmarket.LastPrice);
			printf("	委托平仓多单 --> %s\n", signal.InstrumentID);
			char directiontype='1';   // 0 : 买    1:卖
			char CombOffsetFlag;
			int v =longposition-longreqvolume;

			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)			{
				if(strcmp(opendate, signal.TradingDay)==0)				{
					CombOffsetFlag=THOST_FTDC_OF_CloseToday;

				}
				else				{
					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
				}
			}
			else			{
				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
			}
			this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
					CombOffsetFlag,	depthmarket.BidPrice1,v,stopprice);

		}
		else if(signal.Direction==1 && count_flag && shortposition- shortreqvolume >0
				&& action_count<=maxcount && error_count<4)		{

			printf("	空头合约再次平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
					openprice,stopprice,depthmarket.LastPrice);
			printf("	委托平仓空单 --> %s\n", signal.InstrumentID);
			char directiontype='0';   // 0 : 买    1:卖
			char CombOffsetFlag;   // 0 : 开仓  1: 平仓

			int v= shortposition- shortreqvolume;
			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)			{
				if(strcmp(opendate, signal.TradingDay)==0)				{
					CombOffsetFlag=THOST_FTDC_OF_CloseToday;

				}
				else				{
					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
				}
			}
			else			{
				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
			}
			this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
					CombOffsetFlag,depthmarket.AskPrice1,v,stopprice);

		}
	}


}




//void CloseOrder(Signal signal,
//		double openprice,
//		double stopprice,
//		const char*exchangeid,
//		const char*opendate,int maxcount)
//{
//	int MAX_ERROR_COUNT = 4;
//	std::string tradingdate;
//	tradingdate = signal.TradingDay;
//	tradingdate.append("000000");
//	std::string tablename ="close";
//
//	MysqlOrder mysqlorder;
//
//	MysqlHold mysqlhold;
//	int longposition =mysqlhold.CollectPositions_byInstrumentID(signal.InstrumentID,0);
//	int shortposition =mysqlhold.CollectPositions_byInstrumentID(signal.InstrumentID,1);
//
//	int longreqvolume = mysqlorder.Get_QueueOrderListsVolume(tablename.c_str(),signal.InstrumentID,0, signal.TradingDay);
//	int shortreqvolume = mysqlorder.Get_QueueOrderListsVolume(tablename.c_str(),signal.InstrumentID,1, signal.TradingDay);
//
//	DepthMarket depthmarket;
//	depthmarket = redisdepthmarket.GetDepthMarket(signal.InstrumentID);
//
//
//	MysqlDepthMarket mysqldepthmarket;
//	DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(signal.InstrumentID);
//
//
//	bool long_valid1 =  depthmarket.BidPrice1 >data.LowerLimitPrice;
//	bool long_valid2 = depthmarket.BidPrice1 < data.UpperLimitPrice;
//	bool longclose_valid = long_valid1 && long_valid2 &&depthmarket.BidPrice1!=0;
//
//	bool short_valid1 =  depthmarket.AskPrice1 >data.LowerLimitPrice;
//	bool short_valid2 = depthmarket.AskPrice1 < data.UpperLimitPrice;
//	bool shortclose_valid = long_valid1 && long_valid2 && depthmarket.AskPrice1!=0;
//
//
//
//	int send_count = mysqlorder.Count_OrderListbyStatus(tablename.c_str(),signal.InstrumentID,tradingdate.substr(0,8).c_str(),0);   //发送了, 后台未受理
////					printf("信号激活 --> %s----发单数[%d]  \n",pinstrumentID,send_count);
//	int queue_count = mysqlorder.Count_OrderListbyStatus(tablename.c_str(),signal.InstrumentID,tradingdate.substr(0,8).c_str(),1);  //交易队列中
////					printf("信号激活 --> %s----排队[%d]  \n",pinstrumentID,queue_count);
//	int done_count = mysqlorder.Count_OrderListbyStatus(tablename.c_str(),signal.InstrumentID,tradingdate.substr(0,8).c_str(),2);  // 已成交
////					printf("信号激活 --> %s----成交数[%d]  \n",pinstrumentID,done_count);
//	int action_count = mysqlorder.Count_OrderListbyStatus(tablename.c_str(),signal.InstrumentID,tradingdate.substr(0,8).c_str(),22); //有撤单
////					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);
//
//	int error_count = mysqlorder.Count_OrderListbyStatus(tablename.c_str(),signal.InstrumentID,tradingdate.substr(0,8).c_str(),-21); //有撤单
//	//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);
//
//	bool count_flag =  send_count==0 && queue_count==0 && done_count==0;
//
//	if (signal.SingalActive  &&  !signal.OrderSend )
//	{
//
//
//		if(longclose_valid)
//		{
//			printf("	委托平仓多单 价格合法性检查通过--> %s   longposition %d   longreqvolume %d \n",signal.SingalName,longposition,longreqvolume );
//		}
//
//		if (signal.Direction==0 )
//		{
//			printf("	委托平仓多单 [%s] 信号激活--> %s\n",signal.SingalName, signal.InstrumentID);
//		}
//		else if(signal.Direction==1)
//		{
//			printf("	委托平仓空单 [%s] 信号激活--> %s\n",signal.SingalName, signal.InstrumentID);
//		}
//		else
//		{
//			printf("	委托平仓空单 [%s] 方向不明--> %s\n",signal.SingalName, signal.InstrumentID);
//		}
//
//		if(count_flag)
//		{
//			printf("	委托平仓单 1111111111-->\n");
//		}
//		else
//		{
//			printf("	委托平仓单 22222222-->\n");
//		}
//		// 多头出现止损
//		if(signal.Direction==0  && longclose_valid && longposition-longreqvolume >0 && count_flag)
//		{
//			printf("	多头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
//					openprice,stopprice,depthmarket.LastPrice);
//			printf("	委托平仓多单 --> %s\n", signal.InstrumentID);
//			char directiontype='1';   // 0 : 买    1:卖
//			char CombOffsetFlag;
//			int v =longposition-longreqvolume;
//
//			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)
//			{
//				if(strcmp(opendate, signal.TradingDay)==0)
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseToday;
//
//				}
//				else
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
//				}
//			}
//			else
//			{
//				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
//			}
//			trader.ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
//					CombOffsetFlag,	depthmarket.BidPrice1,v,stopprice);
//			signal.OrderSend=true;
//			redisorder.SetInsSignal(signal,signal.SingalName);
//		}
//
//		//空头出现止损
//		else if(signal.Direction==1 && shortclose_valid && shortposition- shortreqvolume >0  && count_flag)
//		{
//			printf("	空头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
//					openprice,stopprice,depthmarket.LastPrice);
//			printf("	委托平仓空单 --> %s\n", signal.InstrumentID);
//			char directiontype='0';   // 0 : 买    1:卖
//			char CombOffsetFlag;   // 0 : 开仓  1: 平仓
//
//			int v= shortposition- shortreqvolume;
//			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)
//			{
//				if(strcmp(opendate, signal.TradingDay)==0)
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseToday;
//
//				}
//				else
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
//				}
//			}
//			else
//			{
//				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
//			}
//			trader.ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
//					CombOffsetFlag,depthmarket.AskPrice1,v,stopprice);
//			signal.OrderSend=true;
//			redisorder.SetInsSignal(signal,signal.SingalName);
//		}
//	}
//	else if(signal.SingalActive && signal.OrderSend)
//	{
//		// 多头出现亏损
//		if(signal.Direction==0 && count_flag && longposition-longreqvolume >0 && action_count<=maxcount && error_count<=MAX_ERROR_COUNT)
//		{
//
//			printf("	多头合约再次平仓[%s] -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
//											openprice,stopprice,depthmarket.LastPrice);
//			printf("	委托平仓多单 --> %s\n", signal.InstrumentID);
//			char directiontype='1';   // 0 : 买    1:卖
//			char CombOffsetFlag;
//			int v =longposition-longreqvolume;
//
//			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)
//			{
//				if(strcmp(opendate, signal.TradingDay)==0)
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseToday;
//
//				}
//				else
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
//				}
//			}
//			else
//			{
//				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
//			}
//			trader.ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
//					CombOffsetFlag,	depthmarket.BidPrice1,v,stopprice);
//
//		}
//		else if(signal.Direction==1 && count_flag && shortposition- shortreqvolume >0  && action_count<=maxcount && error_count<4)
//		{
//
//			printf("	空头合约再次平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
//					openprice,stopprice,depthmarket.LastPrice);
//			printf("	委托平仓空单 --> %s\n", signal.InstrumentID);
//			char directiontype='0';   // 0 : 买    1:卖
//			char CombOffsetFlag;   // 0 : 开仓  1: 平仓
//
//			int v= shortposition- shortreqvolume;
//			if(strcmp(depthmarket.ExchangeID,"SHFE")==0)
//			{
//				if(strcmp(opendate, signal.TradingDay)==0)
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseToday;
//
//				}
//				else
//				{
//					CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
//				}
//			}
//			else
//			{
//				 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
//			}
//			trader.ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,exchangeid,directiontype,
//					CombOffsetFlag,depthmarket.AskPrice1,v,stopprice);
//
//		}
//	}
//
//
//}

