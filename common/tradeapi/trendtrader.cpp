#include "trendtrader.hpp"


TrendTrader::TrendTrader() {

}
TrendTrader::TrendTrader(const char* strategyname) {

	printf("TrendTrader  带参数构造函数-->初始开始  \n");
	this->SetStrategyName(strategyname);
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

	LogUtil * mylog= new LogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->SetLogUtil(mylog);

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
	printf("TrendTrader  带参数构造函数-->初始结束   Open signalcount:%d \n",this->GetOpenSignalCount());

}
TrendTrader::~TrendTrader() {
}


int TrendTrader::GetMaxActionNumber()
{
	return this->_MAX_ACTION_NUMBER;
}


void TrendTrader::TradeOpenCheck(const char* opensignal)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->begin!!!");

	MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
	MysqlDepthMarket mysqldepthmarket;

	std::string tradingdate;
	tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");

//	// 20180412   ---> 2018-04-12 00:00:00
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());

//	CheckLocalOrderList(tablename.c_str());  //如果有发送的委托单,一直处于status=0 状态,需要确认是否已经到达后台,如过后台不存在该委托单,则更新状态未 error
	MysqlProduct mysqlproduct;
	MysqlSecAssess mysqlsecassess(this->GetPointUserLoginField()->UserID);
	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_TradeSectionList(list);
	DateUtil dateutil;
	printf("TradeOpenCheck----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (auto &item:list)	{

		char pinstrumentid[40];
		strcpy(pinstrumentid,item.InstrumentID);
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID) ;

		bool exist_depthmarket = this->GetRedisDepthMarket()->Exists(pinstrumentid);

		DepthMarket depthmarket;
		if(exist_depthmarket){
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
		}
		std::string signalname;
		signalname=pinstrumentid;
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;

	//	printf("TradeOpenCheck----->   匹配主力合约的状态----->pinstrumentid:%s   exchangeid: %s  \n", pinstrumentID, item.ExchangeID);

		bool openlist = strcmp(item.Section,"Finance")!=0;

		if ( opentime && existsignal   && exist_depthmarket && openlist )	{
//		if (1){
				Signal signal;
				signal=this->GetRedisSignal()->GetInsSignal(signalname);

				//持有多头仓位
				int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
				//多头请求开仓仓位
				int longreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());

				//多头请求撤单的仓位
				int longactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());

				//持有空头仓位
				int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

				//空头请求开仓仓位
				int shortreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());

				// 空头请求撤单的仓位
				int shortactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());

				//合约对应的板块 总开仓合约数是否超过限制

				ProductData productdata;
				mysqlproduct.Find_DatabyProductID(productdata,item.ProductID);

	//			if(longposition>0 || shortposition >0)
	//			{
					printf("%s--------------->持仓----多头仓位[%d] 空头仓位[%d] \n",pinstrumentid,longposition,shortposition);
	////				break;
	//			}
	//			printf("%s--------------->空仓 -多头仓位[%d] 空头仓位[%d]\n",pinstrumentID,longposition,shortposition);

				int v;
				char char_times[10];
				strcpy(char_times, depthmarket.UpdateTime);
				std::string str_time=char_times;
				int hours = atoi(str_time.substr(0,2).c_str());
				int minutes = atoi(str_time.substr(3,2).c_str());
				int seconds = atoi(str_time.substr(6,2).c_str());
				double Q_BarTime	= (1/10e1)*hours + (1/10e3)*minutes + (1/10e5)*seconds;
				int c_time = (int)(fabs(Q_BarTime-dateutil.GetCurrentSHFEBarTime(this->GetDifSec()))*10e5);
	//					printf("TradeOpenCheck ---->%s ------> depthmarket [%.6f]--------current [%.6f] ---差值[%d] \n",
	//							pinstrumentID,Q_BarTime,dateutil.GetCurrentSHFEBarTime(this->GetDifSec()),c_time);
				bool valid_delaytime =c_time<300;   // 行情数据延迟秒数
				bool valid_tradetime;
				if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
					valid_tradetime=  dateutil.CheckSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
				}
				else{
					valid_tradetime=  dateutil.CheckDayTradeSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
				}


				bool valid_settleconfirm = this->CheckSettleInfoConfirmbyTrade();

				double maxmargin = this->GetTotalMaxMargin();
				bool valid_maxmargin = this->GetHoldPositionRate() <maxmargin;
//				printf("	风险资金[保证金]比例 最大占用比例   %.2f%% \n",maxmargin*100);

				int send_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),0);   //发送了, 后台未受理
	//					printf("信号激活 --> %s----发单数[%d]  \n",pinstrumentID,send_count);
				int queue_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),1);  //交易队列中
	//					printf("信号激活 --> %s----排队[%d]  \n",pinstrumentID,queue_count);
				int done_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);  // 已成交
	//					printf("信号激活 --> %s----成交数[%d]  \n",pinstrumentID,done_count);
				int action_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),22); //有撤单,控制撤单次数
	//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);

				MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
				bool closeflag = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);
				bool section_closeflag = mysqlcloseorder.Exist_CloseOrderbySection(item.Section,tradingdate.substr(0,8).c_str());
				bool valid_stoprange = (signal.StopRange <depthmarket.LastPrice*0.05 && signal.StopRange >0.1);
//				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);
//
//				bool long_valid1 =  depthmarket.AskPrice1 >data.LowerLimitPrice;
//				bool long_valid2 = depthmarket.AskPrice1 < data.UpperLimitPrice;
//				bool longopen_valid = long_valid1 && long_valid2 ;
//
//				bool short_valid1 =  depthmarket.BidPrice1 >data.LowerLimitPrice;
//				bool short_valid2 = depthmarket.BidPrice1 < data.UpperLimitPrice;
//				bool shortopen_valid = long_valid1 && long_valid2  ;

				if (signal.SingalActive && !signal.OrderSend){

					printf("	信号激活,等待验证 --> %s---多头仓位[%d] 空头仓位[%d] --发单[%d] -排队[%d]  -多头排队仓位[%d] 空头排队仓位[%d]-----成交[%d] ------>行情时差[%d]s \n",
								pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,c_time);

					bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					bool valid_open = valid_delaytime &&  valid_tradetime && valid_maxmargin &&  !closeflag  && !section_closeflag
							&& queue_count==0 && send_count==0  && done_count==0
							&& orderinsertflag  && valid_depthmarket  && valid_settleconfirm && valid_stoprange;
					if(valid_open)	{
						printf("	信号激活 -->合法性验证通过 --> %s\n", pinstrumentid);
						if(signal.Direction==0  && signal.Volume>longposition  ){

							char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
							char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
							v=signal.Volume-longposition -longreqvolume -longactionvolume;
							if(v>0)	{

								MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
								int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
								if (v-position>0)	{

									v=v-position;
									printf("	信号激活 -->委托下多单 --> %s   ----开仓量[%d]\n", pinstrumentid,v);
	//								this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,CombOffsetFlag,
	//																	depthmarket.AskPrice1,v,signal.StopPrice);

									this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.AskPrice1,v);

									sprintf(logbuf,"TradeOpenCheck:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
											pinstrumentid,v,longposition,longreqvolume,longactionvolume);
									this->GetLogUtil()->WriteLog(logbuf);

									std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
									strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
									signal.OrderSend=true;

									this->GetRedisSignal()->SetInsSignal(signal,signalname);
								}
							}
						}
						else if(signal.Direction==1 && signal.Volume>shortposition  ){
							printf("	信号激活 -->委托下空单 --> %s\n", pinstrumentid);
							char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
							char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
							v=signal.Volume-shortposition -shortreqvolume -shortactionvolume;
							if(v>0)		{

								MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
								int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
								if (v-position>0)	{
									v=v-position;
	//								this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,CombOffsetFlag,
	//																								depthmarket.BidPrice1,v,signal.StopPrice);
									this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.BidPrice1,v);
									sprintf(logbuf,"TradeOpenCheck:	信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
											pinstrumentid,v,shortposition,shortreqvolume,shortactionvolume);
									this->GetLogUtil()->WriteLog(logbuf);

									std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
									strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
									signal.OrderSend=true;

									this->GetRedisSignal()->SetInsSignal(signal,signalname);
								}
							}
						}
//						else if(closeflag)	{
//							printf("	信号未激活 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
//						}

					}
					else{


						if (!valid_delaytime){
							printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
							sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
//							openthis->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_tradetime){
							printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
						}
						if(!valid_maxmargin){
							printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
						}
						if(closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
						}
						if(section_closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
						}
						if(queue_count>0){
							printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
						}
						if(send_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
						}
						if(done_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
						}
						if(!orderinsertflag){
							printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
						}

						if(!valid_depthmarket){
							printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
						}
						if(!valid_settleconfirm){
							printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
						}
						if(!valid_stoprange){
							printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
									pinstrumentid,signal.StopRange,signal.StopPrice);
						}

					}
				}
				else if(signal.SingalActive && signal.OrderSend){

					printf("	信号激活 发送过委托单,等待验证 %s ---多头仓位[%d] 空头仓位[%d] --> 发单数[%d] ---排队[%d]---   -多头排队仓位[%d] 空头排队仓位[%d]-成交单[%d]----> 撤单次数[%d]  \n",
							pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,action_count);

					// 没有队列排队单  1
					// 没有发送待排队单   0

					bool maxactionnumber =action_count<=this->GetMaxActionNumber() ;
					bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					bool valid_open =valid_delaytime &&  valid_tradetime && valid_maxmargin &&    !closeflag && !section_closeflag
							&& queue_count==0 && maxactionnumber && send_count==0 && done_count==0 && orderinsertflag
							&& valid_depthmarket && valid_settleconfirm&& valid_stoprange;
					if(valid_open)	{
						printf("	信号激活 发送过委托单,通过合法性验证 %s \n",pinstrumentid);
							if(signal.Direction==0 && signal.Volume>longposition   )	{
								printf("	撤单后,再次委托下多单 --> %s\n", pinstrumentid);
								char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
								char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
								v=signal.Volume-longposition -longreqvolume -longactionvolume;
								if(v>0)	{
									MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
									int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
									if (v-position>0)	{
										v=v-position;
										sprintf(logbuf,"TradeOpenCheck:	第[%d]次撤单后,再次委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]   撤单队列仓量[%d]\n",
												action_count,pinstrumentid,v,longposition,longreqvolume,longactionvolume);
										this->GetLogUtil()->WriteLog(logbuf);
//										this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,
//												CombOffsetFlag,	depthmarket.AskPrice1,v,signal.StopPrice);

										this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.AskPrice1,v);
									}
								}
							}
							else if(signal.Direction==1 && signal.Volume>shortposition   )	{
								printf("	撤单后,再次委托下空单 --> %s\n", pinstrumentid);
								char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
								char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
								v=signal.Volume-shortposition-shortreqvolume -shortactionvolume;
								if(v>0)	{
									MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
									int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
									if (v-position>0){
										v=v-position;
										sprintf(logbuf,"TradeOpenCheck:	第[%d]撤单后,再次委托下空单 --> %s   ----开仓量[%d]  持仓量[%d]  排队仓量[%d]   撤单队列仓量[%d]\n",
												action_count,pinstrumentid,v,shortposition,shortreqvolume,shortactionvolume);
										this->GetLogUtil()->WriteLog(logbuf);
//										this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,
//												CombOffsetFlag,	depthmarket.BidPrice1,v,signal.StopPrice);

										this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.BidPrice1,v);

									}
								}
							}
						}
					else{


						if (!valid_delaytime){
							printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
							sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
//							openthis->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_tradetime){
							printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
						}
						if(!valid_maxmargin){
							printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
						}
						if(closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
						}

						if(section_closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
						}


						if(queue_count>0){
							printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
						}
						if(send_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
						}
						if(done_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
						}
						if(!orderinsertflag){
							printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
						}

						if (!maxactionnumber){
							printf("	激活信号,未通过验证 --> [%s]交易尝试次数过多，超出限额,不再交易!\n",pinstrumentid);
						}


						if(!valid_depthmarket){
							printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
						}


						if(!valid_settleconfirm){
							printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
						}

						if(!valid_stoprange){
							printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
									pinstrumentid,signal.StopRange,signal.StopPrice);
						}

					}
				}


		}
		else{
			// not opentime
			// !exist depth
			// !exist signal
		}
	}

	vector<MainContractData>().swap(list);

//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->End!!!");
}
void TrendTrader::TradeOpen_Test(const char* opensignal)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


	this->GetLogUtil()->WriteLog("TradeOpen_Test---------------->begin!!!");


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
		printf("TradeOpen_Test----->信号已激活   匹配主力合约的状态----->pinstrumentid:%s   exchangeid: %s  \n",
				pinstrumentid, item->ExchangeID);
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
			printf("TradeOpen_Test:  	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
					pinstrumentid,openvolume,signal.Volume,investposition);
			sprintf(logbuf,"TradeOpen_Test:  	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
										pinstrumentid,openvolume,signal.Volume,investposition);
			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}

/////////是否存在当日平仓单   ///////////////////////////////////////////////////////////////////////////
//		MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
//		bool closeorder_count = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);

		bool closeorder_count = this->GetDataWrapper()->ExistinCloseOrderMap(pinstrumentid,signal.Direction);
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
				sprintf(logbuf,"TradeOpen_Limit_Check:  信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
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
void TrendTrader::TradeOpen_Limit_Check(const char* opensignal)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


//	this->GetLogUtil()->WriteLog("TradeOpen_Limit_Check---------------->begin!!!");


	std::string tradingdate;
	tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");
	std::string funname="TradeOpen_Limit_Check";

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
			printf("	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
					pinstrumentid,openvolume,signal.Volume,investposition);
			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
										pinstrumentid,openvolume,signal.Volume,investposition);
			this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}
//		if(signal.Direction==0){
//			//invest 持仓数 和hold 持仓数 是否相同?
//			MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
//			int invest_longposition=mysqlinvest.GetPositionbyInsDirection(pinstrumentid,2);
//
//			if(invest_longposition==long_holdposition){
//				openvolume=signal.Volume-long_holdposition ;
//				if(openvolume<=0){
//					printf("	激活信号,未通过验证 --> [%s]多头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
//							pinstrumentid,openvolume,signal.Volume,long_holdposition);
//					sprintf(logbuf,"	激活信号,未通过验证 --> [%s]多头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  持仓数:%d\n",
//												pinstrumentid,openvolume,signal.Volume,long_holdposition);
//					this->GetLogUtil()->WriteLog(logbuf);
//					continue;
//				}
//			}
//			else{
//				openvolume=signal.Volume-invest_longposition ;
//				if(openvolume<=0){
//					printf("	激活信号,未通过验证 --> [%s]空头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  invest持仓数:%d hold持仓数:%d\n",
//							pinstrumentid,openvolume,signal.Volume,invest_longposition,long_holdposition);
//					sprintf(logbuf,"	激活信号,未通过验证 --> [%s]空头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  invest持仓数:%d hold持仓数:%d\n",
//							pinstrumentid,openvolume,signal.Volume,invest_longposition,long_holdposition);
//					this->GetLogUtil()->WriteLog(logbuf);
//					continue;
//				}
//			}
//
//		}
//		else if(signal.Direction==1){
//
//			MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
//			int invest_shortposition=mysqlinvest.GetPositionbyInsDirection(pinstrumentid,3);
//
//			if(invest_shortposition==short_holdposition){
//				openvolume=signal.Volume-short_holdposition ;
//				if(openvolume<=0)	{
//					printf("	激活信号,未通过验证 --> [%s]空头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  hold持仓数:%d\n",
//							pinstrumentid,openvolume,signal.Volume,short_holdposition);
//					sprintf(logbuf,"	激活信号,未通过验证 --> [%s]空头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  hold持仓数:%d\n",
//												pinstrumentid,openvolume,signal.Volume,short_holdposition);
//					this->GetLogUtil()->WriteLog(logbuf);
//					continue;
//				}
//			}
//			else{
//				openvolume=signal.Volume-invest_shortposition ;
//				if(openvolume<=0)	{
//					printf("	激活信号,未通过验证 --> [%s]空头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  invest持仓数:%d hold持仓数:%d\n",
//							pinstrumentid,openvolume,signal.Volume,invest_shortposition,short_holdposition);
//					sprintf(logbuf,"	激活信号,未通过验证 --> [%s]空头待开仓手数不足,无法交易!--->[%d]  信号开仓数:%d  invest持仓数:%d hold持仓数:%d\n",
//							pinstrumentid,openvolume,signal.Volume,invest_shortposition,short_holdposition);
//					this->GetLogUtil()->WriteLog(logbuf);
//					continue;
//				}
//			}
//		}

//		if(done_count>2){
//			printf("	激活信号,未通过验证 --> [%s]开仓次数超过限额[%d],不再交易!\n",pinstrumentid,done_count);
//			continue;
//		}
//
//		if(open_error_count>this->GetMaxOpenErrorCount()){
//			printf("	激活信号,未通过验证 --> [%s]开仓过程中 出现%d 次报错,不再交易!\n",pinstrumentid,open_error_count);
//			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]开仓过程中 出现%d 次报错,不再交易!",pinstrumentid,open_error_count);
//			this->GetLogUtil()->WriteLog(logbuf);
//			continue;
//		}

/////////是否存在当日平仓单   ///////////////////////////////////////////////////////////////////////////
//		MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
//		bool closeorder_count = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);
//		if(closeorder_count){
//			printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
//			continue;
//		}

		bool closeorder_count = this->GetDataWrapper()->ExistinCloseOrderMap(pinstrumentid,signal.Direction);
		if(closeorder_count){
			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
			this->GetLogUtil()->WriteLog(logbuf);
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

				sprintf(logbuf,"TradeOpen_Limit_Check:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
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
				sprintf(logbuf,"TradeOpen_Limit_Check:	信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d] \n",
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

				sprintf(logbuf,"TradeOpen_Limit_Check:	撤单后,再次委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]   \n",
						pinstrumentid,openvolume,long_holdposition);
				this->GetLogUtil()->WriteLog(logbuf);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,openvolume);


			}
			else if(signal.Direction==1 && signal.Volume>short_holdposition   )	{

				char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
				char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓

				sprintf(logbuf,"TradeOpen_Limit_Check:	撤单后,再次委托下空单 --> %s   ----开仓量[%d]  持仓量[%d]  \n",
						pinstrumentid,openvolume,short_holdposition);
				this->GetLogUtil()->WriteLog(logbuf);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,openvolume);

			}
		}

	}//for loop

//std::cout<<"TradeOpen_Limit_Check  ---------------->end"<<endl;

//	this->GetLogUtil()->WriteLog("%s---------------->End!!!",funname.c_str());
}
void TrendTrader::TradeOpen_Section_Check(const char* opensignal)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());


//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->begin!!!");

	MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
	MysqlDepthMarket mysqldepthmarket;

	std::string tradingdate;
	tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");

//	// 20180412   ---> 2018-04-12 00:00:00
//	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());

