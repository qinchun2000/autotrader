#include "market.hpp"


Market::Market() {
printf("Market 构造函数  无参 \n");
	 // 产生一个CThostFtdcTraderApi实例

	CodeUtil codeutil;
	std::string flowpath=codeutil.GetCurrentDirectory();
	flowpath.append("/mdflow/");

	this->_pMDApi = CThostFtdcMdApi::CreateFtdcMdApi("./mdflow/");
	// 产生一个事件处理的实例

//		SetLogPathFileName();
}

Market::Market(const char* path) {
	printf("Market构造函数  参数 path: %s  \n",path);
	this->CheckFlowPath(path);
 // 产生一个CThostFtdcTraderApi实例
	this->_pMDApi = CThostFtdcMdApi::CreateFtdcMdApi(path);
	// 产生一个事件处理的实例

//		SetLogPathFileName();
}

Market::Market(DataWrapper * datawrapper,const char*strategyname,const char* path) {
	printf("Market构造函数  参数 path: %s  \n",path);
	m_strategyname=strategyname;
	m_datawrapper = datawrapper;

	m_flowpath=path;

	std::string mslogname="main";
	mslogname.append("_market.log");
	this->SetLogPathFileName(mslogname.c_str());
	std::string logname_market ="main";
	logname_market.append("_market");
	this->SetLogName(logname_market.c_str());
	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());

}

void Market::SetCThostFtdcMdApi(CThostFtdcMdApi *pmdapi)
{

	this->_pMDApi=pmdapi;

}


CThostFtdcMdApi * Market::GetCThostFtdcMdApi()
{

	return this->_pMDApi;

}

void Market::SetCMdSpi(CMdSpi *pmdspi)
{

	this->_pMDSpi=pmdspi;

}


CMdSpi * Market::GetCMdSpi()
{

	return this->_pMDSpi;

}
void Market::SetMarketBase(MarketBase *  md)
{
	this->_pmarketdata=md;
}

MarketBase * Market::GetMarketBase()
{
	return this->_pmarketdata;
}
void Market::SetTaskFlag_UnSubscribe(bool flag)
{
this->_TaskFlag_UnSubscribe =flag;
}
bool  Market::GetTaskFlag_UnSubscribe()
{
return this->_TaskFlag_UnSubscribe ;
}

void Market::SetTaskFlag_Subscribe(bool flag)
{
this->_TaskFlag_Subscribe =flag;
}
bool  Market::GetTaskFlag_Subscribe()
{
return this->_TaskFlag_Subscribe ;
}

void Market::InitialInstrumentLists(vector<std::string>  &lists)
{
	printf("InitialInstrumentLists   begin \n");

	 MysqlMaincontract mymaincontract;
	 mymaincontract.GetTradeInstrumentLists(lists);
	 Config config("config");
	 MysqlSubscribe mysqlsubscribe(config.GetUserID().c_str());

	 if(!mysqlsubscribe.Exist_Table()){
		 mysqlsubscribe.CreateTable();
		 printf("InitialInstrumentLists   create 订阅清单表  \n");
	 }
	 else{
		 mysqlsubscribe.DeleteAllbyProcessor("main");
		 printf("InitialInstrumentLists   清空 订阅清单表  \n");
	 }

	for (auto &item:lists){
		SubscribeData info;
		strcpy(info.InstrumentID,item.c_str());
		info.Status=true;


		strcpy(info.UserID,config.GetUserID().c_str());
		strcpy(info.Processor,"main");
		mysqlsubscribe.Insert(info);

		 printf("InitialInstrumentLists   插入订阅数据 %s    \n",info.InstrumentID);
	}
//	vector<SubscribeData>  sub_lists ;
//	vector<SubscribeData>().swap(sub_lists);
}
void Market::InitialInstrumentLists(vector<std::string>  &lists,const char * processor)
{
	printf("InitialInstrumentLists   begin \n");

	 MysqlMaincontract mymaincontract;

	if(strcmp(processor,"main")==0 || strcmp(processor,"main_zlqh")==0)		{
//		mymaincontract.GetTradeInstrumentLists(lists);
		mymaincontract.GetTradeInstrumentListsbyMain(lists);
		printf("InitialInstrumentLists   初始化主力合约订阅表!!!  \n");
	}
	else if(strcmp(processor,"next")==0 || strcmp(processor,"next_zlqh")==0){
		mymaincontract.GetNextInstrumentLists(lists);
		printf("InitialInstrumentLists   初始化远月合约订阅表!!!  \n");
	}
	else{

	}


	 Config config("config");
	 MysqlSubscribe mysqlsubscribe(config.GetUserID().c_str());

	 if(!mysqlsubscribe.Exist_Table()){
		 mysqlsubscribe.CreateTable();
		 printf("InitialInstrumentLists   create 订阅清单表  \n");
	 }
	 else{
		 mysqlsubscribe.DeleteAllbyProcessor(processor);
		 printf("InitialInstrumentLists   清空 订阅清单表  \n");
	 }

	for (auto &item:lists){
		SubscribeData info;
		strcpy(info.InstrumentID,item.c_str());
		info.Status=true;

		Config config("config");
		strcpy(info.UserID,config.GetUserID().c_str());
		strcpy(info.Processor,processor);
		mysqlsubscribe.Insert(info);

		 printf("InitialInstrumentLists   插入订阅数据 %s    \n",info.InstrumentID);
	}
//	vector<SubscribeData>  sub_lists ;
//	vector<SubscribeData>().swap(sub_lists);
}
void Market::InitialInstrumentLists(vector<std::string>  &lists,const char * processor,const char* strategyname)
{
	printf("InitialInstrumentLists   begin \n");

	 MysqlMaincontract mymaincontract;

	if(strcmp(processor,"main")==0 || strcmp(processor,"main_zlqh")==0)		{
//		mymaincontract.GetTradeInstrumentLists(lists);
		mymaincontract.GetTradeInstrumentListsbyMain(lists);
		printf("InitialInstrumentLists   初始化主力合约订阅表!!!  \n");

//		std::string str;
//		str = "fu1909";
//		lists.push_back(str);

	}
	else if(strcmp(processor,"next")==0 || strcmp(processor,"next_zlqh")==0){
		mymaincontract.GetNextInstrumentLists(lists);
		printf("InitialInstrumentLists   初始化远月合约订阅表!!!  \n");
	}
	else{

	}


	 Config config(strategyname);
	 MysqlSubscribe mysqlsubscribe(config.GetUserID().c_str());

	 if(!mysqlsubscribe.Exist_Table()){
		 mysqlsubscribe.CreateTable();
		 printf("InitialInstrumentLists   create 订阅清单表  \n");
	 }
	 else{
		 mysqlsubscribe.DeleteAllbyProcessor(processor);
		 printf("InitialInstrumentLists   清空 订阅清单表  \n");
	 }

	for (auto &item:lists){
		SubscribeData info;
		strcpy(info.InstrumentID,item.c_str());
		info.Status=true;

//		Config config(strategyname);
		strcpy(info.UserID,config.GetUserID().c_str());
		strcpy(info.Processor,processor);
		mysqlsubscribe.Insert(info);

		 printf("InitialInstrumentLists   插入订阅数据 %s    \n",info.InstrumentID);
	}

//	vector<SubscribeData>  sub_lists ;
//	vector<SubscribeData>().swap(sub_lists);
}
void Market::SetInstrumentLists(vector<std::string> * list)
{
	this->_pinstrumentlists=list;
}

