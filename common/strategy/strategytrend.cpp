#include "strategytrend.hpp"


StrategyTrend::StrategyTrend(const char* strategyname) {
	this->SetStrategyName(strategyname);
}

void  StrategyTrend::Open_MACrossSignal()
{
	printf("Open_MACrossSignal  begin----------->  \n");
//	Signal signal;
	MysqlDayLine mysqldayline;
	MysqlHisMA mysqlhima;
	MysqlHisATR mysqlhisatr;
	MysqlInstrument mysqlinstrument;
//	MysqlTestReport mysqltestreport;
//	RedisDepthMarket redisdepthmarket;

	double stopprice;
	DateUtil dateutil;

	MysqlDepthMarket mysqldepthmarket;


	std::string trading_date;
	trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901

	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());


//	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	this->GetMysqlMaincontract()->Find_TradeSectionList(list);
	if(list.size()>0)
	{
		printf("Open_MACrossSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{


//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )  //判断当前交易时间是否是尾盘
			{

				char pinstrumentid[10];
				strcpy(pinstrumentid,item.InstrumentID_trade);
				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_macrossopen");
				signalname.append("_");
				signalname.append(this->GetUserID().c_str());

				int signalcount =GetSignalCount();

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价

				printf("	开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));

				bool valid_dayline = ValidCheck_DayLine(pinstrumentid,trading_date.c_str(),item.ExchangeID);
				MAData madata =this->GetMADatabyTradingDay(pinstrumentid,item.ExchangeID,depthmarket.LastPrice);
				MAData premadata =this->GetPreMADatabyTradingDay(pinstrumentid,item.ExchangeID);
				HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());

				double tr ;
				double tr_lowclose = abs(depthmarket.LowestPrice-depthmarket.PreClosePrice);
				double tr_highclose =abs(depthmarket.HighestPrice-depthmarket.PreClosePrice);
				double tr_highlow = abs(depthmarket.HighestPrice-depthmarket.LowestPrice);
				tr=max(tr_highlow,max(tr_lowclose,tr_highclose));


				bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice !=0 && depthmarket.OpenPrice!=0 && depthmarket.PreSettlementPrice!=0;
				bool valid5 = madata.MA50!=0 &&  madata.MA20!=0 &&  madata.MA10!=0 &&  madata.MA50_X!=90;
				bool valid6=premadata.MA50!=0 &&  premadata.MA20!=0 &&  premadata.MA10!=0 &&  premadata.MA50_X!=90;
				bool valid = valid_dayline  && valid2 &&valid3 &&valid4  &&valid5  &&valid6;

				if (valid)
				{
//					printf("查询周期价差表 -->  %s   %f  %f  \n",date.c_str(),cycledata.High20,cycledata.Low20);

//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold(this->GetUserID().c_str());
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

					int position =  GetPosition(pinstrumentid, depthmarket,tr);

//							printf("查询MA   %s -->  [%s]   %.3f  %.3f \n",pinstrumentID,tradingday.c_str(),ma60,ma50);

//					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sqltime.c_str());
					ATRData atr_data =mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),20);
					//多头行情
					bool longcond_ma = depthmarket.LastPrice > madata.MA10 && madata.MA10 > madata.MA20  && madata.MA20 >madata.MA50;
					bool longcond_prema = predaylinedata.Close > premadata.MA10 && premadata.MA10 > premadata.MA20  && premadata.MA20 >premadata.MA50;
					bool longcond_ma_kline = depthmarket.LastPrice > depthmarket.OpenPrice;
					bool longcond_ma_angle = madata.MA50_X >0;

					bool longcond_trend = longcond_ma && longcond_ma_kline && longcond_ma_angle;
					bool condition_long = longcond_trend && !longcond_prema;

					bool shortcond_ma =  depthmarket.LastPrice < madata.MA10 && madata.MA10 < madata.MA20  && madata.MA20 < madata.MA50;
					bool shortcond_prema =  predaylinedata.Close  < premadata.MA10 && premadata.MA10 < premadata.MA20  && premadata.MA20 < premadata.MA50;
					bool shortcond_ma_kline = depthmarket.LastPrice < depthmarket.OpenPrice;
					bool shortcond_ma_angle = madata.MA50_X<0;
					bool shortcond_trend = shortcond_ma &&  shortcond_ma_kline && shortcond_ma_angle;
					bool condition_short = shortcond_trend && !shortcond_prema;


					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

					double sedimentary;
					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;

					bool valid_sedimentary = sedimentary>2;
					//					if(opencond_testreport )
//					{
//					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
//					}
					if(longcond_trend  )
					{
						printf("	[大趋势多头] -->%s  \n",pinstrumentid);

					}
					else if(shortcond_trend )
					{
						printf("	[大趋势空头] -->%s  \n",pinstrumentid);
					}




					if (condition_long  && valid_sedimentary)
					{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	MA 交叉 开多 --> %s 现价:%.2f   MA10:%.2f   MA9:%.2f  MA50:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.LastPrice,madata.MA10,madata.MA20,madata.MA50,depthmarket.UpdateTime ,longposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (condition_short  && valid_sedimentary)
					{


//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	MA 交叉 开空 --> %s 现价:%.2f   MA4:%.2f   MA9:%.2f  MA50:%.2f  --->%s  空头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.LastPrice,madata.MA10,madata.MA20,madata.MA50,depthmarket.UpdateTime ,shortposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =1;
						int offset =0;
						OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);

					}
					else
					{
//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
						SignalDispearCheck(signalname);

					}


				}
				else
				{
//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);


						if (!valid2 || !valid3)
						{
							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
									pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);

						}

						if (!valid4)
						{
							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
						}



				}
			}


		}
	vector<MainContractData>().swap(list);

}
void  StrategyTrend::Open_CycleNSignal()
{
//	char logbuf[200];
//	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
//
////	printf("Open_CycleNSignal  begin-----------> \n");
//	DateUtil dateutil;
//	MysqlDayLine mysqldayline;
//
//	MysqlHold mysqlhold(this->GetUserID().c_str());
//
//	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());   //2017-09-01 00:00:00
//	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());
//
//
//	vector<MainContractData>  list;
//
//	this->GetMysqlMaincontract()->Find_CycleNList_Ndays(list,60);
//	if(list.size()>0){
//		printf("Open_CycleNSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}
//
//
//	for (auto &item:list){
//
////			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
////			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
//			char pinstrumentid[10];
//			strcpy(pinstrumentid,item.InstrumentID_trade);
//			bool valid_dayline = ValidCheck_DayLine(pinstrumentid,trading_date.c_str(),item.ExchangeID);
//
//
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)  ){
////			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )  //判断当前交易时间是否是尾盘
//
//				std::string signalname;
//				signalname=pinstrumentid;
//				signalname.append("_cyclenopen");
//				signalname.append("_");
//				signalname.append(this->GetUserID().c_str());
//
//				int signalcount =GetSignalCount();
//
//				char productid[31];
//				CodeUtil codeutil;
//				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());
//
//				DepthMarket depthmarket;
//
//				if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
//					depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
//				}
//
//
//				DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价
//
////				printf("	CycleN 开仓信号检测 ProductID -->  %s ---- %s   \n",productid,pinstrumentid);
//
//
////				HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());
//
//				double tr ;
//				double tr_lowclose = abs(depthmarket.LowestPrice-depthmarket.PreClosePrice);
//				double tr_highclose =abs(depthmarket.HighestPrice-depthmarket.PreClosePrice);
//				double tr_highlow = abs(depthmarket.HighestPrice-depthmarket.LowestPrice);
//				tr=max(tr_highlow,max(tr_lowclose,tr_highclose));
//
//				int n_long =20;
//				int n_short =10;
//				double atr20 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_long,tr);
//				double atr10 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_short,tr);
//
//				int n ;
//				double k ;
//				if(atr10 >0.1 && atr20 >0.1){
//					k = atr20/atr10;
//				}
//				else{
//					k=1.0;
//				}
//
//				n=floor(n_long*k);
//				CycleData cycledata = mysqldayline.Collect_CycleNData(pinstrumentid,tradingday_sqltime.c_str(),n);
//				HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());
//
//				ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),n_long);
//				ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),n_short);
//				int pre_n ;
//				double pre_k ;
//				if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0){
//					pre_k = preatr20_data.ATR/preatr10_data.ATR;
//				}
//				else	{
//					pre_k=1.0;
//				}
//				pre_n=floor(n_long*pre_k);
//				CycleData precycledata = mysqldayline.Collect_CycleNData(pinstrumentid,preday.Time,pre_n);
//
//				bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
//				bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
//				bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
//				bool valid4 = depthmarket.LastPrice >1.0 && depthmarket.OpenPrice>1.0 && depthmarket.PreSettlementPrice>1.0;
//				bool valid5 = atr10>0.1 && atr20 >0.1 && tr>0.1;
////				bool valid6=;
//				bool valid = valid_dayline  &&valid_redisdepth && valid2 &&valid3 &&valid4 && valid5;
//
//				if (valid)	{
////					printf("查询周期价差表 -->  %s   %f  %f  \n",date.c_str(),cycledata.High20,cycledata.Low20);
//
////					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
////					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sqltime.c_str());
//
//					//共用条件
//					bool commcondition_atr = tr >atr10 && atr10>atr20;
//
//					//多头行情
//					bool longcond_cycle = depthmarket.LastPrice >cycledata.High20;
//					bool longcond_precycle = preday.Close < precycledata.High20 ;
//					bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice  &&  depthmarket.OpenPrice > preday.Close;
//
//					bool condition_long = longcond_cycle && longcond_precycle && longcond_kline && commcondition_atr;
//
//					bool shortcond_cycle =  depthmarket.LastPrice < cycledata.Low20;
//					bool shortcond_precycle =  preday.Close  > precycledata.Low20;
//					bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice  &&  depthmarket.OpenPrice < preday.Close;
//
//					bool condition_short = shortcond_cycle && shortcond_precycle && shortcond_kline && commcondition_atr;
//
//					InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);
//
//					double sedimentary;
//					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;
//
//					bool valid_sedimentary = sedimentary>2;
//					//					if(opencond_testreport )
////					{
////					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
////					}
//
//					if(longcond_cycle  ){
//						printf("	1.[CycleN当日趋势突破多头--周期%d] -->%s    现价: %.3f  最高价: %.3f \n",n,pinstrumentid,depthmarket.LastPrice,cycledata.High20);
//							if(longcond_precycle){
//								printf("	2.[CycleN前日趋势突破多头] -->%s  \n",pinstrumentid);
//								if(commcondition_atr){
//									printf("	3.[ATR 数据满足条件] \n");
//								}
//							}
//					}
//					else if(shortcond_cycle )	{
//						printf("	1.[CycleN当日趋势突破空头--周期%d] -->%s    现价: %.3f  最低价: %.3f \n",n,pinstrumentid,depthmarket.LastPrice,cycledata.Low20);
//						if(shortcond_precycle){
//							printf("	2.[CycleN前日趋势突破空头] -->%s  \n",pinstrumentid);
//							if(commcondition_atr){
//								printf("	3.[ATR 数据满足条件] \n");
//							}
//						}
//					}
//
//
//					if (condition_long  && valid_sedimentary  )		{
////						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
////								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
//
////						HoldData holddata;
////						strcpy(holddata.InstrumentID,pinstrumentid);
////						strcpy(holddata.ExchangeID,item.ExchangeID);
////						holddata.Volume =position-longposition;
////						int direction =0;
////						int offset =0;
////						OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);
//
//						int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
////						int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
//
//						int section_hold = mysqlhold.Count_AllListsbySection(item.Section);
//
//						int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);
//
//						printf("查询section_hold   %s -->  [%s]   %d \n",pinstrumentid,item.Section,section_hold);
//						bool sectionnum =this->ValidCheck_SectionPosition(item.Section);
//						if(sectionnum){
//							printf("	CycleN 开多 --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//										pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//
//							sprintf(logbuf,"	CycleN 开多 信号[%s] --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//									signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//							logutil.WriteLog(logbuf);
//							Signal signal;
//
//							strcpy(signal.InstrumentID,item.InstrumentID);
//							strcpy(signal.SingalName,signalname.c_str());
//
//							signal.Volume = position-longposition;
//							signal.MaxSignalCount =signalcount;
//							signal.Direction=0;
//							signal.Offset=0;
//							signal.Price=depthmarket.LastPrice;
//							signal.StopRange =atr20;
//
//							OpenSignalUpdate(&signal);
//						}
//						else{
//							sprintf(logbuf,"	仓位控制:CycleN 开多 信号[%s]未激活 --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//								signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//
//							logutil.WriteLog(logbuf);
//						}
//
//					}
//					else if (condition_short  && valid_sedimentary  )	{
//
////						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
////								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
////						HoldData holddata;
////						strcpy(holddata.InstrumentID,pinstrumentid);
////						strcpy(holddata.ExchangeID,item.ExchangeID);
////						holddata.Volume =position-shortposition;
////						int direction =1;
////						int offset =0;
////						OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);
//
////						int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
//						int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
//
//						int section_hold = mysqlhold.Count_AllListsbySection(item.Section);
//
//						int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);
//
//						printf("查询section_hold   %s -->  [%s]   %d \n",pinstrumentid,item.Section,section_hold);
//						bool sectionnum =this->ValidCheck_SectionPosition(item.Section);
//
//						if(sectionnum){
//							printf("	CycleN 开空 --> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
//								pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);
//
//							sprintf(logbuf,"	CycleN 开空 信号[%s]--> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
//									signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);
//							logutil.WriteLog(logbuf);
//
//							Signal signal;
//
//							strcpy(signal.InstrumentID,item.InstrumentID);
//							strcpy(signal.SingalName,signalname.c_str());
//
//							signal.Volume = position-shortposition;
//							signal.MaxSignalCount =signalcount;
//							signal.Direction=1;
//							signal.Offset=0;
//							signal.Price=depthmarket.LastPrice;
//							signal.StopRange =atr20;
//
//							OpenSignalUpdate(&signal);
//						}
//						else{
//							sprintf(logbuf,"	仓位控制:CycleN 开空 信号[%s]未激活--> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
//								signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);
//
//							logutil.WriteLog(logbuf);
//						}
//					}
//					else{
////						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
//						SignalDispearCheck(signalname);
//					}
//				}
//				else	{
////						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);
//
//						if(!valid_redisdepth){
//							if(strcmp(item.DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//			//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
//								//正常情况 白盘合约在夜盘无数据
//							}
//							else {
//								printf("	请检查行情是否接收正常？ %s    \n",pinstrumentid);
//							}
//						}
//						if (!valid2 || !valid3)		{
//							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//									pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//						}
//
//						if (!valid4)	{
//							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
//						}
//				}
//			}
//		}
//	vector<MainContractData>().swap(list);

}

