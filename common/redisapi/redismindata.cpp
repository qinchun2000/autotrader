/*
 * redismindata.cpp
 *
 *  Created on: 2020年3月9日
 *      Author: qc
 */
#include "redismindata.hpp"

RedisMinData::RedisMinData (){
	this->SetDbNumber(5);
	SelectDb();
//	printf("RedisMinData::RedisMinData    ---> Begin  %d  dbsize %d \n",this->GetDbNumber(),this->GetDbSize());
}

RedisMinData::~RedisMinData(){

}


int RedisMinData::Exist_DatabyInstrumentID(const char * key){

	std::string str_key =key;
	int i= Exists(str_key);
	return i;

}

void RedisMinData::SetMinData(RMinData mindata) {


	 Json::Value root;             // 表示整个 json 对象
	 root["InstrumentID"] = Json::Value(mindata.InstrumentID);
	 root["MinuteTime"] = Json::Value(mindata.MinuteTime);
	 root["OpenPrice"] = Json::Value(mindata.OpenPrice);
	 root["ClosePrice"] = Json::Value(mindata.ClosePrice);
	 root["HighestPrice"] = Json::Value(mindata.HighestPrice);
	 root["LowestPrice"] = Json::Value(mindata.LowestPrice);
	 root["Amount"] = Json::Value(mindata.Amount);
	 root["Volume"] = Json::Value(mindata.Volume);
	 root["AvePrice"] = Json::Value(mindata.AvePrice);

	 root["OpenInterest"] = Json::Value(mindata.OpenInterest);
	 root["LastTickTime"] = Json::Value(mindata.LastTickTime);

	 Json::StyledWriter styled_writer;
	 string value =styled_writer.write(root);
//	 std::cout << styled_writer.write(root) << std::endl;
//			 printf("----->value----> %s",value.c_str());
	 string key = mindata.InstrumentID;
//			 printf("----->key----> %s",key.c_str());

	this->Set(key,value);
}

RMinData RedisMinData::GetMinData(std::string key)
{
	RMinData mindata;
	memset(&mindata,0,sizeof(mindata));

	std::string str = this->Get(key);
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root))	{
		strcpy(mindata.InstrumentID ,root["InstrumentID"].asString().c_str());
		strcpy(mindata.MinuteTime ,root["MinuteTime"].asString().c_str());
		mindata.OpenPrice = root["OpenPrice"].asBool();
		mindata.ClosePrice = root["ClosePrice"].asDouble();
		mindata.HighestPrice = root["HighestPrice"].asDouble();
		mindata.LowestPrice = root["LowestPrice"].asDouble();
		mindata.Amount = root["Amount"].asDouble();
		mindata.Volume = root["Volume"].asInt();
		mindata.OpenInterest = root["OpenInterest"].asInt();
	}
	return mindata ;

}