//	CheckLocalOrderList(tablename.c_str());  //如果有发送的委托单,一直处于status=0 状态,需要确认是否已经到达后台,如过后台不存在该委托单,则更新状态未 error
	MysqlProduct mysqlproduct;
	MysqlSecAssess mysqlsecassess(this->GetPointUserLoginField()->UserID);
	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_TradeSectionList(list);
	DateUtil dateutil;
	printf("TradeOpen_Section_Check----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (auto &item:list)	{

		char pinstrumentid[40];
		strcpy(pinstrumentid,item.InstrumentID);
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID) ;

		if(!opentime){
			continue;
		}

		bool exist_depthmarket = this->GetRedisDepthMarket()->Exists(pinstrumentid);
		if(!exist_depthmarket){
			continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
		if(!existsignal){
			continue;
		}
	//	printf("TradeOpenCheck----->   匹配主力合约的状态----->pinstrumentid:%s   exchangeid: %s  \n", pinstrumentID, item.ExchangeID);

//		bool openlist = strcmp(item.Section,"Finance")!=0;

//		if ( opentime && existsignal   && exist_depthmarket  )	{
//		if (1){
				Signal signal;
				signal=this->GetRedisSignal()->GetInsSignal(signalname);

				//持有多头仓位
				int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
				//多头请求开仓仓位
				int longreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());

				//多头请求撤单的仓位
				int longactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());

				//持有空头仓位
				int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

				//空头请求开仓仓位
				int shortreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());

				// 空头请求撤单的仓位
				int shortactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());

				//合约对应的板块 总开仓合约数是否超过限制

				ProductData productdata;
				mysqlproduct.Find_DatabyProductID(productdata,item.ProductID);

				SectionAssessData sectionassessdata;
				memset(&sectionassessdata,0,sizeof(sectionassessdata));
				sectionassessdata=mysqlsecassess.Find_DataBySection(productdata.Section);

				bool valid_section = sectionassessdata.ProductHoldNumber < sectionassessdata.ProductRiskNumber;
				bool valid_section_long =sectionassessdata.PD_Long_SignalNumber>0 && sectionassessdata.PD_Short_SignalNumber==0;
				bool valid_section_short =sectionassessdata.PD_Long_SignalNumber==0 && sectionassessdata.PD_Short_SignalNumber>0;

				if(valid_section){
					printf("	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata.Section,
							signalname.c_str(),sectionassessdata.ProductRiskNumber,sectionassessdata.ProductHoldNumber);
				}
				else {
					printf("	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
							signalname.c_str(),sectionassessdata.ProductRiskNumber,sectionassessdata.ProductHoldNumber);
				}

				if(valid_section_long){
					printf("	板块%s-------->多头信号%s 满足条件  ------\n",sectionassessdata.Section,signalname.c_str());
				}
				else{
					printf("	板块%s-------->多头信号%s 不满足条件！！！  ------\n",sectionassessdata.Section,signalname.c_str());
				}
				if(valid_section_short){
					printf("	板块%s-------->空头信号%s 满足条件  ------\n",sectionassessdata.Section,signalname.c_str());
				}
				else{
					printf("	板块%s-------->空头信号%s 不满足条件！！！  ------\n",sectionassessdata.Section,signalname.c_str());
				}

	//			if(longposition>0 || shortposition >0)
	//			{
					printf("%s--------------->持仓----多头仓位[%d] 空头仓位[%d] \n",pinstrumentid,longposition,shortposition);
	////				break;
	//			}
	//			printf("%s--------------->空仓 -多头仓位[%d] 空头仓位[%d]\n",pinstrumentID,longposition,shortposition);

				int v;
				char char_times[10];
				strcpy(char_times, depthmarket.UpdateTime);
				std::string str_time=char_times;
				int hours = atoi(str_time.substr(0,2).c_str());
				int minutes = atoi(str_time.substr(3,2).c_str());
				int seconds = atoi(str_time.substr(6,2).c_str());
				double Q_BarTime	= (1/10e1)*hours + (1/10e3)*minutes + (1/10e5)*seconds;
				int c_time = (int)(fabs(Q_BarTime-dateutil.GetCurrentSHFEBarTime(this->GetDifSec()))*10e5);
	//					printf("TradeOpenCheck ---->%s ------> depthmarket [%.6f]--------current [%.6f] ---差值[%d] \n",
	//							pinstrumentID,Q_BarTime,dateutil.GetCurrentSHFEBarTime(this->GetDifSec()),c_time);
				bool valid_delaytime =c_time<300;   // 行情数据延迟秒数
				bool valid_tradetime;
				if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
					valid_tradetime=  dateutil.CheckSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
				}
				else{
					valid_tradetime=  dateutil.CheckDayTradeSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
				}


				bool valid_settleconfirm = this->CheckSettleInfoConfirmbyTrade();

				double maxmargin = this->GetTotalMaxMargin();
				bool valid_maxmargin = this->GetHoldPositionRate() <maxmargin;
//				printf("	风险资金[保证金]比例 最大占用比例   %.2f%% \n",maxmargin*100);

				int send_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),0);   //发送了, 后台未受理
	//					printf("信号激活 --> %s----发单数[%d]  \n",pinstrumentID,send_count);
				int queue_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),1);  //交易队列中
	//					printf("信号激活 --> %s----排队[%d]  \n",pinstrumentID,queue_count);
				int done_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);  // 已成交
	//					printf("信号激活 --> %s----成交数[%d]  \n",pinstrumentID,done_count);
				int action_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),22); //有撤单,控制撤单次数
	//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);

				MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
				bool closeflag = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);
				bool section_closeflag = mysqlcloseorder.Exist_CloseOrderbySection(item.Section,tradingdate.substr(0,8).c_str());
				bool valid_stoprange = (signal.StopRange <depthmarket.LastPrice*0.05 && signal.StopRange >0.1);
//				DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(pinstrumentid);
//
//				bool long_valid1 =  depthmarket.AskPrice1 >data.LowerLimitPrice;
//				bool long_valid2 = depthmarket.AskPrice1 < data.UpperLimitPrice;
//				bool longopen_valid = long_valid1 && long_valid2 ;
//
//				bool short_valid1 =  depthmarket.BidPrice1 >data.LowerLimitPrice;
//				bool short_valid2 = depthmarket.BidPrice1 < data.UpperLimitPrice;
//				bool shortopen_valid = long_valid1 && long_valid2  ;

				if (signal.SingalActive && !signal.OrderSend){

					printf("	信号激活,等待验证 --> %s---多头仓位[%d] 空头仓位[%d] --发单[%d] -排队[%d]  -多头排队仓位[%d] 空头排队仓位[%d]-----成交[%d] ------>行情时差[%d]s \n",
								pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,c_time);

					bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					bool valid_open = valid_delaytime &&  valid_tradetime && valid_maxmargin &&  !closeflag  && !section_closeflag
							&& queue_count==0 && send_count==0  && done_count==0
							&& orderinsertflag &&valid_section && valid_depthmarket  && valid_settleconfirm && valid_stoprange;
					if(valid_open)	{
						printf("	信号激活 -->合法性验证通过 --> %s\n", pinstrumentid);
						if(signal.Direction==0  && signal.Volume>longposition  && valid_section_long){

							char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
							char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
							v=signal.Volume-longposition -longreqvolume -longactionvolume;
							if(v>0)	{

								MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
								int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
								if (v-position>0)	{

									v=v-position;
									printf("	信号激活 -->委托下多单 --> %s   ----开仓量[%d]\n", pinstrumentid,v);
	//								this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,CombOffsetFlag,
	//																	depthmarket.AskPrice1,v,signal.StopPrice);

									this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.AskPrice1,v);

									sprintf(logbuf,"TradeOpenCheck:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
											pinstrumentid,v,longposition,longreqvolume,longactionvolume);
									this->GetLogUtil()->WriteLog(logbuf);

									std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
									strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
									signal.OrderSend=true;

									this->GetRedisSignal()->SetInsSignal(signal,signalname);
								}
							}
						}
						else if(signal.Direction==1 && signal.Volume>shortposition  && valid_section_short){
							printf("	信号激活 -->委托下空单 --> %s\n", pinstrumentid);
							char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
							char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
							v=signal.Volume-shortposition -shortreqvolume -shortactionvolume;
							if(v>0)		{

								MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
								int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
								if (v-position>0)	{
									v=v-position;
	//								this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,CombOffsetFlag,
	//																								depthmarket.BidPrice1,v,signal.StopPrice);
									this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.BidPrice1,v);
									sprintf(logbuf,"TradeOpenCheck:	信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
											pinstrumentid,v,shortposition,shortreqvolume,shortactionvolume);
									this->GetLogUtil()->WriteLog(logbuf);

									std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
									strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
									signal.OrderSend=true;

									this->GetRedisSignal()->SetInsSignal(signal,signalname);
								}
							}
						}
//						else if(closeflag)	{
//							printf("	信号未激活 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
//						}

					}
					else{


						if (!valid_delaytime){
							printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
							sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
//							openthis->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_tradetime){
							printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
						}
						if(!valid_maxmargin){
							printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
						}
						if(closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
						}
						if(section_closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
						}
						if(queue_count>0){
							printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
						}
						if(send_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
						}
						if(done_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
						}
						if(!orderinsertflag){
							printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
						}
						if(!valid_section){
//							printf("	激活信号,未通过验证 --> [%s]板块持仓检测超出风险值,不再交易!\n",pinstrumentid);
							printf("	激活信号,未通过验证 板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
														signalname.c_str(),sectionassessdata.ProductRiskNumber,sectionassessdata.ProductHoldNumber);
						}
						if(!valid_depthmarket){
							printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
						}
						if(!valid_settleconfirm){
							printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
						}
						if(!valid_stoprange){
							printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
									pinstrumentid,signal.StopRange,signal.StopPrice);
						}

					}
				}
				else if(signal.SingalActive && signal.OrderSend){

					printf("	信号激活 发送过委托单,等待验证 %s ---多头仓位[%d] 空头仓位[%d] --> 发单数[%d] ---排队[%d]---   -多头排队仓位[%d] 空头排队仓位[%d]-成交单[%d]----> 撤单次数[%d]  \n",
							pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,action_count);

					// 没有队列排队单  1
					// 没有发送待排队单   0

					bool maxactionnumber =action_count<=this->GetMaxActionNumber() ;
					bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					bool valid_open =valid_delaytime &&  valid_tradetime && valid_maxmargin &&    !closeflag && !section_closeflag
							&& queue_count==0 && maxactionnumber && send_count==0 && done_count==0 && orderinsertflag
							&&valid_section	&& valid_depthmarket && valid_settleconfirm&& valid_stoprange;
					if(valid_open)	{
						printf("	信号激活 发送过委托单,通过合法性验证 %s \n",pinstrumentid);
							if(signal.Direction==0 && signal.Volume>longposition   && valid_section_long)	{
								printf("	撤单后,再次委托下多单 --> %s\n", pinstrumentid);
								char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
								char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
								v=signal.Volume-longposition -longreqvolume -longactionvolume;
								if(v>0)	{
									MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
									int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
									if (v-position>0)	{
										v=v-position;
										sprintf(logbuf,"TradeOpenCheck:	第[%d]次撤单后,再次委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]   撤单队列仓量[%d]\n",
												action_count,pinstrumentid,v,longposition,longreqvolume,longactionvolume);
										this->GetLogUtil()->WriteLog(logbuf);
//										this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,
//												CombOffsetFlag,	depthmarket.AskPrice1,v,signal.StopPrice);

										this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.AskPrice1,v);
									}
								}
							}
							else if(signal.Direction==1 && signal.Volume>shortposition   && valid_section_short)	{
								printf("	撤单后,再次委托下空单 --> %s\n", pinstrumentid);
								char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
								char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
								v=signal.Volume-shortposition-shortreqvolume -shortactionvolume;
								if(v>0)	{
									MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
									int position=mysqlinvest.GetPositionbyIns(pinstrumentid);
									if (v-position>0){
										v=v-position;
										sprintf(logbuf,"TradeOpenCheck:	第[%d]撤单后,再次委托下空单 --> %s   ----开仓量[%d]  持仓量[%d]  排队仓量[%d]   撤单队列仓量[%d]\n",
												action_count,pinstrumentid,v,shortposition,shortreqvolume,shortactionvolume);
										this->GetLogUtil()->WriteLog(logbuf);
//										this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,
//												CombOffsetFlag,	depthmarket.BidPrice1,v,signal.StopPrice);

										this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.BidPrice1,v);

									}
								}
							}
						}
					else{


						if (!valid_delaytime){
							printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
							sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
//							openthis->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_tradetime){
							printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
						}
						if(!valid_maxmargin){
							printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
						}
						if(closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
						}

						if(section_closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
						}


						if(queue_count>0){
							printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
						}
						if(send_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
						}
						if(done_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
						}
						if(!orderinsertflag){
							printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
						}

						if (!maxactionnumber){
							printf("	激活信号,未通过验证 --> [%s]交易尝试次数过多，超出限额,不再交易!\n",pinstrumentid);
						}

						if(!valid_section){
//							printf("	激活信号,未通过验证 --> [%s]板块持仓检测超出风险值,不再交易!\n",pinstrumentid);
							printf("	激活信号,未通过验证 板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
										signalname.c_str(),sectionassessdata.ProductRiskNumber,sectionassessdata.ProductHoldNumber);

						}
						if(!valid_depthmarket){
							printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
						}


						if(!valid_settleconfirm){
							printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
						}

						if(!valid_stoprange){
							printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
									pinstrumentid,signal.StopRange,signal.StopPrice);
						}

					}
				}


//		}
//		else{
//			// not opentime
//			// !exist depth
//			// !exist signal
//		}
	}

	vector<MainContractData>().swap(list);

//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->End!!!");
}


void TrendTrader::TradeOpen_Global_Section_Check(Strategy * strategy,const char* opensignal)
{
	char logbuf[1024];
	DateUtil dateutil;
	Config config(this->GetStrategyName().c_str());

//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->begin!!!");
	MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
	MysqlSecAssess mysqlsecassess(this->GetPointUserLoginField()->UserID);
	MysqlMaincontract mysqlmaincontract;

	std::string tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");
	GlobalViewData globalviewdata;

	strategy->WriteGlobalViewData(strategy->GetGlobalViewData(),&globalviewdata);

	bool valid_globalview_long = false;
	bool valid_globalview_short = false;
	if(globalviewdata.TotalCapital >0 ){
		valid_globalview_long =  globalviewdata.Status ==0;
		valid_globalview_short = globalviewdata.Status ==1;
	}

	vector<MainContractData>  list;
	mysqlmaincontract.Find_TradeSectionList(list);
	printf("TradeOpen_Global_Section_Check----->主力合约品种数[%ld] ----> %s\n",list.size(),
			dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (auto &item:list)	{

		char pinstrumentid[40];
		strcpy(pinstrumentid,item.InstrumentID);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());

//		printf("TradeOpenCheck----->   匹配主力合约的状态----->pinstrumentid:%s   exchangeid: %s  \n", pinstrumentid, item.ExchangeID);

		int precheck_code = this->Valid_PreOrderInsertCheck(pinstrumentid,&item,signalname.c_str());

		if ( precheck_code!=0){  //数据不存在 则返回
//			printf("	Valid_PreOrderInsertCheck Error  >>>>>>>  %s Valid_PreOrderInsertCheck >>>>>>> [%d]\n",pinstrumentid,precheck_code);
			continue;
		}

		Signal signal;
		signal=this->GetRedisSignal()->GetInsSignal(signalname);

		if (!signal.SingalActive ){
			continue;
		}

		DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

				//持有多头仓位
				int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);
				//多头请求开仓仓位
				int longreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());

				//多头请求撤单的仓位
				int longactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());

				//持有空头仓位
				int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);

				//空头请求开仓仓位
				int shortreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());

				// 空头请求撤单的仓位
				int shortactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());

				//总开仓品种是否超过限制
				//当日开仓和平仓的品种操作数
