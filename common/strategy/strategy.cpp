#include "strategy.hpp"

Strategy::Strategy() {
	printf("Strategy  无参数构造函数-->初始  \n");
}


Strategy::Strategy(const char* strategyname) {

	printf("Strategy  带参数构造函数-->初始开始  \n");

	this->SetStrategyName(strategyname);
	Config config(this->GetStrategyName().c_str());
	std::string logname="sta";
	logname.append("_");
	logname.append(config.GetUserID().c_str());


	std::string logfilename="sta";
	std::string path = "/var/log/autotrader/";
	logfilename.append("_");
	logfilename.append(config.GetUserID().c_str());
	logfilename.append(".log");
	this->SetLogName(logname.c_str());
	this->SetLogPathFileName(path.c_str(), logfilename.c_str());

	LogUtil * plogutil = new LogUtil(this->GetLogName(),this->GetLogPathFileName());
	this->SetLogUtil(plogutil);

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

	printf("Strategy  带参数构造函数-->初始结束   Open signalcount:%d \n",this->GetOpenSignalCount());
}

void Strategy::Check_WholeMarketCapital()
{
	DateUtil dateutil;

	//MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;
	//MysqlDepthMarket mysqldepthmarket;
	//MysqlInstrument mysqlinstrument;
//
//	this->GetMysqlMaincontract()->Find_AllList(list);
//	printf("沉淀资金监测----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//
//	double totalcapital=0;
//	double totalsedimentaryio=0;
//	for (auto &item:list)	{
//
//		DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
//		DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);
////		InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);
//
//		double sedimentary;
//		sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);
////		printf("	持仓合约到期月份监测 -->  %s 沉淀资金: %.2f \n",item.InstrumentID,sedimentary);
//
//		totalcapital+=sedimentary;
//		totalsedimentaryio+=depthmarket.Sedimentary -depthmarket.LastSedimentary;
//	}
//	vector<MainContractData>().swap(list);
//	printf("	市场整体沉淀资金 [%.2f]--------------------> 流入流出资金[%.2f]   \n",totalcapital,totalsedimentaryio);
}


void Strategy::Check_UpDownRate()
{
	DateUtil dateutil;

	//MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  list;
	//MysqlDepthMarket mysqldepthmarket;
	//MysqlInstrument mysqlinstrument;

//	this->GetMysqlMaincontract()->Find_AllListorderbySed(list);
//	printf("涨跌幅监测----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//
//	double totalcapital=0;
//	double totalsedimentaryio=0;
//	int i=0;
//	double max_updown;
//	std::string max_ins;
//	double min_updown;
//	std::string min_ins;
//	for (auto &item:list)	{
//
//		char pinstrumentid[40];
//		strcpy(pinstrumentid,item.InstrumentID_trade);
//
//		DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
//
////		DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);
//
//		InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);
//
//		if (i==0)		{
//			max_updown=depthmarket.UpdownRate;
//			max_ins=pinstrumentid;
//
//			min_updown=depthmarket.UpdownRate;
//			min_ins=pinstrumentid;
//		}
//		else		{
//			if(depthmarket.UpdownRate>max_updown)			{
//				max_updown=depthmarket.UpdownRate;
//				max_ins=pinstrumentid;
//			}
//
//			if(depthmarket.UpdownRate<min_updown)			{
//				min_updown=depthmarket.UpdownRate;
//				min_ins=pinstrumentid;
//			}
//		}
//
//		i++;
//	}
//	printf("	市场整体沉淀资金 [%.2f]--------------------> 流入流出资金[%.2f]   \n",totalcapital,totalsedimentaryio);
//	printf("	时间:%s  涨幅最高---->   %s  [%.2f%%]   涨幅最低---->   %s  [%.2f%%]      \n",
//			dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),max_ins.c_str(),max_updown*100,min_ins.c_str(),min_updown*100);
//	this->SetMaxUpDownRate(max_updown,max_ins.c_str());
//	this->SetMinUpDownRate(min_updown,min_ins.c_str());
//	vector<MainContractData>().swap(list);

}




double  Strategy::GetATRStopPrice(const char* pinstrumentid,int direction,double openprice)
{
//	InstrumentData instrumentdata;

//	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday.c_str());
	double atr=this->GetATRbyTradingDay(pinstrumentid);
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentid);

	double stopprice=0;

	if(direction==0)	{
		double m;
		double s;
		s = openprice - atr*this->GetStopRate();

//		m =fmod(s,instrumentdata.PriceTick);
		stopprice = s ;
	}
	else if (direction ==1)	{
		double m;
		double s;
		s = openprice + atr*this->GetStopRate();
//		m =fmod(s,instrumentdata.PriceTick);
		stopprice = s ;
	}

	return stopprice;

}

//按照可用资金计算仓位
int  Strategy::GetPositionByAvalible(MysqlDayLine *mysqldayline,double avalibe,const char* pinstrumentid,DepthMarket depthmarket,double today_tr)
{

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	double ratio=this->GetStopRate();
//	InstrumentData instrumentdata;
	//MysqlInstrument mysqlinstrument;
//	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	std::string tradingday=this->GetTradingDay(); //xxxxxxxx   20170901//
	std::string tradingday_sql =dateutil.ConvertDatetoSqlTime(tradingday.c_str());  //2017-09-01 00:00:00


	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

//	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sql.c_str());
	double atr20=mysqldayline->Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sql.c_str(),20,today_tr);
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentid);

	int position=0;

	if (atr20 >1.0 )	{

		MysqlCommission mysqlcommission;
		Commission commission=mysqlcommission.Find_DatabyProductID(productid);//

		int p_stop;
		double range;

//		if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 )  {
//
////			avalibe=35000.0;    //短线操作 用固定5万起始资金
//			double singlemargin = 0.15;
//
//			range = fabs(atr20*ratio );   // 每次损失控制在1.5倍的atr;
//
//			p_stop=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;
//
//			double marginused = depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin;
//			int p_margin = (avalibe*singlemargin)/( marginused);
//
//			position = min(p_stop,p_margin) ;
//
//			printf("Strategy::GetPositionByAvalible -> %s   avalibe:%.2f  riskrate: %.2f -------可忍受亏损额度: %.2f  \n",
//					pinstrumentid,avalibe,this->GetRiskRate(),avalibe*this->GetRiskRate());
//			printf("Strategy::GetPositionByAvalible -> %s   atr  :%.3f ratio: %.2f  ----------止损价差 : %.3f \n",
//					pinstrumentid,atr20, ratio,range);
//			printf("Strategy::GetPositionByAvalible -> %s    现价: %.3f  交易单位: %d  单合约亏损额度：%.2f ---------固定亏损-->仓位: %d   \n",
//					pinstrumentid,depthmarket.LastPrice, depthmarket.VolumeMultiple, depthmarket.VolumeMultiple*range,p_stop);
//
//			printf("Strategy::GetPositionByAvalible -> %s   单次开仓保证金比例限制 %.2f  交易所保证金比例: %.3f 中粮期货公司保证金比例: %.3f  ---------固定保证金比例-->仓位: %d   \n",
//								pinstrumentid,singlemargin, instrumentdata.LongMarginRatio,commission.Margin,p_margin);
//			printf("Strategy::GetPositionByAvalible--> 最终仓位 : %d   保证金占用: %.3f  保证金占用百分比: %.3f%% \n",
//								position,marginused,marginused/avalibe*100);
//			sprintf(logbuf,"Strategy::GetPositionByAvalible-->zlqh--> %s 最终仓位 : %d   保证金占用: %.3f  保证金占用百分比: %.3f%%\n",
//					pinstrumentid,position,marginused,marginused/avalibe*100);
//			this->GetLogUtil()->WriteLog(logbuf);
//		}
//
//		else {

		//			avalibe=35000.0;    //短线操作 用固定5万起始资金
					double singlemargin_limit = 0.15;

					range = fabs(atr20*ratio );   // 每次损失控制在1.5倍的atr;

					p_stop=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;
					double stop_marginused = (double)p_stop*depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin;

					double single_marginused = depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin;

					int p_margin = (avalibe*singlemargin_limit)/( single_marginused);
					double pmargin_marginused = (double)p_margin*depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin;
		//			int p_margin = (avalibe*singlemargin)/( depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin);

					position = min(p_stop,p_margin) ;
					double positon_marginused = (double)position*depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin;

					printf("Strategy::GetPositionByAvalible -> %s   avalibe:%.2f  riskrate: %.2f -------可忍受亏损额度: %.2f  \n",
							pinstrumentid,avalibe,this->GetRiskRate(),avalibe*this->GetRiskRate());
					printf("Strategy::GetPositionByAvalible -> %s   atr  :%.3f ratio: %.2f  ----------止损价差 : %.3f \n",
							pinstrumentid,atr20, ratio,range);
					printf("Strategy::GetPositionByAvalible ->固定亏损算法 %s    现价: %.3f  交易单位: %d  单合约亏损额度：%.2f ------>开仓数量: %d  保证金:%.3f [%.3f%%] \n",
							pinstrumentid,depthmarket.LastPrice, depthmarket.VolumeMultiple, depthmarket.VolumeMultiple*range,p_stop,stop_marginused,stop_marginused/avalibe*100);
//
//					printf("Strategy::GetPositionByAvalible ->固定亏损算法 %s    现价: %.3f  交易单位: %d  单合约亏损额度：%.2f ------>开仓数量: %d   \n",
//							pinstrumentid,depthmarket.LastPrice, depthmarket.VolumeMultiple, depthmarket.VolumeMultiple*range,p_stop);

					printf("Strategy::GetPositionByAvalible ->固定保证金比例 %s   单次开仓保证金限制 %.2f  期货公司保证金比例: %.3f -->仓位: %d  保证金:%.3f [%.3f%%]  \n",
										pinstrumentid,singlemargin_limit, commission.Margin,p_margin,pmargin_marginused,pmargin_marginused/avalibe*100);
		//			printf("Strategy::GetPositionByAvalible--> 最终仓位 : %d    \n",position);
					printf("Strategy::GetPositionByAvalible--> 最终仓位 : %d   单手开仓保证金: %.3f [%.3f%%]  最终保证金:%.3f [%.3f%%] \n",
							position,single_marginused,single_marginused/avalibe*100,positon_marginused,positon_marginused/avalibe*100);

					sprintf(logbuf,"Strategy::GetPositionByAvalible--> %s 最终仓位 : %d   现价: %.3f 单手开仓保证金比例: %.3f  最终保证金:%.3f [%.3f%%] \n",
							pinstrumentid,position,depthmarket.LastPrice,single_marginused,positon_marginused,positon_marginused/avalibe*100);

//					sprintf(logbuf,"Strategy::GetPositionByAvalible--> --> %s 最终仓位 : %d    \n",pinstrumentid,position);
					this->GetLogUtil()->WriteLog(logbuf);

//		}
//		else{
//			range = fabs(atr20*ratio );   // 每次损失控制在10000.00 ;
//			avalibe=this->GetAssetAvaliblebyStrategyName(this->GetStrategyName().c_str());
//			p_stop=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;
//			position = p_stop ;
//			double marginused = depthmarket.LastPrice* depthmarket.VolumeMultiple*instrumentdata.LongMarginRatio;
//			sprintf(logbuf,"Strategy::GetPositionByAvalible--> %s 最终仓位 : %d   保证金占用: %.3f \n",pinstrumentid,position,marginused);
//			this->GetLogUtil()->WriteLog(logbuf);
//		}


	}
	else {
		position =0;
	}

	return position;

}
//按照可用资金计算仓位
int  Strategy::GetPosition(const char* pinstrumentid,DepthMarket depthmarket,double today_tr)
{
	double ratio=this->GetStopRate();
//	InstrumentData instrumentdata;
	//MysqlInstrument mysqlinstrument;
	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	std::string tradingday=this->GetTradingDay(); //xxxxxxxx   20170901//
	std::string tradingday_sql =dateutil.ConvertDatetoSqlTime(tradingday.c_str());
//	char time[20];
//	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
//			str_tradingdate.substr(6,2).c_str());
//	tradingday=time;   //2017-09-01 00:00:00

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

//	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sql.c_str());
	double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sql.c_str(),20,today_tr);
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentid);

	int position=0;

	if (atr20 >1.0 )	{

		MysqlCommission mysqlcommission;
		Commission commission=mysqlcommission.Find_DatabyProductID(productid);
//
		double avalibe ;
		int p_stop;
		double range;
		avalibe=this->GetAssetAvaliblebyStrategyName(this->GetStrategyName().c_str());
		range = fabs(atr20*ratio );   // 每次损失控制在1.5倍的atr;
		p_stop=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;
		int p_margin = (avalibe*0.1)/( depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin);
		position = min(p_stop,p_margin) ;

		printf("Strategy::GetPosition -> %s   avalibe:%.2f  riskrate: %.2f -------可忍受亏损额度: %.2f  \n",
				pinstrumentid,avalibe,this->GetRiskRate(),avalibe*this->GetRiskRate());
		printf("Strategy::GetPosition -> %s   atr  :%.3f ratio: %.2f  ----------止损价差 : %.3f \n",
				pinstrumentid,atr20, ratio,range);
		printf("Strategy::GetPosition -> %s    现价: %.3f  交易单位: %d  ---------固定亏损-->仓位: %d   \n",
				pinstrumentid,depthmarket.LastPrice, depthmarket.VolumeMultiple,p_stop);

		printf("Strategy::GetPosition -> %s   单次开仓保证金比例 %.2f  期货公司保证金比例: %.3f  ---------固定保证金比例-->仓位: %d   \n",
							pinstrumentid,0.1,commission.Margin,p_margin);
		printf("Strategy::GetPosition--> 最终仓位 : %d    \n",position);

//		int p_margin = (avalibe*0.1)/( depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin);


//		if (strcmp(section,"Single")==0)
//		{
//			position = p /2 - (p/2) %2;
//		}
//		else
//		{
//			position = p-p%2 ;
//		}
	}
	else {
		position =0;
	}

	return position;

}
//按照可用资金计算仓位
int  Strategy::GetMinPosition(const char* pinstrumentid,DepthMarket depthmarket,double today_tr)
{
	double ratio=this->GetStopRate();
/*	InstrumentData instrumentdata;*/
	//MysqlInstrument mysqlinstrument;
	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	std::string tradingday=this->GetTradingDay(); //xxxxxxxx   20170901//
		std::string tradingday_sql =dateutil.ConvertDatetoSqlTime(tradingday.c_str());
//	char time[20];
//	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
//			str_tradingdate.substr(6,2).c_str());
//	tradingday=time;   //2017-09-01 00:00:00

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

//	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,tradingday_sql.c_str());

//	double tr ;
//	double tr_lowclose = abs(depthmarket.LowestPrice-preday.Close);
//	double tr_highclose =abs(depthmarket.HighestPrice-preday.Close);
//	double tr_highlow = abs(depthmarket.HighestPrice-depthmarket.LowestPrice);
//	tr=max(tr_highlow,max(tr_lowclose,tr_highclose));

	double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sql.c_str(),20,today_tr);
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentid);

	int position=0;

	if (atr20 >1.0 )
	{

		double range = fabs(atr20*ratio );   // 每次损失控制在10000.00 ;

		MysqlCommission mysqlcommission;
		Commission commission=mysqlcommission.Find_DatabyProductID(productid);
		double avalibe=this->GetAssetAvaliblebyStrategyName(this->GetStrategyName().c_str());

		int p_stop=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;

		int p_margin = (avalibe*0.1)/( depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin);

		position = min(p_stop,p_margin) ;

//		printf("GetMinPosition -> %s   avalibe:%.2f  现价: %.3f  atr  :%.3f  range : %.3f  ratio: %.2f p_stop: %d p_margin: %d position: %d \n",
//					pinstrumentid,avalibe,depthmarket.LastPrice,atr20,range,ratio,p_stop,p_margin,position);
	}


	return position;

}
//计算远近合约的基差变化，如果输入dif 价格 上破 或者 下破 前期n 天的基差，则ret 返回差价 否则返回0
int  Strategy::CheckDifPriceByNextDayline(MysqlDayLine * pmysqldayline,
		const char*maincontract,const char*nextcontract,int n ,double dif){

	int ret=0;

	vector<HxDayLineData> list_main;
	vector<HxDayLineData> list_next;
	vector<double> list_dif;

	int count_main = pmysqldayline->CountbyAll(maincontract);
	int count_next = pmysqldayline->CountbyAll(nextcontract);


	if(count_main>=n  && count_next >=n ){

		pmysqldayline->Find_ListsLimit(list_main,maincontract,n);
		pmysqldayline->Find_ListsLimit(list_next,nextcontract,n);

		double max_dif ;
		double min_dif;
		for ( int ix = 0; ix < list_main.size(); ++ix ) {
			if(strcmp(list_main[ix].Time,list_next[ix].Time)==0 ){
				double price_dif = list_main[ix].Close- list_next[ix].Close;
	//			printf("	Strategy::CheckDifPriceByNextDayline --->  %s     %.2f   \n ", list_main[ix].Time,price_dif);
				list_dif.push_back(price_dif);

				if(ix==0){
					max_dif=price_dif;
					min_dif=price_dif;
				}
				else {
					if(price_dif>max_dif){
						max_dif=price_dif;
					}

					if(price_dif<min_dif){
						min_dif=price_dif;
					}
				}
			}
			else{
				printf("	Strategy::CheckDifPriceByNextDayline --->日期不同  %s:%s     %s:%s   \n ",
						maincontract,list_main[ix].Time,nextcontract,list_next[ix].Time);
				return 0;
			}

		}
	//	printf("max   %.2f    min    %.2f   \n",max_dif,min_dif);

		if(dif > max_dif ){
	//		printf("max   %.2f    dif    %.2f   \n",max_dif,dif);
			ret=dif-max_dif;
		}
		else if (dif < min_dif){
	//		printf("min   %.2f    dif    %.2f   \n",max_dif,dif);
			ret=dif-min_dif;
		}

	}
	else{
		printf("	Strategy::CheckDifPriceByNextDayline --->ERROR---->日线数据不足%d天!  %s [%d]    %s[%d]   \n ",n,
				maincontract,count_main,nextcontract,count_next );
		ret=0;
	}

	vector<HxDayLineData>().swap(list_main);
	vector<HxDayLineData>().swap(list_next);
	vector<double>().swap(list_dif);

	return ret;
}



