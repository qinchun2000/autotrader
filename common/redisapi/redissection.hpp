
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "depthmarket.hpp"
#include "redisbase.hpp"

using namespace std;

#ifndef _REDISSECTION_H_
#define _REDISSECTION_H_

class SectionInfo
{
public:
        char SectionName[20];
        char MaxInstrumentID[10];
        char MinInstrumentID[10];
        int UpCount;
        int DownCount;
        int ZeroCount;
        double Maxrate;
        double Minrate;
        double TotalUprate;
        double TotalDownrate;
        double AverageUpDownrate;
        double CountUprate;
        double CountDownrate;
        double Sedimentary;   // 板块总沉淀资金
        double SedimentaryIO;  //板块流入流出资金
        double SedimentaryIORate;  // 板块流入流出资金比例
};


class RedisSection:public RedisBase{
public:
	RedisSection();
    ~RedisSection();
	void SetSection(SectionInfo orderstatusinfo);
	SectionInfo GetSection(std::string key);

private:
};


#endif  //_REDISORDER_H_