vector<std::string> * Market::GetInstrumentLists()
{
	return this->_pinstrumentlists;
}
void Market::CheckCommission()
{
	char logbuf[1024];

	MysqlDepthMarket mysqldepthmarket;
	MysqlInstrument mysqlinstrument;
	MysqlCommission mysqlcommission;


	vector<InstrumentData> instrumentidlist;
	mysqlinstrument.Find_AllList(instrumentidlist);
	CodeUtil codeutil;

	for (auto &item:instrumentidlist){
		Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);

		if(commission.VolumeMultiple!=item.VolumeMultiple){
			printf("Market::CheckCommission----->合约品种[%s]  成交单位不同>>>>>>>>> com vm:[%d]---->ins vm:[%d]\n",item.InstrumentID,
					commission.VolumeMultiple,item.VolumeMultiple);
		}



	}
	vector<InstrumentData>().swap(instrumentidlist);

	printf("Market::CheckContract   end\n");
}

void Market::CheckContract(vector<std::string>  &lists)
{
	char logbuf[1024];

	MysqlDepthMarket mysqldepthmarket;
	MysqlInstrument mysqlinstrument;
	MysqlCommission mysqlcommission;

	vector<DepthMarketData> depthlist;

	mysqldepthmarket.Find_AllList(depthlist);
	for (auto &item:depthlist){

		Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);

		InstrumentData	instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
		double sedimentary = (item.OpenInterest*instrumentdata.VolumeMultiple*commission.Margin*item.LastPrice)/(100000000.0);
		if(sedimentary>0.5){
			lists.push_back(item.InstrumentID);
			printf("Market::CheckContract----->合约品种[%s] ---->sed:[%.2f]\n",item.InstrumentID,sedimentary);
		}
	}

vector<DepthMarketData>().swap(depthlist);
	printf("Market::CheckContract   end\n");
}

void Market::CheckContractbyDataWrapper(vector<std::string>  &lists)
{
	char logbuf[1024];
	for (std::map<std::string,std::shared_ptr<DepthMarketData>>::iterator iter = this->m_datawrapper->GetDepthMarketMap()->begin();
			iter != this->m_datawrapper->GetDepthMarketMap()->end();iter++){

		 std::shared_ptr<Commission> commission=this->m_datawrapper->FindCommissionData(iter->second->ProductID);

		 if(commission!=nullptr){
			 std::shared_ptr<InstrumentData> instrumentdata = this->m_datawrapper->FindInstrumentData(iter->second->InstrumentID);
			double sedimentary = (iter->second->OpenInterest*instrumentdata->VolumeMultiple*commission->Margin*iter->second->LastPrice)/(100000000.0);
			if(sedimentary>0.5 && iter->second->LastPrice<10000000000.0){
				lists.push_back(iter->second->InstrumentID);
				printf("Market::CheckContractbyDataWrapper----->合约品种[%s] ---->sed:[%.2f]\n",iter->second->InstrumentID,sedimentary);
			}
		 }

	}

	printf("Market::CheckContractbyDataWrapper   订阅合约总数------->%ld \n",lists.size());
}
void Market::UpdateFlag()
{
	DateUtil dateutil;
	this->_flag_wday = dateutil.Check_wday();
	this->_flag_sunday = dateutil.Check_Sunday();
	this->_open_flag = dateutil.CheckMDOpenTime() && !_flag_sunday;
}

