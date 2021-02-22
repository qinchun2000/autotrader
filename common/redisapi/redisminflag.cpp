#include <hiredis/hiredis.h>
#include "redisminflag.hpp"

RedisMinFlag::RedisMinFlag (){
//	printf("RedisMinFlag::RedisMinFlag   11111  \n");
	this->SetDbNumber(0);
//	printf("RedisMinFlag::RedisMinFlag   22222  \n");
	SelectDb();

//	printf("RedisMinFlag::RedisMinFlag   333333  \n");
	printf("RedisMinFlag::RedisMinFlag    ---> Begin  %d  dbsize %d \n",this->GetDbNumber(),this->GetDbSize());

}

RedisMinFlag::~RedisMinFlag(){

}


int RedisMinFlag::Exist_DatabyInstrumentID(const char * key){

	std::string str_key =key;
	int i= Exists(str_key);
	return i;

}

void RedisMinFlag::SetFlag(ContractFlag contractflag) {

	 Json::Value root;             // 表示整个 json 对象
	 root["MnKlinesig"] = Json::Value(contractflag.MnKlinesig);
	 root["last_open"] = Json::Value(contractflag.last_open);
	 root["last_high"] = Json::Value(contractflag.last_high);
	 root["last_low"] = Json::Value(contractflag.last_low);
	 root["last_close"] = Json::Value(contractflag.last_close);
//			 root["lastprice_array"].append(contractflag.last_close);
	 root["last_opi"] = Json::Value(contractflag.last_opi);
	 root["OpenVolume"] = Json::Value(contractflag.last_volume_open);
	 root["last_volume"] = Json::Value(contractflag.last_volume);
	 root["FirstTickTime"] = Json::Value(contractflag.FirstTickTime);
	 root["LastTickTime"] = Json::Value(contractflag.LastTickTime);

	 Json::StyledWriter styled_writer;
	 string value =styled_writer.write(root);
//	 std::cout << styled_writer.write(root) << std::endl;
//			 printf("----->value----> %s",value.c_str());
	 string key = contractflag.InstrumentID;
//			 printf("----->key----> %s",key.c_str());

	this->Set(key,value);
}

ContractFlag RedisMinFlag::GetFlag(std::string key)
{

	ContractFlag contractflag;

	strcpy(contractflag.InstrumentID, key.c_str());

	std::string str = this->Get(key);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root))
	{
		contractflag.MnKlinesig = root["MnKlinesig"].asBool();
		contractflag.last_open = root["last_open"].asDouble();
		contractflag.last_high = root["last_high"].asDouble();
		contractflag.last_low = root["last_low"].asDouble();
		contractflag.last_close = root["last_close"].asDouble();

		contractflag.last_opi = root["last_opi"].asInt();
		contractflag.last_volume_open = root["OpenVolume"].asInt();
		contractflag.last_volume = root["last_volume"].asInt();
		strcpy(contractflag.FirstTickTime ,root["FirstTickTime"].asString().c_str());
		strcpy(contractflag.LastTickTime ,root["LastTickTime"].asString().c_str());
	//						  printf(" MnKlinesig ---> %d   \n",contractflag.MnKlinesig);
	//						  printf(" last_open ---> %f   \n",contractflag.last_open);
	}


	return contractflag ;

}


