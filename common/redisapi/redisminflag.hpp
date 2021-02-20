#ifndef _REDISMINFLAG_H_
#define _REDISMINFLAG_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "redisbase.hpp"

using namespace std;

class ContractFlag
{
public :
	char InstrumentID[21];
	bool MnKlinesig;
//	double  Mn_open	;			//
//	double  Mn_high;			//
//	double  Mn_low;			//
//	double  Mn_close;			//

	int last_opi_open;
	int last_opi;
	int last_volume_open;
	int last_volume;
//	vector<double> pricelists;
	double  last_open	;			//
	double  last_high;			//
	double  last_low;			//
	double  last_close;			//
	char FirstTickTime[21];
	char LastTickTime[21];

};

class RedisMinFlag:public RedisBase
{

public:

	RedisMinFlag();
    ~RedisMinFlag();

    int Exist_DatabyInstrumentID(const char * key);
    void SetFlag(ContractFlag contractflag);
    ContractFlag GetFlag(std::string key);


private:


};


#endif  //_REDISMINFLAG_H_