//				int totalholdproduct  = mysqlhold.CountbyAll();
				int totaloperate_product  = this->GetMysqlInvestPosition()->Count_OperatedNumber(this->GetTradingDay().c_str());
				int totalreqproduct = mysqlopenorder.Count_QueueOrderProduct(this->GetTradingDay().c_str());
				int totalreqactionproduct = mysqlopenorder.Count_QueueActionOrderProduct(this->GetTradingDay().c_str());

				int product_open_limit =0;

				if (valid_globalview_long){
					product_open_limit=3-totaloperate_product-totalreqproduct-totalreqactionproduct;

					printf("	整体市场多头---->   当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
							product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
				}
				else if (valid_globalview_short){
					product_open_limit=3-totaloperate_product-totalreqproduct-totalreqactionproduct;
					printf("	整体市场空头---->   当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
							product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
				}
				else{
					product_open_limit=1-totaloperate_product-totalreqproduct-totalreqactionproduct;
					printf("	整体市场震荡---->   当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
							product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
				}


				//合约对应的板块 总开仓合约数是否超过限制
				SectionAssessData sectionassessdata ;
				memset(&sectionassessdata,0,sizeof(sectionassessdata));
				sectionassessdata=mysqlsecassess.Find_DataBySection(item.Section);
				if(sectionassessdata.ProductRiskNumber==0){
					printf("	板块%s  %s -------->不允许开仓,直接跳过巡检  ------\n",sectionassessdata.Section,item.InstrumentID);
					continue;
				}

				bool valid_section_holdlimit =false;
				if(sectionassessdata.ProductHoldNumber < sectionassessdata.ProductRiskNumber){
					valid_section_holdlimit= true;
				}
				bool valid_section_long =false;
				if(sectionassessdata.PD_Long_SignalNumber>0 && sectionassessdata.PD_Short_SignalNumber==0){
					valid_section_long=true;
				}
				bool valid_section_short =false;
				if(sectionassessdata.PD_Long_SignalNumber==0 && sectionassessdata.PD_Short_SignalNumber>0){
					valid_section_short=true;
				}

				bool valid_section_conflict =false;
				if(sectionassessdata.PD_Short_SignalNumber>0 && sectionassessdata.PD_Long_SignalNumber>0){
					valid_section_conflict=true;
				}

				if(valid_section_holdlimit){
					printf("	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata.Section,
							signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);

					if(valid_section_long){
						printf("	板块%s 多头信号数:%d -------->存在多头信号%s,并且无空头信号 满足条件  ------\n",
								sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
						if(sectionassessdata.PD_Long_SignalNumber>1){
							printf("	板块%s 多头信号数:%d -------->%s 多个多头信号,需要进行优选才能开仓  ------\n",
								sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
						}
					}
					else if(valid_section_short){
						printf("	板块%s 空头信号数:%d -------->存在空头信号%s,并且无多头信号 满足条件  ------\n",
								sectionassessdata.Section,sectionassessdata.PD_Short_SignalNumber,signalname.c_str());
						if(sectionassessdata.PD_Short_SignalNumber>1 && sectionassessdata.PD_Long_SignalNumber==0){
							printf("	板块%s 多头信号数:%d -------->%s 多个空头信号,需要进行优选才能开仓  ------\n",
								sectionassessdata.Section,sectionassessdata.PD_Short_SignalNumber,signalname.c_str());
						}
					}
					else{
						if(valid_section_conflict){
							printf("	板块%s同时存在多空方向 --------------->多头信号数:%d  空头信号数:%d \n",
								sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
						}
						else if(sectionassessdata.PD_Short_SignalNumber==0 && sectionassessdata.PD_Long_SignalNumber==0){
							printf("	板块%s 多空均无信号!!!--------------->多头信号数:%d  空头信号数:%d \n",
								sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
						}
						else{
							printf("	板块%s--------> 未知条件！！！待查询  ------\n",sectionassessdata.Section);
						}
					}
				}
				else {
					printf("	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
							signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
					continue;
				}

				int v=0;
				int c_time = dateutil.GetDiffTime(depthmarket.UpdateTime,this->GetDifSec());
				bool valid_delaytime=false;   // 行情数据延迟秒数
				bool valid_tradetime=false;
				valid_tradetime=  dateutil.CheckSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ; // 控制开仓时间在开盘后5分钟再操作

				if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
						strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
					valid_delaytime =c_time<10;
				}
				else{
					valid_delaytime =c_time<300;
				}

				bool valid_settleconfirm = this->CheckSettleInfoConfirmbyTrade();

				double maxmargin = this->GetTotalMaxMargin();
				bool valid_maxmargin = this->GetHoldPositionRate() <maxmargin;
//				printf("	风险资金[保证金]比例 最大占用比例   %.2f%% \n",maxmargin*100);

				int send_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),0);   //发送了, 后台未受理
	//					printf("信号激活 --> %s----发单数[%d]  \n",pinstrumentID,send_count);
				int queue_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),1);  //交易队列中
	//					printf("信号激活 --> %s----排队[%d]  \n",pinstrumentID,queue_count);
				int done_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);  // 已成交
	//					printf("信号激活 --> %s----成交数[%d]  \n",pinstrumentID,done_count);
				int action_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),22); //有撤单,控制撤单次数
	//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);

				MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
				bool closeflag = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);
				bool section_closeflag = mysqlcloseorder.Exist_CloseOrderbySection(item.Section,tradingdate.substr(0,8).c_str());
				bool valid_stoprange = (signal.StopRange <depthmarket.LastPrice*0.05 && signal.StopRange >0.1);

				if (signal.SingalActive && !signal.OrderSend){

					sprintf(logbuf,"	信号激活,等待验证 --> %s---多头仓位[%d] 空头仓位[%d] --发单[%d] -排队[%d]  -多头排队仓位[%d] 空头排队仓位[%d]-----成交[%d] ------>行情时差[%d]s \n",
								pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,c_time);
					this->GetLogUtil()->WriteLog(logbuf);

					bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					bool valid_open = valid_delaytime &&  valid_tradetime && valid_maxmargin &&
									!closeflag  && !section_closeflag
									&& queue_count==0 && send_count==0  && done_count==0
									&& orderinsertflag &&valid_section_holdlimit &&
									valid_depthmarket  && 	valid_settleconfirm && valid_stoprange;

					if(valid_open)	{
						printf("	信号激活 -->合法性验证通过 --> %s\n", pinstrumentid);
						if(signal.Direction==0  && signal.Volume>longposition  && valid_section_long && !valid_globalview_short){

							char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
							char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
							v=signal.Volume-longposition -longreqvolume -longactionvolume;
							if(v>0 && product_open_limit>0)	{

								int position=this->GetMysqlInvestPosition()->GetPositionbyIns(pinstrumentid);
								if (v-position>0)	{
									v=v-position;
									printf("	信号激活 -->委托下多单 --> %s   ----开仓量[%d]\n", pinstrumentid,v);

									sprintf(logbuf,"TradeOpen_Global_Section_Check[开仓品种限制:%d]:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
											product_open_limit,pinstrumentid,v,longposition,longreqvolume,longactionvolume);
									this->GetLogUtil()->WriteLog(logbuf);

									if(valid_globalview_long){
										sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
												strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
										this->GetLogUtil()->WriteLog(logbuf);
									}
									else{
										sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
												strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100,globalviewdata.ShortWeight*100);
										this->GetLogUtil()->WriteLog(logbuf);
									}

									sprintf(logbuf,"	整体市场---->   当日总开仓品种限制-->[%d]  已操作品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
											product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
									this->GetLogUtil()->WriteLog(logbuf);

									sprintf(logbuf,"	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",
										sectionassessdata.Section,signalname.c_str(),sectionassessdata.ProductHoldNumber,
										sectionassessdata.ProductRiskNumber);
									this->GetLogUtil()->WriteLog(logbuf);

									if(valid_section_long){
										sprintf(logbuf,"	板块%s 多头信号数:%d -------->存在多头信号%s,并且无空头信号 满足条件  ------\n",
												sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
										this->GetLogUtil()->WriteLog(logbuf);
										if(sectionassessdata.PD_Long_SignalNumber>1&&sectionassessdata.PD_Short_SignalNumber==0){
											printf("	板块%s 多头信号数:%d -------->%s 多个多头信号,需要进行优选才能开仓  ------\n",
												sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
											sprintf(logbuf,"	板块%s 多头信号数:%d -------->%s 多个多头信号,需要进行优选才能开仓  ------\n",
												sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
											this->GetLogUtil()->WriteLog(logbuf);
										}

									}

									this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.AskPrice1,v);

									std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
									strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
									signal.OrderSend=true;

									this->GetRedisSignal()->SetInsSignal(signal,signalname);
								}
							}
						}
						else if(signal.Direction==1 && signal.Volume>shortposition  && valid_section_short && !valid_globalview_long){
							printf("	信号激活 -->委托下空单 --> %s\n", pinstrumentid);
							char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
							char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
							v=signal.Volume-shortposition -shortreqvolume -shortactionvolume;
							if(v>0)		{
								int position=this->GetMysqlInvestPosition()->GetPositionbyIns(pinstrumentid);
								if (v-position>0&& product_open_limit>0)	{
									v=v-position;
									sprintf(logbuf,"TradeOpen_Global_Section_Check[开仓品种限制:%d]:	信号激活 -->委托下空单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
											product_open_limit,pinstrumentid,v,shortposition,shortreqvolume,shortactionvolume);
									this->GetLogUtil()->WriteLog(logbuf);

									if(valid_globalview_short){
										sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
												strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
										this->GetLogUtil()->WriteLog(logbuf);
									}
									else{
										sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
												strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100,globalviewdata.ShortWeight*100);
										this->GetLogUtil()->WriteLog(logbuf);
									}

									sprintf(logbuf,"	整体市场---->   当日总开仓品种限制-->[%d]  已操作品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
											product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
									this->GetLogUtil()->WriteLog(logbuf);
									sprintf(logbuf,"	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata.Section,
												signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
									this->GetLogUtil()->WriteLog(logbuf);

									if(valid_section_short){

										sprintf(logbuf,"	板块%s 空头信号数:%d -------->存在空头信号%s,并且无多头信号 满足条件  ------\n",
											sectionassessdata.Section,sectionassessdata.PD_Short_SignalNumber,signalname.c_str());
										this->GetLogUtil()->WriteLog(logbuf);
										if(sectionassessdata.PD_Short_SignalNumber>1 && sectionassessdata.PD_Long_SignalNumber==0){
											sprintf(logbuf,"	板块%s 多头信号数:%d -------->%s 多个空头信号,需要进行优选才能开仓  ------\n",
												sectionassessdata.Section,sectionassessdata.PD_Short_SignalNumber,signalname.c_str());
											this->GetLogUtil()->WriteLog(logbuf);
										}
									}

									this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.BidPrice1,v);

									std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
									strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
									signal.OrderSend=true;

									this->GetRedisSignal()->SetInsSignal(signal,signalname);
								}
							}
						}
						else{

							if(signal.Direction==0 &&!valid_section_long){
								sprintf(logbuf,"	激活信号,放弃多头合约开仓>>>>>>>>>>板块%s 多头信号数 %d 空头信号数:%d ---\n",
									sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
								this->GetLogUtil()->WriteLog(logbuf);
							}

							if(signal.Direction==1 &&!valid_section_short){
								sprintf(logbuf,"	激活信号,放弃空头合约开仓>>>>>>>>>>板块%s 多头信号数 %d 空头信号数:%d ---\n",
									sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
								this->GetLogUtil()->WriteLog(logbuf);
							}

							if(signal.Direction==0 && valid_globalview_short){
								if(valid_globalview_short){
									sprintf(logbuf,"	激活信号,放弃多头合约开仓>>>>>>>>>>整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
											strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
									this->GetLogUtil()->WriteLog(logbuf);
								}
							}

							if(signal.Direction==1 && valid_globalview_long){
								if(valid_globalview_long){
									sprintf(logbuf,"	激活信号,放弃空头合约开仓>>>>>>>>>>整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
											strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
									this->GetLogUtil()->WriteLog(logbuf);
								}
							}
						}


					}
					else{


						if (!valid_delaytime){
							printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
							sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_tradetime){
							printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_maxmargin){
							printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(section_closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(queue_count>0){
							printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(send_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(done_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!orderinsertflag){
							printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_section_holdlimit){
//							printf("	激活信号,未通过验证 --> [%s]板块持仓检测超出风险值,不再交易!\n",pinstrumentid);
							printf("	激活信号,未通过验证 板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
										signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
							sprintf(logbuf,"	激活信号,未通过验证 板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
																	signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_depthmarket){
							printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_settleconfirm){
							printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_stoprange){
							printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
									pinstrumentid,signal.StopRange,signal.StopPrice);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
										pinstrumentid,signal.StopRange,signal.StopPrice);
							this->GetLogUtil()->WriteLog(logbuf);
						}

						if(valid_section_conflict){
							printf("	激活信号,未通过验证 --> [%s]板块内部存在不同方向信号! 多头方向信号数[%d] 空头方向信号数[%d] \n",
									pinstrumentid,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]板块内部存在不同方向信号! 多头方向信号数[%d] 空头方向信号数[%d] \n",
									pinstrumentid,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
							this->GetLogUtil()->WriteLog(logbuf);
						}

					}
				}
				else if(signal.SingalActive && signal.OrderSend){

					sprintf(logbuf,"	信号激活 发送过委托单,等待验证 %s ---多头仓位[%d] 空头仓位[%d] --> 发单数[%d] ---排队[%d]---   -多头排队仓位[%d] 空头排队仓位[%d]-成交单[%d]----> 撤单次数[%d]  \n",
							pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,action_count);
					this->GetLogUtil()->WriteLog(logbuf);
					// 没有队列排队单  1
					// 没有发送待排队单   0

					bool maxactionnumber =action_count<=this->GetMaxActionNumber() ;
					bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();
					bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
					bool valid_open =valid_delaytime &&  valid_tradetime && valid_maxmargin &&    !closeflag && !section_closeflag
							&& queue_count==0 && maxactionnumber && send_count==0 && done_count==0 && orderinsertflag
							&&valid_section_holdlimit	&& valid_depthmarket && valid_settleconfirm&& valid_stoprange;
					if(valid_open)	{
						printf("	信号激活 发送过委托单,通过合法性验证 %s \n",pinstrumentid);
							if(signal.Direction==0 && signal.Volume>longposition   && valid_section_long && !valid_globalview_short)	{
								printf("	撤单后,再次委托下多单 --> %s\n", pinstrumentid);
								char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
								char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
								v=signal.Volume-longposition -longreqvolume -longactionvolume;
								if(v>0&& product_open_limit>0)	{
//									MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
									int position=this->GetMysqlInvestPosition()->GetPositionbyIns(pinstrumentid);
									if (v-position>0)	{
										v=v-position;
										sprintf(logbuf,"TradeOpen_Global_Section_Check[开仓品种限制:%d]:	第[%d]次撤单后,再次委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]   撤单队列仓量[%d]\n",
												product_open_limit,action_count,pinstrumentid,v,longposition,longreqvolume,longactionvolume);
										this->GetLogUtil()->WriteLog(logbuf);

										if(valid_globalview_long){
											sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
													strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
											this->GetLogUtil()->WriteLog(logbuf);
										}
										else if(valid_globalview_short){
											sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
													strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
											this->GetLogUtil()->WriteLog(logbuf);
										}
										else{
											sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
													strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100,globalviewdata.ShortWeight*100);
											this->GetLogUtil()->WriteLog(logbuf);
										}

										sprintf(logbuf,"	整体市场---->   当日总开仓品种限制-->[%d]  已操作品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
												product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
										this->GetLogUtil()->WriteLog(logbuf);
										sprintf(logbuf,"	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata.Section,
														signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
											this->GetLogUtil()->WriteLog(logbuf);
//										this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,
//												CombOffsetFlag,	depthmarket.AskPrice1,v,signal.StopPrice);
											if(valid_section_long){
												sprintf(logbuf,"	板块%s 多头信号数:%d -------->存在多头信号%s,并且无空头信号 满足条件  ------\n",
														sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
												this->GetLogUtil()->WriteLog(logbuf);
												if(sectionassessdata.PD_Long_SignalNumber>1&&sectionassessdata.PD_Short_SignalNumber==0){
													printf("	板块%s 多头信号数:%d -------->%s 多个多头信号,需要进行优选才能开仓  ------\n",
														sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
													sprintf(logbuf,"	板块%s 多头信号数:%d -------->%s 多个多头信号,需要进行优选才能开仓  ------\n",
														sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,signalname.c_str());
													this->GetLogUtil()->WriteLog(logbuf);
												}

											}
										this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.AskPrice1,v);
									}
								}
							}
							else if(signal.Direction==1 && signal.Volume>shortposition   && valid_section_short && !valid_globalview_long)	{
								printf("	撤单后,再次委托下空单 --> %s\n", pinstrumentid);
								char directiontype=signal.Direction+'0';   // 0 : 买    1:卖
								char CombOffsetFlag='0';   // 0 : 开仓  1: 平仓
								v=signal.Volume-shortposition-shortreqvolume -shortactionvolume;
								if(v>0&& product_open_limit>0)	{
//									MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);
									int position=this->GetMysqlInvestPosition()->GetPositionbyIns(pinstrumentid);
									if (v-position>0){
										v=v-position;
										sprintf(logbuf,"TradeOpen_Global_Section_Check[开仓品种限制:%d]:	第[%d]撤单后,再次委托下空单 --> %s   ----开仓量[%d]  持仓量[%d]  排队仓量[%d]   撤单队列仓量[%d]\n",
												product_open_limit,action_count,pinstrumentid,v,shortposition,shortreqvolume,shortactionvolume);
										this->GetLogUtil()->WriteLog(logbuf);
										if(valid_globalview_long){
											sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
													strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
											this->GetLogUtil()->WriteLog(logbuf);
										}
										else if(valid_globalview_short){
											sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
													strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
											this->GetLogUtil()->WriteLog(logbuf);
										}
										else{
											sprintf(logbuf,"TradeOpen_Global_Section_Check->整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
													strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100,globalviewdata.ShortWeight*100);
											this->GetLogUtil()->WriteLog(logbuf);
										}
										sprintf(logbuf,"	整体市场---->   当日总开仓品种限制-->[%d]  已操作品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
													product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
										this->GetLogUtil()->WriteLog(logbuf);
										sprintf(logbuf,"	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata.Section,
													signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
										this->GetLogUtil()->WriteLog(logbuf);

										if(valid_section_short){

											sprintf(logbuf,"	板块%s 空头信号数:%d -------->存在空头信号%s,并且无多头信号 满足条件  ------\n",
												sectionassessdata.Section,sectionassessdata.PD_Short_SignalNumber,signalname.c_str());
											this->GetLogUtil()->WriteLog(logbuf);
											if(sectionassessdata.PD_Short_SignalNumber>1 && sectionassessdata.PD_Long_SignalNumber==0){
												sprintf(logbuf,"	板块%s 多头信号数:%d -------->%s 多个空头信号,需要进行优选才能开仓  ------\n",
													sectionassessdata.Section,sectionassessdata.PD_Short_SignalNumber,signalname.c_str());
												this->GetLogUtil()->WriteLog(logbuf);
											}
										}


//										this->ReqSqlOrderInsert(signalname.c_str(),pinstrumentid,item.ExchangeID,directiontype,
//												CombOffsetFlag,	depthmarket.BidPrice1,v,signal.StopPrice);

										this->ReqOrderInsertbySignal(&signal,item.ExchangeID,depthmarket.BidPrice1,v);

									}
								}
							}
							else{

								if(signal.Direction==0 &&!valid_section_long){
									sprintf(logbuf,"	激活信号,放弃多头合约开仓>>>>>>>>>>板块%s 多头信号数 %d 空头信号数:%d ---\n",
										sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
									this->GetLogUtil()->WriteLog(logbuf);
								}

								if(signal.Direction==1 &&!valid_section_short){
									sprintf(logbuf,"	激活信号,放弃空头合约开仓>>>>>>>>>>板块%s 多头信号数 %d 空头信号数:%d ---\n",
										sectionassessdata.Section,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
									this->GetLogUtil()->WriteLog(logbuf);
								}

								if(signal.Direction==0 && valid_globalview_short){
									sprintf(logbuf,"	激活信号,放弃多头合约开仓>>>>>>>>>>整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
											strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
									this->GetLogUtil()->WriteLog(logbuf);
								}

								if(signal.Direction==1 && valid_globalview_long){
									sprintf(logbuf,"	激活信号,放弃空头合约开仓>>>>>>>>>>整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
											strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
									this->GetLogUtil()->WriteLog(logbuf);
								}
							}
						}
					else{


						if (!valid_delaytime){
							printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
							sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_tradetime){
							printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!valid_maxmargin){
							printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}

						if(section_closeflag){
							printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}


						if(queue_count>0){
							printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(send_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(done_count>0){
							printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}
						if(!orderinsertflag){
							printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}

						if (!maxactionnumber){
							printf("	激活信号,未通过验证 --> [%s]交易尝试次数过多，超出限额,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]交易尝试次数过多，超出限额,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}

						if(!valid_section_holdlimit){
//							printf("	激活信号,未通过验证 --> [%s]板块持仓检测超出风险值,不再交易!\n",pinstrumentid);
							printf("	激活信号,未通过验证 板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
										signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
							sprintf(logbuf,"	激活信号,未通过验证 板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata.Section,
										signalname.c_str(),sectionassessdata.ProductHoldNumber,sectionassessdata.ProductRiskNumber);
							this->GetLogUtil()->WriteLog(logbuf);

						}
						if(!valid_depthmarket){
							printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}


						if(!valid_settleconfirm){
							printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
							this->GetLogUtil()->WriteLog(logbuf);
						}

						if(!valid_stoprange){
							printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
									pinstrumentid,signal.StopRange,signal.StopPrice);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
										pinstrumentid,signal.StopRange,signal.StopPrice);
							this->GetLogUtil()->WriteLog(logbuf);
						}

						if(valid_section_conflict){
							printf("	激活信号,未通过验证 --> [%s]板块内部存在不同方向信号! 多头方向信号数[%d] 空头方向信号数[%d] \n",
									pinstrumentid,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
							sprintf(logbuf,"	激活信号,未通过验证 --> [%s]板块内部存在不同方向信号! 多头方向信号数[%d] 空头方向信号数[%d] \n",
									pinstrumentid,sectionassessdata.PD_Long_SignalNumber,sectionassessdata.PD_Short_SignalNumber);
							this->GetLogUtil()->WriteLog(logbuf);
						}
					}
				}
				else	{
					// signal 未激活
				}
	} //for 循环

	vector<MainContractData>().swap(list);
//	printf("TradeOpen_Global_Section_Check---------------->End!!!  \n");
//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->End!!!");
}


void TrendTrader::TradeOpen_Global_SecAssess(Strategy * strategy,SecAssess *secassess,const char* opensignal)
{
	char logbuf[1024];
//	Config config(this->GetStrategyName().c_str());

//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->begin!!!");
//	printf("TradeOpen_Global_SecAssess---------------->begin!!!  \n");

	SectionAssessData sectionassessdata ;
	memset(&sectionassessdata,0,sizeof(sectionassessdata));

	bool result=false;

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0)	{
		result=secassess->SectionRecommand(&sectionassessdata); //有信号即刻交易
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0){
//		result=secassess->SectionRecommandbyMoreSignal(&sectionassessdata);
		result=secassess->SectionRecommandbyGlobalView(&sectionassessdata,strategy); //有信号 同时全局对比 资金流动比最高  涨跌幅之最 同时满足 则交易
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
			strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0 ){
			result=secassess->SectionRecommandbyMoreSignal(&sectionassessdata);  // 同板块有多个信号 才交易
	}
	else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0){
		result=secassess->SectionRecommand(&sectionassessdata);
	}
	else{
		result=secassess->SectionRecommand(&sectionassessdata);
	}

	if(!result){
		printf("TradeOpen_Global_SecAssess  >>>>>>>>> 无推荐信号!!! 直接退出板块推荐 >>>>>>>End!!! \n");
		return;
	}
	else{

		int len = strlen(sectionassessdata.Result_InstrumentId);
		if(len>3){
			printf("TradeOpen_Global_SecAssess  >>>>>>>>>推荐板块 %s  推荐合约 %s ---->允许开仓品种数: %d  已持仓品种数: %d\n",
				sectionassessdata.Section,sectionassessdata.Result_InstrumentId,sectionassessdata.ProductRiskNumber,
				sectionassessdata.ProductHoldNumber);
			MysqlMaincontract mysqlmaincontract;
			MainContractData item = mysqlmaincontract.Find_DatabyInstrumentID(sectionassessdata.Result_InstrumentId);
			this->ReqOpen(&item,strategy,&sectionassessdata,opensignal,sectionassessdata.Result_InstrumentId);
		}
		else{
			printf("TradeOpen_Global_SecAssess  ERROR >>>>>>>>> 推荐信号为空!!! >>>>>>>End!!! \n");
		}
	}

//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->End!!!");
}

int TrendTrader::Valid_PreOrderInsertCheck(const char* pinstrumentid,
		MainContractData *maincontractdata,
		const char* signalname){

	int ret=100;
	DateUtil dateutil;

	bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),maincontractdata->ExchangeID,maincontractdata->ProductID) ;
	bool exist_depthmarket = this->GetRedisDepthMarket()->Exists(pinstrumentid);
	bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;

	if(opentime && exist_depthmarket && existsignal){
		ret=0;
	}
	else{

		if(!opentime){
//			printf("	板块%s  %s --------> 非开盘时间 直接跳出! \n",maincontractdata->Section,pinstrumentid);
			ret=1;
		}
		else if(!exist_depthmarket ){
			printf("	板块%s  %s --------> 无行情数据 直接跳出! \n",maincontractdata->Section,pinstrumentid);
			ret=2;
		}
		else if(!existsignal ){
			ret=3;
		}
		else {
			ret=100;  //未知错误
		}


	}

	return ret;
}
int TrendTrader::Valid_OrderInsertCheck(const char* pinstrumentid,
		MainContractData *maincontractdata,
		const char* signalname,DepthMarket *depthmarket){

	int ret=100;
	DateUtil dateutil;

	bool valid_tradetime= dateutil.CheckSHFEInsOrderInsertTime(pinstrumentid,this->GetDifSec()) ;
	int c_time = dateutil.GetDiffTime(depthmarket->UpdateTime,this->GetDifSec());
//	printf("	 ---->%s ------> depthmarket [%.6f]--------current [%.6f] ---差值[%d] \n",
//			pinstrumentID,Q_BarTime,dateutil.GetCurrentSHFEBarTime(this->GetDifSec()),c_time);
	bool valid_delaytime=false;   // 行情数据延迟秒数
	// 控制开仓时间在开盘后5分钟再操作
	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0||
			strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
		valid_delaytime =c_time<10;
	}
	else{
		valid_delaytime =c_time<300;
	}
	bool valid_depthmarket = this->ValidCheck_DepthMarket(depthmarket);
	bool valid_settleconfirm = this->CheckSettleInfoConfirmbyTrade();
	double maxmargin = this->GetTotalMaxMargin();
	bool valid_maxmargin = this->GetHoldPositionRate() <maxmargin;
//	printf("	风险资金[保证金]比例 最大占用比例   %.2f%% \n",maxmargin*100);

	bool orderinsertflag =!this->GetCTraderSpi()->GetFlagOrderInsert();


	if(valid_tradetime &&valid_depthmarket&& valid_settleconfirm
			&& valid_maxmargin && orderinsertflag){
		ret=0;
	}
	else{

		if(!valid_tradetime){
			printf("	激活信号,未通过验证 --> [%s]非策略交易时间,不再交易!\n",pinstrumentid);
			ret=4;
		}
		else if (!valid_delaytime){
			printf("	激活信号,未通过验证 --> [%s]行情数据时延过大,不再交易! 行情延迟 %d 秒 \n",pinstrumentid,c_time);
	//			sprintf (logbuf,"[%s]  -------- 行情延迟 %d 秒 -------",item.InstrumentID,c_time);
	//							openthis->GetLogUtil()->WriteLog(logbuf);
			ret=5;
		}
		else if(!valid_depthmarket){
			printf("	激活信号,未通过验证 --> [%s]行情数据非法,不再交易!\n",pinstrumentid);
			ret=6;
		}
		else if(!valid_settleconfirm){
			printf("	激活信号,未通过验证 --> [%s]结算信息未确认,不再交易!\n",pinstrumentid);
			ret=7;
		}
		else if(!valid_maxmargin){
			printf("	激活信号,未通过验证 --> [%s]超出最大持仓风险值[%.2f%%],不交易!\n",pinstrumentid,maxmargin*100);
			ret=8;
		}
		else if(!orderinsertflag){
			printf("	激活信号,未通过验证 --> [%s]正在交易中,请等待!\n",pinstrumentid);
			ret=9;
		}
		else {
			ret=100;  //未知错误
		}
	}

	return ret;
}
int TrendTrader::Valid_SectionConflictOrderInsertCheck(const char* pinstrumentid,
		SectionAssessData *sectionassessdata,const char* signalname){

	int ret=100;

	if(sectionassessdata->ProductRiskNumber==0){
		printf("	板块%s  %s -------->不允许开仓,直接跳过巡检  ------\n",sectionassessdata->Section,pinstrumentid);
		ret=50;
	}

	bool valid_section_holdlimit =false;
	if(sectionassessdata->ProductHoldNumber < sectionassessdata->ProductRiskNumber){
		valid_section_holdlimit= true;
	}

	bool valid_section_conflict =false;
	if(sectionassessdata->PD_Short_SignalNumber>0 && sectionassessdata->PD_Long_SignalNumber>0){
		valid_section_conflict=true;
	}

	if(valid_section_holdlimit){
		printf("	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata->Section,
				signalname,sectionassessdata->ProductHoldNumber,sectionassessdata->ProductRiskNumber);

		if(valid_section_conflict){
			printf("	板块%s同时存在多空方向 --------------->多头信号数:%d  空头信号数:%d \n",
				sectionassessdata->Section,sectionassessdata->PD_Long_SignalNumber,sectionassessdata->PD_Short_SignalNumber);
		}
		else if(sectionassessdata->PD_Short_SignalNumber==0 && sectionassessdata->PD_Long_SignalNumber==0){
			printf("	板块%s 多空均无信号!!!--------------->多头信号数:%d  空头信号数:%d \n",
				sectionassessdata->Section,sectionassessdata->PD_Long_SignalNumber,sectionassessdata->PD_Short_SignalNumber);
		}
		else{
			printf("	板块%s--------> 未知条件！！！待查询  ------\n",sectionassessdata->Section);
		}

	}
	else {
		printf("	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 已超出最大值[%d],不再开单!!! \n",sectionassessdata->Section,
				signalname,sectionassessdata->ProductHoldNumber,sectionassessdata->ProductRiskNumber);

	}

	if(!valid_section_conflict && valid_section_holdlimit){
		ret=0;
	}
	else{

		if(valid_section_conflict){
			printf("	板块%s  %s --------> 板块同时存在多空信号 直接跳出! \n",sectionassessdata->Section,pinstrumentid);
			ret=50;
		}
		else if(!valid_section_holdlimit ){
			printf("	板块%s  %s --------> 板块持仓单限制 直接跳出! \n",sectionassessdata->Section,pinstrumentid);
			ret=51;
		}
		else {
			ret=100;  //未知错误
		}
	}

	return ret;
}

int TrendTrader::Valid_CloseFlagOrderInsertCheck(const char* pinstrumentid,MainContractData *maincontractdata){

	int ret=100;
	Config config(this->GetStrategyName().c_str());

	std::string tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");

	MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),this->GetPointUserLoginField()->UserID);
	bool closeflag = mysqlcloseorder.Exist_OrderbyInstrumentidStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);
	bool section_closeflag = mysqlcloseorder.Exist_CloseOrderbySection(maincontractdata->Section,tradingdate.substr(0,8).c_str());

	if(!closeflag && !section_closeflag){
		ret=0;
	}
	else{

		if(closeflag){
			printf("	激活信号,未通过验证 --> [%s]当天有平仓单存在,不再交易!\n",pinstrumentid);
			ret=60;
		}
		else if(section_closeflag ){
			printf("	激活信号,未通过验证 --> [%s]当天所在板块有平仓单存在,不再交易!\n",pinstrumentid);
			ret=61;
		}
		else {
			ret=100;  //未知错误
		}
	}

	return ret;
}
void TrendTrader::ReqOpen(MainContractData*item,Strategy * strategy,
		SectionAssessData *sectionassessdata,const char* opensignal,const char* pinstrumentid)
{
	char logbuf[1024];
	Config config(this->GetStrategyName().c_str());

//	this->GetLogUtil()->WriteLog("TrendTrader::ReqOpen---------------->begin!!!");
//	printf("TrendTrader::ReqOpen---------------->begin!!!  \n");

	std::string tradingdate = this->GetTradingDay().c_str();
	tradingdate.append("000000");

	GlobalViewData globalviewdata;
	strategy->WriteGlobalViewData(strategy->GetGlobalViewData(),&globalviewdata);
	bool valid_globalview_long = false;
	bool valid_globalview_short = false;
	if(globalviewdata.TotalCapital >0 ){
		valid_globalview_long =  globalviewdata.Status ==0;
		valid_globalview_short = globalviewdata.Status ==1;
	}


	DateUtil dateutil;
	std::string signalname;
	signalname=pinstrumentid;
	signalname.append(opensignal);
	signalname.append("_");
	signalname.append(this->GetUserID().c_str());
	int precheck_code = this->Valid_PreOrderInsertCheck(pinstrumentid,item,signalname.c_str());

	if ( precheck_code!=0  ){  //数据不存在 则返回
		sprintf(logbuf,"	TrendTrader::ReqOpen->Valid_PreOrderInsertCheck Error  >>>>>>>  %s  >>>>>>> [%d]\n",pinstrumentid,precheck_code);
		this->GetLogUtil()->WriteLog(logbuf);
		return;
	}

	Signal signal;
	signal=this->GetRedisSignal()->GetInsSignal(signalname);
	if (!signal.SingalActive ){ //信号未激活 则返回
		return;
	}

	DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
	int check_code = this->Valid_OrderInsertCheck(pinstrumentid,item,signalname.c_str(),&depthmarket);
	if ( check_code!=0  ){
		sprintf(logbuf,"	TrendTrader::ReqOpen->Valid_OrderInsertCheck Error  >>>>>>>  %s  >>>>>>> [%d]\n",pinstrumentid,check_code);
		this->GetLogUtil()->WriteLog(logbuf);
		return;
	}

/////////////////////////////////////////////////////////////////////////////
	bool valid_section_long =false;
	if(sectionassessdata->PD_Long_SignalNumber>0 && sectionassessdata->PD_Short_SignalNumber==0){
		valid_section_long=true;
	}
	bool valid_section_short =false;
	if(sectionassessdata->PD_Long_SignalNumber==0 && sectionassessdata->PD_Short_SignalNumber>0){
		valid_section_short=true;
	}


	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0||
				strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
		int check_sectionconfilict_code = this->Valid_SectionConflictOrderInsertCheck(pinstrumentid,sectionassessdata,signalname.c_str());
		if ( check_sectionconfilict_code!=0  ){
			sprintf(logbuf,"	TrendTrader::ReqOpen->Valid_SectionConflictOrderInsertCheck Error  >>>>>>>  %s  >>>>>>> [%d]\n",pinstrumentid,check_sectionconfilict_code);
			this->GetLogUtil()->WriteLog(logbuf);
			return;
		}


		if(signal.Direction==0 &&!valid_section_long){
			sprintf(logbuf,"	激活信号,放弃多头合约开仓>>>>>>>>>>板块%s 多头信号数 %d 空头信号数:%d ---\n",
				sectionassessdata->Section,sectionassessdata->PD_Long_SignalNumber,sectionassessdata->PD_Short_SignalNumber);
			this->GetLogUtil()->WriteLog(logbuf);
			return;
		}

		if(signal.Direction==1 &&!valid_section_short){
			sprintf(logbuf,"	激活信号,放弃空头合约开仓>>>>>>>>>>板块%s 多头信号数 %d 空头信号数:%d ---\n",
				sectionassessdata->Section,sectionassessdata->PD_Long_SignalNumber,sectionassessdata->PD_Short_SignalNumber);
			this->GetLogUtil()->WriteLog(logbuf);
			return;
		}
	}
/////////////////////////////////////////////////////////////////////////////



	MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);

	int longposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,0);  //持有多头仓位
	int longreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());//多头请求开仓仓位
	int longactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,0,this->GetTradingDay().c_str());//多头请求撤单的仓位

	int shortposition =mysqlhold.CollectPositions_byInstrumentID(pinstrumentid,1);//持有空头仓位
	int shortreqvolume = mysqlopenorder.Get_QueueOrderListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());//空头请求开仓仓位
	int shortactionvolume = mysqlopenorder.Get_QueueOrderActionListsVolume(pinstrumentid,1,this->GetTradingDay().c_str());// 空头请求撤单的仓位

	int send_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),0);   //发送了, 后台未受理
//					printf("信号激活 --> %s----发单数[%d]  \n",pinstrumentID,send_count);
	int queue_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),1);  //交易队列中
//					printf("信号激活 --> %s----排队[%d]  \n",pinstrumentID,queue_count);
	int done_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),2);  // 已成交
//					printf("信号激活 --> %s----成交数[%d]  \n",pinstrumentID,done_count);
	int action_count = mysqlopenorder.Count_OrderListbyStatus(pinstrumentid,tradingdate.substr(0,8).c_str(),22); //有撤单,控制撤单次数
//					printf("信号激活 --> %s----撤单数[%d]  \n",pinstrumentID,action_count);
	if(queue_count>0){
		printf("	激活信号,未通过验证 --> [%s]请求交易队列存在交易合约,请等待!\n",pinstrumentid);
		return;
	}
	if(send_count>0){
		printf("	激活信号,未通过验证 --> [%s]已经存在发单数,不再交易!\n",pinstrumentid);
		return;
	}
	if(done_count>0){
		printf("	激活信号,未通过验证 --> [%s]已经存在成交单,不再交易!\n",pinstrumentid);
		return;
	}



	//总开仓品种是否超过限制
	//当日开仓和平仓的品种操作数
//				int totalholdproduct  = mysqlhold.CountbyAll();  //hold表 不能算出当日平仓的合约数!!!
	int totaloperate_product  = this->GetMysqlInvestPosition()->Count_OperatedNumber(this->GetTradingDay().c_str());
	int totalreqproduct = mysqlopenorder.Count_QueueOrderProduct(this->GetTradingDay().c_str());
	int totalreqactionproduct = mysqlopenorder.Count_QueueActionOrderProduct(this->GetTradingDay().c_str());

	int product_open_limit =0;
	if (valid_globalview_long){
		product_open_limit=3-totaloperate_product-totalreqproduct-totalreqactionproduct;

		printf("	整体市场多头---->   当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
				product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
	}
	else if (valid_globalview_short){
		product_open_limit=3-totaloperate_product-totalreqproduct-totalreqactionproduct;
		printf("	整体市场空头---->   当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
				product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
	}
	else{
		product_open_limit=1-totaloperate_product-totalreqproduct-totalreqactionproduct;
		printf("	整体市场震荡---->   当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
				product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
	}

	if(product_open_limit<=0){
		printf("	激活信号,未通过验证 -->市场行情级别限制开仓!!!     当日总开仓品种数限制-->[%d]  已开仓品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
						product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
		return;
	}

	int check_closeflag_code = this->Valid_CloseFlagOrderInsertCheck(pinstrumentid,item);
	if ( check_closeflag_code!=0  ){
		sprintf(logbuf,"	TrendTrader::ReqOpen->Valid_CloseFlagOrderInsertCheck Error  >>>>>>>  %s  >>>>>>> [%d]\n",pinstrumentid,check_closeflag_code);
		this->GetLogUtil()->WriteLog(logbuf);
		return;
	}

	bool valid_stoprange = (signal.StopRange <depthmarket.LastPrice*0.05 && signal.StopRange >0.1);
	if(!valid_stoprange){
		printf("	激活信号,未通过验证 --> [%s]止损价差异常,请确认!  --> StopRange:%.3f StopPrice:%.3f\n",
				pinstrumentid,signal.StopRange,signal.StopPrice);
		return;
	}

	int v=0;
	int position=this->GetMysqlInvestPosition()->GetPositionbyIns(pinstrumentid);
	if(signal.Direction==0){
		v=signal.Volume-longposition -longreqvolume -longactionvolume-position;
	}
	else if (signal.Direction==1){
		v=signal.Volume-shortposition -shortreqvolume -shortactionvolume-position;
	}

	if(v<=0){
		printf("	激活信号,未通过验证 --> %s 开仓手数为负数或者零>>>>  %d  已持该合约多头仓位[%d] 已持该合约空头仓位[%d] \n",
				pinstrumentid,v,longposition,shortposition);
		return;
	}

	if(signal.Direction==0 && valid_globalview_short){

		sprintf(logbuf,"	激活信号,放弃多头合约开仓>>>>>>>>>>整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
				strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
		this->GetLogUtil()->WriteLog(logbuf);
		return;
	}
	else if(signal.Direction==1 && valid_globalview_long){

		sprintf(logbuf,"	激活信号,放弃空头合约开仓>>>>>>>>>>整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
				strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
		this->GetLogUtil()->WriteLog(logbuf);
		return;
	}

	sprintf(logbuf,"TrendTrader::ReqOpen[开仓品种限制:%d]:  信号激活 -->委托下多单 --> %s   ----开仓量[%d] 持仓量[%d]  排队仓量[%d]  撤单队列仓量[%d]\n",
			product_open_limit,pinstrumentid,v,longposition,longreqvolume,longactionvolume);
	this->GetLogUtil()->WriteLog(logbuf);

	sprintf(logbuf,"	整体市场---->   当日总开仓品种限制-->[%d]  已操作品种数[%d] 总请求开仓品种[%d] 总撤单品种[%d] \n",
			product_open_limit,totaloperate_product,totalreqproduct,totalreqactionproduct);
	this->GetLogUtil()->WriteLog(logbuf);

	if(signal.Direction==0    && !valid_globalview_short){
		if(valid_globalview_long){
			sprintf(logbuf,"TrendTrader::ReqOpen->整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
					strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100);
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else{
			sprintf(logbuf,"TrendTrader::ReqOpen->整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
					strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100,globalviewdata.ShortWeight*100);
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}
	else if(signal.Direction==1    && !valid_globalview_long){
		if(valid_globalview_short){
			sprintf(logbuf,"TrendTrader::ReqOpen->整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
					strategy->GetGlobalViewData()->Down_Count,strategy->GetGlobalViewData()->Up_Count,globalviewdata.ShortWeight*100);
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else{
			sprintf(logbuf,"TrendTrader::ReqOpen->整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
					strategy->GetGlobalViewData()->Up_Count,strategy->GetGlobalViewData()->Down_Count,globalviewdata.LongWeight*100,globalviewdata.ShortWeight*100);
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}


	sprintf(logbuf,"	板块%s-------->信号%s  ------> 持有仓单的品种数[%d] 未超出最大值[%d]------------> 允许开单!!! \n",sectionassessdata->Section,
			signalname.c_str(),sectionassessdata->ProductRiskNumber,sectionassessdata->ProductHoldNumber);
	this->GetLogUtil()->WriteLog(logbuf);


	/*************************为开仓时记录开仓时的 板块涨跌状态*********************************/
	// Open_SpreadSignal_Average 以下代码和策略函数内容一致

	double section_rate=0.6;
	bool valid_section_up =false;
	bool valid_section_down =false;
	if(this->GetRedisSection()->Exists(sectionassessdata->Section) >0){
		SectionInfo sectioninfo;
		sectioninfo =this->GetRedisSection()->GetSection(sectionassessdata->Section);
		// 此处的rate 要比 正常板块 联动的要低，减少开仓次数
		if(sectioninfo.CountUprate >section_rate){
			valid_section_up=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item.Section);
		}

		if(sectioninfo.CountDownrate >section_rate){
			valid_section_down=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item.Section);
		}
	// Open_SpreadSignal_Average 以上代码和策略函数内容一致

		if(signal.Direction==0){

			sprintf(logbuf,"	%s-----> 上涨品种涨幅[%d]: %.2f%%  上涨品种比例:%.2f%%   SedimentaryIO:%.2f  资金流动比例: %.2f%%  平均涨跌幅: %.2f%% 最高涨跌幅:%.2f%%--> [%s]\n",
					sectioninfo.SectionName,sectioninfo.UpCount,sectioninfo.TotalUprate*100,sectioninfo.CountUprate*100,sectioninfo.SedimentaryIO,sectioninfo.SedimentaryIORate*100,sectioninfo.AverageUpDownrate*100,
					sectioninfo.Maxrate*100,sectioninfo.MaxInstrumentID);
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if (signal.Direction==1){
			sprintf(logbuf,"	%s-----> 下跌品种总跌幅[%d]: %.2f%%  下跌品种比例:%.2f%%  SedimentaryIO:%.2f  资金流动比例: %.2f%%  平均涨跌幅: %.2f%% 最低涨跌幅:%.2f%%--> [%s]\n",
					sectioninfo.SectionName,sectioninfo.DownCount,sectioninfo.TotalDownrate*100,sectioninfo.CountDownrate*100,sectioninfo.SedimentaryIO,sectioninfo.SedimentaryIORate*100,sectioninfo.AverageUpDownrate*100,
					sectioninfo.Minrate*100,sectioninfo.MinInstrumentID);
			this->GetLogUtil()->WriteLog(logbuf);
		}


	}


	/**********************************************************/


	if (signal.SingalActive && !signal.OrderSend){

		printf("	信号激活,等待验证 --> %s---多头仓位[%d] 空头仓位[%d] --发单[%d] -排队[%d]  -多头排队仓位[%d] 空头排队仓位[%d]-----成交[%d] \n",
					pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count);

		if(signal.Direction==0    && !valid_globalview_short){

				printf("	信号激活 -->委托下多单 --> %s   ----开仓量[%d]\n", pinstrumentid,v);
				this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,v);
				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
				signal.OrderSend=true;
				this->GetRedisSignal()->SetInsSignal(signal,signalname);
		}
		else if(signal.Direction==1    && !valid_globalview_long){
			printf("	信号激活 -->委托下空单 --> %s\n", pinstrumentid);
			this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,v);
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
			signal.OrderSend=true;
			this->GetRedisSignal()->SetInsSignal(signal,signalname);
		}

	}
	else if(signal.SingalActive && signal.OrderSend){
		bool maxactionnumber =action_count<=this->GetMaxActionNumber() ;
		if (!maxactionnumber){
			printf("	激活信号,未通过验证 --> [%s]交易尝试次数过多，超出限额,不再交易!\n",pinstrumentid);
			sprintf(logbuf,"	激活信号,未通过验证 --> [%s]交易尝试次数过多，超出限额,不再交易!\n",pinstrumentid);
			this->GetLogUtil()->WriteLog(logbuf);
			return;
		}

		printf("	信号激活 发送过委托单,等待验证 %s ---多头仓位[%d] 空头仓位[%d] --> 发单数[%d] ---排队[%d]---   -多头排队仓位[%d] 空头排队仓位[%d]-成交单[%d]----> 撤单次数[%d]  \n",
				pinstrumentid,longposition,shortposition,send_count,queue_count,longreqvolume,shortreqvolume,done_count,action_count);

		if(signal.Direction==0 && !valid_globalview_short )	{
			sprintf(logbuf,"	撤单[%d]次后,再次委托下多单 --> %s\n", action_count,pinstrumentid);
			this->GetLogUtil()->WriteLog(logbuf);
			this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.AskPrice1,v);
		}
		else if(signal.Direction==1    && !valid_globalview_long )	{
			sprintf(logbuf,"	撤单[%d]次后,再次委托下空单 --> %s\n", action_count,pinstrumentid);
			this->GetLogUtil()->WriteLog(logbuf);
			this->ReqOrderInsertbySignal(&signal,item->ExchangeID,depthmarket.BidPrice1,v);
		}

	}

//	this->GetLogUtil()->WriteLog("TradeOpenCheck---------------->End!!!");
}


void TrendTrader::ClosebySignal(const char * postfix)
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
void TrendTrader::QueueingPriceChange_CancelCheck()
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
void TrendTrader::QueueingClosePriceChange_CancelCheck()
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

// 检测本地的开仓单 是否上报到后台
void TrendTrader::CheckLocalOrderList(const char* tablename)
{


	std::string tradingdate;
	Config config(this->GetStrategyName().c_str());
	MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);

	vector<OrderData>  orderlist;
	vector<OrderData>  sendlists;

	mysqlopenorder.Find_OrderListsbyStatus(sendlists,this->GetTradingDay().c_str(),0);  // 有发送的开仓单

	MysqlOrder mysqlorderlist(config.GetTable_OrderList().c_str(),this->GetPointUserLoginField()->UserID);
	mysqlorderlist.Find_AllLists(orderlist);

	if(sendlists.size()>0)   // 当天存在 open 委托单
	{
		printf("CheckOpenOrderList--->sendlists.size [%ld]  orderlist.size [%ld]\n",sendlists.size(),orderlist.size());
		mysqlorderlist.DeleteAllbyTableName(mysqlorderlist.getTablename().c_str());
		this->ReqQryOrder();


		for (auto &item:sendlists)    // 逐笔比对
		{
			bool exists=false;

			printf("CheckOpenOrderList--->%s\n",item.InstrumentID);
			for (auto &orderitem:orderlist)
			{
				if(strcmp(item.InstrumentID,orderitem.InstrumentID)==0)
				{
					if(orderitem.Direction==0)
					{
						exists=true;
					}

				}
			}



			if (!exists)  // 后台无order委托单
			{

				printf("CheckOpenOrderList--->后台无order委托单  %s\n",item.InstrumentID);
				OrderData info;
				strcpy(info.InstrumentID,item.InstrumentID);
				strcpy(info.OrderRef,item.OrderRef);
				strcpy(info.TradingDay,item.TradingDay);
				info.Status=-1;
				info.ErrorId=25;  // 后台找不到相应报单
				mysqlopenorder.UpdateOrderData_byErrRtnOrder(info);
			}

		}

	}


	vector<OrderData>().swap(orderlist);
	vector<OrderData>().swap(sendlists);
}
void TrendTrader::AccountSelfCheck()
{

	//本地有发送 无回应的委托单  则启动
	Config config(this->GetStrategyName().c_str());
	printf(" AccountSelfCheck  CheckLocalOrderList open \n");
	this->CheckLocalOrderList(config.GetTable_Open().c_str());

	printf(" AccountSelfCheck  CheckLocalOrderList close \n");
	this->CheckLocalOrderList(config.GetTable_Close().c_str());

	printf(" AccountSelfCheck  CheckSqlHoldPositionbyPosDetail\n");
	this->CheckSqlHoldPositionbyPosDetail();

	//	CancelOrderList();

//	printf(" AccountSelfCheck  CheckPosDetailbySqlHoldPosition\n");
//	this->CheckPosDetailbySqlHoldPosition();

	printf(" AccountSelfCheck  CheckInvestbySqlHoldPosition\n");
	this->CheckInvestbySqlHoldPosition();
	printf(" AccountCheck\n");
	this->AccountCheck();

}
void TrendTrader::DayTrade_AccountSelfCheck()
{

	//本地有发送 无回应的委托单  则启动
	Config config(this->GetStrategyName().c_str());
	printf(" DayTrade_AccountSelfCheck  open \n");
	this->CheckLocalOrderList(config.GetTable_Open().c_str());
	printf(" DayTrade_AccountSelfCheck  close\n");
	this->CheckLocalOrderList(config.GetTable_Close().c_str());

	printf(" DayTrade_AccountSelfCheck  DayTrade_CheckSqlHoldPositionbyPosDetail\n");
	//	CancelOrderList();
	this->DayTrade_CheckSqlHoldPositionbyPosDetail();
	printf(" DayTrade_AccountSelfCheck  CheckPosDetailbySqlHoldPosition\n");
	this->CheckPosDetailbySqlHoldPosition();
	printf(" DayTrade_AccountSelfCheck  AccountCheck\n");
		//

	this->AccountCheck();
	printf(" AccountSelfCheck  end \n");
}

void TrendTrader::AccountCheck()
{
	printf(" TrendTrader::AccountCheck:   开始!! \n");
	DateUtil dateutil;
	double totalprofit=0;
	double ydtotalprofit=0;
	double holdtotalprofit=0;
	double totalcolseprofit=0;
	double totalcommission=0;   //佣金总计

	double staticequity=0;   //静态权益
	double dynamicequity=0;  //动态权益
	double HighestProfit=0;  //当日历史最高浮盈

	double CloseProfit=0;   //平仓浮盈
	double totalholding=0;  //总持仓数


//	MysqlAsset mysqlasset(this->GetUserID().c_str());
	AssetData assetdata= this->GetMysqlAsset()->Find_LastDayAsset();

	if(strcmp(assetdata.TradingDay,this->GetTradingDay().c_str())==0)	{
		staticequity = assetdata.PreBalance;
		printf("TrendTrader::AccountCheck:InvestPosition 持仓汇总监测----- ----TradingDay[%s] 静态权益 --%.3f  可用资金---%.3f \n",
				this->GetTradingDay().c_str(),staticequity,assetdata.Available);
	}
	else	{
		this->CheckTradingDay();
		assetdata= this->GetMysqlAsset()->Find_LastDayAsset();
		staticequity = assetdata.PreBalance;   //静态权益
		printf("TrendTrader::AccountCheck:  交易日不同,重新请求后,InvestPosition 持仓汇总监测 ----- ----日期[%s] 静态权益 --%.3f 可用资金---%.3f \n",
				assetdata.TradingDay,staticequity,assetdata.Available);
	}

	this->CheckSettleInfoConfirmbyTrade();

	int c=this->GetMysqlInvestPosition()->Count_AllLists();
	printf("TrendTrader:: AccountCheck:   count %d \n",c);
	vector<InvestPositionData> list;
	this->GetMysqlInvestPosition()->Find_AllLists(list);

	for (auto &item:list)	{
			totalholding +=item.Position; //计算总持仓数
			char productid[31];
			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

			MysqlProduct mysqlproduct;
			ProductData productdata=mysqlproduct.Find_DatabyProductID(productid);
			std:string dir;
			if (dateutil.CheckOpenTime(this->GetDifSec(),productdata.ExchangeID,productid)  )	{

					double profit=0;  //浮盈
					double holdprofit=0;   //持仓浮盈
					double presettleprofit=0;  //昨结浮盈

					DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

					MysqlInstrument mysqlinstrument;
					//检测对应的合约信息是否存在,不存在则请求下载
					if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
						this->ReqQrySingleInstrument(item.InstrumentID);
					}
					InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

					totalcommission+=item.Commission;
					totalcolseprofit+=item.CloseProfit;

					if (item.PosiDirection ==2 ) {		dir="多头";			}
					else if (item.PosiDirection ==3 ) {	dir="空头";			}
//						printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", instrumentid,mat[0].str().c_str());
//						printf("资金账户: ----->  静态权益: %.2f 浮盈: %.2f \n",totalcolseprofit,totalcommission);
					if(item.Position >0)	{

						if (item.PosiDirection ==2 )	{
							if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)
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
							presettleprofit = (item.PreSettlementPrice - item.OpenPriceAverage) *
										(double)instrumentdata.VolumeMultiple*	(double)item.YdPosition;


						}
						else if (item.PosiDirection ==3 )
						{
							if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)
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

							presettleprofit = (item.OpenPriceAverage-item.PreSettlementPrice ) *
												(double)instrumentdata.VolumeMultiple*	(double)item.YdPosition;
						}

						totalprofit =totalprofit +profit;
						ydtotalprofit =ydtotalprofit +presettleprofit;
						holdtotalprofit = holdtotalprofit +holdprofit;

						if(depthmarket.LastPrice >1.0 &&  item.OpenPriceAverage >1.0)
						{
							printf("	持仓合约[%s] ----->%s  仓位[%d] 昨仓[%d] 今仓[%d]---->  开仓均价: %.3f  现价:%.3f  占用保证金: %.2f 浮盈: %.2f 昨结浮盈: %.2f 持仓浮盈: %.2f\n",
								item.InstrumentID,dir.c_str(),item.Position,item.YdPosition,item.TodayPosition,
								item.OpenPriceAverage,depthmarket.LastPrice,item.UseMargin,	profit,presettleprofit,holdprofit);
						}
						else
						{
							printf("	持仓合约[%s] ---  行情数据不合规  开仓价: %.3f   最新价: %.3f \n",item.InstrumentID,item.OpenPriceAverage,depthmarket.LastPrice);
						}

					}

					else
					{
							printf("	平仓合约: ----->  %s 平仓的合约----> 手数: %d  开仓均价: %.3f  占用保证金: %.2f 平仓浮盈: %.2f 持仓浮盈: %.2f\n",
									item.InstrumentID,item.Position,item.OpenPriceAverage,item.UseMargin,
										item.CloseProfit,item.PositionProfit);
					}

				}
				else
				{

					if(item.Position >0)
					{
						printf("	已收盘-持仓合约: ----->  %s ---> 方向: %s 手数: %d  开仓均价: %.3f  占用保证金: %.3f 持仓浮盈: %.3f\n",
						item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,	item.UseMargin,item.PositionProfit);

					}
					else
					{
						printf("	已收盘-平仓合约: ----->  %s --->  方向: %s 手数: %d  开仓均价: %.3f  占用保证金: %.3f 平仓浮盈: %.3f\n",
							item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,	item.UseMargin,item.CloseProfit);
					}

				}
//			printf("资金账户0: ----->  InstrumentID[%d]: %s \n",i,tradeinsidlist.pinstrumentid[i].c_str());

		}

	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0){

		staticequity =50000.0;
		//动态权益
		dynamicequity = staticequity + holdtotalprofit + totalcolseprofit -totalcommission;
		//可用资金
		double avalible = dynamicequity -assetdata.CurrMargin;
		this->SetAvalible(avalible);

		//资金利用率  --> 仓位
		double holdpositionrate= assetdata.CurrMargin / dynamicequity;
		this->SetHoldPositionRate(holdpositionrate);

		//总持仓量
		this->SetTotalHolding(totalholding);

		printf("TrendTrader::AccountCheck:资金账户-->静态权益:%.2f 浮盈:%.2f  持仓盈亏:%.2f 平仓盈亏:%.2f 佣金:%.2f 昨日结算浮盈:%.2f 动态权益:%.2f 占用保证金:%.2f 仓位:%.2f 可用资金:%.2f\n",
				staticequity,totalprofit,holdtotalprofit,totalcolseprofit,totalcommission,ydtotalprofit,dynamicequity,assetdata.CurrMargin,holdpositionrate, avalible);


	}
	else {

		//动态权益
		dynamicequity = staticequity + holdtotalprofit + totalcolseprofit -totalcommission;
		//可用资金
		double avalible = dynamicequity -assetdata.CurrMargin;
		this->SetAvalible(avalible);

		//资金利用率  --> 仓位
		double holdpositionrate= assetdata.CurrMargin / dynamicequity;
		this->SetHoldPositionRate(holdpositionrate);

		//总持仓量
		this->SetTotalHolding(totalholding);

		printf("TrendTrader::AccountCheck:资金账户-->静态权益:%.2f 浮盈:%.2f  持仓盈亏:%.2f 平仓盈亏:%.2f 佣金:%.2f 昨日结算浮盈:%.2f 动态权益:%.2f 占用保证金:%.2f 仓位:%.2f 可用资金:%.2f\n",
				staticequity,totalprofit,holdtotalprofit,totalcolseprofit,totalcommission,ydtotalprofit,dynamicequity,assetdata.CurrMargin,holdpositionrate, avalible);


	}

	if(totalprofit> assetdata.HighestProfit)
	{
		assetdata.HighestProfit =totalprofit;
		assetdata.Profit = totalprofit;
		assetdata.HoldProfit = holdtotalprofit;
		assetdata.DynamicEquity = dynamicequity;
		this->GetMysqlAsset()->Update(assetdata);
	}
	else if(totalprofit< assetdata.HighestProfit/2.0)
	{

	}
	vector<InvestPositionData>().swap(list);

	printf("TrendTrader::AccountCheck:   End!! \n");
}

double TrendTrader::GetProfitbyInvestPositionData(InvestPositionData *item){

	double profit=0.0;
	DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID);

	MysqlInstrument mysqlinstrument;
	//检测对应的合约信息是否存在,不存在则请求下载
	if(!mysqlinstrument.Exist_DatabyInstrumentID(item->InstrumentID))	{
		this->ReqQrySingleInstrument(item->InstrumentID);
	}
	InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item->InstrumentID);

