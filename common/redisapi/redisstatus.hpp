
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "redisbase.hpp"

using namespace std;

#ifndef _REDISINSSTATUS_H_
#define _REDISINSSTATUS_H_


class InstrumentStatusInfo
{

public:
	int InstrumentStatus;
	int EnterReason;
	char ProductID[10];
	char ExchangeID[20];
	char EnterTime[20];

private:

};



class RedisInsStatus:public RedisBase
{

public:

	RedisInsStatus();
    ~RedisInsStatus();
	void SetInsStatus(InstrumentStatusInfo insstatus);
	InstrumentStatusInfo GetInsStatus(std::string key);

private:


};


#endif  //_REDISINSSTATUS_H_
