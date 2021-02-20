#ifndef _MDSEM_HPP_
#define _MDSEM_HPP_
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

class MDSem
{

public:

	MDSem();
	  int TimeWaitSem(sem_t* sem);
	  int ResetSemInit();
	 	 void PostSemInit();
	 	 sem_t* GetSemInit();
	  int ResetSemLogin();
	  void PostSemLogin();
	  sem_t* GetSemLogin();
	  int ResetSemSubscribe();
	  void PostSemSubscribe();
	  sem_t* GetSemSubscribe();

	  int ResetSemUnSubscribe();
	  void PostSemUnSubscribe();
	  sem_t* GetSemUnSubscribe();

private:
	sem_t _sem_init;
		sem_t _sem_login;
		sem_t _sem_subscribe;
		sem_t _sem_unsubscribe;
};

#endif /* _MDSEM_HPP_ */
