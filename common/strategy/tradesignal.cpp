#include "tradesignal.hpp"


TradeSignal::TradeSignal() {

}




void  TradeSignal::Check_MASignal()
{

	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;
	MysqlHisMA mysqlhisma;
	double stopprice;


	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00

	DateUtil dateutil;

	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_AllList(list);
	printf("MA_SignalCheck----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoString(this->GetDifSec()).c_str());

	for (auto &item:list)
		{
			char pinstrumentid[10];
			strcpy(pinstrumentid,item.InstrumentID_trade);

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(pinstrumentid).c_str());

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_maopen");
			int signalcount =30;

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
			{

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

				MAData madata;


				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentid);

				MysqlDepthMarket mysqldepthmarket;
				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);

				mysqlma.CreateTable(pinstrumentid);
				std::string ma_lastdate =mysqlma.GetLastDateTime(pinstrumentid);

				int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);

				if (tradingday.compare(ma_lastdate)==0)
				{
					madata=mysqlma.Find_MADatabyTime(pinstrumentid,tradingday.c_str());
				}
				else  if(count>59)
				{
					printf("lastdate 不同 ------>tradingday: %s    ma_date:%s    depth: %s \n  ",
								tradingday.c_str(),ma_lastdate.c_str(),depthmarket.TradingDay);

					madata=mysqldayline.Collect_TotalMAData(pinstrumentid,tradingday.c_str());
					mysqlma.Insert(pinstrumentid,madata);
				}


				bool valid0 = count > 59;

				bool valid2 = depthmarket.LastPrice >data.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice !=0;

				bool valid = valid0 && valid2 &&valid3 &&valid4;

				if ( valid )
				{


//					printf("	数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f   涨幅： %.2f%% --->%s  \n",
//							pinstrumentid,depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,
//							depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold;
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
					int position = this->GetAssetAvalible() * this->GetRiskRate() /(depthmarket.LastPrice * depthmarket.VolumeMultiple *depthmarket.LongMarginRatio);
					double ma60;
					ma60=(madata.MA59+depthmarket.LastPrice)/60;
					double ma50;
					ma50=(madata.MA49+depthmarket.LastPrice)/50;
					double ma20;
					ma20=(madata.MA19+depthmarket.LastPrice)/20;
					double ma10;
					ma10=(madata.MA9+depthmarket.LastPrice)/10;

					double ma18;
					ma18=(madata.MA17+depthmarket.LastPrice)/18;
					double ma9;
					ma9=(madata.MA8+depthmarket.LastPrice)/9;
					double ma4;
					ma4=(madata.MA3+depthmarket.LastPrice)/4;


					HisMAData hismadata;
					hismadata = mysqlhisma.Find_LastHisMADatabyOffset(pinstrumentid,1);
					double ma50_last =hismadata.MA50;
					double x = atan((ma50/ma50_last-1)*100)*180/M_PI;
//							printf("查询MA   %s -->  [%s]   %.3f  %.3f  %.3f  %.3f \n",
//									pinstrumentID,tradingday.c_str(),ma60,ma50,ma50_last,x);
					//多头行情
					bool longcond = ma10 > ma20  && ma20 >ma50;
					bool longcond1= x>0;
					bool longcond5 = depthmarket.LastPrice > ma10;
					bool longcond4 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;
					bool slongcond= depthmarket.LastPrice > ma4 && ma4 > ma9  && ma9 >ma18;


					bool shortcond = ma10 < ma20  && ma20 <ma50;
					bool shortcond1 =x<0;
					bool shortcond5 = depthmarket.LastPrice < ma10;
					bool shortcond4 = depthmarket.LastPrice > depthmarket.LowerLimitPrice;
					bool sshortcond= depthmarket.LastPrice < ma4 && ma4 < ma9  && ma9 <ma18;



//							printf("查询MA -->  %s   %.3f  %.3f %.3f \n",pinstrumentID,ma50,ma20,ma10);
					if ( longcond   && longcond1 && longcond4 && longcond5 && slongcond)
					{


//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	MA多头趋势 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  ma50:%.3f   ma20:%.3f  ma10:%.3f 涨幅： %.2f%% --->%s  持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.PreSettlementPrice ,
								depthmarket.OpenPrice,depthmarket.LastPrice,ma50,ma20,ma10,
								depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition,position);
						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						SignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (shortcond && shortcond1 && shortcond4 && shortcond5 && sshortcond)
					{
						double m;
						double s;
						s = depthmarket.LastPrice *(1.02);
						m =fmod(s,depthmarket.PriceTick);
						stopprice = s -m;
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	MA空头趋势 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f    ma50:%.3f ma20:%.3f ma10:%.3f  跌幅： %.2f%% ---->%s  持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.PreSettlementPrice ,
								depthmarket.OpenPrice,depthmarket.LastPrice ,ma50,ma20,ma10,
								depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						SignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);


					}
					else
					{
//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
						SignalDispearCheck(signalname);

					}


				}
				else
				{
					if (!valid0)
					{
//							printf("	行情数据不合规: %s  --> 日线数据长度[%d]不足  \n",pinstrumentid,count);

					}

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



void  TradeSignal::Check_CycleSignal()
{
	MysqlDayLine mysqldayline;
	MysqlCycle mycycle;
	MysqlMA mysqlma;
	MysqlEMA mysqlema;
	MysqlHisMA mysqlhisma;
	MysqlHisATR mysqlhisatr;
	MysqlInstrument mysqlinstrument;
//	RedisDepthMarket redisdepthmarket;

	double stopprice;
	DateUtil dateutil;

	MysqlDepthMarket mysqldepthmarket;
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00


	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday.c_str());



	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_AllList(list);
	if(list.size()>0)
	{
		printf("Check_CycleSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{


//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)  )
			{

				char pinstrumentid[10];
				strcpy(pinstrumentid,item.InstrumentID_trade);
				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_cycleopen");
				int signalcount =30;


				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentid);


				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
				bool valid5;
				std::string dayline_lastdate =mysqldayline.GetLastDateTime(pinstrumentid);
				time_t tm_yesterday=dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());

				time_t tm_enddayline =  dateutil.CheckHisDayLine(tradingdaydata.TradingDay,this->GetDifSec());

//				printf("	dayline  ------>tm_yesterday [%s]: %ld    tm_dayline [%s]: %ld     \n  ",
//												dayline_lastdate.c_str(),tm_yesterday,tradingday.c_str(),tm_dayline);

				if (tm_yesterday ==tm_enddayline)
				{
					valid5 =true;
				}
				else
				{
					valid5=false;
					printf("	dayline 数据缺失 ------>tm_yesterday [%s]: %ld    tm_tradingday [%s]: %ld     \n  ",
								dayline_lastdate.c_str(),tm_yesterday,tradingday.c_str(),tm_tradingday);
				}

				CycleData cycledata;
				MAData madata;
				EMAData emadata;

				mycycle.CreateTable(pinstrumentid);
				std::string cycle_lastdate =mycycle.GetLastDateTime(pinstrumentid);



				int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);

				if (tradingday.compare(cycle_lastdate)==0)
				{
//					printf("lastdate 相同 ------>tradingday: %s    cycle_date:%s    depth: %s \n  ",
//												tradingdate.c_str(),cycle_lastdate.c_str(),depthmarket.TradingDay);
					cycledata = mycycle.Find_DatabyDateTime(pinstrumentid,tradingday.c_str());
				}
				else if(count>20)
				{
					printf("	lastdate 不同 ------>tradingday: %s    cycle_date:%s    depth: %s \n  ",
							tradingday.c_str(),cycle_lastdate.c_str(),depthmarket.TradingDay);


					mysqldayline.CheckHttpDayLine(pinstrumentid,item.ExchangeID);
					mysqldayline.Update_CycleData(pinstrumentid);
					cycledata=mysqldayline.Collect_CycleData(pinstrumentid,tradingday.c_str());
					mycycle.Insert(pinstrumentid,cycledata);
				}

				mysqlma.CreateTable(pinstrumentid);
				std::string ma_lastdate =mysqlma.GetLastDateTime(pinstrumentid);
				if (tradingday.compare(ma_lastdate)==0)
				{
					madata=mysqlma.Find_MADatabyTime(pinstrumentid,tradingday.c_str());


				}
				else if(count>59 )
				{
					printf("	lastdate 不同 ------>tradingday: %s    ma_date:%s    depth: %s \n  ",
								tradingday.c_str(),ma_lastdate.c_str(),depthmarket.TradingDay);

					madata=mysqldayline.Collect_TotalMAData(pinstrumentid,tradingday.c_str());
					mysqlma.Insert(pinstrumentid,madata);
				}


				std::string ema_lastdate =mysqlema.GetLastDateTime(pinstrumentid);
				if (tradingday.compare(ema_lastdate)==0)
				{

					emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,tradingday.c_str());
				}
				else if(count>59 )
				{
					printf("	lastdate 不同 ------>tradingday: %s    ema_date:%s    depth: %s \n  ",
								tradingday.c_str(),ma_lastdate.c_str(),depthmarket.TradingDay);

					emadata=mysqldayline.Collect_EMAData(pinstrumentid,tradingday.c_str());
					mysqlema.Insert(pinstrumentid,emadata);

				}

				bool valid0 = count > 59;
				bool valid1 = cycledata.High20 != 0   && cycledata.Low20 !=0;
				bool valid2 = depthmarket.LastPrice >data.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice !=0;

				bool valid = valid0 && valid1 && valid2 &&valid3 &&valid4 &&valid5;

				if (valid)
				{
//					printf("查询周期价差表 -->  %s   %f  %f  \n",date.c_str(),cycledata.High20,cycledata.Low20);

//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold;
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
					int p = this->GetAssetAvalible() * this->GetRiskRate() /(depthmarket.LastPrice * depthmarket.VolumeMultiple *depthmarket.LongMarginRatio);
					int position;

					if (strcmp(item.Section,"Single")==0)
					{
						position = p /2 - (p/2) %2;
					}
					else
					{
						position = p-p%2 ;
					}

					double ma60;
					ma60=(madata.MA59+depthmarket.LastPrice)/60;
					double ma50;
					ma50=(madata.MA49+depthmarket.LastPrice)/50;
					double ma20;
					ma20=(madata.MA19+depthmarket.LastPrice)/20;
					double ma10;
					ma10=(madata.MA9+depthmarket.LastPrice)/10;

					double ema50,ema9,ema4;
					ema50 = (2*depthmarket.LastPrice + 49*emadata.EMA49)/51;
					ema9 = (2*depthmarket.LastPrice + 8*emadata.EMA49)/10;
					ema4 = (2*depthmarket.LastPrice + 3*emadata.EMA49)/5;

					HisMAData hismadata;
					hismadata = mysqlhisma.Find_LastHisMADatabyOffset(pinstrumentid,1);
					double ma50_last =hismadata.MA50;
					double ma20_last =hismadata.MA20;
					double ma10_last =hismadata.MA10;
					double x50 = atan((ma50/ma50_last-1)*100)*180/M_PI;
					double x20 = atan((ma20/ma20_last-1)*100)*180/M_PI;
					double x10 = atan((ma10/ma10_last-1)*100)*180/M_PI;

//							printf("查询MA   %s -->  [%s]   %.3f  %.3f \n",pinstrumentID,tradingday.c_str(),ma60,ma50);
RedisSection redissection;
					SectionInfo sectioninfo=redissection.GetSection(item.Section);
					//多头行情
					bool longcond = depthmarket.LastPrice > ma10 && ma10 > ma20  && ma20 >ma50;
					bool longcond5 = x10> 0 && x20 > 0 && x50 >0;

					bool longcond3 = depthmarket.LastPrice > cycledata.High20 ;

					bool longcond1 = depthmarket.LastPrice > depthmarket.OpenPrice  ;
					bool longcond2 = depthmarket.OpenPrice > depthmarket.PreSettlementPrice;


					bool longcond4 = depthmarket.LastPrice < depthmarket.UpperLimitPrice*0.99;

					bool longcond6 = cycledata.HighCount >=3 ;

					bool longcond7 ;
					if (strcmp(item.Section,"Single")==0)
					{
						longcond7=true;

					}
					else
					{
						longcond7 = sectioninfo.CountUprate> 0.7;
					}
					bool condition_long = longcond && longcond1 && longcond2  && longcond3 && longcond4  && longcond5  && longcond6 && longcond7 ;

					bool shortcond =  depthmarket.LastPrice < ma10 && ma10 < ma20  && ma20 <ma50;
					bool shortcond5 = x10 < 0 && x20 <0 && x50 < 0;

					bool shortcond3 = depthmarket.LastPrice < cycledata.Low20 ;

					bool shortcond1 = depthmarket.LastPrice < depthmarket.OpenPrice ;
					bool shortcond2 = depthmarket.OpenPrice < depthmarket.PreSettlementPrice;

					bool shortcond4 = depthmarket.LastPrice > depthmarket.LowerLimitPrice*1.01;

					bool shortcond6 = cycledata.LowCount >=3  ;
					bool shortcond7;

					if (strcmp(item.Section,"Single")==0)
					{
						shortcond7=true;
					}
					else
					{
						shortcond7 = sectioninfo.CountDownrate> 0.7;
					}

					bool condition_short = shortcond && shortcond1 && shortcond2  && shortcond3 && shortcond4 && shortcond5 && shortcond6 && shortcond7 ;



					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

					double sedimentary;
					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);


					if(longcond && longcond5  && longcond3 && longcond7 && longcond1 && longcond2)
					{
						printf("	[板块联动] [大趋势多头] [上涨速率确认] [周期价差高点突破][%d] [高开高走]-->%s  \n",cycledata.HighCount,pinstrumentid);
					}
					else if(longcond && longcond5  && longcond3 && longcond7)
					{
						printf("	[板块联动] [大趋势多头] [上涨速率确认] [周期价差高点突破]  -->%s  \n",pinstrumentid);
					}
					else  if(longcond && longcond5  && longcond3)
					{
						printf("	[大趋势多头] [上涨速率确认] [周期价差高点突破]-->%s  \n",pinstrumentid);

					}
					else if(longcond && longcond5)
					{
						printf("	[大趋势多头] [上涨速率确认] -->%s  \n",pinstrumentid);

					}
					else if(longcond)
					{
//						printf("	[大趋势多头]-->%s  \n",pinstrumentid);

					}


					if(shortcond && shortcond5  && shortcond3 && shortcond7 && shortcond1 && shortcond2)
					{
						printf("	[板块联动] [大趋势空头] [下跌速率确认] [周期价差低点突破][%d] [低开低走]-->%s  \n",cycledata.LowCount,pinstrumentid);
					}
					else if(shortcond && shortcond5  && shortcond3 && shortcond7)
					{
						printf("	[板块联动] [大趋势空头] [下跌速率确认] [周期价差低点突破] -->%s  \n",pinstrumentid);
					}
					else if(shortcond && shortcond5  && shortcond3)
					{
						printf("	[大趋势空头] [下跌速率确认] [周期价差低点突破]-->%s  \n",pinstrumentid);
					}
					else if(shortcond && shortcond5)
					{
						printf("	[大趋势空头] [下跌速率确认] -->%s  \n",pinstrumentid);

					}
					else if(shortcond)
					{
//						printf("	[大趋势空头]-->%s  \n",pinstrumentid);

					}


					if (condition_long && sedimentary >2)
					{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	高开高走 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f   20日最高:%.2f  板块:%.f%%  highcount: %d 涨幅： %.2f%% --->%s  多头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.PreSettlementPrice ,
								depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,sectioninfo.CountUprate*100,cycledata.HighCount,
								depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						SignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (condition_short && sedimentary >2)
					{

//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	低开低走 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f   20日最低:%.2f   板块:%.f%% lowcount: %d 跌幅： %.2f%% ---->%s  空头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.PreSettlementPrice ,
								depthmarket.OpenPrice,depthmarket.LastPrice ,cycledata.Low20,sectioninfo.CountDownrate*100,cycledata.LowCount,
								depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =1;
						int offset =0;
						SignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);

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

						if (!valid0)
						{
//							printf("	行情数据不合规: %s  --> 日线数据长度[%d]不足  \n",pinstrumentid,count);

						}

						if (!valid1)
						{
							printf("	行情数据不合规: %s  --> 周期价差关键点错误 高点[%.3f] 低点[%.3f]  \n",
									pinstrumentid,cycledata.High20,cycledata.Low20);
						}

						if (!valid2 || !valid3)
						{
							printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
									pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);

						}

						if (!valid4)
						{
							printf("	行情数据不合规: %s  --> 现价数据为零 [%.3f]  \n",pinstrumentid,depthmarket.LastPrice);
						}

						if (!valid5)
						{
							printf("	历史数据不合规: %s  --> 日线数据缺失 最后更新日期[%s]  \n",pinstrumentid,dayline_lastdate.c_str());
						}

				}
			}


		}
	vector<MainContractData>().swap(list);

}

