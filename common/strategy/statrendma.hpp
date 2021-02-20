#ifndef _STATREND_HPP
#define _STATREND_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>

#include "httpbase.hpp"

//#include "mysqlaveprice.hpp"
//#include "mysqlinstrument.hpp"
//#include "mysqlproduct.hpp"
//#include "mysqldepthmarket.hpp"
//#include "mysqlmaincontract.hpp"
//#include "mysqlorder.hpp"
//#include "mysqldata.hpp"
//#include "mysqlposdetail.hpp"
//#include "mysqlinvest.hpp"
//#include "mysqlutil.hpp"
#include "mysqlhold.hpp"
//#include "mysqlhisatr.hpp"
//#include "mysqlma.hpp"
//#include "mysqlema.hpp"
#include "mysqltestreport.hpp"
//
//#include "redismd.hpp"
//#include "redisorder.hpp"
//#include "redissection.hpp"
//#include "depthmarket.hpp"
//#include "dateutil.hpp"

//#include "logutil.hpp"
//#include "lock.hpp"
//#include <boost/regex.hpp>
//#include <json/json.h>
//#include "cping.hpp"


#include "strategy.hpp"


using namespace std;
using namespace log4cplus;



class StaTrendMA:public Strategy
{

public:

	StaTrendMA(const char* strategyname);

	void Open_MACrossSignal();
	void  Open_CycleNSignal();
	void Hold_MA50Reverse_SignalCheck();




private :



};

#endif /* _STATREND_HPP */
