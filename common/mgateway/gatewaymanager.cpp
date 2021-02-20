/*
 * gatewaymanager.cpp
 *
 *  Created on: 2020年2月23日
 *      Author: qc
 */

#include "gatewaymanager.hpp"



Gatewaymanager::Gatewaymanager(EventEngine *eventengine)
{
	m_eventengine = eventengine;
	m_eventengine->RegEvent(EVENT_CONTRACT, std::bind(&Gatewaymanager::getallContract, this, std::placeholders::_1));
	Init();
}

Gatewaymanager::~Gatewaymanager()
{
	exit();
}

void Gatewaymanager::Init()
{

	//初始化所有接口
	//CTP
	std::shared_ptr<CTPGateway>ctpgateway = std::make_shared<CTPGateway>(m_eventengine, "CTP");
	std::unique_lock<std::mutex>lck(gatewaymtx);
	m_gatewaymap.insert(std::pair<std::string, std::shared_ptr<GatewayBase>>("CTP", ctpgateway));
	//IB
	//std::shared_ptr<>ctpgateway = std::make_shared<CTPGateway>(m_eventengine, "IB");
}

std::shared_ptr<Event_Contract>Gatewaymanager::getContract(std::string symbol)
{
	std::unique_lock<std::mutex>lck(contractmutex);
	while (m_contractmap.find(symbol) == m_contractmap.end())
	{
		contractcv.wait(lck);
	}
	return m_contractmap[symbol];//返回合约的对象指针
}

std::shared_ptr<Event_Contract>Gatewaymanager::Find_Contract(std::string symbol)
{
	if (m_contractmap.find(symbol) != m_contractmap.end())
	{
		return m_contractmap[symbol];
	}
	else
	{
		return nullptr;
	}
}


void Gatewaymanager::getallContract(std::shared_ptr<Event>e)
{
	//把所有的合约存到这个容器中
	std::unique_lock<std::mutex>lck(contractmutex);
	std::shared_ptr<Event_Contract> contract = std::static_pointer_cast<Event_Contract>(e);
	m_contractmap.insert(std::pair<std::string, std::shared_ptr<Event_Contract>>(contract->symbol, contract));
	contractcv.notify_one();

	std::cout<<"Gatewaymanager::getallContract  -------------->"<<contract->symbol<<std::endl;
}

void Gatewaymanager::connect(std::string gatewayname)
{
	//连接接口
	std::unique_lock<std::mutex>lck(gatewaymtx);

	m_gatewaymap[gatewayname]->connect();//连接指定的接口
}

void Gatewaymanager::subscribe(SubscribeReq req, std::string gatewayname)
{
	//订阅行情
	std::unique_lock<std::mutex>lck(gatewaymtx);
//	m_gatewaymap[gatewayname]->subscribe(req);//连接指定的接口
}

std::string Gatewaymanager::sendOrder(OrderReq req, std::string gatewayname)
{
	//发单
	std::unique_lock<std::mutex>lck(gatewaymtx);
	return m_gatewaymap[gatewayname]->sendOrder(req);//调用指定接口发单
}

void Gatewaymanager::cancelOrder(CancelOrderReq req, std::string gatewayname)
{
	//撤单
	std::unique_lock<std::mutex>lck(gatewaymtx);
	m_gatewaymap[gatewayname]->cancelOrder(req);//调用指定接口撤单
}

void Gatewaymanager::qryAccont(std::string gatewayname)
{
	//查账户
	std::unique_lock<std::mutex>lck(gatewaymtx);
	m_gatewaymap[gatewayname]->qryAccount();
}

void Gatewaymanager::qryPosition(std::string gatewayname)
{
	//查仓位
	std::unique_lock<std::mutex>lck(gatewaymtx);
	m_gatewaymap[gatewayname]->qryPosition();
}
//std::shared_ptr<Event_Order>Gatewaymanager::getorder(std::string gatewayname, std::string orderID)
//{
//	std::unique_lock<std::mutex>lck(gatewaymtx);
//	if (gatewayname == "CTP")
//	{
//		std::shared_ptr<CTPGateway> ctpgateway = std::static_pointer_cast<CTPGateway>(m_gatewaymap[gatewayname]);
//		return ctpgateway->getorder(orderID);
//	}
//	else
//	{
//		return nullptr;
//	}
//}

void Gatewaymanager::exit()
{
	std::unique_lock<std::mutex>lck(gatewaymtx);
	//关闭全部接口
	if (m_gatewaymap.empty())
	{
		return;
	}
	for (std::map<std::string, std::shared_ptr<GatewayBase>>::iterator it = m_gatewaymap.begin(); it != m_gatewaymap.end(); it++)
	{
		it->second->close();
	}
	m_gatewaymap.clear();
}

void Gatewaymanager::close(std::string gatewayname)
{
	std::unique_lock<std::mutex>lck(gatewaymtx);
	m_gatewaymap[gatewayname]->close();
}


