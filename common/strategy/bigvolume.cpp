/*
 * bigvolume.cpp
 *
 *  Created on: 2020年2月28日
 *      Author: qc
 */


#include "bigvolume.hpp"

BigVolume::BigVolume() {
	printf("BigVolume  无参数构造函数-->初始  \n");
}


BigVolume::BigVolume(DataWrapper * datawrapper,const char* strategyname) {

	this->SetRedisDepthMarket(&m_redisdepthmarket);
	this->SetRedisSignal(&m_redissignal);
	this->SetRedisSection(&m_redissection);

	printf("BigVolume  带参数构造函数-->初始开始  \n");
	this->SetDataWrapper(datawrapper);
	this->SetStrategyName(strategyname);
	Config config(this->GetStrategyName().c_str());
	std::string logname="sta";
	logname.append("_");
	logname.append(config.GetUserID().c_str());

	std::string logfilename="sta";
	logfilename.append("_");
	logfilename.append(config.GetUserID().c_str());
	logfilename.append(".log");
	this->SetLogName(logname.c_str());
	this->SetLogPathFileName(logfilename.c_str());
	LogUtil * plogutil = new LogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->SetLogUtil(plogutil);

	this->SetMysqlHold(config.GetUserID().c_str());

	this->SetInitialMoney(config.GetInitialMoney());
	this->SetRiskRate(config.GetRiskRate());
	this->SetStopRate(config.GetStopRate());
	this->SetWinRate(config.GetWinRate());
	this->SetBigVolumeRate(config.GetBigVolumeRate());
	this->SetMaxSedimentary(config.GetMaxSedimentary());
	this->SetSingleMaxMargin(config.GetSingleMaxMargin());
	this->SetTotalMaxMargin(config.GetTotalMaxMargin());

	this->SetOpenSignalCount(config.GetOpenSignalCount());
	this->SetCloseSignalCount(config.GetCloseSignalCount());
	this->SetMaxCancelOrderCount(config.GetMaxCancelOrderCount());
	this->SetMaxOpenErrorCount(config.GetMaxOpenErrorCount());

	printf("BigVolume  带参数构造函数-->初始结束   Open signalcount:%d \n",this->GetOpenSignalCount());

	this->GetLogUtil()->WriteLog("BigVolume  带参数构造函数-->初始结束 ");
}
void BigVolume::run(){

	DateUtil dateutil;

	this->SetAvalible(this->GetInitialMoney());
	this->SetTradingDay(this->GetDataWrapper()->GetTradingDay().c_str());
	this->SetDifSec(this->GetDataWrapper()->GetDifSec());
//	this->UpdateTradingDay();
//	std::cout<<"Strategy::run sta "<<this->GetTradingDay()<<std::endl;
	 struct timeval start, m,end;

	while(1){

//		gettimeofday( &m, NULL );
//		printf(">>>>>>>>>>>>>>>BigVolume::run() ----------------> %ld.%ld\n", m.tv_sec, m.tv_usec);

		if(this->GetDataWrapper()->GetPredataStatus()){

			gettimeofday( &start, NULL );
	//		printf("start ----------------> %ld.%ld\n", start.tv_sec, start.tv_usec);
//			printf(">>>>>>>>>>>>>>%s::run  -------------------------------------[%ld.%ld]----------------------------------------->>>>>>   [%s] \n",
//					 this->GetStrategyName().c_str(),start.tv_sec, start.tv_usec,dateutil.GetCurrentSqlTimeString().c_str());


				this->Open();
//				opentest();
				if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())){
//					this->QueueingOpenPriceChange_SignalCheck();
					this->OpenPriceSeek();
				}
				this->Close();

		}
		usleep(250000); // 100000us == 100ms
//		usleep(10);
//		sleep(5);

//		 gettimeofday( &end, NULL );
//		printf("end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
//		printf("Strategy::run  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< while end   [%s] \n",
//						dateutil.GetCurrentSqlTimeString().c_str());
	}



	return ;
}

