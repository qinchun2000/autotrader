/*
 * ctamanager.cpp
 *
 *  Created on: 2020年2月23日
 *      Author: qc
 */

#include "ctamanager.hpp"
//#include "json11.h"
//#include "utils.hpp"


//typedef StrategyTemplate*(*Dllfun)(CTAmanager*);
typedef int(*Release)();

CTAmanager::CTAmanager(Gatewaymanager *gatewaymanager, EventEngine *eventengine)
{
	m_eventengine = eventengine;
	m_gatewaymanager = gatewaymanager;
//	m_riskmanager = riskmanager;
	m_connectstatus = false;
//	//初始化MONGODB
//	mongoc_init();													//1
//
//	m_uri = mongoc_uri_new("mongodb://localhost:27017/");			//2
//	// 创建客户端池
//	m_pool = mongoc_client_pool_new(m_uri);							//3

	registerEvent();

//	m_portfolio = new Portfolio(eventengine, gatewaymanager);

	is_LoadStrategy = false;
}
CTAmanager::~CTAmanager()
{
//	mongoc_client_pool_destroy(m_pool);
//	mongoc_uri_destroy(m_uri);
//	mongoc_cleanup();
//
//	for (std::map<std::string, HINSTANCE>::iterator it = dllmap.begin(); it != dllmap.end(); it++)
//	{
//		Release result = (Release)GetProcAddress(it->second, "ReleaseStrategy");//析构策略
//		result();
//	}
//	delete m_portfolio;
}
/******************外部调用***************************/
//
////加载策略
//void CTAmanager::loadStrategy()
//{
//	if (is_LoadStrategy == false)
//	{
//		is_LoadStrategy = true;
//		//从文件夹底下获取所有策略文件 用windowsAPI加载DLL
//		if (_access("./Strategy", 0) != -1)
//		{
//			std::filebuf in;
//			if (!in.open("./Strategy/strategysetting.json", std::ios::in))
//			{
//				std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
//				e->msg = "无法读取本地配置文件";
//				m_eventengine->Put(e);
//				return;
//			}
//			std::istream iss(&in);
//			std::istreambuf_iterator<char> eos;
//			std::string s(std::istreambuf_iterator<char>(iss), eos);
//			std::string err;
//
//			auto json = json11::Json::parse(s, err);
//			if (!err.empty()) {
//				in.close();
//				return;
//			}
//
//			json11::Json::array jsonarray = json.array_items();
//			for (int i = 0; i < jsonarray.size(); i++)
//			{
//				std::string strategy = "./Strategy/" + jsonarray[i]["strategy"].string_value();
//				std::vector<std::string>symbol_v;//临时存储配置文件交易的合约
//				json11::Json::array array_symbol = jsonarray[i]["symbol"].array_items();
//				for (int j = 0; j < array_symbol.size(); j++)
//				{
//					//将合约列表插入symbol_v中
//					symbol_v.push_back(array_symbol[j].string_value());
//				}
//				json11::Json::array array_ = jsonarray[i]["param"].array_items();
//				std::map<std::string, std::string>tmpparammap;
//				for (int j = 0; j < array_.size(); j++)
//				{
//					//遍历参数
//					json11::Json::object obj = array_[j].object_items();
//					for (json11::Json::object::iterator it = obj.begin(); it != obj.end(); it++)
//					{
//						auto t = it->first;//参数
//						auto tt = it->second;//值
//						tmpparammap.insert(std::pair<std::string, std::string>(t, tt.string_value()));
//					}
//					std::string symbol = "";//临时的symbol
//					for (std::vector<std::string>::iterator iter = symbol_v.begin(); iter != symbol_v.end(); iter++)
//					{
//						symbol += (*iter + ",");
//					}
//					tmpparammap.insert(std::pair<std::string, std::string>("symbol", symbol));
//				}
//
//				//插入策略中
//				HINSTANCE his = LoadLibraryA(strategy.c_str());//加载一个策略
//				if (his == NULL)
//				{
//					//没有加载进来DLL
//					std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
//					e->msg = "无法读取策略" + strategy;
//					m_eventengine->Put(e);
//					break;
//				}
//				Dllfun dll = (Dllfun)GetProcAddress(his, "CreateStrategy");//创建策略
//				if (dll == NULL)
//				{
//					//没有加载进来DLL
//					std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
//					e->msg = "无法创建策略函数" + strategy;
//					m_eventengine->Put(e);
//					FreeLibrary(his);
//					break;
//				}
//				StrategyTemplate* strategy_ptr = dll(this);
//				m_tickstrategymtx.lock();
//				for (std::vector<std::string>::iterator iter = symbol_v.begin(); iter != symbol_v.end(); iter++)
//				{
//					if (m_tickstrategymap.find(*iter) == m_tickstrategymap.end())
//					{
//						//没有
//						std::vector<StrategyTemplate*>strategy_v;
//						strategy_v.push_back(strategy_ptr);
//						m_tickstrategymap.insert(std::pair<std::string, std::vector<StrategyTemplate*>>(*iter, strategy_v));
//					}
//					else
//					{
//						m_tickstrategymap[*iter].push_back(strategy_ptr);
//					}
//				}
//				m_tickstrategymtx.unlock();
//				//赋值参数
//				for (std::map<std::string, std::string>::iterator it = tmpparammap.begin(); it != tmpparammap.end(); it++)
//				{
//					//遍历parameter
//					strategy_ptr->checkparam(it->first.c_str(), it->second.c_str());
//				}
//				//插入pos_map
//
//				for (std::vector<std::string>::iterator iter = symbol_v.begin(); iter != symbol_v.end(); iter++)
//				{
//					strategy_ptr->checkSymbol((*iter).c_str());
//				}
//				if (strategy_ptr->getparam("name") == "Null")
//				{
//					this->writeCtaLog("策略中有一个没有给策略起名！", strategy_ptr->gatewayname);
//				}
//				else
//				{
//					strategy_ptr->putEvent();
//					strategy_ptr->putGUI();
//				}
//				m_strategymap.insert(std::pair<std::string, StrategyTemplate*>(strategy_ptr->getparam("name"), strategy_ptr));//策略名和策略
//				dllmap.insert(std::pair<std::string, HINSTANCE>(strategy_ptr->getparam("name"), his));//策略名
//				//订阅合约
//
//				for (std::vector<std::string>::iterator iter = symbol_v.begin(); iter != symbol_v.end(); iter++)
//				{
//					std::shared_ptr<Event_Contract>contract = m_gatewaymanager->getContract(*iter);
//					SubscribeReq req;
//					req.symbol = contract->symbol;
//					req.exchange = contract->exchange;
//					if (strategy_ptr->getparam("currency") != "Null" && strategy_ptr->getparam("productClass") != "Null")
//					{
//						req.currency = strategy_ptr->getparam("currency");
//						req.productClass = strategy_ptr->getparam("productClass");
//					}
//					m_gatewaymanager->subscribe(req, strategy_ptr->getparam("gatewayname"));
//				}
//			}
//		}
//	}
//	else
//	{
//		writeCtaLog("已经加载过策略，请勿重复加载！", "jstradergui");
//	}
//}
////初始化
//void CTAmanager::initStrategy(std::string name)
//{
//	m_portfolio->calculate();
//	m_strategymtx.lock();
//	if (m_strategymap.find(name) != m_strategymap.end())
//	{
//		//判断策略加载进来了
//		StrategyTemplate *temp = m_strategymap[name];
//		if (temp->inited == false)
//		{
//			temp->onInit();
//		}
//		else
//		{
//			this->writeCtaLog("请勿重复初始化", temp->gatewayname);
//		}
//	}
//	else
//	{
//		this->writeCtaLog("策略实例不存在", "");
//	}
//	m_strategymtx.unlock();
//}
////开始策略
//void CTAmanager::startStrategy(std::string name)
//{
//	m_strategymtx.lock();
//	if (m_strategymap.find(name) != m_strategymap.end())
//	{
//		StrategyTemplate *temp = m_strategymap[name];
//
//		if (temp->trading == false && temp->inited == true)
//		{
//			temp->onStart();
//		}
//		else if (temp->inited == false)
//		{
//			this->writeCtaLog("策略还没初始化你就想启动?", temp->gatewayname);
//		}
//	}
//	else
//	{
//		this->writeCtaLog("策略实例不存在", "");
//	}
//	m_strategymtx.unlock();
//}
////停止策略
//void CTAmanager::stopStrategy(std::string name)
//{
//	m_strategymtx.lock();
//	if (m_strategymap.find(name) != m_strategymap.end())
//	{
//		StrategyTemplate *temp = m_strategymap[name];
//		if (temp->trading == true)
//		{
//			temp->onStop();
//			//撤单
//			temp->cancelallorder();
//		}
//	}
//	else
//	{
//		this->writeCtaLog("策略实例不存在", "");
//	}
//	m_strategymtx.unlock();
//}
//
//void CTAmanager::changesupposedpos(std::string strategyname, std::string symbol, double pos)
//{
//	std::unique_lock<std::mutex>lck(m_strategymtx);
//	m_strategymap[strategyname]->m_algorithm->set_supposedPos(symbol, pos);
//}
//
//void CTAmanager::initallStrategy()
//{
//	m_strategymtx.lock();
//	for (std::map<std::string, StrategyTemplate*>::iterator it = m_strategymap.begin(); it != m_strategymap.end(); it++)
//	{
//		StrategyTemplate *temp = it->second;
//		if (temp->inited == false)
//		{
//			temp->onInit();
//		}
//		else
//		{
//			this->writeCtaLog("请勿重复初始化", temp->gatewayname);
//		}
//	}
//	m_strategymtx.unlock();
//}
////启动所有策略
//void CTAmanager::startallStrategy()
//{
//	m_strategymtx.lock();
//	for (std::map<std::string, StrategyTemplate*>::iterator it = m_strategymap.begin(); it != m_strategymap.end(); it++)
//	{
//		StrategyTemplate *temp = it->second;
//		if (temp->trading == false && temp->inited == true)
//		{
//			temp->onStart();
//		}
//		else
//		{
//			this->writeCtaLog("策略还没初始化你就想启动?", temp->gatewayname);
//		}
//	}
//	m_strategymtx.unlock();
//}
////停止所有策略
//void CTAmanager::stopallStrategy()
//{
//	m_strategymtx.lock();
//	for (std::map<std::string, StrategyTemplate*>::iterator it = m_strategymap.begin(); it != m_strategymap.end(); it++)
//	{
//		StrategyTemplate *temp = it->second;
//		if (temp->trading == true)
//		{
//			temp->onStop();
//			//撤单
//			m_orderStrategymtx.lock();
//			for (std::map<std::string, StrategyTemplate*>::iterator it = m_orderStrategymap.begin(); it != m_orderStrategymap.end(); it++)
//			{
//				cancelOrder(it->first, it->second->gatewayname);
//			}
//			m_orderStrategymtx.unlock();
//		}
//	}
//	m_strategymtx.unlock();
//}
//
///******************处理函数***************************/
//void CTAmanager::procecssTickEvent(std::shared_ptr<Event>e)
//{
//	std::shared_ptr<Event_Tick> etick = std::static_pointer_cast<Event_Tick>(e);
//	TickData tick;
//	tick.askprice1 = etick->askprice1;
//	tick.askprice2 = etick->askprice2;
//	tick.askprice3 = etick->askprice3;
//	tick.askprice4 = etick->askprice4;
//	tick.askprice5 = etick->askprice5;
//	tick.askvolume1 = etick->askvolume1;
//	tick.askvolume2 = etick->askvolume2;
//	tick.askvolume3 = etick->askvolume3;
//	tick.askvolume4 = etick->askvolume4;
//	tick.askvolume5 = etick->askvolume5;
//	tick.bidprice1 = etick->bidprice1;
//	tick.bidprice2 = etick->bidprice2;
//	tick.bidprice3 = etick->bidprice3;
//	tick.bidprice4 = etick->bidprice4;
//	tick.bidprice5 = etick->bidprice5;
//	tick.bidvolume1 = etick->bidvolume1;
//	tick.bidvolume2 = etick->bidvolume2;
//	tick.bidvolume3 = etick->bidvolume3;
//	tick.bidvolume4 = etick->bidvolume4;
//	tick.bidvolume5 = etick->bidvolume5;
//	tick.date = etick->date;
//	tick.exchange = etick->exchange;
//	tick.gatewayname = etick->gatewayname;
//	tick.highPrice = etick->highPrice;
//	tick.lastprice = etick->lastprice;
//	tick.lowerLimit = etick->lowerLimit;
//	tick.lowPrice = etick->lowPrice;
//	tick.openInterest = etick->openInterest;
//	tick.openPrice = etick->openPrice;
//	tick.preClosePrice = etick->preClosePrice;
//	tick.symbol = etick->symbol;
//	tick.time = etick->time;
//	tick.unixdatetime = Utils::getsystemunixdatetime(tick.time, "ms");
//	tick.upperLimit = etick->upperLimit;
//	tick.volume = etick->volume;
//
//	m_tickstrategymtx.lock();
//	if (m_tickstrategymap.find(etick->symbol) != m_tickstrategymap.end())
//	{
//		//有的
//		for (std::vector<StrategyTemplate*>::iterator it = m_tickstrategymap[etick->symbol].begin(); it != m_tickstrategymap[etick->symbol].end(); it++)
//		{
//			//循环所有策略实例指针
//			if ((*it)->inited == true)
//			{
//				(*it)->onTick(tick);
//			}
//		}
//	}
//	m_tickstrategymtx.unlock();
//}
//
//void CTAmanager::processOrderEvent(std::shared_ptr<Event>e)
//{
//	std::shared_ptr<Event_Order> eOrder = std::static_pointer_cast<Event_Order>(e);
//	m_orderStrategymtx.lock();
//	if (m_orderStrategymap.find(eOrder->orderID) != m_orderStrategymap.end())
//	{
//		m_orderStrategymap[eOrder->orderID]->onOrder(eOrder);
//	}
//	m_orderStrategymtx.unlock();
//}
//
//void CTAmanager::processTradeEvent(std::shared_ptr<Event>e)
//{
//	std::shared_ptr<Event_Trade> eTrade = std::static_pointer_cast<Event_Trade>(e);
//
//	m_portfolio->calculate_memory(eTrade, m_orderStrategymap);//计算portfolio
//
//	m_orderStrategymtx.lock();
//	if (m_orderStrategymap.find(eTrade->orderID) != m_orderStrategymap.end())
//	{
//		if (eTrade->direction == DIRECTION_LONG)
//		{
//			m_orderStrategymap[eTrade->orderID]->changeposmap(eTrade->symbol, m_orderStrategymap[eTrade->orderID]->getpos(eTrade->symbol) + eTrade->volume);
//		}
//		else
//		{
//			m_orderStrategymap[eTrade->orderID]->changeposmap(eTrade->symbol, m_orderStrategymap[eTrade->orderID]->getpos(eTrade->symbol) - eTrade->volume);
//		}
//		m_orderStrategymap[eTrade->orderID]->onTrade(eTrade);
//
//		savetraderecord(m_orderStrategymap[eTrade->orderID]->getparam("name"), eTrade);
//	}
//	m_orderStrategymtx.unlock();
//
//	//更新持仓
//	m_tickstrategymtx.lock();
//	if (m_tickstrategymap.find(eTrade->symbol) != m_tickstrategymap.end())
//	{
//		m_symbolpositionmtx.lock();
//		PositionBuffer *bufferPtr;
//		if (m_symbolpositionbuffer.find(eTrade->symbol) != m_symbolpositionbuffer.end())
//		{
//			bufferPtr = &m_symbolpositionbuffer[eTrade->symbol];
//		}
//		else
//		{
//			PositionBuffer positionbuffer;
//			bufferPtr = &positionbuffer;
//		}
//		if (eTrade->direction == DIRECTION_LONG)//多
//		{
//			if (eTrade->offset == OFFSET_OPEN)
//			{
//				bufferPtr->longposition += eTrade->volume;
//				bufferPtr->longtodayposition += eTrade->volume;
//			}
//			else if (eTrade->offset == OFFSET_CLOSETODAY)
//			{
//				bufferPtr->shortposition -= eTrade->volume;
//				bufferPtr->shorttodayposition -= eTrade->volume;
//			}
//			else
//			{
//				bufferPtr->shortposition -= eTrade->volume;
//				bufferPtr->shortydposition -= eTrade->volume;
//
//			}
//		}
//		else if (eTrade->direction == DIRECTION_SHORT)//空
//		{
//			if (eTrade->offset == OFFSET_OPEN)
//			{
//				bufferPtr->shortposition += eTrade->volume;
//				bufferPtr->shorttodayposition += eTrade->volume;
//			}
//			else if (eTrade->offset == OFFSET_CLOSETODAY)
//			{
//				bufferPtr->longposition -= eTrade->volume;
//				bufferPtr->longtodayposition -= eTrade->volume;
//			}
//			else
//			{
//				bufferPtr->longposition -= eTrade->volume;
//				bufferPtr->longydposition -= eTrade->volume;
//			}
//		}
//		if (m_symbolpositionbuffer.find(eTrade->symbol) != m_symbolpositionbuffer.end())
//		{
//			//有的话直接更新
//			m_symbolpositionbuffer[eTrade->symbol] = *bufferPtr;
//		}
//		else
//		{
//			//没有的话插入一个
//			m_symbolpositionbuffer.insert(std::pair<std::string, PositionBuffer>(eTrade->symbol, *bufferPtr));
//		}
//		m_symbolpositionmtx.unlock();
//	}
//	m_tickstrategymtx.unlock();
//}

