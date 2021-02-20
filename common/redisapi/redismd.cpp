#include "redismd.hpp"
#include <hiredis/hiredis.h>

RedisDepthMarket::RedisDepthMarket (){
	this->SetDbNumber(1);
	SelectDb();

//	printf("RedisDepthMarket构造函数---> dbnumber:%d  dbsize: %d \n",this->GetDbNumber(),this->GetDbSize());
}

RedisDepthMarket::~RedisDepthMarket(){

}

int RedisDepthMarket::Exist_DatabyInstrumentID(const char * key){

	std::string str_key =key;
	int i= Exists(str_key);
	return i;

}

void RedisDepthMarket::SetDepthMarket(DepthMarket depthmarket) {

	 Json::Value root;             // 表示整个 json 对象
	 root["InstrumentID"] = Json::Value(depthmarket.InstrumentID);
	 root["TradingDay"] = Json::Value(depthmarket.TradingDay);
	 root["ExchangeID"] = Json::Value(depthmarket.ExchangeID);
	 root["ProductID"] = Json::Value(depthmarket.ProductID);
	 root["LastPrice"] = Json::Value(depthmarket.LastPrice);
	 root["AskPrice1"] = Json::Value(depthmarket.AskPrice1);
	 root["BidPrice1"] = Json::Value(depthmarket.BidPrice1);
	 root["PreSettlementPrice"] = Json::Value(depthmarket.PreSettlementPrice);

	 root["PreClosePrice"] = Json::Value(depthmarket.PreClosePrice);
	 root["OpenInterest"] = Json::Value(depthmarket.OpenInterest);
	 root["PreOpenInterest"] = Json::Value(depthmarket.PreOpenInterest);
	 root["OpenPrice"] = Json::Value(depthmarket.OpenPrice);
	 root["HighestPrice"] = Json::Value(depthmarket.HighestPrice);
	 root["LowestPrice"] = Json::Value(depthmarket.LowestPrice);
	 root["Volume"] = Json::Value(depthmarket.Volume);
	 root["Turnover"] = Json::Value(depthmarket.Turnover);
	 root["ClosePrice"] = Json::Value(depthmarket.ClosePrice);
	 root["SettlementPrice"] = Json::Value(depthmarket.SettlementPrice);

	 root["UpperLimitPrice"] = Json::Value(depthmarket.UpperLimitPrice);
	 root["LowerLimitPrice"] = Json::Value(depthmarket.LowerLimitPrice);


	 root["AveragePrice"] = Json::Value(depthmarket.AveragePrice);
	 root["ActionDay"] = Json::Value(depthmarket.ActionDay);
	 root["UpdateTime"] = Json::Value(depthmarket.UpdateTime);
	 root["UpdateMillisec"] = Json::Value(depthmarket.UpdateMillisec);

	 root["LongMarginRatio"] = Json::Value(depthmarket.LongMarginRatio);
	 root["ShortMarginRatio"] = Json::Value(depthmarket.ShortMarginRatio);
	 root["Margin"] = Json::Value(depthmarket.Margin);
	 root["VolumeMultiple"] = Json::Value(depthmarket.VolumeMultiple);
	 root["PriceTick"] = Json::Value(depthmarket.PriceTick);
	 root["Sedimentary"] = Json::Value(depthmarket.Sedimentary);
	 root["LastSedimentary"] = Json::Value(depthmarket.LastSedimentary);
	 root["SedimentaryIORate"] = Json::Value(depthmarket.SedimentaryIORate);
	 root["OpenPreSettleRate"] = Json::Value(depthmarket.OpenPreSettleRate);
	 root["LastOpenRate"] = Json::Value(depthmarket.LastOpenRate);
	 root["UpdownRate"] = Json::Value(depthmarket.UpdownRate);
	 root["AddOpenInterestRate"] = Json::Value(depthmarket.AddOpenInterestRate);

	 root["Section"] = Json::Value(depthmarket.Section);
	 root["DayNight"] = Json::Value(depthmarket.DayNight);

	 Json::StyledWriter styled_writer;
	 string value =styled_writer.write(root);
//	 std::cout << styled_writer.write(root) << std::endl;
//			 printf("----->value----> %s",value.c_str());
	 string key = depthmarket.InstrumentID;
//			 printf("----->key----> %s",key.c_str());

	 this->Set(key,value);

}

