#include "daytradetest.hpp"



DayTradeTest::DayTradeTest()
{

}

DayTradeTest::DayTradeTest(const char* pstrategy,const char* year)
{

	this->SetInitialMoney(50000.00);
	this->SetRiskRate(0.03); //用于控制止损比例

	this->SetStopRatio(0.8);  // atr20 止损基准价的倍数

	this->SetWinRatio(1.25);  // stoprange 最终止损价差的倍数

	this->SetSingleMarginRate(0.2); //每次开仓 保证金不能超过的比例

	this->SetLimitSedimentary(2.0);

	this->SetOpenTimeMode(0);  //0 盘中开仓  1 尾盘开仓
	this->SetCloseTimeMode(1); //0 盘中平仓  1 尾盘平仓


	this->SetStrategy(pstrategy);
	SetYear(year);
	SetTable_Year_DateTime(year);


	this->SetTable_Report();

	SetTable_Record_Prefix();


	MysqlDateTime mysqldatetime;

	if(!mysqldatetime.Exist_Table(GetTable_Year_DateTime().c_str()))	{
		CheckDateTimeSequence();
	}

}
DayTradeTest::~DayTradeTest()
{

}
// year    ----- 2017
void DayTradeTest::AllMainContract_DayTradeTest(const char* year)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlUtil mysqlutil("test");
	vector<MainContractData>  list;

	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)	{

		vector<std::string> instrument_list;

//			printf("i   ----->  %d  \n",i);
		GetInstrumentList(instrument_list,year,item);

//				printf(  " 插入合约列表 ---> %s \n",instrumentid.c_str())	;

//		int i=0;
//	for (auto &ins:instrument_list)
//		{
//			printf(  " %d 待回测合约列表 ---> %s \n",i,ins.c_str())	;
////			if (strcmp(item.ProductID,"ag")!=0  )
////			{
////				mysqlutil.CheckAllHisDataByInstrument(ins.c_str());
////			}
//
//i++;
//		}

		bool not_ap = strcmp(item.ProductID,"AP")!=0;
		bool not_stock = strcmp(item.Section,"Stock")!=0;
		bool not_finance = strcmp(item.Section,"Finance")!=0;
		bool not_metal = strcmp(item.Section,"Metal")!=0;
		bool not_czce = strcmp(item.ExchangeID,"CZCE")!=0;
		if ( not_stock && not_metal&& not_finance ){
//		if (strcmp(item.ProductID,"rb")==0  ){
//		if (strcmp(item.ProductID,"ag")==0  || strcmp(item.ProductID,"rb")==0 ){
//			if (strcmp(item.ProductID,"ag")==0  ){
//if (strcmp(item.ProductID,"MA")==0  || strcmp(item.ProductID,"fu")==0 ){
			if(strcmp(item.ProductID,"ag")==0  ){
				this->SetMaxUpDownLimit(0.05);
			}
			else if(strcmp(item.ProductID,"rb")==0 ){
				this->SetMaxUpDownLimit(0.06);
			}
			else {
				this->SetMaxUpDownLimit(0.05);
			}

			this->SetTableRecord(item.ProductID);
			this->SetProductID(item.ProductID);
			this->SetVolumeMultiple(item.VolumeMultiple);

//			std::string tablename_ps = this->GetProductID();
//			tablename_ps.append("_");
//			tablename_ps.append(this->GetYear());
//			MysqlProStatis mysqlprostatis;
//			if(mysqlprostatis.Exist_Table(tablename_ps.c_str())){
//				mysqlprostatis.DropbyTableName(tablename_ps.c_str());
//			}
//
//			mysqlprostatis.CreateTable(tablename_ps.c_str());

			if (!mysqltestrecord.Exist_Table(GetTableRecord().c_str()))		{
				printf(  " [%s] 品种回测开始--------!!!  VolumeMultiple :%d \n",item.ProductID,this->GetVolumeMultiple())	;
				SingleIns_Trade(instrument_list);


//				SingleIns_Statistics(instrument_list);
			}
			else	{
				printf(  " [%s] 品种已经完成测试 !!! \n",item.ProductID)	;
			}

//			double avesed = mysqlprostatis.GetAverageSedimentary(tablename_ps.c_str());
//			ProductStatisticsData maxdata = mysqlprostatis.Find_MaxSedimentary(tablename_ps.c_str());
//			ProductStatisticsData mindata =  mysqlprostatis.Find_MinSedimentary(tablename_ps.c_str());

//			printf(  " [%s] 品种 ave %.2f  max %.2f  min %.2f \n",item.ProductID,avesed,maxdata.Sedimentary,mindata.Sedimentary)	;
		}
		vector<std::string>().swap(instrument_list);
	}


	vector<MainContractData>().swap(list);
	std::string director="短线回测";
	DateUtil dateutil;
	std::string date = dateutil.GetDateTimeing();

	std::string report_path ="/home/qc/";
	report_path.append(director);
	report_path.append("/");
	report_path.append(date);
	report_path.append("-product-statistics_");
	report_path.append(this->GetYear());
	report_path.append(".xls");

	this->Create_StatisticsXlsFile(report_path);

}

void DayTradeTest::SingleIns_Trade(vector<std::string> &array)
{
	std::string tablename =this->GetTable_Year_DateTime().c_str();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;


	mysqldatetime.Find_AllLists(list,tablename.c_str());
//	mysqldatetime.Find_AllListsbyStartSqlTime(list,tablename.c_str(),"2019-06-30 00:00:00");
	DateTime enddatetime =mysqldatetime.Find_LastDateTime(tablename.c_str());

	printf("[enddatetime  %s]---------\n",enddatetime.Time);
	for (auto &dt:list)	{
		this->SetCusor_DaySqlTime(dt.Time);
		printf("[%s]---------\n",dt.Time);


		long int ccl=0;
		long int maxvol=0;

		HxDayLineData main=	this->Get_CurrentMaincontractbyTime(array,dt.Time);

		std::string maininstrument=main.InstrumentID;
//		printf("[%s]--------- 主力合约 :  %s   open: %.3f  \n",dt.Time,main.InstrumentID,main.Open);

		if (maininstrument.length()>2)	{
			this->SetMainInstrumentID(main.InstrumentID);

			if(!mysqltestrecord.Exist_Table(this->GetTableRecord().c_str()))			{
				mysqltestrecord.CreateTable(this->GetTableRecord().c_str());
			}

			int count  =mysqltestrecord.Count_RecordByStatus(this->GetTableRecord().c_str(),0);

			if(count==0)  {  // 空仓

				//趋势交易
	//			MinuteOpen(section,main.InstrumentID,dt.Time,predata,today.Open,highdata,lowdata,cycledata);
//				BigVolume_Open_MinuteLine();
//				int p= this->process_GetPosition();
				int p=1;
				if(p>0)		{
//					printf("[%s]风险控制[%s]     可开仓位  %d     \n ",this->GetMainInstrumentID().c_str(),
//							this->GetCusor_DaySqlTime().c_str(),p);
//					CycleN_Open_MinuteLine();
//					GKGZ_Open_MinuteLine();
//					CCI_Test();
//					MA_Open_MinuteLine();
//					MinuteCycleN_Open_MinuteLine();
//					MinuteCycleN_AverageBack_Open_MinuteLine();
//					VolumeOPI_Open_MinuteLine();
//					BigBigVolume_Open_MinuteLine();
					BigOneMinVolume_Open_MinuteLine();
//					EMA52060_Open_MinuteLine();
//					PivotPoint_Open_MinuteLine();
//					BigBigVolume_AverageBack_Open_MinuteLine();
				}
				else {
					printf("[%s]风险控制[%s]     仓位空  %d     !!!!!!! \n ",this->GetMainInstrumentID().c_str(),
												this->GetCusor_DaySqlTime().c_str(),p);
				}
			}
			else	{

			}

		}
		else	{
//			printf("[%s]--------- 无主力合约 :  %s    \n",dt.Time,this->GetProductID().c_str());
		}

	}
	vector<DateTime>().swap(list);


}


void DayTradeTest::SingleIns_Trade_Test(vector<std::string> &array)
{
	std::string tablename =this->GetTable_Year_DateTime().c_str();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;
//	MysqlMaincontract mysqlmaincontract;
//	MysqlAvePrice mysqlaveprice;
//	MysqlCycle mysqlcycle;
	mysqldatetime.Find_AllLists(list,tablename.c_str());

	DateTime enddatetime =mysqldatetime.Find_LastDateTime(tablename.c_str());


	//定义开平仓时间  0, 盘中开仓   1,尾盘开仓
	this->SetOpenTimeMode(1);
	this->SetCloseTimeMode(1);

	for (auto &dt:list)
	{
		this->SetCusor_DaySqlTime(dt.Time);

//		printf("[%s]---------\n",dt.Time);

		HxDayLineData main=	this->Get_CurrentMaincontractbyTime(array,dt.Time);
//		printf("[%s]--------- 主力合约 :  %s   open: %.3f  \n",dt.Time,main.InstrumentID,main.Open);

		this->SetMainInstrumentID(main.InstrumentID);

		if(!mysqltestrecord.Exist_Table(this->GetTableRecord().c_str()))
		{
			mysqltestrecord.CreateTable(this->GetTableRecord().c_str());
		}

		int count  =mysqltestrecord.Count_RecordByStatus(this->GetTableRecord().c_str(),0);

		if(count==0)    // 空仓
		{
//				printf("---空仓 %s  -------    presettle: %.3f    -------%s  \n",
//						main.InstrumentID,predata.AveragePrice,dt.Time);
			//趋势交易
//			Cycle_Open_MinuteLine(main.InstrumentID,dt.Time,predata,today.Open,highdata,lowdata,cycledata);

//			EMA_Open_MinuteLine();

//			EMA_Open_MinuteLine();

//			MA_EndTime_Open_MinuteLine();

//			Cycle_Open_MinuteLine();
		}
		else	{
			vector<TestRecordData>  record_list;

			 mysqltestrecord.Find_HoldLists(record_list,this->GetTableRecord().c_str());

			 if(record_list.size() >1)		 {
				 printf("	同时持仓合约数(主力切换)-------> %ld   \n" ,record_list.size());
			 }

			 for (auto &record:record_list)		 {
				 if (strcmp(record.InstrumentID,main.InstrumentID)==0)		 {
					 printf("	主力合约  %s -------  ----%s  \n",record.InstrumentID,dt.Time);
				 }
				 else	 {
					 printf("	非主力合约  %s -------  -------%s  \n",record.InstrumentID,dt.Time);
				 }

				 if(mysqldayline.Exist_DatabySqlTime(record.InstrumentID,dt.Time))	 {

					 HxDayLineData holddata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
					 if (holddata.Volume <30000 )		 {
						 printf("	持仓合约  %s -------  成交量变小 平仓  -------%s  \n",record.InstrumentID,dt.Time);

						 EndDayClose(record.InstrumentID,holddata);
					 }
					 else	 {
 						 printf("	持仓合约 持有  %s -------    -------%s  \n",record.InstrumentID,dt.Time);

//						 EMA_Hold_MinuteLine(record.InstrumentID);
// 						EMA_Hold_MinuteLine(record.InstrumentID);
//						 MA_Hold_MinuteLine(record.InstrumentID);
//						 MA_EndTime_Hold_MinuteLine(record.InstrumentID);
					 }


				 }
				 else		 {
					 printf("	持仓合约 持有无日线数据   %s -------    -------%s  \n",record.InstrumentID,dt.Time);
					 HxDayLineData lastdata=mysqldayline.Find_LastDayLineOffset(record.InstrumentID,5);
					 EndDayClose(record.InstrumentID,lastdata);
				 }


				 if (strcmp(dt.Time,enddatetime.Time)==0)	 {
					 printf("	持仓合约 截止年度最后交易日   %s -------    -------%s  \n",record.InstrumentID,dt.Time);
					 HxDayLineData lastdata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
					 EndDayClose(record.InstrumentID,lastdata);
				 }

			 }


			 vector<TestRecordData>().swap(record_list);
		}
	}
	vector<DateTime>().swap(list);


}

void DayTradeTest::SingleIns_Statistics(vector<std::string> &array)
{
	std::string tablename =this->GetTable_Year_DateTime().c_str();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;



	mysqldatetime.Find_AllLists(list,tablename.c_str());
//	mysqldatetime.Find_AllListsbyStartSqlTime(list,tablename.c_str(),"2019-06-30 00:00:00");
	DateTime enddatetime =mysqldatetime.Find_LastDateTime(tablename.c_str());

	printf("[enddatetime  %s]---------\n",enddatetime.Time);
	for (auto &dt:list)	{
		this->SetCusor_DaySqlTime(dt.Time);
//		printf("[%s]---------\n",dt.Time);


		long int ccl=0;
		long int maxvol=0;

		HxDayLineData main=	this->Get_CurrentMaincontractbyTime(array,dt.Time);

		std::string maininstrument=main.InstrumentID;
//		printf("[%s]--------- 主力合约 :  %s   open: %.3f  \n",dt.Time,main.InstrumentID,main.Open);

		if (maininstrument.length()>2)	{
			this->SetMainInstrumentID(main.InstrumentID);

			if(!mysqltestrecord.Exist_Table(this->GetTableRecord().c_str()))			{
				mysqltestrecord.CreateTable(this->GetTableRecord().c_str());
			}

			int count  =mysqltestrecord.Count_RecordByStatus(this->GetTableRecord().c_str(),0);

			if(count==0)  {  // 空仓

				//趋势交易
	//			MinuteOpen(section,main.InstrumentID,dt.Time,predata,today.Open,highdata,lowdata,cycledata);
//				BigVolume_Open_MinuteLine();
//				int p= this->process_GetPosition();
				int p=1;
				if(p>0)		{
//					printf("[%s]风险控制[%s]     可开仓位  %d     \n ",this->GetMainInstrumentID().c_str(),
//							this->GetCusor_DaySqlTime().c_str(),p);

					Statistics_Ins_MinuteLine();
//					PivotPoint_Open_MinuteLine();
//					BigBigVolume_AverageBack_Open_MinuteLine();
				}
				else {
					printf("[%s]风险控制[%s]     仓位空  %d     !!!!!!! \n ",this->GetMainInstrumentID().c_str(),
												this->GetCusor_DaySqlTime().c_str(),p);
				}
			}
			else	{

			}

		}
		else	{
//			printf("[%s]--------- 无主力合约 :  %s    \n",dt.Time,this->GetProductID().c_str());
		}

	}
	vector<DateTime>().swap(list);


}

void DayTradeTest::BigVolume_Open_MinuteLine()
{

//	printf ("BigVolume_Open_MinuteLine begin \n");

	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	vector<FiveDayLineData> fiveday_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	mysqldayline.Find_FiveDayLineDataListsbyDate(fiveday_list,this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5);
//	int count=2;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	MysqlHisMinData mysqlhismindata;
	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else
	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());

	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());

	int n_long =20;
	int n_short =10;
	ATRData atr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long);
	ATRData atr10_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short);

	int n ;
	double k ;
	if(atr10_data.ATR >1.0 && atr20_data.ATR >1.0)
	{
		k = atr20_data.ATR/atr10_data.ATR;
	}
	else
	{
		k=1.0;
	}

	n=floor(n_long*k);
	CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_long);
	ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_short);
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
	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,pre_n);

	this->SetStopRange(atr20_data.ATR *this->GetStopRatio());

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		vector<MinuteLineData>  minutelists;
		int volume_n=21;

		mysqlhismindata.Find_ListsNbyMinuteTime(minutelists,tablename.c_str(),mindata.MinuteTime,volume_n);
		int listsize=(int)minutelists.size();
		int dif_opi = minutelists.back().OpenInterest-minutelists.front().OpenInterest;



		int dif_volume;
		int long_volume=0 ;  //多头成交量
		int short_volume=0;  //空头成交量

		double high;
		double low;
		double lastclose;
		double lastmin_close;

		int i=0;
		for (auto &data:minutelists){

			if (i< listsize-1)	{
				if(i==0){
					high=data.price;
					low=data.price;
				}
				else{
					high=max(high,data.price);
					low=min(low,data.price);
				}

			}
			else	{
				lastclose=data.price;
			}

			if(i==0){
				if(data.price>today.Open)				{		long_volume+=data.Volume;	}
				else if(data.price<today.Open)			{		short_volume+=data.Volume;	}
				lastmin_close=data.price;
			}
			else{
				if(data.price>lastmin_close)		{	long_volume+=data.Volume;		}
				else if(data.price<lastmin_close)	{	short_volume+=data.Volume;		}
				lastmin_close=data.price;
			}

			i++;

		}

		dif_volume=long_volume-short_volume;

		double amount_minute = dif_volume *mindata.price *maincontractdata.VolumeMultiple*maincontractdata.LongMarginRatio/100000000.0;
		double capital_opi = dif_opi *mindata.price *maincontractdata.VolumeMultiple*maincontractdata.LongMarginRatio/100000000.0;

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;


		 if( opentime_mode &&  t_current<t_endtime && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {
			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

			 if ( preday.Close < precycledata.High20 && mindata.price > cycledata.High20 && mindata.price>today.Open)
			{
				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
						 dif_volume,amount_minute,dif_opi,capital_opi);
			 BuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volumebig_open");
			}

			 if (  preday.Close > precycledata.Low20 && mindata.price < cycledata.Low20 && mindata.price< today.Open)
			{
				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
						 dif_volume,amount_minute,dif_opi,capital_opi);
			 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"volumebig_open");
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)
			 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)
			{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)
			 {
				 Close(tr,mindata,"stop_openday");

			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)
			 {
				 Close(tr,mindata,"stop_openday");

			 }

			 if (tr.Direction ==0 && t_current>t_endtime)
			 {
				 Close(tr,mindata,"stop_endtime");

			 }
			 else if (tr.Direction ==1 &&  t_current>t_endtime)
			 {
				 Close(tr,mindata,"stop_endtime");

			 }


		 }

		 vector<MinuteLineData>().swap(minutelists);

	 }

	vector<MinuteLineData>().swap(min_list);
	vector<FiveDayLineData>().swap(fiveday_list);
}
void DayTradeTest::MA_Open_MinuteLine()
{
//	printf ("CycleN_Open_MinuteLine begin \n");
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		this->CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());


	double tr ;

	double highest=today.Open;
	double lowest=today.Open;
	int i=0;
	int startopi=0;
	int total_volume=0;
