/*
 * turtle.cpp
 *
 *  Created on: 2021年2月4日
 *      Author: qc
 */


#include "turtle.hpp"

Turtle::Turtle() {

//	printf("Turtle  无参数构造函数-->初始  \n");
}


Turtle::Turtle(DataWrapper * datawrapper,const char* strategyname) {

	this->SetRedisDepthMarket(&m_redisdepthmarket);
	this->SetRedisSignal(&m_redissignal);
	this->SetRedisSection(&m_redissection);


	this->SetDataWrapper(datawrapper);
	this->SetStrategyName(strategyname);

	Config config(this->GetStrategyName().c_str());
	std::string logname="turtle";
	logname.append("_");
	logname.append(config.GetUserID().c_str());


	std::string logfilename="turtle";
	std::string path="/var/log/autotrader/";

	logfilename.append("_");
	logfilename.append(config.GetUserID().c_str());
	logfilename.append(".log");
	this->SetLogName(logname.c_str());
	this->SetLogPathFileName(path.c_str(),logfilename.c_str());

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

//	printf("Turtle  带参数构造函数-->初始结束   Open signalcount:%d \n",this->GetOpenSignalCount());

	this->GetLogUtil()->WriteLog("Turtle  带参数构造函数-->初始结束 ");
}
void Turtle::run(){

	DateUtil dateutil;

	this->SetAvalible(this->GetInitialMoney());
	this->SetTradingDay(this->GetDataWrapper()->GetTradingDay().c_str());
	this->SetDifSec(this->GetDataWrapper()->GetDifSec());
//	this->UpdateTradingDay();
//	std::cout<<"Strategy::run sta "<<this->GetTradingDay()<<std::endl;
	 struct timeval start, m,end;

	while(1){

//		gettimeofday( &m, NULL );
//		printf(">>>>>>>>>>>>>>>m ----------------> %ld.%ld\n", m.tv_sec, m.tv_usec);

		if(this->GetDataWrapper()->GetPredataStatus()){

			gettimeofday( &start, NULL );
			printf(">>>>>>>>>>>>>>%s::run  -------------------------------------[%ld.%ld]----------------------------------------->>>>>>   [%s] \n",
					 this->GetStrategyName().c_str(),start.tv_sec, start.tv_usec,dateutil.GetCurrentSqlTimeString().c_str());
				this->Open();
				if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())){
//					this->QueueingOpenPriceChange_SignalCheck();
					this->ClosePriceSeek();
				}
				this->Close();

		}
	usleep(250000);

//		 gettimeofday( &end, NULL );
//		printf("end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
//		printf("Strategy::run  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< while end   [%s] \n",
//						dateutil.GetCurrentSqlTimeString().c_str());
	}
	return ;
}

void  Turtle::Open()
{
//		printf("StrategySpread::Open_Turtle  begin----------->  %ld \n",this->GetDataWrapper()->GetContractLists()->size());
	char logbuf[1024];
	MysqlDayLine mysqldayline;
	double avalible   = this->GetInitialMoney();

	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());

	std::string funname=">>>>>>>>>Turtle::Open";
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

	 //控制开仓时间在开盘后5分钟再操作
//	 if(!dateutil.isNightMorningBreakTime(this->GetDifSec())){
//		printf("%s----->非策略允许交易时间!!!!          \n",funname.c_str());
//		return;
//	}

	bool night=this->GetDataWrapper()->GetNightStatus();

	 struct timeval start, m,end;

	 int n =20;
	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
			item != this->GetDataWrapper()->GetContractLists()->end(); item++){

//			gettimeofday( &start, NULL );
//						printf(">>>>>>>>>>>>>>>>>>start ----------------> %ld.%ld\n", start.tv_sec, start.tv_usec);
//			std::cout<<">>>>>>>>>>>>>>--------------start:"<<start.tv_sec<<"."<<start.tv_usec<<endl;
		std::string signalname;
		signalname=item->InstrumentID;
		signalname.append("_");
		signalname.append(this->GetStrategyName());
		signalname.append("_");
		signalname.append(this->GetUserID());


//			gettimeofday( &m, NULL );
//			std::cout<<">>>>>>>>>>>>>>--------------m:"<<m.tv_sec<<"."<<m.tv_usec<<"     合约:"<<item->InstrumentID<<endl;
		if(night&& strcmp(item->DayNight,"Day")==0){
//				printf(">>>>>>>>>Strategy::Open_Turtle  >>>GetContractLists----->  无夜盘！----->ProductID:%s   exchangeid: %s \n",
//						item->ProductID, item->ExchangeID);
			continue;
		}

		GlobalViewData gbdata;
		memset(&gbdata,0,sizeof(gbdata));
		this->GetDataWrapper()->OutputGlobalViewData(&gbdata);

		if(strcmp(gbdata.AbsMaxUpDownIns,item->InstrumentID)!=0){
			continue;
		}
//////////////////////开盘时间验证///////////////////
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID);
		if (!opentime){
			printf(">>>>>>>>>   策略验证 --> [%s]非开盘时间 !!!  \n",item->InstrumentID);
			continue;
		}