// 开仓信号更新,并写入止损价
void Strategy::DayTrade_OpenSignalCheck(HoldData item,
		std::string signalname,
		const char* tradingday,
		int signalcount,
		int direction,
		int offset,
		double lastprice)
{
	DateUtil dateutil;



	if( this->GetRedisSignal()->Exists(signalname)== 0 )
		{
//										printf("不存在signal --> %s\n", signalname.c_str());
			Signal signal;
			strcpy(signal.InstrumentID,item.InstrumentID);
			strcpy(signal.TradingDay,tradingday);
			strcpy(signal.SingalName,signalname.c_str());
			signal.Volume = item.Volume;


			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			strcpy(signal.SingalStartTime,str_localupdate.c_str());
			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
			signal.SignalCount = 1;
			signal.SingalActive=false;
			signal.OrderSend=false;
			signal.Direction=direction;
			signal.Offset=offset;
			signal.Price=lastprice;

//			double stopprice=GetATRStopPrice(item.InstrumentID,direction,lastprice);
			signal.StopRange=this->GetStopPriceRange() ;
			if(signal.Direction==0)
			{
				signal.WinPrice= lastprice+signal.StopRange*2.0;
				signal.StopPrice=lastprice-signal.StopRange;
			}
			else if (signal.Direction==1)
			{
				signal.WinPrice= lastprice-signal.StopRange*2.0;
				signal.StopPrice=lastprice+signal.StopRange;
			}




			this->GetRedisSignal()->SetInsSignal(signal,signalname);
		}

	else if(this->GetRedisSignal()->Exists(signalname)> 0)
		{
//										printf("存在signal !!!!--> %s\n", signalname.c_str());
			Signal signal;
			signal=this->GetRedisSignal()->GetInsSignal(signalname);
			signal.SignalCount++;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
			if(signal.SignalCount>signalcount)
			{
				signal.SingalActive=true;
			}
			signal.Price=lastprice;



			signal.StopRange=this->GetStopPriceRange() ;
			if(signal.Direction==0)
			{
				signal.WinPrice= lastprice+signal.StopRange*2.0;
				signal.StopPrice=lastprice-signal.StopRange;
			}
			else if (signal.Direction==1)
			{
				signal.WinPrice= lastprice-signal.StopRange*2.0;
				signal.StopPrice=lastprice+signal.StopRange;
			}


			this->GetRedisSignal()->SetInsSignal(signal,signalname);

		}


}






