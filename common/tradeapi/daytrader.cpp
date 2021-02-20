/*
 * daytrade.cpp
 *
 *  Created on: 2020年3月16日
 *      Author: qc
 */

#include "daytrader.hpp"


DayTrader::DayTrader() {

}
DayTrader::DayTrader(DataWrapper * datawrapper,const char* strategyname) {

//	printf("DayTrader  带参数构造函数-->初始开始  \n");
	this->SetStrategyName(strategyname);
	this->SetDataWrapper(datawrapper);
	Config config(this->GetStrategyName().c_str());


	std::string logname="trader";
	logname.append("_");
	logname.append(config.GetUserID().c_str());


	std::string logfilename="trader";
	logfilename.append("_");
	logfilename.append(config.GetUserID().c_str());
	logfilename.append(".log");
	this->SetLogName(logname.c_str());
	this->SetLogPathFileName(logfilename.c_str());
	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());

	SetUserLoginField();
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
//	printf("DayTrader  带参数构造函数-->初始结束   Open signalcount:%d \n",this->GetOpenSignalCount());

}
DayTrader::~DayTrader() {
}
int DayTrader::GetMaxActionNumber()
{
	return this->_MAX_ACTION_NUMBER;
}
void DayTrader::TradeOpen_Test(const char* opensignal)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


//	this->GetLogUtil()->WriteLog("TradeOpen_Test---------------->begin!!!");


	std::string tradingdate;
	tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");
	std::string funname="TradeOpen_Test";

//	// 20180412   ---> 2018-04-12 00:00:00
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());

	DateUtil dateutil;
	printf("%s----->主力合约品种数[%ld] ----> %s\n",funname.c_str(),this->GetDataWrapper()->GetContractLists()->size(),
			dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
				item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);

//		printf("%s----->有行情数据 >>>>>>>>>>>>>>>>>>>>>> %s\n",funname.c_str(),item->ProductID);
		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_");
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
		if(!existsignal){
			continue;
		}


		Signal signal;
		signal=this->GetRedisSignal()->GetInsSignal(signalname);
		if (!signal.SingalActive ){ //信号未激活 则返回
			continue;
		}
//		printf("TradeOpen_Test----->信号已激活   匹配主力合约的状态----->pinstrumentid:%s   exchangeid: %s  \n",
//				pinstrumentid, item->ExchangeID);
/////////是否处于开仓状态   ///////////////////////////////////////////////////////////////////////////
		bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
		if (!orderinsertflag){
			printf("TradeOpen_Test:  	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
			continue;
		}

		// ordermap 存在ORDER 新发单受理中(多空),新发撤单(多空)
		bool exist_order = this->GetDataWrapper()->ExistinOrderMap(pinstrumentid,signal.Direction);
		if(exist_order){
			printf("TradeOpen_Test-----> %s  存在order,不再发单\n",pinstrumentid);
			continue;
		}

		int openvolume;
		int investposition=this->GetDataWrapper()->GetPositionbyInstrumentid(pinstrumentid,signal.Direction);
		openvolume=signal.Volume-investposition ;
		if(openvolume<=0){
//			printf("TradeOpen_Test:  	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
//					pinstrumentid,openvolume,signal.Volume,investposition);
//			sprintf(logbuf,"TradeOpen_Test:  	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
//										pinstrumentid,openvolume,signal.Volume,investposition);
//			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}

/////////是否存在当日平仓单   ///////////////////////////////////////////////////////////////////////////
//		MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
//		bool closeorder_count = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);


		int closedirection;
		if(signal.Direction==0){
			closedirection=1;
		}
		else {
			closedirection=0;
		}

		bool closeorder_count = this->GetDataWrapper()->ExistinCloseOrderMap(pinstrumentid,closedirection);
		if(closeorder_count){
			sprintf(logbuf,"TradeOpen_Test:  	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}


/////////止损价差是否合规   ///////////////////////////////////////////////////////////////////////////
		bool valid_stoprange = (signal.StopRange <depthmarket.LastPrice*0.05 && signal.StopRange >0.1);
		if(!valid_stoprange){
			printf("TradeOpen_Test:  	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> 最新价:%.2f StopRange:%.3f StopPrice:%.3f\n",
				pinstrumentid,depthmarket.LastPrice,signal.StopRange,signal.StopPrice);
			continue;
		}

//		printf("	111111111111111111111111111111--> %s\n",signal.SingalName);
////////////////////

		if (signal.SingalActive && !signal.OrderSend){
			printf("TradeOpen_Test:  	信号激活,合法性验证通过 --> %s\n",signal.SingalName);

//			printf("	信号激活,合法性验证通过 --> %s---多头仓位[%d] 空头仓位[%d] --发单[%d] -排队[%d]  -多头排队仓位[%d] 空头排队仓位[%d]"
//					"-----成交[%d] ------>行情时差[%d]s \n",
//					pinstrumentid,long_holdposition,short_holdposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,c_time);

			if(signal.Direction==0  && signal.Volume>investposition  ){

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓


				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,openvolume);

				sprintf(logbuf,"TradeOpen_Test:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
						pinstrumentid,openvolume,investposition);
				this->GetLogUtil()->WriteLog(logbuf);

				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				signal.OrderSend=true;

				this->GetRedisSignal()->SetInsSignal(signal,signalname);


			}
			else if(signal.Direction==1 && signal.Volume>investposition  ){

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓

				printf("TradeOpen_Test:  信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
										pinstrumentid,openvolume,investposition);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,openvolume);
				sprintf(logbuf,"TradeOpen_Test:  信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
						pinstrumentid,openvolume,investposition);
				this->GetLogUtil()->WriteLog(logbuf);

				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				signal.OrderSend=true;

				this->GetRedisSignal()->SetInsSignal(signal,signalname);
			}

		}
		else if(signal.SingalActive && signal.OrderSend){

//			printf("	信号激活 发送过委托单,通过合法性验证  %s ---多头仓位[%d] 空头仓位[%d] --> 发单数[%d] ---排队[%d]---   -多头排队仓位[%d] 空头排队仓位[%d]-成交单[%d]----> 撤单次数[%d]  \n",
//					pinstrumentid,long_holdposition,short_holdposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,cancelorder_count);

			// 没有队列排队单  1
			// 没有发送待排队单   0

//			bool maxactionnumber =cancelorder_count<=this->GetMaxCancelOrderCount() ;
//			if(!maxactionnumber){
//				printf("	激活信号,未通过验证 --> [%s]交易尝试次数过多--->[%d]，超出限额,不再交易!\n",pinstrumentid,cancelorder_count);
//				continue;
//			}


			if(signal.Direction==0 && signal.Volume>investposition   )	{

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓

				sprintf(logbuf,"TradeOpen_Test:	撤单后,再次委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]   \n",
						pinstrumentid,openvolume,investposition);
				this->GetLogUtil()->WriteLog(logbuf);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,openvolume);


			}
			else if(signal.Direction==1 && signal.Volume>investposition   )	{

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓

				sprintf(logbuf,"TradeOpen_Test:	撤单后,再次委托下空单 --> %s   ----开仓量[%d]  持仓量[%d]  \n",
						pinstrumentid,openvolume,investposition);
				this->GetLogUtil()->WriteLog(logbuf);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,openvolume);

			}
		}

	}//for loop