void Market::CheckFlowPath(const char *flowpath)
{

//	CodeUtil codeutil;
//	std::string flowpath=codeutil.GetCurrentDirectory();
//	flowpath.append("/");
//	flowpath.append(this->GetStrategyName().c_str());
//	flowpath.append("Flow/");

	if(access(flowpath,0)==-1){//access函数是查看文件是不是存在
		printf("CheckFlowPath:开始启动程序  --->>>>不存在文件夹 %s \n",flowpath);
		 if (mkdir(flowpath,0777)){//如果不存在就用mkdir函数来创建
			 printf("CheckFlowPath:开始启动程序  --->>>>创建文件夹 %s Error!!!  \n",flowpath);
		}
		 else{
			 printf("CheckFlowPath:开始启动程序  --->>>>创建文件夹 %s 完成!  \n",flowpath);
		 }
	}
	else{
//		printf("Run_Trade_zlqh:开始启动程序  --->>>>存在文件夹 %s \n",flowpath.c_str());
	}
//	return flowpath;
}
bool Market::isOpen()
{
	DateUtil dateutil;
	this->_open_flag = dateutil.CheckMDOpenTime() && !_flag_sunday;
	return this->_open_flag;
}

bool Market::ConnectRestore()
{
	bool frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();
	bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ;
	bool login_not = ! this->GetCMdSpi()->GetFlagLogin();
	bool ret = isOpen() && frontconnect_ok && frontdisconnect_was && login_not;
	 return ret;
}
bool Market::LoginAgin()
{
	bool frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();
	bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ;
	bool login_ok = this->GetCMdSpi()->GetFlagLogin();
	bool Submarket_reset= !this->GetCMdSpi()->GetFlagResetSubmarket();
	bool Submarket_not= !this->GetCMdSpi()->GetFlagSubmarket();
	bool ret = isOpen() && frontconnect_ok && frontdisconnect_was && login_ok && Submarket_reset && Submarket_not;
	 return ret;

}

void Market::SubmarketAgain(vector<std::string>  &lists)
{
	DateUtil dateutil;
	char buf[1024];


	sprintf (buf,"MdService--->>>SubmarketAgain  重新订阅行情------%s",dateutil.GetDateTimeString().c_str());

	if(lists.size()>0)		{

		sprintf (buf,"MdService--->>>  合约列表存在合约，取消订阅存在合约列表[%ld]!!!-----%s",
						 lists.size(),dateutil.GetDateTimeString().c_str());
		 this->GetLogUtil()->PrintLog(buf);

		this->GetMarketBase()->UnSubMarket(lists);

		vector<std::string>().swap(lists);

		sprintf (buf,"MdService--->>>  清空列表instrumentlists[%ld]!!!-----%s",
								 lists.size(),dateutil.GetDateTimeString().c_str());
		 this->GetLogUtil()->PrintLog(buf);
	}
	else {
		 sprintf (buf,"MdService--->>>  合约列表为空,重新查询instrumentlists!!!-----%s",dateutil.GetDateTimeString().c_str());
		 this->GetLogUtil()->PrintLog(buf);

		 InitialInstrumentLists(lists);

		 sprintf (buf,"MdService--->>> 更新合约列表后,重新再次订阅行情-开始!!!-----%s",dateutil.GetDateTimeString().c_str());
			 this->GetLogUtil()->PrintLog(buf);

		 this->GetMarketBase()->SubMarket(lists);

	 }


}
void Market::SubmarketAgain(vector<std::string>  &lists,const char* processor)
{
	DateUtil dateutil;
	char buf[1024];

	sprintf (buf,"Market::SubmarketAgain  重新订阅行情------%s",dateutil.GetDateTimeString().c_str());

	if(lists.size()>0)		{

		sprintf (buf,"Market::SubmarketAgain--->>>  合约列表存在合约，取消订阅存在合约列表[%ld]!!!-----%s",
						 lists.size(),dateutil.GetDateTimeString().c_str());
		 this->GetLogUtil()->PrintLog(buf);

		this->GetMarketBase()->UnSubMarket(lists);

		vector<std::string>().swap(lists);

		sprintf (buf,"Market::SubmarketAgain--->>>  清空列表instrumentlists[%ld]!!!-----%s",
								 lists.size(),dateutil.GetDateTimeString().c_str());
		 this->GetLogUtil()->PrintLog(buf);
	}



	 sprintf (buf,"Market::SubmarketAgain--->>>  合约列表为空,重新查询instrumentlists!!!-----%s",dateutil.GetDateTimeString().c_str());
	 this->GetLogUtil()->PrintLog(buf);

	 InitialInstrumentLists(lists,processor);

	 sprintf (buf,"Market::SubmarketAgain--->>> 更新合约列表后,重新再次订阅行情-开始!!!-----%s",dateutil.GetDateTimeString().c_str());
	 this->GetLogUtil()->PrintLog(buf);

	 this->GetMarketBase()->SubMarket(lists);
}

