#ifndef MDSPI_HPP_
#define MDSPI_HPP_
#include <stdio.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include<vector>
#include <math.h>
#include <map>
#include <cstdint>
#include <cstdio>
#include <mutex>
//线程控制相关
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <boost/regex.hpp>

#include <unistd.h>
#include <dlfcn.h>



#include <ThostFtdcTraderApi.h>
#include <ThostFtdcMdApi.h>
#include <ThostFtdcUserApiDataType.h>
#include <ThostFtdcUserApiStruct.h>

#include "mysqldata.hpp"
#include "mysqlinstrument.hpp"
#include "mysqlproduct.hpp"
#include "mysqlmaincontract.hpp"

#include "mysqlbase.hpp"
#include "mysqlmindata.hpp"
#include "mysqltradingday.hpp"
#include "mysqlmdlogin.hpp"
#include "mysqlcommission.hpp"

#include "redisminflag.hpp"
//#include "redis.h"
#include "redismd.hpp"
#include "redismindata.hpp"
#include <json/json.h>



// 字符串编码转化
#include "code_convert.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <iomanip>
#include <log4cplus/fileappender.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include "logutil.hpp"
#include "config.hpp"
#include "mdsem.hpp"
#include "logbase.hpp"

using namespace std;

class CMdSpi : public MDSem,public LogBase,public CThostFtdcMdSpi
{  

public:
   // 构造函数，需要一个有效的指向CThostFtdcMduserApi实例的指针
	CMdSpi(CThostFtdcMdApi *pMDApi) : m_pMDApi(pMDApi) {
//		printf("CMdSpi构造函数 -----------------> aaaaaa  \n");
//		connectnumber = 0;
		this->SetMysqlMindata(&_mysqlmindata);
		this->SetMysqlProduct(&_mysqlproduct);
		this->SetMysqlInstrument(&_mysqlinstrument);
		this->SetMysqlCommission(&_mysqlcommission);


		InitCommisionMap();

		std::string mslogname="main";
		mslogname.append("_mdspi.log");
		std::string path = "/var/log/autotrader/";
		this->SetLogPathFileName(path.c_str(),mslogname.c_str());

		std::string logname_market ="main";
		logname_market.append("_mdspi");
		this->SetLogName(logname_market.c_str());

		this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());

		m_redismindata = new RedisMinData();
//		_predisdepthmarket = new RedisDepthMarket();
//		this->_predisminflag = new RedisMinFlag();

//		SetLogPathFileName();
//		printf("CMdSpi构造函数 -----------------> aaaaaa  \n");
		SetConnectErrorCode(0);   //默认启动时  连接无异常
//		printf("CMdSpi构造函数 -----------------> aaaaaa  \n");
	};
	virtual ~CMdSpi() {

		if(m_redismindata!=nullptr){
			delete m_redismindata;
		}
	};

	void SetRedisDepthMarket(RedisDepthMarket* rdepthmarket);
	RedisDepthMarket* GetRedisDepthMarket();
//	void SetRedisMinFlag(RedisMinFlag *minflag);
//	RedisMinFlag* GetRedisMinFlag();
	void SetMysqlMindata(MysqlMinData *pmysqlmindata);
	MysqlMinData* GetMysqlMindata();

	void SetMysqlProduct(MysqlProduct *pmysqlproduct);
	MysqlProduct* GetMysqlProduct();
	void SetMysqlInstrument(MysqlInstrument *pmysqlins);
	MysqlInstrument* GetMysqlInstrument();
	void SetMysqlCommission(MysqlCommission *pmysqlins);
	MysqlCommission* GetMysqlCommission();

	void SetFlagFrontDisConnect(bool flag);
	 bool GetFlagFrontDisConnect();
	 void SetFlagFrontConnectFalg(bool flag);
	 bool GetFlagFrontConnectFalg();

	 int  TryLockMutex();
	 int  UnLockMutex();

	 void SetFlagLogin(bool flag);
	 bool GetFlagLogin();
	 void SetFlagLogout(bool flag);
	 bool GetFlagLogout();
	 void SetFlagMarketOpen(bool flag);
	 bool GetFlagMarketOpen();


	 void SetFlagSubmarket(bool flag);
	 bool GetFlagSubmarket();
	 void SetFlagUnSubmarket(bool flag);
	 bool GetFlagUnSubmarket();
	 void SetFlagResetSubmarket(bool flag);
	 bool GetFlagResetSubmarket();

	 void SetConnectErrorCode(int code);
	  int GetConnectErrorCode();

	  void SetConnectCount(int count);
	  int GetConnectCount();

	  void SetTradingDay(const char* date);
	  std::string GetTradingDay();

	  void SetMnKlinesig(bool flag);
	  bool  GetMnKlinesig();

	void SetDifSec(int sec);
	int GetDifSec();
	void UpdateDifSec();
	int GetTradingDayDifSec();
	void SetCurrentQbarTime(int bartime);
	int GetCurrentQbarTime();

	void SetMindata_Database(const char* database);
	std::string GetMindata_Database();
	void SetMindata_Tablename(const char* table);
	std::string GetMindata_Tablename();


    ///错误应答  
    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,  
        int nRequestID, bool bIsLast);  
  
    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。  
    ///@param nReason 错误原因  
    ///        0x1001 网络读失败  
    ///        0x1002 网络写失败  
    ///        0x2001 接收心跳超时  
    ///        0x2002 发送心跳失败  
    ///        0x2003 收到错误报文  
    virtual void OnFrontDisconnected(int nReason);  
          
    ///心跳超时警告。当长时间未收到报文时，该方法被调用。  
    ///@param nTimeLapse 距离上次接收报文的时间  
