#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include "mdspi.hpp"
#include <hiredis/hiredis.h>
#include "redissection.hpp"

#include <boost/regex.hpp>
#include <json/json.h>
#include "dateutil.hpp"

#include "mysqldata.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlinvest.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqltradingday.hpp"
#include "mysqlbase.hpp"
#include "mysqlmindata.hpp"
#include "mysqlmdlogin.hpp"
#include "mysqlsubscribe.hpp"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include "logutil.hpp"
#include "config.hpp"
#include "logbase.hpp"
#include "datawrapper.hpp"

#include "marketbase.hpp"
using namespace std;


#ifndef MARKET_HPP_
#define MARKET_HPP_

class Market:public LogBase
{

public:

	Market();
	Market(const char* path);
	Market(DataWrapper * datawrapper,const char*strategyname,const char* path) ;

	void SetCThostFtdcMdApi(CThostFtdcMdApi *pmdapi);
	CThostFtdcMdApi * GetCThostFtdcMdApi();
	void SetCMdSpi(CMdSpi *pmdspi);
	CMdSpi * GetCMdSpi();

	void SetMarketBase(MarketBase *  md);
	MarketBase * GetMarketBase();

	void SetTaskFlag_UnSubscribe(bool flag);
	bool  GetTaskFlag_UnSubscribe();
	void SetTaskFlag_Subscribe(bool flag);
	bool  GetTaskFlag_Subscribe();

	void InitialInstrumentLists(vector<std::string>  &lists);
	void InitialInstrumentLists(vector<std::string>  &lists,const char * processor);
	void InitialInstrumentLists(vector<std::string>  &lists,const char * processor,const char* strategyname);

	void SetInstrumentLists(vector<std::string> * list);
	vector<std::string> * GetInstrumentLists();
	void CheckCommission();
	void CheckContract(vector<std::string>  &lists);
	void CheckContractbyDataWrapper(vector<std::string>  &lists);

	void UpdateFlag();
	void CheckFlowPath(const char *flowpath);
	bool isOpen();
	bool ConnectRestore();
	bool LoginAgin();

	void SubmarketAgain(vector<std::string>  &lists);
	void SubmarketAgain(vector<std::string>  &lists,const char* processor);

	void AutoUpdateSubmarket(vector<std::string>  &lists);
	void AutoUpdateSubmarket(vector<std::string>  &lists,const char* processor);


	void AutoConnect();
	void Run();
	void Run(const char* processor,const char*strategyname);
	void pThread_Run(const char* processor);

private:

	RedisMinFlag * m_redisminflag=nullptr;     // redis select 0
	RedisDepthMarket * m_redisdepthmarket=nullptr;   //redis select 1

	CThostFtdcMdApi *_pMDApi=nullptr;
	CMdSpi *_pMDSpi=nullptr;
	MarketBase *_pmarketdata=nullptr;
	std::string _log_pathfilename;

	std::string m_strategyname;
	std::string m_flowpath;
	DataWrapper * m_datawrapper=nullptr;

	vector<std::string> * _pinstrumentlists=nullptr;

	bool _flag_wday=false;
	bool _flag_sunday=false;
	bool _open_flag=false;

	bool _TaskFlag_UnSubscribe=false;
	bool _TaskFlag_Subscribe=false;

	double _Q_BarTime_2=0.0;

};

#endif /* MARKET_HPP_ */
