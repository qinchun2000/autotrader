#include "logbase.hpp"

LogBase::LogBase(){
//	printf("LogBase 构造函数   开始\n");
}
LogBase::~LogBase(){

//	printf("LogBase 析构函数   开始\n");
	EmptyLogUtil();

//	if(m_logutil!=nullptr){
//
//		printf("LogBase EmptyLogUtil   delete :m_logutil占用内存 \n");
//		delete m_logutil;
////		m_logutil=nullptr;
//	}
//	else{
//		printf("LogBase EmptyLogUtil  m_logutil nullptr \n");
//	}

//	printf("LogBase 析构函数   结束\n");
}

void LogBase::EmptyLogUtil()
{
//	printf("LogBase EmptyLogUtil  开始\n");
	if(m_logutil!=nullptr){

//		printf("LogBase EmptyLogUtil   delete :m_logutil占用内存 \n");
		delete m_logutil;
//		m_logutil=nullptr;
	}
	else{
//		printf("LogBase EmptyLogUtil  m_logutil nullptr \n");
	}
//	printf("LogBase EmptyLogUtil  结束\n");
}
void LogBase::NewLogUtil(std::string logname,std::string logfilepath)
{
	m_logutil= new LogUtil(logname,logfilepath);

//	this->m_logutil=logutil;
}
void LogBase::SetLogUtil(LogUtil * logutil)
{
	this->m_logutil=logutil;
}

LogUtil* LogBase::GetLogUtil(){
	return m_logutil;
}

void LogBase::SetLogPathFileName(const char* path,const char *logname)
{
	std::string baselog = path;
	baselog.append(logname);

	this->_log_pathfilename=baselog;
}

//std::string MarketData::GetLogPath()
//{
//	printf("LogBase-->GetLogPath -----> %s \n",this->_log_pathfilename.c_str());
//	return this->_log_pathfilename;
//}

std::string LogBase::GetLogPathFileName()
{
//	printf("LogBase-->GetLogPathFileName -----> %s \n",this->_log_pathfilename.c_str());
	return this->_log_pathfilename;
}
void LogBase::SetLogName(const char *logname)
{

	this->_logname=logname;
}
std::string LogBase::GetLogName()
{
//	printf("LogBase-->GetLogPathFileName -----> %s \n",this->_log_pathfilename.c_str());
	return this->_logname;
}
