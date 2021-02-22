#include "sembase.hpp"


SemBase::SemBase() {

}

void SemBase::SetTimeWaitSeconds(int sec)
 {
	 this->_timewaitseconds=sec;
 }

 int SemBase::GetTimeWaitSeconds()
 {
	 return this->_timewaitseconds;
 }


int SemBase::TimeWaitSem(sem_t* SemBase)
 {

		struct timeval now;
		struct timespec outtime;

		gettimeofday(&now, NULL);
//		cout << now.tv_sec << " " << (now.tv_usec) << "\n";
		DateUtil dateutil;
		dateutil.timeraddMS(&now, this->GetTimeWaitSeconds());
//		printf("SemBase: TimeWaitSem --->%d 秒 \n",this->GetTimeWaitSeconds()/1000);
		outtime.tv_sec = now.tv_sec;
		outtime.tv_nsec = now.tv_usec * 1000;
		int ret;
		ret = sem_timedwait(SemBase,&outtime);
		return ret;
 }

int SemBase::LongTimeWaitSem(sem_t* SemBase)
 {

		int weight =20;
		struct timeval now;
		struct timespec outtime;

		gettimeofday(&now, NULL);
//		cout << now.tv_sec << " " << (now.tv_usec) << "\n";
		DateUtil dateutil;
		dateutil.timeraddMS(&now, this->GetTimeWaitSeconds()*weight);
		printf("SemBase: LongTimeWaitSem --->%d 秒 \n",this->GetTimeWaitSeconds()*weight/1000);
		outtime.tv_sec = now.tv_sec;
		outtime.tv_nsec = now.tv_usec * 1000;
		int ret;
		ret = sem_timedwait(SemBase,&outtime);
		return ret;
 }

int SemBase::ResetSemInit()
{
	 return  sem_init(&_sem_init,0,0);
}

void SemBase::PostSemInit()
{
	 sem_post(&_sem_init);
}
sem_t* SemBase::GetSemInit()
{
//	 sem_t *  ret= &this->_sem_init;
//	 return ret;
	 return &this->_sem_init;
}


int SemBase::ResetSemAuthenticate()
{
	 return  sem_init(&_sem_authenticate,0,0);
}

void SemBase::PostSemAuthenticate()
{
	 sem_post(&_sem_authenticate);
}
sem_t* SemBase::GetSemAuthenticate()
{
	 sem_t*  ret=&_sem_authenticate;
	 return ret;

}


int SemBase::ResetSemLogin()
{
	 return  sem_init(&_sem_login,0,0);
}

void SemBase::PostSemLogin()
{
	 sem_post(&_sem_login);
}
sem_t* SemBase::GetSemLogin()
{


	 sem_t*  ret=&_sem_login;
	  	 return ret;

}
int SemBase::ResetSemInvestor()
{
	 return  sem_init(&_sem_investor,0,0);
}

void SemBase::PostSemInvestor()
{
	 sem_post(&_sem_investor);
}
sem_t* SemBase::GetSemInvestor()
{
	  sem_t*  ret=&_sem_investor;
	 return ret;
}

int SemBase::ResetSemTradingAccount()
 {
	 return  sem_init(&_sem_tradingaccount,0,0);
 }

 void SemBase::PostSemTradingAccount()
 {
	 sem_post(&_sem_tradingaccount);
 }
 sem_t* SemBase::GetSemTradingAccount()
 {
	  sem_t*  ret=&_sem_tradingaccount;
	 return ret;
 }

 int SemBase::ResetSemOrder()
  {
	 return sem_init(&_sem_order,0,0);
  }

  void SemBase::PostSemOrder()
  {
 	 sem_post(&_sem_order);
  }
  sem_t* SemBase::GetSemOrder()
  {
	  sem_t*  ret=&_sem_order;
 	 return ret;
  }


int SemBase::ResetSemTrade()
{
	 return sem_init(&_sem_trade,0,0);
}

void SemBase::PostSemTrade()
{
	 sem_post(&_sem_trade);
}
sem_t* SemBase::GetSemTrade()
{
sem_t*  ret=&_sem_trade;
	 return ret;
}


int SemBase::ResetSemInvest()
{
	 return  sem_init(&_sem_investorpostion,0,0);
}

void SemBase::PostSemInvest()
{
	 sem_post(&_sem_investorpostion);
}
sem_t* SemBase::GetSemInvest()
{
 sem_t*  ret=&_sem_investorpostion;
	 return ret;
}