//void CTAmanager::processPositionEvent(std::shared_ptr<Event>e)
//{
//	std::shared_ptr<Event_Position> ePosition = std::static_pointer_cast<Event_Position>(e);
//	m_tickstrategymtx.lock();
//	if (m_tickstrategymap.find(ePosition->symbol) != m_tickstrategymap.end())
//	{
//		m_symbolpositionmtx.lock();
//		if (m_symbolpositionbuffer.find(ePosition->symbol) != m_symbolpositionbuffer.end())
//		{	//有就直接更新
//			PositionBuffer positionbuffer;
//			if (ePosition->direction == DIRECTION_LONG)//多
//			{
//				positionbuffer.longposition = ePosition->position;
//				positionbuffer.longydposition = ePosition->ydPosition;
//				positionbuffer.longtodayposition = ePosition->todayPosition;
//				positionbuffer.shortposition = m_symbolpositionbuffer[ePosition->symbol].shortposition;
//				positionbuffer.shortydposition = m_symbolpositionbuffer[ePosition->symbol].shortydposition;
//				positionbuffer.shorttodayposition = m_symbolpositionbuffer[ePosition->symbol].shorttodayposition;
//			}
//			else if (ePosition->direction == DIRECTION_SHORT)//空
//			{
//				positionbuffer.longposition = m_symbolpositionbuffer[ePosition->symbol].longposition;
//				positionbuffer.longydposition = m_symbolpositionbuffer[ePosition->symbol].longydposition;
//				positionbuffer.longtodayposition = m_symbolpositionbuffer[ePosition->symbol].longtodayposition;
//				positionbuffer.shortposition = ePosition->position;
//				positionbuffer.shortydposition = ePosition->ydPosition;
//				positionbuffer.shorttodayposition = ePosition->todayPosition;
//			}
//			m_symbolpositionbuffer[ePosition->symbol] = positionbuffer;
//		}
//		else
//		{	//没有就创建一个再更新进去
//			PositionBuffer positionbuffer;
//			if (ePosition->direction == DIRECTION_LONG)//多
//			{
//				positionbuffer.longposition = ePosition->position;
//				positionbuffer.longydposition = ePosition->ydPosition;
//				positionbuffer.longtodayposition = ePosition->todayPosition;
//
//			}
//			else if (ePosition->direction == DIRECTION_SHORT)//空
//			{
//				positionbuffer.shortposition = ePosition->position;
//				positionbuffer.shortydposition = ePosition->ydPosition;
//				positionbuffer.shorttodayposition = ePosition->todayPosition;
//			}
//			m_symbolpositionbuffer.insert(std::pair<std::string, PositionBuffer>(ePosition->symbol, positionbuffer));
//		}
//		m_symbolpositionmtx.unlock();
//	}
//	m_tickstrategymtx.unlock();
//}

