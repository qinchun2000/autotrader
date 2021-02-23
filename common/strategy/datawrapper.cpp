#include "datawrapper.hpp"


DataWrapper::DataWrapper() {


}
DataWrapper::~DataWrapper() {

}
void DataWrapper::run()
{

	DateUtil dateutil;
	//std::cout<<"++++++++++++++++++++ datawrapper run "<<dateutil.GetCurrentDaySqlTime()<<endl;
	m_predatastatus=false;
	SetContractLists();
//	std::cout<<"++++++++++++++++++++ datawrapper runaaa "<<dateutil.GetCurrentDaySqlTime()<<endl;
//	InitDataStatusMap(); //20日日线数据是否合法
//	InitCommisionMap();// 合约 margin
//	Init5DayCycleMap();// 合约的5日最高最低 实体线
	InitTotalTRMap();  // atr20 前19日tr总和

	m_predatastatus=true;
	 struct timeval start, m,end;

//	std::cout<<"++++++++++++++++++++ datawrapper run111 "<<dateutil.GetCurrentDaySqlTime()<<endl;
	while(1){
//	std::cout<<"++++++++++++++++++++ datawrapper runing  "<<dateutil.GetCurrentDaySqlTime()<<endl;

		UpdateOpenStatus();


		if (m_openstatus){

			UpdateDateTimeCheckStatus();
			gettimeofday( &start, NULL );
//			std::cout<<"++++++++++++++++++++ datawrapper run --------"<<start.tv_sec<<"."<< start.tv_usec
//					<<"-----------------"<<dateutil.GetCurrentDaySqlTime()<<endl;
			bool upflag=UpdateDepthMarketMap();
	//		std::cout<<"++++++++++++++++++++ datawrapper 1111111111111 "<<dateutil.GetCurrentDaySqlTime()<<endl;
			if(upflag){
	//			std::cout<<"++++++++++++++++++++ datawrapper 222222222222222222 "<<dateutil.GetCurrentDaySqlTime()<<endl;
				SectionMapView();
	//			std::cout<<"++++++++++++++++++++ datawrapper 3333333333333333333333333 "<<dateutil.GetCurrentDaySqlTime()<<endl;
				GlobalMarketView();

			}
		}

		usleep(50000);
	}
}

void DataWrapper::UpdateDateTimeCheckStatus() {
	DateUtil dateutil;
	if(dateutil.TodayIsHoliday()){
		m_holiday=true;
//		printf("----->本交易日为假日,不运行策略!!!!!         \n");
	}

	if(dateutil.IsHolidaybyNextTradingDay()){
		m_nextholiday=true;
//		printf("----->下一个交易日为假日,本交易日不运行策略!!!!!         \n");
	}

	if(dateutil.isFridayNight(this->GetDifSec())){
		m_fridaynight=true;
//		printf("----->周五夜盘不运行策略!!!!          \n");
	}

	if(dateutil.CheckSHFENightTime(this->GetDifSec())){
		m_night=true;
	}
}
void DataWrapper::UpdateOpenStatus() {

	DateUtil dateutil;
	double Q_BarTime_2 = dateutil.GetCurrentSHFEBarTime(this->GetDifSec());
	bool Timemore_morning = (Q_BarTime_2>=0.0859 && Q_BarTime_2<0.1015);
	bool Timemore_morning1 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1500);
	bool Timemore_night = (Q_BarTime_2>=0.2059 && Q_BarTime_2<=0.2400);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);
	bool Timemore=( Timemore_morning  || Timemore_morning1 || Timemore_afternoon || Timemore_night || Timemore_night1);

	if(!Timemore && m_openstatus){
		m_openstatus=false;
	}
	else if(Timemore && !m_openstatus){
		m_openstatus=true;
	}
}
bool DataWrapper::GetOpenStatus() const
{

	return m_openstatus;
}
bool DataWrapper::GetPredataStatus() const {

	return m_predatastatus;
}

bool DataWrapper::GetHolidayStatus() const {
	bool ret;
	if( m_holiday){
		ret=true;
		}
	else {
		ret=false;
	}
	return ret;
}
bool DataWrapper::GetNextHolidayStatus() const {

	return m_nextholiday;
}
bool DataWrapper::GetFridayNightStatus() const {

	return m_fridaynight;
}
bool DataWrapper::GetNightStatus() const {

	return m_night;
}

void DataWrapper::SetUserID(const char* id) {
   this->m_userid=id;
}
std::string DataWrapper::GetUserID()const {
	return this->m_userid;
}

void DataWrapper::SetTradingDay(const char* day) {
    try {
    	if(strcmp(day,this->m_tradingday.c_str()) != 0 ){
    		 std::lock_guard<std::mutex>lck(m_mutex);  // 函数结束时，自动析构解锁
			this->m_tradingday=day;
    	}
    } catch(std::logic_error&) {
        std::cout << "[exception caught DataWrapper::GetTradingDay ]\n";
    }
}
std::string DataWrapper::GetTradingDay() const {
	return this->m_tradingday;
}
void DataWrapper::SetDifSec(int sec) {
    try {
        std::lock_guard<std::mutex>lck(m_mutex);  // 函数结束时，自动析构解锁
        this->m_diff_seconds=sec;
    } catch(std::logic_error&) {
        std::cout << "[exception caught DataWrapper::SetDifSec]\n";
    }
}
int DataWrapper::GetDifSec() const {
	return this->m_diff_seconds;
}

bool DataWrapper::InsertInstrumentMap(InstrumentData &data)
{
	 bool ret=false;
	try {

			std::shared_ptr<InstrumentData> info = std::make_shared<InstrumentData>(data);
			std::string key=info->InstrumentID;

			if (m_instrumentmap.find(key) != m_instrumentmap.end()) {

				m_instrumentmap.erase(key);
				m_instrumentmap.insert(std::pair<std::string, std::shared_ptr<InstrumentData>>(key, info));
			}
			else	{
				m_instrumentmap.insert(std::pair<std::string, std::shared_ptr<InstrumentData>>(key, info));

			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InsertInstrumentMap >>> ]\n";
			ret=false;

		}
	return ret;
}
std::shared_ptr<InstrumentData> DataWrapper::FindInstrumentData(const char* instrumentid)
{

	try {
		 map<std::string,std::shared_ptr<InstrumentData> >::iterator iter;
			  iter = m_instrumentmap.find(instrumentid);

			  if(iter != m_instrumentmap.end()){
//				  cout<<"+++++++++++++++++++FindAssetData "<<iter->second->AccountID
//						  <<"     prebalance:"<<iter->second->PreBalance<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"++++++++FindInstrumentData   ---->Do not Find"<<endl;
				  return nullptr;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindInstrumentData >>> ]\n";
			return nullptr;
	}
}
bool DataWrapper::InsertDepthMarketDataMap(DepthMarketData &data)
{
	 bool ret=false;
	try {

			std::shared_ptr<DepthMarketData> info = std::make_shared<DepthMarketData>(data);
			std::string key=info->InstrumentID;

			if (m_depthdatamap.find(key) != m_depthdatamap.end()) {

				m_depthdatamap.erase(key);
				m_depthdatamap.insert(std::pair<std::string, std::shared_ptr<DepthMarketData>>(key, info));
			}
			else	{
				m_depthdatamap.insert(std::pair<std::string, std::shared_ptr<DepthMarketData>>(key, info));

			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InsertDepthMarketDataMap >>> ]\n";
			ret=false;

		}
	return ret;
}

std::shared_ptr<DepthMarketData> DataWrapper::FindDepthMarketData(const char* instrumentid)
{

	try {
		 map<std::string,std::shared_ptr<DepthMarketData> >::iterator iter;
			  iter = m_depthdatamap.find(instrumentid);

			  if(iter != m_depthdatamap.end()){
//				  cout<<"+++++++++++++++++++FindAssetData "<<iter->second->AccountID
//						  <<"     prebalance:"<<iter->second->PreBalance<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"++++++++FindDepthMarketData   ---->Do not Find"<<endl;
				  return nullptr;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindDepthMarketData >>> ]\n";
			return nullptr;
	}
}
std::map<std::string,std::shared_ptr<DepthMarketData>>* DataWrapper::GetDepthMarketMap()
{
	return &this->m_depthdatamap;
}
void DataWrapper::SetContractLists()
{
	MysqlMaincontract mysqlmaincontract;
	try {
		std::lock_guard<std::mutex>lck(m_contractmutex);  // 函数结束时，自动析构解锁
		m_contractlist.clear();
		mysqlmaincontract.Find_AllListorderbySed(this->m_contractlist);
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::SetContractLists >>> ]\n";
	}

//	DateUtil dateutil;
//	if(this->m_contractlist.size()>0)	{
//		printf("+++++++++++++++++++++++++DataWrapper::SetlContractLists----->主力合约品种数[%ld] ----> %s\n",
//				this->m_contractlist.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}
//	else{
//		printf("++++++++++++++++++++DataWrapper::SetlContractLists----->主力合约品种数[0]   \n");
//	}

}
vector<MainContractData>* DataWrapper::GetContractLists(){
	return &this->m_contractlist;
}
void DataWrapper::SetLimitContractLists()
{
	MysqlMaincontract mysqlmaincontract;
	try {
		std::lock_guard<std::mutex>lck(m_limitcontractmutex);  // 函数结束时，自动析构解锁
		m_limitcontractlist.clear();
		mysqlmaincontract.Find_LimitListbySed_Ndays(this->m_limitcontractlist,20);
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::SetLimitContractLists >>> ]\n";
	}
}
vector<MainContractData>* DataWrapper::GetLimitContractLists(){
	return &this->m_limitcontractlist;
}


void DataWrapper::InitDataStatusMap()
{
//	bool ret=false;

	try {
		cout<<"+++++++++++++++++++ initDataStatusMap"       <<endl;
		MysqlDayLine mysqldayline;
		DateUtil dateutil;
		std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(m_tradingday.c_str());

		std::lock_guard<std::mutex>lck(m_datastatusmutex);  // 函数结束时，自动析构解锁
		m_datastatusmap.clear();
		for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
				item != this->GetContractLists()->end(); item++){

			bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,
					item->InstrumentID,m_tradingday.c_str(),item->ExchangeID,20);
			cout<<"+++++++++++++++++++ 合约:"<<item->InstrumentID<<"       valid:"<<valid_dayline <<endl;
			std::string key =item->InstrumentID;
			m_datastatusmap.insert(map<string,bool>::value_type(key,valid_dayline));
		}

	} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InitDataStatusMap >>> ]\n";
	}
}