ReferenceData TradeSignal::GetKeyPrice(const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int n)
{
	MysqlCycle mysqlcycle;
	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	CycleData cycledata;
	ReferenceData data;
	EMAData emadata;

	mysqlcycle.CreateTable(pinstrumentid);
	std::string cycle_lastdate =mysqlcycle.GetLastDateTime(pinstrumentid);
	std::string str_tradingdate =tradingdate;
	std::string str_tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
			str_tradingdate.substr(6,2).c_str());
	str_tradingday=time;   //2017-09-01 00:00:00



	int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);

	if (str_tradingday.compare(cycle_lastdate)==0)
	{
//					printf("lastdate 相同 ------>tradingday: %s    cycle_date:%s    depth: %s \n  ",
//												tradingdate.c_str(),cycle_lastdate.c_str(),depthmarket.TradingDay);
		cycledata = mysqlcycle.Find_DatabyDateTime(pinstrumentid,str_tradingday.c_str());
	}
	else if(count>20)
	{
		printf("	lastdate 不同 ------>tradingday: %s    cycle_date:%s    \n  ",
				str_tradingday.c_str(),cycle_lastdate.c_str());


		mysqldayline.CheckHttpDayLine(pinstrumentid,exchangeid);
		mysqldayline.Update_CycleData(pinstrumentid);
		cycledata=mysqldayline.Collect_CycleData(pinstrumentid,str_tradingday.c_str());
		mysqlcycle.Insert(pinstrumentid,cycledata);
	}


	time_t lowest_tt = dateutil.ConvertSqlTimetoTimet( cycledata.LowDate);
	time_t highest_tt = dateutil.ConvertSqlTimetoTimet( cycledata.HighDate);

	time_t trading_tt =  dateutil.ConvertSqlTimetoTimet(str_tradingday.c_str());

	int interval_high  = (trading_tt-highest_tt)/(60*60*24);
	int interval_low  = (trading_tt-lowest_tt)/(60*60*24);