void Strategy::DayTrade_EndTimeClose_SignalCheck()
{
	char logbuf[1024];

	std::string tradingday=this->GetDataWrapper()->GetTradingDay();
	if(tradingday.length()<8){
		std::cout<<"no trading day "<< tradingday.length()<<endl;
		return;
	}
//	TradingDayData tradingdaydata=this->GetTradingDayData();
	DateUtil dateutil;

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;

//	mysqlhold.Find_AllTradingDayList(list,this->GetTradingDay().c_str());

	mysqlhold.Find_AllList(list);
	if(list.size()>0)	{
		printf("持仓策略: 所有持仓品种尾盘调仓----->当日持仓批次[%ld] ----> %s\n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}

	for (auto &item:list)		{

		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_daytrade_endtime");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());


		char ProductID[20];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
		if (!opentime){
			printf("   尾盘平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

		bool endclosetime = dateutil.CheckSHFEEndMarketTime(this->GetDifSec());
		if (!endclosetime){
			printf("   尾盘平仓策略验证 --> [%s]非尾盘平仓时间 !!!  \n",item.InstrumentID);
			continue;
		}

		printf("ProductID -->  %s  %ld   \n",item.InstrumentID,sizeof(item.InstrumentID));

		DepthMarket depthmarket;
		depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
		printf("	持仓信号[%s]-->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f   bid:%.3f  ask:%.3f\n",signalname.c_str(),item.InstrumentID,
							item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice,depthmarket.BidPrice1,depthmarket.AskPrice1);
//		MysqlDepthMarket mysqldepthmarket;
//		DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);
//				bool valid4 = this->ValidCheck_DepthMarket(&depthmarket);

		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf("  尾盘平仓策略验证 --> [%s]行情数据时不合规!!!  \n",item.InstrumentID);
			continue;
		}

//				bool valid =  valid4;
		// 多头出现亏损
		if(item.Direction==0)	{
			printf("	多头合约持仓平仓信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);

			sprintf(logbuf,"	多头合约尾盘平仓信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			sprintf(logbuf,"	多头合约尾盘平仓信号 -->  %s  Redis depthmarket----> 涨停价: %.3f  跌停价: %.3f 昨结算价: %.3f \n",item.InstrumentID,
					depthmarket.UpperLimitPrice,depthmarket.LowerLimitPrice,depthmarket.PreSettlementPrice);
			this->GetLogUtil()->WriteLog(logbuf);


			Signal signal;
			this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
			CloseSignalUpdate(&signal);
		}
		//空头出现亏损
		else if(item.Direction==1)		{
			printf("	空头合约尾盘平仓信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	空头合约尾盘平仓信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);

			sprintf(logbuf,"	空头合约尾盘平仓信号 -->  %s  Redis depthmarket----> 涨停价: %.3f  跌停价: %.3f 昨结算价: %.3f \n",item.InstrumentID,
					depthmarket.UpperLimitPrice,depthmarket.LowerLimitPrice,depthmarket.PreSettlementPrice);
			this->GetLogUtil()->WriteLog(logbuf);

			Signal signal;
			this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
			CloseSignalUpdate(&signal);
		}
		else{
//									printf("平信号消失 --> %s\n", pinstrumentID);
			SignalDispearCheck(signalname);

		}
	}// for loop

//std::cout<< "end endtime sta"<<std::endl;

	vector<HoldData>().swap(list);
}
// 监测持仓单 是否到止盈值,如果达到,根据止盈信号出现次数 激活止盈信号
void Strategy::Close_WinPrice_SignalCheck()
{
	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	DateUtil dateutil;

	if(list.size()>0)	{
		printf("持仓策略: 持仓合约止盈信号监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}

	for (auto &item:list)	{
//				printf("WinPriceSignalCheck----->pinstrumentid %s TraderID %s \n",item.pinstrumentid.c_str(),
//								item.TradeID.c_str());

			std::string signalname;
			signalname=item.InstrumentID;
			signalname.append("_daytrade_winclose");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

			bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
			if (!opentime){
				printf("   尾盘平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
				continue;
			}
//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

			DepthMarket depthmarket;
			memset(&depthmarket,0,sizeof(depthmarket));
			depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			if (!valid_depthmarket){
				printf("  止盈价平仓策略验证 --> [%s]行情数据时不合规!!!  \n",item.InstrumentID);
				continue;
			}


			bool valid_winprice_base = item.WinPrice >1.0;
			if( !valid_winprice_base  )		{
				printf("	止盈价平仓策略验证 --> 止盈价不合规  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
							item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
				continue;
			}

			std::string str_direction;

			bool con1_longwin = depthmarket.LastPrice > item.WinPrice;
			bool con1_shortwin = depthmarket.LastPrice < item.WinPrice;

			if (item.Direction ==0 && con1_longwin )		{
				str_direction="多";
				printf("	多头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %.3f   手数:%d  现价: %.3f    止盈价: %.3f\n",
						item.InstrumentID,item.OpenDate,str_direction.c_str(),
						item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);
			}
			else if (item.Direction ==1 && con1_shortwin )	{
				str_direction="空";
				printf("	空头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %.3f   手数:%d  现价: %.3f   止盈价: %.3f\n",
						item.InstrumentID,item.OpenDate,str_direction.c_str(),
						item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);
			}

			// 多头出现止盈
			if(item.Direction==0  && con1_longwin )			{

				printf("	多头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
						item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

				Signal signal;
				this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
				CloseSignalUpdate(&signal);
			}
			//空头出现止盈
			else if(item.Direction==1  && con1_shortwin )		{
				printf("	空头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
						item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);
				Signal signal;

				this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
				CloseSignalUpdate(&signal);
			}
			else{
//									printf("止损信号消失 --> %s\n", pinstrumentID);
				SignalDispearCheck(signalname);
			}


	}

	vector<HoldData>().swap(list);
}

// 监测持仓单 是否到止损值,如果达到,根据止损信号出现次数 激活止损信号
void Strategy::Close_DynamicStop_SignalCheck()
{
	char logbuf[1024];


	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	std::cout<<trading_date<<endl;

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);

	if(list.size()>0)	{

		printf("持仓合约浮动止盈止损信号监测----->持仓批次[%ld] ----> %s  交易日： %s \n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),this->GetTradingDay().c_str());
	}


	for (auto &item:list)			{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
		char pinstrumentid[40];
//		char pinstrumentIDDate[50];
		strcpy (pinstrumentid,item.InstrumentID);
//		sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_dynamicstop");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(pinstrumentid).c_str());

//////////////////////开盘时间验证///////////////////
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
		if (!opentime){
			printf("   浮动止损平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

//////////////////////日线数据验证///////////////////
		bool valid_dayline = this->GetDataWrapper()->FindDataStatus(pinstrumentid);
		if (!valid_dayline){
			printf("   策略验证 --> [%s]20天日线数据时不合规!!!  \n",pinstrumentid);
			continue;
		}

//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		if (!valid_redisdepth){

			if(!codeutil.IsNightByProductid(ProductID) && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf("	当前夜盘时间,白盘合约在夜盘无数据: %s    如有疑问 请核实!!! \n",pinstrumentid);
				//正常情况 白盘合约在夜盘无数据
				continue;
			}
			else {
				printf("   浮动止损平仓策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
				continue;
			}

		}

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));


		DepthMarket	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf("  浮动止损平仓策略验证 --> [%s]实时行情数据不合规!!!  \n",pinstrumentid);
			continue;
		}

//						printf("止损合约行情比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);

		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(pinstrumentid);
		double atr20=(totaltr+tr)/20;


		bool valid_openprice = item.OpenPriceAverage >1.0 &&  item.OpenPriceAverage <depthmarket.HighestPrice
				&&  item.OpenPriceAverage >depthmarket.LowestPrice;

		if (!valid_openprice){
			printf("  浮动止损平仓策略验证 --> [%s]持仓数据---开仓价[%.2f ]不合规!!!   \n",pinstrumentid,item.OpenPriceAverage);
		}
///////////////////////////////////////持仓止损价合法性验证/////////////////////////////
		bool valid_stopprice = item.StopPrice >1.0 &&
				(item.StopPrice>item.OpenPriceAverage*0.9 || item.StopPrice<item.OpenPriceAverage*1.1);

		if (!valid_stopprice){
			printf("  浮动止损平仓策略验证 --> [%s]持仓数据---止损价[%.2f] 不合规!!!  \n",pinstrumentid,item.StopPrice);
			continue;
		}

		bool valid =  valid_stopprice && valid_openprice;

		HighLowest hl;
		if(strcmp(item.OpenDate,this->GetTradingDay().c_str())==0 )	{
			hl.highest =depthmarket.HighestPrice;
			hl.lowest =depthmarket.LowestPrice;

//						printf("	当日新开仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);

		}
		else{
			 hl= mysqldayline.Collect_CycleDatabyOpenDate(item.InstrumentID,tradingday_sqltime.c_str());

			if (hl.highest < depthmarket.HighestPrice)		{
				hl.highest =depthmarket.HighestPrice;
			}

			if (hl.lowest > depthmarket.LowestPrice || hl.lowest==0)	{
				hl.lowest =depthmarket.LowestPrice;
			}
//							 printf("	历史持仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);

		}

		if(valid && item.Direction ==0 && depthmarket.LastPrice > item.Highest )			{

			item.Highest = depthmarket.LastPrice;
			item.StopPrice = depthmarket.LastPrice - atr20 *this->GetStopRate();
			mysqlhold.UpdateHighest_StopPrice(item);
			printf("	浮动止损平仓策略:多头合约创新高 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	浮动止损平仓策略:多头合约创新高 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",pinstrumentid,
												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);
		}
		else if (valid && item.Direction ==1 && depthmarket.LastPrice < item.Lowest)	{
			item.Lowest = depthmarket.LastPrice;
			item.StopPrice = depthmarket.LastPrice + atr20 *this->GetStopRate();
			mysqlhold.UpdateLowest_StopPrice(item);

			printf("	浮动止损平仓策略:空头合约创新低 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	浮动止损平仓策略:空头合约创新低 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",pinstrumentid,
												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);
		}

		// 多头出现止损
		if(valid && item.Direction==0 && depthmarket.LastPrice <item.StopPrice)			{
			printf("	多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	平仓信号[%s]  多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",signalname.c_str(),pinstrumentid,
												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);

			Signal signal;
			this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
			CloseSignalUpdate(&signal);

		}
		//空头出现止损
		else if(valid && item.Direction==1 && depthmarket.LastPrice > item.StopPrice)	{
			printf("	空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	平仓信号[%s]  空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",signalname.c_str(),pinstrumentid,
												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);
//							int offset =1;

			Signal signal;

			this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());

			CloseSignalUpdate(&signal);

		}
		else	{
//									printf("止损信号消失 --> %s\n", pinstrumentID);
//							sprintf(logbuf,"	止损信号消失 --> %s  %s \n", pinstrumentID,signalname.c_str());
//							this->GetLogUtil()->WriteLog(logbuf);

			SignalDispearCheck(signalname);

		}



	}// for loop

	vector<HoldData>().swap(list);
}

// 监测持仓单 是否到涨跌停价格
void Strategy::Close_LimitPrice_SignalCheck()
{
	char logbuf[1024];


	DateUtil dateutil;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());

	MysqlDayLine mysqldayline;
	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)	{

		printf("持仓合约涨跌停价信号监测----->持仓批次[%ld] ----> %s  交易日： %s \n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),this->GetTradingDay().c_str());
	}


	for (auto &item:list)			{
//				printf("Close_LimitPrice_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
			char pinstrumentid[40];
			strcpy (pinstrumentid,item.InstrumentID);

//			char pinstrumentIDDate[50];
//			sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_limitwin");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char productid[31];
			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

			bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,productid);
			if (!opentime){
				printf("   尾盘平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
				continue;
			}

			int code_base = this->Valid_StaBaseDataCheck(pinstrumentid,&mysqldayline,trading_date.c_str(),&item);
			if(code_base!=0){
				continue;
			}

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
			DepthMarket depthmarket;
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
			DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

			bool valid_price = data.LowerLimitPrice==depthmarket.LowerLimitPrice &&
					data.UpperLimitPrice && depthmarket.UpperLimitPrice;
			if(valid_price){
				printf("	涨跌停合约行情比对(MysqlDepthMarket比对相同) -->  %s 开仓价: %.3f 方向:%d  涨停价: %.3f 跌停价: %.3f   最小变动价位:%.3f 现价: %.3f \n",
						item.InstrumentID,item.OpenPriceAverage,item.Direction,data.UpperLimitPrice,
						data.LowerLimitPrice,depthmarket.PriceTick,depthmarket.LastPrice);
				printf("	涨跌停合约目标价位-->  %s 涨停价: [%.3f ---- %.3f] 跌停价: [%.3f ---- %.3f]  \n",
						item.InstrumentID,depthmarket.UpperLimitPrice-depthmarket.PriceTick *3,data.UpperLimitPrice,
						data.LowerLimitPrice,depthmarket.LowerLimitPrice+depthmarket.PriceTick *3);
			}
			else{
				printf("	涨跌停合约行情比对 -->>>>>>>> %s 开仓价: %.3f 方向:%d  涨停价: %.3f 跌停价: %.3f   现价: %.3f \n",item.InstrumentID,
					item.OpenPriceAverage,item.Direction,depthmarket.UpperLimitPrice,
					depthmarket.LowerLimitPrice,depthmarket.LastPrice);
				printf("	涨跌停合约行情比对(MysqlDepthMarket比对不同!!!) -->>>>>  %s 开仓价: %.3f 方向:%d  涨停价: %.3f 跌停价: %.3f   现价: %.3f \n",item.InstrumentID,
					item.OpenPriceAverage,item.Direction,data.UpperLimitPrice,
					data.LowerLimitPrice,depthmarket.LastPrice);
			}

			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid = false;

			if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0||
							strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
				valid= valid_depthmarket && valid_price;
			}
			else{
				valid=valid_depthmarket;
			}


			bool upperprice = depthmarket.LastPrice>=depthmarket.UpperLimitPrice-depthmarket.PriceTick *3
					&& depthmarket.LastPrice <= depthmarket.UpperLimitPrice ;
			bool lowerprice = depthmarket.LastPrice<=depthmarket.LowerLimitPrice+depthmarket.PriceTick *3
					&& depthmarket.LastPrice >= depthmarket.LowerLimitPrice ;

			if(upperprice){
				printf("	接近涨停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
						depthmarket.UpperLimitPrice-depthmarket.PriceTick *3,depthmarket.UpperLimitPrice);
				sprintf(logbuf,"	接近涨停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
													depthmarket.UpperLimitPrice-depthmarket.PriceTick *3,depthmarket.UpperLimitPrice);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else if(lowerprice){
				printf("	接近跌停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
						depthmarket.LowerLimitPrice+depthmarket.PriceTick *3,depthmarket.LowerLimitPrice);
				sprintf(logbuf,"	接近跌停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
													depthmarket.LowerLimitPrice+depthmarket.PriceTick *3,depthmarket.LowerLimitPrice);
				this->GetLogUtil()->WriteLog(logbuf);
			}

			// 多头出现涨停
			if(valid && item.Direction==0 && upperprice)			{
				printf("	多头合约涨停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				sprintf(logbuf,"	平仓信号[%s]  多头合约涨停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",signalname.c_str(),pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				this->GetLogUtil()->WriteLog(logbuf);

				Signal signal;
				this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
				CloseSignalUpdate(&signal);
			}
			//空头出现跌停
			else if(valid && item.Direction==1 && lowerprice)	{
				printf("	空头合约跌停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				sprintf(logbuf,"	平仓信号[%s]  空头合约跌停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",signalname.c_str(),pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				this->GetLogUtil()->WriteLog(logbuf);

				Signal signal;
				this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
				this->CloseSignalUpdate(&signal);
			}
			else	{
//									printf("止损信号消失 --> %s\n", pinstrumentID);
//							sprintf(logbuf,"	止损信号消失 --> %s  %s \n", pinstrumentID,signalname.c_str());
//							this->GetLogUtil()->WriteLog(logbuf);
				SignalDispearCheck(signalname);
			}

	}

	vector<HoldData>().swap(list);
}

// 监测持仓单 是否大量资金流出  仅仅限多头持仓合约
void Strategy::Close_SedimentaryOut_SignalCheck()
{
	char logbuf[1024];


	DateUtil dateutil;
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());
	MysqlDayLine mysqldayline;
	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)	{

		printf("持仓策略:资金流出过多风控平仓----->持仓批次[%ld] ----> %s  交易日： %s \n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),this->GetTradingDay().c_str());
	}


	for (auto &item:list)			{
//				printf("Close_SedimentaryOut_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());

		if(item.Direction==1){
			continue;
		}
		char pinstrumentid[40];
		strcpy (pinstrumentid,item.InstrumentID);
		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_longsed_out");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		int  signalcount=3;

		int code_base = this->Valid_StaBaseDataCheck(pinstrumentid,&mysqldayline,trading_date.c_str(),&item);
		if(code_base!=0){
		continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);

		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		bool valid = valid_depthmarket;
		bool closecondition_long = depthmarket.SedimentaryIORate <-0.04;

		// 多头
		if(valid && item.Direction==0 && closecondition_long)			{
			printf("	多头合约资金流出风控平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,depthmarket.LastPrice);
			sprintf(logbuf,"	平仓信号[%s]  多头合约资金流出风控平仓 -->  %s 开仓价: %.3f  现价: %.3f  沉淀资金流出比:%.2f%% \n",signalname.c_str(),pinstrumentid,
					item.OpenPriceAverage,depthmarket.LastPrice,depthmarket.SedimentaryIORate*100);
			this->GetLogUtil()->WriteLog(logbuf);

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
			signal.StopRange  = item.StopRange;
			CloseSignalUpdate(&signal);
		}
		else	{
//									printf("止损信号消失 --> %s\n", pinstrumentID);
//							sprintf(logbuf,"	止损信号消失 --> %s  %s \n", pinstrumentID,signalname.c_str());
//							this->GetLogUtil()->WriteLog(logbuf);
			SignalDispearCheck(signalname);
		}

	}

	vector<HoldData>().swap(list);
}
// 监测持仓单 板块走势是否反向,根据止损信号出现次数 激活止损信号
void Strategy::Close_SectionTrendStop_SignalCheck()
{
	char logbuf[1024];

	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetTradingDay().c_str());

	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);

	if(list.size()>0)	{

		printf("持仓合约所在SectionTrend信号监测----->持仓批次[%ld] ----> %s  交易日： %s \n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),this->GetTradingDay().c_str());
	}


	for (auto &item:list)	{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
				char pinstrumentID[40];
				char pinstrumentIDDate[50];
				strcpy (pinstrumentID,item.InstrumentID);
				sprintf(pinstrumentIDDate,"%s_%s",item.InstrumentID,item.OpenDate);

				std::string signalname;
				signalname=pinstrumentID;
				signalname.append("_sectiontrendstop");
				signalname.append("_");
				signalname.append(this->GetUserID().c_str());
				int  signalcount=3;

				char productid[31];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(pinstrumentID).c_str());
				std::string section;
				section =codeutil.GetSectionByProductid(productid);

				SectionInfo sectioninfo;
				strcpy(sectioninfo.SectionName,section.c_str());

				if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,productid)  )  { //盘中执行平仓操作
//					if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )  {//判断当前交易时间是否是尾盘,

						printf("pinstrumentID ---> %s   ---->  %s -->  %s  %ld   %ld \n",pinstrumentID,productid,section.c_str(),
								sizeof(pinstrumentID),sizeof(pinstrumentID));
						DepthMarket depthmarket;
						depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentID);
//						printf("止损合约行情比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);

//						DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

						bool valid_depthmarket =  this->ValidCheck_DepthMarket(&depthmarket);;
						bool valid_section_long =false;
						bool valid_section_short =false;
						double rate=0.6;
						if(this->GetRedisSection()->Exists(section) >0){

							sectioninfo =this->GetRedisSection()->GetSection(section);
							if(sectioninfo.CountUprate >rate){
								valid_section_long=true;
								printf("SectionTrend Long ---> %s   ---->  %s -->  %s  %ld   %ld \n",pinstrumentID,productid,section.c_str(),
																sizeof(pinstrumentID),sizeof(pinstrumentID));
							}

							if(sectioninfo.CountDownrate >rate){
								valid_section_short=true;
								printf("SectionTrend Short ---> %s   ---->  %s -->  %s  %ld   %ld \n",pinstrumentID,productid,section.c_str(),
																sizeof(pinstrumentID),sizeof(pinstrumentID));
							}
						}

						// 多头出现止损
						if(valid_depthmarket && item.Direction==0 && valid_section_short )		{
							printf("	多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							sprintf(logbuf,"	平仓信号[%s]  多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signalname.c_str(),pinstrumentID,
																item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							this->GetLogUtil()->WriteLog(logbuf);
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
						else if(valid_depthmarket && item.Direction==1  && valid_section_long )	{
							printf("	空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentID,
									item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							sprintf(logbuf,"	平仓信号[%s]  空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",signalname.c_str(),pinstrumentID,
																item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
							this->GetLogUtil()->WriteLog(logbuf);
//							int offset =1;

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
						else	{
							SignalDispearCheck(signalname);

						}

					}

			}

	vector<HoldData>().swap(list);
}

void Strategy::Close_ExpireMonthClose_SignalCheck()
{
	//MysqlInstrument mysqlinstrument;
//	TradingDayData tradingdaydata=this->GetTradingDayData();
	char tradingyd[10];
	std::string str_tradingday = this->GetTradingDay();
	strcpy(tradingyd,str_tradingday.substr(0,6).c_str());
	DateUtil dateutil;


	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  list;
	mysqlhold.Find_AllList(list);
	if(list.size()>0)	{
		printf("持仓品种到期月份监测----->持仓批次[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}


	for (auto &item:list)		{
//				printf("EndTimeClose_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
			char pinstrumentID[40];
			strcpy (pinstrumentID,item.InstrumentID);

			std::string signalname;
			signalname=pinstrumentID;
			signalname.append("_expire");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			int signalcount=5;

			char ProductID[31];
			CodeUtil codeutil;
			strcpy(ProductID,codeutil.GetProductID(pinstrumentID).c_str());

//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID)  )  //判断合约到期后的平仓,可以盘中操作
//				{
////					printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
//
////					InstrumentData instrumentdata ;
////					instrumentdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentID);
////					InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);
//					std::string str_exprie=insdata.ExpireDate;
//					char exprie[10];
//					strcpy(exprie,str_exprie.substr(0,6).c_str());
//
//					time_t tt_lasttrade = dateutil.Collect_ExpireTradeDate(insdata.ExpireDate);  // 最后交易日 倒推15天
//					time_t tt = time(NULL);
//
//					DepthMarket depthmarket;
//					depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentID);
//
//					//MysqlDepthMarket mysqldepthmarket;
//					DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);
//
//					double sedimentary;
//					sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);
////					printf("	持仓合约到期月份监测 -->  %s 开仓价: %.3f 方向:%d  到期日: %s  交易日: %s  沉淀资金: %.2f \n",pinstrumentID,
////										item.OpenPriceAverage,item.Direction,tradingyd,exprie,sedimentary);
//
//					bool condition0 = sedimentary<0.5 && sedimentary !=0;
////					bool condition1 = strcmp(tradingyd,exprie)==0;    // 交易日年月   和 到期日 年月 相同
//					bool condition2 = tt >tt_lasttrade;    // 交易日倒推15天  结束交易
////					if(condition2)
//					{
//						printf("	持仓合约到期月份监测 -->  %s 系统自定义截止交易日:%s [%ld]  当前交易日:%s [%ld]  \n",
//								pinstrumentID,dateutil.ConvertTimetoSqlString(tt_lasttrade).c_str(),tt_lasttrade,dateutil.ConvertTimetoSqlString(tt).c_str(),tt);
//					}
//
//					bool valid0 = depthmarket.OpenInterest !=0 ;
//					bool valid1 =  depthmarket.LastPrice >data.LowerLimitPrice;
//					bool valid2 = depthmarket.LastPrice < data.UpperLimitPrice;
//					bool valid3 = depthmarket.LastPrice !=0 ;
//
//
//					bool condition = (condition0 || condition2) &&valid0 && valid1 && valid2 && valid3;
//
//					// 多头出现亏损
//					if(condition && item.Direction==0 )
//					{
//						printf("	多头合约到期 -->  %s 开仓价: %.3f  到期日: %s  交易日: %s\n",pinstrumentID,
//								item.OpenPriceAverage,tradingyd,exprie);
//						int offset =1;   //平仓
//						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);
//
//
//					}
//					//空头出现亏损
//					else if(condition && item.Direction==1 )
//					{
//						printf("	空头合约亏损 -->  %s 开仓价: %.3f   到期日: %s  交易日: %s\n",pinstrumentID,
//								item.OpenPriceAverage,tradingyd,exprie);
//						int offset =1;   // 平仓
//						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);
//
//
//					}
//					else
//					{
//	//									printf("止损信号消失 --> %s\n", pinstrumentID);
//						SignalDispearCheck(signalname);
//					}
//				}


		}

	vector<HoldData>().swap(list);
}
void Strategy::run(){

	DateUtil dateutil;

	this->SetAvalible(this->GetInitialMoney());
	this->SetTradingDay(this->GetDataWrapper()->GetTradingDay().c_str());
	this->SetDifSec(this->GetDataWrapper()->GetDifSec());
//	this->UpdateTradingDay();
	std::cout<<"Strategy::run sta "<<this->GetTradingDay()<<std::endl;
	 struct timeval start, m,end;

	while(1){

//		gettimeofday( &m, NULL );
		printf(">>>>>>>>>>>>>>>m ----------------> %ld.%ld\n", m.tv_sec, m.tv_usec);

		if(this->GetDataWrapper()->GetPredataStatus()){

			gettimeofday( &start, NULL );
	//		printf("start ----------------> %ld.%ld\n", start.tv_sec, start.tv_usec);
			printf(">>>>>>>>>>>>>>Strategy::run  -------------------------------------[%ld.%ld]----------------------------------------->>>>>>   [%s] \n",
					 start.tv_sec, start.tv_usec,dateutil.GetCurrentSqlTimeString().c_str());


			if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadATRTrade")==0 )	{
			//		this->GetStrategy()->Open_Simple();
			//		this->GetStrategy()->Open_CalendarSpreadSignal();
					this->Open_BigVolumOneMinSignal();
			//		this->GetStrategy()->MysqlSectionAssess("_bigvolume");
			//		this->GetSecAssess()->SectionAssess(this->GetStrategy(),"_spread");  // 对开仓信号 按照板块进行整理
				}
				else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0){
			//		this->GetStrategy()->Open_BigVolumOneMinSignal();
					this->Open_AbsMaxUpDownRate();
				}
				else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0 ||
								strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){
					this->Open_BigVolumOneMinSignal();
				}
				else if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade")==0){
					this->Open_AbsMaxUpDownRate();
				}
				else{
		//				this->Open_SpreadSignal_Average();
		//				this->SectionAssess(this->GetStrategy(),"_spread");  // 对开仓信号 按照板块进行整理
				}
				if (!dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())){
					this->QueueingOpenPriceChange_SignalCheck();
				}

				long int holdcount=this->GetDataWrapper()->GetHoldingPosition();
				if(holdcount>0)	{
					printf(">>>>>>>>>>>>>>>>>>>>>>持仓策略: 持仓合约止盈信号监测----->持仓批次[%ld] ----> %s\n",holdcount,
							dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
					this->DayTrade_EndTimeClose_SignalCheck();
					this->Close_DynamicStop_SignalCheck();
					this->Close_LimitPrice_SignalCheck();
					this->Close_WinPrice_SignalCheck();
					this->QueueingClosePriceChange_SignalCheck();
				}
		}





//		sleep (1);
//		usleep(10);
//		sleep(5);

//		 gettimeofday( &end, NULL );
//		printf("end   ----------------------> %ld.%ld\n", end.tv_sec, end.tv_usec);
//		printf("Strategy::run  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< while end   [%s] \n",
//						dateutil.GetCurrentSqlTimeString().c_str());
	}



	return ;
}
void  Strategy::Open()
{

}
void  Strategy::Close()
{

}
void  Strategy::Open_BigVolumOneMinSignal()
{
//	printf("Strategy::Open_BigVolumOneMinSignal  begin----------->  \n");

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double avalible = this->GetInitialMoney() ;
	printf("Strategy::Open_BigVolumOneMinSignal  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	 double bigvolumerate =this->GetBigVolumeRate();

	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());

	std::string funname="Strategy::Open_BigVolumOneMinSignal";


	if(dateutil.TodayIsHoliday()){
		printf("%s----->本交易日为假日,不运行策略!!!!!         \n",funname.c_str());
		return;
	}

	if(dateutil.IsHolidaybyNextTradingDay()){
		printf("%s----->下一个交易日为假日,本交易日不运行策略!!!!!         \n",funname.c_str());
		return;
	}

	if(dateutil.isFridayNight(this->GetDifSec())){
		printf("%s----->周五夜盘不运行策略!!!!          \n",funname.c_str());
		return;
	}

//	if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0 ||
//		strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){

	// 控制开仓时间在开盘后5分钟再操作
	 if(!dateutil.isNightMorningBreakTime(this->GetDifSec())){
		printf("%s----->非策略允许交易时间!!!!          \n",funname.c_str());
		return;
	}
//	}


	CodeUtil codeutil;

	MysqlMinData m_mysqlmindata;
	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
			item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);
//////////////////////开盘时间验证///////////////////
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
//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		if (!valid_redisdepth){

			if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
				//正常情况 白盘合约在夜盘无数据
			}
			else {
				printf("   策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
			}

			continue;
		}

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_bigvolume");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());

		char productid[31];
		strcpy(productid,item->ProductID);

		string mindatabase="real_minute_";
		mindatabase.append(productid);

		string mindata_tablename;
		mindata_tablename=pinstrumentid;
		mindata_tablename.append("_");
		mindata_tablename.append(this->GetDataWrapper()->GetTradingDay().c_str());

		m_mysqlmindata.Set_Database(mindatabase.c_str());
		m_mysqlmindata.Set_Table(mindata_tablename.c_str());

//			printf("   database %s  :   table :%s  \n",mindatabase.c_str(),mindata_tablename.c_str());
		Mysql_MinData mysql_mindata;
		mysql_mindata = m_mysqlmindata.Find_LastDatabyMinuteTime(mindata_tablename.c_str());
		Mysql_MinData mysql_max_mindata;
		mysql_max_mindata = m_mysqlmindata.Find_MaxVolume(mindata_tablename.c_str());

		DepthMarket depthmarket;
		if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
		}
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
		bool longcond_vol = (double) mysql_mindata.Volume > (double)mysql_mindata.OpenInterest*bigvolumerate;
		bool longcond_uprate = depthmarket.LastPrice  <depthmarket.UpperLimitPrice*0.985;
		bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
				&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
				&& depthmarket.UpdownRate>0.005 ;
//				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
//						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
//						&& depthmarket.AveragePrice> depthmarket.OpenPrice && depthmarket.UpdownRate>0.005 ;
//				bool longcond_dif = ret_dif >0;
		bool condition_long =  longcond_vol &&longcond_cycle&&longcond_uprate ;

		//空头行情
		bool shortcond_cycle =  depthmarket.LastPrice < pcycledata->Low20;
		bool shortcond_vol  =  (double) mysql_mindata.Volume > (double)mysql_mindata.OpenInterest*bigvolumerate;
		bool shortcond_downrate =depthmarket.LastPrice  >depthmarket.LowerLimitPrice*1.015;
		bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
				&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
				&& depthmarket.UpdownRate<-0.005;

//				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
//						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
//						&& depthmarket.AveragePrice< depthmarket.OpenPrice && depthmarket.UpdownRate<-0.005;
//				bool shortcond_dif = ret_dif <0;
		bool condition_short = shortcond_vol && shortcond_cycle && shortcond_downrate;

		std::shared_ptr<Commission>  commssion=this->GetDataWrapper()->FindCommissionData(item->ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/100000000.0;

		bool valid_sedimentary = sedimentary>this->GetMaxSedimentary();
		if (!valid_sedimentary){
			printf("	策略验证 -->信号未激活  合约[%s] 沉淀资金:%.2f不符合 沉淀资金低于[%.2f]亿元,无法交易!!!  \n",pinstrumentid,
					sedimentary,this->GetMaxSedimentary());
//					printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
//							pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,commission.Margin,depthmarket.LastPrice);

//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}


		if(longcond_cycle  ){
			printf("	1.[CycleN当日趋势突破多头--周期%d] -->%s    现价: %.3f  最高价: %.3f \n",5,
					pinstrumentid,depthmarket.LastPrice,pcycledata->High20);
			if( (double)mysql_mindata.Volume > (double)mysql_mindata.OpenInterest*bigvolumerate){
				printf("  %s last min :   vol:%d   opi:%.f     ---[  max_vol:%d   max_vol_opi:%.f  max_vol mintime:%s ] \n",pinstrumentid,
						mysql_mindata.Volume,mysql_mindata.OpenInterest,mysql_max_mindata.Volume,mysql_max_mindata.OpenInterest,
						mysql_max_mindata.MinuteTime);
			}
			else {

				if( (double)mysql_max_mindata.Volume > (double)mysql_max_mindata.OpenInterest*bigvolumerate){
				printf("  >>>　%s his min :   vol:%d   opi:%.f     >>> [  max_vol:%d   max_vol_opi:%.f  max_vol mintime:%s ] \n",pinstrumentid,
						mysql_mindata.Volume,mysql_mindata.OpenInterest,mysql_max_mindata.Volume,mysql_max_mindata.OpenInterest,
						mysql_max_mindata.MinuteTime);
				}
				else {
//					printf("  %s 不满足:   vol:%d   opi:%f     ---[  max_vol:%d   max_vol_opi:%f  max_vol mintime:%s ] \n",pinstrumentid,
//						mysql_mindata.Volume,mysql_mindata.OpenInterest,mysql_max_mindata.Volume,mysql_max_mindata.OpenInterest,
//						mysql_max_mindata.MinuteTime);
				}

			}

		}
		else if(shortcond_cycle )	{
			printf("	1.[CycleN当日趋势突破空头--周期%d] -->%s    现价: %.3f  最低价: %.3f \n",5,
					pinstrumentid,depthmarket.LastPrice,pcycledata->Low20);
			if( (double)mysql_mindata.Volume > (double)mysql_mindata.OpenInterest*bigvolumerate){
				printf("  %s last min :   vol:%d   opi:%.f     ---[  max_vol:%d   max_vol_opi:%.f  max_vol mintime:%s ] \n",pinstrumentid,
						mysql_mindata.Volume,mysql_mindata.OpenInterest,mysql_max_mindata.Volume,mysql_max_mindata.OpenInterest,
						mysql_max_mindata.MinuteTime);
			}
			else {

				if( (double)mysql_max_mindata.Volume > (double)mysql_max_mindata.OpenInterest*bigvolumerate){
				printf("  >>>　%s his min :   vol:%d   opi:%.f     >>> [  max_vol:%d   max_vol_opi:%.f  max_vol mintime:%s ] \n",pinstrumentid,
						mysql_mindata.Volume,mysql_mindata.OpenInterest,mysql_max_mindata.Volume,mysql_max_mindata.OpenInterest,
						mysql_max_mindata.MinuteTime);
				}
				else {
//					printf("  %s 不满足:   vol:%d   opi:%f     ---[  max_vol:%d   max_vol_opi:%f  max_vol mintime:%s ] \n",pinstrumentid,
//						mysql_mindata.Volume,mysql_mindata.OpenInterest,mysql_max_mindata.Volume,mysql_max_mindata.OpenInterest,
//						mysql_max_mindata.MinuteTime);
				}

			}
		}


		if (condition_long   )	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf("	Strategy::Open_BigVolumOneMinSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
												signalname.c_str(),position);
				sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
						signalname.c_str(),position);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else {

				printf("	Strategy::Open_BigVolumOneMinSignal 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
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
				printf("	Strategy::Open_BigVolumOneMinSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																		signalname.c_str(),position);
			sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
					signalname.c_str(),position);
			this->GetLogUtil()->WriteLog(logbuf);
			}
			else {
			printf("	Strategy::Open_BigVolumOneMinSignal 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
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
void  Strategy::Open_CalendarSpreadSignal()
{
	printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->  \n");
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double avalible ;
	 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0)	{
		 avalible=this->GetAvalible();
		 printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->	  可用资金(变): %.2f  \n",avalible);
	 }
	 else{
		 avalible = 45000.0;
		 printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	 }

	MysqlDayLine mysqldayline;
//	MysqlUtil mysqlutil(this->GetStrategyName().c_str());
	MysqlCommission mysqlcommission;
//	MysqlHold mysqlhold(this->GetUserID().c_str());

	DateUtil dateutil;
	if(this->GetTradingDay().length()<8){
		printf("StrategySpread::Open_CalendarSpreadSignal  tradingday 为空 ----------->	%ld  \n",this->GetTradingDay().length());
		return;
	}
	else{
		printf("StrategySpread::Open_CalendarSpreadSignal  tradingday----------->	 %s  \n",this->GetTradingDay().c_str());
	}
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901

	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());
	vector<MainContractData>  list;

	bool day=dateutil.CheckSHFEDayTime(this->GetDifSec());
	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());

	if(day){
		this->GetMysqlMaincontract()->Find_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else{
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[0]   \n");
		}
	}
	else if(night){
		this->GetMysqlMaincontract()->Find_Night_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else{
			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[0]   \n");
		}
	}

	CodeUtil codeutil;
	for (auto &item:list)	{

//		double percent_kline = mysqldayline.Get_OpenClosePercent(item.InstrumentID,tradingday_sqltime.c_str(),20);
//		printf("Check_Signal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )
		char pinstrumentid[40];
		strcpy(pinstrumentid,item.InstrumentID);
		bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,pinstrumentid,trading_date.c_str(),item.ExchangeID,20);
		bool valid_dayline_next = ValidCheck_DayLineNdays(&mysqldayline,item.InstrumentID_next,trading_date.c_str(),item.ExchangeID,20);
//		bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
//		bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item.InstrumentID_next);
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item.InstrumentID_next)> 0 ;
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID);
//			bool closetime = dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec()) ;//判断当前交易时间是否是尾盘

		if (valid_dayline &&  valid_dayline_next && valid_redisdepth  && valid_redisdepth_next && opentime )	{

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_spread");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char productid[31];
//			strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());
			strcpy(productid,item.ProductID);

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

//			DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价
//			DepthMarketData data_next=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID_next);   // 采集当天的价格 涨跌停限制价

	//				printf("	开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));

//			HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());


//			bool valid2 = depthmarket.LastPrice <=data.UpperLimitPrice && depthmarket.LastPrice >=data.LowerLimitPrice;
//			bool valid3 = depthmarket_next.LastPrice <= data_next.UpperLimitPrice &&
//					depthmarket_next.LastPrice >= data_next.LowerLimitPrice;
			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid_depthmarket_next = this->ValidCheck_DepthMarket(&depthmarket_next);

			bool valid = valid_depthmarket && valid_depthmarket_next;

			if (valid)	{
//				printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  涨幅： %.2f%% --->%s  \n",
//						pinstrumentid,depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,
//						depthmarket.LastPrice,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
				double tr =this->GetCurrentDayTR(&depthmarket);
				double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),20,tr);

				bool valid_stoprange=false;
				if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("	Strategy:stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
				}

				double section_rate=0.6;
				bool valid_section_long =false;
				bool valid_section_short =false;
				if(this->GetRedisSection()->Exists(item.Section) >0){
					SectionInfo sectioninfo;
					sectioninfo =this->GetRedisSection()->GetSection(item.Section);
					// 此处的rate 要比 正常板块 联动的要低，减少开仓次数
					if(sectioninfo.CountUprate >section_rate){
						valid_section_long=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item.Section);
					}

					if(sectioninfo.CountDownrate >section_rate){
						valid_section_short=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item.Section);
					}
				}
				int longuprate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,0);
				int shortdownrate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,1);
				bool valid_section_updownrate_long =longuprate_order<=2;
				bool valid_section_updownrate_short =shortdownrate_order<=2;

//				bool valid_globalview_long =false;
//				bool valid_globalview_short =false;
//				if(this->GetGlobalViewData()->TotalCapital >0 ){
//
//					if( this->GetGlobalViewData()->Status==0){
//						valid_globalview_long=true;
//					}
//					else if(this->GetGlobalViewData()->Status==1){
//						valid_globalview_short=true;
//					}
//					else{
//					}
//				}


//if(valid_section_long&& valid_section_updownrate_long){
//	printf("板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
//}
//if(valid_section_short&& valid_section_updownrate_short){
//	printf("板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
//
//}
				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
						&& depthmarket.UpdownRate>0.005 ;
//				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
//						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
//						&& depthmarket.AveragePrice> depthmarket.OpenPrice && depthmarket.UpdownRate>0.005 ;
				bool longcond_dif = ret_dif >0;
				bool condition_long = longcond_spread && longcond_dif && longcond_kline && !valid_section_short &&valid_section_updownrate_long;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;
				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
						&& depthmarket.UpdownRate<-0.005;
//				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
//						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
//						&& depthmarket.AveragePrice< depthmarket.OpenPrice && depthmarket.UpdownRate<-0.005;
				bool shortcond_dif = ret_dif <0;
				bool condition_short = shortcond_spread && shortcond_dif && shortcond_kline && !valid_section_long &&valid_section_updownrate_short;

//				InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);
				Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);
				double sedimentary;
				sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commission.Margin*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = sedimentary>2;

				if(longcond_spread  && !valid_section_short && valid_section_updownrate_long)	{

					if(longcond_dif){
						printf("	5-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(longcond_kline){
							printf("	5-4 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}
				else if(shortcond_spread && !valid_section_long && valid_section_updownrate_short)		{

					if(shortcond_dif){
						printf("	5-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(shortcond_kline){
							printf("	5-4 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}

				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commission.Margin);
					if(position==0){
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {

						printf("	Strategy::Open_CalendarSpreadSignal 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
												pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item.InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
					signal.Direction=0;
					signal.Offset=0;
					signal.Price=depthmarket.LastPrice;
					signal.StopRange =atr20;


					OpenSignalUpdate(&signal);

				}
				else if (condition_short  && valid_sedimentary && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commission.Margin);
					if(position==0){
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {
					printf("	Strategy::Open_CalendarSpreadSignal 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
									pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item.InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
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
						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					if(!valid_sedimentary){

						printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
									pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,commission.Margin,depthmarket.LastPrice);

						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
							pinstrumentid,sedimentary);
//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						this->GetLogUtil()->WriteLog(logbuf);
					}

				}


			}
			else{
	//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);

//				if (!valid2 )		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//				}
//				if (!valid3)		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							item.InstrumentID_next,depthmarket_next.LastPrice,data_next.UpperLimitPrice,data_next.LowerLimitPrice);
//				}
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

//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",pinstrumentid);
//			}
//
//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",item.InstrumentID_next);
//			}

			if(!valid_dayline){
				printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
			}

			if(!valid_dayline_next){
				printf("	请检查 %s 日线数据是否合规！     \n",item.InstrumentID_next);
			}
		}
	}
	vector<MainContractData>().swap(list);
	printf("StrategySpread::Open_CalendarSpreadSignal  End!!!  \n");
}
void  Strategy::Open_CalendarSpreadSignal_Average()
{
//	printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->  \n");
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double avalible ;
	 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0)	{
		 avalible=this->GetAvalible();
		 printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->	  可用资金(变): %.2f  \n",avalible);
	 }
	 else{
		 avalible = 45000.0;
		 printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	 }

	MysqlDayLine mysqldayline;
//	MysqlUtil mysqlutil(this->GetStrategyName().c_str());
	MysqlCommission mysqlcommission;
//	MysqlHold mysqlhold(this->GetUserID().c_str());

	DateUtil dateutil;
	if(this->GetTradingDay().length()<8){
		printf("StrategySpread::Open_CalendarSpreadSignal  tradingday 为空 ----------->	%ld  \n",this->GetTradingDay().length());
		return;
	}
	else{
		printf("StrategySpread::Open_CalendarSpreadSignal  tradingday----------->	 %s  \n",this->GetTradingDay().c_str());
	}
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901

	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());
	vector<MainContractData>  list;

	bool day=dateutil.CheckSHFEDayTime(this->GetDifSec());
	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());

	if(day){
		this->GetMysqlMaincontract()->Find_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else{
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[0]   \n");
		}
	}
	else if(night){
		this->GetMysqlMaincontract()->Find_Night_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else{
			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[0]   \n");
		}
	}

//	CodeUtil codeutil;
//	for(auto &item:this->GetContractLists()){
//		printf("GetContractLists----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item.ProductID, item.ExchangeID);
//	}

//	for (vector<MainContractData>::iterator item = this->GetContractLists()->begin(); item != this->GetContractLists()->end(); item++){
//		printf("GetContractLists----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item->ProductID, item->ExchangeID);
//	}

	for (auto &item:list)	{

//		double percent_kline = mysqldayline.Get_OpenClosePercent(item.InstrumentID,tradingday_sqltime.c_str(),20);
//		printf("Check_Signal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )
		char pinstrumentid[40];
		strcpy(pinstrumentid,item.InstrumentID);
		bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,pinstrumentid,trading_date.c_str(),item.ExchangeID,20);
		bool valid_dayline_next = ValidCheck_DayLineNdays(&mysqldayline,item.InstrumentID_next,trading_date.c_str(),item.ExchangeID,20);
//		bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
//		bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item.InstrumentID_next);
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item.InstrumentID_next)> 0 ;
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID);
//			bool closetime = dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec()) ;//判断当前交易时间是否是尾盘

		if (valid_dayline &&  valid_dayline_next && valid_redisdepth  && valid_redisdepth_next && opentime )	{

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_spread");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char productid[31];
//			strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());
			strcpy(productid,item.ProductID);

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

//			DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(pinstrumentid);   // 采集当天的价格 涨跌停限制价
//			DepthMarketData data_next=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID_next);   // 采集当天的价格 涨跌停限制价

	//				printf("	开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));

//			HxDayLineData predaylinedata=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,tradingday_sqltime.c_str());


//			bool valid2 = depthmarket.LastPrice <=data.UpperLimitPrice && depthmarket.LastPrice >=data.LowerLimitPrice;
//			bool valid3 = depthmarket_next.LastPrice <= data_next.UpperLimitPrice &&
//					depthmarket_next.LastPrice >= data_next.LowerLimitPrice;
			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid_depthmarket_next = this->ValidCheck_DepthMarket(&depthmarket_next);

			bool valid = valid_depthmarket && valid_depthmarket_next;

			if (valid)	{
//				printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  涨幅： %.2f%% --->%s  \n",
//						pinstrumentid,depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,
//						depthmarket.LastPrice,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
				double tr =this->GetCurrentDayTR(&depthmarket);
				double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),20,tr);

				bool valid_stoprange=false;
				if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("	Strategy:stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
				}

				double section_rate=0.6;
				bool valid_section_long =false;
				bool valid_section_short =false;
				if(this->GetRedisSection()->Exists(item.Section) >0){
					SectionInfo sectioninfo;
					sectioninfo =this->GetRedisSection()->GetSection(item.Section);
					// 此处的rate 要比 正常板块 联动的要低，减少开仓次数
					if(sectioninfo.CountUprate >section_rate){
						valid_section_long=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item.Section);
					}

					if(sectioninfo.CountDownrate >section_rate){
						valid_section_short=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item.Section);
					}
				}
				int longuprate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,0);
				int shortdownrate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,1);
				bool valid_section_updownrate_long =longuprate_order<=2;
				bool valid_section_updownrate_short =shortdownrate_order<=2;




