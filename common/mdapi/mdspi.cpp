#include "mdspi.hpp"



// mdapi.cpp :
// 一个简单的例子，介绍CThostFtdcTraderApi和CThostFtdcMdSpi接口的使用。
// 本例将演示一个报单录入操作的过程



// 报单录入操作是否完成的标志
// Create a manual reset event with no signal
// 这段需要注释掉，这是windows下的代码
// TODO 映射功能到linux
//HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);

void CMdSpi::SetRedisDepthMarket(RedisDepthMarket *rdepthmarket)
{
	if(this->_predisdepthmarket==nullptr){
		this->_predisdepthmarket=rdepthmarket;
	}

	printf("SetRedisDepthMarket   ---host    %s \n",this->_predisdepthmarket->GetHost().c_str());
}

RedisDepthMarket* CMdSpi::GetRedisDepthMarket()
{

//	printf("GetRedisDepthMarket   ---host    %s \n",this->_predisdepthmarket->GetHost().c_str());
	return this->_predisdepthmarket;
}

//void CMdSpi::SetRedisMinFlag(RedisMinFlag *minflag)
//{
//	if(this->_predisminflag==nullptr){
//		this->_predisminflag=minflag;
//	}
//
//
//}
//RedisMinFlag* CMdSpi::GetRedisMinFlag()
//{
//	return this->_predisminflag;
//}

void CMdSpi::SetMysqlMindata(MysqlMinData *pmysqlmindata)
{
	this->_pmysqlmindata=pmysqlmindata;

}
MysqlMinData* CMdSpi::GetMysqlMindata()
{
	return this->_pmysqlmindata;
}

void CMdSpi::SetMysqlProduct(MysqlProduct *pmysqlproduct)
{
	this->_pmysqlproduct=pmysqlproduct;

}
MysqlProduct* CMdSpi::GetMysqlProduct()
{
	return this->_pmysqlproduct;
}
void CMdSpi::SetMysqlInstrument(MysqlInstrument *pmysqlins)
{
	this->_pmysqlinstrument=pmysqlins;

}
MysqlInstrument* CMdSpi::GetMysqlInstrument()
{
	return this->_pmysqlinstrument;
}
void CMdSpi::SetMysqlCommission(MysqlCommission *pmysqlins)
{
	this->_pmysqlcommission=pmysqlins;

}
MysqlCommission* CMdSpi::GetMysqlCommission()
{
	return this->_pmysqlcommission;
}
void CMdSpi::SetFlagFrontDisConnect(bool flag)
{


	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_disconnect);/*判断上锁*/

	//		printf("SPI: SetFlagFrontDisConnect  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_FrontDisConnectFalg=flag;

			pthread_mutex_unlock(&this->_mutex_disconnect); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetFlagFrontConnectFalg  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetFlagFrontConnectFalg  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}
bool CMdSpi::GetFlagFrontDisConnect()
{


 	bool flag;
 	int ret;

 	bool flag_read=false;
 	while(!flag_read){

 		ret=pthread_mutex_trylock(&this->_mutex_disconnect);/*判断上锁*/
 //		printf("SPI: GetFlagFrontDisConnect  锁状态 %d   \n",ret);
 		if(ret!=EBUSY)	{
 			flag=this->_FrontDisConnectFalg;

 			pthread_mutex_unlock(&this->_mutex_disconnect); /*  解锁 */
 			flag_read=true;
 		}
 		else{
 			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MDSPI: GetFlagFrontDisConnect  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MDSPI: GetFlagFrontDisConnect  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);

 		}
 	}

 	return flag;

}
void CMdSpi::SetFlagFrontConnectFalg(bool flag)
{




	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_connect);/*判断上锁*/

	//		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_FrontConnectFalg=flag;

			pthread_mutex_unlock(&this->_mutex_connect); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetFlagFrontConnectFalg  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetFlagFrontConnectFalg  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}
bool CMdSpi::GetFlagFrontConnectFalg()
{

	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_connect);/*判断上锁*/
//		printf("SPI: GetFlagFrontConnectFalg  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			flag=this->_FrontConnectFalg;

			pthread_mutex_unlock(&this->_mutex_connect); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MDSPI: GetFlagFrontConnectFalg  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MDSPI: GetFlagFrontConnectFalg  ------->未拿到锁  %s  \n",str_localupdate.c_str());


			sleep(1);
		}
	}

	return flag;

}

void CMdSpi::SetConnectErrorCode(int code)
 {
//	printf("CMdSpi: SetConnectErrorCode  ----->begin!!!  \n");


	int nreason;
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_nReason);/*判断上锁*/

// 		printf("CMdSpi: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_nReason=code;

			pthread_mutex_unlock(&this->_mutex_nReason); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetConnectErrorCode  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetConnectErrorCode  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}
//	printf("CMdSpi: SetConnectErrorCode  ----->begin!!!  \n");

 }

 int CMdSpi::GetConnectErrorCode()
 {


	int nreason;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_nReason);/*判断上锁*/

 //		printf("SPI: GetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			nreason=this->_nReason;

			pthread_mutex_unlock(&_mutex_nReason); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MDSPI: GetConnectErrorCode  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MDSPI: GetConnectErrorCode  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

	 return nreason;
 }


 void CMdSpi::SetConnectCount(int count)
  {
 //	printf("CMdSpi: SetConnectCount  ----->begin!!!  \n");



 	int ret;

 	bool flag_write=false;
 	while(!flag_write){

 		ret=pthread_mutex_trylock(&this->_mutex_connectcount);/*判断上锁*/

 // 		printf("CMdSpi: SetConnectCount  锁状态 %d   \n",ret);
 		if(ret!=EBUSY)	{

 			this->_ConnectCount=count;

 			pthread_mutex_unlock(&this->_mutex_connectcount); /*  解锁 */
 			flag_write=true;
 		}
 		else{
 			DateUtil dateutil;
 			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
 			printf("CMdSpi: SetConnectCount  ----->未拿到锁  %s  \n",str_localupdate.c_str());
 			char logbuf[1024];
 			sprintf(logbuf,"CMdSpi: SetConnectCount  ------->未拿到锁  %s  \n",str_localupdate.c_str());
 			sleep(1);
 		}
 	}
 //	printf("CMdSpi: SetConnectCount  ----->begin!!!  \n");

  }

  int CMdSpi::GetConnectCount()
  {


 	int count;
 	int ret;

 	bool flag_read=false;
 	while(!flag_read){

 		ret=pthread_mutex_trylock(&this->_mutex_connectcount);/*判断上锁*/

  //		printf("SPI: GetConnectCount  锁状态 %d   \n",ret);
 		if(ret!=EBUSY)	{
 			count=this->_ConnectCount;

 			pthread_mutex_unlock(&_mutex_connectcount); /*  解锁 */
 			flag_read=true;
 		}
 		else{
 			DateUtil dateutil;
 			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
 			printf("MDSPI: GetConnectCount  ----->未拿到锁  %s  \n",str_localupdate.c_str());
 			char logbuf[1024];
 			sprintf(logbuf,"MDSPI: GetConnectCount  ------->未拿到锁  %s  \n",str_localupdate.c_str());
 			sleep(1);
 		}
 	}

 	 return count;
  }
 void CMdSpi::SetDifSec(int sec)
 {

	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_difsec);/*判断上锁*/

// 		printf("CMdSpi: SetDifSec  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_difsec=sec;

			pthread_mutex_unlock(&this->_mutex_difsec); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetDifSec  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetDifSec  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

 }

 int CMdSpi::GetDifSec()
 {


	int second;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_difsec);/*判断上锁*/

 //		printf("SPI: GetDifSec  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			second=this->_difsec;

			pthread_mutex_unlock(&_mutex_difsec); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MDSPI: GetDifSec  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MDSPI: GetDifSec  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

	 return second;

 }

int  CMdSpi::TryLockMutex()
{
int ret;
//	int ret=pthread_mutex_trylock(&mutex);/*判断上锁*/

	return ret;
}
int  CMdSpi::UnLockMutex()
{
int ret;
//	int ret=pthread_mutex_unlock(&mutex);/*解锁*/
//	printf("CTraderSpi::UnLockMutex:   解锁  \n");
	return ret;
}

void CMdSpi::SetFlagLogin(bool flag)
{

	int nreason;
	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_login);/*判断上锁*/

  //		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_flag_login=flag;

			pthread_mutex_unlock(&this->_mutex_login); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetFlagLogin  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetFlagLogin  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}

bool CMdSpi::GetFlagLogin()
{

	bool ret_login;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_login);/*判断上锁*/

	//		printf("SPI: GetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			ret_login=this->_flag_login;

			pthread_mutex_unlock(&_mutex_login); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("MDSPI: GetFlagLogin  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"MDSPI: GetFlagLogin  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

	 return ret_login;

}
void CMdSpi::SetFlagLogout(bool flag)
{



		int nreason;
		int ret;

		bool flag_write=false;
		while(!flag_write){

			ret=pthread_mutex_trylock(&this->_mutex_logout);/*判断上锁*/

	  //		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
			if(ret!=EBUSY)	{

				this->_flag_logout=flag;

				pthread_mutex_unlock(&this->_mutex_logout); /*  解锁 */
				flag_write=true;
			}
			else{
				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				printf("CMdSpi: SetFlagLogout  ----->未拿到锁  %s  \n",str_localupdate.c_str());
				char logbuf[1024];
				sprintf(logbuf,"CMdSpi: SetFlagLogout  ------->未拿到锁  %s  \n",str_localupdate.c_str());
				sleep(1);
			}
		}

}

bool CMdSpi::GetFlagLogout()
{

	bool ret_logout;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_logout);/*判断上锁*/

	//		printf("SPI: GetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			ret_logout=this->_flag_logout;

			pthread_mutex_unlock(&_mutex_logout); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: GetFlagLogout  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: GetFlagLogout  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

		 return ret_logout;

}

void CMdSpi::SetFlagMarketOpen(bool flag)
{
	 this->_flag_marketopen=flag;
}

bool CMdSpi::GetFlagMarketOpen()
{
	 return this->_flag_marketopen;
}
void CMdSpi::SetFlagSubmarket(bool flag)
{



	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_submarket);/*判断上锁*/

	//		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_Flag_Submarket=flag;

			pthread_mutex_unlock(&this->_mutex_submarket); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetFlagSubmarket  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetFlagSubmarket  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}

bool CMdSpi::GetFlagSubmarket()
{



	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_submarket);/*判断上锁*/
	//		printf("SPI: GetFlagFrontConnectFalg  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			flag=this->_Flag_Submarket;

			pthread_mutex_unlock(&this->_mutex_submarket); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: GetFlagSubmarket  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: GetFlagSubmarket  ------->未拿到锁  %s  \n",str_localupdate.c_str());


			sleep(1);
		}
	}

	return flag;

}

void CMdSpi::SetFlagUnSubmarket(bool flag)
{




	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_unsubmarket);/*判断上锁*/

	//		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			 this->_Flag_unSubmarket=flag;

			pthread_mutex_unlock(&this->_mutex_unsubmarket); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetFlagUnSubmarket  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetFlagUnSubmarket  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}

bool CMdSpi::GetFlagUnSubmarket()
{



		bool flag;
		int ret;

		bool flag_read=false;
		while(!flag_read){

			ret=pthread_mutex_trylock(&this->_mutex_unsubmarket);/*判断上锁*/
		//		printf("SPI: GetFlagFrontConnectFalg  锁状态 %d   \n",ret);
			if(ret!=EBUSY)	{
				flag=this->_Flag_unSubmarket;

				pthread_mutex_unlock(&this->_mutex_unsubmarket); /*  解锁 */
				flag_read=true;
			}
			else{
				DateUtil dateutil;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				printf("CMdSpi: GetFlagUnSubmarket  ----->未拿到锁  %s  \n",str_localupdate.c_str());
				char logbuf[1024];
				sprintf(logbuf,"CMdSpi: GetFlagUnSubmarket  ------->未拿到锁  %s  \n",str_localupdate.c_str());


				sleep(1);
			}
		}

		return flag;

}

void CMdSpi::SetFlagResetSubmarket(bool flag)
{




	int ret;

	bool flag_write=false;
	while(!flag_write){

		ret=pthread_mutex_trylock(&this->_mutex_resetsubmarket);/*判断上锁*/

	//		printf("SPI: SetConnectErrorCode  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{

			this->_ResetSubMarket_Flag=flag;

			pthread_mutex_unlock(&this->_mutex_resetsubmarket); /*  解锁 */
			flag_write=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: SetFlagResetSubmarket  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: SetFlagResetSubmarket  ------->未拿到锁  %s  \n",str_localupdate.c_str());
			sleep(1);
		}
	}

}

bool CMdSpi::GetFlagResetSubmarket()
{



	bool flag;
	int ret;

	bool flag_read=false;
	while(!flag_read){

		ret=pthread_mutex_trylock(&this->_mutex_resetsubmarket);/*判断上锁*/
	//		printf("SPI: GetFlagFrontConnectFalg  锁状态 %d   \n",ret);
		if(ret!=EBUSY)	{
			flag=this->_ResetSubMarket_Flag;

			pthread_mutex_unlock(&this->_mutex_resetsubmarket); /*  解锁 */
			flag_read=true;
		}
		else{
			DateUtil dateutil;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			printf("CMdSpi: GetFlagResetSubmarket  ----->未拿到锁  %s  \n",str_localupdate.c_str());
			char logbuf[1024];
			sprintf(logbuf,"CMdSpi: GetFlagResetSubmarket  ------->未拿到锁  %s  \n",str_localupdate.c_str());


			sleep(1);
		}
	}

	return flag;

}

 void CMdSpi::SetTradingDay(const char* date)
 {
 	this->_tradingday=date;
 }
 std::string CMdSpi::GetTradingDay()
 {

//	 DateUtil dateutil;
//	 double qbartime = dateutil.GetCurrentBarTime();
//
//	 time_t time_tradingday = dateutil.ConvertSqlTimetoTimet(dateutil.ConvertDatetoSqlTime(this->_tradingday.c_str()).c_str());
//
//	 time_t time_current =dateutil.ConvertSqlTimetoTimet(dateutil.ConvertDatetoSqlTime(dateutil.GetTodayString().c_str()).c_str());
//	 if(qbartime >0.2100)	 {
//
//		 if(time_tradingday <=time_current)		 {
////			 printf(" CMdSpi::GetTradingDay  夜盘未更新--> timet_tradingday %ld     timet_current %ld   \n",time_tradingday,time_current);
////			 printf(" CMdSpi::GetTradingDay  夜盘未更新 --> tradingday %s     current %s   \n",
////					 dateutil.ConvertTimetoSqlString(time_tradingday).c_str(),dateutil.ConvertTimetoSqlString(time_current).c_str());
//
//			 MysqlMdLogin mysqlmdlogin;
//			 this->_tradingday = mysqlmdlogin.Find_LastTradingDay().TradingDay;
//		 }
//		 else		 {
//
//		 }
//	 }
//	 else	 {
//		 if(time_tradingday <time_current)	 {
////			 printf(" CMdSpi::GetTradingDay  日盘未更新--> timet_tradingday %ld     timet_current %ld   \n",time_tradingday,time_current);
////			 printf(" CMdSpi::GetTradingDay  日盘未更新 --> tradingday %s     current %s   \n",
////					 dateutil.ConvertTimetoSqlString(time_tradingday).c_str(),dateutil.ConvertTimetoSqlString(time_current).c_str());
//
//			 MysqlMdLogin mysqlmdlogin;
//			 this->_tradingday = mysqlmdlogin.Find_LastTradingDay().TradingDay;
//		 }
//		 else {
//
//		 }
//	 }

 return this->_tradingday;
 }

 void CMdSpi::SetMnKlinesig(bool flag)
 {
	 this->_MnKlinesig=flag;
 }
 bool  CMdSpi::GetMnKlinesig()
  {
 	 return this->_MnKlinesig;
  }



 void CMdSpi::UpdateDifSec()
 {
// 	MysqlTradingDay mysqltradingday;
// 	TradingDayData tradingdaydata;
// 	tradingdaydata= mysqltradingday.Find_LastTradingDay();

// 	this->_difsec=tradingdaydata.DifSeconds;
 }
 int CMdSpi::GetTradingDayDifSec()
 {
 	UpdateDifSec();

 	return GetDifSec();
 }
