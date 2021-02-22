/*
 * mtdspi.cpp
 *
 *  Created on: 2020年3月2日
 *      Author: qc
 */
#include "mytdspi.hpp"

 MyTDSpi::MyTDSpi(DataWrapper * pdatawrapper)
 {
	 m_datawrapper=pdatawrapper;

 }
 MyTDSpi::~MyTDSpi()
 {

 }
void MyTDSpi::SetFlagFrontDisConnect(bool flag)
{
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int nreason;
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_disconnect);/*判断上锁*/

	//		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_FrontDisConnectFalg=flag;

			pthread_mutex_unlock(&this->_mutex_disconnect); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagFrontDisConnect  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagFrontDisConnect  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}
bool MyTDSpi::GetFlagFrontDisConnect()
{
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_disconnect);/*判断上锁*/
 //		printf("MyTDSpi: GetFlagFrontDisConnect  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			flag=this->_FrontDisConnectFalg;

			pthread_mutex_unlock(&this->_mutex_disconnect); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagFrontDisConnect  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagFrontDisConnect  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

}
void MyTDSpi::SetFlagFrontConnectFalg(bool flag)
{
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_connect);/*判断上锁*/
		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: SetFlagFrontConnectFalg  锁状态 %d  --->锁定 \n",ret);
			this->_FrontConnectFalg=flag;
			pthread_mutex_unlock(&this->_mutex_connect); /*  解锁 */
			flag_write=true;
			//		printf("MyTDSpi: SetFlagFrontConnectFalg  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagFrontConnectFalg  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagFrontConnectFalg  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}
bool MyTDSpi::GetFlagFrontConnectFalg()
{
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_connect);/*判断上锁*/

		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: GetFlagFrontConnectFalg  锁状态 %d  --->锁定 \n",ret);
			flag=this->_FrontConnectFalg;

			pthread_mutex_unlock(&this->_mutex_connect); /*  解锁 */
			flag_read=true;
			//		printf("MyTDSpi: GetFlagFrontConnectFalg  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagFrontConnectFalg  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagFrontConnectFalg  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

}

void MyTDSpi::SetFlagOrderInsert(bool flag)
{

	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_orderinsert);/*判断上锁*/
		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: SetFlagOrderInsert  锁状态 %d  --->锁定 \n",ret);
			this->_OrderInsertFlag=flag;
			pthread_mutex_unlock(&this->_mutex_orderinsert); /*  解锁 */
			flag_write=true;
			//		printf("MyTDSpi: SetFlagOrderInsert  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagOrderInsert  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagOrderInsert  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}
}
bool MyTDSpi::GetFlagOrderInsert()
{

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_orderinsert);/*判断上锁*/

		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: GetFlagOrderInsert  锁状态 %d  --->锁定 \n",ret);
			flag=this->_OrderInsertFlag;

			pthread_mutex_unlock(&this->_mutex_orderinsert); /*  解锁 */
			flag_read=true;
			//		printf("MyTDSpi: GetFlagOrderInsert  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagOrderInsert  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagOrderInsert  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

}


void MyTDSpi::SetFlagCloseOrderInsert(bool flag)
{

	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_close_orderinsert);/*判断上锁*/
		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: SetFlagCloseOrderInsert  锁状态 %d  --->锁定 \n",ret);
			this->_CloseOrderInsertFlag=flag;
			pthread_mutex_unlock(&this->_mutex_close_orderinsert); /*  解锁 */
			flag_write=true;
			//		printf("MyTDSpi: SetFlagCloseOrderInsert  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagCloseOrderInsert  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagCloseOrderInsert  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}
}
bool MyTDSpi::GetFlagCloseOrderInsert()
{

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_close_orderinsert);/*判断上锁*/

		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: GetFlagOrderInsert  锁状态 %d  --->锁定 \n",ret);
			flag=this->_CloseOrderInsertFlag;

			pthread_mutex_unlock(&this->_mutex_close_orderinsert); /*  解锁 */
			flag_read=true;
			//		printf("MyTDSpi: GetFlagOrderInsert  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagCloseOrderInsert  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagCloseOrderInsert  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

}
void MyTDSpi::SetFlagAuthenticate(bool flag)
 {
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_authenticate);/*判断上锁*/
		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: SetFlagAuthenticate  锁状态 %d  --->锁定 \n",ret);
			this->_flag_authenticate=flag;
			pthread_mutex_unlock(&this->_mutex_authenticate); /*  解锁 */
			flag_write=true;
			//		printf("MyTDSpi: SetFlagAuthenticate  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagAuthenticate  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagAuthenticate  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

 }

 bool MyTDSpi::GetFlagAuthenticate()
 {
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_authenticate);/*判断上锁*/

		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: GetFlagAuthenticate  锁状态 %d  --->锁定 \n",ret);
			flag=this->_flag_authenticate;

			pthread_mutex_unlock(&this->_mutex_authenticate); /*  解锁 */
			flag_read=true;
			//		printf("MyTDSpi: GetFlagAuthenticate  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagAuthenticate  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagAuthenticate  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

 }
 void MyTDSpi::SetFlagLogin(bool flag)
 {
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_login);/*判断上锁*/
		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: SetFlagLogin  锁状态 %d  --->锁定 \n",ret);
			this->_flag_login=flag;
			pthread_mutex_unlock(&this->_mutex_login); /*  解锁 */
			flag_write=true;
			//		printf("MyTDSpi: SetFlagLogin  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagLogin  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagLogin  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

 }

 bool MyTDSpi::GetFlagLogin()
 {
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_login);/*判断上锁*/

		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: GetFlagLogin  锁状态 %d  --->锁定 \n",ret);
			flag=this->_flag_login;

			pthread_mutex_unlock(&this->_mutex_login); /*  解锁 */
			flag_read=true;
			//		printf("MyTDSpi: GetFlagLogin  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagLogin  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagLogin  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

 }
 bool MyTDSpi::GetLoginStatus(){
	 return this->m_loginstatus;
 }
 void MyTDSpi::SetFlagLogout(bool flag)
 {
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_logout);/*判断上锁*/
		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: SetFlagLogout  锁状态 %d  --->锁定 \n",ret);
			this->_flag_logout=flag;
			pthread_mutex_unlock(&this->_mutex_logout); /*  解锁 */
			flag_write=true;
			//		printf("MyTDSpi: SetFlagLogout  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetFlagLogout  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetFlagLogout  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

 }

 bool MyTDSpi::GetFlagLogout()
 {
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_logout);/*判断上锁*/

		if(ret!=EBUSY)	{
			//		printf("MyTDSpi: GetFlagLogout  锁状态 %d  --->锁定 \n",ret);
			flag=this->_flag_logout;

			pthread_mutex_unlock(&this->_mutex_logout); /*  解锁 */
			flag_read=true;
			//		printf("MyTDSpi: GetFlagLogout  锁状态 %d  --->解锁 \n",ret);
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetFlagLogout  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetFlagLogout  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

		}
	}

	return flag;

 }

  void MyTDSpi::SetConnectErrorCode(int code)
   {


  	//	printf("MyTDSpi: SetConnectErrorCode  ----->begin!!!  \n");

  		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
  		int nreason;
  		int ret;

  		bool flag_write=false;
  		while(!flag_write){

  			ret=pthread_mutex_trylock(&this->_mutex_nreason);/*判断上锁*/

  	// 		printf("MyTDSpi: SetConnectErrorCode  锁状态 %d   \n",ret);
  			if(ret!=EBUSY)	{

  				this->_nReason=code;

  				pthread_mutex_unlock(&this->_mutex_nreason); /*  解锁 */
  				flag_write=true;
  			}
  			else{
  				DateUtil dateutil;
  				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
  				printf("MyTDSpi: SetConnectErrorCode  ----->未拿到锁  %s  \n",str_localupdate.c_str());
  				char logbuf[1024];
  				sprintf(logbuf,"MyTDSpi: SetConnectErrorCode  ------->未拿到锁  %s  \n",str_localupdate.c_str());
  				sleep(1);
  			}
  		}
  	//	printf("CMdSpi: SetConnectErrorCode  ----->begin!!!  \n");

   }

   int MyTDSpi::GetConnectErrorCode()
   {

	   DateUtil dateutil;
  	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int nreason;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_nreason);/*判断上锁*/

		if(ret!=EBUSY)	{
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
//			printf("MyTDSpi: GetConnectErrorCode  ----->锁定  %s  \n",str_localupdate.c_str());
			nreason=this->_nReason;

			pthread_mutex_unlock(&_mutex_nreason); /*  解锁 */
			flag_read=true;
//			printf("MyTDSpi: GetConnectErrorCode  ----->解锁  %s  \n",str_localupdate.c_str());
		}
		else{

			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetConnectErrorCode  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetConnectErrorCode  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

	 return nreason;

   }

   void MyTDSpi::SetConnectCount(int count)
    {
   //	printf("MyTDSpi: SetConnectCount  ----->begin!!!  \n");

   	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

   	int ret;

   	bool flag_write=false;
   	while(!flag_write){

   		ret=pthread_mutex_trylock(&this->_mutex_connectcount);/*判断上锁*/

   // 		printf("MyTDSpi: SetConnectCount  锁状态 %d   \n",ret);
   		if(ret!=EBUSY)	{

   			this->_ConnectCount=count;

   			pthread_mutex_unlock(&this->_mutex_connectcount); /*  解锁 */
   			flag_write=true;
   		}
   		else{
   			DateUtil dateutil;
   			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
   			printf("MyTDSpi: SetConnectCount  ----->未拿到锁  %s  \n",str_localupdate.c_str());
   			char logbuf[1024];
   			sprintf(logbuf,"MyTDSpi: SetConnectCount  ------->未拿到锁  %s  \n",str_localupdate.c_str());
   			sleep(1);
   		}
   	}
   //	printf("CMdSpi: SetConnectCount  ----->begin!!!  \n");

    }

    int MyTDSpi::GetConnectCount()
    {

   	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
   	int count;
   	int ret;

   	bool flag_read=false;
   	while(!flag_read){

   		ret=pthread_mutex_trylock(&this->_mutex_connectcount);/*判断上锁*/

    //		printf("MyTDSpi: GetConnectCount  锁状态 %d   \n",ret);
   		if(ret!=EBUSY)	{
   			count=this->_ConnectCount;

   			pthread_mutex_unlock(&_mutex_connectcount); /*  解锁 */
   			flag_read=true;
   		}
   		else{
   			DateUtil dateutil;
   			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
   			printf("MyTDSpi: GetConnectCount  ----->未拿到锁  %s  \n",str_localupdate.c_str());
   			char logbuf[1024];
   			sprintf(logbuf,"MyTDSpi: GetConnectCount  ------->未拿到锁  %s  \n",str_localupdate.c_str());
   			sleep(1);
   		}
   	}

   	 return count;
    }

int  MyTDSpi::TryLockMutex()
{

	int ret;
//	ret=pthread_mutex_trylock(&mutex);/*判断上锁*/

	return ret;
}
int  MyTDSpi::UnLockMutex()
{

	int ret;
//	ret=pthread_mutex_unlock(&mutex);/*解锁*/
//	printf("MyTDSpi::UnLockMutex:   解锁  \n");
	return ret;
}

bool MyTDSpi::EmptyInvestPositionMap()
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁
		m_investpositionmap.clear();
		ret=true;
	} catch(std::logic_error&) {
		std::cout << "[exception caught MyTDSpi::EmptyInvestPositionMap >>> ]\n";
		ret=false;
	}
	return ret;
}
bool MyTDSpi::InsertInvestPositionMap(InvestPositionBuffer &data)
{
	 bool ret=false;
	try {
			std::unique_lock<std::mutex>lck(m_investpositionmutex);  // 函数结束时，自动析构解锁

			std::shared_ptr<InvestPositionBuffer> info = std::make_shared<InvestPositionBuffer>();
			info->symbol=data.symbol;
			info->longposition=data.longposition;
			info->longydposition=data.longydposition;
			info->longtodayposition=data.longtodayposition;
			info->shortposition=data.shortposition;
			info->shortydposition=data.shortydposition;
			info->shorttodayposition=data.shorttodayposition;

			std::string key=info->symbol;

			if (m_investpositionmap.find(key) != m_investpositionmap.end()) {
				m_investpositionmap[key] = info;
			}
			else	{
				m_investpositionmap.insert(std::pair<std::string, std::shared_ptr<InvestPositionBuffer>>(key, info));

			}
			ret=true;

		} catch(std::logic_error&) {
			std::cout << "[exception caught MyTDSpi::InsertInvestPositionMap >>> ]\n";
			ret=false;

		}
	return ret;
}
std::map<std::string,std::shared_ptr<InvestPositionBuffer>> * MyTDSpi::GetInvestPositionMap()
{
	return &m_investpositionmap;
}

bool MyTDSpi::InsertOpenOrderMap(OrderData &data)
{
	bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁
		std::string key =data.InstrumentID;
//		m_openordermap.insert(map<string,OrderData>::value_type(key,data));
		ret=true;

	} catch(std::logic_error&) {
		std::cout << "[exception caught MyTDSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}

bool MyTDSpi::UpdateOpenOrderMap(OrderData &data)
{

	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_openordermutex);  // 函数结束时，自动析构解锁

		std::string key =data.InstrumentID;
//		std::multimap<std::string,OrderData>

//		pair<multimap<string,OrderData>::iterator,multimap<string,OrderData>::iterator> mmop() ;
//		mmop= m_openordermap.equal_range(key);
//
//		 for (std::multimap<string,OrderData>::iterator it=mmop->begin(); it!=mmop.second; ++it)
//		      std::cout << ' ' << it->second;


		if(m_openordermap.find(key)!=m_openordermap.end()){
//			m_openordermap[key];
			ret=true;
		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught MyTDSpi::InsertOpenOrderMap >>> ]\n";
		ret=false;

	}
	return ret;
}
bool MyTDSpi::ExistinOrderMap(const char* instrumentid)
{

	 bool ret=false;

	 for (map<string,std::shared_ptr<OrderData>>::iterator it=m_openordermap.begin(); it!=m_openordermap.end(); ++it){
		 if(strcmp(it->second->InstrumentID,instrumentid)==0){

			ret=true;
		}
	 }


	return ret;
}

 std::string MyTDSpi::GetTest()
 {
	 return this->test;
 }

 void MyTDSpi::SetStrategyName(const char* strategyname)
 {
 	this->_strategyName=strategyname;
 }

 std::string MyTDSpi::GetStrategyName()
 {
 	return this->_strategyName;
 }


 CThostFtdcReqUserLoginField *MyTDSpi::GetPointUserLoginField()
 {
 	return &this->_userLoginField;
 }
 void MyTDSpi::SetTradingDay(const char * day)
 {
	//	printf("MyTDSpi: SetTradingDay  ----->begin!!!  \n");

	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	int nreason;
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_tradingday);/*判断上锁*/

// 		printf("MyTDSpi: SetTradingDay  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_tradingday=day;

			pthread_mutex_unlock(&this->_mutex_tradingday); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: SetTradingDay  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: SetTradingDay  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}
	//	printf("CMdSpi: SetTradingDay  ----->End!!!  \n");

 }
 std::string MyTDSpi::GetTradingDay()
  {

 	 DateUtil dateutil;
	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	std::string  str_tradingday;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_tradingday);/*判断上锁*/

		if(ret!=EBUSY)	{
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
//			printf("MyTDSpi: GetTradingDay  ----->锁定  %s  \n",str_localupdate.c_str());
			str_tradingday=this->_tradingday;

			pthread_mutex_unlock(&_mutex_tradingday); /*  解锁 */
			flag_read=true;
//			printf("MyTDSpi: GetTradingDay  ----->解锁  %s  \n",str_localupdate.c_str());
		}
		else{

			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MyTDSpi: GetTradingDay  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MyTDSpi: GetTradingDay  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

	 return str_tradingday;

  }
 std::string MyTDSpi::GetTradingDay(const char * userid)
 {
	 MysqlTradingDay mysqltradingday(userid);
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string str=tradingdaydata.TradingDay;
	return str;
 }

 // 未完成  待修改
 void MyTDSpi::UpdateMysqlTradingDay(const char* userid,const char*tradingday)
 {
	 MysqlTradingDay mysqltradingday(userid);
	mysqltradingday.CreateTable();

	std::string tradingdate;
	tradingdate=tradingday;
	char timebuf[20];
	sprintf(timebuf,"%s-%s-%s 00:00:00",
	tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),tradingdate.substr(6,2).c_str());

	if(mysqltradingday.Exist_DateTime(timebuf))
	{
		TradingDayData tradingdaydata;
		tradingdaydata= mysqltradingday.Find_LastTradingDay();
	}
	else
	{


	}

 }

 // 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
 //*