bool DataWrapper::FindDataStatus(const char* id)
{

	try {
			std::lock_guard<std::mutex>lck(m_datastatusmutex);  // 函数结束时，自动析构解锁
			 map<std::string,bool >::iterator iter;
			  iter = m_datastatusmap.find(id);

			  if(iter != m_datastatusmap.end()){
//				  cout<<"+++++++++++++++++ FindDataStatus datastatus:"<<iter->second<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"+++++++++++++FindDataStatus Do not Find"<<endl;
				  return false;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindDataStatus >>> ]\n";
			return false;
	}
}
void DataWrapper::Init5DayCycleMap()
{
//	bool ret=false;

	try {
		MysqlDayLine mysqldayline;
		DateUtil dateutil;
		std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(m_tradingday.c_str());

		std::lock_guard<std::mutex>lck(m_5daycyclenmutex);  // 函数结束时，自动析构解锁
		m_5daycyclemap.clear();
		for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
				item != this->GetContractLists()->end(); item++){

			CycleData cycledata = mysqldayline.Collect_CycleNData(item->InstrumentID,tradingday_sqltime.c_str(),5);


			std::string key =item->InstrumentID;
			std::shared_ptr<CycleData> mdata = std::make_shared<CycleData>(cycledata);
			m_5daycyclemap.insert(std::pair<std::string, std::shared_ptr<CycleData>>(key, mdata));
		}

	} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::Init5DayCycleMap >>> ]\n";

	}


//	 map<std::string,std::shared_ptr<CycleData> >::iterator iter;
//	 for(iter=m_5daycyclemap.begin(); iter!=m_5daycyclemap.end(); iter++){
//		 cout<<"+++++++++++++++++++Init5DayCycleMap 合约:"<<iter->second->InstrumentID<<"       high20:"<<iter->second->High20
//				  <<"     low20:"<<iter->second->Low20<<endl;
//	 }
//		 gettimeofday( &end, NULL );
//				printf("/////////////////end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
//	return ret;
}
std::shared_ptr<CycleData> DataWrapper::Find5DayCycle(const char* id)
{
	 map<std::string,std::shared_ptr<CycleData> >::iterator iter;
	try {
			std::lock_guard<std::mutex>lck(m_5daycyclenmutex);  // 函数结束时，自动析构解锁

			  iter = m_5daycyclemap.find(id);

			  if(iter != m_5daycyclemap.end()){
//				  cout<<"+++++++++++++++++++Find5DayCycle high20:"<<iter->second->High20
//						  <<"     low20:"<<iter->second->Low20<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"++++++++Find5DayCycle   ---->Do not Find"<<endl;
				  return nullptr;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::Find5DayCycle >>> ]\n";
			return nullptr;
	}
}

void DataWrapper::InitTotalTRMap()
{
//	bool ret=false;

	try {
		MysqlDayLine mysqldayline;
		DateUtil dateutil;
		std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(m_tradingday.c_str());

		std::lock_guard<std::mutex>lck(m_totaltrmutex);  // 函数结束时，自动析构解锁
		m_totaltrmap.clear();
		for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
				item != this->GetContractLists()->end(); item++){

			double totaltr = mysqldayline.Get_TRndaysbySqlTime(item->InstrumentID,tradingday_sqltime.c_str(),19);
			std::string key =item->InstrumentID;
			if(totaltr>0){
				m_totaltrmap.insert(map<string,double>::value_type(key,totaltr));
			}

		}

	} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InitTotalTRMap >>> ]\n";

	}

	std::cout<<"++++++++++++++++++++totaltr map size:"<<m_totaltrmap.size()<<endl;
	 for(map<std::string,double >::iterator iter=m_totaltrmap.begin(); iter!=m_totaltrmap.end(); iter++){
		 cout<<"+++++++++++++++++++ 合约:"<<iter->first<<"       totaltr:"<<iter->second  <<endl;
	 }
//		 gettimeofday( &end, NULL );
//				printf("/////////////////end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
//	return ret;
}

double DataWrapper::FindTotalTR(const char* id)
{
	 map<std::string,double >::iterator iter;
	try {
			std::lock_guard<std::mutex>lck(m_totaltrmutex);  // 函数结束时，自动析构解锁

			  iter = m_totaltrmap.find(id);

			  if(iter != m_totaltrmap.end()){
//				  cout<<"+++++++++++++++++++FindCommission margin:"<<iter->second<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"++++++++++++++++++ FindTotalTR Do not Find"<<endl;
				  return 0.0;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindCommission >>> ]\n";
			return 0.0;
	}
//	if(iter != m_totaltrmap.end())	{
//	       cout<<"find margin---------->>>>>>  "<<iter->second<<endl;
//	}
//	else	{
//	       cout<<" not find"<<endl;
//	}

}
void DataWrapper::InitCommisionMap()
{
	MysqlCommission mysqlcommission;
	vector<Commission>list;

	try {
		std::lock_guard<std::mutex>lck(m_commapmutex);  // 函数结束时，自动析构解锁
		m_commissionmap.clear();

		mysqlcommission.Find_AllList(list);
//		cout<<"+++++++++++++++++++InitCommisionMap size:"<<list.size()<<endl;
		for( auto &item:list){
			std::string key=item.ProductID;
			std::shared_ptr<Commission> data = std::make_shared<Commission>(item);
			m_commissionmap.insert(std::pair<std::string, std::shared_ptr<Commission>>(key,data));
//			cout<<"+++++++++++++InitCommisionMap:"<<key<<"--------------"<<data->ProductID<<endl;
		}
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::InitCommisionMap >>> ]\n";
	}

	vector<Commission>().swap(list);
}

//std::map<std::string,double> * DataWrapper::GetCommissionMap()
//{
//return &m_commissionmap;
//}

std::shared_ptr<Commission>  DataWrapper::FindCommissionData(const char* id)
{
	 map<std::string,std::shared_ptr<Commission>>::iterator iter;
	try {
			std::lock_guard<std::mutex>lck(m_commapmutex);  // 函数结束时，自动析构解锁
//			cout<<"+++++++++++++++++++FindCommission size:"<<m_commissionmap.size()<<endl;
			  iter = m_commissionmap.find(id);

			  if(iter != m_commissionmap.end()){
//				  cout<<"+++++++++++++++++++FindCommission margin:"<<iter->second->ProductID<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"++++++++++++++++++FindCommission Do not Find:"<<id<<endl;
				  return nullptr;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindCommission >>> ]\n";
			return nullptr;
	}
//	if(iter != m_commissionmap.end())	{
//	       cout<<"find margin---------->>>>>>  "<<iter->second<<endl;
//	}
//	else	{
//	       cout<<" not find"<<endl;
//	}

}

bool DataWrapper::EmptyHoldMap()
{
	bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁
			m_holdmap.clear();
			ret=true;
		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::EmptyHoldMap >>> ]\n";
			ret=false;
	}
	return ret;
}
bool DataWrapper::ResetHoldMap(vector<HoldData> &list)
{
	bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁
			m_holdmap.clear();
			for(auto &item:list){
				std::string key =item.InstrumentID;
//				m_holdmap.insert(map<string,HoldData>::value_type(key,item));
			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::ResetHoldMap >>> ]\n";
			ret=false;
	}
	return ret;
}
bool DataWrapper::InsertHoldData(HoldData &data)
{

	 map<std::string,std::shared_ptr<HoldData> >::iterator iter;
	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<HoldData> e = std::make_shared<HoldData>(data);
		std::string key =data.InstrumentID;
		char buf[10];
		sprintf(buf,"%d",data.Direction);
		key.append("_");
		key.append(buf);
		if (m_holdmap.find(key) != m_holdmap.end()) 		{
//			m_holdmap.insert(map<string,std::shared_ptr<HoldData>>::value_type(key,e));
		}
		else{
			m_holdmap.insert(map<string,std::shared_ptr<HoldData>>::value_type(key,e));
		}

		ret=true;

	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::InsertHoldMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::UpdateHoldData(HoldData &data)
{

	 map<std::string,std::shared_ptr<HoldData> >::iterator iter;
	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<HoldData> e = std::make_shared<HoldData>(data);
		std::string key =data.InstrumentID;
		char buf[10];
		sprintf(buf,"%d",data.Direction);
		key.append("_");
		key.append(buf);
		if (m_holdmap.find(key) != m_holdmap.end()) 		{
			 cout<<"++++++++UpdateHoldData    erase ---->"<<data.OpenDate<<endl;
			m_holdmap.erase(key);
			m_holdmap.insert(map<string,std::shared_ptr<HoldData>>::value_type(key,e));
		}
		else{
			m_holdmap.insert(map<string,std::shared_ptr<HoldData>>::value_type(key,e));
		}

		ret=true;

	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::InsertHoldMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::DeleteHoldData(const char* instrumentid,const int direction)
{
	 map<std::string,std::shared_ptr<HoldData> >::iterator iter;
	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁
		std::string key =instrumentid;
		char buf[10];
		sprintf(buf,"%d",direction);
		key.append("_");
		key.append(buf);
		if (m_holdmap.find(key) != m_holdmap.end()) 		{
			m_holdmap.erase(key);
		}
		ret=true;

	} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::DeleteHoldData >>> ]\n";
			ret=false;
	}
	return ret;
}