void CMdSpi::SetCurrentQbarTime(int bartime)
{
	if(this->_current_qbar_time<bartime)
	{
		this->_current_qbar_time=bartime;
	}

}

int CMdSpi::GetCurrentQbarTime()
{
	return this->_current_qbar_time;
}
void CMdSpi::SetMindata_Database(const char* database)
{

		this->_mindata_database=database;

}

std::string CMdSpi::GetMindata_Database()
{
	return this->_mindata_database;
}
void CMdSpi::SetMindata_Tablename(const char* table)
{

		this->_mindata_tablename=table;

}

std::string CMdSpi::GetMindata_Tablename()
{
	return this->_mindata_tablename;
}

void CMdSpi::InitCommisionMap()
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
		std::cout << "[exception caught CMdSpi::InitCommisionMap >>> ]\n";
	}

	vector<Commission>().swap(list);
}

//std::map<std::string,double> * DataWrapper::GetCommissionMap()
//{
//return &m_commissionmap;
//}

std::shared_ptr<Commission>  CMdSpi::FindCommissionData(const char* id)
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
			std::cout << "[exception caught CMdSpi::FindCommission >>> ]\n";
			return nullptr;
	}
//	if(iter != m_commissionmap.end())	{
//	       cout<<"find margin---------->>>>>>  "<<iter->second<<endl;
//	}
//	else	{
//	       cout<<" not find"<<endl;
//	}

}

bool CMdSpi::UpdateContractFlag(ContractFlag &data)
{

	 map<std::string,std::shared_ptr<ContractFlag> >::iterator iter;
	 bool ret=false;
	try {
		std::lock_guard<std::mutex>lck(m_contractflagmutex);  // 函数结束时，自动析构解锁
		std::shared_ptr<ContractFlag> e = std::make_shared<ContractFlag>(data);
		std::string key =data.InstrumentID;
		if (m_contractflagmap.find(key) != m_contractflagmap.end()) 		{
			m_contractflagmap.erase(key);
			m_contractflagmap.insert(map<string,std::shared_ptr<ContractFlag>>::value_type(key,e));
		}
		else{
			m_contractflagmap.insert(map<string,std::shared_ptr<ContractFlag>>::value_type(key,e));
		}

		ret=true;

	} catch(std::logic_error&) {
		std::cout << "[exception caught CMdSpi::UpdateContractFlag >>> ]\n";
		ret=false;

	}
	return ret;
}

ContractFlag CMdSpi::FindContractFlag(const char* instrumentid)
{
	ContractFlag  contractflag;
	memset(&contractflag,0,sizeof(contractflag));
	try {
		std::lock_guard<std::mutex>lck(m_contractflagmutex);  // 函数结束时，自动析构解锁

		  std::string key =instrumentid;


		 map<std::string,std::shared_ptr<ContractFlag> >::iterator iter;
		iter = m_contractflagmap.find(key);
		if(iter != m_contractflagmap.end()){

			strcpy(contractflag.InstrumentID, iter->second->InstrumentID);

			contractflag.MnKlinesig = iter->second->MnKlinesig;
			contractflag.last_open = iter->second->last_open;
			contractflag.last_high = iter->second->last_high;
			contractflag.last_low = iter->second->last_low;
			contractflag.last_close = iter->second->last_close;

			contractflag.last_opi = iter->second->last_opi;
			contractflag.last_volume_open = iter->second->last_volume_open;
			contractflag.last_volume = iter->second->last_volume;
			strcpy(contractflag.FirstTickTime ,iter->second->FirstTickTime);
			strcpy(contractflag.LastTickTime ,iter->second->LastTickTime);
		}
		else{
						  cout<<"++++++++FindContractFlag   ---->Do not Find"<<endl;

		}

	} catch(std::logic_error&) {
		std::cout << "[exception caught CMdSpi::FindContractFlag >>> ]\n";

	}
	return contractflag;
}
bool CMdSpi::ExistContractFlag(const char* instrumentid)
{

	std::string key=instrumentid;
	map<std::string,std::shared_ptr<ContractFlag> >::iterator iter;
	iter = m_contractflagmap.find(key);
	if(iter != m_contractflagmap.end()){
		return true;
	}
	return false;

}
 // 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
    //*
void CMdSpi::OnFrontConnected()
{
	printf("CMdSpi:OnFrontConnected----> Begin!!!    \n");

	char buf[1024];
	this->GetLogUtil()->PrintLog("CMdSpi:OnFrontConnected----> Begin!!!    \n");
//	static int connectnumber=0;

	/////// 锁定状态下 修改共享数据
	DateUtil dateutil;
	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);
	double Q_BarTime_2 = dateutil.GetCurrentBarTime();
	bool Break_afternoon = (Q_BarTime_2>=0.1600 && Q_BarTime_2<0.2030);

	bool opentime= dateutil.CheckMDOpenTime();
	bool Timemore=( Break_afternoon );

	this->SetFlagFrontConnectFalg(true);
	this->SetFlagFrontDisConnect(false);
	int count = this->GetConnectCount();

	if (this->GetConnectErrorCode()==0  ){

		if(count==0){
			printf("CMdSpi:OnFrontConnected --->>>  首次网络链路连接成功!!!    %s\n",dateutil.GetDateTimeString().c_str());
			sprintf(buf,"CMdSpi:OnFrontConnected --->>>  首次网络链路连接成功!!!    %s\n",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
			sleep(1);
		}
		else{
			printf("CMdSpi:OnFrontConnected--->>>  无错误码状态----->> 网络链路连接成功!!!    %s\n",dateutil.GetDateTimeString().c_str());
			sprintf(buf,"CMdSpi:OnFrontConnected--->>>  无错误码状态----->> 网络链路连接成功!!!    %s\n",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
			sleep(1);
		}

	}
	else{
		printf("CMdSpi:OnFrontConnected--->>>  侦测到连接异常后[errorcode:%d], 自动连接成功后,将错误码归零!!!    %s \n",
		this->GetConnectErrorCode(),dateutil.GetDateTimeString().c_str());
		sprintf(buf,"CMdSpi:OnFrontConnected--->>>  侦测到连接异常后[errorcode:%d], 自动连接成功后,将错误码归零!!!    %s \n",
				this->GetConnectErrorCode(),dateutil.GetDateTimeString().c_str());
		this->GetLogUtil()->PrintLog(buf);

		SetConnectErrorCode(0);
	}



	if (count==0){
		printf("CMdSpi::OnFrontConnected 程序启动时----> %s  第一次连接 释放sem信号 \n",dateutil.GetDateTimeString().c_str());
		sprintf(buf,"CMdSpi::OnFrontConnected 程序启动时----> %s  第一次连接 释放sem信号 \n",dateutil.GetDateTimeString().c_str());
		this->GetLogUtil()->PrintLog(buf);

		this->PostSemInit();

	}
	else {

		if(opentime){
			printf("CMdSpi::OnFrontConnected 盘中----> %s  重新连接\n",dateutil.GetDateTimeString().c_str());
			sprintf(buf,"CMdSpi::OnFrontConnected 盘中----> %s 重新连接\n",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);
		}
		else {

			if (Timemore){

				int delaytime= 500+dateutil.GetRandomNumber();
				printf("CMdSpi::OnFrontConnected 非开盘时间----> %s  延迟%d秒后 重新连接\n",dateutil.GetDateTimeString().c_str(),delaytime);
				sprintf(buf,"CMdSpi::OnFrontConnected 非开盘时间----> %s  延迟%d秒后 重新连接\n",dateutil.GetDateTimeString().c_str(),delaytime);
				this->GetLogUtil()->PrintLog(buf);

				sleep(delaytime);
			}
		}
	}

	count++;
	this->SetConnectCount(count);
	sleep(1);
	printf("CMdSpi:OnFrontConnected----> End!!!    \n");
	this->GetLogUtil()->PrintLog("CMdSpi:OnFrontConnected----> End!!!    \n");
}
	//*/

// 当客户端与交易托管系统通信连接断开时，该方法被调用
//*
void CMdSpi::OnFrontDisconnected(int nReason)
{
	printf("CMdSpi:OnFrontDisconnected----> %d  Begin!!!   \n",nReason);

	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	printf("CMdSpi:OnFrontDisconnected---->连接断开错误码------[%d]    \n",nReason);
	SetConnectErrorCode(nReason);


	char logbuf[1024];

	DateUtil dateutil;
	sprintf(logbuf,"CMdSpi -------->>>  OnFrontDisconnected   %s",dateutil.GetDateTimeString().c_str());

	this->GetLogUtil()->PrintLog(logbuf);
	bool opentime= dateutil.CheckMDOpenTime();

	this->SetFlagFrontConnectFalg(false);
	this->SetFlagFrontDisConnect(true);

	this->SetFlagLogin(false);
//	this->SetFlagLogout(false);
	this->SetFlagResetSubmarket(false);
	this->SetFlagSubmarket(false);



	switch(nReason)		{
		case 0x1001:
			printf("CMdSpi:网络读失败 =>  %d    %s  \n",nReason,dateutil.GetDateTimeString().c_str());
			sprintf(logbuf,"CMdSpi:网络读失败 =>  %d    %s  \n",nReason,dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(logbuf);

			if(opentime){
				sleep(3);
			}
			else {
				sleep(200+dateutil.GetRandomNumber());
			}

			break;
		case 0x1002:
			printf("CMdSpi:网络写失败 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			sprintf(logbuf,"CMdSpi:网络写失败 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(logbuf);
			if(opentime){
				sleep(3);
			}
			else {
				sleep(200+dateutil.GetRandomNumber());
			}
			break;
		case 0x2001:
			printf("CMdSpi:接收心跳超时 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			sprintf(logbuf,"CMdSpi:接收心跳超时 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(logbuf);
			if(opentime){
				sleep(3);
			}
			else {
				sleep(200+dateutil.GetRandomNumber());
			}
			break;
		case 0x2002:
			printf("CMdSpi:发送心跳失败 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			sprintf(logbuf,"CMdSpi:发送心跳失败 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(logbuf);
			if(opentime){
				sleep(3);
			}
			else {
				sleep(200+dateutil.GetRandomNumber());
			}
			break;
		case 0x2003:
			printf("CMdSpi:收到错误报文 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			sprintf(logbuf,"CMdSpi:收到错误报文 =>  %d    %s    \n",nReason,dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(logbuf);
			if(opentime){
				sleep(3);
			}
			else {
				sleep(200+dateutil.GetRandomNumber());
			}
			break;
		default:
			break;
	}
	printf("CMdSpi:OnFrontDisconnected----> %d      End !!! \n",nReason);
}
	//*/

    // 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
    //*
	void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
        CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
		char buf[1024];

        printf("CMdSpi:OnRspUserLogin:\n");

        this->GetLogUtil()->PrintLog("CMdSpi:OnRspUserLogin():被执行...Begin!!! \n");

//        printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
//            pRspInfo->ErrorMsg);
//        printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
        if (pRspInfo->ErrorID != 0) {

        	char ErrorMsg[90];
			gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));

            // 端登失败，客户端需进行错误处理
            printf("CMdSpi:Failed to login, errorcode=%d errormsg=%s requestid=%d chain=%d",
            pRspInfo->ErrorID, ErrorMsg, nRequestID, bIsLast);
            exit(-1);
        }else{        

        	DateUtil dateutil;
			sprintf(buf,"CMdSpi -----OnRspUserLogin--->>> 登录成功   %s ",dateutil.GetDateTimeString().c_str());
			this->GetLogUtil()->PrintLog(buf);

			printf("CMdSpi: -----登录成功    shfetime %s \n",pRspUserLogin->SHFETime);


			printf("CMdSpi: -----登录成功    时间差 %d\n",this->GetDifSec());
//			printf("CMdSpi: BrokerID= %s  \n",pRspUserLogin->BrokerID);  //
//			printf("CMdSpi: FrontID= %d  \n",pRspUserLogin->FrontID);
//			printf("CMdSpi: SessionID= %d  \n",pRspUserLogin->SessionID);
//			printf("CMdSpi: SystemName= %s  \n",pRspUserLogin->SystemName);
//			printf("CMdSpi: MaxOrderRef= %s  \n",pRspUserLogin->MaxOrderRef);
//
//			printf("CMdSpi CZCETime = %s \n",pRspUserLogin->CZCETime);
//			printf("CMdSpi: DCETime= %s  \n",pRspUserLogin->DCETime);
//			printf("CMdSpi: FFEXTime= %s  \n",pRspUserLogin->FFEXTime);
//			printf("CMdSpi: SHFETime= %s  \n",pRspUserLogin->SHFETime);
//			printf("CMdSpi: INETime= %s  \n",pRspUserLogin->INETime);
//
//			printf("SPI: LoginTime= %s  \n",pRspUserLogin->LoginTime);
			printf("CMdSpi: TradingDay= %s  \n",pRspUserLogin->TradingDay);

			int difsec=dateutil.CollectDifSecBySHFETime(pRspUserLogin->SHFETime);
			this->SetDifSec(difsec);
			sprintf(buf,"CMdSpi: SHFETime= %s  时间差  =%d  TradingDay= %s   \n",pRspUserLogin->SHFETime,
					this->GetDifSec(),pRspUserLogin->TradingDay);
			this->GetLogUtil()->PrintLog(buf);

			TradingDayData tradingdayinfo;
			strcpy(tradingdayinfo.TradingDay,pRspUserLogin->TradingDay);
			strcpy(tradingdayinfo.MaxOrderRef,pRspUserLogin->MaxOrderRef);
			strcpy(tradingdayinfo.DCETime,pRspUserLogin->DCETime);
			strcpy(tradingdayinfo.CZCETime,pRspUserLogin->CZCETime);
			strcpy(tradingdayinfo.SHFETime,pRspUserLogin->SHFETime);
			strcpy(tradingdayinfo.INETime,pRspUserLogin->INETime);
			strcpy(tradingdayinfo.LoginTime,pRspUserLogin->LoginTime);
			tradingdayinfo.FrontID = pRspUserLogin->FrontID;
			tradingdayinfo.SessionID = pRspUserLogin->SessionID;
			tradingdayinfo.DifSeconds = this->GetDifSec();

			this->SetTradingDay(pRspUserLogin->TradingDay);

			strcpy(tradingdayinfo.TradingDateTime,dateutil.ConvertDatetoSqlTime(tradingdayinfo.TradingDay).c_str());

			MysqlMdLogin mysqlmdlogin;

			if(!mysqlmdlogin.Exist_DateTime(tradingdayinfo.TradingDateTime))	{
				printf("CMdSpi: -----登录成功,插入新数据:  %s   本地-后台时间差 %d 秒\n", tradingdayinfo.TradingDay,this->GetDifSec());
				mysqlmdlogin.Insert(tradingdayinfo);
			}
//			sprintf(buf,"CMdSpi -----OnRspUserLogin----->GetTradingDay:  %s", tradingday.c_str());
//			this->GetLogUtil()->PrintLog(buf);CMdSpi
		}

        // 如果响应函数已经返回最后一条信息
		if(bIsLast) {
			// 通知主过程，响应函数将结束
			sprintf(buf,"CMdSpi -----OnRspUserLogin----->释放信号semlogin:  %s", pRspUserLogin->TradingDay);
			this->GetLogUtil()->PrintLog(buf);

			this->SetFlagLogin(true);

			this->SetFlagSubmarket(false);
			this->PostSemLogin();
		}

		this->GetLogUtil()->PrintLog("CMdSpi:OnRspUserLogin():被执行...End!!! ");
		
    }
    




void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "SPI:--->>> 成功订阅合约:" <<"_"<<pSpecificInstrument->InstrumentID<< endl;

	 char buf[200];

	this->GetLogUtil()->PrintLog("SPI:OnRspSubMarketData():被执行...");


//	    sprintf(buf,"OnRspSubMarketData:\n");
//	    this->GetLogUtil()->PrintLog(buf);
//	           sprintf(buf,"ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
//	               pRspInfo->ErrorMsg);
//	           this->GetLogUtil()->PrintLog(buf);
//	           sprintf(buf,"RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
//	           this->GetLogUtil()->PrintLog(buf);
	           if (pRspInfo->ErrorID != 0) {

	           	char ErrorMsg[90];
	           			gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));

	               // 端登失败，客户端需进行错误处理
	               sprintf(buf,"Failed to OnRspSubMarketData, errorcode=%d errormsg=%s requestid=%d chain=%d",
	               pRspInfo->ErrorID, ErrorMsg, nRequestID, bIsLast);
	               this->GetLogUtil()->PrintLog(buf);
	               exit(-1);
	           }else{

//	        	   this->GetLogUtil()->PrintLog("=====订阅行情成功=====");
//	        	   sprintf(buf,"=====订阅行情成功===== 合约代码：  %s ", pSpecificInstrument->InstrumentID);
//	        	   this->GetLogUtil()->PrintLog(buf);


	   		}

	           // 如果响应函数已经返回最后一条信息
	           		if(bIsLast) {
	           			// 通知主过程，响应函数将结束
	           			this->SetFlagUnSubmarket(false);
					   this->SetFlagSubmarket(true);
					   this->SetFlagResetSubmarket(true);
					   sprintf(buf,"CMdSpi -----OnRspSubMarketData=====订阅行情完成!!!! ");
					   this->GetLogUtil()->PrintLog(buf);
//	           			sem_post(&sem_subscribe);
	           			this->PostSemSubscribe();
	           		}
}



