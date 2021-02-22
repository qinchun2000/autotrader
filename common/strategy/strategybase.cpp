#include "strategybase.hpp"


StrategyBase::StrategyBase() {

//	printf("StrategyBase  无参构造函数-->初始开始  \n");
	SetSignalCount(4);   //信号计数次数  n次后,激活信号
//	this->SetMysqlInstrument(&_mysqlinstrument);
	this->SetMysqlDepthMarket(&_mysqldepthmarket);
	this->SetMysqlMaincontract(&_mysqlmaincontract);
	this->SetGlobalViewData(&_globalviewdata);
//	UpdateDifSec();
//	printf("StrategyBase  无参构造函数-->初始结束   signalcount:%d \n",this->GetSignalCount());
}
StrategyBase::~StrategyBase() {

	if (m_mysqlhold != nullptr)
	{
		delete m_mysqlhold;
	}
}


std::string StrategyBase::GetUserID()
{
	return this->m_pdatawrapper->GetUserID();
}


void StrategyBase::SetGlobalViewData(GlobalViewData * data)
{
	this->_pglobalviewdata=data;
}

//void StrategyBase::CopyGlobalViewData(GlobalViewData * data)
//{
//
//	WriteGlobalViewData(data,this->_pglobalviewdata);
//}

inline void StrategyBase::WriteGlobalViewData(GlobalViewData * sourcedata,GlobalViewData * targetdata)
{

	if(sourcedata->TotalCapital>0.0 ){

		strcpy(targetdata->Time,sourcedata->Time);
		strcpy(targetdata->TradingDay,sourcedata->TradingDay);
		targetdata->TotalCapital = sourcedata->TotalCapital;
		targetdata->TotalSedimentaryIO =  sourcedata->TotalSedimentaryIO;
		targetdata->TotalSedimentaryIORate =  sourcedata->TotalSedimentaryIORate;
		targetdata->TotalSedimentaryIn = sourcedata->TotalSedimentaryIn;
		targetdata->TotalSedimentaryOut = sourcedata->TotalSedimentaryOut;
		targetdata->TotalSedimentaryInRate = sourcedata->TotalSedimentaryInRate;
		targetdata->TotalSedimentaryOutRate = sourcedata->TotalSedimentaryOutRate;

		//资金流入流出金额
		targetdata->MaxSedimentaryIO = sourcedata->MaxSedimentaryIO;
		strcpy(targetdata->MaxSedimentaryIOIns,sourcedata->MaxSedimentaryIOIns);
		targetdata->MinSedimentaryIO = sourcedata->MinSedimentaryIO;
		strcpy(targetdata->MinSedimentaryIOIns,sourcedata->MinSedimentaryIOIns);

		//资金流入流出比例
		targetdata->MaxSedimentaryIORate = sourcedata->MaxSedimentaryIORate;
		strcpy(targetdata->MaxSedimentaryIORateIns,sourcedata->MaxSedimentaryIORateIns);
		targetdata->MinSedimentaryIORate = sourcedata->MinSedimentaryIORate;
		strcpy(targetdata->MinSedimentaryIORateIns,sourcedata->MinSedimentaryIORateIns);

		targetdata->Up_Count = sourcedata->Up_Count;
		targetdata->Down_Count = sourcedata->Down_Count;
		targetdata->CurrentCount_InstrumentID = sourcedata->CurrentCount_InstrumentID;
		targetdata->LongWeight = sourcedata->LongWeight;
		targetdata->ShortWeight = sourcedata->ShortWeight;

		targetdata->Total_Up =  sourcedata->Total_Up;
		targetdata->Total_Down = sourcedata->Total_Down;
		targetdata->Average_Up = sourcedata->Average_Up;
		targetdata->Average_Down = sourcedata->Average_Down;

		targetdata->MaxUpDown = sourcedata->MaxUpDown;
		targetdata->MinUpDown = sourcedata->MinUpDown;
		strcpy(targetdata->MaxUpDownIns,sourcedata->MaxUpDownIns);
		strcpy(targetdata->MinUpDownIns,sourcedata->MinUpDownIns);
		strcpy(targetdata->AbsMaxUpDownIns,sourcedata->AbsMaxUpDownIns);
		targetdata->AbsMaxUpDown = sourcedata->AbsMaxUpDown;
		targetdata->Status = sourcedata->Status;
		targetdata->StatusChangeReason = sourcedata->StatusChangeReason;

		//板块资金流入流出
		targetdata->MaxSedimentaryIO_Section = sourcedata->MaxSedimentaryIO_Section;
		strcpy(targetdata->MaxSedIO_SectionName,sourcedata->MaxSedIO_SectionName);
		targetdata->MinSedimentaryIO_Section = sourcedata->MinSedimentaryIO_Section;
		strcpy(targetdata->MinSedIO_SectionName,sourcedata->MinSedIO_SectionName);

		//板块资金流入流出比
		targetdata->MaxSedimentaryIORate_Section = sourcedata->MaxSedimentaryIORate_Section;
		strcpy(targetdata->MaxSedIORate_SectionName,sourcedata->MaxSedIORate_SectionName);
		targetdata->MinSedimentaryIORate_Section = sourcedata->MinSedimentaryIORate_Section;
		strcpy(targetdata->MinSedIORate_SectionName,sourcedata->MinSedIORate_SectionName);
	}


}

GlobalViewData * StrategyBase::GetGlobalViewData()
{
	return this->_pglobalviewdata;
}
void StrategyBase::OutputGlobalViewData(GlobalViewData *view)
{

	WriteGlobalViewData(this->_pglobalviewdata,view);
}
void StrategyBase::InputGlobalViewData(GlobalViewData *view)
{

	WriteGlobalViewData(view,this->_pglobalviewdata);
}

void StrategyBase::SetSignalCount(int count)
{
	this->_signalcount=count;
}

int StrategyBase::GetSignalCount()
{
	return this->_signalcount;
}

void StrategyBase::SetDataWrapper(DataWrapper *point){
	this->m_pdatawrapper=point;

}
DataWrapper* StrategyBase::GetDataWrapper(){
	return this->m_pdatawrapper;
}

void StrategyBase::SetMysqlDepthMarket(MysqlDepthMarket *pmysqdepth)
{
	this->_pmysqldepthmarket=pmysqdepth;

}
MysqlDepthMarket* StrategyBase::GetMysqlDepthMarket()
{
	return this->_pmysqldepthmarket;
}

void StrategyBase::SetMysqlMaincontract(MysqlMaincontract *pmysqmc)
{
	this->_pmysqlmaincontract=pmysqmc;

}
MysqlMaincontract* StrategyBase::GetMysqlMaincontract()
{
	return this->_pmysqlmaincontract;
}
void StrategyBase::SetMysqlHold(const char* userid)
{
	this->m_mysqlhold=new MysqlHold(userid);

}
MysqlHold* StrategyBase::GetMysqlHold()
{
	return this->m_mysqlhold;
}

void StrategyBase::SetContractLists(vector<MainContractData>* p)
{
this->m_pclist = p;
}
void StrategyBase::SetContractLists()
{
//	printf("StrategyBase::SetlContractLists--->Begin  \n");
//	DateUtil dateutil;
//	this->GetMysqlMaincontract()->Find_SpreadListorderbySed_Ndays(this->_contractlist,20);
//	if(this->_contractlist.size()>0)	{
//		printf("StrategyBase::SetlContractLists----->主力合约品种数[%ld] ----> %s\n",this->_contractlist.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}
//	else{
//		printf("StrategyBase::SetlContractLists----->主力合约品种数[0]   \n");
//	}

	this->m_pclist=this->GetDataWrapper()->GetContractLists();

}
vector<MainContractData>* StrategyBase::GetContractLists(){
	return m_pclist;
}

void StrategyBase::SetMaxUpDownRate(double rate,const char* pinstrumentid)
{
	this->_max_priceupdownrate=rate;
	this->_max_updownrate_instrumentid=pinstrumentid;
}

double StrategyBase::GetMaxUpDownRate()
{
	return this->_max_priceupdownrate;
}

std::string StrategyBase::GetMaxUpDownRateInstrumentid()
{
	return this->_max_updownrate_instrumentid;
}

void StrategyBase::SetMinUpDownRate(double rate,const char* pinstrumentid)
{
	this->_min_priceupdownrate=rate;
	this->_min_updownrate_instrumentid=pinstrumentid;
}

double StrategyBase::GetMinUpDownRate()
{
	return this->_min_priceupdownrate;
}

std::string StrategyBase::GetMinUpDownRateInstrumentid()
{
	return this->_min_updownrate_instrumentid;
}

void StrategyBase::SetStopPrice(double price)
{
	 this->_stopprice=price;
}

double StrategyBase::GetStopPrice()
{
	return this->_stopprice;
}
void StrategyBase::SetStopPriceRange(double range)
{
	 this->_stoprange=range;
}

double StrategyBase::GetStopPriceRange()
{
	return this->_stoprange;
}


void StrategyBase::MarketMonitor()
{
	DateUtil dateutil;
	vector<MainContractData>  list;
	this->GetMysqlMaincontract()->Find_AllList(list);
	printf("整体市场监测----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	double totalcapital=0;
	double totalsedimentaryio=0;
	int i=0;
	double max_updown;

	std::string max_ins;
	double min_updown;
	std::string min_ins;

	int up_count=0;
	int down_count=0;
	double total_up=0.0;
	double total_down=0.0;

	double max_sedio=0.0;
	double min_sedio=0.0;
	std::string max_sedio_ins;
	std::string min_sedio_ins;

	double sed_in =0.0;
	double sed_out=0.0;

	GlobalViewData view;

	for (auto &item:list)	{

		char pinstrumentid[40];
		strcpy(pinstrumentid,item.InstrumentID);

		bool open = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,item.ProductID) ;
		bool exist = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0;

		if( open && exist ){

			DepthMarket depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
//			InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentid);

			double sedimentary;
//			sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);
	//		printf("	持仓合约到期月份监测 -->  %s 沉淀资金: %.2f \n",item.InstrumentID,sedimentary);

			totalcapital+=sedimentary;
			double sedio= depthmarket.Sedimentary -depthmarket.LastSedimentary;

			totalsedimentaryio+=sedio;

			if(sedio>0){
				sed_in+=sedio;
			}
			else if(sedio<0){
				sed_out+=sedio;
			}

			if(depthmarket.UpdownRate>0){
				total_up+=depthmarket.UpdownRate;
				up_count++;
			}
			else if(depthmarket.UpdownRate<0){
				total_down+=depthmarket.UpdownRate;
				down_count++;
			}

			if(i==0){
				max_sedio=sedio;
				max_sedio_ins=pinstrumentid;

				min_sedio=sedio;
				min_sedio_ins=pinstrumentid;
			}
			else{
				if(sedio>max_sedio)	{
					max_sedio=sedio;
					max_sedio_ins=pinstrumentid;
				}

				if(sedio<min_sedio)	{
					min_sedio=sedio;
					min_sedio_ins=pinstrumentid;
				}
			}

			if (i==0)	{
				max_updown=depthmarket.UpdownRate;
				max_ins=pinstrumentid;

				min_updown=depthmarket.UpdownRate;
				min_ins=pinstrumentid;
			}
			else{
				if(depthmarket.UpdownRate>max_updown)	{
					max_updown=depthmarket.UpdownRate;
					max_ins=pinstrumentid;
				}

				if(depthmarket.UpdownRate<min_updown)	{
					min_updown=depthmarket.UpdownRate;
					min_ins=pinstrumentid;
				}
			}
		}
		else {

		}
//		double sedimentary;
//		sedimentary = (depthmarket.OpenInterest*insdata.VolumeMultiple*insdata.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);
////		printf("	持仓合约到期月份监测 -->  %s 沉淀资金: %.2f \n",item.InstrumentID,sedimentary);
//
//		totalcapital+=sedimentary;
//		totalsedimentaryio+=depthmarket.Sedimentary -depthmarket.LastSedimentary;

		i++;
	}

	double Q_BarTime_2 = dateutil.GetCurrentSHFEBarTime(this->GetDifSec());
	bool Timemore_morning = (Q_BarTime_2>=0.0850 && Q_BarTime_2<0.1130);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1300 && Q_BarTime_2<0.1500);
	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);
	bool Timemore=( Timemore_morning  || Timemore_afternoon || Timemore_night || Timemore_night1);

	if(Timemore){


		std::string currenttime = dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec());
		strcpy(view.Time,currenttime.c_str());
		view.TotalCapital=totalcapital;
		view.TotalSedimentaryIO = totalsedimentaryio;
		view.TotalSedimentaryIn = sed_in;
		view.TotalSedimentaryOut =sed_out;

		printf("	市场整体沉淀资金 [%.2f]--------------------> 流入流出资金[%.2f]   \n",totalcapital,totalsedimentaryio);
		printf("	时间:%s  总流入资金---->  [%.2f]亿   总流出资金---->   [%.2f]亿      \n",currenttime.c_str(),
						sed_in,sed_out);

		view.MaxSedimentaryIO = max_sedio;
		strcpy(view.MaxSedimentaryIOIns,max_sedio_ins.c_str());
		view.MinSedimentaryIO=min_sedio;
		strcpy(view.MinSedimentaryIOIns,min_sedio_ins.c_str());
		printf("	时间:%s  资金流入最多合约---->   %s  [%.2f]亿   资金流出最多合约---->   %s  [%.2f]亿      \n",currenttime.c_str(),
					max_sedio_ins.c_str(),max_sedio,min_sedio_ins.c_str(),min_sedio);

		view.Up_Count=up_count;
		view.Down_Count=down_count;
		view.Total_Up =total_up;
		view.Total_Down=total_down;
		printf("	时间:%s  上涨合约数[%d]  总涨幅---->    [%.2f%%]   下跌合约数[%d] 总跌幅---->   [%.2f%%]      \n",currenttime.c_str(),
					up_count,total_up*100,down_count,total_down*100);

		if(up_count>0){
			view.Average_Up=total_up*100/(double)up_count;
		}
		else{
			view.Average_Up=0.0;
		}

		if(down_count>0){
			view.Average_Down=total_down*100/(double)down_count;
		}
		else {
			view.Average_Down=0.0;
		}


		view.MaxUpDown=max_updown;
		strcpy(view.MaxUpDownIns,max_ins.c_str());
		view.MinUpDown=min_updown;
		strcpy(view.MinUpDownIns,min_ins.c_str());
		printf("	时间:%s  上涨合约数平均涨幅---->    [%.2f%%]   下跌合约数平均跌幅---->   [%.2f%%]      \n",currenttime.c_str(),
				view.Average_Up,view.Average_Down);
		printf("	时间:%s  涨幅最高合约---->   %s  [%.2f%%]   涨幅最低合约---->   %s  [%.2f%%]      \n",currenttime.c_str(),
				max_ins.c_str(),max_updown*100,min_ins.c_str(),min_updown*100);

strcpy(view.TradingDay,this->GetTradingDay().c_str());