//std::map<std::string,std::shared_ptr<HoldData>> * DataWrapper::GetHoldMap()
//{
//	return &m_holdmap;
//}
std::shared_ptr<HoldData> DataWrapper::FindHoldMap(const char* instrumentid,const int direction)
{
	 map<std::string,std::shared_ptr<HoldData> >::iterator iter;
	try {
			std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁

			  std::string key =instrumentid;
			  char buf[10];
				sprintf(buf,"%d",direction);
				key.append("_");
				key.append(buf);

			iter = m_holdmap.find(key);
			  if(iter != m_holdmap.end()){

				 return iter->second;
			  }
			  else{
//				  cout<<"++++++++FindHoldMap   ---->Do not Find"<<endl;
				  return nullptr;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindHoldMap >>> ]\n";
			return nullptr;
	}
}
HoldData DataWrapper::FindHoldData(const char* instrumentid,const int direction)
{
	HoldData  data;
	memset(&data,0,sizeof(data));
	try {
		std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁

		  std::string key =instrumentid;
		  char buf[10];
			sprintf(buf,"%d",direction);
			key.append("_");
			key.append(buf);

		 map<std::string,std::shared_ptr<HoldData> >::iterator iter;
		iter = m_holdmap.find(key);
		if(iter != m_holdmap.end()){

			strcpy(data.InstrumentID, iter->second->InstrumentID);
			strcpy(data.ExchangeID, iter->second->ExchangeID);
			strcpy(data.TradingDay, iter->second->TradingDay);
			strcpy(data.OpenDate, iter->second->OpenDate);
			data.OpenPriceAverage = iter->second->OpenPriceAverage;
			data.StopRange = iter->second->StopRange;
			data.StopPrice = iter->second->StopPrice;
			data.WinPrice = iter->second->WinPrice;
			data.Highest = iter->second->Highest;
			data.Lowest = iter->second->Lowest;
			data.Margin = iter->second->Margin;
			data.Volume = iter->second->Volume;
			data.Direction = iter->second->Direction;
//			  cout<<"++++++++FindHoldMap   ---->"<<data.OpenDate<<endl;

		}
		else{
		//				  cout<<"++++++++FindHoldMap   ---->Do not Find"<<endl;

		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::FindHoldMap >>> ]\n";

	}
	return data;
}
bool DataWrapper::GetHoldList(vector<HoldData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_holdmutex);  // 函数结束时，自动析构解锁
			multimap<std::string, std::shared_ptr<HoldData> >::iterator item;
			for(item=m_holdmap.begin();item!=m_holdmap.end();item++){
				HoldData  data;
				strcpy(data.InstrumentID, item->second->InstrumentID);
				strcpy(data.ExchangeID, item->second->ExchangeID);
				strcpy(data.TradingDay, item->second->TradingDay);
				strcpy(data.OpenDate, item->second->OpenDate);
				data.OpenPriceAverage = item->second->OpenPriceAverage;
				data.StopRange = item->second->StopRange;
				data.StopPrice = item->second->StopPrice;
				data.WinPrice = item->second->WinPrice;
				data.Highest = item->second->Highest;
				data.Lowest = item->second->Lowest;
				data.Margin = item->second->Margin;
				data.Volume = item->second->Volume;
				data.Direction = item->second->Direction;

				list.push_back(data);

			}

			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InsertHoldMap >>> ]\n";
			ret=false;

		}
	return ret;
}
void DataWrapper::UpdateHoldMap()
{

}
bool DataWrapper::EmptyAssetMap()
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_assetdatamutex);  // 函数结束时，自动析构解锁
		m_assetdatamap.clear();
		ret=true;
	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::EmptyAssetMap >>> ]\n";
		ret=false;
	}
	return ret;
}
bool DataWrapper::InsertAssetMap(AssetData &data)
{
	 bool ret=false;
	try {
			std::unique_lock<std::mutex>lck(m_assetdatamutex);  // 函数结束时，自动析构解锁

			std::shared_ptr<AssetData> info = std::make_shared<AssetData>(data);

			std::string key=info->AccountID;

			if (m_assetdatamap.find(key) != m_assetdatamap.end()) {

				m_assetdatamap[key] = info;
			}
			else	{
				m_assetdatamap.insert(std::pair<std::string, std::shared_ptr<AssetData>>(key, info));

			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught CTraderSpi::InsertInvestPositionMap >>> ]\n";
			ret=false;

		}
	return ret;
}
bool DataWrapper::UpdateAssetMap(AssetData &data)
{
	 bool ret=false;
	try {
			std::unique_lock<std::mutex>lck(m_assetdatamutex);  // 函数结束时，自动析构解锁

			std::shared_ptr<AssetData> info = std::make_shared<AssetData>(data);

			std::string key=info->AccountID;

			if (m_assetdatamap.find(key) != m_assetdatamap.end()) {
				m_assetdatamap.erase(key);
				m_assetdatamap.insert(std::pair<std::string, std::shared_ptr<AssetData>>(key, info));
			}
			else	{
				m_assetdatamap.insert(std::pair<std::string, std::shared_ptr<AssetData>>(key, info));

			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught CTraderSpi::UpdateAssetMap >>> ]\n";
			ret=false;

		}
	return ret;
}
std::shared_ptr<AssetData> DataWrapper::FindAssetData(const char* userid)
{
	 map<std::string,std::shared_ptr<AssetData> >::iterator iter;
	try {
			std::lock_guard<std::mutex>lck(m_assetdatamutex);  // 函数结束时，自动析构解锁

			  iter = m_assetdatamap.find(userid);

			  if(iter != m_assetdatamap.end()){
//				  cout<<"+++++++++++++++++++FindAssetData "<<iter->second->AccountID
//						  <<"     prebalance:"<<iter->second->PreBalance<<endl;
				 return iter->second;
			  }

			  else{
				  cout<<"++++++++FindAssetData   ---->Do not Find"<<endl;
				  return nullptr;
			  }

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::FindAssetData >>> ]\n";
			return nullptr;
	}
}

bool DataWrapper::EmptyPositionDetailMap()
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
		m_positiondetailmap.clear();
		ret=true;
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::EmptyPositionDetailMap >>> ]\n";
		ret=false;
	}
	return ret;
}
bool DataWrapper::InsertPositionDetailMap(PositionDetailData &data)
{
	 bool ret=false;
	try {
//		std::unique_lock<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
//		std::shared_ptr<PositionDetailData> info = std::make_shared<PositionDetailData>(data);
//		std::string key=info->InstrumentID;
//		if (m_positiondetailmap.find(key) != m_positiondetailmap.end()) {
//			m_positiondetailmap.erase(key);
//			m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//		}
//		else	{
//			m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//
//		}

		std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<PositionDetailData> info = std::make_shared<PositionDetailData>(data);
		std::string key=info->InstrumentID;

		multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type entries;
		entries = m_positiondetailmap.count(key);
//		std::cout<< "InsertPositionDetailMap   before:"<<data.OpenDate<<endl;
		multimap<std::string, std::shared_ptr<PositionDetailData> >::iterator iter=m_positiondetailmap.find(key);

		if (m_investpositionmap.find(key) != m_investpositionmap.end()) {
//			std::cout<< "InsertPositionDetailMap   非空 insert:"<<info->OpenDate;
			bool flag=false;
			for(multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type cnt=0;cnt!=entries;++cnt,++iter){

				if (strcmp(iter->second->InstrumentID,info->InstrumentID)==0&&
						strcmp(iter->second->TradeID,info->TradeID)==0 ) {
					flag=true;
//					m_positiondetailmap.erase(key);
//					m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//					std::cout<< "InsertPositionDetailMap   erase:"<<info->OpenDate;
				}
				else	{

//					std::cout<< "InsertPositionDetailMap   insert:"<<info->OpenDate;

				}
			}

			if(flag){
				m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
			}
		}
		else{
			m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
			std::cout<< "InsertPositionDetailMap   空 insert:"<<info->OpenDate;
		}


		ret=true;
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::InsertPositionDetailMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::UpdatePositionDetailMap(PositionDetailData &data)
{
	 bool ret=false;
	try {
//		std::unique_lock<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
//		std::shared_ptr<PositionDetailData> info = std::make_shared<PositionDetailData>(data);
//		std::string key=info->InstrumentID;
//		if (m_positiondetailmap.find(key) != m_positiondetailmap.end()) {
//			m_positiondetailmap.erase(key);
//			m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//		}
//		else	{
//			m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//
//		}

		std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<PositionDetailData> info = std::make_shared<PositionDetailData>(data);
		std::string key=info->InstrumentID;

		multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type entries;
		entries = m_positiondetailmap.count(key);
		std::cout<< "InsertPositionDetailMap   before:"<<data.OpenDate<<endl;
		multimap<std::string, std::shared_ptr<PositionDetailData> >::iterator iter=m_positiondetailmap.find(key);

		if (m_investpositionmap.find(key) != m_investpositionmap.end()) {
//			std::cout<< "InsertPositionDetailMap   非空 insert:"<<info->OpenDate;
			bool flag=false;
			for(multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type cnt=0;cnt!=entries;++cnt,++iter){

				if (strcmp(iter->second->InstrumentID,info->InstrumentID)==0&&
						strcmp(iter->second->TradeID,info->TradeID)==0 ) {
					flag=true;
					m_positiondetailmap.erase(key);
					m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//					std::cout<< "InsertPositionDetailMap   erase:"<<info->OpenDate;
				}
				else	{

//					std::cout<< "InsertPositionDetailMap   insert:"<<info->OpenDate;

				}
			}

			if(!flag){
				m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
			}
		}
		else{
			m_positiondetailmap.insert(std::pair<std::string, std::shared_ptr<PositionDetailData>>(key, info));
//			std::cout<< "InsertPositionDetailMap   空 insert:"<<info->OpenDate;
		}


		ret=true;
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::InsertPositionDetailMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::GetPositionDetailList(vector<PositionDetailData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
			multimap<std::string, std::shared_ptr<PositionDetailData> >::iterator item;
			for(item=m_positiondetailmap.begin();item!=m_positiondetailmap.end();item++){
				PositionDetailData  data;
				strcpy(data.InstrumentID, item->second->InstrumentID);
				strcpy(data.OpenDate,item->second->OpenDate);
				strcpy(data.ExchangeID, item->second->ExchangeID);
				strcpy(data.TradeID,item->second->TradeID);
				strcpy(data.TradingDay, item->second->TradingDay);
				data.OpenPrice = item->second->OpenPrice;
				data.Margin = item->second->Margin;
				data.Volume = item->second->Volume;
				data.Direction = item->second->Direction;
				data.CloseVolume = item->second->CloseVolume;
				data.CloseAmount = item->second->CloseAmount;
				list.push_back(data);
			}

			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InsertHoldMap >>> ]\n";
			ret=false;

		}
	return ret;
}
void DataWrapper::GetPositionDetailListbyIns(const char* instrumentid)
{
	std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
	std::string key=instrumentid;
	CIT item;
//	Range = m_positiondetailmap.equal_range(key);

	multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type entries;
	entries = m_positiondetailmap.count(key);
	multimap<std::string, std::shared_ptr<PositionDetailData> >::iterator iter=m_positiondetailmap.find(key);
	for(multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type cnt=0;cnt!=entries;++cnt,++iter){
		cout<< iter->second << endl;
	}


}
double DataWrapper::CollectAverageOpenPricebyPosDetail(const char* instrumentid)
{

	double averageopenprice=0.0;
	int totalopenvolume=0;
	double totalamount=0.0;
	std::string key=instrumentid;
	CIT item;
//	Range = m_positiondetailmap.equal_range(key);


	std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
	multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type entries;
	entries = m_positiondetailmap.count(key);
	multimap<std::string, std::shared_ptr<PositionDetailData> >::iterator iter=m_positiondetailmap.find(key);
	for(multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type cnt=0;cnt!=entries;++cnt,++iter){

		totalopenvolume+=iter->second->Volume+iter->second->CloseVolume;
		totalamount+=totalopenvolume*iter->second->OpenPrice;

//		printf("++++++++++++++++++posdetail 计算开仓均价 %s volume:%d   openprice:%.2f \n",
//				iter->second->InstrumentID,iter->second->Volume,iter->second->OpenPrice);
	}
	if(totalopenvolume>0){
		averageopenprice=totalamount/(double)totalopenvolume;
	}
	else{
		averageopenprice=0.0;
	}

	return averageopenprice;
}
std::string DataWrapper::CollectOpenDatebyPosDetail(const char* instrumentid)
{
	std::string opendate;
	std::string key=instrumentid;
//	Range = m_positiondetailmap.equal_range(key);
	std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
	multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type entries;
	entries = m_positiondetailmap.count(key);
	multimap<std::string, std::shared_ptr<PositionDetailData> >::iterator iter=m_positiondetailmap.find(key);
	for(multimap<std::string, std::shared_ptr<PositionDetailData> >::size_type cnt=0;cnt!=entries;++cnt,++iter){

		if(iter->second->Volume>0){
			opendate=iter->second->OpenDate;
			printf("++++++++++++++++++posdetail 计算开仓均价 %s volume:%d   openprice:%.2f  opendate:%s\n",
							iter->second->InstrumentID,iter->second->Volume,iter->second->OpenPrice,iter->second->OpenDate);
		}
	}

	return opendate;
}
int DataWrapper::GetDetailPosition()
{
	 int position=0;
	try {
			std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<PositionDetailData> >::iterator item;
			for(item=m_positiondetailmap.begin();item!=m_positiondetailmap.end();item++){

				position+=item->second->Volume;

			}
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::GetDetailPosition >>> ]\n";

	}
	return position;
}
int DataWrapper::CollectDetailCloseVolume_Total()
{
	 int position=0;
	try {
			std::lock_guard<std::mutex>lck(m_positiondetailmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<PositionDetailData> >::iterator item;
			for(item=m_positiondetailmap.begin();item!=m_positiondetailmap.end();item++){

					position+=item->second->CloseVolume;

			}
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::GetDetailPosition >>> ]\n";

	}
	return position;
}
bool DataWrapper::EmptyInvestPositionMap()
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
		m_investpositionmap.clear();
		ret=true;
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::EmptyInvestPositionMap >>> ]\n";
		ret=false;
	}
	return ret;
}
bool DataWrapper::InsertInvestPositionMap(InvestPositionData &data)
{
	 bool ret=false;
	try {
			std::unique_lock<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁

			std::shared_ptr<InvestPositionData> info = std::make_shared<InvestPositionData>(data);
			std::string key=info->InstrumentID;
			char buf[10];
			sprintf(buf,"%d",info->PosiDirection);
			key.append("_");
			key.append(buf);

			if (m_investpositionmap.find(key) != m_investpositionmap.end()) {

				m_investpositionmap.erase(key);
				m_investpositionmap.insert(std::pair<std::string, std::shared_ptr<InvestPositionData>>(key, info));
			}
			else	{
				m_investpositionmap.insert(std::pair<std::string, std::shared_ptr<InvestPositionData>>(key, info));

			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::InsertInvestPositionMap >>> ]\n";
			ret=false;

		}
	return ret;
}