void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << __FUNCTION__ << endl;

	 char buf[200];

	this->GetLogUtil()->PrintLog("CMdSpi::OnRspUnSubMarketData():被执行...");
//
//    sprintf(buf,"OnRspUnSubMarketData:\n");
//    this->GetLogUtil()->PrintLog(buf);
//	   sprintf(buf,"ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,
//		   pRspInfo->ErrorMsg);
//	   this->GetLogUtil()->PrintLog(buf);
//	   sprintf(buf,"RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
//	   this->GetLogUtil()->PrintLog(buf);
	   if (pRspInfo->ErrorID != 0) {

		char ErrorMsg[90];
				gbk2utf8(pRspInfo->ErrorMsg,ErrorMsg,sizeof(ErrorMsg));

		   // 端登失败，客户端需进行错误处理
		   sprintf(buf,"Failed to OnRspUnSubMarketData, errorcode=%d errormsg=%s requestid=%d chain=%d",
		   pRspInfo->ErrorID, ErrorMsg, nRequestID, bIsLast);
		   this->GetLogUtil()->PrintLog(buf);
		   exit(-1);
	   }else{


//		  this->GetLogUtil()->PrintLog("=====取消订阅行情成功=====");
//		   sprintf(buf,"=====取消订阅行情成功===== 取消合约代码： %s", pSpecificInstrument->InstrumentID);
//		   this->GetLogUtil()->PrintLog(buf);


	}

   // 如果响应函数已经返回最后一条信息
		if(bIsLast) {

			this->SetFlagUnSubmarket(true);
			this->SetFlagSubmarket(false);
			 sprintf(buf,"CMdSpi::OnRspUnSubMarketData=====取消订阅行情完成!!!! ");
			 this->GetLogUtil()->PrintLog(buf);
			// 通知主过程，响应函数将结束
//			sem_post(&sem_unsubscribe);
			this->PostSemUnSubscribe();
		}
}



void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	DateUtil dateutil;
	CodeUtil codeutil;
//struct timeval tv;
//struct timeval tv_last;
//long s,l;
//    gettimeofday(&tv,NULL);
////    printf("second:%ld\n",tv.tv_sec);  //秒
////    printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
////    printf("OnRtnDepthMarketData begin  ---->%ld.%ld    %s  %d \n",tv.tv_sec,tv.tv_usec,pDepthMarketData->UpdateTime,
////    		pDepthMarketData->UpdateMillisec);  //秒
//
//    s =tv.tv_sec*1000 + tv.tv_usec/1000;
//	printf("DayTradeSystem:开始启动程序  --->>>>%s \n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
//	printf("SPI--->>>  OnRtnDepthMarketData 回调函数执行...  ------->>>> 开始   %s\n",dateutil.ConvertCurrentTimetoSqlTime().c_str());
//	char buf[2048];
//

	// 如果有返回结果读取返回信息
	if ( pDepthMarketData != NULL  && pDepthMarketData->AskPrice1 <_max_price  &&  pDepthMarketData->AskPrice1 >0
			&& pDepthMarketData->BidPrice1 <_max_price && pDepthMarketData->BidPrice1 >0) {

	//	printf("SPI--->>>  OnRtnDepthMarketData 回调函数执行...000\n");
		//保存tick 行情至内存数据库
		SaveTickDatatoRedis(pDepthMarketData);
	//	printf("SPI--->>>  OnRtnDepthMarketData 回调函数执行...111\n");
//		DateUtil dateutil;




		char productid[31];

		strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

		bool night_product=codeutil.IsNightByProductid(productid);   // 查询该合约是否是夜盘合约

		string mindatabase="real_minute_";
		mindatabase.append(productid);
		this->SetMindata_Database(mindatabase.c_str());
		string mindata_tablename;
		mindata_tablename=pDepthMarketData->InstrumentID;
		mindata_tablename.append("_");

//		string str_currentdate = dateutil.GetCurrentDayString();

		int Q_BarTime	= dateutil.CoverMinuteTimetoIntBarTime(pDepthMarketData->UpdateTime);


//		bool Timemore_night = (Q_BarTime>=200000 && Q_BarTime<240000);
//		if (Timemore_night){
//			if(strcmp(str_currentdate.c_str(),pDepthMarketData->TradingDay)==0){
//				mindata_tablename.append(dateutil.GetTradingDaybyCurrentDay().substr(0,8));
//				this->SetMindata_Tablename(mindata_tablename.c_str());
//				printf("SPI:   [夜盘   ]  保存日线分钟数据   修正后的日线表名  >>>>>>>   database: %s   table: %s \n",mindatabase.c_str(),mindata_tablename.c_str());
//			}
//			else{
//				mindata_tablename.append(pDepthMarketData->TradingDay);
//				this->SetMindata_Tablename(mindata_tablename.c_str());
//		//		printf("SPI:   [夜盘   ]  保存日线分钟数据   database: %s   table: %s \n",mindatabase.c_str(),mindata_tablename.c_str());
//			}
//
//		}
//		else{
//			mindata_tablename.append(pDepthMarketData->TradingDay);
//			this->SetMindata_Tablename(mindata_tablename.c_str());
//	//		printf("SPI:   [日盘   ]  保存日线分钟数据   database: %s   table: %s \n",mindatabase.c_str(),mindata_tablename.c_str());
//		}

		mindata_tablename.append(this->GetTradingDay());
		this->SetMindata_Tablename(mindata_tablename.c_str());
//		printf("SPI:   保存日线分钟数据   database: %s   table: %s \n",mindatabase.c_str(),mindata_tablename.c_str());
		//时间采用秒计

		//私有变量 后台时间
		this->SetCurrentQbarTime(Q_BarTime);
		//毫秒
//		double  Millisec = pDepthMarketData->UpdateMillisec;
	//		printf("SPI:   EXCHANGEID   %s   productid  %s \n",pDepthMarketData->ExchangeID,productid);
		//根据不同合约的交易时段进行判断 当前数据是否符合交易时段
	//		bool isopen=dateutil.CheckMDOpenTimebyInstrumentID(_Q_BarTime_2,pDepthMarketData->ExchangeID,productid);

//		std::string str_time=pDepthMarketData->UpdateTime;

	//		int hours = atoi(str_time.substr(0,2).c_str());
//		int minutes = atoi(str_time.substr(3,2).c_str());
//		int seconds = atoi(str_time.substr(6,2).c_str());

	//	if(seconds==0 || seconds==59)
	//	{
	//		printf ("--->>>  %s   UpdateTime:%s   %.f   actionday:%s   tradingday:%s lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
	//				pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,Millisec,pDepthMarketData->ActionDay,
	//				pDepthMarketData->TradingDay,pDepthMarketData->LastPrice,
	//				pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);
	//	}

	//		if( strcmp(pDepthMarketData->InstrumentID,"rb1910")==0 )	{
//				printf ("--->>>  %s  ExchangeID:%s  %s  %d   >>> lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
//						pDepthMarketData->InstrumentID,pDepthMarketData->ExchangeID,pDepthMarketData->UpdateTime,pDepthMarketData->UpdateMillisec,
//						pDepthMarketData->LastPrice,pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);
	//			printf ("--->>>  %s   >>> askprice2: %.3f   bidprice2: %.3f  AskVolume2 : %d  BidVolume2 : %d\n",
	//					pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice2,pDepthMarketData->BidPrice2,pDepthMarketData->AskVolume2,
	//					pDepthMarketData->BidVolume2);
	//			printf ("--->>>  %s   >>> askprice3: %.3f   bidprice3: %.3f  AskVolume3 : %d  BidVolume3 : %d\n",
	//					pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice3,pDepthMarketData->BidPrice3,pDepthMarketData->AskVolume3,
	//					pDepthMarketData->BidVolume3);
	//			printf ("--->>>  %s   >>> askprice4: %.3f   bidprice4: %.3f  AskVolume4 : %d  BidVolume4 : %d\n",
	//					pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice4,pDepthMarketData->BidPrice4,pDepthMarketData->AskVolume4,
	//					pDepthMarketData->BidVolume4);
	//			printf ("--->>>  %s   >>> askprice5: %.3f   bidprice5: %.3f  AskVolume5 : %d  BidVolume5 : %d\n",
	//					pDepthMarketData->InstrumentID,pDepthMarketData->AskPrice5,pDepthMarketData->BidPrice5,pDepthMarketData->AskVolume5,
	//					pDepthMarketData->BidVolume5);
	//		}


//		if (this->GetRedisMinFlag()->Exist_DatabyInstrumentID(pDepthMarketData->InstrumentID)!=1)	{
		bool existflag=ExistContractFlag(pDepthMarketData->InstrumentID);
		if(!existflag){
			ContractFlag contractflag;
			strcpy(contractflag.InstrumentID, pDepthMarketData->InstrumentID);
			contractflag.MnKlinesig=true;   // 异常启动时,从中间段数据开始,该分钟数据  无需写入数据库
			this->SetMnKlinesig(contractflag.MnKlinesig);

			contractflag.last_open =pDepthMarketData->LastPrice;
			contractflag.last_high =pDepthMarketData->LastPrice;
			contractflag.last_low =pDepthMarketData->LastPrice;
			contractflag.last_close =pDepthMarketData->LastPrice;

			contractflag.last_opi=pDepthMarketData->OpenInterest;
			contractflag.last_volume = pDepthMarketData->Volume;
			contractflag.last_volume_open = pDepthMarketData->Volume;
			strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
			strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

//			this->GetRedisMinFlag()->SetFlag(contractflag);
			this->UpdateContractFlag(contractflag);

			printf ("  %s  %s MnKlinesig不存在，初始化-----> %d    -------->%d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,Q_BarTime,_MnKlinesig);
		}
		else{
			ContractFlag contractflag;
//			contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
			contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
			//同步对应合约的内存数据 k线读写标志
			this->SetMnKlinesig(contractflag.MnKlinesig);
//			printf ("开始写入分钟数据前，读取MnKlinesig-----> %s    -------->%d \n",pDepthMarketData->UpdateTime,contractflag.MnKlinesig);
		}

//		gettimeofday(&tv_last,NULL);
//		 l=tv_last.tv_sec*1000 + tv_last.tv_usec/1000;
//		if (this->GetRedisMinFlag()->Exist_DatabyInstrumentID(pDepthMarketData->InstrumentID)!=1)	{
//			ContractFlag contractflag;
//			strcpy(contractflag.InstrumentID, pDepthMarketData->InstrumentID);
//			contractflag.MnKlinesig=true;   // 异常启动时,从中间段数据开始,该分钟数据  无需写入数据库
//			this->SetMnKlinesig(contractflag.MnKlinesig);
//
//			contractflag.last_open =pDepthMarketData->LastPrice;
//			contractflag.last_high =pDepthMarketData->LastPrice;
//			contractflag.last_low =pDepthMarketData->LastPrice;
//			contractflag.last_close =pDepthMarketData->LastPrice;
//
//			contractflag.last_opi=pDepthMarketData->OpenInterest;
//			contractflag.last_volume = pDepthMarketData->Volume;
//			contractflag.last_volume_open = pDepthMarketData->Volume;
//			strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
//			strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);
//			this->GetRedisMinFlag()->SetFlag(contractflag);
//
//			printf ("  %s  %s MnKlinesig不存在，初始化-----> %d    -------->%d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,Q_BarTime,_MnKlinesig);
//		}
//		else{
//			ContractFlag contractflag;
//			contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
//			//同步对应合约的内存数据 k线读写标志
//			this->SetMnKlinesig(contractflag.MnKlinesig);
//	//		printf ("开始写入分钟数据前，读取MnKlinesig-----> %s    -------->%d \n",pDepthMarketData->UpdateTime,contractflag.MnKlinesig);
//		}

			//***************************************************
			bool Financial = strcmp(productid,"IC")==0 || strcmp(productid,"IF")==0 || strcmp(productid,"IH")==0
									|| strcmp(productid,"T")==0 || strcmp(productid,"TF")==0;

			if ( night_product && !Financial) {

	//			printf(" night  ---> %s     \n",pDepthMarketData->InstrumentID);
	//			if(strcmp(pDepthMarketData->InstrumentID,"rb1810")==0 && (seconds==0 || seconds==59) )
	//			{
	//				sprintf (buf,"SPI:--->>>  %s   %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f   volume: %d \n",
	//					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
	//					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);
	//
	//				this->GetLogUtil()->PrintLog(buf);
	//
	//
	//			}

				NightProduct_0s_DepthMarketData(pDepthMarketData);

			}
			else if ( !night_product && !Financial)		{
	//			printf(" day  ---> %s     \n",pDepthMarketData->InstrumentID);


	//			if(strcmp(pDepthMarketData->InstrumentID,"jd1809")==0 && (seconds==0 || seconds==59))
	//			{
	//				sprintf (buf,"SPI:--->>>  %s   %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f   volume: %d \n",
	//					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
	//					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);
	//
	//				this->GetLogUtil()->PrintLog(buf);
	//			}

				DayProduct_0s_DepthMarketData(pDepthMarketData);

			}
			else if (  Financial)	{
	//			printf(" Financial  ---> %s     \n",pDepthMarketData->InstrumentID);
	//			FinancialProduct_DepthMarketData(pDepthMarketData);
			}
			else		{
				printf(" Other  ---> %s     \n",pDepthMarketData->InstrumentID);
			}


	}
	else	{


		if (pDepthMarketData->AskPrice1 >_max_price && pDepthMarketData->BidPrice1 >_max_price )		{
//			  printf ("-------->>> DepthMarketData  数据不合规----------------->>>!!!!! \n");

			printf ("DepthMarketData  %s 数据不合规---->>>     %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);

		}
		else if(pDepthMarketData->AskPrice1 <_max_price && pDepthMarketData->BidPrice1 >_max_price ){

			 printf ("DepthMarketData  %s 跌停价---->>>     %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);

			SaveTickDatatoRedis(pDepthMarketData);
		}
		else if(pDepthMarketData->AskPrice1 <_max_price && pDepthMarketData->BidPrice1 ==0 ){

			 printf ("DepthMarketData  %s 跌停价---->>>     %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);

			SaveTickDatatoRedis(pDepthMarketData);
		}
		else if(pDepthMarketData->AskPrice1 >_max_price && pDepthMarketData->BidPrice1 <_max_price ){

			 printf ("DepthMarketData  %s 涨停价---->>>     %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);

			SaveTickDatatoRedis(pDepthMarketData);
		}
		else if(pDepthMarketData->AskPrice1 ==0 && pDepthMarketData->BidPrice1 <_max_price ){

			 printf ("DepthMarketData  %s 涨停价---->>>     %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f  volume : %d\n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
					pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);

			SaveTickDatatoRedis(pDepthMarketData);
		}
		else	{
			printf ("-------->>> %s DepthMarketData 空数据----------------->>>!!!!! \n",pDepthMarketData->InstrumentID);
		}
	}


////	 printf("OnRtnDepthMarketData end  ---->%ld.%ld\n",tv_last.tv_sec,tv_last.tv_usec);  //秒
//////	    printf("second:%ld\n",tv_last.tv_sec);  //秒
//////	    printf("millisecond:%ld\n",tv_last.tv_sec*1000 + tv_last.tv_usec/1000);  //毫秒
////

//
//
//	printf("SPI--->>>  OnRtnDepthMarketData 回调函数执行...  %s ------->>>> 结束   [%ld.%ld]   [%s  %d] 消耗  %ld \n",
//			pDepthMarketData->InstrumentID,tv_last.tv_sec,tv_last.tv_usec,
//			pDepthMarketData->UpdateTime,pDepthMarketData->UpdateMillisec,l-s);
}

	//*/

    // 针对用户请求的出错通知
    //*
void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
        bool bIsLast)
    {
		char buf[1024];



        printf("OnRspError:\n");
        printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,  pRspInfo->ErrorMsg);
        printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

        sprintf(buf,"OnRspError:\n");
        this->GetLogUtil()->PrintLog(buf);
	   sprintf(buf,"ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID,  pRspInfo->ErrorMsg);
	   this->GetLogUtil()->PrintLog(buf);
	   sprintf(buf,"RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	   this->GetLogUtil()->PrintLog(buf);
        // 客户端需进行错误处理
        //{客户端的错误处理}


    }

int CMdSpi::GetNightBreakTime(const char* pinstrumentid)
{
	int  Night_Break_Time;

	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	//	ZC=night   //23:30
	//	FG=night
	//	OI=night
	//	RM=night
	//	TA=night
	//	MA=night
	//	SR=night
	//	CF=night

//	i=night
//	j=night
//	jm=night
//	y=night
//	p=night
//	m=night
//	a=night  //23:00

//CZCE
	if ( strcmp(productid,"ZC")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"FG")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"OI")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"RM")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"TA")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"MA")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"SR")==0  )	{
		Night_Break_Time=233000;
	}
	else if ( strcmp(productid,"CF")==0  )	{
		Night_Break_Time=233000;
	}

