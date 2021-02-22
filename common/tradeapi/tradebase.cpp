#include "tradebase.hpp"


TradeBase::TradeBase() {
	std::cout<<"TradeBase 无参构造开始!"<<std::endl;
	// 设置用户名 用户密码
//	SetUserLoginField();

	std::cout<<"TradeBase 无参构造完成!"<<std::endl;
}

TradeBase::~TradeBase() {

	char logbuf[1024];
	if(this->m_pTraderApi!=nullptr)
	{
		this->UserLogout();
		m_pTraderApi->RegisterSpi(NULL);
		m_pTraderApi->Release();
		m_pTraderApi = nullptr;

		sprintf(logbuf,"TradeBase 析构函数---> 释放指针traderapi");
		this->GetLogUtil()->WriteLog(logbuf);
	}
	if(m_pTraderSpi!=nullptr){
		delete m_pTraderSpi;
		m_pTraderSpi=nullptr;
		sprintf(logbuf,"TradeBase 析构函数---> m_pTraderSpi");
		this->GetLogUtil()->WriteLog(logbuf);
	}

	if(m_mytdspi!=nullptr){
		delete m_mytdspi;
		sprintf(logbuf,"TradeBase 析构函数---> 释放指针m_mytdspi");
		this->GetLogUtil()->WriteLog(logbuf);
	}

	if(m_pmysqlasset!=nullptr){
		delete m_pmysqlasset;
	}

	if(m_pmysqlinvestposition!=nullptr){
		delete m_pmysqlinvestposition;
	}

}

void TradeBase::SetDataWrapper(DataWrapper *point){
	this->m_pdatawrapper=point;
}
DataWrapper* TradeBase::GetDataWrapper(){
	return this->m_pdatawrapper;
}
void TradeBase::SetMysqlAsset(MysqlAsset *mysqlasset)
{
	this->m_pmysqlasset=mysqlasset;

}
MysqlAsset* TradeBase::GetMysqlAsset()
{
	return this->m_pmysqlasset;
}


void TradeBase::SetMysqlInvestPosition(MysqlInvestPosition *mysqlinvestp)
{
	this->m_pmysqlinvestposition=mysqlinvestp;

}
MysqlInvestPosition* TradeBase::GetMysqlInvestPosition()
{
	return this->m_pmysqlinvestposition;
}


void TradeBase::SetUserLoginField()
{
	printf("TradeBase::SetUserLoginField()   begin    \n");
	memset(&m_userLoginField, 0, sizeof(m_userLoginField));
	MysqlUser mysqluser;
	Config config(this->GetStrategyName().c_str());

	if(mysqluser.Exist_ActiveDataByStrategyName(this->GetStrategyName().c_str()))	{
		User user = mysqluser.Find_ActiveDatabyStrategyName(this->GetStrategyName().c_str());
		if(strcmp(user.UserID,config.GetUserID().c_str())==0){


			//用户名相同  策略相同
			if (strcmp(user.StrategyName,config.GetStrategyName().c_str())==0){
				strcpy(m_userLoginField.BrokerID, user.BrokerID);
				strcpy(m_userLoginField.UserID, user.UserID);
				strcpy(m_userLoginField.Password, user.Password);

				printf("TradeBase::SetUserLoginField()    MysqlUser库已经存在用户及策略信息  user: %s  策略 %s \n",
						user.UserID,this->GetStrategyName().c_str());
			}
			//用户名相同 策略不同
			else{
				printf("TradeBase::SetUserLoginField()   MysqlUser库中 用户%s相同   策略 %s不同 ---> 更新为  用户%s   策略 %s   \n",
						user.UserID,this->GetStrategyName().c_str(),config.GetUserID().c_str(),this->GetStrategyName().c_str());

				user.Actived =true;

				strcpy(user.BrokerID, config.GetBrokerID().c_str());
				strcpy(user.UserID, config.GetUserID().c_str());
				strcpy(user.InvestorID, config.GetUserID().c_str());
				strcpy(user.Password, config.GetPasswrod().c_str());
				strcpy(user.StrategyName,config.GetStrategyName().c_str());

				mysqluser.Update(user);
				//
				strcpy(m_userLoginField.BrokerID, config.GetBrokerID().c_str());
				strcpy(m_userLoginField.UserID, config.GetUserID().c_str());
				strcpy(m_userLoginField.Password, config.GetPasswrod().c_str());
			}
		}
		else{
			//用户名不同        策略相同
			if (strcmp(user.StrategyName,config.GetStrategyName().c_str())==0){
				printf("TradeBase::SetUserLoginField()   MysqlUser库中 用户%s不同   策略 %s相同 ---> 更新为  用户%s   策略 %s   \n",
						user.UserID,this->GetStrategyName().c_str(),config.GetUserID().c_str(),this->GetStrategyName().c_str());

				user.Actived =true;

				strcpy(user.BrokerID, config.GetBrokerID().c_str());
				strcpy(user.UserID, config.GetUserID().c_str());
				strcpy(user.InvestorID, config.GetUserID().c_str());
				strcpy(user.Password, config.GetPasswrod().c_str());
				strcpy(user.StrategyName,config.GetStrategyName().c_str());

				mysqluser.UpdatebyStrategyName(user);
				//
				strcpy(m_userLoginField.BrokerID, config.GetBrokerID().c_str());
				strcpy(m_userLoginField.UserID, config.GetUserID().c_str());
				strcpy(m_userLoginField.Password, config.GetPasswrod().c_str());
			}
			// 用户名不同  策略不同
			else{
				printf("TradeBase::SetUserLoginField()    MysqlUser库已经存在用户及策略信息  user: %s不同   策略 %s 不同 \n",
										user.UserID,this->GetStrategyName().c_str());

				printf("TradeBase::SetUserLoginField()   新策略 %s  新用户%s !!! \n",
						this->GetStrategyName().c_str(),config.GetUserID().c_str());
				printf("TradeBase::SetUserLoginField()   用户%s  策略:  %s   新策略新用户 插入user库 \n",
						config.GetUserID().c_str(),this->GetStrategyName().c_str());
				User user;
				strcpy(user.BrokerID, config.GetBrokerID().c_str());
				strcpy(user.UserID, config.GetUserID().c_str());
				strcpy(user.InvestorID, config.GetUserID().c_str());
				strcpy(user.Password, config.GetPasswrod().c_str());
				strcpy(user.StrategyName,this->GetStrategyName().c_str());
				user.Actived =true;

				mysqluser.Insert(user);

				strcpy(m_userLoginField.BrokerID, config.GetBrokerID().c_str());
				strcpy(m_userLoginField.UserID, config.GetUserID().c_str());
				strcpy(m_userLoginField.Password, config.GetPasswrod().c_str());
			}


		}
	}
	else{
		if(!mysqluser.Exist_DataByStrategyName(this->GetStrategyName().c_str()))	{


			std::string filename=this->GetStrategyName().c_str();
			Config other(filename.c_str());
			if (!mysqluser.Exist_DatabyUserID(other.GetUserID().c_str()) )		{
				printf("TradeBase::SetUserLoginField()   新策略 %s  新用户%s !!! \n",
						this->GetStrategyName().c_str(),other.GetUserID().c_str());
				printf("TradeBase::SetUserLoginField()   用户%s  策略:  %s   新策略新用户 插入user库 \n",
						other.GetUserID().c_str(),this->GetStrategyName().c_str());
				User user;
				strcpy(user.BrokerID, other.GetBrokerID().c_str());
				strcpy(user.UserID, other.GetUserID().c_str());
				strcpy(user.InvestorID, other.GetUserID().c_str());
				strcpy(user.Password, other.GetPasswrod().c_str());
				strcpy(user.StrategyName,this->GetStrategyName().c_str());
				user.Actived =true;

				mysqluser.Insert(user);

				strcpy(m_userLoginField.BrokerID, other.GetBrokerID().c_str());
				strcpy(m_userLoginField.UserID, other.GetUserID().c_str());
				strcpy(m_userLoginField.Password, other.GetPasswrod().c_str());
			}
			else	{

				User user = mysqluser.Find_DatabyUserID(other.GetUserID().c_str());

				if(strcmp(user.StrategyName,this->GetStrategyName().c_str())==0)	{

					printf("TradeBase::SetUserLoginField()   已经存在用户:%s   已经绑定新策略: %s    \n",
								user.UserID,this->GetStrategyName().c_str());
					user.Actived =true;
					strcpy(user.StrategyName,this->GetStrategyName().c_str());
					mysqluser.Update(user);

					strcpy(m_userLoginField.BrokerID, user.BrokerID);
					strcpy(m_userLoginField.UserID, user.UserID);
					strcpy(m_userLoginField.Password, user.Password);
				}
				else{
					printf("TradeBase::SetUserLoginField()   新策略 %s  对应的用户%s已经绑定其他策略%s \n",
											this->GetStrategyName().c_str(),user.UserID,user.StrategyName);
					printf("TradeBase::SetUserLoginField()   原用户:%s  原策略:%s  >>> 原用户:%s 保持不变  绑定新策略:%s   !!! \n",
							user.UserID,user.StrategyName,user.UserID,this->GetStrategyName().c_str());
					user.Actived =true;
					strcpy(user.StrategyName,this->GetStrategyName().c_str());
					mysqluser.Update(user);

					strcpy(m_userLoginField.BrokerID, user.BrokerID);
					strcpy(m_userLoginField.UserID, user.UserID);
					strcpy(m_userLoginField.Password, user.Password);
				}
			}
		}
		else{

			// 待调整
			printf("TradeBase::SetUserLoginField()   调用数据库---未激活绑定策略  %s 用户信息    激活    \n",this->GetStrategyName().c_str());
			User user = mysqluser.Find_DatabyStrategyName(this->GetStrategyName().c_str());
			user.Actived =true;
			mysqluser.Update(user);
			strcpy(m_userLoginField.BrokerID, user.BrokerID);
			strcpy(m_userLoginField.UserID, user.UserID);
			strcpy(m_userLoginField.Password, user.Password);
		}
	}
	printf(" SetUserLoginField   broker:  %s    \n",m_userLoginField.BrokerID);
	printf(" SetUserLoginField   user:  %s    \n",m_userLoginField.UserID);
	printf("TradeBase::SetUserLoginField()   end    \n");
}
void TradeBase::SetUserLoginField(const char* name)
{
	memset(&m_userLoginField, 0, sizeof(m_userLoginField));
	Config config(this->GetStrategyName().c_str());

		printf(" SetUserLoginField   user:  %s    \n",config.GetUserID().c_str());
		strcpy(m_userLoginField.BrokerID, config.GetBrokerID().c_str());
			strcpy(m_userLoginField.UserID, config.GetUserID().c_str());
			strcpy(m_userLoginField.Password, config.GetPasswrod().c_str());


}
CThostFtdcReqUserLoginField TradeBase::GetUserLoginField()
{
	return this->m_userLoginField;
}