//void CTAmanager::showLog(std::shared_ptr<Event>e)
//{
//	std::shared_ptr<Event_Log> elog = std::static_pointer_cast<Event_Log>(e);
//	if (elog->gatewayname == "CTP" && elog->msg == "行情服务器登录完成")
//	{
//		m_connectstatus = true;
//		m_tickstrategymtx.lock();
//		for (std::map<std::string, std::vector<StrategyTemplate*>>::iterator it = m_tickstrategymap.begin(); it != m_tickstrategymap.end(); it++)
//		{
//			std::shared_ptr<Event_Contract>contract = m_gatewaymanager->getContract(it->first);
//			SubscribeReq req;
//			req.symbol = contract->symbol;
//			req.exchange = contract->exchange;
//			m_gatewaymanager->subscribe(req, "CTP");
//		}
//		m_tickstrategymtx.unlock();
//	}
//}

void CTAmanager::autoConnect(std::shared_ptr<Event>e)
{
	DateUtil dateutil;
	double Q_BarTime_2 =  dateutil.GetCurrentBarTime();

	bool morningbefore = Q_BarTime_2 > 0.023000 && Q_BarTime_2 < 0.085000 ;
	bool dayend = Q_BarTime_2 > 0.203500 && Q_BarTime_2 < 0.205000 ;

//	auto nowtime2 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//	if (((nowtime2 > Utils::timetounixtimestamp(2, 31, 0)) && (nowtime2 < Utils::timetounixtimestamp(8, 50, 0)))
//		|| ((nowtime2 > Utils::timetounixtimestamp(15, 01, 0)) && (nowtime2 < Utils::timetounixtimestamp(20, 50, 0))))

	if(morningbefore ||dayend)
	{
		if (m_connectstatus == true)
		{
			m_connectstatus = false;

			writeCtaLog("CTP接口主动断开连接！", "CTP");
			m_gatewaymanager->close("CTP");
//			m_riskmanager->clearTradeCount();
		}
		else{
			std::cout<<"autoconnect  ---------runing"<<endl;
		}
	}
	else
	{
		//连接
		if (m_connectstatus == false)
		{
			m_connectstatus = true;
			writeCtaLog("CTP接口主动连接！", "CTP");

			m_gatewaymanager->connect("CTP");
		}
	}
}
/****************** 注册***************************/

