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
#include <json/json.h>


#ifndef _ZEROMYUTIL_HPP_
#define _ZEROMYUTIL_HPP_

class ZeroMqUtil:public ThreadBase
{

private:
	std::string m_strategyname;
	DataWrapper * m_datawrapper;

public:
	ZeroMqUtil(DataWrapper * datawrapper,const char* staname);
	virtual ~ZeroMqUtil();
	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();
	std::string RouterPort();
	void run();
	int GetReqCode(const char* req);
	std::string ProcessQueryHoldPosition(const char* req);
	std::string PackHoldPosition(const char* userid);
	std::string PackError(int errorcode);
};

#endif /* _ZEROMYUTIL_HPP_ */
