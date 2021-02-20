#ifndef _ASSET_HPP
#define _ASSET_HPP

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


#include "mysqlasset.hpp"
#include "mysqluser.hpp"

#include "dateutil.hpp"
#include "logutil.hpp"


//#include "lock.hpp"
//#include <boost/regex.hpp>
//#include <json/json.h>

#include <float.h>
//
//#include "config.hpp"


using namespace std;
using namespace log4cplus;



class AssetBase
{

public:

	AssetBase();
	virtual ~AssetBase();
	void SetAvalible(double ava);
	double GetAvalible();
//	void SetMaxMargin(double max);
//	double  GetMaxMargin();

	double GetAssetAvaliblebyStrategyName(const char* strategynam);
	void SetRiskRate(double risk);
	double GetRiskRate();
	void SetStopRate(double stoprate);
	double GetStopRate();
	void SetWinRate(double winrate);
	double GetWinRate();
	void SetBigVolumeRate(double rate);
	double GetBigVolumeRate();

	void SetSingleMaxMargin(double margin);
	double GetSingleMaxMargin();
	void SetTotalMaxMargin(double margin);
	double GetTotalMaxMargin();

	void SetInitialMoney(double initial);
	double GetInitialMoney();
	void SetMaxSedimentary(double num);
	double GetMaxSedimentary();

    void SetHoldPositionRate(double rate);
    double GetHoldPositionRate();

    void SetTotalHolding(int total);
      int GetTotalHolding();

      void SetOpenSignalCount(int count);
      int GetOpenSignalCount();
      void SetCloseSignalCount(int count);
      int GetCloseSignalCount();
      void SetMaxCancelOrderCount(int count);
      int GetMaxCancelOrderCount();
      void SetMaxOpenErrorCount(int count);
      int GetMaxOpenErrorCount();

private :
	double _avalible=0.0; //总可用资金
//	double _riskrate;
	double _initialmoney=0.0;
	double _maxmargin=0.0;
	double _maxsedimentary=0.0;

	//用于止损额度百分比风控计算  例如 50000.00  对应 1500.00 元的亏损
	double _riskrate=0.0;
	//用于止损时 计算固定止损差
	double _stoprate=0.0;
	//用于止盈时 计算固定止盈价差
	double _winrate=0.0;

	double _bigvolumerate=0.0;
	//用于控制单合约的最大资金占用比
	double _singleins_maxmargin=0.0;
	//用于控制仓位，最大不超过的基准线
	double _total_maxmargin=0.0;

	double _holdpositionrate=0;  //持仓仓位
	int _totalholding=0;    //总持仓数

	int _open_signalcount=0;  //开仓信号持续次数满足后 激活信号
	int _close_signalcount=0; //平仓信号持续次数满足后 激活信号
	int _max_cancel_ordercount=0; //订单撤销次数超出后 不再撤销订单
	int _max_openerror_count=0;

};

#endif /* _ASSET_HPP */
