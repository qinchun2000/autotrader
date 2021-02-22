#include <iostream>
#include <string.h>
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "httpboost.hpp"
#include "httpbase.hpp"
#include "httprequest.hpp"



int main()
{

//    std::string url = "http://hq.sinajs.cn/list=sh204001";
//	char host[] ="webftcn.hermes.hexun.com";
//    char path[] ="/shf/historyminute?code=CZCErm1705&date=20170405";

//    http://webftcn.hermes.hexun.com
//
//	char host[] ="webftcn.hermes.hexun.com";
//	char path[] ="/shf/kline?code=SHFE3rb1801&start=20170305000000&number=1000&type=5";
//
//    std::string response_data;
//
//    HttpBoost httpboost;
//
//    response_data = httpboost.GetRequestHost(host,path);
//
//    printf ("---> %s" ,response_data.c_str());
//    std::cout << response_data << std::endl;

	int rv = 0;
//	    std::string url = "http://hq.sinajs.cn/list=sh204001";
	    string w ="http://webftcn.hermes.hexun.com/shf/kline?code=SHFE3RB1710&start=20160605000000&number=1000&type=5";
	    string x ="http://webftcn.hermes.hexun.com/shf/kline?code=SHFE3RB1801&start=20170105000000&number=1000&type=5";
	    string n = "http://webftcn.hermes.hexun.com/shf/historyminute?code=CZCErm1705&date=20170405";
	    std::string response_data;
	    HttpBase httpbase;
	    rv = httpbase.get_url_response(x, response_data);
	    if (rv)  return -1;
	    printf ("x-------------------------------->");
	    std::cout << response_data << std::endl;

	    std::string response_data1;
	    rv = httpbase.get_url_response(n, response_data1);
	   	    if (rv)  return -1;
	   	    printf ("n-------------------------------->");
	   	    std::cout << response_data1 << std::endl;

    return 0;



}
