#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include "dateutil.hpp"


#ifndef _LOGUTIL_HPP_
#define _LOGUTIL_HPP_


using namespace std;
using namespace log4cplus;
class LogUtil
	{

	public :

	LogUtil();
	void SetDebug(bool flag);
	bool GetDebug();

	LogUtil(std::string logname, std::string file);
//	void Set(std::string logname,std::string file);
	void WriteLog(std::string log);
	void PrintLog(const char * log);

	private :
	std::string _filename;
	std::string _logname;

	bool _debug_flag=true;



	};



#endif /* _LOGUTIL_HPP_ */