void CTAmanager::registerEvent()
{
//	m_eventengine->RegEvent(EVENT_TICK, std::bind(&CTAmanager::procecssTickEvent, this, std::placeholders::_1));
//	m_eventengine->RegEvent(EVENT_ORDER, std::bind(&CTAmanager::processOrderEvent, this, std::placeholders::_1));
//	m_eventengine->RegEvent(EVENT_TRADE, std::bind(&CTAmanager::processTradeEvent, this, std::placeholders::_1));
//	m_eventengine->RegEvent(EVENT_POSITION, std::bind(&CTAmanager::processPositionEvent, this, std::placeholders::_1));
	m_eventengine->RegEvent(EVENT_TIMER, std::bind(&CTAmanager::autoConnect, this, std::placeholders::_1));
//	m_eventengine->RegEvent(EVENT_LOG, std::bind(&CTAmanager::showLog, this, std::placeholders::_1));
}
/******************策略调用***************************/

//std::vector<std::string>CTAmanager::sendOrder(std::string symbol, std::string orderType, double price, double volume, StrategyTemplate* Strategy)
//{
//	if (Strategy->trading == true)
//	{
//		OrderReq req;
//		req.symbol = symbol;
//		req.exchange = m_gatewaymanager->getContract(symbol)->exchange;
//		req.price = price;
//		req.volume = volume;
//
//		if (m_riskmanager->checkRisk(req) == false)
//		{
//			std::vector<std::string>result;
//			return result;
//		}
//
//		if (Strategy->getparam("currency") != "Null"&& Strategy->getparam("productClass") != "Null")
//		{
//			req.currency = Strategy->getparam("currency");
//			req.productClass = Strategy->getparam("productClass");
//		}
//		req.priceType = PRICETYPE_LIMITPRICE;//限价单
//		if (orderType == CTAORDER_BUY)
//		{
//			req.direction = DIRECTION_LONG;//做多
//			req.offset = OFFSET_OPEN;//开仓
//			//
//			//发单
//			std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//			m_orderStrategymtx.lock();
//			m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//			m_orderStrategymtx.unlock();
//			writeCtaLog("策略" + Strategy->getparam("name") + "发出委托" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//			std::vector<std::string>result;
//			result.push_back(orderID);
//			return result;
//		}
//		else if (orderType == CTAORDER_SELL)
//		{
//			req.direction = DIRECTION_SHORT;//平多
//			if (m_gatewaymanager->getContract(symbol)->exchange != EXCHANGE_SHFE)
//			{
//				req.offset = OFFSET_CLOSE;
//				std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//				m_orderStrategymtx.lock();
//				m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//				m_orderStrategymtx.unlock();
//				writeCtaLog("策略" + Strategy->getparam("name") + "发出委托" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//				std::vector<std::string>result;
//				result.push_back(orderID);
//				return result;
//			}
//			else
//			{
//				//区分平今平昨
//				//先判断昨仓够不够用
//				if ((m_symbolpositionbuffer[symbol].longydposition - volume) >= 0)
//				{
//					//昨仓够用直接平昨
//					req.offset = OFFSET_CLOSEYESTERDAY;
//					std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					writeCtaLog("策略" + Strategy->getparam("name") + "发出委托(只平昨)" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//					std::vector<std::string>result;
//					result.push_back(orderID);
//					return result;
//				}
//				else if (m_symbolpositionbuffer[symbol].longydposition == 0)
//				{
//					req.offset = OFFSET_CLOSETODAY;
//					std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					writeCtaLog("策略" + Strategy->getparam("name") + "发出委托(只平今)" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//					std::vector<std::string>result;
//					result.push_back(orderID);
//					return result;
//				}
//				else
//				{
//					//昨仓不够用，分开平，先平昨再平今
//					req.offset = OFFSET_CLOSEYESTERDAY;
//					req.volume = m_symbolpositionbuffer[symbol].longydposition;
//					std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					std::vector<std::string>result;
//					result.push_back(orderID);
//					//再平今仓
//					req.offset = OFFSET_CLOSETODAY;
//					req.volume = volume - req.volume;
//					orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					writeCtaLog("策略" + Strategy->getparam("name") + "发出委托(平今和昨)" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//					result.push_back(orderID);
//					return result;
//				}
//			}
//		}
//		else if (orderType == CTAORDER_SHORT)
//		{
//			//做空
//			req.direction = DIRECTION_SHORT;
//			req.offset = OFFSET_OPEN;
//			//发单
//			std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//			m_orderStrategymtx.lock();
//			m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//			m_orderStrategymtx.unlock();
//			writeCtaLog("策略" + Strategy->getparam("name") + "发出委托" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//			std::vector<std::string>result;
//			result.push_back(orderID);
//			return result;
//		}
//		else if (orderType == CTAORDER_COVER)
//		{
//			//平空
//			req.direction = DIRECTION_LONG;
//			if (m_gatewaymanager->getContract(symbol)->exchange != EXCHANGE_SHFE)
//			{
//				req.offset = OFFSET_CLOSE;
//				std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//				m_orderStrategymtx.lock();
//				m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//				m_orderStrategymtx.unlock();
//				writeCtaLog("策略" + Strategy->getparam("name") + "发出委托" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//				std::vector<std::string>result;
//				result.push_back(orderID);
//				return result;
//			}
//			else
//			{
//				//区分平今平昨
//				//先判断昨仓够不够用
//				if ((m_symbolpositionbuffer[symbol].shortydposition - volume) >= 0)
//				{
//					//只平昨
//					req.offset = OFFSET_CLOSEYESTERDAY;
//					std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					writeCtaLog("策略" + Strategy->getparam("name") + "发出委托(只平昨)" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//					std::vector<std::string>result;
//					result.push_back(orderID);
//					return result;
//				}
//				else if (m_symbolpositionbuffer[symbol].shortydposition == 0)
//				{
//					//只平今
//					req.offset = OFFSET_CLOSETODAY;
//					std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					writeCtaLog("策略" + Strategy->getparam("name") + "发出委托(只平今)" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//					std::vector<std::string>result;
//					result.push_back(orderID);
//					return result;
//				}
//				else
//				{
//					//平今也平昨
//					//昨仓不够用，分开平，先平昨再平今
//					req.offset = OFFSET_CLOSEYESTERDAY;
//					req.volume = m_symbolpositionbuffer[symbol].shortydposition;
//					std::string orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					std::vector<std::string>result;
//					result.push_back(orderID);
//					//再平今仓
//					req.offset = OFFSET_CLOSETODAY;
//					req.volume = volume - req.volume;
//					orderID = m_gatewaymanager->sendOrder(req, m_gatewaymanager->getContract(symbol)->gatewayname);
//					m_orderStrategymtx.lock();
//					m_orderStrategymap.insert(std::pair<std::string, StrategyTemplate*>(orderID, Strategy));
//					m_orderStrategymtx.unlock();
//					writeCtaLog("策略" + Strategy->getparam("name") + "发出委托(平今和昨)" + symbol + req.direction + Utils::doubletostring(volume) + " @ " + Utils::doubletostring(price), Strategy->gatewayname);
//					result.push_back(orderID);
//					return result;
//				}
//			}
//		}
//	}
//	std::vector<std::string>v;
//	return v;
//}

