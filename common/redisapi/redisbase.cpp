#include "redisbase.hpp"
#include <hiredis/hiredis.h>

RedisBase::RedisBase (){
//	printf("RedisBase::RedisBase   11111  \n");
	this->_host = "127.0.0.1";
	this->_port = 6379;
	Connect(this->_host,this->_port);
//	printf("RedisBase::RedisBase   22222  \n");
}

RedisBase::~RedisBase(){
	DisConnect();   //redisContext* _connect=nullptr;
	 //redisReply *_reply=nullptr;//  // 该指针无须释放内存
}

bool RedisBase::Connect(std::string host, int port){

	this->_connect = redisConnect(host.c_str(), port);

	if(this->GetRedisContext() != NULL && this->GetRedisContext()->err){
	  printf("connect error: %s\n", this->GetRedisContext()->errstr);
	return 0;
	}

	return 1;
}

void RedisBase::DisConnect(){
	redisFree(this->_connect);
}

std::string RedisBase::GetHost(){
	return this->_host;
}


void RedisBase::SetDbNumber(int num){
	this->_dbnumber=num;
}

int RedisBase::GetDbNumber(){
	return this->_dbnumber;
}

redisContext *RedisBase::GetRedisContext()
{
	return this->_connect;
}

void RedisBase::SetRedisReply(redisReply *reply){
	this->_reply=reply;
}
redisReply *RedisBase::GetRedisReply(){
	return this->_reply;
}


void RedisBase::Empty(){

	 if(this->GetRedisReply() !=nullptr)	{
//		 printf("RedisBase::Empty    非空  执行清除命令\n");
		 freeReplyObject(this->GetRedisReply());
	}
	else{
//		printf("RedisBase::Empty    空  不用执行清除命令!!!\n");
	}
}

void RedisBase::SelectDb(){
//	printf("RedisBase::SelectDb()   11111  \n");
//	SetRedisReply((redisReply*)redisCommand(this->GetRedisContext(), "SELECT %d",this->GetDbNumber()));

	this->_reply = (redisReply*)redisCommand(this->_connect, "SELECT %d", this->GetDbNumber());

//	printf("RedisBase::SelectDb()   22222  \n");
	Empty();
//	printf("RedisBase::SelectDb()   33333  \n");
}

void RedisBase::SelectDb(int num){

//	printf("RedisBase::SelectDb   11111  \n");

	SetRedisReply((redisReply*)redisCommand(this->GetRedisContext(), "SELECT %d",num));
//	printf("RedisBase::SelectDb   11111  \n");
	Empty();
//	printf("RedisBase::SelectDb   11111  \n");
}

int RedisBase::GetDbSize(){

	int ret;
	this->SetRedisReply( (redisReply*)redisCommand(this->GetRedisContext(), "DBSIZE"));

	if (this->GetRedisReply()->integer != REDIS_REPLY_NIL && this->GetRedisContext()->err)   {
		ret= -1 ;
		printf("RedisBase::GetDbSize   ---->error  %d \n",ret);
	}
	else {
		ret= this->GetRedisReply()->integer;
//		printf("RedisBase::GetDbSize   ---->successes  %d \n",ret);
	}
	Empty();
	return ret;
}
/////
//
//////
int RedisBase::Exists(std::string key){

//	printf("RedisBase::Exists   ----> %s \n",key.c_str());
	int ret;
	this->SetRedisReply( (redisReply*)redisCommand(this->GetRedisContext(), "EXISTS %s", key.c_str()));

	if (this->GetRedisReply()->integer != REDIS_REPLY_NIL && this->GetRedisContext()->err)   {
		ret= -1 ;
//		printf("RedisBase::Exists   ---->error  %d \n",ret);
	}
	else {
		ret= this->GetRedisReply()->integer;
//		printf("RedisBase::Exists   ---->successes  %d \n",ret);
	}
	Empty();
	return ret;

}

void RedisBase::FlushDB()  {

//	printf("RedisBase::FlushDB    ---> Begin  %d  dbsize %d \n",this->GetDbNumber(),this->GetDbSize());

//	this->_reply = (redisReply*)redisCommand(this->_connect, "SELECT %d",this->_dbnumber);
//	Empty();
	SelectDb();

//	printf("RedisBase::FlushDB    ---> .....  %d  dbsize %d \n",this->GetDbNumber(),this->GetDbSize());


	SetRedisReply((redisReply*)redisCommand(this->GetRedisContext(), "FLUSHDB"));

	Empty();
	printf("RedisBase::FlushDB    ---> 清空完成  %d dbsize %d \n",this->GetDbNumber(),this->GetDbSize());
}
/////
//
//////
std::string RedisBase::Get(std::string key){

	std::string str;

	SetRedisReply((redisReply*)redisCommand(this->GetRedisContext(), "GET %s",key.c_str()));

//	printf("RedisBase::Get  %s \n",key.c_str());

	if (this->GetRedisReply()->type != REDIS_REPLY_NIL)  {

		str = this->GetRedisReply()->str;
//		printf("RedisBase::Get  Value: %s \n",str.c_str());
	}
	else  {
		printf("RedisBase::Get   DbNumber[%d]  reply type ------>  %s----->connect error: %s \n",
				this->GetDbNumber(),key.c_str(),this->GetRedisContext()->errstr);
		//	  std::string str = this->_reply->str;
		str= "";
	}

	Empty();
	return str ;

}

void RedisBase::Set(std::string key, std::string value){

//	this->_reply = (redisReply*)redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
	SetRedisReply((redisReply*)redisCommand(this->GetRedisContext(),"SET %s %s", key.c_str(), value.c_str()));

//	char sql[4096];   //此写法存在buf 空间不足的隐患
//	sprintf(sql,"SET %s %s",key.c_str(), value.c_str());
//	this->Execute(sql);

	Empty();
}