int n=20;
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(this->GetProductID().c_str());
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );
		 total_volume+=mindata.Volume;
		 if(i==0){
			 startopi=mindata.OpenInterest;
		 }
		 i++;
		 if(mindata.price>highest)		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }

		 if( i<n ){
			 continue;
		 }

		 double ma20 = mysqlhismindata.GetMAbyMinuteTime(this->GetTable_HisMindata().c_str(), n,mindata.MinuteTime);
		 double ma5 = mysqlhismindata.GetMAbyMinuteTime(this->GetTable_HisMindata().c_str(), 5,mindata.MinuteTime);
		 double tr_highlow=abs(highest-lowest);
		 double tr_highclose=abs(highest-preday.Close);
		 double tr_lowclose=abs(lowest-preday.Close);

		 tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,tr);
		 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,tr);
		CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double addopi = ((double)mindata.OpenInterest-(double)startopi);
		double opiadd_rate;
		if(startopi!=0){
			opiadd_rate=(mindata.OpenInterest - startopi ) /(double)startopi;
//			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata.OpenInterest,opiadd_rate);
		}
		else{
			opiadd_rate=0;
		}


		double rate;
		if (mindata.OpenInterest>0){
			rate=(double)total_volume/mindata.OpenInterest;
		}
		else{
			rate=0;
		}


		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * commission.Margin /100000000.0;
		 double averageprice = mysqlhismindata.GetAveragePricebyMinuteTime(this->GetTable_HisMindata().c_str(),
							 maincontractdata.VolumeMultiple,mindata.MinuteTime);


		 if( i>n && opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {
//			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
//			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

//			 bool vol =preatr20_data.ATR /mindata.price >0.04  && preatr20_data.ATR /mindata.price <0.05;
//			 bool vol =preatr20_data.ATR /mindata.price <0.03;


			 std::string tmp_time = mindata.MinuteTime;
			char time[20];
			sprintf(time,"%s",tmp_time.substr(11,8).c_str());

			double Q_BarTime_2 =  dateutil.ConvertMinuteTimetoBarTime(time);
			bool night =  Q_BarTime_2 > 0.210500 && Q_BarTime_2 <=0.235959;
			bool night2 =  Q_BarTime_2 > 0.000000 && Q_BarTime_2 < 0.023000;
			bool day = Q_BarTime_2 > 0.090500 && Q_BarTime_2 < 0.143000;
			bool valid_opentime =false;
			CodeUtil codeutil;
			if(codeutil.IsNightByProductid(this->GetProductID().c_str())  ){


				if ( night || night2 || day){
//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
					valid_opentime=true;
				}
				else{
//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
					continue;
				}
			}
			else if(!codeutil.IsNightByProductid(this->GetProductID().c_str())  ){
				if (day){
//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
					valid_opentime=true;
				}
				else{
//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
					continue;
				}
			}

			 this->SetStopRange(atr20*this->GetStopRatio());
			 int last_3min_volume = mysqlhismindata.GetLast3MinVolumeSum(this->GetTable_HisMindata().c_str(),mindata.MinuteTime);

			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime,3);
			 int pre_3min_opi=pre_3min_data.OpenInterest;

			 bool long_kline = mindata.price>today.Open && today.Open > preday.Close;
			 bool short_kline = mindata.price< today.Open && today.Open < preday.Close  ;
			 bool condition_long = ma5>ma20;
			 bool condition_short =  ma5<ma20;
			 bool condition_atr = tr > atr10 && atr10 > atr20 ;


			 double patr10 = atr10/today.Open;
			bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);
			bool valid_patr= patr10<0.025;
			bool valid_opiaddrate = opiadd_rate>-0.005;
			bool valid=sed>this->GetLimitSedimentary() && !fridaynight  ;

			 if (valid&& condition_long   )	{

//				 printf ("前一天突破做多不满足 check %s date[%s]   precycledata.High20  %.3f preday.High %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//				 			 ,precycledata.High20,preday.High);
//
//				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//								 				 ,cycledata.High20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//						 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 printf("	多头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	多头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	多头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	多头开仓信号 ----------------->均价 : %.2f \n",averageprice);

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = patr10;
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}

			 if (  valid&& condition_short  )	{

//				 printf ("前一天突破做空不满足 check %s date[%s]   precycledata.Low20  %.3f preday.Low %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//									 ,precycledata.Low20,preday.Low);
//
//				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//				 				 ,cycledata.Low20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//										 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 printf("	空头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	空头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	空头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	空头开仓信号 ----------------->均价 : %.2f \n",averageprice);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = patr10;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);



			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

			 if (tr.Direction ==0 && mindata.price < averageprice)	 {
				 Close(tr,mindata,"stop_average");
			 }
			 else if (tr.Direction ==1 && mindata.price > averageprice)	 {
				 Close(tr,mindata,"stop_average");
			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }

			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }

//			 if (tr.Direction ==0 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.OpenInterest<start_opi*0.97 &&mindata.price<tr.OpenPrice*0.997 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }
//			 else if (tr.Direction ==1 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.price>tr.OpenPrice*1.003 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }

		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::CycleN_Open_MinuteLine()
{
//	printf ("CycleN_Open_MinuteLine begin \n");
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		this->CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	this->SetN(20);

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,this->GetN());
	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,this->GetN());
	double tr ;

	double highest=today.Open;
	double lowest=today.Open;
	int i=0;
	int startopi=0;
	int total_volume=0;

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(this->GetProductID().c_str());
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",this->GetTable_HisMindata().c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );
		 total_volume+=mindata.Volume;
		 if(i==0){
			 startopi=mindata.OpenInterest;
		 }
		 i++;
		 if(mindata.price>highest)		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }

		 tr = this->zb_TR(highest,lowest,preday.Close);
		 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),this->GetN(),tr);

		CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),this->GetN());


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double addopi = ((double)mindata.OpenInterest-(double)startopi);
		double opiadd_rate;
		if(startopi!=0){
			opiadd_rate=(mindata.OpenInterest - startopi ) /(double)startopi;
//			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata.OpenInterest,opiadd_rate);
		}
		else{
			opiadd_rate=0;
		}


		double rate;
		if (mindata.OpenInterest>0){
			rate=(double)total_volume/mindata.OpenInterest;
		}
		else{
			rate=0;
		}


		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * commission.Margin /100000000.0;
		 double averageprice = mysqlhismindata.GetAveragePricebyMinuteTime(this->GetTable_HisMindata().c_str(),
							 maincontractdata.VolumeMultiple,mindata.MinuteTime);


		 if( opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {
//			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
//			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

//			 bool vol =preatr20_data.ATR /mindata.price >0.04  && preatr20_data.ATR /mindata.price <0.05;
//			 bool vol =preatr20_data.ATR /mindata.price <0.03;


			 std::string tmp_time = mindata.MinuteTime;
			char time[20];
			sprintf(time,"%s",tmp_time.substr(11,8).c_str());

			if(!this->condition_OpenTime(&mindata,5)){
				continue;
			}

			 this->SetStopRange(atr20*this->GetStopRatio());

//			 bool long_kline = mindata.price>today.Open && today.Open > preday.Close ;
//			 bool short_kline = mindata.price< today.Open && today.Open < preday.Close  ;
//			 bool condition_long = mindata.price > cycledata.High20  ;
//			 bool condition_short =  mindata.price < cycledata.Low20 ;

			 bool condition_long = mindata.price > cycledata.High20  && preday.High < precycledata.High20;
			 bool condition_short =  mindata.price < cycledata.Low20  && preday.Low > precycledata.Low20;

			bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);

			bool valid_opiaddrate = opiadd_rate>-0.005;
			bool valid=sed>this->GetLimitSedimentary() && !fridaynight ;

			 if (valid&& condition_long  )	{
				 printf("\n");
//				 printf ("前一天突破做多不满足 check %s date[%s]   precycledata.High20  %.3f preday.High %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//				 			 ,precycledata.High20,preday.High);
//
				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
								 				 ,cycledata.High20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//						 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);

				 printf("	多头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
//				 printf("	多头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	多头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	多头开仓信号 ----------------->均价 : %.2f \n",averageprice);

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = 0;
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}

			 if (  valid&& condition_short    )	{
				 printf("\n");
//				 printf ("前一天突破做空不满足 check %s date[%s]   precycledata.Low20  %.3f preday.Low %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//									 ,precycledata.Low20,preday.Low);
//
				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
				 				 ,cycledata.Low20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//										 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);

				 printf("	空头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
//				 printf("	空头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	空头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	空头开仓信号 ----------------->均价 : %.2f \n",averageprice);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = 0;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 &&
				 mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && this->GetOpenFlag() && !this->GetCloseFlag())		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);

			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

//			 if (tr.Direction ==0 && mindata.price < averageprice)	 {
//				 Close(tr,mindata,"stop_average");
//			 }
//			 else if (tr.Direction ==1 && mindata.price > averageprice)	 {
//				 Close(tr,mindata,"stop_average");
//			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }

			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }

//			 if (tr.Direction ==0 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.OpenInterest<start_opi*0.97 &&mindata.price<tr.OpenPrice*0.997 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }
//			 else if (tr.Direction ==1 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.price>tr.OpenPrice*1.003 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }

		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}


void DayTradeTest::GKGZ_Open_MinuteLine()
{
//	printf ("GKGZ_Open_MinuteLine begin \n");
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	vector<HxDayLineData>  fivedays_list;

	vector<HxDayLineData>  N20_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;

//	printf ("GKGZ_Open_MinuteLine 111 \n");

	mysqldayline.Find_ListsbyDate(fivedays_list,this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5);
	mysqldayline.Find_ListsbyDate(N20_list,this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),19);

	if(N20_list.size()<19){
		return;
	}
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		this->CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	this->SetN(20);

	int n_short =10;
	int n_small =5;

	int n ;
	double k ;

	HxDayLineData preday =this->data_PreDay();
	double presettleprice = this->zb_PreSettlePrice(preday.Time);

	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,this->GetN());
	ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_short);
	ATRData preatr5_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_small);

	double weight;
	if(fivedays_list.size()==5){
	//		printf ("GKGZ_Open_MinuteLine   %ld \n",fivedays_list.size());
			weight=this->type_FiveDaysATR(fivedays_list,preatr20_data.ATR);
		}
		else{
	//		printf ("GKGZ_Open_MinuteLine ->  %ld \n",fivedays_list.size());
		}

	int pre_n ;
	double pre_k ;
	if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0)	{
		pre_k = preatr20_data.ATR/preatr10_data.ATR;
	}
	else	{
		pre_k=1.0;
	}

	pre_n=floor(this->GetN()*pre_k);
	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,pre_n);
//	 double ma50;
//	ma50 = mysqldayline.Collect_TotalMA_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50);
	double tr ;

	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

	int i=0;
	int startopi=0;
	int total_volume=0;
	double total_amount=0;
	MinuteStaticData staticdata;
	memset(&staticdata,0,sizeof(staticdata));

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(this->GetProductID().c_str());
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );
		 total_volume+=mindata.Volume;
		 total_amount+=mindata.Amount;
		 if(i==0){
			 startopi=mindata.OpenInterest;
		 }
		 i++;
		 this->update_HighLow(&mindata);
		 double averageprice= total_amount/(total_volume*(double)maincontractdata.VolumeMultiple);

		 if(averageprice==0){
			 printf("%s %s ave----------amount:%.2f   volume:%d-----------> %.2f  \n",mindata.MinuteTime,this->GetMainInstrumentID().c_str(),
					 total_amount,total_volume,averageprice);
			 return;
		 }

		 tr = this->zb_TR(preday.Close);

//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),this->GetN(),tr);
		 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short,tr);
		 double atr5=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5,tr);


//		 if(atr10 >1.0 && atr20 >1.0)		{
//			k = atr20/atr10;
//		}
//		else	{
//			k=1.0;
//		}
//		 this->SetK(k);
//		n=floor(n_long*this->GetK());
//		CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double addopi = ((double)mindata.OpenInterest-(double)startopi);
		double opiadd_rate;
		if(startopi!=0){
			opiadd_rate=(mindata.OpenInterest - startopi ) /(double)startopi;
//			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata.OpenInterest,opiadd_rate);
		}
		else{
			opiadd_rate=0;
		}


		double rate;
		if (mindata.OpenInterest>0){
			rate=(double)total_volume/mindata.OpenInterest;
		}
		else{
			rate=0;
		}


		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * commission.Margin /100000000.0;
//		 double averageprice = mysqlhismindata.GetAveragePricebyMinuteTime(this->GetTable_HisMindata().c_str(),
//							 maincontractdata.VolumeMultiple,mindata.MinuteTime);


		 staticdata.MinuteCount++;

//		 printf(" ave----------amount:%.2f   volume:%d-----------> %.2f    sql ave [%.2f]  \n",total_amount,total_volume,average,averageprice);

		 if(mindata.price>averageprice){
			staticdata.LongCount++;
		}
		else if (mindata.price<averageprice){
			staticdata.ShortCount++;
		}


		 double current_tp = (this->GetTodayHigh()+this->GetTodayLow()+mindata.price)/3.0;

		 double cci=0.0;
		if(N20_list.size()==19){
		//		printf ("GKGZ_Open_MinuteLine   %ld \n",fivedays_list.size());
				cci=this->zb_CCI(N20_list,current_tp);
			}
			else{
		//		printf ("GKGZ_Open_MinuteLine ->  %ld \n",fivedays_list.size());
			}

		 bool exist_order = mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);
		 if(  opentime_mode && !exist_order	 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {
//			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
//			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

//			 bool vol =preatr20_data.ATR /mindata.price >0.04  && preatr20_data.ATR /mindata.price <0.05;
//			 bool vol =preatr20_data.ATR /mindata.price <0.03;


//			 CycleNRefData  cyclenrefdata = mysqlhismindata.GetCycleNRefDatabyMinteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime);


			 // 开盘后 s 分钟 可以交易 s 支持 0 ,5,10,20分钟
			if(!this->condition_OpenTime(&mindata,5)){
				continue;
			}

			 this->SetStopRange(atr20*this->GetStopRatio());
//			 int last_3min_volume = mysqlhismindata.GetLast3MinVolumeSum(this->GetTable_HisMindata().c_str(),mindata.MinuteTime);
//
//			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime,3);
//			 int pre_3min_opi=pre_3min_data.OpenInterest;

			 bool long_kline = mindata.price>today.Open && today.Open > preday.Close  && preday.Close >presettleprice;
			 bool short_kline = mindata.price< today.Open && today.Open < preday.Close  && preday.Close <presettleprice;

//			 bool long_kline = mindata.price>today.Open && today.Open > preday.High  ;
//			 bool short_kline = mindata.price< today.Open && today.Open < preday.Low  ;

//			 bool condition_long = mindata.price > cycledata.High20  && preday.Close < precycledata.High20;
//			 bool condition_short =  mindata.price < cycledata.Low20  && preday.Close > precycledata.Low20;

//			 bool count_long = (double)staticdata.LongCount/(double)staticdata.MinuteCount >0.95;
//			 bool count_short = (double)staticdata.ShortCount/(double)staticdata.MinuteCount >0.95;
//			 bool condition_long = mindata.price > cyclenrefdata.Highest && cyclenrefdata.Highest>cyclenrefdata.Open &&count_long;
//			 bool condition_short = mindata.price < cyclenrefdata.Lowest&& cyclenrefdata.Lowest<cyclenrefdata.Open&&count_short;

			 bool condition_sed = sed >this->GetLimitSedimentary();
			 bool condition_atr = tr > atr10 && atr10 > atr20 ;
			 bool condition_atrrate= atr5/mindata.price >0.015;
			 double  jumprate = (mindata.price-preday.Close) / preday.Close;
			 bool condition_long_rate= jumprate >0.005 && jumprate <0.015;
			 bool condition_short_rate= jumprate <-0.005 && jumprate >-0.015;

			 bool count_long = (double)staticdata.LongCount/(double)staticdata.MinuteCount >0.95;
			 bool count_short = (double)staticdata.ShortCount/(double)staticdata.MinuteCount >0.95;

			 bool condition_cci = cci<100 && cci>-100;
			 bool condition_long_cci = cci>100 ;
			 bool condition_short_cci = cci<-100;
			 bool zhendang= weight <1.5;

			 double patr10 = atr10/today.Open;
			bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);
			bool valid_patr= patr10<0.025;
			bool valid_opiaddrate = opiadd_rate>-0.005;
			bool valid=condition_sed && !fridaynight ;

			 if (valid&& long_kline &&zhendang && count_long)	{

//				 printf ("前一天突破做多不满足 check %s date[%s]   precycledata.High20  %.3f preday.High %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//				 			 ,precycledata.High20,preday.High);
//
//				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//								 				 ,cycledata.High20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//						 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 if(staticdata.MinuteCount>0){
					 printf("	多头开仓信号-----current_time %s   MinuteCount: %d--> longcount:%d   shortcount: %d \n",mindata.MinuteTime,staticdata.MinuteCount,
							 staticdata.LongCount,staticdata.ShortCount);
				 }

				 printf("	多头开仓信号 ----------------->现价:%.2f  开盘价:%.2f  昨收价:%.2f  昨结价 : %.2f \n",
						 mindata.price,today.Open,preday.Close,presettleprice);
				 printf("	多头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	多头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	多头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	多头开仓信号 ----------------->均价 : %.2f \n",averageprice);

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = weight;
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}

			 if (  valid&& short_kline && zhendang && count_short )	{

//				 printf ("前一天突破做空不满足 check %s date[%s]   precycledata.Low20  %.3f preday.Low %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//									 ,precycledata.Low20,preday.Low);
//
//				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//				 				 ,cycledata.Low20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//										 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 if(staticdata.MinuteCount>0){
					 printf("	空头开仓信号-----current_time %s   MinuteCount: %d--> longcount:%d   shortcount: %d \n",mindata.MinuteTime,staticdata.MinuteCount,
							 staticdata.LongCount,staticdata.ShortCount);
				 }
				 printf("	空头开仓信号 ----------------->现价:%.2f  开盘价:%.2f  昨收价:%.2f  昨结价 : %.2f \n",
										 mindata.price,today.Open,preday.Close,presettleprice);
				 printf("	空头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	空头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	空头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	空头开仓信号 ----------------->均价 : %.2f \n",averageprice);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = weight;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && exist_order &&	 this->GetOpenFlag() && !this->GetCloseFlag())	 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);



			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

//			 if (tr.Direction ==0 && mindata.price < averageprice)	 {
//				 Close(tr,mindata,"stop_average");
//			 }
//			 else if (tr.Direction ==1 && mindata.price > averageprice)	 {
//				 Close(tr,mindata,"stop_average");
//			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }

			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }

//			 if (tr.Direction ==0 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.OpenInterest<start_opi*0.97 &&mindata.price<tr.OpenPrice*0.997 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }
//			 else if (tr.Direction ==1 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.price>tr.OpenPrice*1.003 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }

		 }
	 }

	vector<MinuteLineData>().swap(min_list);
	vector<HxDayLineData>().swap(fivedays_list);
	vector<HxDayLineData>().swap(N20_list);
}
void DayTradeTest::CCI_Test()
{
//	printf ("GKGZ_Open_MinuteLine begin \n");
	printf ("\n");
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;


	vector<HxDayLineData>  N20_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;

//	printf ("GKGZ_Open_MinuteLine 111 \n");

	mysqldayline.Find_ListsbyDate(N20_list,this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),19);
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		this->CheckErrorData(this->GetProductID().c_str());
	}


	double highest=today.Open;
	double lowest=today.Open;
	int i=0;

	if(N20_list.size()<19){
		return;
	}
	 for (auto &mindata:min_list)	 {

		 i++;
				 if(mindata.price>highest)		 {
					 highest=mindata.price;
		//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
				 }

				 if(mindata.price<lowest)		 {
					 lowest=mindata.price;
		//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
				 }
		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 double current_tp = (highest+lowest+mindata.price)/3.0;
		 if ( t_current>t_endtime)	 {
			 double weight;
			if(N20_list.size()==19){
			//		printf ("GKGZ_Open_MinuteLine   %ld \n",fivedays_list.size());
					weight=this->zb_CCI(N20_list,current_tp);
				}
				else{
			//		printf ("GKGZ_Open_MinuteLine ->  %ld \n",fivedays_list.size());
				}
		 }


	 }

	vector<MinuteLineData>().swap(min_list);
	vector<HxDayLineData>().swap(N20_list);
}
void DayTradeTest::MinuteCycleN_Open_MinuteLine()
{
//	printf ("CycleN_Open_MinuteLine begin \n");
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		this->CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	int n_long =20;
	int n_short =10;
	int n_small =5;

	int n ;
	double k ;

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_long);
	ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_short);
//	ATRData preatr5_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_small);
	int pre_n ;
	double pre_k ;
	if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0)	{
		pre_k = preatr20_data.ATR/preatr10_data.ATR;
	}
	else	{
		pre_k=1.0;
	}

	pre_n=floor(n_long*pre_k);
	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,pre_n);