//bool DataWrapper::InsertInvestPositionMap(std::shared_ptr<InvestPositionData> info)
//{
//	 bool ret=false;
//	try {
//			std::unique_lock<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
//			std::string key=info->symbol;
//
//			if (m_investpositionmap.find(key) != m_investpositionmap.end()) {
//				m_investpositionmap[key] = info;
//			}
//			else	{
//				m_investpositionmap.insert(std::pair<std::string, std::shared_ptr<InvestPositionBuffer>>(key, info));
//
//			}
//			ret=true;
//
//		} catch(std::logic_error&) {
//			std::cout << "[exception caught CTraderSpi::InsertInvestPositionMap >>> ]\n";
//			ret=false;
//
//		}
//	return ret;
//}

bool DataWrapper::GetInvestList(vector<InvestPositionData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<InvestPositionData> >::iterator item;
			for(item=m_investpositionmap.begin();item!=m_investpositionmap.end();item++){
				InvestPositionData  data;
				strcpy(data.InstrumentID, item->second->InstrumentID);
				strcpy(data.ExchangeID, item->second->ExchangeID);
				strcpy(data.TradingDay, item->second->TradingDay);
				data.OpenPriceAverage = item->second->OpenPriceAverage;
				data.UseMargin = item->second->UseMargin;
				data.PositionProfit = item->second->PositionProfit;
				data.CloseProfit = item->second->CloseProfit;
				data.Position = item->second->Position;
				data.YdPosition = item->second->YdPosition;
				data.TodayPosition = item->second->TodayPosition;
				data.PosiDirection = item->second->PosiDirection;
				data.OpenVolume = item->second->OpenVolume;
				data.CloseVolume = item->second->CloseVolume;
				data.OpenAmount = item->second->OpenAmount;
				data.CloseAmount =item->second->CloseAmount;
				data.Commission = item->second->Commission;
				data.PreSettlementPrice = item->second->PreSettlementPrice;
				data.SettlementPrice = item->second->SettlementPrice;
				list.push_back(data);
				ret=true;
			}



		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetInvestList >>> ]\n";
			ret=false;

		}
	return ret;
}

bool DataWrapper::GetHoldingInvestList(vector<InvestPositionData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<InvestPositionData> >::iterator item;
			for(item=m_investpositionmap.begin();item!=m_investpositionmap.end();item++){
				if(item->second->Position>0){
					InvestPositionData  data;
					strcpy(data.InstrumentID, item->second->InstrumentID);
					strcpy(data.ExchangeID, item->second->ExchangeID);
					strcpy(data.TradingDay, item->second->TradingDay);
					data.OpenPriceAverage = item->second->OpenPriceAverage;
					data.UseMargin = item->second->UseMargin;
					data.PositionProfit = item->second->PositionProfit;
					data.CloseProfit = item->second->CloseProfit;
					data.Position = item->second->Position;
					data.YdPosition = item->second->YdPosition;
					data.TodayPosition = item->second->TodayPosition;
					data.PosiDirection = item->second->PosiDirection;
					data.OpenVolume = item->second->OpenVolume;
					data.CloseVolume = item->second->CloseVolume;
					data.OpenAmount = item->second->OpenAmount;
					data.CloseAmount =item->second->CloseAmount;
					data.Commission = item->second->Commission;
					data.PreSettlementPrice = item->second->PreSettlementPrice;
					data.SettlementPrice = item->second->SettlementPrice;
					list.push_back(data);
					ret=true;
				}

			}



		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetInvestList >>> ]\n";
			ret=false;

		}
	return ret;
}
bool DataWrapper::GetCloseInvestList(vector<InvestPositionData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<InvestPositionData> >::iterator item;
			for(item=m_investpositionmap.begin();item!=m_investpositionmap.end();item++){
				if(item->second->Position==0 && item->second->CloseVolume>0){
					InvestPositionData  data;
					strcpy(data.InstrumentID, item->second->InstrumentID);
					strcpy(data.ExchangeID, item->second->ExchangeID);
					strcpy(data.TradingDay, item->second->TradingDay);
					data.OpenPriceAverage = item->second->OpenPriceAverage;
					data.UseMargin = item->second->UseMargin;
					data.PositionProfit = item->second->PositionProfit;
					data.CloseProfit = item->second->CloseProfit;
					data.Position = item->second->Position;
					data.YdPosition = item->second->YdPosition;
					data.TodayPosition = item->second->TodayPosition;
					data.PosiDirection = item->second->PosiDirection;
					data.OpenVolume = item->second->OpenVolume;
					data.CloseVolume = item->second->CloseVolume;
					data.OpenAmount = item->second->OpenAmount;
					data.CloseAmount =item->second->CloseAmount;
					data.Commission = item->second->Commission;
					data.PreSettlementPrice = item->second->PreSettlementPrice;
					data.SettlementPrice = item->second->SettlementPrice;
					list.push_back(data);

					ret=true;
				}

			}
	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::GetInvestList >>> ]\n";
		ret=false;

	}
	return ret;
}
int DataWrapper::GetCloseVolume()
{
	int closevolume=0;
	try {
			std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<InvestPositionData> >::iterator item;
			for(item=m_investpositionmap.begin();item!=m_investpositionmap.end();item++){
				if(item->second->CloseVolume>0){
					closevolume+=item->second->CloseVolume;
				}
			}

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetCloseVolume >>> ]\n";

		}
	return closevolume;
}
int DataWrapper::GetHoldingPosition()
{
	int holdcount=0;
	try {
			std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<InvestPositionData> >::iterator item;
			for(item=m_investpositionmap.begin();item!=m_investpositionmap.end();item++){
				if(item->second->Position>0){
					holdcount+=item->second->Position;
				}
			}

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetHoldingPosition >>> ]\n";

		}
	return holdcount;
}
int DataWrapper::GetPositionbyInstrumentid(const char* ins,const int direction)
{
	int position=0;
	try {
		std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
		map<std::string, std::shared_ptr<InvestPositionData> >::iterator item;
		std::string key=ins;
		char buf[10];
		sprintf(buf,"%d",direction);
		key.append("_");
		key.append(buf);
		item = m_investpositionmap.find(key);
		  if(item != m_investpositionmap.end()){
//				  cout<<"+++++++++++++++++++GetPositionbyInstrumentid:"<<iter->second<<endl;
			  position=item->second->Position;
		  }
		  else{
//			  cout<<"++++++++++++++++++GetPositionbyInstrumentid Do not Find"<<endl;
			  return 0;
		  }

	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::GetPositionbyInstrumentid >>> ]\n";

	}
	return position;
}

bool DataWrapper::InsertOpenOrderMap(OrderData &data)
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
		std::string key =data.OrderRef;
		std::shared_ptr<OrderData> e = std::make_shared<OrderData>(data);

		if (m_openordermap.find(key) == m_openordermap.end()) 		{
			m_openordermap.insert(map<string,std::shared_ptr<OrderData>>::value_type(key,e));
			ret=true;
		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}

bool DataWrapper::UpdateOpenOrderMap(OrderData &data)
{

	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<OrderData> e = std::make_shared<OrderData>(data);
		std::string key =data.OrderRef;

		if(m_openordermap.find(key)!=m_openordermap.end()){
			m_openordermap.erase(key);
			m_openordermap.insert(map<string,std::shared_ptr<OrderData>>::value_type(key,e));
			ret=true;
		}
		else{
			m_openordermap.insert(map<string,std::shared_ptr<OrderData>>::value_type(key,e));
			ret=true;
		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}

bool DataWrapper::DeleteOpenOrderMap(OrderData &data)
{
	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<OrderData> e = std::make_shared<OrderData>(data);
		std::string key =data.OrderRef;

		if(m_openordermap.find(key)!=m_openordermap.end()){
			m_openordermap.erase(key);
			ret=true;
		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::ExistinOrderMap(const char* instrumentid,const int direction)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<OrderData> >::iterator item;
			for(item=m_openordermap.begin();item!=m_openordermap.end();item++){
				if(strcmp(item->second->InstrumentID,instrumentid)==0&& item->second->Direction==direction){
					ret=true;
				}

			}


		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::ExistinOrderMap >>> ]\n";
			ret=false;

		}
	return ret;
}
int DataWrapper::MaxOrderRefMap()
{
	 int openret=0;
	 if(1){
		 std::lock_guard<std::mutex>lck(m_openordermutex);
		 for (map<string,std::shared_ptr<OrderData>>::iterator it=m_openordermap.begin(); it!=m_openordermap.end(); ++it){
			 int orderref = atoi(it->second->OrderRef);
				if(orderref>openret){
					openret=orderref;
				}
		 }
	 }
	 int closeret=0;
	 if(1){
		 std::lock_guard<std::mutex>lck(m_closeordermutex);
		 for (map<string,std::shared_ptr<OrderData>>::iterator it=m_closeordermap.begin(); it!=m_closeordermap.end(); ++it){
			 int orderref = atoi(it->second->OrderRef);
				if(orderref>closeret){
					closeret=orderref;
				}
		 }
	 }

	return max(openret,closeret);
}
bool DataWrapper::GetOpenOrderList(vector<OrderData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<OrderData> >::iterator item;
			for(item=m_openordermap.begin();item!=m_openordermap.end();item++){
					OrderData  data;
					strcpy(data.InstrumentID, item->second->InstrumentID);
					strcpy(data.TradingDay, item->second->TradingDay);
					strcpy(data.ExchangeID, item->second->ExchangeID);
					strcpy(data.OrderRef,item->second->OrderRef);
					strcpy(data.OrderSysID,item->second->OrderSysID);
					data.FrontID=item->second->FrontID;
					data.SessionID=item->second->SessionID;
					data.CombOffsetFlag=item->second->CombOffsetFlag;
					data.VolumeTotalOriginal = item->second->VolumeTotalOriginal;
					data.LimitPrice = item->second->LimitPrice;
					data.Direction= item->second->Direction;
					data.Volume= item->second->Volume;
					data.Status=item->second->Status;
					data.OrderStatus=item->second->OrderStatus;
					data.OrderSubmitStatus=item->second->OrderSubmitStatus;
					list.push_back(data);
			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetOrderQueueList >>> ]\n";
			ret=false;

		}
	return ret;
}
bool DataWrapper::GetOpenOrderQueueList(vector<OrderData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<OrderData> >::iterator item;
			for(item=m_openordermap.begin();item!=m_openordermap.end();item++){

				if(item->second->Status==1){
					OrderData  data;
					strcpy(data.InstrumentID, item->second->InstrumentID);
					strcpy(data.TradingDay, item->second->TradingDay);
					strcpy(data.ExchangeID, item->second->ExchangeID);
					strcpy(data.OrderRef,item->second->OrderRef);
					strcpy(data.OrderSysID,item->second->OrderSysID);
					data.FrontID=item->second->FrontID;
					data.SessionID=item->second->SessionID;
					data.CombOffsetFlag=item->second->CombOffsetFlag;
					data.VolumeTotalOriginal = item->second->VolumeTotalOriginal;
					data.LimitPrice = item->second->LimitPrice;
					data.Direction= item->second->Direction;
					data.Volume= item->second->Volume;
					data.Status=item->second->Status;
					data.OrderStatus=item->second->OrderStatus;
					data.OrderSubmitStatus=item->second->OrderSubmitStatus;
					list.push_back(data);
				}

			}

			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetOrderQueueList >>> ]\n";
			ret=false;

		}
	return ret;
}
bool DataWrapper::InsertCloseOrderMap(OrderData &data)
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_closeordermutex);  // 函数结束时，自动析构解锁
		std::string key =data.OrderRef;

		std::shared_ptr<OrderData> e = std::make_shared<OrderData>(data);

		if (m_closeordermap.find(key) == m_closeordermap.end()) 		{
			m_closeordermap.insert(map<string,std::shared_ptr<OrderData>>::value_type(key,e));
			ret=true;
		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::UpdateCloseOrderMap(OrderData &data)
{

	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_closeordermutex);  // 函数结束时，自动析构解锁
		std::string key =data.OrderRef;

		std::shared_ptr<OrderData> e = std::make_shared<OrderData>(data);

		if (m_closeordermap.find(key) == m_closeordermap.end()) 		{
			m_closeordermap.erase(key);
			m_closeordermap.insert(map<string,std::shared_ptr<OrderData>>::value_type(key,e));
			ret=true;
		}
		else{
			m_closeordermap.insert(map<string,std::shared_ptr<OrderData>>::value_type(key,e));
			ret=true;
		}


	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}