void Market::AutoUpdateSubmarket(vector<std::string>  &lists)
{
	DateUtil dateutil;
		char buf[1024];



		double Q_BarTime_2 = dateutil.GetCurrentBarTime();

		if(this->GetCMdSpi()->GetFlagLogin() && !_flag_wday && this->GetTaskFlag_UnSubscribe())
		{
			bool q_time = Q_BarTime_2 >= 0.205500 && Q_BarTime_2 < 0.205700;
			 if (q_time && this->GetCMdSpi()->GetFlagSubmarket() && !this->GetCMdSpi()->GetFlagUnSubmarket()  )	{
				 sprintf (buf,"Market::AutoUpdateSubmarket--->>> 每日取消订阅行情开始!!!------%s",dateutil.GetDateTimeString().c_str());
				 this->GetLogUtil()->PrintLog(buf);
				 this->GetMarketBase()->UnSubMarket(lists);
				 this->SetTaskFlag_UnSubscribe(false);
				}

	//		if (Q_BarTime_2 < 0.205000  && this->GetCMdSpi()->GetFlagUnSubmarket()  )	{
	//			this->GetCMdSpi()->SetFlagUnSubmarket(false);
	//			}

		}
		if (Q_BarTime_2 < 0.2055000 && !this->GetTaskFlag_UnSubscribe() && !_flag_wday )	{
				this->SetTaskFlag_UnSubscribe(true);
		}


		if(this->GetCMdSpi()->GetFlagLogin() && !_flag_wday && this->GetTaskFlag_Subscribe())	{

			 if (Q_BarTime_2 >= 0.205700 && Q_BarTime_2 < 0.210000 && !this->GetCMdSpi()->GetFlagSubmarket()  )	{

				 SubmarketAgain(lists);

				 //发送更新邮件
				 this->GetMarketBase()->SendSelfCheckMarket(lists);

	//			 if(!this->GetCMdSpi()->GetFlagSubmarket()){
	//				 this->GetCMdSpi()->SetFlagSubmarket(true);
	//			 }

				 this->SetTaskFlag_Subscribe(false);
				}

	//		if (Q_BarTime_2 < 0.205500  && this->GetCMdSpi()->GetFlagSubmarket() )	{
	//
	//			this->GetCMdSpi()->SetFlagSubmarket(false);
	//			}
			}



		if (Q_BarTime_2 < 0.2057000 && !this->GetTaskFlag_Subscribe() && !_flag_wday )
		{
			this->SetTaskFlag_Subscribe(true);
		}

}

void Market::AutoUpdateSubmarket(vector<std::string>  &lists,const char* processor)
{
	DateUtil dateutil;
	char buf[1024];


	sprintf (buf,"Market::AutoUpdateSubmarket------ 完成每日订阅重新订阅任务 %s\n",dateutil.GetDateTimeString().c_str());
	this->GetLogUtil()->PrintLog(buf);


	double Q_BarTime_2 = dateutil.GetCurrentBarTime();

	if(this->GetCMdSpi()->GetFlagLogin() && !_flag_wday && this->GetTaskFlag_UnSubscribe())	{
		bool q_time = Q_BarTime_2 >= 0.205000 && Q_BarTime_2 < 0.205500;
		 if (q_time && this->GetCMdSpi()->GetFlagSubmarket() && !this->GetCMdSpi()->GetFlagUnSubmarket()  )	{
			 sprintf (buf,"Market::AutoUpdateSubmarket--->>> 每日取消订阅行情开始!!!------%s",dateutil.GetDateTimeString().c_str());
			 this->GetLogUtil()->PrintLog(buf);
			 this->GetMarketBase()->UnSubMarket(lists);
			 this->SetTaskFlag_UnSubscribe(false);
			}

//		if (Q_BarTime_2 < 0.205000  && this->GetCMdSpi()->GetFlagUnSubmarket()  )	{
//			this->GetCMdSpi()->SetFlagUnSubmarket(false);
//			}

	}
	if (Q_BarTime_2 < 0.2050000 && !this->GetTaskFlag_UnSubscribe() && !_flag_wday )	{
			this->SetTaskFlag_UnSubscribe(true);
	}


	if(this->GetCMdSpi()->GetFlagLogin() && !_flag_wday && this->GetTaskFlag_Subscribe())	{

		 if (Q_BarTime_2 >= 0.205500 && Q_BarTime_2 < 0.210000 && !this->GetCMdSpi()->GetFlagSubmarket()  )	{

			 SubmarketAgain(lists,processor);

			 //发送更新邮件
			 this->GetMarketBase()->SendSelfCheckMarket(lists);

//			 if(!this->GetCMdSpi()->GetFlagSubmarket()){
//				 this->GetCMdSpi()->SetFlagSubmarket(true);
//			 }

			 this->SetTaskFlag_Subscribe(false);
			}

//		if (Q_BarTime_2 < 0.205500  && this->GetCMdSpi()->GetFlagSubmarket() )	{
//
//			this->GetCMdSpi()->SetFlagSubmarket(false);
//			}
		}



	if (Q_BarTime_2 < 0.2055000 && !this->GetTaskFlag_Subscribe() && !_flag_wday )
	{
		this->SetTaskFlag_Subscribe(true);
	}

}