void MyTDSpi::OnFrontConnected()
{
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	char logbuf[1024];
	printf("MyTDSpi: OnFrontConnected  begin \n");
//	static int connectnumber = 0;
	DateUtil dateutil;
	double Q_BarTime_2 = dateutil.GetCurrentBarTime();
	bool Break_afternoon = (Q_BarTime_2>=0.1600 && Q_BarTime_2<0.2000);
	bool opentime= dateutil.CheckMDOpenTime();
	bool Timemore=( Break_afternoon );

//	printf("SPI: OnFrontConnected():被执行...   i---->[%d]    %s \n",i,str_localupdate.c_str());


//
	logutil.WriteLog("SPI: OnFrontConnected():被执行...");

	// 在登出后系统会重新调用OnFrontConnected，这里简单判断并忽略第1次之后的所有调用。

	int count = this->GetConnectCount();

	if (count==0) {
		 printf("MyTDSpi::OnFrontConnected 程序启动时第一次登录  sem_post()   %s \n",
				 dateutil.GetDateTimeString().c_str());
		 sprintf(logbuf,"MyTDSpi::OnFrontConnected 程序启动时第一次登录  sem_post()   %s ",
						 dateutil.GetDateTimeString().c_str());
		 logutil.WriteLog(logbuf);
		 SetConnectErrorCode(0);
		 this->PostSemInit();
	}
	else{

		 SetConnectErrorCode(0);

		 printf("MyTDSpi::OnFrontConnected 第[%d]次登录  sem_post() -----> 无须释放信号量  \n",count+1);
		 if(opentime){
			printf("MyTDSpi::OnFrontConnected 盘中----> %s  重新连接\n",dateutil.GetDateTimeString().c_str());
		}
		else {
			if (Timemore){
				printf("MyTDSpi::OnFrontConnected 非开盘时间----> %s  延迟300秒后 重新连接\n",dateutil.GetDateTimeString().c_str());
				sleep(300);
			}
		}
	}
	count++;
	this->SetConnectCount(count);
	this->SetFlagFrontConnectFalg(true);
	this->SetFlagFrontDisConnect(false);

	printf("MyTDSpi:OnFrontConnected----> End!!!   [%d] \n",count);
}
	//*/

    // 当客户端与交易托管系统通信连接断开时，该方法被调用
    //*
	void MyTDSpi::OnFrontDisconnected(int nReason)
    {
      	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
		///@param nReason 错误原因
		//		0x1001 网络读失败 => 4097
		//		0x1002 网络写失败
		//		0x2001 接收心跳超时
		//		0x2002 发送心跳失败
		//		0x2003 收到错误报文

		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		char logbuf[1024];
		DateUtil dateutil;

		this->SetFlagFrontConnectFalg(false);
		this->SetFlagFrontDisConnect(true);
		this->SetFlagLogin(false);
		this->SetFlagAuthenticate(false);
		std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
		printf("SPI: OnFrontDisconnected----> %d    %s\n",nReason,str_localupdate.c_str());
		SetConnectErrorCode(nReason);

		int errorcode = this->GetConnectErrorCode();
		switch(errorcode)		{
			case 0x1001:
				printf("SPI: 网络读失败 =>  %d  \n",errorcode);
				sprintf(logbuf,"SPI: 网络读失败 =>  %d  \n",errorcode);
				logutil.WriteLog(logbuf);
				sleep(5);
				printf("SPI: 网络读失败 =>   延迟5秒防止系统崩溃!  \n");

				break;
			case 0x1002:
				printf("SPI: 网络写失败 =>  %d  \n",errorcode);
				sprintf(logbuf,"SPI: 网络写失败 =>  %d  \n",errorcode);
				logutil.WriteLog(logbuf);
				break;
			case 0x2001:
				printf("SPI: 接收心跳超时 =>  %d  \n",errorcode);
				sprintf(logbuf,"SPI: 接收心跳超时 =>  %d  \n",errorcode);
				logutil.WriteLog(logbuf);
				break;
			case 0x2002:
				printf("SPI: 发送心跳失败 =>  %d  \n",errorcode);
				sprintf(logbuf,"SPI: 发送心跳失败 =>  %d  \n",errorcode);
				logutil.WriteLog(logbuf);
				break;
			case 0x2003:
				printf("SPI: 发送心跳失败 =>  %d  \n",errorcode);
				sprintf(logbuf,"SPI: 发送心跳失败 =>  %d  \n",errorcode);
				logutil.WriteLog(logbuf);
				break;
			default:
				break;
		}


    }

	///客户端认证响应
	void MyTDSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField,
			CThostFtdcRspInfoField *pRspInfo,
			int nRequestID,
			bool bIsLast)
	{

		char buf[1050];
		printf("OnRspAuthenticate():被执行...\n");
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		logutil.WriteLog("OnRspAuthenticate():被执行...");

		// 进程返回结果检查
		if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
		  // typedef int TThostFtdcErrorIDType;
		  // typedef char TThostFtdcErrorMsgType[81];
		  char ErrorMsg[243];
		  gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
		  printf("OnRspAuthenticate():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

		  sprintf(buf,"OnRspAuthenticate():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
		  logutil.WriteLog(buf);

		  this->SetFlagAuthenticate(false);

		  if(pRspInfo->ErrorID==63){

			  sprintf(buf,"退出程序>>>>>>>OnRspAuthenticate():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
			 		  logutil.WriteLog(buf);
			//
			exit(-1);
			}
			else{

			}
		}
		else{
			this->SetFlagAuthenticate(true);
		}

		// 如果有返回结果读取返回信息
		if ( pRspAuthenticateField != NULL ) {

			// 读取返回信息,并做编码转化
			///经纪公司代码 TThostFtdcBrokerIDType char[11]
			char BrokerID[33];
			gbk2utf8(pRspAuthenticateField->BrokerID,BrokerID,sizeof(BrokerID));
			printf("OnRspAuthenticate():后台响应  BrokerID=%s  \n",BrokerID);
			//投资者代码 TThostFtdcInvestorIDType char[13]
			char UserID[39];
			gbk2utf8(pRspAuthenticateField->UserID,UserID,sizeof(UserID));
			printf("OnRspAuthenticate():后台响应  UserID=%s  \n",UserID);

			char UserProductInfo[1024];
			gbk2utf8(pRspAuthenticateField->UserProductInfo,UserProductInfo,sizeof(UserProductInfo));
			printf("OnRspAuthenticate():后台响应  UserProductInfo=%s  \n",UserProductInfo);
			sprintf(buf,"OnRspAuthenticate():后台响应  UserProductInfo=%s \n",UserProductInfo);
			logutil.WriteLog(buf);

			// 以下代码切换回模拟盘 需要注释///////////////////////////////////////////////////
			char AppID[1024];
			gbk2utf8(pRspAuthenticateField->AppID,AppID,sizeof(AppID));
			printf("OnRspAuthenticate():后台响应  AppID=%s  \n",AppID);

			//char AppType[1024];
			//gbk2utf8(pRspAuthenticateField->AppType,AppID,sizeof(AppID));
			printf("OnRspAuthenticate():后台响应  AppType=%d  \n",pRspAuthenticateField->AppType-'0');
			// 以上代码切换回模拟盘 需要注释///////////////////////////////////////////////////

		}

		// 如果响应函数已经返回最后一条信息
		if(bIsLast) {
		  // 通知主过程，响应函数将结束

		 this->PostSemAuthenticate();
		  logutil.WriteLog("OnRspAuthenticate():被执行完成...");
		}

	}

	//*/

    // 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
    //*
	void MyTDSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
		char buf[1024];
		printf("MyTDSpi:OnRspUserLogin():被执行...\n");
		strcpy(this->_userLoginField.BrokerID,pRspUserLogin->BrokerID);
		strcpy(this->_userLoginField.UserID,pRspUserLogin->UserID);

		// 只有获取了用户名的情况下 才能设置日志文件名
//		this->SetLogPath();
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		logutil.WriteLog("MyTDSpi:OnRspUserLogin():------>Begin!!!\n");

        if (pRspInfo->ErrorID == 0) {
        	bool flag_connect =  this->GetFlagFrontConnectFalg() ;
        	bool flag_disconnect = this->GetFlagFrontDisConnect();

            if ( pRspUserLogin != NULL  && flag_connect && !flag_disconnect) {

				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();


				int DifSec=dateutil.CollectDifSecBySHFETime(pRspUserLogin->SHFETime);
				sprintf(buf,"Debug: SHFETime 时间差= %d  \n",DifSec);  //
				logutil.WriteLog(buf);


				TradingDayData tradingdayinfo;

				tradingdayinfo.DifSeconds =DifSec;
				strcpy(tradingdayinfo.TradingDay,pRspUserLogin->TradingDay);
				strcpy(tradingdayinfo.MaxOrderRef,pRspUserLogin->MaxOrderRef);
				strcpy(tradingdayinfo.DCETime,pRspUserLogin->DCETime);
				strcpy(tradingdayinfo.CZCETime,pRspUserLogin->CZCETime);
				strcpy(tradingdayinfo.SHFETime,pRspUserLogin->SHFETime);
				strcpy(tradingdayinfo.INETime,pRspUserLogin->INETime);
				strcpy(tradingdayinfo.LoginTime,pRspUserLogin->LoginTime);
				tradingdayinfo.FrontID = pRspUserLogin->FrontID;
				tradingdayinfo.SessionID = pRspUserLogin->SessionID;
				strcpy(tradingdayinfo.LocalCreateTime,str_localupdate.c_str());


				 sprintf(buf,"MyTDSpi:登录成功-----> 本地时间: %s \n",str_localupdate.c_str());
	//			char FrontID[39];
	//			gbk2utf8(pRspUserLogin->FrontID ,FrontID,sizeof(FrontID));
	//			printf("Debug: BrokerID= %s  \n",pRspUserLogin->BrokerID);  //

				sprintf(buf,"Debug: FrontID= %d  \n",pRspUserLogin->FrontID);  //前置编号
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: SessionID= %d  \n",pRspUserLogin->SessionID);  //会话编号
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: MaxOrderRef= %s  \n",pRspUserLogin->MaxOrderRef);  //最大报单编号
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: DCETime= %s  \n",pRspUserLogin->DCETime);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: CZCETime= %s  \n",pRspUserLogin->CZCETime);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: SHFETime= %s  \n",pRspUserLogin->SHFETime);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: INETime= %s  \n",pRspUserLogin->INETime);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: LoginTime= %s  \n",pRspUserLogin->LoginTime);  //
				logutil.WriteLog(buf);

				sprintf(buf,"Debug: BrokerID= %s  ",pRspUserLogin->BrokerID);
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: LoginTime= %s  ",pRspUserLogin->LoginTime);
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: SystemName= %s  \n",pRspUserLogin->SystemName);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: TradingDay= %s  \n",pRspUserLogin->TradingDay);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: UserID= %s  \n",pRspUserLogin->UserID);  //
				logutil.WriteLog(buf);

				// 设置私有变量本地交易日
				this->SetTradingDay(pRspUserLogin->TradingDay);

				MysqlTradingDay mysqltradingday(this->_userLoginField.UserID);
				if(!mysqltradingday.Exist_Table()){
					mysqltradingday.CreateTable();
					printf("SPI:OnRspUserLogin:  创建数据表  MysqlTradingDay \n");
				}

				std::string tradingdate;
				tradingdate=tradingdayinfo.TradingDay;
				strcpy(tradingdayinfo.TradingDateTime,dateutil.ConvertDatetoSqlTime(pRspUserLogin->TradingDay).c_str());
				tradingdayinfo.SettleInfoConfirmStatus = false;
				tradingdayinfo.DataReadyStatus = false;

				if(!mysqltradingday.Exist_DateTime(tradingdayinfo.TradingDateTime) )	{
					printf("SPI:OnRspUserLogin():    插入新的交易日  %s\n",tradingdayinfo.TradingDateTime);
					sprintf(buf,"SPI:OnRspUserLogin():    插入新的交易日  %s\n",tradingdayinfo.TradingDateTime);
					logutil.WriteLog(buf);
					mysqltradingday.Insert(tradingdayinfo);
				}
				else	{
					TradingDayData tradingdaydata;
					tradingdaydata= mysqltradingday.Find_LastTradingDay();


					if (tradingdaydata.DifSeconds != DifSec )	{
						printf("SPI:OnRspUserLogin():    更新交易所时间差  %d 秒\n",DifSec);
						sprintf(buf,"SPI:OnRspUserLogin():    更新交易所时间差  %d 秒\n",DifSec);
						logutil.WriteLog(buf);
						mysqltradingday.UpdateDifSecbyDate(DifSec,tradingdaydata.TradingDay);
					}

					if(tradingdaydata.DataReadyStatus){
						tradingdaydata.DataReadyStatus = false;
						printf("SPI:OnRspUserLogin():   DataReadyStatus标志位重置!!! ----> false \n");
						sprintf(buf,"SPI:OnRspUserLogin():    DataReadyStatus标志位重置!!! ----> false ");
						logutil.WriteLog(buf);
						mysqltradingday.UpdateDataReadyStatusbyDate(&tradingdaydata);
					}
				}

				this->SetFlagLogin(true);
				this->SetFlagLogout(false);

				m_loginstatus=true;

			}
			else       {
				if(pRspUserLogin == NULL){
					printf("MyTDSpi:OnRspUserLogin:-------->登录成功! 数据返回空值！！！！ \n");
					sprintf(buf,"MyTDSpi:OnRspUserLogin:-------->登录成功! 数据返回空值！！！！ ");
					logutil.WriteLog(buf);
				}


				 if(flag_connect){
					 printf("MyTDSpi:OnRspUserLogin:-------->未初始化连接!!! \n");
					 sprintf(buf,"MyTDSpi:OnRspUserLogin:-------->未初始化连接!!! ");
					 logutil.WriteLog(buf);
				 }

				 if(flag_disconnect){
					 printf("MyTDSpi:OnRspUserLogin:-------->初始化连接断开!!! \n");
					 sprintf(buf,"MyTDSpi:OnRspUserLogin:-------->初始化连接断开!!! ");
					 logutil.WriteLog(buf);
				 }
			}

        } else {
        	 // 客户端登录失败，客户端需进行错误处理
        	char Message[93];
        	gbk2utf8(pRspInfo->ErrorMsg,Message,sizeof(Message));

            printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,Message);
            printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
            printf("Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d",
            pRspInfo->ErrorID, Message, nRequestID, bIsLast);


            sprintf(buf,"ErrorCode=[%d], ErrorMsg=[%s]", pRspInfo->ErrorID,Message);
            logutil.WriteLog(buf);
			sprintf(buf,"RequestID=[%d], Chain=[%d]", nRequestID, bIsLast);
			logutil.WriteLog(buf);

            sprintf(buf,"Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d",
                        pRspInfo->ErrorID, Message, nRequestID, bIsLast);
            logutil.WriteLog(buf);

            if(pRspInfo->ErrorID==3){
            	//不合法的登录 密码错误 退出，防止密码锁定
            	exit(-1);
            }
            else{

            }


        }


        // 如果响应函数已经返回最后一条信息
       		if(bIsLast) {
       			// 通知主过程，响应函数将结束
       			sprintf(buf,"MyTDSpi -----OnRspUserLogin----->释放信号semlogin:  %s", pRspUserLogin->TradingDay);
       			logutil.PrintLog(buf);
				this->PostSemLogin();
       		}

       		logutil.WriteLog("MyTDSpi:OnRspUserLogin():被执行------>END!!! \n");

    }




  // 登出结果响应
    void MyTDSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                                 CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
        printf("OnReqUserLogout():被执行...\n");

		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		logutil.WriteLog("OnReqUserLogout():被执行...");

        if (pRspInfo->ErrorID == 0) {
            printf("OnReqUserLogout---->登出成功!\n");
//            flag_logout=true;
            this->SetFlagLogout(true);
//            flag_login=false;
            this->SetFlagLogin(false);

            logutil.WriteLog("OnReqUserLogout():登出成功...");
        } else {
            printf("OnReqUserLogout---->登出失败!\n");
        }

        // 如果响应函数已经返回最后一条信息
		if(bIsLast) {
		  this->PostSemLogout();
		}
    }

    //*/
    //    请求查询投资者响应
    //*/

        void MyTDSpi::OnRspQryInvestor(
            CThostFtdcInvestorField * pInvestor,
            CThostFtdcRspInfoField * pRspInfo,
            int nRequestID,
            bool bIsLast
        ) {
            printf("OnRspQryInvestor():被执行...\n");
            char buf[1024];
			  LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
			  logutil.WriteLog("OnRspQryInvestor():被执行...");


            // 进程返回结果检查
            if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
                // typedef int TThostFtdcErrorIDType;
                // typedef char TThostFtdcErrorMsgType[81];
                char ErrorMsg[243];
                gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
                printf("OnRspQryInvestor():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
            }

            // 如果有返回结果读取返回信息
            if ( pInvestor != NULL && this->GetFlagLogin()) {
//                printf("pInvestor != NULL\n");
    			// 读取返回信息,并做编码转化
                ///投资者代码 TThostFtdcInvestorIDType char[13]
                char InvestorID[39];
                gbk2utf8(pInvestor->InvestorID,InvestorID,sizeof(InvestorID));
                ///经纪公司代码 TThostFtdcBrokerIDType char[11]
                char BrokerID[33];
                gbk2utf8(pInvestor->BrokerID,BrokerID,sizeof(BrokerID));
                ///投资者分组代码 TThostFtdcInvestorIDType char[13]
                char InvestorGroupID[39];
                gbk2utf8(pInvestor->InvestorGroupID,InvestorGroupID,sizeof(InvestorGroupID));
                ///投资者名称 TThostFtdcPartyNameType char[81]
                char InvestorName[243];
                gbk2utf8(pInvestor->InvestorName,InvestorName,sizeof(InvestorName));
                ///证件类型 TThostFtdcIdCardTypeType char
                //// THOST_FTDC_ICT_EID '0' 组织机构代码
                //// THOST_FTDC_ICT_IDCard '1' 中国公民身份证
                //// THOST_FTDC_ICT_OfficerIDCard '2' 军官证
                //// THOST_FTDC_ICT_PoliceIDCard '3' 警官证
                //// THOST_FTDC_ICT_SoldierIDCard '4' 士兵证
                //// THOST_FTDC_ICT_HouseholdRegister '5' 户口簿
                //// THOST_FTDC_ICT_Passport '6' 护照
                //// THOST_FTDC_ICT_TaiwanCompatriotIDCard '7' 台胞证
                //// THOST_FTDC_ICT_HomeComingCard '8' 回乡证
                //// THOST_FTDC_ICT_LicenseNo '9' 营业执照号
                //// THOST_FTDC_ICT_TaxNo 'A' 税务登记号/当地纳税ID
                //// THOST_FTDC_ICT_HMMainlandTravelPermit 'B' 港澳居民来往内地通行证
                //// THOST_FTDC_ICT_TwMainlandTravelPermit 'C' 台湾居民来往大陆通行证
                //// THOST_FTDC_ICT_DrivingLicense 'D' 驾照
                //// THOST_FTDC_ICT_SocialID 'F' 当地社保ID
                //// THOST_FTDC_ICT_LocalID 'G' 当地身份证
                //// THOST_FTDC_ICT_BusinessRegistration 'H' 商业登记证
                //// THOST_FTDC_ICT_HKMCIDCard 'I' 港澳永久性居民身份证
                //// THOST_FTDC_ICT_OtherCard 'x' 其他证件
                char IdentifiedCardType = pInvestor->IdentifiedCardType;
                ///证件号码 TThostFtdcIdentifiedCardNoType char[51]
                char IdentifiedCardNo[153];
                gbk2utf8(pInvestor->IdentifiedCardNo,IdentifiedCardNo,sizeof(IdentifiedCardNo));
                ///是否活跃 TThostFtdcBoolType int
                int IsActive = pInvestor->IsActive;
                ///联系电话 TThostFtdcTelephoneType char[41]
                char Telephone[123];
                gbk2utf8(pInvestor->Telephone,Telephone,sizeof(Telephone));
                ///通讯地址 TThostFtdcAddressType char[101]
                char Address[303];
                gbk2utf8(pInvestor->Address,Address,sizeof(Address));
                ///开户日期 TThostFtdcDateType char[9]
                char OpenDate[27];
                gbk2utf8(pInvestor->OpenDate,OpenDate,sizeof(OpenDate));
                ///手机 TThostFtdcMobileType char[41]
                char Mobile[123];
                gbk2utf8(pInvestor->Mobile,Mobile,sizeof(Mobile));
                ///手续费率模板代码 TThostFtdcInvestorIDType char[13]
                char CommModelID[39];
                gbk2utf8(pInvestor->CommModelID,CommModelID,sizeof(CommModelID));
                ///保证金率模板代码 TThostFtdcInvestorIDType char[13]
                char MarginModelID[39];
                gbk2utf8(pInvestor->MarginModelID,MarginModelID,sizeof(MarginModelID));

    			printf("InvestorID=%s\n",InvestorID);  //投资者代码
//    			printf("BrokerID=%s\n",BrokerID);  // 经纪公司代码
//    			printf("InvestorName=%s\n",InvestorName);  // 投资者名称
//                printf("IdentifiedCardType=%c\n",IdentifiedCardType); // 证件类型
//    			printf("IdentifiedCardNo=%s\n",IdentifiedCardNo);  // 证件号码
//                printf("IsActive=%d\n",IsActive); // 是否活跃
//                printf("OpenDate=%s\n",OpenDate);  // 开户日期

            }

            // 如果响应函数已经返回最后一条信息
            if(bIsLast) {
//                 通知主过程，响应函数将结束
            	this->PostSemInvestor();

            }
        }

        ///请求查询报单响应
	   void MyTDSpi::OnRspQryOrder(
			CThostFtdcOrderField * pOrder,
			CThostFtdcRspInfoField * pRspInfo,
			int nRequestID,
			bool bIsLast
		) {
			printf("OnRspQryOrder():被执行...\n");
			char buf[1024];
			LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
			logutil.WriteLog("\n");
			  logutil.WriteLog("OnRspQryOrder():被执行...");


			// 进程返回结果检查
			if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
				// typedef int TThostFtdcErrorIDType;
				// typedef char TThostFtdcErrorMsgType[81];
				char ErrorMsg[243];
				gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
				printf("OnRspQryOrder():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
				sprintf(buf,"OnRspQryOrder():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
				logutil.WriteLog(buf);
			}

			// 如果有返回结果读取返回信息
			if ( pOrder != NULL  && this->GetFlagLogin()) {
				// 读取返回信息,并做编码转化
				///经纪公司代码 TThostFtdcBrokerIDType char[11]
				char BrokerID[33];
				gbk2utf8(pOrder->BrokerID,BrokerID,sizeof(BrokerID));
				///投资者代码 TThostFtdcInvestorIDType char[13]
				char InvestorID[39];
				gbk2utf8(pOrder->InvestorID,InvestorID,sizeof(InvestorID));
				///合约代码 TThostFtdcInstrumentIDType char[31]
				char InstrumentID[93];
				gbk2utf8(pOrder->InstrumentID,InstrumentID,sizeof(InstrumentID));
				///报单引用 TThostFtdcOrderRefType char[13]
				char OrderRef[39];
				gbk2utf8(pOrder->OrderRef,OrderRef,sizeof(OrderRef));
				///用户代码 TThostFtdcUserIDType char[16]
				char UserID[48];
				gbk2utf8(pOrder->UserID,UserID,sizeof(UserID));
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
				char OrderPriceType = pOrder->OrderPriceType;
				///买卖方向 TThostFtdcDirectionType char
				//// THOST_FTDC_D_Buy '0' 买
				//// THOST_FTDC_D_Sell '1' 卖
				char Direction = pOrder->Direction;
				///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
				char CombOffsetFlag[15];
				gbk2utf8(pOrder->CombOffsetFlag,CombOffsetFlag,sizeof(CombOffsetFlag));
				///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
				char CombHedgeFlag[15];
				gbk2utf8(pOrder->CombHedgeFlag,CombHedgeFlag,sizeof(CombHedgeFlag));
				///价格 TThostFtdcPriceType double
				double LimitPrice = pOrder->LimitPrice;
				///数量 TThostFtdcVolumeType int
				int VolumeTotalOriginal = pOrder->VolumeTotalOriginal;


				///有效期类型 TThostFtdcTimeConditionType char
				//// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
				//// THOST_FTDC_TC_GFS '2' 本节有效
				//// THOST_FTDC_TC_GFD '3' 当日有效
				//// THOST_FTDC_TC_GTD '4' 指定日期前有效
				//// THOST_FTDC_TC_GTC '5' 撤销前有效
				//// THOST_FTDC_TC_GFA '6' 集合竞价有效
				char TimeCondition = pOrder->TimeCondition;
				///GTD日期 TThostFtdcDateType char[9]
				char GTDDate[27];
				gbk2utf8(pOrder->GTDDate,GTDDate,sizeof(GTDDate));
				///成交量类型 TThostFtdcVolumeConditionType char
				//// THOST_FTDC_VC_AV '1' 任何数量
				//// THOST_FTDC_VC_MV '2' 最小数量
				//// THOST_FTDC_VC_CV '3' 全部数量
				char VolumeCondition = pOrder->VolumeCondition;
				///最小成交量 TThostFtdcVolumeType int
				int MinVolume = pOrder->MinVolume;
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
				char ContingentCondition = pOrder->ContingentCondition;
				///止损价 TThostFtdcPriceType double
				double StopPrice = pOrder->StopPrice;
				///强平原因 TThostFtdcForceCloseReasonType char
				//// THOST_FTDC_FCC_NotForceClose '0' 非强平
				//// THOST_FTDC_FCC_LackDeposit '1' 资金不足
				//// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
				//// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
				//// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
				//// THOST_FTDC_FCC_Violation '5' 违规
				//// THOST_FTDC_FCC_Other '6' 其它
				//// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
				char ForceCloseReason = pOrder->ForceCloseReason;
				///自动挂起标志 TThostFtdcBoolType int
				int IsAutoSuspend = pOrder->IsAutoSuspend;
				///业务单元 TThostFtdcBusinessUnitType char[21]
				char BusinessUnit[63];
				gbk2utf8(pOrder->BusinessUnit,BusinessUnit,sizeof(BusinessUnit));
				///请求编号 TThostFtdcRequestIDType int
				int RequestID = pOrder->RequestID;
				///本地报单编号 TThostFtdcOrderLocalIDType char[13]
				char OrderLocalID[39];
				gbk2utf8(pOrder->OrderLocalID,OrderLocalID,sizeof(OrderLocalID));
				///交易所代码 TThostFtdcExchangeIDType char[9]
				char ExchangeID[27];
				gbk2utf8(pOrder->ExchangeID,ExchangeID,sizeof(ExchangeID));
				///会员代码 TThostFtdcParticipantIDType char[11]
				char ParticipantID[33];
				gbk2utf8(pOrder->ParticipantID,ParticipantID,sizeof(ParticipantID));
				///客户代码 TThostFtdcClientIDType char[11]
				char ClientID[33];
				gbk2utf8(pOrder->ClientID,ClientID,sizeof(ClientID));
				///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
				char ExchangeInstID[93];
				gbk2utf8(pOrder->ExchangeInstID,ExchangeInstID,sizeof(ExchangeInstID));
				///交易所交易员代码 TThostFtdcTraderIDType char[21]
				char TraderID[63];
				gbk2utf8(pOrder->TraderID,TraderID,sizeof(TraderID));
				///安装编号 TThostFtdcInstallIDType int
				int InstallID = pOrder->InstallID;
				///报单提交状态 TThostFtdcOrderSubmitStatusType char
				//// THOST_FTDC_OSS_InsertSubmitted '0' 已经提交
				//// THOST_FTDC_OSS_CancelSubmitted '1' 撤单已经提交
				//// THOST_FTDC_OSS_ModifySubmitted '2' 修改已经提交
				//// THOST_FTDC_OSS_Accepted '3' 已经接受
				//// THOST_FTDC_OSS_InsertRejected '4' 报单已经被拒绝
				//// THOST_FTDC_OSS_CancelRejected '5' 撤单已经被拒绝
				//// THOST_FTDC_OSS_ModifyRejected '6' 改单已经被拒绝
				char OrderSubmitStatus = pOrder->OrderSubmitStatus;
				///报单提示序号 TThostFtdcSequenceNoType int
				int NotifySequence = pOrder->NotifySequence;
				///交易日 TThostFtdcDateType char[9]
				char TradingDay[27];
				gbk2utf8(pOrder->TradingDay,TradingDay,sizeof(TradingDay));
				///结算编号 TThostFtdcSettlementIDType int
				int SettlementID = pOrder->SettlementID;
				///报单编号 TThostFtdcOrderSysIDType char[21]
				char OrderSysID[63];
				gbk2utf8(pOrder->OrderSysID,OrderSysID,sizeof(OrderSysID));
				///报单来源 TThostFtdcOrderSourceType char
				//// THOST_FTDC_OSRC_Participant '0' 来自参与者
				//// THOST_FTDC_OSRC_Administrator '1' 来自管理员
				char OrderSource = pOrder->OrderSource;
				///报单状态 TThostFtdcOrderStatusType char
				//// THOST_FTDC_OST_AllTraded '0' 全部成交
				//// THOST_FTDC_OST_PartTradedQueueing '1' 部分成交还在队列中
				//// THOST_FTDC_OST_PartTradedNotQueueing '2' 部分成交不在队列中
				//// THOST_FTDC_OST_NoTradeQueueing '3' 未成交还在队列中
				//// THOST_FTDC_OST_NoTradeNotQueueing '4' 未成交不在队列中
				//// THOST_FTDC_OST_Canceled '5' 撤单
				//// THOST_FTDC_OST_Unknown 'a' 未知
				//// THOST_FTDC_OST_NotTouched 'b' 尚未触发
				//// THOST_FTDC_OST_Touched 'c' 已触发
				char OrderStatus = pOrder->OrderStatus;
				///报单类型 TThostFtdcOrderTypeType char
				//// THOST_FTDC_ORDT_Normal '0' 正常
				//// THOST_FTDC_ORDT_DeriveFromQuote '1' 报价衍生
				//// THOST_FTDC_ORDT_DeriveFromCombination '2' 组合衍生
				//// THOST_FTDC_ORDT_Combination '3' 组合报单
				//// THOST_FTDC_ORDT_ConditionalOrder '4' 条件单
				//// THOST_FTDC_ORDT_Swap '5' 互换单
				char OrderType = pOrder->OrderType;
				///今成交数量 TThostFtdcVolumeType int
				int VolumeTraded = pOrder->VolumeTraded;
				///剩余数量 TThostFtdcVolumeType int
				int VolumeTotal = pOrder->VolumeTotal;
				///报单日期 TThostFtdcDateType char[9]
				char InsertDate[27];
				gbk2utf8(pOrder->InsertDate,InsertDate,sizeof(InsertDate));
				///委托时间 TThostFtdcTimeType char[9]
				char InsertTime[27];
				gbk2utf8(pOrder->InsertTime,InsertTime,sizeof(InsertTime));
				///激活时间 TThostFtdcTimeType char[9]
				char ActiveTime[27];
				gbk2utf8(pOrder->ActiveTime,ActiveTime,sizeof(ActiveTime));
				///挂起时间 TThostFtdcTimeType char[9]
				char SuspendTime[27];
				gbk2utf8(pOrder->SuspendTime,SuspendTime,sizeof(SuspendTime));
				///最后修改时间 TThostFtdcTimeType char[9]
				char UpdateTime[27];
				gbk2utf8(pOrder->UpdateTime,UpdateTime,sizeof(UpdateTime));
				///撤销时间 TThostFtdcTimeType char[9]
				char CancelTime[27];
				gbk2utf8(pOrder->CancelTime,CancelTime,sizeof(CancelTime));
				///最后修改交易所交易员代码 TThostFtdcTraderIDType char[21]
				char ActiveTraderID[63];
				gbk2utf8(pOrder->ActiveTraderID,ActiveTraderID,sizeof(ActiveTraderID));
				///结算会员编号 TThostFtdcParticipantIDType char[11]
				char ClearingPartID[33];
				gbk2utf8(pOrder->ClearingPartID,ClearingPartID,sizeof(ClearingPartID));
				///序号 TThostFtdcSequenceNoType int
				int SequenceNo = pOrder->SequenceNo;
				///前置编号 TThostFtdcFrontIDType int
				int FrontID = pOrder->FrontID;
				///会话编号 TThostFtdcSessionIDType int
				int SessionID = pOrder->SessionID;
				///用户端产品信息 TThostFtdcProductInfoType char[11]
				char UserProductInfo[33];
				gbk2utf8(pOrder->UserProductInfo,UserProductInfo,sizeof(UserProductInfo));
				///状态信息 TThostFtdcErrorMsgType char[81]
				char StatusMsg[243];
				gbk2utf8(pOrder->StatusMsg,StatusMsg,sizeof(StatusMsg));
				///用户强评标志 TThostFtdcBoolType int
				int UserForceClose = pOrder->UserForceClose;
				///操作用户代码 TThostFtdcUserIDType char[16]
				char ActiveUserID[48];
				gbk2utf8(pOrder->ActiveUserID,ActiveUserID,sizeof(ActiveUserID));
				///经纪公司报单编号 TThostFtdcSequenceNoType int
				int BrokerOrderSeq = pOrder->BrokerOrderSeq;
				///相关报单 TThostFtdcOrderSysIDType char[21]
				char RelativeOrderSysID[63];
				gbk2utf8(pOrder->RelativeOrderSysID,RelativeOrderSysID,sizeof(RelativeOrderSysID));
				///郑商所成交数量 TThostFtdcVolumeType int
				int ZCETotalTradedVolume = pOrder->ZCETotalTradedVolume;
				///互换单标志 TThostFtdcBoolType int
				int IsSwapOrder = pOrder->IsSwapOrder;


				printf("InstrumentID=%s,",InstrumentID); // 报单引用
				printf("OrderRef=%s,",OrderRef); // 报单引用
				printf("OrderPriceType=%c,",OrderPriceType); // 报单价格条件
				printf("Direction=%c,",Direction); // 买卖方向
				printf("CombOffsetFlag=%c,",CombOffsetFlag[0]); // 买卖方向

				printf("LimitPrice=%f,",LimitPrice); // 价格
				printf("VolumeTotalOriginal=%d,",VolumeTotalOriginal); // 数量
				printf("TimeCondition=%c,",TimeCondition); // 有效期类型
				printf("OrderStatus=%c,",OrderStatus); // 报单状态
				printf("OrderSubmitStatus=%c,",OrderSubmitStatus); // 报单提交状态
				printf("TradingDay=%s,",TradingDay); //
				printf("OrderSysID=%s,",OrderSysID); // 报单提交状态
				printf("FrontID=%d,",FrontID); // 前置编号
				printf("SessionID=%d\n",SessionID); // 会话编号

//				sprintf(buf,"InstrumentID=%s",InstrumentID); //
//				logutil.WriteLog(buf);
//				sprintf(buf,"TradingDay=%s",TradingDay); //
//				logutil.WriteLog(buf);
//
//
//				sprintf(buf,"InstrumentID=%s",InstrumentID); //
//				logutil.WriteLog(buf);

				sprintf(buf,"InstrumentID=%s LimitPrice=%.3f  TradingDay=%s ",InstrumentID,LimitPrice,TradingDay); //
				logutil.WriteLog(buf);


				OrderData info;
				strcpy(info.InstrumentID,pOrder->InstrumentID);
				strcpy(info.TradingDay,pOrder->TradingDay);
				strcpy(info.OrderRef,pOrder->OrderRef);
				strcpy(info.ExchangeID,pOrder->ExchangeID);

				info.Direction = pOrder->Direction-'0';
				info.CombOffsetFlag = pOrder->CombOffsetFlag[0]-'0';
				info.CombHedgeFlag =pOrder->CombHedgeFlag[0]-'0';
				info.LimitPrice = LimitPrice;
				info.VolumeTotalOriginal =  pOrder->VolumeTotalOriginal;
				info.Volume= pOrder->VolumeTraded;
				info.OrderSubmitStatus = pOrder->OrderSubmitStatus-'0';
				info.OrderStatus = pOrder->OrderStatus-'0';
				info.FrontID=pOrder->FrontID;
				info.SessionID=pOrder->SessionID;



				// 开仓单 状态追踪
				 if(info.CombOffsetFlag==0 )	{
					 Config config(this->GetStrategyName().c_str());
					 MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
					 if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='0') { //全部成交

							 info.Status=2;

							sprintf(buf,"Debug: OnRspQryOrder 开仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						}
					 else if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='a') { //提交  未知

							 info.Status=1;

							sprintf(buf,"Debug: OnRspQryOrder 开仓单已经提交-----后台状态'未知'---%s-----",	pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						}
					 else if ( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='0') { //全部成交

							 info.Status=2;

							sprintf(buf,"Debug: OnRspQryOrder 开仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						}
					 else if( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='1') { //部分成交还在队列中

						 info.Status=1;
						sprintf(buf,"Debug: OnRspQryOrder 开仓单-----部分成交还在队列中%s-----",	pOrder->InstrumentID);  //
						logutil.WriteLog(buf);

					 }
					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='2') { //  部分成交不在队列中


							info.Status =3;
							sprintf(buf,"Debug:OnRspQryOrder 开仓部分成交不在队列中 -----%s-----",pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						 }
					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='3') { //未成交还在队列中


						 info.Status=1;

						sprintf(buf,"Debug:OnRspQryOrder 开仓单 未成交还在队列中 -----%s-----",pOrder->InstrumentID);  //
						logutil.WriteLog(buf);

						 }
					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='4') { //未成交不在队列中

							info.Status =4;
							sprintf(buf,"Debug: OnRspQryOrder 开仓单-----未成交 不在队列中%s-----",pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						 }

					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='5') { //未成交不在队列中

						info.Status =22;
						sprintf(buf,"Debug: OnRspQryOrder 开仓单-----撤单%s-----",pOrder->InstrumentID);  //
						logutil.WriteLog(buf);

					 }
					 else	 {
						 sprintf(buf,"Debug: OnRspQryOrder 开仓单 不明状态 TradingDay= %s  OrderSubmitStatus=%d  OrderStatus=%d",
						 pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
							logutil.WriteLog(buf);
					 }


					 if(!mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
							mysqlorder.NewOrderData_byRspQryOrder(info);
							sprintf(buf,"Debug: OnRspQryOrder 开仓单-----全部成交%s-----不存在orderinfo,新插入!!!",
									pOrder->InstrumentID);  //
							logutil.WriteLog(buf);
						}
						else		{
							OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay);

							 if(orderdata.Status < info.Status )	{
								mysqlorder.UpdateOrderData_byRtnOrder(info);
								sprintf(buf,"Debug: OnRspQryOrder 开仓单-----[%d]    [%d]-----更新  [%s] orderinfo!!!!!!",
										orderdata.Status,info.Status,pOrder->InstrumentID);  //
								logutil.WriteLog(buf);
							}
							 else		 {
								 sprintf(buf,"Debug: OnRspQryOrder 开仓单-----[%d]    [%d]-----状态相同 无需更新  [%s] orderinfo!!!!!!",
								 										orderdata.Status,info.Status,pOrder->InstrumentID);  //
								 								logutil.WriteLog(buf);
							 }



						}

				}



			//平仓单状态追踪
			if (info.CombOffsetFlag>=1 && info.CombOffsetFlag<=4)			{
				 Config config(this->GetStrategyName().c_str());
				 MysqlOrder mysqlorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);

					 if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='0')  {//全部成交

							 info.Status=2;

							sprintf(buf,"Debug: OnRspQryOrder 平仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						}
					 else if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='a')  {//提交  未知

							 info.Status=1;

							sprintf(buf,"Debug: OnRspQryOrder 平仓单已经提交-----后台状态'未知'---%s-----",	pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						}
					 else if ( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='0')  {//全部成交

							 info.Status=2;

							sprintf(buf,"Debug: OnRspQryOrder 平仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						}
						 else if( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='1') { //部分成交还在队列中

							 info.Status=1;
							 sprintf(buf,"Debug:OnRspQryOrder 平仓部分成交还在队列中 ----%s-----", pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						 }
					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='2')  {//  部分成交不在队列中


							info.Status =3;
							sprintf(buf,"Debug: OnRspQryOrder 平仓部分成交不在队列中 -----%s-----",pOrder->InstrumentID);  //
							logutil.WriteLog(buf);

						 }
					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='3')  {//未成交还在队列中


						 info.Status=1;

						sprintf(buf,"Debug:OnRspQryOrder 平仓单 未成交还在队列中 -----%s-----",pOrder->InstrumentID);  //
						logutil.WriteLog(buf);

						 }
					 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='4') { //未成交不在队列中

								info.Status =4;
								sprintf(buf,"Debug: OnRspQryOrder -----未成交 不在队列中%s-----",	pOrder->InstrumentID);  //
								logutil.WriteLog(buf);

						 }
					 else if(pOrder->OrderSubmitStatus =='4' && pOrder->OrderStatus =='5'){


							sprintf(buf,"Debug: OnRspQryOrder 平仓单被拒绝 撤单 TradingDay= %s  ",pOrder->TradingDay);  //
							logutil.WriteLog(buf);
							info.Status =-21;

						}

					 else if(pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='5')	{

							sprintf(buf,"Debug: OnRspQryOrder 平仓单撤单 TradingDay= %s  ",pOrder->TradingDay);  //
							logutil.WriteLog(buf);
							info.Status =22;

						}
					 else	 {
							 sprintf(buf,"Debug: OnRspQryOrder 平仓单 不明状态 TradingDay= %s  OrderSubmitStatus=%d  OrderStatus=%d",
									 pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
													logutil.WriteLog(buf);
							 info.Status=-1;
						 }

					if(!mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
							mysqlorder.NewOrderData_byRspQryOrder(info);
							sprintf(buf,"Debug: OnRspQryOrder 平仓单----全部成交%s-----不存在orderinfo,新插入!!!",pOrder->InstrumentID);  //
							logutil.WriteLog(buf);
						}
						else	{
							OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay);



							 if(orderdata.Status < info.Status )	{
								mysqlorder.UpdateOrderData_byRtnOrder(info);
								sprintf(buf,"Debug: OnRspQryOrder 平仓单-----[%d]    [%d]----- 更新   [%s]   orderinfo!!!!!!",
										orderdata.Status,info.Status,pOrder->InstrumentID);  //
								logutil.WriteLog(buf);

								}
							 else	 {
								 sprintf(buf,"Debug: OnRspQryOrder 平仓单-----[%d]    [%d]-----状态相同 无需更新  [%s] orderinfo!!!!!!",
											orderdata.Status,info.Status,pOrder->InstrumentID);  //
								logutil.WriteLog(buf);
							 }


						}

					}

			 Config config(this->GetStrategyName().c_str());
			 MysqlOrder mysqlorderlist(config.GetTable_OrderList().c_str(),this->GetPointUserLoginField()->UserID);

				if(!mysqlorderlist.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
					mysqlorderlist.NewOrderData_byRspQryOrder(info);
					sprintf(buf,"Debug: OnRspQryOrder -----orderlist %s-----不存在orderinfo,新插入!!!",
							pOrder->InstrumentID);  //
					logutil.WriteLog(buf);

				}

//				if(!mysqlorder.Exist_OrderbyInsOrderRef("hisorder",info.InstrumentID,info.OrderRef,TradingDay) && (info.Status==2 || info.Status==3))
//				{
//					mysqlorder.NewOrderData_byRspQryOrder("hisorder",info);
//					sprintf(buf,"Debug: OnRspQryOrder -----hisorder %s-----不存在hisorder,新插入!!!",
//							pOrder->InstrumentID);  //
//					logutil.WriteLog(buf);
//
//				}

			}



			// 如果响应函数已经返回最后一条信息
			if(bIsLast) {
				// 通知主过程，响应函数将结束
//				sem_post(&sem_order);
				this->PostSemOrder();
			}
		}
        ///请求查询成交响应
        void MyTDSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
        {
//        	 printf("OnRspQryTrade():被执行...\n");
        	 char buf[1024];
        	 LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
        	 logutil.WriteLog("\n");
			  logutil.WriteLog("OnRspQryTrade():被执行...");

			// 进程返回结果检查
			if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
				// typedef int TThostFtdcErrorIDType;
				// typedef char TThostFtdcErrorMsgType[81];
				char ErrorMsg[243];
				gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
				printf("OnRspQryTrade():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
			}

			// 如果有返回结果读取返回信息
			if ( pTrade != NULL && this->GetFlagLogin()) {
//				printf("Debug: BrokerID= %s  \n",pTrade->BrokerID);  //
//				printf("Debug: InvestorID= %s  \n",pTrade->InvestorID);  //
//
//				printf("Debug: InstrumentID= %s  \n",pTrade->InstrumentID);  //
//				printf("Debug: OrderRef= %s  \n",pTrade->OrderRef);  //
//				printf("Debug: TradeType= %d  \n",pTrade->TradeType);  //
//				printf("Debug: TradeDate= %s  \n",pTrade->TradeDate);  //
//				printf("Debug: TradeTime= %s  \n",pTrade->TradeTime);  //
//				printf("Debug: OffsetFlag= %d  \n",pTrade->OffsetFlag-'0');  // 开平标志
//				printf("Debug: Direction= %d  \n",pTrade->Direction-'0');  // 开平方向
//
//				printf("Debug: Price= %f  \n",pTrade->Price);  //
//
//				printf("Debug: Volume= %d  \n",pTrade->Volume);  //
//				printf("Debug: TradingDay= %s  \n",pTrade->TradingDay);  //
//				printf("Debug: SettlementID= %d  \n",pTrade->SettlementID);  //
//				printf("Debug: TradeID= %s  \n",pTrade->TradeID);  //
//				printf("Debug: OrderLocalID= %s  \n",pTrade->OrderLocalID);  //
//				printf("Debug: ClearingPartID= %s  \n",pTrade->ClearingPartID);  //
//				printf("Debug: ClientID= %s  \n",pTrade->ClientID);  //
//				printf("Debug: BusinessUnit= %s  \n",pTrade->BusinessUnit);  //
//				printf("Debug: SequenceNo= %d  \n",pTrade->SequenceNo);  //

//				sprintf(buf,"Debug: BrokerID= %s  ",pTrade->BrokerID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: InvestorID= %s  ",pTrade->InvestorID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: InstrumentID= %s  ",pTrade->InstrumentID);  //
//				logutil.WriteLog(buf);

				sprintf(buf,"Debug: BrokerID= %s  InvestorID= %s  InstrumentID= %s  ",pTrade->BrokerID,
						pTrade->InvestorID,pTrade->InstrumentID);  //
				logutil.WriteLog(buf);

//				sprintf(buf,"Debug: OrderRef= %s  ",pTrade->OrderRef);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: TradeType= %d  ",pTrade->TradeType);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: TradeDate= %s  ",pTrade->TradeDate);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: TradeTime= %s  ",pTrade->TradeTime);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: OffsetFlag= %d  ",pTrade->OffsetFlag-'0');  // 开平标志
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: Direction= %d  ",pTrade->Direction-'0');  // 开平方向
//				logutil.WriteLog(buf);
//
//				sprintf(buf,"Debug: Price= %f  ",pTrade->Price);  //
//				logutil.WriteLog(buf);
//
//				sprintf(buf,"Debug: Volume= %d  ",pTrade->Volume);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: TradingDay= %s  ",pTrade->TradingDay);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: SettlementID= %d  ",pTrade->SettlementID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: TradeID= %s  ",pTrade->TradeID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: OrderLocalID= %s  ",pTrade->OrderLocalID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: ClearingPartID= %s  ",pTrade->ClearingPartID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: ClientID= %s  ",pTrade->ClientID);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: BusinessUnit= %s  ",pTrade->BusinessUnit);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: SequenceNo= %d  ",pTrade->SequenceNo);  //
//				logutil.WriteLog(buf);

//				TradeInfo tradeinfo;
//				strcpy(tradeinfo.InstrumentID,pTrade->InstrumentID);
//				strcpy(tradeinfo.TradeID,pTrade->TradeID);
//				strcpy(tradeinfo.ClientID,pTrade->ClientID);
//				strcpy(tradeinfo.TradingDay,pTrade->TradingDay);
//				strcpy(tradeinfo.TradeDate,pTrade->TradeDate);
//				strcpy(tradeinfo.TradeTime,pTrade->TradeTime);
//				strcpy(tradeinfo.OrderRef,pTrade->OrderRef);
//				tradeinfo.Direction = pTrade->Direction-'0';
//				tradeinfo.OffsetFlag = pTrade->OffsetFlag-'0';
//				tradeinfo.Price = pTrade->Price;
//				tradeinfo.Volume = pTrade->Volume;
//				tradeinfo.SettlementID = pTrade->SettlementID;
//				tradeinfo.SequenceNo = pTrade->SequenceNo;
//
//				MongoTrade mongotrade;
//				mongotrade.Check_Trade(tradeinfo);

			}

			 // 如果响应函数已经返回最后一条信息
			if(bIsLast) {
				// 通知主过程，响应函数将结束
//				sem_post(&sem_trade);
				this->PostSemTrade();
				logutil.WriteLog("OnRspQryTrade():被执行...完成!!!! ");
			}

        }
///请求查询投资者持仓响应
void MyTDSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition,
	CThostFtdcRspInfoField *pRspInfo,
	int nRequestID,
	bool bIsLast)
{
	//    		 printf("OnRspQryInvestorPosition():被执行...\n");

	 char buf[1024];
	  LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	  logutil.WriteLog("OnRspQryInvestorPosition():被执行...");

	// 进程返回结果检查
	if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
		// typedef int TThostFtdcErrorIDType;
		// typedef char TThostFtdcErrorMsgType[81];
		char ErrorMsg[243];
		gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
		printf("OnRspQryInvestorPosition():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
	}

	// 如果有返回结果读取返回信息
	if ( pInvestorPosition != NULL && this->GetFlagLogin()) {
	//				printf("Debug: BrokerID= %s  \n",pInvestorPosition->BrokerID);  //
	//				printf("Debug: InvestorID= %s  \n",pInvestorPosition->InvestorID);  //

//		printf("OnRspQryInvestorPosition --->Debug: InstrumentID= %s  PosiDirection= %d  \n",
//				pInvestorPosition->InstrumentID,pInvestorPosition->PosiDirection-'0');  //
	//				printf("OnRspQryInvestorPosition ---->Debug: PosiDirection= %d  \n",pInvestorPosition->PosiDirection-'0');  //

		///净
		//#define THOST_FTDC_PD_Net '1'
		///多头
		//#define THOST_FTDC_PD_Long '2'
		///空头
		//#define THOST_FTDC_PD_Short '3'


	//				printf("Debug: MarginRateByMoney= %f  \n",pInvestorPosition->MarginRateByMoney);  //
	//				printf("Debug: MarginRateByVolume= %f  \n",pInvestorPosition->MarginRateByVolume);  //
	//				printf("Debug: ExchangeMargin= %f  \n",pInvestorPosition->ExchangeMargin);  //
	//				printf("Debug: YdPosition= %d  \n",pInvestorPosition->YdPosition);  //
	//				printf("Debug: Position= %d  \n",pInvestorPosition->Position);  //
	//				printf("Debug: TodayPosition= %d  \n",pInvestorPosition->TodayPosition);  //
	//
	//				printf("Debug: PreMargin= %f  \n",pInvestorPosition->PreMargin);  //
	//				printf("Debug: UseMargin= %f  \n",pInvestorPosition->UseMargin);  //
	//				printf("Debug: FrozenMargin= %f  \n",pInvestorPosition->FrozenMargin);  //
	//				printf("Debug: FrozenCash= %f  \n",pInvestorPosition->FrozenCash);  //
	//				printf("Debug: FrozenCommission= %f  \n",pInvestorPosition->FrozenCommission);  //
	//				printf("Debug: CashIn= %f  \n",pInvestorPosition->CashIn);  //
	//				printf("Debug: Commission= %f  \n",pInvestorPosition->Commission);  //
	//
	//				printf("Debug: CloseProfit= %f  \n",pInvestorPosition->CloseProfit);  //
	//				printf("Debug: PositionProfit= %f  \n",pInvestorPosition->PositionProfit);  //
	//				printf("Debug: PreSettlementPrice= %f  \n",pInvestorPosition->PreSettlementPrice);  //
	//				printf("Debug: SettlementPrice= %f  \n",pInvestorPosition->SettlementPrice);  //
	//				printf("Debug: TradingDay= %s  \n",pInvestorPosition->TradingDay);  //
	//				printf("Debug: SettlementID= %d  \n",pInvestorPosition->SettlementID);  //
	//				printf("Debug: OpenCost= %f  \n",pInvestorPosition->OpenCost);  //
	//				printf("Debug: PositionDate= %d  \n",pInvestorPosition->PositionDate);  //
	//
	//				printf("Debug: HedgeFlag= %d  \n",pInvestorPosition->HedgeFlag-'0');  //
	//
	//				printf("Debug: CloseVolume= %d  \n",pInvestorPosition->CloseVolume);  //

		sprintf(buf,"OnRspQryInvestorPosition  >>> Debug: InstrumentID= %s  Position= %d   PosiDirection= %d CloseProfit= %f ",
				pInvestorPosition->InstrumentID,pInvestorPosition->Position,pInvestorPosition->PosiDirection-'0',pInvestorPosition->CloseProfit);  //
		logutil.WriteLog(buf);

	//				//#define THOST_FTDC_PSD_Today '1'
	//				//#define THOST_FTDC_PSD_History '2'
	//				sprintf(buf,"Debug: PositionDate= %d  ",pInvestorPosition->PositionDate-'0');  //
	//				logutil.WriteLog(buf);

	//				sprintf(buf,"Debug: PositionProfit= %f  ",pInvestorPosition->PositionProfit);  //
	//				logutil.WriteLog(buf);
	//				sprintf(buf,"Debug: CloseVolume= %d  ",pInvestorPosition->CloseVolume);  //
	//				logutil.WriteLog(buf);
	//				sprintf(buf,"Debug: Commission= %f  ",pInvestorPosition->Commission);  //
	//				logutil.WriteLog(buf);



//		std::shared_ptr<InvestPositionBuffer> info = std::make_shared<InvestPositionBuffer>();
//		info->symbol=pInvestorPosition->InstrumentID;
//
//		if(pInvestorPosition->PosiDirection-'0'==0){
//			info->longposition=pInvestorPosition->Position;
//			info->longydposition=pInvestorPosition->YdPosition;
//			info->longtodayposition=pInvestorPosition->TodayPosition;
//		}
//		else if(pInvestorPosition->PosiDirection-'0'==1){
//			info->shortposition=pInvestorPosition->Position;
//			info->shortydposition=pInvestorPosition->YdPosition;
//			info->shorttodayposition=pInvestorPosition->TodayPosition;
//		}
//
//
//		std::string key=info->symbol;
//
//		std::unique_lock<std::mutex>lck(m_investpositionmutex);
//		if (m_investpositionmap.find(key) != m_investpositionmap.end()) {
//			m_investpositionmap[key] = info;
//		}
//		else	{
//			m_investpositionmap.insert(std::pair<std::string, std::shared_ptr<InvestPositionBuffer>>(key, info));
//
//		}

		InvestPositionData data;
		strcpy(data.InstrumentID,pInvestorPosition->InstrumentID);
		strcpy(data.TradingDay,pInvestorPosition->TradingDay);
		data.PositionDate=pInvestorPosition->PositionDate-'0';
		data.PosiDirection = pInvestorPosition->PosiDirection-'0';
		data.YdPosition = pInvestorPosition->YdPosition;
		data.Position = pInvestorPosition->Position;
		data.TodayPosition = pInvestorPosition->TodayPosition;
		data.CloseProfit = pInvestorPosition->CloseProfit;
		data.OpenAmount = pInvestorPosition->OpenAmount;
		data.CloseAmount = pInvestorPosition->CloseAmount;
		data.OpenVolume = pInvestorPosition->OpenVolume;
		data.CloseVolume = pInvestorPosition->CloseVolume;
		data.Commission = pInvestorPosition->Commission;

		data.UseMargin = pInvestorPosition->UseMargin;
		data.SettlementPrice = pInvestorPosition->SettlementPrice;
		data.PreSettlementPrice = pInvestorPosition->PreSettlementPrice;
		data.PositionProfit = pInvestorPosition->PositionProfit;
		data.HedgeFlag = pInvestorPosition->HedgeFlag-'0';

		InstrumentData instrumentdata;
		MysqlInstrument mysqlinstrument;
		if (mysqlinstrument.Exist_DatabyInstrumentID(data.InstrumentID) && data.Position >0 &&  data.OpenAmount >0 ){

			instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(data.InstrumentID);
			data.OpenPriceAverage = data.OpenAmount/((double)instrumentdata.VolumeMultiple*(double)data.Position);
			printf("Debug: OpenAmount:%.3f Position:%d OpenPriceAverage: %.3f \n",
										data.OpenAmount,data.Position,data.OpenPriceAverage);  //
			sprintf(buf,"Debug: OpenAmount:%.3f Position:%d OpenPriceAverage: %.3f \n",
					data.OpenAmount,data.Position,data.OpenPriceAverage);  //
			logutil.WriteLog(buf);
		}
		else{
			MysqlPosDetail mysqlposdetail(this->GetPointUserLoginField()->UserID);
			if(mysqlposdetail.Exist_DatabyInstrumentid(data.InstrumentID)){
				data.OpenPriceAverage = mysqlposdetail.CollectOpenAveragePrice(data.InstrumentID);
				printf("Debug:MysqlPosDetail>>>  OpenPriceAverage: %.3f \n",data.OpenPriceAverage);  //
				sprintf(buf,"Debug:  OpenPriceAverage: %.3f \n",data.OpenPriceAverage);  //
				logutil.WriteLog(buf);
			}
			else{
				data.OpenPriceAverage=0.0;
				printf(buf,"Debug:  OpenPriceAverage: %.3f  Error!!! \n",data.OpenPriceAverage);  //
				sprintf(buf,"Debug:  OpenPriceAverage: %.3f  Error!!! \n",data.OpenPriceAverage);  //
				logutil.WriteLog(buf);
			}

		}

		m_datawrapper->InsertInvestPositionMap(data);

		MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
		if(!mysqlinvest.Exist_DatabyInstrumentID(data.InstrumentID))	{
			mysqlinvest.Insert(data);
		}
	}

	 // 如果响应函数已经返回最后一条信息
	if(bIsLast) {
		// 通知主过程，响应函数将结束
		this->PostSemInvest();
		logutil.WriteLog("OnRspQryInvestorPosition():被执行...完成!!!! >>>>>>>");
	}
}

    //*/
    ///请求查询资金账户响应
    //*/
        void MyTDSpi::OnRspQryTradingAccount(
            CThostFtdcTradingAccountField * pTradingAccount,
            CThostFtdcRspInfoField * pRspInfo,
            int nRequestID,
            bool bIsLast
        ) {

            printf("MyTDSpi::OnRspQryTradingAccount():被执行...\n");

            char buf[1024];
            LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
			logutil.WriteLog("MyTDSpi::OnRspQryTradingAccount():被执行...");

            // 进程返回结果检查
            if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
                // typedef int TThostFtdcErrorIDType;
                // typedef char TThostFtdcErrorMsgType[81];
                char ErrorMsg[243];
                gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
                printf("MyTDSpi::OnRspQryTradingAccount():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
                sprintf(buf,"MyTDSpi::OnRspQryTradingAccount():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
                logutil.WriteLog(buf);
            }

            // 如果有返回结果读取返回信息
            if ( pTradingAccount != NULL && this->GetFlagLogin()) {
                // 读取返回信息,并做编码转化
                ///经纪公司代码 TThostFtdcBrokerIDType char[11]
                char BrokerID[33];
                gbk2utf8(pTradingAccount->BrokerID,BrokerID,sizeof(BrokerID));
                ///投资者帐号 TThostFtdcAccountIDType char[13]
                char AccountID[39];
                gbk2utf8(pTradingAccount->AccountID,AccountID,sizeof(AccountID));
//    			printf("len(pTradingAccount->AccountID)=%d\n",(int)strlen(pTradingAccount->AccountID));
//    			printf("len(AccountID)=%d\n",(int)strlen(AccountID));
                ///上次质押金额 TThostFtdcMoneyType double
                double PreMortgage = pTradingAccount->PreMortgage;
                ///上次信用额度 TThostFtdcMoneyType double
                double PreCredit = pTradingAccount->PreCredit;
                ///上次存款额 TThostFtdcMoneyType double
                double PreDeposit = pTradingAccount->PreDeposit;
                ///上次结算准备金 TThostFtdcMoneyType double
                double PreBalance = pTradingAccount->PreBalance;
                ///上次占用的保证金 TThostFtdcMoneyType double
                double PreMargin = pTradingAccount->PreMargin;
                ///利息基数 TThostFtdcMoneyType double
                double InterestBase = pTradingAccount->InterestBase;
                ///利息收入 TThostFtdcMoneyType double
                double Interest = pTradingAccount->Interest;
                ///入金金额 TThostFtdcMoneyType double
                double Deposit = pTradingAccount->Deposit;
                ///出金金额 TThostFtdcMoneyType double
                double Withdraw = pTradingAccount->Withdraw;
                ///冻结的保证金 TThostFtdcMoneyType double
                double FrozenMargin = pTradingAccount->FrozenMargin;
                ///冻结的资金 TThostFtdcMoneyType double
                double FrozenCash = pTradingAccount->FrozenCash;
                ///冻结的手续费 TThostFtdcMoneyType double
                double FrozenCommission = pTradingAccount->FrozenCommission;
                ///当前保证金总额 TThostFtdcMoneyType double
                double CurrMargin = pTradingAccount->CurrMargin;
                ///资金差额 TThostFtdcMoneyType double
                double CashIn = pTradingAccount->CashIn;
                ///手续费 TThostFtdcMoneyType double
                double Commission = pTradingAccount->Commission;
                ///平仓盈亏 TThostFtdcMoneyType double
                double CloseProfit = pTradingAccount->CloseProfit;
                ///持仓盈亏 TThostFtdcMoneyType double
                double PositionProfit = pTradingAccount->PositionProfit;
                ///期货结算准备金 TThostFtdcMoneyType double
                double Balance = pTradingAccount->Balance;
                ///可用资金 TThostFtdcMoneyType double
                double Available = pTradingAccount->Available;
                ///可取资金 TThostFtdcMoneyType double
                double WithdrawQuota = pTradingAccount->WithdrawQuota;
                ///基本准备金 TThostFtdcMoneyType double
                double Reserve = pTradingAccount->Reserve;
                ///交易日 TThostFtdcDateType char[9]
                char TradingDay[27];
                gbk2utf8(pTradingAccount->TradingDay,TradingDay,sizeof(TradingDay));
                ///结算编号 TThostFtdcSettlementIDType int
                int SettlementID = pTradingAccount->SettlementID;
                ///信用额度 TThostFtdcMoneyType double
                double Credit = pTradingAccount->Credit;
                ///质押金额 TThostFtdcMoneyType double
                double Mortgage = pTradingAccount->Mortgage;
                ///交易所保证金 TThostFtdcMoneyType double
                double ExchangeMargin = pTradingAccount->ExchangeMargin;
                ///投资者交割保证金 TThostFtdcMoneyType double
                double DeliveryMargin = pTradingAccount->DeliveryMargin;
                ///交易所交割保证金 TThostFtdcMoneyType double
                double ExchangeDeliveryMargin = pTradingAccount->ExchangeDeliveryMargin;
                ///保底期货结算准备金 TThostFtdcMoneyType double
                double ReserveBalance = pTradingAccount->ReserveBalance;
                ///币种代码 TThostFtdcCurrencyIDType char[4]
                char CurrencyID[12];
                gbk2utf8(pTradingAccount->CurrencyID,CurrencyID,sizeof(pTradingAccount->CurrencyID));
                ///上次货币质入金额 TThostFtdcMoneyType double
                double PreFundMortgageIn = pTradingAccount->PreFundMortgageIn;
                ///上次货币质出金额 TThostFtdcMoneyType double
                double PreFundMortgageOut = pTradingAccount->PreFundMortgageOut;
                ///货币质入金额 TThostFtdcMoneyType double
                double FundMortgageIn = pTradingAccount->FundMortgageIn;
                ///货币质出金额 TThostFtdcMoneyType double
                double FundMortgageOut = pTradingAccount->FundMortgageOut;
                ///货币质押余额 TThostFtdcMoneyType double
                double FundMortgageAvailable = pTradingAccount->FundMortgageAvailable;
                ///可质押货币金额 TThostFtdcMoneyType double
                double MortgageableFund = pTradingAccount->MortgageableFund;
                ///特殊产品占用保证金 TThostFtdcMoneyType double
                double SpecProductMargin = pTradingAccount->SpecProductMargin;
                ///特殊产品冻结保证金 TThostFtdcMoneyType double
                double SpecProductFrozenMargin = pTradingAccount->SpecProductFrozenMargin;
                ///特殊产品手续费 TThostFtdcMoneyType double
                double SpecProductCommission = pTradingAccount->SpecProductCommission;
                ///特殊产品冻结手续费 TThostFtdcMoneyType double
                double SpecProductFrozenCommission = pTradingAccount->SpecProductFrozenCommission;
                ///特殊产品持仓盈亏 TThostFtdcMoneyType double
                double SpecProductPositionProfit = pTradingAccount->SpecProductPositionProfit;
                ///特殊产品平仓盈亏 TThostFtdcMoneyType double
                double SpecProductCloseProfit = pTradingAccount->SpecProductCloseProfit;
                ///根据持仓盈亏算法计算的特殊产品持仓盈亏 TThostFtdcMoneyType double
                double SpecProductPositionProfitByAlg = pTradingAccount->SpecProductPositionProfitByAlg;
                ///特殊产品交易所保证金 TThostFtdcMoneyType double
                double SpecProductExchangeMargin = pTradingAccount->SpecProductExchangeMargin;

    			// 打印出关键信息
    			printf("经纪公司代码=%s,投资者帐号=%s,币种代码=%s,可用金额=%.2f\n",BrokerID,AccountID,CurrencyID,Available);
    			printf("上次结算准备金=%f\n",PreBalance);
    			printf("入金=%f\n",Deposit);
    			printf("出金=%f\n",Withdraw);
//    			printf("可用金额=%f\n",Available);
//    			printf("当前保证金总额=%f\n",CurrMargin);
//    			printf("Deposit=%.2f\n",Deposit);
//    			printf("Withdraw=%.2f\n",Withdraw);
//    			printf("CashIn=%.2f\n",CashIn);
//    			printf("Commission=%.2f\n",Commission);
//    			printf("TradingDay: %s\n",TradingDay);
//    			printf("SettlementID: %.d\n",SettlementID);

    			sprintf(buf,"上次结算准备金=%f",PreBalance);
    			logutil.WriteLog(buf);
    			sprintf(buf,"出金=%f",pTradingAccount->Withdraw);
				logutil.WriteLog(buf);
				sprintf(buf,"入金=%f",pTradingAccount->Deposit);
				logutil.WriteLog(buf);
    			sprintf(buf,"可用金额=%f",Available);
    			logutil.WriteLog(buf);
				sprintf(buf,"当前保证金总额=%f",CurrMargin);
				logutil.WriteLog(buf);
				sprintf(buf,"TradingDay: %s",TradingDay);
				logutil.WriteLog(buf);



    			AssetData info;
    			strcpy(info.TradingDay,TradingDay);
    			strcpy(info.AccountID,AccountID);
    			info.PreBalance = PreBalance;
    			info.Available = Available;
    			info.CurrMargin = CurrMargin;
				info.StaticEquity=PreBalance -pTradingAccount->Withdraw+pTradingAccount->Deposit;
				info.Withdraw = pTradingAccount->Withdraw;
				info.Deposit = pTradingAccount->Deposit;
				info.DynamicEquity = PreBalance+PositionProfit+CloseProfit;
				info.HoldProfit=PositionProfit;
				info.HighestProfit=0;
				info.TotalCaptial=0.0;


    			MysqlAsset  mysqlasset(this->_userLoginField.UserID);
				mysqlasset.CreateTable();
    			if(!mysqlasset.Exist_DatabyDate(info.TradingDay))	{
    				mysqlasset.Insert(info);
    				printf("MyTDSpi::OnRspQryTradingAccount 插入新的交易日   -----> MysqlAsset 数据库  %s  \n",info.TradingDay);
    				sprintf(buf,"MyTDSpi::OnRspQryTradingAccount 插入新的交易日   -----> MysqlAsset 数据库  %s  ",info.TradingDay);
					logutil.WriteLog(buf);
    			}

    			//同时更新交易日数据库 不可删除
    			MysqlTradingDay mysqltradingday(this->_userLoginField.UserID);
    			TradingDayData tradingdayinfo;
				strcpy(tradingdayinfo.TradingDay,info.TradingDay);
				DateUtil dateutil;
				strcpy(tradingdayinfo.TradingDateTime,dateutil.ConvertDatetoSqlTime(info.TradingDay).c_str());

				if(!mysqltradingday.Exist_DateTime(tradingdayinfo.TradingDateTime)  ||
						!mysqltradingday.Exist_TradingDay(tradingdayinfo.TradingDay))	{
					printf("MyTDSpi::OnRspQryTradingAccount 插入新的交易日  --->MysqlTradingDay 交易日数据库  %s  \n",tradingdayinfo.TradingDateTime );

					sprintf(buf,"MyTDSpi::OnRspQryTradingAccount 插入新的交易日   -----> MysqlTradingDay 交易日数据库  %s  ",tradingdayinfo.TradingDateTime);
					logutil.WriteLog(buf);

					TradingDayData last;
					last= mysqltradingday.Find_LastTradingDay();

					tradingdayinfo.DifSeconds=last.DifSeconds;
					mysqltradingday.Insert(tradingdayinfo);
				}
				else	{

				}

				// 设置私有变量本地交易日
				this->SetTradingDay(TradingDay);
            }

            // 如果响应函数已经返回最后一条信息
            if(bIsLast) {
                // 通知主过程，响应函数将结束
                this->PostSemTradingAccount();
                logutil.WriteLog("MyTDSpi::OnRspQryTradingAccount():被执行...End!!! ");
            }
        }

	///请求查询投资者持仓明细响应
	void MyTDSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail,
			CThostFtdcRspInfoField *pRspInfo,
			int nRequestID,
			bool bIsLast)

		{
//		  printf("OnRspQryInvestorPositionDetail():被执行...\n");
		  char buf[1024];
		  LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		  logutil.WriteLog("OnRspQryInvestorPositionDetail():被执行...");

			// 进程返回结果检查
			if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
				// typedef int TThostFtdcErrorIDType;
				// typedef char TThostFtdcErrorMsgType[81];
				char ErrorMsg[243];
				gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
				printf("OnRspQryInvestorPositionDetail():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
			}

			// 如果有返回结果读取返回信息
			if ( pInvestorPositionDetail != NULL && this->GetFlagLogin()) {
				// 读取返回信息,并做编码转化
//				printf("Debug: BrokerID= %s  \n",pInvestorPositionDetail->BrokerID);  //
//				printf("Debug: InvestorID= %s  \n",pInvestorPositionDetail->InvestorID);  //

//				printf("Debug: InstrumentID= %s  \n",pInvestorPositionDetail->InstrumentID);  //
//				printf("Debug: Direction= %d  \n",pInvestorPositionDetail->Direction-'0');  //
//				printf("Debug: MarginRateByMoney= %f  \n",pInvestorPositionDetail->MarginRateByMoney);  //
//				printf("Debug: MarginRateByVolume= %f  \n",pInvestorPositionDetail->MarginRateByVolume);  //
//				printf("Debug: OpenPrice= %f  \n",pInvestorPositionDetail->OpenPrice);  //
//				printf("Debug: OpenDate= %s  \n",pInvestorPositionDetail->OpenDate);  //
//				printf("Debug: HedgeFlag= %d  \n",pInvestorPositionDetail->HedgeFlag);  //
//				printf("Debug: LastSettlementPrice= %f  \n",pInvestorPositionDetail->LastSettlementPrice);  //
//				printf("Debug: Volume= %d  \n",pInvestorPositionDetail->Volume);  //

				char TradeID[33];
				gbk2utf8(pInvestorPositionDetail->TradeID,TradeID,sizeof(TradeID));

//				printf("Debug: TradeID= %s  \n",TradeID);  //
//				printf("Debug: TradeType= %d  \n",pInvestorPositionDetail->TradeType);  //
//
//				printf("Debug: CloseProfitByTrade= %f  \n",pInvestorPositionDetail->CloseProfitByTrade);  //逐笔对冲平仓盈亏
//				printf("Debug: CloseProfitByDate= %f  \n",pInvestorPositionDetail->CloseProfitByDate);  // 逐日盯市平仓盈亏
//				printf("Debug: CloseAmount= %f  \n",pInvestorPositionDetail->CloseAmount);  //
//				printf("Debug: PositionProfitByTrade= %f  \n",pInvestorPositionDetail->PositionProfitByTrade);  //逐笔对冲持仓盈亏
//				printf("Debug: PositionProfitByDate= %f  \n",pInvestorPositionDetail->PositionProfitByDate);  //逐日盯市持仓盈亏
//
//
//				printf("Debug: Margin= %f  \n",pInvestorPositionDetail->Margin);  //
//				printf("Debug: ExchMargin= %f  \n",pInvestorPositionDetail->ExchMargin);  //
//
//				printf("Debug: SettlementPrice= %f  \n",pInvestorPositionDetail->SettlementPrice);  //
//				printf("Debug: TradingDay= %s  \n",pInvestorPositionDetail->TradingDay);  //
//				printf("Debug: SettlementID= %d  \n",pInvestorPositionDetail->SettlementID);  //
//				printf("Debug: CombInstrumentID= %s  \n",pInvestorPositionDetail->CombInstrumentID);  //

				sprintf(buf,"Debug: InstrumentID= %s  ",pInvestorPositionDetail->InstrumentID);  //
				logutil.WriteLog(buf);
				sprintf(buf,"Debug: Direction= %d  ",pInvestorPositionDetail->Direction-'0');  //
				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: TradeType= %d  ",pInvestorPositionDetail->TradeType-'0');  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: CloseProfitByTrade= %f  ",pInvestorPositionDetail->CloseProfitByTrade);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: CloseProfitByDate= %f  ",pInvestorPositionDetail->CloseProfitByDate);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: PositionProfitByTrade= %f  ",pInvestorPositionDetail->PositionProfitByTrade);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: PositionProfitByDate= %f  ",pInvestorPositionDetail->PositionProfitByDate);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: OpenPrice= %f  ",pInvestorPositionDetail->OpenPrice);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: SettlementPrice= %f  ",pInvestorPositionDetail->SettlementPrice);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: LastSettlementPrice= %f  ",pInvestorPositionDetail->LastSettlementPrice);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: CloseAmount= %f  ",pInvestorPositionDetail->CloseAmount);  //
