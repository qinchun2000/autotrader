
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "redisbase.hpp"


using namespace std;

#ifndef _REDISORDER_H_
#define _REDISORDER_H_


//class OrderStatusInfo
//{
//
//public:
//
//
//
//	int Direction;
//	int Offset;
//	double Price;
//	double StopPrice;
//	int Volume;
//
//	int SignalCount;
//	bool SingalActive;
//	bool OrderSend;
//	char SingalStartTime[20];
//	char SingalEndTime[20];
//	int OrderSubmitStatus;
//	int OrderStatus;
//
//	int CloseSignalCount;
//	bool CloseSingalActive;
//	bool CloseOrderSend;
//	char CloseSingalStartTime[20];
//	char CloseSingalEndTime[20];
//
//
//	bool EndTimeCloseOrderSend;
//
//	char InstrumentID[10];
//	char ProductID[10];
//	char TradingDay[20];
//
//	char LocalUpdateTime[20];
//
//private:
//
//};

class Signal{

public:
	int Direction;
	int Offset;
	double Price;
	double StopPrice;
	double StopRange;
	double WinPrice;
	int Volume;

	int SignalCount;
	int MaxSignalCount;
	bool SingalActive;
	int SignalActiveCount;
	bool OrderSend;
	bool Status;
	char SingalStartTime[20];
	char SingalEndTime[20];

	int OrderSubmitStatus;
	int OrderStatus;

	//
	double Sedimentary;
	double CloseOpenRate;
	double OpenPrice;
	double PreSettlePrice;
	double PreSettleOpenRate;
	//

	char InstrumentID[40];
	char SingalName[60];
	char ProductID[30];
	char Section[20];
	char TradingDay[20];

	char LocalUpdateTime[20];

private:

};


class RedisSignal:public RedisBase
{
public:
	RedisSignal();
    ~RedisSignal();
	void SetInsSignal(Signal signal,string key);
	Signal GetInsSignal(std::string key);
private:
};


#endif  //_REDISORDER_H_
