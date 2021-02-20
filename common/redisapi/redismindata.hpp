/*
 * redismindata.cpp
 *
 *  Created on: 2020年3月9日
 *      Author: qc
 */

#ifndef REDISMINDATA_HPP_
#define REDISMINDATA_HPP_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "redisbase.hpp"
#include "mysqldata.hpp"

using namespace std;

class RedisMinData:public RedisBase
{

public:

	RedisMinData();
    ~RedisMinData();

    int Exist_DatabyInstrumentID(const char * key);
    void SetMinData(RMinData mindata);
    RMinData GetMinData(std::string key);


private:


};

#endif /* REDISMINDATA_HPP_ */