//if(valid_section_long&& valid_section_updownrate_long){
//	printf("板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
//}
//if(valid_section_short&& valid_section_updownrate_short){
//	printf("板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
//
//}
				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
						&& depthmarket.AveragePrice> depthmarket.PreSettlementPrice && depthmarket.UpdownRate>0.005 ;
				bool longcond_dif = ret_dif >0;
				bool longsedimentaryout = depthmarket.SedimentaryIORate >-0.02;   //做多时需要考虑到资金不能流出太多,否则加大开仓风险
				bool condition_long = longcond_spread && longcond_dif && longcond_kline && !valid_section_short
						&&valid_section_updownrate_long && longsedimentaryout;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;
				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
						&& depthmarket.AveragePrice< depthmarket.PreSettlementPrice && depthmarket.UpdownRate<-0.005;
				bool shortcond_dif = ret_dif <0;
				bool condition_short = shortcond_spread && shortcond_dif && shortcond_kline && !valid_section_long &&valid_section_updownrate_short;

				Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);
//				double sedimentary;
//				sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commission.Margin*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = depthmarket.Sedimentary>2;

				if(longcond_spread  && !valid_section_short && valid_section_updownrate_long)	{

					if(longcond_dif){
						printf("	5-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(longcond_kline){
							printf("	5-4 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}
				else if(shortcond_spread && !valid_section_long && valid_section_updownrate_short)		{

					if(shortcond_dif){
						printf("	5-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(shortcond_kline){
							printf("	5-4 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}

				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commission.Margin);
					if(position==0){
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {

						printf("	Strategy::Open_CalendarSpreadSignal 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
												pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item.InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
					signal.Direction=0;
					signal.Offset=0;
					signal.Price=depthmarket.LastPrice;
					signal.StopRange =atr20;


					OpenSignalUpdate(&signal);

				}
				else if (condition_short  && valid_sedimentary && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,commission.Margin);
					if(position==0){
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {
					printf("	Strategy::Open_CalendarSpreadSignal 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
									pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item.InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
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
						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					if(!valid_sedimentary){

						printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
									pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,commission.Margin,depthmarket.LastPrice);

						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
							pinstrumentid,depthmarket.Sedimentary);
//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						this->GetLogUtil()->WriteLog(logbuf);
					}

				}


			}
			else{
	//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);

//				if (!valid2 )		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//				}
//				if (!valid3)		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							item.InstrumentID_next,depthmarket_next.LastPrice,data_next.UpperLimitPrice,data_next.LowerLimitPrice);
//				}
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

//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",pinstrumentid);
//			}
//
//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",item.InstrumentID_next);
//			}

			if(!valid_dayline){
				printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
			}

			if(!valid_dayline_next){
				printf("	请检查 %s 日线数据是否合规！     \n",item.InstrumentID_next);
			}
		}
	}
	vector<MainContractData>().swap(list);
//	printf("StrategySpread::Open_CalendarSpreadSignal  End!!!  \n");
}
void  Strategy::Open_SpreadSignal_Average()
{
	printf("StrategySpread::Open_SpreadSignal_Average  begin----------->  %ld \n",this->GetContractLists()->size());
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double avalible ;
	 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0)	{
		 avalible=this->GetAvalible();
		 printf("Strategy::Open_SpreadSignal_Average  begin----------->	  可用资金(变): %.2f  \n",avalible);
	 }
	 else{
		 avalible = 45000.0;
		 printf("Strategy::Open_SpreadSignal_Average  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	 }

	MysqlDayLine mysqldayline;

	DateUtil dateutil;
	if(this->GetTradingDay().length()<8){
		printf("Strategy::Open_SpreadSignal_Average  tradingday 为空 ----------->	%ld  \n",this->GetTradingDay().length());
		return;
	}
	else{
		printf("Strategy::Open_SpreadSignal_Average  tradingday----------->	 %s  \n",this->GetTradingDay().c_str());
	}
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901

	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());

	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());

	for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
			item != this->GetContractLists()->end(); item++){
//		printf("GetContractLists----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item->ProductID, item->ExchangeID);
//		printf("Check_Signal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )


		if(night&& strcmp(item->DayNight,"Day")==0){
			printf("	Strategy::Open_SpreadSignal_Average  >>>GetContractLists----->  无夜盘！----->ProductID:%s   exchangeid: %s \n",
					item->ProductID, item->ExchangeID);
			continue;
		}
		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);
		bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,pinstrumentid,trading_date.c_str(),item->ExchangeID,20);
		bool valid_dayline_next = ValidCheck_DayLineNdays(&mysqldayline,item->InstrumentID_next,trading_date.c_str(),item->ExchangeID,20);
//		bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
//		bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item->InstrumentID_next);
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item->InstrumentID_next)> 0 ;
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID);
//			bool closetime = dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec()) ;//判断当前交易时间是否是尾盘

		if (valid_dayline &&  valid_dayline_next && valid_redisdepth  && valid_redisdepth_next && opentime )	{

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_spread");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char productid[31];
//			strcpy(productid,codeutil.GetProductID(item->InstrumentID).c_str());
			strcpy(productid,item->ProductID);

			DepthMarket depthmarket;
			if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
			}

			DepthMarket depthmarket_next;
			if( this->GetRedisDepthMarket()->Exists(item->InstrumentID_next)> 0 ){
				depthmarket_next = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID_next);
			}

			double dif = depthmarket.LastPrice - depthmarket_next.LastPrice ;
			int ret_dif= CheckDifPriceByNextDayline(&mysqldayline,pinstrumentid,item->InstrumentID_next,20,dif);

	//				printf("	开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));
			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid_depthmarket_next = this->ValidCheck_DepthMarket(&depthmarket_next);
			bool valid = valid_depthmarket && valid_depthmarket_next;

			if (valid)	{
//				printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  涨幅： %.2f%% --->%s  \n",
//						pinstrumentid,depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,
//						depthmarket.LastPrice,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
				double tr =this->GetCurrentDayTR(&depthmarket);
				double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),20,tr);

				bool valid_stoprange=false;
				if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("	Strategy:stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
				}

				double section_rate=0.6;
				bool valid_section_long =false;
				bool valid_section_short =false;
				if(this->GetRedisSection()->Exists(item->Section) >0){
					SectionInfo sectioninfo;
					sectioninfo =this->GetRedisSection()->GetSection(item->Section);
					// 此处的rate 要比 正常板块 联动的要低，减少开仓次数
					if(sectioninfo.CountUprate >section_rate){
						valid_section_long=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item->Section);
					}

					if(sectioninfo.CountDownrate >section_rate){
						valid_section_short=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item->Section);
					}
				}
				int longuprate_order=this->GetUpDownRateOrderNumberBySection(item->InstrumentID,item->Section,0);
				int shortdownrate_order=this->GetUpDownRateOrderNumberBySection(item->InstrumentID,item->Section,1);
				bool valid_section_updownrate_long =longuprate_order<=2;
				bool valid_section_updownrate_short =shortdownrate_order<=2;