//		this->SetMaxUpDownRate(max_updown,max_ins.c_str());
//		this->SetMinUpDownRate(min_updown,min_ins.c_str());

		if(view.TotalCapital >0 ){
			this->InputGlobalViewData(&view);

			int allcount = this->GetGlobalViewData()->Up_Count + this->GetGlobalViewData()->Down_Count ;
			double globalview_longweight	;
			double  globalview_shortweight	 ;
			if (allcount>0){
				globalview_longweight= (double )this->GetGlobalViewData()->Up_Count /(double )allcount ;
				globalview_shortweight = (double )this->GetGlobalViewData()->Down_Count /(double )allcount ;
			}
			else {
				globalview_longweight=0.0;
				globalview_shortweight=0.0;
			}

			bool valid_globalview_long = globalview_longweight>0.7;
			bool valid_globalview_short = globalview_shortweight>0.7;

			if(valid_globalview_long){

				view.Status=0;
				printf("!!!整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
				this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100);
			}
			else if(valid_globalview_short){
				view.Status=1;
				printf("!!!整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
				this->GetGlobalViewData()->Down_Count,this->GetGlobalViewData()->Up_Count,globalview_shortweight*100);
			}
			else{
				view.Status=2;
				printf("!!!整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
				this->GetGlobalViewData()->Up_Count,this->GetGlobalViewData()->Down_Count,globalview_longweight*100,globalview_shortweight*100);
			}


//			MysqlGlobalView mysqlglobalview;
//
//			if(!mysqlglobalview.Exist_Table()){
//				mysqlglobalview.CreateTable();
//			}
//
//			if(mysqlglobalview.Exist_DataByTime(view.Time)){
//
//			}
//			else{
//				GlobalViewData lastview=mysqlglobalview.Find_LastData();
//
//				if(lastview.Status!=view.Status){
//					printf("  整体市场------> 行情转变  Status: %d ----->  %d \n",lastview.Status,view.Status);
//					mysqlglobalview.Insert(view);
//				}
//				else {
//
//				}
//			}

		}
		else{
			view.Status=2;
		}

	}

	vector<MainContractData>().swap(list);
}
void StrategyBase::SectionMarketView()
{
	MysqlSection mysqlsection;
	vector<std::string>  sectionname_lists;
	vector<SectionInfo>  section_list;
	mysqlsection.Find_SectionLists(sectionname_lists);
	for (auto &item:sectionname_lists){

		SectionInfo info;
		strcpy(info.SectionName,item.c_str());
		info.UpCount=0;
		info.DownCount=0;
		info.ZeroCount=0;
		info.TotalUprate=0;
		info.TotalDownrate=0;
		info.Maxrate=0;
		info.Minrate=1;
		info.CountUprate=0;
		info.CountDownrate=0;
		info.AverageUpDownrate=0;
		info.Sedimentary=0;
		info.SedimentaryIO=0;
		info.SedimentaryIORate=0;

//		printf("SectionCheck 板块联动监测 initial----- ----------%s \n",item.c_str());
		section_list.push_back(info);

	}

	DateUtil dateutil;

	for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
				item != this->GetContractLists()->end(); item++){

		bool open = dateutil.CheckOpenTime(this->GetDifSec(),item->ExchangeID,item->ProductID) ;
		bool exist = this->GetRedisDepthMarket()->Exists(item->InstrumentID)> 0;
		if(!exist){
			continue;
		}

		if (open   )	{

			DepthMarket depthmarket;
//				printf("SectionCheck -----%s \n",this->GetRedisDepthMarket()->GetHost().c_str());

			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item->InstrumentID);
//				printf("SectionCheck -----11111 \n");

//			InstrumentData insdata = this->GetMysqlInstrument()->Find_DatabyInstrumentID(item->InstrumentID);

//			double sedimentary;
//			sedimentary = (depthmarket.OpenInterest*depthmarket.VolumeMultiple*depthmarket.LongMarginRatio*depthmarket.LastPrice)/(100000000.0);

			for (auto &sec_item:section_list)	{
//					if (strcmp(item->Section,sec_item->SectionName)==0 && trader.CheckTradingDay())
					if (strcmp(item->Section,sec_item.SectionName)==0 )	{
//							printf("%s ---%s-->  %s 合约涨跌幅----->%.2f%% \n", item->Section,sec_item.SectionName,item->InstrumentID_trade, depthmarket.UpdownRate*100);
						if(depthmarket.UpdownRate >0 )	{
//								printf("%s----->  up: %d  上涨品种总涨幅: %.2f%% down : %d   下跌品种总跌幅 : %.2f%% 上涨品种比例:%.2f%%  下跌品种比例:%.2f%%\n",
//										section.c_str(),up,uprate*100,down,downrate*100,d_up,d_down);

							sec_item.UpCount++;
							sec_item.TotalUprate+=depthmarket.UpdownRate;
						}
						else if(depthmarket.UpdownRate <0)	{

							sec_item.DownCount++;
							sec_item.TotalDownrate+=depthmarket.UpdownRate;
						}
						else{
							sec_item.ZeroCount++;
						}

						if(depthmarket.UpdownRate>sec_item.Maxrate)	{
							sec_item.Maxrate = depthmarket.UpdownRate;
							strcpy(sec_item.MaxInstrumentID,depthmarket.InstrumentID);
						}

						if(depthmarket.UpdownRate<sec_item.Minrate)	{
							sec_item.Minrate = depthmarket.UpdownRate;
							strcpy(sec_item.MinInstrumentID,depthmarket.InstrumentID);
						}

						sec_item.Sedimentary+=depthmarket.Sedimentary;
						sec_item.SedimentaryIO += depthmarket.Sedimentary - depthmarket.LastSedimentary;
//						sec_item.SedimentaryIORate += depthmarket.SedimentaryIORate;

//						printf("%s ---%s-->  %s 合约涨跌幅----->%.2f%% \n",item.Section,sec_item.SectionName,
//								item.InstrumentID_trade, depthmarket.UpdownRate*100);
//						if(strcmp(item.Section,"Black")==0){
//
//
//						printf("\n%s ---%s-->  合约:%s   LastSedimentary:%.2f   Sedimentary:%.2f   合约SedimentaryIORate:%.2f \n",
//								item.Section,sec_item.SectionName,item.InstrumentID,
//								depthmarket.LastSedimentary,depthmarket.Sedimentary,depthmarket.SedimentaryIORate*100);
//						printf("%s ---%s-->    Sedimentary:%.2f   SedimentaryIO:%.2f   \n",
//							item.Section,sec_item.SectionName,sec_item.Sedimentary,sec_item.SedimentaryIO);
//						}
					}
			}


		}
		else{
			//redis 无数据  或者 停盘中
		}
	}


	for (auto &sec_item:section_list)	{

		double Q_BarTime_2 = dateutil.GetCurrentSHFEBarTime(this->GetDifSec());
		bool Timemore_morning = (Q_BarTime_2>=0.0859 && Q_BarTime_2<0.1015);
		bool Timemore_morning1 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
		bool Timemore_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1500);
		bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
		bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);
		bool Timemore=( Timemore_morning  || Timemore_morning1 ||Timemore_afternoon || Timemore_night || Timemore_night1);
		if(Timemore){

			int updowncount = sec_item.UpCount+sec_item.DownCount+sec_item.ZeroCount;
			sec_item.SedimentaryIORate = sec_item.SedimentaryIO/sec_item.Sedimentary;

			if(sec_item.Sedimentary>0){
				sec_item.SedimentaryIORate = sec_item.SedimentaryIO/sec_item.Sedimentary;
			}
			else{
				sec_item.SedimentaryIORate=0.0;
			}
//			printf("SectionMarketView  %s-->   Sedimentary:%.2f  SedimentaryIO:%.2f  SedimentaryIORate:%.2f%% \n",sec_item.SectionName,
//					sec_item.Sedimentary,sec_item.SedimentaryIO,sec_item.SedimentaryIORate*100);


			if(updowncount>0)	{
				sec_item.CountDownrate = (double)((double)sec_item.DownCount/((double)updowncount));
			}
			else{
				sec_item.CountDownrate=0.0;
			}
			if(updowncount>0)	{
				sec_item.CountUprate =(double)((double)sec_item.UpCount/((double)updowncount));
			}
			else{
				sec_item.CountUprate=0.0;
			}

			if(updowncount>0)	{
				sec_item.AverageUpDownrate = (sec_item.TotalDownrate +sec_item.TotalUprate)/((double)updowncount);
			}
			else{
				sec_item.AverageUpDownrate =0.0;
			}


			// section view 增加最后更新时间，确保
			if(sec_item.CountUprate>0.7){
				printf("	%s---板块联动(上涨行情)--> 上涨品种涨幅[%d]: %.2f%%  上涨品种比例:%.2f%%   SedimentaryIO:%.2f  资金流动比例: %.2f%%  平均涨跌幅: %.2f%% 最高涨跌幅:%.2f%%--> [%s]\n",
							sec_item.SectionName,sec_item.UpCount,sec_item.TotalUprate*100,sec_item.CountUprate*100,sec_item.SedimentaryIO,sec_item.SedimentaryIORate*100,sec_item.AverageUpDownrate*100,
							sec_item.Maxrate*100,sec_item.MaxInstrumentID);
//				this->GetRedisSection()->SetSection(sec_item);
			}
			else if(sec_item.CountDownrate>0.7)	{
				printf("	%s---板块联动(下跌行情)--> 下跌品种总跌幅[%d]: %.2f%%  下跌品种比例:%.2f%%  SedimentaryIO:%.2f  资金流动比例: %.2f%%  平均涨跌幅: %.2f%% 最低涨跌幅:%.2f%%--> [%s]\n",
						sec_item.SectionName,sec_item.DownCount,sec_item.TotalDownrate*100,sec_item.CountDownrate*100,sec_item.SedimentaryIO,sec_item.SedimentaryIORate*100,sec_item.AverageUpDownrate*100,
						sec_item.Minrate*100,sec_item.MinInstrumentID);
//				this->GetRedisSection()->SetSection(sec_item);
			}
			else{
				printf("	%s---板块整体走势不明晰,继续观察中----------上涨品种比例[%.2f%%]--------下跌品种比例[%.2f%%]----   SedimentaryIO:%.2f  SedimentaryIORate:%.2f%% \n",
						sec_item.SectionName,sec_item.CountUprate*100,sec_item.CountDownrate*100,sec_item.SedimentaryIO,sec_item.SedimentaryIORate*100);
			}

			if(this->GetRedisSection()->Exists(sec_item.SectionName) >0){

				this->GetRedisSection()->SetSection(sec_item);

//				SectionInfo last = this->GetRedisSection()->Get(sec_item.SectionName);
//					if(last.AverageUpDownrate >0){
//
//					}
			}
			else{
				this->GetRedisSection()->SetSection(sec_item);
			}
		}
	}

	vector<std::string>().swap(sectionname_lists);
	vector<SectionInfo>().swap(section_list);
}
void StrategyBase::Analysis(){

	char logbuf[1024];


	GlobalViewData v;
	memset(&v,0,sizeof(v));

	this->OutputGlobalViewData(&v);
	GlobalViewData *view=&v;

//	if(view->TotalSedimentaryIORate>0 && view->TotalSedimentaryIORate<0.05){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流入弱 <\n");
//	}
//	else if(view->TotalSedimentaryIORate>=0.05 && view->TotalSedimentaryIORate<0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流入小 <<< \n");
//	}
//	else if(view->TotalSedimentaryIORate>=0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流入大 <<<<<<<<<< \n");
//	}
//	else if(view->TotalSedimentaryIORate<0 && view->TotalSedimentaryIORate>-0.05){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流出弱 >\n");
//	}
//	else if(view->TotalSedimentaryIORate<=-0.05 && view->TotalSedimentaryIORate>-0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流出小 >>> \n");
//	}
//	else if(view->TotalSedimentaryIORate<=-0.1){
//		printf("GetAbsMaxUpDownIns	整体分析: 全市场资金流出大 >>>>>>>>>> \n");
//	}


	bool max_rateup_ins=strcmp(view->MaxSedimentaryIORateIns,view->MaxUpDownIns)==0;
//	bool max_ins=strcmp(view->MaxSedimentaryIORateIns,view->MaxSedimentaryIOIns)==0;
	bool max_ratelevel=view->MaxSedimentaryIORate>0.008;
	bool max_sedio=view->MaxSedimentaryIO>0.5;


	bool min_rateup_ins=strcmp(view->MinSedimentaryIORateIns,view->MinUpDownIns)==0;
//	bool min_ins=strcmp(view->MinSedimentaryIORateIns,view->MinSedimentaryIOIns)==0;
	bool min_ratelevel=view->MinSedimentaryIORate<-0.008;
	bool min_sedio=view->MinSedimentaryIO<-0.5;


	if (this->GetGlobalViewData()->CurrentCount_InstrumentID >15){

		// 0 - 第一笔初始记录
		// 1 - 多头，震荡，空头状态发生变化
		//2- 资金流入流出最大合约发生变化
		//3- 涨跌幅绝对值最大合约发生变化

		MysqlGlobalView mysqlglobalview(this->GetUserID().c_str());
		printf("GetAbsMaxUpDownIns  整体市场------> mysqlglobalview userid ----->  %s  \n",this->GetUserID().c_str());
		if(mysqlglobalview.Exist_Table() && mysqlglobalview.Count_ColumnsbyTableName()<37){
			printf("	DropSqlTable:drop数据表  mysqlglobalview !!!\n");
			mysqlglobalview.DropTable();
		}

		if(!mysqlglobalview.Exist_Table()){
			mysqlglobalview.CreateTable();
		}

		if(!mysqlglobalview.Exist_DataByTradingDay(view->TradingDay)){
			sprintf(logbuf,"  整体市场------> 并不存在本交易日任何全局市场数据  写入本交易日第一个全局数据 ----->  %s  \n",view->TradingDay);
			this->GetLogUtil()->WriteLog(logbuf);
			v.StatusChangeReason=0;
			mysqlglobalview.Insert(v);
		}

		if(!mysqlglobalview.Exist_DataByTime(view->Time)){

			GlobalViewData lastview=mysqlglobalview.Find_LastDatabyTradingDay(view->TradingDay);
			if(lastview.Status!=view->Status  ){

				if(lastview.Status==0 || view->Status==0){
					if(fabs(view->LongWeight-lastview.LongWeight)>0.03){
						sprintf(logbuf,"  整体市场------> 行情转变  Status: %d ----->  %d \n",lastview.Status,view->Status);
						this->GetLogUtil()->WriteLog(logbuf);
						v.StatusChangeReason=1;
						mysqlglobalview.Insert(v);
					}
				}
				else if(lastview.Status==1 || view->Status==1){
					if(fabs(view->ShortWeight-lastview.ShortWeight)>0.03){
						sprintf(logbuf,"  整体市场------> 行情转变  Status: %d ----->  %d \n",lastview.Status,view->Status);
						this->GetLogUtil()->WriteLog(logbuf);
						v.StatusChangeReason=1;
						mysqlglobalview.Insert(v);
					}
				}

			}
			else if(max_rateup_ins&&max_ratelevel&&max_sedio && strcmp(lastview.MaxSedimentaryIORateIns,view->MaxSedimentaryIORateIns)!=0){
//					printf("	整体分析: 多头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MaxSedimentaryIORateIns,view->MaxSedimentaryIO,
//					view->MaxSedimentaryIORate*100,view->MaxUpDown*100);
					sprintf(logbuf,"	整体分析: 多头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MaxSedimentaryIORateIns,view->MaxSedimentaryIO,
							view->MaxSedimentaryIORate*100,view->MaxUpDown*100);
					this->GetLogUtil()->WriteLog(logbuf);
					v.StatusChangeReason=2;
					mysqlglobalview.Insert(v);
				}

			else if(min_rateup_ins&&min_ratelevel&&min_sedio && strcmp(lastview.MinSedimentaryIORateIns,view->MinSedimentaryIORateIns)!=0){
//					printf("	整体分析: 空头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MinSedimentaryIORateIns,view->MinSedimentaryIO,
//					view->MinSedimentaryIORate*100,view->MinUpDown*100);
					sprintf(logbuf,"	整体分析: 空头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MinSedimentaryIORateIns,view->MinSedimentaryIO,
							view->MinSedimentaryIORate*100,view->MinUpDown*100);
					this->GetLogUtil()->WriteLog(logbuf);
					v.StatusChangeReason=2;
					mysqlglobalview.Insert(v);
				}
			else if(strcmp(lastview.AbsMaxUpDownIns,view->AbsMaxUpDownIns)!=0){

				sprintf(logbuf,"	整体分析----> 涨跌幅最大合约更新  %s >>>>>  %s  [%.4f]\n",
						lastview.AbsMaxUpDownIns,view->AbsMaxUpDownIns,view->AbsMaxUpDown);
				this->GetLogUtil()->WriteLog(logbuf);
				v.StatusChangeReason=3;
				mysqlglobalview.Insert(v);
			}
//			else if (view->MaxSedimentaryIORate > lastview.MaxSedimentaryIORate+0.01 && view->MaxSedimentaryIORate >0.07){
//				printf("  整体市场------> 单合约最大资金流入比 改变 %s MaxSedimentaryIORate: %.2f%% ----->  %s MaxSedimentaryIORate: %.2f%%  \n",
//						lastview.MaxSedimentaryIORateIns,lastview.MaxSedimentaryIORate*100,
//						view->MaxSedimentaryIORateIns,view->MaxSedimentaryIORate*100);
//				mysqlglobalview.Insert(v);
//			}
//			else if(view->MinSedimentaryIORate < lastview.MinSedimentaryIORate-0.01 && view->MinSedimentaryIORate <-0.05){
//				printf("  整体市场------> 单合约最大资金流出比 改变 %s MinSedimentaryIORate: %.2f%% ----->  %s MinSedimentaryIORate: %.2f%%  \n",
//						lastview.MinSedimentaryIORateIns,lastview.MinSedimentaryIORate*100,
//						view->MinSedimentaryIORateIns,view->MinSedimentaryIORate*100);
//				mysqlglobalview.Insert(v);
//			}
		}
	}
}