CThostFtdcReqUserLoginField *TradeBase::GetPointUserLoginField()
{
//	printf("TradeBase::GetPointUserLoginField   userid  --->%s    \n",m_userLoginField.UserID);
	return &this->m_userLoginField;
}

std::string TradeBase::GetBrokerID()
{
	return this->m_userLoginField.BrokerID;
}
std::string TradeBase::GetUserID()
{
	return this->m_userLoginField.UserID;
}

void TradeBase::SetUserLogoutField()
{
	memset(&m_userLogoutField, 0, sizeof(m_userLogoutField));
	cout<< "TradeBase setuserlogout field.. "<< this->GetStrategyName().c_str() <<endl ;
	Config config(this->GetStrategyName().c_str());
	cout << "-------------"<<endl;

	strcpy(m_userLogoutField.BrokerID, config.GetBrokerID().c_str());
	strcpy(m_userLogoutField.UserID, config.GetUserID().c_str());
}

CThostFtdcUserLogoutField *TradeBase::GetPointUserLogoutField()
{
	return &this->m_userLogoutField;
}

void TradeBase::SetCThostFtdcTraderApi(CThostFtdcTraderApi *ptraderapi)
{

	this->m_pTraderApi=ptraderapi;

}


CThostFtdcTraderApi * TradeBase::GetCThostFtdcTraderApi()
{

	return this->m_pTraderApi;

}

void TradeBase::SetCTraderSpi(CTraderSpi *ptraderspi)
{

	this->m_pTraderSpi=ptraderspi;

}


CTraderSpi * TradeBase::GetCTraderSpi()
{

	return this->m_pTraderSpi;

}

void TradeBase::SetFlagTradeLimit(bool flag)
{
this->m_flagtradelimit=flag;
}
bool TradeBase::GetFlagTradeLimit()
{
return this->m_flagtradelimit;
}
void TradeBase::SetFlagInvestUpdate(bool flag)
{
this->m_flaginvestupdate=flag;
}
bool TradeBase::GetFlagInvestUpdate()
{
return this->m_flaginvestupdate;
}

void TradeBase::SetRequestID(int requestid)
{
this->m_requestID=requestid;
}
int TradeBase::UpdateRequestID()
{
return this->m_requestID++;
}

int TradeBase::GetRequestID()
{
return this->m_requestID;
}
void TradeBase::SetMaxOrderRef(int maxorderref)
{
this->m_MaxOrderRef=maxorderref;
}
int TradeBase::UpdateMaxOrderRef()
{
//	char ORDER_REF[13];

	MysqlTradingDay mysqltradingday(this->GetUserID().c_str());
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
//strcpy(ORDER_REF,tradingdaydata.MaxOrderRef);
	this->m_MaxOrderRef = atoi(tradingdaydata.MaxOrderRef);
	return this->m_MaxOrderRef++;
}

int TradeBase::GetMaxOrderRef()
{
return this->m_MaxOrderRef;
}

std::string TradeBase::AutoIncretMaxOrderRef()
{
//	Config config(this->GetStrategyName().c_str());
	char logbuf[1024];

	MysqlTradingDay mysqltradingday(this->GetPointUserLoginField()->UserID);
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	char ORDER_REF[13];

	int ref = atoi(tradingdaydata.MaxOrderRef);

	SetMaxOrderRef(ref+1);

	// 将数字转为字符
	sprintf(ORDER_REF, "%d", this->GetMaxOrderRef());

	//写入日志
	sprintf(logbuf, "AutoIncretMaxOrderRef: 更新TradingDay Update_MaxOrderRef  ---> %s", ORDER_REF);
	this->GetLogUtil()->PrintLog(logbuf);

	printf("AutoIncretMaxOrderRef: 更新TradingDay Update_MaxOrderRef  TradingDateTime:%s --> ORDER_REF: %s \n",tradingdaydata.TradingDateTime,ORDER_REF);
	//
	mysqltradingday.UpdateMaxOrderRefbyDate(ORDER_REF,tradingdaydata.TradingDateTime);

	sprintf(logbuf,"AutoIncretMaxOrderRef: 更新TradingDay Update_MaxOrderRef %s  \n",tradingdaydata.TradingDateTime);
	this->GetLogUtil()->PrintLog(logbuf);

	return ORDER_REF;
}


void TradeBase::SetNextOrderRef(int nref)
{
this->m_iNextOrderRef=nref;
}
int TradeBase::UpdateNextOrderRef()
{

	MysqlTradingDay mysqltradingday(this->GetUserLoginField().UserID);
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	char ORDER_REF[13];
	this->m_iNextOrderRef = atoi(tradingdaydata.MaxOrderRef);
	return this->m_iNextOrderRef++;
}
int TradeBase::GetNextOrderRef()
{
return this->m_iNextOrderRef;
}

char TradeBase::GetCombOffsetFlagbyCloseOrder(DepthMarket *depthmarket,HoldData *item)
{
	char CombOffsetFlag;

	//上海交易所 需要区分平今天 平昨
	if(strcmp(depthmarket->ExchangeID,"SHFE")==0)		{
		if(strcmp(item->OpenDate,this->GetTradingDay().c_str())==0)		{
			CombOffsetFlag=THOST_FTDC_OF_CloseToday;
		}
		else	{
			CombOffsetFlag=THOST_FTDC_OF_CloseYesterday;
		}
	}
	else		{
		 CombOffsetFlag=THOST_FTDC_OF_Close;   // 0 : 开仓  1: 平仓
	}

	return CombOffsetFlag;
}
//bool TradeBase::EmptyMainContractMap()
//{
//	bool ret=false;
//	try {
//		m_maincontractmap.clear();
//		ret=true;
//	} catch(std::logic_error&) {
//		std::cout << "[exception caught TradeBase::EmptyMainContractMap >>> ]\n";
//		ret=false;
//	}
//	return ret;
//}
//bool TradeBase::InsertMainContractMap(MainContractData &data)
//{
//
//	bool ret=false;
//	try {
//		std::string key =data.InstrumentID;
//		m_maincontractmap.insert(map<string,MainContractData>::value_type(key,data));
//		ret=true;
//
//	} catch(std::logic_error&) {
//		std::cout << "[exception caught TradeBase::InsertMainContractMap >>> ]\n";
//		ret=false;
//	}
//	return ret;
//}
//std::map<std::string,MainContractData> * TradeBase::GetMainContractMap()
//{
//	return &m_maincontractmap;
//}
void TradeBase::CTP_Initial()
{
	// 注册一事件处理的实例
	this->GetCThostFtdcTraderApi()->RegisterSpi(this->GetCTraderSpi());
	// 订阅私有流
	// TERT_RESTART:从本交易日开始重传
	// TERT_RESUME:从上次收到的续传
	// TERT_QUICK:只传送登录后私有流的内容
	this->GetCThostFtdcTraderApi()->SubscribePrivateTopic(THOST_TERT_RESUME);
	///订阅公共流。
	///@param nResumeType 公共流重传方式
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	this->GetCThostFtdcTraderApi()->SubscribePublicTopic(THOST_TERT_RESTART);

	vector<std::string> list;
	Config config(this->GetStrategyName().c_str());
	config.GetFrontAddressLists(list);

	for (auto &address:list)	{
		char temp[address.length()];
		strcpy(temp,address.c_str());
		printf(" %s   ---> %s  \n",address.c_str(),temp);
		// 设置交易托管系统服务的地址，可以注册多个地址备用
		this->GetCThostFtdcTraderApi()->RegisterFront(temp);
	}
	vector<std::string>().swap(list);

	this->GetCTraderSpi()->ResetSemInit();
	printf("CTP_Initial  Ready   --->>>>:\n");

	sleep(2);
	while (!this->GetCTraderSpi()->GetFlagFrontConnectFalg())	{
		printf("CTP_Initial 未完成   --->>>>:\n");
		this->GetCThostFtdcTraderApi()->Init();
		// 等待服务器发出登录消息
		printf("CTP_Initial ---- sem_wait(&sem_init) ----->>>>  \n");
		this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemInit());
	}
	printf("<<<<<--------CTP_Initial 完成 ！！！！！  \n");
}
std::string  TradeBase::CheckFlowPath(){

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
	printf("checkflow  --->>>>end  \n");
	return flowpath;
}
void TradeBase::CTP_AutoInitial()
{

	CodeUtil codeutil;

	std::string flowpath=this->CheckFlowPath();
	printf("CTP_AutoInitial  --->>>>创建ctp api进程 %s  \n",flowpath.c_str());
//	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());
	this->m_pTraderApi =  CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());

	if(m_pdatawrapper!=nullptr){
		// 产生一个事件处理的实例
			this->m_pTraderSpi = new CTraderSpi(m_pdatawrapper,m_pTraderApi);
			this->m_pTraderSpi->SetStrategyName(this->GetStrategyName().c_str());
	}
	else{
		printf(" ------------> nullptr \n");
	}


