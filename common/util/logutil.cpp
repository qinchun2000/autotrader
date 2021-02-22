#include "logutil.hpp"

//Logger LogUtil::_tradelogger = log4cplus::Logger::getInstance(_logname);

LogUtil::LogUtil()
{


}
void LogUtil::SetDebug(bool flag)
{
	this->_debug_flag=flag;

}
bool LogUtil::GetDebug()
{
	return this->_debug_flag;
}
LogUtil::LogUtil(std::string logname, std::string file)
{

	_logname = logname;

	_filename = file;
//	_pFileAppender = new RollingFileAppender ((_filename));
//	SharedAppenderPtr _pFileAppender(new RollingFileAppender ((_filename)));
//	this->_tradelogger = Logger::getInstance(("test"));
//	_tradelogger.removeAllAppenders();
//	this->_tradelogger.addAppender(_pFileAppender);
//	this->_tradelogger.setLogLevel(ALL_LOG_LEVEL);
}

//void LogUtil::Set(std::string logname, std::string file)
//{
//	_filename = file;
//	_logname = logname;
////	_pFileAppender=new SharedAppenderPtr(new RollingFileAppender ((_filename)));
//	_tradelogger = Logger::getInstance((_logname));
//}

void LogUtil::WriteLog(std::string log)
	{
		if(this->GetDebug())
		{

			DateUtil dateutil;
			SharedAppenderPtr _pFileAppender(new RollingFileAppender ((_filename)));
			Logger tradelogger = Logger::getInstance((_logname));
			tradelogger.removeAllAppenders();
			tradelogger.addAppender(_pFileAppender);
			tradelogger.setLogLevel(ALL_LOG_LEVEL);
			LOG4CPLUS_INFO(tradelogger,dateutil.GetDateTimeString()+"---------->"+log);
		}

	}

void LogUtil::PrintLog(const char * log)
	{
	  if(this->GetDebug())
	  {

		  DateUtil dateutil;
		SharedAppenderPtr _pFileAppender(new RollingFileAppender ((_filename)));
		Logger tradelogger = Logger::getInstance((_logname));
		tradelogger.removeAllAppenders();
		tradelogger.addAppender(_pFileAppender);
		tradelogger.setLogLevel(ALL_LOG_LEVEL);
		LOG4CPLUS_INFO(tradelogger,dateutil.GetDateTimeString()+"---------->"+log);
	  }

	}