//std::cout<<"TradeOpen_Limit_Check  ---------------->end"<<endl;

//	this->GetLogUtil()->WriteLog("%s---------------->End!!!",funname.c_str());
}
void DayTrader::OpenSignalCheck(const char* opensignal)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


//	this->GetLogUtil()->WriteLog("TradeOpen_Limit_Check---------------->begin!!!");


	std::string tradingdate;
	tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");
	std::string funname="OpenSignalCheck";

//	// 20180412   ---> 2018-04-12 00:00:00
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());

	DateUtil dateutil;

	if(dateutil.TodayIsHoliday()){
		printf("%s----->本交易日为假日,不交易!!!!!         \n",funname.c_str());
		return;
	}

	if(dateutil.IsHolidaybyNextTradingDay()){
		printf("%s----->下一个交易日为假日,本交易日不交易!!!!!         \n",funname.c_str());
		return;
	}

	if(dateutil.isFridayNight(this->GetDifSec())){
		printf("%s----->周五夜盘不交易!!!!          \n",funname.c_str());
		return;
	}

	if(strcmp(this->GetStrategyName().c_str(),"bigvolume")==0 ||
			strcmp(this->GetStrategyName().c_str(),"bigvolume_zlqh")==0){

		// 控制开仓时间在开盘后5分钟再操作
		 if(!dateutil.isNightMorningBreakTime(this->GetDifSec())){
			printf("%s----->非策略允许交易时间!!!!          \n",funname.c_str());
			return;
		}
	}

	printf("%s----->主力合约品种数[%ld] ----> %s\n",funname.c_str(),this->GetDataWrapper()->GetContractLists()->size(),
			dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
				item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID) ;

		if(!opentime){
//			printf("%s----->%s 合约收盘时间,不交易!!!          \n",funname.c_str(),pinstrumentid);
			continue;
		}
//		std::cout<<"TradeOpen_Limit_Check  ---------------->ins:"<<pinstrumentid<<endl;

		if(strcmp(this->GetStrategyName().c_str(),"bigvolume_zlqh")==0){

			if((strcmp(item->ProductID,"rb")!=0) && (strcmp(item->ProductID,"ag")!=0)){

//			printf("TradeOpen_Limit_Check----->非交易品种 跳过 ----> %s\n",item->ProductID);
			continue;
			}
			printf("%s----->交易品种 继续 >>>>>>>>>>>>>>>>>>>>>> %s\n",funname.c_str(),item->ProductID);
		}

		bool exist_depthmarket = this->GetRedisDepthMarket()->Exists(pinstrumentid);
		if(!exist_depthmarket){
//			printf("%s----->无行情数据 >>>>>>>>>>>>>>>>>>>>>> %s\n",funname.c_str(),item->ProductID);
			continue;
		}