//////////////////////日线数据验证///////////////////
		bool valid_dayline = this->GetDataWrapper()->FindDataStatus(item->InstrumentID);

		if (!valid_dayline){
			printf(">>>>>>>>>   策略验证 --> [%s]20天日线数据时不合规!!!  \n",item->InstrumentID);
			continue;
		}



//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(item->InstrumentID)> 0 ;
		if (!valid_redisdepth){

			if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf(">>>>>>>>>	请检查白盘合约在夜盘无数据？: %s    \n",item->InstrumentID);
				//正常情况 白盘合约在夜盘无数据
			}
			else {
				printf(">>>>>>>>>   Turtle策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",item->InstrumentID);
			}

			continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID);
		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
//				printf(">>>>>>>>>  策略验证 --> [%s]行情数据时不合规!!!  \n",item->InstrumentID);
			continue;
		}


		CycleData cycledata = mysqldayline.Collect_CycleNData(item->InstrumentID,tradingday_sqltime.c_str(),n);


		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(item->InstrumentID);
		double atr20=(totaltr+tr)/20.0;

		bool valid_stoprange=false;
		if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
			valid_stoprange=true;
		}
		else{
			valid_stoprange=false;
//				printf(">>>>>>>>>	Strategy:Open_Turtle stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,item->InstrumentID);

		}

		if (!valid_stoprange){

//				printf(">>>>>>>>>	策略验证 -->  止损区间价格不合规 --> %s atr20:%.2f \n",	item->InstrumentID,atr20);
			sprintf(logbuf,"	Strategy::Open_Turtle 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
					item->InstrumentID,atr20);
			this->GetLogUtil()->WriteLog(logbuf);

			continue;
		}



		//多头行情
		bool longcond_cycle = depthmarket.LastPrice >cycledata.High20;
		bool condition_long =  false;

		//空头行情
		bool shortcond_cycle =  depthmarket.LastPrice < cycledata.Low20;
		bool condition_short =  false;

		 condition_long =  longcond_cycle ;
		 condition_short =  shortcond_cycle ;


		std::shared_ptr<Commission>  commssion=this->GetDataWrapper()->FindCommissionData(item->ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/100000000.0;
//			printf(">>>>>>>>> 合约[%s] --->行情id[%s]  沉淀资金:%.2f   maxsed:%.2f  opi:%.2f   vm:%d    margin: %.2f   price: %.2f\n",
//					item->InstrumentID,depthmarket.InstrumentID,sedimentary,this->GetMaxSedimentary(),depthmarket.OpenInterest,
//					depthmarket.VolumeMultiple,margin,depthmarket.LastPrice);

		bool valid_sedimentary = sedimentary>this->GetMaxSedimentary();
		if (!valid_sedimentary){
			printf(">>>>>>>>>	策略验证 -->信号未激活  合约[%s] 沉淀资金:%.2f不符合 沉淀资金低于[%.2f]亿元,无法交易!!!  \n",item->InstrumentID,
					sedimentary,this->GetMaxSedimentary());
//					printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
//							pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,commission.Margin,depthmarket.LastPrice);

//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						GetLogUtil->WriteLog(logbuf);
			continue;
		}
//			gettimeofday( &end, NULL );
//			std::cout<<">>>>>>>>>>>>>>--------------end:"<<end.tv_sec<<"."<<end.tv_usec<<endl;
		if (condition_long   &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByATR(item->InstrumentID, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf(">>>>>>>>>	Strategy::Open_Turtle--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
							signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						GetLogUtil->WriteLog(logbuf);
			}
			else {

				printf(">>>>>>>>>	Strategy::Open_Turtle 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
						item->InstrumentID,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
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
		else if (condition_short   && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByATR(item->InstrumentID, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf(">>>>>>>>>	Strategy::Open_Turtle--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																		signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->->WriteLog(logbuf);
			}
			else {
			printf(">>>>>>>>>	Strategy::Open_Turtle 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
					item->InstrumentID,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
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
}

void  Turtle::Close()
{
	DateUtil dateutil;
	int holdcount=this->GetDataWrapper()->GetHoldingPosition();
	if(holdcount>0)	{
		printf(">>>>>>>>>>>>>>>>>Turtle::Close: 所有持仓品种----->当日持仓批次[%d] ----> %s\n",holdcount,
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

		this->DayTrade_EndTimeClose_SignalCheck();
		this->Close_DynamicStop_SignalCheck();
		this->Close_LimitPrice_SignalCheck();
		this->Close_WinPrice_SignalCheck();
//		this->QueueingClosePriceChange_SignalCheck();
		this->ClosePriceSeek();
	}
	else {
//		std::cout<<">>>>>>>Turtle hold empty"<<endl;
	}


}