//	 double ma50;
//	ma50 = mysqldayline.Collect_TotalMA_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50);
	double tr ;

	double highest=today.Open;
	double lowest=today.Open;
	int i=0;
	int startopi=0;
	int total_volume=0;
	double total_amount=0;
	MinuteStaticData staticdata;
	memset(&staticdata,0,sizeof(staticdata));

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(this->GetProductID().c_str());
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );
		 total_volume+=mindata.Volume;
		 total_amount+=mindata.Amount;
		 if(i==0){
			 startopi=mindata.OpenInterest;
		 }
		 i++;
		 if(mindata.price>highest)		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }


		 double averageprice= total_amount/(total_volume*(double)maincontractdata.VolumeMultiple);

		 if(averageprice==0){
			 printf("%s %s ave----------amount:%.2f   volume:%d-----------> %.2f  \n",mindata.MinuteTime,this->GetMainInstrumentID().c_str(),
					 total_amount,total_volume,averageprice);
			 return;
		 }

		 double tr_highlow=abs(highest-lowest);
		 double tr_highclose=abs(highest-preday.Close);
		 double tr_lowclose=abs(lowest-preday.Close);

		 tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long,tr);
		 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short,tr);
		 double atr5=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5,tr);


//		 if(atr10 >1.0 && atr20 >1.0)		{
//			k = atr20/atr10;
//		}
//		else	{
//			k=1.0;
//		}
//		 this->SetK(k);
//		n=floor(n_long*this->GetK());
//		CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double addopi = ((double)mindata.OpenInterest-(double)startopi);
		double opiadd_rate;
		if(startopi!=0){
			opiadd_rate=(mindata.OpenInterest - startopi ) /(double)startopi;
//			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata.OpenInterest,opiadd_rate);
		}
		else{
			opiadd_rate=0;
		}


		double rate;
		if (mindata.OpenInterest>0){
			rate=(double)total_volume/mindata.OpenInterest;
		}
		else{
			rate=0;
		}


		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * commission.Margin /100000000.0;
//		 double averageprice = mysqlhismindata.GetAveragePricebyMinuteTime(this->GetTable_HisMindata().c_str(),
//							 maincontractdata.VolumeMultiple,mindata.MinuteTime);


		 staticdata.MinuteCount++;

//		 printf(" ave----------amount:%.2f   volume:%d-----------> %.2f    sql ave [%.2f]  \n",total_amount,total_volume,average,averageprice);

		 if(mindata.price>averageprice){
			staticdata.LongCount++;
		}
		else if (mindata.price<averageprice){
			staticdata.ShortCount++;
		}




		 if( i>3 && opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {
//			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
//			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

//			 bool vol =preatr20_data.ATR /mindata.price >0.04  && preatr20_data.ATR /mindata.price <0.05;
//			 bool vol =preatr20_data.ATR /mindata.price <0.03;

			 // 开盘后 s 分钟 可以交易 s 支持 0 ,5,10,20分钟



			 CycleNRefData  cyclenrefdata = mysqlhismindata.GetCycleNRefDatabyMinteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime);


			if(!this->condition_OpenTime(&mindata,20)){
				continue;
			}


			 this->SetStopRange(atr20*this->GetStopRatio());
			 int last_3min_volume = mysqlhismindata.GetLast3MinVolumeSum(this->GetTable_HisMindata().c_str(),mindata.MinuteTime);

			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime,3);
			 int pre_3min_opi=pre_3min_data.OpenInterest;

			 bool long_kline = mindata.price>today.Open && today.Open > preday.Close ;
			 bool short_kline = mindata.price< today.Open && today.Open < preday.Close  ;
//			 bool condition_long = mindata.price > cycledata.High20  && preday.Close < precycledata.High20;
//			 bool condition_short =  mindata.price < cycledata.Low20  && preday.Close > precycledata.Low20;

			 bool count_long = (double)staticdata.LongCount/(double)staticdata.MinuteCount >0.95;
			 bool count_short = (double)staticdata.ShortCount/(double)staticdata.MinuteCount >0.95;
			 bool condition_long = mindata.price > cyclenrefdata.Highest && cyclenrefdata.Highest>cyclenrefdata.Open;
			 bool condition_short = mindata.price < cyclenrefdata.Lowest&& cyclenrefdata.Lowest<cyclenrefdata.Open;

			 bool condition_sed = sed >this->GetLimitSedimentary();
			 bool condition_atr = tr > atr10 && atr10 > atr20 ;
			 bool condition_atrrate= atr5/mindata.price >0.015;

			 double patr10 = atr10/today.Open;
			bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);
			bool valid_patr= patr10<0.025;
			bool valid_opiaddrate = opiadd_rate>-0.005;
			bool valid=condition_sed && !fridaynight ;

			 if (valid&& condition_long  )	{

//				 printf ("前一天突破做多不满足 check %s date[%s]   precycledata.High20  %.3f preday.High %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//				 			 ,precycledata.High20,preday.High);
//
//				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//								 				 ,cycledata.High20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//						 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 if(staticdata.MinuteCount>0){
					 printf("	多头开仓信号-----current_time %s   MinuteCount: %d--> longcount:%d   shortcount: %d \n",mindata.MinuteTime,staticdata.MinuteCount,
							 staticdata.LongCount,staticdata.ShortCount);
				 }
				 printf("	多头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	多头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	多头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	多头开仓信号 ----------------->均价 : %.2f \n",averageprice);

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = patr10;
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}

			 if (  valid&& condition_short    )	{

//				 printf ("前一天突破做空不满足 check %s date[%s]   precycledata.Low20  %.3f preday.Low %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//									 ,precycledata.Low20,preday.Low);
//
//				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//				 				 ,cycledata.Low20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//										 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 if(staticdata.MinuteCount>0){
					 printf("	多头开仓信号-----current_time %s   MinuteCount: %d--> longcount:%d   shortcount: %d \n",mindata.MinuteTime,staticdata.MinuteCount,
							 staticdata.LongCount,staticdata.ShortCount);
				 }
				 printf("	空头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	空头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	空头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	空头开仓信号 ----------------->均价 : %.2f \n",averageprice);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = patr10;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);


			 CycleNRefData  cyclenrefdata = mysqlhismindata.GetCycleNRefDatabyNMinteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime,10);

			 if (tr.Direction ==0 && mindata.price < cyclenrefdata.Lowest)	 {
				 Close(tr,mindata,"stop_cycle");
			 }
			 else if (tr.Direction ==1 && mindata.price > cyclenrefdata.Highest)	 {
				 Close(tr,mindata,"stop_cycle");
			 }


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
//
//			 if (tr.Direction ==0 && mindata.price < averageprice)	 {
//				 Close(tr,mindata,"stop_average");
//			 }
//			 else if (tr.Direction ==1 && mindata.price > averageprice)	 {
//				 Close(tr,mindata,"stop_average");
//			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }

//			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
//				 Close(tr,mindata,"stop_win");
//			  }
//			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )	 {
//				 Close(tr,mindata,"stop_win");
//			  }

//			 if (tr.Direction ==0 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.OpenInterest<start_opi*0.97 &&mindata.price<tr.OpenPrice*0.997 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }
//			 else if (tr.Direction ==1 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.price>tr.OpenPrice*1.003 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }

		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::MinuteCycleN_AverageBack_Open_MinuteLine()
{
//	printf ("CycleN_Open_MinuteLine begin \n");
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		this->CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	double tr ;

	double highest=today.Open;
	double lowest=today.Open;
	int i=0;
	int startopi=0;
	int total_volume=0;
	MinuteStaticData staticdata;
	memset(&staticdata,0,sizeof(staticdata));


	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(this->GetProductID().c_str());
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );
		 total_volume+=mindata.Volume;
		 if(i==0){
			 startopi=mindata.OpenInterest;
		 }
		 i++;
		 if(mindata.price>highest)		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }

		 double tr_highlow=abs(highest-lowest);
		 double tr_highclose=abs(highest-preday.Close);
		 double tr_lowclose=abs(lowest-preday.Close);

		 tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,tr);
		 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,tr);
		 double atr5=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5,tr);

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double addopi = ((double)mindata.OpenInterest-(double)startopi);
		double opiadd_rate;
		if(startopi!=0){
			opiadd_rate=(mindata.OpenInterest - startopi ) /(double)startopi;
//			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata.OpenInterest,opiadd_rate);
		}
		else{
			opiadd_rate=0;
		}


		double rate;
		if (mindata.OpenInterest>0){
			rate=(double)total_volume/mindata.OpenInterest;
		}
		else{
			rate=0;
		}


		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * commission.Margin /100000000.0;
		 double averageprice = mysqlhismindata.GetAveragePricebyMinuteTime(this->GetTable_HisMindata().c_str(),
							 maincontractdata.VolumeMultiple,mindata.MinuteTime);
		 staticdata.MinuteCount++;

		 if(mindata.price>averageprice){
			staticdata.LongCount++;
		}
		else if (mindata.price<averageprice){
			staticdata.ShortCount++;
		}

		 if( i>3 && opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {
//			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
//			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

//			 bool vol =preatr20_data.ATR /mindata.price >0.04  && preatr20_data.ATR /mindata.price <0.05;
//			 bool vol =preatr20_data.ATR /mindata.price <0.03;


			 CycleNRefData  cyclenrefdata = mysqlhismindata.GetCycleNRefDatabyMinteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime);
//			 MinuteStaticData staticdata = mysqlhismindata.GetMinuteStaticDatabyMinteTime(this->GetTable_HisMindata().c_str(),
//					 maincontractdata.VolumeMultiple,mindata.MinuteTime);


			 std::string tmp_time = mindata.MinuteTime;
			char time[20];
			sprintf(time,"%s",tmp_time.substr(11,8).c_str());

			double Q_BarTime_2 =  dateutil.ConvertMinuteTimetoBarTime(time);
			bool night =  Q_BarTime_2 > 0.212000 && Q_BarTime_2 <=0.235959;
			bool night2 =  Q_BarTime_2 > 0.000000 && Q_BarTime_2 < 0.023000;
			bool day = Q_BarTime_2 > 0.092000 && Q_BarTime_2 < 0.143000;
			bool valid_opentime =false;
			CodeUtil codeutil;
			if(codeutil.IsNightByProductid(this->GetProductID().c_str())  ){


				if ( night || night2 || day){
//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
					valid_opentime=true;
				}
				else{
//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
					continue;
				}
			}
			else if(!codeutil.IsNightByProductid(this->GetProductID().c_str())  ){
				if (day){
//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
					valid_opentime=true;
				}
				else{
//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
					continue;
				}
			}

			 this->SetStopRange(atr20*this->GetStopRatio());

			 bool short_kline = mindata.price>today.Open && today.Open > preday.Close && sed >2;
			 bool long_kline = mindata.price< today.Open && today.Open < preday.Close  && sed >2;

			 bool condition_short = mindata.price > cyclenrefdata.Highest && cyclenrefdata.Highest>cyclenrefdata.Open &&staticdata.ShortCount!=0;
			 bool condition_long = mindata.price < cyclenrefdata.Lowest&& cyclenrefdata.Lowest<cyclenrefdata.Open &&staticdata.LongCount!=0;

			 bool condition_atr = tr > atr10 && atr10 > atr20 ;
			 bool condition_atrrate= atr5/mindata.price >0.015;

			 double patr10 = atr10/today.Open;
			bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);
			bool valid_patr= patr10<0.025;
			bool valid_opiaddrate = opiadd_rate>-0.005;
			bool valid=sed>this->GetLimitSedimentary() && !fridaynight && valid_patr && valid_opiaddrate;

			 if (valid&& condition_long && long_kline )	{

//				 printf ("前一天突破做多不满足 check %s date[%s]   precycledata.High20  %.3f preday.High %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//				 			 ,precycledata.High20,preday.High);
//
//				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//								 				 ,cycledata.High20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//						 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 if(staticdata.MinuteCount>0){
					 printf("	多头开仓信号-----current_time %s   MinuteCount: %d--> longcount:%d   shortcount: %d \n",mindata.MinuteTime,staticdata.MinuteCount,
							 staticdata.LongCount,staticdata.ShortCount);
				 }
				 printf("	多头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	多头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	多头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	多头开仓信号 ----------------->均价 : %.2f \n",averageprice);

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = patr10;
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}

			 if (  valid&& condition_short  && short_kline  )	{

//				 printf ("前一天突破做空不满足 check %s date[%s]   precycledata.Low20  %.3f preday.Low %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
//									 ,precycledata.Low20,preday.Low);
//
//				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
//				 				 ,cycledata.Low20,mindata.price);
//				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
//										 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 printf("\n");
				 if(staticdata.MinuteCount>0){
					 printf("	空头开仓信号-----current_time %s   MinuteCount: %d--> longcount:%d   shortcount: %d \n",mindata.MinuteTime,staticdata.MinuteCount,
							 staticdata.LongCount,staticdata.ShortCount);
				 }
				 printf("	空头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  Open_VolOpiRate : %.2f%% \n",
											 total_volume,startopi,mindata.OpenInterest,rate*100);
				 printf("	空头开仓信号 ----------------->波动率 : %.2f%% \n", patr10*100);
				 printf("	空头开仓信号 ----------------->增仓比 : %.2f%% \n", opiadd_rate*100);
				 printf("	空头开仓信号 ----------------->均价 : %.2f \n",averageprice);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
				 data.pATR = patr10;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopWinPrice(&tr,&mindata);



			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

			 if (tr.Direction ==0 && mindata.price > averageprice)	 {
				 Close(tr,mindata,"stop_average");
			 }
			 else if (tr.Direction ==1 && mindata.price < averageprice)	 {
				 Close(tr,mindata,"stop_average");
			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }

			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }

//			 if (tr.Direction ==0 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.OpenInterest<start_opi*0.97 &&mindata.price<tr.OpenPrice*0.997 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }
//			 else if (tr.Direction ==1 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.price>tr.OpenPrice*1.003 )	 {
//				 Close(tr,mindata,"stop_volopi");
//			  }

		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::CycleNVolumeOpi_Open_MinuteLine()
{

//	printf ("CycleN_Open_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 10:15:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else	{
//		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	int n_long =20;
	int n_short =10;
	int n_small =5;

	int n ;
	double k ;

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_long);
	ATRData preatr10_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_short);
//	ATRData preatr5_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_small);
	int pre_n ;
	double pre_k ;
	if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0)	{
		pre_k = preatr20_data.ATR/preatr10_data.ATR;
	}
	else	{
		pre_k=1.0;
	}

	pre_n=floor(n_long*pre_k);
	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,pre_n);
//	 double ma50;
//	ma50 = mysqldayline.Collect_TotalMA_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50);
	double tr ;

	double highest=today.Open;
	double lowest=today.Open;
	int i=0;
	int start_opi=0;

	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 if(i==0){
			 start_opi=mindata.OpenInterest;
		 }