//在cyclen的基础上 增加 远月合约 近月合约价差 升水 贴水 信号参考
void  StrategyTrend::Open_CycleN_NewSignal()
{
//	char logbuf[1024];
//	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
//
////	printf("Open_CycleNSignal  begin-----------> \n");
//	DateUtil dateutil;
//	MysqlDayLine mysqldayline;
//
//	MysqlHold mysqlhold(this->GetUserID().c_str());
//
//	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());   //2017-09-01 00:00:00
//	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());
//
//	vector<MainContractData>  list;
//	this->GetMysqlMaincontract()->Find_SpreadListorderbySed_Ndays(list,20);
//	if(list.size()>0){
//		printf("Open_CycleN_NewSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}
//
//	for (auto &item:list){
//
////			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
////			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
//			char pinstrumentid[10];
//			strcpy(pinstrumentid,item.InstrumentID);
//
//			bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,pinstrumentid,trading_date.c_str(),item.ExchangeID,60);
//			bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
//			bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
//
//			bool valid_dayline_next = ValidCheck_DayLineNdays(&mysqldayline,item.InstrumentID_next,trading_date.c_str(),item.ExchangeID,20);
//			bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item.InstrumentID_next);
//			bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item.InstrumentID_next)> 0 ;
//
//			bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID);
////			bool closetime = dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec()) ;//判断当前交易时间是否是尾盘
//
//			if ( valid_dayline &&  valid_dayline_next && valid_mysqldepthmarket && valid_mysqldepthmarket_next
//					&& valid_redisdepth  && valid_redisdepth_next && opentime ){
//				std::string signalname;
//				signalname=pinstrumentid;
//				signalname.append("_cyclennewopen");
//				signalname.append("_");
//				signalname.append(this->GetUserID().c_str());
//
//				int signalcount =GetSignalCount();
//
//				char productid[10];
//				CodeUtil codeutil;
//				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());
//
//				DepthMarket depthmarket;
//				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
//
//				DepthMarket depthmarket_next;
//				depthmarket_next = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID_next);
//
//				// 采集当天的价格 涨跌停限制价
//				DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(pinstrumentid);
//				DepthMarketData data_next=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID_next);
//
////				printf("	CycleN 开仓信号检测 ProductID -->  %s ---- %s   \n",productid,pinstrumentid);
////				HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());
//				double dif = depthmarket.LastPrice - depthmarket_next.LastPrice ;
//				int ret_dif= CheckDifPriceByNextDayline(&mysqldayline,pinstrumentid,item.InstrumentID_next,20,dif);
//
//				double tr =GetCurrentDayTR(&depthmarket);
//
//				int n_long =20;
//				int n_short =10;
//				double atr20 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_long,tr);
//				double atr10 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_short,tr);
//
//				int n ;
//				double k ;
//				if(atr10 >0.1 && atr20 >0.1){
//					k = atr20/atr10;
//				}
//				else{
//					k=1.0;
//				}
//
//				n=floor(n_long*k);
//				CycleData cycledata = mysqldayline.Collect_CycleNData(pinstrumentid,tradingday_sqltime.c_str(),n);
//				HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());
//
//				ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),n_long);
//				ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),n_short);
//				int pre_n ;
//				double pre_k ;
//				if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0){
//					pre_k = preatr20_data.ATR/preatr10_data.ATR;
//				}
//				else	{
//					pre_k=1.0;
//				}
//				pre_n=floor(n_long*pre_k);
//				CycleData precycledata = mysqldayline.Collect_CycleNData(pinstrumentid,preday.Time,pre_n);
//
//
//				bool valid2 = depthmarket.LastPrice <=data.UpperLimitPrice && depthmarket.LastPrice >=data.LowerLimitPrice;
//				bool valid3 = depthmarket_next.LastPrice <= data_next.UpperLimitPrice &&
//						depthmarket_next.LastPrice >= data_next.LowerLimitPrice;
//				bool valid4 = depthmarket.LastPrice >1.0 && depthmarket.OpenPrice>1.0 && depthmarket.PreSettlementPrice>1.0;
//				bool valid5 = depthmarket_next.LastPrice >1.0 && depthmarket_next.OpenPrice>1.0 &&
//						depthmarket_next.PreSettlementPrice>1.0;
//				bool valid6 = atr10>0.1 && atr20 >0.1 && tr>0.1;
//
//				bool valid = valid2 &&valid3 &&valid4 && valid5 && valid6;
//
//				if (valid)	{
////					printf("查询周期价差表 -->  %s   %f  %f  \n",date.c_str(),cycledata.High20,cycledata.Low20);
//
////					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
////					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sqltime.c_str());
//
//					//共用条件
//					bool commcondition_atr = tr >atr10 && atr10>atr20;
//
//					//多头行情
//					bool longcond_cycle = depthmarket.LastPrice >cycledata.High20;
//					bool longcond_precycle = preday.Close < precycledata.High20 ;
//					bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice  &&  depthmarket.OpenPrice > preday.Close;
//					bool longcond_dif = ret_dif>0;
//
//					bool condition_long = longcond_cycle && longcond_precycle && longcond_kline && commcondition_atr && longcond_dif;
//
//					bool shortcond_cycle =  depthmarket.LastPrice < cycledata.Low20;
//					bool shortcond_precycle =  preday.Close  > precycledata.Low20;
//					bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice  &&  depthmarket.OpenPrice < preday.Close;
//					bool shortcond_dif = ret_dif<0;
//
//					bool condition_short = shortcond_cycle && shortcond_precycle && shortcond_kline && commcondition_atr && shortcond_dif;
//
//					InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);
//
//					double sedimentary;
//					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;
//
//					bool valid_sedimentary = sedimentary>2;
//					//					if(opencond_testreport )
////					{
////					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
////					}
//
//					if(longcond_cycle  ){
//						printf("	4-1.[CycleN当日趋势突破多头--周期%d] -->%s    现价: %.3f  最高价: %.3f \n",n,pinstrumentid,depthmarket.LastPrice,cycledata.High20);
//							if(longcond_precycle){
//								printf("	4-2.[CycleN前日趋势突破多头] -->%s  \n",pinstrumentid);
//								if(commcondition_atr){
//									printf("	4-3.[ATR 数据满足条件]  ----> %s  \n",pinstrumentid);
//									if(longcond_dif){
//										printf("	4-4.[基差 多头数据满足条件] ----> %s  %d \n",pinstrumentid,ret_dif);
//									}
//								}
//							}
//					}
//					else if(shortcond_cycle )	{
//						printf("	4-1.[CycleN当日趋势突破空头--周期%d] -->%s    现价: %.3f  最低价: %.3f \n",n,pinstrumentid,depthmarket.LastPrice,cycledata.Low20);
//						if(shortcond_precycle){
//							printf("	4-2.[CycleN前日趋势突破空头] -->%s  \n",pinstrumentid);
//							if(commcondition_atr){
//								printf("	4-3.[ATR 数据满足条件] -->%s  \n",pinstrumentid);
//								if(shortcond_dif){
//									printf("	4-4.[基差 空头数据满足条件] ----->%s  %d \n",pinstrumentid ,ret_dif);
//								}
//							}
//						}
//					}
//
//
//					if (condition_long  && valid_sedimentary  )		{
//
//						int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
////						int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
//
//						int section_hold = mysqlhold.Count_AllListsbySection(item.Section);
//						int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);
//
//						printf("查询section_hold   %s -->  [%s]   %d \n",pinstrumentid,item.Section,section_hold);
//						bool sectionnum =this->ValidCheck_SectionPosition(item.Section);
//						if(sectionnum){
//							printf("	CycleN 开多 --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//										pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//
//							sprintf(logbuf,"StrategyTrend::Open_CycleN_NewSignal	CycleN 开多 信号[%s] --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//									signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//							logutil.WriteLog(logbuf);
//							Signal signal;
//
//							strcpy(signal.InstrumentID,item.InstrumentID);
//							strcpy(signal.SingalName,signalname.c_str());
//
//							signal.Volume = position-longposition;
//							signal.MaxSignalCount =signalcount;
//							signal.Direction=0;
//							signal.Offset=0;
//							signal.Price=depthmarket.LastPrice;
//							signal.StopRange =atr20;
//
//							OpenSignalUpdate(&signal);
//						}
//						else{
//							sprintf(logbuf,"StrategyTrend::Open_CycleN_NewSignal	仓位控制:CycleN 开多 信号[%s]未激活 --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//								signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//
//							logutil.WriteLog(logbuf);
//						}
//
//					}
//					else if (condition_short  && valid_sedimentary  )	{
//
//						int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
//						int section_hold = mysqlhold.Count_AllListsbySection(item.Section);
//						int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);
//
//						printf("查询section_hold   %s -->  [%s]   %d \n",pinstrumentid,item.Section,section_hold);
//						bool sectionnum =this->ValidCheck_SectionPosition(item.Section);
//
//						if(sectionnum){
//							printf("	CycleN 开空 --> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
//								pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);
//
//							sprintf(logbuf,"StrategyTrend::Open_CycleN_NewSignal	CycleN 开空 信号[%s]--> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
//									signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);
//							logutil.WriteLog(logbuf);
//
//							Signal signal;
//
//							strcpy(signal.InstrumentID,item.InstrumentID);
//							strcpy(signal.SingalName,signalname.c_str());
//
//							signal.Volume = position-shortposition;
//							signal.MaxSignalCount =signalcount;
//							signal.Direction=1;
//							signal.Offset=0;
//							signal.Price=depthmarket.LastPrice;
//							signal.StopRange =atr20;
//
//							OpenSignalUpdate(&signal);
//						}
//						else{
//							sprintf(logbuf,"StrategyTrend::Open_CycleN_NewSignal	仓位控制:CycleN 开空 信号[%s]未激活--> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
//								signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);
//
//							logutil.WriteLog(logbuf);
//						}
//					}
//					else{
////						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
//						SignalDispearCheck(signalname);
//					}
//				}
//				else	{
////						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);
//						if (!valid2 )		{
//							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//									pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//						}
//						if (!valid3)		{
//							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//									item.InstrumentID_next,depthmarket_next.LastPrice,data_next.UpperLimitPrice,data_next.LowerLimitPrice);
//						}
//
//						if (!valid4)	{
//							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
//						}
//						if (!valid5)	{
//							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",item.InstrumentID_next,depthmarket_next.LastPrice);
//						}
//				}
//			}
//			else {
//				if(!valid_redisdepth){
//					if(strcmp(item.DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//	//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
//						//正常情况 白盘合约在夜盘无数据
//					}
//					else {
//						printf("	请检查行情是否接收正常？ %s    \n",pinstrumentid);
//					}
//
//				}
//
//				if(!valid_redisdepth_next){
//
//					if(strcmp(item.DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//	//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",item.InstrumentID_next);
//						//正常情况 白盘合约在夜盘无数据
//					}
//					else {
//						printf("	请检查行情是否接收正常？ %s    \n",item.InstrumentID_next);
//					}
//
//				}
//
//				if(!valid_mysqldepthmarket_next){
//					printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",pinstrumentid);
//				}
//
//				if(!valid_mysqldepthmarket_next){
//					printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",item.InstrumentID_next);
//				}
//
//				if(!valid_dayline){
//					printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
//				}
//
//				if(!valid_dayline_next){
//					printf("	请检查 %s 日线数据是否合规！     \n",item.InstrumentID_next);
//				}
//			}
//		}
//	vector<MainContractData>().swap(list);
}