//				bool valid_globalview_long =false;
//				bool valid_globalview_short =false;
//				if(this->GetGlobalViewData()->TotalCapital >0 ){
//
//					if( this->GetGlobalViewData()->Status==0){
//						valid_globalview_long=true;
//					}
//					else if(this->GetGlobalViewData()->Status==1){
//						valid_globalview_short=true;
//					}
//					else{
//					}
//				}


//if(valid_section_long&& valid_section_updownrate_long){
//	printf("板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item->Section,longuprate_order,item->InstrumentID,depthmarket.UpdownRate);
//}
//if(valid_section_short&& valid_section_updownrate_short){
//	printf("板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item->Section,shortdownrate_order,item->InstrumentID,depthmarket.UpdownRate);
//
//}
				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline =false ;
				 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0)	{
					 longcond_kline= depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
						&& depthmarket.AveragePrice> depthmarket.PreSettlementPrice && depthmarket.UpdownRate>0.005 ;
				 }
				 else if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
									 longcond_kline= depthmarket.LastPrice > depthmarket.OpenPrice*1.003
										&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
										&& depthmarket.AveragePrice> depthmarket.PreSettlementPrice
										&&depthmarket.LastPrice >depthmarket.AveragePrice && depthmarket.UpdownRate>0.005 ;
								 }
				 else{
					 longcond_kline= depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
						&& depthmarket.UpdownRate>0.005 ;
				 }
				bool longcond_dif = ret_dif >0;
				bool longsedimentaryout = depthmarket.SedimentaryIORate >-0.02;   //做多时需要考虑到资金不能流出太多,否则加大开仓风险
				bool condition_long = longcond_spread && longcond_dif && longcond_kline && !valid_section_short
						&&valid_section_updownrate_long &&longsedimentaryout;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;

				bool shortcond_kline =false;
				 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0)	{
					 shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
							&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
							&& depthmarket.AveragePrice< depthmarket.PreSettlementPrice && depthmarket.UpdownRate<-0.005;
				 }
				 else if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
					 shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice*0.997
							&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
							&& depthmarket.AveragePrice< depthmarket.PreSettlementPrice
							&&depthmarket.LastPrice <depthmarket.AveragePrice && depthmarket.UpdownRate<-0.005;
				 }
				 else{
					 shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
						&& depthmarket.UpdownRate<-0.005;
				 }
				bool shortcond_dif = ret_dif <0;
				bool condition_short = shortcond_spread && shortcond_dif && shortcond_kline && !valid_section_long &&valid_section_updownrate_short;

				double sedimentary;
				sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*depthmarket.Margin*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = sedimentary>2;

				if(longcond_spread  && !valid_section_short && valid_section_updownrate_long)	{

					if(longcond_dif){
						printf("	5-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item->Section);
						printf("	5-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
								pinstrumentid,depthmarket.LastPrice ,item->InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item->Section,longuprate_order,item->InstrumentID,depthmarket.UpdownRate);
						if(longcond_kline){
							printf("	5-4 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}
				else if(shortcond_spread && !valid_section_long && valid_section_updownrate_short)		{

					if(shortcond_dif){
						printf("	5-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item->Section);
						printf("	5-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
								pinstrumentid,depthmarket.LastPrice ,item->InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item->Section,shortdownrate_order,item->InstrumentID,depthmarket.UpdownRate);
						if(shortcond_kline){
							printf("	5-4 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}

				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,depthmarket.Margin);
					if(position==0){
						printf("	Strategy::Open_SpreadSignal_Average--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {

						printf("	Strategy::Open_SpreadSignal_Average 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
												pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item->InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
					signal.Direction=0;
					signal.Offset=0;
					signal.Price=depthmarket.LastPrice;
					signal.StopRange =atr20;


					OpenSignalUpdate(&signal);

				}
				else if (condition_short  && valid_sedimentary && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,depthmarket.Margin);
					if(position==0){
						printf("	Strategy::Open_SpreadSignal_Average--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {
					printf("	Strategy::Open_SpreadSignal_Average 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
									pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item->InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
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
						printf("	Strategy::Open_SpreadSignal_Average 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						sprintf(logbuf,"	Strategy::Open_SpreadSignal_Average 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					if(!valid_sedimentary){

						printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
									pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,depthmarket.Margin,depthmarket.LastPrice);

						printf("	Strategy::Open_SpreadSignal_Average 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
							pinstrumentid,sedimentary);
//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						this->GetLogUtil()->WriteLog(logbuf);
					}

				}


			}
			else{
	//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);

//				if (!valid2 )		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//				}
//				if (!valid3)		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							item->InstrumentID_next,depthmarket_next.LastPrice,data_next.UpperLimitPrice,data_next.LowerLimitPrice);
//				}
			}
		}
		else{

			if(!valid_redisdepth){
				if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
					//正常情况 白盘合约在夜盘无数据
				}
				else {
					printf("	无行情数据,请检查行情是否接收正常？ %s    \n",pinstrumentid);
				}
			}

			if(!valid_redisdepth_next){

				if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",item->InstrumentID_next);
					//正常情况 白盘合约在夜盘无数据
				}
				else {
					printf("	无行情数据,请检查行情是否接收正常？ %s    \n",item->InstrumentID_next);
				}
			}

//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",pinstrumentid);
//			}
//
//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",item->InstrumentID_next);
//			}

			if(!valid_dayline){
				printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
			}

			if(!valid_dayline_next){
				printf("	请检查 %s 日线数据是否合规！     \n",item->InstrumentID_next);
			}
		}
	}

}
void  Strategy::Open_AbsMaxUpDownRate()
{
//	printf("StrategySpread::Open_AbsMaxUpDownRate  begin----------->  %ld \n",this->GetContractLists()->size());
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double avalible   = this->GetInitialMoney();
//	printf("Strategy::Open_AbsMaxUpDownRate  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	double bigvolumerate =this->GetBigVolumeRate();


	DateUtil dateutil;
	MysqlMinData m_mysqlmindata;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());

	std::string funname=">>>>>>>>>Strategy::Open_AbsMaxUpDownRate";
	if(dateutil.TodayIsHoliday()){
		printf("%s----->本交易日为假日,不运行策略!!!!!         \n",funname.c_str());
		return;
	}

	if(dateutil.IsHolidaybyNextTradingDay()){
		printf("%s----->下一个交易日为假日,本交易日不运行策略!!!!!         \n",funname.c_str());
		return;
	}

	if(dateutil.isFridayNight(this->GetDifSec())){
		printf("%s----->周五夜盘不运行策略!!!!          \n",funname.c_str());
		return;
	}

	 //控制开仓时间在开盘后5分钟再操作
//	 if(!dateutil.isNightMorningBreakTime(this->GetDifSec())){
//		printf("%s----->非策略允许交易时间!!!!          \n",funname.c_str());
//		return;
//	}

	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());


	for (vector<MainContractData>::iterator item = this->GetDataWrapper()->GetContractLists()->begin();
			item != this->GetDataWrapper()->GetContractLists()->end(); item++){

		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);

		std::string signalname;
		signalname=pinstrumentid;
		signalname.append("_absmaxupdown");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());

		char productid[31];
		strcpy(productid,item->ProductID);

		if(night&& strcmp(item->DayNight,"Day")==0){
			printf(">>>>>>>>>Strategy::Open_AbsMaxUpDownRate  >>>GetContractLists----->  无夜盘！----->ProductID:%s   exchangeid: %s \n",
					item->ProductID, item->ExchangeID);
			continue;
		}
//		std::cout<<">>>>>>>>>>>>>>>>>>>>>>>>Open_AbsMaxUpDownRate 22222222222222最大涨跌幅合约匹配:"<<pinstrumentid<<std::endl;
		GlobalViewData gbdata;
		memset(&gbdata,0,sizeof(gbdata));
		this->GetDataWrapper()->OutputGlobalViewData(&gbdata);
//		std::cout<<">>>>>>>>>>>>>>>>>>>>>>>>Open_AbsMaxUpDownRate 111111111111最大涨跌幅合约匹配  :"<<pinstrumentid<<std::endl;
		if(strcmp(gbdata.AbsMaxUpDownIns,pinstrumentid)!=0){
			continue;
		}

//////////////////////开盘时间验证///////////////////
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID);
		if (!opentime){
//			printf(">>>>>>>>>   策略验证 --> [%s]非开盘时间 !!!  \n",pinstrumentid);
			continue;
		}

//////////////////////日线数据验证///////////////////
		bool valid_dayline = this->GetDataWrapper()->FindDataStatus(pinstrumentid);

		if (!valid_dayline){
			printf(">>>>>>>>>   策略验证 --> [%s]20天日线数据时不合规!!!  \n",pinstrumentid);
			continue;
		}
//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		if (!valid_redisdepth){

			if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf(">>>>>>>>>	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
				//正常情况 白盘合约在夜盘无数据
			}
			else {
				printf(">>>>>>>>>   策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
			}

			continue;
		}


		string mindatabase="real_minute_";
		mindatabase.append(productid);

		string mindata_tablename;
		mindata_tablename=pinstrumentid;
		mindata_tablename.append("_");
		mindata_tablename.append(this->GetDataWrapper()->GetTradingDay().c_str());

		m_mysqlmindata.Set_Database(mindatabase.c_str());
		m_mysqlmindata.Set_Table(mindata_tablename.c_str());

//			printf("   database %s  :   table :%s  \n",mindatabase.c_str(),mindata_tablename.c_str());
		Mysql_MinData mysql_mindata;
		mysql_mindata = m_mysqlmindata.Find_LastDatabyMinuteTime(mindata_tablename.c_str());
		Mysql_MinData mysql_max_mindata;
		mysql_max_mindata = m_mysqlmindata.Find_MaxVolume(mindata_tablename.c_str());

		DepthMarket depthmarket;
		if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
		}
		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf(">>>>>>>>>  策略验证 --> [%s]行情数据时不合规!!!  \n",pinstrumentid);
			continue;
		}
		printf(">>>>>>>>>   合约为涨跌幅绝对值最大合约 >>> [%s]   %.2f \n",pinstrumentid,depthmarket.UpdownRate);


//				printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  涨幅： %.2f%% --->%s  \n",
//						pinstrumentid,depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,
//						depthmarket.LastPrice,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(pinstrumentid);
		double atr20=(totaltr+tr)/20;

		bool valid_stoprange=false;
		if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
			valid_stoprange=true;
		}
		else{
			valid_stoprange=false;
			printf(">>>>>>>>>	Strategy:Open_AbsMaxUpDownRate stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);

		}

		if (!valid_stoprange){

			printf(">>>>>>>>>	策略验证 -->  止损区间价格不合规 --> %s atr20:%.2f \n",	pinstrumentid,atr20);
			sprintf(logbuf,"	Strategy::Open_AbsMaxUpDownRate 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
				pinstrumentid,atr20);
			this->GetLogUtil()->WriteLog(logbuf);

			continue;
		}

		bool cond_vol = (double) mysql_mindata.Volume > (double)mysql_mindata.OpenInterest*bigvolumerate;
		bool cond_opi =  depthmarket.SedimentaryIORate >0.02;;

		//多头行情
		bool longcond_kline= depthmarket.UpdownRate>0.005 && depthmarket.LastPrice >depthmarket.OpenPrice;
		bool longcond_uprate = depthmarket.LastPrice  <depthmarket.UpperLimitPrice*0.985;
		bool condition_long =  false;



		//空头行情
		bool shortcond_kline= depthmarket.UpdownRate<-0.005 && depthmarket.LastPrice <depthmarket.OpenPrice;
		bool shortcond_downrate =depthmarket.LastPrice  >depthmarket.LowerLimitPrice*1.015;
		bool condition_short =  false;

		 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0)	{
			 condition_long =  longcond_kline  &&cond_vol &&longcond_uprate && cond_opi;
			 condition_short =  shortcond_kline   && cond_vol && shortcond_downrate && cond_opi;
		 }
		 else{
			 condition_long =  longcond_kline  &&cond_vol &&longcond_uprate;
			 condition_short =  shortcond_kline   && cond_vol && shortcond_downrate;
		 }

		 std::shared_ptr<Commission>  commssion=this->GetDataWrapper()->FindCommissionData(item->ProductID);
		double sedimentary;
		sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*commssion->Margin*depthmarket.LastPrice)/100000000.0;

		bool valid_sedimentary = sedimentary>this->GetMaxSedimentary();
		if (!valid_sedimentary){
			printf(">>>>>>>>>	策略验证 -->信号未激活  合约[%s] 沉淀资金:%.2f不符合 沉淀资金低于[%.2f]亿元,无法交易!!!  \n",pinstrumentid,
					sedimentary,this->GetMaxSedimentary());
//					printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
//							pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,commission.Margin,depthmarket.LastPrice);

//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						this->GetLogUtil()->WriteLog(logbuf);
			continue;
		}

		if (condition_long   &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByATR(pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf(">>>>>>>>>	Strategy::Open_AbsMaxUpDownRate--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
							signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
			}
			else {

				printf(">>>>>>>>>	Strategy::Open_AbsMaxUpDownRate 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
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
		else if (condition_short   && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

			int position =  this->GetPositionByATR(pinstrumentid, depthmarket,atr20,commssion->Margin);
			if(position==0){
				printf(">>>>>>>>>	Strategy::Open_AbsMaxUpDownRate--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																		signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
			}
			else {
			printf(">>>>>>>>>	Strategy::Open_AbsMaxUpDownRate 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
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

}
void  Strategy::Open_Simple()
{
	printf("Strategy::Open_Simple  begin----------->  %ld \n",this->GetContractLists()->size());
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double avalible ;
	 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0)	{
		 avalible=this->GetAvalible();
		 printf("Strategy::Open_Simple  begin----------->	  可用资金(变): %.2f  \n",avalible);
	 }
	 else{
		 avalible = 45000.0;
		 printf("Strategy::Open_Simple  begin----------->	 可用资金(固定): %.2f  \n",avalible);
	 }

	MysqlDayLine mysqldayline;

	DateUtil dateutil;
	if(this->GetTradingDay().length()<8){
		printf("Strategy::Open_Simple  tradingday 为空 ----------->	%ld  \n",this->GetTradingDay().length());
		return;
	}
	else{
		printf("Strategy::Open_Simple  tradingday----------->	 %s  \n",this->GetTradingDay().c_str());
	}
	std::string trading_date = this->GetTradingDay().c_str();   //xxxxxxxx   20170901

	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(trading_date.c_str());
	time_t tm_tradingday=dateutil.ConvertSqlTimetoTimet(tradingday_sqltime.c_str());

	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());

	for (vector<MainContractData>::iterator item = this->GetContractLists()->begin(); item != this->GetContractLists()->end(); item++){
//		printf("GetContractLists----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item->ProductID, item->ExchangeID);
//		printf("Check_Signal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s \n",item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )


		if(night&& strcmp(item->DayNight,"Day")==0){
			printf("	Strategy::Open_Simple  >>>GetContractLists----->  无夜盘！----->ProductID:%s   exchangeid: %s \n",
					item->ProductID, item->ExchangeID);
			continue;
		}
		char pinstrumentid[40];
		strcpy(pinstrumentid,item->InstrumentID);
		bool valid_dayline = ValidCheck_DayLineNdays(&mysqldayline,pinstrumentid,trading_date.c_str(),item->ExchangeID,20);
		bool valid_dayline_next = ValidCheck_DayLineNdays(&mysqldayline,item->InstrumentID_next,trading_date.c_str(),item->ExchangeID,20);
//		bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
//		bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item->InstrumentID_next);
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item->InstrumentID_next)> 0 ;
		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID);
//			bool closetime = dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec()) ;//判断当前交易时间是否是尾盘

		if (valid_dayline &&  valid_dayline_next && valid_redisdepth  && valid_redisdepth_next && opentime )	{

			std::string signalname;
			signalname=pinstrumentid;
			signalname.append("_spread");
			signalname.append("_");
			signalname.append(this->GetUserID().c_str());

			char productid[31];
//			strcpy(productid,codeutil.GetProductID(item->InstrumentID).c_str());
			strcpy(productid,item->ProductID);

			DepthMarket depthmarket;
			if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
			}

			DepthMarket depthmarket_next;
			if( this->GetRedisDepthMarket()->Exists(item->InstrumentID_next)> 0 ){
				depthmarket_next = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID_next);
			}

			double dif = depthmarket.LastPrice - depthmarket_next.LastPrice ;
			int ret_dif= CheckDifPriceByNextDayline(&mysqldayline,pinstrumentid,item->InstrumentID_next,20,dif);

	//				printf("	开仓信号检测 ProductID -->  %s  %ld   %ld \n",pinstrumentid,sizeof(pinstrumentid),sizeof(pinstrumentid));
			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid_depthmarket_next = this->ValidCheck_DepthMarket(&depthmarket_next);
			bool valid = valid_depthmarket && valid_depthmarket_next;

			if (valid)	{
//				printf("数据准备 --> %s 昨结:%.2f  开盘价:%.2f   现价:%.2f  涨幅： %.2f%% --->%s  \n",
//						pinstrumentid,depthmarket.PreSettlementPrice ,depthmarket.OpenPrice,
//						depthmarket.LastPrice,depthmarket.UpdownRate*100,depthmarket.UpdateTime );
				double tr =this->GetCurrentDayTR(&depthmarket);
				double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),20,tr);

				bool valid_stoprange=false;
				if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("	Strategy:stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
				}

				double section_rate=0.6;
				bool valid_section_long =false;
				bool valid_section_short =false;
				if(this->GetRedisSection()->Exists(item->Section) >0){
					SectionInfo sectioninfo;
					sectioninfo =this->GetRedisSection()->GetSection(item->Section);
					// 此处的rate 要比 正常板块 联动的要低，减少开仓次数
					if(sectioninfo.CountUprate >section_rate){
						valid_section_long=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item->Section);
					}

					if(sectioninfo.CountDownrate >section_rate){
						valid_section_short=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item->Section);
					}
				}
				int longuprate_order=this->GetUpDownRateOrderNumberBySection(item->InstrumentID,item->Section,0);
				int shortdownrate_order=this->GetUpDownRateOrderNumberBySection(item->InstrumentID,item->Section,1);
				bool valid_section_updownrate_long =longuprate_order<=2;
				bool valid_section_updownrate_short =shortdownrate_order<=2;

//				bool valid_globalview_long =false;
//				bool valid_globalview_short =false;
//				if(this->GetGlobalViewData()->TotalCapital >0 ){
//
//					if( this->GetGlobalViewData()->Status==0){
//						valid_globalview_long=true;
//					}
//					else if(this->GetGlobalViewData()->Status==1){
//						valid_globalview_short=true;
//					}
//					else{
//					}
//				}


//if(valid_section_long&& valid_section_updownrate_long){
//	printf("板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item->Section,longuprate_order,item->InstrumentID,depthmarket.UpdownRate);
//}
//if(valid_section_short&& valid_section_updownrate_short){
//	printf("板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item->Section,shortdownrate_order,item->InstrumentID,depthmarket.UpdownRate);
//
//}
				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline =false ;
				 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0)	{
					 longcond_kline= depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
						&& depthmarket.AveragePrice> depthmarket.PreSettlementPrice && depthmarket.UpdownRate>0.005 ;
				 }
				 else if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
									 longcond_kline= depthmarket.LastPrice > depthmarket.OpenPrice*1.003
										&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
										&& depthmarket.AveragePrice> depthmarket.PreSettlementPrice
										&&depthmarket.LastPrice >depthmarket.AveragePrice && depthmarket.UpdownRate>0.005 ;
								 }
				 else{
					 longcond_kline= depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice
						&& depthmarket.UpdownRate>0.005 ;
				 }
				bool longcond_dif = ret_dif >0;
				bool longsedimentaryout = depthmarket.SedimentaryIORate >-0.02;   //做多时需要考虑到资金不能流出太多,否则加大开仓风险
				bool condition_long =  longcond_kline ;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;

				bool shortcond_kline =false;
				 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0)	{
					 shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
							&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
							&& depthmarket.AveragePrice< depthmarket.PreSettlementPrice && depthmarket.UpdownRate<-0.005;
				 }
				 else if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
					 shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice*0.997
							&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
							&& depthmarket.AveragePrice< depthmarket.PreSettlementPrice
							&&depthmarket.LastPrice <depthmarket.AveragePrice && depthmarket.UpdownRate<-0.005;
				 }
				 else{
					 shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice
						&& depthmarket.UpdownRate<-0.005;
				 }
				bool shortcond_dif = ret_dif <0;
				bool condition_short =  shortcond_kline ;

				double sedimentary;
				sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*depthmarket.Margin*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = sedimentary>2;