//		printf("%s----->有行情数据 >>>>>>>>>>>>>>>>>>>>>> %s\n",funname.c_str(),item->ProductID);
		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_");
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
		if(!existsignal){
			continue;
		}
//		printf("TradeOpen_Limit_Check----->   匹配主力合约的状态----->pinstrumentid:%s   exchangeid: %s  \n",
//				pinstrumentid, item->ExchangeID);

		Signal signal;
		signal=this->GetRedisSignal()->GetInsSignal(signalname);
		if (!signal.SingalActive ){ //信号未激活 则返回
			continue;
		}

/////////数据延迟检测   ///////////////////////////////////////////////////////////////////////////
		int c_time = dateutil.GetDiffTime(depthmarket.UpdateTime,this->GetDifSec());
		//	printf("	 ---->%s ------> depthmarket [%.6f]--------current [%.6f] ---差值[%d] \n",
		//			pinstrumentID,Q_BarTime,dateutil.GetCurrentSHFEBarTime(this->GetDifSec()),c_time);
		bool valid_delaytime=false;   // 行情数据延迟秒数

		if(	strcmp(this->GetStrategyName().c_str(),"bigvolume_zlqh")==0)	{
			valid_delaytime =c_time<10;
		}
		else{
			valid_delaytime =c_time<300;
		}

		if (!valid_delaytime){
			printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
			continue;
		}
/////////交易时间合法性验证   ///////////////////////////////////////////////////////////////////////////
		//开盘后五分钟内不交易
		bool valid_tradetime=  dateutil.CheckSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
		//开盘后一分钟内不交易
//			valid_tradetime=  dateutil.CheckDayTradeSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
		if (!valid_tradetime){
			printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
			continue;
		}

/////////当日是否确认结算信息   ///////////////////////////////////////////////////////////////////////////
		bool valid_settleconfirm = this->CheckSettleInfoConfirmbyTrade();
		if (!valid_settleconfirm){
			printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
			continue;
		}
/////////是否超出当日最大持仓比例   ///////////////////////////////////////////////////////////////////////////
		double maxmargin = this->GetTotalMaxMargin();
		bool valid_maxmargin = this->GetHoldPositionRate() <maxmargin;
		if (!valid_maxmargin){
			printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
			continue;
		}
		//				printf("	风险资金[保证金]比例 最大占用比例   %.2f%% \n",maxmargin*100);
/////////当日行情数据是否合规   ///////////////////////////////////////////////////////////////////////////
		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
			continue;
		}
/////////是否处于开仓状态   ///////////////////////////////////////////////////////////////////////////
		bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
		if (!orderinsertflag){
			printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
			continue;
		}

		int long_holdposition;
		int short_holdposition;

		// ordermap 存在ORDER 新发单受理中(多空),新发撤单(多空)


		bool exist_order = this->GetDataWrapper()->ExistinOrderMap(pinstrumentid,signal.Direction);
		if(exist_order){
			continue;
		}

		int openvolume;
		int investposition=this->GetDataWrapper()->GetPositionbyInstrumentid(pinstrumentid,signal.Direction);
		openvolume=signal.Volume-investposition ;
		if(openvolume<=0){
//			printf("	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
//					pinstrumentid,openvolume,signal.Volume,investposition);
//			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
//										pinstrumentid,openvolume,signal.Volume,investposition);
//			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}


		int closedirection;
		if(signal.Direction==0){
			closedirection=1;
		}
		else {
			closedirection=0;
		}
		bool closeorder_count = this->GetDataWrapper()->ExistinCloseOrderMap(pinstrumentid,closedirection);
		if(closeorder_count){
//			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
//			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}

		bool exist_closetrade = this->GetCTraderSpi()->ExistinCloseTradeMap(pinstrumentid);
		if(exist_closetrade){
			continue;
		}

/////////止损价差是否合规   ///////////////////////////////////////////////////////////////////////////
		bool valid_stoprange = (signal.StopRange <depthmarket.LastPrice*0.05 && signal.StopRange >0.1);
		if(!valid_stoprange){
			printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
				pinstrumentid,signal.StopRange,signal.StopPrice);
			continue;
		}


////////////////////

		if (signal.SingalActive && !signal.OrderSend){
//			printf("	信号激活,合法性验证通过 --> %s---多头仓位[%d] 空头仓位[%d] --发单[%d] -排队[%d]  -多头排队仓位[%d] 空头排队仓位[%d]"
//					"-----成交[%d] ------>行情时差[%d]s \n",
//					pinstrumentid,long_holdposition,short_holdposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,c_time);

			if(signal.Direction==0  && signal.Volume>long_holdposition  ){

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓


				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,openvolume);

				sprintf(logbuf,"OpenSignalCheck:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
						pinstrumentid,openvolume,long_holdposition);
				this->GetLogUtil()->WriteLog(logbuf);

				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				signal.OrderSend=true;

				this->GetRedisSignal()->SetInsSignal(signal,signalname);


			}
			else if(signal.Direction==1 && signal.Volume>short_holdposition  ){

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓


				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,openvolume);
				sprintf(logbuf,"OpenSignalCheck:	信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
						pinstrumentid,openvolume,short_holdposition);
				this->GetLogUtil()->WriteLog(logbuf);

				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				signal.OrderSend=true;

				this->GetRedisSignal()->SetInsSignal(signal,signalname);
			}

		}
		else if(signal.SingalActive && signal.OrderSend){

//			printf("	信号激活 发送过委托单,通过合法性验证  %s ---多头仓位[%d] 空头仓位[%d] --> 发单数[%d] ---排队[%d]---   -多头排队仓位[%d] 空头排队仓位[%d]-成交单[%d]----> 撤单次数[%d]  \n",
//					pinstrumentid,long_holdposition,short_holdposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,cancelorder_count);

			// 没有队列排队单  1
			// 没有发送待排队单   0

//			bool maxactionnumber =cancelorder_count<=this->GetMaxCancelOrderCount() ;
//			if(!maxactionnumber){
//				printf("	激活信号,未通过验证 --> [%s]交易尝试次数过多--->[%d]，超出限额,不再交易!\n",pinstrumentid,cancelorder_count);
//				continue;
//			}


			if(signal.Direction==0 && signal.Volume>long_holdposition   )	{

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓

				sprintf(logbuf,"OpenSignalCheck:	撤单后,再次委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]   \n",
						pinstrumentid,openvolume,long_holdposition);
				this->GetLogUtil()->WriteLog(logbuf);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,openvolume);


			}
			else if(signal.Direction==1 && signal.Volume>short_holdposition   )	{

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓

				sprintf(logbuf,"OpenSignalCheck:	撤单后,再次委托下空单 --> %s   ----开仓量[%d]  持仓量[%d]  \n",
						pinstrumentid,openvolume,short_holdposition);
				this->GetLogUtil()->WriteLog(logbuf);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,openvolume);

			}
		}

	}//for loop