bool DataWrapper::DeleteCloseOrderMap(OrderData &data)
{
	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_closeordermutex);  // 函数结束时，自动析构解锁
		std::string key =data.OrderRef;

		std::shared_ptr<OrderData> e = std::make_shared<OrderData>(data);

		if (m_closeordermap.find(key) == m_closeordermap.end()) 		{
			m_closeordermap.erase(key);
			ret=true;
		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught CTraderSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool DataWrapper::ExistinCloseOrderMap(const char* instrumentid,const int direction)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_closeordermutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<OrderData> >::iterator item;
			for(item=m_closeordermap.begin();item!=m_closeordermap.end();item++){
				if(strcmp(item->second->InstrumentID,instrumentid)==0&& item->second->Direction==direction){
					ret=true;
				}

			}

	} catch(std::logic_error&) {
		std::cout << "[exception caught DataWrapper::ExistinCloseOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}

bool DataWrapper::GetOrderQueueList(vector<OrderData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_closeordermutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<OrderData> >::iterator item;
			for(item=m_closeordermap.begin();item!=m_closeordermap.end();item++){
					OrderData  data;
					strcpy(data.InstrumentID, item->second->InstrumentID);
					strcpy(data.TradingDay, item->second->TradingDay);
					strcpy(data.ExchangeID, item->second->ExchangeID);
					strcpy(data.OrderRef,item->second->OrderRef);
					strcpy(data.OrderSysID,item->second->OrderSysID);
					data.FrontID=item->second->FrontID;
					data.SessionID=item->second->SessionID;
					data.CombOffsetFlag=item->second->CombOffsetFlag;
					data.VolumeTotalOriginal = item->second->VolumeTotalOriginal;
					data.LimitPrice = item->second->LimitPrice;
					data.Direction= item->second->Direction;
					data.Volume= item->second->Volume;
					data.Status=item->second->Status;
					data.OrderStatus=item->second->OrderStatus;
					data.OrderSubmitStatus=item->second->OrderSubmitStatus;
					list.push_back(data);
			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetCloseOrderQueueList >>> ]\n";
			ret=false;

		}
	return ret;
}
bool DataWrapper::GetCloseOrderQueueList(vector<OrderData> &list)
{
	 bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_closeordermutex);  // 函数结束时，自动析构解锁
			map<std::string, std::shared_ptr<OrderData> >::iterator item;
			for(item=m_closeordermap.begin();item!=m_closeordermap.end();item++){

				if(item->second->Status==1){
					OrderData  data;
					strcpy(data.InstrumentID, item->second->InstrumentID);
					strcpy(data.TradingDay, item->second->TradingDay);
					strcpy(data.ExchangeID, item->second->ExchangeID);
					strcpy(data.OrderRef,item->second->OrderRef);
					strcpy(data.OrderSysID,item->second->OrderSysID);
					data.FrontID=item->second->FrontID;
					data.SessionID=item->second->SessionID;
					data.CombOffsetFlag=item->second->CombOffsetFlag;
					data.VolumeTotalOriginal = item->second->VolumeTotalOriginal;
					data.LimitPrice = item->second->LimitPrice;
					data.Direction= item->second->Direction;
					data.Volume= item->second->Volume;
					data.Status=item->second->Status;
					data.OrderStatus=item->second->OrderStatus;
					data.OrderSubmitStatus=item->second->OrderSubmitStatus;
					list.push_back(data);
				}

			}

			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::GetCloseOrderQueueList >>> ]\n";
			ret=false;

		}
	return ret;
}
bool DataWrapper::UpdateDepthMarketMap()
{
//	 struct timeval start, end;
//	 gettimeofday( &start, NULL );
//	printf("/////////////////UpdateDepthMarketMap start ----------------> %ld.%ld\n", start.tv_sec, start.tv_usec);
	bool ret=false;
	try {
			std::lock_guard<std::mutex>lck(m_depthmarketmutex);  // 函数结束时，自动析构解锁
			m_depthmarketmap.clear();
			for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
					item != this->GetContractLists()->end(); item++){


				if( m_redisdepthmarket.Exists(item->InstrumentID)> 0 ){

					DepthMarket depthmarket = m_redisdepthmarket.GetDepthMarket(item->InstrumentID);
//					std::cout<<"++++++++++++++++++"<<item->InstrumentID<<"dep updatetime:"
//							<<depthmarket.UpdateTime<<" mseconds:"<<depthmarket.UpdateMillisec<<std::endl;

					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					if (valid_depthmarket){
						std::string key =item->InstrumentID;
						std::shared_ptr<DepthMarket> mdata = std::make_shared<DepthMarket>(depthmarket);
						m_depthmarketmap.insert(std::pair<std::string, std::shared_ptr<DepthMarket>>(key, mdata));

//					m_depthmarketmap.insert(map<string,std::shared_ptr<DepthMarket>>::value_type(key,mdata));
//					std::cout<<"////////////////////market data  ---> "<<item->InstrumentID<<"   updatetime:"<<item->UpdateTime<<
//											"   lastprice:"<<item->LastPrice<<std::endl;
					}
				}
//				else{
////					std::cout<<"///////////////////no data"<<std::endl;
//				}
			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught DataWrapper::UpdateDepthMarketMap >>> ]\n";
			ret=false;
	}