//	printf("1111111111111111111111111 \n");
//	std::string spilogfilename="traderspi";
//	spilogfilename.append("_");
//	spilogfilename.append(this->GetPointUserLoginField()->UserID);
//	spilogfilename.append(".log");
//	this->m_pTraderSpi->SetLogName(this->GetPointUserLoginField()->UserID);
//	this->m_pTraderSpi->SetLogPathFileName(spilogfilename.c_str());

// 注册一事件处理的实例
	this->m_pTraderApi->RegisterSpi(m_pTraderSpi);
	// 订阅私有流
	// TERT_RESTART:从本交易日开始重传
	// TERT_RESUME:从上次收到的续传
	// TERT_QUICK:只传送登录后私有流的内容
	this->m_pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	///订阅公共流。
	///@param nResumeType 公共流重传方式
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	this->m_pTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);

	vector<std::string> list;
	Config config(this->GetStrategyName().c_str());
	config.GetFrontAddressLists(list);

	for (auto &address:list)	{
		char temp[address.length()];
		strcpy(temp,address.c_str());
		printf("CTP_AutoInitial %s   ---> %s  \n",address.c_str(),temp);
		// 设置交易托管系统服务的地址，可以注册多个地址备用
		this->m_pTraderApi->RegisterFront(temp);
	}
	vector<std::string>().swap(list);

}
void TradeBase::CTP_AutoInitial_MD()
{
	CodeUtil codeutil;
	std::string flowpath=this->CheckFlowPath();
	printf("CTP_AutoInitial_MD  --->>>>创建ctp api进程 %s  \n",flowpath.c_str());
//	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());
	this->m_pTraderApi =  CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());

	if(m_pdatawrapper!=nullptr){
		// 产生一个事件处理的实例
			this->m_pTraderSpi = new CTraderSpi(m_pdatawrapper,m_pTraderApi);

			// spi 处理 区分 纯粹的行情订阅请求 和  交易端的数据准备请求 ！！！！！
			m_pTraderSpi->SetMDStatus(true);
			this->m_pTraderSpi->SetStrategyName(this->GetStrategyName().c_str());

			// 注册一事件处理的实例
			this->m_pTraderApi->RegisterSpi(m_pTraderSpi);
			// 订阅私有流
			// TERT_RESTART:从本交易日开始重传
			// TERT_RESUME:从上次收到的续传
			// TERT_QUICK:只传送登录后私有流的内容
			this->m_pTraderApi->SubscribePrivateTopic(THOST_TERT_RESUME);
			///订阅公共流。
			///@param nResumeType 公共流重传方式
			///        THOST_TERT_RESTART:从本交易日开始重传
			///        THOST_TERT_RESUME:从上次收到的续传
			///        THOST_TERT_QUICK:只传送登录后公共流的内容
			///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
			this->m_pTraderApi->SubscribePublicTopic(THOST_TERT_RESTART);

			vector<std::string> list;
			Config config(this->GetStrategyName().c_str());
			config.GetFrontAddressLists(list);

			for (auto &address:list)	{
				char temp[address.length()];
				strcpy(temp,address.c_str());
				printf("CTP_AutoInitial_MD %s   ---> %s  \n",address.c_str(),temp);
				// 设置交易托管系统服务的地址，可以注册多个地址备用
				this->m_pTraderApi->RegisterFront(temp);
			}
			vector<std::string>().swap(list);
	}
	else{
		printf("error  CTP_AutoInitial_MD------------>m_pdatawrapper: nullptr \n");
	}



}
void TradeBase::CTP_Set()
{
	// 注册一事件处理的实例
	this->GetCThostFtdcTraderApi()->RegisterSpi(this->GetCTraderSpi());
	// 订阅私有流
	// TERT_RESTART:从本交易日开始重传
	// TERT_RESUME:从上次收到的续传
	// TERT_QUICK:只传送登录后私有流的内容
	this->GetCThostFtdcTraderApi()->SubscribePrivateTopic(THOST_TERT_RESUME);
	///订阅公共流。
	///@param nResumeType 公共流重传方式
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	this->GetCThostFtdcTraderApi()->SubscribePublicTopic(THOST_TERT_RESTART);

	vector<std::string> list;
	Config config(this->GetStrategyName().c_str());
	config.GetFrontAddressLists(list);

	for (auto &address:list)
	{
		char temp[address.length()];
		strcpy(temp,address.c_str());
		printf("TradeBase::CTP_Set %s   ---> %s  \n",address.c_str(),temp);
		// 设置交易托管系统服务的地址，可以注册多个地址备用
		this->GetCThostFtdcTraderApi()->RegisterFront(temp);
	}
	vector<std::string>().swap(list);



}
void TradeBase::FrontConnect()
{

	struct timeval tv;
	gettimeofday(&tv,NULL);
//	printf("second:%ld\n",tv.tv_sec);  //秒
//	printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
	long s =tv.tv_sec*1000 + tv.tv_usec/1000;

	char buf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	bool flag=false;
	while(!flag){

		printf(" TradeBase::FrontConnect--->>> loop begin -->  [%d] \n",this->GetCTraderSpi()->GetConnectCount());
		// 使客户端开始与后台服务建立连接
		 if (this->GetCTraderSpi()->GetConnectCount()==0){

			 int res=this->GetCTraderSpi()->ResetSemInit();
			sprintf(buf," TradeBase::FrontConnect--->>> 首次 Initialing MdApi!!!  ");
			this->GetLogUtil()->PrintLog(buf);
			sleep(2);
			printf("TradeBase::FrontConnect 尝试连接------->>>>\n");
			this->GetCThostFtdcTraderApi()->Init();

				// 等待服务器发出登录消息

			 printf("TradeBase::FrontConnect-------sem_wait(&sem_init)   ------>> \n");
			 this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemInit());
			 printf("TradeBase::FrontConnect-------sem_wait(&sem_init)   <<------ \n");

		 }
		 else{
			 sprintf(buf," TradeBase::FrontConnect--->>>  Initialing MdApi 已经发送，无需再次发送!!!  ");
		 }

		 sleep(5);

		 if (this->GetCTraderSpi()->GetConnectCount()>0 && this->GetCTraderSpi()->GetFlagFrontConnectFalg()){
			 flag=true;

			struct timeval tv_last;
			gettimeofday(&tv_last,NULL);
	 //	    printf("second:%ld\n",tv_last.tv_sec);  //秒
	 //	    printf("millisecond:%ld\n",tv_last.tv_sec*1000 + tv_last.tv_usec/1000);  //毫秒

			long l =tv_last.tv_sec*1000 + tv_last.tv_usec/1000;

			 printf(" TradeBase::FrontConnect--->>> loop true -->  [%d]   网络链路连接成功,响应时间[%ld]毫秒  %ld \n",
					 this->GetCTraderSpi()->GetConnectCount(),l-s,tv_last.tv_sec-tv.tv_sec);
		 }
		 else{
			 printf(" TradeBase::FrontConnect--->>> loop false -->  [%d] \n",this->GetCTraderSpi()->GetConnectCount());
			 sleep(10);
		 }

	}

}
void TradeBase::UserLogin()
{
	 // 初始化线程同步变量
	int rest=this->GetCTraderSpi()->ResetSemLogin();

	 if(rest) {//初始化信号量失败
		perror("TradeBase::UserLogin --->Semaphore initialization failed\n");
		exit(EXIT_FAILURE);
	}
//	 printf("TradeBase::UserLogin   尝试Login--->>>>等待3秒\n");
	sleep(3);

//	printf("TradeBase::UserLogin   尝试Login--->>>>\n");

	CThostFtdcReqUserLoginField userLoginField;
	memset(&userLoginField, 0, sizeof(userLoginField));
	Config config(this->GetStrategyName().c_str());
	strcpy(userLoginField.BrokerID, config.GetBrokerID().c_str());
	strcpy(userLoginField.UserID, config.GetUserID().c_str());
	strcpy(userLoginField.Password, config.GetPasswrod().c_str());
	strcpy(userLoginField.UserProductInfo,config.GetProductInfo().c_str());

	printf("TradeBase::UserLogin   配置文件策略名:%s--->>>> \n"
			"TradeBase::UserLogin   BrokerID:%s \n"
			"TradeBase::UserLogin   UserID:%s  \n"
			"TradeBase::UserLogin   UserProductInfo:%s \n",
			this->GetStrategyName().c_str(),userLoginField.BrokerID,
			userLoginField.UserID,userLoginField.UserProductInfo);

	int result_login=this->GetCThostFtdcTraderApi()->ReqUserLogin(&userLoginField, this->UpdateRequestID());
	if (result_login==0)				{
//		printf("TradeBase::UserLogin 发送登录请求成功！  :\n");
		// 等待登录成功消息
//		printf("TradeBase::UserLogin  sem_wait(&sem_login)   Begin-------->>>> \n");
		int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemLogin());
//		printf("TradeBase::UserLogin  sem_wait(&sem_login)   <<<<--------End \n");


		if (this->GetCTraderSpi()->GetFlagLogin())	{
//			printf("TradeBase::UserLogin   <<<<<<-------登录成功后，确认登录标志GetFlagLogin--->成功！！！ \n");
//			sleep(3);
			this->UpdateDifSec();
			this->UpdateTradingDay();
			this->UpdateTradingDaySqlTime();
			printf("TradeBase::UserLogin   <<<<<<-------完成登录,本地<---->远程时间差 [%d]秒 \n",this->GetDifSec());
		}
		else	{
			printf("TradeBase::UserLogin   <<<<<<-------登录成功后，确认登录标志GetFlagLogin--->失败！！！ \n");
		}

	}
	else				{
		printf("TradeBase::UserLogin ReqUserLogin failed！  :\n");
//		sleep(2);

		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (result_login==-1){
			printf("TradeBase::UserLogin failed！ [-1] 网络连接失败!!! \n");
		}
		else if(result_login==-2){
			printf("TradeBase::UserLogin failed！ [-2] 未处理请求超过许可数!!! \n");
		}
		else if(result_login==-3){
			printf("TradeBase::UserLogin failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("TradeBase::UserLogin failed！ [%d] 未知故障!!! \n",result_login);
		}

	}

}