int SemBase::ResetSemInvestPosDetail()
 {
	 return  sem_init(&_sem_investorpostiondetail,0,0);
 }

 void SemBase::PostSemInvestPosDetail()
 {
	 sem_post(&_sem_investorpostiondetail);
 }
 sem_t* SemBase::GetSemInvestPosDetail()
 {
  sem_t*  ret=&_sem_investorpostiondetail;
	 return ret;
 }

 int SemBase::ResetSemNotice()
  {
	 return  sem_init(&_sem_notice,0,0);
  }

  void SemBase::PostSemNotice()
  {
 	 sem_post(&_sem_notice);
  }
  sem_t* SemBase::GetSemNotice()
  {
   sem_t*  ret=&_sem_notice;
 	 return ret;
  }


  int SemBase::ResetSemOrderAction()
   {
	  return sem_init(&_sem_orderaction,0,0);
   }

   void SemBase::PostSemOrderAction()
   {
  	 sem_post(&_sem_orderaction);
   }
   sem_t* SemBase::GetSemOrderAction()
   {
    sem_t*  ret=&_sem_orderaction;
  	 return ret;
   }


   int SemBase::ResetSemMaxOrder()
    {
	   return sem_init(&_sem_querymaxordervolume,0,0);
    }

    void SemBase::PostSemMaxOrder()
    {
   	 sem_post(&_sem_querymaxordervolume);
    }
    sem_t* SemBase::GetSemMaxOrder()
    {
     sem_t*  ret=&_sem_querymaxordervolume;
   	 return ret;
    }


int SemBase::ResetSemQrySettleInfo()
{
	 return sem_init(&_sem_qrysettleinfo,0,0);
}

void SemBase::PostSemQrySettleInfo()
{
 sem_post(&_sem_qrysettleinfo);
}
sem_t* SemBase::GetSemQrySettleInfo()
{
sem_t*  ret=&_sem_qrysettleinfo;
 return ret;
}



int SemBase::ResetSemQrySettleInfoConfirm()
{
	 return sem_init(&_sem_qrysettleinfoconfirm,0,0);
}

void SemBase::PostSemQrySettleInfoConfirm()
{
 sem_post(&_sem_qrysettleinfoconfirm);
}
sem_t* SemBase::GetSemQrySettleInfoConfirm()
{
sem_t*  ret=&_sem_qrysettleinfoconfirm;
 return ret;
}


int SemBase::ResetSemSettleInfoConfirm()
{
	 return sem_init(&_sem_settleinfoconfirm,0,0);
}

void SemBase::PostSemSettleInfoConfirm()
{
 sem_post(&_sem_settleinfoconfirm);
}
sem_t* SemBase::GetSemSettleInfoConfirm()
{
sem_t*  ret=&_sem_settleinfoconfirm;
 return ret;
}



int SemBase::ResetSemExchange()
{
	 return sem_init(&_sem_exchange,0,0);
}

void SemBase::PostSemExchange()
{
 sem_post(&_sem_exchange);
}
sem_t* SemBase::GetSemExchange()
{
sem_t*  ret=&_sem_exchange;
 return ret;
}


int SemBase::ResetSemInstrument()
{
	 return sem_init(&_sem_instrument,0,0);
}

void SemBase::PostSemInstrument()
{
 sem_post(&_sem_instrument);
}
sem_t* SemBase::GetSemInstrument()
{
sem_t*  ret=&_sem_instrument;
 return ret;
}




int SemBase::ResetSemDepthMarket()
{
	 return sem_init(&_sem_depthmarketdata,0,0);
}

void SemBase::PostSemDepthMarket()
{
 sem_post(&_sem_depthmarketdata);
}
sem_t* SemBase::GetSemDepthMarket()
{
sem_t*  ret=&_sem_depthmarketdata;
 return ret;
}



int SemBase::ResetSemProductGroup()
{
	 return sem_init(&_sem_productgroup,0,0);
}

void SemBase::PostSemProductGroup()
{
 sem_post(&_sem_productgroup);
}
sem_t* SemBase::GetSemProductGroup()
{
sem_t*  ret=&_sem_productgroup;
 return ret;
}


int SemBase::ResetSemLogout()
{
	 return sem_init(&_sem_logout,0,0);
}

void SemBase::PostSemLogout()
{
 sem_post(&_sem_logout);
}
sem_t* SemBase::GetSemLogout()
{
sem_t*  ret=&_sem_logout;
 return ret;
}