//						printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", instrumentid,mat[0].str().c_str());
//						printf("资金账户: ----->  静态权益: %.2f 浮盈: %.2f \n",totalcolseprofit,totalcommission);
	if(item->Position >0)	{

		if (item->PosiDirection ==2 )	{
			if(depthmarket.LastPrice >1.0 && item->OpenPriceAverage >1.0)		{

				profit = (depthmarket.LastPrice - item->OpenPriceAverage) *depthmarket.VolumeMultiple*(double)item->Position;
			}
			else	{

				profit=0;
			}


		}
		else if (item->PosiDirection ==3 )	{
			if(depthmarket.LastPrice >1.0 && item->OpenPriceAverage >1.0)	{

				profit = ( item->OpenPriceAverage - depthmarket.LastPrice) *depthmarket.VolumeMultiple*	(double)item->Position;
			}
			else	{

				profit=0;
			}

		}

	}

	return profit;
}
void TrendTrader::FixAccountCheck(double avalible)
{
	printf("------------------>TrendTrader::FixAccountCheck:   开始!!    固定资金 : %.3f  \n",avalible);
	DateUtil dateutil;
	double totalprofit=0;
	double ydtotalprofit=0;
	double holdtotalprofit=0;
	double totalcolseprofit=0;
	double totalcommission=0;   //佣金总计

	double staticequity=0;   //静态权益
	double dynamicequity=0;  //动态权益
	double HighestProfit=0;  //当日历史最高浮盈

	double CloseProfit=0;   //平仓浮盈
	double totalholding=0;  //总持仓数
	double total_margin=0;
	AssetData assetdata= this->GetMysqlAsset()->Find_LastDayAsset();

	if(strcmp(assetdata.TradingDay,this->GetTradingDay().c_str())==0)	{
		staticequity = assetdata.StaticEquity;
//		printf("TrendTrader::FixAccountCheck	InvestPosition 持仓汇总监测----- ----TradingDay[%s] 静态权益 --%.3f  可用资金---%.3f \n",
//				this->GetTradingDay().c_str(),staticequity,assetdata.Available);
	}
	else	{
		sleep(60);
		this->ReqQryTradingAccount();
		sleep(5);

//		this->CheckSettleInfoConfirmbyTrade();

		this->CheckTradingDay();
		assetdata= this->GetMysqlAsset()->Find_LastDayAsset();
		staticequity = assetdata.PreBalance;   //静态权益
		printf("TrendTrader::FixAccountCheck	交易日不同,重新请求后,InvestPosition 持仓汇总监测 ----- ----日期[%s] 静态权益 --%.3f 可用资金---%.3f \n",
				assetdata.TradingDay,staticequity,assetdata.Available);
	}

	int c=this->GetMysqlInvestPosition()->Count_AllLists();
//	printf("TrendTrader::FixAccountCheck	当前账户持仓合约数: %d \n",c);
	vector<InvestPositionData> list;
	this->GetMysqlInvestPosition()->Find_AllLists(list);

	for (auto &item:list)	{
		totalholding +=item.Position; //计算总持仓数
		char productid[31];
		CodeUtil codeutil;
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

		MysqlProduct mysqlproduct;
		ProductData productdata=mysqlproduct.Find_DatabyProductID(productid);
		std:string dir;
		if (item.PosiDirection ==2 ) {		dir="多头";			}
		else if (item.PosiDirection ==3 ) {	dir="空头";			}
		else{		dir="未知";		}

		if (dateutil.CheckOpenTime(this->GetDifSec(),productdata.ExchangeID,productid)  )	{

				double profit=0;  //浮盈
				double holdprofit=0;   //持仓浮盈
				double presettleprofit=0;  //昨结浮盈

				DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

				int VolumeMultiple=0;
				if(depthmarket.VolumeMultiple>0){
					VolumeMultiple = depthmarket.VolumeMultiple;
				}
				else{

					MysqlInstrument mysqlinstrument;
					//检测对应的合约信息是否存在,不存在则请求下载
					if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
						this->ReqQrySingleInstrument(item.InstrumentID);
					}
					InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
					VolumeMultiple = instrumentdata.VolumeMultiple;
				}


				totalcommission+=item.Commission;


//						printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", instrumentid,mat[0].str().c_str());
//						printf("资金账户: ----->  静态权益: %.2f 浮盈: %.2f \n",totalcolseprofit,totalcommission);
				if(item.Position >0)	{

					total_margin+=item.UseMargin;

					if (item.PosiDirection ==2 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)		{
							holdprofit = ( depthmarket.LastPrice - item.OpenPriceAverage) *
										(double)VolumeMultiple*	(double)item.TodayPosition
										+(depthmarket.LastPrice - item.PreSettlementPrice) *
										(double)VolumeMultiple*	(double)item.YdPosition;

							profit = (depthmarket.LastPrice - item.OpenPriceAverage) *
							(double)VolumeMultiple*	(double)item.Position;
						}
						else	{
							holdprofit=0;
							profit=0;
						}
						presettleprofit = (item.PreSettlementPrice - item.OpenPriceAverage) *
									(double)VolumeMultiple*	(double)item.YdPosition;


					}
					else if (item.PosiDirection ==3 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)	{
							holdprofit = (item.OpenPriceAverage- depthmarket.LastPrice ) *
										(double)VolumeMultiple*	(double)item.TodayPosition
										+( item.PreSettlementPrice - depthmarket.LastPrice) *
											(double)VolumeMultiple*	(double)item.YdPosition;
							profit = ( item.OpenPriceAverage - depthmarket.LastPrice) *
											(double)VolumeMultiple*	(double)item.Position;
						}
						else	{
							holdprofit=0;
							profit=0;
						}

						presettleprofit = (item.OpenPriceAverage-item.PreSettlementPrice ) *
											(double)VolumeMultiple*	(double)item.YdPosition;
					}

					totalprofit =totalprofit +profit;
					ydtotalprofit =ydtotalprofit +presettleprofit;
					holdtotalprofit = holdtotalprofit +holdprofit;

					if(depthmarket.LastPrice >1.0 &&  item.OpenPriceAverage >1.0)	{
						printf("	持仓合约[%s] ----->%s  仓位[%d] 昨仓[%d] 今仓[%d]---->  开仓均价: %.3f  现价:%.3f  占用保证金: %.2f 浮盈: %.2f 昨结浮盈: %.2f 持仓浮盈: %.2f\n",
							item.InstrumentID,dir.c_str(),item.Position,item.YdPosition,item.TodayPosition,
							item.OpenPriceAverage,depthmarket.LastPrice,item.UseMargin,	profit,presettleprofit,holdprofit);
					}
					else	{
						printf("	持仓合约[%s] ---  行情数据不合规  开仓价: %.3f   最新价: %.3f \n",item.InstrumentID,item.OpenPriceAverage,depthmarket.LastPrice);
					}

				}
				else	{

					double closeprice= 	item.CloseAmount/(double)item.CloseVolume;
					totalprofit =totalprofit +profit;
					totalcolseprofit+=item.CloseProfit;
					printf("	平仓合约[%s]: ----->  %s 平仓的合约----> 手数: %d  开仓均价: %.3f  平仓价: %.3f  平仓盈利: %.2f \n",
						item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,closeprice,item.CloseProfit);
				}

			}
			else	{   // 未在开盘时间内
				double profit=0;  //浮盈
				totalcommission+=item.Commission;

				if(item.Position >0)		{
					total_margin+=item.UseMargin;

					DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

					int VolumeMultiple=0;
					if(depthmarket.VolumeMultiple>0){
						VolumeMultiple = depthmarket.VolumeMultiple;
					}
					else{

						MysqlInstrument mysqlinstrument;
						//检测对应的合约信息是否存在,不存在则请求下载
						if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
							this->ReqQrySingleInstrument(item.InstrumentID);
						}
						InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
						VolumeMultiple = instrumentdata.VolumeMultiple;
					}

					if (item.PosiDirection ==2 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)		{
							profit = (depthmarket.LastPrice - item.OpenPriceAverage) *
							(double)VolumeMultiple*	(double)item.Position;
//							printf("TrendTrader::FixAccountCheck  行情价格合规	   %.3f \n",depthmarket.LastPrice);
						}
						else	{
							profit=0;
							printf("TrendTrader::FixAccountCheck 行情价格合规不合规	   %.3f \n",depthmarket.LastPrice);
						}
					}
					else if (item.PosiDirection ==3 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)	{
							profit = ( item.OpenPriceAverage - depthmarket.LastPrice) *
											(double)VolumeMultiple*	(double)item.Position;
//							printf("TrendTrader::FixAccountCheck  行情价格合规	   %.3f \n",depthmarket.LastPrice);
						}
						else	{
							profit=0;
							printf("TrendTrader::FixAccountCheck 行情价格合规不合规	   %.3f \n",depthmarket.LastPrice);
						}
					}

				totalprofit =totalprofit +profit;
				printf("	已收盘-持仓合约: ----->  %s ---> 方向: %s 手数: %d  开仓均价: %.3f  现价: %.3f 占用保证金: %.3f[%.2f%%]  浮盈: %.3f\n",
				item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,depthmarket.LastPrice,item.UseMargin,item.UseMargin/staticequity*100,profit);

			}
			else		{
				DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
				int VolumeMultiple=0;
				if(depthmarket.VolumeMultiple>0){
					VolumeMultiple = depthmarket.VolumeMultiple;
				}
				else{

					MysqlInstrument mysqlinstrument;
					//检测对应的合约信息是否存在,不存在则请求下载
					if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
						this->ReqQrySingleInstrument(item.InstrumentID);
					}
					InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
					VolumeMultiple = instrumentdata.VolumeMultiple;
				}


				double closeprice= 	item.CloseAmount/((double)item.CloseVolume*VolumeMultiple);
				totalprofit =totalprofit +profit;
				totalcolseprofit+=item.CloseProfit;
				printf("	已收盘-平仓合约: >>>>>>  %s --->  方向: %s 手数: %d  开仓均价: %.3f  平仓均价: %.3f  平仓浮盈: %.3f\n",
					item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,closeprice,item.CloseProfit);
			}

		}
