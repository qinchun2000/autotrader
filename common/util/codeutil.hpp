#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <regex.h>
#include <fstream>
#include <unistd.h>
#include <iconv.h>

#include <iostream> /* cout */
#include <unistd.h>/* gethostname */
#include <netdb.h> /* struct hostent */
#include <arpa/inet.h> /* inet_ntop */


#ifndef _CODEUTIL_HPP_
#define _CODEUTIL_HPP_

using namespace std;

class CodeUtil
	{

	public :

	CodeUtil();
	std::string GetProductID(const char * pinstrumentid);
	std::string GetNumber(const char * pinstrumentid);
	std::string GetInstrumentid_NumberPartString(const char * pinstrumentid);
	std::string GetLastYearStringbyInstrumentID(const char * pinstrumentid);
	int GetYearbyInstrumentID(const char * pinstrumentid);
	int GetYearMonthbyInstrumentID(const char * pinstrumentid);
	std::string GetSectionByInstrumentid(const char * pinstrumentid);
	std::string GetSectionByProductid(const char * productid);
	std::string GetDayNightByProductid(const char * productid);
	bool IsNightByProductid(const char * productid);
	std::string GetWorkDirectory();
	std::string GetCurrentDirectory();

	void codeConvert(char * from,char * to,char * src,char * dst,size_t dstSize);
	void gbk2utf8(char * src,char * dst,size_t dstSize);
	void utf82gbk(char * src,char * dst,size_t dstSize);
	bool GetHostInfo(std::string& hostName, std::string& Ip) ;
	std::string GetCurrentYearString();
	int GetCurrentYearNumber();
	int GetCurrentMonthNumber();
	int GetCurrentDayNumber();

//	bool readConfigFile(const char * cfgfilepath, const string & key, string & value);

	private :



	};



#endif /* _CODEUTIL_HPP_ */
