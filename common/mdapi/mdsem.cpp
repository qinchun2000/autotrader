#include "mdsem.hpp"


MDSem::MDSem() {

}


int MDSem::TimeWaitSem(sem_t* sem)
 {

		struct timeval now;
		struct timespec outtime;

		gettimeofday(&now, NULL);
		cout << now.tv_sec << " " << (now.tv_usec) << "\n";
		DateUtil dateutil;
		dateutil.timeraddMS(&now, 15000);
		outtime.tv_sec = now.tv_sec;
		outtime.tv_nsec = now.tv_usec * 1000;
		int ret;
		ret = sem_timedwait(sem,&outtime);
		return ret;
 }

int MDSem::ResetSemInit()
{
	 return  sem_init(&_sem_init,0,0);
}

void MDSem::PostSemInit()
{
	 sem_post(&_sem_init);
}
sem_t* MDSem::GetSemInit()
{
//	 sem_t *  ret= &this->_sem_init;
//	 return ret;
	 return &this->_sem_init;
}

int MDSem::ResetSemLogin()
{
	 return  sem_init(&_sem_login,0,0);
}

void MDSem::PostSemLogin()
{
	 sem_post(&_sem_login);
}

sem_t* MDSem::GetSemLogin()
{

	 return &this->_sem_login;
}

int MDSem::ResetSemSubscribe()
{
	 return  sem_init(&_sem_subscribe,0,0);
}

void MDSem::PostSemSubscribe()
{
	 sem_post(&_sem_subscribe);
}
sem_t* MDSem::GetSemSubscribe()
{

	 return &this->_sem_subscribe;
}

int MDSem::ResetSemUnSubscribe()
{
	 return  sem_init(&_sem_unsubscribe,0,0);
}

void MDSem::PostSemUnSubscribe()
{
	 sem_post(&_sem_unsubscribe);
}
sem_t* MDSem::GetSemUnSubscribe()
{

	 return &this->_sem_unsubscribe;
}