void StrategyBase::GetAbsMaxUpDownIns(){


	GlobalViewData v;
	memset(&v,0,sizeof(v));

	this->OutputGlobalViewData(&v);
	GlobalViewData *view=&v;

	if (view->CurrentCount_InstrumentID >15){

		if(fabs(view->MaxUpDown)>fabs(view->MinUpDown)){

			strcpy(view->AbsMaxUpDownIns,view->MaxUpDownIns);
			view->AbsMaxUpDown=view->MaxUpDown;
			this->InputGlobalViewData(&v);
//			printf(">>>>>>>>>>>>>>>GetAbsMaxUpDownIns多头合约优选----->%s   [%.2f] \n", view->AbsMaxUpDownIns,view->MaxUpDown);
		}
		else if(fabs(view->MaxUpDown)<fabs(view->MinUpDown)){

			strcpy(view->AbsMaxUpDownIns,view->MinUpDownIns);
			view->AbsMaxUpDown=view->MinUpDown;
			this->InputGlobalViewData(&v);
//			printf(">>>>>>>>>>>>>>>GetAbsMaxUpDownIns空头合约优选----->%s   [%.2f] \n",view->AbsMaxUpDownIns,view->MinUpDown);
		}
	}


}
void StrategyBase::StatisticsSectionSedminentaryIO(GlobalViewData *view)
{
	MysqlSection mysqlsection;
	vector<std::string>  sectionname_lists;
	vector<SectionInfo>  section_list;
	mysqlsection.Find_SectionLists(sectionname_lists);
	for (auto &item:sectionname_lists){

		SectionInfo info;
		strcpy(info.SectionName,item.c_str());
		info.UpCount=0;
		info.DownCount=0;
		info.ZeroCount=0;
		info.TotalUprate=0;
		info.TotalDownrate=0;
		info.Maxrate=0;
		info.Minrate=1;
		info.CountUprate=0;
		info.CountDownrate=0;
		info.AverageUpDownrate=0;
		info.Sedimentary=0;
		info.SedimentaryIO=0;
		info.SedimentaryIORate=0;

//		printf("SectionCheck 板块联动监测 initial----- ----------%s \n",item.c_str());
		section_list.push_back(info);

	}

	DateUtil dateutil;

	double MaxSedimentaryIO_Section=0.0;  //资金流入流出最大板块的资金量
	double MaxSedimentaryIORate_Section=0.0;
	char MaxSedIO_SectionName[40];  //资金流入流出最大板块的资金量
	char MaxSedimentaryIORate_SectionName[40];  //

	double MinSedimentaryIO_Section=0.0; //资金流入流出最小板块的资金量
	double MinSedimentaryIORate_Section=0.0;
	char MinSedIO_SectionName[40]; //资金流入流入最小板块名
	char MinSedimentaryIORate_SectionName[40];  //

	double Q_BarTime_2 = dateutil.GetCurrentSHFEBarTime(this->GetDifSec());
	bool Timemore_morning = (Q_BarTime_2>=0.0859 && Q_BarTime_2<0.1015);
	bool Timemore_morning1 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1130);
	bool Timemore_afternoon = (Q_BarTime_2>=0.1330 && Q_BarTime_2<0.1500);
	bool Timemore_night = (Q_BarTime_2>=0.20550000 && Q_BarTime_2<=0.240000);
	bool Timemore_night1 = (Q_BarTime_2>=0.0000 && Q_BarTime_2<0.0230);
	bool Timemore=( Timemore_morning  || Timemore_morning1 ||Timemore_afternoon || Timemore_night || Timemore_night1);
	if(Timemore){

		int i=0;
		for (auto &sec_item:section_list)	{

				if(this->GetRedisSection()->Exists(sec_item.SectionName) >0){

					SectionInfo last = this->GetRedisSection()->GetSection(sec_item.SectionName);
					if(i==0){
						MaxSedimentaryIO_Section = last.SedimentaryIO;
						strcpy(MaxSedIO_SectionName,sec_item.SectionName);
						MinSedimentaryIO_Section=last.SedimentaryIO;
						strcpy(MinSedIO_SectionName,sec_item.SectionName);

						MaxSedimentaryIORate_Section = last.SedimentaryIORate;
						strcpy(MaxSedimentaryIORate_SectionName,sec_item.SectionName);
						MinSedimentaryIORate_Section=last.SedimentaryIORate;
						strcpy(MinSedimentaryIORate_SectionName,sec_item.SectionName);

					}
					else {

						if(last.SedimentaryIO>MaxSedimentaryIO_Section){
							MaxSedimentaryIO_Section = last.SedimentaryIO;
							strcpy(MaxSedIO_SectionName,sec_item.SectionName);
						}

						if(last.SedimentaryIO<MinSedimentaryIO_Section){
							MinSedimentaryIO_Section=last.SedimentaryIO;
							strcpy(MinSedIO_SectionName,sec_item.SectionName);
						}

						if(last.SedimentaryIORate>MaxSedimentaryIORate_Section){
							MaxSedimentaryIORate_Section = last.SedimentaryIORate;
							strcpy(MaxSedimentaryIORate_SectionName,sec_item.SectionName);
						}

						if(last.SedimentaryIORate<MinSedimentaryIORate_Section){
							MinSedimentaryIORate_Section = last.SedimentaryIORate;
							strcpy(MinSedimentaryIORate_SectionName,sec_item.SectionName);
						}
					}
				}
			i++;
		}
		std::string currenttime = dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec());

		if(view->MaxSedimentaryIO!=0){



			strcpy(view->MaxSedIO_SectionName,MaxSedIO_SectionName);
			view->MaxSedimentaryIO_Section= MaxSedimentaryIO_Section;

			strcpy(view->MinSedIO_SectionName,MinSedIO_SectionName);
			view->MinSedimentaryIO_Section= MinSedimentaryIO_Section;

			strcpy(view->MaxSedIORate_SectionName,MaxSedimentaryIORate_SectionName);
			view->MaxSedimentaryIORate_Section=MaxSedimentaryIORate_Section;

			strcpy(view->MinSedIORate_SectionName,MinSedimentaryIORate_SectionName);
			view->MinSedimentaryIORate_Section=MinSedimentaryIORate_Section;

//			printf("	时间:%s  资金流最高板块   ---->   %s  [%.2f]   资金流最低板块       ---->   %s  [%.2f]      \n",currenttime.c_str(),
//					view->MaxSedIO_SectionName,view->MaxSedimentaryIO_Section,
//					view->MinSedIO_SectionName,view->MinSedimentaryIO_Section);

//			printf("	时间:%s  资金流入比最高板块---->   %s  [%.2f%%]   资金流入流出比最低板块---->   %s  [%.2f%%]      \n",currenttime.c_str(),
//					view->MaxSedIORate_SectionName,view->MaxSedimentaryIORate_Section*100,
//					view->MinSedIORate_SectionName,view->MinSedimentaryIORate_Section*100);
		}

	}

	vector<std::string>().swap(sectionname_lists);
	vector<SectionInfo>().swap(section_list);
}
void StrategyBase::MysqlSectionAssess(const char* opensignal)
{
	printf("Strategy::SectionAssess----->   板块级别信号评估 begin!!!  \n");

	MysqlSecAssess mysqlsecasses(this->GetUserID().c_str());
	vector<SectionAssessData> sectionlists;

	// 多空信号数 归零
	mysqlsecasses.Find_AllLists(sectionlists);
	for (auto &item:sectionlists)	{

		if(item.ProductHoldNumber>0){
			mysqlsecasses.UpdateProductHoldNumber(0,item.Section);
			printf("	Strategy::SectionAssess----->板块%s 清空持仓品种数   \n",item.Section);
		}

		if(item.PD_Long_SignalNumber>0){
			mysqlsecasses.UpdateLongProductDaySignalNumber(0,item.Section);
			printf("	Strategy::SectionAssess----->板块%s 清空多头信号   \n",item.Section);
		}

		if(item.PD_Short_SignalNumber>0){
			mysqlsecasses.UpdateShortProductDaySignalNumber(0,item.Section);
			printf("	Strategy::SectionAssess----->板块%s 清空空头信号   \n",item.Section);
		}
	}

	//更新当日的信号统计数目

	MysqlMaincontract mysqlmaincontract;
	vector<MainContractData>  contract_lists;

//	mysqlmaincontract.Find_AllList(list);
	mysqlmaincontract.Find_SpreadListorderbySed_Ndays(contract_lists,20);
	DateUtil dateutil;
	printf("	Strategy::SectionAssess----->板块级别信号评估 主力合约品种数[%ld] ----> %s\n",contract_lists.size(),
			dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());

	for (auto &item:contract_lists)	{

		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;

		if(existsignal){
//			printf("Strategy::SectionAssess----->  板块%s  :存在信号   [%s]   \n",item.Section,signalname.c_str());
			Signal signal;
			signal=this->GetRedisSignal()->GetInsSignal(signalname);
			if (signal.SingalActive ){
				printf("	Strategy::SectionAssess----->板块%s  :存在激活信号   [%s]   \n",item.Section,signalname.c_str());
				SectionAssessData data=mysqlsecasses.Find_DataBySection(item.Section);

				if(signal.Direction==0){
					data.PD_Long_SignalNumber++;

					if(data.PD_Long_SignalNumber==1){
						strcpy(data.PD_Long_InstrumentId,signal.InstrumentID);
						DepthMarket depthmarket;
						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}
						data.PD_Long_Inst_UpdownRate=depthmarket.UpdownRate;
						mysqlsecasses.UpdateLongSectionRecommend(item.Section,
								data.PD_Long_SignalNumber,data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate);
						printf("	Strategy::SectionAssess----->板块%s 存在激活多头信号  %s \n",item.Section,signal.SingalName);
					}
					else{

						DepthMarket depthmarket_last;
						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket_last = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}

						if(depthmarket_last.UpdownRate > data.PD_Long_Inst_UpdownRate){

							printf("	Strategy::SectionAssess----->板块%s 更新优选激活多头信号>>>  %s[%.3f] --> %s[%.3f]\n",item.Section,
									data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
							strcpy(data.PD_Long_InstrumentId,signal.InstrumentID);
							data.PD_Long_Inst_UpdownRate=depthmarket_last.UpdownRate;
							mysqlsecasses.UpdateLongSectionRecommend(item.Section,
									data.PD_Long_SignalNumber,data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate);

						}
					}

				}

				if(signal.Direction==1){
					data.PD_Short_SignalNumber++;
					if(data.PD_Short_SignalNumber==1){
						strcpy(data.PD_Short_InstrumentId,signal.InstrumentID);
						DepthMarket depthmarket;
						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}
						data.PD_Short_Inst_UpdownRate=depthmarket.UpdownRate;
						mysqlsecasses.UpdateShortSectionRecommend(item.Section,
								data.PD_Short_SignalNumber,data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate);
						printf("	Strategy::SectionAssess----->板块%s 存在激活空头信号>>> %s \n",item.Section,signal.SingalName);
					}
					else{
						DepthMarket depthmarket_last;
						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket_last = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}

						if(depthmarket_last.UpdownRate < data.PD_Short_Inst_UpdownRate){

							printf("	Strategy::SectionAssess----->板块%s 更新优选激活空头信号>>>  %s[%.3f] --> %s[%.3f]\n",item.Section,
									data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
							strcpy(data.PD_Short_InstrumentId,signal.InstrumentID);
							data.PD_Short_Inst_UpdownRate=depthmarket_last.UpdownRate;
							mysqlsecasses.UpdateShortSectionRecommend(item.Section,
									data.PD_Short_SignalNumber,data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate);
						}
					}


				}

			}
			else{
//				if(signal.Direction==0){
//					printf("Strategy::SectionAssess----->  板块%s  :存在未激活多头信号   [%s]   \n",item.Section,signalname.c_str());
//				}
//				if(signal.Direction==1){
//					printf("Strategy::SectionAssess----->  板块%s  :存在未激活空头信号   [%s]   \n",item.Section,signalname.c_str());
//				}
			}
		}
		else{
//			printf("Strategy::SectionAssess----->  无此信号   [%s]   \n",signalname.c_str());
		}


	}

	//评估多头信号列表  多头信号数超过阈值

	//评估空头信号列表 空头信号数超过阈值


	MysqlHold mysqlhold(this->GetUserID().c_str());
	vector<HoldData>  holdlist;
	mysqlhold.Find_AllTradingDayList(holdlist,this->GetTradingDay().c_str());
	if(holdlist.size()>0)	{
		printf("	Strategy::SectionAssess-----> ----->当日持仓批次[%ld] %s\n",holdlist.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
	}

	CodeUtil codeutil;
	for (auto &item:holdlist)	{

		std::string str_section = codeutil.GetSectionByInstrumentid(item.InstrumentID);
		SectionAssessData data=mysqlsecasses.Find_DataBySection(str_section.c_str());
		data.ProductHoldNumber++;
		mysqlsecasses.UpdateProductHoldNumber(data.ProductHoldNumber,str_section.c_str());

		printf("	Strategy::SectionAssess----->  板块%s  :更新持仓合约数 %s   [%d]   \n",str_section.c_str(),
				item.InstrumentID,data.ProductHoldNumber);
	}


	vector<MainContractData>().swap(contract_lists);
	vector<SectionAssessData>().swap(sectionlists);
	vector<HoldData>().swap(holdlist);

}


