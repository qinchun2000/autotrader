#ifndef _REDIS_H_
#define _REDIS_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>

class Redis
{
public:

    Redis(){}

    ~Redis()
 {
        this->_connect = NULL;
  this->_reply = NULL;        
 }

 bool connect(std::string host, int port)
 {
    this->_connect = redisConnect(host.c_str(), port);
  if(this->_connect != NULL && this->_connect->err)
  {
      printf("connect error: %s\n", this->_connect->errstr);
   return 0;
  }
  return 1;
 }

 /////
 //
 //////
int exists(std::string key)
  {
	   this->_reply = (redisReply*)redisCommand(this->_connect, "exists %s", key.c_str());

	   printf("Get the aaaaaaa name is null\n");
	   if (this->_reply->integer != REDIS_REPLY_NIL && this->_connect->err)
	   {


			freeReplyObject(this->_reply);
			return -1 ;
	   }
	   else
	   {


		  int a = this->_reply->integer;

				freeReplyObject(this->_reply);
				return a;

	   }

  }

/////
//
//////
std::string get(std::string key)
 {
	  this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());

	  printf("Get the aaaaaaa name is null\n");
	  if (this->_reply->type != REDIS_REPLY_NIL)
	  {
		  printf("reply type ----->connect error: %s \n", this->_connect->errstr);
			std::string str = this->_reply->str;
			freeReplyObject(this->_reply);
			return str ;
	  }
	  else
	  {

		  printf("Get the bbbbbb name is null\n");
	//	  std::string str = this->_reply->str;
		  printf("Get the ccccccc name is null\n");
				freeReplyObject(this->_reply);
				return "";

	  }


 }

 void set(std::string key, std::string value)
 {
        redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
 }

private:

    redisContext* _connect;
 redisReply* _reply;
    
};

#endif  //_REDIS_H_