//std::cout<<"TradeOpen_Limit_Check  ---------------->end"<<endl;

//	this->GetLogUtil()->WriteLog("%s---------------->End!!!",funname.c_str());
}
void DayTrader::CloseSignalCheck(const char * postfix)
{
	vector<HoldData>  list;
	this->GetDataWrapper()->GetHoldList(list);
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


	DateUtil dateutil;

//	if(list.size()>0)	{
		printf("平仓操作 信号分类[%s]:----->持仓批次[%ld] ----> %s\n",postfix,list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}

	for (auto &item:list)		{
		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_");
		signalname.append(postfix);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());

		char productid[30];
		CodeUtil codeutil;
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

		bool exist_depthmarket = this->GetRedisDepthMarket()->Exists(item.InstrumentID);
		if(!exist_depthmarket){
			continue;
		}

		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
		if(!existsignal){
			continue;
		}

		bool valid_opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,productid);

		if (!valid_opentime){
			continue;
		}
//		//平仓后 报错的单
//		int close_error_count = mysqlcloseorder.Count_OrderListbyStatus(item.InstrumentID,this->GetTradingDay().c_str(),-1);
		//1.如果有发送出的平仓单 Status=0
		//2.如果有发送出的平仓单,已经提交到交易所 Status=1
		//3.如果有发送出的撤单 Status=20
		//4.如果有发送出的撤单,已经提交到交易所 Status=21
		//5.如果有多头撤单量>0
		//6.如果有空头撤单量>0
		// 以上6种情况下  都不再发送任何平仓单



