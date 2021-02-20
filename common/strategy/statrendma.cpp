#include "statrendma.hpp"


StaTrendMA::StaTrendMA(const char* strategyname) {
	this->SetStrategyName(strategyname);
}

void  StaTrendMA::Open_MACrossSignal()
{
	printf("Open_MACrossSignal   \n");
	Signal signal;
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


	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_TradeSectionList(list);
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
void  StaTrendMA::Open_CycleNSignal()
{
	printf("Open_CycleNSignal   \n");
	Signal signal;
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

	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_CycleNList(list);
	if(list.size()>0)
	{
		printf("Open_CycleNSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{

//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
			char pinstrumentid[10];
			strcpy(pinstrumentid,item.InstrumentID_trade);
			bool valid_dayline = ValidCheck_DayLine(pinstrumentid,trading_date.c_str(),item.ExchangeID);

			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )  //判断当前交易时间是否是尾盘
			{



				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_cyclenopen");
				int signalcount =GetSignalCount();

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价

				printf("	CycleN 开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));


				HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());

				double tr ;
								double tr_lowclose = abs(depthmarket.LowestPrice-depthmarket.PreClosePrice);
								double tr_highclose =abs(depthmarket.HighestPrice-depthmarket.PreClosePrice);
								double tr_highlow = abs(depthmarket.HighestPrice-depthmarket.LowestPrice);
								tr=max(tr_highlow,max(tr_lowclose,tr_highclose));

				int n_long =20;
				int n_short =10;
				double atr20 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_long,depthmarket.LastPrice);
				double atr10 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_short,depthmarket.LastPrice);

				int n ;
				double k ;
				if(atr10 >1.0 && atr20 >1.0)
				{
					k = atr20/atr10;
				}
				else
				{
					k=1.0;
				}

				n=floor(n_long*k);
				CycleData cycledata = mysqldayline.Collect_CycleNData(pinstrumentid,tradingday_sqltime.c_str(),n);
				HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());

				ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,preday.Time,n_long);
				ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,preday.Time,n_short);
				int pre_n ;
				double pre_k ;
				if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0)
				{
					pre_k = preatr20_data.ATR/preatr10_data.ATR;
				}
				else
				{
					pre_k=1.0;
				}
				pre_n=floor(n_long*pre_k);
				CycleData precycledata = mysqldayline.Collect_CycleNData(pinstrumentid,preday.Time,pre_n);

				bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice !=0 && depthmarket.OpenPrice!=0 && depthmarket.PreSettlementPrice!=0;
				bool valid5 = atr10>1.0 && atr20 >1.0 && tr>1.0;
//				bool valid6=;
				bool valid = valid_dayline  && valid2 &&valid3 &&valid4 && valid5;

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

					//共用条件
					bool commcondition_atr = tr >atr10 && atr10>atr20;

					//多头行情
					bool longcond_cycle = depthmarket.LastPrice >cycledata.High20;
					bool longcond_precycle = predaylinedata.Close < precycledata.High20 ;
					bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice;

					bool condition_long = longcond_cycle && longcond_precycle && longcond_kline && commcondition_atr;

					bool shortcond_cycle =  depthmarket.LastPrice < cycledata.Low20;
					bool shortcond_precycle =  predaylinedata.Close  > precycledata.Low20;
					bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice;

					bool condition_short = shortcond_cycle && shortcond_precycle && shortcond_kline && commcondition_atr;


					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

					double sedimentary;
					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;

					bool valid_sedimentary = sedimentary>2;
					//					if(opencond_testreport )
//					{
//					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
//					}
					if(condition_long  )
					{
						printf("	[CycleN大趋势多头] -->%s  \n",pinstrumentid);

					}
					else if(condition_short )
					{
						printf("	[CycleN大趋势空头] -->%s  \n",pinstrumentid);
					}

					if (condition_long  && valid_sedimentary)
					{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	CycleN 开多 --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);

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
						printf("	CycleN 开空 --> %s 现价:%.2f   区间最低价:%.2f   --->%s  空头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.LastPrice,cycledata.Low20,depthmarket.UpdateTime ,shortposition,position);

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

void StaTrendMA::Hold_MA50Reverse_SignalCheck()
{
	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;
	double stopprice;

//	TradingDayData tradingdaydata=this->GetTradingDayData();

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
			int signalcount=60;

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
					int offset =1;
					SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

					}
					else if (shortcond && item.Direction ==0 )
					{
		//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
		//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
							printf("多头合约趋势转空头 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f    ma50:%.3f 跌幅： %.2f%% ---->%s  持仓: %d  \n",
									pinstrumentid,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice ,madata.MA50,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition);
							int offset =1;
							SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);
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
