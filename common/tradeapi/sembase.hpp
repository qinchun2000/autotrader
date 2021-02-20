#ifndef _SEM_HPP_
#define _SEM_HPP_
// 标准C库文件
#include <cstdint>
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <boost/regex.hpp>

// 线程控制相关
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// 定时器相关
#include <signal.h>
#include <sys/time.h>
#include "lock.hpp"
#include "dateutil.hpp"



using namespace boost;
using namespace std;

class SemBase
{

public:

	SemBase();
	 void SetTimeWaitSeconds(int sec);
	  int GetTimeWaitSeconds();



	  int TimeWaitSem(sem_t* sem);
	  int LongTimeWaitSem(sem_t* SemBase);
	 int ResetSemInit();
	 void PostSemInit();
	 sem_t* GetSemInit();

	 int ResetSemAuthenticate();
	 void PostSemAuthenticate();
	 sem_t* GetSemAuthenticate();

	 int ResetSemLogin();
	 void PostSemLogin();
	 sem_t* GetSemLogin();

	 int ResetSemInvestor();
	 void PostSemInvestor();
	 sem_t* GetSemInvestor();

	 int ResetSemTradingAccount();
	 void PostSemTradingAccount();
	 sem_t* GetSemTradingAccount();

	 int ResetSemOrder();
	 void PostSemOrder();
	 sem_t* GetSemOrder();

	 int ResetSemTrade();
	 void PostSemTrade();
	 sem_t* GetSemTrade();

	 int ResetSemInvest();
	 void PostSemInvest();
	 sem_t* GetSemInvest();


	 int ResetSemInvestPosDetail();
	 void PostSemInvestPosDetail();
	 sem_t* GetSemInvestPosDetail();

	 int ResetSemNotice();
	 void PostSemNotice();
	 sem_t* GetSemNotice();

	 int ResetSemOrderAction();
	 void PostSemOrderAction();
	 sem_t* GetSemOrderAction();

	 int ResetSemMaxOrder();
	 void PostSemMaxOrder();
	 sem_t* GetSemMaxOrder();


	 int ResetSemQrySettleInfoConfirm();
	 void PostSemQrySettleInfoConfirm();
	 sem_t* GetSemQrySettleInfoConfirm();


	 int ResetSemQrySettleInfo();
	 void PostSemQrySettleInfo();
	 sem_t* GetSemQrySettleInfo();


	 int ResetSemSettleInfoConfirm();
	 void PostSemSettleInfoConfirm();
	 sem_t* GetSemSettleInfoConfirm();

	 int ResetSemExchange();
	 void PostSemExchange();
	 sem_t* GetSemExchange();

	 int ResetSemInstrument();
	 void PostSemInstrument();
	 sem_t* GetSemInstrument();


	 int ResetSemDepthMarket();
	 void PostSemDepthMarket();
	 sem_t* GetSemDepthMarket();

	 int ResetSemProductGroup();
	 void PostSemProductGroup();
	 sem_t* GetSemProductGroup();

	 int ResetSemLogout();
		 void PostSemLogout();
		 sem_t* GetSemLogout();

private:
	   int _timewaitseconds=15000;
	   sem_t _sem_init;
	   sem_t _sem_authenticate;
	   sem_t _sem_login;
	   sem_t _sem_investor;
	   sem_t _sem_tradingaccount;
	   sem_t _sem_order;
	   sem_t _sem_trade;
	   sem_t _sem_investorpostion;
	   sem_t _sem_investorpostiondetail;
	   sem_t _sem_notice;
	   sem_t _sem_orderaction;
	   sem_t _sem_querymaxordervolume;
	   sem_t _sem_qrysettleinfoconfirm;
	   sem_t _sem_qrysettleinfo;
	   sem_t _sem_settleinfoconfirm;
	   sem_t _sem_exchange;
	   sem_t _sem_instrument;
	   sem_t _sem_depthmarketdata;
	   sem_t _sem_productgroup;
	   //extern sem_t sem_orderinsert;
	   sem_t _sem_logout;
};

#endif /* _SEM_HPP_ */
