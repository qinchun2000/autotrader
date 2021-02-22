#include "marketbase.hpp"


MarketBase::MarketBase(const char* strategyname) {

	this->SetStrategyName(strategyname);
	std::string logpathfilename="marketbase_";
	logpathfilename.append(strategyname);
	logpathfilename.append(".log");
	std::string path = "/var/log/autotrader/";
	this->SetLogPathFileName(path.c_str(),logpathfilename.c_str());

	std::string logname="marketbase_";
	logname.append(strategyname);
	this->SetLogName(logname.c_str());
	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());
}
MarketBase::~MarketBase()
{

}
void MarketBase::SetCThostFtdcMdApi(CThostFtdcMdApi *pmdapi)
{
	this->_pMDApi=pmdapi;
}
CThostFtdcMdApi * MarketBase::GetCThostFtdcMdApi()
{
	return this->_pMDApi;
}
void MarketBase::SetCMdSpi(CMdSpi *pmdspi)
{
	this->_pMDSpi=pmdspi;
}
CMdSpi * MarketBase::GetCMdSpi()
{
	return this->_pMDSpi;
}

void MarketBase::SetStrategyName(const char* strategyname)
{
	this->_strategyName=strategyname;
}

std::string MarketBase::GetStrategyName()
{
	return this->_strategyName;
}
void MarketBase::SetPorcessor(const char* processor)
{
	this->_processer=processor;
}

std::string MarketBase::GetPorcessor()
{
	return this->_processer;
}

void MarketBase::Initial()
{
		printf("登录:  MarketBase-->Initial -----> \n");


		char buf[1024];

	 // 注册一事件处理的实例
	    this->GetCThostFtdcMdApi()->RegisterSpi(this->GetCMdSpi());


		vector<std::string> list;
		Config config("config");
		config.GetMdFrontAddressLists(list);

		for (auto &item:list)	{
			char temp[item.length()];
			strcpy(temp,item.c_str());
			printf(" MD Address   ---> %s  \n",temp);
			this->GetCThostFtdcMdApi()->RegisterFront(temp);
		}
		vector<std::string>().swap(list);

}
void MarketBase::InitialbyStrategy(const char* strategy)
{
	printf("MarketBase::InitialbyStrategy  >>>登录:  MarketBase-->Initial -----> \n");


	char buf[1024];

 // 注册一事件处理的实例
	this->GetCThostFtdcMdApi()->RegisterSpi(this->GetCMdSpi());


	vector<std::string> list;
	Config config(strategy);
	config.GetMdFrontAddressLists(list);

	for (auto &item:list)	{
		char temp[item.length()];
		strcpy(temp,item.c_str());
		printf("MarketBase::InitialbyStrategy   >>> MD Address   ---> %s  \n",temp);
		this->GetCThostFtdcMdApi()->RegisterFront(temp);
	}
	vector<std::string>().swap(list);

}
void MarketBase::FrontConnect()
{
	char buf[1024];

	printf(" MarketBase::FrontConnect--->>> Begin!!!  \n");

	bool flag=false;
	while(!flag){

		printf(" MarketBase::FrontConnect--->>> loop begin -->  [%d] \n",this->GetCMdSpi()->GetConnectCount());
		// 使客户端开始与后台服务建立连接
		 if (this->GetCMdSpi()->GetConnectCount()==0){

			 printf(" MarketBase::FrontConnect--->>> 11111111111111!!!  \n");
			 int res=this->GetCMdSpi()->ResetSemInit();
			 printf(" MarketBase::FrontConnect--->>> 22222222222222222222!!!  \n");
			sprintf(buf," MarketBase--->>> 首次 Initialing MdApi!!!  ");
			this->GetLogUtil()->PrintLog(buf);
			 printf(" MarketBase::FrontConnect--->>> 333333333333333333!!!  \n");
			 this->GetCThostFtdcMdApi()->Init();
			 printf("MarketBase::FrontConnect-------sem_wait(&sem_init)   ------>> \n");
			 this->GetCMdSpi()->TimeWaitSem(this->GetCMdSpi()->GetSemInit());
			 printf("MarketBase::FrontConnect-------sem_wait(&sem_init)   <<------ \n");

		 }
		 else{
			 sprintf(buf," MarketBase--->>>  Initialing MdApi 已经发送，无需再次发送!!!  ");
		 }



		 sleep(5);

		 if (this->GetCMdSpi()->GetConnectCount()>0 && this->GetCMdSpi()->GetFlagFrontConnectFalg()){
			 flag=true;
			 printf(" MarketBase::FrontConnect--->>> loop true -->  [%d] \n",this->GetCMdSpi()->GetConnectCount());
		 }
		 else{
			 printf(" MarketBase::FrontConnect--->>> loop false -->  [%d] \n",this->GetCMdSpi()->GetConnectCount());
			 sleep(10);
		 }

	}

	printf(" MarketBase::FrontConnect--->>> End!!!  \n");
}
void MarketBase::UserLogin()
{
	char buf[1024];


  	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	printf("MarketBase::UserLogin--->>> Begin!!!  \n");
	sprintf(buf,"MarketBase::UserLogin--->>> Begin!!!  \n");
	this->GetLogUtil()->PrintLog(buf);
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	Config config("test");
	strcpy(req.BrokerID, config.GetBrokerID().c_str());
	strcpy(req.UserID, config.GetUserID().c_str());
	strcpy(req.Password, config.GetPasswrod().c_str());


	sleep(1);

	this->GetCMdSpi()->ResetSemLogin();

	 int ret=this->GetCThostFtdcMdApi()->ReqUserLogin(&req, 0);

	 if (ret==0){
		 printf("MarketBase::UserLogin--->>> 发送请求成功!!!  \n");
		 printf("MarketBase::UserLogin-------sem_wait(&sem_login)   ------>> \n");
		 this->GetCMdSpi()->TimeWaitSem(this->GetCMdSpi()->GetSemLogin());
		 printf("MarketBase::UserLogin-------sem_wait(&sem_login)   <<------ \n");
	 }
	 else{
		 printf("MarketBase::UserLogin--->>> 发送请求失败!!!  \n");
	 }

	 printf("MarketBase::UserLogin--->>> End!!!  \n");
	 sprintf(buf,"MarketBase::UserLogin--->>> End!!!  \n");
	 this->GetLogUtil()->PrintLog(buf);

}
void MarketBase::UserLoginbyStrategy(const char* strategy)
{
  	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	char buf[1024];

	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));

	Config config(strategy);
	strcpy(req.BrokerID, config.GetBrokerID().c_str());
	strcpy(req.UserID, config.GetUserID().c_str());
	strcpy(req.Password, config.GetPasswrod().c_str());

	printf(" --->>> UserLogin !!!   \n");
	sleep(1);

	this->GetCMdSpi()->ResetSemLogin();