//void CTAmanager::cancelOrder(std::string orderID, std::string gatewayname)
//{
//	std::shared_ptr<Event_Order>order = m_gatewaymanager->getorder(gatewayname, orderID);
//	if (order != nullptr)
//	{
//		//报单有效
//		if (!(order->status == STATUS_ALLTRADED || order->status == STATUS_CANCELLED))
//		{
//			//可撤单状态
//			CancelOrderReq req;
//			req.symbol = order->symbol;
//			req.exchange = order->exchange;
//			req.frontID = order->frontID;
//			req.sessionID = order->sessionID;
//			req.orderID = order->orderID;
//			m_gatewaymanager->cancelOrder(req, order->gatewayname);
//		}
//	}
//}

void CTAmanager::writeCtaLog(std::string msg, std::string gatewayname)
{
	std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
	e->msg = msg;
	e->gatewayname = gatewayname;
	m_eventengine->Put(e);
}

void CTAmanager::PutEvent(std::shared_ptr<Event>e)
{
	m_eventengine->Put(e);
}

//void CTAmanager::savetraderecord(std::string strategyname, std::shared_ptr<Event_Trade>etrade)
//{
//	//交易记录
//	if (_access("./traderecord", 0) != -1)
//	{
//		std::fstream f;
//		f.open("./traderecord/" + strategyname + ".csv", std::ios::app | std::ios::out);
//		if (!f.is_open())
//		{
//			//如果打不开文件
//			std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
//			e->msg = "无法保存交易记录";
//			e->gatewayname = "CTP";
//			m_eventengine->Put(e);
//			return;
//		}
//		std::string symbol = etrade->symbol;
//		std::string direction = etrade->direction;
//		std::string offset = etrade->offset;
//		std::string tradetime = etrade->tradeTime;
//		std::string volume = Utils::doubletostring(etrade->volume);
//		std::string price = Utils::doubletostring(etrade->price);
//
//		f << strategyname << "," << tradetime << "," << symbol << "," << direction << "," << offset << "," << price << "," << volume << "\n";
//
//		f.close();
//	}
//}
//
//std::vector<TickData> CTAmanager::loadTick(std::string tickDbName, std::string symbol, int days)
//{
//	std::vector<TickData>datavector;
//	if (symbol == " " || symbol == "")
//	{
//		return datavector;
//	}
//	const char* databasename = tickDbName.c_str();
//	const char* collectionsname = symbol.c_str();
//	auto targetday = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - (days * 24 * 3600);//获取当前的系统时间
//
//
//	mongoc_cursor_t *cursor;
//	bson_error_t error;
//	const bson_t *doc;
//
//
//	// 从客户端池中获取一个客户端
//	mongoc_client_t      *client = mongoc_client_pool_pop(m_pool);																//取一个mongo连接
//
//	bson_t parent;
//	bson_t child;
//	mongoc_collection_t *collection;
//	bson_init(&parent);
//	BSON_APPEND_DOCUMENT_BEGIN(&parent, "datetime", &child);
//	BSON_APPEND_TIME_T(&child, "$gt", targetday);
//	bson_append_document_end(&parent, &child);
//
//
//	char * str = bson_as_json(&parent, NULL);
//	//	printf("\n%s\n", str);
//
//	collection = mongoc_client_get_collection(client, tickDbName.c_str(), symbol.c_str());
//
//	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, &parent, NULL, NULL);
//
//	while (mongoc_cursor_next(cursor, &doc)) {
//		str = bson_as_json(doc, NULL);
//		std::string s = str;
//		std::string err;
//
//
//		auto json = json11::Json::parse(s, err);
//		if (!err.empty())
//		{
//			mongoc_cursor_destroy(cursor);
//			mongoc_client_pool_push(m_pool, client);																						//放回一个mongo连接
//			return datavector;
//		}
//		TickData tickdata;
//
//		tickdata.symbol = json["symbol"].string_value();
//		tickdata.exchange = json["exchange"].string_value();
//		tickdata.gatewayname = json["gatewayname"].string_value();
//
//		tickdata.lastprice = json["lastprice"].number_value();
//		tickdata.volume = json["volume"].number_value();
//		tickdata.openInterest = json["openInterest"].number_value();
//
//		json11::Json::object datetime = json["datetime"].object_items();
//		tickdata.unixdatetime = datetime["$date"].number_value() / 1000;
//
//		tickdata.date = json["date"].string_value();
//		tickdata.time = json["time"].string_value();
//
//		tickdata.openPrice = json["openPrice"].number_value();//今日开
//		tickdata.highPrice = json["highPrice"].number_value();//今日高
//		tickdata.lowPrice = json["lowPrice"].number_value();//今日低
//		tickdata.preClosePrice = json["preClosePrice"].number_value();//昨收
//
//		tickdata.upperLimit = json["upperLimit"].number_value();//涨停
//		tickdata.lowerLimit = json["lowerLimit"].number_value();//跌停
//
//		tickdata.bidprice1 = json["bidprice1"].number_value();
//		tickdata.bidprice2 = json["bidprice2"].number_value();
//		tickdata.bidprice3 = json["bidprice3"].number_value();
//		tickdata.bidprice4 = json["bidprice4"].number_value();
//		tickdata.bidprice5 = json["bidprice5"].number_value();
//
//		tickdata.askprice1 = json["askprice1"].number_value();
//		tickdata.askprice2 = json["askprice2"].number_value();
//		tickdata.askprice3 = json["askprice3"].number_value();
//		tickdata.askprice4 = json["askprice4"].number_value();
//		tickdata.askprice5 = json["askprice5"].number_value();
//
//		tickdata.bidvolume1 = json["bidvolume1"].number_value();
//		tickdata.bidvolume2 = json["bidvolume2"].number_value();
//		tickdata.bidvolume3 = json["bidvolume3"].number_value();
//		tickdata.bidvolume4 = json["bidvolume4"].number_value();
//		tickdata.bidvolume5 = json["bidvolume5"].number_value();
//
//		tickdata.askvolume1 = json["askvolume1"].number_value();
//		tickdata.askvolume2 = json["askvolume2"].number_value();
//		tickdata.askvolume3 = json["askvolume3"].number_value();
//		tickdata.askvolume4 = json["askvolume4"].number_value();
//		tickdata.askvolume5 = json["askvolume5"].number_value();
//
//		datavector.push_back(tickdata);
//
//		//		printf("%s\n", str);
//		bson_free(str);
//	}
//
//	if (mongoc_cursor_error(cursor, &error)) {
//		fprintf(stderr, "An error occurred: %s\n", error.message);
//	}
//	mongoc_cursor_destroy(cursor);
//	mongoc_client_pool_push(m_pool, client);																						//放回一个mongo连接
//	return datavector;
//}
//std::vector<BarData> CTAmanager::loadBar(std::string BarDbName, std::string symbol, int days)
//{
//	std::vector<BarData>datavector;
//	if (symbol == " " || symbol == "")
//	{
//		return datavector;
//	}
//	const char* databasename = BarDbName.c_str();
//	const char* collectionsname = symbol.c_str();
//	auto targetday = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - (days * 24 * 3600);//获取当前的系统时间
//
//	mongoc_cursor_t *cursor;
//	bson_error_t error;
//	const bson_t *doc;
//
//
//	bson_t parent;
//	bson_t child;
//	mongoc_collection_t *collection;
//	bson_init(&parent);
//	BSON_APPEND_DOCUMENT_BEGIN(&parent, "datetime", &child);
//	BSON_APPEND_TIME_T(&child, "$gt", targetday);
//	bson_append_document_end(&parent, &child);
//
//
//	char * str = bson_as_json(&parent, NULL);
//	//	printf("\n%s\n", str);
//
//	// 从客户端池中获取一个客户端
//	mongoc_client_t    *client = mongoc_client_pool_pop(m_pool);																				//取一个mongo连接
//
//	collection = mongoc_client_get_collection(client, BarDbName.c_str(), symbol.c_str());
//
//	cursor = mongoc_collection_find(collection, MONGOC_QUERY_NONE, 0, 0, 0, &parent, NULL, NULL);
//
//	while (mongoc_cursor_next(cursor, &doc))
//	{
//		str = bson_as_json(doc, NULL);
//		std::string s = str;
//		std::string err;
//
//
//		auto json = json11::Json::parse(s, err);
//		if (!err.empty())
//		{
//			mongoc_cursor_destroy(cursor);
//			mongoc_client_pool_push(m_pool, client);																						//放回一个mongo连接
//			return datavector;
//		}
//		BarData bardata;
//		bardata.symbol = json["symbol"].string_value();
//		bardata.exchange = json["exchange"].string_value();
//		bardata.open = json["open"].number_value();
//		bardata.high = json["high"].number_value();
//		bardata.low = json["low"].number_value();
//		bardata.close = json["close"].number_value();
//		bardata.volume = json["volume"].number_value();
//
//		json11::Json::object datetime = json["datetime"].object_items();
//		bardata.unixdatetime = datetime["$date"].number_value() / 1000;
//
//		bardata.date = json["date"].string_value();
//		bardata.time = json["time"].string_value();
//
//		bardata.openPrice = json["openPrice"].number_value();//今日开
//		bardata.highPrice = json["highPrice"].number_value();//今日高
//		bardata.lowPrice = json["lowPrice"].number_value();//今日低
//		bardata.preClosePrice = json["preClosePrice"].number_value();//昨收
//
//		bardata.upperLimit = json["upperLimit"].number_value();//涨停
//		bardata.lowerLimit = json["lowerLimit"].number_value();//跌停
//
//		bardata.openInterest = json["openInterest"].number_value();//持仓
//
//		datavector.push_back(bardata);
//
//		//		printf("%s\n", str);
//		bson_free(str);
//	}
//
//	if (mongoc_cursor_error(cursor, &error)) {
//		fprintf(stderr, "An error occurred: %s\n", error.message);
//	}
//
//	mongoc_cursor_destroy(cursor);
//	mongoc_client_pool_push(m_pool, client);																						//放回一个mongo连接
//	return datavector;
//}