//void TradeBase::Login()
//{
//
//
//	while( !this->GetCTraderSpi()->GetFlagLogin())
//	   {
//			// 使客户端开始与后台服务建立连接
//			if(this->GetCTraderSpi()->GetConnectErrorCode()>=4097)
//				{
//					printf("登录:TradeBase::Login 网络不可用 =>  %d  每隔10秒重新尝试 \n",this->GetCTraderSpi()->GetConnectErrorCode());
//					sleep(10);
//					this->FrontConnect();
//					sleep(5);
//				}
//			else if ( !this->GetCTraderSpi()->GetFlagFrontConnectFalg())
//				{
//				printf("登录:TradeBase::Login FrontConnect --> 未连接,尝试连接! \n");
//				this->FrontConnect();
//				sleep(5);
//				}
//			else{
//				printf("登录:TradeBase::Login FrontConnect --> 连接建立!!!  \n");
//				sleep(3);
//				printf("登录:TradeBase::Login FrontConnect --> 连接建立!!!  等待5秒 确认网络正常! \n");
//			}
//
//			if(this->GetCTraderSpi()->GetFlagFrontConnectFalg() && this->GetCTraderSpi()->GetConnectErrorCode()<4097)
//			{
//				//登录
//				this->UserLogin();
//			}
//			else{
//				printf("登录:TradeBase::Login ---> UserLogin   登录前信号检测未通过!   \n");
//			}
//
//
//	   }
//
//	this->UpdateDifSec();
//
//}

