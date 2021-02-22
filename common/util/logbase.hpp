#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <vector>

#include "config.hpp"
#include "logutil.hpp"

#ifndef _LOGBASE_HPP_
#define _LOGBASE_HPP_

class LogBase
{

private:
	std::string _log_pathfilename;
	std::string _logname;
	LogUtil * m_logutil=nullptr;

public:
	LogBase();
	virtual ~LogBase();

	void EmptyLogUtil();
	void NewLogUtil(std::string logname,std::string logfilepath);
	void SetLogUtil(LogUtil * logutil);
	LogUtil* GetLogUtil();
	void SetLogPathFileName(const char* path,const char *logname);
	std::string GetLogPathFileName();
	void SetLogName(const char *logname);
	std::string GetLogName();
};

#endif /* _LOGBASE_HPP_ */