//void StrategyBase::SectionAssess(const char* opensignal)
//{
//	printf("Strategy::SectionAssess----->   板块级别信号评估 begin!!!  \n");
//
//	// 多空信号数 归零
//
//	this->GetSecAcessess()->Reset();
//
//	//更新当日的信号统计数目
//	DateUtil dateutil;
//	printf("	Strategy::SectionAssess----->板块级别信号评估 主力合约品种数[%ld] ----> %s\n",this->GetContractLists()->size(),
//			dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//
//	for (vector<MainContractData>::iterator item = this->GetContractLists()->begin();
//			item != this->GetContractLists()->end(); item++){
//
//		std::string signalname;
//		signalname=item->InstrumentID;
//		signalname.append(opensignal);
//		signalname.append("_");
//		signalname.append(this->GetUserID().c_str());
//		bool existsignal = this->GetRedisSignal()->Exists(signalname)> 0;
//
//		if(existsignal){
////			printf("Strategy::SectionAssess----->  板块%s  :存在信号   [%s]   \n",item->Section,signalname.c_str());
//			Signal signal;
//			signal=this->GetRedisSignal()->GetInsSignal(signalname);
//			if (signal.SingalActive ){
//				printf("	Strategy::SectionAssess----->板块%s  :存在激活信号   [%s]   \n",item->Section,signalname.c_str());
//				SectionAssessData data=this->GetSecAcessess()->GetSecAssessbySection(item->Section);
//
//				if(signal.Direction==0){
//					data.PD_Long_SignalNumber++;
//					this->GetSecAcessess()->UpdateSection_PD_Long_SignalNumber(item->Section,data.PD_Long_SignalNumber);
//					//该板块第一个多头信号 无须比对涨幅最高合约 直接录入即可
//					if(data.PD_Long_SignalNumber==1){
//						strcpy(data.PD_Long_InstrumentId,signal.InstrumentID);
//						DepthMarket depthmarket;
//						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
//							depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
//						}
//						data.PD_Long_Inst_UpdownRate=depthmarket.UpdownRate;
//						this->GetSecAcessess()->UpdateSection_PD_Long(item->Section,data);
//						printf("	Strategy::SectionAssess----->板块%s 存在激活多头信号  %s \n",item->Section,signal.SingalName);
//					}
//					else{   // 该板块出现多个多头信号  则需要比对再更新涨跌幅最高合约
//
//						DepthMarket depthmarket_last;
//						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
//							depthmarket_last = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
//						}
//
//						if(depthmarket_last.UpdownRate > data.PD_Long_Inst_UpdownRate){
//
//							printf("	Strategy::SectionAssess----->板块%s 更新优选激活多头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
//									data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
//							strcpy(data.PD_Long_InstrumentId,signal.InstrumentID);
//							data.PD_Long_Inst_UpdownRate=depthmarket_last.UpdownRate;
//							this->GetSecAcessess()->UpdateSection_PD_Long(item->Section,data);
//						}
//					}
//
//				}
//
//				if(signal.Direction==1){
//					data.PD_Short_SignalNumber++;
//					this->GetSecAcessess()->UpdateSection_PD_Long_SignalNumber(item->Section,data.PD_Long_SignalNumber);
//					if(data.PD_Short_SignalNumber==1){
//						strcpy(data.PD_Short_InstrumentId,signal.InstrumentID);
//						DepthMarket depthmarket;
//						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
//							depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
//						}
//						data.PD_Short_Inst_UpdownRate=depthmarket.UpdownRate;
//						this->GetSecAcessess()->UpdateSection_PD_Short(item->Section,data);
//						printf("	Strategy::SectionAssess----->板块%s 存在激活空头信号>>> %s \n",item->Section,signal.SingalName);
//					}
//					else{
//						DepthMarket depthmarket_last;
//						if( this->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
//							depthmarket_last = this->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
//						}
//
//						if(depthmarket_last.UpdownRate < data.PD_Short_Inst_UpdownRate){
//
//							printf("	Strategy::SectionAssess----->板块%s 更新优选激活空头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
//									data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
//							strcpy(data.PD_Short_InstrumentId,signal.InstrumentID);
//							data.PD_Short_Inst_UpdownRate=depthmarket_last.UpdownRate;
//							this->GetSecAcessess()->UpdateSection_PD_Short(item->Section,data);
//						}
//					}
//				}
//
//			}
//			else{
////				if(signal.Direction==0){
////					printf("Strategy::SectionAssess----->  板块%s  :存在未激活多头信号   [%s]   \n",item->Section,signalname.c_str());
////				}
////				if(signal.Direction==1){
////					printf("Strategy::SectionAssess----->  板块%s  :存在未激活空头信号   [%s]   \n",item->Section,signalname.c_str());
////				}
//			}
//		}
//		else{
////			printf("Strategy::SectionAssess----->  无此信号   [%s]   \n",signalname.c_str());
//		}
//
//
//	}
//
//	//评估多头信号列表  多头信号数超过阈值
//
//	//评估空头信号列表 空头信号数超过阈值
//
//
//	MysqlHold mysqlhold(this->GetUserID().c_str());
//	vector<HoldData>  holdlist;
//	mysqlhold.Find_AllTradingDayList(holdlist,this->GetTradingDay().c_str());
//	if(holdlist.size()>0)	{
//		printf("	Strategy::SectionAssess-----> ----->当日持仓批次[%ld] %s\n",holdlist.size(),
//				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}
//
//	CodeUtil codeutil;
//	for (auto &item:holdlist)	{
//
//		std::string str_section = codeutil.GetSectionByInstrumentid(item.InstrumentID);
//
//		SectionAssessData data=this->GetSecAcessess()->GetSecAssessbySection(str_section.c_str());
//		data.ProductHoldNumber++;
//		this->GetSecAcessess()->UpdateSection_ProductHoldNumber(str_section.c_str(),data.ProductHoldNumber);
//		printf("	Strategy::SectionAssess----->  板块%s  :更新持仓合约数 %s   [%d]   \n",str_section.c_str(),
//				item.InstrumentID,data.ProductHoldNumber);
//	}
//
//	vector<HoldData>().swap(holdlist);
//
//}
int StrategyBase::Valid_StaBaseDataCheck(const char* pinstrumentid,MysqlDayLine *mysqldayline,
		const char* tradingdate,HoldData *holddata){
	int ret=100;
	DateUtil dateutil;

	char productid[31];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	bool valid_dayline = ValidCheck_DayLineNdays(mysqldayline,pinstrumentid,tradingdate,holddata->ExchangeID,20);
	bool valid_mysqldepthmarket = this->GetMysqlDepthMarket()->Exist_DatabyInstrumentID(pinstrumentid);
	bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
	bool valid_opentime = dateutil.CheckOpenTime(this->GetDifSec(),holddata->ExchangeID,productid);
//	bool valid_endtime =dateutil.CheckSHFEEndCloseMarketTime(this->GetDifSec())   //判断当前交易时间是否是尾盘
	if(valid_dayline && valid_mysqldepthmarket && valid_redisdepth &&valid_opentime){
			ret=0;
		}
		else{

			if(!valid_dayline){
			printf("	  %s --------> 日线数据检测非法 直接跳出! \n",pinstrumentid);
				ret=1;
			}
			else if(!valid_mysqldepthmarket ){
				printf("	%s -------->mysqldepthmarket 数据非法  直接跳出! \n",pinstrumentid);
				ret=2;
			}
			else if(!valid_redisdepth ){
				printf("	%s -------->redis行情数据非法  直接跳出! \n",pinstrumentid);
				ret=3;
			}
			else if(!valid_opentime ){
//			printf("	  %s --------> 非开盘时间 直接跳出! \n",pinstrumentid);
				ret=4;
			}
			else {
				ret=100;  //未知错误
			}

		}

	return ret;
}
bool StrategyBase::ValidCheck_DayLine(const char* pinstrumentid,const char* tradingdate,const char* exchangeid){

//	printf("ValidCheck_DayLine [%s]   交易日[%s]     exchange: %s \n",pinstrumentid,tradingdate,exchangeid);
	bool ret=false;
	MysqlDayLine mysqldayline;
	MysqlUtil mysqlutil(this->GetStrategyName().c_str());
	DateUtil dateutil;
	std::string str_sqltradingday = dateutil.ConvertDatetoSqlTime(tradingdate);

	bool valid5;
	int count;

	if(mysqldayline.Exist_Table(pinstrumentid))	{
		count=mysqldayline.Count_AllListsbyTableName(pinstrumentid);
		if(count==0)		{
			printf("ValidCheck_DayLine [%s]   日线无数据,需要更新     exchange: %s \n",pinstrumentid,exchangeid);

			count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
		}
	}
	else	{
		printf("ValidCheck_DayLine [%s]   不存在日线数据表     exchange: %s \n",pinstrumentid,exchangeid);
		mysqldayline.CreateTable(pinstrumentid);

		count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	}

	std::string dayline_lastdate =mysqldayline.GetLastDateTime(pinstrumentid);
	time_t tm_lastdate=dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());

	time_t tm_enddayline =  dateutil.CheckHisLastDayLinebyDate_ExcludeHoliday(tradingdate,this->GetDifSec());

	time_t tm_tradingday;
	tm_tradingday=dateutil.ConvertSqlTimetoTimet(str_sqltradingday.c_str());

	//				printf("	dayline  ------>tm_yesterday [%s]: %ld    tm_dayline [%s]: %ld     \n  ",
	//												dayline_lastdate.c_str(),tm_yesterday,tradingday.c_str(),tm_dayline);

	if (tm_lastdate ==tm_enddayline)	{
		valid5 =true;
	}
	else	{
		printf("ValidCheck_DayLine  [%s]-----> 补齐数据 尝试   \n",pinstrumentid);

		std::string dayline_lastdate_new =mysqldayline.GetLastDateTime(pinstrumentid);
		time_t tm_lastdate_new=dateutil.ConvertSqlTimetoTimet(dayline_lastdate_new.c_str());
		if (tm_lastdate_new ==tm_enddayline){
				valid5 =true;
		}
		else if(tm_lastdate_new >tm_enddayline){
			valid5 =true;
			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
			printf("ValidCheck_DayLine[%s]-->	dayline 数据大于截止日期 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
								str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);
		}
		else{
			valid5=false;
			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
			printf("ValidCheck_DayLine[%s]-->	dayline 数据缺失 截止日期不相同 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
					str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);

			if (this->GetMysqlMaincontract()->Exist_DatabyInstrumentID(pinstrumentid)){
//				MainContractData mcdata;
//				mcdata = this->GetMysqlMaincontract()->Find_DatabyInstrumentID(pinstrumentid);
//				mcdata.ErrorDaylineCount=1;

				this->GetMysqlMaincontract()->UpdateErrorDaylineCount(1,pinstrumentid);

				printf("ValidCheck_DayLineNdays[%s]-->	更新数据错误的主力合约   \n",pinstrumentid);
			}

			else if (this->GetMysqlMaincontract()->Exist_DatabyInstrumentIDNext(pinstrumentid)){
//				MainContractData mcdata;
//				mcdata = this->GetMysqlMaincontract()->Find_DatabyInstrumentIDNext(pinstrumentid);
//				mcdata.ErrorDaylineCount=1;

				this->GetMysqlMaincontract()->UpdateErrorDaylineCountNext(1,pinstrumentid);
				printf("ValidCheck_DayLineNdays[%s]-->	更新数据错误的远月合约   \n",pinstrumentid);
			}
			else{
				printf("ValidCheck_DayLineNdays[%s]-->	数据错误的合约  未设置标识，策略轮询无法终止! \n",pinstrumentid);
			}

		}
	}


	bool valid0 = count > 59;

	bool b_holiday = dateutil.CheckHoliday_SqlTime(str_sqltradingday.c_str());   //判断当前交易日是否是假日

	ret = valid0 && valid5 && !b_holiday;

	if (!ret)	{
		if (b_holiday)		{
			printf("	当前交易日未假日: %s  --> 休市日 %s \n",pinstrumentid,str_sqltradingday.c_str());
		}

		if (!valid0)			{
				printf("	行情数据不合规: %s  --> 日线数据长度[%d]不足  \n",pinstrumentid,count);

		}

		if (!valid5)			{
			printf("	历史数据不合规: %s  --> 日线数据缺失 最后更新日期[%s]  \n",pinstrumentid,dayline_lastdate.c_str());
		}
	}


	return ret;
}
//bool StrategyBase::ValidCheck_DayLineNdays(const char* pinstrumentid,const char* tradingdate,const char* exchangeid,int N){
//
////	printf("ValidCheck_DayLine [%s]   交易日[%s]     exchange: %s \n",pinstrumentid,tradingdate,exchangeid);
//	bool ret=false;
//	MysqlDayLine mysqldayline;
//	MysqlUtil mysqlutil(this->GetStrategyName().c_str());
//	DateUtil dateutil;
//	std::string str_sqltradingday = dateutil.ConvertDatetoSqlTime(tradingdate);
//
//	bool valid5;
//	int count;
//
//	if(mysqldayline.Exist_Table(pinstrumentid))	{
//		count=mysqldayline.Count_AllListsbyTableName(pinstrumentid);
//		if(count==0)		{
//			printf("ValidCheck_DayLineNdays [%s]   日线无数据,需要更新     exchange: %s \n",pinstrumentid,exchangeid);
//			mysqldayline.CheckHttpDayLine(pinstrumentid,exchangeid);
//			mysqlutil.CorrectDayLineByInstrument(pinstrumentid);
//			count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
//		}
//	}
//	else	{
//		printf("ValidCheck_DayLineNdays [%s]   不存在日线数据表     exchange: %s \n",pinstrumentid,exchangeid);
//		mysqldayline.CreateTable(pinstrumentid);
//		mysqldayline.CheckHttpDayLine(pinstrumentid,exchangeid);
//		mysqlutil.CorrectDayLineByInstrument(pinstrumentid);
//		count =mysqldayline.Count_AllListsbyTableName(pinstrumentid);
//	}
//
//	std::string dayline_lastdate =mysqldayline.GetLastDateTime(pinstrumentid);
//	time_t tm_lastdate=dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());
//
//	time_t tm_enddayline =  dateutil.CheckHisLastDayLinebyDate_ExcludeHoliday(tradingdate,this->GetDifSec());
//
//	time_t tm_tradingday;
//	tm_tradingday=dateutil.ConvertSqlTimetoTimet(str_sqltradingday.c_str());
//
//	//				printf("	dayline  ------>tm_yesterday [%s]: %ld    tm_dayline [%s]: %ld     \n  ",
//	//												dayline_lastdate.c_str(),tm_yesterday,tradingday.c_str(),tm_dayline);
//
//	if (tm_lastdate ==tm_enddayline)	{
//		valid5 =true;
//	}
//	else	{
//		printf("ValidCheck_DayLineNdays  [%s]-----> 补齐数据 尝试   \n",pinstrumentid);
//		mysqldayline.CheckHttpDayLine(pinstrumentid,exchangeid);
//		mysqlutil.CorrectDayLineByInstrument(pinstrumentid);
//		std::string dayline_lastdate_new =mysqldayline.GetLastDateTime(pinstrumentid);
//		time_t tm_lastdate_new=dateutil.ConvertSqlTimetoTimet(dayline_lastdate_new.c_str());
//		if (tm_lastdate_new ==tm_enddayline){
//				valid5 =true;
//		}
//		else if(tm_lastdate_new >tm_enddayline){
//			valid5 =true;
//			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
//			printf("ValidCheck_DayLineNdays[%s]-->	dayline 数据大于截止日期 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
//								str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);
//		}
//		else{
//			valid5=false;
//			std::string str_end =dateutil.ConvertTimetoSqlString(tm_enddayline);
//			printf("ValidCheck_DayLineNdays[%s]-->	dayline 数据缺失 截止日期不相同 ------>日线最后数据日 [%s]: %ld    截止日期 [%s]: %ld     \n  ",
//					str_sqltradingday.c_str(),dayline_lastdate.c_str(),tm_lastdate,str_end.c_str(),tm_enddayline);
////
////			if (this->GetMysqlMaincontract()->Exist_DatabyInstrumentID(pinstrumentid)){
//////				MainContractData mcdata;
//////				mcdata = this->GetMysqlMaincontract()->Find_DatabyInstrumentID(pinstrumentid);
//////				mcdata.ErrorDaylineCount=1;
////
////				this->GetMysqlMaincontract()->UpdateErrorDaylineCount(1,pinstrumentid);
////
////				printf("ValidCheck_DayLineNdays[%s]-->	更新数据错误的主力合约   \n",pinstrumentid);
////			}
////
////			else if (this->GetMysqlMaincontract()->Exist_DatabyInstrumentIDNext(pinstrumentid)){
//////				MainContractData mcdata;
//////				mcdata = this->GetMysqlMaincontract()->Find_DatabyInstrumentIDNext(pinstrumentid);
//////				mcdata.ErrorDaylineCount=1;
////
////				this->GetMysqlMaincontract()->UpdateErrorDaylineCountNext(1,pinstrumentid);
////				printf("ValidCheck_DayLineNdays[%s]-->	更新数据错误的远月合约   \n",pinstrumentid);
////			}
////			else{
////				printf("ValidCheck_DayLineNdays[%s]-->	数据错误的合约  未设置标识，策略轮询无法终止! \n",pinstrumentid);
////			}
//		}
//	}
//
//
//	bool valid0 = count > N;
//
//	bool b_holiday = dateutil.CheckHoliday_SqlTime(str_sqltradingday.c_str());   //判断当前交易日是否是假日
//
//	ret = valid0 && valid5 && !b_holiday;
//
//	if (!ret)	{
//		if (b_holiday)		{
//			printf("	当前交易日未假日: %s  --> 休市日 %s \n",pinstrumentid,str_sqltradingday.c_str());
//		}
//
//		if (!valid0)			{
//				printf("	行情数据不合规: %s  --> 日线数据长度[%d]不足 %d \n",pinstrumentid,count,N);
//
//		}
//
//		if (!valid5)			{
//			printf("	历史数据不合规: %s  --> 日线数据缺失 最后更新日期[%s]  \n",pinstrumentid,dayline_lastdate.c_str());
//		}
//	}
//
//
//	return ret;
//}
bool StrategyBase::ValidCheck_SectionPosition(const char*section)
{
	CodeUtil codeutil;
	bool sectionnum =false;
	vector<HoldData> lists;

	MysqlHold mysqlhold(this->GetUserID().c_str());
//	int section_hold = mysqlhold.Count_AllListsbySection(section);
	int section_hold=0;

	mysqlhold.Find_AllList(lists);
	for (auto &item:lists)	{

		char productid[31];
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());

		std::string sec =codeutil.GetSectionByProductid(productid);
		if(strcmp(sec.c_str(),section)==0){

//			DepthMarket depthmarket;
//			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
//			double profit;
			//如果同板块的合约 未锁定盈利 则增加板块持仓计数
			if(item.Direction==0 && item.StopPrice < item.OpenPriceAverage) {
				section_hold++;
			}
			else if(item.Direction==1 && item.StopPrice > item.OpenPriceAverage){
				section_hold++;
			}


		}



	}



	if(strcmp(section,"Black")){
		sectionnum= section_hold < 2;
	}
	if(strcmp(section,"Oil")){
		sectionnum= section_hold < 2;
	}
	if(strcmp(section,"Metal")){
		sectionnum= section_hold < 0;
	}
	if(strcmp(section,"Single")){
		sectionnum= section_hold < 1;
	}
	if(strcmp(section,"CookOil")){
		sectionnum= section_hold < 1;
	}
	if(strcmp(section,"Feed")){
		sectionnum= section_hold < 1;
	}
	if(strcmp(section,"Corn")){
		sectionnum= section_hold < 1;
	}
	else{
		sectionnum= section_hold < 1;
	}

	vector<HoldData>().swap(lists);
	return sectionnum;
}

