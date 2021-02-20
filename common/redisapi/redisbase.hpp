#ifndef _REDISBASE_H_
#define _REDISBASE_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
#include <json/json.h>
#include "depthmarket.hpp"

using namespace std;



class RedisBase
{

public:

	RedisBase();
    ~RedisBase();

    std::string GetHost();
    void SetDbNumber(int num);
    int GetDbNumber();
    redisContext *GetRedisContext();
    void SetRedisReply(redisReply *reply);
    redisReply *GetRedisReply();

	bool Connect(string host, int port);
	void DisConnect();

	void Empty();
	void SelectDb();
	void SelectDb(int num);
	int GetDbSize();
	int Exists(string key);
	void FlushDB();
	string Get(string key);
	void Set(string key, string value);

private:

 redisContext* _connect=nullptr;
 redisReply *_reply=nullptr;

 std::string _host;
 int _port;
 int _dbnumber=0;

};


#endif  //_REDISBASE_H_