//		 gettimeofday( &end, NULL );
//				printf("/////////////////end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
	return ret;
}
bool DataWrapper::ValidCheck_DepthMarket(DepthMarket *depthmarket){

	bool ret=false;

	bool valid_upperlimitprice = depthmarket->UpperLimitPrice >1.0 ;   // 涨停价
	if(!valid_upperlimitprice){
		printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 涨停价数据不合规 [%.3f]  \n",
				depthmarket->InstrumentID,depthmarket->UpperLimitPrice);

		return false;
	}

	bool valid_lowerlimitprice = depthmarket->LowerLimitPrice >1.0 ;  //跌停价
	if(!valid_lowerlimitprice){
		printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 跌停价数据不合规 [%.3f]  \n",
				depthmarket->InstrumentID,depthmarket->LowerLimitPrice);
		return false;
	}

	bool valid_lastprice_base  = depthmarket->LastPrice >1.0 ;
	bool valid_lastprice_normal = depthmarket->LastPrice >= depthmarket->LowerLimitPrice  &&
			depthmarket->LastPrice <= depthmarket->UpperLimitPrice;

	bool valid_lastprice= valid_lastprice_base && valid_lastprice_normal;

	if(!valid_lastprice){
		printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> lastprice [%.3f] 价格不合规  \n",
				depthmarket->InstrumentID,depthmarket->LastPrice);
		return false;
	}
	bool valid1 = depthmarket->OpenPrice >1.0 ;
	bool valid2 = depthmarket->PreSettlementPrice >1.0 ;
	bool valid3 = depthmarket->PreClosePrice >1.0 ;
	bool valid6 = depthmarket->AveragePrice >1.0 ;

	bool valid_ask_base = depthmarket->AskPrice1 >1.0 ;
	bool valid_ask_normal= depthmarket->AskPrice1 <=depthmarket->UpperLimitPrice &&
			depthmarket->AskPrice1 >=depthmarket->LowerLimitPrice;    //跌停 ask = lower
	bool valid_uplimit_ask =  depthmarket->AskPrice1 ==0.0 ||  depthmarket->AskPrice1>1000000000000.0;

	bool valid_bid_base = depthmarket->BidPrice1 >1.0 ;
	bool valid_bid_normal= depthmarket->BidPrice1 <=depthmarket->UpperLimitPrice &&
				depthmarket->BidPrice1 >=depthmarket->LowerLimitPrice;   //涨停 bid = upper
	bool valid_lowlimit_bid =  depthmarket->BidPrice1 ==0.0 ||  depthmarket->BidPrice1>1000000000000.0;

	bool valid_ask = valid_ask_base&& valid_ask_normal;
	bool valid_bid = valid_bid_base&& valid_bid_normal;

	bool valid_uplimit = valid_uplimit_ask && valid_bid;   //涨停
	bool valid_lowlimit =valid_lowlimit_bid &&valid_ask;  //跌停

	if(!valid_ask){

		//再查看是否处于涨停状态,如果处于涨停状态，则按照正常情况通过
		if(!valid_uplimit){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> AskPrice1 申卖价一 [%.3f]  up[%.3f]  down[%.3f] 不合规!!!!   \n",
					depthmarket->InstrumentID,depthmarket->AskPrice1,depthmarket->UpperLimitPrice,depthmarket->LowerLimitPrice);
			return false;
		}
	}

	if(!valid_bid){

		//再查看是否处于跌停状态,如果处于跌停状态，则按照正常情况通过
		if(!valid_lowlimit){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> BidPrice1 申买价一 [%.3f] up[%.3f]  down[%.3f]  不合规!!!! \n",
					depthmarket->InstrumentID,depthmarket->BidPrice1,depthmarket->UpperLimitPrice,depthmarket->LowerLimitPrice);
			return false;
		}

	}

	bool valid9 = depthmarket->VolumeMultiple >=1  &&  depthmarket->VolumeMultiple <=20000 ;
	bool valid_margin = depthmarket->Margin >0.01 && depthmarket->Margin < 0.3 ;

	if(valid1 && valid2  && valid3 && valid6   && valid9 && valid_margin){
		ret = true;
	}
	else{
		if (!valid1){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 开盘价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->OpenPrice);
		}
		else if (!valid2){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 昨结算价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->PreSettlementPrice);
		}
		else if (!valid3){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 昨收盘价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->PreClosePrice);
		}
		else if (!valid6){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 均价数据为零 [%.3f]  \n",depthmarket->InstrumentID,depthmarket->AveragePrice);
		}
		else if (!valid9){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> 交易单位[%d] 不合规,超出范围[1-20000]   \n",depthmarket->InstrumentID,
					depthmarket->VolumeMultiple);
		}
		else if (!valid_margin){
			printf("	ValidCheck_DepthMarket 行情数据不合规: %s  --> Margin[%.2f] 不合规,超出范围[0.01-0.3]   \n",depthmarket->InstrumentID,
					depthmarket->Margin);
		}
		else {

		}

	}
	return ret;
}
bool DataWrapper::ValidCheck_DayLineNdays(MysqlDayLine *mysqldayline,const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int N){

//	printf("ValidCheck_DayLine [%s]   交易日[%s]     exchange: %s \n",pinstrumentid,tradingdate,exchangeid);
	bool ret=false;


	DateUtil dateutil;
	std::string str_sqltradingday = dateutil.ConvertDatetoSqlTime(tradingdate);

	bool valid5;
	int count;

	if(mysqldayline->Exist_Table(pinstrumentid))	{
		count=mysqldayline->Count_AllListsbyTableName(pinstrumentid);
		if(count==0)		{
			printf("ValidCheck_DayLineNdays [%s]   日线无数据,需要更新     exchange: %s \n",pinstrumentid,exchangeid);

			count =mysqldayline->Count_AllListsbyTableName(pinstrumentid);
	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> ccc \n",pinstrumentid);
		}
	}
	else	{
		printf("ValidCheck_DayLineNdays [%s]   不存在日线数据表     exchange: %s \n",pinstrumentid,exchangeid);
		mysqldayline->CreateTable(pinstrumentid);

		count =mysqldayline->Count_AllListsbyTableName(pinstrumentid);
	}
	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> bbb \n",pinstrumentid);

	std::string dayline_lastdate =mysqldayline->GetLastDateTime(pinstrumentid);
	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> bbb1 \n",pinstrumentid);
	time_t tm_lastdate=dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());
	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> bbb2 \n",pinstrumentid);

	std::string path = "/root/autotrader/config/";
	std::string filename = "holiday.csv";
	time_t tm_enddayline =  dateutil.CheckHisLastDayLinebyDate_ExcludeHoliday(
			tradingdate,this->GetDifSec(),path.c_str(),filename.c_str());
	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> bbb3 \n",pinstrumentid);

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(str_sqltradingday.c_str());

	printf("	[%s] dayline  ------>tm_yesterday [%s]: %ld    tm_dayline [%s]: %ld     \n  ",pinstrumentid,
					dayline_lastdate.c_str(),tm_lastdate,tradingdate,tm_enddayline);

	if (tm_lastdate ==tm_enddayline)	{
		valid5 =true;
	}
	else	{
		printf("+++++++ValidCheck_DayLineNdays  [%s]-----> 补齐数据 尝试   \n",pinstrumentid);

		std::string dayline_lastdate_new =mysqldayline->GetLastDateTime(pinstrumentid);
		time_t tm_lastdate_new=dateutil.ConvertSqlTimetoTimet(dayline_lastdate_new.c_str());
		if (tm_lastdate_new ==tm_enddayline){
				valid5 =true;
		}
		else if(tm_lastdate_new >tm_enddayline){
			valid5 =true;
			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
//			printf("ValidCheck_DayLineNdays[%s]-->	dayline 数据大于截止日期 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
//								str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);
		}
		else{
			valid5=false;
			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
			printf("++++++++++ValidCheck_DayLineNdays[%s]-->	dayline 数据缺失 截止日期不相同 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
					str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);

		}
	}


	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> aaa \n",pinstrumentid);
	bool valid0 = count > N;

	bool b_holiday = dateutil.CheckHoliday_SqlTime(str_sqltradingday.c_str());   //判断当前交易日是否是假日

	ret = valid0 && valid5 && !b_holiday;

	if (!ret)	{
		if (b_holiday)		{
			printf("	当前交易日未假日: %s  --> 休市日 %s \n",pinstrumentid,str_sqltradingday.c_str());
		}

		if (!valid0)			{
				printf("	行情数据不合规: %s  --> 日线数据长度[%d]不足 %d \n",pinstrumentid,count,N);

		}

		if (!valid5)			{
			printf("	历史数据不合规: %s  --> 日线数据缺失 最后更新日期[%s]  \n",pinstrumentid,dayline_lastdate.c_str());
		}
	}
	printf("+++++++ValidCheck_DayLineNdays  [%s]-----> end \n",pinstrumentid);


	return ret;
}


void DataWrapper::InitialSectionMap()
{
	MysqlSection mysqlsection;
	vector<std::string>  sectionname_lists;
	std::unique_lock<std::mutex>lck(m_sectionmutex);
	m_sectionmap.clear();
	mysqlsection.Find_SectionLists(sectionname_lists);
	for (auto &item:sectionname_lists){

		std::shared_ptr<SectionInfo>info = std::make_shared<SectionInfo>();
		strcpy(info->SectionName,item.c_str());
		info->UpCount=0;
		info->DownCount=0;
		info->ZeroCount=0;
		info->TotalUprate=0;
		info->TotalDownrate=0;
		info->Maxrate=0;
		info->Minrate=1;
		info->CountUprate=0;
		info->CountDownrate=0;
		info->AverageUpDownrate=0;
		info->Sedimentary=0;
		info->SedimentaryIO=0;
		info->SedimentaryIORate=0;

		std::string key=info->SectionName;
		m_sectionmap.insert(std::pair<std::string, std::shared_ptr<SectionInfo>>(key, info));
	}

	vector<std::string>().swap(sectionname_lists);
}

void DataWrapper::SectionMapView()
{

//	std::cout<<"++++++++++++++++++++ SectionMapView 1111111111111111111111111 "<<endl;
	DateUtil dateutil;
	std::unique_lock<std::mutex>lck(m_sectionmutex);

	m_sectionmap.clear();

	for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
				item != this->GetContractLists()->end(); item++){

		bool exist = m_redisdepthmarket.Exists(item->InstrumentID)> 0;
		if(!exist){
			continue;
		}

//		std::cout<<"++++++++++++++sectionmapview:"<<item->InstrumentID<<"    map size:"<<m_sectionmap.size()<<endl;
		DepthMarket depthmarket;
		depthmarket = m_redisdepthmarket.GetDepthMarket(item->InstrumentID);
		std::string key=item->Section;

		if(m_sectionmap.find(key)!=m_sectionmap.end()){

//			std::cout<<"++++++++++++++sectionmapview   find ok:"<<item->InstrumentID<<endl;
			if(depthmarket.UpdownRate >0 )	{
				m_sectionmap[key]->UpCount++;
				m_sectionmap[key]->TotalUprate+=depthmarket.UpdownRate;
			}
			else if(depthmarket.UpdownRate <0)	{
				m_sectionmap[key]->DownCount++;
				m_sectionmap[key]->TotalDownrate+=depthmarket.UpdownRate;
			}
			else{
				m_sectionmap[key]->ZeroCount++;
			}

			if(depthmarket.UpdownRate>m_sectionmap[key]->Maxrate)	{
				m_sectionmap[key]->Maxrate = depthmarket.UpdownRate;
				strcpy(m_sectionmap[key]->MaxInstrumentID,depthmarket.InstrumentID);
			}

			if(depthmarket.UpdownRate<m_sectionmap[key]->Minrate)	{
				m_sectionmap[key]->Minrate = depthmarket.UpdownRate;
				strcpy(m_sectionmap[key]->MinInstrumentID,depthmarket.InstrumentID);
			}

			m_sectionmap[key]->Sedimentary+=depthmarket.Sedimentary;
			m_sectionmap[key]->SedimentaryIO += depthmarket.Sedimentary - depthmarket.LastSedimentary;
		}
		else{
//			std::cout<<"++++++++++++++sectionmapview   no find:"<<item->InstrumentID<<endl;

			std::shared_ptr<SectionInfo> sectioninfo = std::make_shared<SectionInfo>();
			strcpy(sectioninfo->SectionName,item->Section);
			if(depthmarket.UpdownRate >0 )	{
				sectioninfo->UpCount=1;
				sectioninfo->TotalUprate=depthmarket.UpdownRate;
			}
			else if(depthmarket.UpdownRate <0)	{
				sectioninfo->DownCount=1;
				sectioninfo->TotalDownrate=depthmarket.UpdownRate;
			}
			else{
				sectioninfo->ZeroCount=1;
				sectioninfo->TotalUprate=0;
				sectioninfo->TotalDownrate=0;
			}
			sectioninfo->Maxrate = depthmarket.UpdownRate;
			strcpy(sectioninfo->MaxInstrumentID,depthmarket.InstrumentID);
			sectioninfo->Minrate = depthmarket.UpdownRate;
			strcpy(sectioninfo->MinInstrumentID,depthmarket.InstrumentID);
			sectioninfo->Sedimentary=depthmarket.Sedimentary;
			sectioninfo->SedimentaryIO = depthmarket.Sedimentary - depthmarket.LastSedimentary;

			m_sectionmap.insert(std::pair<std::string, std::shared_ptr<SectionInfo>>(key, sectioninfo));

//			std::cout<<"++++++++++++++sectionmapview   insert new :"<<item->InstrumentID<<endl;
		}
	}