//				if(longcond_spread  && !valid_section_short && valid_section_updownrate_long)	{
//
////					if(longcond_dif){
////						printf("	5-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item->Section);
////						printf("	5-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
////								pinstrumentid,depthmarket.LastPrice ,item->InstrumentID_next,depthmarket_next.LastPrice);
////						printf("	5-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
////						printf("	5-3 %s 板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
////									item->Section,longuprate_order,item->InstrumentID,depthmarket.UpdownRate);
//						if(longcond_kline){
//							printf("	5-4 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
//									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
//						}
////					}
//				}
//				else if(shortcond_spread && !valid_section_long && valid_section_updownrate_short)		{
//
////					if(shortcond_dif){
////						printf("	5-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item->Section);
////						printf("	5-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
////								pinstrumentid,depthmarket.LastPrice ,item->InstrumentID_next,depthmarket_next.LastPrice);
////						printf("	5-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
////						printf("	5-3 %s 板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
////									item->Section,shortdownrate_order,item->InstrumentID,depthmarket.UpdownRate);
//						if(shortcond_kline){
//							printf("	5-4 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
//									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
//						}
////					}
//				}

				if(longcond_kline){
					printf("	1-1 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
							pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
				}
				else if(shortcond_kline){
					printf("	1-1 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
							pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
				}

				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,depthmarket.Margin);
					if(position==0){
						printf("	Strategy::Open_Simple--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {

						printf("	Strategy::Open_Simple 开多 --> %s 现价:%.2f  --->%s 可开仓位: %d \n",
												pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item->InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
					signal.Direction=0;
					signal.Offset=0;
					signal.Price=depthmarket.LastPrice;
					signal.StopRange =atr20;


					OpenSignalUpdate(&signal);

				}
				else if (condition_short  && valid_sedimentary && valid_stoprange)		{
//						printf("买: %f  卖: %f lastprice : %f  m :%f  s:%f  stopprice: %f pricetick: %f\n",depthmarket.AskPrice1,
//								depthmarket.BidPrice1,depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);

					int position =  this->GetPositionByAvalibleATR(avalible,pinstrumentid, depthmarket,atr20,depthmarket.Margin);
					if(position==0){
						printf("	Strategy::Open_Simple--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
//						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//								signalname.c_str(),position);
//						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {
					printf("	Strategy::Open_Simple 开空 --> %s 现价:%.2f    --->%s   可开仓位: %d \n",
									pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,position);
					}
					Signal signal;
					strcpy(signal.InstrumentID,item->InstrumentID);
					strcpy(signal.SingalName,signalname.c_str());

					signal.Volume = position;
					signal.MaxSignalCount =this->GetSignalCount();
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
						printf("	Strategy::Open_Simple 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						sprintf(logbuf,"	Strategy::Open_Simple 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
							pinstrumentid,atr20);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					if(!valid_sedimentary){

						printf("	信号未激活  沉淀资金不符合--> %s OpenInterest:%.3f VolumeMultiple:%d Margin: %.2f LastPrice:%.3f  \n",
									pinstrumentid,depthmarket.OpenInterest,depthmarket.VolumeMultiple,depthmarket.Margin,depthmarket.LastPrice);

						printf("	Strategy::Open_Simple 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
							pinstrumentid,sedimentary);
//						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
//							pinstrumentid,sedimentary);
////
//						this->GetLogUtil()->WriteLog(logbuf);
					}

				}


			}
			else{
	//						printf("	行情数据不合规: %s  --> 行情数据不合规 [%.3f] 请检查 %s \n",pinstrumentid,depthmarket.LastPrice,tradingdaydata.TradingDay);

//				if (!valid2 )		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							pinstrumentid,depthmarket.LastPrice,data.UpperLimitPrice,data.LowerLimitPrice);
//				}
//				if (!valid3)		{
//					printf("	行情数据不合规: %s  --> 现价脱离涨跌停限制  现价[%.3f] 涨停[%.3f] 跌停[%.3f] \n",
//							item->InstrumentID_next,depthmarket_next.LastPrice,data_next.UpperLimitPrice,data_next.LowerLimitPrice);
//				}
			}
		}
		else{

			if(!valid_redisdepth){
				if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
					//正常情况 白盘合约在夜盘无数据
				}
				else {
					printf("	无行情数据,请检查行情是否接收正常？ %s    \n",pinstrumentid);
				}
			}

			if(!valid_redisdepth_next){

				if(strcmp(item->DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",item->InstrumentID_next);
					//正常情况 白盘合约在夜盘无数据
				}
				else {
					printf("	无行情数据,请检查行情是否接收正常？ %s    \n",item->InstrumentID_next);
				}
			}

//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",pinstrumentid);
//			}
//
//			if(!valid_mysqldepthmarket_next){
//				printf("	请检查 %s mysqldepthmarket 库是否正常更新！     \n",item->InstrumentID_next);
//			}

			if(!valid_dayline){
				printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
			}

			if(!valid_dayline_next){
				printf("	请检查 %s 日线数据是否合规！     \n",item->InstrumentID_next);
			}
		}
	}

}
void  Strategy::Open_CalendarSpread10Signal()
{
	printf("Strategy::Open_CalendarSpread10Signal  begin----------->  \n");

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());

	double avalible ;

	 if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade10")==0)	{
		 avalible=this->GetAvalible();

		 printf("	Open_CalendarSpreadSignal  可用资金(变): %.2f  \n",avalible);
	 }
	 else{
		 avalible = 45000.0;
		 printf("	Open_CalendarSpreadSignal  可用资金(固定): %.2f  \n",avalible);
	 }


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
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else
		{
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[0]   \n");
		}
	}
	else if(night){
		this->GetMysqlMaincontract()->Find_Night_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else{
				printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[0]   \n");
			}
	}


	for (auto &item:list)	{

//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日

		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )



		char pinstrumentid[40];
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
			signalname.append("_spread10");
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