void  BigVolume::Open()
{
//	printf("Strategy::Open_BigVolumOneMinSignal  begin----------->  \n");

	char logbuf[1024];
	double avalible = this->GetInitialMoney() ;
//	printf(">>>>>>>>>>>>>>>BigVolume::Open  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	 double bigvolumerate =this->GetBigVolumeRate();

	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<">>>>>>>>>>>>>>> tradingday_sqltime:"<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());

	std::string funname=">>>>>>>>>>>>>>>>BigVolume::Open";
	if(this->GetDataWrapper()->GetHolidayStatus()){
			printf("%s----->本交易日为假日,不运行策略!!!!!         \n",funname.c_str());
		return;
	}

	if(this->GetDataWrapper()->GetNextHolidayStatus()){
			printf("%s----->下一个交易日为假日,本交易日不运行策略!!!!!         \n",funname.c_str());
		return;
	}

	if(this->GetDataWrapper()->GetFridayNightStatus()){
			printf("%s----->周五夜盘不运行策略!!!!          \n",funname.c_str());
		return;
	}

	// 控制开仓时间在开盘后5分钟再操作
	 if(!dateutil.isNightMorningBreakTime(this->GetDifSec())){
		printf("%s----->非策略允许交易时间!!!!          \n",funname.c_str());
		return;
	}

//	printf(">>>>>>>>>>>>>>>>>>>----->GetContractLists       %ld \n", this->GetDataWrapper()->GetContractLists()->size());

	 bool night=this->GetDataWrapper()->GetNightStatus();
	CodeUtil codeutil;

	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
			item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);

		char productid[31];
		strcpy(productid,item->ProductID);
//		printf(">>>>>>>>bigvolume ----->      %s  \n",pinstrumentid);

		if(strcmp(this->GetStrategyName().c_str(),"bigvolume_zlqh")==0){
			if(strcmp(item->ProductID,"rb")==0 || strcmp(item->ProductID,"ag")==0){
				printf(">>>>>>>>bigvolume_zlqh ----->      %s  \n",pinstrumentid);
			}
			else{
				continue;
			}

		}


////////////////////开盘时间验证///////////////////
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID);
		if (!opentime){
			printf("   策略验证 --> [%s]非开盘时间 !!!  \n",pinstrumentid);
			continue;
		}

//////////////////////日线数据验证///////////////////
		bool valid_dayline = this->GetDataWrapper()->FindDataStatus(pinstrumentid);
		if (!valid_dayline){
			printf("   策略验证 --> [%s]20天日线数据时不合规!!!  \n",pinstrumentid);
			continue;
		}
////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		if (!valid_redisdepth){

			if(night&& strcmp(item->DayNight,"Day")==0){
//				printf(">>>>>>>>>>>>GetContractLists----->  无夜盘！----->ProductID:%s   exchangeid: %s \n",
//						item->ProductID, item->ExchangeID);
				continue;
			}
			else {
				printf("   bigvolume策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
				continue;
			}

		}

