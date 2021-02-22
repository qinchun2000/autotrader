/*
 * mdzmq.hpp
 *
 *  Created on: 2020年3月30日
 *      Author: qc
 */

#ifndef MDZMQ_HPP_
#define MDZMQ_HPP_

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <zmq.hpp>

#include "threadbase.hpp"
#include "mysqlhold.hpp"
#include "datawrapper.hpp"
#include "marketbase.hpp"
#include <json/json.h>

class Mdzmq:public ThreadBase
{

private:
	std::string m_strategyname;
	DataWrapper * m_datawrapper=nullptr;
	MarketBase * m_market=nullptr;

public:
	Mdzmq(DataWrapper * datawrapper,MarketBase *marketbase,const char* staname);
	virtual ~Mdzmq();
	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();
	std::string RouterPort();
	void run();
	int GetReqCode(const char* req);
	std::string ProcessSubMarket(const char* req);
	std::string PackHoldPosition(const char* userid);
	std::string PackError(int errorcode);
};


#endif /* MDZMQ_HPP_ */