int  StrategyBase::GetUpDownRateOrderNumberBySection(const char* pinstrumentid,const char* section,int direction){

	DepthMarket depthmarket_ins;

	if( this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ){
		depthmarket_ins = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);	}

	vector<MainContractData>  list;
	this->GetMysqlMaincontract()->Find_ListsbySection(list,section);
	int ordernumber=list.size();
	for (auto &item:list)	{

		if(strcmp(pinstrumentid,item.InstrumentID)!=0){
			DepthMarket depthmarket;

			if( this->GetRedisDepthMarket()->Exists(item.InstrumentID)> 0 ){
				depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
				//多头
				if(direction==0){
					if (depthmarket_ins.UpdownRate >depthmarket.UpdownRate ){
						ordernumber--;
//						printf("StrategyBase::GetUpRateOrderNumberBySection   ----> 多头方向 涨幅递增  %s  %d  %.3f [%s %.3f]\n",
//								pinstrumentid,ordernumber,depthmarket_ins.UpdownRate,item.InstrumentID,depthmarket.UpdownRate);
					}
				}
				else if(direction==1){
					if (depthmarket_ins.UpdownRate <depthmarket.UpdownRate ){
						ordernumber--;
//						printf("StrategyBase::GetUpRateOrderNumberBySection   ----> 空头方向 跌幅递增  %s  %d  %.3f [%s %.3f]\n",
//								pinstrumentid,ordernumber,depthmarket_ins.UpdownRate,item.InstrumentID,depthmarket.UpdownRate);
					}
				}

			}
		}



	}
	vector<MainContractData>().swap(list);

	return ordernumber;
}
// 可以根据需要 定制不同的止损价---> 以开仓价为基准,进行计算
double  StrategyBase::GetStopPrice(const char* pinstrumentid,int direction,double openprice){

//	InstrumentData instrumentdata;

	double atr=this->GetATRbyTradingDay(pinstrumentid);
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(pinstrumentid);

	double stopprice=0;

	if(direction==0)	{
		double m=0;
		double s=0;
		s = openprice - atr*this->GetStopRate();
//		m =fmod(s,instrumentdata.PriceTick);
		stopprice = s;
	}
	else if (direction ==1)	{
		double m;
		double s;
		s = openprice + atr*this->GetStopRate();
//		m =fmod(s,instrumentdata.PriceTick);
		stopprice = s;
	}

	return stopprice;

}

double StrategyBase::GetCurrentDayTR(DepthMarket *pdepthmarket){
	double tr=0.0;
	double tr_lowclose = abs(pdepthmarket->LowestPrice-pdepthmarket->PreClosePrice);
	double tr_highclose =abs(pdepthmarket->HighestPrice-pdepthmarket->PreClosePrice);
	double tr_highlow = abs(pdepthmarket->HighestPrice-pdepthmarket->LowestPrice);
	tr=max(tr_highlow,max(tr_lowclose,tr_highclose));
	return tr;
}
//按照可用资金计算仓位
int  StrategyBase::GetPositionByAvalibleATR(double avalibe,const char* pinstrumentid,
		DepthMarket depthmarket,double atr20,double margin)
{
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double ratio=this->GetStopRate();
	double singlemargin_limit = this->GetSingleMaxMargin();
	int position=0;

	if (atr20 >1.0 )	{

		int p_stop;
		double range;

		range = fabs(atr20*ratio );   // 每次损失控制在1.5倍的atr;

		p_stop=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;
		double stop_marginused = (double)p_stop*depthmarket.LastPrice* depthmarket.VolumeMultiple*margin;

		double single_marginused = depthmarket.LastPrice* depthmarket.VolumeMultiple*margin;

		int p_margin = (avalibe*singlemargin_limit)/( single_marginused);
		double pmargin_marginused = (double)p_margin*depthmarket.LastPrice* depthmarket.VolumeMultiple*margin;
//			int p_margin = (avalibe*singlemargin)/( depthmarket.LastPrice* depthmarket.VolumeMultiple*commission.Margin);

		position = min(p_stop,p_margin) ;
		double positon_marginused = (double)position*depthmarket.LastPrice* depthmarket.VolumeMultiple*margin;

//		printf("Strategy::GetPositionByAvalibleATR -> %s   avalibe:%.2f  riskrate: %.2f -------可忍受亏损额度: %.2f  \n",
//				pinstrumentid,avalibe,this->GetRiskRate(),avalibe*this->GetRiskRate());
//		printf("Strategy::GetPositionByAvalibleATR -> %s   atr  :%.3f ratio: %.2f  ----------止损价差 : %.3f \n",
//				pinstrumentid,atr20, ratio,range);
//		printf("Strategy::GetPositionByAvalibleATR ->固定亏损算法 %s    现价: %.3f  交易单位: %d  单合约亏损额度：%.2f ------>开仓数量: %d  保证金:%.3f [%.3f%%] \n",
//				pinstrumentid,depthmarket.LastPrice, depthmarket.VolumeMultiple, depthmarket.VolumeMultiple*range,p_stop,stop_marginused,stop_marginused/avalibe*100);
////
////					printf("Strategy::GetPositionByAvalible ->固定亏损算法 %s    现价: %.3f  交易单位: %d  单合约亏损额度：%.2f ------>开仓数量: %d   \n",
////							pinstrumentid,depthmarket.LastPrice, depthmarket.VolumeMultiple, depthmarket.VolumeMultiple*range,p_stop);
//
//		printf("Strategy::GetPositionByAvalibleATR ->固定保证金比例 %s   单次开仓保证金限制 %.2f  期货公司保证金比例: %.3f -->仓位: %d  保证金:%.3f [%.3f%%]  \n",
//							pinstrumentid,singlemargin_limit, margin,p_margin,pmargin_marginused,pmargin_marginused/avalibe*100);
////			printf("Strategy::GetPositionByAvalible--> 最终仓位 : %d    \n",position);
//		printf("Strategy::GetPositionByAvalibleATR--> 最终仓位 : %d   单手开仓保证金: %.3f [%.3f%%]  最终保证金:%.3f [%.3f%%] \n",
//				position,single_marginused,single_marginused/avalibe*100,positon_marginused,positon_marginused/avalibe*100);

//		sprintf(logbuf,"Strategy::GetPositionByAvalibleATR--> %s 最终仓位 : %d   现价: %.3f 单手开仓保证金比例: %.3f  最终保证金:%.3f [%.3f%%] \n",
//				pinstrumentid,position,depthmarket.LastPrice,single_marginused,positon_marginused,positon_marginused/avalibe*100);
//
////					sprintf(logbuf,"Strategy::GetPositionByAvalible--> --> %s 最终仓位 : %d    \n",pinstrumentid,position);
//		this->GetLogUtil()->WriteLog(logbuf);
	}
	else {
		position =0;
	}

	return position;

}
//按照可用资金计算仓位
int  StrategyBase::GetPositionByATR(const char* pinstrumentid,DepthMarket depthmarket,double atr20,double margin)
{
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName());
	double ratio=this->GetStopRate();
	double singlemargin_limit = this->GetSingleMaxMargin();
	int position=0;
	double avalibe=this->GetInitialMoney();

	if (atr20 >1.0 )	{


		double 	range = fabs(atr20*ratio );   // 每次损失控制在1.5倍的atr;

		position=  avalibe* this->GetRiskRate()/(range * depthmarket.VolumeMultiple ) ;


	}
	else {
		position =0;
	}

	return position;

}

void StrategyBase::DayTrade_EndTimeClose_SignalCheck()
{
	char logbuf[1024];
	DateUtil dateutil;

	vector<HoldData>  list;
	this->GetDataWrapper()->GetHoldList(list);
//	if(list.size()>0)	{
		printf(">>>>>>>>>>>>>>持仓策略: 所有持仓品种尾盘调仓----->当日持仓批次[%ld] ----> %s\n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}

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
//			printf(">>>>>>>>>>>>>>   尾盘平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

		bool endclosetime = dateutil.CheckSHFEEndMarketTime(this->GetDifSec());
		if (!endclosetime){
//			printf(">>>>>>>>>>>>>>   尾盘平仓策略验证 --> [%s]非尾盘平仓时间 !!!  \n",item.InstrumentID);
			continue;
		}

//		printf(">>>>>>>>>>>>>>ProductID -->  %s  %ld   \n",item.InstrumentID,sizeof(item.InstrumentID));
		//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(item.InstrumentID)> 0 ;
		if (!valid_redisdepth){

			if(!codeutil.IsNightByProductid(ProductID) && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf(">>>>>>>>>>>>>>	当前夜盘时间,白盘合约在夜盘无数据: %s    如有疑问 请核实!!! \n",item.InstrumentID);
				//正常情况 白盘合约在夜盘无数据
				continue;
			}
			else {
				printf(">>>>>>>>>>>>>>   尾盘平仓策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",item.InstrumentID);
				continue;
			}

		}
		DepthMarket depthmarket;
		depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);
//		printf(">>>>>>>>>>>>>>	持仓信号[%s]-->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f   bid:%.3f  ask:%.3f\n",signalname.c_str(),item.InstrumentID,
//							item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice,depthmarket.BidPrice1,depthmarket.AskPrice1);


		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf(">>>>>>>>>>>>>>  尾盘平仓策略验证 --> [%s]行情数据时不合规!!!  \n",item.InstrumentID);
			continue;
		}