//	printf("cycledata.LowDate   %s lowest_tt [%ld]  cycledata.HighDate %s  highest_tt[%ld]    tradingday.c_str() %s trading_tt[%ld]  \n ",
//			cycledata.LowDate,lowest_tt,cycledata.HighDate,highest_tt, tradingday.c_str() ,trading_tt);



if (cycledata.HighCount >n)
{
	data.High20 = cycledata.High20;
	data.HighCount = cycledata.HighCount;

}
else
{
	if (interval_high >10)
	{
		data.High20 = cycledata.High20;
		data.HighCount =interval_high;
	}
}

if(cycledata.LowCount >n)
{
	data.Low20 = cycledata.Low20;
	data.LowCount = cycledata.LowCount;
//printf("1111111\n");
}
else
{
//	printf("22222222222222\n");
	if(interval_low >10)
	{
		data.Low20 = cycledata.Low20;
		data.LowCount =interval_low;
//		printf("3333333333333333333\n");
	}
//	printf("4444444444444444\n");
}

//printf("%s interval_high %d    interval_low   %d   data.lowcount %d \n",pinstrumentid,interval_high,interval_low,data.LowCount );

	return data;
}




EMAData  TradeSignal::GetEMAData(const char* pinstrumentid,const char* tradingdate,const char* exchangeid,double lastprice)
{
	EMAData emadata;
	MysqlDayLine mysqldayline;
	MysqlEMA mysqlema;

	emadata.EMA49 =0;
	emadata.EMA8=0;
	emadata.EMA3=0;
	emadata.EMA50=0;
	emadata.EMA9=0;
	emadata.EMA4=0;

	std::string str_tradingdate =tradingdate;
	std::string str_tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
			str_tradingdate.substr(6,2).c_str());
	str_tradingday=time;   //2017-09-01 00:00:00

	int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	std::string ema_lastdate =mysqlema.GetLastDateTime(pinstrumentid);

	if (str_tradingday.compare(ema_lastdate)==0)
	{
//		emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,str_tradingday.c_str());

//		emadata.EMA50 = (2*lastprice + 49*emadata.EMA49)/51;
//		emadata.EMA9 = (2*lastprice + 8*emadata.EMA8)/10;
//		emadata.EMA4 = (2*lastprice + 3*emadata.EMA3)/5;


		emadata.EMA50 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),50,lastprice);
		emadata.EMA9 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),9,lastprice);
		emadata.EMA4 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),4,lastprice);


	}
	else
	{

		if(count>59 )
		{
			printf("GetEMAData  	lastdate 不同 ------>tradingday: %s    ema_date:%s    ",
							str_tradingday.c_str(),ema_lastdate.c_str());

			emadata=mysqldayline.Collect_EMAData(pinstrumentid,str_tradingday.c_str());
			mysqlema.Insert(pinstrumentid,emadata);
//			emadata.EMA50 = (2*lastprice + 49*emadata.EMA49)/51;
//			emadata.EMA9 = (2*lastprice + 8*emadata.EMA8)/10;
//			emadata.EMA4 = (2*lastprice + 3*emadata.EMA3)/5;

			emadata.EMA50 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),50,lastprice);
			emadata.EMA9 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),9,lastprice);
			emadata.EMA4 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,str_tradingday.c_str(),4,lastprice);
		}
		else
		{
			printf("GetEMAData  ----dayline count [%d]   不合规   \n",count);
		}


	}


	return emadata;
}

HisEMAData  TradeSignal::GetPreEMAData(const char* pinstrumentid,const char* tradingdate)
{
	HisEMAData emadata;
	MysqlDayLine mysqldayline;
	MysqlHisEMA mysqlhisema;

	std::string str_tradingdate =tradingdate;
	std::string str_tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
			str_tradingdate.substr(6,2).c_str());
	str_tradingday=time;   //2017-09-01 00:00:00

	int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	std::string ema_lastdate =mysqlhisema.GetLastDateTime(pinstrumentid);

	if (str_tradingday.compare(ema_lastdate)==0)
	{
		emadata=mysqlhisema.Find_PreHisEMADatabyDateTime(pinstrumentid,str_tradingday.c_str());

	}
	else
	{

		if(count>59 )
		{
			printf("GetEMAData  	lastdate 不同 ------>tradingday: %s    ema_date:%s    ",
							str_tradingday.c_str(),ema_lastdate.c_str());

			emadata=mysqldayline.Collect_HisEMAData(pinstrumentid,str_tradingday.c_str());
			printf("[%s]不存在该数据,插入新HisEMA数据[%s] --->>>>EMA50[%.3f] EMA9[%.3f]  EMA4[%.3f]:\n",
					pinstrumentid,emadata.Time,emadata.EMA50,emadata.EMA9,emadata.EMA4);
			mysqlhisema.Insert(pinstrumentid,emadata);

		}
		else
		{
			printf("GetPreEMAData  ----dayline count [%d]   不合规   \n",count);
		}


	}


	return emadata;
}

HisEMAData  TradeSignal::GetHisEMAData_Angle(const char* pinstrumentid,const char* tradingdate)
{
	HisEMAData emadata;
	MysqlDayLine mysqldayline;
	MysqlHisEMA mysqlhisema;

	std::string str_tradingdate =tradingdate;
	std::string str_tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
			str_tradingdate.substr(6,2).c_str());
	str_tradingday=time;   //2017-09-01 00:00:00

	int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	std::string ema_lastdate =mysqlhisema.GetLastDateTime(pinstrumentid);

	if (str_tradingday.compare(ema_lastdate)==0)
	{

		emadata=mysqlhisema.Find_HisEMADatabyDateTime_Offset(pinstrumentid,str_tradingday.c_str(),1);
	}
	else
	{

		if(count>59 )
		{
			printf("GetEMAData  	lastdate 不同 ------>tradingday: %s    ema_date:%s    ",
							str_tradingday.c_str(),ema_lastdate.c_str());
			MysqlUtil mysqlutil;
			mysqlutil.CheckHisEMADataByInstrument(pinstrumentid);

			emadata=mysqlhisema.Find_HisEMADatabyDateTime_Offset(pinstrumentid,str_tradingday.c_str(),1);

		}
		else
		{
			printf("GetHisEMAData_Angle  ----dayline count [%d]   不合规   \n",count);
		}


	}


	return emadata;
}



