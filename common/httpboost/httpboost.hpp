#include <iostream>
#include <istream>
#include <ostream>
#include <string.h>
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <boost/asio.hpp>
using boost::asio::ip::tcp;
using namespace std;


#ifndef _HTTPBOOST_HPP_
#define _HTTPBOOST_HPP_


class HttpBoost
{

public:
	HttpBoost();
	~HttpBoost();

	/// GET请求
	string GetRequestHost(char* host, char* path);
	string GetRequest(string url);

	/// POST请求
	string PostRequest(char* host, char* path, string form);
private:
//	int boost_http_sync_client(const std::string& server, const std::string& port, const std::string& path,
//	    std::string& out_response_status_line, std::string& out_response_header, std::string& out_response_data);


};
#endif  //_HTTPBOOST_HPP_
