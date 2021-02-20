#ifndef _STATEGYSPREAD_HPP
#define _STATEGYSPREAD_HPP

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
#include "mysqlhold.hpp"
#include "mysqltestreport.hpp"
#include "strategy.hpp"


using namespace std;
using namespace log4cplus;

class StrategySpread:public Strategy
{

public:
	StrategySpread(const char* strategyname);
	void Open_CalendarSpreadSignal();


private :



};

#endif /* _STATEGYSPREAD_HPP */
