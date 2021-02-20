#ifndef _REDISMD_H_
#define _REDISMD_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "redisbase.hpp"
#include "depthmarket.hpp"

using namespace std;



class RedisDepthMarket:public RedisBase
{

public:

	RedisDepthMarket();
    ~RedisDepthMarket();

    int Exist_DatabyInstrumentID(const char * key);
	void SetDepthMarket(DepthMarket depthmarket);
	DepthMarket GetDepthMarket(std::string key);

private:


};


#endif  //_REDISMD_H_