//	this->GetCThostFtdcMdApi()->ReqUserLogin(&req, 0);
//
//	this->GetCMdSpi()->TimeWaitSem(this->GetCMdSpi()->GetSemLogin());
//	printf("<<<<-------sem_wait(&sem_login)   完成!!! \n");
//

	 int ret=this->GetCThostFtdcMdApi()->ReqUserLogin(&req, 0);

	 if (ret==0){
		 printf("MarketBase::UserLogin--->>> 发送请求成功!!!  \n");
		 printf("MarketBase::UserLogin-------sem_wait(&sem_login)   ------>> \n");
		 this->GetCMdSpi()->TimeWaitSem(this->GetCMdSpi()->GetSemLogin());
		 printf("MarketBase::UserLogin-------sem_wait(&sem_login)   <<------ \n");
	 }
	 else{
		 printf("MarketBase::UserLogin--->>> 发送请求失败!!!  \n");
	 }

	 printf("MarketBase::UserLogin--->>> End!!!  \n");
	 sprintf(buf,"MarketBase::UserLogin--->>> End!!!  \n");
	 this->GetLogUtil()->PrintLog(buf);
}
void MarketBase::Login()
{
	printf("MarketBase::Login ----->Begin!!! \n");

	int loopflag=false;
	while(!loopflag)	{

		bool frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();
		bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ;
		bool login_not = ! this->GetCMdSpi()->GetFlagLogin();
		bool noerrorcode = this->GetCMdSpi()->GetConnectErrorCode()==0;

		bool readylogin = frontconnect_ok&& frontdisconnect_was &&login_not && noerrorcode;

		if(readylogin)		{
			printf("MarketBase::Login ----->Ready Login!!! \n");
			//登录
			this->UserLogin();

			frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();
			frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ;
			bool login_fin = this->GetCMdSpi()->GetFlagLogin();
			noerrorcode = this->GetCMdSpi()->GetConnectErrorCode()==0;
			bool loginok = frontconnect_ok&& frontdisconnect_was &&login_fin && noerrorcode;

			if (loginok){
				loopflag=true;
				printf("MarketBase::Login ----->Login 确认登录成功!!! \n");
			}
			else{
				printf("MarketBase::Login ----->Login 确认登录失败!!! 休眠1秒!!!\n");

				if(!frontconnect_ok){
					printf("MarketBase::Login ----->frontconnect_ok 确认登录失败!!! \n");
				}
				if(!frontdisconnect_was){
					printf("MarketBase::Login ----->frontdisconnect_was 确认登录失败!!! \n");
				}
				if(!login_fin){
					printf("MarketBase::Login ----->login_fin 确认登录失败!!! \n");
				}
				if(!noerrorcode){
					printf("MarketBase::Login ----->noerrorcode 确认登录失败!!! \n");
				}
				sleep(1);
			}

		}
		else{
			printf("MarketBase::Login ----->Not Ready Login!!! 休眠1秒!!! \n");
			sleep(1);
		}

	}
	printf("MarketBase::Login ----->End!!! \n");
}