//				//纯测试代码
//				this->GetPositionByAvalible(47000,item.InstrumentID, depthmarket,tr);

				bool valid_stoprange=false;
				if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("	Strategy:stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
				}

				double section_rate=0.6;
				bool valid_section_long =false;
				bool valid_section_short =false;
				if(this->GetRedisSection()->Exists(item.Section) >0){
					SectionInfo sectioninfo;
					sectioninfo =this->GetRedisSection()->GetSection(item.Section);
					// 此处的rate 要比 正常板块 联动的要低，减少开仓次数
					if(sectioninfo.CountUprate >section_rate){
						valid_section_long=true;
//						printf("板块联动上涨 ---> %s   ---->  %s --> 板块 %s   \n",pinstrumentid,productid,item.Section);
					}

					if(sectioninfo.CountDownrate >section_rate){
						valid_section_short=true;
//						printf("板块联动下跌 ---> %s   ---->  %s -->  板块 %s  \n",pinstrumentid,productid,item.Section);
					}
				}
				int longuprate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,0);
				int shortdownrate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,1);
				bool valid_section_updownrate_long =longuprate_order<=2;
				bool valid_section_updownrate_short =shortdownrate_order<=2;

				bool valid_globalview_long =false;
				bool valid_globalview_short =false;
				if(this->GetGlobalViewData()->TotalCapital >0 ){

					int allcount = this->GetGlobalViewData()->Up_Count + this->GetGlobalViewData()->Down_Count ;
					double globalview_longweight	;
					double  globalview_shortweight	 ;
					if (allcount>0){
						globalview_longweight= this->GetGlobalViewData()->Up_Count /allcount ;
						globalview_shortweight = this->GetGlobalViewData()->Down_Count /allcount ;
					}
					else {
						globalview_longweight=0.0;
						globalview_shortweight=0.0;
					}

					valid_globalview_long = globalview_longweight>0.7;
					valid_globalview_short = globalview_shortweight>0.7;

//					if(valid_globalview_long){
//						printf("整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
//							this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100);
//					}
//					else if(valid_globalview_short){
//						printf("整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
//							this->GetGlobalViewData()->Down_Count,this->GetGlobalViewData()->Up_Count,globalview_shortweight*100);
//					}
//					else{
//						printf("整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
//							this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100,globalview_shortweight*100);
//					}
				}


