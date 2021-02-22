#ifndef _SECASSESS_HPP
#define _SECASSESS_HPP

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

#include "mysqldata.hpp"
#include "mysqlsecassess.hpp"
#include "dateutil.hpp"
#include "config.hpp"
#include "logbase.hpp"
#include "logutil.hpp"
#include "redissignal.hpp"
#include "redissection.hpp"
#include "mysqlsignal.hpp"
#include "strategy.hpp"

using namespace std;
using namespace log4cplus;



class SecAssess:public  LogBase
{

public:

	SecAssess();
	void SetSignalLabel(const char* label);
	std::string GetSignalLabel();
	void SetStrategyName(const char* strategyname);
	std::string GetStrategyName();
	void SetTradingDay(const char* date);
	std::string GetTradingDay();

	void SetSectionAcessessLists(const char* user);
	vector<SectionAssessData>* GetSectionAcessessLists();
	SectionAssessData GetSecAssessbySection(const char* section);
	void GetSecAssessbySection(const char* section,SectionAssessData *data);
	void GetSecAssessbyInstrumentID(const char* instrumentid,SectionAssessData *data,Strategy * strategy);
	SectionAssessData GetSecAssessbyPriority(vector<SectionAssessData> &list);
	void GetSecAssessbyPriority(vector<SectionAssessData> &list,SectionAssessData * data);

	void GetSecAssessbySedimentaryIORate(vector<SectionAssessData> &list,SectionAssessData * data);
	void UpdateSection_ProductHoldNumber(const char* section,int num);
	void UpdateSection_PD_Long_SignalNumber(const char* section,int num);
	void UpdateSection_PD_Short_SignalNumber(const char* section,int num);
	void UpdateSection_PD_Long(const char* section,SectionAssessData data);
	void UpdateSection_PD_Short(const char* section,SectionAssessData data);
	void Reset();
	void SectionAssess(Strategy * strategy,const char* opensignal);

	void FilterSectionList(vector<SectionAssessData>  &seclists);
	void StaticsSectionList(vector<SectionAssessData>  &seclists,SignalAssessData *data);
	void UpdateMysqlSignal(SignalAssessData* data);
	bool SectionRecommand(SectionAssessData * ret_data);
	bool SectionRecommandbyMoreSignal(SectionAssessData * ret_data);
	bool SectionRecommandbyGlobalView(SectionAssessData * ret_data,Strategy * strategy);
private :

	vector<SectionAssessData> _sectionlists;
	std::string _signallabel;
	std::string _strategyName;
	std::string _tradingday;

};

#endif /* _SECASSESS_HPP */