/////////是否处于平仓状态   ///////////////////////////////////////////////////////////////////////////
		bool closeorderinsertflag =!this->GetCTraderSpi()->GetFlagCloseOrderInsert();
		if (!closeorderinsertflag){
			printf("	激活信号,未通过验证 --> [%s]有合约处于平仓交易中,请等待!\n",item.InstrumentID);
			continue;
		}

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

		/////////当日行情数据是否合规   ///////////////////////////////////////////////////////////////////////////
		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",item.InstrumentID);
			continue;
		}

		Signal signal;
		signal=this->GetRedisSignal()->GetInsSignal(signalname);


		// ordermap 存在ORDER 新发单受理中(多空),新发撤单(多空)
		bool exist_closeorder = this->GetDataWrapper()->ExistinCloseOrderMap(item.InstrumentID,signal.Direction);
		if(exist_closeorder){
			continue;
		}

		int closevolume=0;
		int investposition=this->GetDataWrapper()->GetPositionbyInstrumentid(item.InstrumentID,signal.Direction);
		closevolume=investposition ;
		if(closevolume<=0){
			printf("	激活信号,未通过验证 --> [%s]手数不足,无法交易!--->[%d]  信号仓数:%d  持仓数:%d\n",
					item.InstrumentID,closevolume,signal.Volume,investposition);
			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]手数不足,无法交易!--->[%d]  信号仓数:%d  持仓数:%d\n",
					item.InstrumentID,closevolume,signal.Volume,investposition);
			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}

		if (signal.SingalActive && !signal.OrderSend  )	{
			sprintf(logbuf,"ClosebySignal:  信号[%s]激活 -->委托平仓单 --> %s   ---可平仓量[%d]\n",signal.SingalName,
					item.InstrumentID,item.Volume);
			this->GetLogUtil()->WriteLog(logbuf);

			if(signal.Direction==0  && closevolume>0)		{
				printf("	多头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,item.InstrumentID,
						item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
				printf("	委托平仓多单 --> %s\n", signal.InstrumentID);
				char directiontype='1';   // 0 : 买    1:卖
				char CombOffsetFlag=this->GetCombOffsetFlagbyCloseOrder(&depthmarket,&item);

				this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,item.ExchangeID,directiontype,
						CombOffsetFlag,	depthmarket.BidPrice1,closevolume,item.StopPrice);

				signal.OrderSend=true;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				this->GetRedisSignal()->SetInsSignal(signal,signal.SingalName);

			}
			else if(signal.Direction==1  && closevolume>0) {//空头出现止损

				printf("	空头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,item.InstrumentID,
						item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
				printf("	委托平仓空单 --> %s\n", signal.InstrumentID);


				char directiontype='0';   // 0 : 买    1:卖
				char CombOffsetFlag=this->GetCombOffsetFlagbyCloseOrder(&depthmarket,&item);
				this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,item.ExchangeID,directiontype,
									CombOffsetFlag,depthmarket.AskPrice1,closevolume,item.StopPrice);
				signal.OrderSend=true;
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				this->GetRedisSignal()->SetInsSignal(signal,signal.SingalName);
			}


		}else if(signal.SingalActive && signal.OrderSend )	{
			sprintf(logbuf,"ClosebySignal:  信号[%s]激活 -->再次发送委托平仓单 --> %s   ---可平仓量[%d]\n",
					signal.SingalName,item.InstrumentID,item.Volume);
			this->GetLogUtil()->WriteLog(logbuf);

			if(signal.Direction==0   && closevolume>0)		{
				printf("	多头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
						item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
				printf("	委托平仓多单 --> %s\n", signal.InstrumentID);


				char directiontype='1';   // 0 : 买    1:卖
				char CombOffsetFlag=this->GetCombOffsetFlagbyCloseOrder(&depthmarket,&item);
				this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,item.ExchangeID,directiontype,
								CombOffsetFlag,	depthmarket.BidPrice1,closevolume,item.StopPrice);

			}
			else if(signal.Direction==1  && closevolume>0) {//空头出现止损

				printf("	空头合约平仓 [%s]-->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signal.SingalName,signal.InstrumentID,
						item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
				printf("	委托平仓空单 --> %s\n", signal.InstrumentID);

				char directiontype='0';   // 0 : 买    1:卖
				char CombOffsetFlag=this->GetCombOffsetFlagbyCloseOrder(&depthmarket,&item);

				this->ReqSqlCloseOrderInsert(signal.SingalName,signal.InstrumentID,item.ExchangeID,directiontype,
							CombOffsetFlag,depthmarket.AskPrice1,closevolume,item.StopPrice);
			}
		}

	}//for loop

	vector<HoldData>().swap(list);
}



// 开仓撤单
void DayTrader::QueueingPriceChange_CancelCheck()
{
	DateUtil dateutil;
	char logbuf[1024];
	vector<OrderData>  list;
	this->GetDataWrapper()->GetOpenOrderQueueList(list);
	if(list.size()>0)	{
		printf("Action [追加队列跳跃点数撤单]:----->队列排队合约数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoString(this->GetDifSec()).c_str());
	}

	for (auto &item:list)	{

//				printf("追加队列跳跃点数撤单----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//													item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());

		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_pricechange");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
		if(!existsignal){
			continue;
		}

		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID) ;

		if(!opentime){
			printf("QueueingPriceChange_CancelCheck----->%s 合约收盘时间,无法启动追价!!!          \n",item.InstrumentID);
			continue;
		}

		//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
		Signal signal;
		signal=this->GetRedisSignal()->GetInsSignal(signalname);

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);


		if (signal.SingalActive ==true && signal.OrderSend==false)		{

				if(item.Direction==0 )   {// 多头开仓追价撤单

					printf("多头合约开仓单撤单 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
							item.LimitPrice,item.StopPrice,depthmarket.LastPrice);
					printf("多头委托开仓单撤单 --> %s\n", item.InstrumentID);

					sprintf(logbuf,"多头合约开仓单撤单 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
														item.LimitPrice,item.StopPrice,depthmarket.LastPrice);
					this->GetLogUtil()->WriteLog(logbuf);

					this->ReqSqlOrderAction(signalname.c_str(),item.InstrumentID,item.ExchangeID,item.OrderRef,
							item.FrontID,item.SessionID);
					signal.OrderSend=true;
					this->GetRedisSignal()->SetInsSignal(signal,signalname);
				}
				else if(item.Direction==1 )  {//空头开仓追价撤单

					printf("空头合约开仓单撤单 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
							item.LimitPrice,item.StopPrice,depthmarket.LastPrice);
					printf("空头委托开仓单撤单 --> %s\n", item.InstrumentID);

					printf(logbuf,"空头合约开仓单撤单 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
														item.LimitPrice,item.StopPrice,depthmarket.LastPrice);
					this->GetLogUtil()->WriteLog(logbuf);

					this->ReqSqlOrderAction(signalname.c_str(),item.InstrumentID,item.ExchangeID,item.OrderRef,
							item.FrontID,item.SessionID);
					signal.OrderSend=true;
					this->GetRedisSignal()->SetInsSignal(signal,signalname);
				}
			}

	}
	vector<OrderData>().swap(list);
}



