#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include "mdspi.hpp"
#include <hiredis/hiredis.h>
//#include "tradespi.hpp"
//#include "depthmarket.hpp"
#include "redissection.hpp"

#include <boost/regex.hpp>
#include <json/json.h>
#include "dateutil.hpp"

#include "mysqldata.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlinvest.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqltradingday.hpp"
#include "mysqlbase.hpp"
#include "mysqlmindata.hpp"
#include "mysqlmdlogin.hpp"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include "logutil.hpp"
#include "logbase.hpp"
#include "config.hpp"

using namespace std;
// mdservice.cpp :
// CThostFtdcMdSpi接口的使用。

#ifndef MARKETDATA_HPP_
#define MARKETDATA_HPP_

class MarketBase:public LogBase
{

public:

	MarketBase(const char* strategyname);
	virtual ~MarketBase();

	void SetCThostFtdcMdApi(CThostFtdcMdApi *pmdapi);
	CThostFtdcMdApi * GetCThostFtdcMdApi();
	void SetCMdSpi(CMdSpi *pmdspi);
	CMdSpi * GetCMdSpi();
	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();
	void SetPorcessor(const char* processor);
	std::string GetPorcessor();

	void Initial();
	void InitialbyStrategy(const char* strategy);
	void FrontConnect();
	void UserLogin();
	void UserLoginbyStrategy(const char* strategy);
	void Login();
	void LoginbyStrategy(const char* strategy);

	void SubMarket(vector<std::string> &instrumentlists);
	void UnSubMarket(vector<std::string> &instrumentlists);
	void SendSelfCheckMarket(vector<std::string> &instrumentlists);

private:
	CThostFtdcMdApi *_pMDApi=NULL;
	CMdSpi *_pMDSpi=NULL;
	std::string _strategyName;
	std::string _processer;
};

#endif /* MARKETDATA_HPP_ */
