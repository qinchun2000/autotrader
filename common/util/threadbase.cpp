#include "threadbase.hpp"

//int ThreadBase::start()
//{
//	if(pthread_create(&pid,NULL,start_thread,(void *)this) != 0)                 //创建一个线程(必须是全局函数)
//	{
//		return -1;
//	}
//	return 0;
//}
//
//void* ThreadBase::start_thread(void *arg) //静态成员函数只能访问静态变量或静态函数，通过传递this指针进行调用
//{
//	ThreadBase *ptr = (ThreadBase *)arg;
//    ptr->run();
//    return 0;                                                                                             //线程的实体是run
//}

void ThreadBase::SetInstrumentID(const char* ins)
{
	this->_instrument=ins;
}
std::string ThreadBase::GetInstrumentID()
{
return this->_instrument;
}
void* ThreadBase::run0(void* pVoid)
{
	ThreadBase* p = (ThreadBase*) pVoid;
    p->run1();
    return p;
}

void* ThreadBase::run1()
{
    threadStatus = THREAD_STATUS_RUNNING;
    tid = pthread_self();
    run();
    threadStatus = THREAD_STATUS_EXIT;
    tid = 0;
    pthread_exit(NULL);
}

ThreadBase::ThreadBase()
{
//	printf("ThreadBase  构造函数 \n");
    tid = 0;
    threadStatus = THREAD_STATUS_NEW;
}

bool ThreadBase::start()
{
        int iRet = 0;
     if (pthread_create(&tid, NULL, thread_proxy_func, this) == 0) {
    	 printf("ThreadBase  start ok \n");
    	 return true;
     }
     else{
    	 printf("ThreadBase  start error \n");
    	 return false;
     }
}

//void* ThreadBase::run0(void* pVoid)
//{
//	ThreadBase* p = (ThreadBase*) pVoid;
//    p->run1();
//    return p;
//}
//
//void* ThreadBase::run1()
//{
//    threadStatus = THREAD_STATUS_RUNNING;
//    tid = pthread_self();
//    run();
//    threadStatus = THREAD_STATUS_EXIT;
//    tid = 0;
//    pthread_exit(NULL);
//}


pthread_t ThreadBase::getThreadID()
{
    return tid;
}

int ThreadBase::getState()
{
    return threadStatus;
}

void ThreadBase::join()
{
    if (tid > 0)
    {
        pthread_join(tid, NULL);
    }
}
void * ThreadBase::thread_proxy_func(void * args)
{
	ThreadBase * pThread = static_cast<ThreadBase *>(args);

        pThread->run();

        return NULL;
}

void ThreadBase::join(unsigned long millisTime)
{
    if (tid == 0)
    {
        return;
    }
    if (millisTime == 0)
    {
        join();
    }else
    {
        unsigned long k = 0;
        while (threadStatus != THREAD_STATUS_EXIT && k <= millisTime)
        {
            usleep(100);
            k++;
        }
    }
}
