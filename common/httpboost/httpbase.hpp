#include <iostream>
#include <istream>
#include <ostream>
#include <string.h>
#include <string>
#include <boost/asio.hpp>
#include <cstdint>
#include <cstdio>
#include <stdio.h>
#include <unistd.h>


#ifndef _HTTPBASE_HPP__
#define _HTTPBASE_HPP__



using namespace std;


class HttpBase
{

public:

    HttpBase();

    virtual ~HttpBase();
    int64_t getTimeMsec();
    int boost_http_sync_client(const std::string& server, const std::string& port, const std::string& path,
            std::string& out_response_status_line, std::string& out_response_header, std::string& out_response_data);

    int boost_http_sync_client1(const std::string& server,
    const std::string& port, const std::string& path,
    std::string& out_response_status_line, std::string& out_response_header,
    std::string& out_response_data);

    int parse_hq_sinajs_cn_and_get_last_price(const std::string& market_data, double& last_price);
    int get_url_response(const std::string& url, std::string& out_response_data);
    int parse_url(const std::string& url, std::string& out_server, std::string& out_port, std::string& out_path);





};



#endif // _HTTPBASE_HPP__