//			printf("资金账户0: ----->  InstrumentID[%d]: %s \n",i,tradeinsidlist.pinstrumentid[i].c_str());

	}

	//动态权益
	dynamicequity = staticequity + holdtotalprofit + totalcolseprofit -totalcommission;
	//可用资金
	double ava = dynamicequity -assetdata.CurrMargin;
	this->SetAvalible(ava);

	//资金利用率  --> 仓位
	double holdpositionrate= total_margin/dynamicequity;
	this->SetHoldPositionRate(holdpositionrate);

	//总持仓量
	this->SetTotalHolding(totalholding);

//	printf("	资金账户-->静态权益:%.2f 动态权益:%.2f 实际账户可用资金:%.2f 固定可用资金:%.2f 出金:%.2f  入金:%.2f\n",
//			staticequity,dynamicequity, ava,avalible,assetdata.Withdraw,assetdata.Deposit);
//	printf("	资金账户-->浮盈:%.2f  持仓盈亏:%.2f 平仓盈亏:%.2f 佣金:%.2f 昨日结算浮盈:%.2f  占用保证金:%.2f[%.2f%%] \n",
//				totalprofit,holdtotalprofit,totalcolseprofit,totalcommission,ydtotalprofit,total_margin,holdpositionrate*100);

	if(totalprofit> assetdata.HighestProfit)	{
		assetdata.HighestProfit =totalprofit;
		assetdata.Profit = totalprofit;
		assetdata.HoldProfit = holdtotalprofit;
		assetdata.DynamicEquity = dynamicequity;
		this->GetMysqlAsset()->Update(assetdata);
	}
	else if(totalprofit< assetdata.HighestProfit/2.0)	{

	}
	vector<InvestPositionData>().swap(list);

	printf("------------------>TrendTrader::FixAccountCheck	   End!! \n");
}
int TrendTrader::UpdateInvestPosition()
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
	//
	this->SetTotalHolding(investlist.size());
	 std::cout<<"TrendTrader::UpdateInvestPosition---------->总持仓合约品种:"<<investlist.size()
			 <<"总仓位单数:"<<investholdposions<<std::endl;
	for (auto &item:investlist)	{
		if(item.Position>0){
			printf("	持仓合约: ----->  %s ---> 方向:%d  手数:%d  开仓均价: %.3f OpenAmount:%.2f OpenVolume:%d 占用保证金: %.3f[%.2f%%]  持仓浮盈:%.3f\n",
						item.InstrumentID,item.PosiDirection,item.Position,item.OpenPriceAverage,item.OpenAmount,item.OpenVolume,
						item.UseMargin,item.UseMargin/assetdata->PreBalance*100,item.PositionProfit);
		}
		else{

			printf("	平仓合约: ----->  %s ---> 方向:%d  手数:%d  开仓均价: %.3f CloseAmount:%.3f  CloseVolume:%d 平仓浮盈:%.3f\n",
						item.InstrumentID,item.PosiDirection,item.Position,item.OpenPriceAverage,
						item.CloseAmount,item.CloseVolume,item.CloseProfit);
		}
	}

	vector<PositionDetailData> detaillist;
	this->GetDataWrapper()->GetPositionDetailList(detaillist);
	int position=this->GetDataWrapper()->GetDetailPosition();
	std::cout<<"TrendTrader::UpdateInvestPosition-------->开仓品种:"<<detaillist.size()<<"总仓位单数:"<<position<<std::endl;
	for (auto &item:detaillist)	{
		printf("	持仓明细: ----->  %s ---> 方向:%d  手数:%d  开仓价:%.2f  保证金:%.2f TradeID:%s  opendate:%s\n",
				item.InstrumentID,item.Direction,item.Volume,item.OpenPrice,item.Margin,item.TradeID,item.OpenDate);
	}

	vector<InvestPositionData>().swap(investlist);
	vector<PositionDetailData>().swap(detaillist);
	return num;

}
int TrendTrader::MarketClose_UpdateInvestPosition()
{
	int num;
	std::shared_ptr<AssetData> assetdata = this->GetDataWrapper()->FindAssetData(this->GetUserID().c_str());
	 printf("账户[%s]       收盘--->账户查询 交易日[%s] 静态权益:%.3f  动态权益:%.3f  保证金:%.3f  可用资金:%.3f 浮盈:%.3f\n",
			 assetdata->AccountID,assetdata->TradingDay,assetdata->PreBalance,assetdata->DynamicEquity,assetdata->CurrMargin,
			 assetdata->Available,assetdata->HoldProfit);
	 printf("账户[%s]       收盘--->手续费:%.3f  出金:%.3f  入金:%.3f  本地更新时间:%s\n",
			 assetdata->AccountID,assetdata->Commission,assetdata->Withdraw,assetdata->Deposit,assetdata->LocalUpdatetime);
	 this->SetAvalible(assetdata->PreBalance);
	//资金利用率  --> 仓位
	double holdpositionrate= assetdata->CurrMargin/assetdata->PreBalance;
	this->SetHoldPositionRate(holdpositionrate);

	 //总持仓量
	vector<InvestPositionData> investlist;
	this->GetDataWrapper()->GetInvestList(investlist);
	this->SetTotalHolding(investlist.size());
	 std::cout<<"TrendTrader::MarketClose_UpdateInvestPosition---------->总持仓合约品种:"<<investlist.size()<<std::endl;
	for (auto &item:investlist)	{
		if(item.Position>0){
			printf("	已收盘-持仓合约: ----->  %s ---> 方向:%d  手数:%d  开仓均价: %.3f 占用保证金: %.3f[%.2f%%]  持仓浮盈:%.3f\n",
						item.InstrumentID,item.PosiDirection,item.Position,item.OpenPriceAverage,
						item.UseMargin,item.UseMargin/assetdata->PreBalance*100,item.PositionProfit);
		}
		else{

			printf("	已收盘-平仓合约: ----->  %s ---> 方向:%d  手数:%d  开仓均价: %.3f 平仓均价:%.3f  平仓浮盈:%.3f\n",
						item.InstrumentID,item.PosiDirection,item.Position,item.OpenPriceAverage,
						item.CloseAmount,item.CloseProfit);
		}
	}

	vector<PositionDetailData> detaillist;
	this->GetDataWrapper()->GetPositionDetailList(detaillist);
	std::cout<<"TrendTrader::MarketClose-------->持仓明细数:"<<detaillist.size()<<std::endl;
	for (auto &item:detaillist)	{
		printf("	已收盘-持仓明细: ----->  %s ---> 方向:%d  手数:%d  开仓价:%.2f  保证金:%.2f TradeID:%s\n",
				item.InstrumentID,item.Direction,item.Volume,item.OpenPrice,item.Margin,item.TradeID);
	}
	num=investlist.size();
	vector<InvestPositionData>().swap(investlist);
	vector<PositionDetailData>().swap(detaillist);
	return num;

}
void TrendTrader::UpdateAccount()
{

	DateUtil dateutil;

	AssetData tmp_asset;
	tmp_asset.Profit=0.0;

	double totalprofit=0;


	double ydtotalprofit=0;
	double holdtotalprofit=0;
	double totalcolseprofit=0;
	double totalcommission=0;   //佣金总计

	double staticequity=0;   //静态权益
	double dynamicequity=0;  //动态权益
	double HighestProfit=0;  //当日历史最高浮盈

	double CloseProfit=0;   //平仓浮盈
	double totalholding=0;  //总持仓数
	double total_margin=0;

//	MysqlAsset mysqlasset(this->GetUserID().c_str());

	AssetData assetdata= this->GetMysqlAsset()->Find_LastDayAsset();

	if(strcmp(assetdata.TradingDay,this->GetTradingDay().c_str())==0)	{
		staticequity = assetdata.StaticEquity;
		printf("TrendTrader::FixAccountCheck	InvestPosition 持仓汇总监测----- ----TradingDay[%s] 静态权益 --%.3f  可用资金---%.3f \n",
				this->GetTradingDay().c_str(),staticequity,assetdata.Available);
	}
	else	{
		sleep(60);
		this->ReqQryTradingAccount();
		sleep(5);

//		this->CheckSettleInfoConfirmbyTrade();

		this->CheckTradingDay();
		assetdata= this->GetMysqlAsset()->Find_LastDayAsset();
		staticequity = assetdata.PreBalance;   //静态权益
		printf("TrendTrader::FixAccountCheck	交易日不同,重新请求后,InvestPosition 持仓汇总监测 ----- ----日期[%s] 静态权益 --%.3f 可用资金---%.3f \n",
				assetdata.TradingDay,staticequity,assetdata.Available);
	}

	int c=this->GetMysqlInvestPosition()->Count_AllLists();
	printf("TrendTrader::FixAccountCheck	当前账户持仓合约数: %d \n",c);
	vector<InvestPositionData> list;
	this->GetMysqlInvestPosition()->Find_AllLists(list);

	for (auto &item:list)	{
		totalholding +=item.Position; //计算总持仓数
		char productid[31];
		CodeUtil codeutil;
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

		MysqlProduct mysqlproduct;
		ProductData productdata=mysqlproduct.Find_DatabyProductID(productid);
		std:string dir;
		if (item.PosiDirection ==2 ) {		dir="多头";			}
		else if (item.PosiDirection ==3 ) {	dir="空头";			}
		else{		dir="未知";		}


		if (dateutil.CheckOpenTime(this->GetDifSec(),productdata.ExchangeID,productid)  )	{

				double profit=0;  //浮盈
				double holdprofit=0;   //持仓浮盈
				double presettleprofit=0;  //昨结浮盈

				DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

				int VolumeMultiple=0;
				if(depthmarket.VolumeMultiple>0){
					VolumeMultiple = depthmarket.VolumeMultiple;
				}
				else{

					MysqlInstrument mysqlinstrument;
					//检测对应的合约信息是否存在,不存在则请求下载
					if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
						this->ReqQrySingleInstrument(item.InstrumentID);
					}
					InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
					VolumeMultiple = instrumentdata.VolumeMultiple;
				}


				totalcommission+=item.Commission;


//						printf("----->  %s 匹配主力合约的关键字--mat-[]--->%s \n", instrumentid,mat[0].str().c_str());
//						printf("资金账户: ----->  静态权益: %.2f 浮盈: %.2f \n",totalcolseprofit,totalcommission);
				if(item.Position >0)	{

					total_margin+=item.UseMargin;

					if (item.PosiDirection ==2 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)		{
							holdprofit = ( depthmarket.LastPrice - item.OpenPriceAverage) *
										(double)VolumeMultiple*	(double)item.TodayPosition
										+(depthmarket.LastPrice - item.PreSettlementPrice) *
										(double)VolumeMultiple*	(double)item.YdPosition;

							profit = (depthmarket.LastPrice - item.OpenPriceAverage) *
							(double)VolumeMultiple*	(double)item.Position;
						}
						else	{
							holdprofit=0;
							profit=0;
						}
						presettleprofit = (item.PreSettlementPrice - item.OpenPriceAverage) *
									(double)VolumeMultiple*	(double)item.YdPosition;


					}
					else if (item.PosiDirection ==3 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)	{
							holdprofit = (item.OpenPriceAverage- depthmarket.LastPrice ) *
										(double)VolumeMultiple*	(double)item.TodayPosition
										+( item.PreSettlementPrice - depthmarket.LastPrice) *
											(double)VolumeMultiple*	(double)item.YdPosition;
							profit = ( item.OpenPriceAverage - depthmarket.LastPrice) *
											(double)VolumeMultiple*	(double)item.Position;
						}
						else	{
							holdprofit=0;
							profit=0;
						}

						presettleprofit = (item.OpenPriceAverage-item.PreSettlementPrice ) *
											(double)VolumeMultiple*	(double)item.YdPosition;
					}

					totalprofit =totalprofit +profit;
					ydtotalprofit =ydtotalprofit +presettleprofit;
					holdtotalprofit = holdtotalprofit +holdprofit;

					if(depthmarket.LastPrice >1.0 &&  item.OpenPriceAverage >1.0)	{
						printf("	持仓合约[%s] ----->%s  仓位[%d] 昨仓[%d] 今仓[%d]---->  开仓均价: %.3f  现价:%.3f  占用保证金: %.2f 浮盈: %.2f 昨结浮盈: %.2f 持仓浮盈: %.2f\n",
							item.InstrumentID,dir.c_str(),item.Position,item.YdPosition,item.TodayPosition,
							item.OpenPriceAverage,depthmarket.LastPrice,item.UseMargin,	profit,presettleprofit,holdprofit);
					}
					else	{
						printf("	持仓合约[%s] ---  行情数据不合规  开仓价: %.3f   最新价: %.3f \n",item.InstrumentID,item.OpenPriceAverage,depthmarket.LastPrice);
					}

				}
				else	{

					double closeprice= 	item.CloseAmount/(double)item.CloseVolume;
					totalprofit =totalprofit +profit;
					totalcolseprofit+=item.CloseProfit;
					printf("	平仓合约[%s]: ----->  %s 平仓的合约----> 手数: %d  开仓均价: %.3f  平仓价: %.3f  平仓盈利: %.2f \n",
						item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,closeprice,item.CloseProfit);
				}

			}
			else	{   // 未在开盘时间内
				double profit=0;  //浮盈
				totalcommission+=item.Commission;

				if(item.Position >0)		{
					total_margin+=item.UseMargin;

					DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

					int VolumeMultiple=0;
					if(depthmarket.VolumeMultiple>0){
						VolumeMultiple = depthmarket.VolumeMultiple;
					}
					else{

						MysqlInstrument mysqlinstrument;
						//检测对应的合约信息是否存在,不存在则请求下载
						if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
							this->ReqQrySingleInstrument(item.InstrumentID);
						}
						InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
						VolumeMultiple = instrumentdata.VolumeMultiple;
					}

					if (item.PosiDirection ==2 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)		{
							profit = (depthmarket.LastPrice - item.OpenPriceAverage) *
							(double)VolumeMultiple*	(double)item.Position;
//							printf("TrendTrader::FixAccountCheck  行情价格合规	   %.3f \n",depthmarket.LastPrice);
						}
						else	{
							profit=0;
							printf("TrendTrader::FixAccountCheck 行情价格合规不合规	   %.3f \n",depthmarket.LastPrice);
						}
					}
					else if (item.PosiDirection ==3 )	{
						if(depthmarket.LastPrice >1.0 && item.OpenPriceAverage >1.0)	{
							profit = ( item.OpenPriceAverage - depthmarket.LastPrice) *
											(double)VolumeMultiple*	(double)item.Position;
//							printf("TrendTrader::FixAccountCheck  行情价格合规	   %.3f \n",depthmarket.LastPrice);
						}
						else	{
							profit=0;
							printf("TrendTrader::FixAccountCheck 行情价格合规不合规	   %.3f \n",depthmarket.LastPrice);
						}
					}

				totalprofit =totalprofit +profit;
				printf("	已收盘-持仓合约: ----->  %s ---> 方向: %s 手数: %d  开仓均价: %.3f  现价: %.3f 占用保证金: %.3f[%.2f%%]  浮盈: %.3f\n",
				item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,depthmarket.LastPrice,item.UseMargin,item.UseMargin/staticequity*100,profit);

			}
			else		{
				DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
				int VolumeMultiple=0;
				if(depthmarket.VolumeMultiple>0){
					VolumeMultiple = depthmarket.VolumeMultiple;
				}
				else{

					MysqlInstrument mysqlinstrument;
					//检测对应的合约信息是否存在,不存在则请求下载
					if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
						this->ReqQrySingleInstrument(item.InstrumentID);
					}
					InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
					VolumeMultiple = instrumentdata.VolumeMultiple;
				}


				double closeprice= 	item.CloseAmount/((double)item.CloseVolume*VolumeMultiple);
				totalprofit =totalprofit +profit;
				totalcolseprofit+=item.CloseProfit;
				printf("	已收盘-平仓合约: >>>>>>  %s --->  方向: %s 手数: %d  开仓均价: %.3f  平仓均价: %.3f  平仓浮盈: %.3f\n",
					item.InstrumentID,dir.c_str(),item.Position,item.OpenPriceAverage,closeprice,item.CloseProfit);
			}

		}
