/*
 * ctamanager.hpp
 *
 *  Created on: 2020年2月23日
 *      Author: qc
 */

#ifndef CTAMANAGER_HPP_
#define CTAMANAGER_HPP_


#include "eventengine.hpp"
#include "gatewaymanager.hpp"
//#include "riskmanager.hpp"
#include "ctaapibase.hpp"
//#include "StrategyTemplate.h"
//#include "portfolio.hpp"
#include "dateutil.hpp"

#include<fstream>
/*******************************************/
class PositionBuffer//用来缓存持仓
{
public:
	std::string symbol;//合约

	//多头
	double longposition = 0;
	double longtodayposition = 0;
	double longydposition = 0;
	//空头
	double shortposition = 0;
	double shorttodayposition = 0;
	double shortydposition = 0;
};

class CTAmanager :public CTAAPIBase
{
public:
	//CTA管理器
//	CTAmanager(Gatewaymanager *gatewaymanager, EventEngine *eventengine, riskmanager *riskmanager);
	CTAmanager(Gatewaymanager *gatewaymanager, EventEngine *eventengine);
	~CTAmanager();

	//策略所需函数
//	std::vector<std::string> sendOrder(std::string symbol, std::string orderType, double price, double volume, StrategyTemplate* Strategy);
//	void cancelOrder(std::string orderID, std::string gatewayname);
	void writeCtaLog(std::string msg, std::string gatewayname);
	void PutEvent(std::shared_ptr<Event>e);
//	std::vector<TickData> loadTick(std::string tickDbName, std::string symbol, int days);
//	std::vector<BarData> loadBar(std::string BarDbName, std::string symbol, int days);


//	//主程序调用所需函数
//	void loadStrategy();																//加载策略
//	void initStrategy(std::string name);												//初始化
//	void startStrategy(std::string name);												//开始策略
//	void stopStrategy(std::string name);												//停止策略
//	void initallStrategy();															//初始化所有策略
//	void startallStrategy();															//启动所有策略
//	void stopallStrategy();															//停止所有策略
//	void changesupposedpos(std::string name,std::string symbol,double pos);			//通过策略面板控制持仓

	//交易留底
	void savetraderecord(std::string strategyname, std::shared_ptr<Event_Trade>etrade);
private:
	EventEngine *m_eventengine;
	Gatewaymanager *m_gatewaymanager;
//	riskmanager *m_riskmanager;

	bool is_LoadStrategy;

	//缓存  和   变量锁   多线程安全
//	//key 是OrderID  value 是策略对象 用途是为了保证这个单是这个策略发出去的  成交回报计算持仓正确加载对应的策略上，以防多个策略交易同一个合约出现BUG
//	std::map<std::string, StrategyTemplate*>m_orderStrategymap;		std::mutex m_orderStrategymtx;
	//key 合约名 value 持仓对象   用来缓存每一个合约的今仓昨仓是多少
	std::map<std::string, PositionBuffer>m_symbolpositionbuffer;	std::mutex m_symbolpositionmtx;
//	//key 合约名，value 策略指针(实例)vector  用来把不同的合约tick推送到每一个策略对象
//	std::map<std::string, std::vector<StrategyTemplate*>>m_tickstrategymap;	std::mutex m_tickstrategymtx;
//	//key 策略名, value 为策略指针    用来把界面选中的策略名 对应的的策略对象启动
//	std::map<std::string, StrategyTemplate*>m_strategymap;			std::mutex m_strategymtx;

	//处理函数
//	void procecssTickEvent(std::shared_ptr<Event>e);
//	void processOrderEvent(std::shared_ptr<Event>e);
//	void processTradeEvent(std::shared_ptr<Event>e);
//	void processPositionEvent(std::shared_ptr<Event>e);

	//注册处理函数
	void registerEvent();

	//DLL存储
//	std::map<std::string, HINSTANCE>dllmap;//存放策略dll容器

	//自动重连
//	void showLog(std::shared_ptr<Event>e);
	std::atomic_bool m_connectstatus;
	void autoConnect(std::shared_ptr<Event>e);

//	//MONGOC 线程池
//	mongoc_uri_t         *m_uri;
//
//	//portfolio
//	Portfolio *m_portfolio;
};




#endif /* CTAMANAGER_HPP_ */
