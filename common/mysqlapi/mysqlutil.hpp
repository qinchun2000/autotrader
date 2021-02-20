/*
 * mongo.hpp
 *
 *  Created on: 2017-8-22
 *      Author: qinchun
 */

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <json/json.h>
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
#include <pthread.h>




#include "mysqldata.hpp"
#include "mysqltradingday.hpp"
#include "mysqldayline.hpp"
#include "mysqlhismindata.hpp"
#include "mysqlcycle.hpp"
#include "mysqlma.hpp"
#include "mysqlema.hpp"
#include "mysqlhisma.hpp"
#include "mysqlhisema.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlaveprice.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlrecord.hpp"
#include "mysqlorder.hpp"
#include "mysqldepthmarket.hpp"
#include "mysqlproduct.hpp"
#include "mysqlhisatr.hpp"
#include "mysqluser.hpp"
#include "dateutil.hpp"
#include "logutil.hpp"
#include "config.hpp"

#include "threadbase.hpp"
#ifndef _MYSQLUTIL_HPP_
#define _MYSQLUTIL_HPP_


class MysqlUtil: public ThreadBase
	{

	public :

	MysqlUtil(const char* staname);
	virtual ~MysqlUtil();
	void run();

	void SetLogPathFileName();
	std::string GetLogPathFileName();

	void CheckAveragePriceData();
	void CheckAveragePriceByInstrument(const char * pinstrumentid);

	void CheckCycleData();
	void CheckCycleDataByInstrument(const char* pinstrumentid);
	void CheckHisMAData();
	void CheckHisMADataByInstrument(const char* pinstrumentid);
	void CheckHisEMAData();
	void CheckHisEMADataByInstrument(const char* pinstrumentid);

	void CheckMADataByInstrument(const char* pinstrumentid);   //历史数据回测用
	void CheckEMAData();
	void CheckEMADataByInstrument(const char* pinstrumentid);
	void CheckHisATRData();
	void CheckHisATRDataByInstrument(const char* pinstrumentid);

	TradingDayData  GetTradingDayData(const char* userid);
	std::string GetTradingDay(const char* userid);
	std::string GetTradingDateTime(const char* userid);
	void CheckTradingDayMAData(const char* userid);
	void CheckTradingDayEMAData(const char* userid);
	void CheckTradingDayCycleData(const char* userid);
	void CheckRecordReport(const char* userid);


	void CheckAllCsvDataByInstrument(const char * pinstrumentid);

	time_t GetHisLastDate_DayLine(const char* date,int difsec);



	private :
		std::string _strategyname;
		std::string _log_pathfilename;


	};


#endif /* _MYSQLUTIL_HPP_ */