void TradeBase::FirstLogin()
{
	char logbuf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	printf("TradeBase::FirstLogin ----->Begin!!! \n");

	bool loopflag=false;
	while(!loopflag)	{

		bool frontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();
//		bool frontdisconnect_was =! this->GetCTraderSpi()->GetFlagFrontDisConnect() ;
		bool login_ok = this->GetCTraderSpi()->GetFlagLogin();
		bool noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;

		bool readylogin = frontconnect_ok  &&!login_ok && noerrorcode;

		if(readylogin)		{
			printf("TradeBase::FirstLogin ----->Ready Login!!! \n");
			//登录
			this->UserLogin();

			frontconnect_ok = this->GetCTraderSpi()->GetFlagFrontConnectFalg();
//			frontdisconnect_was =! this->GetCTraderSpi()->GetFlagFrontDisConnect() ;
			login_ok = this->GetCTraderSpi()->GetFlagLogin();
			noerrorcode = this->GetCTraderSpi()->GetConnectErrorCode()==0;

			bool loginok = frontconnect_ok  &&login_ok && noerrorcode;

			if (loginok){
				loopflag=true;
				printf("TradeBase::FirstLogin ----->Login 确认登录成功!!! \n");
			}
			else{
//				printf("TradeBase::FirstLogin ----->Login 确认登录失败!!! 休眠1秒!!!\n");

				if(!frontconnect_ok){
					printf("TradeBase::FirstLogin ----->frontconnect_ok 确认登录失败!!! \n");
					sprintf(logbuf,"TradeBase::FirstLogin ----->frontconnect_ok 确认登录失败!!! ");
					this->GetLogUtil()->WriteLog(logbuf);
				}
//				if(!frontdisconnect_was){
//					printf("TradeBase::FirstLogin ----->frontdisconnect_was 确认登录失败!!! \n");
//				}
				if(!login_ok){
					printf("TradeBase::FirstLogin ----->login_fin 确认登录失败!!! \n");
					sprintf(logbuf,"TradeBase::FirstLogin ----->login_fin 确认登录失败!!! ");
					this->GetLogUtil()->WriteLog(logbuf);
				}
				if(!noerrorcode){
					printf("TradeBase::FirstLogin----->noerrorcode 确认登录失败!!!   [%d]  \n",
							this->GetCTraderSpi()->GetConnectErrorCode());
					sprintf(logbuf,"TradeBase::FirstLogin----->noerrorcode 确认登录失败!!!   [%d]  ",
												this->GetCTraderSpi()->GetConnectErrorCode());
					this->GetLogUtil()->WriteLog(logbuf);
				}
				sleep(1);
			}

		}
		else{
			printf("TradeBase::FirstLogin ----->Not Ready Login!!! 休眠1秒!!! \n");
			sleep(3);
		}

	}
	printf("TradeBase::FirstLogin ----->End!!! \n");
}
void TradeBase::UserLogout()
{

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	this->GetCTraderSpi()->ResetSemLogout();
	// 拷贝用户登录信息到登出信息
	strcpy(this->m_userLogoutField.BrokerID,this->GetBrokerID().c_str());
	strcpy(this->m_userLogoutField.UserID,this->GetUserID().c_str());

printf("TradeBase: UserLogout begin   brokerid: %s   userid: %s！  :\n",this->m_userLogoutField.BrokerID,this->m_userLogoutField.UserID);
	sleep(1);
	int result=this->GetCThostFtdcTraderApi()->ReqUserLogout(this->GetPointUserLogoutField(), this->UpdateRequestID());

	if (result==0)	{

		// 等待登出成功

		printf("<<<<<<---- sem_wait(&sem_logout)   :\n");

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
		//	ret = sem_timedwait(&sem_logout,&outtime);
		int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemLogout());
		printf("sem_wait(&sem_logout)   --->>>>:\n");

		this->SetFlagTradeLimit(false);

		// 释放API实例
	//	this->GetCThostFtdcTraderApi()->Release();
	}
	else{

		printf("TradeBase: UserLogout failed！  :\n");
				sprintf(logbuf,"Debug: UserLogout failed！  :\n");
				this->GetLogUtil()->WriteLog(logbuf);

				if (result==-1){
					printf("TradeBase::UserLogout failed！ [-1] 网络连接失败!!! \n");
					sprintf(logbuf,"TradeBase::UserLogout failed！ [-1] 网络连接失败!!! \n");
					this->GetLogUtil()->WriteLog(logbuf);
				}
				else if(result==-2){
					printf("TradeBase::UserLogout failed！ [-2] 未处理请求超过许可数!!! \n");
					sprintf(logbuf,"TradeBase::UserLogout failed！ [-2] 未处理请求超过许可数!!! \n");
					this->GetLogUtil()->WriteLog(logbuf);
					this->SetFlagTradeLimit(true);
				}
				else if(result==-3){
					printf("TradeBase::UserLogout failed！ [-3] 每秒发送请求数超过许可数!!! \n");
					sprintf(logbuf,"TradeBase::UserLogout failed！ [-3] 每秒发送请求数超过许可数!!! \n");
					this->GetLogUtil()->WriteLog(logbuf);
				}
				else{
					printf("TradeBase::UserLogout failed！ [%d] 未知故障!!! \n",result);
					sprintf(logbuf,"TradeBase::UserLogout failed！ [%d] 未知故障!!! \n",result);
					this->GetLogUtil()->WriteLog(logbuf);
				}

	}




}
void TradeBase::ReleaseTradeApi()
{


	this->GetCTraderSpi()->ResetSemLogout();
	sleep(1);
	this->GetCThostFtdcTraderApi()->ReqUserLogout(this->GetPointUserLogoutField(), this->UpdateRequestID());

	// 等待登出成功

	printf("<<<<<<---- sem_wait(&sem_logout)   :\n");

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
//	ret = sem_timedwait(&sem_logout,&outtime);
	int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemLogout());
	printf("sem_wait(&sem_logout)   --->>>>:\n");


	// 释放API实例
	this->GetCThostFtdcTraderApi()->Release();



}
void TradeBase::Close()
{
	this->GetCTraderSpi()->ResetSemLogout();
	sleep(1);
	this->GetCThostFtdcTraderApi()->ReqUserLogout(this->GetPointUserLogoutField(), this->UpdateRequestID());

	// 等待登出成功

	printf("<<<<<<---- sem_wait(&sem_logout)   :\n");

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
//	ret = sem_timedwait(&sem_logout,&outtime);
	int ret=this->GetCTraderSpi()->TimeWaitSem(this->GetCTraderSpi()->GetSemLogout());


	if(ret==0){

		printf("sem_wait(&sem_logout)   --->>>>:\n");
		this->m_pTraderApi->RegisterSpi(NULL);
		// 释放API实例
		this->GetCThostFtdcTraderApi()->Release();
		this->m_pTraderApi=nullptr;
		if(this->m_pTraderSpi!=nullptr){
			delete this->m_pTraderSpi;
			this->m_pTraderSpi=nullptr;
		}

		printf("TradeBase:: --->>>>Close CTP Connect !!!\n");
	}
	else	{
		printf("TradeBase::UserLogin ReqUserLogin failed！  :\n");
//		sleep(2);

		//-1，表示网络连接失败；
		//-2，表示未处理请求超过许可数；
		//-3，表示每秒发送请求数超过许可数。

		if (ret==-1){
			printf("TradeBase::UserLogin failed！ [-1] 网络连接失败!!! \n");
		}
		else if(ret==-2){
			printf("TradeBase::UserLogin failed！ [-2] 未处理请求超过许可数!!! \n");
		}
		else if(ret==-3){
			printf("TradeBase::UserLogin failed！ [-3] 每秒发送请求数超过许可数!!! \n");
		}
		else{
			printf("TradeBase::UserLogin failed！ [%d] 未知故障!!! \n",ret);
		}

	}



}
void TradeBase::ReqOrderInsertbySignal(Signal *psignal,const char* exchangeid,double price,int volume)
{
	char logbuf[2048];


	sprintf(logbuf,"ReqOrderInsertbySignal:请求委托下单-----Begin!!!   %s   %s  %.2f  %d ",psignal->SingalName,exchangeid,price,volume);
	this->GetLogUtil()->WriteLog(logbuf);



	////////////////////////////////////////////////////////////////////////////////////////////////
	///报单录入请求
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcInputOrderField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));


	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
	//////////////////////////////////////////////////////////
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserID().c_str());
	//////////////////////////////////////////////////////////
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(requestData.InstrumentID,psignal->InstrumentID);
	//////////////////////////////////////////////////////////
	///报单引用 TThostFtdcOrderRefType char[13]
	strcpy(requestData.OrderRef,this->AutoIncretMaxOrderRef().c_str());
	//////////////////////////////////////////////////////////
	///用户代码 TThostFtdcUserIDType char[16]
	strcpy(requestData.UserID,this->GetUserID().c_str());

	strcpy(requestData.ExchangeID,exchangeid);

	//////////////////////////////////////////////////////////
	///报单价格条件 TThostFtdcOrderPriceTypeType char
	//// THOST_FTDC_OPT_AnyPrice '1' 任意价
	//// THOST_FTDC_OPT_LimitPrice '2' 限价
	//// THOST_FTDC_OPT_BestPrice '3' 最优价
	//// THOST_FTDC_OPT_LastPrice '4' 最新价
	//// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 最新价浮动上浮1个ticks
	//// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 最新价浮动上浮2个ticks
	//// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 最新价浮动上浮3个ticks
	//// THOST_FTDC_OPT_AskPrice1 '8' 卖一价
	//// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 卖一价浮动上浮1个ticks
	//// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 卖一价浮动上浮2个ticks
	//// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 卖一价浮动上浮3个ticks
	//// THOST_FTDC_OPT_BidPrice1 'C' 买一价
	//// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 买一价浮动上浮1个ticks
	//// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 买一价浮动上浮2个ticks
	//// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 买一价浮动上浮3个ticks
	requestData.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	//////////////////////////////////////////////////////////
	///买卖方向 TThostFtdcDirectionType char
	//// THOST_FTDC_D_Buy '0' 买
	//// THOST_FTDC_D_Sell '1' 卖
	requestData.Direction = psignal->Direction+'0';
	//////////////////////////////////////////////////////////
	///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
	//// THOST_FTDC_OF_Open '0' 开仓
	//// THOST_FTDC_OF_Close '1' 平仓
	//// THOST_FTDC_OF_ForceClose '2' 强平
	//// THOST_FTDC_OF_CloseToday '3' 平今
	//// THOST_FTDC_OF_CloseYesterday '4' 平昨
	//// THOST_FTDC_OF_ForceOff '5' 强减
	//// THOST_FTDC_OF_LocalForceClose '6' 本地强平
	requestData.CombOffsetFlag[0] = '0';
	//////////////////////////////////////////////////////////
	///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
	//// THOST_FTDC_HF_Speculation '1' 投机
	//// THOST_FTDC_HF_Arbitrage '2' 套利
	//// THOST_FTDC_HF_Hedge '3' 套保
	requestData.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	//////////////////////////////////////////////////////////
	///价格 TThostFtdcPriceType double
	requestData.LimitPrice = price;
	//////////////////////////////////////////////////////////
	///数量 TThostFtdcVolumeType int
	requestData.VolumeTotalOriginal = volume;
	//////////////////////////////////////////////////////////
	///有效期类型 TThostFtdcTimeConditionType char
	//// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
	//// THOST_FTDC_TC_GFS '2' 本节有效
	//// THOST_FTDC_TC_GFD '3' 当日有效
	//// THOST_FTDC_TC_GTD '4' 指定日期前有效
	//// THOST_FTDC_TC_GTC '5' 撤销前有效
	//// THOST_FTDC_TC_GFA '6' 集合竞价有效
	requestData.TimeCondition = THOST_FTDC_TC_GFD;
	//////////////////////////////////////////////////////////
	///GTD日期 TThostFtdcDateType char[9]
	strcpy(requestData.GTDDate,this->GetTradingDay().c_str());
	//////////////////////////////////////////////////////////
	///成交量类型 TThostFtdcVolumeConditionType char
	//// THOST_FTDC_VC_AV '1' 任何数量
	//// THOST_FTDC_VC_MV '2' 最小数量
	//// THOST_FTDC_VC_CV '3' 全部数量
	requestData.VolumeCondition = THOST_FTDC_VC_AV;
	//////////////////////////////////////////////////////////
	///最小成交量 TThostFtdcVolumeType int
	requestData.MinVolume = 1;
	//////////////////////////////////////////////////////////
	///触发条件 TThostFtdcContingentConditionType char
	//// THOST_FTDC_CC_Immediately '1' 立即
	//// THOST_FTDC_CC_Touch '2' 止损
	//// THOST_FTDC_CC_TouchProfit '3' 止赢
	//// THOST_FTDC_CC_ParkedOrder '4' 预埋单
	//// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 最新价大于条件价
	//// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 最新价大于等于条件价
	//// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 最新价小于条件价
	//// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 最新价小于等于条件价
	//// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 卖一价大于条件价
	//// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 卖一价大于等于条件价
	//// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 卖一价小于条件价
	//// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 卖一价小于等于条件价
	//// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 买一价大于条件价
	//// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 买一价大于等于条件价
	//// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 买一价小于条件价
	//// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 买一价小于等于条件价
	requestData.ContingentCondition = THOST_FTDC_CC_Immediately;
	//////////////////////////////////////////////////////////
	///止损价 TThostFtdcPriceType double
	requestData.StopPrice = psignal->StopPrice;
	//////////////////////////////////////////////////////////
	///强平原因 TThostFtdcForceCloseReasonType char
	//// THOST_FTDC_FCC_NotForceClose '0' 非强平
	//// THOST_FTDC_FCC_LackDeposit '1' 资金不足
	//// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
	//// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
	//// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
	//// THOST_FTDC_FCC_Violation '5' 违规
	//// THOST_FTDC_FCC_Other '6' 其它
	//// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
	requestData.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	//////////////////////////////////////////////////////////
	///自动挂起标志 TThostFtdcBoolType int
	requestData.IsAutoSuspend = 0;   //否
	//////////////////////////////////////////////////////////
	///业务单元 TThostFtdcBusinessUnitType char[21]