////////////////////当日行情数据验证///////////////////
		bool valid_mindata = this->m_redismindata.Exists(item->InstrumentID)> 0 ;
		if (!valid_mindata){
			printf("   策略验证 --> [%s]不存在Redis分钟行情数据 !!!  请检查行情是否接收正常？   \n",item->InstrumentID);
			continue;
		}

		RMinData rmindata;
		rmindata=this->m_redismindata.GetMinData(item->InstrumentID);//


		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_");
		signalname.append(this->GetStrategyName());
		signalname.append("_");
		signalname.append(this->GetDataWrapper()->GetUserID());


		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf("  策略验证 --> [%s]行情数据时不合规!!!  \n",pinstrumentid);
			continue;
		}

		std::shared_ptr<CycleData> pcycledata = this->GetDataWrapper()->Find5DayCycle(pinstrumentid);
		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(pinstrumentid);
		double atr20=(totaltr+tr)/20;

		bool valid_stoprange=false;
		if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
			valid_stoprange=true;
		}
		else{
			valid_stoprange=false;
			printf("	Strategy:Open_BigVolumOneMinSignal stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);

		}

		if (!valid_stoprange){

			printf("	策略验证 -->  止损区间价格不合规 --> %s atr20:%.2f \n",	pinstrumentid,atr20);
			sprintf(logbuf,"	Strategy::Open_BigVolumOneMinSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
				pinstrumentid,atr20);
			this->GetLogUtil()->WriteLog(logbuf);

			continue;
		}
		//多头行情
		bool longcond_cycle = depthmarket.LastPrice >pcycledata->High20;
		bool cond_vol = (double) rmindata.Volume > (double)rmindata.OpenInterest*bigvolumerate;
		bool longcond_uprate = depthmarket.LastPrice  <depthmarket.UpperLimitPrice*0.985;
		bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
				&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
				&& depthmarket.UpdownRate>0.005 ;
//				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
//						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
//						&& depthmarket.AveragePrice> depthmarket.OpenPrice && depthmarket.UpdownRate>0.005 ;
//				bool longcond_dif = ret_dif >0;
		bool condition_long =  cond_vol &&longcond_cycle&&longcond_uprate ;

		//空头行情
		bool shortcond_cycle =  depthmarket.LastPrice < pcycledata->Low20;

		bool shortcond_downrate =depthmarket.LastPrice  >depthmarket.LowerLimitPrice*1.015;
		bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
				&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
				&& depthmarket.UpdownRate<-0.005;

//				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
//						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
//						&& depthmarket.AveragePrice< depthmarket.OpenPrice && depthmarket.UpdownRate<-0.005;
//				bool shortcond_dif = ret_dif <0;
		bool condition_short = cond_vol && shortcond_cycle && shortcond_downrate;

		std::shared_ptr<Commission>  commssion=this->GetDataWrapper()->FindCommissionData(item->ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/100000000.0;

		bool valid_sedimentary = sedimentary>this->GetMaxSedimentary();
		if (!valid_sedimentary){
			printf("	策略验证 -->信号未激活  合约[%s] 沉淀资金:%.2f不符合 沉淀资金低于[%.2f]亿元,无法交易!!!  \n",pinstrumentid,
					sedimentary,this->GetMaxSedimentary());
			continue;
		}


//		if(longcond_cycle  ){
//			printf(">>>>>>>>>>>>>>>>>>>>1.[CycleN当日趋势突破多头--周期%d] -->%s    现价: %.3f  最高价: %.3f \n",5,
//					pinstrumentid,depthmarket.LastPrice,pcycledata->High20);
//			if( (double)rmindata.Volume > (double)rmindata.OpenInterest*bigvolumerate){
//				printf(">>>>>>>>>>>>>>>>>>>>  %s last min :   vol:%d   opi:%.d      \n",pinstrumentid,
//						rmindata.Volume,rmindata.OpenInterest);
//			}
//		}
//		else if(shortcond_cycle )	{
//			printf(">>>>>>>>>>>>>>>>>>>>	1.[CycleN当日趋势突破空头--周期%d] -->%s    现价: %.3f  最低价: %.3f \n",5,
//					pinstrumentid,depthmarket.LastPrice,pcycledata->Low20);
//			if( (double)rmindata.Volume > (double)rmindata.OpenInterest*bigvolumerate){
//					printf(">>>>>>>>>>>>>>>>>>>>  %s last min :   vol:%d   opi:%.d      \n",pinstrumentid,
//						rmindata.Volume,rmindata.OpenInterest);
//			}
//
//		}


		if (condition_long   )	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
//				printf(">>>>>>>>>>>	BigVolume::Open--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//												signalname.c_str(),position);
//				sprintf(logbuf,"BigVolume::Open--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//						signalname.c_str(),position);
//				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {

				printf(">>>>>>>>>>>	BigVolume::Open 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
										pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
			}
			Signal signal;
			strcpy(signal.InstrumentID,item->InstrumentID);
			strcpy(signal.SingalName,signalname.c_str());

			signal.Volume = position;
			signal.MaxSignalCount =this->GetOpenSignalCount();
			signal.Direction=0;
			signal.Offset=0;
			signal.Price=depthmarket.LastPrice;
			signal.StopRange =atr20;


			OpenSignalUpdate(&signal);

		}
		else if (condition_short  )		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
//				printf(">>>>>>>>>>>BigVolume::Open--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//																		signalname.c_str(),position);
//				sprintf(logbuf,"BigVolume::Open--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//						signalname.c_str(),position);
//				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {
				printf(">>>>>>>>>>>BigVolume::Open 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
							pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
			}
			Signal signal;
			strcpy(signal.InstrumentID,item->InstrumentID);
			strcpy(signal.SingalName,signalname.c_str());

			signal.Volume = position;
			signal.MaxSignalCount =this->GetOpenSignalCount();
			signal.Direction=1;
			signal.Offset=0;
			signal.Price=depthmarket.LastPrice;
			signal.StopRange =atr20;

			OpenSignalUpdate(&signal);

		}
		else	{
//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
			SignalDispearCheck(signalname);

		}
	}