//DCE 23:00

	else if ( strcmp(productid,"c")==0  )	{
			Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"cs")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"m")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"a")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"v")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"l")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"pp")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"p")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"y")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"i")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"jm")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"j")==0  )	{
		Night_Break_Time=230000;
	}

// SHFE
	else if ( strcmp(productid,"rb")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"hc")==0   )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"bu")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"ru")==0  )	{
		Night_Break_Time=230000;
	}
	else if ( strcmp(productid,"cu")==0  )	{
		Night_Break_Time=10000;
	}
	else if ( strcmp(productid,"ni")==0  )	{
		Night_Break_Time=10000;
	}
	else if ( strcmp(productid,"zn")==0  )	{
		Night_Break_Time=10000;
	}
	else if ( strcmp(productid,"al")==0   )	{
		Night_Break_Time=10000;
	}
	else if ( strcmp(productid,"pb")==0  )	{
		Night_Break_Time=10000;
	}
	else if ( strcmp(productid,"sn")==0  )	{
		Night_Break_Time=10000;
	}
	else if ( strcmp(productid,"ag")==0  )	{
		Night_Break_Time=23000;
	}
	else if ( strcmp(productid,"au")==0  )	{
		Night_Break_Time=23000;
	}
	else{
		Night_Break_Time=233000;
	}

//	rb=night    //23:00
//	hc=night    //23:00
//	bu=night   //23:00
//	ru=night   //23:00
//
//
//	cu=night   //1:00
//	ni=night  //1:00
//	zn=night  //1:00
//	al=night  //1:00
//	pb=night   //1:00
//	sn=night  //1:00

//	au=night   //2:30
//	ag=night    //2:30

	return Night_Break_Time;
}

int CMdSpi::GetPreSecondNightBreakTime(const char* pinstrumentid)
{
	int Night_Break_Time;


	char productid[31];
	CodeUtil codeutil;

	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	//	ZC=night   //23:30
	//	FG=night
	//	OI=night
	//	RM=night
	//	TA=night
	//	MA=night
	//	SR=night
	//	CF=night

//	i=night
//	j=night
//	jm=night
//	y=night
//	p=night
//	m=night
//	a=night  //23:30


	if ( strcmp(productid,"ZC")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"FG")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"OI")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"RM")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"TA")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"MA")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"SR")==0  )
	{
		Night_Break_Time=232959;
	}
	else if ( strcmp(productid,"CF")==0  )
	{
		Night_Break_Time=232959;
	}


	else if ( strcmp(productid,"c")==0  )	{
			Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"cs")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"m")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"a")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"v")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"l")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"pp")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"p")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"y")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"i")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"jm")==0  )	{
		Night_Break_Time=225950;
	}
	else if ( strcmp(productid,"j")==0  )	{
		Night_Break_Time=225950;
	}



	else if ( strcmp(productid,"rb")==0  )
	{
		Night_Break_Time=225959;
	}
	else if ( strcmp(productid,"hc")==0   )
	{
		Night_Break_Time=225959;
	}
	else if ( strcmp(productid,"bu")==0  )
	{
		Night_Break_Time=225959;
	}
	else if ( strcmp(productid,"ru")==0  )
	{
		Night_Break_Time=225959;
	}
	else if ( strcmp(productid,"cu")==0  )
	{
		Night_Break_Time=5959;
	}
	else if ( strcmp(productid,"ni")==0  )
	{
		Night_Break_Time=5959;
	}
	else if ( strcmp(productid,"zn")==0  )
	{
		Night_Break_Time=5959;
	}
	else if ( strcmp(productid,"al")==0   )
	{
		Night_Break_Time=5959;
	}
	else if ( strcmp(productid,"pb")==0  )
	{
		Night_Break_Time=5959;
	}
	else if ( strcmp(productid,"sn")==0  )
	{
		Night_Break_Time=5959;
	}
	else if ( strcmp(productid,"ag")==0  )
	{
		Night_Break_Time=22959;
	}
	else if ( strcmp(productid,"au")==0  )
	{
		Night_Break_Time=22959;
	}

//	rb=night    //23:00
//	hc=night    //23:00
//	bu=night   //23:00
//	ru=night   //23:00
//
//
//	cu=night   //1:00
//	ni=night  //1:00
//	zn=night  //1:00
//	al=night  //1:00
//	pb=night   //1:00
//	sn=night  //1:00

//	au=night   //2:30
//	ag=night    //2:30

	return Night_Break_Time;
}
void CMdSpi::NightProduct_0s_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
//	char buf[2048];
//

	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

	DateUtil dateutil;
	int Q_BarTime	= dateutil.CoverMinuteTimetoIntBarTime(pDepthMarketData->UpdateTime);

	double  Millisec = pDepthMarketData->UpdateMillisec;
	//		printf("SPI:   EXCHANGEID   %s   productid  %s \n",pDepthMarketData->ExchangeID,productid);
			//根据不同合约的交易时段进行判断 当前数据是否符合交易时段
	//		bool isopen=dateutil.CheckMDOpenTimebyInstrumentID(_Q_BarTime_2,pDepthMarketData->ExchangeID,productid);

	std::string str_time=pDepthMarketData->UpdateTime;

//		int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());

	bool Timemore_morning1 = (Q_BarTime>=85900 && Q_BarTime<101505);
	bool Timemore_morning2 = (Q_BarTime>=103000 && Q_BarTime<113005);
	bool Timemore_afternoon = (Q_BarTime>=133000 && Q_BarTime<150029);
	bool Timemore_night = (Q_BarTime>=205900 && Q_BarTime<240000);
	bool Timemore_night1 = (Q_BarTime>=0 && Q_BarTime<23005);
	bool Timemore3=( Timemore_morning1  || Timemore_afternoon || Timemore_night || Timemore_night1);

	// 夜盘合约,无夜盘合约,金融板块合约
	bool Night_Time=( Timemore_morning1  ||  Timemore_morning2  || Timemore_afternoon || Timemore_night || Timemore_night1);

	int Night_Break_Time=this->GetNightBreakTime(pDepthMarketData->InstrumentID);
	int Pre_NIght_Break_Time=this->GetPreSecondNightBreakTime(pDepthMarketData->InstrumentID);


	if (Night_Time )	{

		if (  seconds>=0 && seconds<=30 && this->GetMnKlinesig()==false)	{
//			printf ("15 -----> %s    %d -------->%s    qtime %d \n",pDepthMarketData->InstrumentID,this->GetMnKlinesig(),
//					pDepthMarketData->UpdateTime,Q_BarTime);

				if (Q_BarTime==205900  )   //集合竞价
				{
					Night_Begin(pDepthMarketData);
				}
				else if ( Q_BarTime>205900  && Q_BarTime<=210030 )   //开盘第一分钟
				{
					Minute_MidUpdateTime(pDepthMarketData);
				}
				else if(  Q_BarTime == Night_Break_Time || Q_BarTime==101500 || (Q_BarTime>=113000 && Q_BarTime<=113005))   //盘休开始
				{
					Minute_MidUpdateTime(pDepthMarketData);
					SaveBreakTimeTailData_0s(pDepthMarketData);
				}
				else if ((Q_BarTime==90000 || Q_BarTime==103000 ||Q_BarTime==133000 ) )  //盘休结束,准备开盘
				{
					BreakMinute_Begin(pDepthMarketData);
				}
				else if ( Q_BarTime>133000  && Q_BarTime<=133030 )   //下午开盘第一分钟
				{
					Minute_MidUpdateTime(pDepthMarketData);
				}
				else if(  Q_BarTime>=150000 && Q_BarTime<=150005)   //当天最后停盘时间
				{
					Minute_MidUpdateTime(pDepthMarketData);
					SaveBreakTimeTailData_0s(pDepthMarketData);

				}
				else	{
//					printf ("15 Minute_0s_SaveData -----> %s    %d -------->%s \n",pDepthMarketData->InstrumentID,
//							this->GetMnKlinesig(),pDepthMarketData->UpdateTime);
						Minute_0s_SaveData(pDepthMarketData);


				}
			}

			else	{

//				printf ("30 else  -----> %s    %d -------->%s \n",pDepthMarketData->InstrumentID,
//						this->GetMnKlinesig(),pDepthMarketData->UpdateTime);

				if(  Q_BarTime == Pre_NIght_Break_Time || Q_BarTime==101459 ||Q_BarTime==112959 )	{
					printf ("Q_BarTime %d ---------休盘开始 更新休盘前最后一分钟数据 -----------\n",Q_BarTime);
					Minute_MidUpdateTime(pDepthMarketData);

					SaveBreakTimeTailData_59s(pDepthMarketData);

				}
				else{

//					if(seconds==59)
//					{
//						printf ("Q_BarTime %d ---------中间时刻 更新数据 -----------\n",Q_BarTime);
//					}
					//中间时刻 更新数据
					Minute_MidUpdateTime(pDepthMarketData);
				}



		//				printf ("---------ing -----------\n");
			}

		if (seconds>30 && seconds<=59 && this->GetMnKlinesig()==true)	{
			ContractFlag contractflag;
//				printf ("45 -----> %s    -------->%s \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime);
			this->SetMnKlinesig(false);
//			contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
			contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
			contractflag.MnKlinesig=this->GetMnKlinesig();
//			this->GetRedisMinFlag()->SetFlag(contractflag);
			this->UpdateContractFlag(contractflag);
		}


	}



}
void CMdSpi::DayProduct_0s_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
//	char buf[2048];
//

	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

	DateUtil dateutil;
	int Q_BarTime	= dateutil.CoverMinuteTimetoIntBarTime(pDepthMarketData->UpdateTime);

	double  Millisec = pDepthMarketData->UpdateMillisec;
	//		printf("SPI:   EXCHANGEID   %s   productid  %s \n",pDepthMarketData->ExchangeID,productid);
			//根据不同合约的交易时段进行判断 当前数据是否符合交易时段
	//		bool isopen=dateutil.CheckMDOpenTimebyInstrumentID(_Q_BarTime_2,pDepthMarketData->ExchangeID,productid);

	std::string str_time=pDepthMarketData->UpdateTime;

