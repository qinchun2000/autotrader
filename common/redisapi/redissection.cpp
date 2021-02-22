#include "redissection.hpp"
#include <hiredis/hiredis.h>

RedisSection::RedisSection (){
	this->SetDbNumber(4);
	SelectDb();
//	printf("RedisSection  构造函数---> dbnumber:%d  dbsize: %d \n",this->GetDbNumber(),this->GetDbSize());
}

RedisSection::~RedisSection(){

}


void RedisSection::SetSection(SectionInfo info){

	Json::Value root;             // 表示整个 json 对象
	root["SectionName"] = Json::Value(info.SectionName);
	root["MaxInstrumentID"] = Json::Value(info.MaxInstrumentID);
	root["MinInstrumentID"] = Json::Value(info.MinInstrumentID);
	root["UpCount"] = Json::Value(info.UpCount);
	root["DownCount"] = Json::Value(info.DownCount);
	root["ZeroCount"] = Json::Value(info.ZeroCount);

	root["Maxrate"] = Json::Value(info.Maxrate);
	root["Minrate"] = Json::Value(info.Minrate);
	root["TotalUprate"] = Json::Value(info.TotalUprate);
	root["TotalDownrate"] = Json::Value(info.TotalDownrate);
	root["AverageUpDownrate"] = Json::Value(info.AverageUpDownrate);
	root["CountUprate"] = Json::Value(info.CountUprate);
	root["CountDownrate"] = Json::Value(info.CountDownrate);

	root["SedimentaryIO"] = Json::Value(info.SedimentaryIO);
	root["SedimentaryIORate"] = Json::Value(info.SedimentaryIORate);
	root["Sedimentary"] = Json::Value(info.Sedimentary);



	Json::StyledWriter styled_writer;
	string value =styled_writer.write(root);
	//	 std::cout << styled_writer.write(root) << std::endl;
	//			 printf("----->value----> %s",value.c_str());
	string key = info.SectionName;
	//			 printf("----->key----> %s",key.c_str());

	this->Set(key,value);
}
SectionInfo RedisSection::GetSection(std::string key){

	SectionInfo info;
	strcpy(info.SectionName, key.c_str());
	//		 			  printf("reply type ----->connect error: %s \n", this->_connect->errstr);
	std::string str = this->Get(key);
	//		 				 printf("Get the GetDepthMarket  ---------> %s \n",str.c_str());
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root))	{
	//							printf ("depthmarket.UpdateTime->%s  ------%s   --->%s    \n ",root["UpdateTime"].asString().c_str(),UpdateTime.c_str(),depthmarket.UpdateTime);
		strcpy(info.SectionName,root["SectionName"].asString().c_str());
		strcpy(info.MaxInstrumentID,root["MaxInstrumentID"].asString().c_str());
		strcpy(info.MinInstrumentID,root["MinInstrumentID"].asString().c_str());
		info.UpCount = root["UpCount"].asInt();
		info.DownCount = root["DownCount"].asInt();
		info.ZeroCount = root["ZeroCount"].asInt();
		info.Maxrate = root["Maxrate"].asDouble();
		info.Minrate = root["Minrate"].asDouble();
		info.TotalUprate = root["TotalUprate"].asDouble();
		info.TotalDownrate = root["TotalDownrate"].asDouble();
		info.AverageUpDownrate = root["AverageUpDownrate"].asDouble();
		info.CountUprate = root["CountUprate"].asDouble();
		info.CountDownrate = root["CountDownrate"].asDouble();
		info.SedimentaryIO = root["SedimentaryIO"].asDouble();
		info.SedimentaryIORate = root["SedimentaryIORate"].asDouble();
		info.Sedimentary = root["Sedimentary"].asDouble();
	}
	return info ;

}