//	    strcpy(requestData.BusinessUnit,"");
	//////////////////////////////////////////////////////////
	///请求编号 TThostFtdcRequestIDType int
	requestData.RequestID = this->UpdateRequestID();
	//////////////////////////////////////////////////////////
	///用户强评标志 TThostFtdcBoolType int
	requestData.UserForceClose = 0;  //否
	//////////////////////////////////////////////////////////
	///互换单标志 TThostFtdcBoolType int
	    requestData.IsSwapOrder = 0;
	//////////////////////////////////////////////////////////


	OrderData info;
	strcpy(info.SignalName,psignal->SingalName);
	strcpy(info.InstrumentID,psignal->InstrumentID);
	strcpy(info.ExchangeID,exchangeid);
	info.Direction=requestData.Direction-'0';
	info.CombOffsetFlag=requestData.CombOffsetFlag[0]-'0';
	info.CombHedgeFlag=requestData.CombHedgeFlag[0]-'0';
	info.LimitPrice = requestData.LimitPrice;
	info.VolumeTotalOriginal = requestData.VolumeTotalOriginal;
	info.StopPrice = psignal->StopPrice;
	info.StopRange = psignal->StopRange;
	info.WinPrice= psignal->WinPrice;
	strcpy(info.TradingDay,psignal->TradingDay);
	strcpy(info.OrderRef,requestData.OrderRef);
	e_OrderStatus eos=sendout;
	info.Status=eos;

	this->GetDataWrapper()->InsertOpenOrderMap(info);

	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqOrderInsert(&requestData,this->UpdateRequestID());

	if (result==0)	{
		printf("Debug: ReqOrderInsertbySignal successed！  :\n");

		sprintf(logbuf,"ReqOrderInsertbySignal:请求委托下单成功-----pinstrumentid: %s   price : %.3f ",info.InstrumentID,info.LimitPrice);
		this->GetLogUtil()->WriteLog(logbuf);
		sprintf(logbuf,"ReqOrderInsertbySignal:请求委托下单成功-----tradingdayinfo.TradingDay: %s",this->GetTradingDay().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
	}
	else{
		e_OrderStatus eos=ordererror;
		info.Status=eos;
		this->GetDataWrapper()->UpdateOpenOrderMap(info);
		printf("Debug: ReqOrderInsertbySignal failed！  :\n");
		sprintf(logbuf,"请求委托下单-----ReqOrderInsertbySignal failed！ ---TradingDay: %s",this->GetTradingDay().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
	}

	sprintf(logbuf,"ReqOrderInsertbySignal:请求委托下单-----End!!!  ");
	this->GetLogUtil()->WriteLog(logbuf);

}
//void TradeBase::ReqSqlOrderInsert(const char *signalname,
//		const char *pinstrumentid,
//		const char* exchangeid,
//		char directiontype,
//		char CombOffsetFlag,
//		double price,
//		int volume,
//		double stopprice)
//{
//	char logbuf[1024];
//
//
//	this->GetLogUtil()->WriteLog("请求委托下单-----ReqSqlOrderInsert!!!");
//
//	////////////////////////////////////////////////////////////////////////////////////////////////
//	///报单录入请求
//	///////////////////////////////////////////////////////////////////////////////////////////////
//	// 定义调用API的数据结构
//	CThostFtdcInputOrderField requestData;
//	// 确保没有初始化的数据不会被访问
//	memset(&requestData,0,sizeof(requestData));
//	// 为调用结构题设置参数信息
//	///经纪公司代码 TThostFtdcBrokerIDType char[11]
//	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
//	//////////////////////////////////////////////////////////
//	///投资者代码 TThostFtdcInvestorIDType char[13]
//	strcpy(requestData.InvestorID,this->GetUserID().c_str());
//	//////////////////////////////////////////////////////////
//	///合约代码 TThostFtdcInstrumentIDType char[31]
//	strcpy(requestData.InstrumentID,pinstrumentid);
//	//////////////////////////////////////////////////////////
//	///报单引用 TThostFtdcOrderRefType char[13]
//	strcpy(requestData.OrderRef,this->AutoIncretMaxOrderRef().c_str());
//	//////////////////////////////////////////////////////////
//	///用户代码 TThostFtdcUserIDType char[16]
//	strcpy(requestData.UserID,this->GetUserID().c_str());
//	strcpy(requestData.ExchangeID,exchangeid);
//	//////////////////////////////////////////////////////////
//	///报单价格条件 TThostFtdcOrderPriceTypeType char
//	//// THOST_FTDC_OPT_AnyPrice '1' 任意价
//	//// THOST_FTDC_OPT_LimitPrice '2' 限价
//	//// THOST_FTDC_OPT_BestPrice '3' 最优价
//	//// THOST_FTDC_OPT_LastPrice '4' 最新价
//	//// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 最新价浮动上浮1个ticks
//	//// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 最新价浮动上浮2个ticks
//	//// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 最新价浮动上浮3个ticks
//	//// THOST_FTDC_OPT_AskPrice1 '8' 卖一价
//	//// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 卖一价浮动上浮1个ticks
//	//// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 卖一价浮动上浮2个ticks
//	//// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 卖一价浮动上浮3个ticks
//	//// THOST_FTDC_OPT_BidPrice1 'C' 买一价
//	//// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 买一价浮动上浮1个ticks
//	//// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 买一价浮动上浮2个ticks
//	//// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 买一价浮动上浮3个ticks
//	requestData.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
//	//////////////////////////////////////////////////////////
//	///买卖方向 TThostFtdcDirectionType char
//	//// THOST_FTDC_D_Buy '0' 买
//	//// THOST_FTDC_D_Sell '1' 卖
//	requestData.Direction = directiontype;
//	//////////////////////////////////////////////////////////
//	///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
//	//// THOST_FTDC_OF_Open '0' 开仓
//	//// THOST_FTDC_OF_Close '1' 平仓
//	//// THOST_FTDC_OF_ForceClose '2' 强平
//	//// THOST_FTDC_OF_CloseToday '3' 平今
//	//// THOST_FTDC_OF_CloseYesterday '4' 平昨
//	//// THOST_FTDC_OF_ForceOff '5' 强减
//	//// THOST_FTDC_OF_LocalForceClose '6' 本地强平
//	requestData.CombOffsetFlag[0] = CombOffsetFlag;
//	//////////////////////////////////////////////////////////
//	///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
//	//// THOST_FTDC_HF_Speculation '1' 投机
//	//// THOST_FTDC_HF_Arbitrage '2' 套利
//	//// THOST_FTDC_HF_Hedge '3' 套保
//	requestData.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
//	//////////////////////////////////////////////////////////
//	///价格 TThostFtdcPriceType double
//	requestData.LimitPrice = price;
//	//////////////////////////////////////////////////////////
//	///数量 TThostFtdcVolumeType int
//	requestData.VolumeTotalOriginal = volume;
//	//////////////////////////////////////////////////////////
//	///有效期类型 TThostFtdcTimeConditionType char
//	//// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
//	//// THOST_FTDC_TC_GFS '2' 本节有效
//	//// THOST_FTDC_TC_GFD '3' 当日有效
//	//// THOST_FTDC_TC_GTD '4' 指定日期前有效
//	//// THOST_FTDC_TC_GTC '5' 撤销前有效
//	//// THOST_FTDC_TC_GFA '6' 集合竞价有效
//	requestData.TimeCondition = THOST_FTDC_TC_GFD;
//	//////////////////////////////////////////////////////////
//	///GTD日期 TThostFtdcDateType char[9]
//	strcpy(requestData.GTDDate,this->GetTradingDay().c_str());
//	//////////////////////////////////////////////////////////
//	///成交量类型 TThostFtdcVolumeConditionType char
//	//// THOST_FTDC_VC_AV '1' 任何数量
//	//// THOST_FTDC_VC_MV '2' 最小数量
//	//// THOST_FTDC_VC_CV '3' 全部数量
//	requestData.VolumeCondition = THOST_FTDC_VC_AV;
//	//////////////////////////////////////////////////////////
//	///最小成交量 TThostFtdcVolumeType int
//	requestData.MinVolume = 0;
//	//////////////////////////////////////////////////////////
//	///触发条件 TThostFtdcContingentConditionType char
//	//// THOST_FTDC_CC_Immediately '1' 立即
//	//// THOST_FTDC_CC_Touch '2' 止损
//	//// THOST_FTDC_CC_TouchProfit '3' 止赢
//	//// THOST_FTDC_CC_ParkedOrder '4' 预埋单
//	//// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 最新价大于条件价
//	//// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 最新价大于等于条件价
//	//// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 最新价小于条件价
//	//// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 最新价小于等于条件价
//	//// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 卖一价大于条件价
//	//// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 卖一价大于等于条件价
//	//// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 卖一价小于条件价
//	//// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 卖一价小于等于条件价
//	//// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 买一价大于条件价
//	//// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 买一价大于等于条件价
//	//// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 买一价小于条件价
//	//// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 买一价小于等于条件价
//	requestData.ContingentCondition = THOST_FTDC_CC_Immediately;
//	//////////////////////////////////////////////////////////
//	///止损价 TThostFtdcPriceType double
//	requestData.StopPrice = stopprice;
//	//////////////////////////////////////////////////////////
//	///强平原因 TThostFtdcForceCloseReasonType char
//	//// THOST_FTDC_FCC_NotForceClose '0' 非强平
//	//// THOST_FTDC_FCC_LackDeposit '1' 资金不足
//	//// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
//	//// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
//	//// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
//	//// THOST_FTDC_FCC_Violation '5' 违规
//	//// THOST_FTDC_FCC_Other '6' 其它
//	//// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
//	requestData.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
//	//////////////////////////////////////////////////////////
//	///自动挂起标志 TThostFtdcBoolType int
//	requestData.IsAutoSuspend = 0;   //否
//	//////////////////////////////////////////////////////////
//	///业务单元 TThostFtdcBusinessUnitType char[21]
////	    strcpy(requestData.BusinessUnit,"");
//	//////////////////////////////////////////////////////////
//	///请求编号 TThostFtdcRequestIDType int
//	requestData.RequestID = this->UpdateRequestID();
//	//////////////////////////////////////////////////////////
//	///用户强评标志 TThostFtdcBoolType int
//	requestData.UserForceClose = 0;  //否
//	//////////////////////////////////////////////////////////
//	///互换单标志 TThostFtdcBoolType int
////	    requestData.IsSwapOrder = 0;
//	//////////////////////////////////////////////////////////
//
//
//	OrderData info;
//	strcpy(info.SignalName,signalname);
//	strcpy(info.InstrumentID,pinstrumentid);
//	strcpy(info.ExchangeID,exchangeid);
//	info.Direction=requestData.Direction-'0';
//	info.CombOffsetFlag=requestData.CombOffsetFlag[0]-'0';
//	info.CombHedgeFlag=requestData.CombHedgeFlag[0]-'0';
//	info.LimitPrice = requestData.LimitPrice;
//	info.VolumeTotalOriginal = requestData.VolumeTotalOriginal;
//	info.StopPrice = stopprice;
//
//	strcpy(info.TradingDay,this->GetTradingDay().c_str());
//	strcpy(info.OrderRef,requestData.OrderRef);
//
//	sprintf(logbuf,"请求委托下单-----pinstrumentid: %s OrderRef: %s  SignalName: %s",pinstrumentid,info.OrderRef,info.SignalName);
//	this->GetLogUtil()->WriteLog(logbuf);
//	sprintf(logbuf,"请求委托下单-----tradingdayinfo.TradingDay: %s",this->GetTradingDay().c_str());
//	this->GetLogUtil()->WriteLog(logbuf);
//
//	Config config(this->GetStrategyName().c_str());
//	MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
//
//	if(!mysqlopenorder.Exist_Table()){
//		mysqlopenorder.CreateTable();
//	}
//
//	info.Status=0;
//	if(!mysqlopenorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,this->GetTradingDay().c_str())){
//		mysqlopenorder.Insert(info);
//		sprintf(logbuf,"请求委托下单-----插入order: %s Status: %d",info.InstrumentID,info.Status);
//		this->GetLogUtil()->WriteLog(logbuf);
//	}
//	else{
//		sprintf(logbuf,"请求委托下单-----存在order 无法插入数据: %s OrderRef: %s",info.InstrumentID,info.OrderRef);
//		this->GetLogUtil()->WriteLog(logbuf);
//	}
//
//
//
//	// 调用API,并等待响应函数返回
//	int result = this->GetCThostFtdcTraderApi()->ReqOrderInsert(&requestData,this->UpdateRequestID());
//
//	if (result==0)		{
//		printf("Debug: ReqSqlOrderInsert successed！  :\n");
//
//
//		sprintf(logbuf,"请求委托下单-----插入order: %s Status: %d",pinstrumentid,info.Status);
//		this->GetLogUtil()->WriteLog(logbuf);
//	}
//	else		{
//		printf("Debug: ReqSqlOrderInsert failed！  :\n");
//	}
//
//}

void TradeBase::ReqSqlCloseOrderInsert(const char *signalname,
		const char *pinstrumentid,
		const char *exchangeid,
		char directiontype,
		char CombOffsetFlag,
		double price,
		int volume,
		double stopprice)
{
	char logbuf[1024];


	this->GetLogUtil()->WriteLog("平仓请求委托下单-----ReqSqlCloseOrderInsert!!!");

	////////////////////////////////////////////////////////////////////////////////////////////////
	///报单录入请求
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcInputOrderField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetUserLoginField().BrokerID);
	//////////////////////////////////////////////////////////
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserLoginField().UserID);
	//////////////////////////////////////////////////////////
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(requestData.InstrumentID,pinstrumentid);
	//////////////////////////////////////////////////////////
	///报单引用 TThostFtdcOrderRefType char[13]
	strcpy(requestData.OrderRef,this->AutoIncretMaxOrderRef().c_str());
	//////////////////////////////////////////////////////////
	///用户代码 TThostFtdcUserIDType char[16]
	strcpy(requestData.UserID,this->GetUserLoginField().UserID);
	strcpy(requestData.ExchangeID,exchangeid);
	//////////////////////////////////////////////////////////
	///报单价格条件 TThostFtdcOrderPriceTypeType char
	//// THOST_FTDC_OPT_AnyPrice '1' 任意价
	//// THOST_FTDC_OPT_LimitPrice '2' 限价
	//// THOST_FTDC_OPT_BestPrice '3' 最优价
	//// THOST_FTDC_OPT_LastPrice '4' 最新价
	//// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 最新价浮动上浮1个ticks
	//// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 最新价浮动上浮2个ticks
	//// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 最新价浮动上浮3个ticks
	//// THOST_FTDC_OPT_AskPrice1 '8' 卖一价
	//// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 卖一价浮动上浮1个ticks
	//// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 卖一价浮动上浮2个ticks
	//// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 卖一价浮动上浮3个ticks
	//// THOST_FTDC_OPT_BidPrice1 'C' 买一价
	//// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 买一价浮动上浮1个ticks
	//// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 买一价浮动上浮2个ticks
	//// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 买一价浮动上浮3个ticks
	requestData.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	//////////////////////////////////////////////////////////
	///买卖方向 TThostFtdcDirectionType char
	//// THOST_FTDC_D_Buy '0' 买
	//// THOST_FTDC_D_Sell '1' 卖
	requestData.Direction = directiontype;
	//////////////////////////////////////////////////////////
	///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
	//// THOST_FTDC_OF_Open '0' 开仓
	//// THOST_FTDC_OF_Close '1' 平仓
	//// THOST_FTDC_OF_ForceClose '2' 强平
	//// THOST_FTDC_OF_CloseToday '3' 平今
	//// THOST_FTDC_OF_CloseYesterday '4' 平昨
	//// THOST_FTDC_OF_ForceOff '5' 强减
	//// THOST_FTDC_OF_LocalForceClose '6' 本地强平
	requestData.CombOffsetFlag[0] = CombOffsetFlag;
	//////////////////////////////////////////////////////////
	///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
	//// THOST_FTDC_HF_Speculation '1' 投机
	//// THOST_FTDC_HF_Arbitrage '2' 套利
	//// THOST_FTDC_HF_Hedge '3' 套保
	requestData.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	//////////////////////////////////////////////////////////
	///价格 TThostFtdcPriceType double
	requestData.LimitPrice = price;
	//////////////////////////////////////////////////////////
	///数量 TThostFtdcVolumeType int
	requestData.VolumeTotalOriginal = volume;
	//////////////////////////////////////////////////////////
	///有效期类型 TThostFtdcTimeConditionType char
	//// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
	//// THOST_FTDC_TC_GFS '2' 本节有效
	//// THOST_FTDC_TC_GFD '3' 当日有效
	//// THOST_FTDC_TC_GTD '4' 指定日期前有效
	//// THOST_FTDC_TC_GTC '5' 撤销前有效
	//// THOST_FTDC_TC_GFA '6' 集合竞价有效
	requestData.TimeCondition = THOST_FTDC_TC_GFD;
	//////////////////////////////////////////////////////////
	///GTD日期 TThostFtdcDateType char[9]
	strcpy(requestData.GTDDate,this->GetTradingDay().c_str());
	//////////////////////////////////////////////////////////
	///成交量类型 TThostFtdcVolumeConditionType char
	//// THOST_FTDC_VC_AV '1' 任何数量
	//// THOST_FTDC_VC_MV '2' 最小数量
	//// THOST_FTDC_VC_CV '3' 全部数量
	requestData.VolumeCondition = THOST_FTDC_VC_AV;
	//////////////////////////////////////////////////////////
	///最小成交量 TThostFtdcVolumeType int
	requestData.MinVolume = 0;
	//////////////////////////////////////////////////////////
	///触发条件 TThostFtdcContingentConditionType char
	//// THOST_FTDC_CC_Immediately '1' 立即
	//// THOST_FTDC_CC_Touch '2' 止损
	//// THOST_FTDC_CC_TouchProfit '3' 止赢
	//// THOST_FTDC_CC_ParkedOrder '4' 预埋单
	//// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 最新价大于条件价
	//// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 最新价大于等于条件价
	//// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 最新价小于条件价
	//// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 最新价小于等于条件价
	//// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 卖一价大于条件价
	//// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 卖一价大于等于条件价
	//// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 卖一价小于条件价
	//// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 卖一价小于等于条件价
	//// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 买一价大于条件价
	//// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 买一价大于等于条件价
	//// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 买一价小于条件价
	//// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 买一价小于等于条件价
	requestData.ContingentCondition = THOST_FTDC_CC_Immediately;
	//////////////////////////////////////////////////////////
	///止损价 TThostFtdcPriceType double
	requestData.StopPrice = stopprice;
	//////////////////////////////////////////////////////////
	///强平原因 TThostFtdcForceCloseReasonType char
	//// THOST_FTDC_FCC_NotForceClose '0' 非强平
	//// THOST_FTDC_FCC_LackDeposit '1' 资金不足
	//// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
	//// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
	//// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
	//// THOST_FTDC_FCC_Violation '5' 违规
	//// THOST_FTDC_FCC_Other '6' 其它
	//// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
	requestData.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	//////////////////////////////////////////////////////////
	///自动挂起标志 TThostFtdcBoolType int
	requestData.IsAutoSuspend = 0;   //否
	//////////////////////////////////////////////////////////
	///业务单元 TThostFtdcBusinessUnitType char[21]
