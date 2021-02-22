#include "redisstatus.hpp"
#include <hiredis/hiredis.h>

RedisInsStatus::RedisInsStatus ()	{
	this->SetDbNumber(2);
	SelectDb();

	printf("RedisDepthMarket::RedisInsStatus    ---> Begin  %d  dbsize %d \n",this->GetDbNumber(),this->GetDbSize());

}

RedisInsStatus::~RedisInsStatus(){


}

void RedisInsStatus::SetInsStatus(InstrumentStatusInfo insstatus){

	Json::Value root;             // 表示整个 json 对象
	root["InstrumentStatus"] = Json::Value(insstatus.InstrumentStatus);
	root["EnterReason"] = Json::Value(insstatus.EnterReason);
	root["ProductID"] = Json::Value(insstatus.ProductID);
	root["ExchangeID"] = Json::Value(insstatus.ExchangeID);
	root["EnterTime"] = Json::Value(insstatus.EnterTime);

	Json::StyledWriter styled_writer;
	string value =styled_writer.write(root);
	//	 std::cout << styled_writer.write(root) << std::endl;
	//			 printf("----->value----> %s",value.c_str());
	string key = insstatus.ProductID;
	//			 printf("----->key----> %s",key.c_str());

	this->Set(key,value);
}

InstrumentStatusInfo RedisInsStatus::GetInsStatus(std::string key)
{
	InstrumentStatusInfo insstatus;
	strcpy(insstatus.ProductID, key.c_str());
	std::string str = this->Get(key);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root)){

		insstatus.InstrumentStatus = root["InstrumentStatus"].asInt();
		insstatus.EnterReason = root["EnterReason"].asInt();
		strcpy(insstatus.ExchangeID,root["ExchangeID"].asString().c_str());
		strcpy(insstatus.EnterTime,root["EnterTime"].asString().c_str());

	}

	return insstatus ;
}