i++;
		 if(mindata.price>highest)		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }

		 double tr_highlow=abs(highest-lowest);
		 double tr_highclose=abs(highest-preday.Close);
		 double tr_lowclose=abs(lowest-preday.Close);

		 tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long,tr);
		 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short,tr);


		 if(atr10 >1.0 && atr20 >1.0)		{
			k = atr20/atr10;
		}
		else	{
			k=1.0;
		}
		 this->SetK(k);
		n=floor(n_long*this->GetK());
		CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		 if( i>3 && opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {
//			bool condition_capital_in = dif_volume > dif_opi*2  && dif_volume>0 && dif_opi>0  && amount_minute >5.0  && capital_opi >0.5 ;
//			bool condition_capital_out =dif_volume < dif_opi*2  && dif_volume<0 && dif_opi<0 && amount_minute <-5.0  && capital_opi <-0.5 ;

//			 bool vol =preatr20_data.ATR /mindata.price >0.04  && preatr20_data.ATR /mindata.price <0.05;
//			 bool vol =preatr20_data.ATR /mindata.price <0.03;
			 this->SetStopRange(atr20*this->GetStopRatio());
			 int last_3min_volume = mysqlhismindata.GetLast3MinVolumeSum(tablename.c_str(),mindata.MinuteTime);

			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,3);
			 int pre_3min_opi=pre_3min_data.OpenInterest;

			 bool long_kline = mindata.price>today.Open && today.Open > preday.Close && sed >2;
			 bool short_kline = mindata.price< today.Open && today.Open < preday.Close  && sed >2;
			 bool condition_long = mindata.price > cycledata.High20  && preday.Close < precycledata.High20;
			 bool condition_short =  mindata.price < cycledata.Low20  && preday.Close > precycledata.Low20;
			 bool condition_vol =last_3min_volume>mindata.OpenInterest*0.04;
			 bool condition_opi=mindata.OpenInterest>pre_3min_opi;
			 double rate=(double)last_3min_volume/((double)mindata.OpenInterest-(double)pre_3min_opi);
			 double amount=((double)mindata.OpenInterest-(double)pre_3min_opi)*mindata.price* (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

			 bool condition_volopirate = rate<2 && rate>0.5;
			 bool condition_atr = tr > atr10 && atr10 > atr20;
			 if (condition_long && condition_atr && long_kline && condition_vol &&condition_opi&&condition_volopirate&& i>3)	{

				 printf ("前一天突破做多不满足 check %s date[%s]   precycledata.High20  %.3f preday.High %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
				 			 ,precycledata.High20,preday.High);

				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
								 				 ,cycledata.High20,mindata.price);
				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
						 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
//				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
//				 data.pATR = patr10;

				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}

			 if (  condition_short && condition_atr &&  short_kline && condition_vol &&condition_opi&& condition_volopirate&& i>3)	{

				 printf ("前一天突破做空不满足 check %s date[%s]   precycledata.Low20  %.3f preday.Low %.3f \n",this->GetMainInstrumentID().c_str(),preday.Time
									 ,precycledata.Low20,preday.Low);

				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
				 				 ,cycledata.Low20,mindata.price);
				 printf("  3分钟成交量总计:%d   3分钟前持仓量:%d  持仓量: %d    成交量持仓量比:%.2f  dif_opi_amount:%.2f \n",
										 last_3min_volume,pre_3min_opi,mindata.OpenInterest,rate,amount);
//				 printf("BigVolume_Open_MinuteLine   listsize--> %d  dif_opi %d  : front[%d] -back[%d] \n",
//				 				listsize,dif_opi,minutelists.front().OpenInterest,minutelists.back().OpenInterest);
//				 printf("BigVolume_Open_MinuteLine   成交量:%d  成交金额  %.2f 持仓量变化: %d 资金流入流出 %.2f   \n",
//						 dif_volume,amount_minute,dif_opi,capital_opi);
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
//				 data.Open_AddOPIRate = opiadd_rate;
				 data.Open_VolOpiRate= rate;
//				 data.pATR = patr10;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");

			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");

			 }

			 if (tr.Direction ==0 && t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");

			 }
			 else if (tr.Direction ==1 &&  t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }

			 if (tr.Direction ==0 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.OpenInterest<start_opi*0.97 &&mindata.price<tr.OpenPrice*0.997 )	 {
				 Close(tr,mindata,"stop_volopi");
			  }
			 else if (tr.Direction ==1 && mindata.Volume > mindata.OpenInterest*0.008 &&mindata.price>tr.OpenPrice*1.003 )	 {
				 Close(tr,mindata,"stop_volopi");
			  }

		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::VolumeOPI_Open_MinuteLine()
{

//	printf ("VolumeOPI_Open_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
		sprintf(moringtime,"%s-%s-%s 11:25:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
		time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	int total_volume=0;
	int start_opi=0;
	int i=0;

	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
		if(i==0){
			start_opi = mindata.OpenInterest;
		}
		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		total_volume+=mindata.Volume;
		i++;
		 if( opentime_mode &&  t_current<t_moringtime &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {

//			 printf("  成交量:%d   持仓量:%d   \n",total_volume,mindata.OpenInterest);
			 bool condition_long =total_volume > mindata.OpenInterest *0.7 &&mindata.OpenInterest >start_opi*1.01 && mindata.price>today.Open*1.005;
			 bool condition_short =  total_volume > mindata.OpenInterest *0.7
					 &&mindata.OpenInterest <start_opi*0.99 && mindata.price<today.Open*0.995;

			 this->SetStopRange(mindata.price*0.01);
			 if (condition_long &&  mindata.price>today.Open  && sed >2 )	{
				 printf("  成交量:%d   持仓量:%d   >>>>>>>开多 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open");
			 }

			 if (  condition_short && mindata.price< today.Open &&   sed >2 )	{
				 printf("  成交量:%d   持仓量:%d   >>>>>>>开空 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d  \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open");
			 }
		 }

		 else if (this->GetOpenTimeMode() ==0 &&
				 mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && this->GetOpenFlag() && !this->GetCloseFlag())		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
//			 printf("	当日有开仓,持仓中   ------- 现价: %.3f \n",mindata.price);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

			 if (tr.Direction ==0 && t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }
			 else if (tr.Direction ==1 &&  t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }


//			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
//				 Close(tr,mindata,"stop_win");
//			  }
//			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
//				 Close(tr,mindata,"stop_win");
//			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
}
string DayTradeTest::type_FiveDay(vector<FiveDayLineData> fivedaydata_list,MinuteLineData *mindata,int direction)
{
	MysqlDayLine mysqldayline;
	 fivedaydata_list.clear();
	mysqldayline.Find_FiveDayLineDataListsbyDate(fivedaydata_list,this->GetMainInstrumentID().c_str(),
			this->GetCusor_DaySqlTime().c_str(),this->GetN()-1);
	printf("  fiveday_list size  %ld  \n",fivedaydata_list.size());

	 FiveDayLineData fivedaylinedata;
	 fivedaylinedata.Number=5;
	 fivedaylinedata.Close=mindata->price;
	 strcpy(fivedaylinedata.Time,this->GetCusor_DaySqlTime().c_str());
	 fivedaydata_list.push_back(fivedaylinedata);

	 for (auto &data:fivedaydata_list){

		printf("Find_FiveDayLineDataListsbyDate  number:%d   instrumentid:%s    time:%s close:%.3f  \n",
				data.Number,this->GetMainInstrumentID().c_str(),data.Time,data.Close);

	}

	if(direction==0){
		 sort(fivedaydata_list.begin(),fivedaydata_list.end(),this->LongSortbyNumber);
	}
	else if(direction==1){
		 sort(fivedaydata_list.begin(),fivedaydata_list.end(),this->ShortSortbyNumber);
	}

	 string fivedaytype ;
	 for (auto &data:fivedaydata_list){

			printf("排序后  number:%d   instrumentid:%s    time:%s close:%.3f  \n",
					data.Number,this->GetMainInstrumentID().c_str(),data.Time,data.Close);
			char tmp[5] ;
			sprintf(tmp,"%d",data.Number);
			fivedaytype.append(tmp);

		}

	printf(" 当前的5日线形态--------------> %s  \n",fivedaytype.c_str());
	return fivedaytype;
}
string DayTradeTest::type_FiveMinOPI(MinuteLineData *mindata,int direction)
{
	vector<MinuteLineData>  min_list;
	vector<TypeMinuteLineData>  typemin_list;
	MysqlHisMinData mysqlhismindata;

	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());
	mysqlhismindata.Find_ListsNbyMinuteTime(min_list,this->GetTable_HisMindata().c_str(),mindata->MinuteTime,5);
string type;
int n=1;
	 for (auto &data:min_list){

		 TypeMinuteLineData tmd;
		 tmd.OpenInterest = data.OpenInterest;
		 tmd.Number=n;
		 strcpy(tmd.MinuteTime,data.MinuteTime);
		 n++;
		 typemin_list.push_back(tmd);
		printf("type_FiveMinOPI  number:%d   instrumentid:%s    time:%s opi:%d  \n",
				tmd.Number,this->GetMainInstrumentID().c_str(),data.MinuteTime,data.OpenInterest);

	}
	 sort(typemin_list.begin(),typemin_list.end(),this->Min_LongSortbyNumber);
	 for (auto &data:typemin_list){
		 printf("排序后  number:%d   instrumentid:%s    time:%s opi:%d  \n",
				data.Number,this->GetMainInstrumentID().c_str(),data.MinuteTime,data.OpenInterest);
		char tmp[5] ;
		sprintf(tmp,"%d",data.Number);
		type.append(tmp);
	 }

	 printf("      type ---------> %s   \n",type.c_str());
//
//		if(direction==0){
//			 sort(fivedaydata_list.begin(),fivedaydata_list.end(),this->LongSortbyNumber);
//		}
//		else if(direction==1){
//			 sort(fivedaydata_list.begin(),fivedaydata_list.end(),this->ShortSortbyNumber);
//		}

	vector<MinuteLineData>().swap(min_list);
	vector<TypeMinuteLineData>().swap(typemin_list);
	return type;
}
void DayTradeTest::Statistics_Ins_MinuteLine()
{

//	printf ("Statistics_Ins_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());


	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

//	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());


	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);



//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{

		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
		if(min_list.size()>0){

		}
		else{

			printf(" >>>分钟数据   ---> %ld    \n",min_list.size());
			return;
		}

	}
	else	{
		printf(" >>>空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	MinuteLineData lastmin=mysqlhismindata.Find_LastData(tablename.c_str());
	double sed=(double)lastmin.OpenInterest * lastmin.price *(double)maincontractdata.VolumeMultiple *
			maincontractdata.LongMarginRatio /100000000.0;


	ProductStatisticsData pdata;
	memset(&pdata,0,sizeof(pdata));
	MysqlProStatis mysqlprostatis;


	std::string tablename_ps = this->GetProductID();
	tablename_ps.append("_");
	tablename_ps.append(this->GetYear());


//	printf(" %ld   \n",strlen(lastmin.MinuteTime));
	if (strlen(lastmin.MinuteTime)>8 && sed>0.0){
		pdata.Sedimentary =sed;
		pdata.Year=atoi(this->GetYear().c_str());
		strcpy(pdata.ProductID,this->GetProductID().c_str());
		strcpy(pdata.InstrumentID,this->GetMainInstrumentID().c_str());
		strcpy(pdata.Time,dateutil.ConvertSqlTimetoDate(lastmin.MinuteTime).c_str());
		printf(" %s   [%s] min lastopi--> %d    %.3f    sed:%.3f \n",this->GetMainInstrumentID().c_str(),pdata.Time,
					lastmin.OpenInterest,lastmin.price,sed);

		mysqlprostatis.Insert(tablename_ps.c_str(),pdata);

	}
	else{
		printf("error>>>>>>>>> %s   [%s] min lastopi--> %d    %.3f    sed:%.3f \n",this->GetMainInstrumentID().c_str(),lastmin.MinuteTime,
					lastmin.OpenInterest,lastmin.price,sed);

	}

	//
//	 for (auto &mindata:min_list)	 {
////		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );
//
//		 std::string mintime=mindata.MinuteTime;
//		 char time[20];  // 格式"2017-09-06 18:32:00"   --> "18:32:00"
//		sprintf(time,"%s",mintime.substr(11,8).c_str());
//		double Q_BarTime_2 = dateutil.ConvertMinuteTimetoBarTime(time);
//
//		//早盘开盘后5分钟内不开仓
//		bool Open_morning1 = (Q_BarTime_2>=0.090500 && Q_BarTime_2<0.093100);
//
//		//夜盘五分钟内不开仓
//		bool Open_night = (Q_BarTime_2>=0.210500 && Q_BarTime_2<=0.235959);
//		bool Open_night1 = (Q_BarTime_2>=0.000000 && Q_BarTime_2<0.023000);
//
//		bool qtimebar_opentime=Open_morning1 || Open_night || Open_night1;
//
//
//		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
//		if(i==0){
//			start_opi = mindata.OpenInterest;
//		}
//
//		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;
//
//
//		i++;
//
//	 }

	vector<MinuteLineData>().swap(min_list);

}
void DayTradeTest::BigOneMinVolume_Open_MinuteLine()
{

//	printf ("BigOneMinVolume_Open_MinuteLine begin %s \n",this->GetMainInstrumentID().c_str());
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
//	vector<HxDayLineData>  fivedays_list;
//	vector<FiveDayLineData> fivedaydata_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	std::map<std::string ,std::shared_ptr<HighLowAnalyst>> m_hlamap;
	vector<HighLowAnalyst>  hlalist;


//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=m_mysqldayline->Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());


//	int count=2;
	HxDayLineData preday =m_mysqldayline->Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	if(strlen(preday.Time)<10){
		return;
	}
//	std::cout << "preday.Time>>>>>>>>> "<<preday.Time<<endl;


	this->SetN(3);
	CycleData cycledata = m_mysqldayline->Collect_CycleNData(this->GetMainInstrumentID().c_str(),
			this->GetCusor_DaySqlTime().c_str(),this->GetN());

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	m_pmysqlhismindata->Set_Database(this->GetDataBase_HisMindata());
//	m_pmysqlhismindata->Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 09:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	char moringbreaktime[20];
		sprintf(moringbreaktime,"%s-%s-%s 10:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
		time_t t_moringbreaktime = dateutil.ConvertSqlTimetoTimet(moringbreaktime);

	char moringbegintime[20];
	sprintf(moringbegintime,"%s-%s-%s 09:00:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringbegintime = dateutil.ConvertSqlTimetoTimet(moringbegintime);

	char moringbegin1time[20];
	sprintf(moringbegin1time,"%s-%s-%s 09:05:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringbegin1time = dateutil.ConvertSqlTimetoTimet(moringbegin1time);

	char moringendtime[20];
	sprintf(moringendtime,"%s-%s-%s 11:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringendtime = dateutil.ConvertSqlTimetoTimet(moringendtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);
	double presettleprice = this->zb_PreSettlePrice(preday.Time);


	if (m_pmysqlhismindata->Exist_Table(tablename.c_str()))	{
		m_pmysqlhismindata->Find_AllList(min_list,tablename.c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = m_pmysqlmaincontract->Find_DatabyProductID(this->GetProductID().c_str());
	int total_volume=0;
	int start_opi=0;
	int i=0;

//	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,20);

//	double weight=0.0;
//	if(fivedays_list.size()==5){
////			printf ("BigOneMinVolume_Open_MinuteLine   %ld \n",fivedays_list.size());
//			weight=this->type_FiveDaysATR(fivedays_list,preatr20_data.ATR);
//	}
//	else{
////		printf ("BigOneMinVolume_Open_MinuteLine ->  %ld \n",fivedays_list.size());
//	}

	double tr ;

	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

	bool maxvol_dis=false;
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),
//				 this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 double ema60=this->m_mysqldayline->Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
					this->GetCusor_DaySqlTime().c_str(),60,mindata.price);
		 double ema5=this->m_mysqldayline->Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
		 					this->GetCusor_DaySqlTime().c_str(),5,mindata.price);
		 if(m_hlamap.empty()){
			 if(mindata.price>today.Open){
				 HighLowAnalyst data;
				 data.MinuteTime=mindata.MinuteTime;
				 data.Direction=0;
				 data.last_highlow=today.Open;
				 data.HighLowPrice=mindata.price;
				 data.HLOpenRate=(data.HighLowPrice-data.last_highlow)/data.last_highlow;
				 std::string key= data.MinuteTime;
				 std::shared_ptr<HighLowAnalyst> pdata=std::make_shared<HighLowAnalyst>(data);
				 m_hlamap.insert(std::pair<std::string,std::shared_ptr<HighLowAnalyst>>(key,pdata));
				 printf("--------插入第一个数据:  %s    %d    %.2f  -->开盘价:%.2f   rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
				 						 data.HighLowPrice,today.Open,data.HLOpenRate*100);
			 }
			 else if(mindata.price<today.Open){
				 HighLowAnalyst data;
				 data.MinuteTime=mindata.MinuteTime;
				 data.Direction=1;
				 data.last_highlow=today.Open;
				 data.HighLowPrice=mindata.price;
				 data.HLOpenRate=(data.HighLowPrice-data.last_highlow)/data.last_highlow;
				 std::string key= data.MinuteTime;
				 std::shared_ptr<HighLowAnalyst> pdata=std::make_shared<HighLowAnalyst>(data);
				 m_hlamap.insert(std::pair<std::string,std::shared_ptr<HighLowAnalyst>>(key,pdata));
				 printf("--------插入第一个数据:  %s    %d    %.2f   -->开盘价:%.2f     rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
						 data.HighLowPrice,today.Open,data.HLOpenRate*100);
			 }
		 }
		 else{

			 map<std::string,std::shared_ptr<HighLowAnalyst>>::reverse_iterator iter = m_hlamap.rbegin();
			if(iter!=m_hlamap.rend()){
//				printf("--------最后一个数据:  %s    %d    %.2f\n",iter->second->MinuteTime.c_str(),iter->second->Direction,
//						iter->second->HighLowPrice);

				if( mindata.price>today.Open&&mindata.price>iter->second->HighLowPrice){

					if(iter->second->Direction==0){
						iter->second->HighLowPrice=mindata.price;
						iter->second->MinuteTime=mindata.MinuteTime;
						iter->second->HLOpenRate=(iter->second->HighLowPrice-iter->second->last_highlow)/iter->second->last_highlow;
//						printf("--------多头新高 更新数据:  %s    %d    %.2f    -->开盘价:%.2f     rate:%.2f%%\n",iter->second->MinuteTime.c_str(),
//								iter->second->Direction,iter->second->HighLowPrice,today.Open,iter->second->HLOpenRate*100);
					}
					else if(iter->second->Direction==1){
						 HighLowAnalyst data;
						 data.MinuteTime=mindata.MinuteTime;
						 data.Direction=0;
						 data.last_highlow=iter->second->HighLowPrice;
						 data.HighLowPrice=mindata.price;
						 data.HLOpenRate=(data.HighLowPrice-data.last_highlow)/data.last_highlow;
						 std::string key= data.MinuteTime;
						 std::shared_ptr<HighLowAnalyst> pdata=std::make_shared<HighLowAnalyst>(data);
						 m_hlamap.insert(std::pair<std::string,std::shared_ptr<HighLowAnalyst>>(key,pdata));
						 printf("--------空头转多头---->插入新的方向数据:  %s    %d    %.2f    -->开盘价:%.2f     rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
								 data.HighLowPrice,today.Open,data.HLOpenRate*100);
					}


				}
				else if( mindata.price<today.Open&&mindata.price<iter->second->HighLowPrice){
					if(iter->second->Direction==1 ){
						iter->second->HighLowPrice=mindata.price;
						iter->second->MinuteTime=mindata.MinuteTime;
						iter->second->HLOpenRate=(iter->second->HighLowPrice-iter->second->last_highlow)/iter->second->last_highlow;
//						printf("--------空头新低  更新数据:  %s    %d    %.2f     -->开盘价:%.2f     rate:%.2f%%\n",iter->second->MinuteTime.c_str(),iter->second->Direction,
//						iter->second->HighLowPrice,today.Open,iter->second->HLOpenRate*100);
					}
					else if(iter->second->Direction==0){
						 HighLowAnalyst data;
						 data.MinuteTime=mindata.MinuteTime;
						 data.Direction=1;
						 data.last_highlow=iter->second->HighLowPrice;
						 data.HighLowPrice=mindata.price;
						 data.HLOpenRate=(data.HighLowPrice-data.last_highlow)/data.last_highlow;
						 std::string key= data.MinuteTime;
						 std::shared_ptr<HighLowAnalyst> pdata=std::make_shared<HighLowAnalyst>(data);
						 m_hlamap.insert(std::pair<std::string,std::shared_ptr<HighLowAnalyst>>(key,pdata));
						 printf("--------多头转空头---->插入新的方向数据:  %s    %d    %.2f      -->开盘价:%.2f     rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
								 data.HighLowPrice,today.Open,data.HLOpenRate*100);
					}

				}

			}
		 }


		 double maxrate=0.0;
		 map<std::string,std::shared_ptr<HighLowAnalyst>>::iterator item = m_hlamap.begin();
		if(item!=m_hlamap.end()){
				if(item->second->HLOpenRate >maxrate){
					maxrate=item->second->HLOpenRate;
				}
		}
		 this->update_HighLow(&mindata);

		 std::string mintime=mindata.MinuteTime;
		 char time[20];  // 格式"2017-09-06 18:32:00"   --> "18:32:00"
		sprintf(time,"%s",mintime.substr(11,8).c_str());
		double Q_BarTime_2 = dateutil.ConvertMinuteTimetoBarTime(time);

		//早盘开盘后5分钟内不开仓
		bool Open_morning1 = (Q_BarTime_2>=0.090500 && Q_BarTime_2<0.093100);

		//夜盘五分钟内不开仓
		bool Open_night = (Q_BarTime_2>=0.210500 && Q_BarTime_2<=0.235959);
		bool Open_night1 = (Q_BarTime_2>=0.000000 && Q_BarTime_2<0.023000);

		bool qtimebar_opentime=Open_morning1 || Open_night || Open_night1;


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
		if(i==0){
			start_opi = mindata.OpenInterest;
		}
		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}



//		 if(!qtimebar_opentime){
//			continue;
//		}
//		 else{
////			 printf ("check Q_BarTime_2 : %s   -->%.6f \n",time,Q_BarTime_2);
//		 }

		 tr = this->zb_TR(preday.Close);


		 double volopirate=0.015;
//		 double attackvolume_rate = this->zb_AttackVolume(&mysqlhismindata,&mindata,volopirate);

		//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
		//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=m_mysqldayline->Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,tr);


//		 std::cout<< "  tr:"<<tr<<" atr20:"<<atr20<<endl;
		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		total_volume+=mindata.Volume;
		i++;
		//  第i根k线后开始交易
		//  限定交易时间
		//
		 bool exist_order = m_pmysqltestrecord->Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),0);

//		 bool mapsize = m_hlamap.size()<=1  fabs(maxrate)<0.0001;
		 bool mapsize = fabs(maxrate)<0.0001;
		 if(  i>5  && qtimebar_opentime &&opentime_mode  && !exist_order	 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {


//			 printf("  成交量:%d   持仓量:%d   \n",total_volume,mindata.OpenInterest);
			 MinuteLineData maxvol=m_pmysqlhismindata->Find_MaxVolumeEndbyMinuteTime(tablename.c_str(),mindata.MinuteTime);
			 MinuteLineData lastmin = m_pmysqlhismindata->GetLastMinbyMinuteTime(tablename.c_str(),mindata.MinuteTime);  //  < 前一分钟
//			 int last_min_volume = m_pmysqlhismindata->GetLastMinVolumeSum(tablename.c_str(),mindata.MinuteTime);
//			 MinuteLineData pre_min_data = m_pmysqlhismindata->Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,1);
//			 int pre_min_opi=pre_min_data.OpenInterest;
//			 double pre_min_price=pre_min_data.price;

//			 double rate=(double)last_3min_volume/((double)mindata.OpenInterest-(double)pre_3min_opi);
//			 double amount=((double)mindata.OpenInterest-(double)pre_min_opi)*mindata.price* (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

			 double average_volume = m_pmysqlhismindata->GetAverageVolumeNbyMinuteTime(tablename.c_str(),5,mindata.MinuteTime);
			 bool condition_vol =lastmin.Volume>mindata.OpenInterest*volopirate;
//			 bool condition_vol =average_volume>mindata.OpenInterest*0.15;

//			 bool condition_opi=mindata.OpenInterest>pre_min_opi;

//			 bool condition_zhendang= weight <1.5;

//			 bool condition_volopirate = rate<2 && rate>0.5;
			 bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);

//			 bool condition_long = mindata.price>today.Open  && today.Open > preday.Close  && condition_vol;
//			 bool condition_short = mindata.price<today.Open  && today.Open > preday.Close && condition_vol;

			bool con_uplimit = mindata.price<presettleprice*1.037;
			bool con_downlimit = mindata.price>presettleprice*0.963;

//			bool max_vol = maxvol.Volume >lastmin.Volume;
bool trend_long = mindata.price>ema5 && ema5> ema60;
bool trend_short = mindata.price<ema5 && ema5< ema60;

			//标准版
			 bool condition_long = trend_long&&mindata.price > cycledata.High20    && condition_vol && !fridaynight && con_uplimit ;
			 bool condition_short =trend_short&&  mindata.price < cycledata.Low20  && condition_vol && !fridaynight && con_downlimit;

//			 if(mindata.price > cycledata.High20    && condition_vol && !fridaynight && con_uplimit &&max_vol && !maxvol_dis){
//				printf("%s  ---> max time:%s  max vol:%d \n",mindata.MinuteTime,maxvol.MinuteTime,maxvol.Volume);
//				maxvol_dis=true;
//
//			}
//			 else if(mindata.price < cycledata.Low20  && condition_vol && !fridaynight && con_downlimit && max_vol&& !maxvol_dis){
//				 printf("%s  ---> max time:%s  max vol:%d \n",mindata.MinuteTime,maxvol.MinuteTime,maxvol.Volume);
//				 maxvol_dis=true;
//			 }

//			 bool condition_long = mindata.price > ma5 && condition_vol && !fridaynight && con_uplimit;
//			 bool condition_short =  mindata.price < ma5   && condition_vol && !fridaynight && con_downlimit;

//			 bool condition_long = mindata.price > cycledata.High20   && attackvolume_rate>0.2   && condition_vol;
//			 bool condition_short =  mindata.price < cycledata.Low20  && attackvolume_rate>0.2   && condition_vol;

//			 bool condition_long = mindata.price > pre_min_price    && condition_vol;
//			 bool condition_short =  mindata.price < pre_min_price  && condition_vol;

//			 bool condition_long = mindata.price > pre_min_price   && pre_min_price>today.Open  && condition_vol ;
//			 bool condition_short =  mindata.price < pre_min_price  && pre_min_price<today.Open && condition_vol;

			 this->SetStopRange(atr20*this->GetStopRatio());
//			 this->SetStopRange(atr20);

//			 bool openorder =m_pmysqltestrecord->Exist_RecordByStatus(this->GetTableRecord().c_str(),1);
//			 bool day=false;
//			 if (openorder){
//				 TestRecordData lastdata=m_pmysqltestrecord->Find_LastRecordByStatus(this->GetTableRecord().c_str(),1);
//
//				 time_t t_last = dateutil.ConvertSqlTimetoTimet(lastdata.OpenDateTime);
//				 time_t t_now = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
//				 day=t_now-t_last<37*60*60;
//				 if(day &&(condition_long||condition_short)){
//					std::cout<<"   >>>>>last order :"<<lastdata.OpenDateTime <<"    now:"<<mindata.MinuteTime<<std::endl;
//				 }
//			 }

const double limitsed =20.0;
			 if ( condition_long  && sed >limitsed )	{

				 printf("  最后一分钟成交量:%d   持仓量:%d   >>>>>>>开多 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d \n",
						 lastmin.Volume,mindata.OpenInterest,lastmin.price,today.Open,start_opi);
				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",
						 this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
						 ,cycledata.High20,mindata.price);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = lastmin.Volume;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.PreSettlePrice=presettleprice;
//				 data.pATR = attackvolume_rate;
//				 if(presettleprice!=0){
//					 data.pATR=(mindata.price-presettleprice)/presettleprice;
//					 printf("  updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
//				 else{
//					 data.pATR=0.0;
//					 printf("  error:updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;

//				 string typefivemin =type_FiveMinOPI(&mindata,0);
//				 strcpy(data.FiveDayLineType , typefivemin.c_str());
				 bvDayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open",&data,lastmin.price);


			 }

			 if ( condition_short  &&   sed >limitsed )	{

				 printf("  最后一分钟成交量:%d   持仓量:%d   >>>>>>>开空 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d  \n",
						 lastmin.Volume,mindata.OpenInterest,lastmin.price,today.Open,start_opi);
				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",
						 this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
								 ,cycledata.Low20,mindata.price);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = lastmin.Volume;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.PreSettlePrice=presettleprice;
//				 data.pATR = attackvolume_rate;
//				 if(presettleprice!=0){
//					 data.pATR=(mindata.price-presettleprice)/presettleprice;
//
//					 printf("  updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
//				 else{
//					 data.pATR=0.0;
//					 printf("  error:updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;
//				 string typefivemin =type_FiveMinOPI(&mindata,1);
//				 strcpy(data.FiveDayLineType , typefivemin.c_str());
				 bvDaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open",&data,lastmin.price);
			 }
		 }

		 else if (this->GetOpenTimeMode() ==0 && exist_order && this->GetOpenFlag() && !this->GetCloseFlag()) {

			 TestRecordData tr =m_pmysqltestrecord->Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);
			 MinuteLineData lastmin = m_pmysqlhismindata->GetLastMinbyMinuteTime(tablename.c_str(),mindata.MinuteTime);
			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
			 time_t t_open = dateutil.ConvertSqlTimetoTimet(tr.OpenDateTime);