//			printf("资金账户0: ----->  InstrumentID[%d]: %s \n",i,tradeinsidlist.pinstrumentid[i].c_str());

	}

	//动态权益
	dynamicequity = staticequity + holdtotalprofit + totalcolseprofit -totalcommission;
	//可用资金
	double ava = dynamicequity -assetdata.CurrMargin;
	this->SetAvalible(ava);

	//资金利用率  --> 仓位
	double holdpositionrate= total_margin/dynamicequity;
	this->SetHoldPositionRate(holdpositionrate);

	//总持仓量
	this->SetTotalHolding(totalholding);

	printf("	资金账户-->静态权益:%.2f 动态权益:%.2f 实际账户可用资金:%.2f  出金:%.2f  入金:%.2f\n",
			staticequity,dynamicequity, ava,assetdata.Withdraw,assetdata.Deposit);
	printf("	资金账户-->浮盈:%.2f  持仓盈亏:%.2f 平仓盈亏:%.2f 佣金:%.2f 昨日结算浮盈:%.2f  占用保证金:%.2f[%.2f%%] \n",
				totalprofit,holdtotalprofit,totalcolseprofit,totalcommission,ydtotalprofit,total_margin,holdpositionrate*100);

	if(totalprofit> assetdata.HighestProfit)	{
		assetdata.HighestProfit =totalprofit;
		assetdata.Profit = totalprofit;
		assetdata.HoldProfit = holdtotalprofit;
		assetdata.DynamicEquity = dynamicequity;
		this->GetMysqlAsset()->Update(assetdata);
	}
	else if(totalprofit< assetdata.HighestProfit/2.0)	{

	}
	vector<InvestPositionData>().swap(list);

	printf("TrendTrader::FixAccountCheck	   End!! \n");
}
// 监测持仓单
void TrendTrader::TotalHoldingCheck()
{
	MysqlPosDetail  mysqlposdetail(this->GetPointUserLoginField()->UserID);
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);

	int totalholding=0;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);

	for (auto &item:list)
			{
				totalholding+=item.Volume;
//				if(!mysqlposdetail.Exist_PositionbyInstrumentid()(item.InstrumentID,item.TradeID))
//				{
//					printf("TotalHoldingCheck不同步----->  pinstrumentid [%s]----->\n", item.InstrumentID);
//				}



			}

//	cout << "after swap capacity:" << list.capacity() << endl;

	if(this->GetTotalHolding()!=totalholding)
	{
		printf("TotalHoldingCheck-----> 不同  TotalHolding[%d]----->totalholding[%d] \n",this->GetTotalHolding(),totalholding);
	}
	else
	{
		printf("TotalHoldingCheck----->  相同 [%d]----->\n", this->GetTotalHolding());
	}

	vector<HoldData>().swap(list);
//	cout << "after swap capacity:" << list.capacity() << endl;
}