//				bool valid =  valid4;
		// 多头出现亏损
		if(item.Direction==0)	{
			printf(">>>>>>>>>>>>>>	多头合约持仓平仓信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
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
			printf(">>>>>>>>>>>>>>	空头合约尾盘平仓信号 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",item.InstrumentID,
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
void StrategyBase::Close_WinPrice_SignalCheck()
{
	vector<HoldData>  list;
	this->GetDataWrapper()->GetHoldList(list);
	DateUtil dateutil;

//	if(list.size()>0)	{
		printf(">>>>>>>>>>>>>>持仓策略: 持仓合约止盈信号监测----->持仓批次[%ld] ----> %s\n",
				list.size(),dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}

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
//				printf(">>>>>>>>>>>>>>   止盈价平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
				continue;
			}
//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));

//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(item.InstrumentID)> 0 ;
		if (!valid_redisdepth){

			if(!codeutil.IsNightByProductid(ProductID) && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf(">>>>>>>>>>>>>>	当前夜盘时间,白盘合约在夜盘无数据: %s    如有疑问 请核实!!! \n",item.InstrumentID);
				//正常情况 白盘合约在夜盘无数据
				continue;
			}
			else {
				printf(">>>>>>>>>>>>>>   止盈平仓策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",item.InstrumentID);
				continue;
			}

		}

			DepthMarket depthmarket;
			memset(&depthmarket,0,sizeof(depthmarket));
			depthmarket =  this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			if (!valid_depthmarket){
				printf(">>>>>>>>>>>>>>  止盈价平仓策略验证 --> [%s]行情数据时不合规!!!  \n",item.InstrumentID);
				continue;
			}


			bool valid_winprice_base = item.WinPrice >1.0;
			if( !valid_winprice_base  )		{
				printf(">>>>>>>>>>>>>>	止盈价平仓策略验证 --> 止盈价不合规  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
							item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
				continue;
			}

			std::string str_direction;

			bool con1_longwin = depthmarket.LastPrice > item.WinPrice;
			bool con1_shortwin = depthmarket.LastPrice < item.WinPrice;

//			if (item.Direction ==0 && con1_longwin )		{
//				str_direction="多";
//				printf(">>>>>>>>>>>>>>	多头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %.3f   手数:%d  现价: %.3f    止盈价: %.3f\n",
//						item.InstrumentID,item.OpenDate,str_direction.c_str(),
//						item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);
//			}
//			else if (item.Direction ==1 && con1_shortwin )	{
//				str_direction="空";
//				printf(">>>>>>>>>>>>>>	空头止盈监测启动:  %s 持仓的合约----> 开仓时间:%s 方向:%s 开仓价: %.3f   手数:%d  现价: %.3f   止盈价: %.3f\n",
//						item.InstrumentID,item.OpenDate,str_direction.c_str(),
//						item.OpenPriceAverage,item.Volume,depthmarket.LastPrice,item.WinPrice);
//			}

			// 多头出现止盈
			if(item.Direction==0  && con1_longwin )			{

				printf(">>>>>>>>>>>>>>	多头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
						item.OpenPriceAverage,item.WinPrice,depthmarket.LastPrice);
//						int offset =1;
//						SignalCheck(item,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

				Signal signal;
				this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
				CloseSignalUpdate(&signal);
			}
			//空头出现止盈
			else if(item.Direction==1  && con1_shortwin )		{
				printf(">>>>>>>>>>>>>>	空头合约止盈信号激活 -->  %s 开仓价: %.3f  止盈价: %.3f  现价: %.3f \n",item.InstrumentID,
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
void StrategyBase::Single_Close_EndTime(HoldData &item)
{
	char logbuf[1024];

//	std::string tradingday=this->GetDataWrapper()->GetTradingDay();
//	if(tradingday.length()<8){
//		std::cout<<"no trading day "<< tradingday.length()<<endl;
//		return;
//	}
////	TradingDayData tradingdaydata=this->GetTradingDayData();
	DateUtil dateutil;
//
//	MysqlHold mysqlhold(this->GetUserID().c_str());
//	vector<HoldData>  list;
//
////	mysqlhold.Find_AllTradingDayList(list,this->GetTradingDay().c_str());
//
//	mysqlhold.Find_AllList(list);
//	if(list.size()>0)	{
//		printf("持仓策略: 所有持仓品种尾盘调仓----->当日持仓批次[%ld] ----> %s\n",list.size(),
//				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str());
//	}
//
//	for (auto &item:list)		{

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
			return;
		}

		bool endclosetime = dateutil.CheckSHFEEndMarketTime(this->GetDifSec());
		if (!endclosetime){
			printf("   尾盘平仓策略验证 --> [%s]非尾盘平仓时间 !!!  \n",item.InstrumentID);
			return;
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
			return;
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

}


// 监测持仓单 是否到止损值,如果达到,根据止损信号出现次数 激活止损信号
void StrategyBase::Close_DynamicStop_SignalCheck()
{
	char logbuf[1024];
//	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());
//	std::cout<<tradingday_sqltime<<endl;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901

	vector<HoldData>  list;
//	this->GetMysqlHold()->Find_AllList(list);
	this->GetDataWrapper()->GetHoldList(list);
//	if(list.size()>0)	{
//
		printf(">>>>>>>>>>>>>>持仓合约浮动止盈止损信号监测----->持仓批次[%ld] ----> %s  交易日： %s \n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),this->GetTradingDay().c_str());
//	}


	for (auto &item:list)			{
//				printf("Stop_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
		char pinstrumentid[40];
//		char pinstrumentIDDate[60];
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
//			printf(">>>>>>>>>>>>>>   浮动止损平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

//////////////////////日线数据验证///////////////////
		bool valid_dayline = this->GetDataWrapper()->FindDataStatus(pinstrumentid);
		if (!valid_dayline){
			printf(">>>>>>>>>>>>>>   策略验证 --> [%s]20天日线数据时不合规!!!  \n",pinstrumentid);
			continue;
		}

//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		if (!valid_redisdepth){

			if(!codeutil.IsNightByProductid(ProductID) && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf(">>>>>>>>>>>>>>	当前夜盘时间,白盘合约在夜盘无数据: %s    如有疑问 请核实!!! \n",pinstrumentid);
				//正常情况 白盘合约在夜盘无数据
				continue;
			}
			else {
				printf(">>>>>>>>>>>>>>   浮动止损平仓策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
				continue;
			}

		}

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));


		DepthMarket	depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
		bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
		if (!valid_depthmarket){
			printf(">>>>>>>>>>>>>>  浮动止损平仓策略验证 --> [%s]实时行情数据不合规!!!  \n",pinstrumentid);
			continue;
		}

//						printf("止损合约行情比对 -->  %s 开仓价: %.3f 方向:%d  止损价: %.3f  现价: %.3f \n",pinstrumentID,
//													item.OpenPriceAverage,item.Direction,item.StopPrice,depthmarket.LastPrice);

		double tr =this->GetCurrentDayTR(&depthmarket);
		double totaltr=this->GetDataWrapper()->FindTotalTR(pinstrumentid);
		double atr20=(totaltr+tr)/20;


//		bool valid_openprice = item.OpenPriceAverage >1.0 &&  item.OpenPriceAverage <depthmarket.HighestPrice
//				&&  item.OpenPriceAverage >depthmarket.LowestPrice;
//
//		if (!valid_openprice){
//			printf("  浮动止损平仓策略验证 --> [%s]持仓数据---开仓价[%.2f ]不合规!!!   \n",pinstrumentid,item.OpenPriceAverage);
//		}
///////////////////////////////////////持仓止损价合法性验证/////////////////////////////
		bool valid_stopprice = item.StopPrice >1.0 &&
				(item.StopPrice>item.OpenPriceAverage*0.9 || item.StopPrice<item.OpenPriceAverage*1.1);

		if (!valid_stopprice){
			printf(">>>>>>>>>>>>>>  浮动止损平仓策略验证 --> [%s]持仓数据---止损价[%.2f] 不合规!!!  \n",pinstrumentid,item.StopPrice);
			continue;
		}

//		HighLowest hl;
//		if(strcmp(item.OpenDate,this->GetTradingDay().c_str())==0 )	{
//			hl.highest =depthmarket.HighestPrice;
//			hl.lowest =depthmarket.LowestPrice;
//
////						printf("	当日新开仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);
//
//		}
//		else{
//			 hl= mysqldayline.Collect_CycleDatabyOpenDate(item.InstrumentID,tradingday_sqltime.c_str());
//
//			if (hl.highest < depthmarket.HighestPrice)		{
//				hl.highest =depthmarket.HighestPrice;
//			}
//
//			if (hl.lowest > depthmarket.LowestPrice || hl.lowest==0)	{
//				hl.lowest =depthmarket.LowestPrice;
//			}
////							 printf("	历史持仓 %s 止盈  %s--历史最高价 %.3f  历史最低价 %.3f  开仓日期: %s \n",item.OpenDate,item.InstrumentID,hl.highest,hl.lowest, time_buf);
//
//		}
//
//		if( item.Direction ==0 && depthmarket.LastPrice > item.Highest )			{
//
//			item.Highest = depthmarket.LastPrice;
//			item.StopPrice = depthmarket.LastPrice - atr20 *this->GetStopRate();
////			this->GetMysqlHold()->UpdateHighest_StopPrice(item);
//			printf(">>>>>>>>>>>>>>	浮动止损平仓策略:多头合约创新高 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
//					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//			sprintf(logbuf,"	浮动止损平仓策略:多头合约创新高 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",pinstrumentid,
//												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//			this->GetLogUtil()->WriteLog(logbuf);
//		}
//		else if ( item.Direction ==1 && depthmarket.LastPrice < item.Lowest)	{
//			item.Lowest = depthmarket.LastPrice;
//			item.StopPrice = depthmarket.LastPrice + atr20 *this->GetStopRate();
////			this->GetMysqlHold()->UpdateLowest_StopPrice(item);
//
//			printf(">>>>>>>>>>>>>>	浮动止损平仓策略:空头合约创新低 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
//					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//			sprintf(logbuf,"	浮动止损平仓策略:空头合约创新低 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",pinstrumentid,
//												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
//			this->GetLogUtil()->WriteLog(logbuf);
//		}

		// 多头出现止损
		if( item.Direction==0 && depthmarket.LastPrice <item.StopPrice)			{
			printf(">>>>>>>>>>>>>>	多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	平仓信号[%s]  多头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",signalname.c_str(),pinstrumentid,
												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);

			Signal signal;
			this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
			CloseSignalUpdate(&signal);

		}
		//空头出现止损
		else if( item.Direction==1 && depthmarket.LastPrice > item.StopPrice)	{
			printf(">>>>>>>>>>>>>>	空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f \n",pinstrumentid,
					item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			sprintf(logbuf,"	平仓信号[%s]  空头合约止损 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f ",signalname.c_str(),pinstrumentid,
												item.OpenPriceAverage,item.StopPrice,depthmarket.LastPrice);
			this->GetLogUtil()->WriteLog(logbuf);
			Signal signal;
			this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
			CloseSignalUpdate(&signal);
		}
		else	{
			SignalDispearCheck(signalname);
		}
	}// for loop

	vector<HoldData>().swap(list);
}

// 监测持仓单 是否到涨跌停价格
void StrategyBase::Close_LimitPrice_SignalCheck()
{
	char logbuf[1024];


	DateUtil dateutil;
	CodeUtil codeutil;
	std::string trading_date = this->GetDataWrapper()->GetTradingDay().c_str();   //xxxxxxxx   20170901
	std::string tradingday_sqltime = dateutil.ConvertDatetoSqlTime(this->GetDataWrapper()->GetTradingDay().c_str());

	vector<HoldData>  list;
//		this->GetMysqlHold()->Find_AllList(list);
		this->GetDataWrapper()->GetHoldList(list);
//	if(list.size()>0)	{
//
		printf(">>>>>>>>>>>>>>持仓合约涨跌停价信号监测----->持仓批次[%ld] ----> %s  交易日： %s \n",list.size(),
				dateutil.ConvertSHFETimetoSqlTime(this->GetDifSec()).c_str(),this->GetTradingDay().c_str());
//	}


	for (auto &item:list)			{
//				printf("Close_LimitPrice_SignalCheck----->pinstrumentid %s TraderID %s ----> %s\n",item.pinstrumentid.c_str(),
//																item.TradeID.c_str(),dateutil.GetDateTimeString().c_str());
			char pinstrumentid[40];
			strcpy (pinstrumentid,item.InstrumentID);

//			char pinstrumentIDDate[60];
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
//				printf(">>>>>>>>>>>>>>>>   涨跌停平仓策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
				continue;
			}
//////////////////////日线数据验证///////////////////
		bool valid_dayline = this->GetDataWrapper()->FindDataStatus(pinstrumentid);
		if (!valid_dayline){
			printf(">>>>>>>>>>>>>>>>   涨跌停策略验证 --> [%s]20天日线数据时不合规!!!  \n",pinstrumentid);
			continue;
		}

//////////////////////当日行情数据验证///////////////////
		bool valid_redisdepth = this->GetRedisDepthMarket()->Exists(pinstrumentid)> 0 ;
		if (!valid_redisdepth){

			if(!codeutil.IsNightByProductid(productid) && dateutil.CheckSHFENightTime(this->GetDifSec())) {
				printf(">>>>>>>>>>>>>>>>	当前夜盘时间,白盘合约在夜盘无数据: %s    如有疑问 请核实!!! \n",pinstrumentid);
				//正常情况 白盘合约在夜盘无数据
				continue;
			}
			else {
				printf(">>>>>>>>>>>>>>>>   涨跌停平仓策略验证 --> [%s]不存在Redis行情数据 !!!  请检查行情是否接收正常？   \n",pinstrumentid);
				continue;
			}

		}
//			int code_base = this->Valid_StaBaseDataCheck(pinstrumentid,&mysqldayline,trading_date.c_str(),&item);
//			if(code_base!=0){
//				continue;
//			}

//				printf("ProductID -->  %s  %ld   %ld \n",pinstrumentID,sizeof(pinstrumentID),sizeof(pinstrumentID));
			DepthMarket depthmarket;
			depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(pinstrumentid);
//			DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

//			bool valid_price = data.LowerLimitPrice==depthmarket.LowerLimitPrice &&
//					data.UpperLimitPrice && depthmarket.UpperLimitPrice;
//			if(valid_price){
//				printf(">>>>>>>>>>>>>>>>	涨跌停合约行情比对(MysqlDepthMarket比对相同) -->  %s 开仓价: %.3f 方向:%d  涨停价: %.3f 跌停价: %.3f   最小变动价位:%.3f 现价: %.3f \n",
//						item.InstrumentID,item.OpenPriceAverage,item.Direction,data.UpperLimitPrice,
//						data.LowerLimitPrice,depthmarket.PriceTick,depthmarket.LastPrice);
//				printf(">>>>>>>>>>>>>>>>	涨跌停合约目标价位-->  %s 涨停价: [%.3f ---- %.3f] 跌停价: [%.3f ---- %.3f]  \n",
//						item.InstrumentID,depthmarket.UpperLimitPrice-depthmarket.PriceTick *3,data.UpperLimitPrice,
//						data.LowerLimitPrice,depthmarket.LowerLimitPrice+depthmarket.PriceTick *3);
//			}
//			else{
//				printf(">>>>>>>>>>>>>>>>	涨跌停合约行情比对 -->>>>>>>> %s 开仓价: %.3f 方向:%d  涨停价: %.3f 跌停价: %.3f   现价: %.3f \n",item.InstrumentID,
//					item.OpenPriceAverage,item.Direction,depthmarket.UpperLimitPrice,
//					depthmarket.LowerLimitPrice,depthmarket.LastPrice);
//				printf(">>>>>>>>>>>>>>>>	涨跌停合约行情比对(MysqlDepthMarket比对不同!!!) -->>>>>  %s 开仓价: %.3f 方向:%d  涨停价: %.3f 跌停价: %.3f   现价: %.3f \n",item.InstrumentID,
//					item.OpenPriceAverage,item.Direction,data.UpperLimitPrice,
//					data.LowerLimitPrice,depthmarket.LastPrice);
//			}

			bool valid_depthmarket = this->ValidCheck_DepthMarket(&depthmarket);
			bool valid = false;

			if(strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0||
							strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0)	{
				valid= valid_depthmarket ;
			}
			else{
				valid=valid_depthmarket;
			}


			bool upperprice = depthmarket.LastPrice>=depthmarket.UpperLimitPrice-depthmarket.PriceTick *3
					&& depthmarket.LastPrice <= depthmarket.UpperLimitPrice ;
			bool lowerprice = depthmarket.LastPrice<=depthmarket.LowerLimitPrice+depthmarket.PriceTick *3
					&& depthmarket.LastPrice >= depthmarket.LowerLimitPrice ;

			if(upperprice){
				printf(">>>>>>>>>>>>>>>>	接近涨停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
						depthmarket.UpperLimitPrice-depthmarket.PriceTick *3,depthmarket.UpperLimitPrice);
				sprintf(logbuf,">>>>>>>>>>>>>>>>	接近涨停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
													depthmarket.UpperLimitPrice-depthmarket.PriceTick *3,depthmarket.UpperLimitPrice);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else if(lowerprice){
				printf(">>>>>>>>>>>>>>>>	接近跌停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
						depthmarket.LowerLimitPrice+depthmarket.PriceTick *3,depthmarket.LowerLimitPrice);
				sprintf(logbuf,">>>>>>>>>>>>>>>>	接近跌停板附近价位 --> PriceTick %.2f [%.2f - %.2f]  \n",depthmarket.PriceTick,
													depthmarket.LowerLimitPrice+depthmarket.PriceTick *3,depthmarket.LowerLimitPrice);
				this->GetLogUtil()->WriteLog(logbuf);
			}

			// 多头出现涨停
			if(valid && item.Direction==0 && upperprice)			{
				printf(">>>>>>>>>>>>>>>>	多头合约涨停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				sprintf(logbuf,">>>>>>>>>>>>>>>>	平仓信号[%s]  多头合约涨停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",signalname.c_str(),pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				this->GetLogUtil()->WriteLog(logbuf);

				Signal signal;
				this->PreProcess_CloseSignalHoldData(&signal,&item,depthmarket.LastPrice,signalname.c_str());
				CloseSignalUpdate(&signal);
			}
			//空头出现跌停
			else if(valid && item.Direction==1 && lowerprice)	{
				printf(">>>>>>>>>>>>>>>>	空头合约跌停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",pinstrumentid,
						item.OpenPriceAverage,depthmarket.LastPrice);
				sprintf(logbuf,">>>>>>>>>>>>>>>>	平仓信号[%s]  空头合约跌停平仓 -->  %s 开仓价: %.3f  现价: %.3f \n",signalname.c_str(),pinstrumentid,
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
inline void StrategyBase::PreProcess_CloseSignalHoldData(Signal * signal,HoldData *holddata,double lastprice,const char* signalname )
{
	strcpy(signal->InstrumentID,holddata->InstrumentID);
	strcpy(signal->TradingDay,this->GetTradingDay().c_str());
	strcpy(signal->SingalName,signalname);
	signal->Volume = holddata->Volume;
	signal->MaxSignalCount =this->GetCloseSignalCount();
	signal->Direction=holddata->Direction;
	signal->Offset=1;
	signal->Price=lastprice;
	signal->StopPrice=holddata->StopPrice;
	signal->WinPrice=holddata->WinPrice;
	signal->StopRange  = holddata->StopRange;
}
void StrategyBase::SignalCheck(HoldData item,
		std::string signalname,
		const char* tradingday,
		int signalcount,
		int direction,
		int offset,
		double lastprice)
{
	DateUtil dateutil;
	InstrumentData instrumentdata;
	//MysqlInstrument mysqlinstrument;
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);

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


			double stopprice;

			if(direction==0)
			{
				double m;
				double s;

				s = lastprice *(0.98) ;
				m =fmod(s,instrumentdata.PriceTick);
				stopprice = s -m;
			}
			else
			{
				double m;
				double s;
				s = lastprice *(1.02);
				m =fmod(s,instrumentdata.PriceTick);
				stopprice = s -m;
			}

			signal.StopPrice=stopprice;
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

			double stopprice;

			if(item.Direction==0)
			{
				double m;
				double s;

				s = lastprice *(0.98) ;
				m =fmod(s,instrumentdata.PriceTick);
				stopprice = s -m;
			}
			else
			{
				double m;
				double s;
				s = lastprice *(1.02);
				m =fmod(s,instrumentdata.PriceTick);
				stopprice = s -m;
			}

			signal.StopPrice=stopprice;
			this->GetRedisSignal()->SetInsSignal(signal,signalname);

		}


}
void StrategyBase::CloseSignalCheck(HoldData item,
		std::string signalname,
		int signalcount,
		int direction,
		int offset,
		double lastprice)
{
	DateUtil dateutil;
	InstrumentData instrumentdata;
	//MysqlInstrument mysqlinstrument;
//	instrumentdata=this->GetMysqlInstrument()->Find_DatabyInstrumentID(item.InstrumentID);

	if( this->GetRedisSignal()->Exists(signalname)== 0 )
		{
//										printf("不存在signal --> %s\n", signalname.c_str());
			Signal signal;
			strcpy(signal.InstrumentID,item.InstrumentID);
			strcpy(signal.TradingDay,this->GetTradingDay().c_str());
			strcpy(signal.SingalName,signalname.c_str());
			signal.Volume = item.Volume;


			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			strcpy(signal.SingalStartTime,str_localupdate.c_str());
			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
			signal.SignalCount = 1;
			signal.MaxSignalCount = signalcount;
			signal.SingalActive=false;
			signal.OrderSend=false;
			signal.Direction=direction;
			signal.Offset=offset;
			signal.Price=lastprice;
			signal.StopPrice=item.StopPrice;
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
			if(signal.SignalCount>signal.MaxSignalCount)
				{
				signal.SingalActive=true;
				}
			signal.Price=lastprice;
			this->GetRedisSignal()->SetInsSignal(signal,signalname);

		}


}
// 开仓信号更新,并写入止损价
void StrategyBase::OpenSignalCheck(HoldData item,
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
//										printf("OpenSignalCheck 不存在signal --> %s\n", signalname.c_str());
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
				signal.WinPrice= lastprice+signal.StopRange;
				signal.StopPrice=lastprice-signal.StopRange;
			}
			else if (signal.Direction==1)
			{
				signal.WinPrice= lastprice-signal.StopRange;;
				signal.StopPrice=lastprice+signal.StopRange;
			}




			this->GetRedisSignal()->SetInsSignal(signal,signalname);
		}

	else if(this->GetRedisSignal()->Exists(signalname)> 0)
		{
//										printf("OpenSignalCheck 存在signal !!!!--> %s\n", signalname.c_str());
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

//			double stopprice=GetATRStopPrice(item.InstrumentID,direction,lastprice);
//			signal.StopPrice=stopprice;

			signal.StopRange=this->GetStopPriceRange() ;
			if(signal.Direction==0)
			{
				signal.WinPrice= lastprice+signal.StopRange;;
				signal.StopPrice=lastprice-signal.StopRange;
			}
			else if (signal.Direction==1)
			{
				signal.WinPrice= lastprice-signal.StopRange;;
				signal.StopPrice=lastprice+signal.StopRange;
			}


			this->GetRedisSignal()->SetInsSignal(signal,signalname);

		}


}

// 开仓信号更新,并写入止损价
void StrategyBase::OpenSignalUpdate(Signal * originalsignal)
{
	DateUtil dateutil;

	char logbuf[1024];


	double stoprate=this->GetStopRate();
	double winrate=this->GetWinRate();

	if( this->GetRedisSignal()->Exists(originalsignal->SingalName)== 0 )	{

			Signal signal;
			strcpy(signal.InstrumentID,originalsignal->InstrumentID);

			strcpy(signal.TradingDay,this->GetTradingDay().c_str());
			strcpy(signal.SingalName,originalsignal->SingalName);
			signal.Volume = originalsignal->Volume;


			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			strcpy(signal.SingalStartTime,str_localupdate.c_str());
			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());

			signal.MaxSignalCount =originalsignal->MaxSignalCount;
			signal.SignalCount = 1;
			signal.SingalActive=false;
			signal.OrderSend=false;


			signal.Direction=originalsignal->Direction;
			signal.Offset=originalsignal->Offset;
			signal.Price=originalsignal->Price;
			signal.StopRange =originalsignal->StopRange;


//			double stopprice=GetATRStopPrice(item.InstrumentID,direction,lastprice);
//			signal.StopRange=this->GetStopPriceRange() ;
			if(signal.Direction==0)		{
				signal.WinPrice= originalsignal->Price+signal.StopRange*winrate;
				signal.StopPrice=originalsignal->Price-signal.StopRange*stoprate;
			}
			else if (signal.Direction==1)	{
				signal.WinPrice= originalsignal->Price-signal.StopRange*winrate;
				signal.StopPrice=originalsignal->Price+signal.StopRange*stoprate;
			}


			this->GetRedisSignal()->SetInsSignal(signal,originalsignal->SingalName);

			sprintf(logbuf,"StrategyBase::OpenSignalUpdate 不存在开仓signal --> 合约:%s 信号:%s 方向:%d  手数:%d  价格:%.3f 止损区间:%.3f  "
					"止损价:%.3f   止盈价:%.3f   更新时间:%s  \n",
					signal.InstrumentID,signal.SingalName,signal.Direction,signal.Volume,signal.Price,
					signal.StopRange,signal.StopPrice,signal.WinPrice,signal.LocalUpdateTime);
			this->GetLogUtil()->WriteLog(logbuf);
		}

	else if(this->GetRedisSignal()->Exists(originalsignal->SingalName)> 0)	{
//										printf("OpenSignalUpdate 存在signal !!!!--> %s\n", signalname.c_str());
			Signal signal;
			signal=this->GetRedisSignal()->GetInsSignal(originalsignal->SingalName);
			signal.SignalCount++;
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());

			signal.Price=originalsignal->Price;

//			double stopprice=GetATRStopPrice(item.InstrumentID,direction,lastprice);
//			signal.StopPrice=stopprice;

//			signal.StopRange=this->GetStopPriceRange() ;
			signal.StopRange =originalsignal->StopRange;

			if(signal.Direction==0)	{
				signal.WinPrice= originalsignal->Price+signal.StopRange*winrate;
				signal.StopPrice=originalsignal->Price-signal.StopRange*stoprate;
			}
			else if (signal.Direction==1)	{
				signal.WinPrice= originalsignal->Price-signal.StopRange*winrate;
				signal.StopPrice=originalsignal->Price+signal.StopRange*stoprate;
			}

			if(signal.SignalCount>originalsignal->MaxSignalCount && !signal.SingalActive)		{
				signal.SingalActive=true;
				sprintf(logbuf,"StrategyBase::OpenSignalUpdate 开仓signal激活 --> 合约:%s 信号:%s 方向:%d  手数:%d 价格:%.3f 止损区间:%.3f "
						"止损价:%.3f   止盈价:%.3f    更新时间:%s  信号计数:%d\n",
						signal.InstrumentID,signal.SingalName,signal.Direction,signal.Volume,signal.Price,
						signal.StopRange,signal.StopPrice,signal.WinPrice,signal.LocalUpdateTime,signal.SignalCount);
				this->GetLogUtil()->WriteLog(logbuf);
			}

			this->GetRedisSignal()->SetInsSignal(signal,originalsignal->SingalName);

		}


}


void StrategyBase::CloseSignalUpdate(Signal * originalsignal)
{
	DateUtil dateutil;
	char logbuf[1024];

	std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
//	InstrumentData instrumentdata;
//	//MysqlInstrument mysqlinstrument;
//	instrumentdata=mysqlinstrument.Find_DatabyInstrumentID(originalsignal.InstrumentID);

	if( this->GetRedisSignal()->Exists(originalsignal->SingalName)== 0 ){
//			printf("不存在signal --> %s\n", originalsignal->SingalName);
			Signal signal;
			strcpy(signal.InstrumentID,originalsignal->InstrumentID);
			strcpy(signal.TradingDay,this->GetTradingDay().c_str());
			strcpy(signal.SingalName,originalsignal->SingalName);
			signal.Volume = originalsignal->Volume;
			signal.MaxSignalCount =originalsignal->MaxSignalCount;
			signal.Direction=originalsignal->Direction;
			signal.Offset=originalsignal->Offset;
			signal.Price=originalsignal->Price;
			signal.StopPrice=originalsignal->StopPrice;
			signal.WinPrice=originalsignal->WinPrice;
			signal.StopRange=originalsignal->StopRange;


			strcpy(signal.SingalStartTime,str_localupdate.c_str());
			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
			signal.SignalCount = 1;
			signal.SingalActive=false;
			signal.OrderSend=false;
			signal.Status=false;

			sprintf(logbuf,"StrategyBase::CloseSignalUpdate >>>>>>不存在平仓signal --> 合约:%s 信号:%s 方向:%d   手数:%d 价格:%.3f 止损区间:%.3f  更新时间:%s  \n",
					signal.InstrumentID,signal.SingalName,signal.Direction,signal.Volume,signal.Price,
					signal.StopRange,signal.LocalUpdateTime);
			this->GetLogUtil()->WriteLog(logbuf);


			this->GetRedisSignal()->SetInsSignal(signal,originalsignal->SingalName);
		}

	else if(this->GetRedisSignal()->Exists(originalsignal->SingalName)> 0){
//			printf("存在signal !!!!--> %s\n", originalsignal->SingalName);
			Signal signal;
			signal=this->GetRedisSignal()->GetInsSignal(originalsignal->SingalName);
			signal.SignalCount++;
			signal.Price=originalsignal->Price;

			strcpy(signal.SingalEndTime,str_localupdate.c_str());
			strcpy(signal.LocalUpdateTime,str_localupdate.c_str());
			if(signal.SignalCount>=signal.MaxSignalCount && !signal.SingalActive)	{
				signal.SingalActive=true;
				sprintf(logbuf,"StrategyBase::CloseSignalUpdate >>>>>>平仓signal激活 --> 合约:%s 信号:%s 方向:%d  手数:%d 价格:%.3f 止损区间:%.3f 更新时间:%s  信号计数:%d\n",
						signal.InstrumentID,signal.SingalName,signal.Direction,signal.Volume,signal.Price,
						signal.StopRange,signal.LocalUpdateTime,signal.SignalCount);
				this->GetLogUtil()->WriteLog(logbuf);
			}
			else if(signal.SignalCount<signal.MaxSignalCount && !signal.SingalActive){
				sprintf(logbuf,"StrategyBase::CloseSignalUpdate >>>>>>平仓signal未激活 计数中 [%d]<[%d]--> 合约:%s 信号:%s 方向:%d  手数:%d 价格:%.3f 止损区间:%.3f 更新时间:%s  信号计数:%d\n",
						signal.SignalCount,signal.MaxSignalCount,signal.InstrumentID,signal.SingalName,signal.Direction,
						signal.Volume,signal.Price,	signal.StopRange,signal.LocalUpdateTime,signal.SignalCount);
				this->GetLogUtil()->WriteLog(logbuf);
			}

			this->GetRedisSignal()->SetInsSignal(signal,originalsignal->SingalName);
	}
}
void StrategyBase::SignalDispearCheck(std::string signalname)
{

	DateUtil dateutil;
	char logbuf[1024];


	if(this->GetRedisSignal()->Exists(signalname)> 0 )	{

//		printf("SignalDispearCheck 存在信号 signal消失 !!!!--> %s\n", signalname.c_str());
			Signal signal;
			signal=this->GetRedisSignal()->GetInsSignal(signalname);
			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();

			if(signal.SingalActive)		{
//				strcpy(signal.SingalStartTime,str_localupdate.c_str());
				strcpy(signal.SingalEndTime,str_localupdate.c_str());
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());

				signal.SignalCount=0;
				signal.SingalActive=false;
				signal.OrderSend=false;

				sprintf(logbuf,"StrategyBase::SignalDispearCheck signal消失!!! --> 合约:%s 信号:%s 方向:%d  手数:%d 价格:%.3f 止损区间:%.3f 开始时间:%s  结束时间:%s 信号计数:%d\n",
							signal.InstrumentID,signal.SingalName,signal.Direction,signal.Volume,signal.Price,
							signal.StopRange,signal.SingalStartTime,signal.SingalEndTime,signal.SignalCount);
				this->GetLogUtil()->WriteLog(logbuf);
				this->GetRedisSignal()->SetInsSignal(signal,signalname);
			}
			else if(signal.SingalActive==false &&signal.SignalCount > 0)	{
//				strcpy(signal.SingalStartTime,str_localupdate.c_str());
				strcpy(signal.SingalEndTime,str_localupdate.c_str());
				strcpy(signal.LocalUpdateTime,str_localupdate.c_str());

				signal.SignalCount=0;
				this->GetRedisSignal()->SetInsSignal(signal,signalname);
			}
		}
	else	{
//		printf("SignalDispearCheck 不存在信号，signal消失 !!!!--> %s\n", signalname.c_str());
	}
}
void StrategyBase::OpenPriceSeek(){

	char logbuf[2048];

	DateUtil dateutil;
//	this->GetLogUtil(()->WriteLog("QueueingOpenPriceChange_SignalCheck---------------->begin!!!");

	std::string tradingday=this->GetTradingDay(); //xxxxxxxx   20170901//
	vector<OrderData>  list;
	this->GetDataWrapper()->GetOpenOrderList(list);
//	printf(">>>>>>>>>>>%s::OpenPriceSeek----->   openorder count[%ld] \n",this->GetStrategyName().c_str(),list.size());
//
//	for (auto &item:list)	{
//		printf(">>>>>>>>>>>%s::OpenPriceSeek----->   openorder 合约:%s    状态:%d \n",
//				this->GetStrategyName().c_str(),item.InstrumentID,item.Status);
//	}
	vector<OrderData>  queuelist;

	this->GetDataWrapper()->GetOpenOrderQueueList(queuelist);

	if(queuelist.size()>0)
	{
		printf(">>>>>>>>>>>%s::OpenPriceSeek-----> 开仓信号数: [%d]-----> 开仓队列未成交合约数 [%ld]    成交合约数[%ld]\n",
				this->GetStrategyName().c_str(),this->GetRedisSignal()->GetDbSize(),queuelist.size(),list.size());
	}

	for (auto &item:queuelist)		{
		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_pricechange");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		int signalcount =1;

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
		if (!opentime){
//			printf(">>>>>>>>>>>>>>BigVolume::OpenPriceSeek--->追价信号策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

		//MysqlDepthMarket mysqldepthmarket;
//		DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

		if(depthmarket.AskPrice1>=DBL_MAX)	{
//			printf(">>>>>>>>>>>>>>>委托单交易队列追价比对-----> 无穷大  askprice\n");
		}
//		else	{
//			printf("委托单交易队列追价比对-----> !!!!\n");
//		}
		bool valid0 =  depthmarket.LastPrice >depthmarket.LowerLimitPrice;
		bool valid1 = depthmarket.LastPrice < depthmarket.UpperLimitPrice;
		bool valid2 = depthmarket.AskPrice1<DBL_MAX;
		bool valid3 = depthmarket.BidPrice1<DBL_MAX;
		bool valid = valid0 && valid1 && valid2 && valid3;

		if(valid)	{

			int longtick;
			int shorttick;

			longtick =(depthmarket.AskPrice1-item.LimitPrice)/depthmarket.PriceTick;
			shorttick=(depthmarket.BidPrice1-item.LimitPrice)/depthmarket.PriceTick;

//			printf(">>>>>>>>>>>>>>>>委托单交易队列追价比对-----> %s direction:%d  pricetick: %.2f limitprice:%.2f -----> 申买价 :%.2f 申卖价: %.2f  多头价格跳跃点数: %d  空头价格跳跃点数: %d \n",
//										item.InstrumentID,item.Direction,depthmarket.PriceTick,
//										item.LimitPrice,depthmarket.BidPrice1,depthmarket.AskPrice1,longtick,shorttick);
			// 多头
			if( item.Direction==0 && longtick>0)	{
//				printf(">>>>>>>>>>>>>.多头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f 价格跳跃点数: %d \n",item.InstrumentID,
//						item.LimitPrice,item.StopPrice,depthmarket.LastPrice,longtick);
				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;
				SignalCheck(holddata,signalname,tradingday.c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			//空头
			else if( item.Direction==1 && shorttick<0)	{
//				printf(">>>>>>>>>>>>>>>>>空头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f   价格跳跃点数: %d \n",item.InstrumentID,
//						item.LimitPrice,item.StopPrice,depthmarket.LastPrice,shorttick);

				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;
				SignalCheck(holddata,signalname,tradingday.c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			else	{
//									printf("追价信号消失 --> %s\n", pinstrumentID);
				// 开新单后,相当于信号消失,对撤单信号 进行重置
				SignalDispearCheck(signalname);


			}
		}
		else	{
		}
	}

	vector<OrderData>().swap(list);
	 vector<OrderData>().swap(queuelist);

//	this->GetLogUtil(()->WriteLog("QueueingOpenPriceChange_SignalCheck---------------->End!!!");
}
void StrategyBase::ClosePriceSeek()
{
	char logbuf[2048];

	vector<OrderData>  list;
	this->GetDataWrapper()->GetCloseOrderQueueList(list);
	DateUtil dateutil;
	if(list.size()>0)
	{
		printf(">>>>>>>>>>>>>>>委托平仓单队列追价信号监测----->   redisorder.DbSize[%d]----->[%ld]\n", this->GetRedisSignal()->GetDbSize(),list.size());
	}


	for (auto &item:list)		{
		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_closepricechange");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		int signalcount=1;

		this->GetLogUtil()->WriteLog(logbuf);


		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
		if (!opentime){
			printf("   委托平仓单交易队列--->追价信号策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

		//MysqlDepthMarket mysqldepthmarket;
		DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

		if(depthmarket.AskPrice1>=DBL_MAX)	{
			printf("平仓单交易队列追价比对-----> 无穷大  askprice\n");
		}
//							else
//							{
//								printf("委托单交易队列追价比对-----> !!!!\n");
//							}
		bool valid0 =  depthmarket.LastPrice >data.LowerLimitPrice;
		bool valid1 = depthmarket.LastPrice < data.UpperLimitPrice;
		bool valid2 = depthmarket.AskPrice1<DBL_MAX;
		bool valid3 = depthmarket.BidPrice1<DBL_MAX;
		bool valid = valid0 && valid1 && valid2 && valid3;

		if(valid)	{

			int closelongtick;
			int closeshorttick;

			closelongtick =(item.LimitPrice-depthmarket.BidPrice1)/depthmarket.PriceTick;   //平多头  卖平  ---> 对价 申买价BidPrice1
			closeshorttick=(depthmarket.AskPrice1-item.LimitPrice)/depthmarket.PriceTick;   //平空头 买平   ---> 对价 申卖价AskPrice1

			sprintf(logbuf,"委托平仓单交易队列追价比对-----> %s direction:%d  pricetick: %.2f limitprice:%.2f -----> 申买价 :%.2f 申卖价: %.2f  多头价格跳跃点数: %d  空头价格跳跃点数: %d \n",
																item.InstrumentID,item.Direction,depthmarket.PriceTick,
																item.LimitPrice,depthmarket.BidPrice1,depthmarket.AskPrice1,closelongtick,closeshorttick);
			this->GetLogUtil()->WriteLog(logbuf);


			// 平空单撤单 --> 方向多
			if( item.Direction==0 && closeshorttick>0)		{
//						printf("多头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f 价格跳跃点数: %d \n",item.InstrumentID,
//								item.LimitPrice,item.StopPrice,depthmarket.LastPrice,longtick);
				sprintf(logbuf,"平空单合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f 价格跳跃点数: %d \n",item.InstrumentID,
												item.LimitPrice,item.StopPrice,depthmarket.LastPrice,closeshorttick);
				this->GetLogUtil()->WriteLog(logbuf);
				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;   //撤单
				SignalCheck(holddata,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			//平多单撤单 ---> 方向空
			else if(item.Direction==1 && closelongtick>0)		{
//						printf("空头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f   价格跳跃点数: %d \n",item.InstrumentID,
//								item.LimitPrice,item.StopPrice,depthmarket.LastPrice,shorttick);
				sprintf(logbuf,"平多单合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f   价格跳跃点数: %d \n",item.InstrumentID,
												item.LimitPrice,item.StopPrice,depthmarket.LastPrice,closelongtick);
				this->GetLogUtil()->WriteLog(logbuf);

				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;   //撤单
				SignalCheck(holddata,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			else	{
//									printf("追价信号消失 --> %s\n", pinstrumentID);
				SignalDispearCheck(signalname);

			}

		}
		else	{
			sprintf(logbuf,"平仓单队列追价信号监测-----> 数据非法!!! ");
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}

	 vector<OrderData>().swap(list);

//	this->GetLogUtil(()->WriteLog("QueueingClosePriceChange_SignalCheck---------------->End!!!");
}
//开仓撤单信号
void StrategyBase::QueueingOpenPriceChange_SignalCheck()
{
	char logbuf[2048];
//	Config config(this->GetStrategyName().c_str());

//	this->GetLogUtil(()->WriteLog("QueueingOpenPriceChange_SignalCheck---------------->begin!!!");

	std::string tradingday=this->GetTradingDay(); //xxxxxxxx   20170901//
	vector<OrderData>  list;
	DateUtil dateutil;
	this->GetDataWrapper()->GetOpenOrderQueueList(list);
	if(list.size()>0)	{
		printf("委托开仓单交易队列追价信号监测----->   内存数据库 开仓信号数: redisorder.DbSize[%d]----->开仓队列发送交易数 [%ld]\n",
				this->GetRedisSignal()->GetDbSize(),list.size());
	}

	for (auto &item:list)		{
		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_pricechange");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		int signalcount =1;

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
		if (!opentime){
			printf("   委托开仓单交易队列--->追价信号策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

		//MysqlDepthMarket mysqldepthmarket;
		DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

		if(depthmarket.AskPrice1>=DBL_MAX)	{
			printf("委托单交易队列追价比对-----> 无穷大  askprice\n");
		}
//		else	{
//			printf("委托单交易队列追价比对-----> !!!!\n");
//		}
		bool valid0 =  depthmarket.LastPrice >data.LowerLimitPrice;
		bool valid1 = depthmarket.LastPrice < data.UpperLimitPrice;
		bool valid2 = depthmarket.AskPrice1<DBL_MAX;
		bool valid3 = depthmarket.BidPrice1<DBL_MAX;
		bool valid = valid0 && valid1 && valid2 && valid3;

		if(valid)	{

			int longtick;
			int shorttick;

			longtick =(depthmarket.AskPrice1-item.LimitPrice)/depthmarket.PriceTick;
			shorttick=(depthmarket.BidPrice1-item.LimitPrice)/depthmarket.PriceTick;

			printf("委托单交易队列追价比对-----> %s direction:%d  pricetick: %.2f limitprice:%.2f -----> 申买价 :%.2f 申卖价: %.2f  多头价格跳跃点数: %d  空头价格跳跃点数: %d \n",
										item.InstrumentID,item.Direction,depthmarket.PriceTick,
										item.LimitPrice,depthmarket.BidPrice1,depthmarket.AskPrice1,longtick,shorttick);
			// 多头
			if( item.Direction==0 && longtick>0)	{
				printf("多头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f 价格跳跃点数: %d \n",item.InstrumentID,
						item.LimitPrice,item.StopPrice,depthmarket.LastPrice,longtick);
				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;
				SignalCheck(holddata,signalname,tradingday.c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			//空头
			else if( item.Direction==1 && shorttick<0)	{
				printf("空头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f   价格跳跃点数: %d \n",item.InstrumentID,
						item.LimitPrice,item.StopPrice,depthmarket.LastPrice,shorttick);

				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;
				SignalCheck(holddata,signalname,tradingday.c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			else	{
//									printf("追价信号消失 --> %s\n", pinstrumentID);
				// 开新单后,相当于信号消失,对撤单信号 进行重置
				SignalDispearCheck(signalname);


			}
		}
		else	{

		}



	}


	 vector<OrderData>().swap(list);

//	this->GetLogUtil(()->WriteLog("QueueingOpenPriceChange_SignalCheck---------------->End!!!");
}

//平仓撤单信号
void StrategyBase::QueueingClosePriceChange_SignalCheck()
{
	char logbuf[2048];
//	Config config(this->GetStrategyName().c_str());
//	this->GetLogUtil(()->WriteLog("QueueingOpenPriceChange_SignalCheck---------------->begin!!!");

	std::string tradingday=this->GetTradingDay(); //xxxxxxxx   20170901//
	vector<OrderData>  list;

	DateUtil dateutil;
	this->GetDataWrapper()->GetCloseOrderQueueList(list);
	if(list.size()>0)	{
		printf("委托平仓单队列追价信号监测----->   redisorder.DbSize[%d]----->[%ld]\n", this->GetRedisSignal()->GetDbSize(),list.size());
	}


	for (auto &item:list)		{
		std::string signalname;
		signalname=item.InstrumentID;
		signalname.append("_closepricechange");
		signalname.append("_");
		signalname.append(this->GetUserID().c_str());
		int signalcount=1;

		this->GetLogUtil()->WriteLog(logbuf);


		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());

		bool opentime = dateutil.CheckOpenTime(this->GetDifSec(),item.ExchangeID,ProductID);
		if (!opentime){
			printf("   委托平仓单交易队列--->追价信号策略验证 --> [%s]非开盘时间 !!!  \n",item.InstrumentID);
			continue;
		}

		DepthMarket depthmarket;
		depthmarket = this->GetRedisDepthMarket()->GetDepthMarket(item.InstrumentID);

		//MysqlDepthMarket mysqldepthmarket;
		DepthMarketData data=this->GetMysqlDepthMarket()->Find_DatabyInstrumentID(item.InstrumentID);

		if(depthmarket.AskPrice1>=DBL_MAX)	{
			printf("平仓单交易队列追价比对-----> 无穷大  askprice\n");
		}
//							else
//							{
//								printf("委托单交易队列追价比对-----> !!!!\n");
//							}
		bool valid0 =  depthmarket.LastPrice >data.LowerLimitPrice;
		bool valid1 = depthmarket.LastPrice < data.UpperLimitPrice;
		bool valid2 = depthmarket.AskPrice1<DBL_MAX;
		bool valid3 = depthmarket.BidPrice1<DBL_MAX;
		bool valid = valid0 && valid1 && valid2 && valid3;

		if(valid)	{

			int closelongtick;
			int closeshorttick;

			closelongtick =(item.LimitPrice-depthmarket.BidPrice1)/depthmarket.PriceTick;   //平多头  卖平  ---> 对价 申买价BidPrice1
			closeshorttick=(depthmarket.AskPrice1-item.LimitPrice)/depthmarket.PriceTick;   //平空头 买平   ---> 对价 申卖价AskPrice1

			sprintf(logbuf,"委托平仓单交易队列追价比对-----> %s direction:%d  pricetick: %.2f limitprice:%.2f -----> 申买价 :%.2f 申卖价: %.2f  多头价格跳跃点数: %d  空头价格跳跃点数: %d \n",
																item.InstrumentID,item.Direction,depthmarket.PriceTick,
																item.LimitPrice,depthmarket.BidPrice1,depthmarket.AskPrice1,closelongtick,closeshorttick);
			this->GetLogUtil()->WriteLog(logbuf);


			// 平空单撤单 --> 方向多
			if( item.Direction==0 && closeshorttick>0)		{
//						printf("多头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f 价格跳跃点数: %d \n",item.InstrumentID,
//								item.LimitPrice,item.StopPrice,depthmarket.LastPrice,longtick);
				sprintf(logbuf,"平空单合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f 价格跳跃点数: %d \n",item.InstrumentID,
												item.LimitPrice,item.StopPrice,depthmarket.LastPrice,closeshorttick);
				this->GetLogUtil()->WriteLog(logbuf);
				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;   //撤单
				SignalCheck(holddata,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			//平多单撤单 ---> 方向空
			else if(item.Direction==1 && closelongtick>0)		{
//						printf("空头合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f   价格跳跃点数: %d \n",item.InstrumentID,
//								item.LimitPrice,item.StopPrice,depthmarket.LastPrice,shorttick);
				sprintf(logbuf,"平多单合约价格跳跃点数 -->  %s 开仓价: %.3f  止损价: %.3f  现价: %.3f   价格跳跃点数: %d \n",item.InstrumentID,
												item.LimitPrice,item.StopPrice,depthmarket.LastPrice,closelongtick);
				this->GetLogUtil()->WriteLog(logbuf);

				HoldData holddata;
				strcpy(holddata.InstrumentID,item.InstrumentID);
				strcpy(holddata.ExchangeID,item.ExchangeID);
				holddata.Volume =item.Volume;
				int offset =2;   //撤单
				SignalCheck(holddata,signalname,this->GetTradingDay().c_str(),signalcount,item.Direction,offset,depthmarket.LastPrice);

			}
			else	{
//									printf("追价信号消失 --> %s\n", pinstrumentID);
				SignalDispearCheck(signalname);

			}

		}
		else	{
			sprintf(logbuf,"平仓单队列追价信号监测-----> 数据非法!!! ");
			this->GetLogUtil()->WriteLog(logbuf);
		}
	}

	 vector<OrderData>().swap(list);

//	this->GetLogUtil(()->WriteLog("QueueingClosePriceChange_SignalCheck---------------->End!!!");
}
