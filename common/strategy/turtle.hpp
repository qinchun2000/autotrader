/*
 * turtle.hpp
 *
 *  Created on: 2021年2月4日
 *      Author: qc
 */

#ifndef TURTLE_HPP_
#define TURTLE_HPP_



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



#include "mysqlinstrument.hpp"
#include "mysqltradingday.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlorder.hpp"
#include "mysqldata.hpp"
#include "mysqlhisatr.hpp"
#include "mysqlsection.hpp"
#include "mysqlsecassess.hpp"
#include "mysqlglobalview.hpp"

#include "redismd.hpp"
#include "redissignal.hpp"
#include "redissection.hpp"


#include "mysqldayline.hpp"
#include "mysqlmindata.hpp"
#include "mysqlutil.hpp"
#include "mysqlhold.hpp"
#include "mysqlcommission.hpp"
#include "tech.hpp"
#include "dateutil.hpp"
#include "logutil.hpp"
#include <float.h>
#include "config.hpp"
#include "assetbase.hpp"
#include "logbase.hpp"
#include "strategybase.hpp"
#include "redismindata.hpp"


using namespace std;
using namespace log4cplus;



class Turtle:public StrategyBase
{

public:
	Turtle();
	Turtle(DataWrapper * datawrapper,const char* strategyname);
	void  Open();
	void  Close();
	void run();

private :

	RedisDepthMarket m_redisdepthmarket;
	RedisSignal m_redissignal;
	RedisSection m_redissection;
	RedisMinData  m_redismindata;

};


#endif /* TURTLE_HPP_ */