// 板块持仓品种数统计
void TrendTrader::SectionHoldingCheck()
{
	MysqlSecAssess mysqlsecassess(this->GetPointUserLoginField()->UserID);
	vector<SectionAssessData> sectionlists;
	mysqlsecassess.Find_AllLists(sectionlists);

	// 持仓的板块数清零
	for (auto &item:sectionlists)	{

		if(item.ProductHoldNumber>0){

//			printf("TrendTrader::SectionHoldingCheck-----> %s  板块  持仓品种数目 [%d]  ---> 清零  \n",
//										item.Section,item.ProductHoldNumber);
			mysqlsecassess.UpdateProductHoldNumber(0,item.Section);
		}

	}
	vector<SectionAssessData>().swap(sectionlists);


	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	for (auto &item:list)	{

		char productid[10];
		CodeUtil codeutil;
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

		MysqlProduct mysqlproduct;
		ProductData productdata;
		mysqlproduct.Find_DatabyProductID(productdata,productid);

		SectionAssessData data=mysqlsecassess.Find_DataBySection(productdata.Section);
		data.ProductHoldNumber++;
		mysqlsecassess.UpdateProductHoldNumber(data.ProductHoldNumber,productdata.Section);
//		printf("TrendTrader::SectionHoldingCheck-----> %s  板块 持有%s 合约---> 持仓品种数目自动加1 [%d]    \n",
//				productdata.Section,item.InstrumentID,data.ProductHoldNumber);
	}

	mysqlsecassess.Find_AllLists(sectionlists);
	for (auto &item:sectionlists)	{
		if(item.ProductHoldNumber >0 ){
			printf("TrendTrader::SectionHoldingCheck-----> %s  板块  存在持仓，持仓品种数目 [%d]    \n",
					item.Section,item.ProductHoldNumber);
		}
	}
	vector<SectionAssessData>().swap(sectionlists);

}

void TrendTrader::HoldPositionDetailCheck()
{
	vector<PositionDetailData> list;
	MysqlPosDetail detail(this->GetPointUserLoginField()->UserID);
	detail.Find_AllList(list);
	DateUtil dateutil;
	printf("HoldPositionDetailCheck----->pinstrumentid[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (auto &item:list)
	{

			DepthMarket depthmarket;
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

			std::string str_direction;
			int holding;

			if (item.Direction ==1 )
			{
				str_direction="空";
			}
			else
			{
				str_direction="多";
			}

	}
	vector<PositionDetailData>().swap(list);
printf("HoldPositionDetailCheck end  \n");
}



//posdetail 存在  hold 不存在   插入 hold 记录
void TrendTrader::CheckSqlHoldPositionbyPosDetail()
{
	char logbuf[1024];
	DateUtil dateutil;


	MysqlPosDetail mysqlposdetail(this->GetPointUserLoginField()->UserID);
	MysqlInstrument mysqlinstrument;
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
//	mysqlhold.DeleteAll();
	sprintf(logbuf,"TrendTrader::CheckSqlHoldPositionbyPosDetail   --> Begin  !!!\n");
	this->GetLogUtil()->WriteLog(logbuf);
	vector<PositionDetailData>  list;
	mysqlposdetail.Find_AllList(list);

	for (auto &item:list)	{

		printf("CheckSqlHoldPositionbyPosDetail----->  instrumentid:%s  opendate:%s   direction:%d   volume: %d  ----->\n", item.InstrumentID,
				item.OpenDate,item.Direction,item.Volume);
		InstrumentData instrumentdata;
		instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

		bool exist=mysqlhold.Exist_HoldPositionbyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
		HoldData posdetail_data=mysqlposdetail.CollectHoldPositionbyOpenDate(item.InstrumentID,item.OpenDate,item.Direction);

		if(!exist )			{
			printf("CheckSqlHoldPositionbyPosDetail   --> no exist !!!\n");
			if(posdetail_data.Volume>0)			{

				printf("CheckSqlHoldPositionbyPosDetail   --> >0 !!!\n");
				HoldData holdstop ;
				 if(strcmp(this->GetStrategyName().c_str(),"TrendTradeCycleN")==0)	{

					 holdstop =this->CollectDynamicStopPriceByOpenDate(item.InstrumentID,
							dateutil.ConvertDatetoSqlTime(item.OpenDate).c_str(),posdetail_data.OpenPriceAverage,item.Direction) ;
				 }
				 else if(strcmp(this->GetStrategyName().c_str(),"TrendTrade")==0)	{
					 holdstop =this->CollectDynamicStopPriceByOpenDate(item.InstrumentID,
						dateutil.ConvertDatetoSqlTime(item.OpenDate).c_str(),posdetail_data.OpenPriceAverage,item.Direction) ;
				 }
				 else if(strcmp(this->GetStrategyName().c_str(),"DayTrade")==0){
					 holdstop =this->DayTrade_CollectDayLine_ATRStopPriceByOpenDate(item.InstrumentID,
									dateutil.ConvertDatetoSqlTime(item.OpenDate).c_str(),posdetail_data.OpenPriceAverage,item.Direction) ;
				 }
				 else{

					 holdstop =this->DayTrade_CollectDayLine_ATRStopPriceByOpenDate(item.InstrumentID,
					dateutil.ConvertDatetoSqlTime(item.OpenDate).c_str(),posdetail_data.OpenPriceAverage,item.Direction) ;
				 }
				printf("CheckSqlHoldPositionbyPosDetail   --> 止损价 %f 止损差 %.3f \n",holdstop.StopPrice,holdstop.StopRange);

				//本地系统不存在开仓记录,需要根据开仓后的行情 计算最新的止损价.
				posdetail_data.Highest = holdstop.Highest;
				posdetail_data.Lowest = holdstop.Lowest;
				posdetail_data.StopPrice = holdstop.StopPrice;
				posdetail_data.StopRange = holdstop.StopRange;
				mysqlhold.Insert(posdetail_data);

				printf("CheckSqlHoldPositionbyPosDetail   --> 不存在持仓数据  插入%s  !!!\n",posdetail_data.InstrumentID);
				sprintf(logbuf,"CheckSqlHoldPositionbyPosDetail   --> 不存在持仓数据  插入%s  !!!\n",posdetail_data.InstrumentID);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else	{
				printf("CheckSqlHoldPositionbyPosDetail   --> 空 \n");
			}

		}
		else{

				if(posdetail_data.Volume >0)		{
					printf("CheckSqlHoldPositionbyPosDetail   --> 存在合约 持仓中  !!!\n");
					HoldData holddata=mysqlhold.Find_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);

					if(posdetail_data.Volume != holddata.Volume  ||  posdetail_data.OpenPriceAverage  != holddata.OpenPriceAverage)		{
						mysqlhold.Update(posdetail_data);
						printf("CheckSqlHoldPositionbyPosDetail   --> update !!!   data.volume :%d  holddata.Volume  %d\n",posdetail_data.Volume,holddata.Volume  );
						sprintf(logbuf,"CheckSqlHoldPositionbyPosDetail   --> update !!!   data.volume :%d  holddata.Volume  %d\n",posdetail_data.Volume,holddata.Volume  );

						this->GetLogUtil()->WriteLog(logbuf);
					}
				}
				else		{
					printf("CheckSqlHoldPositionbyPosDetail   --> 存在合约 已平仓  !!!\n");
					HoldData holddata=mysqlhold.Find_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
					if (posdetail_data.Volume ==0 && holddata.Volume >0)		{
						mysqlhold.Delete_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
						printf("CheckSqlHoldPositionbyPosDetail   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
								item.InstrumentID,item.OpenDate,item.Direction);
						sprintf(logbuf,"CheckSqlHoldPositionbyPosDetail   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
														item.InstrumentID,item.OpenDate,item.Direction);
						this->GetLogUtil()->WriteLog(logbuf);
					}
				}

		}
	}

	vector<PositionDetailData>().swap(list);

	sprintf(logbuf,"TrendTrader::CheckSqlHoldPositionbyPosDetail   --> End  !!!\n");
		this->GetLogUtil()->WriteLog(logbuf);
}

void TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail()
{

	char logbuf[1024];
	DateUtil dateutil;


	MysqlPosDetail mysqlposdetail(this->GetPointUserLoginField()->UserID);
//	MysqlInstrument mysqlinstrument;
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
//	mysqlhold.DeleteAll();
	sprintf(logbuf,"TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail   --> Begin  !!!");
	this->GetLogUtil()->WriteLog(logbuf);

	vector<PositionDetailData>  list;
	mysqlposdetail.Find_AllList(list);

	for (auto &item:list)	{

		printf("DayTrade_CheckSqlHoldPositionbyPosDetail----->  instrumentid:%s  opendate:%s   direction:%d   volume: %d  ----->\n", item.InstrumentID,
				item.OpenDate,item.Direction,item.Volume);
//		InstrumentData instrumentdata;
//		instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

		bool exist=mysqlhold.Exist_HoldPositionbyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
		HoldData posdetail_data=mysqlposdetail.CollectHoldPositionbyOpenDate(item.InstrumentID,item.OpenDate,item.Direction);



		if(!exist )		{
				sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   %s--> hold持仓表不存在该合约 持仓量:%d !!!",
						item.InstrumentID,posdetail_data.Volume);
				this->GetLogUtil()->WriteLog(logbuf);
				if(posdetail_data.Volume>0)		{

//					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> >持仓量 %d !!!",posdetail_data.Volume);
//					this->GetLogUtil()->WriteLog(logbuf);
//					HoldData holdstop = DayTrade_CollectStopPriceByOpenDate(item.InstrumentID,item.OpenDate,
//							item.TradeID,posdetail_data.OpenPriceAverage,item.Direction);

					HoldData holdstop =DayTrade_CollectDayLine_ATRStopPriceByOpenDate(item.InstrumentID,item.OpenDate,
							posdetail_data.OpenPriceAverage,item.Direction);
					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail  %s  重新计算止损价 止盈价>>>>>>> "
							"direction: %d --> 开仓价:%.3f  StopRange: %.3f WinPrice: %.3f  StopPrice: %.3f",item.InstrumentID,
							item.Direction,posdetail_data.OpenPriceAverage,holdstop.StopRange,holdstop.WinPrice,holdstop.StopPrice);
					this->GetLogUtil()->WriteLog(logbuf);


					//本地系统不存在开仓记录,需要根据开仓后的行情 计算最新的止损价.
					posdetail_data.WinPrice = holdstop.WinPrice;
					posdetail_data.StopRange = holdstop.StopRange;
					posdetail_data.StopPrice = holdstop.StopPrice;
					posdetail_data.Highest =posdetail_data.OpenPriceAverage;
					posdetail_data.Lowest =posdetail_data.OpenPriceAverage;
					mysqlhold.Insert(posdetail_data);

//					printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 不存在持仓数据  插入%s  !!!\n",posdetail_data.InstrumentID);
					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> 不存在持仓数据  插入%s  !!!",posdetail_data.InstrumentID);
					this->GetLogUtil()->WriteLog(logbuf);
				}
				else			{
					printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 空 \n");

//					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   %s--> >持仓量空>>>>>>>> %d !!!",
//							item.InstrumentID,posdetail_data.Volume);
//					this->GetLogUtil()->WriteLog(logbuf);

				}

			}
		else	{

				if(posdetail_data.Volume >0)			{
					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> 存在合约 %s 持仓中  !!!",item.InstrumentID);
					this->GetLogUtil()->WriteLog(logbuf);

					HoldData holddata=mysqlhold.Find_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);

					if(posdetail_data.Volume != holddata.Volume  ||  posdetail_data.OpenPriceAverage  != holddata.OpenPriceAverage)		{
						mysqlhold.Update(posdetail_data);
						printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> update !!!   data.volume :%d  holddata.Volume  %d\n",posdetail_data.Volume,holddata.Volume  );
						sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> update !!!   data.volume :%d  holddata.Volume  %d",posdetail_data.Volume,holddata.Volume  );

						this->GetLogUtil()->WriteLog(logbuf);
					}
				}
				else		{
					printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 存在合约 已平仓  !!!\n");
					HoldData holddata=mysqlhold.Find_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
					if (posdetail_data.Volume ==0 && holddata.Volume >0)			{
						mysqlhold.Delete_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
						printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
								item.InstrumentID,item.OpenDate,item.Direction);
						sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!",
														item.InstrumentID,item.OpenDate,item.Direction);
						this->GetLogUtil()->WriteLog(logbuf);
					}
				}

			}
		}

	vector<PositionDetailData>().swap(list);

	vector<HoldData>  holdlists;
	mysqlhold.Find_AllList(holdlists);
	if(holdlists.size()>0)	{
		printf("TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail 当前持仓品种明细---->当日持仓批次[%ld] ----> %s\n",
				holdlists.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		sprintf(logbuf,"TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail 当前持仓品种明细---->当日持仓批次[%ld] ----> %s",
						holdlists.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		this->GetLogUtil()->WriteLog(logbuf);

		for (auto &item:holdlists)		{
			printf("持仓合约[%s]-->   开仓价: %.3f 方向:%d   stoprange:%.3f 止损价: %.3f  止盈价: %.3f  \n",
					item.InstrumentID,	item.OpenPriceAverage,item.Direction,item.StopRange,item.StopPrice,item.WinPrice);
			sprintf(logbuf,"	持仓合约[%s]-->   开仓价: %.3f 方向:%d   stoprange:%.3f 止损价: %.3f  止盈价: %.3f  ",
							item.InstrumentID,	item.OpenPriceAverage,item.Direction,item.StopRange,item.StopPrice,item.WinPrice);
			this->GetLogUtil()->WriteLog(logbuf);
		}

	}



	vector<HoldData>().swap(holdlists);

	sprintf(logbuf,"TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail   --> End  !!!\n");
	this->GetLogUtil()->WriteLog(logbuf);

}
HoldData TrendTrader::CollectHoldDatabyInvest(InvestPositionData *item)
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

	if(item->PosiDirection==2){  //long
		holddata.Direction=0;
		holddata.WinPrice = item->OpenPriceAverage+atr20*this->GetWinRate();
		holddata.StopPrice = item->OpenPriceAverage-atr20*this->GetStopRate();
	}
	else if(item->PosiDirection==3){ //short
		holddata.Direction=1;
		holddata.WinPrice = item->OpenPriceAverage-atr20*this->GetWinRate();
		holddata.StopPrice = item->OpenPriceAverage+atr20*this->GetStopRate();
	}
	return holddata;
}
bool TrendTrader::UpdateHoldDatabyInvest(InvestPositionData *item,HoldData &holddata)
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
		sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 stoprange %.2f ",holddata.InstrumentID,holddata.StopRange);
		this->GetLogUtil()->WriteLog(logbuf);
		flag=true;
	}

	if(holddata.StopPrice==0.0){
		if(item->PosiDirection==2){  //long
			holddata.Direction=0;
			holddata.StopPrice = item->OpenPriceAverage-holddata.StopRange*this->GetStopRate();
			sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 StopPrice %.2f ",holddata.InstrumentID,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
		else if(item->PosiDirection==3){ //short
			holddata.Direction=1;
			holddata.StopPrice = item->OpenPriceAverage+holddata.StopRange*this->GetStopRate();
			sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 StopPrice %.2f ",holddata.InstrumentID,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);

			flag=true;
		}
	}

	if(holddata.WinPrice==0.0){
		if(item->PosiDirection==2){  //long
			holddata.Direction=0;
			holddata.WinPrice = item->OpenPriceAverage+holddata.StopRange*this->GetWinRate();
			sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 WinPrice %.2f ",holddata.InstrumentID,holddata.WinPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
		else if(item->PosiDirection==3){ //short
			holddata.Direction=1;
			holddata.WinPrice = item->OpenPriceAverage-holddata.StopRange*this->GetWinRate();
			sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 WinPrice %.2f ",holddata.InstrumentID,holddata.WinPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
	}

	if(item->Position!=holddata.Volume){
		holddata.Volume=item->Position;
		sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 Volume %d ",holddata.InstrumentID,holddata.Volume);
		this->GetLogUtil()->WriteLog(logbuf);
		flag=true;
	}
	if(item->PosiDirection==2){  //long
		holddata.Direction=0;
		if(depthmarket.LastPrice>holddata.Highest){
			holddata.Highest=depthmarket.LastPrice;
			holddata.StopPrice=depthmarket.LastPrice-holddata.StopRange;
			sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 Highest %.2f StopPrice %.2f",holddata.InstrumentID,
					holddata.Highest,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}

	}
	else if(item->PosiDirection==3){ //short
		holddata.Direction=1;
		if(depthmarket.LastPrice<holddata.Lowest){
			holddata.Lowest=depthmarket.LastPrice;
			holddata.StopPrice=depthmarket.LastPrice+holddata.StopRange;
			sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 Lowest %.2f StopPrice %.2f",holddata.InstrumentID,
					holddata.Lowest,holddata.StopPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			flag=true;
		}
	}

	if(strlen(holddata.OpenDate)<8){
		strcpy(holddata.OpenDate,this->GetDataWrapper()->CollectOpenDatebyPosDetail(holddata.InstrumentID).c_str());
		sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->更新 opendate %s",holddata.InstrumentID,holddata.OpenDate);
		this->GetLogUtil()->WriteLog(logbuf);
		flag=true;
	}

	if(flag){
		sprintf(logbuf,"TrendTrader::UpdateHoldDatabyInvest  %s-->holdmap 数据更新",holddata.InstrumentID);
					this->GetLogUtil()->WriteLog(logbuf);
		this->GetDataWrapper()->UpdateHoldData(holddata);
	}
return flag;
}
void TrendTrader::SyncHoldInvestMap()
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
				sprintf(logbuf,"SyncHoldInvestMap   %s->新插入 holddata",item.InstrumentID);
				this->GetLogUtil()->WriteLog(logbuf);
				HoldData holddata=this->CollectHoldDatabyInvest(&item);
				this->GetDataWrapper()->InsertHoldData(holddata);
			}
			else{

				if(item.Position==0 ){
	//				sprintf(logbuf,"SyncHoldInvestMap   %s->删除 holddata\n",item.InstrumentID);
	//				this->GetLogUtil()->WriteLog(logbuf);
					this->GetDataWrapper()->DeleteHoldData(item.InstrumentID,item.PosiDirection);
				}
				else{
					this->UpdateHoldDatabyInvest(&item,f_data);
	//				sprintf(logbuf,"SyncHoldInvestMap   %s->更新 holddata\n",item.InstrumentID);
	//				this->GetLogUtil()->WriteLog(logbuf);
				}
			}
		}

		vector<InvestPositionData>().swap(list);


		vector<HoldData>  holdlist;
		this->GetDataWrapper()->GetHoldList(holdlist);

		printf("        持仓合约品种:%ld   \n",holdlist.size());
		for (auto &item:holdlist)		{
			printf("hold    合约:%s   开仓价:%.2f  方向:%d 手数: %d   止损价差:%.2f  stopprice:%.2f   winprice:%.2f\n",
					item.InstrumentID,item.OpenPriceAverage,item.Direction,item.Volume,item.StopRange,item.StopPrice,item.WinPrice);

			if(this->GetDataWrapper()->GetPositionbyInstrumentid(item.InstrumentID,item.Direction)==0){
				printf("SyncHoldInvestMap   %s->删除 holddata\n",item.InstrumentID);
				this->GetDataWrapper()->DeleteHoldData(item.InstrumentID,item.Direction);
			}
		}
		vector<HoldData>().swap(holdlist);

}
void TrendTrader::UpdateHoldMapbyPosDetail()
{

	char logbuf[1024];
	DateUtil dateutil;


	MysqlPosDetail mysqlposdetail(this->GetPointUserLoginField()->UserID);
	this->GetDataWrapper()->EmptyHoldMap();

	sprintf(logbuf,"TrendTrader::UpdateHoldMapbyPosDetail   --> Begin  !!!");
	this->GetLogUtil()->WriteLog(logbuf);

	vector<PositionDetailData>  list;
	mysqlposdetail.Find_AllList(list);

	for (auto &item:list)	{

		printf("UpdateHoldMapbyPosDetail----->  instrumentid:%s  opendate:%s   direction:%d   volume: %d  ----->\n", item.InstrumentID,
				item.OpenDate,item.Direction,item.Volume);

//		bool exist=mysqlhold.Exist_HoldPositionbyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
//		HoldData posdetail_data=mysqlposdetail.CollectHoldPositionbyOpenDate(item.InstrumentID,item.OpenDate,item.Direction);
//
//		if(!exist )		{
//				sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   %s--> hold持仓表不存在该合约 持仓量:%d !!!",
//						item.InstrumentID,posdetail_data.Volume);
//				this->GetLogUtil()->WriteLog(logbuf);
//				if(posdetail_data.Volume>0)		{
//
////					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> >持仓量 %d !!!",posdetail_data.Volume);
////					this->GetLogUtil()->WriteLog(logbuf);
////					HoldData holdstop = DayTrade_CollectStopPriceByOpenDate(item.InstrumentID,item.OpenDate,
////							item.TradeID,posdetail_data.OpenPriceAverage,item.Direction);
//
//					HoldData holdstop =DayTrade_CollectDayLine_ATRStopPriceByOpenDate(item.InstrumentID,item.OpenDate,
//							posdetail_data.OpenPriceAverage,item.Direction);
//					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail  %s  重新计算止损价 止盈价>>>>>>> "
//							"direction: %d --> 开仓价:%.3f  StopRange: %.3f WinPrice: %.3f  StopPrice: %.3f",item.InstrumentID,
//							item.Direction,posdetail_data.OpenPriceAverage,holdstop.StopRange,holdstop.WinPrice,holdstop.StopPrice);
//					this->GetLogUtil()->WriteLog(logbuf);
//
//
//					//本地系统不存在开仓记录,需要根据开仓后的行情 计算最新的止损价.
//					posdetail_data.WinPrice = holdstop.WinPrice;
//					posdetail_data.StopRange = holdstop.StopRange;
//					posdetail_data.StopPrice = holdstop.StopPrice;
//					posdetail_data.Highest =posdetail_data.OpenPriceAverage;
//					posdetail_data.Lowest =posdetail_data.OpenPriceAverage;
//					mysqlhold.Insert(posdetail_data);
//
////					printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 不存在持仓数据  插入%s  !!!\n",posdetail_data.InstrumentID);
//					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> 不存在持仓数据  插入%s  !!!",posdetail_data.InstrumentID);
//					this->GetLogUtil()->WriteLog(logbuf);
//				}
//				else			{
//					printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 空 \n");
//
////					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   %s--> >持仓量空>>>>>>>> %d !!!",
////							item.InstrumentID,posdetail_data.Volume);
////					this->GetLogUtil()->WriteLog(logbuf);
//
//				}
//
//			}
//		else	{
//
//				if(posdetail_data.Volume >0)			{
//					sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> 存在合约 %s 持仓中  !!!",item.InstrumentID);
//					this->GetLogUtil()->WriteLog(logbuf);
//
//					HoldData holddata=mysqlhold.Find_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
//
//					if(posdetail_data.Volume != holddata.Volume  ||  posdetail_data.OpenPriceAverage  != holddata.OpenPriceAverage)		{
//						mysqlhold.Update(posdetail_data);
//						printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> update !!!   data.volume :%d  holddata.Volume  %d\n",posdetail_data.Volume,holddata.Volume  );
//						sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> update !!!   data.volume :%d  holddata.Volume  %d",posdetail_data.Volume,holddata.Volume  );
//
//						this->GetLogUtil()->WriteLog(logbuf);
//					}
//				}
//				else		{
//					printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 存在合约 已平仓  !!!\n");
//					HoldData holddata=mysqlhold.Find_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
//					if (posdetail_data.Volume ==0 && holddata.Volume >0)			{
//						mysqlhold.Delete_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
//						printf("DayTrade_CheckSqlHoldPositionbyPosDetail   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
//								item.InstrumentID,item.OpenDate,item.Direction);
//						sprintf(logbuf,"DayTrade_CheckSqlHoldPositionbyPosDetail   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!",
//														item.InstrumentID,item.OpenDate,item.Direction);
//						this->GetLogUtil()->WriteLog(logbuf);
//					}
//				}
//
//			}
//		}
//
//	vector<PositionDetailData>().swap(list);
//
//	vector<HoldData>  holdlists;
//	mysqlhold.Find_AllList(holdlists);
//	if(holdlists.size()>0)	{
//		printf("TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail 当前持仓品种明细---->当日持仓批次[%ld] ----> %s\n",
//				holdlists.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//		sprintf(logbuf,"TrendTrader::DayTrade_CheckSqlHoldPositionbyPosDetail 当前持仓品种明细---->当日持仓批次[%ld] ----> %s",
//						holdlists.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//		this->GetLogUtil()->WriteLog(logbuf);
//
//		for (auto &item:holdlists)		{
//			printf("持仓合约[%s]-->   开仓价: %.3f 方向:%d   stoprange:%.3f 止损价: %.3f  止盈价: %.3f  \n",
//					item.InstrumentID,	item.OpenPriceAverage,item.Direction,item.StopRange,item.StopPrice,item.WinPrice);
//			sprintf(logbuf,"	持仓合约[%s]-->   开仓价: %.3f 方向:%d   stoprange:%.3f 止损价: %.3f  止盈价: %.3f  ",
//							item.InstrumentID,	item.OpenPriceAverage,item.Direction,item.StopRange,item.StopPrice,item.WinPrice);
//			this->GetLogUtil()->WriteLog(logbuf);
//		}
//
	}

	sprintf(logbuf,"TrendTrader::UpdateHoldMapbyPosDetail   --> End  !!!\n");
	this->GetLogUtil()->WriteLog(logbuf);

}
//posdetail 不存在的合约   hold存在   删除 hold记录
void TrendTrader::CheckPosDetailbySqlHoldPosition()
{

	char logbuf[1024];
	DateUtil dateutil;


	MysqlPosDetail  mysqlposdetail(this->GetPointUserLoginField()->UserID);
	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);

	int totalholding=0;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);

	for (auto &item:list)		{
			totalholding+=item.Volume;
			if(!mysqlposdetail.Exist_HoldbyInstrumentid(item))		{
				printf("CheckPosDetailbySqlHoldPosition 交易所不存在该合约! ----->  pinstrumentid [%s]----->\n", item.InstrumentID);

				mysqlhold.Delete_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
				printf("CheckPosDetailbySqlHoldPosition   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
						item.InstrumentID,item.OpenDate,item.Direction);

				sprintf(logbuf,"CheckPosDetailbySqlHoldPosition   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
												item.InstrumentID,item.OpenDate,item.Direction);
				this->GetLogUtil()->WriteLog(logbuf);
			}

		}