//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
//			 printf("	当日有开仓,持仓中   ------- 现价: %.3f \n",mindata.price);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest - this->GetStopRange();
				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 m_pmysqltestrecord->UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );
				 m_pmysqltestrecord->UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 bvClose(tr,mindata,"stop_openday",mindata.price);
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 bvClose(tr,mindata,"stop_openday",mindata.price);
			 }

//			 if ( t_current>t_endtime)	 {
//				 bvClose(tr,mindata,"stop_endtime",mindata.price);
//			 }


//			 int holdlast_min_volume = m_pmysqlhismindata->GetLastMinVolumeSum(tablename.c_str(),mindata.MinuteTime);
//			 MinuteLineData hold_pre_min_data = m_pmysqlhismindata->Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,1);
//
//			 if (tr.Direction ==0 && mindata.price < hold_pre_min_data.price && holdlast_min_volume>tr.OpenOPI*0.8)	 {
//
//				 printf("	%s 多头持仓中  行情反转   当前价:%.2f  ---> 前一分钟价格:%.2f   开仓时成交量:%d   反转时成交量:%d\n",
//						 mindata.MinuteTime, mindata.price ,hold_pre_min_data.price,tr.OpenOPI,holdlast_min_volume	 );
//				 Close(tr,mindata,"stop_reverse");
//			  }
//			 else if (tr.Direction ==1 && mindata.price > hold_pre_min_data.price && holdlast_min_volume>tr.OpenOPI*0.8 )		 {
//				 printf("	%s 空头持仓中  行情反转   当前价:%.2f  ---> 前一分钟价格:%.2f   开仓时成交量:%d   反转时成交量:%d\n",
//										 mindata.MinuteTime, mindata.price ,hold_pre_min_data.price,tr.OpenOPI,holdlast_min_volume	 );
//				 Close(tr,mindata,"stop_reverse");
//			  }
//			 if (t_current-t_open>300)	 {
//				 printf("	%s 开仓日   平仓时间: %s \n",mindata.MinuteTime,tr.OpenDateTime );
//				 Close(tr,mindata,"stop_3min");
//			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 bvClose(tr,mindata,"stop_win",mindata.price);
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
				 bvClose(tr,mindata,"stop_win",mindata.price);
			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
//	vector<HxDayLineData>().swap(fivedays_list)  ;
//	 vector<FiveDayLineData>().swap(fivedaydata_list);
	 vector<HighLowAnalyst>().swap(hlalist);
}

void DayTradeTest::BigVolume_Hold_MinuteLine(const char* pinstrumentid)
{

//	printf ("BigVolume_Hold_MinuteLine begin \n");

	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;


	vector<MinuteLineData>  min_list;

	HxDayLineData today=this->m_mysqldayline->Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());

	DateUtil dateutil;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	this->m_pmysqlhismindata->Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);


	bool existorder=this->m_pmysqltestrecord->Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
	if(this->m_pmysqlhismindata->Exist_Table(tablename.c_str()))	{
		this->m_pmysqlhismindata->Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )	{

			ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
			this->SetStopRange(atr_data.ATR *this->GetStopRatio());

			 for (auto &mindata:min_list)
			 {
				 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

				 bool closetime_mode;
				if(this->GetCloseTimeMode()==1)
				{
					closetime_mode=t_current>t_endtime;
				}
				else{
					closetime_mode=true;
				}

					 if(closetime_mode && existorder )
					 {

			//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

						 TestRecordData tr =this->m_pmysqltestrecord->Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
						 double ma50 =this->m_mysqldayline->Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);


						 if (tr.Direction ==0   ) {

							 if (mindata.price < tr.StopPrice )	 {
								 Close(tr,mindata,"stop_hold");
							  }
							 else if (mindata.price < ma50)	 {
								 Close(tr,mindata,"stop_ma50");
							 }

						 }
						 else if (tr.Direction ==1    )	 {

							 if (mindata.price > tr.StopPrice)	{
								 Close(tr,mindata,"stop_hold");
							 }
							 else if (mindata.price > ma50)	 {
								 Close(tr,mindata,"stop_ema50");
							 }
						 }

					 }//exist 存在持仓单


		//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

			 }// for
		}
		else	{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(this->GetProductID().c_str());
		}


	}
	else
	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	 if( existorder) {
		 TestRecordData tr =this->m_pmysqltestrecord->Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
		 if (  tr.Direction==0 && today.Close> tr.Highest)	 {
				 tr.Highest=today.Close;
				 tr.StopPrice = tr.Highest -this->GetStopRange();
				 printf("	%s 持仓中   更新最高收盘价   -------  %.3f   止损价  %.3f \n", today.Time,tr.Highest,tr.StopPrice );
				 this->m_pmysqltestrecord->UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 pinstrumentid,today.Close,tr.StopPrice,0);
			 }
			 else if ( tr.Direction==1 && today.Close < tr.Lowest )	{
				 tr.Lowest=today.Close;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 持仓中   更新最低收盘价   -------%.3f    止损价  %.3f\n",today.Time,tr.Lowest,tr.StopPrice );
				 this->m_pmysqltestrecord->UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 pinstrumentid,today.Close,tr.StopPrice,0);
			}
	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::EMA52060_Open_MinuteLine()
{
//	printf ("BigOneMinVolume_Open_MinuteLine begin %s \n",this->GetMainInstrumentID().c_str());
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	vector<HxDayLineData>  fivedays_list;
	vector<FiveDayLineData> fivedaydata_list;

	vector<double> last5;
	vector<double>  last60;

	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);

//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=m_mysqldayline->Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());


//	int count=2;
	HxDayLineData preday =m_mysqldayline->Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	if(strlen(preday.Time)<10){
		return;
	}
//	std::cout << "preday.Time>>>>>>>>> "<<preday.Time<<endl;


	this->SetN(3);
	CycleData cycledata = m_mysqldayline->Collect_CycleNData(this->GetMainInstrumentID().c_str(),
			this->GetCusor_DaySqlTime().c_str(),this->GetN());

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	m_pmysqlhismindata->Set_Database(this->GetDataBase_HisMindata());
//	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
	sprintf(moringtime,"%s-%s-%s 09:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	char moringbreaktime[20];
		sprintf(moringbreaktime,"%s-%s-%s 10:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
		time_t t_moringbreaktime = dateutil.ConvertSqlTimetoTimet(moringbreaktime);

	char moringbegintime[20];
	sprintf(moringbegintime,"%s-%s-%s 09:00:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringbegintime = dateutil.ConvertSqlTimetoTimet(moringbegintime);

	char moringbegin1time[20];
	sprintf(moringbegin1time,"%s-%s-%s 09:05:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringbegin1time = dateutil.ConvertSqlTimetoTimet(moringbegin1time);

	char moringendtime[20];
	sprintf(moringendtime,"%s-%s-%s 11:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_moringendtime = dateutil.ConvertSqlTimetoTimet(moringendtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);
	double presettleprice = this->zb_PreSettlePrice(preday.Time);


	if (m_pmysqlhismindata->Exist_Table(tablename.c_str()))	{
		m_pmysqlhismindata->Find_AllList(min_list,tablename.c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = m_pmysqlmaincontract->Find_DatabyProductID(this->GetProductID().c_str());
	int total_volume=0;
	int start_opi=0;
	int i=0;

//	ATRData preatr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,20);

//	double weight=0.0;
//	if(fivedays_list.size()==5){
////			printf ("BigOneMinVolume_Open_MinuteLine   %ld \n",fivedays_list.size());
//			weight=this->type_FiveDaysATR(fivedays_list,preatr20_data.ATR);
//	}
//	else{
////		printf ("BigOneMinVolume_Open_MinuteLine ->  %ld \n",fivedays_list.size());
//	}

	double tr ;

	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

	bool maxvol_dis=false;
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),
//				 this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 double maxrate=0.0;

		 this->update_HighLow(&mindata);

//int nn=60;
		 double ema60=this->zb_ema(&preday,mindata.MinuteTime,i,60);
		 double ema5=this->zb_ema(&preday,mindata.MinuteTime,i,5);

		 std::string mintime=mindata.MinuteTime;
		 char time[20];  // 格式"2017-09-06 18:32:00"   --> "18:32:00"
		sprintf(time,"%s",mintime.substr(11,8).c_str());
		double Q_BarTime_2 = dateutil.ConvertMinuteTimetoBarTime(time);

		//早盘开盘后5分钟内不开仓
		bool Open_morning1 = (Q_BarTime_2>=0.090500 && Q_BarTime_2<0.093100);

		//夜盘五分钟内不开仓
		bool Open_night = (Q_BarTime_2>=0.210500 && Q_BarTime_2<=0.235959);
		bool Open_night1 = (Q_BarTime_2>=0.000000 && Q_BarTime_2<0.023000);

		bool qtimebar_opentime=Open_morning1 || Open_night || Open_night1;


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
		if(i==0){
			start_opi = mindata.OpenInterest;
		}
		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}



//		 if(!qtimebar_opentime){
//			continue;
//		}
//		 else{
////			 printf ("check Q_BarTime_2 : %s   -->%.6f \n",time,Q_BarTime_2);
//		 }

		 tr = this->zb_TR(preday.Close);


		 double volopirate=0.015;
//		 double attackvolume_rate = this->zb_AttackVolume(&mysqlhismindata,&mindata,volopirate);

		//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
		//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

		 double atr20=m_mysqldayline->Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,tr);


//		 std::cout<< "  tr:"<<tr<<" atr20:"<<atr20<<endl;
		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		total_volume+=mindata.Volume;
		i++;
		//  第i根k线后开始交易
		//  限定交易时间
		//
		 bool exist_order = m_pmysqltestrecord->Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),0);

//		 bool mapsize = m_hlamap.size()<=1  fabs(maxrate)<0.0001;
		 bool mapsize = fabs(maxrate)<0.0001;
		 if(  i>5  && qtimebar_opentime &&opentime_mode  && !exist_order	 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {


//			 printf("  成交量:%d   持仓量:%d   \n",total_volume,mindata.OpenInterest);
			 MinuteLineData maxvol=m_pmysqlhismindata->Find_MaxVolumeEndbyMinuteTime(tablename.c_str(),mindata.MinuteTime);
			 MinuteLineData lastmin = m_pmysqlhismindata->GetLastMinbyMinuteTime(tablename.c_str(),mindata.MinuteTime);  //  < 前一分钟
//			 int last_min_volume = m_pmysqlhismindata->GetLastMinVolumeSum(tablename.c_str(),mindata.MinuteTime);
//			 MinuteLineData pre_min_data = m_pmysqlhismindata->Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,1);
//			 int pre_min_opi=pre_min_data.OpenInterest;
//			 double pre_min_price=pre_min_data.price;

//			 double rate=(double)last_3min_volume/((double)mindata.OpenInterest-(double)pre_3min_opi);
//			 double amount=((double)mindata.OpenInterest-(double)pre_min_opi)*mindata.price* (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

			 bool condition_vol =lastmin.Volume>mindata.OpenInterest*volopirate;
//			 bool condition_opi=mindata.OpenInterest>pre_min_opi;

//			 bool condition_zhendang= weight <1.5;

//			 bool condition_volopirate = rate<2 && rate>0.5;
			 bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);

//			 bool condition_long = mindata.price>today.Open  && today.Open > preday.Close  && condition_vol;
//			 bool condition_short = mindata.price<today.Open  && today.Open > preday.Close && condition_vol;

			bool con_uplimit = mindata.price<presettleprice*1.037;
			bool con_downlimit = mindata.price>presettleprice*0.963;

//			bool max_vol = maxvol.Volume >lastmin.Volume;



			//标准版
//			 bool condition_long = mindata.price > cycledata.High20   && condition_vol && !fridaynight && con_uplimit ;
//			 bool condition_short =  mindata.price < cycledata.Low20  && condition_vol && !fridaynight && con_downlimit;


			//ema版
			 bool condition_long = mindata.price>ema5 && ema5>ema60  && condition_vol && !fridaynight && con_uplimit ;
			 bool condition_short = mindata.price<ema5 && ema5<ema60  && condition_vol && !fridaynight && con_downlimit;

//			 if(mindata.price > cycledata.High20    && condition_vol && !fridaynight && con_uplimit &&max_vol && !maxvol_dis){
//				printf("%s  ---> max time:%s  max vol:%d \n",mindata.MinuteTime,maxvol.MinuteTime,maxvol.Volume);
//				maxvol_dis=true;
//
//			}
//			 else if(mindata.price < cycledata.Low20  && condition_vol && !fridaynight && con_downlimit && max_vol&& !maxvol_dis){
//				 printf("%s  ---> max time:%s  max vol:%d \n",mindata.MinuteTime,maxvol.MinuteTime,maxvol.Volume);
//				 maxvol_dis=true;
//			 }

//			 bool condition_long = mindata.price > ma5 && condition_vol && !fridaynight && con_uplimit;
//			 bool condition_short =  mindata.price < ma5   && condition_vol && !fridaynight && con_downlimit;

//			 bool condition_long = mindata.price > cycledata.High20   && attackvolume_rate>0.2   && condition_vol;
//			 bool condition_short =  mindata.price < cycledata.Low20  && attackvolume_rate>0.2   && condition_vol;

//			 bool condition_long = mindata.price > pre_min_price    && condition_vol;
//			 bool condition_short =  mindata.price < pre_min_price  && condition_vol;

//			 bool condition_long = mindata.price > pre_min_price   && pre_min_price>today.Open  && condition_vol ;
//			 bool condition_short =  mindata.price < pre_min_price  && pre_min_price<today.Open && condition_vol;

			 this->SetStopRange(atr20*this->GetStopRatio());
//			 this->SetStopRange(atr20);

//			 bool openorder =m_pmysqltestrecord->Exist_RecordByStatus(this->GetTableRecord().c_str(),1);
//			 bool day=false;
//			 if (openorder){
//				 TestRecordData lastdata=m_pmysqltestrecord->Find_LastRecordByStatus(this->GetTableRecord().c_str(),1);
//
//				 time_t t_last = dateutil.ConvertSqlTimetoTimet(lastdata.OpenDateTime);
//				 time_t t_now = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
//				 day=t_now-t_last<37*60*60;
//				 if(day &&(condition_long||condition_short)){
//					std::cout<<"   >>>>>last order :"<<lastdata.OpenDateTime <<"    now:"<<mindata.MinuteTime<<std::endl;
//				 }
//			 }

const double limitsed =20.0;
			 if (mapsize&& condition_long  && sed >limitsed )	{

				 printf("  最后一分钟成交量:%d   持仓量:%d   >>>>>>>开多 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d \n",
						 lastmin.Volume,mindata.OpenInterest,lastmin.price,today.Open,start_opi);
				 printf ("当天突破做多 check %s date[%s]   cycledata.High20  %.3f mindata.price %.3f \n",
						 this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
						 ,cycledata.High20,mindata.price);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = lastmin.Volume;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.PreSettlePrice=presettleprice;
//				 data.pATR = attackvolume_rate;
//				 if(presettleprice!=0){
//					 data.pATR=(mindata.price-presettleprice)/presettleprice;
//					 printf("  updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
//				 else{
//					 data.pATR=0.0;
//					 printf("  error:updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;

//				 string typefivemin =type_FiveMinOPI(&mindata,0);
//				 strcpy(data.FiveDayLineType , typefivemin.c_str());
				 bvDayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open",&data,lastmin.price);


			 }

			 if ( mapsize&& condition_short  &&   sed >limitsed )	{

				 printf("  最后一分钟成交量:%d   持仓量:%d   >>>>>>>开空 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d  \n",
						 lastmin.Volume,mindata.OpenInterest,lastmin.price,today.Open,start_opi);
				 printf ("当天突破做空 check %s date[%s]   cycledata.Low20  %.3f mindata.price %.3f \n",
						 this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str()
								 ,cycledata.Low20,mindata.price);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = lastmin.Volume;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.PreSettlePrice=presettleprice;
//				 data.pATR = attackvolume_rate;
//				 if(presettleprice!=0){
//					 data.pATR=(mindata.price-presettleprice)/presettleprice;
//
//					 printf("  updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
//				 else{
//					 data.pATR=0.0;
//					 printf("  error:updownrate-------------------------->%.3f  \n",data.pATR);
//				 }
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;
//				 string typefivemin =type_FiveMinOPI(&mindata,1);
//				 strcpy(data.FiveDayLineType , typefivemin.c_str());
				 bvDaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open",&data,lastmin.price);
			 }
		 }

		 else if (this->GetOpenTimeMode() ==0 && exist_order){




			 TestRecordData tr = m_pmysqltestrecord->Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);
			 MinuteLineData lastmin = m_pmysqlhismindata->GetLastMinbyMinuteTime(tablename.c_str(),mindata.MinuteTime);
			 bool condition_vol =lastmin.Volume>mindata.OpenInterest*volopirate;
			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
			 time_t t_open = dateutil.ConvertSqlTimetoTimet(tr.OpenDateTime);
//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
//			 printf("	当日有开仓,持仓中   ------- 现价: %.3f \n",mindata.price);
			 // 更新持仓后的历史新低



			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 m_pmysqltestrecord->UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 m_pmysqltestrecord->UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 &&  mindata.price<ema5 && ema5<ema60 &&condition_vol)	 {
				 bvClose(tr,mindata,"stop_ema",mindata.price);
			 }
			 else if (tr.Direction ==1 &&  mindata.price>ema5 && ema5>ema60 &&condition_vol)	 {
				 bvClose(tr,mindata,"stop_ema",mindata.price);
			 }

			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 bvClose(tr,mindata,"stop_openday",mindata.price);
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 bvClose(tr,mindata,"stop_openday",mindata.price);
			 }

			 if ( t_current>t_endtime)	 {
				 bvClose(tr,mindata,"stop_endtime",mindata.price);
			 }