//平仓撤单
void DayTrader::QueueingClosePriceChange_CancelCheck()
{
	Config config(this->GetStrategyName().c_str());
	DateUtil dateutil;

	char logbuf[2048];

	vector<OrderData>  list;
	this->GetDataWrapper()->GetCloseOrderQueueList(list);
	if(list.size()>0)	{
		printf("Action [平仓单撤单]----->队列排队合约数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoString(this->GetDifSec()).c_str());
	}

	for (auto &item:list)	{

//				printf("追加队列跳跃点数撤单----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//													item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_closepricechange");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
			if(!existsignal){
				continue;
			}

			bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID) ;

			if(!opentime){
				printf("QueueingClosePriceChange_CancelCheck----->%s 合约收盘时间,无法启动追价!!!          \n",item.InstrumentID);
				continue;
			}

				//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
			Signal signal;
			signal=this->GetRedisSignal()->GetInsSignal(signalname);

			DepthMarket depthmarket;
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);


			if (signal.SingalActive ==true && signal.OrderSend==false)	{
					sprintf(logbuf,"	平仓单撤单信号[%s]激活---> %s \n",signalname.c_str(),item.InstrumentID);
					this->GetLogUtil()->WriteLog(logbuf);

					// 多头开仓追价撤单
					if(item.Direction==0 )			{
						sprintf(logbuf,"	平空单撤单 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.LimitPrice,item.StopPrice,depthmarket.LastPrice);
						this->GetLogUtil()->WriteLog(logbuf);
						printf("	平空单撤单 --> %s\n", item.InstrumentID);

						this->ReqSqlCloseOrderAction(signalname.c_str(),item.InstrumentID,item.ExchangeID,item.OrderRef,
								item.FrontID,item.SessionID);
						signal.OrderSend=true;
						this->GetRedisSignal()->SetInsSignal(signal,signalname);
					}

					//空头开仓追价撤单
					else if(item.Direction==1 )		{
						sprintf(logbuf,"	平多单撤单 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
								item.LimitPrice,item.StopPrice,depthmarket.LastPrice);
						this->GetLogUtil()->WriteLog(logbuf);

						printf("	平多单撤单 --> %s\n", item.InstrumentID);

						this->ReqSqlCloseOrderAction(signalname.c_str(),item.InstrumentID,item.ExchangeID,item.OrderRef,
										item.FrontID,item.SessionID);
						signal.OrderSend=true;
						this->GetRedisSignal()->SetInsSignal(signal,signalname);
					}
				}

		}
	vector<OrderData>().swap(list);
}

int DayTrader::UpdateInvestPosition()
{
	int num;
	std::shared_ptr<AssetData> assetdata = this->GetDataWrapper()->FindAssetData(this->GetUserID().c_str());

	this->SetAvalible(assetdata->PreBalance);
	//资金利用率  --> 仓位
	double holdpositionrate= assetdata->CurrMargin/assetdata->PreBalance;
	this->SetHoldPositionRate(holdpositionrate);

	 printf("资金账户[%s]       交易日[%s]查询时间[%s] 静态权益:%.3f  动态权益:%.3f  保证金:%.3f[%.2f%%]  可用资金:%.3f 浮盈:%.3f\n",
			 assetdata->AccountID,assetdata->TradingDay,assetdata->LocalUpdatetime,assetdata->PreBalance,
			 assetdata->DynamicEquity,assetdata->CurrMargin,holdpositionrate*100,
			 assetdata->Available,assetdata->HoldProfit);
	 printf("资金账户[%s]       手续费:%.3f  出金:%.3f  入金:%.3f  \n",
			 assetdata->AccountID,assetdata->Commission,assetdata->Withdraw,assetdata->Deposit);


	 //总持仓量
	vector<InvestPositionData> investlist;
	this->GetDataWrapper()->GetInvestList(investlist);
	int investholdposions=this->GetDataWrapper()->GetHoldingPosition();
	int total_closevolume=this->GetDataWrapper()->GetCloseVolume();
	//
	this->SetTotalHolding(investlist.size());
//	 std::cout<<"DayTrader::UpdateInvestPosition---------->总持仓合约品种:"<<investlist.size()
//			 <<"总仓位单数:"<<investholdposions<<std::endl;

	 printf("DayTrader::UpdateInvestPosition---------->总持仓合约品种: %ld  总持仓单:%d  总平仓单:%d\n",investlist.size(),investholdposions,total_closevolume);
	for (auto &item:investlist)	{
		if(item.Position>0 ){
			printf("	持仓合约: ----->  %s ---> 方向:%d  手数:%d  开仓均价: %.3f OpenAmount:%.2f OpenVolume:%d 占用保证金: %.3f[%.2f%%]  持仓浮盈:%.3f\n",
						item.InstrumentID,item.PosiDirection,item.Position,item.OpenPriceAverage,item.OpenAmount,item.OpenVolume,
						item.UseMargin,item.UseMargin/assetdata->PreBalance*100,item.PositionProfit);
		}
		else if(item.Position==0 && item.CloseVolume>0){

			printf("	平仓合约: ----->  %s ---> 方向:%d  手数:%d  开仓均价: %.3f  OpenVolume:%d  CloseAmount:%.3f  CloseVolume:%d 平仓浮盈:%.3f\n",
						item.InstrumentID,item.PosiDirection,item.Position,item.OpenPriceAverage,item.OpenVolume,
						item.CloseAmount,item.CloseVolume,item.CloseProfit);
		}
		else if(item.Position==0 && item.OpenVolume==0 && item.CloseVolume==0){
			printf("	未成交合约: ----->  %s ---> 方向:%d  手数:%d  OpenVolume:%d CloseAmount:%.3f  CloseVolume:%d \n",
					item.InstrumentID,item.PosiDirection,item.Position,item.OpenVolume,
					item.CloseAmount,item.CloseVolume);
		}
	}

	vector<PositionDetailData> detaillist;
	this->GetDataWrapper()->GetPositionDetailList(detaillist);
	int position=this->GetDataWrapper()->GetDetailPosition();
	std::cout<<"DayTrader::UpdateInvestPosition-------->detail 品种:"<<detaillist.size()<<"   detail 总仓位单数:"<<position<<std::endl;
	for (auto &item:detaillist)	{
		printf("	持仓明细: ----->  %s ---> 方向:%d  手数:%d  开仓价:%.2f  保证金:%.2f TradeID:%s  opendate:%s\n",
				item.InstrumentID,item.Direction,item.Volume,item.OpenPrice,item.Margin,item.TradeID,item.OpenDate);
	}

	vector<InvestPositionData>().swap(investlist);
	vector<PositionDetailData>().swap(detaillist);

	if(investholdposions!=position){
		this->SetFlagInvestUpdate(true);
	}
	return num;

}