//	cout << "after swap capacity:" << list.capacity() << endl;

	if(this->GetTotalHolding()!=totalholding)	{
		printf("CheckPosDetailbySqlHoldPosition-----> 不同  TotalHolding[%d]----->totalholding[%d] \n",this->GetTotalHolding(),totalholding);
	}
	else	{
		printf("CheckPosDetailbySqlHoldPosition----->  相同 [%d]----->\n", this->GetTotalHolding());
	}


	/// Reset 内存缓存 持仓表
	this->GetDataWrapper()->ResetHoldMap(list);

	vector<HoldData>().swap(list);
}
void TrendTrader::CheckInvestbySqlHoldPosition()
{

	char logbuf[1024];
	DateUtil dateutil;



	MysqlHold mysqlhold(this->GetPointUserLoginField()->UserID);
	MysqlInvestPosition mysqlinvest(this->GetPointUserLoginField()->UserID);

	int totalholding=0;
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);

	for (auto &item:list)		{
			totalholding+=item.Volume;
			if(mysqlinvest.GetPositionbyIns(item.InstrumentID)==0)			{
				printf("CheckInvestbySqlHoldPosition 交易所不存在该合约! ----->  pinstrumentid [%s]----->\n", item.InstrumentID);

				mysqlhold.Delete_DatabyInstrumentid(item.InstrumentID,item.OpenDate,item.Direction);
				printf("CheckInvestbySqlHoldPosition   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
						item.InstrumentID,item.OpenDate,item.Direction);

				sprintf(logbuf,"CheckInvestbySqlHoldPosition   --> 删除已经平仓的合约   %s   开仓时间: %s  开仓方向: %d!!!\n",
												item.InstrumentID,item.OpenDate,item.Direction);
				this->GetLogUtil()->WriteLog(logbuf);
			}
		}

//	cout << "after swap capacity:" << list.capacity() << endl;

	if(this->GetTotalHolding()!=totalholding)	{
		printf("CheckInvestbySqlHoldPosition-----> 不同  TotalHolding[%d]----->totalholding[%d] \n",this->GetTotalHolding(),totalholding);
	}
	else
	{
		printf("CheckInvestbySqlHoldPosition----->  相同 [%d]----->\n", this->GetTotalHolding());
	}

	vector<HoldData>().swap(list);

}

HoldData  TrendTrader::CollectStopPriceByOpenDate(const char* pinstrumentid,const char* opendate_sqltime,double openprice,int direction)
{
	HoldData ret;
//	printf("CollectStopPriceByOpenDate   --> begin !!!\n");
	ret.StopPrice=openprice;
	ret.Highest=openprice;
	ret.Lowest=openprice;
	vector<HxDayLineData> list;

	MysqlDayLine mysqldayline;
	mysqldayline.Find_ListsbyOpenDate(list,pinstrumentid,opendate_sqltime);

	std::string str_tradingsqltime = this->GetTradingDaySqlTime();
	double hismax;
	double hismin;

	int i=0;
	for (auto &item:list)
	{
		if (i==0)
		{
			hismax=item.High;
			hismin=item.Low;

		}


		if(item.High>hismax)
		{
			hismax=item.High;
		}
		if(item.Low <hismin)
		{
			hismin=item.Low;
		}
		i++;
	}

	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

//	double hisatr=this->GetATRbyOpenDate(pinstrumentid,opendate_sqltime);

	if(strcmp(str_tradingsqltime.c_str(),opendate_sqltime)==0)
	{
		printf("CollectStopPriceByOpenDate   --> 当天开仓  %s !!!\n",opendate_sqltime);

		double atr=this->GetATRbyTradingDay(pinstrumentid);
		if (direction==0)
		{
			double highest;
			if(depthmarket.HighestPrice > 1.0)
			{
				highest =max(depthmarket.HighestPrice,openprice);
			}
			else
			{
				highest=openprice;
			}



			if(openprice>highest)
			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
			}
			else
			{
				ret.StopPrice=highest-atr*this->GetStopRate();
			}


		}
		else if (direction==1)
		{
			double lowest;
			if(depthmarket.LowestPrice > 1.0)
			{
				lowest = min(depthmarket.LowestPrice,openprice);

			}
			else
			{
				lowest=openprice;

			}

			if(openprice<lowest)
			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
			}
			else
			{
				ret.StopPrice=lowest+atr*this->GetStopRate();
			}

		}

	}
	else
	{
		printf("CollectStopPriceByOpenDate   --> 非当天开仓  %s !!!\n",opendate_sqltime);
		double atr=this->GetATRbyTradingDay(pinstrumentid);
		if (direction==0)
		{
			double highest;
			if(depthmarket.HighestPrice > 1.0)
			{
				highest =max(depthmarket.HighestPrice,hismax);
			}
			else
			{
				highest=hismax;
			}


			//程序有潜在问题,最高价 可能发生在开仓前
			if(openprice>highest)
			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
				ret.Highest=openprice;
			}
			else
			{
				ret.StopPrice=highest-atr*this->GetStopRate();
				ret.Highest=highest;
			}


		}
		else if (direction==1)
		{
//			printf("CollectStopPriceByOpenDate   min --> 空lowest %f  hismin %f !!!\n",depthmarket.LowestPrice,hismin);
			double lowest;
			if(depthmarket.LowestPrice > 1.0)
			{
				lowest = min(depthmarket.LowestPrice,hismin);
//				printf("CollectStopPriceByOpenDate   1111--> 空lowest %f  open %f !!!\n",lowest,openprice);
			}
			else
			{
				lowest=hismin;
//				printf("CollectStopPriceByOpenDate   2222--> 空lowest %f  open %f !!!\n",lowest,openprice);
			}

//			printf("CollectStopPriceByOpenDate   --> 空lowest %f  open %f !!!\n",lowest,openprice);
			if(openprice<lowest)
			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
				ret.Lowest = openprice;
			}
			else
			{
				ret.StopPrice=lowest+atr*this->GetStopRate();
				ret.Lowest=lowest;
			}

		}

	}



	vector<HxDayLineData> ().swap(list);
//	printf("CollectStopPriceByOpenDate   --> end %f !!!\n",ret.StopPrice);
	return ret;
}


HoldData  TrendTrader::CollectDynamicStopPriceByOpenDate(const char* pinstrumentid,const char* opendate_sqltime,double openprice,int direction)
{
	HoldData ret;
//	printf("CollectStopPriceByOpenDate   --> begin !!!\n");
	ret.StopPrice=openprice;
	ret.Highest=openprice;
	ret.Lowest=openprice;
	vector<HxDayLineData> list;

	MysqlDayLine mysqldayline;

	ATRData atr20_data = mysqldayline.Collect_PreHisATR_NData(pinstrumentid,opendate_sqltime,20);
	ret.StopRange = atr20_data.ATR;

	mysqldayline.Find_ListsbyOpenDate(list,pinstrumentid,opendate_sqltime);

	std::string str_tradingsqltime = this->GetTradingDaySqlTime();
	double hismax;
	double hismin;

	int i=0;
	for (auto &item:list){
		if (i==0)		{
			hismax=item.High;
			hismin=item.Low;

		}


		if(item.High>hismax)		{
			hismax=item.High;
		}
		if(item.Low <hismin)		{
			hismin=item.Low;
		}
		i++;
	}

	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

//	double hisatr=this->GetATRbyOpenDate(pinstrumentid,opendate_sqltime);

	if(strcmp(str_tradingsqltime.c_str(),opendate_sqltime)==0)	{
		printf("CollectStopPriceByOpenDate   --> 当天开仓  %s !!!\n",opendate_sqltime);

		double atr=this->GetATRbyTradingDay(pinstrumentid);
		if (direction==0)		{
			double highest;
			if(depthmarket.HighestPrice > 1.0)			{
				highest =max(depthmarket.HighestPrice,openprice);
			}
			else			{
				highest=openprice;
			}



			if(openprice>highest)			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
			}
			else			{
				ret.StopPrice=highest-atr*this->GetStopRate();
			}


		}
		else if (direction==1)		{
			double lowest;
			if(depthmarket.LowestPrice > 1.0)			{
				lowest = min(depthmarket.LowestPrice,openprice);

			}
			else			{
				lowest=openprice;

			}

			if(openprice<lowest)			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
			}
			else			{
				ret.StopPrice=lowest+atr*this->GetStopRate();
			}

		}

	}
	else
	{
		printf("CollectStopPriceByOpenDate   --> 非当天开仓  %s !!!\n",opendate_sqltime);
		double atr=atr20_data.ATR;
		if (direction==0)		{
			double highest;
			if(depthmarket.HighestPrice > 1.0)			{
				highest =max(depthmarket.HighestPrice,hismax);
			}
			else			{
				highest=hismax;
			}


			//程序有潜在问题,最高价 可能发生在开仓前
			if(openprice>highest)			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
				ret.Highest=openprice;
			}
			else			{
				ret.StopPrice=highest-atr*this->GetStopRate();
				ret.Highest=highest;
			}


		}
		else if (direction==1)	{
//			printf("CollectStopPriceByOpenDate   min --> 空lowest %f  hismin %f !!!\n",depthmarket.LowestPrice,hismin);
			double lowest;
			if(depthmarket.LowestPrice > 1.0)			{
				lowest = min(depthmarket.LowestPrice,hismin);
//				printf("CollectStopPriceByOpenDate   1111--> 空lowest %f  open %f !!!\n",lowest,openprice);
			}
			else			{
				lowest=hismin;
//				printf("CollectStopPriceByOpenDate   2222--> 空lowest %f  open %f !!!\n",lowest,openprice);
			}

//			printf("CollectStopPriceByOpenDate   --> 空lowest %f  open %f !!!\n",lowest,openprice);
			if(openprice<lowest)			{
				ret.StopPrice=openprice-atr*this->GetStopRate();
				ret.Lowest = openprice;
			}
			else			{
				ret.StopPrice=lowest+atr*this->GetStopRate();
				ret.Lowest=lowest;
			}

		}

	}



	vector<HxDayLineData> ().swap(list);
//	printf("CollectStopPriceByOpenDate   --> end %f !!!\n",ret.StopPrice);
	return ret;
}
HoldData  TrendTrader::DayTrade_CollectStopPriceByOpenDate(const char* pinstrumentid,
		const char* opendate,const char* tradeid,double openprice,int direction)
{
	HoldData ret;
//	printf("DayTrade_CollectStopPriceByOpenDate   --> begin !!!\n");


	std::string str_tradingsqltime = this->GetTradingDaySqlTime();

	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

//	double hisatr=this->GetATRbyOpenDate(pinstrumentid,opendate_sqltime);

	if(strcmp(str_tradingsqltime.c_str(),opendate)==0)	{
		printf("CollectStopPriceByOpenDate   --> 当天开仓  %s !!!\n",opendate);
		Config config(this->GetStrategyName().c_str());
		MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),this->GetPointUserLoginField()->UserID);
		OrderData opendata;

		opendata=mysqlopenorder.Find_OrderbyInsTradeID(pinstrumentid,tradeid,opendate);

		ret.StopPrice=opendata.StopPrice;
		ret.StopRange = opendata.StopRange;
		ret.WinPrice = opendata.WinPrice;

	}
	else	{
		printf("CollectStopPriceByOpenDate   --> 非当天开仓  %s !!!\n",opendate);

	}


	return ret;
}

HoldData  TrendTrader::DayTrade_CollectATRStopPriceByOpenDate(const char* pinstrumentid,const char* opendate,double openprice,int direction)
{
	char logbuf[1024];
		DateUtil dateutil;


	HoldData ret;
//	printf("DayTrade_CollectATRStopPriceByOpenDate   --> begin !!!\n");
	MysqlMinData mysqlmindata;

	std::string str_tradingsqltime = this->GetTradingDaySqlTime();
	std::string str_tradingtime = this->GetTradingDay();
	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
	mysqlmindata.Set_Database("minute");
	mysqlmindata.Set_Table(pinstrumentid);
	ATRData atrdata =mysqlmindata.Get_ATRDataByPreLists(pinstrumentid,20);

	if(strcmp(str_tradingtime.c_str(),opendate)==0)	{
		printf("DayTrade_CollectATRStopPriceByOpenDate   --> 当天开仓  %s !!!\n",opendate);
		sprintf(logbuf,"DayTrade_CollectATRStopPriceByOpenDate   --> 当天开仓  %s !!!   atr  %.3f \n",opendate,atrdata.ATR);
		this->GetLogUtil()->WriteLog(logbuf);

		if(direction ==0)		{
			ret.StopRange = atrdata.ATR *2;
			ret.StopPrice=openprice - ret.StopRange ;
			ret.WinPrice = openprice + ret.StopRange ;
		}

		else if (direction ==1)		{
			ret.StopRange =  atrdata.ATR *2;
			ret.StopPrice=openprice + ret.StopRange ;
			ret.WinPrice = openprice -ret.StopRange ;
		}


	}
	else	{
		sprintf(logbuf,"CollectStopPriceByOpenDate   --> 非当天开仓  %s !!!\n",opendate);
		this->GetLogUtil()->WriteLog(logbuf);

	}


	return ret;
}

HoldData  TrendTrader::DayTrade_CollectDayLine_ATRStopPriceByOpenDate(const char* pinstrumentid,const char* opendate,double openprice,int direction)
{
	char logbuf[1024];
	DateUtil dateutil;


	double stoprate=this->GetStopRate();
	double winrate=this->GetWinRate();

	HoldData ret;
//	printf("DayTrade_CollectATRStopPriceByOpenDate   --> begin !!!\n");
//	MysqlMinData mysqlmindata;
	MysqlDayLine  mysqldayline;
	std::string str_tradingsqltime = this->GetTradingDaySqlTime();
	std::string str_tradingtime = this->GetTradingDay();
	DepthMarket depthmarket;
	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
	ATRData dayline_atrdata = mysqldayline.Collect_HisATR_NData(pinstrumentid,str_tradingtime.c_str(),20);

//	mysqlmindata.Set_Database("minute");
//	mysqlmindata.Set_Table(pinstrumentid);
//	ATRData atrdata =mysqlmindata.Get_ATRDataByPreLists(pinstrumentid,20);


	if(strcmp(str_tradingtime.c_str(),opendate)==0)	{
		printf("DayTrade_CollectDayLine_ATRStopPriceByOpenDate   --> 当天开仓  %s !!!\n",opendate);
		sprintf(logbuf,"DayTrade_CollectDayLine_ATRStopPriceByOpenDate   --> %s 当天开仓  %s !!!   atr  %.3f \n",pinstrumentid,
				opendate,dayline_atrdata.ATR);
		this->GetLogUtil()->WriteLog(logbuf);

		if(direction ==0)		{
			ret.StopRange = dayline_atrdata.ATR ;
			ret.StopPrice=openprice - ret.StopRange*stoprate ;
			ret.WinPrice = openprice + ret.StopRange *winrate;
		}

		else if (direction ==1)		{
			ret.StopRange =  dayline_atrdata.ATR;
			ret.StopPrice=openprice + ret.StopRange*stoprate ;
			ret.WinPrice = openprice -ret.StopRange*winrate ;
		}


	}
	else	{

		if(direction ==0)		{
			ret.StopRange = openprice*0.02 ;
			ret.StopPrice=openprice - ret.StopRange*stoprate ;
			ret.WinPrice = openprice + ret.StopRange *winrate;
		}

		else if (direction ==1)		{
			ret.StopRange =  openprice*0.02;
			ret.StopPrice=openprice + ret.StopRange*stoprate ;
			ret.WinPrice = openprice -ret.StopRange*winrate ;
		}

		sprintf(logbuf,"DayTrade_CollectDayLine_ATRStopPriceByOpenDate   --> %s 非当天开仓  %s  stoprange 取开仓价的百分之2计算 --->%.2f!!!\n",
				pinstrumentid,opendate,ret.StopRange);
		this->GetLogUtil()->WriteLog(logbuf);

	}


	return ret;
}