//		int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());

	bool Timemore_morning1 = (Q_BarTime>=85900 && Q_BarTime<101505);
	bool Timemore_morning2 = (Q_BarTime>=103000 && Q_BarTime<113005);
	bool Timemore_afternoon = (Q_BarTime>=133000 && Q_BarTime<150029);

	bool Day_Time= ( Timemore_morning1  || Timemore_morning2   || Timemore_afternoon );

	if (  Day_Time ){


		if ( seconds>=0 &&  seconds<=30 && this->GetMnKlinesig()==false)	{
			if (Q_BarTime==85900 )		{
				Day_Begin(pDepthMarketData);
			}
			else if (Q_BarTime>85900 && Q_BarTime<=90030)	{
				Minute_MidUpdateTime(pDepthMarketData);
			}
			else if(Q_BarTime==101500 || (Q_BarTime>=113000 && Q_BarTime<=113005))   //盘休开始
			{
				Minute_MidUpdateTime(pDepthMarketData);

				SaveBreakTimeTailData_0s(pDepthMarketData);

//				ContractFlag contractflag;
//				this->SetMnKlinesig(true);
//				contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
//				contractflag.MnKlinesig=this->GetMnKlinesig();
//				contractflag.last_volume_open = contractflag.last_volume;   // 59秒时的成交量作为 下一分钟的起始成交量
//				this->GetRedisMinFlag()->SetFlag(contractflag);

			}
			else if ((Q_BarTime==103000 ||Q_BarTime==133000 ) )  //盘休结束,准备开盘
			{
				BreakMinute_Begin(pDepthMarketData);
			}
			else if ( Q_BarTime>133000  && Q_BarTime<=133030 )   //下午开盘第一分钟
			{
				Minute_MidUpdateTime(pDepthMarketData);
			}
			else if(   Q_BarTime>=150000 && Q_BarTime<=150005)   //当天最后停盘时间
			{
				Minute_MidUpdateTime(pDepthMarketData);
				SaveBreakTimeTailData_0s(pDepthMarketData);

			}
			else{
					Minute_0s_SaveData(pDepthMarketData);
			}
		}
		else	{
			if( Q_BarTime==101459 ||Q_BarTime==112959 )	{
				printf ("Q_BarTime %d ---------休盘开始  -----------\n",Q_BarTime);
				Minute_MidUpdateTime(pDepthMarketData);
				SaveBreakTimeTailData_59s(pDepthMarketData);
			}
			else{
				//中间时刻 更新数据
				Minute_MidUpdateTime(pDepthMarketData);
			}
		}

		if (seconds>30 && seconds<=59 && this->GetMnKlinesig()==true)	{
				ContractFlag contractflag;
	//				printf ("45 -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
				this->SetMnKlinesig(false);
//				contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
				contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
				contractflag.MnKlinesig=this->GetMnKlinesig();
//				this->GetRedisMinFlag()->SetFlag(contractflag);
				this->UpdateContractFlag(contractflag);
		}
	}
	else	{
		//休盘
		this->SetFlagMarketOpen(false);
	}

}
void CMdSpi::NightProduct_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	char buf[1024];


	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

	DateUtil dateutil;
	double Q_BarTime	= dateutil.ConvertMinuteTimetoBarTime(pDepthMarketData->UpdateTime);

	double  Millisec = pDepthMarketData->UpdateMillisec;
	//		printf("SPI:   EXCHANGEID   %s   productid  %s \n",pDepthMarketData->ExchangeID,productid);
			//根据不同合约的交易时段进行判断 当前数据是否符合交易时段
	//		bool isopen=dateutil.CheckMDOpenTimebyInstrumentID(_Q_BarTime_2,pDepthMarketData->ExchangeID,productid);

	std::string str_time=pDepthMarketData->UpdateTime;

//		int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());

	bool Timemore_morning1 = (Q_BarTime>=0.085900 && Q_BarTime<0.1016);
	bool Timemore_morning2 = (Q_BarTime>=0.1030 && Q_BarTime<0.1131);
	bool Timemore_afternoon = (Q_BarTime>=0.1330 && Q_BarTime<0.1501);
	bool Timemore_night = (Q_BarTime>=0.205900 && Q_BarTime<0.240000);
	bool Timemore_night1 = (Q_BarTime>=0.000000 && Q_BarTime<0.023100);
	bool Timemore3=( Timemore_morning1  || Timemore_afternoon || Timemore_night || Timemore_night1);

	// 夜盘合约,无夜盘合约,金融板块合约
	bool Night_Time=( Timemore_morning1  ||  Timemore_morning2  || Timemore_afternoon || Timemore_night || Timemore_night1);

	if (Night_Time )
	{
		// 盘中保存tick数据
		SaveTickDatatoRedis(pDepthMarketData);

		bool morning_break=Q_BarTime>=0.101459 && Q_BarTime<0.103000 && Millisec>450;
		bool noon_break=Q_BarTime>=0.112959 && Q_BarTime<0.103000 && Millisec>450;

		if( (Timemore_night || Timemore_night1))
		{
			bool night_au_break =Q_BarTime>=0.022959 && Q_BarTime<0.023500 && Millisec>450;
			bool auag=strcmp(productid,"au")==0 || strcmp(productid,"ag")==0;
			if(night_au_break && auag )
			{
				this->SetFlagMarketOpen(false);
			}
		}
		else if(morning_break || noon_break)
		{
			this->SetFlagMarketOpen(false);
		}
		else
		{
			this->SetFlagMarketOpen(true);
		}

	}
	else
	{
		this->SetFlagMarketOpen(false);
	}


	if ( Night_Time  && seconds>=59 && this->GetMnKlinesig()==false && Millisec>450)
	{

		bool time_break = ( Q_BarTime==0.090000 || Q_BarTime==0.103000 ||Q_BarTime==0.133000);

		if (Q_BarTime==0.205900   )
		{
			Minute_Begin(pDepthMarketData);
		}
		else if ( Q_BarTime==0.210000 )
		{
			Minute_MidUpdateTime(pDepthMarketData);
		}
		else if(time_break)
		{
			Minute_Begin(pDepthMarketData);
		}

		else
		{
			// 非起点分钟，写入分钟数据
			Minute_0s_SaveData(pDepthMarketData);

		}
	}

	else
	{
		if (Night_Time )
		{
			//中间时刻 更新数据
			Minute_MidUpdateTime(pDepthMarketData);
		}


//				printf ("---------ing -----------\n");
	}

	if (Night_Time )
	{
		if (seconds>45 && seconds<=58 && this->GetMnKlinesig()==true)
		{
			ContractFlag contractflag;
//				printf ("45 -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
			this->SetMnKlinesig(false);
//			contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
			contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
			contractflag.MnKlinesig=this->GetMnKlinesig();
//			this->GetRedisMinFlag()->SetFlag(contractflag);
			this->UpdateContractFlag(contractflag);
		}


	}


//		printf ("--->>>  %s   %s  lastprice: %f  askprice1: %f   bidprice1: %f \n",
//									pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
//									pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1);

	if( strcmp(pDepthMarketData->InstrumentID,"rb1810")==0)
	{
		sprintf (buf,"SPI:--->>>  %s   %s  lastprice: %.3f  askprice1: %.3f   bidprice1: %.3f   volume: %d \n",
			pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,pDepthMarketData->LastPrice,
			pDepthMarketData->AskPrice1,pDepthMarketData->BidPrice1,pDepthMarketData->Volume);

		this->GetLogUtil()->PrintLog(buf);
	}



}

void CMdSpi::DayProduct_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	char buf[1024];


	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

	DateUtil dateutil;
	double Q_BarTime	= dateutil.ConvertMinuteTimetoBarTime(pDepthMarketData->UpdateTime);

	double  Millisec = pDepthMarketData->UpdateMillisec;
	//		printf("SPI:   EXCHANGEID   %s   productid  %s \n",pDepthMarketData->ExchangeID,productid);
			//根据不同合约的交易时段进行判断 当前数据是否符合交易时段
	//		bool isopen=dateutil.CheckMDOpenTimebyInstrumentID(_Q_BarTime_2,pDepthMarketData->ExchangeID,productid);

	std::string str_time=pDepthMarketData->UpdateTime;

//		int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());

	bool Timemore_morning1 = (Q_BarTime>=0.085900 && Q_BarTime<0.1016);
	bool Timemore_morning2 = (Q_BarTime>=0.1030 && Q_BarTime<0.1131);
	bool Timemore_afternoon = (Q_BarTime>=0.1330 && Q_BarTime<0.1501);

	bool Day_Time= ( Timemore_morning1  || Timemore_morning2   || Timemore_afternoon );
	bool morning_break=Q_BarTime>=0.101459 && Q_BarTime<0.103000 && Millisec>450;
	bool noon_break=Q_BarTime>=0.112959 && Q_BarTime<0.103000 && Millisec>450;


	if (  Day_Time )
	{
		//保存tick 行情至内存数据库
		SaveTickDatatoRedis(pDepthMarketData);

		if (seconds>5 && seconds<=58 && this->GetMnKlinesig()==true)
		{
			ContractFlag contractflag;
//				printf ("45 -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
			this->SetMnKlinesig(false);
//			contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
			contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
			contractflag.MnKlinesig=this->GetMnKlinesig();
//			this->GetRedisMinFlag()->SetFlag(contractflag);
			this->UpdateContractFlag(contractflag);
		}

		if(morning_break || noon_break)
		{
			this->SetFlagMarketOpen(false);
		}
		else
		{
			this->SetFlagMarketOpen(true);
		}

		if ( seconds==0 )
		{

			bool time_break = (Q_BarTime==0.103000 ||Q_BarTime==0.133000);

			if (Q_BarTime==0.085900 )
			{

				Day_Begin(pDepthMarketData);
			}
			else if (Q_BarTime==0.090000 )
			{
				Minute_MidUpdateTime(pDepthMarketData);
			}
//			else if (time_break )
//			{
//				Minute_Begin(pDepthMarketData);
//			}
			else
			{
				if(this->GetMnKlinesig()==false)
				{

					Minute_0s_SaveData(pDepthMarketData);
					sprintf (buf,"DayProduct_DepthMarketData:--->>>  %s   %s 上一分钟 未写入数据 \n",
							pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime);
					this->GetLogUtil()->PrintLog(buf);

					//59s 未写入数据
					Minute_Begin(pDepthMarketData);
				}
				else{
					Minute_Begin(pDepthMarketData);
				}

			}

		}

		if (seconds>=59 && this->GetMnKlinesig()==false && Millisec>450)
		{

			Minute_MidUpdateTime(pDepthMarketData);
			Minute_59s_SaveData(pDepthMarketData);
			Minute_SaveOpenVolume(pDepthMarketData);
		}
		else
		{
			//中间时刻 更新数据
			Minute_MidUpdateTime(pDepthMarketData);
			if (!this->GetFlagMarketOpen())
			{

			}
//				printf ("---------ing -----------\n");
		}




	}
	else
	{
		//休盘
		this->SetFlagMarketOpen(false);
	}

}

void CMdSpi::FinancialProduct_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	char buf[1024];


	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

	DateUtil dateutil;
	double Q_BarTime	= dateutil.ConvertMinuteTimetoBarTime(pDepthMarketData->UpdateTime);

	double  Millisec = pDepthMarketData->UpdateMillisec;
	//		printf("SPI:   EXCHANGEID   %s   productid  %s \n",pDepthMarketData->ExchangeID,productid);
			//根据不同合约的交易时段进行判断 当前数据是否符合交易时段
	//		bool isopen=dateutil.CheckMDOpenTimebyInstrumentID(_Q_BarTime_2,pDepthMarketData->ExchangeID,productid);

	std::string str_time=pDepthMarketData->UpdateTime;

//		int hours = atoi(str_time.substr(0,2).c_str());
	int minutes = atoi(str_time.substr(3,2).c_str());
	int seconds = atoi(str_time.substr(6,2).c_str());



	// 夜盘合约,无夜盘合约,金融板块合约

	bool Financial_Time =(Q_BarTime>=0.092900 && Q_BarTime<0.1131) || (Q_BarTime>=0.1300 && Q_BarTime<0.1501); // IC IF IH T TF  13:00 开盘

	if ( Financial_Time)
	{
		// 盘中保存tick数据
		SaveTickDatatoRedis(pDepthMarketData);

		bool morning_break=Q_BarTime>=0.101459 && Q_BarTime<0.103000 && Millisec>450;
		bool noon_break=Q_BarTime>=0.112959 && Q_BarTime<0.103000 && Millisec>450;

	if(morning_break || noon_break)
		{
			this->SetFlagMarketOpen(false);
		}
		else
		{
			this->SetFlagMarketOpen(true);
		}

	}
	else
	{
		this->SetFlagMarketOpen(false);
	}


if (    Financial_Time && seconds>=59 && this->GetMnKlinesig()==false && Millisec>450)
	{

		bool time_break = (Q_BarTime==0.130000);

		if (Q_BarTime==0.0929000   )
		{
			Minute_Begin(pDepthMarketData);
		}
		else if ( Q_BarTime==0.093000 )
		{
			Minute_MidUpdateTime(pDepthMarketData);
		}
		else if(time_break)
		{
			Minute_Begin(pDepthMarketData);
		}

		else
		{
			// 非起点分钟，写入分钟数据
			Minute_0s_SaveData(pDepthMarketData);

		}
	}
	else
	{
		if ( Financial_Time)
		{
			//中间时刻 更新数据
			Minute_MidUpdateTime(pDepthMarketData);
		}


//				printf ("---------ing -----------\n");
	}

	if (  Financial_Time)
	{
		if (seconds>45 && seconds<=58 && this->GetMnKlinesig()==true)
		{
			ContractFlag contractflag;
//				printf ("45 -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
			this->SetMnKlinesig(false);
//			contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
			contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
			contractflag.MnKlinesig=this->GetMnKlinesig();
//			this->GetRedisMinFlag()->SetFlag(contractflag);
			this->UpdateContractFlag(contractflag);
		}


	}
}
void CMdSpi::Day_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	char buf[2048];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	ContractFlag contractflag;

	this->SetMnKlinesig(true);
	strcpy(contractflag.InstrumentID, pDepthMarketData->InstrumentID);
	contractflag.MnKlinesig=this->GetMnKlinesig();
	contractflag.last_open =pDepthMarketData->LastPrice;
	contractflag.last_high =pDepthMarketData->LastPrice;
	contractflag.last_low =pDepthMarketData->LastPrice;
	contractflag.last_close =pDepthMarketData->LastPrice;

	contractflag.last_opi=pDepthMarketData->OpenInterest;
	contractflag.last_volume_open = pDepthMarketData->Volume;
	contractflag.last_volume = pDepthMarketData->Volume;
	strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);
	strcpy(contractflag.FirstTickTime,"08:59:00");
//	this->GetRedisMinFlag()->SetFlag(contractflag);
	this->UpdateContractFlag(contractflag);


}
void CMdSpi::Night_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	char buf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	ContractFlag contractflag;

	this->SetMnKlinesig(true);
	strcpy(contractflag.InstrumentID, pDepthMarketData->InstrumentID);
	contractflag.MnKlinesig=this->GetMnKlinesig();
	contractflag.last_open =pDepthMarketData->LastPrice;
	contractflag.last_high =pDepthMarketData->LastPrice;
	contractflag.last_low =pDepthMarketData->LastPrice;
	contractflag.last_close =pDepthMarketData->LastPrice;

	contractflag.last_opi=pDepthMarketData->OpenInterest;
	contractflag.last_volume_open = pDepthMarketData->Volume;
	contractflag.last_volume = pDepthMarketData->Volume;
	strcpy(contractflag.FirstTickTime,"20:59:00");
	strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