//	std::cout<<"++++++++++++++++++++ SectionMapView 22222222222222222222222222222 "<<endl;

	for (map<std::string,std::shared_ptr<SectionInfo>>::iterator it = m_sectionmap.begin();
			it != m_sectionmap.end(); it++){

			int updowncount = it->second->UpCount+it->second->DownCount+it->second->ZeroCount;


			if(it->second->Sedimentary>0){
//				std::cout<<"   ++++++++++++before:"<<it->second->SedimentaryIORate<<endl;
				it->second->SedimentaryIORate = it->second->SedimentaryIO/it->second->Sedimentary;
//				std::cout<<"   ++++++++++++after:"<<it->second->SedimentaryIORate<<endl;
			}
			else{
				it->second->SedimentaryIORate=0.0;
			}
//			printf("SectionMarketView  %s-->   Sedimentary:%.2f  SedimentaryIO:%.2f  SedimentaryIORate:%.2f%% \n",it->second->SectionName,
//					it->second->Sedimentary,it->second->SedimentaryIO,it->second->SedimentaryIORate*100);


			if(updowncount>0)	{
				it->second->CountDownrate = (double)((double)it->second->DownCount/((double)updowncount));
			}
			else{
				it->second->CountDownrate=0.0;
			}
			if(updowncount>0)	{
				it->second->CountUprate =(double)((double)it->second->UpCount/((double)updowncount));
			}
			else{
				it->second->CountUprate=0.0;
			}

			if(updowncount>0)	{
				it->second->AverageUpDownrate = (it->second->TotalDownrate +it->second->TotalUprate)/((double)updowncount);
			}
			else{
				it->second->AverageUpDownrate =0.0;
			}


			//
//			if(it->second->CountUprate>0.7){
//				printf("+++++++++++++++%s---板块联动(上涨行情)--> 上涨品种涨幅[%d]: %.2f%%  上涨品种比例:%.2f%%   SedimentaryIO:%.2f  资金流动比例: %.2f%%  平均涨跌幅: %.2f%% 最高涨跌幅:%.2f%%--> [%s]\n",
//							it->second->SectionName,it->second->UpCount,it->second->TotalUprate*100,it->second->CountUprate*100,it->second->SedimentaryIO,it->second->SedimentaryIORate*100,it->second->AverageUpDownrate*100,
//							it->second->Maxrate*100,it->second->MaxInstrumentID);
//
//			}
//			else if(it->second->CountDownrate>0.7)	{
//				printf("+++++++++++++++%s---板块联动(下跌行情)--> 下跌品种总跌幅[%d]: %.2f%%  下跌品种比例:%.2f%%  SedimentaryIO:%.2f  资金流动比例: %.2f%%  平均涨跌幅: %.2f%% 最低涨跌幅:%.2f%%--> [%s]\n",
//						it->second->SectionName,it->second->DownCount,it->second->TotalDownrate*100,it->second->CountDownrate*100,it->second->SedimentaryIO,it->second->SedimentaryIORate*100,it->second->AverageUpDownrate*100,
//						it->second->Minrate*100,it->second->MinInstrumentID);
//
//			}
//			else{
//				printf("+++++++++++++++%s---板块整体走势不明晰,继续观察中----------上涨品种比例[%.2f%%]--------下跌品种比例[%.2f%%]----   SedimentaryIO:%.2f  SedimentaryIORate:%.2f%% \n",
//						it->second->SectionName,it->second->CountUprate*100,it->second->CountDownrate*100,it->second->SedimentaryIO,it->second->SedimentaryIORate*100);
//			}



	}
//	std::cout<<"++++++++++++++++++++ SectionMapView   end-----------------!!! "<<endl;
}

