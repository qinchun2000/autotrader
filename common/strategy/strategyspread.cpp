#include "strategyspread.hpp"


StrategySpread::StrategySpread(const char* strategyname) {
	this->SetStrategyName(strategyname);
}

void  StrategySpread::Open_CalendarSpreadSignal()
{
//	printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->  \n");

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	MysqlDayLine mysqldayline;
	MysqlInstrument mysqlinstrument;
	MysqlDepthMarket mysqldepthmarket;

	DateUtil dateutil;
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());


//	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	bool day=dateutil.CheckSHFEDayTime(this->GetDifSec());
	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());

	if(day){
		this->GetMysqlMaincontract()->Find_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("	StrategySpread::Open_CalendarSpreadSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
	}
	else if(night){
		this->GetMysqlMaincontract()->Find_Night_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("	StrategySpread::Open_CalendarSpreadSignal----->夜盘主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
	}

	for (auto &item:list)	{

//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日

		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )

		char pinstrumentid[10];
		strcpy(pinstrumentid,item.InstrumentID);
		bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,pinstrumentid,trading_date.c_str(),item.ExchangeID,20);
		bool valid_dayline_next = ValidCheck_DayLineNdays(&mysqldayline,item.InstrumentID_next,trading_date.c_str(),item.ExchangeID,20);
		bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
		bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item.InstrumentID_next);
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item.InstrumentID_next)> 0 ;
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID);
//			bool closetime = dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec()) ;//判断当前交易时间是否是尾盘

		if (valid_dayline &&  valid_dayline_next && valid_mysqldepthmarket && valid_mysqldepthmarket_next
				&& valid_redisdepth  && valid_redisdepth_next && opentime )	{

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_spread");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			int signalcount =GetSignalCount();

			char productid[31];
			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

			DepthMarket depthmarket;

			if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
			}

			DepthMarket depthmarket_next;
			if( this->GetRedisDepthMarket()->Exists(item.InstrumentID_next)> 0 ){
				depthmarket_next = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID_next);
			}

			double dif = depthmarket.LastPrice - depthmarket_next.LastPrice ;
			int ret_dif= CheckDifPriceByNextDayline(&mysqldayline,pinstrumentid,item.InstrumentID_next,20,dif);

			DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价
			DepthMarketData data_next=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID_next);   // 采集当天的价格 涨跌停限制价

	//				printf("	开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));

			HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());


			bool valid2 = depthmarket.LastPrice <=data.UpperLimitPrice && depthmarket.LastPrice >=data.LowerLimitPrice;
			bool valid3 = depthmarket_next.LastPrice <= data_next.UpperLimitPrice &&
					depthmarket_next.LastPrice >= data_next.LowerLimitPrice;
			bool valid4 = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid5 = this->ValidCheck_DepthMarket(&depthmarket_next);

			bool valid = valid2 &&valid3 &&valid4 && valid5;

			if (valid)	{
	//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

				MysqlHold mysqlhold(this->GetUserID().c_str());
				int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
				int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
	//							printf("查询MA   %s -->  [%s]   %.3f  %.3f \n",pinstrumentID,tradingday.c_str(),ma60,ma50);

//				ATRData atr_data =mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),20);

				double tr =this->GetCurrentDayTR(&depthmarket);
				double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),20,tr);


				bool valid_stoprange=false;
				if(atr20 <depthmarket.LastPrice*0.05 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
				}
				bool valid_section_long =false;
				bool valid_section_short =false;
				if(this->GetRedisSection()->Exists(item.Section) >0){
					SectionInfo sectioninfo;
					sectioninfo =this->GetRedisSection()->GetSection(item.Section);
					if(sectioninfo.CountUprate >0.80){
						valid_section_long=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item.Section);
					}

					if(sectioninfo.CountDownrate >0.80){
						valid_section_short=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item.Section);
					}
				}

				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice && depthmarket.OpenPrice > depthmarket.PreSettlementPrice;
				bool longcond_dif = ret_dif >0;
				bool condition_long = longcond_spread && longcond_dif && longcond_kline && !valid_section_short;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;
				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice && depthmarket.OpenPrice < depthmarket.PreSettlementPrice;
				bool shortcond_dif = ret_dif <0;
				bool condition_short = shortcond_spread && shortcond_dif && shortcond_kline && !valid_section_long;

				InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

				double sedimentary;
				sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = sedimentary>2;

				if(longcond_spread  && !valid_section_short)	{

					if(longcond_dif){
						printf("	3-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	3-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	3-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
						if(longcond_kline){
							printf("	3-3 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  \n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice );
						}
					}
				}
				else if(shortcond_spread && !valid_section_long )		{

					if(shortcond_dif){
						printf("	3-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	3-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	3-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
						if(shortcond_kline){
							printf("	3-3 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  \n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice);
						}
					}
				}
				double avalible = 45000.0;
				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);


					int position =  this->GetPositionByAvalible(&mysqldayline,avalible,pinstrumentid, depthmarket,tr);

					if(position==0){
						printf("	StrategySpread::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
						sprintf(logbuf,"StrategySpread::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
								signalname.c_str(),position);
						logutil.WriteLog(logbuf);
					}
					else {

						printf("	StrategySpread::Open_CalendarSpreadSignal 开多 --> %s 现价:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
												pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,longposition,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item.InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =signalcount;
					signal.Direction=0;
					signal.Offset=0;
					signal.Price=depthmarket.LastPrice;
					signal.StopRange =atr20;

					OpenSignalUpdate(&signal);

				}
				else if (condition_short  && valid_sedimentary && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalible(&mysqldayline,avalible,pinstrumentid, depthmarket,tr);
					if(position==0){
						printf("	StrategySpread::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
						sprintf(logbuf,"StrategySpread::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
								signalname.c_str(),position);
						logutil.WriteLog(logbuf);
					}
					else {
					printf("	StrategySpread::Open_CalendarSpreadSignal 开空 --> %s 现价:%.2f    --->%s  空头持仓: %d 可开仓位: %d \n",
												pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,shortposition,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item.InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position-shortposition;
					signal.MaxSignalCount =signalcount;
					signal.Direction=1;
					signal.Offset=0;
					signal.Price=depthmarket.LastPrice;
					signal.StopRange =atr20;

					OpenSignalUpdate(&signal);

				}
				else	{
//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
					SignalDispearCheck(signalname);

					if(!valid_stoprange){
						printf("	StrategySpread::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
								pinstrumentid,atr20);
						sprintf(logbuf,"	StrategySpread::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
														pinstrumentid,atr20);
						logutil.WriteLog(logbuf);
					}
					if(!valid_sedimentary){
						printf("	StrategySpread::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
								pinstrumentid,sedimentary);
						sprintf(logbuf,"	StrategySpread::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
														pinstrumentid,sedimentary);
						logutil.WriteLog(logbuf);
					}

				}


			}
			else{
	//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);

				if (!valid2 )		{
					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
							pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
				}
				if (!valid3)		{
					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
							item.InstrumentID_next,depthmarket_next.LastPrice,data_next.UpperLimitPrice,data_next.LowerLimitPrice);
				}



			}
		}
		else{

			if(!valid_redisdepth){
				if(strcmp(item.DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
					//正常情况 白盘合约在夜盘无数据
				}
				else {
					printf("	无行情数据,请检查行情是否接收正常？ %s    \n",pinstrumentid);
				}

			}

			if(!valid_redisdepth_next){

				if(strcmp(item.DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",item.InstrumentID_next);
					//正常情况 白盘合约在夜盘无数据
				}
				else {
					printf("	无行情数据,请检查行情是否接收正常？ %s    \n",item.InstrumentID_next);
				}

			}

			if(!valid_mysqldepthmarket_next){
				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",pinstrumentid);
			}

			if(!valid_mysqldepthmarket_next){
				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",item.InstrumentID_next);
			}

			if(!valid_dayline){
				printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
			}

			if(!valid_dayline_next){
				printf("	请检查 %s 日线数据是否合规！     \n",item.InstrumentID_next);
			}
		}
	}
	vector<MainContractData>().swap(list);

}

