#include "eventengine.hpp"
EventEngine::EventEngine()
{
	//构造函数
	//先创建一个队列对象给引擎用
	m_event_queue = new SynQueue<Event>;
	//创建一个对比映射的multimap
	m_task_map = new std::multimap<std::string, TASK>;
}

EventEngine::~EventEngine()
{
	this->StopEngine();
	if (m_event_queue)
	{
		delete m_event_queue;
		m_event_queue = nullptr;
	}

	if (m_task_map)
	{
		delete m_task_map;
		m_task_map = nullptr;
	}
}

void EventEngine::RegEvent(std::string eventtype, TASK task)
{
	//注册事件和对应处理函数到映射
	m_mutex.lock();
	m_task_map->insert(std::make_pair(eventtype, task));
	m_mutex.unlock();
}

void EventEngine::UnregEvent(std::string eventtype)
{
	//退订对比映射的事件
	m_mutex.lock();
	m_task_map->erase(eventtype);
	m_mutex.unlock();
}

void EventEngine::Put(std::shared_ptr<Event>e)
{
	m_event_queue->Push(e);
}

void EventEngine::StartEngine()
{
	//启动引擎
	m_active = true;

	m_timer_thread = new std::thread(std::bind(&EventEngine::Timer, this));

	m_task_pool = new std::vector<std::thread*>;
	std::function<void()>f = std::bind(&EventEngine::DoTask, this);

	for (unsigned i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		//启动线程池
		std::thread* thread_worker = new std::thread(f);
		m_task_pool->push_back(thread_worker);
		std::cout<<"start engine--->"<<i<<std::endl;
	}
}
void EventEngine::StartSingleEngine()
{
	//启动引擎
	m_active = true;
	m_timer_thread = new std::thread(std::bind(&EventEngine::Timer, this));
	m_task_pool = new std::vector<std::thread*>;
	std::function<void()>f = std::bind(&EventEngine::DoTask, this);

	for (unsigned i = 0; i < 1; i++)
	{
		//启动线程池
		std::thread* thread_worker = new std::thread(f);
		m_task_pool->push_back(thread_worker);
		std::cout <<"s engine   ---> "<<i<<std::endl;
	}
}
void EventEngine::StopEngine()
{
	//停止引擎前先将阻塞的条件变量通过递交退出事件 退出否则会内存泄漏
	for (unsigned int i = 0; i < std::thread::hardware_concurrency(); i++)
	{
		std::shared_ptr<Event_Exit>e = std::make_shared<Event_Exit>();
		this->Put(e);
	}

	m_active = false;

	if (m_timer_thread)
	{
		m_timer_thread->join();
		delete m_timer_thread;
		m_timer_thread = nullptr;
	}

	if (m_task_pool != nullptr)
	{
		for (std::vector<std::thread*>::iterator it = m_task_pool->begin(); it != m_task_pool->end(); it++)
		{
			(*it)->join();
			delete (*it);
		}
		delete m_task_pool;
		m_task_pool = nullptr;
	}
}

void EventEngine::DoTask()
{
	while (m_active)
	{


//		 std::thread::id this_id = std::this_thread::get_id();
//
//		std::cout << "do task: id = " << this_id  << std::endl;

		sleep(1);
		std::shared_ptr<Event>e = m_event_queue->Take();															//将事件取出。
		//处理关闭时退出
		e->GetEventType();
		if (e->GetEventType() == EVENT_QUIT)
		{
			break;
		}
		std::pair<std::multimap<std::string, TASK>::iterator, std::multimap<std::string, TASK>::iterator>ret;
		m_mutex.lock();
		ret = m_task_map->equal_range(e->GetEventType());															//获取事件类型进行对比
		m_mutex.unlock();
		for (std::multimap<std::string, TASK>::iterator it = ret.first; it != ret.second; ++it)
		{
			TASK t;
			t = it->second;																							//要运行的函数
			t(e);																									//把参数e塞进函数中
		}
	}
}

void EventEngine::Timer()
{
	static int num=0;
	while (m_active)
	{
		 std::thread::id this_id = std::this_thread::get_id();
		std::cout << "Timer: id = " << this_id  <<"-------------->num:"<<num++<< std::endl;

		std::shared_ptr<Event_Timer>e = std::make_shared<Event_Timer>();
		Put(e);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