//				logutil.WriteLog(buf);
//				sprintf(buf,"Debug: Volume= %d  ",pInvestorPositionDetail->Volume);  //
//				logutil.WriteLog(buf);

				PositionDetailData data;
				strcpy(data.InstrumentID,pInvestorPositionDetail->InstrumentID);
				strcpy(data.TradeID,TradeID);
				strcpy(data.TradingDay,pInvestorPositionDetail->TradingDay);
				data.Direction = pInvestorPositionDetail->Direction-'0';
				data.MarginRateByMoney = pInvestorPositionDetail->MarginRateByMoney;
				data.MarginRateByVolume = pInvestorPositionDetail->MarginRateByVolume;
				data.OpenPrice = pInvestorPositionDetail->OpenPrice;
				strcpy(data.OpenDate,pInvestorPositionDetail->OpenDate);
				strcpy(data.ExchangeID,pInvestorPositionDetail->ExchangeID);
				data.CloseProfitByTrade = pInvestorPositionDetail->CloseProfitByTrade;
				data.CloseProfitByDate = pInvestorPositionDetail->CloseProfitByDate;
				data.PositionProfitByTrade = pInvestorPositionDetail->PositionProfitByTrade;
				data.PositionProfitByDate = pInvestorPositionDetail->PositionProfitByDate;
				data.Margin = pInvestorPositionDetail->Margin;
				data.Volume = pInvestorPositionDetail->Volume;
				data.CloseAmount = pInvestorPositionDetail->CloseAmount;
				data.CloseVolume =pInvestorPositionDetail->CloseVolume;



				MysqlPosDetail mysqlposdetail(this->GetPointUserLoginField()->UserID);

				mysqlposdetail.CreateTable();

				if(!mysqlposdetail.Exist_PositionbyInstrumentid(data.InstrumentID,data.TradeID,data.OpenDate))		{
					mysqlposdetail.Insert(data);
				}


			}

			// 如果响应函数已经返回最后一条信息
			if(bIsLast) {
				// 通知主过程，响应函数将结束
				sem_post(this->GetSemInvestPosDetail());

			}
		}
    //*/
    //    请求查询客户通知响应
    //*/

    void MyTDSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
    	 printf("OnRspQryNotice():被执行...\n");
    	 char buf[4096];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		logutil.WriteLog("OnRspQryNotice():被执行...");


		// 进程返回结果检查
		if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
			// typedef int TThostFtdcErrorIDType;
			// typedef char TThostFtdcErrorMsgType[81];
			char ErrorMsg[243];
			gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
			printf("OnRspQryNotice():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
		}

		// 如果有返回结果读取返回信息
		if ( pNotice != NULL&& this->GetFlagLogin() ) {

			printf("OnRspQryNotice  ----->BrokerID: %s \n ",pNotice->BrokerID);
			printf("OnRspQryNotice  ----->Content:  %s  \n ",pNotice->Content);
			printf("OnRspQryNotice  ----->SequenceLabel: %s  \n ",pNotice->SequenceLabel);

			sprintf(buf,"OnRspQryNotice  ----->BrokerID: %s \n ",pNotice->BrokerID);
			logutil.WriteLog(buf);
			sprintf(buf,"OnRspQryNotice  ----->Content:  %s  \n ",pNotice->Content);  //501字节
			logutil.WriteLog(buf);
			sprintf(buf,"OnRspQryNotice  ----->SequenceLabel: %s  \n ",pNotice->SequenceLabel);
			logutil.WriteLog(buf);

		}


		// 如果响应函数已经返回最后一条信息
		if(bIsLast) {
			// 通知主过程，响应函数将结束

			this->PostSemNotice();
		}

    }


    ///查询最大报单数量响应
       void MyTDSpi::OnRspQueryMaxOrderVolume(
           CThostFtdcQueryMaxOrderVolumeField * pQueryMaxOrderVolume,
           CThostFtdcRspInfoField * pRspInfo,
           int nRequestID,
           bool bIsLast
       ) {
           printf("OnRspQueryMaxOrderVolume():被执行...\n");
           LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

           logutil.WriteLog("OnRspQueryMaxOrderVolume():被执行...");
           // 进程返回结果检查
           if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
               // typedef int TThostFtdcErrorIDType;
               // typedef char TThostFtdcErrorMsgType[81];
               char ErrorMsg[243];
               gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
               printf("OnRspQueryMaxOrderVolume():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
           }

           // 如果有返回结果读取返回信息
           if ( pQueryMaxOrderVolume != NULL && this->GetFlagLogin()) {
   			printf("pQueryMaxOrderVolume != NULL\n");
               // 读取返回信息,并做编码转化
               ///经纪公司代码 TThostFtdcBrokerIDType char[11]
               char BrokerID[33];
               gbk2utf8(pQueryMaxOrderVolume->BrokerID,BrokerID,sizeof(BrokerID));
               ///投资者代码 TThostFtdcInvestorIDType char[13]
               char InvestorID[39];
               gbk2utf8(pQueryMaxOrderVolume->InvestorID,InvestorID,sizeof(InvestorID));
               ///合约代码 TThostFtdcInstrumentIDType char[31]
               char InstrumentID[93];
               gbk2utf8(pQueryMaxOrderVolume->InstrumentID,InstrumentID,sizeof(InstrumentID));
               ///买卖方向 TThostFtdcDirectionType char
               //// THOST_FTDC_D_Buy '0' 买
               //// THOST_FTDC_D_Sell '1' 卖
               char Direction = pQueryMaxOrderVolume->Direction;
               ///开平标志 TThostFtdcOffsetFlagType char
               //// THOST_FTDC_OF_Open '0' 开仓
               //// THOST_FTDC_OF_Close '1' 平仓
               //// THOST_FTDC_OF_ForceClose '2' 强平
               //// THOST_FTDC_OF_CloseToday '3' 平今
               //// THOST_FTDC_OF_CloseYesterday '4' 平昨
               //// THOST_FTDC_OF_ForceOff '5' 强减
               //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
               char OffsetFlag = pQueryMaxOrderVolume->OffsetFlag;
               ///投机套保标志 TThostFtdcHedgeFlagType char
               //// THOST_FTDC_HF_Speculation '1' 投机
               //// THOST_FTDC_HF_Arbitrage '2' 套利
               //// THOST_FTDC_HF_Hedge '3' 套保
               char HedgeFlag = pQueryMaxOrderVolume->HedgeFlag;
               ///最大允许报单数量 TThostFtdcVolumeType int
               int MaxVolume = pQueryMaxOrderVolume->MaxVolume;

   			printf("BrokerID=%s,InvestorID=%s,InstrumentID=%s,Direction=%c,OffsetFlag=%c,HedgeFlag=%c,MaxVolume=%d\n",
   					BrokerID,InvestorID,InstrumentID,Direction,OffsetFlag,HedgeFlag,MaxVolume);

   			char buf[1024];
   			sprintf(buf,"BrokerID=%s,InvestorID=%s,InstrumentID=%s,Direction=%c,OffsetFlag=%c,HedgeFlag=%c,MaxVolume=%d ",
   			   					BrokerID,InvestorID,InstrumentID,Direction,OffsetFlag,HedgeFlag,MaxVolume);
   			logutil.WriteLog(buf);
           }

           // 如果响应函数已经返回最后一条信息
           if(bIsLast) {
               // 通知主过程，响应函数将结束

               this->PostSemMaxOrder();
           }
       }


	//*/
	///请求查询结算信息确认响应
	//*/
       void MyTDSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    		   CThostFtdcRspInfoField *pRspInfo,
			   int nRequestID,
			   bool bIsLast)
              {
              	char buf[1050];
                  printf("OnRspQrySettlementInfoConfirm():被执行...\n");
                  LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
          		logutil.WriteLog("OnRspQrySettlementInfoConfirm():被执行...");

                  // 进程返回结果检查
                  if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
                      // typedef int TThostFtdcErrorIDType;
                      // typedef char TThostFtdcErrorMsgType[81];
                      char ErrorMsg[243];
                      gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
                      printf("OnRspQrySettlementInfoConfirm():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

                      sprintf(buf,"OnRspQrySettlementInfoConfirm():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
                      logutil.WriteLog(buf);
                  }

                  // 如果有返回结果读取返回信息
                  if ( pSettlementInfoConfirm != NULL && this->GetFlagLogin() && this->GetFlagFrontConnectFalg()) {

                      // 读取返回信息,并做编码转化
                	  ///经纪公司代码 TThostFtdcBrokerIDType char[11]
                	  CodeUtil codeutil;
                      char BrokerID[33];
                      codeutil.gbk2utf8(pSettlementInfoConfirm->BrokerID,BrokerID,sizeof(BrokerID));
                      ///投资者代码 TThostFtdcInvestorIDType char[13]
                      char InvestorID[39];
                      codeutil.gbk2utf8(pSettlementInfoConfirm->InvestorID,InvestorID,sizeof(InvestorID));
                      ///确认日期 TThostFtdcDateType char[9]
                      char ConfirmDate[27];
                      codeutil.gbk2utf8(pSettlementInfoConfirm->ConfirmDate,ConfirmDate,sizeof(ConfirmDate));
                      ///纭鏃堕棿 TThostFtdcTimeType char[9]
                      char ConfirmTime[27];
                      codeutil.gbk2utf8(pSettlementInfoConfirm->ConfirmTime,ConfirmTime,sizeof(ConfirmTime));

          			 printf("MyTDSpi::OnRspQrySettlementInfoConfirm  请求查询结算信息确认响应  ConfirmDate=%s,ConfirmTime=%s\n",ConfirmDate,ConfirmTime);
          			 sprintf(buf,"MyTDSpi::OnRspQrySettlementInfoConfirm  请求查询结算信息确认响应  ConfirmDate=%s,ConfirmTime=%s\n",ConfirmDate,ConfirmTime);
          			 logutil.WriteLog(buf);


          			 //同时更新交易日数据库 不可删除
					MysqlTradingDay mysqltradingday(this->_userLoginField.UserID);

					if(mysqltradingday.Exist_Table()){

						if(mysqltradingday.Exist_TradingDay(this->GetTradingDay().c_str())){

							TradingDayData last;
							last= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());
							if (!last.SettleInfoConfirmStatus){

								 printf("MyTDSpi::OnRspQrySettlementInfoConfirm  结算信息未确认  ConfirmDate=%s  \n",ConfirmDate);
								 sprintf(buf,"MyTDSpi::OnRspQrySettlementInfoConfirm  结算信息未确认   ConfirmDate=%s\n",ConfirmDate);
								 logutil.WriteLog(buf);

								if (strcmp(last.TradingDay,ConfirmDate)==0){
									DateUtil dateutil;
									double Q_BarTime_2 = dateutil.ConvertMinuteTimetoBarTime(ConfirmTime);
									bool Timemore_1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.1500);
									bool Timemore_2 = (Q_BarTime_2>=0.2000 && Q_BarTime_2<0.2359);
									if(Timemore_1||Timemore_2){
										printf("MyTDSpi::OnRspQrySettlementInfoConfirm ConfirmDate=%s   ---> 确认成功，更新状态 \n",ConfirmDate);
										sprintf(buf,"MyTDSpi::OnRspQrySettlementInfoConfirm  ConfirmDate=%s --> 确认成功，更新状态  \n",ConfirmDate);
										logutil.WriteLog(buf);

										last.SettleInfoConfirmStatus =true;
										strcpy(last.ConfirmDate,ConfirmDate);
										strcpy(last.ConfirmTime,ConfirmTime);
										mysqltradingday.UpdateSettleInfoConfirmStatusbyDate(&last);
									}
									else{
										printf("MyTDSpi::OnRspQrySettlementInfoConfirm ConfirmDate=%s  ConfirmTime:%s ---> 非结算时间!!! \n",
												ConfirmDate,ConfirmTime);
										sprintf(buf,"MyTDSpi::OnRspQrySettlementInfoConfirm  ConfirmDate=%s -ConfirmTime:%s --> 非结算时间!!!  \n",
												ConfirmDate,ConfirmTime);
										logutil.WriteLog(buf);
									}

								}
							}
							else{
								if (strcmp(last.TradingDay,ConfirmDate)!=0){

									printf("MyTDSpi::OnRspQrySettlementInfoConfirm ConfirmDate=%s   ---> 结算单未确认，更新状态 \n",ConfirmDate);
									sprintf(buf,"MyTDSpi::OnRspQrySettlementInfoConfirm  ConfirmDate=%s --> 结算单未确认，更新状态  \n",ConfirmDate);
									logutil.WriteLog(buf);

									last.SettleInfoConfirmStatus =false;
	//								strcpy(last.ConfirmDate,ConfirmDate);
									mysqltradingday.ResetSettleInfoConfirmStatusbyDate(last.SettleInfoConfirmStatus,last.TradingDay);
								}
							}
						}


					}


                  }

                  // 如果响应函数已经返回最后一条信息
                  if(bIsLast) {
                      // 通知主过程，响应函数将结束

                 	 this->PostSemQrySettleInfoConfirm();
                      logutil.WriteLog("OnRspQrySettlementInfoConfirm():被执行完成...");
                  }
              }

	//*/
	///请求查询投资者结算结果响应
	//*/

   	void MyTDSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo,
   			CThostFtdcRspInfoField *pRspInfo,
			int nRequestID,
   			bool bIsLast)

         {
         	char buf[1050];
             printf("OnRspQrySettlementInfo():被执行...\n");
             LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
     		logutil.WriteLog("OnRspQrySettlementInfo():被执行...");

             // 进程返回结果检查
             if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
                 // typedef int TThostFtdcErrorIDType;
                 // typedef char TThostFtdcErrorMsgType[81];
                 char ErrorMsg[243];
                 gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
                 printf("OnRspQrySettlementInfo():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

                 sprintf(buf,"OnRspQrySettlementInfo():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
                 logutil.WriteLog(buf);
             }

             // 如果有返回结果读取返回信息
             if ( pSettlementInfo != NULL && this->GetFlagLogin() && this->GetFlagFrontConnectFalg()) {
                 // 读取返回信息,并做编码转化
                 ///经纪公司代码 TThostFtdcBrokerIDType char[11]
                 char BrokerID[33];
                 gbk2utf8(pSettlementInfo->BrokerID,BrokerID,sizeof(BrokerID));
                 ///投资者代码 TThostFtdcInvestorIDType char[13]
                 char InvestorID[39];
                 gbk2utf8(pSettlementInfo->InvestorID,InvestorID,sizeof(InvestorID));
                 ///确认日期 TThostFtdcDateType char[9]
                 char TradingDay[27];
                 gbk2utf8(pSettlementInfo->TradingDay,TradingDay,sizeof(TradingDay));
                 ///确认时间 TThostFtdcTimeType char[9]
                 char Content[1024];
                 gbk2utf8(pSettlementInfo->Content,Content,sizeof(Content));

                 if(Content != NULL )
                 {
//                	 sprintf(buf,"结算信息查询操作:TradingDay=%s,Content=%s\n",TradingDay,Content);
                	 sprintf(buf,"%s\n",Content);
					logutil.WriteLog(buf);
                 }

             }

             // 如果响应函数已经返回最后一条信息
             if(bIsLast) {
                 // 通知主过程，响应函数将结束

            	 this->PostSemQrySettleInfo();


                 logutil.WriteLog("OnRspQrySettlementInfo():被执行完成...");
             }
         }
    //*/
    ///投资者结算结果确认响应
    //*/
    void MyTDSpi::OnRspSettlementInfoConfirm(
        CThostFtdcSettlementInfoConfirmField * pSettlementInfoConfirm,
        CThostFtdcRspInfoField * pRspInfo,
        int nRequestID,
        bool bIsLast
    )
    {
    	char buf[1024];
        printf("OnRspSettlementInfoConfirm():被执行...\n");
        LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		logutil.WriteLog("OnRspSettlementInfoConfirm():被执行...");

        // 进程返回结果检查
        if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
            // typedef int TThostFtdcErrorIDType;
            // typedef char TThostFtdcErrorMsgType[81];
            char ErrorMsg[243];
            gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
            printf("OnRspSettlementInfoConfirm():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

            sprintf(buf,"OnRspSettlementInfoConfirm():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
            logutil.WriteLog(buf);
        }

        // 如果有返回结果读取返回信息
        if ( pSettlementInfoConfirm != NULL && this->GetFlagLogin() && this->GetFlagFrontConnectFalg()) {
            // 读取返回信息,并做编码转化
            ///经纪公司代码 TThostFtdcBrokerIDType char[11]
            char BrokerID[33];
            gbk2utf8(pSettlementInfoConfirm->BrokerID,BrokerID,sizeof(BrokerID));
            ///投资者代码 TThostFtdcInvestorIDType char[13]
            char InvestorID[39];
            gbk2utf8(pSettlementInfoConfirm->InvestorID,InvestorID,sizeof(InvestorID));
            ///确认日期 TThostFtdcDateType char[9]
            char ConfirmDate[27];
            gbk2utf8(pSettlementInfoConfirm->ConfirmDate,ConfirmDate,sizeof(ConfirmDate));
            ///确认时间 TThostFtdcTimeType char[9]
            char ConfirmTime[27];
            gbk2utf8(pSettlementInfoConfirm->ConfirmTime,ConfirmTime,sizeof(ConfirmTime));

			printf("结算信息操作确认:ConfirmDate=%s,ConfirmTime=%s\n",ConfirmDate,ConfirmTime);

			sprintf(buf,"结算信息操作确认:ConfirmDate=%s,ConfirmTime=%s\n",ConfirmDate,ConfirmTime);
			logutil.WriteLog(buf);

			 //同时更新交易日数据库 不可删除
			MysqlTradingDay mysqltradingday(pSettlementInfoConfirm->InvestorID);

			if(mysqltradingday.Exist_Table()){

				if(mysqltradingday.Exist_TradingDay(this->GetTradingDay().c_str())){

					TradingDayData last;
					last= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());

					if (!last.SettleInfoConfirmStatus){

						 printf("MyTDSpi::OnRspSettlementInfoConfirm  结算信息未确认  ConfirmDate=%s  \n",ConfirmDate);
						 sprintf(buf,"MyTDSpi::OnRspSettlementInfoConfirm  结算信息未确认   ConfirmDate=%s\n",ConfirmDate);
						 logutil.WriteLog(buf);

						if (strcmp(last.TradingDay,ConfirmDate)==0){

							DateUtil dateutil;
							double Q_BarTime_2 = dateutil.ConvertMinuteTimetoBarTime(ConfirmTime);
							bool Timemore_1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<=0.1500);
							bool Timemore_2 = (Q_BarTime_2>0.2000 && Q_BarTime_2<=0.2359);
							if(Timemore_1||Timemore_2){
								printf("MyTDSpi::OnRspSettlementInfoConfirm ConfirmDate=%s  ConfirmTime:%s ---> 确认成功，更新状态 \n",
										ConfirmDate,ConfirmTime);
								sprintf(buf,"MyTDSpi::OnRspSettlementInfoConfirm  ConfirmDate=%s ConfirmTime:%s--> 确认成功，更新状态  \n",
										ConfirmDate,ConfirmTime);
								logutil.WriteLog(buf);

								last.SettleInfoConfirmStatus =true;
								strcpy(last.ConfirmDate,ConfirmDate);
								strcpy(last.ConfirmTime,ConfirmTime);
								mysqltradingday.UpdateSettleInfoConfirmStatusbyDate(&last);
							}
							else{
								printf("MyTDSpi::OnRspSettlementInfoConfirm ConfirmDate=%s  ConfirmTime:%s ---> 非结算时间!!! \n",
										ConfirmDate,ConfirmTime);
								sprintf(buf,"MyTDSpi::OnRspSettlementInfoConfirm  ConfirmDate=%s ConfirmTime:%s --> 非结算时间!!!  \n",
										ConfirmDate,ConfirmTime);
								logutil.WriteLog(buf);
							}
						}
						else{
							printf("MyTDSpi::OnRspSettlementInfoConfirm ConfirmDate=%s   last.TradingDay=%s---> 结算信息确认失败 \n",
									ConfirmDate,last.TradingDay);
							sprintf(buf,"MyTDSpi::OnRspSettlementInfoConfirm ConfirmDate=%s   last.TradingDay=%s---> 结算信息确认失败 \n",
								ConfirmDate,last.TradingDay);
							logutil.WriteLog(buf);
						}
					}
				}
			}
        }

        // 如果响应函数已经返回最后一条信息
        if(bIsLast) {
            // 通知主过程，响应函数将结束
            this->PostSemSettleInfoConfirm();
        }
    }


	// 报单录入应答
    //*
	void MyTDSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

		logutil.WriteLog("OnRspOrderInsert():被执行...");
        printf("OnRspOrderInsert():被执行...\n");
        OrderData info;

        // 输出报单录入结果
		  if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {

			  char ErrorMsg[243];
			  gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
			  printf("OnRspOrderInsert():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
			  sprintf(buf,"OnRspOrderInsert():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
			  logutil.WriteLog(buf);
			  info.ErrorId = pRspInfo->ErrorID;
			  info.Status =-1;

			  if(pRspInfo->ErrorID ==42) {
				  sprintf(buf,"ERROR: CTP:结算结果未确认   ----> 重置结算标志，重新确认结算  UserID %s",pInputOrder->UserID);
				  logutil.WriteLog(buf);
				  sprintf(buf,"ERROR: CTP:结算结果未确认   ----> 重置结算标志，重新确认结算  InvestorID %s",pInputOrder->InvestorID);
				  logutil.WriteLog(buf);


				 //同时更新交易日数据库 不可删除
					MysqlTradingDay mysqltradingday(pInputOrder->UserID);

					if(mysqltradingday.Exist_Table()){

						if(mysqltradingday.Exist_TradingDay(this->GetTradingDay().c_str()))	{
							TradingDayData last;
							last= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());
							if (last.SettleInfoConfirmStatus){

									printf("MyTDSpi::OnRspOrderInsert:ERROR  TradingDay=%s   ---> 未结算，重新结算确认状态 \n",
											this->GetTradingDay().c_str());
									sprintf(buf,"MyTDSpi::OnRspOrderInsert:ERROR  TradingDay=%s --> 未结算，重新结算确认状态 \n",
											this->GetTradingDay().c_str());
									logutil.WriteLog(buf);

									last.SettleInfoConfirmStatus =false;
									mysqltradingday.ResetSettleInfoConfirmStatusbyDate(last.SettleInfoConfirmStatus,this->GetTradingDay().c_str());

							}
						}


					}

			  }
		  }


              // 如果有返回结果读取返回信息
		 if ( pInputOrder != NULL && this->GetFlagLogin()) {
			 // 读取返回信息,并做编码转化
			 ///经纪公司代码 TThostFtdcBrokerIDType char[11]
			 char BrokerID[33];
			 gbk2utf8(pInputOrder->BrokerID,BrokerID,sizeof(BrokerID));
			 ///投资者代码 TThostFtdcInvestorIDType char[13]
			 char InvestorID[39];
			 gbk2utf8(pInputOrder->InvestorID,InvestorID,sizeof(InvestorID));
			 ///合约代码 TThostFtdcInstrumentIDType char[31]
			 char InstrumentID[93];
			 gbk2utf8(pInputOrder->InstrumentID,InstrumentID,sizeof(InstrumentID));
			 ///报单引用 TThostFtdcOrderRefType char[13]
			 char OrderRef[39];
			 gbk2utf8(pInputOrder->OrderRef,OrderRef,sizeof(OrderRef));
			 ///用户代码 TThostFtdcUserIDType char[16]
			 char UserID[48];
			 gbk2utf8(pInputOrder->UserID,UserID,sizeof(UserID));
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
			 char OrderPriceType = pInputOrder->OrderPriceType;
			 ///买卖方向 TThostFtdcDirectionType char
			 //// THOST_FTDC_D_Buy '0' 买
			 //// THOST_FTDC_D_Sell '1' 卖
			 char Direction = pInputOrder->Direction;
			 ///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
			 //// THOST_FTDC_OF_Open '0' 开仓
			 //// THOST_FTDC_OF_Close '1' 平仓
			 //// THOST_FTDC_OF_ForceClose '2' 强平
			 //// THOST_FTDC_OF_CloseToday '3' 平今
			 //// THOST_FTDC_OF_CloseYesterday '4' 平昨
			 //// THOST_FTDC_OF_ForceOff '5' 强减
			 //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
			 char CombOffsetFlag[15];
			 gbk2utf8(pInputOrder->CombOffsetFlag,CombOffsetFlag,sizeof(CombOffsetFlag));
			 ///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
			 //// THOST_FTDC_HF_Speculation '1' 投机
			 //// THOST_FTDC_HF_Arbitrage '2' 套利
			 //// THOST_FTDC_HF_Hedge '3' 套保
			 char CombHedgeFlag[15];
			 gbk2utf8(pInputOrder->CombHedgeFlag,CombHedgeFlag,sizeof(CombHedgeFlag));
			 ///价格 TThostFtdcPriceType double
			 double LimitPrice = pInputOrder->LimitPrice;
			 ///数量 TThostFtdcVolumeType int
			 int VolumeTotalOriginal = pInputOrder->VolumeTotalOriginal;
			 ///有效期类型 TThostFtdcTimeConditionType char
			 //// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
			 //// THOST_FTDC_TC_GFS '2' 本节有效
			 //// THOST_FTDC_TC_GFD '3' 当日有效
			 //// THOST_FTDC_TC_GTD '4' 指定日期前有效
			 //// THOST_FTDC_TC_GTC '5' 撤销前有效
			 //// THOST_FTDC_TC_GFA '6' 集合竞价有效
			 char TimeCondition = pInputOrder->TimeCondition;
			 ///GTD日期 TThostFtdcDateType char[9]
			 char GTDDate[27];
			 gbk2utf8(pInputOrder->GTDDate,GTDDate,sizeof(GTDDate));
			 ///成交量类型 TThostFtdcVolumeConditionType char
			 //// THOST_FTDC_VC_AV '1' 任何数量
			 //// THOST_FTDC_VC_MV '2' 最小数量
			 //// THOST_FTDC_VC_CV '3' 全部数量
			 char VolumeCondition = pInputOrder->VolumeCondition;
			 ///最小成交量 TThostFtdcVolumeType int
			 int MinVolume = pInputOrder->MinVolume;
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
			 char ContingentCondition = pInputOrder->ContingentCondition;
			 ///止损价 TThostFtdcPriceType double
			 double StopPrice = pInputOrder->StopPrice;
			 ///强平原因 TThostFtdcForceCloseReasonType char
			 //// THOST_FTDC_FCC_NotForceClose '0' 非强平
			 //// THOST_FTDC_FCC_LackDeposit '1' 资金不足
			 //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
			 //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
			 //// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
			 //// THOST_FTDC_FCC_Violation '5' 违规
			 //// THOST_FTDC_FCC_Other '6' 其它
			 //// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
			 char ForceCloseReason = pInputOrder->ForceCloseReason;
			 ///自动挂起标志 TThostFtdcBoolType int
			 int IsAutoSuspend = pInputOrder->IsAutoSuspend;
			 ///业务单元 TThostFtdcBusinessUnitType char[21]
			 char BusinessUnit[63];
			 gbk2utf8(pInputOrder->BusinessUnit,BusinessUnit,sizeof(BusinessUnit));
			 ///请求编号 TThostFtdcRequestIDType int
			 int RequestID = pInputOrder->RequestID;
			 ///用户强评标志 TThostFtdcBoolType int
			 int UserForceClose = pInputOrder->UserForceClose;
			 ///互换单标志 TThostFtdcBoolType int
			 int IsSwapOrder = pInputOrder->IsSwapOrder;

			 printf("Debug: BrokerID= %s  \n",pInputOrder->BrokerID);  //
			 printf("Debug: UserID= %s  \n",pInputOrder->UserID);  //
			printf("Debug: AccountID= %s  \n",pInputOrder->AccountID);  //
			printf("Debug: BusinessUnit= %s  \n",pInputOrder->BusinessUnit);  //
			printf("Debug: CombHedgeFlag= %s  \n",pInputOrder->CombHedgeFlag);  //
			printf("Debug: CombOffsetFlag= %s  \n",pInputOrder->CombOffsetFlag);  //
			printf("Debug: IPAddress= %s  \n",pInputOrder->IPAddress);  //
			printf("Debug: InstrumentID= %s  \n",pInputOrder->InstrumentID);  //
			printf("Debug: LimitPrice= %f  \n",pInputOrder->LimitPrice);  //
			printf("Debug: OrderRef= %s  \n",pInputOrder->OrderRef);  //


			char buf[1024];
			sprintf(buf,"Debug: InstrumentID= %s  ",pInputOrder->InstrumentID);
			logutil.WriteLog(buf);
			sprintf(buf,"Debug: LimitPrice= %f  ",pInputOrder->LimitPrice);
			logutil.WriteLog(buf);
			sprintf(buf,"Debug: Direction= %d  ",pInputOrder->Direction);
			logutil.WriteLog(buf);
			sprintf(buf,"Debug: VolumeTotalOriginal= %d  ",pInputOrder->VolumeTotalOriginal);
			logutil.WriteLog(buf);

			strcpy(info.InstrumentID,pInputOrder->InstrumentID);

			strcpy(info.TradingDay,this->GetTradingDay().c_str());
			strcpy(info.OrderRef,pInputOrder->OrderRef);
			strcpy(info.ExchangeID,pInputOrder->ExchangeID);


			if(pInputOrder->CombOffsetFlag[0]=='0' )	{
				info.Status =-1;
				 Config config(this->GetStrategyName().c_str());
				MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
				mysqlorder.UpdateOrderData_byOnRspOrderInsert(info);

			}
			else if (pInputOrder->CombOffsetFlag[0]>='1' && pInputOrder->CombOffsetFlag[0] <='4')	{

				info.Status =-1;
				 Config config(this->GetStrategyName().c_str());
				MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
				mysqlorder.UpdateOrderData_byOnRspOrderInsert(info);
			}


		 }

  // 如果响应函数已经返回最后一条信息
        if(bIsLast) {
            // 通知主过程，响应函数将结束

//            sem_post(&sem_orderinsert);
        }

    };



	 ///报单操作请求响应
	void MyTDSpi::OnRspOrderAction(
		CThostFtdcInputOrderActionField * pInputOrderAction,
		CThostFtdcRspInfoField * pRspInfo,
		int nRequestID,
		bool bIsLast
	) {

		printf("OnRspOrderAction():被执行...\n");
		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		logutil.WriteLog("OnRspOrderAction():被执行...");

		// 进程返回结果检查
		if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
			// typedef int TThostFtdcErrorIDType;
			// typedef char TThostFtdcErrorMsgType[81];
			char ErrorMsg[243];
			gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
			printf("OnRspOrderAction():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
			sprintf(buf,"OnRspOrderAction():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
			logutil.WriteLog(buf);
		}

		// 如果有返回结果读取返回信息
		if ( pInputOrderAction != NULL && this->GetFlagLogin()) {
			// 读取返回信息,并做编码转化
			///经纪公司代码 TThostFtdcBrokerIDType char[11]
			char BrokerID[33];
			gbk2utf8(pInputOrderAction->BrokerID,BrokerID,sizeof(BrokerID));
			///投资者代码 TThostFtdcInvestorIDType char[13]
			char InvestorID[39];
			gbk2utf8(pInputOrderAction->InvestorID,InvestorID,sizeof(InvestorID));
			///报单操作引用 TThostFtdcOrderActionRefType int
			int OrderActionRef = pInputOrderAction->OrderActionRef;
			///报单引用 TThostFtdcOrderRefType char[13]
			char OrderRef[39];
			gbk2utf8(pInputOrderAction->OrderRef,OrderRef,sizeof(OrderRef));
			///请求编号 TThostFtdcRequestIDType int
			int RequestID = pInputOrderAction->RequestID;
			///前置编号 TThostFtdcFrontIDType int
			int FrontID = pInputOrderAction->FrontID;
			///会话编号 TThostFtdcSessionIDType int
			int SessionID = pInputOrderAction->SessionID;
			///交易所代码 TThostFtdcExchangeIDType char[9]
			char ExchangeID[27];
			gbk2utf8(pInputOrderAction->ExchangeID,ExchangeID,sizeof(ExchangeID));
			///报单编号 TThostFtdcOrderSysIDType char[21]
			char OrderSysID[63];
			gbk2utf8(pInputOrderAction->OrderSysID,OrderSysID,sizeof(OrderSysID));
			///操作标志 TThostFtdcActionFlagType char
			//// THOST_FTDC_AF_Delete '0' 删除
			//// THOST_FTDC_AF_Modify '3' 修改
			char ActionFlag = pInputOrderAction->ActionFlag;
			///价格 TThostFtdcPriceType double
			double LimitPrice = pInputOrderAction->LimitPrice;
			///数量变化 TThostFtdcVolumeType int
			int VolumeChange = pInputOrderAction->VolumeChange;
			///用户代码 TThostFtdcUserIDType char[16]
			char UserID[48];
			gbk2utf8(pInputOrderAction->UserID,UserID,sizeof(UserID));
			///合约代码 TThostFtdcInstrumentIDType char[31]
			char InstrumentID[93];
			gbk2utf8(pInputOrderAction->InstrumentID,InstrumentID,sizeof(InstrumentID));

		}

		// 如果响应函数已经返回最后一条信息
		if(bIsLast) {
			// 通知主过程，响应函数将结束

			this->PostSemOrderAction();
		}
	}
/////报单通知
void MyTDSpi::OnRtnOrder(CThostFtdcOrderField * pOrder) {

	char buf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	logutil.WriteLog("\n");
//		logutil.WriteLog("\nOnRtnOrder():被执行-------------------------------> OrderSysID=[%s]    ", pOrder->OrderSysID);
	sprintf(buf,"OnRtnOrder():被执行----------> OrderSysID=[%s]    ", pOrder->OrderSysID);
	logutil.WriteLog(buf);

	// 如果有返回结果读取返回信息
	if ( pOrder != NULL && this->GetFlagLogin()) {

		char TradingDay[27];
		gbk2utf8(pOrder->TradingDay,TradingDay,sizeof(TradingDay));

		sprintf(buf,"Debug: InstrumentID= %s  OrderRef= %s  TradingDay= %s  OrderSubmitStatus= %d  OrderStatus= %d  ",
				pOrder->InstrumentID,pOrder->OrderRef,pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
		logutil.WriteLog(buf);
		sprintf(buf,"Debug: InsertDate= %s  InsertTime= %s Direction= %d   VolumeTotalOriginal= %d   CombOffsetFlag[0]= %d",
				pOrder->InsertDate,pOrder->InsertTime,pOrder->Direction-'0',pOrder->VolumeTotalOriginal,pOrder->CombOffsetFlag[0]-'0');  //
		logutil.WriteLog(buf);

		OrderData info;
		strcpy(info.InstrumentID,pOrder->InstrumentID);
		strcpy(info.TradingDay,pOrder->TradingDay);
		strcpy(info.OrderRef,pOrder->OrderRef);
		strcpy(info.ExchangeID,pOrder->ExchangeID);
		info.Direction = pOrder->Direction-'0';
		info.CombOffsetFlag = pOrder->CombOffsetFlag[0]-'0';
		info.OrderSubmitStatus = pOrder->OrderSubmitStatus-'0';
		info.OrderStatus = pOrder->OrderStatus-'0';
		strcpy(info.OrderSysID,pOrder->OrderSysID);
		info.FrontID = pOrder->FrontID;
		info.ErrorId=0;
		info.SessionID = pOrder->SessionID;
		info.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
		info.Volume = pOrder->VolumeTraded;

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(pOrder->InstrumentID).c_str());


		//数据校验 exchangeid
		ProductData productdata;
		MysqlProduct mysqlproduct;
		productdata=mysqlproduct.Find_DatabyProductID(ProductID);
		if(!strcmp(productdata.ExchangeID,pOrder->ExchangeID)==0){
			sprintf(buf,">>> MyTDSpi::OnRtnOrder 返回数据Error   本次响应不作任何数据库操作!!!   合约：%s   ExchangeID:%s    ----> 后台返回  ExchangeID:%s  ",
					pOrder->InstrumentID,productdata.ExchangeID,pOrder->ExchangeID);  //
			logutil.WriteLog(buf);
			return;
		}
		else{
			sprintf(buf,"MyTDSpi::OnRtnOrder ExchangeID校验正确!!!   合约：%s   ExchangeID:%s    ----> 后台返回  ExchangeID:%s  ",
									pOrder->InstrumentID,productdata.ExchangeID,pOrder->ExchangeID);  //
			logutil.WriteLog(buf);
		}

		std::shared_ptr<OrderData>e = std::make_shared<OrderData>();

		strcpy(e->InstrumentID,pOrder->InstrumentID);
		strcpy(e->TradingDay,pOrder->TradingDay);
		strcpy(e->OrderRef,pOrder->OrderRef);
		strcpy(e->ExchangeID,pOrder->ExchangeID);
		e->Direction = pOrder->Direction-'0';
		e->CombOffsetFlag = pOrder->CombOffsetFlag[0]-'0';
		e->OrderSubmitStatus = pOrder->OrderSubmitStatus-'0';
		e->OrderStatus = pOrder->OrderStatus-'0';
		strcpy(e->OrderSysID,pOrder->OrderSysID);
		e->FrontID = pOrder->FrontID;
		e->SessionID = pOrder->SessionID;
		e->ErrorId=0;
		e->VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
		e->Volume = pOrder->VolumeTraded;

		std::string key=info.OrderRef;

		 //维护订单状态
		if (e->OrderStatus == '5' || e->OrderStatus == '0')	 	{
			std::unique_lock<std::mutex>lck(m_openordermutex);
			if (m_openordermap.find(key) != m_openordermap.end())	{
				m_openordermap.erase(key);
				sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  --->删除order",e->InstrumentID,e->OrderStatus);  //
				logutil.WriteLog(buf);
			}
		}
		else 	{
			std::unique_lock<std::mutex>lck(m_openordermutex);
			if (m_openordermap.find(key) != m_openordermap.end()) 		{
				m_openordermap[key] = e;
				sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  --->插入order",e->InstrumentID,e->OrderStatus);  //
				logutil.WriteLog(buf);
			}
			else	{
				m_openordermap.insert(std::pair<std::string, std::shared_ptr<OrderData>>(key, e));
				sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  --->更新order",e->InstrumentID,e->OrderStatus);  //
				logutil.WriteLog(buf);
			}
		}

	}

}
/////报单通知
//    void MyTDSpi::OnRtnOrder(
//        CThostFtdcOrderField * pOrder
//    ) {
//        printf("OnRtnOrder():被执行...\n");
//		printf("OrderSysID=[%s]\n", pOrder->OrderSysID);
//
//		char buf[1024];
//		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
//		logutil.WriteLog("\n");
////		logutil.WriteLog("\nOnRtnOrder():被执行-------------------------------> OrderSysID=[%s]    ", pOrder->OrderSysID);
//		sprintf(buf,"OnRtnOrder():被执行----------> OrderSysID=[%s]    ", pOrder->OrderSysID);
//		logutil.WriteLog(buf);
//
//        // 如果有返回结果读取返回信息
//        if ( pOrder != NULL && this->GetFlagLogin()) {
//
//        	printf("OnRtnOrder():被执行111...\n");
//            // 读取返回信息,并做编码转化
//            ///经纪公司代码 TThostFtdcBrokerIDType char[11]
////            char BrokerID[33];
////            gbk2utf8(pOrder->BrokerID,BrokerID,sizeof(BrokerID));
////            ///投资者代码 TThostFtdcInvestorIDType char[13]
////            char InvestorID[39];
////            gbk2utf8(pOrder->InvestorID,InvestorID,sizeof(InvestorID));
////            ///合约代码 TThostFtdcInstrumentIDType char[31]
////            char InstrumentID[93];
////            gbk2utf8(pOrder->InstrumentID,InstrumentID,sizeof(InstrumentID));
////            ///报单引用 TThostFtdcOrderRefType char[13]
////            char OrderRef[39];
////            gbk2utf8(pOrder->OrderRef,OrderRef,sizeof(OrderRef));
////            ///用户代码 TThostFtdcUserIDType char[16]
////            char UserID[48];
////            gbk2utf8(pOrder->UserID,UserID,sizeof(UserID));
////            ///报单价格条件 TThostFtdcOrderPriceTypeType char
////            //// THOST_FTDC_OPT_AnyPrice '1' 任意价
////            //// THOST_FTDC_OPT_LimitPrice '2' 限价
////            //// THOST_FTDC_OPT_BestPrice '3' 最优价
////            //// THOST_FTDC_OPT_LastPrice '4' 最新价
////            //// THOST_FTDC_OPT_LastPricePlusOneTicks '5' 最新价浮动上浮1个ticks
////            //// THOST_FTDC_OPT_LastPricePlusTwoTicks '6' 最新价浮动上浮2个ticks
////            //// THOST_FTDC_OPT_LastPricePlusThreeTicks '7' 最新价浮动上浮3个ticks
////            //// THOST_FTDC_OPT_AskPrice1 '8' 卖一价
////            //// THOST_FTDC_OPT_AskPrice1PlusOneTicks '9' 卖一价浮动上浮1个ticks
////            //// THOST_FTDC_OPT_AskPrice1PlusTwoTicks 'A' 卖一价浮动上浮2个ticks
////            //// THOST_FTDC_OPT_AskPrice1PlusThreeTicks 'B' 卖一价浮动上浮3个ticks
////            //// THOST_FTDC_OPT_BidPrice1 'C' 买一价
////            //// THOST_FTDC_OPT_BidPrice1PlusOneTicks 'D' 买一价浮动上浮1个ticks
////            //// THOST_FTDC_OPT_BidPrice1PlusTwoTicks 'E' 买一价浮动上浮2个ticks
////            //// THOST_FTDC_OPT_BidPrice1PlusThreeTicks 'F' 买一价浮动上浮3个ticks
////            char OrderPriceType = pOrder->OrderPriceType;
////            ///买卖方向 TThostFtdcDirectionType char
////            //// THOST_FTDC_D_Buy '0' 买
////            //// THOST_FTDC_D_Sell '1' 卖
////            char Direction = pOrder->Direction;
////            ///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
////            //// THOST_FTDC_OF_Open '0' 开仓
////            //// THOST_FTDC_OF_Close '1' 平仓
////            //// THOST_FTDC_OF_ForceClose '2' 强平
////            //// THOST_FTDC_OF_CloseToday '3' 平今
////            //// THOST_FTDC_OF_CloseYesterday '4' 平昨
////            //// THOST_FTDC_OF_ForceOff '5' 强减
////            //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
////            char CombOffsetFlag[15];
////            gbk2utf8(pOrder->CombOffsetFlag,CombOffsetFlag,sizeof(CombOffsetFlag));
////            ///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
////            //// THOST_FTDC_HF_Speculation '1' 投机
////            //// THOST_FTDC_HF_Arbitrage '2' 套利
////            //// THOST_FTDC_HF_Hedge '3' 套保
////            char CombHedgeFlag[15];
////            gbk2utf8(pOrder->CombHedgeFlag,CombHedgeFlag,sizeof(CombHedgeFlag));
////            ///价格 TThostFtdcPriceType double
////            double LimitPrice = pOrder->LimitPrice;
////            ///数量 TThostFtdcVolumeType int
////            int VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
////            ///有效期类型 TThostFtdcTimeConditionType char
////            //// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
////            //// THOST_FTDC_TC_GFS '2' 本节有效
////            //// THOST_FTDC_TC_GFD '3' 当日有效
////            //// THOST_FTDC_TC_GTD '4' 指定日期前有效
////            //// THOST_FTDC_TC_GTC '5' 撤销前有效
////            //// THOST_FTDC_TC_GFA '6' 集合竞价有效
////            char TimeCondition = pOrder->TimeCondition;
////            ///GTD日期 TThostFtdcDateType char[9]
////            char GTDDate[27];
////            gbk2utf8(pOrder->GTDDate,GTDDate,sizeof(GTDDate));
////            ///成交量类型 TThostFtdcVolumeConditionType char
////            //// THOST_FTDC_VC_AV '1' 任何数量
////            //// THOST_FTDC_VC_MV '2' 最小数量
////            //// THOST_FTDC_VC_CV '3' 全部数量
////            char VolumeCondition = pOrder->VolumeCondition;
////            ///最小成交量 TThostFtdcVolumeType int
////            int MinVolume = pOrder->MinVolume;
////            ///触发条件 TThostFtdcContingentConditionType char
////            //// THOST_FTDC_CC_Immediately '1' 立即
////            //// THOST_FTDC_CC_Touch '2' 止损
////            //// THOST_FTDC_CC_TouchProfit '3' 止赢
////            //// THOST_FTDC_CC_ParkedOrder '4' 预埋单
////            //// THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5' 最新价大于条件价
////            //// THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6' 最新价大于等于条件价
////            //// THOST_FTDC_CC_LastPriceLesserThanStopPrice '7' 最新价小于条件价
////            //// THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8' 最新价小于等于条件价
////            //// THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9' 卖一价大于条件价
////            //// THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A' 卖一价大于等于条件价
////            //// THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B' 卖一价小于条件价
////            //// THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C' 卖一价小于等于条件价
////            //// THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D' 买一价大于条件价
////            //// THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E' 买一价大于等于条件价
////            //// THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F' 买一价小于条件价
////            //// THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H' 买一价小于等于条件价
////            char ContingentCondition = pOrder->ContingentCondition;
////            ///止损价 TThostFtdcPriceType double
////            double StopPrice = pOrder->StopPrice;
////            ///强平原因 TThostFtdcForceCloseReasonType char
////            //// THOST_FTDC_FCC_NotForceClose '0' 非强平
////            //// THOST_FTDC_FCC_LackDeposit '1' 资金不足
////            //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
////            //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
////            //// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
////            //// THOST_FTDC_FCC_Violation '5' 违规
////            //// THOST_FTDC_FCC_Other '6' 其它
////            //// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
////            char ForceCloseReason = pOrder->ForceCloseReason;
////            ///自动挂起标志 TThostFtdcBoolType int
////            int IsAutoSuspend = pOrder->IsAutoSuspend;
////            ///业务单元 TThostFtdcBusinessUnitType char[21]
////            char BusinessUnit[63];
////            gbk2utf8(pOrder->BusinessUnit,BusinessUnit,sizeof(BusinessUnit));
////            ///请求编号 TThostFtdcRequestIDType int
////            int RequestID = pOrder->RequestID;
////            ///本地报单编号 TThostFtdcOrderLocalIDType char[13]
////            char OrderLocalID[39];
////            gbk2utf8(pOrder->OrderLocalID,OrderLocalID,sizeof(OrderLocalID));
////            ///交易所代码 TThostFtdcExchangeIDType char[9]
////            char ExchangeID[30];
////            gbk2utf8(pOrder->ExchangeID,ExchangeID,sizeof(ExchangeID));
////            ///会员代码 TThostFtdcParticipantIDType char[11]
////            char ParticipantID[33];
////            gbk2utf8(pOrder->ParticipantID,ParticipantID,sizeof(ParticipantID));
////            ///客户代码 TThostFtdcClientIDType char[11]
////            char ClientID[33];
////            gbk2utf8(pOrder->ClientID,ClientID,sizeof(ClientID));
////            ///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
////            char ExchangeInstID[93];
////            gbk2utf8(pOrder->ExchangeInstID,ExchangeInstID,sizeof(ExchangeInstID));
////            ///交易所交易员代码 TThostFtdcTraderIDType char[21]
////            char TraderID[63];
////            gbk2utf8(pOrder->TraderID,TraderID,sizeof(TraderID));
////            ///安装编号 TThostFtdcInstallIDType int
////            int InstallID = pOrder->InstallID;
////            ///报单提交状态 TThostFtdcOrderSubmitStatusType char
////            //// THOST_FTDC_OSS_InsertSubmitted '0' 已经提交
////            //// THOST_FTDC_OSS_CancelSubmitted '1' 撤单已经提交
////            //// THOST_FTDC_OSS_ModifySubmitted '2' 修改已经提交
////            //// THOST_FTDC_OSS_Accepted '3' 已经接受
////            //// THOST_FTDC_OSS_InsertRejected '4' 报单已经被拒绝
////            //// THOST_FTDC_OSS_CancelRejected '5' 撤单已经被拒绝
////            //// THOST_FTDC_OSS_ModifyRejected '6' 改单已经被拒绝
////            char OrderSubmitStatus = pOrder->OrderSubmitStatus;
////            ///报单提示序号 TThostFtdcSequenceNoType int
////            int NotifySequence = pOrder->NotifySequence;
//            ///交易日 TThostFtdcDateType char[9]
//            char TradingDay[27];
//            gbk2utf8(pOrder->TradingDay,TradingDay,sizeof(TradingDay));
////            ///结算编号 TThostFtdcSettlementIDType int
////            int SettlementID = pOrder->SettlementID;
////            ///报单编号 TThostFtdcOrderSysIDType char[21]
////            char OrderSysID[63];
////            gbk2utf8(pOrder->OrderSysID,OrderSysID,sizeof(OrderSysID));
////            ///报单来源 TThostFtdcOrderSourceType char
////            //// THOST_FTDC_OSRC_Participant '0' 来自参与者
////            //// THOST_FTDC_OSRC_Administrator '1' 来自管理员
////            char OrderSource = pOrder->OrderSource;
////
////            ///报单状态 TThostFtdcOrderStatusType char
////            //// THOST_FTDC_OST_AllTraded '0' 全部成交
////            //// THOST_FTDC_OST_PartTradedQueueing '1' 部分成交还在队列中
////            //// THOST_FTDC_OST_PartTradedNotQueueing '2' 部分成交不在队列中
////            //// THOST_FTDC_OST_NoTradeQueueing '3' 未成交还在队列中
////            //// THOST_FTDC_OST_NoTradeNotQueueing '4' 未成交不在队列中
////            //// THOST_FTDC_OST_Canceled '5' 撤单
////            //// THOST_FTDC_OST_Unknown 'a' 未知
////            //// THOST_FTDC_OST_NotTouched 'b' 尚未触发
////            //// THOST_FTDC_OST_Touched 'c' 已触发
////            char OrderStatus = pOrder->OrderStatus;
////
////            ///报单类型 TThostFtdcOrderTypeType char
////            //// THOST_FTDC_ORDT_Normal '0' 正常
////            //// THOST_FTDC_ORDT_DeriveFromQuote '1' 报价衍生
////            //// THOST_FTDC_ORDT_DeriveFromCombination '2' 组合衍生
////            //// THOST_FTDC_ORDT_Combination '3' 组合报单
////            //// THOST_FTDC_ORDT_ConditionalOrder '4' 条件单
////            //// THOST_FTDC_ORDT_Swap '5' 互换单
////            char OrderType = pOrder->OrderType;
////            ///今成交数量 TThostFtdcVolumeType int
////            int VolumeTraded = pOrder->VolumeTraded;
////            ///剩余数量 TThostFtdcVolumeType int
////            int VolumeTotal = pOrder->VolumeTotal;
////            ///报单日期 TThostFtdcDateType char[9]
////            char InsertDate[27];
////            gbk2utf8(pOrder->InsertDate,InsertDate,sizeof(InsertDate));
////            ///委托时间 TThostFtdcTimeType char[9]
////            char InsertTime[27];
////            gbk2utf8(pOrder->InsertTime,InsertTime,sizeof(InsertTime));
////            ///激活时间 TThostFtdcTimeType char[9]
////            char ActiveTime[27];
////            gbk2utf8(pOrder->ActiveTime,ActiveTime,sizeof(ActiveTime));
////            ///挂起时间 TThostFtdcTimeType char[9]
////            char SuspendTime[27];
////            gbk2utf8(pOrder->SuspendTime,SuspendTime,sizeof(SuspendTime));
////            ///最后修改时间 TThostFtdcTimeType char[9]
////            char UpdateTime[27];
////            gbk2utf8(pOrder->UpdateTime,UpdateTime,sizeof(UpdateTime));
////            ///撤销时间 TThostFtdcTimeType char[9]
////            char CancelTime[27];
////            gbk2utf8(pOrder->CancelTime,CancelTime,sizeof(CancelTime));
////            ///最后修改交易所交易员代码 TThostFtdcTraderIDType char[21]
////            char ActiveTraderID[63];
////            gbk2utf8(pOrder->ActiveTraderID,ActiveTraderID,sizeof(ActiveTraderID));
////            ///结算会员编号 TThostFtdcParticipantIDType char[11]
////            char ClearingPartID[33];
////            gbk2utf8(pOrder->ClearingPartID,ClearingPartID,sizeof(ClearingPartID));
////            ///序号 TThostFtdcSequenceNoType int
////            int SequenceNo = pOrder->SequenceNo;
////            ///前置编号 TThostFtdcFrontIDType int
////            int FrontID = pOrder->FrontID;
////            ///会话编号 TThostFtdcSessionIDType int
////            int SessionID = pOrder->SessionID;
////            ///用户端产品信息 TThostFtdcProductInfoType char[11]
////            char UserProductInfo[33];
////            gbk2utf8(pOrder->UserProductInfo,UserProductInfo,sizeof(UserProductInfo));
////            ///状态信息 TThostFtdcErrorMsgType char[81]
////            char StatusMsg[243];
////            gbk2utf8(pOrder->StatusMsg,StatusMsg,sizeof(StatusMsg));
////            ///用户强评标志 TThostFtdcBoolType int
////            int UserForceClose = pOrder->UserForceClose;
////            ///操作用户代码 TThostFtdcUserIDType char[16]
////            char ActiveUserID[48];
////            gbk2utf8(pOrder->ActiveUserID,ActiveUserID,sizeof(ActiveUserID));
////            ///经纪公司报单编号 TThostFtdcSequenceNoType int
////            int BrokerOrderSeq = pOrder->BrokerOrderSeq;
////            ///相关报单 TThostFtdcOrderSysIDType char[21]
////            char RelativeOrderSysID[63];
////            gbk2utf8(pOrder->RelativeOrderSysID,RelativeOrderSysID,sizeof(RelativeOrderSysID));
////            ///郑商所成交数量 TThostFtdcVolumeType int
////            int ZCETotalTradedVolume = pOrder->ZCETotalTradedVolume;
////            ///互换单标志 TThostFtdcBoolType int
////            int IsSwapOrder = pOrder->IsSwapOrder;
//
////            printf("Debug: InstrumentID= %s  \n",pOrder->InstrumentID);  //
////            printf("Debug: OrderSubmitStatus= %d  \n",pOrder->OrderSubmitStatus-'0');  //
////            printf("Debug: OrderStatus= %d  \n",pOrder->OrderStatus-'0');  //
////            printf("Debug: InsertDate= %s  \n",pOrder->InsertDate);  //
////            printf("Debug: InsertTime= %s  \n",pOrder->InsertTime);  //
////            printf("Debug: LimitPrice= %f  \n",pOrder->LimitPrice);  //
////            printf("Debug: Direction= %d  \n",pOrder->Direction-'0');  //
////            printf("Debug: VolumeTotalOriginal= %d  \n",pOrder->VolumeTotalOriginal);  //
//
////            sprintf(buf,"TEST: TradingDay= %s  ",TradingDay);  //
////		   logutil.WriteLog(buf);
//
//
////            sprintf(buf,"Debug: InstrumentID= %s  ",pOrder->InstrumentID);  //
////            logutil.WriteLog(buf);
////            sprintf(buf,"Debug: OrderRef= %s  ",pOrder->OrderRef);  //
////			logutil.WriteLog(buf);
////			 sprintf(buf,"Debug: TradingDay= %s  ",pOrder->TradingDay);  //
////			logutil.WriteLog(buf);
////			sprintf(buf,"Debug: OrderSubmitStatus= %d  ",pOrder->OrderSubmitStatus-'0');  //
////			logutil.WriteLog(buf);
////			sprintf(buf,"Debug: OrderStatus= %d  ",pOrder->OrderStatus-'0');  //
////			logutil.WriteLog(buf);
////			sprintf(buf,"Debug: InsertDate= %s  ",pOrder->InsertDate);  //
////			logutil.WriteLog(buf);
////			sprintf(buf,"Debug: InsertTime= %s  ",pOrder->InsertTime);  //
////			logutil.WriteLog(buf);
////			sprintf(buf,"Debug: Direction= %d  ",pOrder->Direction-'0');  //
////			logutil.WriteLog(buf);
////			sprintf(buf,"Debug: VolumeTotalOriginal= %d  ",pOrder->VolumeTotalOriginal);  //
////			logutil.WriteLog(buf);
////
////			sprintf(buf,"Debug: CombOffsetFlag[0]= %d  ",pOrder->CombOffsetFlag[0]-'0');  //
////			logutil.WriteLog(buf);
//
//
//			sprintf(buf,"Debug: InstrumentID= %s  OrderRef= %s  TradingDay= %s  OrderSubmitStatus= %d  OrderStatus= %d  ",
//					pOrder->InstrumentID,pOrder->OrderRef,pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
//			logutil.WriteLog(buf);
//			sprintf(buf,"Debug: InsertDate= %s  InsertTime= %s Direction= %d   VolumeTotalOriginal= %d   CombOffsetFlag[0]= %d",
//					pOrder->InsertDate,pOrder->InsertTime,pOrder->Direction-'0',pOrder->VolumeTotalOriginal,pOrder->CombOffsetFlag[0]-'0');  //
//			logutil.WriteLog(buf);
//
//			OrderData info;
//			strcpy(info.InstrumentID,pOrder->InstrumentID);
//			strcpy(info.TradingDay,pOrder->TradingDay);
//			strcpy(info.OrderRef,pOrder->OrderRef);
//			strcpy(info.ExchangeID,pOrder->ExchangeID);
//			info.Direction = pOrder->Direction-'0';
//			info.CombOffsetFlag = pOrder->CombOffsetFlag[0]-'0';
//			info.OrderSubmitStatus = pOrder->OrderSubmitStatus-'0';
//			info.OrderStatus = pOrder->OrderStatus-'0';
//			strcpy(info.OrderSysID,pOrder->OrderSysID);
//			info.FrontID = pOrder->FrontID;
//			info.ErrorId=0;
//			info.SessionID = pOrder->SessionID;
//			info.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
//			info.Volume = pOrder->VolumeTraded;
//
//			char ProductID[31];
//			CodeUtil codeutil;
//			strcpy(ProductID,codeutil.GetProductID(pOrder->InstrumentID).c_str());
//
//			ProductData productdata;
//			MysqlProduct mysqlproduct;
//			productdata=mysqlproduct.Find_DatabyProductID(ProductID);
//			if(!strcmp(productdata.ExchangeID,pOrder->ExchangeID)==0){
//				sprintf(buf,">>> MyTDSpi::OnRtnOrder 返回数据Error   本次响应不作任何数据库操作!!!   合约：%s   ExchangeID:%s    ----> 后台返回  ExchangeID:%s  ",
//						pOrder->InstrumentID,productdata.ExchangeID,pOrder->ExchangeID);  //
//				logutil.WriteLog(buf);
//				return;
//			}
//			else{
//				sprintf(buf,"MyTDSpi::OnRtnOrder ExchangeID校验正确!!!   合约：%s   ExchangeID:%s    ----> 后台返回  ExchangeID:%s  ",
//										pOrder->InstrumentID,productdata.ExchangeID,pOrder->ExchangeID);  //
//				logutil.WriteLog(buf);
//			}
//
//		 if(info.CombOffsetFlag==0) {  //开仓
//
//			std::shared_ptr<OrderData>e = std::make_shared<OrderData>();
//
//			strcpy(e->InstrumentID,pOrder->InstrumentID);
//			strcpy(e->TradingDay,pOrder->TradingDay);
//			strcpy(e->OrderRef,pOrder->OrderRef);
//			strcpy(e->ExchangeID,pOrder->ExchangeID);
//			e->Direction = pOrder->Direction-'0';
//			e->CombOffsetFlag = pOrder->CombOffsetFlag[0]-'0';
//			e->OrderSubmitStatus = pOrder->OrderSubmitStatus-'0';
//			e->OrderStatus = pOrder->OrderStatus-'0';
//			strcpy(e->OrderSysID,pOrder->OrderSysID);
//			e->FrontID = pOrder->FrontID;
//			e->SessionID = pOrder->SessionID;
//			e->VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
//			e->Volume = pOrder->VolumeTraded;
//
//			std::string key=info.OrderRef;
//
//			 //维护订单状态
//			if (e->OrderStatus == '5' || e->OrderStatus == '0')	 	{
//				std::unique_lock<std::mutex>lck(m_openordermutex);
//				if (m_openordermap.find(key) != m_openordermap.end())	{
//					m_openordermap.erase(key);
//					sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  --->删除order",e->InstrumentID,e->OrderStatus);  //
//					logutil.WriteLog(buf);
//				}
//			}
//			else 	{
//				std::unique_lock<std::mutex>lck(m_openordermutex);
//				if (m_openordermap.find(key) != m_openordermap.end()) 		{
//					m_openordermap[key] = e;
//					sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  --->插入order",e->InstrumentID,e->OrderStatus);  //
//					logutil.WriteLog(buf);
//				}
//				else	{
//					m_openordermap.insert(std::pair<std::string, std::shared_ptr<OrderData>>(key, e));
//					sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  --->更新order",e->InstrumentID,e->OrderStatus);  //
//					logutil.WriteLog(buf);
//				}
//			}
//
//			  Config config(this->GetStrategyName().c_str());
//			MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
//			 OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,info.TradingDay);
//
//			 if (orderdata.Status<20 && orderdata.Status >= -1)	 {
//				 sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单合约状态 status= %d  ",config.GetTable_Open().c_str(),orderdata.Status);  //
//					logutil.WriteLog(buf);
//				 OpenOrder(pOrder, info, TradingDay);
//			 }
//			 else	if(orderdata.Status<50 && orderdata.Status >= 20) {
//				 sprintf(buf,"MyTDSpi::OnRtnOrder: %s 开仓单撤单合约状态 status= %d  ",config.GetTable_Open().c_str(),orderdata.Status);  //
//				logutil.WriteLog(buf);
//				OpenActionOrder(pOrder, info, TradingDay);
//			 }
//			 else{
//				 sprintf(buf,"MyTDSpi::OnRtnOrder: %s 错误的合约状态 status= %d ！！！ ",config.GetTable_Open().c_str(),orderdata.Status);  //
//				logutil.WriteLog(buf);
//			 }
//		} else if (info.CombOffsetFlag>=1 && info.CombOffsetFlag<=4)  { //平仓
//
//			  Config config(this->GetStrategyName().c_str());
//			MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
//
//			if(mysqlcloseorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,info.TradingDay)){
//				OrderData orderdata=mysqlcloseorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,info.TradingDay);
//
//				 if (orderdata.Status<20 && orderdata.Status >= -1)	{
//					 sprintf(buf,"MyTDSpi::OnRtnOrder: %s 平仓单合约状态 status= %d  ",config.GetTable_Close().c_str(),orderdata.Status);  //
//					logutil.WriteLog(buf);
//					 CloseOrder(pOrder, info, TradingDay);
//				 }
//				 else	if(orderdata.Status<50 && orderdata.Status >= 20) {
//					 sprintf(buf,"MyTDSpi::OnRtnOrder: %s 平仓单撤单合约状态 status= %d  ",config.GetTable_Close().c_str(),orderdata.Status);  //
//					logutil.WriteLog(buf);
//					 CloseActionOrder(pOrder, info, TradingDay);
//				 }
//				 else{
//					 sprintf(buf,"MyTDSpi::OnRtnOrder: %s 错误的合约状态 status= %d ！！！ ",config.GetTable_Close().c_str(),orderdata.Status);  //
//					logutil.WriteLog(buf);
//				 }
//
//			}
//			else{
//				sprintf(buf,"MyTDSpi::OnRtnOrder: %s 不存在平仓单原始数据 OrderRef= %s InstrumentID :%s TradingDay :%s！！！ ",
//						config.GetTable_Close().c_str(),info.OrderRef,info.InstrumentID,info.TradingDay);  //
//				logutil.WriteLog(buf);
//			}
//	 }
//
//
////		 if(!mysqlorder.Exist_OrderbyInsOrderRef("hisorder",info.InstrumentID,info.OrderRef,TradingDay) && (info.Status==2 || info.Status==3))
////			{
////				mysqlorder.NewOrderData_byRspQryOrder("hisorder",info);
////				sprintf(buf,"Debug: OnRspQryOrder -----hisorder %s-----不存在hisorder,新插入!!!",
////						pOrder->InstrumentID);  //
////				logutil.WriteLog(buf);
////
////			}
//
//
//
//
//        }
//
//    }
void MyTDSpi::OnRtnTrade(  CThostFtdcTradeField * pTrade) {

//   printf("OnRtnTrade():被执行...\n");

   char buf[1024];
   LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
//   logutil.WriteLog("\nOnRtnTrade():被执行...");
   logutil.WriteLog("\n");
   sprintf(buf,"OnRtnTrade():被执行----------> OrderSysID=[%s]    ", pTrade->OrderSysID);
   logutil.WriteLog(buf);
   // 如果有返回结果读取返回信息
   if ( pTrade != NULL && this->GetFlagLogin()) {

	   char OffsetFlag = pTrade->OffsetFlag;
	   char TradingDay[27];
	   gbk2utf8(pTrade->TradingDay,TradingDay,sizeof(TradingDay));

		sprintf(buf,"Debug: InstrumentID:%s  OrderRef:%s   TraderID:%s  PriceSource:%d TradeDate:%s TradeTime:%s ",pTrade->InstrumentID,
				pTrade->OrderRef,pTrade->TraderID,pTrade->PriceSource-'0',pTrade->TradeDate,pTrade->TradeTime);  //
		logutil.WriteLog(buf);
		sprintf(buf,"Debug: 成交价:%.3f  方向:%d  交易手数:%d ",pTrade->Price,pTrade->Direction-'0',pTrade->Volume);  //
		logutil.WriteLog(buf);

		OrderData info;
		strcpy(info.InstrumentID,pTrade->InstrumentID);
		strcpy(info.TradingDay,pTrade->TradingDay);
		strcpy(info.OrderRef,pTrade->OrderRef);
		strcpy(info.ExchangeID,pTrade->ExchangeID);
		info.Direction = pTrade->Direction-'0';
		info.CombOffsetFlag = pTrade->OffsetFlag-'0';
		info.CombHedgeFlag=pTrade->HedgeFlag-'0';
		strcpy(info.OrderSysID,pTrade->OrderSysID);
		info.Price = pTrade->Price;
		info.Volume = pTrade->Volume;

		strcpy(info.TradeID,pTrade->TradeID);
		strcpy(info.TradeDate,pTrade->TradeDate);
		strcpy(info.TradeTime,pTrade->TradeTime);

		info.Status=2;

		if(OffsetFlag=='0')		{

			//需要更新持仓列表 充值标志位
	//				OrderInsertFlag=true;

			sprintf(buf,"Debug:OnRtnTrade ---->  开仓标志位--->OrderInsertFlag= %d  ！！！！！",this->GetFlagOrderInsert());  //
			logutil.WriteLog(buf);


			m_tradetatus=true;
			this->SetFlagOrderInsert(true);
			 Config config(this->GetStrategyName().c_str());
			MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);

			 if(!mysqlopenorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))		{
				 mysqlopenorder.NewOrderData_byRtnTrade(info);
					sprintf(buf,"Debug: OnRtnTrade 开仓单-----全部成交%s-----不存在orderinfo,新插入!!!",
							pTrade->InstrumentID);  //
					logutil.WriteLog(buf);
			}
			else	{
				mysqlopenorder.UpdateOrderData_byRtnTrade(info);
				sprintf(buf,"Debug:OnRtnTrade  开仓单成交 Update_Trade  -----更新orderinfo!!!");  //
				logutil.WriteLog(buf);
			}
		}
		else if (OffsetFlag>='1' && OffsetFlag<='4')	{

			// 平仓标志位
			this->SetFlagCloseOrderInsert(true);
			sprintf(buf,"Debug:OnRtnTrade ----> 平仓标志位--->CloseOrderInsertFlag= %d  ！！！！！",this->GetFlagOrderInsert());  //
			logutil.WriteLog(buf);
			 Config config(this->GetStrategyName().c_str());
			MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);


			 if(!mysqlcloseorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
				 mysqlcloseorder.NewOrderData_byRtnTrade(info);
					sprintf(buf,"Debug: OnRtnTrade 平仓单-----全部成交%s-----不存在orderinfo,新插入!!!",
							pTrade->InstrumentID);  //
					logutil.WriteLog(buf);
			}
			else {
				mysqlcloseorder.UpdateOrderData_byRtnTrade(info);
				sprintf(buf,"Debug: OnRtnTrade 平仓单成交 Update_Trade  -----更新orderinfo!!!");  //
				logutil.WriteLog(buf);

			}

		}
   }

}

    /////成交通知
