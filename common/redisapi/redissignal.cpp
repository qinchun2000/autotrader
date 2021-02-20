#include "redissignal.hpp"

#include <hiredis/hiredis.h>

RedisSignal::RedisSignal (){
	this->SetDbNumber(3);
	SelectDb();

//	printf("RedisSignal  构造函数---> dbnumber:%d  dbsize: %d \n",this->GetDbNumber(),this->GetDbSize());
}

RedisSignal::~RedisSignal(){
}


void RedisSignal::SetInsSignal(Signal signal,string key) {

//			 printf("----->key----> %s",key.c_str());

	 Json::Value root;             // 表示整个 json 对象
	 root["InstrumentID"] = Json::Value(signal.InstrumentID);
	 root["TradingDay"] = Json::Value(signal.TradingDay);
	 root["SingalStartTime"] = Json::Value(signal.SingalStartTime);
	 root["SingalEndTime"] = Json::Value(signal.SingalEndTime);
	 root["SignalCount"] = Json::Value(signal.SignalCount);
	 root["MaxSignalCount"] = Json::Value(signal.MaxSignalCount);
	 root["SingalActive"] = Json::Value(signal.SingalActive);
	 root["OrderSend"] = Json::Value(signal.OrderSend);
	 root["Status"] = Json::Value(signal.Status);

	 root["Direction"] = Json::Value(signal.Direction);
	 root["Offset"] = Json::Value(signal.Offset);
	 root["Price"] = Json::Value(signal.Price);
	 root["StopPrice"] = Json::Value(signal.StopPrice);
	 root["WinPrice"] = Json::Value(signal.WinPrice);
	 root["StopRange"] = Json::Value(signal.StopRange);
	 root["Volume"] = Json::Value(signal.Volume);
	 root["LocalUpdateTime"] = Json::Value(signal.LocalUpdateTime);
	 root["SingalName"] = Json::Value(signal.SingalName);


	 Json::StyledWriter styled_writer;
	 string value =styled_writer.write(root);
//	 std::cout << styled_writer.write(root) << std::endl;
//			 printf("----->value----> %s",value.c_str());

	this->Set(key,value);
}


Signal RedisSignal::GetInsSignal(std::string key)	 {
	Signal signal;
	memset(&signal,0,sizeof(signal));

	std::string str = this->Get(key);
//	printf("RedisSignal::GetInsSignal  ---------> %s \n",str.c_str());

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root))	{

		strcpy(signal.InstrumentID,root["InstrumentID"].asString().c_str());
		strcpy(signal.TradingDay,root["TradingDay"].asString().c_str());
		strcpy(signal.SingalName,root["SingalName"].asString().c_str());
		strcpy(signal.SingalStartTime,root["SingalStartTime"].asString().c_str());
		strcpy(signal.SingalEndTime,root["SingalEndTime"].asString().c_str());
		signal.SignalCount = root["SignalCount"].asInt();
		signal.MaxSignalCount = root["MaxSignalCount"].asInt();

		signal.SingalActive = root["SingalActive"].asBool();
		signal.OrderSend = root["OrderSend"].asBool();
		signal.Status = root["Status"].asBool();

		signal.Direction = root["Direction"].asInt();
		signal.Offset = root["Offset"].asInt();
		signal.Price = root["Price"].asDouble();
		signal.StopPrice = root["StopPrice"].asDouble();
		signal.WinPrice = root["WinPrice"].asDouble();
		signal.StopRange = root["StopRange"].asDouble();
		signal.Volume = root["Volume"].asInt();

		strcpy(signal.LocalUpdateTime,root["LocalUpdateTime"].asString().c_str());

	}


	return signal ;
}
