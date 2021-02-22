#include "staday.hpp"


StaDay::StaDay() {

	this->SetMysqlMindata(&_mysqlmindata);
}

void StaDay::SetMysqlMindata(MysqlMinData * pmysqlmindata)
{
	this->_pmysqlmindata=pmysqlmindata;
}

MysqlMinData * StaDay::GetMysqlMindata()
{
return this->_pmysqlmindata;
}

void StaDay::SetMinTableName(const char* table)
{
	this->_min_tablename=table;
}

std::string StaDay::GetMinTableName()
{
	return this->_min_tablename;
}

void  StaDay::DayTrade_Open_BigVolume()
{
	Signal signal;
	MysqlDayLine mysqldayline;
	MysqlMinData mysqlmindata;
	MysqlInstrument mysqlinstrument;
//	double stopprice;
	DateUtil dateutil;

	MysqlDepthMarket mysqldepthmarket;
	std::string trading_date;
	trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901


	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_TradeSectionList(list);
	if(list.size()>0){
		printf("开仓策略: 成交量活跃合约----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)	{
//		printf("	Open_BigVolume begin \n ");

//			printf("Open_BigVolume----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)  && item.Sedimentary>5.0)  //判断当前交易日是否是假日
			{
				char pinstrumentid[40];
				strcpy(pinstrumentid,item.InstrumentID_trade);

				printf("	开仓策略--监测合约:  %s ----> %s\n",pinstrumentid,dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_bigvolumeopen");

				int signalcount =GetSignalCount();

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
				DepthMarketData depth_sqldata=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价

				bool valid2 = depthmarket.LastPrice >=depth_sqldata.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice <= depth_sqldata.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice >1.0 && depthmarket.OpenPrice> 1.0 && depthmarket.PreSettlementPrice>1.0;

				bool valid =  valid2 &&valid3 &&valid4  ;

				SetMinTableName(pinstrumentid);
				this->GetMysqlMindata()->Set_Database("minute");
				this->GetMysqlMindata()->Set_Table(this->GetMinTableName());

				if (valid && this->GetMysqlMindata()->Exist_Table(this->GetMinTableName().c_str()))	{

					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
					int bigvolume;

					Mysql_MinData mindata = this->GetMysqlMindata()->Find_LastDatabyMinuteTime(this->GetMinTableName().c_str());
					double amount_minute = mindata.Volume *mindata.ClosePrice *insdata.VolumeMultiple*insdata.LongMarginRatio/10000000.0;

					bigvolume=Test(&depthmarket,&insdata);
					double rate=amount_minute/item.Sedimentary;  //分钟内交易金额 占总沉淀资金比例

					if(bigvolume>=0)	{
						printf("	开仓信号检测 ProductID  方向 [%d] -->  %s  %s  成交量: %d  成交额: %.2f  沉淀资金: %.2f   rate: %.2f%%\n",bigvolume,pinstrumentid,
													mindata.MinuteTime,mindata.Volume,amount_minute,item.Sedimentary,rate*100);
					}

//					int position =  GetPosition(pinstrumentid, depthmarket,item.Section);
					int position=1;
					//多头行情

					bool condition_long = bigvolume==0  ;
					bool condition_short =bigvolume==1  ;

					if (condition_long ){
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);


						MysqlHold mysqlhold(this->GetUserID().c_str());

						int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
						printf("	短期成交量活跃 开多 --> %s 现价:%.2f     --->%s  多头持仓: %d  可开仓位: %d \n",
										pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,longposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (condition_short )	{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

						MysqlHold mysqlhold(this->GetUserID().c_str());
						int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
						printf("	短期成交量活跃 开空 --> %s 现价:%.2f    --->%s  空头持仓: %d 可开仓位: %d \n",
									pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,shortposition,position);
						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =1;
						int offset =0;
						OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);

					}
					else{
//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
						SignalDispearCheck(signalname);

					}


				}
				else		{
//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);


						if (!valid2 || !valid3)	{
							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
									pinstrumentid,depthmarket.LastPrice,depth_sqldata.UpperLimitPrice,depth_sqldata.LowerLimitPrice);
						}

						if (!valid4){
							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
						}



				}

			}






		}

	if(list.size()>0)
	{
		printf("开仓策略: end  成交量活跃合约----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}
	vector<MainContractData>().swap(list);


}

void  StaDay::DayTrade_Open_CycleN()
{
	Signal signal;
	MysqlDayLine mysqldayline;
	MysqlMinData mysqlmindata;
	MysqlInstrument mysqlinstrument;
//	double stopprice;
	DateUtil dateutil;

	MysqlDepthMarket mysqldepthmarket;
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str()); //xxxx:xx:xx 2017:09:01 00:00:00

	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_TradeSectionList(list);
	if(list.size()>0)
	{
		printf("开仓策略:    日内突破合约----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
//		printf("	DayTrade_Open_CycleN begin \n ");

//			printf("Open_BigVolume----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)  && item.Sedimentary>5.0)  //判断当前交易日是否是假日
			{
				char pinstrumentid[30];
				strcpy(pinstrumentid,item.InstrumentID_trade);

//				printf("	开仓策略--监测合约:  %s ----> %s\n",pinstrumentid,dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_daycyclen");

				int signalcount =GetSignalCount();

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
				DepthMarketData depth_sqldata=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价

				HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());
				int dayline_count =mysqldayline.CountbyAll(pinstrumentid);
//				printf("	[%s]昨日收盘价	 preday.close  %.2f   depthmarket.PreClosePrice  %.2f   \n",pinstrumentid,	preday.Close,depthmarket.PreClosePrice);

				int n_long =20;
				int n_short =10;

				double tr ;
				double tr_lowclose = abs(depthmarket.LowestPrice-depthmarket.PreClosePrice);
				double tr_highclose =abs(depthmarket.HighestPrice-depthmarket.PreClosePrice);
				double tr_highlow = abs(depthmarket.HighestPrice-depthmarket.LowestPrice);
				tr=max(tr_highlow,max(tr_lowclose,tr_highclose));
//				printf("	[%s]tr 计算数据  HighestPrice %.2f     LowestPrice %.2f    PreClosePrice %.2f  \n",pinstrumentid,
//						depthmarket.HighestPrice,depthmarket.LowestPrice,depthmarket.PreClosePrice);
//				printf("	[%s]tr 计算中 	 tr_highlow  %.2f   tr_highclose  %.2f    tr_lowclose %.2f     tr:%.2f \n",pinstrumentid,
//						tr_highlow,tr_highclose,tr_lowclose,tr);


				// 当日atr10 atr20
//				double atr20 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_long,tr);
//				double atr10 = mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),n_short,tr);
//				printf("	[%s]当日atr 计算中 	 当日atr10  %.2f   当日atr20  %.2f   \n",pinstrumentid,	atr10,atr20);


				//前日atr10  atr20
				ATRData atr20_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),n_long);
				ATRData atr10_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,tradingday_sqltime.c_str(),n_short);
//				printf("	[%s]昨日atr 计算中 	 昨日atr10  %.2f   昨日atr20  %.2f   \n",pinstrumentid,	atr10_data.ATR,atr20_data.ATR);

				int n ;
				double k ;
				if(atr20_data.ATR >1.0 && atr10_data.ATR >1.0)
				{
					k = atr20_data.ATR/atr10_data.ATR;
				}
				else
				{
					k=1.0;
				}

				n=floor(n_long*k);
				CycleData cycledata = mysqldayline.Collect_CycleNData(pinstrumentid,tradingday_sqltime.c_str(),n);


				//前前日atr
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
				bool valid1 =dayline_count>30;
				bool valid2 = depthmarket.LastPrice >=depth_sqldata.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice <= depth_sqldata.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice >1.0 && depthmarket.OpenPrice> 1.0 && depthmarket.PreSettlementPrice>1.0;
				bool valid5 = atr10_data.ATR>0.1 && atr20_data.ATR >0.1 && tr>0.1;
				bool valid6 = depthmarket.HighestPrice >1.0 && depthmarket.LowestPrice> 1.0 &&  depthmarket.PreClosePrice>1.0;
				bool valid7 = preatr10_data.ATR>0.1 && preatr20_data.ATR >0.1 ;
				bool valid8 = precycledata.High20 >1.0 && precycledata.Low20>1.0;
				bool valid9 = cycledata.High20 >1.0 && cycledata.Low20>1.0 ;

				bool valid =  valid1 && valid2 &&valid3 &&valid4 && valid5  && valid6 && valid7 && valid8 && valid9;

				SetMinTableName(pinstrumentid);
				this->GetMysqlMindata()->Set_Database("minute");
				this->GetMysqlMindata()->Set_Table(this->GetMinTableName());

				if (valid && this->GetMysqlMindata()->Exist_Table(this->GetMinTableName().c_str()))
				{

					this->SetStopPriceRange(atr20_data.ATR/2.0);

					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);


					Mysql_MinData mindata = this->GetMysqlMindata()->Find_LastDatabyMinuteTime(this->GetMinTableName().c_str());
					double amount_minute = mindata.Volume *mindata.ClosePrice *insdata.VolumeMultiple*insdata.LongMarginRatio/10000000.0;


					double rate=amount_minute/item.Sedimentary;  //分钟内交易金额 占总沉淀资金比例

					MysqlHold mysqlhold(this->GetUserID().c_str());
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

					int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);

//							printf("查询MA   %s -->  [%s]   %.3f  %.3f \n",pinstrumentID,tradingday.c_str(),ma60,ma50);

//					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sqltime.c_str());

					//共用条件
					bool commcondition_atr = tr >atr10_data.ATR && atr10_data.ATR>atr20_data.ATR;

					bool longcond_cycle = depthmarket.LastPrice >cycledata.High20;
					bool longcond_not_precycle =depthmarket.PreClosePrice < precycledata.High20 ;
					bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice  &&  depthmarket.OpenPrice > depthmarket.PreClosePrice;

					bool condition_long = longcond_cycle && longcond_not_precycle && longcond_kline && commcondition_atr;

					bool shortcond_cycle =  depthmarket.LastPrice < cycledata.Low20;
					bool shortcond_not_precycle =  depthmarket.PreClosePrice  > precycledata.Low20;
					bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice  &&  depthmarket.OpenPrice < depthmarket.PreClosePrice;

					bool condition_short = shortcond_cycle && shortcond_not_precycle && shortcond_kline && commcondition_atr;

					if(commcondition_atr)
					{
						printf("\n	[%s]行情出现波动   tr :%.2f  昨日atr10: %.2f 昨日atr20: %.2f   \n",pinstrumentid,tr,atr10_data.ATR,atr20_data.ATR);
						if(longcond_kline)
						{
							printf("	多1.[%s]当日K线符合多头   昨结: %.3f   开盘价: %.3f  现价: %.3f\n",pinstrumentid,depthmarket.PreClosePrice,depthmarket.OpenPrice,depthmarket.LastPrice);

							if(longcond_cycle)
							{
								printf("	多2.[%s]当日多头突破   高点: %.3f 现价: %.3f\n",pinstrumentid,cycledata.High20,depthmarket.LastPrice);
								if(longcond_not_precycle)
								{
									printf("	多3.[%s]前日多头未突破   高点: %.3f 昨日收盘价: %.3f\n",pinstrumentid,precycledata.High20,depthmarket.PreClosePrice);

								}
							}
						}
						else if(shortcond_kline)
						{
							printf("	空1.[%s]当日K线符合空头   昨结: %.3f   开盘价: %.3f  现价: %.3f \n",pinstrumentid,depthmarket.PreClosePrice,depthmarket.OpenPrice,depthmarket.LastPrice);

							if(shortcond_cycle)
							{
								printf("	空2.[%s]当日空头突破   低点: %.3f 现价: %.3f\n",pinstrumentid,cycledata.Low20,depthmarket.LastPrice);
								if(shortcond_not_precycle)
								{
									printf("	空3.[%s]前日空头未突破   低点: %.3f 昨日收盘价: %.3f\n",pinstrumentid,precycledata.Low20,depthmarket.PreClosePrice);

								}
							}
						}

					}
					else
					{
//						printf("\n	[%s]行情波动不符合开仓!!!   tr :%.2f  昨日atr10: %.2f 昨日atr20: %.2f   \n",pinstrumentid,tr,atr10_data.ATR,atr20_data.ATR);
					}





					double sedimentary;
					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;

					bool valid_sedimentary = sedimentary>2;

					//多头行情

					if (condition_long && valid_sedimentary)
					{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);


						MysqlHold mysqlhold(this->GetUserID().c_str());

						int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
						printf("	日内突破交易 开多 --> %s 现价:%.2f     --->%s  多头持仓: %d  可开仓位: %d \n",
										pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,longposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position-longposition;
						int direction =0;
						int offset =0;
						DayTrade_OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (condition_short && valid_sedimentary)
					{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

						MysqlHold mysqlhold(this->GetUserID().c_str());
						int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
						printf("	日内突破交易 开空 --> %s 现价:%.2f    --->%s  空头持仓: %d 可开仓位: %d \n",
									pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,shortposition,position);
						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position-shortposition;
						int direction =1;
						int offset =0;
						DayTrade_OpenSignalCheck(holddata,signalname,trading_date.c_str(),signalcount,direction,offset,depthmarket.LastPrice);

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
					if (!valid1)
					{
						printf("	行情数据不合规: %s  --> 日线数据[%d]天太少，无法计算开仓条件   \n",pinstrumentid,dayline_count);
					}

					else if (!valid2 || !valid3)
						{
							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
									pinstrumentid,depthmarket.LastPrice,depth_sqldata.UpperLimitPrice,depth_sqldata.LowerLimitPrice);

						}

						else if (!valid4)
						{
							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
						}
						else if (!valid5)
						{
							printf("	行情数据不合规: %s  -->ATR 数据为零   \n",pinstrumentid);
						}
						else if (!valid6)
						{
							printf("	行情数据不合规: %s  --> 行情最高价  最低价 为零  \n",pinstrumentid);
						}
						else if (!valid7)
						{
							printf("	行情数据不合规: %s  -->  preatr10_data 为零   \n",pinstrumentid);
						}
						else if (!valid8)
						{
							printf("	行情数据不合规: %s  --> precycledata 为零  \n",pinstrumentid);
						}
						else if (!valid9)
						{
							printf("	行情数据不合规: %s  -->cycledata 为零   \n",pinstrumentid);
						}
						else
						{
							printf("	行情数据不合规: %s  !!!   \n",pinstrumentid);
						}


				}

			}






		}

	if(list.size()>0)
	{
		printf("开仓策略: end  价格突破----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}
	vector<MainContractData>().swap(list);


}
int StaDay::Test(DepthMarket *pdepthmarket,InstrumentData *pinsdata)
{
	char buf[1024];
	LogUtil logutil("baselog",this->GetLogPathFileName());

	int ret=-1;
	vector<Mysql_MinData>  list;

	int n=21;

	this->GetMysqlMindata()->Find_LastListsbyN(list,this->GetMinTableName().c_str(),n);

	//			printf("%s --->front %f     %s -->end  %f \n",list.front().MinuteTime,list.front().OpenInterest,
	//							list.back().MinuteTime,list.back().OpenInterest);

	double dif_opi = list.back().OpenInterest-list.front().OpenInterest;
	int dif_volume;
	int long_volume=0 ;
	int short_volume=0;

	double high;
	double low;
	std::string lastminute;
	double lastclose;
	int i=0;
	for (auto &data:list)
	{

		if (i<=19)		{
			if(i==0){
				high=data.ClosePrice;
				low=data.ClosePrice;
			}
			else{
				high=max(high,data.ClosePrice);
				low=min(low,data.ClosePrice);
			}

			if(i==19)	{
				lastminute=data.MinuteTime;
			}
		}
		else{
			lastclose=data.ClosePrice;
		}


		if(data.ClosePrice>data.OpenPrice){
			long_volume+=data.Volume;
	//					printf("long_volume  %d \n", long_volume);
		}
		else if(data.ClosePrice<data.OpenPrice)	{
			short_volume+=data.Volume;

	//					printf("short_volume  %d \n", short_volume);
		}
		i++;
	}

	dif_volume=long_volume-short_volume;

	double amount_minute = dif_volume *pdepthmarket->LastPrice *pinsdata->VolumeMultiple*pinsdata->LongMarginRatio/10000000.0;
	double capital_opi = dif_opi *pdepthmarket->LastPrice *pinsdata->VolumeMultiple*pinsdata->LongMarginRatio/10000000.0;

	if(dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >50.0  && capital_opi >5.0)	{
		printf("	[%s][%s]成交量活跃,增仓中---> dif_opi:%.f   资金流入: %.2f  dif_volume: %d  成交金额: %.2f (千万) \n",
				pdepthmarket->InstrumentID,lastminute.c_str(),dif_opi	, capital_opi,dif_volume,amount_minute);

		if(lastclose  > high  && pdepthmarket->LastPrice >1.0)	{
			printf("  多头---->high: %.3f  low: %.3f   分钟收盘价: %.3f  close: %.3f   dif_range: %.3f \n",high, low,lastclose,pdepthmarket->LastPrice,high-low);

			this->SetStopPriceRange(high-low);
			sprintf(buf,"	[%s]成交量活跃,增仓中---> dif_opi:%.f    资金流入: %.2f  dif_volume: %d  成交金额: %.2f (千万)\n",
							pdepthmarket->InstrumentID,dif_opi	, capital_opi,dif_volume,amount_minute);
			logutil.WriteLog(buf);
			sprintf(buf,"  多头---->high: %.3f  low: %.3f    close: %.3f   dif_range: %.3f \n",high, low,pdepthmarket->LastPrice,high-low);
			logutil.WriteLog(buf);

			ret=0;
		}
	}

	if(dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-50.0  && capital_opi <-5.0)	{
		printf("	[%s][%s]成交量活跃,减仓中---> dif_opi:%.f     资金流出: %.2f dif_volume: %d  成交金额: %.2f (千万)\n",
						pdepthmarket->InstrumentID,lastminute.c_str(),dif_opi	, capital_opi,dif_volume,amount_minute);

		if(lastclose < low  && pdepthmarket->LastPrice >1.0)	{
			printf("  空头---->high: %.3f  low: %.3f    分钟收盘价: %.3fclose: %.3f   dif_range: %.3f \n",high, low,lastclose,pdepthmarket->LastPrice,high-low);
			ret=1;
			this->SetStopPriceRange(high-low);
			sprintf(buf,"	[%s]成交量活跃,增仓中---> dif_opi:%.f   资金流出: %.2f  dif_volume: %d  成交金额: %.2f (千万)\n",
										pdepthmarket->InstrumentID,dif_opi	, capital_opi,dif_volume,amount_minute);
			logutil.WriteLog(buf);


			sprintf(buf,"  空头---->high: %.3f  low: %.3f    close: %.3f   dif_range: %.3f \n",high, low,pdepthmarket->LastPrice,high-low);
			logutil.WriteLog(buf);
		}
	}

	vector<Mysql_MinData>().swap(list);

	return ret;
}
void  StaDay::Check_MinuteVolumeOpiChange()
{

	Check_VolumeOpiChange(this->GetMaxUpDownRateInstrumentid().c_str());
	Check_VolumeOpiChange(this->GetMinUpDownRateInstrumentid().c_str());

}
void  StaDay::Check_VolumeOpiChange(const char* pinstrumentid)
{
	Signal signal;
	MysqlDayLine mysqldayline;
	MysqlInstrument mysqlinstrument;
	MysqlDepthMarket mysqldepthmarket;

	double stopprice;
	DateUtil dateutil;

	std::string trading_date;
	trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901

	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());


	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	MainContractData item=	mysqlmaincontract.Find_DatabyInstrumentID(pinstrumentid);

//		printf("	Open_BigVolume begin \n ");

	printf("Check_VolumeOpiChange----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
	if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
	{

		char pinstrumentid[30];
		strcpy(pinstrumentid,item.InstrumentID_trade);
		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_bigvolumeopen");
		int signalcount =GetSignalCount();

		char productid[31];
		CodeUtil codeutil;
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价
		InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

		double sedimentary;
		SetMinTableName(pinstrumentid);
		this->GetMysqlMindata()->Set_Database("minute");

		this->GetMysqlMindata()->Set_Table(this->GetMinTableName());

		if (this->GetMysqlMindata()->Exist_Table(this->GetMinTableName().c_str()))
		{
			Mysql_MinData mindata = this->GetMysqlMindata()->Find_LastDatabyMinuteTime(this->GetMinTableName().c_str());
			sedimentary = (mindata.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*mindata.ClosePrice)/10000000.0;
			double amount_minute = mindata.Volume *mindata.ClosePrice *insdata.VolumeMultiple*insdata.LongMarginRatio/10000000.0;

			int direction =Test(&depthmarket,&insdata);


			double rate=amount_minute/sedimentary;  //分钟内交易金额 占总沉淀资金比例
			printf("	开仓信号检测 ProductID  方向 [%d] -->  %s  %s  成交量: %d  成交额: %.2f  沉淀资金: %.2f   rate: %.2f%%\n",direction,pinstrumentid,
					mindata.MinuteTime,mindata.Volume,amount_minute,sedimentary,rate*100);
		}
		else
		{
					printf("	Open_BigVolume 不存在  %s  \n ",pinstrumentid);
		}



		bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
		bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
		bool valid4 = depthmarket.LastPrice !=0 && depthmarket.OpenPrice!=0 && depthmarket.PreSettlementPrice!=0;

		bool valid =  valid2 &&valid3 &&valid4  ;

//				if (valid)
		{


			MysqlHold mysqlhold(this->GetUserID().c_str());
			int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
			int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
//					int position =  GetPosition(pinstrumentid, depthmarket,item.Section);
			int position=1;
			//多头行情

			bool condition_long = false;


			bool condition_short =false;


			bool valid_sedimentary = sedimentary>2;
			//					if(opencond_testreport )
//					{
//					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
//					}
			if(condition_long  )
			{
				printf("	[大趋势多头] -->%s  \n",pinstrumentid);

			}
			else if(condition_short )
			{
				printf("	[大趋势空头] -->%s  \n",pinstrumentid);
			}




			if (condition_long  && valid_sedimentary)
			{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
//						printf("	MA 交叉 开多 --> %s 现价:%.2f   MA10:%.2f   MA9:%.2f  MA50:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
//								pinstrumentid,depthmarket.LastPrice,madata.MA10,madata.MA20,madata.MA50,depthmarket.UpdateTime ,longposition,position);

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
//						printf("	MA 交叉 开空 --> %s 现价:%.2f   MA4:%.2f   MA9:%.2f  MA50:%.2f  --->%s  空头持仓: %d 可开仓位: %d \n",
//								pinstrumentid,depthmarket.LastPrice,madata.MA10,madata.MA20,madata.MA50,depthmarket.UpdateTime ,shortposition,position);

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
//				else
//				{
////						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);
//
//
//						if (!valid2 || !valid3)
//						{
//							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//									pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//
//						}
//
//						if (!valid4)
//						{
//							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
//						}
//
//
//
//				}
	}


}

void StaDay::Hold_MA50Reverse_SignalCheck()
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
	MysqlUser mysqluser;
	User userinfo=mysqluser.Find_DatabyStrategyName(this->GetStrategyName().c_str());
	MysqlHold mysqlhold(userinfo.UserID);
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("MA趋势 短周期反转监测---->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
			char pinstrumentid[30];
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

					MysqlUser mysqluser;
					User userinfo=mysqluser.Find_DatabyStrategyName(this->GetStrategyName().c_str());
					MysqlHold mysqlhold(userinfo.UserID);
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

void StaDay::DayTrade_VolumeReverse_SignalCheck()
{
	MysqlDayLine mysqldayline;
	MysqlDepthMarket mysqldepthmarket;

	MysqlInstrument mysqlinstrument;

//	TradingDayData tradingdaydata=this->GetTradingDayData();
//
//	std::string tradingdate;
//	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
//
//	std::string tradingday;
//	char time[20];
//	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
//			tradingdate.substr(6,2).c_str());
//	tradingday=time;   //2017-09-01 00:00:00


	DateUtil dateutil;

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("持仓策略: 交易量活跃度反转监测---->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{

			char pinstrumentid[30];
			strcpy(pinstrumentid,item.InstrumentID);

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_daytrade_volumereverse");


			char productid[31];
			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())  )
			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,productid)  )
			{

				int signalcount =GetSignalCount();

				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价
				InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);


				double sedimentary;

				SetMinTableName(pinstrumentid);
				this->GetMysqlMindata()->Set_Database("minute");

				this->GetMysqlMindata()->Set_Table(this->GetMinTableName());
				int direction;
				if (this->GetMysqlMindata()->Exist_Table(this->GetMinTableName().c_str()))
				{
					Mysql_MinData mindata = this->GetMysqlMindata()->Find_LastDatabyMinuteTime(this->GetMinTableName().c_str());
					sedimentary = (mindata.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*mindata.ClosePrice)/10000000.0;
					double amount_minute = mindata.Volume *mindata.ClosePrice *insdata.VolumeMultiple*insdata.LongMarginRatio/10000000.0;

					direction=Test(&depthmarket,&insdata);
					double rate=amount_minute/sedimentary;  //分钟内交易金额 占总沉淀资金比例
					if(direction>=0)
					{
						printf("	开仓信号检测 ProductID  方向 [%d] -->  %s  %s  成交量: %d  成交额: %.2f  沉淀资金: %.2f   rate: %.2f%%\n",direction,pinstrumentid,
													mindata.MinuteTime,mindata.Volume,amount_minute,sedimentary,rate*100);
					}

				}
				else
				{
							printf("	Open_BigVolume 不存在  %s  \n ",pinstrumentid);
				}



				bool valid2 =  depthmarket.LastPrice >depthmarket.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;

				bool valid =valid2 && valid3;
				if ( valid &&  item.Volume>0)
				{
	//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );


					MysqlHold mysqlhold(this->GetUserID().c_str());
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

					//多头行情
					bool longcond = direction==0;
					bool shortcond = direction==1;


	//							printf("查询MA -->  %s   %.3f  %.3f %.3f \n",pinstrumentID,ma50,ma20,ma10);
					if ( longcond  && item.Direction ==1  )
					{

	//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
	//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
					printf("空头合约成交量转多  --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f 止损价: %.2f  涨幅： %.2f%% --->%s  持仓: %d  \n",
							pinstrumentid,depthmarket.PreSettlementPrice ,
							depthmarket.OpenPrice,depthmarket.LastPrice,item.StopPrice,
							depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition);
//					int offset =1;
//					SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
							printf("多头合约成交量转空 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f     跌幅： %.2f%% ---->%s  持仓: %d  \n",
									pinstrumentid,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice ,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition);
//							int offset =1;
//							SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
void StaDay::DayTrade_EndTimeClose_SignalCheck()
{
//	TradingDayData tradingdaydata=this->GetTradingDayData();
	DateUtil dateutil;


	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllTradingDayList(list,this->GetTradingDay().c_str());
	if(list.size()>0)
	{
		printf("持仓策略: 当日开仓品种尾盘调仓----->当日持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
//				printf("EndTimeClose_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_daytrade_endtime");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			int signalcount=5;
			char ProductID[10];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  && dateutil.CheckSHFEEndMarketTime(this->GetDifSec()))
				{
			//					printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

					DepthMarket depthmarket;
					depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
					printf("	持仓信号[%s]-->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f   bid:%.3f  ask:%.3f\n",signalname.c_str(),item.InstrumentID,
										item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice,depthmarket.BidPrice1,depthmarket.AskPrice1);
					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice;
					bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
					bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;
					// 多头出现亏损
					if(valid && item.Direction==0)
					{
						printf("	多头合约持仓 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
					//空头出现亏损
					else if(valid && item.Direction==1)
					{
						printf("	空头合约持仓 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
			//									printf("止损信号消失 --> %s\n", pinstrumentID);

						SignalDispearCheck(signalname);

					}
				}

		}

	vector<HoldData>().swap(list);
}



// 监测持仓单 是否到止盈值,如果达到,根据止盈信号出现次数 激活止盈信号
void StaDay::DayTrade_WinPrice_SignalCheck()
{
//	TradingDayData tradingdaydata=this->GetTradingDayData();

	std::string tradingdate;
	tradingdate = this->GetTradingDay();   //xxxxxxxx   20170901
	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00

	MysqlDayLine mysqldayline;


	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{
	printf("持仓策略: 持仓合约止盈信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}

	for (auto &item:list)
		{
//				printf("WinPriceSignalCheck----->pinstrumentid %s TraderID %s \n",item.pinstrumentid.c_str(),
//								item.TradeID.c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_daytrade_winclose");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			int signalcount=5;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
				{

	//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

					std::string insid;
					insid = item.InstrumentID;

//					std::string temp =item.OpenDate;
//					char time_buf[30];
//					//  20170309    >>>>    2017-03-09 00:00:00
//					sprintf(time_buf,"%s-%s-%s 00:00:00",temp.substr(0,4).c_str(),temp.substr(4,2).c_str(),
//							temp.substr(6,2).c_str());

					DepthMarket depthmarket;

					memset(&depthmarket,0,sizeof(depthmarket));

					depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

					std::string str_direction;



					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice && data.LowerLimitPrice >0;
					bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice && data.UpperLimitPrice >0;
					bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;

					bool con1_longwin = depthmarket.LastPrice > item.WinPrice;
					bool con1_shortwin = depthmarket.LastPrice < item.WinPrice;

					if (item.Direction ==0 && con1_longwin )
					{
						str_direction="多";
						printf("	多头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %.3f   手数:%d  现价: %.3f    止盈价: %.3f\n",
								item.InstrumentID,item.OpenDate,str_direction.c_str(),
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);

						if( valid  )
								{
								printf("	多头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
												item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);

								}
					}
					else if (item.Direction ==1 && con1_shortwin )
					{
						str_direction="空";
						printf("	空头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %.3f   手数:%d  现价: %.3f   止盈价: %.3f\n",
								item.InstrumentID,item.OpenDate,str_direction.c_str(),
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);
						if( valid  )
						{
							printf("	空头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
													item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
						}

					}


					// 多头出现止盈
					if(item.Direction==0 && valid && con1_longwin )
					{


						printf("	多头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);




					}
					//空头出现止盈
					else if(item.Direction==1 && valid && con1_shortwin )
					{
						printf("	空头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);


					}
					else
					{
//									printf("止损信号消失 --> %s\n", pinstrumentID);
						SignalDispearCheck(signalname);
					}

		}




		}

	vector<HoldData>().swap(list);
}


// 监测持仓单 是否到止盈值,如果达到,根据止盈信号出现次数 激活止盈信号
void StaDay::DayTrade_WinProfit_SignalCheck()
{
	MysqlDayLine mysqldayline;


	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{
	printf("持仓策略: 持仓合约盈利回撤平仓----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}

	for (auto &item:list)
		{
//				printf("WinPriceSignalCheck----->pinstrumentid %s TraderID %s \n",item.pinstrumentid.c_str(),
//								item.TradeID.c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_daytrade_profitclose");
			int signalcount=5;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
				{

	//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

					std::string insid;
					insid = item.InstrumentID;

//					std::string temp =item.OpenDate;
//					char time_buf[30];
//					//  20170309    >>>>    2017-03-09 00:00:00
//					sprintf(time_buf,"%s-%s-%s 00:00:00",temp.substr(0,4).c_str(),temp.substr(4,2).c_str(),
//							temp.substr(6,2).c_str());

					DepthMarket depthmarket;

					memset(&depthmarket,0,sizeof(depthmarket));

					depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice && data.LowerLimitPrice >0;
					bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice && data.UpperLimitPrice >0;
					bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;
					double profit;

					bool con1_longwin=false;
					bool con1_shortwin=false;

					if (item.Direction ==0 )
					{
						profit = (depthmarket.LastPrice -item.OpenPriceAverage)*depthmarket.VolumeMultiple;

						con1_longwin = profit <item.HighProfit/2.0  && item.HighProfit>1000.0;
						printf("	多头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:多头 开仓价: %.3f   手数:%d  现价: %.3f    止盈价: %.3f\n",
								item.InstrumentID,item.OpenDate,
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);

						if( valid  )
								{

								printf("	多头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
												item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);

								}
					}
					else if (item.Direction ==1  )
					{
						profit = ( item.OpenPriceAverage  -depthmarket.LastPrice )*depthmarket.VolumeMultiple;
						con1_shortwin = profit <item.HighProfit/2.0  && item.HighProfit>1000.0;
						printf("	空头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:空头 开仓价: %.3f   手数:%d  现价: %.3f   止盈价: %.3f\n",
								item.InstrumentID,item.OpenDate,
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);
						if( valid  )
						{

							printf("	空头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
													item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
						}

					}


					// 多头出现止盈
					if(item.Direction==0 && valid && con1_longwin )
					{


						printf("	多头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
					//空头出现止盈
					else if(item.Direction==1 && valid && con1_shortwin )
					{
						printf("	空头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
//									printf("止损信号消失 --> %s\n", pinstrumentID);
						SignalDispearCheck(signalname);
					}

		}




		}

	vector<HoldData>().swap(list);
}
// 监测持仓单 是否到止损值,如果达到,根据止损信号出现次数 激活止损信号
void StaDay::DayTrade_DynamicStop_SignalCheck()
{
//	Config config;
	char buf[1024];
	LogUtil logutil("baselog",this->GetLogPathFileName());

	MysqlDayLine mysqldayline;
	MysqlHisATR mysqlhisatr;

//	TradingDayData tradingdaydata=this->GetTradingDayData();
//	std::string tradingdate;
//	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
//
//	std::string tradingday;
//	char time[20];
//	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
//			tradingdate.substr(6,2).c_str());
//	tradingday=time;   //2017-09-01 00:00:00



	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{

		printf("持仓策略: 合约浮动止盈止损信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
			{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
				char pinstrumentID[40];
				char pinstrumentIDDate[50];
				strcpy (pinstrumentID,item.InstrumentID);
				sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

				std::string signalname;
				signalname=pinstrumentID;
				signalname.append("_daytrade_dynamicstop");
				signalname.append("_");
				signalname.append(this->GetUserID().c_str());

				int  signalcount=5;

				char ProductID[31];
				CodeUtil codeutil;
				strcpy(ProductID,codeutil.GetProductID(pinstrumentID).c_str());


					if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)   )  //判断当前交易日是否是假日
//					if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )  //判断当前交易时间是否是尾盘
					{
		//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
						DepthMarket depthmarket;
						depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentID);
//						printf("止损合约行情比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);
						MysqlDepthMarket mysqldepthmarket;
						DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);



						bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice;
						bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
						bool valid = valid2 && valid3 && depthmarket.LastPrice !=0  ;


//						HighLowest hl;
//						if(strcmp(item.OpenDate,tradingdaydata.TradingDay)==0 )
//						{
//							hl.highest =depthmarket.HighestPrice;
//							hl.lowest =depthmarket.LowestPrice;
//
//	//						printf("	当日新开仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);
//
//						}



						if(valid && item.Direction ==0 && depthmarket.LastPrice > item.Highest )
						{

							item.Highest = depthmarket.LastPrice;
							item.StopPrice = depthmarket.LastPrice - item.StopRange;
							item.HighProfit = (depthmarket.LastPrice -item.OpenPriceAverage)*depthmarket.VolumeMultiple;
							mysqlhold.UpdateHighest_StopPrice(item);
							printf("	多头合约创新高 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							sprintf(buf,"DayTrade_DynamicStop_SignalCheck-->	多头合约创新高 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
																item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							logutil.WriteLog(buf);
						}
						else if (valid && item.Direction ==1 && depthmarket.LastPrice < item.Lowest)
						{
							item.Lowest = depthmarket.LastPrice;
							item.StopPrice = depthmarket.LastPrice + item.StopRange;

							item.HighProfit = (item.OpenPriceAverage - depthmarket.LastPrice )*depthmarket.VolumeMultiple;

							mysqlhold.UpdateLowest_StopPrice(item);

							printf("	空头合约创新低 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							sprintf(buf,"DayTrade_DynamicStop_SignalCheck-->	空头合约创新低 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
																item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							logutil.WriteLog(buf);
						}

						// 多头出现止损
						if(valid && item.Direction==0 && depthmarket.LastPrice <item.StopPrice)
						{
							printf("	多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//							int offset =1;
//							SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);
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
						//空头出现止损
						else if(valid && item.Direction==1 && depthmarket.LastPrice > item.StopPrice)
						{
							printf("	空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//							int offset =1;
//							SignalCheck(item,signalname,signalcount,item.Direction,offset,depthmarket.LastPrice);
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
//									printf("止损信号消失 --> %s\n", pinstrumentID);

							SignalDispearCheck(signalname);

						}

					}

			}

	vector<HoldData>().swap(list);
}
// 监测持仓单 是否到止损值,如果达到,根据止损信号出现次数 激活止损信号
void StaDay::DayTrade_Stop_SignalCheck()
{

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{

		printf("持仓策略: 合约止损信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
			{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
				char pinstrumentID[40];
				char pinstrumentIDDate[50];
				strcpy (pinstrumentID,item.InstrumentID);
				sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

				std::string signalname;
				signalname=pinstrumentID;
				signalname.append("_stop");
				int  signalcount=5;

				char ProductID[31];
				CodeUtil codeutil;
				strcpy(ProductID,codeutil.GetProductID(pinstrumentID).c_str());

				if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
					{
		//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
						DepthMarket depthmarket;
						depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentID);
//						printf("止损合约行情比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);
						MysqlDepthMarket mysqldepthmarket;
						DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

						bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice;
						bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
						bool valid = valid2 && valid3 && depthmarket.LastPrice !=0 ;
						// 多头出现止损
						if(valid && item.Direction==0 && depthmarket.LastPrice <item.StopPrice)
						{
							printf("多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							int offset =1;
							SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);



						}
						//空头出现止损
						else if(valid && item.Direction==1 && depthmarket.LastPrice > item.StopPrice)
						{
							printf("空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							int offset =1;
							SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);


						}
						else
						{
//									printf("止损信号消失 --> %s\n", pinstrumentID);

							SignalDispearCheck(signalname);

						}

					}

			}

	vector<HoldData>().swap(list);
}
// 监测持仓单 是否到止损值,如果达到,根据止损信号出现次数 激活止损信号
void StaDay::DayTrade_RiskMangement_SignalCheck()
{
	DateUtil dateutil;
	MysqlAsset mysqlasset(this->GetUserID().c_str());
	AssetData assetdata;
	assetdata= mysqlasset.Find_LastDayAsset();

	MysqlInvestPosition mysqlinvest(this->GetUserID().c_str());

	vector<InvestPositionData> investlist;
	mysqlinvest.Find_AllLists(investlist);

for (auto &item:investlist)
	{

		char instrumentid[20];
		strcpy(instrumentid,item.InstrumentID);

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(instrumentid).c_str());


		MysqlProduct mysqlproduct;
		ProductData productdata;
		productdata=mysqlproduct.Find_DatabyProductID(ProductID);

		MysqlInstrument mysqlinstrument;



//			printf("资金账户: ----->  InstrumentID: %s CloseProfit: %.2f \n",instrumentid,item.CloseProfit);
		std:string dir;
		if (dateutil.CheckOpenTime(this->GetDifSec(),productdata.ExchangeID,ProductID)  && mysqlinstrument.Exist_DatabyInstrumentID(instrumentid))
			{

				int holding;
				double holdprofit;
				double profit;
				holdprofit=0;
				profit=0;

				holding = item.Position;
				DepthMarket depthmarket;
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(instrumentid);

				InstrumentData instrumentdata;

				instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(instrumentid);


				//0: 开盘前  1: 非交易     2:连续交易   3:集合竞价报单   4: 集合竞价价格平衡  5: 集合竞价撮合  6： 收盘

//						printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", instrumentid,mat[0].str().c_str());
//						printf("资金账户: ----->  静态权益: %.2f 浮盈: %.2f \n",totalcolseprofit,totalcommission);
				if(item.Position >0)
				{

					if (item.PosiDirection ==2 )
							{
								if(depthmarket.LastPrice >1.0)
								{
									holdprofit = ( depthmarket.LastPrice - item.OpenPriceAverage) *
												(double)instrumentdata.VolumeMultiple*	(double)item.TodayPosition
												+(depthmarket.LastPrice - item.PreSettlementPrice) *
												(double)instrumentdata.VolumeMultiple*	(double)item.YdPosition;

									profit = (depthmarket.LastPrice - item.OpenPriceAverage) *
												(double)instrumentdata.VolumeMultiple*	(double)item.Position;

								}
								else
								{
									holdprofit=0;
									profit=0;
								}

								dir="多头";
							}
							else if (item.PosiDirection ==3 )
							{
								if(depthmarket.LastPrice >1.0)
								{
									holdprofit = (item.OpenPriceAverage- depthmarket.LastPrice ) *
												(double)instrumentdata.VolumeMultiple*	(double)item.TodayPosition
												+( item.PreSettlementPrice - depthmarket.LastPrice) *
													(double)instrumentdata.VolumeMultiple*	(double)item.YdPosition;
									profit = ( item.OpenPriceAverage - depthmarket.LastPrice) *
												(double)instrumentdata.VolumeMultiple*	(double)item.Position;
								}
								else
								{
									holdprofit=0;
									profit=0;
								}


								dir="空头";
							}


					if(depthmarket.LastPrice >1.0)
					{
						printf("	持仓合约[%s] ----->%s  仓位[%d] 昨仓[%d] 今仓[%d]---->  开仓均价: %.3f  现价:%.3f  占用保证金: %.2f 持仓浮盈: %.2f\n",
												item.InstrumentID,dir.c_str(),item.Position,item.YdPosition,item.TodayPosition,
												item.OpenPriceAverage,depthmarket.LastPrice,
												item.UseMargin,holdprofit);
					}
					else
					{
						printf("	持仓合约[%s] ---  行情数据不合规  \n",item.InstrumentID);
					}


				}

			}

//			printf("资金账户0: ----->  InstrumentID[%d]: %s \n",i,tradeinsidlist.pinstrumentid[i].c_str());

	}

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);


	if(list.size()>0)
	{

		printf("持仓策略: 合约止损信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
			{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
				char pinstrumentID[40];
				char pinstrumentIDDate[50];
				strcpy (pinstrumentID,item.InstrumentID);
				sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

				std::string signalname;
				signalname=pinstrumentID;
				signalname.append("_stop");
				int  signalcount=5;

				char ProductID[31];
				CodeUtil codeutil;
				strcpy(ProductID,codeutil.GetProductID(pinstrumentID).c_str());

				if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
					{
		//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
						DepthMarket depthmarket;
						depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentID);
//						printf("止损合约行情比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);
						MysqlDepthMarket mysqldepthmarket;
						DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

						bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice;
						bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
						bool valid = valid2 && valid3 && depthmarket.LastPrice !=0 ;
						// 多头出现止损
						if(valid && item.Direction==0 && depthmarket.LastPrice <item.StopPrice)
						{
							printf("多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							int offset =1;
							SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);



						}
						//空头出现止损
						else if(valid && item.Direction==1 && depthmarket.LastPrice > item.StopPrice)
						{
							printf("空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							int offset =1;
							SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);


						}
						else
						{
//									printf("止损信号消失 --> %s\n", pinstrumentID);

							SignalDispearCheck(signalname);

						}

					}

			}

	vector<InvestPositionData>().swap(investlist);
	vector<HoldData>().swap(list);
}
