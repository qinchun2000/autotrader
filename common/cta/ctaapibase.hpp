/*
 * ctaapi.hpp
 *
 *  Created on: 2020年2月23日
 *      Author: qc
 */

#ifndef CTAAPIBASE_HPP_
#define CTAAPIBASE_HPP_


#define CTAORDER_BUY "ctaorderbuy"
#define CTAORDER_SELL "ctaordersell"
#define CTAORDER_SHORT "ctaordershort"
#define CTAORDER_COVER "ctaordercover"
#include<memory>
#include<vector>

#include "structs.hpp"

//class StrategyTemplate;
class CTAAPIBase
{
public:
	CTAAPIBase();
	virtual ~CTAAPIBase();

//	virtual std::vector<std::string> sendOrder(std::string symbol, std::string orderType, double price, double volume, StrategyTemplate* Strategy) = 0;
//	virtual void cancelOrder(std::string orderID,std::string gatewayname) = 0;
	virtual void writeCtaLog(std::string msg, std::string gatewayname)=0;
	virtual void PutEvent(std::shared_ptr<Event>e)=0;
//	virtual std::vector<TickData> loadTick(std::string tickDbName, std::string symbol, int days)=0;
//	virtual std::vector<BarData> loadBar(std::string BarDbName, std::string symbol, int days)=0;
//	mongoc_client_pool_t *m_pool;
};

#endif /* CTAAPIBASE_HPP_ */