//if(valid_section_long&& valid_section_updownrate_long){
//	printf("板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
//}
//if(valid_section_short&& valid_section_updownrate_short){
//	printf("板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
//
//}
				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice  && depthmarket.UpdownRate>0.005;
				bool longcond_dif = ret_dif >0;
				bool condition_long = longcond_spread && longcond_dif && longcond_kline && !valid_section_short &&valid_section_updownrate_long;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;
				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice && depthmarket.UpdownRate<-0.005;
				bool shortcond_dif = ret_dif <0;
				bool condition_short = shortcond_spread && shortcond_dif && shortcond_kline && !valid_section_long &&valid_section_updownrate_short;

				InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

				double sedimentary;
				sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = sedimentary>2;

				if(longcond_spread  && !valid_section_short && valid_section_updownrate_long)	{

					if(longcond_dif){
						printf("	5-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(longcond_kline){
							printf("	5-4 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}
				else if(shortcond_spread && !valid_section_long && valid_section_updownrate_short)		{

					if(shortcond_dif){
						printf("	5-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(shortcond_kline){
							printf("	5-4 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}


				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);



					int position =  this->GetPositionByAvalible(&mysqldayline,avalible,pinstrumentid, depthmarket,tr);

					if(position==0){
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
								signalname.c_str(),position);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {

						printf("	Strategy::Open_CalendarSpreadSignal 开多 --> %s 现价:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
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
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
								signalname.c_str(),position);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {
					printf("	Strategy::Open_CalendarSpreadSignal 开空 --> %s 现价:%.2f    --->%s  空头持仓: %d 可开仓位: %d \n",
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
						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
								pinstrumentid,atr20);
						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
														pinstrumentid,atr20);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					if(!valid_sedimentary){
						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
								pinstrumentid,sedimentary);
						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
														pinstrumentid,sedimentary);
						this->GetLogUtil()->WriteLog(logbuf);
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

void  Strategy::Open_CalendarSpreadATRSignal()
{
	printf("StrategySpread::Open_CalendarSpreadSignal  begin----------->  \n");

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
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else
		{
			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[0]   \n");
		}
	}
	else if(night){
		this->GetMysqlMaincontract()->Find_Night_SpreadListorderbySed_Ndays(list,20);
		if(list.size()>0)	{
			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
		}
		else{
				printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[0]   \n");
			}
	}


	for (auto &item:list)	{

//			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
//			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日

		//判断当前交易时间是否是尾盘
//			if (dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   )

		char pinstrumentid[40];
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
			signalname.append("_spread_atr");
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
				double atr10=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,tradingday_sqltime.c_str(),10,tr);

				bool valid_stoprange=false;
				if(atr20 <=depthmarket.LastPrice*0.1 && atr20 >0.1){
					valid_stoprange=true;
				}
				else{
					valid_stoprange=false;
					printf("	Strategy:stoprange  数据异常---> %.3f   ---->  %s  \n",atr20,pinstrumentid);
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
				int longuprate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,0);
				int shortdownrate_order=this->GetUpDownRateOrderNumberBySection(item.InstrumentID,item.Section,1);
				bool valid_section_updownrate_long =longuprate_order<=2;
				bool valid_section_updownrate_short =shortdownrate_order<=2;

				bool valid_globalview_long =false;
				bool valid_globalview_short =false;
				if(this->GetGlobalViewData()->TotalCapital >0 ){

					int allcount = this->GetGlobalViewData()->Up_Count + this->GetGlobalViewData()->Down_Count ;
					double globalview_longweight	;
					double  globalview_shortweight	 ;
					if (allcount>0){
						globalview_longweight= this->GetGlobalViewData()->Up_Count /allcount ;
						globalview_shortweight = this->GetGlobalViewData()->Down_Count /allcount ;
					}
					else {
						globalview_longweight=0.0;
						globalview_shortweight=0.0;
					}

					valid_globalview_long = globalview_longweight>0.7;
					valid_globalview_short = globalview_shortweight>0.7;

//					if(valid_globalview_long){
//						printf("整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
//							this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100);
//					}
//					else if(valid_globalview_short){
//						printf("整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
//							this->GetGlobalViewData()->Down_Count,this->GetGlobalViewData()->Up_Count,globalview_shortweight*100);
//					}
//					else{
//						printf("整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
//							this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100,globalview_shortweight*100);
//					}
				}


//if(valid_section_long&& valid_section_updownrate_long){
//	printf("板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
//}
//if(valid_section_short&& valid_section_updownrate_short){
//	printf("板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",
//			item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
//
//}

				//共用条件
				bool commcondition_atr = tr >atr10 && atr10>atr20;
				//多头行情
				bool longcond_spread = depthmarket.LastPrice > depthmarket_next.LastPrice;
				bool longcond_kline = depthmarket.LastPrice > depthmarket.OpenPrice
						&& depthmarket.OpenPrice > depthmarket.PreSettlementPrice  && depthmarket.UpdownRate>0.005;
				bool longcond_dif = ret_dif >0;
				bool condition_long = longcond_spread && longcond_dif && longcond_kline  && commcondition_atr
						&& !valid_section_short &&valid_section_updownrate_long;

				//空头行情
				bool shortcond_spread =  depthmarket.LastPrice < depthmarket_next.LastPrice;
				bool shortcond_kline = depthmarket.LastPrice < depthmarket.OpenPrice
						&& depthmarket.OpenPrice < depthmarket.PreSettlementPrice && depthmarket.UpdownRate<-0.005;
				bool shortcond_dif = ret_dif <0;
				bool condition_short = shortcond_spread && shortcond_dif && shortcond_kline && commcondition_atr
						&& !valid_section_long &&valid_section_updownrate_short;

				InstrumentData insdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);

				double sedimentary;
				sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/100000000.0;

				bool valid_sedimentary = sedimentary>2;

				if(longcond_spread  && !valid_section_short && valid_section_updownrate_long)	{

					if(longcond_dif){
						printf("	5-0 %s---->%s 板块空头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1.近期合约价[%s]  %.3f  高于 远期合约价[%s]  %.3f   \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足多头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格涨幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,longuprate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(longcond_kline){
							printf("	5-4 %s 当日日k线为阳线 	现价: %.3f  开盘价: %.3f  昨收: %.3f   涨幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}
				else if(shortcond_spread && !valid_section_long && valid_section_updownrate_short)		{

					if(shortcond_dif){
						printf("	5-0 %s--->%s  板块多头未联动------> 未逆势操作[板块趋势] \n",pinstrumentid,item.Section);
						printf("	5-1近期合约价[%s]  %.3f  低于 远期合约价[%s]  %.3f  \n",
								pinstrumentid,depthmarket.LastPrice ,item.InstrumentID_next,depthmarket_next.LastPrice);
						printf("	5-2 %s 基差满足空头 %d	 \n",pinstrumentid,ret_dif);
						printf("	5-3 %s 板块价格跌幅排名前2  ---> %s   ---->[%d] %s  %.3f   \n",pinstrumentid,
									item.Section,shortdownrate_order,item.InstrumentID,depthmarket.UpdownRate);
						if(shortcond_kline){
							printf("	5-4 %s 当日日k线为阴线 	现价: %.3f  开盘价: %.3f  昨收: %.3f  跌幅: %.3f%%\n",
									pinstrumentid,depthmarket.LastPrice ,depthmarket.OpenPrice ,depthmarket.PreClosePrice,depthmarket.UpdownRate*100 );
						}
					}
				}
				double avalible = 45000.0;
				if (condition_long  && valid_sedimentary &&valid_stoprange)	{
//						printf(" 买: %f  卖: %f lastprice : %f  m :%f  s: %f  stopprice: %f  pricetick: %f\n",depthmarket.AskPrice1,depthmarket.BidPrice1,
//								depthmarket.LastPrice,m,s,stopprice,depthmarket.PriceTick);


					int position =  this->GetPositionByAvalible(&mysqldayline,avalible,pinstrumentid, depthmarket,tr);

					if(position==0){
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
														signalname.c_str(),position);
						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
								signalname.c_str(),position);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {

						printf("	Strategy::Open_CalendarSpreadSignal 开多 --> %s 现价:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
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
						printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
																				signalname.c_str(),position);
						sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
								signalname.c_str(),position);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					else {
					printf("	Strategy::Open_CalendarSpreadSignal 开空 --> %s 现价:%.2f    --->%s  空头持仓: %d 可开仓位: %d \n",
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
						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
								pinstrumentid,atr20);
						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  止损区间价格不合规 --> %s atr20:%.2f \n",
														pinstrumentid,atr20);
						this->GetLogUtil()->WriteLog(logbuf);
					}
					if(!valid_sedimentary){
						printf("	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
								pinstrumentid,sedimentary);
						sprintf(logbuf,"	Strategy::Open_CalendarSpreadSignal 信号未激活  沉淀资金不符合策略特征 --> %s 沉淀资金:%.2f \n",
														pinstrumentid,sedimentary);
						this->GetLogUtil()->WriteLog(logbuf);
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
void  Strategy::Open_MACrossSignal()
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

				char pinstrumentid[40];
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
void  Strategy::Open_CycleNSignal()
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
//
//	vector<MainContractData>  list;
//	bool day=dateutil.CheckSHFEDayTime(this->GetDifSec());
//	bool night=dateutil.CheckSHFENightTime(this->GetDifSec());
//
//	if(day){
//		this->GetMysqlMaincontract()->Find_CycleNList_Ndays(list,60);
//		if(list.size()>0)	{
//			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//		}
//		else
//		{
//			printf("Strategy::Open_CalendarSpreadSignal----->主力合约品种数[0]   \n");
//		}
//	}
//	else if(night){
//		this->GetMysqlMaincontract()->Find_Night_CycleNList_Ndays(list,60);
//		if(list.size()>0)	{
//			printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//		}
//		else{
//				printf("Strategy::Open_CalendarSpreadSignal----->夜盘主力合约品种数[0]   \n");
//			}
//	}
//
//
//	for (auto &item:list){
//
////			printf("Check_CycleSignal----->  匹配主力合约的状态----->ProductID:%s   exchangeid: %s  \n", item.ProductID, item.ExchangeID);
////			if (dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID)   )  //判断当前交易日是否是假日
//			char pinstrumentid[40];
//			strcpy(pinstrumentid,item.InstrumentID_trade);
//			bool valid_dayline = ValidCheck_DayLine(pinstrumentid,trading_date.c_str(),item.ExchangeID);
////			bool valid_dayline = ValidCheck_DayLineNdays(pinstrumentid,trading_date.c_str(),item.ExchangeID,20);
////			bool valid_dayline_next = ValidCheck_DayLineNdays(item.InstrumentID_next,trading_date.c_str(),item.ExchangeID,20);
//			bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
////			bool valid_mysqldepthmarket_next = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(item.InstrumentID_next);
//			bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
////			bool valid_redisdepth_next = this->GetRedisDepthMarket()->Exists(item.InstrumentID_next)> 0 ;
//			bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID);
//
//
//			if (valid_dayline &&   valid_mysqldepthmarket && valid_redisdepth && opentime){
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
////				bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
//				bool valid2 = depthmarket.LastPrice >=data.LowerLimitPrice;
//				bool valid3 = depthmarket.LastPrice <= data.UpperLimitPrice;
//
//				bool valid5 = atr10>0.1 && atr20 >0.1 && tr>0.1;
//				bool valid4 = this->ValidCheck_DepthMarket(&depthmarket);
//
////				bool valid6=;
//				bool valid = valid2 &&valid3 &&valid4 && valid5;
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
////						int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);
//						double avalible=47000.0;
//						int position =  this->GetPositionByAvalible(&mysqldayline,avalible,pinstrumentid, depthmarket,tr);
//
//						if(position==0){
//							printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//															signalname.c_str(),position);
//							sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//									signalname.c_str(),position);
//							this->GetLogUtil()->WriteLog(logbuf);
//						}
//						else {
//
//							printf("	Strategy::Open_CalendarSpreadSignal 开多 --> %s 现价:%.2f  --->%s  多头持仓: %d 可开仓位: %d \n",
//													pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,longposition,position);
//						}
//
//						printf("查询section_hold   %s -->  [%s]   %d \n",pinstrumentid,item.Section,section_hold);
//						bool sectionnum =this->ValidCheck_SectionPosition(item.Section);
//						if(sectionnum){
//							printf("	CycleN 开多 --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//										pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//
//							sprintf(logbuf,"	CycleN 开多 信号[%s] --> %s 现价:%.2f   区间最高价:%.2f   --->%s  多头持仓: %d 可开仓位: %d \n",
//									signalname.c_str(),pinstrumentid,depthmarket.LastPrice,cycledata.High20,depthmarket.UpdateTime ,longposition,position);
//							this->GetLogUtil()->WriteLog(logbuf);
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
//							this->GetLogUtil()->WriteLog(logbuf);
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
////						int position =  this->GetMinPosition(pinstrumentid, depthmarket,tr);
//
//						double avalible=47000.0;
//						int position =  this->GetPositionByAvalible(&mysqldayline,avalible,pinstrumentid, depthmarket,tr);
//							if(position==0){
//								printf("	Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//																						signalname.c_str(),position);
//								sprintf(logbuf,"Strategy::Open_CalendarSpreadSignal--> 出现开仓信号 %s ,基于风控最终开仓量为0!!!----> %d   \n",
//										signalname.c_str(),position);
//								this->GetLogUtil()->WriteLog(logbuf);
//							}
//							else {
//								printf("	Strategy::Open_CalendarSpreadSignal 开空 --> %s 现价:%.2f    --->%s  空头持仓: %d 可开仓位: %d \n",
//								pinstrumentid,depthmarket.LastPrice,depthmarket.UpdateTime ,shortposition,position);
//							}
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
//							this->GetLogUtil()->WriteLog(logbuf);
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
//							this->GetLogUtil()->WriteLog(logbuf);
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
//
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
//			else{
//
//				if(!valid_redisdepth){
//					if(strcmp(item.DayNight ,"Day")==0  && dateutil.CheckSHFENightTime(this->GetDifSec())) {
//	//					printf("	请检查白盘合约在夜盘无数据？: %s    \n",pinstrumentid);
//						//正常情况 白盘合约在夜盘无数据
//					}
//					else {
//						printf("	无行情数据,请检查行情是否接收正常？ %s    \n",pinstrumentid);
//					}
//
//				}
//
//
//				if(!valid_dayline){
//					printf("	请检查 %s 日线数据是否合规！     \n",pinstrumentid);
//				}
//
//			}
//	}
//	vector<MainContractData>().swap(list);

}

//在cyclen的基础上 增加 远月合约 近月合约价差 升水 贴水 信号参考
void  Strategy::Open_CycleN_NewSignal()
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
//			char pinstrumentid[40];
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
//							this->GetLogUtil()->WriteLog(logbuf);
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
//							this->GetLogUtil()->WriteLog(logbuf);
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
//							this->GetLogUtil()->WriteLog(logbuf);
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
//							this->GetLogUtil()->WriteLog(logbuf);
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


void Strategy::Hold_MA50Reverse_SignalCheck()
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
			char pinstrumentid[40];
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
