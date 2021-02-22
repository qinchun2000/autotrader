/*
 * test.cpp
 *
 *  Created on: 2020年3月10日
 *      Author: qc
 */





#include "statest.hpp"

StaTest::StaTest() {
//	printf("StaTest  无参数构造函数-->初始  \n");

}
StaTest::~StaTest() {

}

StaTest::StaTest(DataWrapper * datawrapper,const char* strategyname) {

	this->SetRedisDepthMarket(&m_redisdepthmarket);
	this->SetRedisSignal(&m_redissignal);
	this->SetRedisSection(&m_redissection);

//	printf("StaTest  带参数构造函数-->初始开始  \n");
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
	std::string path = "/var/log/autotrader/";
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

//	printf("StaTest  带参数构造函数-->初始结束   Open signalcount:%d \n",this->GetOpenSignalCount());

	this->GetLogUtil()->WriteLog("StaTest  带参数构造函数-->初始结束 ");
}
void StaTest::run(){

	DateUtil dateutil;

	this->SetAvalible(this->GetInitialMoney());
	this->SetTradingDay(this->GetDataWrapper()->GetTradingDay().c_str());
	this->SetDifSec(this->GetDataWrapper()->GetDifSec());
//	this->UpdateTradingDay();
//	std::cout<<"Strategy::run sta "<<this->GetTradingDay()<<std::endl;
	 struct timeval start, m,end;

	while(1){

//		gettimeofday( &m, NULL );
//		printf(">>>>>>>>>>>>>>>StaTest::run() ----------------> %ld.%ld\n", m.tv_sec, m.tv_usec);

		if(this->GetDataWrapper()->GetPredataStatus()){

			gettimeofday( &start, NULL );
	//		printf("start ----------------> %ld.%ld\n", start.tv_sec, start.tv_usec);
//			printf(">>>>>>>>>>>>>>%s::run  -------------------------------------[%ld.%ld]----------------------------------------->>>>>>   [%s] \n",
//					 this->GetStrategyName().c_str(),start.tv_sec, start.tv_usec,dateutil.GetCurrentSqlTimeString().c_str());


				this->Open();
				this->OpenPriceSeek();
//				if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())){
////					this->QueueingOpenPriceChange_SignalCheck();
//					OpenPriceSeek();
//				}
				this->Close();

		}
//		sleep (1);
		usleep(500000);
//		sleep(5);

//		 gettimeofday( &end, NULL );
//		printf("end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
//		printf("Strategy::run  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< while end   [%s] \n",
//						dateutil.GetCurrentSqlTimeString().c_str());
	}



	return ;
}


void  StaTest::Open()
{
//	printf("Strategy::Open_BigVolumOneMinSignal  begin----------->  \n");

	char logbuf[1024];
	double avalible = this->GetInitialMoney() ;
//	printf(">>>>>>>>>>>>>>>StaTest::Open  begin----------->	 可用资金: %.2f  \n",avalible);
	 double bigvolumerate =this->GetBigVolumeRate();

	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<">>>>>>>>>>>>>>> tradingday_sqltime:"<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());



//	printf(">>>>>>>>>>>>>>>>>>>----->GetContractLists       %ld \n", this->GetDataWrapper()->GetContractLists()->size());

	 bool night=this->GetDataWrapper()->GetNightStatus();
	CodeUtil codeutil;

	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
			item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);

////////////////////当日行情数据验证///////////////////
		bool valid_mindata = this->m_redismindata.Exists(pinstrumentid)> 0 ;
		if (!valid_mindata){
			printf("   statest开仓策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
			continue;
		}

		RMinData rmindata;
		rmindata=this->m_redismindata.GetMinData(pinstrumentid);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_");
		signalname.append(this->GetStrategyName());
		signalname.append("_");
		signalname.append(this->GetDataWrapper()->GetUserID());

		char productid[31];
		strcpy(productid,item->ProductID);

//		if(strcmp(productid,"SR")!=0){
//			continue;
//		}
		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		std::shared_ptr<CycleData> pcycledata = this->GetDataWrapper()->Find5DayCycle(pinstrumentid);
		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(pinstrumentid);
		double atr20=(totaltr+tr)/20;


		bool cond_vol = (double) rmindata.Volume > (double)rmindata.OpenInterest*bigvolumerate;

//		if(cond_vol){
//			printf(">>>>>>>>>>>成交量波动   %s :             volume: %d         opi:%d\n",
//					pinstrumentid,rmindata.Volume,rmindata.OpenInterest);
//		}
//		else{
//			printf(">>>>>>>>>>>成交量无波动   %s :             volume: %d         opi:%d      rate:[%.2f%%]\n",
//								pinstrumentid,rmindata.Volume,rmindata.OpenInterest,
//								(double)rmindata.Volume/(double)rmindata.OpenInterest*100);
//		}
		//多头行情
		bool longcond_cycle = depthmarket.LastPrice >pcycledata->High20;
		bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice&& depthmarket.OpenPrice>depthmarket.PreSettlementPrice;
		bool condition_long =  longcond_kline  ;

		//空头行情
		bool shortcond_cycle =  depthmarket.LastPrice < pcycledata->Low20;
		bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice && depthmarket.OpenPrice<depthmarket.PreSettlementPrice;

		bool condition_short = shortcond_kline;

		std::shared_ptr<Commission>  commssion=this->GetDataWrapper()->FindCommissionData(item->ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/100000000.0;

		bool valid_sedimentary = sedimentary>this->GetMaxSedimentary();
		if (!valid_sedimentary){
//			printf("	策略验证 -->信号未激活  合约[%s] 沉淀资金:%.2f不符合 沉淀资金低于[%.2f]亿元,无法交易!!!  \n",pinstrumentid,
//					sedimentary,this->GetMaxSedimentary());
			continue;
		}

		if (condition_long   )	{
//			printf(" 买: %f  卖: %f lastprice : %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//					depthmarket.LastPrice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
//				printf(">>>>>>>>>>>StaTest:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//												signalname.c_str(),position);
//				sprintf(logbuf,">>>>>>>>>>>StaTest:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//						signalname.c_str(),position);
//				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {

//				printf(">>>>>>>>>>>StaTest:opentest 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
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
//			printf("买: %f  卖: %f lastprice : %f  pricetick: %f\n",depthmarket.AskPrice1,
//					depthmarket.BidPrice1,depthmarket.LastPrice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
//				printf(">>>>>>>>>>>StaTest:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//																		signalname.c_str(),position);
//				sprintf(logbuf,">>>>>>>>>>>StaTest:opentest--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//						signalname.c_str(),position);
//				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {
//				printf(">>>>>>>>>>>StaTest:opentest 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
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

void  StaTest::Close()
{
	DateUtil dateutil;
	int detailposition= this->GetDataWrapper()->GetDetailPosition();
//	printf(">>>>>>>>>>>>>>>>>StaTest   持仓明细表持仓数[%d] ----> %s\n",detailposition,
//		dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	if(detailposition)	{
		printf(">>>>>>>>>>>>>>>>>StaTest::Close: 所有持仓品种----->持仓明细表持仓数[%d] ----> %s\n",detailposition,
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		this->DayTrade_EndTimeClose_SignalCheck();
		this->Close_DynamicStop_SignalCheck();
		this->Close_LimitPrice_SignalCheck();
		this->Close_WinPrice_SignalCheck();
		this->ClosePriceSeek();
	}
	else{
//		std::cout<<">>>>>>>hold empty"<<endl;
	}


}