void  TradeSignal::Check_EMACycleSignal()
{
	MysqlDayLine mysqldayline;
	MysqlCycle mycycle;
	MysqlMA mysqlma;
	MysqlEMA mysqlema;
	MysqlHisMA mysqlhisma;
	MysqlHisATR mysqlhisatr;
	MysqlInstrument mysqlinstrument;
	MysqlTestReport mysqltestreport;
//	RedisDepthMarket redisdepthmarket;

	double stopprice;
	DateUtil dateutil;

	MysqlDepthMarket mysqldepthmarket;
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday.c_str());


	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_AllList(list);
	if(list.size()>0)
	{
		printf("Check_EMACycleSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{


//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)  )
			{

				char pinstrumentid[10];
				strcpy(pinstrumentid,item.InstrumentID_trade);
				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_emacycleopen");
				int signalcount =30;

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentid);


				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

				bool valid_dayline = ValidCheck_DayLine(pinstrumentid,tradingdate.c_str(),item.ExchangeID);
				ReferenceData reference =GetKeyPrice(pinstrumentid,tradingdate.c_str(),item.ExchangeID,2);  // 关键高低点 持续2天以上
				EMAData emadata =GetEMAData(pinstrumentid,tradingdate.c_str(),item.ExchangeID,depthmarket.LastPrice);

				bool valid1 = reference.High20 != 0   && reference.Low20 !=0;
				bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice !=0 && depthmarket.OpenPrice!=0 && depthmarket.PreSettlementPrice!=0;
				bool valid_ema = emadata.EMA49 !=0 && emadata.EMA8 !=0 && emadata.EMA3 !=0;

				bool valid = valid_dayline && valid1 && valid2 &&valid3 &&valid4  && valid_ema;

				if (valid)
				{
//					printf("查询周期价差表 -->  %s   %f  %f  \n",date.c_str(),cycledata.High20,cycledata.Low20);

//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold;
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);
					int p = this->GetAssetAvalible() * this->GetRiskRate() /(depthmarket.LastPrice * depthmarket.VolumeMultiple *depthmarket.LongMarginRatio);
					int position;

					if (strcmp(item.Section,"Single")==0)
					{
						position = p /2 - (p/2) %2;
					}
					else
					{
						position = p-p%2 ;
					}





//							printf("查询MA   %s -->  [%s]   %.3f  %.3f \n",pinstrumentID,tradingday.c_str(),ma60,ma50);

					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday.c_str());
					TestReportData testreport = mysqltestreport.Find_DatabyProductID("v1",productid);
					RedisSection redissection;
					SectionInfo sectioninfo=redissection.GetSection(item.Section);
					//多头行情
					bool longcond_ema = depthmarket.LastPrice > emadata.EMA4 && emadata.EMA4 > emadata.EMA9  && emadata.EMA9 >emadata.EMA50;


					bool longcond3 = depthmarket.LastPrice > reference.High20 ;

					bool longcond1 = depthmarket.LastPrice > depthmarket.OpenPrice  ;
					bool longcond2 = depthmarket.OpenPrice > depthmarket.PreSettlementPrice;
					bool longcond4 = depthmarket.LastPrice < depthmarket.UpperLimitPrice*0.99;

					bool longcond6 = reference.HighCount >=3 ;

					bool longcond7 ;
					if (strcmp(item.Section,"Single")==0)
					{
						longcond7=true;

					}
					else
					{
						longcond7 = sectioninfo.CountUprate> 0.7;
					}

					bool opencond_atr = atr_data.ATR /depthmarket.LastPrice  < 0.35;
					bool opencond_testreport = testreport.Expectation > fabs(testreport.AverageLoseProfit) && testreport.Expectation  >0 ;

					bool condition_long = longcond_ema && longcond1 && longcond2  && longcond3 && longcond4   && longcond6 && longcond7 && opencond_atr && opencond_testreport;

					bool shortcond_ema =  depthmarket.LastPrice < emadata.EMA4 && emadata.EMA4 < emadata.EMA9  && emadata.EMA9 < emadata.EMA50;


					bool shortcond3 = depthmarket.LastPrice < reference.Low20 ;

					bool shortcond1 = depthmarket.LastPrice < depthmarket.OpenPrice ;
					bool shortcond2 = depthmarket.OpenPrice < depthmarket.PreSettlementPrice;

					bool shortcond4 = depthmarket.LastPrice > depthmarket.LowerLimitPrice*1.01;

					bool shortcond6 = reference.LowCount >=3  ;
					bool shortcond7;

					if (strcmp(item.Section,"Single")==0)
					{
						shortcond7=true;
					}
					else
					{
						shortcond7 = sectioninfo.CountDownrate> 0.7;
					}

					bool condition_short = shortcond_ema && shortcond1 && shortcond2  && shortcond3 && shortcond4  && shortcond6 && shortcond7 && opencond_atr && opencond_testreport;



					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

					double sedimentary;
					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);
//					if(opencond_testreport )
//					{
//					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
//					}
					if(opencond_testreport && longcond_ema   && opencond_atr && longcond3 && longcond7 && longcond1 && longcond2)
					{
						printf("	[板块联动] [大趋势多头] [行情振幅合理] [周期价差高点突破][%d] [高开高走]-->%s  \n",reference.HighCount,pinstrumentid);
					}
					else if(opencond_testreport && longcond_ema && opencond_atr  && longcond3 && longcond7)
					{
						printf("	[板块联动] [大趋势多头] [行情振幅合理] [周期价差高点突破]  -->%s  \n",pinstrumentid);
					}
					else  if(opencond_testreport && longcond_ema && opencond_atr  && longcond3)
					{
						printf("	[大趋势多头] [行情振幅合理] [周期价差高点突破]-->%s  \n",pinstrumentid);

					}
					else if(opencond_testreport && longcond_ema && opencond_atr)
					{
						printf("	[大趋势多头] [行情振幅合理] -->%s  \n",pinstrumentid);

					}
					else if(opencond_testreport && longcond_ema)
					{
//						printf("	[大趋势多头]-->%s  \n",pinstrumentid);

					}


					if(opencond_testreport && shortcond_ema && opencond_atr  && shortcond3 && shortcond7 && shortcond1 && shortcond2)
					{
						printf("	[板块联动] [大趋势空头] [行情振幅合理] [周期价差低点突破][%d][低开低走]-->%s  \n",reference.LowCount,pinstrumentid);
					}
					else if(opencond_testreport && shortcond_ema && opencond_atr  && shortcond3 && shortcond7)
					{
						printf("	[板块联动] [大趋势空头] [行情振幅合理] [周期价差低点突破] -->%s  \n",pinstrumentid);
					}
					else if(opencond_testreport && shortcond_ema && opencond_atr  && shortcond3)
					{
						printf("	[大趋势空头] [行情振幅合理] [周期价差低点突破]-->%s  \n",pinstrumentid);
					}
					else if(opencond_testreport && shortcond_ema && opencond_atr)
					{
						printf("	[大趋势空头] [行情振幅合理] -->%s  \n",pinstrumentid);

					}
					else if(opencond_testreport && shortcond_ema)
					{
//						printf("	[大趋势空头]-->%s  \n",pinstrumentid);

					}


					if (condition_long && sedimentary >2)
					{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	高开高走 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f   20日最高:%.2f  板块:%.f%%  highcount: %d 涨幅： %.2f%% --->%s  多头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.PreSettlementPrice ,
								depthmarket.OpenPrice,depthmarket.LastPrice,reference.High20,sectioninfo.CountUprate*100,reference.HighCount,
								depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						OpenSignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (condition_short && sedimentary >2)
					{

//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	低开低走 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f   20日最低:%.2f   板块:%.f%% lowcount: %d 跌幅： %.2f%% ---->%s  空头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.PreSettlementPrice ,
								depthmarket.OpenPrice,depthmarket.LastPrice ,reference.Low20,sectioninfo.CountDownrate*100,reference.LowCount,
								depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =1;
						int offset =0;
						OpenSignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);

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
							printf("	行情数据不合规: %s  --> 周期价差关键点错误 高点[%.3f] 低点[%.3f]  \n",
									pinstrumentid,reference.High20,reference.Low20);
						}

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

