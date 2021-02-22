/*
 * gatewaymanager.hpp
 *
 *  Created on: 2020年2月23日
 *      Author: qc
 */

#ifndef GATEWAYMANAGER_HPP_
#define GATEWAYMANAGER_HPP_

#include "eventengine.hpp"
#include "structs.hpp"
#include "ctpgateway.hpp"
#include "gatewaybase.hpp"

class Gatewaymanager
{
public:
	Gatewaymanager(EventEngine *eventengine);
	~Gatewaymanager();
	void Init();
	void connect(std::string gatewayname);
	void subscribe(SubscribeReq req, std::string gatewayname);
	std::string sendOrder(OrderReq req, std::string gatewayname);
	void cancelOrder(CancelOrderReq req, std::string gatewayname);
	void qryAccont(std::string gatewayname);
	void qryPosition(std::string gatewayname);
	std::shared_ptr<Event_Contract>getContract(std::string symbol);
	std::shared_ptr<Event_Contract>Find_Contract(std::string symbol);
//	std::shared_ptr<Event_Order>getorder(std::string gatewayname, std::string orderID);
	void close(std::string gatewayname);
	void exit();
private:
	void getallContract(std::shared_ptr<Event>e);
	EventEngine *m_eventengine;
	std::map<std::string, std::shared_ptr<GatewayBase>>m_gatewaymap;			std::mutex gatewaymtx;                               //保存gateway名称与gateway对象的对应关系
	std::map<std::string, std::shared_ptr<Event_Contract>>m_contractmap;    std::mutex contractmutex;  std::condition_variable contractcv;//保存symbol和合约指针
};


#endif /* GATEWAYMANAGER_HPP_ */
