#ifndef __TICKTOKLINEUTIL_HPP__
#define __TICKTOKLINEUTIL_HPP__

#include <fstream>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include "csvfile.hpp"
#include <math.h>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <regex.h>
#include "stdlib.h"
#include <dirent.h>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>
#include <iostream>


#include <unistd.h>
#include <boost/regex.hpp>

#include <unistd.h>
#include <dlfcn.h>
#include "codeutil.hpp"
#include "dateutil.hpp"

class KLineDataType
{
public:
	char Time[20];
    double open_price;   // 开
    double high_price;   // 高
    double low_price;    // 低
    double close_price;  // 收
    int volume;          // 量
    int opi;
};

class TickDataField
{
public:
	char	TradingDay[10];
	char	InstrumentID[10];
	char	ExchangeID[10];
	double	LastPrice;
	int Volume;
	int	OpenInterest;
	char	UpdateTime[20];
	char	UpdateMillisec[10];
	double	BidPrice1;
	int	BidVolume1;
	double	AskPrice1;
	int	AskVolume1;
	double	AveragePrice;

};

class TickToKlineUtil
{
public:
	TickToKlineUtil();
	void WriteSingleMinuteline(const char* pinstrumentid);
	void ConvertTick_Minuteline(const char* productid,const char* year,
			const char*tradingday,const char* preday,const char* contract_month);

	void ReadTickData_CSV(vector<KLineDataType> &temp,const char* path,const char*filename,const char* preday);

private:
	typedef std::vector<double> OneMinute_TickPriceVEC;
	typedef std::vector<int> OneMinute_TickVolumeVEC;
	typedef std::vector<int> OneMinute_TickOPIVEC;

	OneMinute_TickPriceVEC m_priceVec;
	OneMinute_TickVolumeVEC m_volumeVec;
	OneMinute_TickOPIVEC m_opiVec;
};

#endif // __TICKTOKLINEUTIL_HPP__