//	this->GetRedisMinFlag()->SetFlag(contractflag);
	this->UpdateContractFlag(contractflag);

}
//
void CMdSpi::Minute_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	ContractFlag contractflag;
	this->SetMnKlinesig(true);
	strcpy(contractflag.InstrumentID, pDepthMarketData->InstrumentID);
	contractflag.MnKlinesig=this->GetMnKlinesig();
	contractflag.last_open =pDepthMarketData->LastPrice;
	contractflag.last_high =pDepthMarketData->LastPrice;
	contractflag.last_low =pDepthMarketData->LastPrice;
	contractflag.last_close =pDepthMarketData->LastPrice;

	contractflag.last_opi=pDepthMarketData->OpenInterest;
	contractflag.last_volume = pDepthMarketData->Volume;

	strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
	strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

//	this->GetRedisMinFlag()->SetFlag(contractflag);
	this->UpdateContractFlag(contractflag);

}

void CMdSpi::BreakMinute_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	ContractFlag contractflag;
	this->SetMnKlinesig(true);
	strcpy(contractflag.InstrumentID, pDepthMarketData->InstrumentID);
	contractflag.MnKlinesig=this->GetMnKlinesig();
	contractflag.last_open =pDepthMarketData->LastPrice;
	contractflag.last_high =pDepthMarketData->LastPrice;
	contractflag.last_low =pDepthMarketData->LastPrice;
	contractflag.last_close =pDepthMarketData->LastPrice;

	contractflag.last_opi=pDepthMarketData->OpenInterest;
	contractflag.last_volume_open = pDepthMarketData->Volume;
	contractflag.last_volume = pDepthMarketData->Volume;

	strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
	strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

//	this->GetRedisMinFlag()->SetFlag(contractflag);
	this->UpdateContractFlag(contractflag);

	printf ("BreakMinute_Begin: 成交量 %s %s--->>> 起始成交量: %d  截止成交量 : %d     \n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
					contractflag.last_volume_open,contractflag.last_volume);

}
//
void CMdSpi::Minute_MidUpdateTime(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	ContractFlag contractflag;
//				printf ("else -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
//	this->GetRedisMinFlag()->GetContractFlag(pDepthMarketData->InstrumentID,contractflag);
//	contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
	contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
	contractflag.last_high = max(contractflag.last_high,pDepthMarketData->LastPrice);
	contractflag.last_low	=min(contractflag.last_low,pDepthMarketData->LastPrice);
	contractflag.last_close = pDepthMarketData->LastPrice;

	contractflag.last_opi=pDepthMarketData->OpenInterest;
	contractflag.last_volume = pDepthMarketData->Volume;
	strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

	if (contractflag.last_volume < contractflag.last_volume_open   )
	{
		printf ("Minute_MidUpdateTime:起始成交量异常  %s  %s 修正前--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,
								pDepthMarketData->UpdateTime,contractflag.last_volume_open,contractflag.last_volume);
//		contractflag.last_volume_open =pDepthMarketData->Volume;
//		strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);

		printf("Minute_MidUpdateTime:  起始数据待初始化  -->>> 合约名称:%s 时间: %s   \n",pDepthMarketData->InstrumentID,
								pDepthMarketData->UpdateTime);
						//下一分钟第一个数据
							contractflag.last_open =pDepthMarketData->LastPrice;
							contractflag.last_high =pDepthMarketData->LastPrice;
							contractflag.last_low =pDepthMarketData->LastPrice;
							contractflag.last_close =pDepthMarketData->LastPrice;

							contractflag.last_opi=pDepthMarketData->OpenInterest;
							contractflag.last_volume_open = pDepthMarketData->Volume;   // 59秒时的成交量作为 下一分钟的起始成交量
							contractflag.last_volume = pDepthMarketData->Volume;
							strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
							strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

		printf ("Minute_MidUpdateTime:起始成交量异常  %s  %s 修正后--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,
				pDepthMarketData->UpdateTime,contractflag.last_volume_open,contractflag.last_volume);


	}
	else if(contractflag.last_volume_open <0)
	{
		printf ("Minute_MidUpdateTime:起始成交量为负数  %s  %s 修正前--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,
								pDepthMarketData->UpdateTime,contractflag.last_volume_open,contractflag.last_volume);


		printf("Minute_MidUpdateTime:  起始数据待初始化  -->>> 合约名称:%s 时间: %s   \n",pDepthMarketData->InstrumentID,
						pDepthMarketData->UpdateTime);
				//下一分钟第一个数据
					contractflag.last_open =pDepthMarketData->LastPrice;
					contractflag.last_high =pDepthMarketData->LastPrice;
					contractflag.last_low =pDepthMarketData->LastPrice;
					contractflag.last_close =pDepthMarketData->LastPrice;

					contractflag.last_opi=pDepthMarketData->OpenInterest;
					contractflag.last_volume_open = pDepthMarketData->Volume;   // 59秒时的成交量作为 下一分钟的起始成交量
					contractflag.last_volume = pDepthMarketData->Volume;
					strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
					strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);

		printf ("Minute_MidUpdateTime:起始成交量为负数  %s  %s 修正后--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,
				pDepthMarketData->UpdateTime,contractflag.last_volume_open,contractflag.last_volume);
	}
	else if(contractflag.last_volume ==0)
	{
//		printf ("Minute_MidUpdateTime:起始成交量为负数  %s  %s 修正前--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,
//								pDepthMarketData->UpdateTime,contractflag.last_volume_open,contractflag.last_volume);
	}

//	this->GetRedisMinFlag()->SetContractFlag(&contractflag);
//	this->GetRedisMinFlag()->SetFlag(contractflag);
	this->UpdateContractFlag(contractflag);
//	ContractFlag lcontractflag;
//	//				printf ("else -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
//	this->GetRedisMinFlag()->GetContractFlag(pDepthMarketData->InstrumentID,lcontractflag);
//	printf ("Minute_MidUpdateTime:  %s  %s 修正确认--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,
//					pDepthMarketData->UpdateTime,lcontractflag.last_volume_open,lcontractflag.last_volume);
}
void CMdSpi::Minute_SaveOpenVolume(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	ContractFlag contractflag;
//				printf ("else -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
//	contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
	contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
	contractflag.last_volume_open = pDepthMarketData->Volume;

//	this->GetRedisMinFlag()->SetFlag(contractflag);
	this->UpdateContractFlag(contractflag);
}
void CMdSpi::Minute_0s_SaveData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	// 非起点分钟，写入分钟数据
	ContractFlag contractflag;

	this->SetMnKlinesig(true);
//	contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
	contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
	contractflag.MnKlinesig=this->GetMnKlinesig();

	//打印1分钟K线数据
//	printf ("打印1分钟K线数据--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f  成交量: %d \n",
//			pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
//			contractflag.last_open,contractflag.last_high,contractflag.last_low,contractflag.last_close,contractflag.last_volume);

	bool valid1 = contractflag.last_open>(double)1.0;
	bool valid2 = contractflag.last_high>(double)1.0;
	bool valid3 = contractflag.last_low>(double)1.0;
	bool valid4 = contractflag.last_close>(double)1.0;

	//					printf ("11111--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f   \n",
	//						names,minute_time.c_str(),mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,mysql_mindata.LowestPrice,mysql_mindata.ClosePrice);

	bool valid =valid1 && valid2 && valid3 && valid4;

	if (valid)	{

		//打印1分钟K线数据
//		printf ("打印1分钟K线数据,准备写入--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f  成交量: %d \n",
//				pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
//				contractflag.last_open,contractflag.last_high,contractflag.last_low,contractflag.last_close,contractflag.last_volume);
		//写入分钟数据
		SaveMinData(pDepthMarketData,&contractflag);

		//下一分钟第一个数据
		contractflag.last_open =pDepthMarketData->LastPrice;
		contractflag.last_high =pDepthMarketData->LastPrice;
		contractflag.last_low =pDepthMarketData->LastPrice;
		contractflag.last_close =pDepthMarketData->LastPrice;

		contractflag.last_opi=pDepthMarketData->OpenInterest;
		contractflag.last_volume_open = contractflag.last_volume;   // 59秒时的成交量作为 下一分钟的起始成交量
		contractflag.last_volume = pDepthMarketData->Volume;
		strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
		strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);


//		this->GetRedisMinFlag()->SetFlag(contractflag);
		this->UpdateContractFlag(contractflag);
	}
	else	{
		printf("Minute_0s_SaveData:  起始数据待初始化  -->>> 合约名称:%s 时间: %s   \n",pDepthMarketData->InstrumentID,
				pDepthMarketData->UpdateTime);
		//下一分钟第一个数据
			contractflag.last_open =pDepthMarketData->LastPrice;
			contractflag.last_high =pDepthMarketData->LastPrice;
			contractflag.last_low =pDepthMarketData->LastPrice;
			contractflag.last_close =pDepthMarketData->LastPrice;

			contractflag.last_opi=pDepthMarketData->OpenInterest;
			contractflag.last_volume_open = pDepthMarketData->Volume;   // 59秒时的成交量作为 下一分钟的起始成交量
			contractflag.last_volume = pDepthMarketData->Volume;
			strcpy(contractflag.FirstTickTime,pDepthMarketData->UpdateTime);
			strcpy(contractflag.LastTickTime,pDepthMarketData->UpdateTime);


//			this->GetRedisMinFlag()->SetFlag(contractflag);
			this->UpdateContractFlag(contractflag);

	}





}

void CMdSpi::Minute_59s_SaveData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	// 非起点分钟，写入分钟数据
	ContractFlag contractflag;

	this->SetMnKlinesig(true);
//	contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
	contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
	contractflag.MnKlinesig=this->GetMnKlinesig();

	//打印1分钟K线数据
//	printf ("打印1分钟K线数据--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f  成交量: %d \n",
//			pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
//			contractflag.last_open,contractflag.last_high,contractflag.last_low,contractflag.last_close,contractflag.last_volume);

	//写入分钟数据
	SaveMinData_59s(pDepthMarketData,contractflag);


}
std::string  CMdSpi::CheckCurrentDate(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	std::string str_date;
	DateUtil dateutil;
	int Q_BarTime	= dateutil.CoverMinuteTimetoIntBarTime(pDepthMarketData->UpdateTime);
	bool Timemore_night = (Q_BarTime>=210000 && Q_BarTime<240000);
	std::string str_lastminute=dateutil.GetLastMinuteBarTime(pDepthMarketData->UpdateTime);

	// 防止跨天的时间点 和本地时间有差异,导致日期错误
	if (Timemore_night)	{

		str_date=pDepthMarketData->ActionDay;

		string currentdate = dateutil.GetCurrentDayString();
		if(currentdate.compare(str_date)==0){
//			printf ("SaveMinData    -------> 相同  currentdate %s   actionday: %s\n ",currentdate.c_str(),str_date.c_str());
		}
		else{

			printf ("SaveMinData   %s  修正前>>> currentdate %s   actionday: %s   行情日期 修正为当前日期 >>>>>>>>> %s  \n ",pDepthMarketData->InstrumentID,
					currentdate.c_str(),str_date.c_str(),currentdate.c_str());
			str_date=currentdate;

//			printf ("SaveMinData   重新修正  actionday: %s\n ",str_date.c_str());
		}

	}
	else if(strcmp(str_lastminute.c_str(),"23:59:00")==0)    { // 2018-05-08 0:00:00   减一分钟后 ---> 2018-05-07 23:59:00

		std::string sqltime_today  = dateutil.ConvertDatetoSqlTime(pDepthMarketData->ActionDay);
		time_t t_today = dateutil.ConvertSqlTimetoTimet(sqltime_today.c_str());
		str_date = dateutil.ConvertTimetoString(t_today - 60*60*24).substr(0,8);
	}
	else	{
		str_date=pDepthMarketData->ActionDay;
	}
	return str_date;
}
void CMdSpi::SaveRedisMinData(CThostFtdcDepthMarketDataField *pDepthMarketData,Mysql_MinData mysql_mindata)
{
	// 分钟数据写入redismindata 库
		RMinData rmindata;
		strcpy(rmindata.InstrumentID,pDepthMarketData->InstrumentID);
		strcpy(rmindata.MinuteTime,mysql_mindata.MinuteTime);

		rmindata.Volume=mysql_mindata.Volume;
		rmindata.OpenInterest=mysql_mindata.OpenInterest;
		rmindata.OpenPrice = mysql_mindata.OpenPrice;
		rmindata.ClosePrice = mysql_mindata.ClosePrice;
		rmindata.HighestPrice = mysql_mindata.HighestPrice;
		rmindata.LowestPrice = mysql_mindata.LowestPrice;

		m_redismindata->SetMinData(rmindata);
}
	//*/
void CMdSpi::SaveMinData(CThostFtdcDepthMarketDataField *pDepthMarketData,ContractFlag *pcontractflag)
{

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());


	bool night=codeutil.IsNightByProductid(productid);

//	std:string database ="minute_";
//	database.append(productid);
//
//	std::string tablename =pDepthMarketData->InstrumentID;
//	tablename.append("_");
//	tablename.append(this->GetTradingDay().c_str());

	DateUtil dateutil;
//	int Q_BarTime	= dateutil.CoverMinuteTimetoIntBarTime(pDepthMarketData->UpdateTime);

	Mysql_MinData mysql_mindata;
	std::string str_date=this->CheckCurrentDate(pDepthMarketData);

	//当前时间如果是   23:20:00   ,则分钟数据时间未 23:19:00 ,分钟数需要减一
	std::string str_lastminute=dateutil.GetLastMinuteBarTime(pDepthMarketData->UpdateTime);
	std::string mintime_sql;
	char temp[20];
	sprintf(temp,"%s-%s-%s ",str_date.substr(0,4).c_str(),str_date.substr(4,2).c_str(),str_date.substr(6,2).c_str());
	mintime_sql =temp;
	mintime_sql.append(str_lastminute);

	strcpy(mysql_mindata.MinuteTime,mintime_sql.c_str());
	mysql_mindata.OpenPrice =pcontractflag->last_open;
	mysql_mindata.HighestPrice =pcontractflag->last_high;
	mysql_mindata.LowestPrice =pcontractflag->last_low;
	mysql_mindata.ClosePrice =pcontractflag->last_close;
	strcpy(mysql_mindata.LastTickTime,pcontractflag->LastTickTime);
	//持仓量
