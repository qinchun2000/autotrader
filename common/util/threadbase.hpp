#ifndef THREADBASE_HPP
#define THREADBASE_HPP
#include <iostream>
#include <pthread.h>
#include <cstdint>
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <string>

using namespace std;
class ThreadBase
{

private:
    //当前线程的线程ID
    pthread_t tid;
    //线程的状态
    int threadStatus;
    std::string _instrument;
    //获取执行方法的指针
    static void* run0(void* pVoid);
    static void * thread_proxy_func(void * args);
    //内部执行方法
    void* run1();
public:
    //线程的状态－新建
    static const int THREAD_STATUS_NEW = 0;
    //线程的状态－正在运行
    static const int THREAD_STATUS_RUNNING = 1;
    //线程的状态－运行结束
    static const int THREAD_STATUS_EXIT = -1;

    //构造函数
    ThreadBase();
	//线程的运行实体
	virtual void run()=0;
	//开始执行线程
	bool start();
	void SetInstrumentID(const char* ins);
	std::string GetInstrumentID();
    ///////////////////////////////
//    private:
//        pthread_t pid;
//    private:
//        static void * start_thread(void *arg);                                                       //静态成员函数
//    public:
//        int start();
//        virtual void run() = 0;//基类中的虚函数要么实现，要么是纯虚函数（绝对不允许声明不实现，也不纯虚）
        virtual ~ThreadBase() {}; // 注意，有{}实现

        //获取线程ID
		pthread_t getThreadID();
		//获取线程状态
		int getState();
		//等待线程直至退出
		void join();
		//等待线程退出或者超时
		void join(unsigned long millisTime);
};

class MThread : public ThreadBase
{
public:
    void run()
    {
        int number = 0;
        for (int i = 0; i < 10; i++)
        {
            cout << "Current number is " << number++;
            cout << " PID is " << getpid() << " TID is " << getThreadID() << endl;
            sleep(1);
        }
    }
};
#endif
