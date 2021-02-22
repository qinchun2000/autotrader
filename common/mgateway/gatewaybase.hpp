#ifndef _GATEWAYBASE_H
#define _GATEWAYBASE_H


//抽象类，给所有接口类继承
#include <string>
#include "structs.hpp"
#include "eventengine.hpp"


class GatewayBase
{
public:
	GatewayBase(EventEngine *eventengine, std::string gatewayname);
	virtual ~GatewayBase();

//	void GatewayBase::onTick(std::shared_ptr<Event_Tick>e)
//	{
//		m_eventengine->Put(e);
//	}
//	void GatewayBase::onTrade(std::shared_ptr<Event_Trade>e)
//	{
//		m_eventengine->Put(e);
//	}
	void onOrder(std::shared_ptr<Event_Order>e);

//	void GatewayBase::onPosition(std::shared_ptr<Event_Position>e)
//	{
//		m_eventengine->Put(e);
//	}
	void onAccount(std::shared_ptr<Event_Account>e);
	void onError(std::shared_ptr<Event_Error>e);
	void onLog(std::shared_ptr<Event_Log>e);
	void onContract(std::shared_ptr<Event_Contract>e);

	virtual void connect() = 0;																					//连接
	virtual void subscribe(SubscribeReq& subscribeReq) = 0;														//订阅
	virtual std::string sendOrder(OrderReq & req) = 0;															//发单
	virtual void cancelOrder(CancelOrderReq & req) = 0;															//撤单
	virtual void qryAccount() = 0;																				//查询账户资金
	virtual void qryPosition() = 0;																				//查询持仓
	virtual void close() = 0;																					//断开API
private:
	EventEngine *m_eventengine;
	std::string m_gatewayname;
};
#endif