DepthMarket RedisDepthMarket::GetDepthMarket(std::string key)
{
	DepthMarket depthmarket;
	memset(&depthmarket,0,sizeof(depthmarket));
	std::string str = this->Get(key);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root))	{

		strcpy(depthmarket.InstrumentID, root["InstrumentID"].asString().c_str());

		string tradingday;
		tradingday=root["TradingDay"].asString();
		strcpy(depthmarket.TradingDay,tradingday.c_str());

		string exchangeid;
		exchangeid=root["ExchangeID"].asString();
		strcpy(depthmarket.ExchangeID,exchangeid.c_str());

		string productid;
		productid=root["ProductID"].asString();
		strcpy(depthmarket.ProductID,productid.c_str());

		string UpdateTime;
		UpdateTime=root["UpdateTime"].asString();

		strcpy(depthmarket.UpdateTime,UpdateTime.c_str());
	//							printf ("depthmarket.UpdateTime->%s  ------%s   --->%s    \n ",root["UpdateTime"].asString().c_str(),UpdateTime.c_str(),depthmarket.UpdateTime);

		depthmarket.LastPrice = root["LastPrice"].asDouble();

	//							printf ("depthmarket.LastPrice->%.3f  ------%s   --->%s    \n ",root["LastPrice"].asDouble(),UpdateTime.c_str(),depthmarket.UpdateTime);
		depthmarket.UpdateMillisec=root["UpdateMillisec"].asInt();
		depthmarket.AskPrice1 = root["AskPrice1"].asDouble();
		depthmarket.BidPrice1 = root["BidPrice1"].asDouble();
		depthmarket.PreSettlementPrice = root["PreSettlementPrice"].asDouble();
		depthmarket.PreClosePrice = root["PreClosePrice"].asDouble();
		depthmarket.OpenInterest = root["OpenInterest"].asDouble();
		depthmarket.PreOpenInterest = root["PreOpenInterest"].asDouble();

		depthmarket.OpenPrice = root["OpenPrice"].asDouble();
		depthmarket.HighestPrice = root["HighestPrice"].asDouble();
		depthmarket.LowestPrice = root["LowestPrice"].asDouble();
		depthmarket.Volume = root["Volume"].asDouble();
		depthmarket.Turnover = root["Turnover"].asDouble();
		depthmarket.ClosePrice = root["ClosePrice"].asDouble();
		depthmarket.SettlementPrice = root["SettlementPrice"].asDouble();

		depthmarket.UpperLimitPrice = root["UpperLimitPrice"].asDouble();
		depthmarket.LowerLimitPrice = root["LowerLimitPrice"].asDouble();

		depthmarket.AveragePrice = root["AveragePrice"].asDouble();

		depthmarket.LongMarginRatio = root["LongMarginRatio"].asDouble();
		depthmarket.ShortMarginRatio = root["ShortMarginRatio"].asDouble();
		depthmarket.Margin = root["Margin"].asDouble();
		depthmarket.VolumeMultiple = root["VolumeMultiple"].asInt();
		depthmarket.PriceTick = root["PriceTick"].asDouble();
		depthmarket.Sedimentary = root["Sedimentary"].asDouble();
		depthmarket.LastSedimentary = root["LastSedimentary"].asDouble();
		depthmarket.SedimentaryIORate = root["SedimentaryIORate"].asDouble();


		depthmarket.OpenPreSettleRate = root["OpenPreSettleRate"].asDouble();
		depthmarket.LastOpenRate = root["LastOpenRate"].asDouble();
		depthmarket.UpdownRate = root["UpdownRate"].asDouble();
		depthmarket.AddOpenInterestRate = root["AddOpenInterestRate"].asDouble();

		string actionday;
		actionday=root["ActionDay"].asString();
		strcpy(depthmarket.ActionDay,actionday.c_str());

		strcpy(depthmarket.Section,root["Section"].asString().c_str());
		strcpy(depthmarket.DayNight,root["DayNight"].asString().c_str());

	}


	return depthmarket ;

}



