/*
 * gatewaybase.cpp
 *
 *  Created on: 2020年2月22日
 *      Author: qc
 */

#include "gatewaybase.hpp"

GatewayBase::GatewayBase(EventEngine *eventengine, std::string gatewayname)
{
	m_eventengine = eventengine;
	m_gatewayname = gatewayname;
}
GatewayBase::~GatewayBase()
{

}

void GatewayBase::onOrder(std::shared_ptr<Event_Order>e)
{
	m_eventengine->Put(e);
}

void GatewayBase::onAccount(std::shared_ptr<Event_Account>e)
{
	m_eventengine->Put(e);
}
void GatewayBase::onError(std::shared_ptr<Event_Error>e)
{
	m_eventengine->Put(e);
}
void GatewayBase::onLog(std::shared_ptr<Event_Log>e)
{
	m_eventengine->Put(e);
}

void GatewayBase::onContract(std::shared_ptr<Event_Contract>e)
{
	m_eventengine->Put(e);
}