//			 int holdlast_min_volume = m_pmysqlhismindata->GetLastMinVolumeSum(tablename.c_str(),mindata.MinuteTime);
//			 MinuteLineData hold_pre_min_data = m_pmysqlhismindata->Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,1);
//
//			 if (tr.Direction ==0 && mindata.price < hold_pre_min_data.price && holdlast_min_volume>tr.OpenOPI*0.8)	 {
//
//				 printf("	%s 多头持仓中  行情反转   当前价:%.2f  ---> 前一分钟价格:%.2f   开仓时成交量:%d   反转时成交量:%d\n",
//						 mindata.MinuteTime, mindata.price ,hold_pre_min_data.price,tr.OpenOPI,holdlast_min_volume	 );
//				 Close(tr,mindata,"stop_reverse");
//			  }
//			 else if (tr.Direction ==1 && mindata.price > hold_pre_min_data.price && holdlast_min_volume>tr.OpenOPI*0.8 )		 {
//				 printf("	%s 空头持仓中  行情反转   当前价:%.2f  ---> 前一分钟价格:%.2f   开仓时成交量:%d   反转时成交量:%d\n",
//										 mindata.MinuteTime, mindata.price ,hold_pre_min_data.price,tr.OpenOPI,holdlast_min_volume	 );
//				 Close(tr,mindata,"stop_reverse");
//			  }
//			 if (t_current-t_open>300)	 {
//				 printf("	%s 开仓日   平仓时间: %s \n",mindata.MinuteTime,tr.OpenDateTime );
//				 Close(tr,mindata,"stop_3min");
//			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 bvClose(tr,mindata,"stop_win",mindata.price);
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
				 bvClose(tr,mindata,"stop_win",mindata.price);
			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
	vector<HxDayLineData>().swap(fivedays_list)  ;
	 vector<FiveDayLineData>().swap(fivedaydata_list);
	 vector<double>().swap( last5);
	 	vector<double>().swap(  last60);
}


double DayTradeTest::zb_ema(HxDayLineData *preday,const char * mintime, int x,int y)
{
	double ret;

	string lastday=preday->Time;
	char lastdate[20];
	sprintf(lastdate,"%s%s%s",lastday.substr(0,4).c_str(),lastday.substr(5,2).c_str(),lastday.substr(8,2).c_str());
	std::string lasttablename;
	lasttablename=this->GetMainInstrumentID().c_str();
	lasttablename.append("_");
	lasttablename.append(lastdate);

	vector<double> prelist,list;

	if(x<y){

		this->m_pmysqlhismindata->Find_PriceListsN(prelist,lasttablename.c_str(),y-x);
//		printf(" [%s]拼接分钟数据 %s--------------->%ld      \n ",lastdate,mintime,prelist.size());
		this->m_pmysqlhismindata->Find_PriceListsNbyMinuteTime(list,this->GetTable_HisMindata().c_str(),mintime,x);

		for(int j=0;j<x;j++){
			prelist.push_back(list[j]);
		}
		ret = EMA(prelist,y);
//		printf(" 拼接分钟数据 --------------->%ld     ema:%.2f \n ",prelist.size(),ret);


	}
	else{
		this->m_pmysqlhismindata->Find_PriceListsNbyMinuteTime(list,this->GetTable_HisMindata().c_str(),mintime,y);
		ret = EMA(list,y);

//		printf(" 当日分钟数据 --------------->%ld     ema:%.2f \n",list.size(),ret);
	}

	vector<double>().swap(prelist);
	vector<double>().swap(list);
	return ret;
}
double  DayTradeTest::EMA(std::vector<double> &X, int N)
{
    std::vector<double> vec;
    double ret;
    int nLen = X.size();
    if(nLen >= 1)
    {
        if(N > nLen) N = nLen;

        vec.resize(nLen);
        //vec.reserve(nLen);
//        vec[0] = X[0];

        double k =2.0/((double)N+1.0);

//        printf ("k   --->   %f \n",k);
        for(int i = 0; i < nLen; i++)
        {
//            vec[i] = (2 * X[i] + (N - 1) * vec[i - 1]) / (N + 1);
//        	int n=i+1;
        	if (i==0)
        	{
        		 vec[i] = X[i];
        	}
        	else
        	{
        		vec[i] = k* X[i] + vec[i - 1]*(1-k);
        	}


            ret =vec[i];
//            printf("EMA  --------  i :%d  close: %f   vec[i-1]  %f  vec[i]: %f    ret: %f   \n",i,X[i],vec[i - 1], vec[i],ret);
        }
    }

    vector<double>().swap(vec);
    return ret;
}
bool  DayTradeTest::Min_LongSortbyNumber( TypeMinuteLineData min1, TypeMinuteLineData min2)
{
	if(min1.OpenInterest>min2.OpenInterest)
		return true;
	else
		return false;

}
bool  DayTradeTest::Min_ShosrtSortbyNumber( TypeMinuteLineData min1, TypeMinuteLineData min2)
{
	if(min1.OpenInterest<min2.OpenInterest)
		return true;
	else
		return false;

}

bool  DayTradeTest::LongSortbyNumber( FiveDayLineData stu1, FiveDayLineData stu2)
{
	if(stu1.Close>stu2.Close)
		return true;
	else
		return false;

}

