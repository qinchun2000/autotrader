#ifndef _DATAWRAPPER_HPP
#define _DATAWRAPPER_HPP
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <map>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <mutex>
#include <stdexcept>


#include "mysqldata.hpp"
#include "mysqlmaincontract.hpp"
#include "mysqlcommission.hpp"
#include "mysqlsection.hpp"
#include "mysqlsecassess.hpp"
#include "mysqldayline.hpp"
#include "redissection.hpp"
#include "dateutil.hpp"
#include "redismd.hpp"
#include "threadbase.hpp"

using namespace std;
using namespace log4cplus;

//typedef multimap<std::string,std::shared_ptr<PositionDetailData>>::iterator POSMMAP;
//typedef std::pair<POSMMAP,POSMMAP> RANGE;


typedef multimap<std::string,std::shared_ptr<PositionDetailData>> USERTABLE;
typedef USERTABLE::const_iterator CIT;
typedef pair<CIT, CIT> Range;

class DataWrapper:public ThreadBase
{

public:

	DataWrapper();
	virtual ~DataWrapper();
	void run();

	void UpdateDateTimeCheckStatus();
	void UpdateOpenStatus();
	bool GetOpenStatus() const;
	bool GetPredataStatus() const;
	bool GetHolidayStatus() const;
	bool GetNextHolidayStatus() const;
	bool GetFridayNightStatus() const;
	bool GetNightStatus() const;

	void SetUserID(const char* id) ;
	std::string GetUserID() const;

	void SetTradingDay(const char* day);
	std::string GetTradingDay() const;

	void SetDifSec(int sec);
	int GetDifSec() const;

	bool InsertInstrumentMap(InstrumentData &data);
	std::shared_ptr<InstrumentData> FindInstrumentData(const char* instrumentid);
	bool InsertDepthMarketDataMap(DepthMarketData &data);
	std::shared_ptr<DepthMarketData> FindDepthMarketData(const char* instrumentid);
	std::map<std::string,std::shared_ptr<DepthMarketData>>* GetDepthMarketMap();
	void SetContractLists();
	vector<MainContractData>* GetContractLists();

	void SetLimitContractLists();
	vector<MainContractData>* GetLimitContractLists();

	void InitDataStatusMap();
	bool FindDataStatus(const char* id);

	void Init5DayCycleMap();
	std::shared_ptr<CycleData> Find5DayCycle(const char* id);

	void InitTotalTRMap();
	double FindTotalTR(const char* id);

	void InitCommisionMap();
	std::shared_ptr<Commission>  FindCommissionData(const char* id);

	bool EmptyHoldMap();
	bool ResetHoldMap(vector<HoldData> &list);
	bool InsertHoldData(HoldData &data);

	bool UpdateHoldData(HoldData &data);
	bool DeleteHoldData(const char* instrumentid,const int direction);
//	std::map<std::string,std::shared_ptr<HoldData>> * GetHoldMap();
	std::shared_ptr<HoldData> FindHoldMap(const char* instrumentid,const int direction);
	HoldData FindHoldData(const char* instrumentid,const int direction);
	bool GetHoldList(vector<HoldData> &list);
	void UpdateHoldMap();

	bool EmptyAssetMap();
	bool InsertAssetMap(AssetData &data);
	bool UpdateAssetMap(AssetData &data);
	std::shared_ptr<AssetData> FindAssetData(const char* userid);

	bool EmptyPositionDetailMap();
	bool InsertPositionDetailMap(PositionDetailData &data);
	bool UpdatePositionDetailMap(PositionDetailData &data);
	bool GetPositionDetailList(vector<PositionDetailData> &list);
	void GetPositionDetailListbyIns(const char* instrumentid);
	int GetDetailPosition();
	int CollectDetailCloseVolume_Total();
	double CollectAverageOpenPricebyPosDetail(const char* instrumentid);
	std::string CollectOpenDatebyPosDetail(const char* instrumentid);

	bool EmptyInvestPositionMap();
	bool InsertInvestPositionMap(InvestPositionData &data);
	bool InsertInvestPositionMap(std::shared_ptr<InvestPositionData> info);
	bool GetInvestList(vector<InvestPositionData> &list);
	bool GetHoldingInvestList(vector<InvestPositionData> &list);
	bool GetCloseInvestList(vector<InvestPositionData> &list);
	int GetCloseVolume();
	int GetHoldingPosition();
	int GetPositionbyInstrumentid(const char* ins,const int direction);