//void MyTDSpi::OnRtnTrade(  CThostFtdcTradeField * pTrade) {
//
////   printf("OnRtnTrade():被执行...\n");
//
//   char buf[1024];
//   LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
////   logutil.WriteLog("\nOnRtnTrade():被执行...");
//   logutil.WriteLog("\n");
//   sprintf(buf,"OnRtnTrade():被执行----------> OrderSysID=[%s]    ", pTrade->OrderSysID);
//   logutil.WriteLog(buf);
//   // 如果有返回结果读取返回信息
//   if ( pTrade != NULL && this->GetFlagLogin()) {
//	   // 读取返回信息,并做编码转化
//	   ///经纪公司代码 TThostFtdcBrokerIDType char[11]
////	   char BrokerID[33];
////	   gbk2utf8(pTrade->BrokerID,BrokerID,sizeof(BrokerID));
////	   ///投资者代码 TThostFtdcInvestorIDType char[13]
////	   char InvestorID[39];
////	   gbk2utf8(pTrade->InvestorID,InvestorID,sizeof(InvestorID));
////	   ///合约代码 TThostFtdcInstrumentIDType char[31]
////	   char InstrumentID[93];
////	   gbk2utf8(pTrade->InstrumentID,InstrumentID,sizeof(InstrumentID));
////	   ///报单引用 TThostFtdcOrderRefType char[13]
////	   char OrderRef[39];
////	   gbk2utf8(pTrade->OrderRef,OrderRef,sizeof(OrderRef));
////	   ///用户代码 TThostFtdcUserIDType char[16]
////	   char UserID[48];
////	   gbk2utf8(pTrade->UserID,UserID,sizeof(UserID));
////	   ///交易所代码 TThostFtdcExchangeIDType char[9]
////	   char ExchangeID[27];
////	   gbk2utf8(pTrade->ExchangeID,ExchangeID,sizeof(ExchangeID));
////	   ///成交编号 TThostFtdcTradeIDType char[21]
////	   char TradeID[63];
////	   gbk2utf8(pTrade->TradeID,TradeID,sizeof(TradeID));
////	   ///买卖方向 TThostFtdcDirectionType char
////	   //// THOST_FTDC_D_Buy '0' 买
////	   //// THOST_FTDC_D_Sell '1' 卖
////	   char Direction = pTrade->Direction;
////	   ///报单编号 TThostFtdcOrderSysIDType char[21]
////	   char OrderSysID[63];
////	   gbk2utf8(pTrade->OrderSysID,OrderSysID,sizeof(OrderSysID));
////	   ///会员代码 TThostFtdcParticipantIDType char[11]
////	   char ParticipantID[33];
////	   gbk2utf8(pTrade->ParticipantID,ParticipantID,sizeof(ParticipantID));
////	   ///客户代码 TThostFtdcClientIDType char[11]
////	   char ClientID[33];
////	   gbk2utf8(pTrade->ClientID,ClientID,sizeof(ClientID));
////	   ///交易角色 TThostFtdcTradingRoleType char
////	   //// THOST_FTDC_ER_Broker '1' 代理
////	   //// THOST_FTDC_ER_Host '2' 自营
////	   //// THOST_FTDC_ER_Maker '3' 做市商
////	   char TradingRole = pTrade->TradingRole;
////	   ///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
////	   char ExchangeInstID[93];
////	   gbk2utf8(pTrade->ExchangeInstID,ExchangeInstID,sizeof(ExchangeInstID));
////	   ///开平标志 TThostFtdcOffsetFlagType char
////	   //// THOST_FTDC_OF_Open '0' 开仓
////	   //// THOST_FTDC_OF_Close '1' 平仓
////	   //// THOST_FTDC_OF_ForceClose '2' 强平
////	   //// THOST_FTDC_OF_CloseToday '3' 平今
////	   //// THOST_FTDC_OF_CloseYesterday '4' 平昨
////	   //// THOST_FTDC_OF_ForceOff '5' 强减
////	   //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
//	   char OffsetFlag = pTrade->OffsetFlag;
////	   ///投机套保标志 TThostFtdcHedgeFlagType char
////	   //// THOST_FTDC_HF_Speculation '1' 投机
////	   //// THOST_FTDC_HF_Arbitrage '2' 套利
////	   //// THOST_FTDC_HF_Hedge '3' 套保
////	   char HedgeFlag = pTrade->HedgeFlag;
////	   ///价格 TThostFtdcPriceType double
////	   double Price = pTrade->Price;
////	   ///数量 TThostFtdcVolumeType int
////	   int Volume = pTrade->Volume;
////	   ///成交时期 TThostFtdcDateType char[9]
////	   char TradeDate[27];
////	   gbk2utf8(pTrade->TradeDate,TradeDate,sizeof(TradeDate));
////	   ///成交时间 TThostFtdcTimeType char[9]
////	   char TradeTime[27];
////	   gbk2utf8(pTrade->TradeTime,TradeTime,sizeof(TradeTime));
////	   ///成交类型 TThostFtdcTradeTypeType char
////	   //// THOST_FTDC_TRDT_SplitCombination '#' 组合持仓拆分为单一持仓,初始化不应包含该类型的持仓
////	   //// THOST_FTDC_TRDT_Common '0' 普通成交
////	   //// THOST_FTDC_TRDT_OptionsExecution '1' 期权执行
////	   //// THOST_FTDC_TRDT_OTC '2' OTC成交
////	   //// THOST_FTDC_TRDT_EFPDerived '3' 期转现衍生成交
////	   //// THOST_FTDC_TRDT_CombinationDerived '4' 组合衍生成交
////	   char TradeType = pTrade->TradeType;
////	   ///成交价来源 TThostFtdcPriceSourceType char
////	   //// THOST_FTDC_PSRC_LastPrice '0' 前成交价
////	   //// THOST_FTDC_PSRC_Buy '1' 买委托价
////	   //// THOST_FTDC_PSRC_Sell '2' 卖委托价
////	   char PriceSource = pTrade->PriceSource;
////	   ///交易所交易员代码 TThostFtdcTraderIDType char[21]
////	   char TraderID[21];
////	   gbk2utf8(pTrade->TraderID,TraderID,sizeof(TraderID));
////	   ///本地报单编号 TThostFtdcOrderLocalIDType char[13]
////	   char OrderLocalID[39];
////	   gbk2utf8(pTrade->OrderLocalID,OrderLocalID,sizeof(OrderLocalID));
////	   ///结算会员编号 TThostFtdcParticipantIDType char[11]
////	   char ClearingPartID[33];
////	   gbk2utf8(pTrade->ClearingPartID,ClearingPartID,sizeof(ClearingPartID));
////	   ///业务单元 TThostFtdcBusinessUnitType char[21]
////	   char BusinessUnit[63];
////	   gbk2utf8(pTrade->BusinessUnit,BusinessUnit,sizeof(BusinessUnit));
////	   ///序号 TThostFtdcSequenceNoType int
////	   int SequenceNo = pTrade->SequenceNo;
////	   ///交易日 TThostFtdcDateType char[9]
//	   char TradingDay[27];
//	   gbk2utf8(pTrade->TradingDay,TradingDay,sizeof(TradingDay));
////	   ///结算编号 TThostFtdcSettlementIDType int
////	   int SettlementID = pTrade->SettlementID;
////	   ///经纪公司报单编号 TThostFtdcSequenceNoType int
////	   int BrokerOrderSeq = pTrade->BrokerOrderSeq;
////	   ///成交来源 TThostFtdcTradeSourceType char
////	   //// THOST_FTDC_TSRC_NORMAL '0' 来自交易所普通回报
////	   //// THOST_FTDC_TSRC_QUERY '1' 来自查询
////	   char TradeSource = pTrade->TradeSource;
//
//
////	   printf("Debug: InstrumentID= %s  \n",pTrade->InstrumentID);  //
////		printf("Debug: TraderID= %s  \n",pTrade->TraderID);  //
////		printf("Debug: PriceSource= %d  \n",pTrade->PriceSource-'0');  //
////		printf("Debug: TradeDate= %s  \n",pTrade->TradeDate);  //
////		printf("Debug: TradeTime= %s  \n",pTrade->TradeTime);  //
////		printf("Debug: Price= %f  \n",pTrade->Price);  //
////		printf("Debug: Direction= %d  \n",pTrade->Direction);  //
////		printf("Debug: Volume= %d  \n",pTrade->Volume);  //
//
////		sprintf(buf,"Debug: InstrumentID= %s  ",pTrade->InstrumentID);  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: OrderRef= %s  ",pTrade->OrderRef);  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: TraderID= %s  ",pTrade->TraderID);  //
////		logutil.WriteLog(buf);
//////		sprintf(buf,"Debug: TradeID= %s  ",pTrade->TradeID);  //
//////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: PriceSource= %d  ",pTrade->PriceSource-'0');  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: TradeDate= %s  ",pTrade->TradeDate);  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: TradeTime= %s  ",pTrade->TradeTime);  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: Price= %f  ",pTrade->Price);  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: Direction= %d  ",pTrade->Direction-'0');  //
////		logutil.WriteLog(buf);
////		sprintf(buf,"Debug: Volume= %d  ",pTrade->Volume);  //
////		logutil.WriteLog(buf);
//
//		sprintf(buf,"Debug: InstrumentID:%s  OrderRef:%s   TraderID:%s  PriceSource:%d TradeDate:%s TradeTime:%s ",pTrade->InstrumentID,
//				pTrade->OrderRef,pTrade->TraderID,pTrade->PriceSource-'0',pTrade->TradeDate,pTrade->TradeTime);  //
//		logutil.WriteLog(buf);
//		sprintf(buf,"Debug: 成交价:%.3f  方向:%d  交易手数:%d ",pTrade->Price,pTrade->Direction-'0',pTrade->Volume);  //
//		logutil.WriteLog(buf);
//
//		OrderData info;
//		strcpy(info.InstrumentID,pTrade->InstrumentID);
//		strcpy(info.TradingDay,pTrade->TradingDay);
//		strcpy(info.OrderRef,pTrade->OrderRef);
//		strcpy(info.ExchangeID,pTrade->ExchangeID);
//		info.Direction = pTrade->Direction-'0';
//		info.CombOffsetFlag = pTrade->OffsetFlag-'0';
//		info.CombHedgeFlag=pTrade->HedgeFlag-'0';
//		strcpy(info.OrderSysID,pTrade->OrderSysID);
//		info.Price = pTrade->Price;
//		info.Volume = pTrade->Volume;
//
//		strcpy(info.TradeID,pTrade->TradeID);
//		strcpy(info.TradeDate,pTrade->TradeDate);
//		strcpy(info.TradeTime,pTrade->TradeTime);
//
//		info.Status=2;
//
//		if(OffsetFlag=='0')		{
//
//			//需要更新持仓列表 充值标志位
//	//				OrderInsertFlag=true;
//
//			sprintf(buf,"Debug:OnRtnTrade ---->  开仓标志位--->OrderInsertFlag= %d  ！！！！！",this->GetFlagOrderInsert());  //
//			logutil.WriteLog(buf);
//
//			this->SetFlagOrderInsert(true);
//			 Config config(this->GetStrategyName().c_str());
//			MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
//
//			 if(!mysqlopenorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))		{
//				 mysqlopenorder.NewOrderData_byRtnTrade(info);
//					sprintf(buf,"Debug: OnRtnTrade 开仓单-----全部成交%s-----不存在orderinfo,新插入!!!",
//							pTrade->InstrumentID);  //
//					logutil.WriteLog(buf);
//				}
//				else	{
//					mysqlopenorder.UpdateOrderData_byRtnTrade(info);
//					sprintf(buf,"Debug:OnRtnTrade  开仓单成交 Update_Trade  -----更新orderinfo!!!");  //
//					logutil.WriteLog(buf);
//
//				}
//
//
//		}
//		else if (OffsetFlag>='1' && OffsetFlag<='4')	{
//
//			// 平仓标志位
//			this->SetFlagCloseOrderInsert(true);
//			sprintf(buf,"Debug:OnRtnTrade ----> 平仓标志位--->CloseOrderInsertFlag= %d  ！！！！！",this->GetFlagOrderInsert());  //
//			logutil.WriteLog(buf);
//			 Config config(this->GetStrategyName().c_str());
//			MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
//
//
//			 if(!mysqlcloseorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
//				 mysqlcloseorder.NewOrderData_byRtnTrade(info);
//					sprintf(buf,"Debug: OnRtnTrade 平仓单-----全部成交%s-----不存在orderinfo,新插入!!!",
//							pTrade->InstrumentID);  //
//					logutil.WriteLog(buf);
//				}
//				else {
//					mysqlcloseorder.UpdateOrderData_byRtnTrade(info);
//					sprintf(buf,"Debug: OnRtnTrade 平仓单成交 Update_Trade  -----更新orderinfo!!!");  //
//					logutil.WriteLog(buf);
//
//				}
//
//		}
//   }
//
//}

    /////报单录入错误回报
    void MyTDSpi::OnErrRtnOrderInsert(
        CThostFtdcInputOrderField * pInputOrder,
        CThostFtdcRspInfoField * pRspInfo
    ) {
    	char buf[1024];
        printf("OnErrRtnOrderInsert():被执行...\n");
        LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

        logutil.WriteLog("OnErrRtnOrderInsert():被执行...");

        if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
                   // typedef int TThostFtdcErrorIDType;
                   // typedef char TThostFtdcErrorMsgType[81];
                   char ErrorMsg[243];
                   gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
                   printf("OnErrRtnOrderInsert():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

                   sprintf(buf,"OnErrRtnOrderInsert():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
                   logutil.WriteLog(buf);

                   if(pRspInfo->ErrorID ==42) {
                	   sprintf(buf,"OnErrRtnOrderInsert():出错 CTP:结算结果未确认   ----> 重置结算标志，重新确认结算  UserID %s",pInputOrder->UserID);
					  logutil.WriteLog(buf);
					  sprintf(buf,"OnErrRtnOrderInsert():出错 CTP:结算结果未确认   ----> 重置结算标志，重新确认结算  InvestorID %s",pInputOrder->InvestorID);
					  logutil.WriteLog(buf);


						 //同时更新交易日数据库 不可删除
							MysqlTradingDay mysqltradingday(pInputOrder->UserID);

							if(mysqltradingday.Exist_Table()){

								if(mysqltradingday.Exist_TradingDay(this->GetTradingDay().c_str()))	{
									TradingDayData last;
									last= mysqltradingday.Find_DatabyTradingDay(this->GetTradingDay().c_str());
									if (last.SettleInfoConfirmStatus){

											printf("MyTDSpi::OnErrRtnOrderInsert:ERROR  TradingDay=%s   ---> 未结算，重新结算确认状态 \n",
													this->GetTradingDay().c_str());
											sprintf(buf,"MyTDSpi::OnErrRtnOrderInsert:ERROR  TradingDay=%s --> 未结算，重新结算确认状态  \n",
													this->GetTradingDay().c_str());
											logutil.WriteLog(buf);

											last.SettleInfoConfirmStatus =false;
											mysqltradingday.ResetSettleInfoConfirmStatusbyDate(last.SettleInfoConfirmStatus,this->GetTradingDay().c_str());

									}
								}


							}

					  }
               }

        // 如果有返回结果读取返回信息
        if ( pInputOrder != NULL && this->GetFlagLogin()) {
            // 读取返回信息,并做编码转化
            ///经纪公司代码 TThostFtdcBrokerIDType char[11]
            char BrokerID[33];
            gbk2utf8(pInputOrder->BrokerID,BrokerID,sizeof(BrokerID));
            ///投资者代码 TThostFtdcInvestorIDType char[13]
            char InvestorID[39];
            gbk2utf8(pInputOrder->InvestorID,InvestorID,sizeof(InvestorID));
            ///合约代码 TThostFtdcInstrumentIDType char[31]
            char InstrumentID[93];
            gbk2utf8(pInputOrder->InstrumentID,InstrumentID,sizeof(InstrumentID));
            ///报单引用 TThostFtdcOrderRefType char[13]
            char OrderRef[39];
            gbk2utf8(pInputOrder->OrderRef,OrderRef,sizeof(OrderRef));
            ///用户代码 TThostFtdcUserIDType char[16]
            char UserID[48];
            gbk2utf8(pInputOrder->UserID,UserID,sizeof(UserID));
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
            char OrderPriceType = pInputOrder->OrderPriceType;
            ///买卖方向 TThostFtdcDirectionType char
            //// THOST_FTDC_D_Buy '0' 买
            //// THOST_FTDC_D_Sell '1' 卖
            char Direction = pInputOrder->Direction;
            ///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
            //// THOST_FTDC_OF_Open '0' 开仓
            //// THOST_FTDC_OF_Close '1' 平仓
            //// THOST_FTDC_OF_ForceClose '2' 强平
            //// THOST_FTDC_OF_CloseToday '3' 平今
            //// THOST_FTDC_OF_CloseYesterday '4' 平昨
            //// THOST_FTDC_OF_ForceOff '5' 强减
            //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
            char CombOffsetFlag[15];
            gbk2utf8(pInputOrder->CombOffsetFlag,CombOffsetFlag,sizeof(CombOffsetFlag));
            ///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
            //// THOST_FTDC_HF_Speculation '1' 投机
            //// THOST_FTDC_HF_Arbitrage '2' 套利
            //// THOST_FTDC_HF_Hedge '3' 套保
            char CombHedgeFlag[15];
            gbk2utf8(pInputOrder->CombHedgeFlag,CombHedgeFlag,sizeof(CombHedgeFlag));
            ///价格 TThostFtdcPriceType double
            double LimitPrice = pInputOrder->LimitPrice;
            ///数量 TThostFtdcVolumeType int
            int VolumeTotalOriginal = pInputOrder->VolumeTotalOriginal;
            ///有效期类型 TThostFtdcTimeConditionType char
            //// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
            //// THOST_FTDC_TC_GFS '2' 本节有效
            //// THOST_FTDC_TC_GFD '3' 当日有效
            //// THOST_FTDC_TC_GTD '4' 指定日期前有效
            //// THOST_FTDC_TC_GTC '5' 撤销前有效
            //// THOST_FTDC_TC_GFA '6' 集合竞价有效
            char TimeCondition = pInputOrder->TimeCondition;
            ///GTD日期 TThostFtdcDateType char[9]
            char GTDDate[27];
            gbk2utf8(pInputOrder->GTDDate,GTDDate,sizeof(GTDDate));
            ///成交量类型 TThostFtdcVolumeConditionType char
            //// THOST_FTDC_VC_AV '1' 任何数量
            //// THOST_FTDC_VC_MV '2' 最小数量
            //// THOST_FTDC_VC_CV '3' 全部数量
            char VolumeCondition = pInputOrder->VolumeCondition;
            ///最小成交量 TThostFtdcVolumeType int
            int MinVolume = pInputOrder->MinVolume;
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
            char ContingentCondition = pInputOrder->ContingentCondition;
            ///止损价 TThostFtdcPriceType double
            double StopPrice = pInputOrder->StopPrice;
            ///强平原因 TThostFtdcForceCloseReasonType char
            //// THOST_FTDC_FCC_NotForceClose '0' 非强平
            //// THOST_FTDC_FCC_LackDeposit '1' 资金不足
            //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
            //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
            //// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
            //// THOST_FTDC_FCC_Violation '5' 违规
            //// THOST_FTDC_FCC_Other '6' 其它
            //// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
            char ForceCloseReason = pInputOrder->ForceCloseReason;
            ///自动挂起标志 TThostFtdcBoolType int
            int IsAutoSuspend = pInputOrder->IsAutoSuspend;
            ///业务单元 TThostFtdcBusinessUnitType char[21]
            char BusinessUnit[63];
            gbk2utf8(pInputOrder->BusinessUnit,BusinessUnit,sizeof(BusinessUnit));
            ///请求编号 TThostFtdcRequestIDType int
            int RequestID = pInputOrder->RequestID;
            ///用户强评标志 TThostFtdcBoolType int
            int UserForceClose = pInputOrder->UserForceClose;
            ///互换单标志 TThostFtdcBoolType int
            int IsSwapOrder = pInputOrder->IsSwapOrder;


            OrderData info;
			strcpy(info.InstrumentID,pInputOrder->InstrumentID);
			strcpy(info.OrderRef,pInputOrder->OrderRef);

			info.Direction = pInputOrder->Direction-'0';
			info.CombOffsetFlag = pInputOrder->CombOffsetFlag[0]-'0';
			strcpy(info.TradingDay,this->GetTradingDay().c_str());
			info.ErrorId = pRspInfo->ErrorID;


			info.Status =-1;
			if(info.CombOffsetFlag==0){

				 Config config(this->GetStrategyName().c_str());
				MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
				if (mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,info.TradingDay))	{
					mysqlorder.UpdateOrderData_byErrRtnOrder(info);
					sprintf(buf,"Debug: OnErrRtnOrderInsert 开仓单-----出错 %s-----更新orderinfo!!!",pInputOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				}
				else	{
					strcpy(info.SignalName,"unknown");
					mysqlorder.Insert(info);
					sprintf(buf,"Debug: OnErrRtnOrderInsert 开仓单-----出错 %s-----插入错误数据 orderinfo!!!",pInputOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				}


			}
			else if (info.CombOffsetFlag>=1 && info.CombOffsetFlag<=4)	{
				 Config config(this->GetStrategyName().c_str());
				MysqlOrder mysqlorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);

				if (mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,info.TradingDay)){
					mysqlorder.UpdateOrderData_byErrRtnOrder(info);
					sprintf(buf,"Debug: OnErrRtnOrderInsert 平仓单----出错-%s-----更新orderinfo!!!",pInputOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				}
				else	{
					strcpy(info.SignalName,"unknown");
					mysqlorder.Insert(info);
					sprintf(buf,"Debug: OnErrRtnOrderInsert 平仓单-----出错 %s-----插入错误数据 orderinfo!!!",pInputOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				}

			}


        }

    }

    /////报单操作错误回报
       void MyTDSpi::OnErrRtnOrderAction(
    		   CThostFtdcOrderActionField *pOrderAction,
           CThostFtdcRspInfoField * pRspInfo
       ) {
    	   char buf[1024];
           printf("OnErrRtnOrderInsert():被执行...\n");
           LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

		   logutil.WriteLog("OnErrRtnOrderInsert():被执行...");

		   if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
		                     // typedef int TThostFtdcErrorIDType;
		                     // typedef char TThostFtdcErrorMsgType[81];
		                     char ErrorMsg[243];
		                     gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
		                     printf("OnErrRtnOrderAction():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

		                     sprintf(buf,"OnErrRtnOrderAction():出错:ErrorId=%d,ErrorMsg=%s",pRspInfo->ErrorID,ErrorMsg);
		                     logutil.WriteLog(buf);
		                 }

           // 如果有返回结果读取返回信息
           if ( pOrderAction != NULL && this->GetFlagLogin()) {
               // 读取返回信息,并做编码转化
               ///经纪公司代码 TThostFtdcBrokerIDType char[11]
               char BrokerID[33];
               gbk2utf8(pOrderAction->BrokerID,BrokerID,sizeof(BrokerID));
               ///投资者代码 TThostFtdcInvestorIDType char[13]
               char InvestorID[39];
               gbk2utf8(pOrderAction->InvestorID,InvestorID,sizeof(InvestorID));
               ///合约代码 TThostFtdcInstrumentIDType char[31]
               char InstrumentID[93];
               gbk2utf8(pOrderAction->InstrumentID,InstrumentID,sizeof(InstrumentID));
               ///报单引用 TThostFtdcOrderRefType char[13]
               char OrderRef[39];
               gbk2utf8(pOrderAction->OrderRef,OrderRef,sizeof(OrderRef));
               ///用户代码 TThostFtdcUserIDType char[16]
               char UserID[48];
               gbk2utf8(pOrderAction->UserID,UserID,sizeof(UserID));
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
//               char OrderPriceType = pOrderAction->OrderPriceType;
               ///买卖方向 TThostFtdcDirectionType char
               //// THOST_FTDC_D_Buy '0' 买
               //// THOST_FTDC_D_Sell '1' 卖
//               char Direction = pOrderAction->Direction;
               ///组合开平标志 TThostFtdcCombOffsetFlagType char[5]
               //// THOST_FTDC_OF_Open '0' 开仓
               //// THOST_FTDC_OF_Close '1' 平仓
               //// THOST_FTDC_OF_ForceClose '2' 强平
               //// THOST_FTDC_OF_CloseToday '3' 平今
               //// THOST_FTDC_OF_CloseYesterday '4' 平昨
               //// THOST_FTDC_OF_ForceOff '5' 强减
               //// THOST_FTDC_OF_LocalForceClose '6' 本地强平
               char CombOffsetFlag[15];
//               gbk2utf8(pOrderAction->CombOffsetFlag,CombOffsetFlag,sizeof(CombOffsetFlag));
               ///组合投机套保标志 TThostFtdcCombHedgeFlagType char[5]
               //// THOST_FTDC_HF_Speculation '1' 投机
               //// THOST_FTDC_HF_Arbitrage '2' 套利
               //// THOST_FTDC_HF_Hedge '3' 套保
               char CombHedgeFlag[15];
//               gbk2utf8(pOrderAction->CombHedgeFlag,CombHedgeFlag,sizeof(CombHedgeFlag));
               ///价格 TThostFtdcPriceType double
               double LimitPrice = pOrderAction->LimitPrice;
               ///数量 TThostFtdcVolumeType int
//               int VolumeTotalOriginal = pOrderAction->VolumeTotalOriginal;
               ///有效期类型 TThostFtdcTimeConditionType char
               //// THOST_FTDC_TC_IOC '1' 立即完成，否则撤销
               //// THOST_FTDC_TC_GFS '2' 本节有效
               //// THOST_FTDC_TC_GFD '3' 当日有效
               //// THOST_FTDC_TC_GTD '4' 指定日期前有效
               //// THOST_FTDC_TC_GTC '5' 撤销前有效
               //// THOST_FTDC_TC_GFA '6' 集合竞价有效
//               char TimeCondition = pOrderAction->TimeCondition;
               ///GTD日期 TThostFtdcDateType char[9]
               char GTDDate[27];
//               gbk2utf8(pOrderAction->GTDDate,GTDDate,sizeof(GTDDate));
               ///成交量类型 TThostFtdcVolumeConditionType char
               //// THOST_FTDC_VC_AV '1' 任何数量
               //// THOST_FTDC_VC_MV '2' 最小数量
               //// THOST_FTDC_VC_CV '3' 全部数量
//               char VolumeCondition = pOrderAction->VolumeCondition;
               ///最小成交量 TThostFtdcVolumeType int
//               int MinVolume = pOrderAction->MinVolume;
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
//               char ContingentCondition = pOrderAction->ContingentCondition;
               ///止损价 TThostFtdcPriceType double
//               double StopPrice = pOrderAction->StopPrice;
               ///强平原因 TThostFtdcForceCloseReasonType char
               //// THOST_FTDC_FCC_NotForceClose '0' 非强平
               //// THOST_FTDC_FCC_LackDeposit '1' 资金不足
               //// THOST_FTDC_FCC_ClientOverPositionLimit '2' 客户超仓
               //// THOST_FTDC_FCC_MemberOverPositionLimit '3' 会员超仓
               //// THOST_FTDC_FCC_NotMultiple '4' 持仓非整数倍
               //// THOST_FTDC_FCC_Violation '5' 违规
               //// THOST_FTDC_FCC_Other '6' 其它
               //// THOST_FTDC_FCC_PersonDeliv '7' 自然人临近交割
//               char ForceCloseReason = pOrderAction->ForceCloseReason;
               ///自动挂起标志 TThostFtdcBoolType int
//               int IsAutoSuspend = pOrderAction->IsAutoSuspend;
               ///业务单元 TThostFtdcBusinessUnitType char[21]
               char BusinessUnit[63];
               gbk2utf8(pOrderAction->BusinessUnit,BusinessUnit,sizeof(BusinessUnit));
               ///请求编号 TThostFtdcRequestIDType int
               int RequestID = pOrderAction->RequestID;
               ///用户强评标志 TThostFtdcBoolType int
//               int UserForceClose = pOrderAction->UserForceClose;
               ///互换单标志 TThostFtdcBoolType int
//               int IsSwapOrder = pOrderAction->IsSwapOrder;

           }

       }

    ///请求查询交易所响应
       void MyTDSpi::OnRspQryExchange(
           CThostFtdcExchangeField * pExchange,
           CThostFtdcRspInfoField * pRspInfo,
           int nRequestID,
           bool bIsLast
       ) {
           printf("OnRspQryExchange():被执行...\n");

           // 进程返回结果检查
           if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
               // typedef int TThostFtdcErrorIDType;
               // typedef char TThostFtdcErrorMsgType[81];
               char ErrorMsg[243];
               gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
               printf("OnRspQryExchange():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
           }

           // 如果有返回结果读取返回信息
           if ( pExchange != NULL && this->GetFlagLogin()) {
               // 读取返回信息,并做编码转化
               ///交易所代码 TThostFtdcExchangeIDType char[9]
        	   char ExchangeID[27];

               gbk2utf8(pExchange->ExchangeID,ExchangeID,sizeof(ExchangeID));
               ///交易所名称 TThostFtdcExchangeNameType char[61]
               char ExchangeName[183];
               gbk2utf8(pExchange->ExchangeName,ExchangeName,sizeof(ExchangeName));
               ///交易所属性 TThostFtdcExchangePropertyType char
               //// THOST_FTDC_EXP_Normal '0' 正常
               //// THOST_FTDC_EXP_GenOrderByTrade '1' 根据成交生成报单
               char ExchangeProperty = pExchange->ExchangeProperty;

               int p = atoi(&ExchangeProperty);
//   			printf("ExchangeID=%s,ExchangeName=%s,ExchangeProperty=%c, p =%d \n",ExchangeID,ExchangeName,ExchangeProperty,p);
   			ExchangeData info;
   			strcpy(info.ExchangeID,ExchangeID);
			strcpy(info.ExchangeName,ExchangeName);
//			strcpy(exchangeInfo.ExchangeProperty,ExchangeProperty);

			info.ExchangeProperty=p;

//			exchangeInfo.ExchangeP = std::atoi( exchangeInfo.ExchangeProperty );


   			MysqlExchange mysqlexchange;

			mysqlexchange.CreateTable();


   			if(!mysqlexchange.Exist_DatabyExchangeID(info.ExchangeID))
   			{
   				mysqlexchange.Insert(info);
   			}


           }

           // 如果响应函数已经返回最后一条信息
           if(bIsLast) {

        	   //printf("释放信号，查询交易所信息结束！！！\n");
               // 通知主过程，响应函数将结束

               this->PostSemExchange();
           }
       }

       ///请求查询投资者品种/跨品种保证金响应
           void MyTDSpi::OnRspQryInvestorProductGroupMargin(
        		   CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin,
				   CThostFtdcRspInfoField *pRspInfo,
				   int nRequestID,
				   bool bIsLast)
           {
        	   printf("OnRspQryInvestorProductGroupMargin():被执行...\n");

        	   // 进程返回结果检查
				if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
					// typedef int TThostFtdcErrorIDType;
					// typedef char TThostFtdcErrorMsgType[81];
					char ErrorMsg[243];
					gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
					printf("OnRspQryInvestorProductGroupMargin():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
				}

				 // 如果有返回结果读取返回信息
				  if ( pInvestorProductGroupMargin != NULL && this->GetFlagLogin()) {
					  // 读取返回信息,并做编码转化
					  ///合约代码 TThostFtdcInstrumentIDType char[31]
					  //TThostFtdcInstrumentIDType	ProductGroupID char[31];
					  char ProductGroupID[93];
					  gbk2utf8(pInvestorProductGroupMargin->ProductGroupID,ProductGroupID,sizeof(ProductGroupID));
					///交易所代码 TThostFtdcExchangeIDType char[9]
					//char ExchangeID[27];
					//gbk2utf8(pInvestorProductGroupMargin->ExchangeID,ExchangeID,sizeof(ExchangeID));

					//TThostFtdcMoneyType	UseMargin;

					  printf("ProductGroupID=%s\n",ProductGroupID);
					}

					// 如果响应函数已经返回最后一条信息
					if(bIsLast) {
						// 通知主过程，响应函数将结束

						this->PostSemProductGroup();
					}

           }

       ///请求查询合约响应
          void MyTDSpi::OnRspQryInstrument(
              CThostFtdcInstrumentField * pInstrument,
              CThostFtdcRspInfoField * pRspInfo,
              int nRequestID,
              bool bIsLast
          ) {

//              printf("OnRspQryInstrument():被执行...\n");

        	  char buf[1024];
				LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
				logutil.WriteLog("OnRspQryInstrument():被执行...");

              // 进程返回结果检查
              if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
                  // typedef int TThostFtdcErrorIDType;
                  // typedef char TThostFtdcErrorMsgType[81];
                  char ErrorMsg[243];
                  gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
                  printf("OnRspQryInstrument():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);

                  sprintf(buf,"OnRspQryInstrument():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
                  logutil.WriteLog(buf);
              }

              // 如果有返回结果读取返回信息
              if ( pInstrument != NULL && this->GetFlagLogin() && this->GetFlagFrontConnectFalg()) {
                  // 读取返回信息,并做编码转化
                  ///合约代码 TThostFtdcInstrumentIDType char[31]
                  char InstrumentID[93];
                  gbk2utf8(pInstrument->InstrumentID,InstrumentID,sizeof(InstrumentID));
                  ///交易所代码 TThostFtdcExchangeIDType char[9]
                  char ExchangeID[27];
                  gbk2utf8(pInstrument->ExchangeID,ExchangeID,sizeof(ExchangeID));
                  ///合约名称 TThostFtdcInstrumentNameType char[21]
                  char InstrumentName[63];
                  gbk2utf8(pInstrument->InstrumentName,InstrumentName,sizeof(InstrumentName));
                  ///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
                  char ExchangeInstID[93];
                  gbk2utf8(pInstrument->ExchangeInstID,ExchangeInstID,sizeof(ExchangeInstID));
                  ///产品代码 TThostFtdcInstrumentIDType char[31]
                  char ProductID[31];
                  gbk2utf8(pInstrument->ProductID,ProductID,sizeof(ProductID));
                  ///产品类型 TThostFtdcProductClassType char
                  //// THOST_FTDC_PC_Futures '1' 期货
                  //// THOST_FTDC_PC_Options '2' 期权
                  //// THOST_FTDC_PC_Combination '3' 组合
                  //// THOST_FTDC_PC_Spot '4' 即期
                  //// THOST_FTDC_PC_EFP '5' 期转现
//                  char ProductClass = pInstrument->ProductClass;
//                  char test = pInstrument->ProductClass;
//                  int productclass = atoi(&ProductClass);

                  char ProductClass = pInstrument->ProductClass;
                  int productclass = ProductClass-'0';

                  ///交割年份 TThostFtdcYearType int
                  int DeliveryYear = pInstrument->DeliveryYear;
                  ///交割月 TThostFtdcMonthType int
                  int DeliveryMonth = pInstrument->DeliveryMonth;
                  ///市价单最大下单量 TThostFtdcVolumeType int
                  int MaxMarketOrderVolume = pInstrument->MaxMarketOrderVolume;
                  ///市价单最小下单量 TThostFtdcVolumeType int
                  int MinMarketOrderVolume = pInstrument->MinMarketOrderVolume;
                  ///限价单最大下单量 TThostFtdcVolumeType int
                  int MaxLimitOrderVolume = pInstrument->MaxLimitOrderVolume;
                  ///限价单最小下单量 TThostFtdcVolumeType int
                  int MinLimitOrderVolume = pInstrument->MinLimitOrderVolume;
                  ///合约数量乘数 TThostFtdcVolumeMultipleType int
                  int VolumeMultiple = pInstrument->VolumeMultiple;
                  ///最小变动价位 TThostFtdcPriceType double
                  double PriceTick = pInstrument->PriceTick;
                  ///创建日 TThostFtdcDateType char[9]
                  char CreateDate[27];
                  gbk2utf8(pInstrument->CreateDate,CreateDate,sizeof(CreateDate));
                  ///上市日 TThostFtdcDateType char[9]
                  char OpenDate[27];
                  gbk2utf8(pInstrument->OpenDate,OpenDate,sizeof(OpenDate));
                  ///到期日 TThostFtdcDateType char[9]
                  char ExpireDate[27];
                  gbk2utf8(pInstrument->ExpireDate,ExpireDate,sizeof(ExpireDate));
                  ///开始交割日 TThostFtdcDateType char[9]
                  char StartDelivDate[27];
                  gbk2utf8(pInstrument->StartDelivDate,StartDelivDate,sizeof(StartDelivDate));
                  ///结束交割日 TThostFtdcDateType char[9]
                  char EndDelivDate[27];
                  gbk2utf8(pInstrument->EndDelivDate,EndDelivDate,sizeof(EndDelivDate));
                  ///合约生命周期状态 TThostFtdcInstLifePhaseType char
                  //// THOST_FTDC_IP_NotStart '0' 未上市
                  //// THOST_FTDC_IP_Started '1' 上市
                  //// THOST_FTDC_IP_Pause '2' 停牌
                  //// THOST_FTDC_IP_Expired '3' 到期
                  char InstLifePhase = pInstrument->InstLifePhase;
                  int instlifephase = atoi(&InstLifePhase);
//                  char InstLifePhase = pInstrument->InstLifePhase[0]-'0';
//                  int instlifephase =InstLifePhase;

                  ///当前是否交易 TThostFtdcBoolType int
                  int IsTrading = pInstrument->IsTrading;
                  ///持仓类型 TThostFtdcPositionTypeType char
                  //// THOST_FTDC_PT_Net '1' 净持仓
                  //// THOST_FTDC_PT_Gross '2' 综合持仓
                  char PositionType = pInstrument->PositionType;
                  ///持仓日期类型 TThostFtdcPositionDateTypeType char
                  //// THOST_FTDC_PDT_UseHistory '1' 使用历史持仓
                  //// THOST_FTDC_PDT_NoUseHistory '2' 不使用历史持仓
                  char PositionDateType = pInstrument->PositionDateType;
                  ///多头保证金率 TThostFtdcRatioType double
                  double LongMarginRatio = pInstrument->LongMarginRatio;
                  ///空头保证金率 TThostFtdcRatioType double
                  double ShortMarginRatio = pInstrument->ShortMarginRatio;
                  ///是否使用大额单边保证金算法 TThostFtdcMaxMarginSideAlgorithmType char
                  //// THOST_FTDC_MMSA_NO '0' 不使用大额单边保证金算法
                  //// THOST_FTDC_MMSA_YES '1' 使用大额单边保证金算法
                  char MaxMarginSideAlgorithm = pInstrument->MaxMarginSideAlgorithm;
DateUtil dateutil;
      			sprintf(buf,"InstrumentID=%s,ExchangeID=%s,InstrumentName=%s ProductID=%s ProductClass=%d  %s \n",
      					InstrumentID,ExchangeID,InstrumentName,ProductID,productclass,dateutil.GetCurrentSqlTimeString().c_str());
      			logutil.WriteLog(buf);

      			InstrumentData info;
				strcpy(info.ExchangeID,ExchangeID);
				strcpy(info.InstrumentID,InstrumentID);
				strcpy(info.InstrumentName,InstrumentName);
				strcpy(info.ExchangeInstID,ExchangeInstID);
				strcpy(info.ProductID,ProductID);
				info.ProductClass = productclass;
				info.DeliveryYear = DeliveryYear;
				info.DeliveryMonth= DeliveryMonth;
				info.VolumeMultiple = VolumeMultiple;
				info.PriceTick=PriceTick;

				strcpy(info.CreateDate,CreateDate);
				strcpy(info.OpenDate,OpenDate);
				strcpy(info.ExpireDate,ExpireDate);
				strcpy(info.StartDelivDate,StartDelivDate);
				strcpy(info.EndDelivDate,EndDelivDate);
				info.InstLifePhase= instlifephase;
				info.LongMarginRatio=LongMarginRatio;
				info.ShortMarginRatio=ShortMarginRatio;



//				if(info.ProductClass==1){

//					printf("OnRspQryInstrument: 期货合约----> InstrumentID=%s,ExchangeID=%s,InstrumentName=%s ProductID=%s ProductClass=%d\n",
//							InstrumentID,ExchangeID,InstrumentName,ProductID,productclass);

					MysqlInstrument mysqlinstrument;

					if(!mysqlinstrument.Exist_DatabyInstrumentID(info.InstrumentID))	{
						mysqlinstrument.Insert(info);
					}
//				}
//				else if(info.ProductClass==2){
//
////					printf("OnRspQryInstrument: 期权合约----> InstrumentID=%s,ExchangeID=%s,InstrumentName=%s ProductID=%s ProductClass=%d\n",
////											InstrumentID,ExchangeID,InstrumentName,ProductID,productclass);
//				}
//				else if(info.ProductClass==3){
////					printf("OnRspQryInstrument: 组合  InstrumentID=%s\n",InstrumentID);
//				}
//				else if(info.ProductClass==4){
////					printf("OnRspQryInstrument: 即期  InstrumentID=%s\n",InstrumentID);
//				}
//				else if(info.ProductClass==5){
////					printf("OnRspQryInstrument: 期转现 InstrumentID=%s \n",InstrumentID);
//				}
//				else{
//					printf("OnRspQryInstrument: 未知  InstrumentID=%s\n",InstrumentID);
//				}


              }

              // 如果响应函数已经返回最后一条信息
              if(bIsLast) {

            	  printf("OnRspQryInstrument():通知主过程，响应函数将结束...\n");
                  // 通知主过程，响应函数将结束

                  this->PostSemInstrument();
              }
          }


    void MyTDSpi::OnRspQryDepthMarketData(
    		CThostFtdcDepthMarketDataField *pDepthMarketData,
			CThostFtdcRspInfoField *pRspInfo,
			int nRequestID,
			bool bIsLast)
          {

    	//printf("OnRspQryDepthMarketData():被执行...\n");

    			char buf[4096];
    			LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
    			logutil.WriteLog("OnRspQryDepthMarketData():被执行...");

			  // 进程返回结果检查
			  if ( (pRspInfo) && (pRspInfo->ErrorID != 0) )  {
				  // typedef int TThostFtdcErrorIDType;
				  // typedef char TThostFtdcErrorMsgType[81];
				  char ErrorMsg[243];
				  gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
				  sprintf(buf,"OnRspQryDepthMarketData():出错:ErrorId=%d,ErrorMsg=%s\n",pRspInfo->ErrorID,ErrorMsg);
				  logutil.WriteLog(buf);
			  }

			  // 如果有返回结果读取返回信息
			  if ( pDepthMarketData != NULL && this->GetFlagLogin() ) {


				  char InstrumentID[93];
				   gbk2utf8(pDepthMarketData->InstrumentID,InstrumentID,sizeof(InstrumentID));
				   ///交易所代码 TThostFtdcExchangeIDType char[9]
				   char ExchangeID[27];
				   gbk2utf8(pDepthMarketData->ExchangeID,ExchangeID,sizeof(ExchangeID));
				   ///合约名称 TThostFtdcInstrumentNameType char[21]

				   char ActionDay[27];
				   gbk2utf8(pDepthMarketData->ActionDay,ActionDay,sizeof(ActionDay));


					char UpdateTime[10];
					int openinterest = pDepthMarketData->OpenInterest;
					int volume = pDepthMarketData->Volume;
					int amount =pDepthMarketData->Turnover;
				  char TradingDay[27];
				  gbk2utf8(pDepthMarketData->TradingDay,TradingDay,sizeof(TradingDay));


				  double lastprice = pDepthMarketData->LastPrice;
				  strcpy(UpdateTime, pDepthMarketData->UpdateTime);

				  sprintf (buf,"--->>> Actionday:%s InstrumentID: %s  UpdateTime: %s  lastprice: %f  持仓量: %d 成交量: %d Amount: %d 日期: %s  交易所: %s",
						  ActionDay,InstrumentID,UpdateTime,lastprice,openinterest,volume,amount,TradingDay,ExchangeID);
				  logutil.WriteLog(buf);

				  DepthMarketData depthmarket;
				  strcpy(depthmarket.InstrumentID,InstrumentID);

				  char ProductID[31];
				  CodeUtil codeutil;
				  strcpy(ProductID,codeutil.GetProductID(InstrumentID).c_str());


				  strcpy(depthmarket.ExchangeID,ExchangeID);
				  strcpy(depthmarket.ActionDay,ActionDay);
				  strcpy(depthmarket.TradingDay,TradingDay);
				  strcpy(depthmarket.UpdateTime,UpdateTime);
				  strcpy(depthmarket.ProductID,ProductID);
				  depthmarket.OpenInterest = openinterest;
				  depthmarket.LastPrice = lastprice;

				  depthmarket.OpenPrice = pDepthMarketData->OpenPrice;
				  depthmarket.HighestPrice = pDepthMarketData->HighestPrice;
				  depthmarket.LowestPrice = pDepthMarketData->LowestPrice;
				  depthmarket.ClosePrice = pDepthMarketData->ClosePrice;
				  depthmarket.PreClosePrice = pDepthMarketData->PreClosePrice;

				  depthmarket.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
				  depthmarket.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;
				  depthmarket.Volume = pDepthMarketData->Volume;
				  depthmarket.Turnover = pDepthMarketData->Turnover;

				  depthmarket.PreSettlementPrice =  pDepthMarketData->PreSettlementPrice;
				  depthmarket.SettlementPrice =  pDepthMarketData->SettlementPrice;

				  DateUtil dateutil;
				  double Q_BarTime_writedayline =  dateutil.GetCurrentBarTime();

				bool dayend_closetime = Q_BarTime_writedayline > 0.152000 && Q_BarTime_writedayline < 0.160000 ;
				bool valid_exchange = strcmp(depthmarket.ExchangeID,"INE")!=0;

				if(dayend_closetime && valid_exchange){
					InstrumentData instrumentdata;
					MysqlInstrument mysqlinstrument;

					if (mysqlinstrument.Exist_DatabyInstrumentID(pDepthMarketData->InstrumentID)){
						instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(pDepthMarketData->InstrumentID);

						 MysqlDepthMarket mysqldepthmarket("depthmarket_endtime");
						 if(!mysqldepthmarket.Exist_Table()){
								mysqldepthmarket.CreateTable();
								printf("	OnRspQryDepthMarketData:创建数据表  MysqlDepthMarket-->depthmarket_endtime  \n");
							}
						  if(instrumentdata.ProductClass ==1 && !mysqldepthmarket.Exist_DatabyInstrumentID(InstrumentID))		  {

		//    	            		  sprintf (buf,"不存在数据--->>> 插入新数据 depthmarket : 合约[%s]  更新日期:%s  最新价: %f  持仓量: %d 成交量: %d  ",
		//    	            		      	        InstrumentID,UpdateTime,lastprice,depthmarket.OpenInterest,depthmarket.Volume);
		//							  logutil.WriteLog(buf);
							  mysqldepthmarket.Insert(depthmarket);

							  if(depthmarket.LastPrice<100000000.0){

							  }
							  else  {
								  sprintf (buf,">>>depthmarket_endtime 数据不合规--->>> 插入新数据 depthmarket : "
										  "合约[%s]  更新日期:%s  最新价: %f  持仓量: %d 成交量: %d  ",
										InstrumentID,UpdateTime,lastprice,depthmarket.OpenInterest,depthmarket.Volume);
								  logutil.WriteLog(buf);
							  }
						  }
					}


				}


				if(valid_exchange){
					InstrumentData instrumentdata;
					MysqlInstrument mysqlinstrument;

					if (mysqlinstrument.Exist_DatabyInstrumentID(pDepthMarketData->InstrumentID)){
						instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(pDepthMarketData->InstrumentID);

						 MysqlDepthMarket mysqldepthmarket;
						  if(instrumentdata.ProductClass ==1 && !mysqldepthmarket.Exist_DatabyInstrumentID(InstrumentID))		  {

		//    	            		  sprintf (buf,"不存在数据--->>> 插入新数据 depthmarket : 合约[%s]  更新日期:%s  最新价: %f  持仓量: %d 成交量: %d  ",
		//    	            		      	        InstrumentID,UpdateTime,lastprice,depthmarket.OpenInterest,depthmarket.Volume);
		//							  logutil.WriteLog(buf);
							  mysqldepthmarket.Insert(depthmarket);

							  if(depthmarket.LastPrice<100000000.0){

							  }
							  else  {

								  sprintf (buf,"数据不合规--->>> 插入新数据 depthmarket : 合约[%s]  更新日期:%s  最新价: %f  持仓量: %d 成交量: %d  ",
										InstrumentID,UpdateTime,lastprice,depthmarket.OpenInterest,depthmarket.Volume);
								  logutil.WriteLog(buf);
							  }
						  }
					}

				}


				if( dayend_closetime && valid_exchange){
					InstrumentData instrumentdata;
					MysqlInstrument mysqlinstrument;

					if (mysqlinstrument.Exist_DatabyInstrumentID(pDepthMarketData->InstrumentID)){
						instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(pDepthMarketData->InstrumentID);
						bool valid_depthmarketdata = pDepthMarketData->OpenPrice>1.0 && pDepthMarketData->HighestPrice>1.0 &&pDepthMarketData->LowestPrice>1.0
								&& pDepthMarketData->ClosePrice>1.0 && pDepthMarketData->PreClosePrice>1.0 ;
						bool valid_depthmarketdata_max = pDepthMarketData->OpenPrice<100000000.0 && pDepthMarketData->HighestPrice<100000000.0
								&&pDepthMarketData->LowestPrice<100000000.0	&& pDepthMarketData->ClosePrice<100000000.0
								&& pDepthMarketData->PreClosePrice<100000000.0 ;
						//期货数据 ProductClass =1 ,时间 为收盘后 数据不能为空
						if(instrumentdata.ProductClass ==1 && valid_depthmarketdata && valid_depthmarketdata_max){
								HxDayLineData hxdaylinedata;
							  MysqlDayLine mysqldayline;
							  strcpy(hxdaylinedata.InstrumentID,pDepthMarketData->InstrumentID);
							  strcpy (hxdaylinedata.Time,pDepthMarketData->TradingDay);

							  std::string sqltime = dateutil.ConvertDatetoSqlTime(pDepthMarketData->TradingDay);
							  hxdaylinedata.Open = pDepthMarketData->OpenPrice;
							  hxdaylinedata.High = pDepthMarketData->HighestPrice;
							  hxdaylinedata.Low = pDepthMarketData->LowestPrice;
							  hxdaylinedata.Close = pDepthMarketData->ClosePrice;
							  hxdaylinedata.LastClose = pDepthMarketData->PreClosePrice;
							  hxdaylinedata.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
							  hxdaylinedata.SettlementPrice = pDepthMarketData->SettlementPrice;
							  hxdaylinedata.OpenInterest = pDepthMarketData->OpenInterest;
							  hxdaylinedata.Amount = pDepthMarketData->Turnover;
							  hxdaylinedata.Volume = pDepthMarketData->Volume;

							  if(mysqldayline.Exist_Table(hxdaylinedata.InstrumentID) &&
									  mysqldayline.Count_ColumnsbyTableName(hxdaylinedata.InstrumentID)==8){
								printf("		----->> [%s]  Drop 日线数据表    \n",hxdaylinedata.InstrumentID);
								mysqldayline.DropbyTableName(hxdaylinedata.InstrumentID);
							}

							  if(!mysqldayline.Exist_Table(hxdaylinedata.InstrumentID)){
								  mysqldayline.CreateTable(hxdaylinedata.InstrumentID);
							  }

							  if(mysqldayline.Exist_DatabySqlTime(hxdaylinedata.InstrumentID,sqltime.c_str())){

								  HxDayLineData exist_data=mysqldayline.Find_DataBySqlTime(hxdaylinedata.InstrumentID,sqltime.c_str());
								  if(exist_data.Open!=hxdaylinedata.Open || exist_data.High!=hxdaylinedata.High ||
										  exist_data.Low!=hxdaylinedata.Low || exist_data.Close!=hxdaylinedata.Close  ){
									  printf ("日线数据全部更新--->>> 存在不同数据 dayline 全部更新---->>>  合约:%s  日期:%s  开盘价: %f 最高价: %f 最低价: %f "
										  "收盘价: %f 昨收价: %f 成交量: %f 成交额: %f  \n",
											InstrumentID,sqltime.c_str(),hxdaylinedata.Open,hxdaylinedata.High,hxdaylinedata.Low,
											hxdaylinedata.Close,hxdaylinedata.LastClose, hxdaylinedata.Volume, hxdaylinedata.Amount);
									  sprintf (buf,"日线数据全部更新--->>> 存在不同数据 dayline 全部更新------原数据---->>>  合约:%s  日期:%s  开盘价: %f 最高价: %f 最低价: %f "
											  "收盘价: %f 昨收价: %f 成交量: %f 成交额: %f  ",
												InstrumentID,sqltime.c_str(),exist_data.Open,exist_data.High,exist_data.Low,
												exist_data.Close,exist_data.LastClose, exist_data.Volume, exist_data.Amount);
										  logutil.WriteLog(buf);

									  sprintf (buf,"日线数据全部更新--->>> 存在不同数据 dayline 全部更新---->>>  合约:%s  日期:%s  开盘价: %f 最高价: %f 最低价: %f "
										  "收盘价: %f 昨收价: %f 成交量: %f 成交额: %f  ",
											InstrumentID,sqltime.c_str(),hxdaylinedata.Open,hxdaylinedata.High,hxdaylinedata.Low,
											hxdaylinedata.Close,hxdaylinedata.LastClose, hxdaylinedata.Volume, hxdaylinedata.Amount);
									  logutil.WriteLog(buf);
									  mysqldayline.UpdateAllPriceDepthmarket(hxdaylinedata.InstrumentID,hxdaylinedata);
								  }
								  else if (exist_data.PreSettlementPrice!=hxdaylinedata.PreSettlementPrice ||
										  exist_data.SettlementPrice!=hxdaylinedata.SettlementPrice ||
										  exist_data.OpenInterest!=hxdaylinedata.OpenInterest ){
									  printf ("日线数据部分更新--->>> 存在相同数据 dayline : 合约[%s]  日期:%s  PreSettlementPrice: %f SettlementPrice: %f OpenInterest: %d \n",
										InstrumentID,sqltime.c_str(),hxdaylinedata.PreSettlementPrice,hxdaylinedata.SettlementPrice,
										hxdaylinedata.OpenInterest);
									  sprintf (buf,"日线数据部分更新--->>> 存在相同数据 dayline : 合约[%s]  日期:%s  PreSettlementPrice: %f SettlementPrice: %f OpenInterest: %d ",
											InstrumentID,sqltime.c_str(),hxdaylinedata.PreSettlementPrice,hxdaylinedata.SettlementPrice,
											hxdaylinedata.OpenInterest);
									  logutil.WriteLog(buf);
									  mysqldayline.UpdateDepthmarket(hxdaylinedata.InstrumentID,hxdaylinedata);
								  }

							  }
							  else{
								  mysqldayline.InsertDepthmarket(hxdaylinedata.InstrumentID,hxdaylinedata);
								  sprintf (buf,"日线数据写入--->>> 插入新数据 dayline : 合约[%s]  日期:%s  开盘价: %f 最高价: %f 最低价: %f "
										  "收盘价: %f 昨收价: %f 成交量: %f 成交额: %f  ",
											InstrumentID,sqltime.c_str(),hxdaylinedata.Open,hxdaylinedata.High,hxdaylinedata.Low,
											hxdaylinedata.Close,hxdaylinedata.LastClose, hxdaylinedata.Volume, hxdaylinedata.Amount);
								  logutil.WriteLog(buf);
							  }
						}
					}
				}



			  }


			// 如果响应函数已经返回最后一条信息
			if(bIsLast) {
				printf("OnRspQryDepthMarketData():通知主过程，响应函数将结束...\n");
				// 通知主过程，响应函数将结束

				this->PostSemDepthMarket();
			}

	  }

    ///合约交易状态通知
    	void MyTDSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
    	{
//    		printf("OnRtnInstrumentStatus():被执行...\n");
//    		char buf[1024];
//    		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
//
//			logutil.WriteLog("OnRtnInstrumentStatus():被执行...");
//    		  if ( pInstrumentStatus != NULL && this->GetFlagLogin()) {
    			  //对被保护资源（以下打印语句）自动加锁
    			      //线程函数结束前，自动解锁


//    				///交易所代码
//    				TThostFtdcExchangeIDType	ExchangeID;
//    				gbk2utf8(pInstrumentStatus->ExchangeID,ExchangeID,sizeof(ExchangeID));
//    				///合约在交易所的代码
//    				TThostFtdcExchangeInstIDType	ExchangeInstID;
//    				gbk2utf8(pInstrumentStatus->ExchangeInstID,ExchangeInstID,sizeof(ExchangeInstID));
//    				///结算组代码
//    				TThostFtdcSettlementGroupIDType	SettlementGroupID;
//    				gbk2utf8(pInstrumentStatus->SettlementGroupID,SettlementGroupID,sizeof(SettlementGroupID));
//    				///合约代码
//    				TThostFtdcInstrumentIDType	InstrumentID;
//    				gbk2utf8(pInstrumentStatus->InstrumentID,InstrumentID,sizeof(InstrumentID));
//    				///合约交易状态
//    				TThostFtdcInstrumentStatusType	InstrumentStatus;
//
//    				InstrumentStatus=pInstrumentStatus->InstrumentStatus;

    				//公有流和私有流的订阅
    				//SubscribePublicTopic(THOST_TERT_RESTART); //公有流只有OnRtnInstrumentStatus，THOST_TERT_RESTART这样你可以实时跟踪每一种分类、每一个代码的当前状态
    				/////////////////////////////////////////////////////////////////////////
    				///TFtdcInstrumentStatusType是一个合约交易状态类型
    				/////////////////////////////////////////////////////////////////////////
    				///开盘前
    				//#define THOST_FTDC_IS_BeforeTrading '0'
    				///非交易
    				//#define THOST_FTDC_IS_NoTrading '1'
    				///连续交易
    				//#define THOST_FTDC_IS_Continous '2'
    				///集合竞价报单
    				//#define THOST_FTDC_IS_AuctionOrdering '3'
    				///集合竞价价格平衡
    				//#define THOST_FTDC_IS_AuctionBalance '4'
    				///集合竞价撮合
    				//#define THOST_FTDC_IS_AuctionMatch '5'
    				///收盘
    				//#define THOST_FTDC_IS_Closed '6'

    				///交易阶段编号
//    				TThostFtdcTradingSegmentSNType	TradingSegmentSN;
//    				TradingSegmentSN = pInstrumentStatus->TradingSegmentSN;
//
//    				///进入本状态时间
//    				TThostFtdcTimeType	EnterTime;
//    				gbk2utf8(pInstrumentStatus->EnterTime,EnterTime,sizeof(EnterTime));
//    				///进入本状态原因
//    				TThostFtdcInstStatusEnterReasonType	EnterReason;
//    				EnterReason = pInstrumentStatus->EnterReason;

//				printf("Debug: InstrumentStatus= %d  \n",InstrumentStatus-'0');  //
//				printf("Debug: EnterReason= %d  \n",EnterReason-'0');  //
//				printf("Debug: InstrumentID= %s   InstrumentStatus:%d  EnterTime: %s\n",InstrumentID,InstrumentStatus-'0',EnterTime);  //
//				printf("Debug: ExchangeID= %s  \n",ExchangeID);  //
//				printf("Debug: SettlementGroupID= %s  \n",SettlementGroupID);  //
//				printf("Debug: TradingSegmentSN= %d  \n",TradingSegmentSN);  //
//				printf("Debug: EnterTime= %s  \n",EnterTime);  //

//				sprintf(buf,"Debug: InstrumentID= %s   InstrumentStatus:%d  EnterTime: %s",InstrumentID,InstrumentStatus-'0',EnterTime);
//				logutil.WriteLog(buf);


//				RedisInsStatus redisinsstatus;
//				InstrumentStatusInfo instrumentstatus;
//
//				instrumentstatus.InstrumentStatus = int(InstrumentStatus-'0');
//				instrumentstatus.EnterReason = int (EnterReason-'0');
//
//				strcpy(instrumentstatus.ProductID,InstrumentID);
//				strcpy(instrumentstatus.ExchangeID,ExchangeID);
//				strcpy(instrumentstatus.EnterTime,EnterTime);
//
//
//				redisinsstatus.SetInsStatus(instrumentstatus);
//				redisinsstatus.disConnect();
//    		  }

    	}
	//*/

    // 针对用户请求的出错通知
    //*
	void MyTDSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
        bool bIsLast)
    {
		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

		logutil.WriteLog("OnRspError():被执行...");

		char Message[93];
		gbk2utf8(pRspInfo->ErrorMsg,Message,sizeof(Message));

        printf("OnRspError:\n");
        printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,Message);
        printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

        sprintf(buf,"OnRspError:");
        logutil.WriteLog(buf);
		sprintf(buf,"ErrorCode=[%d], ErrorMsg=[%s]", pRspInfo->ErrorID,Message);
		logutil.WriteLog(buf);
		sprintf(buf,"RequestID=[%d], Chain=[%d]", nRequestID, bIsLast);
		logutil.WriteLog(buf);
        // 客户端需进行错误处理
        //{客户端的错误处理}
    }

	std::string  MyTDSpi::CheckFlowPath(){

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

void MyTDSpi::FrontConnect()
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

		printf(" TradeBase::FrontConnect--->>> loop begin -->  [%d] \n",this->GetConnectCount());
		// 使客户端开始与后台服务建立连接
		 if (this->GetConnectCount()==0){

			 int res=this->ResetSemInit();
			sprintf(buf," TradeBase::FrontConnect--->>> 首次 Initialing MdApi!!!  ");
			logutil.PrintLog(buf);
			sleep(2);
			printf("TradeBase::FrontConnect 尝试连接------->>>>\n");
			this->m_pUserApi->Init();

				// 等待服务器发出登录消息

			 printf("TradeBase::FrontConnect-------sem_wait(&sem_init)   ------>> \n");
			 this->TimeWaitSem(this->GetSemInit());
			 printf("TradeBase::FrontConnect-------sem_wait(&sem_init)   <<------ \n");

		 }
		 else{
			 sprintf(buf," TradeBase::FrontConnect--->>>  Initialing MdApi 已经发送，无需再次发送!!!  ");
		 }

		 sleep(5);

		 if (this->GetConnectCount()>0 && this->GetFlagFrontConnectFalg()){
			 flag=true;

			struct timeval tv_last;
			gettimeofday(&tv_last,NULL);
	 //	    printf("second:%ld\n",tv_last.tv_sec);  //秒
	 //	    printf("millisecond:%ld\n",tv_last.tv_sec*1000 + tv_last.tv_usec/1000);  //毫秒

			long l =tv_last.tv_sec*1000 + tv_last.tv_usec/1000;

			 printf(" TradeBase::FrontConnect--->>> loop true -->  [%d]   网络链路连接成功,响应时间[%ld]毫秒  %ld \n",
					 this->GetConnectCount(),l-s,tv_last.tv_sec-tv.tv_sec);
		 }
		 else{
			 printf(" TradeBase::FrontConnect--->>> loop false -->  [%d] \n",this->GetConnectCount());
			 sleep(10);
		 }

	}

}
void MyTDSpi::UserLogin()
{
	 // 初始化线程同步变量
	int rest=this->ResetSemLogin();

	 if(rest) {//初始化信号量失败

			perror("TradeBase::UserLogin --->Semaphore initialization failed\n");
			exit(EXIT_FAILURE);
		}
	 printf("TradeBase::UserLogin   尝试Login--->>>>等待3秒\n");
	sleep(3);

	printf("TradeBase::UserLogin   尝试Login--->>>>\n");


	int result_login ;


	Config config(this->GetStrategyName().c_str());
	CThostFtdcReqUserLoginField userLoginField;
	memset(&userLoginField, 0, sizeof(userLoginField));

	strcpy(userLoginField.BrokerID, config.GetBrokerID().c_str());
	strcpy(userLoginField.UserID, config.GetUserID().c_str());
	strcpy(userLoginField.Password, config.GetPasswrod().c_str());
	strcpy(userLoginField.UserProductInfo,config.GetProductInfo().c_str());

	result_login=this->m_pUserApi->ReqUserLogin(&userLoginField, m_requestID++);

	if (result_login==0)				{
		printf("TradeBase::UserLogin 发送登录请求成功！  :\n");
		// 等待登录成功消息
		printf("TradeBase::UserLogin  sem_wait(&sem_login)   Begin-------->>>> \n");
		int ret=this->TimeWaitSem(this->GetSemLogin());
		printf("TradeBase::UserLogin  sem_wait(&sem_login)   <<<<--------End \n");


		if (this->GetFlagLogin())	{
			printf("TradeBase::UserLogin   <<<<<<-------登录成功后，确认登录标志GetFlagLogin--->成功！！！ \n");
//			sleep(3);
//			this->UpdateDifSec();
//			this->UpdateTradingDay();
//			this->UpdateTradingDaySqlTime();
//			printf("TradeBase::UserLogin   <<<<<<-------完成登录,本地<---->远程时间差 [%d]秒 \n",this->GetDifSec());
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
void MyTDSpi::FirstLogin()
{
	char logbuf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	printf("TradeBase::FirstLogin ----->Begin!!! \n");

	bool loopflag=false;
	while(!loopflag)	{

		bool frontconnect_ok = this->GetFlagFrontConnectFalg();
//		bool frontdisconnect_was =! this->GetCTraderSpi()->GetFlagFrontDisConnect() ;
		bool login_ok = this->GetFlagLogin();
		bool noerrorcode = this->GetConnectErrorCode()==0;

		bool readylogin = frontconnect_ok  &&!login_ok && noerrorcode;

		if(readylogin)		{
			printf("TradeBase::FirstLogin ----->Ready Login!!! \n");
			//登录
			this->UserLogin();

			frontconnect_ok = this->GetFlagFrontConnectFalg();
//			frontdisconnect_was =! this->GetCTraderSpi()->GetFlagFrontDisConnect() ;
			login_ok = this->GetFlagLogin();
			noerrorcode = this->GetConnectErrorCode()==0;

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
					logutil.WriteLog(logbuf);
				}
//				if(!frontdisconnect_was){
//					printf("TradeBase::FirstLogin ----->frontdisconnect_was 确认登录失败!!! \n");
//				}
				if(!login_ok){
					printf("TradeBase::FirstLogin ----->login_fin 确认登录失败!!! \n");
					sprintf(logbuf,"TradeBase::FirstLogin ----->login_fin 确认登录失败!!! ");
					logutil.WriteLog(logbuf);
				}
				if(!noerrorcode){
					printf("TradeBase::FirstLogin----->noerrorcode 确认登录失败!!!   [%d]  \n",
							this->GetConnectErrorCode());
					sprintf(logbuf,"TradeBase::FirstLogin----->noerrorcode 确认登录失败!!!   [%d]  ",
												this->GetConnectErrorCode());
					logutil.WriteLog(logbuf);
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

void MyTDSpi::ReqAuthenticate()
{

	printf("Trader::ReqAuthenticate   ---->Begin!!!   \n");

	CThostFtdcReqAuthenticateField authenticatefield ;
	memset(&authenticatefield,0,sizeof(authenticatefield));

	strcpy(authenticatefield.BrokerID, this->GetBrokerID().c_str());
	strcpy(authenticatefield.UserID,  this->GetUserID().c_str());

//	gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));
	char info[11];
	char code[17];
	CodeUtil codeutil;
	std::string userproductinfo;
	std::string authcode;
	std::string appid;

	strcpy(authenticatefield.UserProductInfo,this->GetProductInfo().c_str()); // 11 字节 限定
	strcpy(authenticatefield.AuthCode, this->GetAuthCode().c_str());  // 17字节限定
	strcpy(authenticatefield.AppID,this->GetAppID().c_str());

	printf("Trader::ReqAuthenticate   上报信息----> BrokerID:%s  \n",authenticatefield.BrokerID);
	printf("Trader::ReqAuthenticate   上报信息----> UserID:%s  \n",authenticatefield.UserID);
	printf("Trader::ReqAuthenticate   上报信息----> UserProductInfo:%s  \n",authenticatefield.UserProductInfo);
	printf("Trader::ReqAuthenticate   上报信息----> AuthCode:%s  \n",authenticatefield.AuthCode);
	printf("Trader::ReqAuthenticate   上报信息----> AppID:%s  \n",authenticatefield.AppID);

	m_requestID++;
	sleep(1);

	int rt_pTraderApi =this->m_pUserApi->ReqAuthenticate(&authenticatefield,m_requestID);

}

void MyTDSpi::Connect()
{
	CodeUtil codeutil;
	std::string flowpath=this->CheckFlowPath();
	printf("CTP_AutoConnect  --->>>>创建ctp进程 %s  \n",flowpath.c_str());
//	pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());
	this->m_pUserApi =  CThostFtdcTraderApi::CreateFtdcTraderApi(flowpath.c_str());


		// 产生一个事件处理的实例
//			this->_pTraderSpi = new CTraderSpi(m_pdatawrapper,_pTraderApi);
//			this->_pTraderSpi->SetStrategyName(this->GetStrategyName().c_str());
//
//			if(this->GetCTraderSpi()->GetLoginStatus()){
//					std::cout<<"5555555555555555555555555"<<endl;
//				}
//			else{
//				std::cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
//			}

//			printf("CTP_AutoConnect  --->>>>00000 \n");
//			std::string spilogfilename="traderspi";
//			spilogfilename.append("_");
//			spilogfilename.append(this->GetPointUserLoginField()->UserID);
//			spilogfilename.append(".log");
//			this->_pTraderSpi->SetLogName(this->GetPointUserLoginField()->UserID);
//			this->_pTraderSpi->SetLogPathFileName(spilogfilename.c_str());

	// 注册一事件处理的实例
		this->m_pUserApi->RegisterSpi(this);
		// 订阅私有流
		// TERT_RESTART:从本交易日开始重传
		// TERT_RESUME:从上次收到的续传
		// TERT_QUICK:只传送登录后私有流的内容
		this->m_pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
		///订阅公共流。
		///@param nResumeType 公共流重传方式
		///        THOST_TERT_RESTART:从本交易日开始重传
		///        THOST_TERT_RESUME:从上次收到的续传
		///        THOST_TERT_QUICK:只传送登录后公共流的内容
		///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
		this->m_pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);

		vector<std::string> list;
		Config config(this->GetStrategyName().c_str());
		config.GetFrontAddressLists(list);

		for (auto &address:list)
		{
			char temp[address.length()];
			strcpy(temp,address.c_str());
			printf("CTP_AutoConnect %s   ---> %s  \n",address.c_str(),temp);
			// 设置交易托管系统服务的地址，可以注册多个地址备用
			this->m_pUserApi->RegisterFront(temp);
		}
		vector<std::string>().swap(list);
}
	//*/
	void MyTDSpi::OpenOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay)
		{
		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		 Config config(this->GetStrategyName().c_str());
		MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
		mysqlorder.CreateTable();

		 OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay);
		 if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='0')  {//全部成交

			 info.Status=2;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		}
		 else if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='a') { //提交  未知

			 info.Status=1;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单已经提交-----后台状态'未知'---%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		}
		 else if ( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='0') { //全部成交

			 info.Status=2;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		}
		 else if( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='1') { //部分成交还在队列中

			 info.Status=1;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----部分成交还在队列中%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='2')  {//  部分成交不在队列中

			info.Status =3;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓部分成交不在队列中 -----%s-----",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='3') { //未成交还在队列中

			 info.Status=1;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单 未成交还在队列中 -----%s-----",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='4')  {//未成交不在队列中
			info.Status =4;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----未成交 不在队列中%s-----",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);
		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='5')  {//未成交不在队列中
			info.Status =22;
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----已经全部撤单 %s-----TradingDay= %s ",pOrder->InstrumentID,pOrder->TradingDay);  //
			logutil.WriteLog(buf);
		 }
		 else if(pOrder->OrderSubmitStatus =='4' && pOrder->OrderStatus =='5')	{

				sprintf(buf,"MyTDSpi::OpenOrder 平仓单被拒绝 撤单 %s TradingDay= %s  ",pOrder->InstrumentID,pOrder->TradingDay);  //
				logutil.WriteLog(buf);
				info.Status =-21;

			}
		 else {
			 sprintf(buf,"MyTDSpi::OpenOrder 开仓单 不明状态 TradingDay= %s  OrderSubmitStatus=%d  OrderStatus=%d",
					 pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
									logutil.WriteLog(buf);
		 }

		 if(!mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
			mysqlorder.NewOrderData_byRspQryOrder(info);
			sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----全部成交%s-----不存在orderinfo,新插入!!!",
					pOrder->InstrumentID);  //
			logutil.WriteLog(buf);
		}
		else	{

			 if(orderdata.Status != info.Status  && info.Status <50 && info.Status >-30)	{
				mysqlorder.UpdateOrderData_byRtnOrder(info);
				sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----[%d]    [%d]-----更新  [%s] orderinfo!!!!!!",
						orderdata.Status,info.Status,pOrder->InstrumentID);  //
				logutil.WriteLog(buf);
			}
			 else		 {
				if(orderdata.Status == info.Status && info.Status <50 && info.Status >-30){
						 sprintf(buf,"MyTDSpi::OpenOrder 开仓单-----[%d]    [%d]-----状态相同 无需更新  [%s] orderinfo!!!!!!",
							orderdata.Status,info.Status,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				 }
				 else{
					 sprintf(buf,"MyTDSpi::OpenOrder  平仓单-----[%d]    [%d]-----状态不明 待确认是否更新!!!   [%s] orderinfo!!!!!!",
								orderdata.Status,info.Status,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				 }

			 }

		}
	}


	void MyTDSpi::CloseOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay)
	{

		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		 Config config(this->GetStrategyName().c_str());
		MysqlOrder mysqlorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
		mysqlorder.CreateTable();
		 OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay);

		 if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='0') { //全部成交

			 info.Status=2;
			sprintf(buf,"CloseOrder:  平仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		}
		 else if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='a')  {//提交  未知

			 info.Status=1;

			sprintf(buf,"CloseOrder:  平仓单已经提交-----后台状态'未知'---%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		}
		 else if ( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='0')  {//全部成交

			 info.Status=2;

			sprintf(buf,"CloseOrder:  平仓单-----全部成交%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		}
		 else if( pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='1') { //部分成交还在队列中

			 info.Status=1;
			 sprintf(buf,"CloseOrder: 平仓部分成交还在队列中 ----%s-----", pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='2') { //  部分成交不在队列中

			info.Status =3;
			sprintf(buf,"CloseOrder:  平仓部分成交不在队列中 -----%s-----",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='3')  {//未成交还在队列中

			 info.Status=1;
			sprintf(buf,"CloseOrder: 平仓单 未成交还在队列中 -----%s-----",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='4') { //未成交不在队列中

			info.Status =4;
			sprintf(buf,"CloseOrder:  -----未成交 不在队列中%s-----",	pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

		 }
		 else if(pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='5')	{

			sprintf(buf,"CloseOrder: TradingDay= %s  平仓单撤单完成 %s ",pOrder->TradingDay,pOrder->InstrumentID);  //
			logutil.WriteLog(buf);
			info.Status =22;

		}
		 else if(pOrder->OrderSubmitStatus =='4' && pOrder->OrderStatus =='5')	{

			sprintf(buf,"CloseOrder: 平仓单被拒绝 撤单 TradingDay= %s  ",pOrder->TradingDay);  //
			logutil.WriteLog(buf);
			info.Status =-21;

		}
		 else	 {
				 sprintf(buf,"CloseOrder:平仓单 不明状态 TradingDay= %s  OrderSubmitStatus=%d  OrderStatus=%d",
					 pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
									logutil.WriteLog(buf);
				info.Status=-1;
		 }


		if(!mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
			mysqlorder.NewOrderData_byRspQryOrder(info);
			sprintf(buf,"CloseOrder:  平仓单----全部成交%s-----不存在orderinfo,新插入!!!",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);
		}
		else	{

			 if(orderdata.Status != info.Status  && info.Status <50 && info.Status >-30)			{
				mysqlorder.UpdateOrderData_byRtnOrder(info);
				sprintf(buf,"CloseOrder:  平仓单-----[%d]    [%d]----- 更新   [%s]   orderinfo!!!!!!",
						orderdata.Status,info.Status,pOrder->InstrumentID);  //
				logutil.WriteLog(buf);

				}
			 else 	{
				 if(orderdata.Status == info.Status && info.Status <50 && info.Status >-30){
					 sprintf(buf,"CloseOrder:  平仓单-----[%d]    [%d]-----状态相同 无需更新  [%s] orderinfo!!!!!!",
					 							orderdata.Status,info.Status,pOrder->InstrumentID);  //
					 				logutil.WriteLog(buf);
				 }
				 else{
					 sprintf(buf,"CloseOrder:  平仓单-----[%d]    [%d]-----状态不明 待确认是否更新!!!   [%s] orderinfo!!!!!!",
								orderdata.Status,info.Status,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				 }

			 }


		}
	}

	void MyTDSpi::OpenActionOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay)
		{

		char buf[1024];
		LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
		 Config config(this->GetStrategyName().c_str());
		MysqlOrder mysqlorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
		 OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay);

		 if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='a') { //提交  未知

				 info.Status=21;

				sprintf(buf,"Debug: OpenActionOrder 开仓单撤单已经提交-----后台状态'未知'---%s-----",	pOrder->InstrumentID);  //
				logutil.WriteLog(buf);

			}

		 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='3')  {//未成交还在队列中


			 info.Status=21;

			sprintf(buf,"Debug:OpenActionOrder 开仓单撤单 未成交还在队列中 -----%s-----",pOrder->InstrumentID);  //
			logutil.WriteLog(buf);

			 }
		 else if(pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='5')		{

				sprintf(buf,"Debug: OpenActionOrder TradingDay:%s  开仓单撤单完成  %s  ",pOrder->TradingDay,pOrder->InstrumentID);  //
				logutil.WriteLog(buf);
				info.Status =22;

			}
		 else	 {
			 sprintf(buf,"Debug: OpenActionOrder 开仓单撤单 不明状态 TradingDay= %s  OrderSubmitStatus=%d  OrderStatus=%d",
			 pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
				logutil.WriteLog(buf);
		 }


		 if(!mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
				mysqlorder.NewOrderData_byRspQryOrder(info);
				sprintf(buf,"Debug: OpenActionOrder 开仓单撤单-----全部成交%s-----不存在orderinfo,新插入!!!",
						pOrder->InstrumentID);  //
				logutil.WriteLog(buf);
			}
			else	{

				 if(orderdata.Status < info.Status )	{
					mysqlorder.UpdateOrderData_byRtnOrder(info);
					sprintf(buf,"Debug: OpenActionOrder 开仓单撤单-----[%d]    [%d]-----更新  [%s] orderinfo!!!!!!",
							orderdata.Status,info.Status,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				}
				 else		 {
					 sprintf(buf,"Debug: OpenActionOrder 开仓单撤单-----[%d]    [%d]-----状态相同 无需更新  [%s] orderinfo!!!!!!",
							orderdata.Status,info.Status,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				 }
			}
		}


	void MyTDSpi::CloseActionOrder( CThostFtdcOrderField * pOrder,OrderData info,const char* TradingDay)
			{

			char buf[1024];
			LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());


			 Config config(this->GetStrategyName().c_str());
			MysqlOrder mysqlorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
			 OrderData orderdata=mysqlorder.Find_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay);

			 if ( pOrder->OrderSubmitStatus =='0' && pOrder->OrderStatus =='a')  {//提交  未知

				 info.Status=21;
				sprintf(buf,"CloseActionOrder:  平仓单撤单已经提交-----后台状态'未知'---%s-----",	pOrder->InstrumentID);  //
				logutil.WriteLog(buf);

			}
			 else if(pOrder->OrderSubmitStatus =='3' &&  pOrder->OrderStatus =='3') { //未成交还在队列中

				 info.Status=21;
				sprintf(buf,"CloseActionOrder: 平仓单撤单 未成交还在队列中 -----%s-----",pOrder->InstrumentID);  //
				logutil.WriteLog(buf);

			 }
			 else if(pOrder->OrderSubmitStatus =='3' && pOrder->OrderStatus =='5')	{

					sprintf(buf,"CloseActionOrder:  TradingDay:%s  平仓单撤单完成  %s  ",pOrder->TradingDay,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
					info.Status =22;

			}
			 else	 {
				 sprintf(buf,"CloseActionOrder:  平仓单撤单 不明状态 TradingDay= %s  OrderSubmitStatus=%d  OrderStatus=%d",
				 pOrder->TradingDay,pOrder->OrderSubmitStatus-'0',pOrder->OrderStatus-'0');  //
					logutil.WriteLog(buf);
			 }


			 if(!mysqlorder.Exist_OrderbyInsOrderRef(info.InstrumentID,info.OrderRef,TradingDay))	{
				mysqlorder.NewOrderData_byRspQryOrder(info);
				sprintf(buf,"CloseActionOrder:  平仓单撤单-----全部成交%s-----不存在orderinfo,新插入!!!",
						pOrder->InstrumentID);  //
				logutil.WriteLog(buf);
			}
			else	{

				 if(orderdata.Status < info.Status )		{
					mysqlorder.UpdateOrderData_byRtnOrder(info);
					sprintf(buf,"CloseActionOrder:  平仓单撤单-----[%d]    [%d]-----更新  [%s] orderinfo!!!!!!",
							orderdata.Status,info.Status,pOrder->InstrumentID);  //
					logutil.WriteLog(buf);
				}
				 else	 {
					 sprintf(buf,"CloseActionOrder:  平仓单撤单-----[%d]    [%d]-----状态相同 无需更新  [%s] orderinfo!!!!!!",
															orderdata.Status,info.Status,pOrder->InstrumentID);  //
													logutil.WriteLog(buf);
				 }



			}


		}


void MyTDSpi::SetBrokerID(const char* id)
{
	this->m_brokerid=id;
}

std::string MyTDSpi::GetBrokerID()
{
	return this->m_brokerid;
}

void MyTDSpi::SetUserID(const char* id)
{
	this->m_userid=id;
}

std::string MyTDSpi::GetUserID()
{
	return this->m_userid;
}

void MyTDSpi::SetProductInfo(const char* info)
{
	this->m_productinfo=info;
}

std::string MyTDSpi::GetProductInfo()
{
	return this->m_productinfo;
}

void MyTDSpi::SetAuthCode(const char* code)
{
	this->m_authcode=code;
}

std::string MyTDSpi::GetAuthCode()
{
	return this->m_authcode;
}
void MyTDSpi::SetAppID(const char* id)
{
	this->m_appid=id;
}

std::string MyTDSpi::GetAppID()
{
	return this->m_appid;
}