//    virtual void OnHeartBeatWarning(int nTimeLapse);
  
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。  
    virtual void OnFrontConnected();  
      
    ///登录请求响应  
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
  
    ///订阅行情应答  
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
  
    ///取消订阅行情应答  
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);  
  
    ///深度行情通知  
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);  
  
    ///请求查询行情响应  
    //virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};  

private:  

    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);  
    int GetNightBreakTime(const char* pinstrumentid);
    int GetPreSecondNightBreakTime(const char* pinstrumentid);

    void NightProduct_0s_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void DayProduct_0s_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void NightProduct_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void DayProduct_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void FinancialProduct_DepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void Night_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void Day_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void Minute_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void BreakMinute_Begin(CThostFtdcDepthMarketDataField *pDepthMarketData);

    void Minute_MidUpdateTime(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void Minute_0s_SaveData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void Minute_59s_SaveData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void Minute_SaveOpenVolume(CThostFtdcDepthMarketDataField *pDepthMarketData);
    std::string  CheckCurrentDate(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void SaveRedisMinData(CThostFtdcDepthMarketDataField *pDepthMarketData,Mysql_MinData mysql_mindata);
    void SaveMinData(CThostFtdcDepthMarketDataField *pDepthMarketData,ContractFlag *pcontractflag);

    void SaveBreakTimeTailData_59s(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void SaveBreakTimeTailData_0s(CThostFtdcDepthMarketDataField *pDepthMarketData);

    void SaveMinData_59s(CThostFtdcDepthMarketDataField *pDepthMarketData,ContractFlag contractflag);
    void SaveTickDatatoRedis(CThostFtdcDepthMarketDataField *pDepthMarketData);

	void InitCommisionMap();
	std::shared_ptr<Commission>  FindCommissionData(const char* id);
	bool UpdateContractFlag(ContractFlag &data);
	ContractFlag FindContractFlag(const char* instrumentid);
	bool ExistContractFlag(const char* instrumentid);
private:
    // 指向CThostFtdcMduserApi实例的指针
    CThostFtdcMdApi *m_pMDApi=nullptr;

    RedisDepthMarket *_predisdepthmarket=nullptr;
//    RedisMinFlag* _predisminflag=nullptr;
    RedisMinData* m_redismindata=nullptr;

    MysqlMinData _mysqlmindata;
	MysqlMinData *_pmysqlmindata=NULL;

	MysqlProduct _mysqlproduct;
	MysqlProduct * _pmysqlproduct=NULL;
	MysqlInstrument _mysqlinstrument;
	MysqlInstrument *_pmysqlinstrument=NULL;
	MysqlCommission _mysqlcommission;
	MysqlCommission *_pmysqlcommission=NULL;

	std::mutex m_commapmutex;
	std::map<std::string,std::shared_ptr<Commission>> m_commissionmap;

	std::mutex m_contractflagmutex;
	std::map<std::string,std::shared_ptr<ContractFlag>> m_contractflagmap;

	int _ConnectCount=0;
	pthread_mutex_t _mutex_connectcount=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

    bool _FrontDisConnectFalg=false;
    pthread_mutex_t _mutex_connect=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

    bool _FrontConnectFalg=false;
    pthread_mutex_t _mutex_disconnect=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

//    static int connectnumber ;
    int _nReason=0;
    pthread_mutex_t _mutex_nReason=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/


    int _difsec=0;
    pthread_mutex_t _mutex_difsec=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

	bool _flag_logout=false;
	 pthread_mutex_t _mutex_logout=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

	bool _flag_login=false;
	 pthread_mutex_t _mutex_login=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

	bool _flag_marketopen=false;

	bool _Flag_Submarket=false;
	pthread_mutex_t _mutex_submarket=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
	bool _Flag_unSubmarket=false;
	pthread_mutex_t _mutex_unsubmarket=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/
	bool _ResetSubMarket_Flag=false;
	pthread_mutex_t _mutex_resetsubmarket=PTHREAD_MUTEX_INITIALIZER;/*互斥锁*/

	std::string _log_pathfilename;
	std::string _tradingday;

	std::string _mindata_database;
	std::string _mindata_tablename;

	int _current_qbar_time=0;

	bool	_MnKlinesig=false;		//
	double _max_price = 100000000.0;
};  
#endif /* MDSPI_HPP_ */