//	    strcpy(requestData.BusinessUnit,"");
	//////////////////////////////////////////////////////////
	///请求编号 TThostFtdcRequestIDType int
	requestData.RequestID = this->UpdateRequestID();
	//////////////////////////////////////////////////////////
	///用户强评标志 TThostFtdcBoolType int
	requestData.UserForceClose = 0;  //否
	//////////////////////////////////////////////////////////
	///互换单标志 TThostFtdcBoolType int
//	    requestData.IsSwapOrder = 0;
	//////////////////////////////////////////////////////////

	OrderData info;
	strcpy(info.SignalName,signalname);
	strcpy(info.InstrumentID,pinstrumentid);
	strcpy(info.ExchangeID,exchangeid);
	strcpy(info.TradingDay,this->GetTradingDay().c_str());
	strcpy(info.OrderRef,requestData.OrderRef);

	info.Direction=requestData.Direction-'0';
	info.CombOffsetFlag=requestData.CombOffsetFlag[0]-'0';
	info.CombHedgeFlag=requestData.CombHedgeFlag[0]-'0';
	info.LimitPrice = requestData.LimitPrice;
	info.VolumeTotalOriginal = requestData.VolumeTotalOriginal;
	info.StopPrice = stopprice;
	e_OrderStatus eos=sendout;
	info.Status=eos;

	this->GetDataWrapper()->InsertCloseOrderMap(info);

	sprintf(logbuf,"平仓请求委托下单-----pinstrumentid: %s  direction:%d volume:%d  OrderRef: %s  SignalName: %s",info.InstrumentID,
			requestData.Direction,info.VolumeTotalOriginal,info.OrderRef,info.SignalName);
	this->GetLogUtil()->WriteLog(logbuf);
	sprintf(logbuf,"平仓请求委托下单-----tradingdayinfo.TradingDay: %s",this->GetTradingDay().c_str());
	this->GetLogUtil()->WriteLog(logbuf);

	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqOrderInsert(&requestData,this->UpdateRequestID());


	if (result==0)	{
		printf("Debug: ReqSqlCloseOrderInsert req successed！  :\n");

		sprintf(logbuf,"ReqSqlCloseOrderInsert:平仓请求委托下单请求发送成功-----pinstrumentid: %s   price : %.3f ",info.InstrumentID,info.LimitPrice);
		this->GetLogUtil()->WriteLog(logbuf);
		sprintf(logbuf,"ReqSqlCloseOrderInsert:平仓请求委托下单请求发送成功-----tradingdayinfo.TradingDay: %s",this->GetTradingDay().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
	}
	else{
		e_OrderStatus eos=ordererror;
		info.Status=eos;
		this->GetDataWrapper()->UpdateCloseOrderMap(info);

		printf("Debug: ReqSqlCloseOrderInsert failed！  :\n");

		sprintf(logbuf,"平仓请求委托下单-----ReqSqlCloseOrderInsert req failed！ ---TradingDay: %s",this->GetTradingDay().c_str());
		this->GetLogUtil()->WriteLog(logbuf);
	}

}