void MarketBase::LoginbyStrategy(const char* strategy)
{
	printf("MarketBase::Login ----->Begin!!! \n");

	int loopflag=false;
	while(!loopflag)	{

		bool frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();
		bool frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ;
		bool login_not = ! this->GetCMdSpi()->GetFlagLogin();
		bool noerrorcode = this->GetCMdSpi()->GetConnectErrorCode()==0;

		bool readylogin = frontconnect_ok&& frontdisconnect_was &&login_not && noerrorcode;

		if(readylogin)		{
			printf("MarketBase::Login ----->Ready Login!!! \n");
			//登录
			this->UserLoginbyStrategy(strategy);

			frontconnect_ok = this->GetCMdSpi()->GetFlagFrontConnectFalg();
			frontdisconnect_was =! this->GetCMdSpi()->GetFlagFrontDisConnect() ;
			bool login_fin = this->GetCMdSpi()->GetFlagLogin();
			noerrorcode = this->GetCMdSpi()->GetConnectErrorCode()==0;
			bool loginok = frontconnect_ok&& frontdisconnect_was &&login_fin && noerrorcode;

			if (loginok){
				loopflag=true;
				printf("MarketBase::Login ----->Login 确认登录成功!!! \n");
			}
			else{
				printf("MarketBase::Login ----->Login 确认登录失败!!! 休眠1秒!!!\n");

				if(!frontconnect_ok){
					printf("MarketBase::Login ----->frontconnect_ok 确认登录失败!!! \n");
				}
				if(!frontdisconnect_was){
					printf("MarketBase::Login ----->frontdisconnect_was 确认登录失败!!! \n");
				}
				if(!login_fin){
					printf("MarketBase::Login ----->login_fin 确认登录失败!!! \n");
				}
				if(!noerrorcode){
					printf("MarketBase::Login ----->noerrorcode 确认登录失败!!! \n");
				}
				sleep(1);
			}

		}
		else{
			printf("MarketBase::Login ----->Not Ready Login!!! 休眠1秒!!! \n");
			sleep(1);
		}

	}
	printf("MarketBase::Login ----->End!!! \n");
}
void MarketBase::SubMarket(vector<std::string> &instrumentlists)
{

	this->GetLogUtil()->PrintLog("MarketBase::SubMarket-----------------begin");

	 char *pinstrumentID[instrumentlists.size()];

//	for (auto &item:instrumentlists)
//	{
//		printf("ProductID -->  %s   \n",item.c_str());
//
//	}
//	 MysqlMinData mysqlmindata;
	printf ("MarketBase::SubMarket--->>> pSubscribeMarketData\n");

	int k=0;
	for (auto &item:instrumentlists)
	{
//		printf("订阅合约ppInstrumentID -->  %s   \n",item.c_str());
		char *char_instrumentid= new char[10];
		strcpy(char_instrumentid,item.c_str());
		pinstrumentID[k]=char_instrumentid;
		k++;
	}

	this->GetCMdSpi()->ResetSemSubscribe();

	this->GetCThostFtdcMdApi()->SubscribeMarketData(pinstrumentID,instrumentlists.size());
	printf("MarketBase::SubMarket-----sem_wait(&sem_subscribe)   ------>>>>>\n");
	this->GetCMdSpi()->TimeWaitSem(this->GetCMdSpi()->GetSemSubscribe());
	printf("MarketBase::SubMarket<<<<-----sem_wait(&sem_subscribe)   完成!!! \n");

	for (int i =0; i < instrumentlists.size(); i++)
	   delete [] pinstrumentID[i];

	this->GetLogUtil()->PrintLog("MarketBase::SubMarket-----------------end!!!!");
}


