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


#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

using namespace std;

class Config
	{

public :

//	Config();
	Config(const char* configfilename);
	std::string GetWorkDirectory();

	void Initial();

	void SetConfigFileName(const char* filename);
	std::string GetConfigFileName();

	void SetConfigPath(const char* path);
	std::string GetConfigPath();

	std::string GetConfigPathFile();

	void SetNightPorductPathFile(const char* pathfile);
	std::string GetNightPorductPathFile();

	void SetFrontAddressLists(const char* address);
	void GetFrontAddressLists(vector<std::string> &list);

	void SetMdFrontAddressLists(const char* address);
	void GetMdFrontAddressLists(vector<std::string> &list);

	void SetBrokerID(const char* brokerid);
	void SetBrokerID();
	std::string GetBrokerID();

	void SetUserID();
	void SetUserID(const char* userid);
	std::string GetUserID();

	void SetPasswrod();
	void SetPasswrod(const char* password);
	std::string GetPasswrod();

	void SetStrategyName();
	std::string GetStrategyName();

	void SetFrontAddress();
	std::string GetFrontAddress();
	void SetMDAddress();
	std::string GetMDAddress();
	void SetAppID();
	std::string GetAppID();
	void SetProductInfo();
	std::string GetProductInfo();
	void SetAuthCode();
	std::string GetAuthCode();

	void SetEmailAddress(const char* address);
	std::string GetEmailAddress();

	void SetEmailPasswrod(const char* password);
	std::string GetEmailPasswrod();

	void SetSmtpAddress(const char* address);
	std::string GetSmtpAddress();

	void SetInitialMoney();
	double GetInitialMoney();

	void SetRiskRate();
	double GetRiskRate();

	void SetStopRate();
	double GetStopRate();

	void SetWinRate();
	double GetWinRate();

	void SetBigVolumeRate();
	double GetBigVolumeRate();

	void SetMaxSedimentary();
	double GetMaxSedimentary();

	void SetSingleMaxMargin();
	double GetSingleMaxMargin();
	void SetTotalMaxMargin();
	double GetTotalMaxMargin();

	void SetOpenSignalCount();
	int GetOpenSignalCount();
	void SetCloseSignalCount();
	int GetCloseSignalCount();
	void SetMaxCancelOrderCount();
	int GetMaxCancelOrderCount();
	void SetMaxOpenErrorCount();
	int GetMaxOpenErrorCount();
	void SetMaxCloseErrorCount();
	int GetMaxCloseErrorCount();


	void SetLogPath(const char* path);
	std::string GetLogPath();

//	void SetLogPath_Base(const char* path);
//	std::string GetLogPath_Base();

	void SetHolidayPath(const char* path);
	std::string GetHolidayPath();

	void SetHolidayFileName(const char* filename);
	std::string GetHolidayFileName();

	std::string GetHisXlsPath();

	void SetTable_Open(const char* open);
	std::string GetTable_Open();

	void SetTable_Close(const char* close);
	std::string GetTable_Close();

	void SetTable_OrderList(const char* orderlist);
	std::string GetTable_OrderList();

	bool readConfigFile(const char * cfgfilepath, const string & key, string & value);
	bool IsNightProduct(const char* product);

private :


	std::string _brokerid;
	std::string _userid;
	std::string _password;
	std::string _strategyname;
	std::string _frontaddress;
	std::string _mdaddress;
	std::string _appid;
	std::string _productinfo;
	std::string _authcode;

	std::string _email_address;
	std::string _email_password;
	std::string _smtp_address;

	double _initialmoney;
	//用于止损额度百分比风控计算  例如 50000.00  对应 1500.00 元的亏损
	double _riskrate;
	//用于止损时 计算固定止损差
	double _stoprate;
	//用于止盈时 计算固定止盈价差
	double _winrate;
	double _bigvolumerate;
	double _maxsedimentary;
	//用于控制单合约的最大资金占用比
	double _singleins_maxmargin;
	//用于控制仓位，最大不超过的基准线
	double _total_maxmargin;

	int _open_signalcount;  //开仓信号持续次数满足后 激活信号
	int _close_signalcount; //平仓信号持续次数满足后 激活信号
	int _max_cancel_ordercount; //订单撤销次数超出后 不再撤销订单

	int _max_openerror_count;
	int _max_closeerror_count;

	vector<std::string> _front_list;
	vector<std::string> _md_front_list;

	std::string _holiday_path;
	std::string _holiday_filename;

	std::string _table_open;
	std::string _table_close;
	std::string _table_orderlist;

	std::string _log_path;
//	std::string _logpath_baseinfo;
	std::string _config_path;
	std::string _config_filename;
	std::string _nightproduct_pathfile;
	std::string _hisxls_path;

	};



#endif /* _CONFIG_HPP_ */