void TradeBase::ReqSqlOrderAction(const char *signalname,
		const char *pinstrumentid,
		const char *exchangeid,
		const char*orderref,
		int frontid,
		int sessionid)
{

	char logbuf[1024];


	this->GetLogUtil()->WriteLog("请求委托撤单-----ReqSqlOrderAction!!!");


	////////////////////////////////////////////////////////////////////////////////////////////////
	///报单操作请求
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcInputOrderActionField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
	//////////////////////////////////////////////////////////
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserID().c_str());
	//////////////////////////////////////////////////////////
	///报单操作引用 TThostFtdcOrderActionRefType int
	requestData.OrderActionRef = 0;
	//////////////////////////////////////////////////////////
	///报单引用 TThostFtdcOrderRefType char[13]
	strcpy(requestData.OrderRef,orderref);
	//////////////////////////////////////////////////////////
	///请求编号 TThostFtdcRequestIDType int
	requestData.RequestID = this->UpdateRequestID();
	//////////////////////////////////////////////////////////
	///前置编号 TThostFtdcFrontIDType int
	requestData.FrontID =frontid;
	//////////////////////////////////////////////////////////
	///会话编号 TThostFtdcSessionIDType int
	requestData.SessionID = sessionid;
	//////////////////////////////////////////////////////////
	///交易所代码 TThostFtdcExchangeIDType char[9]
//	strcpy(requestData.ExchangeID,"CFFEX");
	//////////////////////////////////////////////////////////
	///报单编号 TThostFtdcOrderSysIDType char[21]
//	strcpy(requestData.OrderSysID,"      622154");
	//////////////////////////////////////////////////////////
	///操作标志 TThostFtdcActionFlagType char
	//// THOST_FTDC_AF_Delete '0' 删除
	//// THOST_FTDC_AF_Modify '3' 修改
	requestData.ActionFlag = '0';
	//////////////////////////////////////////////////////////
	///价格 TThostFtdcPriceType double
//	requestData.LimitPrice = 0;
	//////////////////////////////////////////////////////////
	///数量变化 TThostFtdcVolumeType int
//	requestData.VolumeChange = 0;
	//////////////////////////////////////////////////////////
	///用户代码 TThostFtdcUserIDType char[16]
	strcpy(requestData.UserID,this->GetUserID().c_str());
	//////////////////////////////////////////////////////////
	///合约代码 TThostFtdcInstrumentIDType char[31]

	strcpy(requestData.InstrumentID,pinstrumentid);
	strcpy(requestData.ExchangeID,exchangeid);
	//////////////////////////////////////////////////////////

	OrderData orderdata;
	strcpy(orderdata.InstrumentID,pinstrumentid);


	strcpy(orderdata.TradingDay,this->GetTradingDay().c_str());
	strcpy(orderdata.OrderRef,requestData.OrderRef);
	strcpy(orderdata.CancelSignalName,signalname);


	sprintf(logbuf,"开仓请求委托单撤单-----pinstrumentid: %s OrderRef %s  CancelSignalName %s ",
			pinstrumentid,orderdata.OrderRef,orderdata.CancelSignalName);
	this->GetLogUtil()->WriteLog(logbuf);
	sprintf(logbuf,"开仓请求委托单撤单-----tradingdayinfo.TradingDay: %s",this->GetTradingDay().c_str());
	this->GetLogUtil()->WriteLog(logbuf);


	orderdata.Status=20;
	this->GetDataWrapper()->UpdateOpenOrderMap(orderdata);


	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqOrderAction(&requestData,this->GetRequestID());

	if (result==0)	{
		printf("Debug: ReqSqlOrderAction successed！  :\n");
	}
	else	{
		printf("Debug: ReqSqlOrderAction failed！  :\n");
		orderdata.Status=-1;
		this->GetDataWrapper()->UpdateOpenOrderMap(orderdata);

		sprintf(logbuf,"请求委托下单撤单-----ReqSqlOrderAction failed！ ---TradingDay: %s",this->GetTradingDay().c_str());
			this->GetLogUtil()->WriteLog(logbuf);

	}


}

void TradeBase::ReqSqlCloseOrderAction(const char *signalname,
		const char *pinstrumentid,
		const char *exchangeid,
		const char*orderref,
		int frontid,
		int sessionid)
{

	char logbuf[1024];


	this->GetLogUtil()->WriteLog("请求委托撤单-----ReqSqlCloseOrderAction!!!");

	////////////////////////////////////////////////////////////////////////////////////////////////
	///报单操作请求
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 定义调用API的数据结构
	CThostFtdcInputOrderActionField requestData;
	// 确保没有初始化的数据不会被访问
	memset(&requestData,0,sizeof(requestData));
	// 为调用结构题设置参数信息
	///经纪公司代码 TThostFtdcBrokerIDType char[11]
	strcpy(requestData.BrokerID,this->GetBrokerID().c_str());
	//////////////////////////////////////////////////////////
	///投资者代码 TThostFtdcInvestorIDType char[13]
	strcpy(requestData.InvestorID,this->GetUserID().c_str());
	//////////////////////////////////////////////////////////
	///报单操作引用 TThostFtdcOrderActionRefType int
	requestData.OrderActionRef = 0;
	//////////////////////////////////////////////////////////
	///报单引用 TThostFtdcOrderRefType char[13]
	strcpy(requestData.OrderRef,orderref);
	//////////////////////////////////////////////////////////
	///请求编号 TThostFtdcRequestIDType int
	requestData.RequestID = this->UpdateRequestID();
	//////////////////////////////////////////////////////////
	///前置编号 TThostFtdcFrontIDType int
	requestData.FrontID =frontid;
	//////////////////////////////////////////////////////////
	///会话编号 TThostFtdcSessionIDType int
	requestData.SessionID = sessionid;
	//////////////////////////////////////////////////////////
	///交易所代码 TThostFtdcExchangeIDType char[9]
//	strcpy(requestData.ExchangeID,"CFFEX");
	//////////////////////////////////////////////////////////
	///报单编号 TThostFtdcOrderSysIDType char[21]
//	strcpy(requestData.OrderSysID,"      622154");
	//////////////////////////////////////////////////////////
	///操作标志 TThostFtdcActionFlagType char
	//// THOST_FTDC_AF_Delete '0' 删除
	//// THOST_FTDC_AF_Modify '3' 修改
	requestData.ActionFlag = '0';
	//////////////////////////////////////////////////////////
	///价格 TThostFtdcPriceType double
//	requestData.LimitPrice = 0;
	//////////////////////////////////////////////////////////
	///数量变化 TThostFtdcVolumeType int
//	requestData.VolumeChange = 0;
	//////////////////////////////////////////////////////////
	///用户代码 TThostFtdcUserIDType char[16]
	strcpy(requestData.UserID,this->GetUserID().c_str());
	//////////////////////////////////////////////////////////
	///合约代码 TThostFtdcInstrumentIDType char[31]
	strcpy(requestData.ExchangeID,exchangeid);
	strcpy(requestData.InstrumentID,pinstrumentid);
	//////////////////////////////////////////////////////////

	OrderData orderdata;
	strcpy(orderdata.InstrumentID,pinstrumentid);

	strcpy(orderdata.TradingDay,this->GetTradingDay().c_str());
	strcpy(orderdata.OrderRef,requestData.OrderRef);
	strcpy(orderdata.CancelSignalName,signalname);


	sprintf(logbuf,"平仓请求委托单撤单-----pinstrumentid: %s OrderRef %s  CancelSignalName %s ",
			pinstrumentid,orderdata.OrderRef,orderdata.CancelSignalName);
	this->GetLogUtil()->WriteLog(logbuf);
	sprintf(logbuf,"平仓请求委托单撤单-----tradingdayinfo.TradingDay: %s",this->GetTradingDay().c_str());
	this->GetLogUtil()->WriteLog(logbuf);


	orderdata.Status=20;
	this->GetDataWrapper()->UpdateCloseOrderMap(orderdata);

	// 调用API,并等待响应函数返回
	int result = this->GetCThostFtdcTraderApi()->ReqOrderAction(&requestData,this->GetRequestID());

	if (result==0)	{
		printf("Debug: ReqSqlCloseOrderAction successed！  :\n");
	}
	else{
		printf("Debug: ReqSqlCloseOrderAction failed！  :\n");

		orderdata.Status=-1;
		this->GetDataWrapper()->UpdateCloseOrderMap(orderdata);

		sprintf(logbuf,"平仓请求委托单撤单-----ReqSqlCloseOrderAction failed！ ---TradingDay: %s",this->GetTradingDay().c_str());
		this->GetLogUtil()->WriteLog(logbuf);

	}


}