void MarketBase::UnSubMarket(vector<std::string> &instrumentlists)
{
	 char buf[1024];

	this->GetLogUtil()->PrintLog("MarketBase::UnSubMarket-----------------begin");

	sprintf (buf,"MarketBase::UnSubMarket--->>> ------%ld",instrumentlists.size());
	this->GetLogUtil()->PrintLog(buf);
	char *pinstrumentID[instrumentlists.size()];

	int i=0;
	for (auto &item:instrumentlists)	{
//		sprintf(buf,"取消订阅合约 -->  %s   ",item.c_str());
//		this->GetLogUtil()->PrintLog(buf);
		char *char_instrumentid= new char[10];
		strcpy(char_instrumentid,item.c_str());
		pinstrumentID[i]=char_instrumentid;
		i++;
	}

	sprintf(buf,"MarketBase::UnSubMarket -->begin    ");
	this->GetLogUtil()->PrintLog(buf);

	this->GetCMdSpi()->ResetSemUnSubscribe();

	int result =this->GetCThostFtdcMdApi()->UnSubscribeMarketData(pinstrumentID,instrumentlists.size());
	if (result==0)	{
		printf("MarketBase::UnSubMarket successed！  :\n");
		sprintf(buf,"MarketBase::UnSubMarket -->request send ok!!!    ");
		this->GetLogUtil()->PrintLog(buf);

	}
	else	{
		printf("MarketBase::UnSubMarket failed！  :\n");
		 sprintf(buf,"MarketBase::UnSubMarket -->request send failed!!!    ");
		this->GetLogUtil()->PrintLog(buf);
	}
	printf("MarketBase::UnSubMarket sem_wait(&sem_unsubscribe)   --->>>> \n");
	this->GetCMdSpi()->TimeWaitSem(this->GetCMdSpi()->GetSemUnSubscribe());
	printf("MarketBase::UnSubMarket <<<<<<------sem_wait(&sem_unsubscribe)   \n");
	for (int i =0; i < instrumentlists.size(); i++)
				   delete [] pinstrumentID[i];
	this->GetLogUtil()->PrintLog("MarketBase::UnSubMarket-----------------end");
}

void MarketBase::SendSelfCheckMarket(vector<std::string> &instrumentlists)
{

	DateUtil dateutil;
	char buf[1024];
	std::string title;

	MysqlUser mysqluser;
	User userinfo=mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());


	if(instrumentlists.size()>0)
	{
		sprintf(buf,"%s-%s-%s-行情订阅表-[%ld]", userinfo.UserID,this->GetPorcessor().c_str(),dateutil.GetTodayString().c_str(),instrumentlists.size());
		title=buf;
	}
	else
	{
		sprintf(buf,"%s-%s-%s-行情订阅表-[空]", userinfo.UserID,this->GetPorcessor().c_str(),dateutil.GetTodayString().c_str());
		title=buf;
	}



	std::string content ="hi: \n";
	content.append(" \n");

	sprintf(buf,"---->>> pSubscribeMarketData --> 交易日[%s]   \n",this->GetCMdSpi()->GetTradingDay().c_str());
	content.append(buf);


	for (auto &item:instrumentlists)
	{
		sprintf(buf,"订阅合约ppInstrumentID -->  %s   \n",item.c_str());
		content.append(buf);
	}

	content.append(" \n");
	std::string hostName;
	std::string Ip;
	std::string tradername;
	CodeUtil codeutil;
	bool ret = codeutil.GetHostInfo(hostName, Ip);
	if (true == ret) {
//		std::cout << "hostname: " << hostName << std::endl;
//		std::cout << "Ip: " << Ip << std::endl;
		tradername = hostName;
		tradername.append("  \n");
		content.append(tradername);

	}
	else{
		content.append("MarketBase   \n");
	}
	sprintf(buf,"%s \n",dateutil.GetDateTimeString().c_str());
	content.append(buf);

}