void Market::AutoConnect()
{
	char buf[1024];

	this->GetLogUtil()->PrintLog("行情读取程序启动-----------------");

	DateUtil dateutil;
	sprintf (buf,"行情读取MdService--->>> 启动------%s\n",dateutil.GetDateTimeString().c_str());
	this->GetLogUtil()->PrintLog(buf);


//	m_redisminflag =  new RedisMinFlag () ;     // redis select 0
	m_redisdepthmarket = new RedisDepthMarket() ;   //redis select 1

//	m_redisminflag->FlushDB();
	m_redisdepthmarket->FlushDB();

	MysqlMdLogin mysqlmdlogin;
	mysqlmdlogin.CreateTable();
	vector<std::string>  instrumentlists;


	this->CheckFlowPath(m_flowpath.c_str());
 // 产生一个CThostFtdcTraderApi实例
	this->_pMDApi = CThostFtdcMdApi::CreateFtdcMdApi(m_flowpath.c_str());
	// 产生一个事件处理的实例

	_pMDSpi = new CMdSpi (this->_pMDApi);

//	this->SetCMdSpi(&spi);

	this->_pMDSpi->SetRedisDepthMarket(m_redisdepthmarket);


	_pmarketdata = new MarketBase (m_strategyname.c_str());
	_pmarketdata->SetCThostFtdcMdApi(this->GetCThostFtdcMdApi());
	_pmarketdata->SetCMdSpi(_pMDSpi);

	_pmarketdata->InitialbyStrategy(m_strategyname.c_str());
	_pmarketdata->FrontConnect();
	_pmarketdata->LoginbyStrategy(m_strategyname.c_str());

//	this->CheckContract(instrumentlists);
	CheckContractbyDataWrapper(instrumentlists);

	_pmarketdata->SubMarket(instrumentlists);


//printf("Ready for While ----------------->   \n");
//	while(1)
//	{
////		printf("Ready for While -----------------> 000000  \n");
//		UpdateFlag();
//
//		if ( ConnectRestore() )
//		{
//			 sprintf (buf,"MdService--->>> 线路中断,重新登录开始!!!-----%s",dateutil.GetDateTimeString().c_str());
//			 this->GetLogUtil()->PrintLog(buf);
//
//			 marketbase.Login();
//			 sprintf (buf,"MdService--->>> 线路中断,重新登录完成!!!-----%s",dateutil.GetDateTimeString().c_str());
//			 this->GetLogUtil()->PrintLog(buf);
//		}
//
//
//		if (LoginAgin()  )
//		{
//
//			sprintf (buf,"MdService--->>> 线路恢复成功,,重新登录完成,重新订阅行情!!!-----%s",dateutil.GetDateTimeString().c_str());
//			this->GetLogUtil()->PrintLog(buf);
//
//			SubmarketAgain(instrumentlists);
//
//			sprintf (buf,"MdService--->>> 线路恢复成功,,重新登录完成,重新订阅行情完成!!!-----%s",dateutil.GetDateTimeString().c_str());
//			this->GetLogUtil()->PrintLog(buf);
//		}
//
//
//		AutoUpdateSubmarket(instrumentlists);
//
//		sleep(3);
//
//	}

	 // 释放API实例
//	this->_pMDApi->Release();

	vector<std::string>().swap(instrumentlists);
	return;
}
void Market::Run()
{
	char buf[1024];
	std::string mslogname="test";
	mslogname.append("_market.log");
	this->SetLogPathFileName(mslogname.c_str());

	std::string logname_market ="test";
	logname_market.append("_market");
	this->SetLogName(logname_market.c_str());
	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->GetLogUtil()->PrintLog("行情读取程序启动-----------------");

	DateUtil dateutil;
	sprintf (buf,"行情读取MdService--->>> 启动------%s\n",dateutil.GetDateTimeString().c_str());
	this->GetLogUtil()->PrintLog(buf);


	RedisMinFlag redisminflag;     // redis select 0
	RedisDepthMarket redisdepthmarket;   //redis select 1

	redisminflag.FlushDB();
	redisdepthmarket.FlushDB();

	MysqlMdLogin mysqlmdlogin;
	mysqlmdlogin.CreateTable();
	vector<std::string>  instrumentlists;

	CMdSpi spi(this->_pMDApi);

	this->SetCMdSpi(&spi);

	this->GetCMdSpi()->SetRedisDepthMarket(&redisdepthmarket);


	MarketBase marketbase("test");
	marketbase.SetCThostFtdcMdApi(this->GetCThostFtdcMdApi());
	marketbase.SetCMdSpi(&spi);

	marketbase.Initial();
	marketbase.Login();

	SetMarketBase(&marketbase);


	InitialInstrumentLists(instrumentlists);
	marketbase.SubMarket(instrumentlists);


printf("Ready for While ----------------->   \n");
	while(1)
	{
//		printf("Ready for While -----------------> 000000  \n");
		UpdateFlag();

		if ( ConnectRestore() )
		{
			 sprintf (buf,"MdService--->>> 线路中断,重新登录开始!!!-----%s",dateutil.GetDateTimeString().c_str());
			 this->GetLogUtil()->PrintLog(buf);

			 marketbase.Login();
			 sprintf (buf,"MdService--->>> 线路中断,重新登录完成!!!-----%s",dateutil.GetDateTimeString().c_str());
			 this->GetLogUtil()->PrintLog(buf);
		}


		if (LoginAgin()  )
		{

			sprintf (buf,"MdService--->>> 线路恢复成功,,重新登录完成,重新订阅行情!!!-----%s",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);

			SubmarketAgain(instrumentlists);

			sprintf (buf,"MdService--->>> 线路恢复成功,,重新登录完成,重新订阅行情完成!!!-----%s",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
		}


		AutoUpdateSubmarket(instrumentlists);

		sleep(3);

	}

	 // 释放API实例
//	this->_pMDApi->Release();

	vector<std::string>().swap(instrumentlists);
	return;
}

void Market::Run(const char* processor,const char*strategyname)
{
	char buf[1024];
	Config config(strategyname);
	std::string mslogname=processor;
	mslogname.append("_market.log");
	this->SetLogPathFileName(mslogname.c_str());

	std::string logname_market =processor;
	logname_market.append("_market");
	this->SetLogName(logname_market.c_str());
	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->GetLogUtil()->PrintLog("Market:Run  行情读取程序启动-----------------");

	DateUtil dateutil;
	sprintf (buf,"Market:Run  行情读取MdService--->>> 启动------%s\n",dateutil.GetDateTimeString().c_str());
	this->GetLogUtil()->PrintLog(buf);

	DataWrapper datawrapper;
	datawrapper.SetUserID(config.GetUserID().c_str());
	datawrapper.InitCommisionMap();
	datawrapper.SetContractLists();

	RedisMinFlag redisminflag;     // redis select 0
	RedisDepthMarket redisdepthmarket;   //redis select 1


	if(strcmp(processor,"main")==0)		{
		redisminflag.FlushDB();
		redisdepthmarket.FlushDB();
	}
	if(strcmp(processor,"main_zlqh")==0)		{
		redisminflag.FlushDB();
		redisdepthmarket.FlushDB();
	}
	else if(strcmp(processor,"next")==0){

	}
	else if(strcmp(processor,"next_zlqh")==0){

	}
	else{

	}


	MysqlMdLogin mysqlmdlogin;
	mysqlmdlogin.CreateTable();
	vector<std::string>  instrumentlists;

	CMdSpi spi(this->_pMDApi);

	this->SetCMdSpi(&spi);

	this->GetCMdSpi()->SetRedisDepthMarket(&redisdepthmarket);

	std::string logname_spi =processor;
	logname_spi.append("_spi");
	this->GetCMdSpi()->SetLogName(logname_spi.c_str());
	std::string mdspilog = processor;
	mdspilog.append("_mdspi.log");
	this->GetCMdSpi()->SetLogPathFileName(mdspilog.c_str());

	MarketBase marketbase(strategyname);
	marketbase.SetCThostFtdcMdApi(this->GetCThostFtdcMdApi());
	marketbase.SetCMdSpi(&spi);
	marketbase.SetLogPathFileName(mslogname.c_str());
	std::string logname_marketbase =processor;
	logname_marketbase.append("_base");
	marketbase.SetLogName(logname_marketbase.c_str());
	marketbase.NewLogUtil(this->GetLogName(),this->GetLogPathFileName());

	marketbase.SetPorcessor(processor);

	//程序启动后的第一次登录

	if(strcmp(processor,"main_zlqh")==0||strcmp(processor,"next_zlqh")==0)	{
		marketbase.InitialbyStrategy(strategyname);
		marketbase.FrontConnect();
		marketbase.LoginbyStrategy(strategyname);
	}
	else if(strcmp(processor,"main")==0||strcmp(processor,"next")==0){
		marketbase.InitialbyStrategy(strategyname);
		marketbase.FrontConnect();
		marketbase.LoginbyStrategy(strategyname);
	}
	else {
		marketbase.Initial();
		marketbase.FrontConnect();
		marketbase.Login();
	}

	SetMarketBase(&marketbase);
	datawrapper.start();

	bool submarket =false;
	if(!submarket){

		this->CheckContract(instrumentlists);

		marketbase.SubMarket(instrumentlists);
		submarket =true;
		sprintf(buf,"主力合约订阅完成 -->%ld %s    \n",instrumentlists.size(),dateutil.GetDateTimeString().c_str());
		this->GetLogUtil()->PrintLog(buf);
	}

	 //发送邮件
	marketbase.SendSelfCheckMarket(instrumentlists);

	printf("Market:Run   Ready for While ----------------->   \n");
	while(1){
//		printf("Ready for While -----------------> 000000  \n");
//		UpdateFlag();
		printf("Market:Run--> while 循环开始 %s    \n",dateutil.GetDateTimeString().c_str());
		sprintf(buf,"Market:Run--> while 循环开始 %s    \n",dateutil.GetDateTimeString().c_str());
		this->GetLogUtil()->PrintLog(buf);
		DateUtil dateutil;
		bool flag_sunday = dateutil.Check_Sunday();

		bool open_flag = dateutil.CheckMDOpenTime() && !flag_sunday;
//		bool open_flag=1;



		printf("Market:Run--> while 检测是否开盘时间  %s    \n",dateutil.GetDateTimeString().c_str());
		sprintf(buf,"Market:Run--> while 检测是否开盘时间 %s    \n",dateutil.GetDateTimeString().c_str());
		this->GetLogUtil()->PrintLog(buf);

		if (open_flag){
			printf("Market:Run-----------------> 开盘时间内 循环操作 %s    \n",dateutil.GetDateTimeString().c_str());
			sprintf(buf,"Market:Run-----------------> 开盘时间内 循环操作 %s    \n",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
			bool frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();  //网络连接成功
//			bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ; //网络断开连接
			bool login_not = ! this->GetCMdSpi()->GetFlagLogin();    //未登录
			bool noerrorcode = this->GetCMdSpi()->GetConnectErrorCode()==0;    //网络连接无错误码
			//订阅合约重置---->网络连接断开 FlagResetSubmarket才会重置设置为false, 订阅成功后FlagResetSubmarket设置未true
			bool Submarket_reset= this->GetCMdSpi()->GetFlagResetSubmarket();

//			bool Submarket_not= !this->GetCMdSpi()->GetFlagSubmarket();    //未订阅合约
			bool Submarket_ok=this->GetCMdSpi()->GetFlagSubmarket() ;    //订阅合约完成


			bool connect_error= !frontconnect_ok &&login_not &&!noerrorcode;   //网络连接断开
			bool readylogin = frontconnect_ok &&login_not && noerrorcode; // 网络连接成功,未登录
			bool readysubscribe = frontconnect_ok &&!login_not && noerrorcode && !Submarket_ok  ; //网络连接成功,完成登录 未订阅行情
			bool subscribe_ok = frontconnect_ok &&!login_not && noerrorcode && Submarket_ok ; //网络连接成功,完成登录 完成订阅行情


			printf("Market:Run-----------------> 开盘时间内 标志位读写准备完成 %s    \n",dateutil.GetDateTimeString().c_str());
			sprintf(buf,"Market:Run-----------------> 开盘时间内 标志位读写准备完成 %s    \n",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);

			if(connect_error){
				printf("Market:Run-----------------> 网络链接断开中!!!  %s \n",dateutil.GetDateTimeString().c_str());
				sprintf(buf,"Market:Run-----------------> 网络链接断开中!!!  %s \n",dateutil.GetDateTimeString().c_str());
				this->GetLogUtil()->PrintLog(buf);
				sleep(60);
			}
			else if (readylogin){
				printf("Market:Run-----------------> 网络链接恢复,准备登录!!!   \n");
				 sprintf (buf,"Market:Run-----------------> 盘中 %s 线路中断,重新登录开始!!!-----%s",processor,dateutil.GetDateTimeString().c_str());
				 this->GetLogUtil()->PrintLog(buf);

				 if(strcmp(processor,"main_zlqh")==0||strcmp(processor,"next_zlqh")==0)	{
					 sprintf (buf,"Market:Run-----------------> 盘中 %s 线路中断,zlqh 登录中-----%s",processor,dateutil.GetDateTimeString().c_str());
					 this->GetLogUtil()->PrintLog(buf);
					marketbase.UserLoginbyStrategy("CalendarSpreadTrade_zlqh");
				}
				else {
					sprintf (buf,"Market:Run-----------------> 盘中 %s 线路中断,登录中-----%s",processor,dateutil.GetDateTimeString().c_str());
					this->GetLogUtil()->PrintLog(buf);
					marketbase.UserLogin();
				}

				 sprintf (buf,"Market:Run-----------------> 盘中 %s 线路中断,重新登录完成!!!-----%s",processor,dateutil.GetDateTimeString().c_str());
				 this->GetLogUtil()->PrintLog(buf);
			}
			else if (readysubscribe  )	{
				printf("Market:Run-----------------> 网络链接恢复,完成登录,准备重新订阅行情!!!   \n");
				sprintf (buf,"Market:Run-----------------> 盘中 线路恢复成功,%s,重新登录完成,重新订阅行情!!!-----%s",processor,dateutil.GetDateTimeString().c_str());
				this->GetLogUtil()->PrintLog(buf);

				SubmarketAgain(instrumentlists,processor);

				sprintf (buf,"Market:Run-----------------> 盘中 线路恢复成功,%s,重新登录完成,重新订阅行情完成!!!-----%s",processor,dateutil.GetDateTimeString().c_str());
				this->GetLogUtil()->PrintLog(buf);
			}
			else if(subscribe_ok){

				printf("Market:Run-----------------> 网络链接ok,登录ok,订阅行情ok!!!   \n");

				sleep(60);
			}

			if(!login_not){
				printf ("Market:Run-----------------> %s  登录状态下 进行每日的定时任务检测!!!-----%s   \n",processor,dateutil.GetDateTimeString().c_str());
				 sprintf (buf,"Market:Run-----------------> %s  登录状态下 进行每日的定时任务检测!!!-----%s",processor,dateutil.GetDateTimeString().c_str());
				 this->GetLogUtil()->PrintLog(buf);
				 AutoUpdateSubmarket(instrumentlists,processor);
				 sleep(60);
			}
			sleep(2);
			printf("Market:Run-----------------> 开盘时间内 单次循环操作完成 %s    \n",dateutil.GetDateTimeString().c_str());
			sprintf(buf,"Market:Run-----------------> 开盘时间内 单次循环操作完成 %s    \n",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
		}
		else {
//
			printf("Market:Run-----------------> 非开盘时间 进入休眠 \n");

			DateUtil dateutil;
			char logbuf[1024];
			int waitseconds = dateutil.CheckWaitSecondsbyOpenTime();
			if(waitseconds>=3600){
				printf("Market:  网络链接断开!!!! ---->等待[3600]秒后重新链接！\n");
				sprintf(logbuf,"Market:  网络链接断开!!!! ---->等待[3600]秒后重新链接！  %s",dateutil.GetCurrentSqlTimeString().c_str());
				this->GetLogUtil()->WriteLog(logbuf);
				sleep(3600);
			}
			else if(waitseconds>0){
				printf("Market:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				sprintf(logbuf,"Market:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				this->GetLogUtil()->WriteLog(logbuf);
				sleep(waitseconds);
			}
			else{
				waitseconds=60;
				printf("Market:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s \n",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				sprintf(logbuf,"Market:  网络链接断开!!!! ---->等待[%d]秒后重新链接！%s ",waitseconds,dateutil.GetCurrentSqlTimeString().c_str());
				this->GetLogUtil()->WriteLog(logbuf);
				sleep(waitseconds);
			}

//			if ( ConnectRestore() )		{
//				printf("Market:Run-----------------> 非开盘时间 网络恢复 \n");
//			}
//
		}

	}

	 // 释放API实例
//	this->_pMDApi->Release();

	vector<std::string>().swap(instrumentlists);
	return;
}

void Market::pThread_Run(const char* processor)
{
	char buf[1024];
	std::string mslogname="thread";
	mslogname.append("_market.log");
	this->SetLogPathFileName(mslogname.c_str());

	std::string logname_market ="thread";
	logname_market.append("_market");
	this->SetLogName(logname_market.c_str());
	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->GetLogUtil()->PrintLog("行情读取程序启动-----------------");

	DateUtil dateutil;
	sprintf (buf,"行情读取MdService--->>> 启动------%s\n",dateutil.GetDateTimeString().c_str());
	this->GetLogUtil()->PrintLog(buf);

	RedisMinFlag redisminflag;     // redis select 0
	RedisDepthMarket redisdepthmarket;   //redis select 1

	if(strcmp(processor,"main")==0)		{
		redisminflag.FlushDB();
		redisdepthmarket.FlushDB();
	}
	else if(strcmp(processor,"next")==0){

	}
	else{

	}

	MysqlMdLogin mysqlmdlogin;
	mysqlmdlogin.CreateTable();
	vector<std::string>  instrumentlists;

	CMdSpi spi(this->_pMDApi);

	this->SetCMdSpi(&spi);

	this->GetCMdSpi()->SetRedisDepthMarket(&redisdepthmarket);


	MarketBase marketbase("test");
	marketbase.SetCThostFtdcMdApi(this->GetCThostFtdcMdApi());
	marketbase.SetCMdSpi(&spi);

	marketbase.Initial();
	marketbase.Login();

	SetMarketBase(&marketbase);


	InitialInstrumentLists(instrumentlists,processor);
	marketbase.SubMarket(instrumentlists);


printf("Ready for While ----------------->   \n");
	while(1)	{
//		printf("Ready for While -----------------> 000000  \n");
		UpdateFlag();

		if ( ConnectRestore() )		{
			 sprintf (buf,"MdService--->>> 线路中断,重新登录开始!!!-----%s",dateutil.GetDateTimeString().c_str());
			 this->GetLogUtil()->PrintLog(buf);
			 marketbase.Login();
			 sprintf (buf,"MdService--->>> 线路中断,重新登录完成!!!-----%s",dateutil.GetDateTimeString().c_str());
			 this->GetLogUtil()->PrintLog(buf);
		}

		if (LoginAgin()  )		{
			sprintf (buf,"MdService--->>> 线路恢复成功,,重新登录完成,重新订阅行情!!!-----%s",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
			SubmarketAgain(instrumentlists,processor);
			sprintf (buf,"MdService--->>> 线路恢复成功,,重新登录完成,重新订阅行情完成!!!-----%s",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
		}

		AutoUpdateSubmarket(instrumentlists,processor);
		sleep(3);

	}

	 // 释放API实例
//	this->_pMDApi->Release();

	vector<std::string>().swap(instrumentlists);
	return;
}
