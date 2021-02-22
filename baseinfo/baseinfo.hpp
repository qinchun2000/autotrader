#ifndef _BASEINFO_HPP
#define _BASEINFO_HPP
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <regex.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <float.h>


#include "tradespi.hpp"

#include "redismd.hpp"
#include "redisstatus.hpp"
#include "redissection.hpp"

#include "codeutil.hpp"
#include "dateutil.hpp"
#include "logutil.hpp"
#include "lock.hpp"
#include <boost/regex.hpp>
#include <json/json.h>
#include "trader.hpp"
#include "mysqltradingday.hpp"
#include "mysqldata.hpp"
#include "mysqlexchange.hpp"
#include "mysqlinstrument.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlproduct.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlmindata.hpp"
#include "mysqldayline.hpp"
#include "mysqlcycle.hpp"
#include "mysqlorder.hpp"
#include "mysqlhold.hpp"
#include "mysqlinvest.hpp"
#include "mysqlposdetail.hpp"
#include "mysqlasset.hpp"
#include "mysqlutil.hpp"
#include "threadtrade.hpp"

using namespace std;
using namespace log4cplus;

#endif /* _BASEINFO_HPP */