void  TradeSignal::Check_EMACrossSignal()
{
	MysqlDayLine mysqldayline;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;
	MysqlHisATR mysqlhisatr;
	MysqlInstrument mysqlinstrument;
	MysqlTestReport mysqltestreport;
//	RedisDepthMarket redisdepthmarket;

	double stopprice;
	DateUtil dateutil;

	MysqlDepthMarket mysqldepthmarket;
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday.c_str());


	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_AllList(list);
	if(list.size()>0)
	{
		printf("Check_EMACrossSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{


//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
			{

				char pinstrumentid[10];
				strcpy(pinstrumentid,item.InstrumentID_trade);
				std::string signalname;
				signalname=pinstrumentid;
				signalname.append("_emacrossopen");
				int signalcount =30;

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentid);


				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

				bool valid_dayline = ValidCheck_DayLine(pinstrumentid,tradingdate.c_str(),item.ExchangeID);
				ReferenceData reference =GetKeyPrice(pinstrumentid,tradingdate.c_str(),item.ExchangeID,2);  // 关键高低点 持续2天以上
				EMAData emadata =GetEMAData(pinstrumentid,tradingdate.c_str(),item.ExchangeID,depthmarket.LastPrice);

				HisEMAData pre_emadata = GetPreEMAData(pinstrumentid,tradingdate.c_str());

				HisEMAData his_emadata_angle = GetHisEMAData_Angle(pinstrumentid,tradingdate.c_str());

				double ema_x50 = atan((emadata.EMA50/his_emadata_angle.EMA50-1)*100)*180/M_PI;


				bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
				bool valid4 = depthmarket.LastPrice !=0 && depthmarket.OpenPrice!=0 && depthmarket.PreSettlementPrice!=0;
				bool valid_ema = emadata.EMA50 !=0 && emadata.EMA9 !=0 && emadata.EMA4 !=0;
				bool valid_preema = pre_emadata.EMA50 !=0 && pre_emadata.EMA9 !=0  && pre_emadata.EMA4 !=0;
				bool valid_hisema_angle = his_emadata_angle.EMA50 !=0 && his_emadata_angle.EMA9 !=0  && his_emadata_angle.EMA4 !=0;

				bool valid = valid_dayline  && valid2 &&valid3 &&valid4   && valid_preema && valid_hisema_angle;

				if (valid)
				{
//					printf("查询周期价差表 -->  %s   %f  %f  \n",date.c_str(),cycledata.High20,cycledata.Low20);

//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold;
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

					int position =  GetPosition(pinstrumentid, depthmarket,item.Section);

//							printf("查询MA   %s -->  [%s]   %.3f  %.3f \n",pinstrumentID,tradingday.c_str(),ma60,ma50);

					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday.c_str());


//					TestReportData testreport = mysqltestreport.Find_DatabyProductID("v1",productid);

//					SectionInfo sectioninfo=redissection.GetSection(item.Section);
					//多头行情
					bool longcond_ema = depthmarket.LastPrice > emadata.EMA4 && emadata.EMA4 > emadata.EMA9  && emadata.EMA9 >emadata.EMA50;
					bool longcond_pre_ema = pre_emadata.EMA4 < pre_emadata.EMA9;
					bool longcond_ema_angle = ema_x50>0;

					bool longcond_trend = longcond_ema && longcond_pre_ema && longcond_ema_angle;

//					bool longcond_section ;
//					if (strcmp(item.Section,"Single")==0)
//					{
//						longcond_section=true;
//
//					}
//					else
//					{
//						longcond_section = sectioninfo.CountUprate> 0.7;
//					}
//
//
//					bool opencond_testreport = testreport.Expectation > fabs(testreport.AverageLoseProfit) && testreport.Expectation  >0 ;

					bool condition_long = longcond_trend;

					bool shortcond_ema =  depthmarket.LastPrice < emadata.EMA4 && emadata.EMA4 < emadata.EMA9  && emadata.EMA9 < emadata.EMA50;
					bool shortcond_pre_ema = pre_emadata.EMA4 > pre_emadata.EMA9;
					bool shortcond_ema_angle = ema_x50<0;

					bool shortcond_trend = shortcond_ema && shortcond_pre_ema && shortcond_ema_angle;
//					bool shortcond_section;
//
//					if (strcmp(item.Section,"Single")==0)
//					{
//						shortcond_section=true;
//					}
//					else
//					{
//						shortcond_section = sectioninfo.CountDownrate> 0.7;
//					}

					bool condition_short = shortcond_trend;



					InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

					double sedimentary;
					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);

					bool valid_sedimentary = sedimentary>2;
					//					if(opencond_testreport )
//					{
//					printf("testreport:  %s    期望值---> %.3f   平均亏损    ----> %.3f fabs[%.3f]   \n ",item.InstrumentID,testreport.Expectation , testreport.AverageLoseProfit,fabs(testreport.AverageLoseProfit));
//					}
					if(longcond_trend  )
					{
						printf("	[大趋势多头] -->%s  \n",pinstrumentid);

					}
					else if(longcond_trend  )
					{
						printf("	[大趋势多头] -->%s  \n",pinstrumentid);
					}

					if(shortcond_trend )
					{
						printf("	[大趋势空头] -->%s  \n",pinstrumentid);
					}
					else if(shortcond_trend)
					{
						printf("	[大趋势空头] -->%s  \n",pinstrumentid);
					}



					if (condition_long  && valid_sedimentary)
					{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	EMA 交叉 开多 --> %s 现价:%.2f   EMA4:%.2f   EMA9:%.2f  EMA50:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
								pinstrumentid,depthmarket.LastPrice,emadata.EMA4,emadata.EMA9,emadata.EMA50,depthmarket.UpdateTime ,longposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =0;
						int offset =0;
						OpenSignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);
					}
					else if (condition_short  && valid_sedimentary)
					{

						printf("查询上一交易日 EMA   %s -->  [%s]   pre_emadata.EMA4 :%.3f  pre_emadata.EMA9:%.3f \n",
								pinstrumentid,pre_emadata.Time,pre_emadata.EMA4,pre_emadata.EMA9);
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
						printf("	EMA 交叉 开空 --> %s 现价:%.2f   EMA4:%.2f   EMA9:%.2f  EMA50:%.2f  --->%s  空头持仓: %d 可开仓位: %d \n",
														pinstrumentid,depthmarket.LastPrice,emadata.EMA4,emadata.EMA9,emadata.EMA50,depthmarket.UpdateTime ,shortposition,position);

						HoldData holddata;
						strcpy(holddata.InstrumentID,pinstrumentid);
						strcpy(holddata.ExchangeID,item.ExchangeID);
						holddata.Volume =position;
						int direction =1;
						int offset =0;
						OpenSignalCheck(holddata,signalname,tradingdaydata.TradingDay,signalcount,direction,offset,depthmarket.LastPrice);

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


// 监测持仓单 是否到止损值,如果达到,根据止损信号出现次数 激活止损信号
void TradeSignal::Stop_SignalCheck()
{

	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();



	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{

		printf("持仓合约止损信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(tradingdaydata.DifSeconds).c_str());
	}


	for (auto &item:list)
			{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
				char pinstrumentID[10];
				char pinstrumentIDDate[30];
				strcpy (pinstrumentID,item.InstrumentID);
				sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

				std::string signalname;
				signalname=pinstrumentID;
				signalname.append("_stop");
				int  signalcount=5;

				char ProductID[31];
				CodeUtil codeutil;
				strcpy(ProductID,codeutil.GetProductID(pinstrumentID).c_str());

				if (dateutil.CheckOpenTime(tradingdaydata.DifSeconds,item.ExchangeID,ProductID)  )
					{
		//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
						DepthMarket depthmarket;
						depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentID);
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
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);



						}
						//空头出现止损
						else if(valid && item.Direction==1 && depthmarket.LastPrice > item.StopPrice)
						{
							printf("空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);


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




// 监测持仓单 是否出现反转行情, 如果达到,根据止损信号出现次数 激活止损信号
void TradeSignal::Reverse_SignalCheck()
{
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	DateUtil dateutil;

	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("持仓合约行情反转信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
	{
//				printf("Reverse_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_reverse");
		int  signalcount=30;

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
		{

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

			DepthMarket depthmarket;
			depthmarket = redisdepthmarket.GetDepthMarket(item.InstrumentID);
//								printf("行情反转比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													orderinfo.OpenPrice,orderinfo.Direction,orderinfo.StopPrice,depthmarket.LastPrice);
			MysqlDepthMarket mysqldepthmarket;
			DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

			bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice;
			bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
			bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;
			bool con1_long = depthmarket.LastPrice <depthmarket.OpenPrice;
			bool con2_long =  depthmarket.OpenPrice <depthmarket.PreSettlementPrice;

			bool con1_short = depthmarket.LastPrice >depthmarket.OpenPrice;
			bool con2_short =  depthmarket.OpenPrice >depthmarket.PreSettlementPrice;


			// 多头出现止损
			if(item.Direction==0 && valid && con1_long  && con2_long  && depthmarket.LastPrice < item.OpenPriceAverage  )
			{
				printf("多头合约行情反转止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
						item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
				int offset =1;
				SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			//空头出现止损
			else if(item.Direction==1 && valid&& con1_short && con2_short && depthmarket.LastPrice > item.OpenPriceAverage )
			{
				printf("空头合约行情反转止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
						item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
				int offset =1;
				SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);
			}
			else
			{
				SignalDispearCheck(signalname);
			}

		}

	}
	vector<HoldData>().swap(list);
}

void TradeSignal::TrendReverse_SignalCheck()
{

	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;
	double stopprice;

	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();



	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00
	DateUtil dateutil;


	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("MA大趋势消失监测---->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
			char pinstrumentID[10];
			strcpy(pinstrumentID,item.InstrumentID);

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
			{


				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentID);

				std::string signalname;
				signalname=pinstrumentID;
				signalname.append("_trendreverse");
				int signalcount =30;



//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

				MAData madata;


				mysqlma.CreateTable(pinstrumentID);
				std::string ma_lastdate =mysqlma.GetLastDateTime(pinstrumentID);

				int count =mysqldayline.Count_AllListsbyTableName(pinstrumentID);

				if (tradingday.compare(ma_lastdate)==0)
				{
					madata=mysqlma.Find_MADatabyTime(pinstrumentID,tradingday.c_str());
				}
				else if(count>59)
				{
					printf("lastdate 不同 ------>tradingday: %s    ma_date:%s    depth: %s \n  ",
								tradingday.c_str(),ma_lastdate.c_str(),depthmarket.TradingDay);

					madata=mysqldayline.Collect_TotalMAData(pinstrumentID,tradingday.c_str());
					mysqlma.Insert(pinstrumentID,madata);
				}


	//						printf("数据准备 --> %s  --->%d  \n",pinstrumentID,count);


				bool valid =count>59;
				bool valid2 =  depthmarket.LastPrice >depthmarket.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;
				if (valid && valid2 && valid3)
				{


	//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold;
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentID,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentID,1);

					double ma60;
					ma60=(madata.MA59+depthmarket.LastPrice)/60;
					double ma50;
					ma50=(madata.MA49+depthmarket.LastPrice)/50;
					double ma20;
					ma20=(madata.MA19+depthmarket.LastPrice)/20;
					double ma10;
					ma10=(madata.MA9+depthmarket.LastPrice)/10;

					double ma18;
					ma18=(madata.MA17+depthmarket.LastPrice)/18;
					double ma9;
					ma9=(madata.MA8+depthmarket.LastPrice)/9;
					double ma4;
					ma4=(madata.MA3+depthmarket.LastPrice)/4;
//					printf("查询MA   %s -->  [%s]   ma18[%.3f]  ma9[%.3f] ma4[%.3f]\n",pinstrumentID,tradingday.c_str(),ma18,ma9,ma4
//							);

					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					//多头行情
					bool longcond = ma10 > ma20  && ma20 >ma50;
					bool longcond1 = depthmarket.LastPrice > ma10;
					bool longcond4 = depthmarket.LastPrice < data.UpperLimitPrice;



					bool shortcond = ma10 < ma20  && ma20 <ma50;
					bool shortcond1 = depthmarket.LastPrice < ma10;
					bool shortcond4 = depthmarket.LastPrice > data.LowerLimitPrice;

					if ( item.Direction==0 && !longcond  && !longcond1   )
					{
						printf("	多头趋势消失 -->  %s   %.3f  %.3f %.3f \n",pinstrumentID,ma50,ma20,ma10);
					}
					else if(item.Direction==1 && !shortcond && !shortcond1  )
					{
						printf("	空头趋势消失 -->  %s   %.3f  %.3f %.3f \n",pinstrumentID,ma50,ma20,ma10);
					}
					else{
//						printf("	趋势[%d] -->  %s   %.3f  %.3f %.3f \n",item.Direction,pinstrumentID,ma50,ma20,ma10);
					}

					if ( item.Direction==0 && !longcond  && !longcond1 && longcond4  )
					{

		//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
		//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
							printf("	多头合约  多头趋势消失--> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  ma50:%.3f   ma20:%.3f  ma10:%.3f 涨幅： %.2f%% --->%s  持仓: %d \n",
									pinstrumentID,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice,ma50,ma20,ma10,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition);
							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);



					}
					else if ( item.Direction==1 && !shortcond && !shortcond1 && shortcond4  )
					{


		//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
		//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
							printf("	空头合约 空头趋势消失 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f    ma50:%.3f ma20:%.3f ma10:%.3f  跌幅： %.2f%% ---->%s  持仓: %d  \n",
									pinstrumentID,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice ,ma50,ma20,ma10,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition);
							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

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

void TradeSignal::MAReverse_SignalCheck()
{

	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;
	double stopprice;

	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();



	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00


	DateUtil dateutil;
	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("MA趋势 短周期反转监测---->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
			char pinstrumentID[10];
			strcpy(pinstrumentID,item.InstrumentID);



			std::string signalname;
			signalname=pinstrumentID;
			signalname.append("_mareverse");
			int signalcount=60;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
			{

				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentID);

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

				MAData madata;


				mysqlma.CreateTable(pinstrumentID);
				std::string ma_lastdate =mysqlma.GetLastDateTime(pinstrumentID);

				int count =mysqldayline.Count_AllListsbyTableName(pinstrumentID);

				if (tradingday.compare(ma_lastdate)==0)
				{
					madata=mysqlma.Find_MADatabyTime(pinstrumentID,tradingday.c_str());
				}
				else if(count>59)
				{
					printf("lastdate 不同 ------>tradingday: %s    ma_date:%s    depth: %s \n  ",
								tradingday.c_str(),ma_lastdate.c_str(),depthmarket.TradingDay);

					madata=mysqldayline.Collect_TotalMAData(pinstrumentID,tradingday.c_str());
					mysqlma.Insert(pinstrumentID,madata);
				}


	//						printf("数据准备 --> %s  --->%d  \n",pinstrumentID,count);


				bool valid =count>59;
				bool valid2 =  depthmarket.LastPrice >depthmarket.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;
				if ( valid && valid2 && valid3)
				{


	//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );

					MysqlHold mysqlhold;
					int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentID,0);
					int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentID,1);

					double ma60;
					ma60=(madata.MA59+depthmarket.LastPrice)/60;
					double ma50;
					ma50=(madata.MA49+depthmarket.LastPrice)/50;
					double ma20;
					ma20=(madata.MA19+depthmarket.LastPrice)/20;
					double ma10;
					ma10=(madata.MA9+depthmarket.LastPrice)/10;

					double ma18;
					ma18=(madata.MA17+depthmarket.LastPrice)/18;
					double ma9;
					ma9=(madata.MA8+depthmarket.LastPrice)/9;
					double ma4;
					ma4=(madata.MA3+depthmarket.LastPrice)/4;
//					printf("查询MA   %s -->  [%s]   ma18[%.3f]  ma9[%.3f] ma4[%.3f]\n",pinstrumentID,tradingday.c_str(),ma18,ma9,ma4
//							);
					//多头行情
					bool longcond = ma10 > ma20  && ma20 >ma50;
					bool longcond5 = depthmarket.LastPrice > ma10;
					bool longcond4 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;

					bool slongcond= depthmarket.LastPrice > ma4 && ma4 > ma9  && ma9 >ma18;

					bool shortcond = ma10 < ma20  && ma20 <ma50;
					bool shortcond5 = depthmarket.LastPrice < ma10;
					bool shortcond4 = depthmarket.LastPrice > depthmarket.LowerLimitPrice;

					bool sshortcond= depthmarket.LastPrice < ma4 && ma4 < ma9  && ma9 <ma18;
	//							printf("查询MA -->  %s   %.3f  %.3f %.3f \n",pinstrumentID,ma50,ma20,ma10);
					if ( longcond   && longcond4 && longcond5 && sshortcond)
					{

						if(slongcond)
						{
//							printf("多头顺势   小周期顺势%s -->  [%s] ma18[%.3f]  ma9[%.3f] ma4[%.3f]\n",pinstrumentID,tradingday.c_str(),ma18,ma9,ma4);
						}
						else if (sshortcond)
						{

							printf("多头趋势  小周期逆势   %s -->  [%s] ma18[%.3f]  ma9[%.3f] ma4[%.3f]\n",pinstrumentID,tradingday.c_str(),ma18,ma9,ma4);

		//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
		//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
							printf("MA多头趋势  小周期逆势--> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f 止损价: %.2f  ma50:%.3f   ma20:%.3f  ma10:%.3f 涨幅： %.2f%% --->%s  持仓: %d  \n",
									pinstrumentID,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice,stopprice,ma50,ma20,ma10,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime ,longposition);
							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);



						}




					}
					else if (shortcond && shortcond4 && shortcond5 &&slongcond)
					{

						if(sshortcond)
						{
//							printf("多头顺势   小周期顺势%s -->  [%s] ma18[%.3f]  ma9[%.3f] ma4[%.3f]\n",pinstrumentID,tradingday.c_str(),ma18,ma9,ma4);
						}
						else if (slongcond)
						{

							printf("多头趋势  小周期逆势   %s -->  [%s] ma18[%.3f]  ma9[%.3f] ma4[%.3f]\n",pinstrumentID,tradingday.c_str(),ma18,ma9,ma4);


		//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
		//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);
							printf("MA空头趋势 小周期逆势 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f    ma50:%.3f ma20:%.3f ma10:%.3f  跌幅： %.2f%% ---->%s  持仓: %d  \n",
									pinstrumentID,depthmarket.PreSettlementPrice ,
									depthmarket.OpenPrice,depthmarket.LastPrice ,ma50,ma20,ma10,
									depthmarket.UpdownRate*100,depthmarket.UpdateTime,shortposition);
							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);


						}


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
void TradeSignal::EMAReverse_SignalCheck()
{

	MysqlDayLine mysqldayline;
	MysqlEMA mysqlema;
	double stopprice;

	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();



	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901

	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00


	DateUtil dateutil;
	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)
	{
		printf("EMA趋势 大趋势反转监测(亏损单)---->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
			char pinstrumentid[10];
			strcpy(pinstrumentid,item.InstrumentID);
			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_emareverse");
			int signalcount=60;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
			{
				DepthMarket depthmarket;
				depthmarket = redisdepthmarket.GetDepthMarket(pinstrumentid);

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

				EMAData emadata =GetEMAData(pinstrumentid,tradingdate.c_str(),item.ExchangeID,depthmarket.LastPrice);

				int count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);

	//						printf("数据准备 --> %s  --->%d  \n",pinstrumentID,count);
				bool valid =count>59;
				bool valid2 =  depthmarket.LastPrice >depthmarket.LowerLimitPrice;
				bool valid3 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;
				bool valid4 = emadata.EMA50 !=0;
				if ( valid && valid2 && valid3 && valid4)
				{
//					printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  20日最高:%.2f   涨幅： %.2f%% --->%s  \n",pinstrumentID[i],depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdownRate*100,depthmarket.UpdateTime );


					//有持仓合约
					if (item.Volume >0)
					{
						bool longcond_cross =  depthmarket.LastPrice < emadata.EMA50 ;
						bool longcond_valid = emadata.EMA50 !=0 && depthmarket.LastPrice < depthmarket.UpperLimitPrice;

						bool shortcond_cross =   depthmarket.LastPrice > emadata.EMA50;
						bool shortcond_valid = emadata.EMA50 !=0 && depthmarket.LastPrice > depthmarket.LowerLimitPrice;

						if ( longcond_cross   && longcond_valid && item.Direction ==0 && depthmarket.LastPrice< item.OpenPriceAverage)
						{

							printf("趋势线交叉的多头亏损合约   %s -->  [%s] 最新价[%.3f]  ema50[%.3f] \n",
									pinstrumentid,tradingday.c_str(),depthmarket.LastPrice , emadata.EMA50);


							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);



						}
						else if (shortcond_cross && shortcond_valid && item.Direction==1 && depthmarket.LastPrice > item.OpenPriceAverage)
						{
							printf("趋势线交叉的空头亏损合约   %s -->  [%s] 最新价[%.3f]  ema50[%.3f] \n",
																pinstrumentid,tradingday.c_str(),depthmarket.LastPrice , emadata.EMA50);

		//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
		//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

							int offset =1;
							SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

						}
						else
						{
							//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);

							SignalDispearCheck(signalname);
						}

					}

				}
				else
				{
	//						printf("合约: %s  --> 数据不合规！！！！  %s \n",pinstrumentID[i],date.c_str());
				}
			}  // opentime check


		}// for
	vector<HoldData>().swap(list);

}

void TradeSignal::EndTimeClose_SignalCheck()
{
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	DateUtil dateutil;

	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllTradingDayList(list,tradingdaydata.TradingDay);
	if(list.size()>0)
	{
		printf("当日开仓品种尾盘调仓----->当日持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)
		{
//				printf("EndTimeClose_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_endtime");
			int signalcount=5;
			char ProductID[10];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  && dateutil.CheckSHFEEndMarketTime(this->GetDifSec()))
				{
			//					printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

					DepthMarket depthmarket;
					depthmarket = redisdepthmarket.GetDepthMarket(item.InstrumentID);
					printf("	持仓信号[%s]-->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",signalname.c_str(),item.InstrumentID,
										item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);
					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice;
					bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice;
					bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;
					// 多头出现亏损
					if(valid && item.Direction==0 && depthmarket.LastPrice < item.OpenPriceAverage && depthmarket.LastPrice <depthmarket.AveragePrice)
					{
						printf("	多头合约持仓 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

					}
					//空头出现亏损
					else if(valid && item.Direction==1 && depthmarket.LastPrice > item.OpenPriceAverage && depthmarket.LastPrice > depthmarket.AveragePrice)
					{
						printf("	空头合约持仓 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

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
void TradeSignal::WinPrice_SignalCheck()
{
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00

	MysqlDayLine mysqldayline;

	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{
	printf("持仓合约止盈信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}

	for (auto &item:list)
		{
//				printf("WinPriceSignalCheck----->pinstrumentid %s TraderID %s \n",item.pinstrumentid.c_str(),
//								item.TradeID.c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_winclose");
			int signalcount=5;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
				{

	//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

					mysqldayline.CreateTable(item.InstrumentID);
					std::string dayline_lastdate =mysqldayline.GetLastDateTime(item.InstrumentID);

					if (tradingday.compare(dayline_lastdate)==0)
					{
	//					printf("lastdate 相同 ------>tradingday: %s    cycle_date:%s    depth: %s \n  ",
	//												tradingdate.c_str(),cycle_lastdate.c_str(),depthmarket.TradingDay);

					}
					else
					{
//						printf("	lastdate 不同 ------>tradingday: %s    dayline_lastdate:%s    depth: %s \n  ",
//								tradingday.c_str(),dayline_lastdate.c_str(),tradingdayinfo.TradingDay);

						MysqlCycle mycycle;
						CycleData cycledata;
//						mysqldayline.CheckHttpDayLine(item.InstrumentID,item.ExchangeID);

						if(!mycycle.Exist_DateTime(item.InstrumentID,tradingday.c_str()))
						{
							mysqldayline.Update_CycleData(item.InstrumentID);
							cycledata=mysqldayline.Collect_CycleData(item.InstrumentID,tradingday.c_str());
							printf("	%s 不存在当日[%s]周期价差数据 ----->插入新数据\n ",item.InstrumentID,tradingday.c_str());
							mycycle.Insert(item.InstrumentID,cycledata);
						}

					}

					std::string insid;
					insid = item.InstrumentID;

					std::string temp =item.OpenDate;
					char time_buf[30];
					//  20170309    >>>>    2017-03-09 00:00:00
					sprintf(time_buf,"%s-%s-%s 00:00:00",temp.substr(0,4).c_str(),temp.substr(4,2).c_str(),
							temp.substr(6,2).c_str());

					DepthMarket depthmarket;

					memset(&depthmarket,0,sizeof(depthmarket));

					depthmarket = redisdepthmarket.GetDepthMarket(item.InstrumentID);

					HighLowest hl;
					if(strcmp(item.OpenDate,tradingdaydata.TradingDay)==0 )
					{
						hl.highest =depthmarket.HighestPrice;
						hl.lowest =depthmarket.LowestPrice;

//						printf("	当日新开仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);

					}
					else
					{
						 hl= mysqldayline.Collect_CycleDatabyOpenDate(insid.c_str(),time_buf);


							if (hl.highest < depthmarket.HighestPrice)
							{
								hl.highest =depthmarket.HighestPrice;
							}

							if (hl.lowest > depthmarket.LowestPrice || hl.lowest==0)
							{
								hl.lowest =depthmarket.LowestPrice;

							}

//							 printf("	历史持仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);

					}






					std::string str_direction;



					double w_long;
					double m_long;
					double longwin;

					double w_short;
					double m_short;
					double shortwin;

					w_long = hl.highest *(0.97) ;
					m_long =fmod(w_long,depthmarket.PriceTick);
					longwin = w_long -m_long;


					w_short = hl.lowest *1.03;
					m_short = fmod(w_short,depthmarket.PriceTick);
					shortwin = w_short-m_short;


					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice && data.LowerLimitPrice >0;
					bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice && data.UpperLimitPrice >0;
					bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;

					bool con1_long =hl.highest >item.OpenPriceAverage*1.06 && depthmarket.LastPrice <= longwin;
					bool con1_short = hl.lowest <item.OpenPriceAverage*0.94 && depthmarket.LastPrice >= shortwin;

					if (item.Direction ==0 && hl.highest >item.OpenPriceAverage*1.06 )
					{
						str_direction="多";
						printf("	多头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %f   手数:%d  现价: %f   持仓最高价: %f 止盈价: %f\n",
								item.InstrumentID,item.OpenDate,str_direction.c_str(),
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,hl.highest,longwin);

						if( valid  )
								{
								printf("	多头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
												item.OpenPriceAverage,longwin,depthmarket.LastPrice);

								}
					}
					else if (item.Direction ==1 && hl.lowest <item.OpenPriceAverage*0.94 )
					{
						str_direction="空";
						printf("	空头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %f   手数:%d  现价: %f   持仓最低价: %f 止盈价: %f\n",
								item.InstrumentID,item.OpenDate,str_direction.c_str(),
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,hl.lowest,shortwin);
						if( valid  )
						{
							printf("	空头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
													item.OpenPriceAverage,shortwin,depthmarket.LastPrice);
						}

					}


					// 多头出现止盈
					if(item.Direction==0 && valid && con1_long )
					{


						printf("	多头合约止盈信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);




					}
					//空头出现止盈
					else if(item.Direction==1 && valid && con1_short )
					{
						printf("	空头合约止盈信号 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,shortwin,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);


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

// 监测最大浮盈回撤 激活止盈信号
void TradeSignal::RiskMangement_SignalCheck()
{
	MysqlTradingDay mysqltradingday;
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string tradingdate;
	tradingdate = tradingdaydata.TradingDay;   //xxxxxxxx   20170901
	std::string tradingday;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",tradingdate.substr(0,4).c_str(),tradingdate.substr(4,2).c_str(),
			tradingdate.substr(6,2).c_str());
	tradingday=time;   //2017-09-01 00:00:00

	MysqlDayLine mysqldayline;

	MysqlHold mysqlhold;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)
	{
	printf("持仓合约止盈信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}
	for (auto &item:list)
		{
//				printf("WinPriceSignalCheck----->pinstrumentid %s TraderID %s \n",item.pinstrumentid.c_str(),
//								item.TradeID.c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_winclose");

			int signalcount=5;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )
				{

	//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

					mysqldayline.CreateTable(item.InstrumentID);
					std::string dayline_lastdate =mysqldayline.GetLastDateTime(item.InstrumentID);

					if (tradingday.compare(dayline_lastdate)==0)
					{
	//					printf("lastdate 相同 ------>tradingday: %s    cycle_date:%s    depth: %s \n  ",
	//												tradingdate.c_str(),cycle_lastdate.c_str(),depthmarket.TradingDay);

					}
					else
					{
//						printf("	lastdate 不同 ------>tradingday: %s    dayline_lastdate:%s    depth: %s \n  ",
//								tradingday.c_str(),dayline_lastdate.c_str(),tradingdayinfo.TradingDay);

						MysqlCycle mycycle;
						CycleData cycledata;
//						mysqldayline.CheckHttpDayLine(item.InstrumentID,item.ExchangeID);

						if(!mycycle.Exist_DateTime(item.InstrumentID,tradingday.c_str()))
						{
							mysqldayline.Update_CycleData(item.InstrumentID);
							cycledata=mysqldayline.Collect_CycleData(item.InstrumentID,tradingday.c_str());
							printf("	%s 不存在当日[%s]周期价差数据 ----->插入新数据\n ",item.InstrumentID,tradingday.c_str());
							mycycle.Insert(item.InstrumentID,cycledata);
						}

					}

					std::string insid;
					insid = item.InstrumentID;

					std::string temp =item.OpenDate;
					char time_buf[30];
					//  20170309    >>>>    2017-03-09 00:00:00
					sprintf(time_buf,"%s-%s-%s 00:00:00",temp.substr(0,4).c_str(),temp.substr(4,2).c_str(),
							temp.substr(6,2).c_str());

					DepthMarket depthmarket;

					memset(&depthmarket,0,sizeof(depthmarket));

					depthmarket = redisdepthmarket.GetDepthMarket(item.InstrumentID);

					HighLowest hl;
					if(strcmp(item.OpenDate,tradingdaydata.TradingDay)==0 )
					{
						hl.highest =depthmarket.HighestPrice;
						hl.lowest =depthmarket.LowestPrice;

//						printf("	当日新开仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);

					}
					else
					{
						 hl= mysqldayline.Collect_CycleDatabyOpenDate(insid.c_str(),time_buf);


							if (hl.highest < depthmarket.HighestPrice)
							{
								hl.highest =depthmarket.HighestPrice;
							}

							if (hl.lowest > depthmarket.LowestPrice || hl.lowest==0)
							{
								hl.lowest =depthmarket.LowestPrice;

							}

//							 printf("	历史持仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);

					}






					std::string str_direction;



					double w_long;
					double m_long;
					double longwin;

					double w_short;
					double m_short;
					double shortwin;

					w_long = hl.highest *(0.97) ;
					m_long =fmod(w_long,depthmarket.PriceTick);
					longwin = w_long -m_long;


					w_short = hl.lowest *1.03;
					m_short = fmod(w_short,depthmarket.PriceTick);
					shortwin = w_short-m_short;


					MysqlDepthMarket mysqldepthmarket;
					DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

					bool valid2 =  depthmarket.LastPrice >data.LowerLimitPrice && data.LowerLimitPrice >0;
					bool valid3 = depthmarket.LastPrice < data.UpperLimitPrice && data.UpperLimitPrice >0;
					bool valid = valid2 && valid3 && depthmarket.LastPrice !=0;

					bool con1_long =hl.highest >item.OpenPriceAverage*1.06 && depthmarket.LastPrice <= longwin;
					bool con1_short = hl.lowest <item.OpenPriceAverage*0.94 && depthmarket.LastPrice >= shortwin;

					if (item.Direction ==0 && hl.highest >item.OpenPriceAverage*1.06 )
					{
						str_direction="多";
						printf("	多头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %f   手数:%d  现价: %f   持仓最高价: %f 止盈价: %f\n",
								item.InstrumentID,item.OpenDate,str_direction.c_str(),
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,hl.highest,longwin);

						if( valid  )
								{
								printf("	多头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
												item.OpenPriceAverage,longwin,depthmarket.LastPrice);

								}
					}
					else if (item.Direction ==1 && hl.lowest <item.OpenPriceAverage*0.94 )
					{
						str_direction="空";
						printf("	空头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %f   手数:%d  现价: %f   持仓最低价: %f 止盈价: %f\n",
								item.InstrumentID,item.OpenDate,str_direction.c_str(),
								item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,hl.lowest,shortwin);
						if( valid  )
						{
							printf("	空头通过数据合法性验证 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
													item.OpenPriceAverage,shortwin,depthmarket.LastPrice);
						}

					}


					// 多头出现止盈
					if(item.Direction==0 && valid && con1_long )
					{
						printf("	多头合约止盈信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

					}
					//空头出现止盈
					else if(item.Direction==1 && valid && con1_short )
					{
						printf("	空头合约止盈信号 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.OpenPriceAverage,shortwin,depthmarket.LastPrice);
						int offset =1;
						SignalCheck(item,signalname,tradingdaydata.TradingDay,signalcount,item.Direction,offset,depthmarket.LastPrice);

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