bool  DayTradeTest::ShortSortbyNumber( FiveDayLineData stu1, FiveDayLineData stu2)
{
	if(stu1.Close<stu2.Close)
		return true;
	else
		return false;

}
void DayTradeTest::BigBigVolume_Open_MinuteLine()
{

//	printf ("VolumeOPI_Open_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
		sprintf(moringtime,"%s-%s-%s 09:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
		time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	int total_volume=0;
	int start_opi=0;
	int i=0;

	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
		if(i==0){
			start_opi = mindata.OpenInterest;
		}
		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		total_volume+=mindata.Volume;
		i++;
		 if(  i>3 &&opentime_mode &&  t_current<t_moringtime &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {

//			 printf("  成交量:%d   持仓量:%d   \n",total_volume,mindata.OpenInterest);

			 int last_3min_volume = mysqlhismindata.GetLast3MinVolumeSum(tablename.c_str(),mindata.MinuteTime);
			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,3);
			 int pre_3min_opi=pre_3min_data.OpenInterest;
			 double pre_3min_price=pre_3min_data.price;

			 double rate=(double)last_3min_volume/((double)mindata.OpenInterest-(double)pre_3min_opi);
			 double amount=((double)mindata.OpenInterest-(double)pre_3min_opi)*mindata.price* (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

			 bool condition_vol =last_3min_volume>mindata.OpenInterest*0.04;
			 bool condition_opi=mindata.OpenInterest>pre_3min_opi;
			 bool condition_volopirate = rate<2 && rate>0.5;
			 bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);

			 bool condition_long = mindata.price>pre_3min_price &&condition_vol && condition_opi
					 &&condition_volopirate;
			 bool condition_short = mindata.price<pre_3min_price&&condition_vol && condition_opi
					 &&condition_volopirate;

			 this->SetStopRange(mindata.price*0.02*this->GetStopRatio());
			 if (condition_long  && sed >2 &&!fridaynight)	{
				 printf("  成交量:%d   持仓量:%d   >>>>>>>开多 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open");
			 }

			 if (  condition_short  &&   sed >2 &&!fridaynight)	{
				 printf("  成交量:%d   持仓量:%d   >>>>>>>开空 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d  \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open");
			 }
		 }

		 else if (this->GetOpenTimeMode() ==0 &&
				 mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && this->GetOpenFlag() && !this->GetCloseFlag())		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
time_t t_open = dateutil.ConvertSqlTimetoTimet(tr.OpenDateTime);
//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
//			 printf("	当日有开仓,持仓中   ------- 现价: %.3f \n",mindata.price);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }


			 if (t_current-t_open>300)	 {
				 printf("	%s 开仓日   平仓时间: %s \n",mindata.MinuteTime,tr.OpenDateTime );
				 Close(tr,mindata,"stop_3min");
			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
				 Close(tr,mindata,"stop_win");
			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::BigBigVolume_AverageBack_Open_MinuteLine()
{

//	printf ("VolumeOPI_Open_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	int count=2;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
		sprintf(moringtime,"%s-%s-%s 09:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
		time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",this->GetTable_HisMindata().c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	int total_volume=0;
	int start_opi=0;
	int i=0;
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,20);

	double range = precycledata.Highest-precycledata.Lowest;

	double h1 = precycledata.Highest -range*0.2;
	double h2 =  precycledata.Highest;

	double l1= precycledata.Lowest;
	double l2 = precycledata.Lowest+range*0.2;

	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		if(i==0){
			start_opi = mindata.OpenInterest;
			total_volume+=0;
		}
		else{
			total_volume+=mindata.Volume;
		}

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;


		i++;

//		 printf("  %s 总成交量:%d  成交量:%d   持仓量:%d   \n",mindata.MinuteTime,total_volume,mindata.Volume,mindata.OpenInterest);
		 if(  i>4 &&opentime_mode &&  t_current<t_moringtime &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {



			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(this->GetTable_HisMindata().c_str(),mindata.MinuteTime,3);

			 //流入资金
//			 double amount=((double)mindata.OpenInterest-(double)pre_3min_data.OpenInterest )*mindata.price*
//					 (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;
			 // 3分钟内成交量 占持仓比
			 double volopirate_3min = zb_3Min_VolOpiRate(&mysqlhismindata,&mindata);
			 bool condition_volopirate_3min = volopirate_3min > 0.1;

			 bool condition_3min_long =mindata.OpenInterest>pre_3min_data.OpenInterest  && mindata.price>pre_3min_data.price;
			 bool condition_3min_short =mindata.OpenInterest< pre_3min_data.OpenInterest  && mindata.price<pre_3min_data.price;

			 bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);

			 bool price_long = mindata.price >h1 && mindata.price<h2;
			 bool price_short = mindata.price >l1 && mindata.price<l2;
			 bool condition_long =  condition_3min_long&& condition_volopirate_3min  &&price_long;
			 bool condition_short =  condition_3min_short&& condition_volopirate_3min  &&price_short;

			 double addopirate = this->zb_AddOpiRate(&mindata,start_opi);
			 this->SetStopRange(mindata.price*0.02*this->GetStopRatio());

			 if (condition_long  && sed >2 &&!fridaynight)	{
				 printf ("\n");
				 printf ("	date[%s]    ---->    h1:%.2f   h2:%.2f\n",mindata.MinuteTime,h1,h2 );
				 printf("	>>>总成交量:%d   持仓量:%d   >>>>>>>开多 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 printf("	>>>3min vol/opi  %.2f%%\n",volopirate_3min*100);
				 printf("	增仓比:%.2f%%   start: %d    current: %d   %s \n",addopirate*100,start_opi,mindata.OpenInterest,mindata.MinuteTime);

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = addopirate;
				 data.Open_VolOpiRate= volopirate_3min;
				 data.pATR = 0;
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open",&data);
			 }

			 if (  condition_short  &&   sed >2 &&!fridaynight)	{
				 printf ("\n");
				 printf ("	date[%s]    ---->    l1:%.2f   l2:%.2f\n",mindata.MinuteTime,l1,l2 );
				 printf("	<<<成交量:%d   持仓量:%d   >>>>>>>开空 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d  \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 printf("	<<<3min vol/opi  %.2f%%\n",volopirate_3min*100);
				 printf("	增仓比  %.2f%%   start:%d    current:%d   %s \n",addopirate*100,start_opi,mindata.OpenInterest,mindata.MinuteTime);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= volopirate_3min;
				 data.pATR = 0;
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open",&data);
			 }
		 }

		 else if (this->GetOpenTimeMode() ==0 &&
				 mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && this->GetOpenFlag() && !this->GetCloseFlag())		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
			 time_t t_open = dateutil.ConvertSqlTimetoTimet(tr.OpenDateTime);
//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
//			 printf("	当日有开仓,持仓中   ------- 现价: %.3f \n",mindata.price);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }

//
//			 if (t_current-t_open>300)	 {
//				 printf("	%s 开仓日   平仓时间: %s \n",mindata.MinuteTime,tr.OpenDateTime );
//				 Close(tr,mindata,"stop_3min");
//			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
				 Close(tr,mindata,"stop_win");
			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
}

double DayTradeTest::zb_AttackVolume(MysqlHisMinData *mysqlhismindata,MinuteLineData *mindata,double volopirate)
{
	double rate=0.0;

	vector<MinuteLineData>  min_list;

	MinuteLineData maxdata= mysqlhismindata->Find_MaxVolumebyMinuteTime(this->GetTable_HisMindata().c_str(),mindata->MinuteTime);


	if((double)maxdata.Volume>(double)maxdata.OpenInterest*volopirate){
		mysqlhismindata->Find_ListsbyMinuteTime(min_list,this->GetTable_HisMindata().c_str(),mindata->MinuteTime);


		int count=0;

		 for (auto &mindata:min_list)	{
			if((double)mindata.Volume>(double)maxdata.Volume*0.4){
				count++;
			}
		 }
		 rate= (double)count/(double)min_list.size();

//		 printf(" maxdata time %s   -------> %s \n",maxdata.MinuteTime , mindata->MinuteTime);
	}


	vector<MinuteLineData>().swap(min_list);
	return rate;
}
double DayTradeTest::zb_AddOpiRate(MinuteLineData *mindata,int startopi)
{
	double opiadd_rate;
	if(startopi!=0){
		opiadd_rate=((double)mindata->OpenInterest - (double)startopi ) /(double)startopi;
//			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata->OpenInterest,opiadd_rate);
	}
	else{
		opiadd_rate=0;
	}
	return opiadd_rate;
}
double DayTradeTest::zb_3Min_VolOpiRate(MysqlHisMinData *mysqlhismindata,MinuteLineData *mindata)
{
	double opiadd_rate;
	int last_3min_volume = mysqlhismindata->GetLast3MinVolumeSum(this->GetTable_HisMindata().c_str(),mindata->MinuteTime);

	if(mindata->OpenInterest>0){
		 opiadd_rate=(double)last_3min_volume/(double)mindata->OpenInterest;
//		 printf("   volume sum:%d    opi:%d    %s\n",last_3min_volume,mindata->OpenInterest,mindata->MinuteTime);
	}
	else{
		opiadd_rate=0;
	}

	return opiadd_rate;
}

void DayTradeTest::PivotPoint_Open_MinuteLine()
{

//	printf ("VolumeOPI_Open_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
//	printf ("check %s Find_DataBySqlTime : %s \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

//	int count=2;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	char moringtime[20];
		sprintf(moringtime,"%s-%s-%s 09:30:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
		time_t t_moringtime = dateutil.ConvertSqlTimetoTimet(moringtime);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	int total_volume=0;
	int start_opi=0;
	int i=0;

	double pivot = (preday.High + preday.Low + preday.Close) / 3; //（前一天的最高、最低和收盘）

	double r1= 2*pivot - preday.Low;
	double s1= 2*pivot - preday.High;

	double r2= pivot + (r1-s1);
	double s2= pivot - (r1-s1);

	double r3= preday.High - (2 * (preday.Low - pivot));
	double s3= preday.Low - (2 * (preday.High - pivot));

	double sm1=(pivot+s1)/2;
	double sm2=(s1+s2)/2;
	double sm3=(s2+s3)/2;

	double rm1=(pivot+r1)/2;
	double rm2=(r1+r2)/2;
	double rm3=(r2+r3)/2;
	ATRData preatr5_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,5);
//
//
//	double lastsed=preday.OpenInterest * preday.Close * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;
//if(lastsed<2){
//	printf ("\n	%s 昨日最高: %.2f  昨日最低:%.2f   昨日收盘:%.2f    昨日持仓:%d   VolumeMultiple:%d margin:%.2f  lastsed:%.2f \n ",
//			tablename.c_str(),preday.High,preday.Low,preday.Close, preday.OpenInterest,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,lastsed);
//	return;
//}
//else{
//
//}

//	printf ("\n	%s 昨日最高: %.2f  昨日最低:%.2f   昨日收盘:%.2f    \n ",tablename.c_str(),preday.High,preday.Low,preday.Close);
//
//	printf ("	%s r1-s1 [%.2f --->%.2f]    \n ",tablename.c_str(),r1,s1);
//	printf ("	%s r2-s2 [%.2f --->%.2f]    \n ",tablename.c_str(),r2,s2);
//	printf ("	%s r3-s3 [%.2f --->%.2f]    \n",tablename.c_str(),r3,s3);
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
		if(i==0){
			start_opi = mindata.OpenInterest;
		}
		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		if(sed<2){
			continue;
		}
		total_volume+=mindata.Volume;
		i++;
		 if(  i>3 &&opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {

//			 printf("  		价格:%.2f 成交量:%d   持仓量:%d   sed:%.2f \n",mindata.price,total_volume,mindata.OpenInterest,sed);

			 int last_3min_volume = mysqlhismindata.GetLast3MinVolumeSum(tablename.c_str(),mindata.MinuteTime);
			 MinuteLineData pre_3min_data = mysqlhismindata.Find_PreNDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime,3);
			 int pre_3min_opi=pre_3min_data.OpenInterest;
			 double pre_3min_price=pre_3min_data.price;

			 double rate=(double)last_3min_volume/((double)mindata.OpenInterest-(double)pre_3min_opi);
			 double amount=((double)mindata.OpenInterest-(double)pre_3min_opi)*mindata.price* (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

			 bool condition_vol =last_3min_volume>mindata.OpenInterest*0.04;
			 bool condition_opi=mindata.OpenInterest>pre_3min_opi;
			 bool condition_volopirate = rate<2 && rate>0.5;
bool condition_atr = preatr5_data.ATR /mindata.price >0.015;
			 bool condition_long = mindata.price>r2&&condition_atr;
			 bool condition_short = mindata.price<s2&&condition_atr;

//			 bool condition_short = mindata.price>pre_3min_price &&condition_vol && condition_opi
//					 &&condition_volopirate;
//			 bool condition_long = mindata.price<pre_3min_price&&condition_vol && condition_opi
//					 &&condition_volopirate;

			 this->SetStopRange(mindata.price*0.02);
			 if (condition_long  && sed >2 )	{
				 printf("  成交量:%d   持仓量:%d   >>>>>>>开多 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 DayBuyOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open");
			 }

			 if (  condition_short  &&   sed >2 )	{
				 printf("  成交量:%d   持仓量:%d   >>>>>>>开空 现价:%.2f --> 开盘价:%.2f  开盘持仓: %d  \n",
						 total_volume,mindata.OpenInterest,mindata.price,today.Open,start_opi);
				 DaySellOpen(mindata, this->GetMainInstrumentID().c_str(),"volopi_open");
			 }
		 }

		 else if (this->GetOpenTimeMode() ==0 &&
				 mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && this->GetOpenFlag() && !this->GetCloseFlag())		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
time_t t_open = dateutil.ConvertSqlTimetoTimet(tr.OpenDateTime);
//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
//			 printf("	当日有开仓,持仓中   ------- 现价: %.3f \n",mindata.price);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange()*this->GetStopRatio();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange()*this->GetStopRatio();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }

			 if ( t_current>t_endtime)	 {
				 Close(tr,mindata,"stop_endtime");
			 }


//			 if (t_current-t_open>300)	 {
//				 printf("	%s 开仓日   平仓时间: %s \n",mindata.MinuteTime,tr.OpenDateTime );
//				 Close(tr,mindata,"stop_3min");
//			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
				 Close(tr,mindata,"stop_win");
			  }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
				 Close(tr,mindata,"stop_win");
			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::MinuteOpen(const char*section,
		const char *pinstrumentid,
		const char *datetime,
		SqlAveragePriceData predata,
		double openprice,
		CycleData highdata,
		CycleData lowdata,
		CycleData cycledata)
{
//	printf ("MinuteLine_OpenTest begin \n");
	bool openflag =false;
	bool closeflag=false;
	int count=2;
	double stoppercent =0.01;
	double winpercent =0.03;
	MysqlTestRecord mysqltestrecord;
	MysqlHisATR mysqlhisatr;
	MysqlMA mysqlma;
	MysqlEMA mysqlema;
	MysqlHisMA mysqlhisma;
	MysqlAvePrice mysqlaveprice;
	MysqlHisMinData mysqlhismindata;
	MysqlHisSection mysqlhissection;
	MysqlMaincontract mysqlmaincontract;
	HisSectionData hissectiondata;
	hissectiondata.AverageUpDownrate=0;

	DateUtil dateutil;
	SqlAveragePriceData todayavedata = mysqlaveprice.Find_DateTime(pinstrumentid,datetime);
	int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(pinstrumentid,datetime,20);



	vector<MinuteLineData>  min_list;


	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = datetime;
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);

	std::string hissection_table=section ;
	hissection_table.append("_");
	hissection_table.append(date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
	}


	double uplimit = openprice *1.01;
	double downlimit = openprice *0.99;

	MAData madata;
	madata=mysqlma.Find_MADatabyTime(pinstrumentid,datetime);

	EMAData emadata;
	emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,datetime);

	HisMAData hismadata;
	hismadata = mysqlhisma.Find_HisMADatabyDateTime_Offset(pinstrumentid,datetime,1);


	int i=0;
	double total_price;
	double aveprice;
	 for (auto &mindata:min_list)	 {

		 total_price += mindata.price;
		 i++;
		 aveprice = total_price/i;
//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

		 if(!mysqltestrecord.Exist_RecordByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,0) && openflag ==false)		 {

			double ma50;
			ma50=(madata.MA49+mindata.price)/50;
			double ma20;
			ma20=(madata.MA19+mindata.price)/20;
			double ma10;
			ma10=(madata.MA9+mindata.price)/10;

			double ema50,ema9,ema4;
			ema50 = (2*mindata.price + 49*emadata.EMA49)/51;
			ema9 = (2*mindata.price + 8*emadata.EMA8)/10;
			ema4 = (2*mindata.price + 3*emadata.EMA3)/5;




			double ma50_last =hismadata.MA50;
			double ma20_last =hismadata.MA20;
			double ma10_last =hismadata.MA10;
			double x50 = atan((ma50/ma50_last-1)*100)*180/M_PI;
			double x20 = atan((ma20/ma20_last-1)*100)*180/M_PI;
			double x10 = atan((ma10/ma10_last-1)*100)*180/M_PI;

			if (strcmp(section,"none")!=0)			{

				hissectiondata = mysqlhissection.Find_DatabyMinuteTime(hissection_table.c_str(),mindata.MinuteTime);
			}



			ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);

//			printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f   下跌品种:%.2f   平均涨跌幅 : %.2f \n",hissection_table.c_str(),
//					mindata.MinuteTime,hissectiondata.CountUprate,hissectiondata.CountDownrate,hissectiondata.AverageUpDownrate);
			bool condition_long1  = ma50<ma20 && ma20< ma10;
			bool condition_long6 = x50 >0 && x20 >0 && x10>0;

			bool condition_long_ema = ema9<ema4 && ema50<ema9;
//			bool condition_long_trend = condition_long1 && condition_long6;
			bool condition_long_trend = condition_long_ema;

			bool condition_long2 = mindata.price >openprice*1.005 && openprice > predata.AveragePrice*1.0025;
			bool condition_long4 = atrcount2x ==0;
			bool condition_long3 =false;
			bool condition_long5 = (mindata.price  -predata.AveragePrice )/predata.AveragePrice < 0.04;

			bool condition_long5_a =false;
			bool condition_long5_b =false;
			bool condition_long5_c =false;

			if (strcmp(section,"none")!=0)		{
				condition_long5_a =(mindata.price  -predata.AveragePrice )/predata.AveragePrice >hissectiondata.AverageUpDownrate;
				condition_long5_b =hissectiondata.CountUprate>0.5;
				condition_long5_c = strcmp(hissectiondata.MaxInstrumentID,pinstrumentid)==0;
			}

			bool condition_long_section =condition_long5_a && condition_long5_b ;


			bool condition_atr = atr_data.ATR/mindata.price <0.035;
			double highest ;

//			printf ("check %s mindata : 33333  %s cycledata.HighCount: %d  mindata.price: %.3f  highdata.High20:%.3f  \n",
//																tablename.c_str(),mindata.MinuteTime,cycledata.HighCount,mindata.price,highdata.High20);

//			if (condition_long1)
//			{
//				printf ("check %s mindata : condition_long1 %s  ma50: %.3f ma20:%.3f ma10:%.3f  \n",
//																	tablename.c_str(),mindata.MinuteTime,ma50,ma20,ma10);
//			}
//
//			if (condition_long2)
//			{
//				printf ("check %s mindata : condition_long2  %s mindata.price : %.3f  openprice: %.3f  predata.AveragePrice:%.3f  \n",
//											tablename.c_str(),mindata.MinuteTime,mindata.price ,openprice,predata.AveragePrice);
//			}
			if (cycledata.HighCount>count && mindata.price > cycledata.High20)		{
				condition_long3=true;
//				printf ("check %s mindata : condition_long3  %s cycledata.HighCount: %d  mindata.price: %.3f  cycledata.High20:%.3f  \n",
//													tablename.c_str(),mindata.MinuteTime,cycledata.HighCount,mindata.price,cycledata.High20);
			}
			else	{


				if (highdata.High20 > cycledata.High20)		{
					highest = highdata.High20;
				}
				else	{
					highest = cycledata.High20;
				}

				if (mindata.price > highest){
//					condition_long3=true;
				}
			}

			bool condition_long_pattern =condition_long3  && condition_long2 && condition_long5 && mindata.price > aveprice;

			// 开多总条件
			bool condition_long =  condition_long_trend && condition_long_pattern ;



			bool condition_short1  = ma50>ma20 && ma20> ma10;
			bool condition_short6 = x50 < 0 && x20 <0 && x10<0;

			bool condition_short_ema =  ema9>ema4 && ema50>ema9;
//			bool condition_short_trend = condition_short1 && condition_short6;
			bool condition_short_trend = condition_short_ema;

			bool condition_short2 = mindata.price < openprice*0.995 && openprice < predata.AveragePrice*0.9975;
			bool condition_short3=false ;
			bool condition_short4 = atrcount2x ==0;
			bool condition_short5 =(predata.AveragePrice -mindata.price  )/predata.AveragePrice < 0.04;

			bool condition_short5_a=false;
			bool condition_short5_b=false;
			bool condition_short5_c=false;

			if (strcmp(section,"none")!=0)	{
				condition_short5_b =hissectiondata.CountDownrate >0.7;
				condition_short5_a =(mindata.price  -predata.AveragePrice )/predata.AveragePrice < hissectiondata.AverageUpDownrate;
				condition_short5_c = strcmp(hissectiondata.MinInstrumentID,pinstrumentid)==0;

			}
			bool condition_short_section =condition_short5_a && condition_short5_b  ;

			bool condition_short7 = x10 < x20 && x20 <x50 && x50<0;
			double lowest ;
			if (cycledata.LowCount >count && mindata.price < cycledata.Low20)	{
				condition_short3=true;
			}
			else	{


				if (lowdata.Low20 < cycledata.Low20)				{
					lowest = lowdata.Low20;
				}
				else			{
					lowest = cycledata.Low20;
				}

				if (mindata.price < lowest)	{
//					condition_short3=true;
				}
			}

			bool condition_short_pattern = condition_short3  && condition_short2 && condition_short5  && mindata.price < aveprice ;

			//开空总条件
			bool condition_short =   condition_short_trend  && condition_short_pattern ;


			 if (condition_long )		{
				 int atrcount = mysqlhisatr.Count_ListsByTime_N(pinstrumentid,datetime,20);
				 	int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(pinstrumentid,datetime,20);
				 	double atr_indicator = mysqlhisatr.Indicator_ListsByTime_N(pinstrumentid,datetime,20);
				 	printf("ema50 %.3f  ema9 %.3f ema4 %.3f   \n",ema50,ema9,ema4);

//				 	 printf("  %s-%s   atrcount -----[%d]    atrcount2x -----[%d]   atr_indicator  ---> %.2f  ATR  ---> %.3f \n",
//				 					 				 			pinstrumentid,datetime,atrcount,atrcount2x,atr_indicator, atr_data.ATR);
//				 	printf("  %s-%s   x50 -----[%.2f]    x20 -----[%.2f]   x10  ---> [%.2f] \n ",
//				 			pinstrumentid,datetime,x50,x20,x10);
//				 	printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f%%   下跌品种:%.2f%%   平均涨跌幅 : %.2f%% MaxInstrumentID %s \n",hissection_table.c_str(),
//				 						mindata.MinuteTime,hissectiondata.CountUprate*100,hissectiondata.CountDownrate*100,100*hissectiondata.AverageUpDownrate,hissectiondata.MaxInstrumentID);
//				 	int p= 20000.00/( mindata.price* maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio);

				 	int p= 20000.00/(atr_data.ATR* maincontractdata.VolumeMultiple);
					int position = p-p%2 ;



					TestRecordData tr;
					strcpy(tr.OpenDateTime,mindata.MinuteTime);
					strcpy(tr.InstrumentID,pinstrumentid);
					tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
					tr.pATR = atr_data.ATR/mindata.price;
					tr.OpenPrice = mindata.price ;
					tr.Direction =0;
					tr.Status =0;
					tr.Volume =position;
					tr.PreSettlePrice = predata.AveragePrice;
					tr.DayOpenPrice = openprice;
					tr.OpenOPI = mindata.OpenInterest;
//					tr.StopPrice = mindata.price  - atr_data.ATR;

					tr.StopPrice = mindata.price*(1-stoppercent);
					tr.WinPrice = mindata.price*(1+winpercent);

					tr.Highest = mindata.price ;
					tr.Lowest = mindata.price;
					strcpy(tr.OpenSignal,"cycleopen");
					if (tr.Volume >0)		{
						mysqltestrecord.Insert(this->tablename_test.c_str(),tr);

						printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
								mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
						openflag =true;
					}
					else	{
						printf("   position  %d <=0     \n",tr.Volume);
					}

				}

			 if (condition_short )		{
				 int atrcount = mysqlhisatr.Count_ListsByTime_N(pinstrumentid,datetime,20);
				 	int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(pinstrumentid,datetime,20);
				 	double atr_indicator = mysqlhisatr.Indicator_ListsByTime_N(pinstrumentid,datetime,20);
				 	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);
				 	printf("ema50 %.3f  ema9 %.3f ema4 %.3f   \n",ema50,ema9,ema4);
//				 	printf("  %s-%s   atrcount -----[%d]    atrcount2x -----[%d]   atr_indicator  ---> %.2f ATR  ---> %.3f \n ",
//				 			pinstrumentid,datetime,atrcount,atrcount2x,atr_indicator, atr_data.ATR);
//				 	printf("  %s-%s   x50 -----[%.2f]    x20 -----[%.2f]   x10  ---> [%.2f] \n ",
//				 													pinstrumentid,datetime,x50,x20,x10);
//				 	printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f%%   下跌品种:%.2f%%   平均涨跌幅 : %.2f%% MinInstrumentID %s \n",hissection_table.c_str(),
//				 						mindata.MinuteTime,hissectiondata.CountUprate*100,hissectiondata.CountDownrate*100,100*hissectiondata.AverageUpDownrate,hissectiondata.MinInstrumentID);
//				 	int p= 20000.00/( mindata.price* maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio);

				 	int p= 20000.00/(atr_data.ATR* maincontractdata.VolumeMultiple);

					int position = p-p%2 ;

					TestRecordData tr;
					strcpy(tr.OpenDateTime,mindata.MinuteTime);
					strcpy(tr.InstrumentID,pinstrumentid);
					tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
					tr.pATR = atr_data.ATR/mindata.price;
					tr.OpenPrice = mindata.price;
					tr.Direction =1;
					tr.Status =0;
					tr.Volume =position;
					tr.PreSettlePrice = predata.AveragePrice;
					tr.DayOpenPrice = openprice;
					tr.OpenOPI = mindata.OpenInterest;
//					tr.StopPrice = mindata.price + atr_data.ATR;

					tr.StopPrice = mindata.price*(1+stoppercent);
					tr.WinPrice = mindata.price*(1-winpercent);

					tr.Highest = mindata.price ;
					tr.Lowest = mindata.price;
					strcpy(tr.OpenSignal,"cycleopen");
					if (tr.Volume >0)		{
						mysqltestrecord.Insert(this->tablename_test.c_str(),tr);
						printf("	开空   -------%s  板块下跌: %.2f 昨结:%.3f 今开:%.3f  openprice %.3f    lowest %.3f lowdata.Low20 %.3f cycledata.low20 %.3f LowCount --> %d StopPrice  ---> %.3f \n",
								mindata.MinuteTime,hissectiondata.CountDownrate,predata.AveragePrice, openprice,mindata.price,lowest,lowdata.Low20,cycledata.Low20,cycledata.LowCount,tr.StopPrice);
						openflag =true;
					}
					else	{
						printf("   position  %d <=0     \n",tr.Volume);
					}

				}
		 }

		 else if (mysqltestrecord.Exist_RecordByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,0) && closeflag ==false) {

			 ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);
			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

			 double addopirate =(double) (mindata.OpenInterest - tr.OpenOPI)/(double)tr.OpenOPI;

//			 printf("	当日有开仓,持仓中   -------openopi %d  curren_opi   %d  addopirate: %.3f \n",tr.OpenOPI,mindata.OpenInterest,addopirate);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest*(1-stoppercent);
//				 printf("	当日有开仓,持仓中   更新最高价   -------  %.3f   止损价  %.3f \n", tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price > tr.OpenPrice)	{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest*(1+stoppercent);
//				 printf("	当日有开仓,持仓中   更新最低价   -------%.3f    止损价  %.3f\n",tr.Lowest,tr.StopPrice );
				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
			}

//			 if ( addopirate < -0.03)
//			 {
//
//				 tr.ClosePrice = mindata.price;
//				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
//				 tr.Status =1;
//				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume ;
//				 tr.CloseOPI = mindata.OpenInterest;
//				 tr.AddOPIRate = addopirate;
//				 strcpy(tr.CloseSignal,"stop_opi");
//				 mysqltestrecord.UpdatebyInstrument(tablename_test.c_str(),pinstrumentid,tr);
//				 printf("	风控平仓   -------%s  ClosePrice %.3f Profit: %.2f \n",mindata.MinuteTime,mindata.price,tr.Profit);
//
//				 closeflag=true;
//			 }


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {

				 tr.ClosePrice = mindata.price;
				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
				 tr.Status =1;
				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume ;
				 tr.CloseOPI = mindata.OpenInterest;
				 tr.AddOPIRate = addopirate;
				 strcpy(tr.CloseSignal,"stop_openday");
				 mysqltestrecord.UpdatebyInstrument(this->tablename_test.c_str(),pinstrumentid,tr);
				 printf("	止损   -------%s  ClosePrice %.3f Profit: %.2f \n",mindata.MinuteTime,mindata.price,tr.Profit);

				 closeflag=true;
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {

				 tr.ClosePrice = mindata.price;
				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
				 tr.Status =1;
				 tr.Profit = (double)maincontractdata.VolumeMultiple  *( tr.OpenPrice -tr.ClosePrice ) *tr.Volume;
				 tr.CloseOPI = mindata.OpenInterest;
				 tr.AddOPIRate = addopirate;
				 strcpy(tr.CloseSignal,"stop_openday");
				 mysqltestrecord.UpdatebyInstrument(this->tablename_test.c_str(),pinstrumentid,tr);
				 printf("	止损   -------%s  ClosePrice %.3f Profit: %.2f  \n",mindata.MinuteTime,mindata.price,tr.Profit);

				 closeflag=true;
			 }

			bool win_long0 = ( tr.Highest - mindata.price) > tr.Highest*0.01 && mindata.price > tr.OpenPrice;
			bool win_long1 =  mindata.price  > tr.OpenPrice  + atr_data.ATR ;
//			bool win_long = win_long0 || win_long1;

			bool win_short0 =  (  mindata.price -tr.Lowest ) > tr.Lowest*0.01&&  mindata.price  < tr.OpenPrice;
			bool win_short1 =  mindata.price   < tr.OpenPrice  - atr_data.ATR;
//			bool win_short = win_short0 || win_short1;

//			 if (tr.Direction ==0 &&  win_long0)
//			 {
//
//				 tr.ClosePrice = mindata.price;
//				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
//				 tr.Status =1;
//				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume;
//				 strcpy(tr.CloseSignal,"stop_openday_win");
//				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(tablename_test.c_str(),pinstrumentid,0))
//				 {
//					 mysqltestrecord.UpdatebyInstrument(tablename_test.c_str(),pinstrumentid,tr);
//					 printf("	当日止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//						 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//				 }
//
//
//			 }
//			 else if (tr.Direction ==1 && win_short0)
//			 {
//				 tr.ClosePrice = mindata.price;
//				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
//				 tr.Status =1;
//				 tr.Profit = (double)maincontractdata.VolumeMultiple  *( tr.OpenPrice -tr.ClosePrice ) *tr.Volume;
//				 strcpy(tr.CloseSignal,"stop_openday_win");
//				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(tablename_test.c_str(),pinstrumentid,0))
//				 {
//					 mysqltestrecord.UpdatebyInstrument(tablename_test.c_str(),pinstrumentid,tr);
//					 printf("	当日止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//						 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//				 }
//
//			 }
//
//
//
//
//
//			 if (tr.Direction ==0 &&  win_long1)
//			 {
//
//				 tr.ClosePrice = mindata.price;
//				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
//				 tr.Status =1;
//				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume;
//				 strcpy(tr.CloseSignal,"openday_win");
//				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(tablename_test.c_str(),pinstrumentid,0))
//				 {
//					 mysqltestrecord.UpdatebyInstrument(tablename_test.c_str(),pinstrumentid,tr);
//					 printf("	当日止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//						 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//				 }
//
//
//			 }
//			 else if (tr.Direction ==1 && win_short1)
//			 {
//				 tr.ClosePrice = mindata.price;
//				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
//				 tr.Status =1;
//				 tr.Profit = (double)maincontractdata.VolumeMultiple  *( tr.OpenPrice -tr.ClosePrice ) *tr.Volume;
//				 strcpy(tr.CloseSignal,"openday_win");
//				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(tablename_test.c_str(),pinstrumentid,0))
//				 {
//					 mysqltestrecord.UpdatebyInstrument(tablename_test.c_str(),pinstrumentid,tr);
//					 printf("	当日止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//						 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//				 }
//
//			 }


			 if (tr.Direction ==0 && mindata.price > tr.WinPrice)	 {

				 tr.ClosePrice = mindata.price;
				 tr.CloseOPI = mindata.OpenInterest;
				 tr.AddOPIRate = addopirate;
				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
				 tr.Status =1;
				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume;
				 strcpy(tr.CloseSignal,"winprice");
				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,0))	 {
					 mysqltestrecord.UpdatebyInstrument(this->tablename_test.c_str(),pinstrumentid,tr);
					 printf("	当日止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
						 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
				 }


			 }
			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice)	 {
				 tr.ClosePrice = mindata.price;
				 tr.CloseOPI = mindata.OpenInterest;
				 tr.AddOPIRate = addopirate;
				 strcpy(tr.CloseDateTime,mindata.MinuteTime);
				 tr.Status =1;
				 tr.Profit = (double)maincontractdata.VolumeMultiple  *( tr.OpenPrice -tr.ClosePrice ) *tr.Volume;
				 strcpy(tr.CloseSignal,"winprice");
				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->tablename_test.c_str(),pinstrumentid,0))	 {
					 mysqltestrecord.UpdatebyInstrument(this->tablename_test.c_str(),pinstrumentid,tr);
					 printf("	当日止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
						 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
				 }

			 }

			 if (t_current>t_endtime )		 {

//			 if (tr.Direction ==0 && mindata.price < todayavedata.AveragePrice)
				 if (tr.Direction ==0 )	 {

					 tr.ClosePrice = mindata.price;
					 tr.CloseOPI = mindata.OpenInterest;
					 tr.AddOPIRate = addopirate;
					 strcpy(tr.CloseDateTime,mindata.MinuteTime);
					 tr.Status =1;
					 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume;
					 strcpy(tr.CloseSignal,"stop_endtime");
					 mysqltestrecord.UpdatebyInstrument(this->tablename_test.c_str(),pinstrumentid,tr);
					 printf("	尾盘平仓   -------%s  ClosePrice %.3f Profit: %.2f \n",mindata.MinuteTime,mindata.price,tr.Profit);

					 closeflag=true;
				 }
//				 else if (tr.Direction ==1 && mindata.price > todayavedata.AveragePrice)
				 else if (tr.Direction ==1 )	 {

					 tr.ClosePrice = mindata.price;
					 tr.CloseOPI = mindata.OpenInterest;
					 tr.AddOPIRate = addopirate;
					 strcpy(tr.CloseDateTime,mindata.MinuteTime);
					 tr.Status =1;
					 tr.Profit = (double)maincontractdata.VolumeMultiple  *( tr.OpenPrice -tr.ClosePrice ) *tr.Volume;
					 strcpy(tr.CloseSignal,"stop_endtime");
					 mysqltestrecord.UpdatebyInstrument(this->tablename_test.c_str(),pinstrumentid,tr);
					 printf("	尾盘平仓   -------%s  ClosePrice %.3f Profit: %.2f  \n",mindata.MinuteTime,mindata.price,tr.Profit);

					 closeflag=true;
				 }
			 }
		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}