void StrategyTrend::Hold_MA50Reverse_SignalCheck()
{
	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;
	double stopprice;

	std::string tradingdate;
	tradingdate = this->GetTradingDay();   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00


	DateUtil dateutil;

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("MA趋势 短周期反转监测---->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
			char pinstrumentid[10];
			strcpy(pinstrumentid,item.InstrumentID);

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_ma50reverse");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());
			int signalcount=10;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())  )
			{

				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentid),sizeof(pinstrumentid));


				int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);

				MAData madata =this->GetMADatabyTradingDay(pinstrumentid,item.ExchangeID,depthmarket.LastPrice);
	//						printf("数据准备 --> %s  --->%d  \n",pinstrumentID,count);


				bool valid =count>59;
				bool valid2 =  depthmarket.LastPrice >depthmarket.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;
				if ( valid && valid2 && valid3&& item.Volume>0)
				{
	//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );


					MysqlHold mysqlhold(this->GetUserID().c_str());
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

					//多头行情
					bool longcond = depthmarket.LastPrice > madata.MA50;
					bool shortcond = depthmarket.LastPrice < madata.MA50;


	//							printf("查询MA -->  %s   %.3f  %.3f %.3f \n",pinstrumentID,ma50,ma20,ma10);
					if ( longcond  && item.Direction ==1  )
					{

	//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
	//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
					printf("空头合约趋势转多头  --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f 止损价: %.2f  ma50:%.3f  涨幅： %.2f%% --->%s  持仓: %d  \n",
							pinstrumentid,depthmarket.PreSettlementPrice ,
							depthmarket.OpenPrice,depthmarket.LastPrice,stopprice,madata.MA50,
							depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition);
//					int offset =1;
//					SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

						Signal signal;

						strcpy(signal.InstrumentID,item.InstrumentID);
						strcpy(signal.TradingDay,this->GetTradingDay().c_str());
						strcpy(signal.SingalName,signalname.c_str());

						signal.Volume = item.Volume;
						signal.MaxSignalCount =signalcount;
						signal.Direction=item.Direction;
						signal.Offset=1;
						signal.Price=depthmarket.LastPrice;
						signal.StopPrice=item.StopPrice;
						CloseSignalUpdate(&signal);
					}
					else if (shortcond && item.Direction ==0 )
					{
		//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
		//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
							printf("多头合约趋势转空头 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f    ma50:%.3f 跌幅： %.2f%% ---->%s  持仓: %d  \n",
									pinstrumentid,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice ,madata.MA50,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition);
//							int offset =1;
//							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);
							Signal signal;

							strcpy(signal.InstrumentID,item.InstrumentID);
							strcpy(signal.TradingDay,this->GetTradingDay().c_str());
							strcpy(signal.SingalName,signalname.c_str());

							signal.Volume = item.Volume;
							signal.MaxSignalCount =signalcount;
							signal.Direction=item.Direction;
							signal.Offset=1;
							signal.Price=depthmarket.LastPrice;
							signal.StopPrice=item.StopPrice;
							CloseSignalUpdate(&signal);
					}
					else
					{
	//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);

						SignalDispearCheck(signalname);

					}


				}
				else
				{
	//						printf("合约: %s  --> 返回数据为空！！！！  %s \n",pinstrumentID[i],date.c_str());
				}
			}


		}
	vector<HoldData>().swap(list);

}