void DataWrapper::GlobalMarketView()
{
	DateUtil dateutil;
	CodeUtil codeutil;
	GlobalViewData globalview;
	memset(&globalview,0,sizeof(globalview));

	for(std::vector<MainContractData>::size_type i = 0; i != m_contractlist.size(); i++){
		char pinstrumentid[40];
		strcpy(pinstrumentid,m_contractlist[i].InstrumentID);

		bool exist = m_redisdepthmarket.Exists(pinstrumentid)> 0;
		if(!exist){

//			std::cout<<">>>>>>>>>>>>>>>>>>>>无数据合约 count:"<<ins_count<<endl;
			continue;
		}

		DepthMarket depthmarket = m_redisdepthmarket.GetDepthMarket(pinstrumentid);

		std::shared_ptr<Commission>  commssion=this->FindCommissionData(m_contractlist[i].ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*m_contractlist[i].VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/(100000000.0);

		//全市场沉淀资金统计
		globalview.TotalCapital+=sedimentary;


		double SedimentaryIO = depthmarket.Sedimentary -depthmarket.LastSedimentary;
		//全市场沉淀资金流入流出总计
		globalview.TotalSedimentaryIO+=SedimentaryIO;


		if(SedimentaryIO>0){
			globalview.TotalSedimentaryIn+=SedimentaryIO; //全市场沉淀资金总流入
		}
		else if(SedimentaryIO<0){
			globalview.TotalSedimentaryOut+=SedimentaryIO; //全市场沉淀资金总流出
		}

		if(depthmarket.UpdownRate>0){
			globalview.Total_Up+=depthmarket.UpdownRate;      //所有上涨合约涨幅总计
			globalview.Up_Count++;        //所有上涨合约数量总计
		}
		else if(depthmarket.UpdownRate<0){
			globalview.Total_Down+=depthmarket.UpdownRate;  //所有下跌合约跌幅总计
			globalview.Down_Count++;    //所有下跌合约数量总计
		}

		if(i==0){
			globalview.MaxSedimentaryIO=SedimentaryIO;
			strcpy(globalview.MaxSedimentaryIOIns,pinstrumentid);

			globalview.MinSedimentaryIO=SedimentaryIO;
			strcpy(globalview.MinSedimentaryIOIns,pinstrumentid);

			globalview.MaxSedimentaryIORate=depthmarket.SedimentaryIORate;
			strcpy(globalview.MaxSedimentaryIORateIns,pinstrumentid);

			globalview.MinSedimentaryIORate=depthmarket.SedimentaryIORate;
			strcpy(globalview.MinSedimentaryIORateIns,pinstrumentid);

			globalview.MaxUpDown=depthmarket.UpdownRate;
			strcpy(globalview.MaxUpDownIns,pinstrumentid);

			globalview.MinUpDown=depthmarket.UpdownRate;
			strcpy(globalview.MinUpDownIns,pinstrumentid);
		}
		else{
			if(SedimentaryIO>globalview.MaxSedimentaryIO)	{
				globalview.MaxSedimentaryIO=SedimentaryIO;
				strcpy(globalview.MaxSedimentaryIOIns,pinstrumentid);
			}

			if(SedimentaryIO<globalview.MinSedimentaryIO)	{
				globalview.MinSedimentaryIO=SedimentaryIO;
				strcpy(globalview.MinSedimentaryIOIns,pinstrumentid);
			}

			if(depthmarket.SedimentaryIORate>globalview.MaxSedimentaryIORate)	{
				globalview.MaxSedimentaryIORate=depthmarket.SedimentaryIORate;
				strcpy(globalview.MaxSedimentaryIORateIns,pinstrumentid);
			}

			if(depthmarket.SedimentaryIORate<globalview.MinSedimentaryIORate)	{
				globalview.MinSedimentaryIORate=depthmarket.SedimentaryIORate;
				strcpy(globalview.MinSedimentaryIORateIns,pinstrumentid);
			}

			if(depthmarket.UpdownRate>globalview.MaxUpDown)	{
				globalview.MaxUpDown=depthmarket.UpdownRate;
				strcpy(globalview.MaxUpDownIns,pinstrumentid);
			}

			if(depthmarket.UpdownRate<globalview.MinUpDown)	{
				globalview.MinUpDown=depthmarket.UpdownRate;
				strcpy(globalview.MinUpDownIns,pinstrumentid);
			}
		}
	}

	if(globalview.TotalCapital<=0){
		return;
	}

	std::string currenttime = dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec());
	strcpy(globalview.Time,currenttime.c_str());
	strcpy(globalview.TradingDay,m_tradingday.c_str());
	globalview.TotalSedimentaryIORate = globalview.TotalSedimentaryIO/globalview.TotalCapital;
	globalview.CurrentCount_InstrumentID = globalview.Up_Count+globalview.Down_Count;

	if(globalview.Up_Count>0){
		globalview.Average_Up=globalview.Total_Up*100/(double)globalview.Up_Count;
	}
	else{
		globalview.Average_Up=0.0;
	}

	if(globalview.Down_Count>0){
		globalview.Average_Down=globalview.Total_Down*100/(double)globalview.Down_Count;
	}
	else {
		globalview.Average_Down=0.0;
	}

//
//			this->StatisticsSectionSedminentaryIO(&view);
//
	double globalview_longweight=0.0	;
	double  globalview_shortweight=0.0	 ;
	if (globalview.CurrentCount_InstrumentID>0){
		globalview_longweight= (double )globalview.Up_Count /(double )globalview.CurrentCount_InstrumentID ;
		globalview_shortweight = (double )globalview.Down_Count /(double )globalview.CurrentCount_InstrumentID ;
	}
	else {
		globalview_longweight=0.0;
		globalview_shortweight=0.0;
	}

	// 整体市场多头 空头 需要超过75%的合约 同涨或者同跌
	double longshort_weight;

	bool day=dateutil.CheckSHFEDayTime(this->GetDifSec());
	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());

	if(day){
		longshort_weight = 0.8;
	}
	else{
		longshort_weight = 0.85;
	}

	bool valid_globalview_long = globalview_longweight>longshort_weight && globalview.Average_Up >0.8;
	bool valid_globalview_short = globalview_shortweight>longshort_weight  && globalview.Average_Down <-0.8;

	if(valid_globalview_long){

		globalview.Status=0;
//				printf(">>>>>>>>>>>>>>>!!!整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
//				this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100);
	}
	else if(valid_globalview_short){
		globalview.Status=1;
//				printf(">>>>>>>>>>>>>>>!!!整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
//				this->GetGlobalViewData()->Down_Count,this->GetGlobalViewData()->Up_Count,globalview_shortweight*100);
	}
	else{
		globalview.Status=2;
//				printf(">>>>>>>>>>>>>>>!!!整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
//				this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100,globalview_shortweight*100);
	}

	globalview.LongWeight = globalview_longweight;
	globalview.ShortWeight= globalview_shortweight;


	if(fabs(globalview.MaxUpDown)>fabs(globalview.MinUpDown)){
		strcpy(globalview.AbsMaxUpDownIns,globalview.MaxUpDownIns);
		globalview.AbsMaxUpDown=globalview.MaxUpDown;
//		printf("++++++++++++++++++GlobalMarketView   多头合约优选----->%s   [%.2f%%] \n",
//				globalview.AbsMaxUpDownIns,globalview.MaxUpDown*100);
	}
	else if(fabs(globalview.MaxUpDown)<fabs(globalview.MinUpDown)){
		strcpy(globalview.AbsMaxUpDownIns,globalview.MinUpDownIns);
		globalview.AbsMaxUpDown=globalview.MinUpDown;
//		printf("++++++++++++++++++GlobalMarketView   空头合约优选----->%s   [%.2f%%] \n",
//				globalview.AbsMaxUpDownIns,globalview.MinUpDown*100);
	}

	this->InputGlobalViewData(&globalview);  //临时变量写入私有变量
}
void DataWrapper::Analysis()
{

	char logbuf[1024];
//	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

	GlobalViewData v;
	memset(&v,0,sizeof(v));

	this->OutputGlobalViewData(&v);
	GlobalViewData *view=&v;

//	if(view->TotalSedimentaryIORate>0 && view->TotalSedimentaryIORate<0.05){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流入弱 <\n");
//	}
//	else if(view->TotalSedimentaryIORate>=0.05 && view->TotalSedimentaryIORate<0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流入小 <<< \n");
//	}
//	else if(view->TotalSedimentaryIORate>=0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流入大 <<<<<<<<<< \n");
//	}
//	else if(view->TotalSedimentaryIORate<0 && view->TotalSedimentaryIORate>-0.05){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流出弱 >\n");
//	}
//	else if(view->TotalSedimentaryIORate<=-0.05 && view->TotalSedimentaryIORate>-0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流出小 >>> \n");
//	}
//	else if(view->TotalSedimentaryIORate<=-0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流出大 >>>>>>>>>> \n");
//	}


	bool max_rateup_ins=strcmp(view->MaxSedimentaryIORateIns,view->MaxUpDownIns)==0;
//	bool max_ins=strcmp(view->MaxSedimentaryIORateIns,view->MaxSedimentaryIOIns)==0;
	bool max_ratelevel=view->MaxSedimentaryIORate>0.008;
	bool max_sedio=view->MaxSedimentaryIO>0.5;


	bool min_rateup_ins=strcmp(view->MinSedimentaryIORateIns,view->MinUpDownIns)==0;
//	bool min_ins=strcmp(view->MinSedimentaryIORateIns,view->MinSedimentaryIOIns)==0;
	bool min_ratelevel=view->MinSedimentaryIORate<-0.008;
	bool min_sedio=view->MinSedimentaryIO<-0.5;


	if (m_globalview.CurrentCount_InstrumentID >15){

		// 0 - 第一笔初始记录
		// 1 - 多头，震荡，空头状态发生变化
		//2- 资金流入流出最大合约发生变化
		//3- 涨跌幅绝对值最大合约发生变化

//		MysqlGlobalView mysqlglobalview(this->GetUserID().c_str());
//		printf("GetAbsMaxUpDownIns  整体市场------> mysqlglobalview userid ----->  %s  \n",this->GetUserID().c_str());
//		if(mysqlglobalview.Exist_Table() && mysqlglobalview.Count_ColumnsbyTableName()<37){
//			printf("	DropSqlTable:drop数据表  mysqlglobalview !!!\n");
//			mysqlglobalview.DropTable();
//		}
//
//		if(!mysqlglobalview.Exist_Table()){
//			mysqlglobalview.CreateTable();
//		}
//
//		if(!mysqlglobalview.Exist_DataByTradingDay(view->TradingDay)){
//			sprintf(logbuf,"  整体市场------> 并不存在本交易日任何全局市场数据  写入本交易日第一个全局数据 ----->  %s  \n",view->TradingDay);
//			logutil.WriteLog(logbuf);
//			v.StatusChangeReason=0;
//			mysqlglobalview.Insert(v);
//		}
//
//		if(!mysqlglobalview.Exist_DataByTime(view->Time)){
//
//			GlobalViewData lastview=mysqlglobalview.Find_LastDatabyTradingDay(view->TradingDay);
//			if(lastview.Status!=view->Status  ){
//
//				if(lastview.Status==0 || view->Status==0){
//					if(fabs(view->LongWeight-lastview.LongWeight)>0.03){
//						sprintf(logbuf,"  整体市场------> 行情转变  Status: %d ----->  %d \n",lastview.Status,view->Status);
//						logutil.WriteLog(logbuf);
//						v.StatusChangeReason=1;
//						mysqlglobalview.Insert(v);
//					}
//				}
//				else if(lastview.Status==1 || view->Status==1){
//					if(fabs(view->ShortWeight-lastview.ShortWeight)>0.03){
//						sprintf(logbuf,"  整体市场------> 行情转变  Status: %d ----->  %d \n",lastview.Status,view->Status);
//						logutil.WriteLog(logbuf);
//						v.StatusChangeReason=1;
//						mysqlglobalview.Insert(v);
//					}
//				}
//
//			}
//			else if(max_rateup_ins&&max_ratelevel&&max_sedio && strcmp(lastview.MaxSedimentaryIORateIns,view->MaxSedimentaryIORateIns)!=0){
////					printf("	整体分析: 多头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MaxSedimentaryIORateIns,view->MaxSedimentaryIO,
////					view->MaxSedimentaryIORate*100,view->MaxUpDown*100);
//					sprintf(logbuf,"	整体分析: 多头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MaxSedimentaryIORateIns,view->MaxSedimentaryIO,
//							view->MaxSedimentaryIORate*100,view->MaxUpDown*100);
//					logutil.WriteLog(logbuf);
//					v.StatusChangeReason=2;
//					mysqlglobalview.Insert(v);
//				}
//
//			else if(min_rateup_ins&&min_ratelevel&&min_sedio && strcmp(lastview.MinSedimentaryIORateIns,view->MinSedimentaryIORateIns)!=0){
////					printf("	整体分析: 空头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MinSedimentaryIORateIns,view->MinSedimentaryIO,
////					view->MinSedimentaryIORate*100,view->MinUpDown*100);
//					sprintf(logbuf,"	整体分析: 空头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MinSedimentaryIORateIns,view->MinSedimentaryIO,
//							view->MinSedimentaryIORate*100,view->MinUpDown*100);
//					logutil.WriteLog(logbuf);
//					v.StatusChangeReason=2;
//					mysqlglobalview.Insert(v);
//				}
//			else if(strcmp(lastview.AbsMaxUpDownIns,view->AbsMaxUpDownIns)!=0){
//
//				sprintf(logbuf,"	整体分析----> 涨跌幅最大合约更新  %s >>>>>  %s  [%.4f]\n",
//						lastview.AbsMaxUpDownIns,view->AbsMaxUpDownIns,view->AbsMaxUpDown);
//				logutil.WriteLog(logbuf);
//				v.StatusChangeReason=3;
//				mysqlglobalview.Insert(v);
//			}
////			else if (view->MaxSedimentaryIORate > lastview.MaxSedimentaryIORate+0.01 && view->MaxSedimentaryIORate >0.07){
////				printf("  整体市场------> 单合约最大资金流入比 改变 %s MaxSedimentaryIORate: %.2f%% ----->  %s MaxSedimentaryIORate: %.2f%%  \n",
////						lastview.MaxSedimentaryIORateIns,lastview.MaxSedimentaryIORate*100,
////						view->MaxSedimentaryIORateIns,view->MaxSedimentaryIORate*100);
////				mysqlglobalview.Insert(v);
////			}
////			else if(view->MinSedimentaryIORate < lastview.MinSedimentaryIORate-0.01 && view->MinSedimentaryIORate <-0.05){
////				printf("  整体市场------> 单合约最大资金流出比 改变 %s MinSedimentaryIORate: %.2f%% ----->  %s MinSedimentaryIORate: %.2f%%  \n",
////						lastview.MinSedimentaryIORateIns,lastview.MinSedimentaryIORate*100,
////						view->MinSedimentaryIORateIns,view->MinSedimentaryIORate*100);
////				mysqlglobalview.Insert(v);
////			}
//		}
	}
}
GlobalViewData * DataWrapper::GetGlobalViewData()
{
	return &m_globalview;
}
inline void DataWrapper::WriteGlobalViewData(GlobalViewData * sourcedata,GlobalViewData * targetdata)
{

	if(sourcedata->TotalCapital>0.0 ){

		strcpy(targetdata->Time,sourcedata->Time);
		strcpy(targetdata->TradingDay,sourcedata->TradingDay);
		targetdata->TotalCapital = sourcedata->TotalCapital;
		targetdata->TotalSedimentaryIO =  sourcedata->TotalSedimentaryIO;
		targetdata->TotalSedimentaryIORate =  sourcedata->TotalSedimentaryIORate;
		targetdata->TotalSedimentaryIn = sourcedata->TotalSedimentaryIn;
		targetdata->TotalSedimentaryOut = sourcedata->TotalSedimentaryOut;
		targetdata->TotalSedimentaryInRate = sourcedata->TotalSedimentaryInRate;
		targetdata->TotalSedimentaryOutRate = sourcedata->TotalSedimentaryOutRate;

		//资金流入流出金额
		targetdata->MaxSedimentaryIO = sourcedata->MaxSedimentaryIO;
		strcpy(targetdata->MaxSedimentaryIOIns,sourcedata->MaxSedimentaryIOIns);
		targetdata->MinSedimentaryIO = sourcedata->MinSedimentaryIO;
		strcpy(targetdata->MinSedimentaryIOIns,sourcedata->MinSedimentaryIOIns);

		//资金流入流出比例
		targetdata->MaxSedimentaryIORate = sourcedata->MaxSedimentaryIORate;
		strcpy(targetdata->MaxSedimentaryIORateIns,sourcedata->MaxSedimentaryIORateIns);
		targetdata->MinSedimentaryIORate = sourcedata->MinSedimentaryIORate;
		strcpy(targetdata->MinSedimentaryIORateIns,sourcedata->MinSedimentaryIORateIns);

		targetdata->Up_Count = sourcedata->Up_Count;
		targetdata->Down_Count = sourcedata->Down_Count;
		targetdata->CurrentCount_InstrumentID = sourcedata->CurrentCount_InstrumentID;
		targetdata->LongWeight = sourcedata->LongWeight;
		targetdata->ShortWeight = sourcedata->ShortWeight;

		targetdata->Total_Up =  sourcedata->Total_Up;
		targetdata->Total_Down = sourcedata->Total_Down;
		targetdata->Average_Up = sourcedata->Average_Up;
		targetdata->Average_Down = sourcedata->Average_Down;

		targetdata->MaxUpDown = sourcedata->MaxUpDown;
		targetdata->MinUpDown = sourcedata->MinUpDown;
		strcpy(targetdata->MaxUpDownIns,sourcedata->MaxUpDownIns);
		strcpy(targetdata->MinUpDownIns,sourcedata->MinUpDownIns);
		strcpy(targetdata->AbsMaxUpDownIns,sourcedata->AbsMaxUpDownIns);
		targetdata->AbsMaxUpDown = sourcedata->AbsMaxUpDown;
		targetdata->Status = sourcedata->Status;
		targetdata->StatusChangeReason = sourcedata->StatusChangeReason;

		//板块资金流入流出
		targetdata->MaxSedimentaryIO_Section = sourcedata->MaxSedimentaryIO_Section;
		strcpy(targetdata->MaxSedIO_SectionName,sourcedata->MaxSedIO_SectionName);
		targetdata->MinSedimentaryIO_Section = sourcedata->MinSedimentaryIO_Section;
		strcpy(targetdata->MinSedIO_SectionName,sourcedata->MinSedIO_SectionName);

		//板块资金流入流出比
		targetdata->MaxSedimentaryIORate_Section = sourcedata->MaxSedimentaryIORate_Section;
		strcpy(targetdata->MaxSedIORate_SectionName,sourcedata->MaxSedIORate_SectionName);
		targetdata->MinSedimentaryIORate_Section = sourcedata->MinSedimentaryIORate_Section;
		strcpy(targetdata->MinSedIORate_SectionName,sourcedata->MinSedIORate_SectionName);
	}


}

void DataWrapper::OutputGlobalViewData(GlobalViewData *view)
{
	std::unique_lock<std::mutex>lck(m_gviewmutex);
	WriteGlobalViewData(&m_globalview,view);
}
void DataWrapper::InputGlobalViewData(GlobalViewData *view)
{
	std::unique_lock<std::mutex>lck(m_gviewmutex);
	WriteGlobalViewData(view,&m_globalview);
}