void DayTradeTest::Create_TestReportXlsFile(std::string pathfile)
	{

		workbook wb;
		xlslib_core::xf_t* xf = wb.xformat();

		xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
		xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
		xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
		xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

		worksheet* ws;
		ws = wb.sheet("testreport");

		int MAX_Length =15;
		int start=1;
		int i=start;

		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*18);
		ws->colwidth(i++,256*18);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*15);
		ws->colwidth(i++,256*15);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*12);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*15);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);

		i = start;

		ws->label(1,i++,"序号",xf);
		ws->label(1,i++,"品种",xf);
		ws->label(1,i++,"初始资金",xf);
		ws->label(1,i++,"总盈利额",xf);
		ws->label(1,i++,"总亏损额",xf);
		ws->label(1,i++,"总交易次数",xf);
		ws->label(1,i++,"盈利次数",xf);
		ws->label(1,i++,"平均盈利",xf);
		ws->label(1,i++,"平均亏损",xf);
		ws->label(1,i++,"盈亏比",xf);
		ws->label(1,i++,"胜率",xf);
		ws->label(1,i++,"期望值",xf);
		ws->label(1,i++,"错误数据天数",xf);
		ws->label(1,i++,"最终收益",xf);
		ws->label(1,i++,"Section",xf);

//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);

		range * _range;

		_range = ws->rangegroup(1,1,1,15);
		_range->cellcolor(CLR_GOLD);

		MysqlTestAsset mysqltestasset;

				std::string totaltablename;
				totaltablename.append(this->GetStrategy().c_str());
				totaltablename.append(this->GetYear().c_str());


		MysqlTestReport mysqltestreport;
		vector<TestReportData>  list;

		printf("最终汇总表   -------%s   \n ",this->GetTableReport().c_str());
		mysqltestreport.Find_ListsOrderbyExp(list,this->GetTableReport().c_str());



		int y=0;
int wincount=0;
int tradecount=0;
double totalprofit=0;
double totalcommission=0;
int row;

		for (auto &item:list){

		  int row=y+2;


//		  char number[10];
//		  sprintf(number,"%d",i+1);
		  ws->number(row,1,y+1,xf);
		  ws->label(row,2,item.Product,xf);

		  char buf[20];
		  sprintf(buf,"%.2f",item.InitialMoney);
		  ws->number(row,3,item.InitialMoney,xf);


//		  sprintf(buf,"%.2f",item.TotalWinProfit);
		  ws->number(row,4,item.TotalWinProfit,xf);

//		  sprintf(buf,"%.2f",item.TotalLoseProfit);
		  ws->number(row,5,item.TotalLoseProfit,xf);

//		  sprintf(buf,"%d",item.TradeCount);
		  ws->number(row,6,item.TradeCount,xf);

//		  sprintf(buf,"%d",item.WinCount);
		  ws->number(row,7,item.WinCount,xf);

//		  sprintf(buf,"%.2f",item.AverageWinProfit);
		  ws->number(row,8,item.AverageWinProfit,xf);


//		  sprintf(buf,"%.2f",item.AverageLoseProfit);
		  ws->number(row,9,item.AverageLoseProfit,xf);

//		  sprintf(buf,"%.2f",item.WinLoseRate);
		  ws->number(row,10,item.WinLoseRate,xf);

//		  sprintf(buf,"%.2f%%",item.WinRate*100);
		  ws->number(row,11,item.WinRate,xf);

//		  sprintf(buf,"%.2f",item.Expectation);
		  ws->number(row,12,item.Expectation,xf);

//		  sprintf(buf,"%d",item.ErrorMinuteData);
		  ws->number(row,13,item.ErrorMinuteData,xf);

//		  sprintf(buf,"%.2f",item.Asset);
//		  tradecount+=item.TradeCount;
//		  wincount+=item.WinCount;
		  totalcommission+=item.Commission;
		  totalprofit+=item.Asset;
		  ws->number(row,14,item.Asset,xf);
		  ws->label(row,15,item.Section,xf);

		  if(item.Expectation <0 )	{
					range * range_r;

					range_r = ws->rangegroup(row,1,row,14);
					range_r->cellcolor(CLR_GOLD);

//				_range = ws->rangegroup(row,1,row,18);
//				_range->cellcolor(CLR_GOLD);
//				printf("    %s 品种期望值为负值  \n",item.Product);
			}

			y++;
		}

		y++;
				row=y+2;
				ws->label(row,3,"总盈利",xf);
				ws->label(row,4,"总交易次数",xf);
				ws->label(row,5,"盈利次数",xf);
				ws->label(row,6,"胜率",xf);
				ws->label(row,7,"历史最高",xf);
				ws->label(row,8,"最大回撤",xf);
				ws->label(row,9,"佣金",xf);
				ws->label(row,10,"最多持仓合约",xf);
				ws->label(row,11,"最高保证金",xf);
				y++;

				row=y+2;
				tradecount = mysqltestasset.CountbyAll(totaltablename.c_str());
				 wincount = mysqltestasset.CountbyWinProfit(totaltablename.c_str());
				 ws->number(row,3,mysqltestasset.Get_SumbyProfit(totaltablename.c_str()),xf);
				 ws->number(row,4,tradecount,xf);
				 ws->number(row,5,wincount,xf);

				  char buf[20];
				  sprintf(buf,"%.2f%%",(double)wincount/(double)tradecount*100.0);
				  ws->label(row,6,buf,xf);

				 ws->number(row,7,mysqltestasset.Get_MaxbyHighestAsset(totaltablename.c_str()),xf);
				 ws->number(row,8,mysqltestasset.Get_MaxbyBackDownRate(totaltablename.c_str()),xf);
				 ws->number(row,9,mysqltestasset.Get_SumbyCommission(totaltablename.c_str()),xf);

				  ws->number(row,10,mysqltestasset.Get_MaxbySameTimeHold(totaltablename.c_str()),xf);
				 ws->number(row,11,mysqltestasset.Get_MaxbyMarginRate(totaltablename.c_str()),xf);



		wb.Dump(pathfile);
		vector<TestReportData> ().swap(list);
		printf("最终汇总表完成  end! ...\n");

}
void DayTradeTest::Create_TestRecordXlsFile(std::string pathfile)
	{

		workbook wb;
		xf_t* xf = wb.xformat();
		xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
		xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
		xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
		xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

		worksheet* ws;
		ws = wb.sheet("testrecord");
			ws->colwidth(0,256*2);
			ws->colwidth(1,256*5);
			ws->colwidth(2,256*10);
			ws->colwidth(3,256*22);
			ws->colwidth(4,256*5);
			ws->colwidth(5,256*10);
			ws->colwidth(6,256*10);
			ws->colwidth(7,256*10);
			ws->colwidth(8,256*10);
			ws->colwidth(9,256*10);
			ws->colwidth(10,256*10);
			ws->colwidth(11,256*15);
			ws->colwidth(12,256*15);
			ws->colwidth(13,256*10);
			ws->colwidth(14,256*12);
			ws->colwidth(15,256*10);
			ws->colwidth(16,256*15);
			ws->colwidth(17,256*10);
			ws->colwidth(18,256*10);
			ws->colwidth(19,256*15);
			ws->colwidth(20,256*22);
			ws->colwidth(21,256*8);
			ws->colwidth(22,256*8);
			ws->colwidth(23,256*8);
			ws->colwidth(24,256*8);
			ws->colwidth(25,256*8);
			ws->colwidth(26,256*8);
			ws->colwidth(27,256*8);
			ws->colwidth(28,256*8);
			ws->colwidth(29,256*8);
			ws->colwidth(30,256*8);

			ws->label(1,1,"序号",xf);
			ws->label(1,2,"合约",xf);
			ws->label(1,3,"开仓时间",xf);
			ws->label(1,4,"方向",xf);
			ws->label(1,5,"保证金",xf);
			ws->label(1,6,"资金利用率",xf);
			ws->label(1,7,"当日开盘价",xf);
			ws->label(1,8,"开仓价",xf);
			ws->label(1,9,"涨跌幅",xf);
			ws->label(1,10,"止损价",xf);
			ws->label(1,11,"平仓价",xf);
			ws->label(1,12,"持仓最高价",xf);
			ws->label(1,13,"持仓最低价",xf);
			ws->label(1,14,"交易手数",xf);
			ws->label(1,15,"状态",xf);
			ws->label(1,16,"盈亏",xf);
			ws->label(1,17,"佣金",xf);
			ws->label(1,18,"最终收益",xf);
			ws->label(1,19,"平仓信号",xf);
			ws->label(1,20,"平仓时间",xf);
			ws->label(1,21,"pATR",xf);
			ws->label(1,22,"Open_AddOPIRate",xf);
			ws->label(1,23,"Open_VolOpiRate",xf);

			ws->label(1,24,"形态",xf);
			ws->label(1,25,"EMA20_X",xf);
			ws->label(1,26,"EMA9_X",xf);
			ws->label(1,27,"EMA4_X",xf);
			ws->label(1,28,"EMA50",xf);
			ws->label(1,29,"EMA9",xf);
			ws->label(1,30,"EMA4",xf);
	//				cell_t * cell;
	//				cell = ws->label(0,0,"world");
	//
	//				cell->fillfgcolor(CLR_RED);
	//				cell->borderstyle(0,1);
			int colume_size =30;

		range * _range;

		_range = ws->rangegroup(1,1,1,colume_size);
		_range->cellcolor(CLR_GOLD);


		vector<MainContractData>  list;
		MysqlTestRecord mysqltestrecord;
		MysqlMaincontract mysqlmaincontract;
		mysqlmaincontract.Find_AllList(list);
		int i=0;
		for (auto &item:list)	{


			vector<TestRecordData>  ins_list;

			std::string tablename =this->GetTable_Record_Prefix();
			tablename.append(item.ProductID);
			if (mysqltestrecord.Exist_Table(tablename.c_str()))		{
				mysqltestrecord.Find_AllLists(ins_list,tablename.c_str());


				for (auto &record:ins_list)		{
					  int row=i+2;


					  ws->number(row,1,i+1,xf);

					  char buf[100];
					  sprintf(buf,"%s",record.InstrumentID);
					  ws->label(row,2,buf,xf);


					  sprintf(buf,"%s",record.OpenDateTime);
					  ws->label(row,3,buf,xf);

					  ws->number(row,4,record.Direction,xf);
					  ws->number(row,5,record.UseMargin,xf);
					  ws->number(row,6,record.UseMarginRate,xf);
					  ws->number(row,7,record.DayOpenPrice,xf);
					  ws->number(row,8,record.OpenPrice,xf);

					  ws->number(row,9,(record.OpenPrice-record.PreSettlePrice)/record.PreSettlePrice,xf);

					  ws->number(row,10,record.StopPrice,xf);
					  ws->number(row,11,record.ClosePrice,xf);


					  ws->number(row,12,record.Highest,xf);
					  ws->number(row,13,record.Lowest,xf);
					  ws->number(row,14,record.Volume,xf);
					  ws->number(row,15,record.Status,xf);
					  ws->number(row,16,record.Profit,xf);
					  ws->number(row,17,record.Commission,xf);
					  ws->number(row,18,record.Asset,xf);

					  sprintf(buf,"%s",record.CloseSignal);
					  ws->label(row,19,buf,xf);

					  sprintf(buf,"%s",record.CloseDateTime);
					  ws->label(row,20,buf,xf);

					  ws->number(row,21,record.pATR,xf);
					  ws->number(row,22,record.Open_AddOPIRate,xf);
					  ws->number(row,23,record.Open_VolOpiRate,xf);

					  sprintf(buf,"%s",record.FiveDayLineType);
					  ws->label(row,24,buf,xf);

					  ws->number(row,25,record.EMA20_X,xf);
					  ws->number(row,26,record.EMA9_X,xf);
					  ws->number(row,27,record.EMA4_X,xf);
					  ws->number(row,28,record.EMA50,xf);
					  ws->number(row,29,record.EMA9,xf);
					  ws->number(row,30,record.EMA4,xf);

				  if(record.Profit <0 )		{
						range * range_r;

						range_r = ws->rangegroup(row,1,row,colume_size);
						range_r->cellcolor(CLR_GOLD);

		//				_range = ws->rangegroup(row,1,row,18);
		//				_range->cellcolor(CLR_GOLD);
		//				printf("    %s 品种期望值为负值  \n",item.Product);
					}

					i++;
				}
			}

			vector<TestRecordData> ().swap(ins_list);
	//						printf("MongoMainContract Find_All  end! ...\n");

		}

		wb.Dump(pathfile);

		vector<MainContractData> ().swap(list);
}

void DayTradeTest::Create_StatisticsXlsFile(std::string pathfile)
	{

		workbook wb;
		xf_t* xf = wb.xformat();
		xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
		xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
		xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
		xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

		worksheet* ws;
		ws = wb.sheet("testrecord");
			ws->colwidth(0,256*2);
			ws->colwidth(1,256*5);
			ws->colwidth(2,256*10);
			ws->colwidth(3,256*15);
			ws->colwidth(4,256*15);
			ws->colwidth(5,256*15);

			ws->label(1,1,"序号",xf);
			ws->label(1,2,"品种",xf);
			ws->label(1,3,"最大沉淀资金",xf);
			ws->label(1,4,"最小沉淀资金",xf);
			ws->label(1,5,"平均沉淀资金",xf);

	//				cell_t * cell;
	//				cell = ws->label(0,0,"world");
	//
	//				cell->fillfgcolor(CLR_RED);
	//				cell->borderstyle(0,1);
			int colume_size =5;

		range * _range;

		_range = ws->rangegroup(1,1,1,colume_size);
		_range->cellcolor(CLR_GOLD);


		vector<MainContractData>  list;

		MysqlMaincontract mysqlmaincontract;
		mysqlmaincontract.Find_AllList(list);

		MysqlProStatis mysqlprostatis;

		int i=0;
		for (auto &item:list)	{

			std::string tablename_ps = item.ProductID;
			tablename_ps.append("_");
			tablename_ps.append(this->GetYear());

			if(!mysqlprostatis.Exist_Table(tablename_ps.c_str())){
				continue;
			}
			double avesed =0.0;
			avesed=mysqlprostatis.GetAverageSedimentary(tablename_ps.c_str());

			ProductStatisticsData maxdata ;
			memset(&maxdata,0,sizeof(maxdata));
			maxdata= mysqlprostatis.Find_MaxSedimentary(tablename_ps.c_str());

			ProductStatisticsData mindata;
			memset(&mindata,0,sizeof(mindata));
			mindata =  mysqlprostatis.Find_MinSedimentary(tablename_ps.c_str());

			int row=i+2;


			ws->number(row,1,i+1,xf);

			char buf[100];
			sprintf(buf,"%s",item.ProductID);
			ws->label(row,2,buf,xf);

			ws->number(row,3,maxdata.Sedimentary,xf);
			ws->number(row,4,mindata.Sedimentary,xf);

			sprintf(buf,"%.2f",avesed);
			ws->number(row,5,atof(buf),xf);

			i++;
		}

		wb.Dump(pathfile);

		vector<MainContractData> ().swap(list);
}