HoldData DayTrader::CollectHoldDatabyInvest(InvestPositionData *item)
{
	HoldData holddata;
	strcpy(holddata.InstrumentID,item->InstrumentID);
	strcpy(holddata.ExchangeID,item->ExchangeID);
	holddata.OpenPriceAverage = item->OpenPriceAverage;

	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID);

	double tr =this->GetCurrentDayTRbyDepth(&depthmarket);
	double totaltr=this->GetDataWrapper()->FindTotalTR(item->InstrumentID);
	double atr20=(totaltr+tr)/20;
	holddata.StopRange = atr20;
	holddata.Highest =item->OpenPriceAverage;
	holddata.Lowest =item->OpenPriceAverage;
	holddata.Volume=item->Position;

	if(item->PosiDirection==0){  //long
		holddata.Direction=0;
		holddata.WinPrice = item->OpenPriceAverage+atr20*this->GetWinRate();
		holddata.StopPrice = item->OpenPriceAverage-atr20*this->GetStopRate();
	}
	else if(item->PosiDirection==1){ //short
		holddata.Direction=1;
		holddata.WinPrice = item->OpenPriceAverage-atr20*this->GetWinRate();
		holddata.StopPrice = item->OpenPriceAverage+atr20*this->GetStopRate();
	}
	return holddata;
}
bool DayTrader::UpdateHoldDatabyInvest(InvestPositionData *item,HoldData &holddata)
{
	char logbuf[1024];
	bool flag=false;
	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID);

	if(holddata.StopRange==0.0){
		double tr =this->GetCurrentDayTRbyDepth(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(item->InstrumentID);
		double atr20=(totaltr+tr)/20;
		holddata.StopRange = atr20;
		sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 stoprange %.2f ",holddata.InstrumentID,holddata.StopRange);
		this->GetLogUtil()->WriteLog(logbuf);
		flag=true;
	}

	if(holddata.StopPrice==0.0){
		if(item->PosiDirection==0){  //long
			holddata.Direction=0;
			holddata.StopPrice = item->OpenPriceAverage-holddata.StopRange*this->GetStopRate();
			sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 StopPrice %.2f ",holddata.InstrumentID,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
		else if(item->PosiDirection==1){ //short
			holddata.Direction=1;
			holddata.StopPrice = item->OpenPriceAverage+holddata.StopRange*this->GetStopRate();
			sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 StopPrice %.2f ",holddata.InstrumentID,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);

			flag=true;
		}
	}

	if(holddata.WinPrice==0.0){
		if(item->PosiDirection==0){  //long
			holddata.Direction=0;
			holddata.WinPrice = item->OpenPriceAverage+holddata.StopRange*this->GetWinRate();
			sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 WinPrice %.2f ",holddata.InstrumentID,holddata.WinPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
		else if(item->PosiDirection==1){ //short
			holddata.Direction=1;
			holddata.WinPrice = item->OpenPriceAverage-holddata.StopRange*this->GetWinRate();
			sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 WinPrice %.2f ",holddata.InstrumentID,holddata.WinPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
	}

	if(item->Position!=holddata.Volume){
		holddata.Volume=item->Position;
		sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 Volume %d ",holddata.InstrumentID,holddata.Volume);
		this->GetLogUtil()->WriteLog(logbuf);
		flag=true;
	}
	if(item->PosiDirection==0){  //long
		holddata.Direction=0;
		if(depthmarket.LastPrice>holddata.Highest){
			holddata.Highest=depthmarket.LastPrice;
			holddata.StopPrice=depthmarket.LastPrice-holddata.StopRange;
			sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 Highest %.2f StopPrice %.2f",holddata.InstrumentID,
					holddata.Highest,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}

	}
	else if(item->PosiDirection==1){ //short
		holddata.Direction=1;
		if(depthmarket.LastPrice<holddata.Lowest){
			holddata.Lowest=depthmarket.LastPrice;
			holddata.StopPrice=depthmarket.LastPrice+holddata.StopRange;
			sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新 Lowest %.2f StopPrice %.2f",holddata.InstrumentID,
					holddata.Lowest,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
	}

	if(strlen(holddata.OpenDate)<8){
		std::string opendate=this->GetDataWrapper()->CollectOpenDatebyPosDetail(holddata.InstrumentID).c_str();
		sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新前 hold opendate: %s--->%s",
				holddata.InstrumentID,holddata.OpenDate,opendate.c_str());
		strcpy(holddata.OpenDate,opendate.c_str());
		sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->更新后 opendate :%s",holddata.InstrumentID,holddata.OpenDate);
		this->GetLogUtil()->WriteLog(logbuf);
		flag=true;
	}

	if(flag){
//		sprintf(logbuf,"DayTrader::UpdateHoldDatabyInvest  %s-->holdmap 数据更新",holddata.InstrumentID);
//		this->GetLogUtil()->WriteLog(logbuf);

		this->GetDataWrapper()->UpdateHoldData(holddata);
	}
return flag;
}
void DayTrader::SyncHoldInvestMap()
{
	char logbuf[1024];
	vector<InvestPositionData>  list;

	this->GetDataWrapper()->GetHoldingInvestList(list);
	for (auto &item:list){

		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(item.InstrumentID)> 0 ;
		if (!valid_redisdepth){
			printf("SyncHoldInvestMap   %s->不存在行情数据,无法更新持仓列表!!! \n",item.InstrumentID);
//			sprintf(logbuf,"SyncHoldInvestMap   %s->不存在行情数据,无法更新持仓列表!!! \n",item.InstrumentID);
//			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}

//		printf("SyncHoldInvestMap   %s->%d\n",item.InstrumentID,item.PosiDirection);

		HoldData f_data= this->GetDataWrapper()->FindHoldData(item.InstrumentID,item.PosiDirection);
		if(strlen(f_data.InstrumentID)==0 && item.Position>0){    //存在investposition position大于0 插入持仓列表
//			sprintf(logbuf,"SyncHoldInvestMap   %s->新插入 holddata   ",item.InstrumentID);
			this->GetLogUtil()->WriteLog(logbuf);
			HoldData holddata;
			memset(&holddata,0,sizeof(holddata));
			holddata=this->CollectHoldDatabyInvest(&item);
//			cout<<"insert holddata by invest "<<holddata.OpenDate<<endl;
			this->GetDataWrapper()->InsertHoldData(holddata);
		}
		else{

			if(item.Position==0 ){
//				sprintf(logbuf,"SyncHoldInvestMap   %s->删除 holddata\n",item.InstrumentID);
//				this->GetLogUtil()->WriteLog(logbuf);
				this->GetDataWrapper()->DeleteHoldData(item.InstrumentID,item.PosiDirection);
			}
			else{
//				sprintf(logbuf,"SyncHoldInvestMap   %s->更新 holddata %s \n",item.InstrumentID,f_data.OpenDate);
//				this->GetLogUtil()->WriteLog(logbuf);
				this->UpdateHoldDatabyInvest(&item,f_data);

			}
		}
	}

	vector<InvestPositionData>().swap(list);


	vector<HoldData>  holdlist;
	this->GetDataWrapper()->GetHoldList(holdlist);

	printf("        持仓合约品种:%ld   \n",holdlist.size());
	for (auto &item:holdlist)		{
		printf("hold    合约:%s   开仓价:%.2f  方向:%d 手数: %d   止损价差:%.2f  stopprice:%.2f   winprice:%.2f  opendate:%s\n",
				item.InstrumentID,item.OpenPriceAverage,item.Direction,item.Volume,item.StopRange,item.StopPrice,
				item.WinPrice,item.OpenDate);

		if(this->GetDataWrapper()->GetPositionbyInstrumentid(item.InstrumentID,item.Direction)==0){
			printf("SyncHoldInvestMap   %s->删除 holddata\n",item.InstrumentID);
			this->GetDataWrapper()->DeleteHoldData(item.InstrumentID,item.Direction);
		}
	}
	vector<HoldData>().swap(holdlist);


}