	bool InsertOpenOrderMap(OrderData &data);
	bool UpdateOpenOrderMap(OrderData &data);
	bool DeleteOpenOrderMap(OrderData &data);
	bool ExistinOrderMap(const char* instrumentid,const int direction);

	int MaxOrderRefMap();
	bool GetOpenOrderList(vector<OrderData> &list);
	bool GetOpenOrderQueueList(vector<OrderData> &list);

	bool InsertCloseOrderMap(OrderData &data);
	bool UpdateCloseOrderMap(OrderData &data);
	bool DeleteCloseOrderMap(OrderData &data);
	bool ExistinCloseOrderMap(const char* instrumentid,const int direction);
	bool GetOrderQueueList(vector<OrderData> &list);
	bool GetCloseOrderQueueList(vector<OrderData> &list);

	bool UpdateDepthMarketMap();
	bool ValidCheck_DayLineNdays(MysqlDayLine *mysqldayline,
			const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int N);

	bool ValidCheck_DepthMarket(DepthMarket *depthmarket);
	void InitialSectionMap();
	void SectionMapView();

	void GlobalMarketView();
	void Analysis();
	GlobalViewData * GetGlobalViewData();
	void WriteGlobalViewData(GlobalViewData * sourcedata,GlobalViewData * targetdata);
	void OutputGlobalViewData(GlobalViewData *view);
	void InputGlobalViewData(GlobalViewData *view);
private :

	RedisDepthMarket m_redisdepthmarket;
//	RedisSection m_redissection;
	std::mutex m_mutex;
	std::string m_tradingday;
	std::string m_userid;
	int m_diff_seconds=0;

	std::atomic_bool m_openstatus{false};
	std::atomic_bool m_predatastatus{false};

	std::atomic_bool m_holiday{false};
	std::atomic_bool m_nextholiday{false};
	std::atomic_bool m_fridaynight{false};
	std::atomic_bool m_night{false};



	std::map<std::string,std::shared_ptr<InstrumentData>> m_instrumentmap;
	std::map<std::string,std::shared_ptr<DepthMarketData>> m_depthdatamap;

	std::mutex m_submarketmutex;
	std::set<std::string> m_submarketsets;

	std::mutex m_contractmutex;
	std::vector<MainContractData>  m_contractlist;

	std::mutex m_limitcontractmutex;
	std::vector<MainContractData>  m_limitcontractlist;


	std::mutex m_datastatusmutex;
	std::map<std::string,bool> m_datastatusmap;


	std::mutex m_commapmutex;
	std::map<std::string,std::shared_ptr<Commission>> m_commissionmap;

	std::mutex m_totaltrmutex;
	std::map<std::string,double> m_totaltrmap;

//	 std::mutex m_trademutex;
//	 std::map<std::string,std::shared_ptr<OrderData>> m_trademap;
//


	std::mutex m_openordermutex;
	std::map<std::string,std::shared_ptr<OrderData>> m_openordermap;

	std::mutex m_closeordermutex;
	std::map<std::string,std::shared_ptr<OrderData>> m_closeordermap;


	 std::mutex m_assetdatamutex;
	 std::map<std::string,std::shared_ptr<AssetData>> m_assetdatamap;

	 std::mutex m_investpositionmutex;
	 std::map<std::string,std::shared_ptr<InvestPositionData>> m_investpositionmap;

	 std::mutex m_positiondetailmutex;
	 std::multimap<std::string,std::shared_ptr<PositionDetailData>> m_positiondetailmap;

	std::mutex m_holdmutex;
	std::map<std::string,std::shared_ptr<HoldData>> m_holdmap;

	std::mutex m_holddetailmutex;
	std::multimap<std::string,std::shared_ptr<HoldData>> m_holddetailmap;

	std::mutex m_depthmarketmutex;
	std::map<std::string,std::shared_ptr<DepthMarket>> m_depthmarketmap;

	std::mutex m_sectionmutex;
	std::map<std::string,std::shared_ptr<SectionInfo>> m_sectionmap;

	std::mutex m_5daycyclenmutex;
	std::map<std::string,std::shared_ptr<CycleData>> m_5daycyclemap;

	std::mutex m_gviewmutex;
	GlobalViewData m_globalview;
};

#endif /* _STADAY_HPP */