//	double dOpenInterest = pDepthMarketData->OpenInterest;
	mysql_mindata.OpenInterest =pcontractflag->last_opi;


	// 开盘的第一分钟  成交量 =就是当前的成交量-0
	if (night && strcmp(str_lastminute.c_str(),"21:00:00")==0)	{
		mysql_mindata.Volume = pcontractflag->last_volume;
		printf ("SaveMinData 夜盘第一分钟成交量  %s  %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				pcontractflag->last_volume_open,pcontractflag->last_volume);
	}
	else  if (!night && strcmp(str_lastminute.c_str(),"09:00:00")==0){
		printf ("SaveMinData 日盘第一分钟成交量  %s   %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				pcontractflag->last_volume_open,pcontractflag->last_volume);
		mysql_mindata.Volume =pcontractflag->last_volume ;
	}
	else	{
		//成交量
		int min_vol=pcontractflag->last_volume - pcontractflag->last_volume_open;
		if (min_vol >0 )	{
			mysql_mindata.Volume =min_vol;
//			printf ("SaveMinData: 成交量 %s %s--->>> 起始成交量: %d  截止成交量 : %d     --->分钟内成交量 %d \n",
//					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
//					contractflag.last_volume_open,contractflag.last_volume,min_vol);
		}
		else	{
			printf ("SaveMinData: 成交量异常 %s %s  修正前--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
					pcontractflag->last_volume_open,pcontractflag->last_volume);
			if(pcontractflag->last_volume<=0)		{
				pcontractflag->last_volume=pDepthMarketData->Volume;
			}

			if(pcontractflag->last_volume_open <=0)	{
				pcontractflag->last_volume_open=pDepthMarketData->Volume;
			}


			printf ("SaveMinData: 成交量异常 %s %s  修正后--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
					pcontractflag->last_volume_open,pcontractflag->last_volume);

			mysql_mindata.Volume=0;
//			this->GetRedisMinFlag()->SetFlag(contractflag);
		}
	}

//	printf ("Set_Database --->>> %s ",database.c_str());
	this->GetMysqlMindata()->Set_Database(this->GetMindata_Database());
//	printf ("CreateTable --->>> %s \n",tablename.c_str());
	this->GetMysqlMindata()->CreateTable(this->GetMindata_Tablename().c_str());
	bool valid1 = mysql_mindata.OpenPrice>(double)1.0;
	bool valid2 = mysql_mindata.ClosePrice>(double)1.0;
	bool valid3 = mysql_mindata.HighestPrice>(double)1.0;
	bool valid4 = mysql_mindata.LowestPrice>(double)1.0;

	//					printf ("11111--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f   \n",
	//						names,minute_time.c_str(),mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,mysql_mindata.LowestPrice,mysql_mindata.ClosePrice);

	bool valid =valid1 && valid2 && valid3 && valid4;
	//					if (valid)
	//					{
	//						printf ("22222--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f   \n",
	//												names,minute_time.c_str(),mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,mysql_mindata.LowestPrice,mysql_mindata.ClosePrice);
	//
	//					}
//	printf ("Exist_MinuteTime --->>> %s ",tablename.c_str());
	bool data_exist=this->GetMysqlMindata()->Exist_MinuteTime(this->GetMindata_Tablename().c_str(),mysql_mindata.MinuteTime);
	if( !data_exist&& valid)	{
		printf ("SaveMinData 插入数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
				pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
				mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

		this->GetMysqlMindata()->Insert(this->GetMindata_Tablename().c_str(),mysql_mindata);
		SaveRedisMinData(pDepthMarketData,mysql_mindata);
	}
	else if (!valid)
	{
		printf("SaveMinData  %s 数据[%s]非法------> 暂不操作 \n",pDepthMarketData->InstrumentID,mysql_mindata.MinuteTime);
	}
	else if(data_exist)
	{
		printf("SaveMinData  %s 存在[%s]数据------> 暂不写入 \n",pDepthMarketData->InstrumentID,mysql_mindata.MinuteTime);
	}

}

void CMdSpi::SaveBreakTimeTailData_59s(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	printf("SaveBreakTimeTailData_59s---> %s   beging \n",pDepthMarketData->UpdateTime);
	char buf[1024];



	ContractFlag contractflag;
//	contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
	contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

//	std:string database ="minute_";
//	database.append(productid);
//
//	std::string tablename =pDepthMarketData->InstrumentID;
//	tablename.append("_");
//	tablename.append(this->GetTradingDay().c_str());


	bool night=codeutil.IsNightByProductid(productid);


	DateUtil dateutil;
	double Q_BarTime	= dateutil.ConvertMinuteTimetoBarTime(pDepthMarketData->UpdateTime);

	Mysql_MinData mysql_mindata;

	bool Timemore_night = (Q_BarTime>=0.210000 && Q_BarTime<0.240000);

	//当前时间如果是   23:20:00   ,则分钟数据时间未 23:19:00 ,分钟数需要减一
	std::string str_time=pDepthMarketData->UpdateTime;
	int seconds = atoi(str_time.substr(6,2).c_str());

	if (seconds==59)
	{
		std::string mintime_sql;
		std::string str_date=pDepthMarketData->ActionDay;
		char c_buf[20];
		sprintf(c_buf,"%s:00",str_time.substr(0,5).c_str());
		std::string str_lastminute=c_buf;

		char temp[20];
		sprintf(temp,"%s-%s-%s ",str_date.substr(0,4).c_str(),str_date.substr(4,2).c_str(),str_date.substr(6,2).c_str());
		mintime_sql =temp;
		mintime_sql.append(str_lastminute);

		strcpy(mysql_mindata.MinuteTime,mintime_sql.c_str());

	}


	mysql_mindata.OpenPrice =contractflag.last_open;
	mysql_mindata.HighestPrice =contractflag.last_high;
	mysql_mindata.LowestPrice =contractflag.last_low;
	mysql_mindata.ClosePrice =contractflag.last_close;
	strcpy(mysql_mindata.LastTickTime,contractflag.LastTickTime);
	//持仓量
	double dOpenInterest = pDepthMarketData->OpenInterest;
	mysql_mindata.OpenInterest =dOpenInterest;


	int min_vol=contractflag.last_volume - contractflag.last_volume_open;
	if (min_vol >=0 )
	{
		mysql_mindata.Volume =min_vol;
		printf ("SaveBreakTimeTailData_59s: 成交量 %s %s--->>> 起始成交量: %d  截止成交量 : %d     --->分钟内成交量 %d \n",
				pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				contractflag.last_volume_open,contractflag.last_volume,min_vol);
	}
	else
	{
		mysql_mindata.Volume =contractflag.last_volume ;
		printf ("SaveBreakTimeTailData_59s: 成交量error %s %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				contractflag.last_volume_open,contractflag.last_volume);
	}

//	printf ("Set_Database --->>> %s ",database.c_str());
	this->GetMysqlMindata()->Set_Database(this->GetMindata_Database());
//	printf ("CreateTable --->>> %s \n",tablename.c_str());
	this->GetMysqlMindata()->CreateTable(this->GetMindata_Tablename().c_str());


	bool valid1 = mysql_mindata.OpenPrice>(double)1.0;
	bool valid2 = mysql_mindata.ClosePrice>(double)1.0;
	bool valid3 = mysql_mindata.HighestPrice>(double)1.0;
	bool valid4 = mysql_mindata.LowestPrice>(double)1.0;

	bool valid =valid1 && valid2 && valid3 && valid4;
	if (valid && seconds==59)
	{

		if(!this->GetMysqlMindata()->Exist_MinuteTime(this->GetMindata_Tablename().c_str(),mysql_mindata.MinuteTime) )
		{
			printf ("SaveBreakTimeTailData_59s 插入数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
								pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
								mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

			sprintf (buf,"SaveBreakTimeTailData_59s 插入数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
					mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

			this->GetLogUtil()->PrintLog(buf);
			this->GetMysqlMindata()->Insert(this->GetMindata_Tablename().c_str(),mysql_mindata);

		}
		else
		{

			Mysql_MinData lastmindata=this->GetMysqlMindata()->Find_LastDatabyMinuteTime(this->GetMindata_Tablename().c_str());
			mysql_mindata.Volume = mysql_mindata.Volume +lastmindata.Volume;

			if (mysql_mindata.Volume != lastmindata.Volume  || mysql_mindata.ClosePrice != lastmindata.ClosePrice ||
					mysql_mindata.OpenInterest != lastmindata.OpenInterest  || mysql_mindata.HighestPrice != lastmindata.HighestPrice ||
					mysql_mindata.LowestPrice != lastmindata.LowestPrice)
			{
				this->GetMysqlMindata()->Update(this->GetMindata_Tablename().c_str(),mysql_mindata);
				printf ("SaveBreakTimeTailData_59s  更新数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
													pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
													mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );
				sprintf (buf,"SaveBreakTimeTailData_59s  更新数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
									pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
									mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

				this->GetLogUtil()->PrintLog(buf);
			}
			else
			{
				printf("SaveBreakTimeTailData_59s  数据相同------> 暂不更新 \n");
			}


		}


//		ContractFlag new_contractflag;
//										//				printf ("else -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
//		new_contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
//		new_contractflag.last_volume_open = new_contractflag.last_volume;   // 59秒时的成交量作为 下一分钟的起始成交量
//		this->GetRedisMinFlag()->SetFlag(new_contractflag);


												//				printf ("else -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);

		contractflag.last_volume_open = contractflag.last_volume;   // 59秒时的成交量作为 下一分钟的起始成交量
//		this->GetRedisMinFlag()->SetFlag(contractflag);
		this->UpdateContractFlag(contractflag);

	}
	else
	{
		printf("SaveBreakTimeTailData_59s  数据非法------> 暂不操作 \n");
	}
//	printf ("Exist_MinuteTime --->>> %s ",tablename.c_str());


}

void CMdSpi::SaveBreakTimeTailData_0s(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	char buf[2048];


	printf ("SaveBreakTimeTailData_0s:----->begin     \n");
	ContractFlag contractflag;
//	contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);
	contractflag=this->FindContractFlag(pDepthMarketData->InstrumentID);
	printf ("SaveBreakTimeTailData_0s: contractflag %s %s %s--->>> 起始成交量: %d  截止成交量 : %d   数据校验 \n",
					pDepthMarketData->InstrumentID,pDepthMarketData->ActionDay,pDepthMarketData->UpdateTime,
					contractflag.last_volume_open,contractflag.last_volume);

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());

//	std:string database ="minute_";
//	database.append(productid);
//
//	std::string tablename =pDepthMarketData->InstrumentID;
//	tablename.append("_");
//	tablename.append(this->GetTradingDay().c_str());


	bool night=codeutil.IsNightByProductid(productid);
	DateUtil dateutil;
	double Q_BarTime	= dateutil.ConvertMinuteTimetoBarTime(pDepthMarketData->UpdateTime);

	Mysql_MinData mysql_mindata;
	bool Timemore_night = (Q_BarTime>=0.210000 && Q_BarTime<0.240000);

	//当前时间如果是   23:20:00   ,则分钟数据时间未 23:19:00 ,分钟数需要减一
	std::string str_time=pDepthMarketData->UpdateTime;
	int seconds = atoi(str_time.substr(6,2).c_str());

	if (seconds<=30)	{

		std::string str_date;
		//当前时间如果是   23:20:00   ,则分钟数据时间未 23:19:00 ,分钟数需要减一
		std::string str_lastminute=dateutil.GetLastMinuteBarTime(pDepthMarketData->UpdateTime);
		std::string mintime_sql;

		// 防止跨天的时间点 和本地时间有差异,导致日期错误
		if (Timemore_night)		{
			str_date=pDepthMarketData->ActionDay;

		}
		if(strcmp(str_lastminute.c_str(),"23:59:00")==0)    { // 2018-05-09 0:00:00   减一分钟后 ---> 2018-05-07 23:59:00

			std::string sqltime_today  = dateutil.ConvertDatetoSqlTime(pDepthMarketData->ActionDay);
			time_t t_today = dateutil.ConvertSqlTimetoTimet(sqltime_today.c_str());
			str_date = dateutil.ConvertTimetoString(t_today - 60*60*24).substr(0,8);
		}
		else	{
			str_date=pDepthMarketData->ActionDay;
		}

		char temp[20];
		sprintf(temp,"%s-%s-%s ",str_date.substr(0,4).c_str(),str_date.substr(4,2).c_str(),str_date.substr(6,2).c_str());
		mintime_sql =temp;
		mintime_sql.append(str_lastminute);

		strcpy(mysql_mindata.MinuteTime,mintime_sql.c_str());

	}

	mysql_mindata.OpenPrice =contractflag.last_open;
	mysql_mindata.HighestPrice =contractflag.last_high;
	mysql_mindata.LowestPrice =contractflag.last_low;
	mysql_mindata.ClosePrice =contractflag.last_close;
	strcpy(mysql_mindata.LastTickTime,contractflag.LastTickTime);
	//持仓量
	double dOpenInterest = pDepthMarketData->OpenInterest;
	mysql_mindata.OpenInterest =dOpenInterest;


	int min_vol=contractflag.last_volume - contractflag.last_volume_open;
	if (min_vol >=0 )	{
		mysql_mindata.Volume =min_vol;
		printf ("SaveBreakTimeTailData_0s: 成交量 %s %s--->>> 起始成交量: %d  截止成交量 : %d     --->分钟内成交量 %d \n",
				pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				contractflag.last_volume_open,contractflag.last_volume,min_vol);
	}
	else	{
		mysql_mindata.Volume =contractflag.last_volume ;
		printf ("SaveBreakTimeTailData_0s: 成交量error %s %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				contractflag.last_volume_open,contractflag.last_volume);
	}

//	printf ("Set_Database --->>> %s ",database.c_str());
	this->GetMysqlMindata()->Set_Database(this->GetMindata_Database());
//	printf ("CreateTable --->>> %s \n",tablename.c_str());
	this->GetMysqlMindata()->CreateTable(this->GetMindata_Tablename().c_str());


	bool valid1 = mysql_mindata.OpenPrice>(double)1.0  &&  mysql_mindata.OpenPrice<(double)10000000.0;
	bool valid2 = mysql_mindata.ClosePrice>(double)1.0&&  mysql_mindata.ClosePrice<(double)10000000.0;
	bool valid3 = mysql_mindata.HighestPrice>(double)1.0&&  mysql_mindata.HighestPrice<(double)10000000.0;
	bool valid4 = mysql_mindata.LowestPrice>(double)1.0&&  mysql_mindata.LowestPrice<(double)10000000.0;

	bool valid =valid1 && valid2 && valid3 && valid4;
	if (valid && seconds<=30)	{

		if(!this->GetMysqlMindata()->Exist_MinuteTime(this->GetMindata_Tablename().c_str(),mysql_mindata.MinuteTime) )		{
			printf ("SaveBreakTimeTailData_0s 插入数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
					pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
					mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

			this->GetMysqlMindata()->Insert(this->GetMindata_Tablename().c_str(),mysql_mindata);

		}
		else
		{

			Mysql_MinData lastmindata=this->GetMysqlMindata()->Find_LastDatabyMinuteTime(this->GetMindata_Tablename().c_str());
			mysql_mindata.Volume = mysql_mindata.Volume +lastmindata.Volume;

			if (mysql_mindata.Volume != lastmindata.Volume  || mysql_mindata.ClosePrice != lastmindata.ClosePrice ||
					mysql_mindata.OpenInterest != lastmindata.OpenInterest  || mysql_mindata.HighestPrice != lastmindata.HighestPrice ||
					mysql_mindata.LowestPrice != lastmindata.LowestPrice)		{
				this->GetMysqlMindata()->Update(this->GetMindata_Tablename().c_str(),mysql_mindata);
				printf ("SaveBreakTimeTailData_0s  更新数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
													pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
													mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );
				sprintf (buf,"SaveBreakTimeTailData_0s  更新数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
									pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
									mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

				this->GetLogUtil()->PrintLog(buf);
			}
			else	{
				printf("SaveBreakTimeTailData_0s  数据相同------> 暂不更新 \n");
			}
		}

//		ContractFlag contractflag;
//										//				printf ("else -----> %f    -------->%d \n",Q_BarTime_2,MnKlinesig);
//		contractflag=this->GetRedisMinFlag()->GetFlag(pDepthMarketData->InstrumentID);

		contractflag.last_volume_open = contractflag.last_volume;   // 59秒时的成交量作为 下一分钟的起始成交量
//		this->GetRedisMinFlag()->SetFlag(contractflag);
		this->UpdateContractFlag(contractflag);
	}
	else	{
		printf("SaveBreakTimeTailData_0s  数据非法------> 暂不操作 \n");
	}
//	printf ("Exist_MinuteTime --->>> %s ",tablename.c_str());

	printf ("SaveBreakTimeTailData_0s:----->end      \n");
}

void CMdSpi::SaveMinData_59s(CThostFtdcDepthMarketDataField *pDepthMarketData,ContractFlag contractflag)
{

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pDepthMarketData->InstrumentID).c_str());


	bool night=codeutil.IsNightByProductid(productid);

	std:string database ="minute_";
	database.append(productid);

	std::string tablename =pDepthMarketData->InstrumentID;
	tablename.append("_");
	tablename.append(this->GetTradingDay().c_str());
	DateUtil dateutil;
	double Q_BarTime	= dateutil.ConvertMinuteTimetoBarTime(pDepthMarketData->UpdateTime);

	Mysql_MinData mysql_mindata;

	bool Timemore_night = (Q_BarTime>=0.210000 && Q_BarTime<0.240000);

	std::string str_date;
	//当前时间如果是   23:20:00   ,则分钟数据时间未 23:19:00 ,分钟数需要减一
	std::string str_temp=pDepthMarketData->UpdateTime;
	char c_buf[20];
	sprintf(c_buf,"%s:00",str_temp.substr(0,5).c_str());
	std::string str_lastminute=c_buf;

	std::string mintime_sql;

	// 防止跨天的时间点 和本地时间有差异,导致日期错误
	if (Timemore_night)
	{
		str_date=pDepthMarketData->ActionDay;

	}
	if(strcmp(str_lastminute.c_str(),"23:59:00")==0)     // 2018-05-09 0:00:00   减一分钟后 ---> 2018-05-07 23:59:00
	{
		std::string sqltime_today  = dateutil.ConvertDatetoSqlTime(pDepthMarketData->ActionDay);
		time_t t_today = dateutil.ConvertSqlTimetoTimet(sqltime_today.c_str());
		str_date = dateutil.ConvertTimetoString(t_today - 60*60*24).substr(0,8);
	}
	else
	{
		str_date=pDepthMarketData->ActionDay;
	}

	char temp[20];
	sprintf(temp,"%s-%s-%s ",str_date.substr(0,4).c_str(),str_date.substr(4,2).c_str(),str_date.substr(6,2).c_str());
	mintime_sql =temp;
	mintime_sql.append(str_lastminute);

	strcpy(mysql_mindata.MinuteTime,mintime_sql.c_str());
	mysql_mindata.OpenPrice =contractflag.last_open;
	mysql_mindata.HighestPrice =contractflag.last_high;
	mysql_mindata.LowestPrice =contractflag.last_low;
	mysql_mindata.ClosePrice =contractflag.last_close;
	strcpy(mysql_mindata.LastTickTime,contractflag.LastTickTime);

	//持仓量
	double dOpenInterest = pDepthMarketData->OpenInterest;
	mysql_mindata.OpenInterest =dOpenInterest;


	// 开盘的第一分钟  成交量 =就是当前的成交量-0
	if (night && strcmp(str_lastminute.c_str(),"21:00:00")==0)
	{
		mysql_mindata.Volume = contractflag.last_volume;
		printf ("夜盘第一分钟成交量  %s  %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				contractflag.last_volume_open,contractflag.last_volume);
	}
	else  if (!night && strcmp(str_lastminute.c_str(),"09:00:00")==0)
	{
		printf ("日盘第一分钟成交量  %s  %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
				contractflag.last_volume_open,contractflag.last_volume);
		mysql_mindata.Volume =contractflag.last_volume ;
	}
	else
	{
		//成交量
		int min_vol=contractflag.last_volume - contractflag.last_volume_open;
		if (min_vol >0 )
		{
			mysql_mindata.Volume =min_vol;
			printf ("SaveMinData_59s:成交量 %s %s--->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
					contractflag.last_volume_open,contractflag.last_volume);
		}
		else
		{
			mysql_mindata.Volume =contractflag.last_volume ;
			printf ("SaveMinData_59s:???成交量 %s %s --->>> 起始成交量: %d  截止成交量 : %d \n",pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,
					contractflag.last_volume_open,contractflag.last_volume);
		}
	}



//	printf ("Set_Database --->>> %s ",database.c_str());
	MysqlMinData _mysqlmindata;
	_mysqlmindata.Set_Database(database.c_str());

//	printf ("CreateTable --->>> %s \n",tablename.c_str());

	_mysqlmindata.CreateTable(tablename.c_str());


	bool valid1 = mysql_mindata.OpenPrice>(double)1.0;
	bool valid2 = mysql_mindata.ClosePrice>(double)1.0;
	bool valid3 = mysql_mindata.HighestPrice>(double)1.0;
	bool valid4 = mysql_mindata.LowestPrice>(double)1.0;

	//					printf ("11111--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f   \n",
	//						names,minute_time.c_str(),mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,mysql_mindata.LowestPrice,mysql_mindata.ClosePrice);

	bool valid =valid1 && valid2 && valid3 && valid4;
	//					if (valid)
	//					{
	//						printf ("22222--->>> 合约名称:%s 时间: %s  开盘价：%f  最高价: %f 最低价: %f  收盘价: %f   \n",
	//												names,minute_time.c_str(),mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,mysql_mindata.LowestPrice,mysql_mindata.ClosePrice);
	//
	//					}
//	printf ("Exist_MinuteTime --->>> %s ",tablename.c_str());
	if(!_mysqlmindata.Exist_MinuteTime(tablename.c_str(),mysql_mindata.MinuteTime) && valid)
	{
		printf ("插入数据--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
				pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
				mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );

		_mysqlmindata.Insert(tablename.c_str(),mysql_mindata);
		SaveRedisMinData(pDepthMarketData,mysql_mindata);
	}
	else
	{
		printf ("数据不合规--->>> 合约名称:%s  分钟时间:%s ---> %s  开盘价：%.3f  最高价: %.3f 最低价: %.3f  收盘价: %.3f  成交量: %d \n",
						pDepthMarketData->InstrumentID,pDepthMarketData->UpdateTime,mysql_mindata.MinuteTime,mysql_mindata.OpenPrice,mysql_mindata.HighestPrice,
						mysql_mindata.LowestPrice,mysql_mindata.ClosePrice,mysql_mindata.Volume );
	}

}
void CMdSpi::SaveTickDatatoRedis(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
//	printf("--->>>  SaveTickDatatoRedis...\n");
	///合约名称 TThostFtdcInstrumentNameType char[21]

//	struct timeval tv;
//	    gettimeofday(&tv,NULL);
////	    printf("SaveTickDatatoRedis second:%ld\n",tv.tv_sec);  //秒
////	    printf("SaveTickDatatoRedis millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
////
//	    printf("SaveTickDatatoRedis begin  ---->%ld.%ld\n",tv.tv_sec,tv.tv_usec);  //秒
//	    long s =tv.tv_sec*1000 + tv.tv_usec/1000;

	char InstrumentID[40];
	gbk2utf8(pDepthMarketData->InstrumentID,InstrumentID,sizeof(InstrumentID));



	char productid[30];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(InstrumentID).c_str());


	char ActionDay[27];
	gbk2utf8(pDepthMarketData->ActionDay,ActionDay,sizeof(ActionDay));

	char UpdateTime[20];
	gbk2utf8(pDepthMarketData->UpdateTime,UpdateTime,sizeof(UpdateTime));

	double OpenInterest = pDepthMarketData->OpenInterest;
	double Volume =(double) pDepthMarketData->Volume;

	char TradingDay[27];
	gbk2utf8(pDepthMarketData->TradingDay,TradingDay,sizeof(TradingDay));

	DepthMarket depthmarket;

	strcpy(depthmarket.InstrumentID,InstrumentID);

	strcpy(depthmarket.UpdateTime,UpdateTime);
	strcpy(depthmarket.TradingDay,TradingDay);
	strcpy(depthmarket.ActionDay,ActionDay);


	depthmarket.UpdateMillisec=pDepthMarketData->UpdateMillisec;
	depthmarket.OpenInterest=OpenInterest;
	depthmarket.Volume=Volume;


	depthmarket.LastPrice = pDepthMarketData->LastPrice;
	depthmarket.AskPrice1 = pDepthMarketData->AskPrice1;
	depthmarket.BidPrice1 = pDepthMarketData->BidPrice1;
	depthmarket.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
	depthmarket.PreClosePrice = pDepthMarketData->PreClosePrice;
	depthmarket.PreOpenInterest =pDepthMarketData->PreOpenInterest;

	depthmarket.OpenPrice = pDepthMarketData->OpenPrice;
	depthmarket.HighestPrice = pDepthMarketData->HighestPrice;
	depthmarket.LowestPrice =pDepthMarketData->LowestPrice;
	depthmarket.Volume =pDepthMarketData->Volume;
	depthmarket.Turnover = pDepthMarketData->Turnover;
	depthmarket.ClosePrice = pDepthMarketData->ClosePrice;
	depthmarket.SettlementPrice =pDepthMarketData->SettlementPrice;

	depthmarket.UpperLimitPrice = pDepthMarketData->UpperLimitPrice;
	depthmarket.LowerLimitPrice = pDepthMarketData->LowerLimitPrice;

	ProductData productdata;
	productdata = this->GetMysqlProduct()->Find_DatabyProductID(productid);

	strcpy(depthmarket.ExchangeID,productdata.ExchangeID);
	strcpy(depthmarket.ProductID,productid);
	strcpy(depthmarket.Section,productdata.Section);
	strcpy(depthmarket.DayNight,productdata.DayNight);

//	if(!this->GetMysqlInstrument()->Exist_DatabyInstrumentID(InstrumentID)){
//		printf ("-----无基础数据  VolumeMultiple----->%s   %s   %s  \n" ,depthmarket.ExchangeID,InstrumentID,UpdateTime);
//		return;
//	}
//
//	if(!this->GetMysqlCommission()->Exist_DatabyProductID(productid)){
//		printf ("-----无基础数据  Margin----->%s   %s   %s  \n" ,depthmarket.ExchangeID,InstrumentID,UpdateTime);
//		return;
//	}
//
//	InstrumentData insdata;
//	insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(InstrumentID);
//
//	Commission commissiondata=this->GetMysqlCommission()->Find_DatabyProductID(productid);

//	std::string key=productid;
	std::shared_ptr<Commission>  commission=this->FindCommissionData(productid);

	 if(strcmp(depthmarket.ExchangeID,"SHFE")==0 || strcmp(depthmarket.ExchangeID,"DCE")==0)	 {

		 if(commission->VolumeMultiple>0){
			 depthmarket.AveragePrice = pDepthMarketData->AveragePrice/commission->VolumeMultiple;
		 }
		 else{
			 depthmarket.AveragePrice = pDepthMarketData->AveragePrice;
		 }

	 }
	 else	 {
		 depthmarket.AveragePrice = pDepthMarketData->AveragePrice;
	 }


//	printf ("------------------exchangeid ----->%s   %s   %s  \n" ,depthmarket.ExchangeID,InstrumentID,UpdateTime);
	 depthmarket.Margin = commission->Margin;
	depthmarket.LongMarginRatio = commission->Margin;
	depthmarket.ShortMarginRatio = commission->Margin;
	depthmarket.VolumeMultiple = commission->VolumeMultiple;

	// 待完善
	depthmarket.PriceTick=commission->ChangeNumber;
//	depthmarket.PriceTick = insdata.PriceTick;


	depthmarket.Sedimentary = depthmarket.LastPrice * commission->Margin *depthmarket.VolumeMultiple *depthmarket.OpenInterest/100000000.0;
	depthmarket.LastSedimentary = depthmarket.PreClosePrice * commission->Margin*depthmarket.VolumeMultiple *depthmarket.PreOpenInterest/100000000.0;

	if(depthmarket.Sedimentary>0){
		depthmarket.SedimentaryIORate = ( depthmarket.Sedimentary  -depthmarket.LastSedimentary)/depthmarket.LastSedimentary;
	}
	else{
		depthmarket.SedimentaryIORate=0.0;
	}

	if(depthmarket.PreSettlementPrice>0){
		depthmarket.OpenPreSettleRate = (depthmarket.OpenPrice - depthmarket.PreSettlementPrice )/depthmarket.PreSettlementPrice;
	}
	else{
		depthmarket.OpenPreSettleRate=0.0;
	}

	if(depthmarket.OpenPrice>0){
		depthmarket.LastOpenRate = (depthmarket.LastPrice - depthmarket.OpenPrice )/depthmarket.OpenPrice;
	}
	else{
		depthmarket.LastOpenRate=0.0;
	}

	if(depthmarket.PreSettlementPrice>0){
		depthmarket.UpdownRate = (depthmarket.LastPrice - depthmarket.PreSettlementPrice )/depthmarket.PreSettlementPrice;
	}
	else{
		depthmarket.UpdownRate=0.0;
	}


	//日增仓比例
	if(depthmarket.PreOpenInterest>0){
		depthmarket.AddOpenInterestRate = (depthmarket.OpenInterest - depthmarket.PreOpenInterest )/depthmarket.PreOpenInterest;
	}
	else{
		depthmarket.AddOpenInterestRate=0.0;
	}




//			printf("--->>>  redisdepthmarket.SetDepthMarket...\n");
	this->GetRedisDepthMarket()->SetDepthMarket(depthmarket);
//			redisdepthmarket.disConnect();

//			printf("--->>>  mongotickdata.Check_TickData...\n");
//			mongotickdata.Check_TickData(depthmarket);

//	char times1[10];
//	strcpy(times1, pDepthMarketData->UpdateTime);
//	std::string str_times=times1;
//
//	string str_sec=str_times.substr(6,2);		//
//	int iseconds = atoi(str_sec.c_str());

//			if (iseconds==58 || iseconds==59 || iseconds==00 || iseconds==01 )
//			{
//				printf ("--->>>交易日期:%s  执行日期:%s  合约名:%s   更新日期:%s   最新价:%f  持仓量: %f 成交量: %f  UpdateMillisec %d \n",TradingDay,ActionDay,InstrumentID,UpdateTime,depthmarket.LastPrice,OpenInterest,Volume,depthmarket.UpdateMillisec);
//
//			}

//	struct timeval tv_last;
//		    gettimeofday(&tv_last,NULL);
////		    printf("SaveTickDatatoRedis second:%ld\n",tv_last.tv_sec);  //秒
////		    printf("SaveTickDatatoRedis millisecond:%ld\n",tv_last.tv_sec*1000 + tv_last.tv_usec/1000);  //毫秒
////
//
//		    printf("SaveTickDatatoRedis end------------->%ld.%ld\n",tv_last.tv_sec,tv_last.tv_usec);  //秒
//		    long l =tv_last.tv_sec*1000 + tv_last.tv_usec/1000;
//
//DateUtil dateutil;
//		printf("SaveTickDatatoRedis...  %s------->>>> 结束   %s    消耗  %ld \n",InstrumentID,
//				dateutil.ConvertCurrentTimetoSqlTime().c_str(),l-s);

//		printf("--->>>  SaveTickDatatoRedis..   end !!! \n");
}
