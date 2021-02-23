#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <boost/regex.hpp>
#include "codeutil.hpp"
#include "csvfile.hpp"
#include "config.hpp"

#ifndef _DATEUTIL_HPP_
#define _DATEUTIL_HPP_

using namespace std;

class DateUtil
	{

	public :

		DateUtil();
		int GetRandomNumber();
		void timeraddMS(struct timeval *a, long int ms);
		time_t GetCurrentTimet();
		double GetCurrentBarTime();
		int GetCurrentIntBarTime();
		double ConvertMinuteTimetoBarTime(const char* mtime);
		int GetDiffTime(const char* time,int sec);
		int CoverMinuteTimetoIntBarTime(const char* mtime);
		double GetCurrentSHFEBarTime(int difsec);
		std::string GetLastMinuteBarTime(const char * minutetime);
		std::string ConvertTimetoString(time_t t);
		std::string ConvertTimetoSqlString(time_t t);
		std::string ConvertCurrentTimetoString();
		std::string ConvertCurrentTimetoSqlTime();
		std::string ConvertSHFETimetoString(int difsec);
		std::string ConvertSHFETimetoSqlTime(int difsec);

		std::string GetTodayString();
		time_t GetCurrentDayTimet();
		std::string GetCurrentDaySqlTime();
		std::string GetCurrentYearString();
		std::string GetCurrentMonthString();
		std::string GetDateStringByTimet(time_t * tt );
		std::string GetCurrentDayString();
		int ConvertCurrentDatetoNumber();
		bool IsCurrentFridayisMonthEnd();
		std::string GetDateTimeString();
		std::string GetUTimeString();
		std::string GetTimeString();
		std::string GetCurrent_Date_TimeString();
		std::string GetCurrentSqlTimeString();

		std::string GetDateTimeing();


		std::string ConvertDatetoSqlTime(const char* date);
		std::string ConvertSqlTimetoDate(const char * datetime);

		int CollectDifSecBySHFETime(const char* rsp_shfe_time);
		int  getDaysByYearAndMonth();


		bool Check_FridayNithtbySqlTime(const char* sqltime);
		bool Check_Friday();
		bool Check_wday();
		bool Check_Sunday();
		time_t Collect_ExpireTradeDate(const char* expiredate);
		string GetNextTradingDay();
		string GetTradingDaybyCurrentDay();

		time_t GetLastCloseTime();

		time_t ConvertStringtoTimet(const char * datetime);
		time_t ConvertSqlTimetoTimet(const char * datetime);
		time_t ConvertSqlTimetoCloseTimet(const char * datetime);
		int SetSystemTime(char *dt);

		bool CheckOpenMarketTime();
		bool CheckMDOpenTime();
		time_t CheckHisDayLine(const char *tradingday,int difsec);
		time_t CheckHisLastDayLinebyDate_ExcludeHoliday(const char *tradingday,int DifSec,const char* path,const char* filename);
		time_t CheckHisLastDayLine(vector<std::string> &temp,const char *tradingday,int difsec);
		bool CheckHoliday_SqlTime(const char *tradingday_sqltime);

		bool CheckOrderInsertTime();
		bool CheckInsOrderInsertTime(const char *pinstrumentid);
		bool CheckCloseMarketTime();
		bool isFridayNight(int difsec);
		bool isNightMorningBreakTime(int difsec);
		bool CheckSHFENightTime(int difsec);
		bool CheckSHFEDayTime(int difsec);
		bool CheckSHFEOpenMarketTime(int difsec);
		bool CheckSHFEEndCloseMarketTime(int difsec);
		bool CheckDayTradeSHFEOrderInsertTime(int difsec);
		bool CheckSHFELoginMarketTime(int difsec);
		int CheckWaitSecondsbySHFELoginMarketTime(const char* tradingday);
		int CheckWaitSecondsbyOpenTime();

		bool CheckSHFEOrderInsertTime(int difsec);
		bool CheckSHFEInsOrderInsertTime(const char *pinstrumentid,int difsec);
		bool CheckDayTradeSHFEInsOrderInsertTime(const char *pinstrumentid,int difsec);
		bool CheckSHFEEndMarketTime(int difsec);

		bool CheckOpenTime(int difsec,const char *exchangeid,const char* productid);
		bool CheckMDOpenTimebyInstrumentID(double Q_BarTime_2,const char *exchangeid,const char* productid);
		bool CheckCloseAll(int difsec);

		void ReadHoliday_CSV(vector<std::string> &temp,const char* path,const char*filename);

		bool IsOpen(int difsec,const char *exchangeid,const char* productid);
		bool TodayIsHoliday();
		bool IsHolidaybyNextTradingDay();
	private :

		std::string m_path = "/root/autotrader/config/";
		std::string m_file = "holiday.csv";

	};



#endif /* _DATEUTIL_HPP_ */
