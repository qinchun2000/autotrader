#ifndef _STRATEGYBASE_HPP
#define _STRATEGYBASE_HPP

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
#include <float.h>//
#include "config.hpp"
#include "assetbase.hpp"
//#include "secassess.hpp"
#include "logbase.hpp"
#include "threadbase.hpp"
#include "datawrapper.hpp"

using namespace std;
using namespace log4cplus;

class ReferenceData
{
public:
	double High20;
	double Low20;
	double highest;
	double lowest;
	int HighCount;
	int LowCount;
};


class StrategyBase:public Tech,public LogBase,public AssetBase,public ThreadBase
{

public:

	StrategyBase();
	virtual ~StrategyBase();

	virtual void Open()=0;
	virtual void Close()=0;


	std::string GetUserID();

	void SetDataWrapper(DataWrapper *point);
	DataWrapper* GetDataWrapper();

//	void SetMysqlInstrument(MysqlInstrument *pmysqlins);
//	MysqlInstrument* GetMysqlInstrument();

	void SetMysqlDepthMarket(MysqlDepthMarket *pmysqdepth);
	MysqlDepthMarket* GetMysqlDepthMarket();

	void SetMysqlMaincontract(MysqlMaincontract *pmysqmc);
	MysqlMaincontract* GetMysqlMaincontract();

	MysqlHold* GetMysqlHold();
	void SetMysqlHold(const char* userid);

	void SetContractLists(vector<MainContractData>* p);
	void SetContractLists();
	vector<MainContractData>* GetContractLists();
//	void SetSectionAcessessLists();
//	vector<SectionAssessData>* GetSectionAcessessLists();
//	SectionAssessData* GetSectionAcessessLists_pElement(const char* section);

//	void SetSecAcessess(SecAssess* psassess);
//	SecAssess* GetSecAcessess();
	void SetGlobalViewData(GlobalViewData * data);

//	void CopyGlobalViewData(GlobalViewData * data);
	void WriteGlobalViewData(GlobalViewData * sourcedata,GlobalViewData * targetdata);
	GlobalViewData * GetGlobalViewData();
	void OutputGlobalViewData(GlobalViewData *view);
	void InputGlobalViewData(GlobalViewData *view);

	void SetSignalCount(int count);
	int GetSignalCount();

	void SetMaxUpDownRate(double rate,const char* pinstrumentid);
	double GetMaxUpDownRate();
	std::string GetMaxUpDownRateInstrumentid();

	void SetMinUpDownRate(double rate,const char* pinstrumentid);
	double GetMinUpDownRate();
	std::string GetMinUpDownRateInstrumentid();

	void SetStopPrice(double price);
	double GetStopPrice();
	void SetStopPriceRange(double range);
	double GetStopPriceRange();

	void MarketMonitor();
	void SectionMarketView();
	void Analysis();
	void GetAbsMaxUpDownIns();
	void StatisticsSectionSedminentaryIO(GlobalViewData *view);
	void MysqlSectionAssess(const char* opensignal);
//	void SectionAssess(const char* opensignal);
	int Valid_StaBaseDataCheck(const char* pinstrumentid,MysqlDayLine *mysqldayline,
			const char* tradingdate,HoldData *holddata);
	bool ValidCheck_DayLine(const char* pinstrumentid,const char* tradingdate,const char* exchangeid);
//	bool ValidCheck_DayLineNdays(const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int N);
	bool ValidCheck_SectionPosition(const char*section);

	int  GetUpDownRateOrderNumberBySection(const char* pinstrumentid,const char* section,int direction);
	double  GetStopPrice(const char* pinstrumentid,int direction,double openprice);
	double GetCurrentDayTR(DepthMarket *pdepthmarket);
	int  GetPositionByAvalibleATR(double avalibe,const char* pinstrumentid,
			DepthMarket depthmarket,double atr20,double margin);
	int  GetPositionByATR(const char* pinstrumentid,DepthMarket depthmarket,double atr20,double margin);
	void DayTrade_EndTimeClose_SignalCheck();
	void Close_WinPrice_SignalCheck();
	void Close_DynamicStop_SignalCheck();
	void Close_LimitPrice_SignalCheck();
	void Single_Close_EndTime(HoldData &item);

	void PreProcess_CloseSignalHoldData(Signal * signal,
				HoldData *holddata,double price,const char* singalname );
	void SignalCheck(HoldData item,std::string signalname,
			const char* tradingday,int signalcount,
			int direction,int offset,double lastprice);
	//	void SignalCheck(HoldData item,
	//			std::string signalname,
	//			int signalcount,
	//			int direction,
	//			int offset,
	//			double lastprice);

	void CloseSignalCheck(HoldData item,
			std::string signalname,
			int signalcount,
			int direction,
			int offset,
			double lastprice);


	void OpenSignalCheck(HoldData item,
				std::string signalname,
				const char* tradingday,
				int signalcount,
				int direction,
				int offset,
				double lastprice);

	void OpenSignalUpdate(Signal * originalsignal);

	void CloseSignalUpdate(Signal * originalsignal);
	void SignalDispearCheck(std::string signalname);

	void OpenPriceSeek();
	void ClosePriceSeek();
	void QueueingOpenPriceChange_SignalCheck();
	void QueueingClosePriceChange_SignalCheck();

private :

	GlobalViewData _globalviewdata;
	GlobalViewData *_pglobalviewdata=nullptr;
	DataWrapper * m_pdatawrapper=nullptr;

	std::string _holiday_path;
	std::string _holiday_filename;
	std::string _log_pathfilename;
	int _signalcount ;

	double _max_priceupdownrate;
	std::string  _max_updownrate_instrumentid;
	double _min_priceupdownrate;
	std::string  _min_updownrate_instrumentid;

	double _stopprice;
	double _stoprange;

	vector<MainContractData>  _contractlist;
	vector<MainContractData> * m_pclist;

	MysqlDepthMarket _mysqldepthmarket;
	MysqlDepthMarket *_pmysqldepthmarket=NULL;

	MysqlMaincontract _mysqlmaincontract;
	MysqlMaincontract *_pmysqlmaincontract=NULL;

	MysqlHold * m_mysqlhold=nullptr;


};

#endif /* _STRATEGYBASE_HPP */