//	printf("Strategy::Open_BigVolumOneMinSignal  End!!!  \n");
}
void  BigVolume::opentest()
{
//	printf("Strategy::Open_BigVolumOneMinSignal  begin----------->  \n");

	char logbuf[1024];
	double avalible = this->GetInitialMoney() ;
//	printf(">>>>>>>>>>>>>>>BigVolume::Open  begin----------->	 可用资金: %.2f  \n",avalible);
	 double bigvolumerate =this->GetBigVolumeRate();

	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<">>>>>>>>>>>>>>> tradingday_sqltime:"<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());



//	printf(">>>>>>>>>>>>>>>>>>>----->GetContractLists       %ld \n", this->GetDataWrapper()->GetContractLists()->size());

	 bool night=this->GetDataWrapper()->GetNightStatus();
	CodeUtil codeutil;
	MysqlMinData m_mysqlmindata;
	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
			item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_");
		signalname.append(this->GetStrategyName());
		signalname.append("_");
		signalname.append(this->GetDataWrapper()->GetUserID());

		char productid[31];
		strcpy(productid,item->ProductID);

		if(strcmp(productid,"j")!=0){
			continue;
		}
		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);


		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(pinstrumentid);
		double atr20=(totaltr+tr)/20;

		bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice;
		bool condition_long =  longcond_kline  ;

		//空头行情
		bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice;

		bool condition_short = shortcond_kline;

		std::shared_ptr<Commission>  commssion=this->GetDataWrapper()->FindCommissionData(item->ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/100000000.0;

		bool valid_sedimentary = sedimentary>this->GetMaxSedimentary();
		if (!valid_sedimentary){
			printf("	策略验证 -->信号未激活  合约[%s] 沉淀资金:%.2f不符合 沉淀资金低于[%.2f]亿元,无法交易!!!  \n",pinstrumentid,
					sedimentary,this->GetMaxSedimentary());
			continue;
		}




		if (condition_long   )	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf(">>>>>>>>>>>bigvolume:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
												signalname.c_str(),position);
				sprintf(logbuf,">>>>>>>>>>>bigvolume:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
						signalname.c_str(),position);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {

//				printf(">>>>>>>>>>>bigvolume:opentest 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
//										pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
			}
			Signal signal;
			strcpy(signal.InstrumentID,item->InstrumentID);
			strcpy(signal.SingalName,signalname.c_str());

			signal.Volume = position;
			signal.MaxSignalCount =this->GetOpenSignalCount();
			signal.Direction=0;
			signal.Offset=0;
			signal.Price=depthmarket.LastPrice;
			signal.StopRange =atr20;


			OpenSignalUpdate(&signal);

		}
		else if (condition_short  )		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf(">>>>>>>>>>>bigvolume:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																		signalname.c_str(),position);
				sprintf(logbuf,">>>>>>>>>>>bigvolume:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
						signalname.c_str(),position);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {
//				printf(">>>>>>>>>>>bigvolume:opentest 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
//							pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
			}
			Signal signal;
			strcpy(signal.InstrumentID,item->InstrumentID);
			strcpy(signal.SingalName,signalname.c_str());

			signal.Volume = position;
			signal.MaxSignalCount =this->GetOpenSignalCount();
			signal.Direction=1;
			signal.Offset=0;
			signal.Price=depthmarket.LastPrice;
			signal.StopRange =atr20;

			OpenSignalUpdate(&signal);

		}
		else	{
//						printf("开仓信号    ??????????? --> %s\n", pinstrumentID);
			SignalDispearCheck(signalname);

		}
	}

//	printf("Strategy::Open_BigVolumOneMinSignal  End!!!  \n");
}

void  BigVolume::Close()
{
	DateUtil dateutil;

//	vector<HoldData>  list;

//	this->GetMysqlHold()->Find_AllList(list);
	int detailposition= this->GetDataWrapper()->GetDetailPosition();

//	printf(">>>>>>>>>>>>>>>>>BigVolume   持仓明细表持仓数[%d] ----> %s\n",detailposition,
//		dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	if(detailposition)	{
//		printf(">>>>>>>>>>>>>>>>>BigVolume::Close: 所有持仓品种----->当日持仓批次[%ld] ----> %s\n",list.size(),
//				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

		this->DayTrade_EndTimeClose_SignalCheck();
		this->Close_DynamicStop_SignalCheck();
		this->Close_LimitPrice_SignalCheck();
		this->Close_WinPrice_SignalCheck();
		this->ClosePriceSeek();
	}
	else{
//		std::cout<<">>>>>>>hold empty"<<endl;
	}

//	vector<HoldData>().swap(list);
}

