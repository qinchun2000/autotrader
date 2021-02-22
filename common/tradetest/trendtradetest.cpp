#include "trendtradetest.hpp"

TrendTradeTest::TrendTradeTest()
{
	printf("TrendTradeTest --> 构造函数   \n");
}

TrendTradeTest::TrendTradeTest(const char* pstrategy,const char* year)
{
	printf("TrendTradeTest --> 带参数 构造函数   \n");
	this->SetInitialMoney(50000.00);

	//定义开平仓时间  0, 盘中开仓   1,尾盘开仓
	this->SetOpenTimeMode(0);
	this->SetCloseTimeMode(0);

	this->SetRiskRate(0.03);
	this->SetStopRatio(0.8);  // atr20 止损基准价的倍数
	this->SetWinRatio(1.2);  // stoprange 最终止损价差的倍数
	this->SetSingleMarginRate(0.2); //每次开仓 保证金不能超过的比例
	this->SetLimitSedimentary(2); //沉淀资金最低限制,不满足则不开仓

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
// year    ----- 2017
void TrendTradeTest::AllMainContract_TrendTradeTest(const char* year)
{
	MysqlUtil mysqlutil("test");
	vector<MainContractData>  list;
	MysqlMaincontract mysqlmaincontract;
	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)	{

		vector<std::string> instrument_list;

//			printf("i   ----->  %d  \n",i);


		GetInstrumentList(instrument_list,year,item);

		bool not_ap = strcmp(item.ProductID,"AP")!=0;
		bool not_stock = strcmp(item.Section,"Stock")!=0;
		bool not_finance = strcmp(item.Section,"Finance")!=0;
		bool not_metal = strcmp(item.Section,"Metal")!=0;
		bool not_precious = strcmp(item.Section,"Precious")!=0;

//		if ( not_ap  && not_stock  &&  not_finance  && not_metal && not_precious ) {
//		if ( not_stock && not_metal&& not_finance ){

//			if(strcmp(item.ProductID,"b")!=0 || strcmp(item.ProductID,"jd")!=0 || strcmp(item.ProductID,"j")!=0
//					|| strcmp(item.ProductID,"SF")!=0 || strcmp(item.ProductID,"ru")!=0 || strcmp(item.ProductID,"FG")!=0
//					|| strcmp(item.ProductID,"cs")!=0 ){
//		if(strcmp(item.Section,"Black")==0){
		if (strcmp(item.ProductID,"i")==0 || strcmp(item.ProductID,"rb")==0 || strcmp(item.ProductID,"ag")==0
				|| strcmp(item.ProductID,"MA")==0){
//		if (strcmp(item.ProductID,"rb")==0 || strcmp(item.ProductID,"ag")==0 ){

				MysqlTestRecord mysqltestrecord;
				this->SetTableRecord(item.ProductID);
				this->SetProductID(item.ProductID);
				this->SetVolumeMultiple(item.VolumeMultiple);

				if (!mysqltestrecord.Exist_Table(GetTableRecord().c_str()))		{
					SingleIns_Trade(instrument_list);
				}
				else	{
					printf(  " [%s] 品种已经完成测试 !!! \n",item.ProductID)	;
				}

			}
//		}


		vector<std::string>().swap(instrument_list);

	}

	vector<MainContractData>().swap(list);
}

void TrendTradeTest::SingleIns_Trade(vector<std::string> &array)
{
	std::string tablename =this->GetTable_Year_DateTime().c_str();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;
	mysqldatetime.Find_AllLists(list,tablename.c_str());

//	mysqldatetime.Find_AllListsbyStartSqlTime(list,tablename.c_str(),"2018-11-27 00:00:00");
	MysqlDayLine mysqldayline;
	DateTime enddatetime =mysqldatetime.Find_LastDateTime(tablename.c_str());

	int i=0;

	for (auto &dt:list)	{

		i++;
//		if (i<61){
//			continue;
//		}

		this->SetCusor_DaySqlTime(dt.Time);

		printf("[%s]---------\n",dt.Time);

		HxDayLineData main=	this->Get_CurrentMaincontractbyTime(array,dt.Time);
//		printf("[%s]--------- 主力合约 :  %s   open: %.3f  \n",dt.Time,main.InstrumentID,main.Open);
		std::string maininstrument=main.InstrumentID;

		if (maininstrument.length()>2)	{
			this->SetMainInstrumentID(main.InstrumentID);

			CheckErrorData(main.InstrumentID,dt.Time);
			MysqlTestRecord mysqltestrecord;
			if(!mysqltestrecord.Exist_Table(this->GetTableRecord().c_str()))	{
				mysqltestrecord.CreateTable(this->GetTableRecord().c_str());
			}

			int count  =mysqltestrecord.Count_RecordByStatus(this->GetTableRecord().c_str(),0);   // 0 有开仓未平的合约   1   有开仓已经平仓的合约

			if(count==0)   	{// 空仓
//							printf("---空仓 %s  -------    -------%s  \n",
//									main.InstrumentID,this->GetCusor_DaySqlTime().c_str());
				//趋势交易
//						OneDay_MinuteLine_Cycle_Open_();
//				OneDay_MinuteLine_DynamicCycle_Open_();
//						OneDay_MinuteLine_DynamicLastATRCycle_Open_();
//						OneDay_MinuteLine_MA102050_Open_();
//						OneDay_MinuteLine_DynamicMACycle_Open_();
//						OneDay_MinuteLine_MA_Open_();
//						OneDay_MinuteLine_PreDayPrice_Open_();
//						OneDay_MinuteLine_EMA_Open_();

						BigOneMinVolume_Open_MinuteLine();
//						OneDay_MinuteLine_MA4918_Open_();
//						OneDay_MinuteLine_MA4950_Open_();
			}
			else	{

//						printf("---持仓 %s  -------    -------%s  \n",main.InstrumentID,this->GetCusor_DaySqlTime().c_str());
				vector<TestRecordData>  record_list;

				 mysqltestrecord.Find_HoldLists(record_list,this->GetTableRecord().c_str());

				 if(record_list.size() >1)	 {
					 printf("	同时持仓合约数(主力切换)-------> %ld   \n" ,record_list.size());
				 }

				 for (auto &record:record_list)	 {
					 if (strcmp(record.InstrumentID,main.InstrumentID)==0)	 {
						 printf("	主力合约  %s -------  ----%s  \n",record.InstrumentID,dt.Time);
					 }
					 else	 {
						 printf("	非主力合约  %s -------  -------%s  \n",record.InstrumentID,dt.Time);
					 }

					 if(mysqldayline.Exist_DatabySqlTime(record.InstrumentID,dt.Time))	 {

						 HxDayLineData holddata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
						 if (holddata.Volume <30000 )	 {
							 printf("	持仓合约  %s -------  成交量变小 平仓  -------%s  \n",record.InstrumentID,dt.Time);

							 EndDayClose(record.InstrumentID,holddata);
						 }
						 else	 {
							 printf("	持仓合约   %s 现持有仓位:[%d] 已平仓数:[%d]-------    -------%s  \n",record.InstrumentID,record.Volume-record.CloseVolume,record.CloseVolume,dt.Time);

//							 OneDay_MinuteLine_PreDayPrice_Hold(record.InstrumentID);
	//						 MA_EndTime_Hold_MinuteLine(record.InstrumentID);
//							 OneDay_MinuteLine_EMA_Hold(record.InstrumentID);
							 BigVolume_Hold_MinuteLine(record.InstrumentID);
						 }


					 }
					 else	 {
						 printf("	持仓合约 持有无日线数据   %s -------    -------%s  \n",record.InstrumentID,dt.Time);
						 HxDayLineData lastdata=mysqldayline.Find_LastDayLineOffset(record.InstrumentID,5);
						 EndDayClose(record.InstrumentID,lastdata);
					 }


					 if (strcmp(dt.Time,enddatetime.Time)==0) {
						 printf("	持仓合约 截止年度最后交易日   %s -------    -------%s  \n",record.InstrumentID,dt.Time);
						 HxDayLineData lastdata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
						 EndDayClose(record.InstrumentID,lastdata);
					 }

				 }

				 vector<TestRecordData>().swap(record_list);
			}

		}
		else	{
			printf("[%s]--------- 无主力合约 :  %s    \n",dt.Time,this->GetProductID().c_str());
		}

	}
	vector<DateTime>().swap(list);


}

void TrendTradeTest::Cycle_Open_MinuteLine()
{
//	printf ("Cycle_Open_MinuteLine begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;

	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlAvePrice mysqlaveprice;
	MysqlTestRecord mysqltestrecord;
	MysqlCycle mysqlcycle;


	DateUtil dateutil;
	SqlAveragePriceData todayavedata = mysqlaveprice.Find_DateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

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
		CheckErrorData(productid);

	}

	CycleData cycledata = mysqlcycle.Find_DatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	CycleData highdata = mysqlcycle.GetHighestbyHighCount(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
	CycleData lowdata = mysqlcycle.GetLowestbyLowCount(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);


	SqlAveragePriceData predata = mysqlaveprice.Find_YesterdayDatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	double uplimit = today.Open *1.01;
	double downlimit = today.Open *0.99;



	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	this->SetStopRange(atr_data.ATR *this->GetStopRatio());

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		 if(opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 double ma50;
			double ma20,ma10;

			ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
			ma20 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,mindata.price);
			ma10 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,mindata.price);


			 bool condition_long1  = ma50<ma20 && ma20< ma10 ;
			bool condition_long2 = mindata.price > today.Open*1.005 &&  today.Open > predata.AveragePrice*1.0025;
			bool condition_long4 = atrcount2x ==0;
			bool condition_long3 =false;


			bool condition_atr = atr_data.ATR/mindata.price <0.035;
			double highest ;


			if (cycledata.HighCount>count && mindata.price > cycledata.High20)
			{
				condition_long3=true;
//				printf ("check %s mindata : condition_long3  %s cycledata.HighCount: %d  mindata.price: %.3f  cycledata.High20:%.3f  \n",
//													tablename.c_str(),mindata.MinuteTime,cycledata.HighCount,mindata.price,cycledata.High20);
			}
			else
			{


				if (highdata.High20 > cycledata.High20)
				{
					highest = highdata.High20;
				}
				else
				{
					highest = cycledata.High20;
				}

				if (mindata.price > highest)
				{
//					condition_long3=true;
				}
			}

			bool condition_long_pattern =condition_long3  && condition_long2;


			bool condition_short1  = ma50>ma20 && ma20> ma10;
			bool condition_short2 = mindata.price < today.Open*0.995 && today.Open < predata.AveragePrice*0.9975;
			bool condition_short3=false ;
			bool condition_short4 = atrcount2x ==0;



			double lowest ;
			if (cycledata.LowCount >count && mindata.price < cycledata.Low20)
			{
				condition_short3=true;
			}
			else
			{


				if (lowdata.Low20 < cycledata.Low20)
				{
					lowest = lowdata.Low20;
				}
				else
				{
					lowest = cycledata.Low20;
				}

				if (mindata.price < lowest)
				{
//					condition_short3=true;
				}
			}

			bool condition_short_pattern =condition_short3  && condition_short2 ;

			/*  价差开仓法 */

			bool condition_long_trend = condition_long1 ;
			bool condition_long = condition_long_trend    && condition_long_pattern;

			bool condition_short_trend = condition_short1;
			bool condition_short = condition_short_trend  && condition_short_pattern;


			 if (condition_long  )
				{



				 int atrcount = mysqlhisatr.Count_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
				 	int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
				 	double atr_indicator = mysqlhisatr.Indicator_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_open");

				}

			 if (condition_short )
				{
				 int atrcount = mysqlhisatr.Count_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
				 	int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
				 	double atr_indicator = mysqlhisatr.Indicator_ListsByTime_N(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
				 	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//				 	printf("ema50 %.3f  ema9 %.3f ema4 %.3f   \n",ema50,ema9,ema4);
//				 	printf("  %s-%s   atrcount -----[%d]    atrcount2x -----[%d]   atr_indicator  ---> %.2f ATR  ---> %.3f \n ",
//				 			pinstrumentid,datetime,atrcount,atrcount2x,atr_indicator, atr_data.ATR);
//				 	printf("  %s-%s   x50 -----[%.2f]    x20 -----[%.2f]   x10  ---> [%.2f] \n ",
//				 													pinstrumentid,datetime,x50,x20,x10);
//				 	printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f%%   下跌品种:%.2f%%   平均涨跌幅 : %.2f%% MinInstrumentID %s \n",hissection_table.c_str(),
//				 						mindata.MinuteTime,hissectiondata.CountUprate*100,hissectiondata.CountDownrate*100,100*hissectiondata.AverageUpDownrate,hissectiondata.MinInstrumentID);
//				 	int p= 20000.00/( mindata.price* maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio);

				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_open");


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


		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}
void TrendTradeTest::OneDay_MinuteLine_Cycle_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicCycle_Open_ begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

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

int n=20;

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else
	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);

	}


	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	CycleData precycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,n);


	HxDayLineData prepreday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),preday.Time);
	CycleData preprecycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),prepreday.Time,n);

	HxDayLineData pre_3_day =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),prepreday.Time);
	CycleData pre_3_cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),pre_3_day.Time,n);

//	this->SetStopRange(tr *this->GetStopRatio());

	double highest=today.Open;
	double lowest=today.Open;

int i=0;
int startopi;

	 for (auto &mindata:min_list)
	 {

		 if (i==0)
		 {
			 startopi=mindata.OpenInterest;
		 }
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );
		 if(mindata.price>highest)
		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)
		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }



		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;


		}
		else{
			opentime_mode=true;
		}

		 if(opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 double today_tr;
			 double tr_highlow=abs(highest-lowest);
			 double tr_highclose=abs(highest-preday.Close);
			 double tr_lowclose=abs(lowest-preday.Close);

			 today_tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
	//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
	//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n,today_tr);
			 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,today_tr);
//			 double atr5=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5,today_tr);

			 this->SetStopRange(atr20 *this->GetStopRatio());
			CycleData cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);

			 double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

//			 double ma50;
//			 ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);

//			 double ma_x50 = atan((ma50/pre_ma50_offset-1)*100)*180/M_PI;

			bool condition_long =   mindata.price > cycledata.High20   && preday.Close < precycledata.High20
					&& prepreday.Close < preprecycledata.High20  && pre_3_day.Close < pre_3_cycledata.High20 ;
			bool condition_short = mindata.price <  cycledata.Low20  && preday.Close > precycledata.Low20
					&& prepreday.Close > preprecycledata.Low20  && pre_3_day.Close < pre_3_cycledata.Low20;

//			bool condition_long_high =cycledata.High20  ==precycledata.High20 && precycledata.High20 ==preprecycledata.High20;
//			bool condition_long_short =cycledata.Low20  ==precycledata.Low20 && precycledata.Low20 ==preprecycledata.Low20;
//			bool condition_angle_long  =  ma_x50>0  && ma_x50!=90;
//			bool condition_angle_short = ma_x50<0  && ma_x50!=90 ;

			bool condition_atr = today_tr>atr10 && atr10>atr20 ;

			double opiadd_rate =(mindata.OpenInterest - startopi ) /startopi;
			bool condition_opi = opiadd_rate >0.05;

			bool condition_vot_long = mindata.price > today.Open  ;
			bool condition_vot_short= mindata.price < today.Open ;

//			bool condition_ma_long =mindata.price >ma50 && today.Open >ma50;
//			bool condition_ma_short =mindata.price <ma50 && today.Open <ma50;

			 if (condition_long && condition_vot_long  && sed>2)
				{

				 printf ("多头信号: cycledata  %s  n:  %d   date[%s]  最高价: %.2f tr[%.3f]  atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%      \n",
				 				tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,cycledata.High20 ,today_tr,atr10,
				 				atr20,atr20/today.Open*100,atr10/today.Open*100 );
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_n_open");

				}

			 if (condition_short && condition_vot_short   && sed>2)
				{

				 printf ("空头信号: cycledata  %s  n:  %d   date[%s]  最低价: %.2f tr[%.3f]  atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%      \n",
				 				tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,cycledata.Low20,today_tr,atr10,
				 				atr20,atr20/today.Open*100,atr10/today.Open*100);
				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_n_open");

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


		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}

void TrendTradeTest::OneDay_MinuteLine_PreDayPrice_Open_()
{
//	printf ("OneDay_MinuteLine_PreDayPrice_Open_ begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
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

int n=20;

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);

	}


	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	HxDayLineData prepreday =mysqldayline.Find_PreNDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),1);


//bool condition_long_dayline =  preday.Close >prepreday.High;
// bool condition_short_dayline =  preday.Close <prepreday.Low;
//
// if(condition_long_dayline){
//	 std::cout<< "pre 2--> "<<prepreday.Time<<" high:" <<prepreday.High<< "  close:"<<preday.Close<< " pre 1--> "<<preday.Time<<std::endl;
// }
// else if(condition_short_dayline){
//	 std::cout<< "pre 2--> "<<prepreday.Time<<" low:" <<prepreday.Low<< "  close:"<<preday.Close<< " pre 1--> "<<preday.Time<<std::endl;
// }
//	this->SetStopRange(tr *this->GetStopRatio());

	double tr;
	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

int i=0;
int startopi;
int total_volume=0;
	 for (auto &mindata:min_list)	 {
		 total_volume+=mindata.Volume;
		 if (i==0)		 {
			 startopi=mindata.OpenInterest;
		 }

		 this->update_HighLow(&mindata);
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		bool exist_order = mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),0);

		tr = this->zb_TR(preday.Close);

		 if(opentime_mode && !exist_order &&	  !this->GetOpenFlag() && !this->GetCloseFlag()) {

			 int preday_volume = this->zb_PreVolume(preday.Time);
			 int preday_opi =this->zb_PreOpi(preday.Time);
//std::cout<<"  minutetime:"<<mindata.MinuteTime<<"    preday_volume:"<<preday_volume<<"   preday_opi:"<<preday_opi<<endl;
			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n,tr);

//			 std::cout<< "  tr:"<<tr<<" atr20:"<<atr20<<endl;


			 double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;


			 bool long_kline = mindata.price>preday.High;
			 bool short_kline = mindata.price<preday.Low;
//			 printf ("   昨日最高:[%.2f] 昨日最低:[%.2f] 现价:[%.2f]  \n",preday.High,preday.Low,mindata.price);
//			 if(long_kline){
//				 printf (">>>>>昨日最高:[%.2f] 现价:[%.2f]  \n",preday.High,mindata.price);
//			 }
//			 else if(short_kline){
//				 printf (">>>>>昨日最低:[%.2f] 现价:[%.2f]  \n",preday.Low,mindata.price);
//			 }
			bool condition_atr = tr>atr20 ;
			double opiadd_rate =(mindata.OpenInterest - startopi ) /startopi;
			bool condition_opi = opiadd_rate >0.05;

			bool condition_vot_long = mindata.price > today.Open  ;
			bool condition_vot_short= mindata.price < today.Open ;

			bool condition_vol_long = total_volume > preday_volume;
			bool condition_vol_short = total_volume > preday_volume;

			bool condition_opi_long = mindata.OpenInterest > preday_opi;
			bool condition_opi_short = mindata.OpenInterest > preday_opi;


			 bool condition_long = long_kline &&condition_vol_long &&condition_opi_long;
			 bool condition_short = short_kline && condition_vol_short&&condition_opi_short;




			 if (condition_long   && sed>2)	{
				 double stoprange=fabs(mindata.price-preday.Low);
				 this->SetStopRange(stoprange);
				 printf ("多头信号:   %s  n:  %d   date[%s]  昨日最高:[%.2f] 现价:[%.2f] 止损价差:%.2f  tr[%.3f] --- atr20[%.3f]---20日波动率---> %.2f%%   \n",
				 				tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,preday.High,mindata.price,stoprange,tr,
				 				atr20,atr20/today.Open*100);
//				 std::cout<< this->GetCusor_DaySqlTime().c_str()<< "--->"<<std::endl;

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = 0;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;
				 Buy(mindata,this->GetMainInstrumentID().c_str(),"predayprice",&data);

				}

			 if (condition_short &&  sed>2)		{
				 double stoprange=fabs(preday.High-mindata.price);
				 this->SetStopRange(stoprange);
				 printf ("空头信号: cycledata  %s  n:  %d   date[%s]  昨日最低:[%.2f] 现价:[%.2f]  止损价差:%.2f tr[%.3f] --- atr20[%.3f]---20日波动率---> %.2f%%   \n",
							tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,preday.Low,mindata.price,stoprange,tr,atr20,atr20/today.Open*100);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = 0;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;

				 Sell(mindata, this->GetMainInstrumentID().c_str(),"predayprice",&data);

				}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)	 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)	{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}

//
//			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
//
//				 Close(tr,mindata,"stop_openday");
//
//			 }
//			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
//
//				 Close(tr,mindata,"stop_openday");
//			 }

		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}

void TrendTradeTest::OneDay_MinuteLine_PreDayPrice_Hold(const char* pinstrumentid)
{

//	printf ("OneDay_MinuteLine_PreDayPrice_Hold begin \n");

	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlHisMinData mysqlhismindata;

	vector<MinuteLineData>  min_list;

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());

	DateUtil dateutil;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	mysqlhismindata.Set_Database(min_database);

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

	if(!mysqlhismindata.Exist_Table(tablename.c_str()))	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}

	mysqlhismindata.Find_AllList(min_list,tablename.c_str());

	if(min_list.size()<=0 )	{

		printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	int total_volume=0;

	 for (auto &mindata:min_list)		 {

		 total_volume+=mindata.Volume;

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool closetime_mode;
		if(this->GetCloseTimeMode()==1)		{
			closetime_mode=t_current>t_endtime;
		}
		else{
			closetime_mode=true;
		}

		 if(closetime_mode && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) ) {

			 int preday_volume = this->zb_PreVolume(preday.Time);
			 int preday_opi =this->zb_PreOpi(preday.Time);

//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);
			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);

			 if (tr.Direction ==0   )		 {

				 if (mindata.price < preday.Low && mindata.OpenInterest<preday_opi && total_volume>preday_volume)	 {
					 Close(tr,mindata,"stop_reverse");
				  }
				 if (mindata.price < tr.StopPrice )	 {
					 Close(tr,mindata,"stop_hold");
				  }

			 }
			 else if (tr.Direction ==1    )		 {

				 if (mindata.price > preday.High && mindata.OpenInterest<preday_opi && total_volume>preday_volume)		 {
					 Close(tr,mindata,"stop_reverse");
				 }
				 if (mindata.price > tr.StopPrice )	 {
					 Close(tr,mindata,"stop_hold");
				  }

			 }

		 }//exist 存在持仓单


//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

	 }// for


//	printf ("check tablename : %s \n",tablename.c_str());

//	ATRData atr_data =mysqlhisatr.Find_DatabyDateTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
//		this->SetStopRange(atr_data.ATR *this->GetStopRatio());


	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )	 {
		 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
		 if (  tr.Direction==0 && today.Close> tr.Highest)		 {
				 tr.Highest=today.Close;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s 持仓中   更新最高收盘价   -------  %.3f   止损价  %.3f \n", today.Time,tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 pinstrumentid,today.Close,tr.StopPrice,0);

			 }
			 else if ( tr.Direction==1 && today.Close < tr.Lowest )	{
				 tr.Lowest=today.Close;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 持仓中   更新最低收盘价   -------%.3f    止损价  %.3f\n",today.Time,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,today.Close,tr.StopPrice,0);
			}

	 }


	vector<MinuteLineData>().swap(min_list);
}

void TrendTradeTest::OneDay_MinuteLine_EMA_Open_()
{
//	printf ("OneDay_MinuteLine_PreDayPrice_Open_ begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
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

int n=20;

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);

	}


	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
//	HxDayLineData prepreday =mysqldayline.Find_PreNDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),1);


//bool condition_long_dayline =  preday.Close >prepreday.High;
// bool condition_short_dayline =  preday.Close <prepreday.Low;
//
// if(condition_long_dayline){
//	 std::cout<< "pre 2--> "<<prepreday.Time<<" high:" <<prepreday.High<< "  close:"<<preday.Close<< " pre 1--> "<<preday.Time<<std::endl;
// }
// else if(condition_short_dayline){
//	 std::cout<< "pre 2--> "<<prepreday.Time<<" low:" <<prepreday.Low<< "  close:"<<preday.Close<< " pre 1--> "<<preday.Time<<std::endl;
// }
//	this->SetStopRange(tr *this->GetStopRatio());

	double tr;
	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

int i=0;
int startopi;
int total_volume=0;
	 for (auto &mindata:min_list)	 {
		 total_volume+=mindata.Volume;
		 if (i==0)		 {
			 startopi=mindata.OpenInterest;
		 }

		 this->update_HighLow(&mindata);
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );


		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		bool exist_order = mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),0);

		tr = this->zb_TR(preday.Close);

		 if(opentime_mode && !exist_order &&	  !this->GetOpenFlag() && !this->GetCloseFlag()) {

			 int preday_volume = this->zb_PreVolume(preday.Time);
			 int preday_opi =this->zb_PreOpi(preday.Time);
//std::cout<<"  minutetime:"<<mindata.MinuteTime<<"    preday_volume:"<<preday_volume<<"   preday_opi:"<<preday_opi<<endl;
			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n,tr);

//			 std::cout<< "  tr:"<<tr<<" atr20:"<<atr20<<endl;

			 double ema5 = mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
					 this->GetCusor_DaySqlTime().c_str(),5,mindata.price);
			 double ema10 = mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
								 this->GetCusor_DaySqlTime().c_str(),10,mindata.price);
			 double ema20 = mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
								 this->GetCusor_DaySqlTime().c_str(),20,mindata.price);

			 double ema50 = mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
											 this->GetCusor_DaySqlTime().c_str(),50,mindata.price);

			 double pre_ema50 = mysqldayline.Collect_EMA_NData(this->GetMainInstrumentID().c_str(),
			 											 this->GetCusor_DaySqlTime().c_str(),50);

			 double ema_x50 = atan((ema50/pre_ema50-1)*100)*180/M_PI;

			 double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;


			 bool long_ema =mindata.price>ema5&& ema5>ema10 && ema10 > ema20 && ema_x50>0;

			 bool short_ema =mindata.price<ema5&& ema5<ema10 && ema10 < ema20 && ema_x50<0;

			 bool long_kline = mindata.price>preday.High;
			 bool short_kline = mindata.price<preday.Low;
//			 printf ("   昨日最高:[%.2f] 昨日最低:[%.2f] 现价:[%.2f]  \n",preday.High,preday.Low,mindata.price);
//			 if(long_kline){
//				 printf (">>>>>昨日最高:[%.2f] 现价:[%.2f]  \n",preday.High,mindata.price);
//			 }
//			 else if(short_kline){
//				 printf (">>>>>昨日最低:[%.2f] 现价:[%.2f]  \n",preday.Low,mindata.price);
//			 }
			bool condition_atr = tr>atr20 ;
			double opiadd_rate =(mindata.OpenInterest - startopi ) /startopi;
			bool condition_opi = opiadd_rate >0.05;

			bool condition_vot_long = mindata.price > today.Open  ;
			bool condition_vot_short= mindata.price < today.Open ;

			bool condition_vol_long = total_volume > preday_volume;
			bool condition_vol_short = total_volume > preday_volume;

			bool condition_opi_long = mindata.OpenInterest > preday_opi;
			bool condition_opi_short = mindata.OpenInterest > preday_opi;


			 bool condition_long = long_ema;
			 bool condition_short = short_ema;




			 if (condition_long   && sed>2)	{
				 double stoprange=fabs(atr20*2);
				 this->SetStopRange(stoprange);
				 printf ("多头信号:   date[%s]  ema5:%.2f ema10:%.2f ema20:%.2f  ema50:%.2f  \n",
								this->GetCusor_DaySqlTime().c_str() ,ema5,ema10,ema20,ema50);
				 printf ("多头信号:   %s  n:  %d   date[%s]  昨日最高:[%.2f] 现价:[%.2f] 止损价差:%.2f  tr[%.3f] --- atr20[%.3f]---20日波动率---> %.2f%%   \n",
				 				tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,preday.High,mindata.price,stoprange,tr,
				 				atr20,atr20/today.Open*100);
//				 std::cout<< this->GetCusor_DaySqlTime().c_str()<< "--->"<<std::endl;

				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = 0;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;
				 Buy(mindata,this->GetMainInstrumentID().c_str(),"predayprice",&data);

				}

			 if (condition_short &&  sed>2)		{
				 double stoprange=fabs(atr20*2);
				 this->SetStopRange(stoprange);
				 printf ("空头信号:   date[%s]  ema5:%.2f ema10:%.2f ema20:%.2f  ema50:%.2f  \n",
				 								this->GetCusor_DaySqlTime().c_str() ,ema5,ema10,ema20,ema50);
				 printf ("空头信号: cycledata  %s  n:  %d   date[%s]  昨日最低:[%.2f] 现价:[%.2f]  止损价差:%.2f tr[%.3f] --- atr20[%.3f]---20日波动率---> %.2f%%   \n",
							tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,preday.Low,mindata.price,stoprange,tr,atr20,atr20/today.Open*100);
				 TestRecordData data;
				 data.DayOpenPrice= today.Open;
				 data.OpenOPI = 0;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR =sed;
				 data.Close_AddOPIRate=0.0;

				 Sell(mindata, this->GetMainInstrumentID().c_str(),"predayprice",&data);

				}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)	 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)	{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),mindata.price,tr.StopPrice,0);
			}

//
//			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
//
//				 Close(tr,mindata,"stop_openday");
//
//			 }
//			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
//
//				 Close(tr,mindata,"stop_openday");
//			 }

		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}


void TrendTradeTest::OneDay_MinuteLine_EMA_Hold(const char* pinstrumentid)
{

//	printf ("OneDay_MinuteLine_PreDayPrice_Hold begin \n");

	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlHisMinData mysqlhismindata;

	vector<MinuteLineData>  min_list;

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());

	DateUtil dateutil;

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	mysqlhismindata.Set_Database(min_database);

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

	if(!mysqlhismindata.Exist_Table(tablename.c_str()))	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}

	mysqlhismindata.Find_AllList(min_list,tablename.c_str());

	if(min_list.size()<=0 )	{

		printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	int total_volume=0;

	 for (auto &mindata:min_list)		 {

		 total_volume+=mindata.Volume;

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool closetime_mode;
		if(this->GetCloseTimeMode()==1)		{
			closetime_mode=t_current>t_endtime;
		}
		else{
			closetime_mode=true;
		}

		 if(closetime_mode && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) ) {

			 int preday_volume = this->zb_PreVolume(preday.Time);
			 int preday_opi =this->zb_PreOpi(preday.Time);

//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);
			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);

			 if (tr.Direction ==0   )		 {

//				 if (mindata.price < preday.Low && mindata.OpenInterest<preday_opi && total_volume>preday_volume)	 {
//					 Close(tr,mindata,"stop_reverse");
//				  }
				 if (mindata.price < tr.StopPrice )	 {
					 Close(tr,mindata,"stop_hold");
				  }

			 }
			 else if (tr.Direction ==1    )		 {

//				 if (mindata.price > preday.High && mindata.OpenInterest<preday_opi && total_volume>preday_volume)		 {
//					 Close(tr,mindata,"stop_reverse");
//				 }
				 if (mindata.price > tr.StopPrice )	 {
					 Close(tr,mindata,"stop_hold");
				  }

			 }

		 }//exist 存在持仓单


//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

	 }// for


//	printf ("check tablename : %s \n",tablename.c_str());

//	ATRData atr_data =mysqlhisatr.Find_DatabyDateTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
//		this->SetStopRange(atr_data.ATR *this->GetStopRatio());


	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )	 {
		 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
		 if (  tr.Direction==0 && today.Close> tr.Highest)		 {
				 tr.Highest=today.Close;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s 持仓中   更新最高收盘价   -------  %.3f   止损价  %.3f \n", today.Time,tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 pinstrumentid,today.Close,tr.StopPrice,0);

			 }
			 else if ( tr.Direction==1 && today.Close < tr.Lowest )	{
				 tr.Lowest=today.Close;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 持仓中   更新最低收盘价   -------%.3f    止损价  %.3f\n",today.Time,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,today.Close,tr.StopPrice,0);
			}

	 }


	vector<MinuteLineData>().swap(min_list);
}
void TrendTradeTest::InitialSingleTest()
{
	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	MysqlMaincontract mysqlmaincontract;

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	this->SetVolumeMultiple(maincontractdata.VolumeMultiple);

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(this->GetProductID().c_str());
	this->SetMargin(commission.Margin);

	this->SetDataBase_HisMindata(this->GetProductID().c_str());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);


}
// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_DynamicCycle_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicCycle_Open_ begin \n");

//	int count=2;
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlHisMinData mysqlhismindata;
	vector<HxDayLineData>  N20_list;
	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	InitialSingleTest();
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",this->GetTable_HisMindata().c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}
	this->SetN(20);
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	mysqldayline.Find_ListsbyDate(N20_list,this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),this->GetN()-1);

	if(N20_list.size()<this->GetN()-1){
		return;
	}
	int n_short =10;

	int n;
	double k ;

	HxDayLineData preday = this->data_PreDay();
	double presettleprice = this->zb_PreSettlePrice(preday.Time);
	if(presettleprice==0){
		return;
	}

	CycleData precycledata = this->data_PreDynamicCycleData(preday.Time);
	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

	int i=0;
	int startopi;
	int total_volume=0;

	 for (auto &mindata:min_list)	 {

		 total_volume+=mindata.Volume;
		 if (i==0)		 {
			 startopi=mindata.OpenInterest;
		 }
		 i++;

		 this->update_HighLow(&mindata);

//		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//		double addopi = ((double)mindata.OpenInterest-(double)startopi);
//		double opiadd_rate;
//		if(startopi!=0){
//			opiadd_rate=(mindata.OpenInterest - startopi ) /(double)startopi;
////			printf("	startopi:%d   mindata.OpenInterest:%d opiadd_rate  %.2f\n",startopi,mindata.OpenInterest,opiadd_rate);
//		}
//		else{
//			opiadd_rate=0;
//		}
//
//
//		double rate;
//		if (mindata.OpenInterest>0){
//			rate=(double)total_volume/mindata.OpenInterest;
//		}
//		else{
//			rate=0;
//		}

		 double current_tp = (this->GetTodayHigh()+this->GetTodayLow()+mindata.price)/3.0;
		 double cci=0.0;
		if(N20_list.size()==this->GetN()-1){
		//		printf ("GKGZ_Open_MinuteLine   %ld \n",fivedays_list.size());
				cci=this->zb_CCI(N20_list,current_tp);
			}
			else{
		//		printf ("GKGZ_Open_MinuteLine ->  %ld \n",fivedays_list.size());
			}

		 bool exist_order = mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),0);
		 bool valid_opentime =this->condition_OpenTime(&mindata,0) ;
//		 printf ("check %s     ---->   mindata : %s \n",tablename.c_str(),mindata.MinuteTime  );
		 if(valid_opentime && !exist_order	 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {

			 double sed=this->zb_Sedimentary(&mindata);

			 if(mindata.OpenInterest>0 && sed<this->GetLimitSedimentary()){
//				printf(" current_time %s  sed:%.2f \n",mindata.MinuteTime,sed);
				 return;
			 }
			 else{
//				 printf(" current_time %s  opi:%d sed:%.2f \n",mindata.MinuteTime,mindata.OpenInterest,sed);
			 }

			 double today_tr=this->zb_TR(preday.Close);

			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),this->GetN(),today_tr);
			 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short,today_tr);
			 double atr5=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5,today_tr);

			 this->SetStopRange(atr20 *this->GetStopRatio());

			 if(atr10 >1.0 && atr20 >1.0)			{
				k = atr20/atr10;
			}
			else			{
				k=1.0;
			}
			n=floor(this->GetN()*k);
			CycleData cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);


			bool condition_long =   mindata.price > cycledata.High20   && preday.Close < precycledata.High20 ;
			bool condition_short = mindata.price <  cycledata.Low20  && preday.Close > precycledata.Low20 ;

//			bool condition_angle_long  =  ma_x50>0  && ma_x50!=90;
//			bool condition_angle_short = ma_x50<0  && ma_x50!=90 ;

			bool condition_atr = today_tr>atr10 && atr10>atr20 ;
//			bool condition_opi = opiadd_rate >0.05;

			bool condition_kline_long = mindata.price > today.Open && today.Open >presettleprice ;
			bool condition_kline_short= mindata.price < today.Open && today.Open <presettleprice ;

//			bool condition_ma_long =mindata.price >ma50 && today.Open >ma50;
//			bool condition_ma_short =mindata.price <ma50 && today.Open <ma50;

			 bool condition_long_cci = cci>100 ;
			 bool condition_short_cci = cci<-100;

			double patr10 = atr10/today.Open;
			bool fridaynight = dateutil.Check_FridayNithtbySqlTime(mindata.MinuteTime);
			bool valid_patr= patr10<0.025;
//			bool valid_opiaddrate = opiadd_rate>-0.005;
			bool valid= sed>this->GetLimitSedimentary() && !fridaynight ;

			 if (valid && condition_long && condition_kline_long  &&condition_long_cci  )				{

//				 printf ("\n	多头信号: cycledata  %s  n:  %d   date[%s]  增仓率 %.2f%% \n",
//						 this->GetTable_HisMindata().c_str(),n,this->GetCusor_DaySqlTime().c_str() ,opiadd_rate*100);
				 printf ("	tr[%.3f] atr5[%.3f] atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
							today_tr,atr5,atr10,atr20,atr20/today.Open*100,atr10/today.Open*100 ,k);
//				 printf("	多头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  rate : %.2f \n",
//							 total_volume,startopi,mindata.OpenInterest,rate);
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR = patr10;
				 data.DayOpenPrice = today.Open;
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_n_open",&data);
			}
			 else if (valid &&condition_short && condition_kline_short&&condition_short_cci )				{

//				 printf ("\n	空头信号: cycledata  %s  n:  %d   date[%s]  增仓率 %.2f%%   \n",
//						 this->GetTable_HisMindata().c_str(),n,this->GetCusor_DaySqlTime().c_str() ,opiadd_rate*100);
				 printf ("	tr[%.3f]  atr5[%.3f] atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
							 today_tr,atr5,atr10,atr20,atr20/today.Open*100,atr10/today.Open*100 ,k);
//				 printf("	空头开仓信号 ----------------->totalvolume:%d  staropi:%d  endopi:%d  rate : %.2f \n",
//							 total_volume,startopi,mindata.OpenInterest,rate);
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR = patr10;
				 data.DayOpenPrice = today.Open;
				 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_n_open",&data);
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && exist_order	 && this->GetOpenFlag() && !this->GetCloseFlag())	 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }


//			 if (tr.Direction ==0 &&  this->condition_EndTime(&mindata) &&( rate<0.5 || rate>2))	 {
//				 printf("	多头合约 开仓日尾盘平仓信号 [stop_endvolopi]----------------->totalvolume:%d  staropi:%d  endopi:%d  rate : %.2f \n",
//						 total_volume,startopi,mindata.OpenInterest,rate);
//				 Close(tr,mindata,"stop_endvolopi");
//			 }
//			 else if(tr.Direction ==1 && this->condition_EndTime(&mindata)  ){
//
//			 }
		 }
	 }

	vector<MinuteLineData>().swap(min_list);
	vector<HxDayLineData>().swap(N20_list);
}

// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_DynamicLastATRCycle_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicCycle_Open_ begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:58:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
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
		CheckErrorData(productid);

	}


	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	int n_long =20;
	int n_short =10;
	ATRData preday_atr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long);
	ATRData preday_atr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short);
//
	int n ;
	double k ;
	if(preday_atr10_data.ATR >1.0 && preday_atr20_data.ATR >1.0)
	{
		k = preday_atr20_data.ATR/preday_atr10_data.ATR;
	}
	else
	{
		k=1.0;
	}

	n=floor(n_long*k);
	CycleData cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);


	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	ATRData preatr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_long);
	ATRData preatr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_short);

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
	CycleData precycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,pre_n);


//	double today_tr ;
//	today_tr=max(abs(today.High-today.Low),max(abs(today.High-today.LastClose),abs(today.Low-today.LastClose)));


	int total_volume=0;
	int startopi=0;
	int i=0;
	double highest=today.Open;
	double lowest=today.Open;

	 for (auto &mindata:min_list)	 {

		 total_volume+=mindata.Volume;
		 if (i==0)		 {
			 startopi=mindata.OpenInterest;
		 }
		 i++;

		 if(mindata.price>highest) {
			 highest=mindata.price;
	//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest) {
			 lowest=mindata.price;
	//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }

		 double today_tr;
		 double tr_highlow=abs(highest-lowest);
//		 double tr_highclose=abs(highest-preday.Close);
//		 double tr_lowclose=abs(lowest-preday.Close);

		 double tr_highclose=abs(highest-today.LastClose);
		 double tr_lowclose=abs(lowest-today.LastClose);

		 today_tr=max(tr_highlow,max(tr_highclose,tr_lowclose));

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		double rate;
		if (mindata.OpenInterest>0){
			rate=(double)total_volume/mindata.OpenInterest;
		}
		else{
			rate=0;
		}

		 if(opentime_mode &&
				 !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0)
				 && !this->GetOpenFlag() && !this->GetCloseFlag())	 {



			double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;
			bool condition_long =   mindata.price > cycledata.High20   && preday.Close < precycledata.High20 ;
			bool condition_short = mindata.price <  cycledata.Low20  && preday.Close > precycledata.Low20 ;

//			bool condition_long =   mindata.price > cycledata.Highest   && preday.Close < precycledata.Highest ;
//			bool condition_short = mindata.price <  cycledata.Lowest  && preday.Close > precycledata.Lowest ;

			bool condition_atr = today_tr>preday_atr10_data.ATR && preday_atr10_data.ATR>preday_atr20_data.ATR ;

			bool condition_vot_long = mindata.price > today.Open ;
			bool condition_vot_short= mindata.price < today.Open ;

			this->SetStopRange( preday_atr20_data.ATR *this->GetStopRatio());

			 if (condition_long && condition_vot_long  && condition_atr  && sed>2)
				{

				 printf ("多头信号: cycledata  %s  n:  %d   date[%s]   tr[%.3f] atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
				 				tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,today_tr,preday_atr10_data.ATR,
								preday_atr20_data.ATR,preday_atr20_data.ATR/today.Open*100,preday_atr10_data.ATR/today.Open*100 ,k);
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_n_open");

				}

			 if (condition_short && condition_vot_short  && condition_atr  && sed>2)
				{

				 printf ("空头信号: cycledata  %s  n:  %d   date[%s]  tr[%.3f]  atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
				 				tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,today_tr,preday_atr10_data.ATR,
								preday_atr20_data.ATR,preday_atr20_data.ATR/today.Open*100,preday_atr10_data.ATR/today.Open*100 ,k);
				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_n_open");


				}
		 }
		 else if (mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())
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
		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}
// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_MA_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicCycle_Open_ begin \n");

	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	InitialSingleTest();
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",this->GetTable_HisMindata().c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}
	this->SetN(20);

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	double pre_ma50_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,0);
	double pre_ma20_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,0);
	double pre_ma10_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,0);

	bool pre_ma_long =preday.Close> pre_ma10_offset && pre_ma10_offset>pre_ma20_offset && pre_ma20_offset>pre_ma50_offset;
	bool pre_ma_short =preday.Close< pre_ma10_offset && pre_ma10_offset<pre_ma20_offset && pre_ma20_offset<pre_ma50_offset;

	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

	int i=0;

	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 i++;
		 this->update_HighLow(&mindata);

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool exist_order = mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),0);
		 bool valid_opentime =this->condition_OpenTime(&mindata,0) ;

		 if(!valid_opentime){
			 continue;
		 }

		 if( !exist_order && !this->GetOpenFlag() && !this->GetCloseFlag())		 {
			 double tr=this->zb_TR(preday.Close);

			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),this->GetN(),tr);

			 double sed=this->zb_Sedimentary(&mindata);

			 this->SetStopRange(atr20 *this->GetStopRatio());

			 double ma50,ma20,ma10;

			ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
			ma20 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,mindata.price);
			ma10 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,mindata.price);
			double ma_x50 = atan((ma50/pre_ma50_offset-1)*100)*180/M_PI;

			bool condition_long_ma50_20 =ma50< ma20 && ma20< ma10  && ma10< mindata.price  && mindata.price > today.Open;
			bool condition_short_ma50_20 = ma50> ma20 && ma20>ma10 && ma10 > mindata.price  && mindata.price < today.Open;

//			bool condition_long_atr=atr10_data.ATR>atr20_data.ATR && tr >atr10_data.ATR &&  today.Close > today.Open;
//			bool condition_short_atr=atr10_data.ATR>atr20_data.ATR && tr >atr10_data.ATR &&  today.Close < today.Open;

			/* MA 开仓法*/
			// 开多总条件
			bool condition_long_trend = condition_long_ma50_20  && !pre_ma_long ;


			//开空总条件
			bool condition_short_trend = condition_short_ma50_20 && !pre_ma_short;


//			 double vot =0.025;
//			 double k_ref =0.95;
//
//			 double dif =0.005;
//			 double v20=atr20_data.ATR/ mindata.price;
//			 double v10 =atr10_data.ATR/ mindata.price;


			bool condition_long = condition_long_trend  && sed>this->GetLimitSedimentary()  ;
			bool condition_short =  condition_short_trend &&  sed>this->GetLimitSedimentary() ;

			 if (condition_long  )	{
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR = 0;
				 data.DayOpenPrice = today.Open;
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"ma_open",&data);


			}
			 else if (condition_short )		{
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR = 0;
				 data.DayOpenPrice = today.Open;
				 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"ma_open",&data);
				}
		 }

		 else if (this->GetOpenTimeMode() ==0 && exist_order && this->GetOpenFlag() && !this->GetCloseFlag()) {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

			 this->UpdateStopPrice(&tr,&mindata);

			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)	 {

				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {

				 Close(tr,mindata,"stop_openday");

			 }


		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}
// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_MA4918_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicCycle_Open_ begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

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
		CheckErrorData(productid);

	}

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	int n_long =20;
	int n_short =10;
	ATRData atr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long);
	ATRData atr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short);

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
//	CycleData cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);
//
//
//
//	printf ("cycledata  %s  n:  %d   date[%s]   atr20[%.3f]--- atr10[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
//			tablename.c_str(),n,this->GetCusor_DaySqlTime().c_str() ,atr20_data.ATR,
//			atr10_data.ATR,atr20_data.ATR/today.Open*100,atr10_data.ATR/today.Open*100 ,k);

	double pre_ma18_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),18);
	double pre_ma9_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9);
	double pre_ma4_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4);

	bool pre_ma_long =preday.Close> pre_ma4_offset && pre_ma4_offset>pre_ma9_offset && pre_ma9_offset>pre_ma18_offset;
	bool pre_ma_short =preday.Close< pre_ma4_offset && pre_ma4_offset<pre_ma9_offset && pre_ma9_offset<pre_ma18_offset;

	this->SetStopRange(atr20_data.ATR *this->GetStopRatio());

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		 if(opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {


			 double ma18,ma9,ma4;

			ma18 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),18,mindata.price);
			ma9 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9,mindata.price);
			ma4 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4,mindata.price);


			bool condition_long_ma50_20 = ma18< ma9 && ma9< ma4  && ma4< mindata.price  && mindata.price > today.Open;
			bool condition_short_ma50_20 = ma18> ma9 && ma9>ma4 && ma4 > mindata.price  && mindata.price < today.Open;


//						if (condition_long_ma50_20)
//						{
//							 printf("long --->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}
//						else if(condition_short_ma50_20)
//						{
//							 printf("short--->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}


			/* MA 开仓法*/
			// 开多总条件
			bool condition_long_trend = condition_long_ma50_20 &&!pre_ma_long ;


			//开空总条件
			bool condition_short_trend = condition_short_ma50_20 &&!pre_ma_short;


			 double vot =0.03;
			 double k_ref =0.95;

			 double dif =0.005;
			 double v20=atr20_data.ATR/ mindata.price;
			 double v10 =atr10_data.ATR/ mindata.price;

			 double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;



			bool condition_long =  condition_long_trend  && sed>2 ;
			bool condition_short = condition_short_trend && sed >2 ;

			 if (condition_long  )
				{
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"ma4918_open");
				}

			 if (condition_short )
				{
				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"ma4918_open");
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


		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}

// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_MA4950_Open_()
{
	printf ("OneDay_MinuteLine_MA4950_Open_ begin \n");
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlHisMinData mysqlhismindata;
	vector<HxDayLineData>  N20_list;
	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	InitialSingleTest();
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",this->GetTable_HisMindata().c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}


	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	double pre_ma50_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50);
	double pre_ma9_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9);
	double pre_ma4_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4);

	bool pre_ma_long =preday.Close> pre_ma4_offset && pre_ma4_offset>pre_ma9_offset && pre_ma9_offset>pre_ma50_offset;
	bool pre_ma_short =preday.Close< pre_ma4_offset && pre_ma4_offset<pre_ma9_offset && pre_ma9_offset<pre_ma50_offset;

	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);
	int i=0;
	this->SetN(20);
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 i++;

		 this->update_HighLow(&mindata);
		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool exist_order = mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
						 this->GetMainInstrumentID().c_str(),0);
		 bool valid_opentime =this->condition_OpenTime(&mindata,0);

		 if( valid_opentime &&!exist_order && !this->GetOpenFlag() && !this->GetCloseFlag())	 {


			 double tr=this->zb_TR(preday.Close);

			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),
					 this->GetCusor_DaySqlTime().c_str(),this->GetN(),tr);

			 this->SetStopRange(atr20 *this->GetStopRatio());

			 double ma50,ma9,ma4;

			ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
			ma9 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9,mindata.price);
			ma4 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4,mindata.price);


			bool condition_long_ma50_20 = ma50< ma9 && ma9< ma4  && ma4< mindata.price  && mindata.price > today.Open;
			bool condition_short_ma50_20 = ma50> ma9 && ma9>ma4 && ma4 > mindata.price  && mindata.price < today.Open;


//						if (condition_long_ma50_20)
//						{
//							 printf("long --->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}
//						else if(condition_short_ma50_20)
//						{
//							 printf("short--->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}


			/* MA 开仓法*/
			// 开多总条件
			bool condition_long_trend = condition_long_ma50_20 &&!pre_ma_long ;
			//开空总条件
			bool condition_short_trend = condition_short_ma50_20 &&!pre_ma_short;


			 double sed=this->zb_Sedimentary(&mindata);

			bool condition_long =  condition_long_trend  && sed>2 ;
			bool condition_short = condition_short_trend && sed >2 ;

			 if (condition_long  )				{
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR = 0;
				 data.DayOpenPrice = today.Open;
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"ma4950_open");
			}

			 if (condition_short )			{
				 TestRecordData data;
				 data.Open_LastOpenRate = (mindata.price-today.Open) / today.Open;
				 data.Open_AddOPIRate = 0;
				 data.Open_VolOpiRate= 0;
				 data.pATR = 0;
				 data.DayOpenPrice = today.Open;
				 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"ma4950_open");
			}
		 }

		 else if (this->GetOpenTimeMode() ==0 && exist_order && this->GetOpenFlag() && !this->GetCloseFlag())	 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);

			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)		 {
				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {
				 Close(tr,mindata,"stop_openday");
			 }
		 }
	 }

	vector<MinuteLineData>().swap(min_list);
	vector<HxDayLineData>().swap(N20_list);
}
// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_MA102050_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicCycle_Open_ begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;

	DateUtil dateutil;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

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
		CheckErrorData(productid);

	}

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	int n_long =20;
	int n_short =10;


	double pre_ma50_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50);
	double pre_ma20_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
	double pre_ma10_offset =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10);

	bool pre_ma_long =preday.Close> pre_ma10_offset && pre_ma10_offset>pre_ma20_offset && pre_ma20_offset>pre_ma50_offset;
	bool pre_ma_short =preday.Close< pre_ma10_offset && pre_ma10_offset<pre_ma20_offset && pre_ma20_offset<pre_ma50_offset;

	double highest=today.Open;
	double lowest=today.Open;

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 if(mindata.price>highest)
		 {
			 highest=mindata.price;
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
		 }

		 if(mindata.price<lowest)
		 {
			 lowest=mindata.price;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
		 }

		 double today_tr;
		 double tr_highlow=abs(highest-lowest);
		 double tr_highclose=abs(highest-preday.Close);
		 double tr_lowclose=abs(lowest-preday.Close);

		 today_tr=max(tr_highlow,max(tr_highclose,tr_lowclose));

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		 if(opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {


			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long,today_tr);
			 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short,today_tr);

			 this->SetStopRange(atr20 *this->GetStopRatio());

			 double ma50,ma20,ma10;

			ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
			ma20 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,mindata.price);
			ma10 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,mindata.price);


			bool condition_long_ma50_20 = ma50< ma20 && ma20< ma10  && ma10< mindata.price  && mindata.price > today.Open;
			bool condition_short_ma50_20 = ma50> ma20 && ma20>ma10 && ma10 > mindata.price  && mindata.price < today.Open;


//						if (condition_long_ma50_20)
//						{
//							 printf("long --->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}
//						else if(condition_short_ma50_20)
//						{
//							 printf("short--->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}


			/* MA 开仓法*/
			// 开多总条件
			bool condition_long_trend = condition_long_ma50_20 &&!pre_ma_long ;

			//开空总条件
			bool condition_short_trend = condition_short_ma50_20 &&!pre_ma_short;

			bool condition_atr = today_tr>atr10 && atr10>atr20 ;

			 double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;



			bool condition_long =  condition_long_trend    && sed>2 ;
			bool condition_short = condition_short_trend   && sed >2 ;

			 if (condition_long  )
				{
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"ma102050_open");
				}

			 if (condition_short )
				{
				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"ma102050_open");
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


		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}

// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void TrendTradeTest::OneDay_MinuteLine_DynamicMACycle_Open_()
{
//	printf ("OneDay_MinuteLine_DynamicMACycle_Open_ begin \n");
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;
	MysqlMaincontract mysqlmaincontract;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	this->SetTable_HisMindata(this->GetMainInstrumentID().c_str(),date);

//	printf ("check tablename : %s \n",tablename.c_str());
	if (mysqlhismindata.Exist_Table(this->GetTable_HisMindata().c_str()))	{
		mysqlhismindata.Find_AllList(min_list,this->GetTable_HisMindata().c_str());
	}
	else	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",this->GetTable_HisMindata().c_str());
		CheckErrorData(productid);
	}

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	int n_long =20;
	int n_short =10;
//	ATRData atr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long);
//	ATRData atr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short);
//
	int n ;
	double k ;
//	if(atr10_data.ATR >1.0 && atr20_data.ATR >1.0)
//	{
//		k = atr20_data.ATR/atr10_data.ATR;
//	}
//	else
//	{
//		k=1.0;
//	}
//
//	n=floor(n_long*k);
//	CycleData cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	ATRData preatr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_long);
	ATRData preatr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),preday.Time,n_short);
	int pre_n ;
	double pre_k ;
	if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0)	{
		pre_k = preatr20_data.ATR/preatr10_data.ATR;
	}
	else	{
		pre_k=1.0;
	}
	pre_n=floor(n_long*pre_k);
	CycleData precycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,pre_n);

//if( preday.Close > precycledata.Low20)
//{
//
//}
//else if (preday.Close < precycledata.High20)
//{
//	printf ("cycledata  %s  n:  %d   date[%s]   atr20[%.3f]--- atr10[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
//				this->GetTable_HisMindata().c_str(),n,this->GetCusor_DaySqlTime().c_str() ,atr20_data.ATR,
//				atr10_data.ATR,atr20_data.ATR/today.Open*100,atr10_data.ATR/today.Open*100 ,k);
//}
//double tr ;
//tr=max(abs(today.High-today.Low),max(abs(today.High-today.LastClose),abs(today.Low-today.LastClose)));


//double pre_ma50_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,n);

//	this->SetStopRange(tr *this->GetStopRatio());



	this->SetTodayHigh(today.Open);
	this->SetTodayLow(today.Open);

int i=0;
int startopi;

	 for (auto &mindata:min_list)	 {

		 if (i==0)		 {
			 startopi=mindata.OpenInterest;
		 }
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",this->GetTable_HisMindata().c_str(),datetime,mindata.MinuteTime  );
		this->update_HighLow(&mindata);

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)	{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		 if(opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())		 {



			 double tr=this->zb_TR(preday.Close);
	//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
	//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long,tr);
			 double atr10=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short,tr);
			 double atr5=mysqldayline.Get_ATRDatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5,tr);



			 if(atr5 >1.0 && atr20 >1.0)			{
				k = atr20/atr5;
			}
			else		{
				k=1.0;
			}
			n=floor(n_long*k);
			CycleData cycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);




//			 double Ref_vot =0.02;
//			 double k_ref =0.95;
//
//			double dif =0.005;
//			 double v20=atr20_data.ATR/ mindata.price;
//			 double v10 =atr10_data.ATR/ mindata.price;

//			 double k=atr20_data.ATR/atr10_data.ATR;
			 double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

			 double ma50;
			 ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);

			 double ma20 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,mindata.price);

			 double ma10 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,mindata.price);

//			 double stoprange=abs(mindata.price-ma50);
			 this->SetStopRange(atr20 *this->GetStopRatio());

//			 double ma_x50 = atan((ma50/pre_ma50_offset-1)*100)*180/M_PI;

			bool condition_long =   mindata.price > cycledata.High20   && preday.Close < precycledata.High20 ;
			bool condition_short = mindata.price <  cycledata.Low20  && preday.Close > precycledata.Low20 ;

//			bool condition_angle_long  =  ma_x50>0  && ma_x50!=90;
//			bool condition_angle_short = ma_x50<0  && ma_x50!=90 ;

			bool condition_atr = tr>atr5 && atr5>atr20 ;

			double opiadd_rate =(mindata.OpenInterest - startopi ) /startopi;
			bool condition_opi = opiadd_rate >0.05;

			bool condition_vot_long = mindata.price > today.Open  && today.Open  > preday.Close;
			bool condition_vot_short= mindata.price < today.Open && today.Open  > preday.Close ;

			bool condition_ma_long =mindata.price >ma50 && mindata.price >ma10  && ma10 > ma20 && ma20>ma50 ;
			bool condition_ma_short =mindata.price <ma50 && mindata.price <ma10  && ma10 < ma20 && ma20<ma50;

			 if (condition_long && condition_vot_long   && condition_ma_long && sed>2)		{

				 printf ("	多头信号: cycledata  %s  n:  %d   date[%s]  增仓率 %.2f%% \n	tr[%.3f] atr5[%.3f] atr10[%.3f] atr20[%.3f] 20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
				 				this->GetTable_HisMindata().c_str(),n,this->GetCusor_DaySqlTime().c_str() ,opiadd_rate*100,tr,atr5,atr10,
				 				atr20,atr20/today.Open*100,atr10/today.Open*100 ,k);
				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_n_open");

				}

			 if (condition_short && condition_vot_short  && condition_ma_short && sed>2)		{

				 printf ("	空头信号: cycledata  %s  n:  %d   date[%s]  增仓率 %.2f%%  \n	tr[%.3f]  atr5[%.3f] atr10[%.3f]--- atr20[%.3f]---20日波动率---> %.2f%%  -10日波动率-----> %.2f%%     k:  %.3f  \n",
				 				this->GetTable_HisMindata().c_str(),n,this->GetCusor_DaySqlTime().c_str() ,opiadd_rate*100,tr,atr5,atr10,
				 				atr20,atr20/today.Open*100,atr10/today.Open*100 ,k);
				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_n_open");
				}
		 }

		 else if (this->GetOpenTimeMode() ==0 && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),0) && this->GetOpenFlag() && !this->GetCloseFlag())	 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 this->UpdateStopPrice(&tr,&mindata);

			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)		 {

				 Close(tr,mindata,"stop_openday");
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)	 {

				 Close(tr,mindata,"stop_openday");
			 }
		 }
	 }

	vector<MinuteLineData>().swap(min_list);
}
void TrendTradeTest::OneDay_MinuteLine_Dynamic_Hold(const char* pinstrumentid)
{
//	printf ("OneDay_Dynamic_Hold_MinuteLine begin  当前日期: %s \n", this->GetCusor_DaySqlTime().c_str());
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;
	vector<MinuteLineData>  min_list;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());
	MysqlHisMinData mysqlhismindata;
	this->SetDataBase_HisMindata(this->GetProductID().c_str());
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());


	CycleData precycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,10);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());


	DateUtil dateutil;
	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);

	if(mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )		{

			this->SetTodayHigh(today.Open);
			this->SetTodayLow(today.Open);

			int i=0;
			int startopi;
			int total_volume=0;

			 for (auto &mindata:min_list)		 {

				 total_volume+=mindata.Volume;
				 if (i==0)		 {
					 startopi=mindata.OpenInterest;
				 }
				 i++;

				 double rate;
				if (mindata.OpenInterest>0){
					rate=(double)total_volume/mindata.OpenInterest;
				}
				else{
					rate=0;
				}


			 this->update_HighLow(&mindata);

			 double tr=this->zb_TR(preday.Close);

	//printf("   tr 原始数据   highest: %.2f    lowest: %.2f     preday.Close: %.2f \n",highest,lowest,preday.Close);
	//printf("   tr 计算数据   tr_highlow: %.2f    tr_highclose: %.2f     tr_lowclose: %.2f---> tr  %.2f \n",tr_highlow,tr_highclose,tr_lowclose,tr);

			 double atr20=mysqldayline.Get_ATRDatabyMinPrice(pinstrumentid,this->GetCusor_DaySqlTime().c_str(),20,tr);

			 double stoprange=atr20 * this->GetStopRatio();
			int p= (this->GetInitialMoney()*this->GetRiskRate())/(stoprange* maincontractdata.VolumeMultiple);
			int position = p ;

//				 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);
				 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

				 MinuteLineData last_mindata;

				 bool closetime_mode;
				if(this->GetCloseTimeMode()==1)	{
					closetime_mode=t_current>t_endtime;
				}
				else{
					closetime_mode=true;
				}

//				double ma50;
//				ma50 =mysqldayline.Get_MADatabyMinPrice(pinstrumentid,this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
//				ma50 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
//				 printf("	%s %s MinuteLine_Hold ma  -------%.3f  \n",pinstrumentid,mindata.MinuteTime,ma50 );

				if (mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0))		{
					 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);


					 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)		 {
						 tr.Highest=mindata.price;
						 tr.StopPrice = tr.Highest -stoprange;
						 tr.Profit = (double)maincontractdata.VolumeMultiple  *(mindata.price- tr.OpenPrice) *tr.Volume-tr.Commission;

						 printf("	%s 持仓中   更新最高价   -------  %.3f   止损价  %.3f  profit:  %.2f \n",
								 mindata.MinuteTime,tr.Highest,tr.StopPrice,tr.Profit );
//						 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
//								 pinstrumentid,mindata.price,tr.StopPrice,0);
						 mysqltestrecord.UpdateProfit(this->GetTableRecord().c_str(),tr);

					 }
					 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
						 tr.Lowest=mindata.price;
						 tr.StopPrice = tr.Lowest + stoprange;
						 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - mindata.price ) *tr.Volume-tr.Commission;
						 printf("	%s 持仓中   更新最低价   -------%.3f    止损价  %.3f  profit:  %.2f\n",
								 mindata.MinuteTime,tr.Lowest,tr.StopPrice,tr.Profit );

//						 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
						 mysqltestrecord.UpdateProfit(this->GetTableRecord().c_str(),tr);
					}
				}

				 if(closetime_mode && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) ) {

					 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);

//					 if(tr.Volume -tr.CloseVolume > position)
//					 {
//						 printf("	MinuteLine_Hold 现持仓数[%d]  需要调仓至目标仓位[%d]  -------%s  \n",tr.Volume -tr.CloseVolume,position,pinstrumentid);
//					 }

					 double profit;

					 if (tr.Direction ==0)		 {
						 profit = (double)maincontractdata.VolumeMultiple  *(mindata.price- tr.OpenPrice) *tr.Volume-tr.Commission;
					 }
					 else if (tr.Direction ==1)	 {
						 profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - mindata.price ) *tr.Volume-tr.Commission;
					 }
//					 printf(" hold----->%s   maxprofit: %.2f   profit: %.2f    \n ",mindata.MinuteTime,tr.Profit,profit);
//
//					int n_long =20;
//					int n_short =10;
//					ATRData atr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_long);
//					ATRData atr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n_short);
//
//					int n ;
//					double k ;
//					if(atr10_data.ATR >1.0 && atr20_data.ATR >1.0)
//					{
//						k = atr20_data.ATR/atr10_data.ATR;
//					}
//					else
//					{
//						k=1.0;
//					}

//					 if (tr.Direction ==0 && t_current>t_endtime &&( rate<0.5 || rate>2))	 {
//						 printf("	多头合约 持仓日尾盘平仓信号 [stop_hold_endvolopi]----------------->totalvolume:%d  staropi:%d  endopi:%d  rate : %.2f \n",
//								 total_volume,startopi,mindata.OpenInterest,rate);
//						 Close(tr,mindata,"stop_hold_endvolopi");
//					 }
//					 else if(tr.Direction ==1 && t_current>t_endtime ){
//
//					 }

					 if (tr.Direction ==0  )	 {

						 double dif= mindata.price -last_mindata.price ;

						 if (mindata.price < tr.StopPrice )	 {
							 Close(tr,mindata,"stop_hold");
						  }

//						 if (mindata.price < precycledata.Lowest )	 {
//							 Close(tr,mindata,"stop_n10");
//						  }

//
//						 if(tr.Profit >5000 && profit< tr.Profit/2.0 && tr.HalfStop==false )
//						 {
////							 Close(tr,mindata,"stop_profit");
//							 int v=floor((tr.Volume -tr.CloseVolume)/2);
//							 HalfCloseAdjust(tr,mindata,"stop_halfadjust",v);
//						 }
//						 if (mindata.price < tr.StopPrice + stoprange/2 && tr.HalfStop==false)
//						 {
//							 int v=floor((tr.Volume -tr.CloseVolume)/2);
//							 HalfCloseAdjust(tr,mindata,"stop_halfadjust",v);
//						  }

//						 if (k>=1 )
//						 {
//							 Close(tr,mindata,"stop_k");
//						  }
//						 if (mindata.price > tr.WinPrice )
//						 {
//							 Close(tr,mindata,"stop_hold_win");
//						  }
//						 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 )	 {
//							 Close(tr,mindata,"stop_opi_less");
//						 }
//						 else if (mindata.price < ma50 && t_current>t_endtime)
//						 {
//							 printf("	[%s] MinuteLine_Hold 现持仓数[%d] ----- 当前时间 : %s 现价: %.3f ma50： %.3f    \n",
//									 pinstrumentid,tr.Volume -tr.CloseVolume,mindata.MinuteTime,mindata.price ,ma50);
//
//							 Close(tr,mindata,"stop_ma50");
//
//						 }
//						 else if(tr.Volume -tr.CloseVolume > position  && t_current>t_endtime)
//						 {
//							 printf("	MinuteLine_Hold 现持仓数[%d]  需要调仓至目标仓位[%d]  -------%s  \n",tr.Volume -tr.CloseVolume,position,pinstrumentid);
//
//							 CloseAdjust(tr,mindata,"stop_adjust",tr.Volume-position);
//
//						 }
					 }
					 else if (tr.Direction ==1  )	 {

						 double dif=  last_mindata.price - mindata.price ;

						 if (mindata.price > tr.StopPrice)	 {

							 Close(tr,mindata,"stop_hold");

						 }

//						 if (mindata.price > precycledata.Highest )	 {
//								 Close(tr,mindata,"stop_n10");
//							  }

//						 if(tr.Profit >5000 && profit< tr.Profit/2.0 && tr.HalfStop==false)
//						 {
////							 Close(tr,mindata,"stop_profit");
//							 int v=floor((tr.Volume -tr.CloseVolume)/2);
//							 HalfCloseAdjust(tr,mindata,"stop_halfadjust",v);
//
//						 }
//						 if (mindata.price > tr.StopPrice -stoprange/2 && tr.HalfStop==false)
//						 {
//							 int v=floor((tr.Volume -tr.CloseVolume)/2);
//							 HalfCloseAdjust(tr,mindata,"stop_halfadjust",v);
//						  }
//						 if (k>=1 )
//						 {
//							 Close(tr,mindata,"stop_k");
//						  }
//						 if (mindata.price < tr.WinPrice )
//						 {
//							 Close(tr,mindata,"stop_hold_win");
//						  }
//						 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2)	 {
//							 Close(tr,mindata,"stop_opi_less");
//
//						 }
//						 else if (mindata.price > ma50&& t_current>t_endtime)
//						 {
//
//							 Close(tr,mindata,"stop_ma50");
//
//						 }
//						 else if(tr.Volume -tr.CloseVolume > position && t_current>t_endtime)
//						 {
//							 CloseAdjust(tr,mindata,"stop_adjust",tr.Volume-position);
//						 }

					 }
				 }//exist 存在持仓单
			 }// for
		}
		else{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(this->GetProductID().c_str());
		}
	}
	else{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}
//	printf ("check tablename : %s \n",tablename.c_str());

	vector<MinuteLineData>().swap(min_list);
}
int TrendTradeTest::LineType(vector<HxDayLineData> list)
{
//	int ret;

	if (list.size()==0) return 0;
	double total,average;
	for (auto &day:list){

		total+=day.Close;

//		if(day.Close>day.Open){
//			total+=day.Close;
//		}
//		else{
//			total+=day.Open;
//		}

	}
	average = total/(double)list.size();

double stddev,fc;
	for(auto &day:list){
		fc+=(day.Close-average)*(day.Close-average);
	}
	fc=fc/(double)list.size();
	stddev=sqrt(fc);

	return stddev;

}
void TrendTradeTest::BigOneMinVolume_Open_MinuteLine()
{

//	printf ("BigOneMinVolume_Open_MinuteLine begin %s \n",this->GetMainInstrumentID().c_str());
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;
	vector<HxDayLineData>  days_list;
//	vector<FiveDayLineData> fivedaydata_list;
	//当天未开仓  false
	this->SetOpenFlag(false);
	//当天未平仓 false
	this->SetCloseFlag(false);
	std::map<std::string ,std::shared_ptr<HighLowAnalyst>> m_hlamap;
	vector<HighLowAnalyst>  hlalist;

	m_mysqldayline->Find_ListsbyDate(days_list,this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);


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

	double stddev = this->LineType(days_list);
	printf(">>>>>>>>>>>%s             标准差:%.2f\n",this->GetMainInstrumentID().c_str(),stddev);

	bool maxvol_dis=false;
	 for (auto &mindata:min_list)	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),
//				 this->GetCusor_DaySqlTime().c_str(),mindata.MinuteTime  );

		 double ema60=this->m_mysqldayline->Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
					this->GetCusor_DaySqlTime().c_str(),60,mindata.price);
		 double ema5=this->m_mysqldayline->Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
		 					this->GetCusor_DaySqlTime().c_str(),5,mindata.price);

		 double ma20 =this->m_mysqldayline->Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),
				 this->GetCusor_DaySqlTime().c_str(),20,mindata.price);

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
//				 printf("--------插入第一个数据:  %s    %d    %.2f  -->开盘价:%.2f   rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
//				 						 data.HighLowPrice,today.Open,data.HLOpenRate*100);
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
//				 printf("--------插入第一个数据:  %s    %d    %.2f   -->开盘价:%.2f     rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
//						 data.HighLowPrice,today.Open,data.HLOpenRate*100);
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
//						 printf("--------空头转多头---->插入新的方向数据:  %s    %d    %.2f    -->开盘价:%.2f     rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
//								 data.HighLowPrice,today.Open,data.HLOpenRate*100);
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
//						 printf("--------多头转空头---->插入新的方向数据:  %s    %d    %.2f      -->开盘价:%.2f     rate:%.2f%%\n",data.MinuteTime.c_str(),data.Direction,
//								 data.HighLowPrice,today.Open,data.HLOpenRate*100);
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

			bool trend_long = mindata.price>ema5 && ema5> ema60  && this->GetTodayLow()>ma20 ;
			bool trend_short = mindata.price<ema5 && ema5< ema60 && this->GetTodayHigh()<ma20;



			//标准版
			 bool condition_long = trend_long    &&mindata.price > cycledata.High20    && condition_vol && !fridaynight && con_uplimit ;
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
				 data.pATR =stddev;
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
				 data.pATR =stddev;
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


//			 if (tr.Direction ==0 && mindata.price > tr.WinPrice )	 {
//				 bvClose(tr,mindata,"stop_win",mindata.price);
//			  }
//			 else if (tr.Direction ==1 && mindata.price < tr.WinPrice )		 {
//				 bvClose(tr,mindata,"stop_win",mindata.price);
//			  }
		 }

	 }

	vector<MinuteLineData>().swap(min_list);
	vector<HxDayLineData>().swap(days_list)  ;
//	 vector<FiveDayLineData>().swap(fivedaydata_list);
	 vector<HighLowAnalyst>().swap(hlalist);
}

void TrendTradeTest::BigVolume_Hold_MinuteLine(const char* pinstrumentid)
{
//	printf ("BigVolume_Hold_MinuteLine begin \n");
	vector<MinuteLineData>  min_list;
	HxDayLineData today=this->m_mysqldayline->Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
	MainContractData maincontractdata = this->m_pmysqlmaincontract->Find_DatabyProductID(this->GetProductID().c_str());
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



	if(!this->m_pmysqlhismindata->Exist_Table(tablename.c_str()))	{

		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
				CheckErrorData(this->GetProductID().c_str());
				return;
	}
	this->m_pmysqlhismindata->Find_AllList(min_list,tablename.c_str());

	if(min_list.size()<=0 )	{
		printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
		return;
	}

	 for (auto &mindata:min_list)		 {


		 bool existorder=this->m_pmysqltestrecord->Exist_RecordByInstrumentStatus(
				 this->GetTableRecord().c_str(),pinstrumentid,0);

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);


		 double ema60=this->m_mysqldayline->Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
					this->GetCusor_DaySqlTime().c_str(),60,mindata.price);
		 double ema5=this->m_mysqldayline->Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),
							this->GetCusor_DaySqlTime().c_str(),5,mindata.price);

		 bool closetime_mode;
		if(this->GetCloseTimeMode()==1)		{
			closetime_mode=t_current>t_endtime;
		}
		else{
			closetime_mode=true;
		}

		 if(closetime_mode && existorder )	 {

	//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

			 TestRecordData tr =this->m_pmysqltestrecord->Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);

			 if (tr.Direction ==0   ) {

				 if (mindata.price < tr.StopPrice )	 {
					 bvClose(tr,mindata,"stop_hold",mindata.price);
				  }
				 else if (mindata.price < ema5 && ema5<ema60)	 {
					 bvClose(tr,mindata,"stop_ma50",mindata.price);
				 }

			 }
			 else if (tr.Direction ==1    )	 {

				 if (mindata.price > tr.StopPrice)	{
					 bvClose(tr,mindata,"stop_hold",mindata.price);
				 }
				 else if (mindata.price > ema5 && ema5>ema60)	 {
					 bvClose(tr,mindata,"stop_ema50",mindata.price);
				 }
			 }

		 }//exist 存在持仓单


//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

	 }// for

	 bool existorder_close=this->m_pmysqltestrecord->Exist_RecordByInstrumentStatus(
					 this->GetTableRecord().c_str(),pinstrumentid,0);

	 if( existorder_close) {
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
void TrendTradeTest::MA_Open_MinuteLine()
{
//	printf ("MA_Open_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlHisATR mysqlhisatr;
	MysqlHisMinData mysqlhismindata;
	MysqlAvePrice mysqlaveprice;
	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	this->SetOpenFlag(false);
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

	double pre_ma50_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,0);

//	int pre_direction =this->Strategy_MA10_20_50_Pre();

	ATRData atr_data =mysqlhisatr.Find_DatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	this->SetStopRange(atr_data.ATR *this->GetStopRatio());

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}


		 if( opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
				 {
					 double ma50,ma9,ma4;
					double ma20,ma10;

					ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
					ma20 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,mindata.price);
					ma10 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,mindata.price);



					double ma_x50 = atan((ma50/pre_ma50_offset-1)*100)*180/M_PI;
					bool condition_long_ma_x = ma_x50>0  && ma_x50!=90 ;
					bool condition_short_ma_x = ma_x50<0   && ma_x50!=90 ;


//						if(condition_long_ma_x)
//						{
//							printf("x----long %f   \n",ma_x50);
//						}
//						else if(condition_short_ma_x)
//						{
//							printf("x----short %f   \n",ma_x50);
//						}


		//			ma9 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9,mindata.price);
		//			ma4 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4,mindata.price);
		//			bool condition_long_ma = ma50< ma9 && ma9<ma4  && ma4< mindata.price  && mindata.price > today.Open;
		//			bool condition_short_ma = ma50> ma9 && ma9>ma4 && ma4 > mindata.price  && mindata.price < today.Open ;


					bool condition_long_ma50_20 = ma50< ma20 && ma20< ma10  && ma10< mindata.price  && mindata.price > today.Open;
					bool condition_short_ma50_20 = ma50> ma20 && ma20>ma10 && ma10 > mindata.price  && mindata.price < today.Open;


//						if (condition_long_ma50_20)
//						{
//							 printf("long --->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}
//						else if(condition_short_ma50_20)
//						{
//							 printf("short--->ma50  %.3f     ma20 %.3f  ma10 %.3f   \n",ma50,ma20,ma10);
//						}


					/* EMA 开仓法*/
					// 开多总条件
					bool condition_long_trend = condition_long_ma50_20  && condition_long_ma_x;
					bool condition_long = condition_long_trend    ;

					//开空总条件
					bool condition_short_trend = condition_short_ma50_20 && condition_short_ma_x;
					bool condition_short = condition_short_trend ;

					 if (condition_long    )
						{
						 printf("多头 -->ma50  %.3f     ma9 %.3f  ma4 %.3f   \n",ma50,ma9,ma4);
						 BuyOpen(mindata, this->GetMainInstrumentID().c_str(),"ma_open");

						}

					 if (condition_short )
						{
						 printf("空头--->ma50  %.3f     ma9 %.3f  ma4 %.3f   \n",ma50,ma9,ma4);
						 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"ma_open");

						}
				 }







	 }

	vector<MinuteLineData>().swap(min_list);
}


void TrendTradeTest::MA_Hold_MinuteLine(const char* pinstrumentid)
{

//	printf ("MA_Hold_MinuteLine begin \n");

	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlHisMinData mysqlhismindata;

	vector<MinuteLineData>  min_list;

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());

	DateUtil dateutil;

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

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);

	if(mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )
		{

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

					 if(closetime_mode && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )
					 {

			//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

						 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
						 double ma50 =mysqldayline.Get_MADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);


						 if (tr.Direction ==0   )
						 {

							 if (mindata.price < tr.StopPrice )
							 {
								 Close(tr,mindata,"stop_hold");
							  }
							 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 )
							 {
								 Close(tr,mindata,"stop_opi_less");
							 }
							 else if (mindata.price < ma50)
							 {
								 Close(tr,mindata,"stop_ma50");

							 }

						 }
						 else if (tr.Direction ==1    )
						 {

							 if (mindata.price > tr.StopPrice)
							 {
								 Close(tr,mindata,"stop_hold");
							 }
							 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2)
							 {
								 Close(tr,mindata,"stop_opi_less");
							 }
							 else if (mindata.price > ma50)
							 {
								 Close(tr,mindata,"stop_ema50");
							 }

						 }

					 }//exist 存在持仓单


		//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

			 }// for
		}
		else
		{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(this->GetProductID().c_str());
		}


	}
	else
	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}
//	printf ("check tablename : %s \n",tablename.c_str());

	ATRData atr_data =mysqlhisatr.Find_DatabyDateTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
		this->SetStopRange(atr_data.ATR *this->GetStopRatio());


	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )
	 {
		 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
		 if (  tr.Direction==0 && today.Close> tr.Highest)
			 {
				 tr.Highest=today.Close;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s 持仓中   更新最高收盘价   -------  %.3f   止损价  %.3f \n", today.Time,tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 pinstrumentid,today.Close,tr.StopPrice,0);

			 }
			 else if ( tr.Direction==1 && today.Close < tr.Lowest )
			{
				 tr.Lowest=today.Close;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 持仓中   更新最低收盘价   -------%.3f    止损价  %.3f\n",today.Time,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,today.Close,tr.StopPrice,0);
			}

	 }


	vector<MinuteLineData>().swap(min_list);
}

void TrendTradeTest::EMA_Open_MinuteLine()
{
//	printf ("EMA_EndTime_Open_MinuteLine begin \n");
	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlHisATR mysqlhisatr;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;
	MysqlAvePrice mysqlaveprice;

	DateUtil dateutil;
	vector<MinuteLineData>  min_list;

	this->SetOpenFlag(false);
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


		return;
	}


	EMAData emadata;
	emadata=mysqlema.Find_EMADatabyTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	HisEMAData hisemadata;
	hisemadata = mysqlhisema.Find_PreHisEMADatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());


	bool pre_direction_long =false;
	bool pre_direction_short =false;
	if (hisemadata.EMA4 > hisemadata.EMA9 )
	{
		pre_direction_long=true;
		printf("pre_direction_long   ---> true  \n");
	}
	else if (hisemadata.EMA4 < hisemadata.EMA9 )
	{
		pre_direction_short =true;
		printf("pre_direction_short   ---> true  \n");
	}

	printf("pre---> ema9  %.3f   ema4   %.3f   \n ",hisemadata.EMA9,hisemadata.EMA4);

	bool pre_direction_long4950 =false;
	bool pre_direction_short4950 =false;
	if (hisemadata.EMA4 > hisemadata.EMA9 && hisemadata.EMA9 > hisemadata.EMA50)
	{
		pre_direction_long4950=true;
	}
	else if (hisemadata.EMA4 < hisemadata.EMA9 && hisemadata.EMA9 < hisemadata.EMA50)
	{
		pre_direction_short4950 =true;
	}


	bool pre_direction_long42050 =false;
		bool pre_direction_short42050 =false;
		if (hisemadata.EMA9 > hisemadata.EMA20 && hisemadata.EMA20 > hisemadata.EMA50)
		{
			pre_direction_long42050=true;
		}
		else if (hisemadata.EMA9 < hisemadata.EMA20 && hisemadata.EMA20 < hisemadata.EMA50)
		{
			pre_direction_short42050 =true;
		}

	HisEMAData hisemadata_x;
	hisemadata_x = mysqlhisema.Find_HisEMADatabyDateTime_Offset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),1);


	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	this->SetStopRange(atr_data.ATR *this->GetStopRatio());

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

		 EMAData emadata_current;

		 bool opentime_mode;
		if(this->GetOpenTimeMode()==1)
		{
			opentime_mode=t_current>t_endtime;
		}
		else{
			opentime_mode=true;
		}

		 if(opentime_mode && !mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),this->GetMainInstrumentID().c_str(),0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {
			double ema50,ema9,ema4;
			double ema20,ema10;

//			ema4 = (2*mindata.price + 3*emadata.EMA3)/5;

			ema50 =mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),50,mindata.price);
			ema9 =mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9,mindata.price);
			ema4 =mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4,mindata.price);

			ema20 =mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20,mindata.price);
			ema10 =mysqldayline.Get_EMADatabyMinPrice(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10,mindata.price);

			double ema_x50 = atan((ema50/hisemadata_x.EMA50-1)*100)*180/M_PI;


			emadata_current.EMA50 =ema50;
			emadata_current.EMA9 =ema9;
			emadata_current.EMA4 =ema4;
			emadata_current.EMA50_x =ema_x50;


//			printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f   下跌品种:%.2f   平均涨跌幅 : %.2f \n",hissection_table.c_str(),
//					mindata.MinuteTime,hissectiondata.CountUprate,hissectiondata.CountDownrate,hissectiondata.AverageUpDownrate);

			bool condition_long_ema = ema50< ema9 && ema9<ema4  && ema4< mindata.price  && mindata.price > today.Open;
			bool condition_long_ema50_20 = ema50< ema20 && ema20<ema10  && ema10< mindata.price  && mindata.price > today.Open;
			bool condition_long_ema_x = ema_x50>0  && ema_x50!=90 ;


			bool condition_short_ema = ema50> ema9 && ema9>ema4 && ema4 > mindata.price  && mindata.price < today.Open ;
			bool condition_short_ema50_20 = ema50> ema20 && ema20>ema10 && ema10 > mindata.price  && mindata.price < today.Open;
			bool condition_short_ema_x = ema_x50<0   && ema_x50!=90 ;

			/* EMA 开仓法*/
			// 开多总条件
			bool condition_long_trend = condition_long_ema50_20  && condition_long_ema_x;
			bool condition_long = condition_long_trend   ;

			//开空总条件
			bool condition_short_trend = condition_short_ema50_20 && condition_short_ema_x;
			bool condition_short = condition_short_trend  ;


			 if (condition_long  )
				{
//				 printf("ema50  %.3f     ema9 %.3f  ema4 %.3f   \n",emadata_current.EMA50,emadata_current.EMA9,emadata_current.EMA4);
				 printf("ema50  %.3f     ema20 %.3f  ema10 %.3f   \n",ema50,ema20,ema10);
				 BuyOpen(mindata, this->GetMainInstrumentID().c_str(),"ema_open");
				 this->UpdateOpenReference_EMA( this->GetMainInstrumentID().c_str(),mindata.MinuteTime,emadata_current);
				}

			 if (condition_short )
				{
//				 printf("ema50  %.3f     ema9 %.3f   ema4 %.3f  \n",emadata_current.EMA50,emadata_current.EMA9,emadata_current.EMA4);
				 printf("ema50  %.3f     ema20 %.3f  ema10 %.3f   \n",ema50,ema20,ema10);
				 SellOpen(mindata,this->GetMainInstrumentID().c_str(),"ema_open");
				 this->UpdateOpenReference_EMA( this->GetMainInstrumentID().c_str(),mindata.MinuteTime,emadata_current);
				}
		 }


	 }

	vector<MinuteLineData>().swap(min_list);
}

void TrendTradeTest::EMA_Hold_MinuteLine(const char* pinstrumentid)
{

//	printf ("EMA_Hold_MinuteLine begin \n");
	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;


	vector<MinuteLineData>  min_list;

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());


	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(this->GetProductID().c_str());

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);

	std::string tmp = this->GetCusor_DaySqlTime().c_str();
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());


	DateUtil dateutil;
	char endtime[20];
	sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
	time_t t_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);

	if(mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )
		{

			ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());
			EMAData emadata;
			emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

			 for (auto &mindata:min_list)
			 {
		//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);
				 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

				 MinuteLineData last_mindata;

				 bool closetime_mode;
				if(this->GetCloseTimeMode()==1)
				{
					closetime_mode=t_current>t_endtime;
				}
				else{
					closetime_mode=true;
				}

				 if(closetime_mode && mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )
				 {

		//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

					 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
					 double ema50;
					ema50 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,this->GetCusor_DaySqlTime().c_str(),50,mindata.price);


					 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)
					 {
						 tr.Highest=mindata.price;
						 tr.StopPrice = tr.Highest -this->GetStopRange();


						 printf("	%s 持仓中   更新最高价   -------  %.3f   止损价  %.3f \n", mindata.MinuteTime,tr.Highest,tr.StopPrice );
						 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
								 pinstrumentid,mindata.price,tr.StopPrice,0);

					 }
					 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)
					{
						 tr.Lowest=mindata.price;
						 tr.StopPrice = tr.Lowest + this->GetStopRange();
						 printf("	%s 持仓中   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

						 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
					}


					 if (tr.Direction ==0  )
					 {

						 double dif= mindata.price -last_mindata.price ;

						 if (mindata.price < tr.StopPrice )
						 {

							 Close(tr,mindata,"stop_hold");
						  }
						 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 )
						 {
							 Close(tr,mindata,"stop_opi_less");
						 }
						 else if (mindata.price < ema50)
						 {
							 Close(tr,mindata,"stop_ema50");

						 }



					 }
					 else if (tr.Direction ==1  )
					 {

						 double dif=  last_mindata.price - mindata.price ;

						 if (mindata.price > tr.StopPrice)
						 {

							 Close(tr,mindata,"stop_hold");

						 }
						 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2)
						 {
							 Close(tr,mindata,"stop_opi_less");

						 }
						 else if (mindata.price > ema50)
						 {

							 Close(tr,mindata,"stop_ema50");

						 }


					 }


				 }//exist 存在持仓单



			 }// for
		}
		else
		{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(this->GetProductID().c_str());
		}


	}
	else
	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(this->GetProductID().c_str());
	}
//	printf ("check tablename : %s \n",tablename.c_str());




	vector<MinuteLineData>().swap(min_list);
}
void TrendTradeTest::EMA_Hold_Emergence_MinuteLine(const char *pinstrumentid,
		const char *datetime,
		SqlAveragePriceData predata,
		HxDayLineData today)
{

//	printf ("MinuteLine_Hold begin \n");

	MysqlDayLine mysqldayline;
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;
	MysqlHisMinData mysqlhismindata;

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

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);

	if(mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )
		{

			ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);
			EMAData emadata;
			emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,datetime);


int i=0;
			 for (auto &mindata:min_list)
			 {
		//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);
				 MinuteLineData last_mindata;
				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )
				 {

		//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

					 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
					 double ema50,ema9,ema4;
					ema50 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,50,mindata.price);
					ema9 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,9,mindata.price);
					ema4 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,4,mindata.price);


					if (i==0)
					{
						last_mindata.price=today.Open;
					}
					else
					{
						last_mindata = mysqlhismindata.Find_PreDatabyMinuteTime(tablename.c_str(),mindata.MinuteTime);
					}



					 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)
					 {
						 tr.Highest=mindata.price;
						 tr.StopPrice = tr.Highest -this->GetStopRange();


						 printf("	%s 持仓中   更新最高价   -------  %.3f   止损价  %.3f \n", mindata.MinuteTime,tr.Highest,tr.StopPrice );
						 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
								 pinstrumentid,mindata.price,tr.StopPrice,0);

					 }
					 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)
					{
						 tr.Lowest=mindata.price;
						 tr.StopPrice = tr.Lowest + this->GetStopRange();
						 printf("	%s 持仓中   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

						 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
					}


					 if (tr.Direction ==0  )
					 {

						 double dif= mindata.price -last_mindata.price ;

						 if (mindata.price < tr.StopPrice )
						 {

							 Close(tr,mindata,"stop_hold");
						  }
						 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 )
						 {
							 Close(tr,mindata,"stop_opi_less");
						 }
						 else if (mindata.price < ema50)
						 {
							 Close(tr,mindata,"stop_ema50");

						 }
						 else if (dif < - mindata.price *0.01)
							 {
								 Close(tr,mindata,"stop_l_emergance");

							 }


					 }
					 else if (tr.Direction ==1  )
					 {

						 double dif=  last_mindata.price - mindata.price ;

						 if (mindata.price > tr.StopPrice)
						 {

							 Close(tr,mindata,"stop_hold");

						 }
						 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2)
						 {
							 Close(tr,mindata,"stop_opi_less");

						 }
						 else if (mindata.price > ema50)
						 {

							 Close(tr,mindata,"stop_ema50");

						 }
						 else if (dif <- mindata.price *0.01)
						 {
							 Close(tr,mindata,"stop_s_emergance");

						 }

					 }


				 }//exist 存在持仓单

i++;

			 }// for
		}
		else
		{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(productid);
		}


	}
	else
	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);
	}
//	printf ("check tablename : %s \n",tablename.c_str());




	vector<MinuteLineData>().swap(min_list);
}
void TrendTradeTest::Hold_OMA_MinuteLine(const char *pinstrumentid,
		const char *datetime,
		SqlAveragePriceData predata,
		HxDayLineData today)
{

//	printf ("MinuteLine_Hold begin \n");
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;
	MysqlTestRecord mysqltestrecord;
	MysqlDayLine mysqldayline;
	vector<MinuteLineData>  min_list;
	MysqlHisMinData mysqlhismindata;
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

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);

	if(mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )
		{

			ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);
			EMAData emadata;
			emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,datetime);



			 for (auto &mindata:min_list)
			 {
		//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )
				 {

		//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

					 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
					 char open[20];
					std::string str_open=tr.OpenDateTime;
					sprintf(open,"%s 00:00:00",str_open.substr(0,10).c_str());
					int holddays=mysqldayline.Count_HoldDays(pinstrumentid,open,datetime);

					int y;
					if (50-y >10)
					{
						y=50-y;
					}
					else
					{
						y=10;
					}
					double oma=mysqldayline.Get_MADatabyMinPrice(pinstrumentid,datetime,y,mindata.price);


					 double ema50,ema9,ema4;


					ema50 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,50,mindata.price);
					ema9 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,9,mindata.price);
					ema4 =mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,4,mindata.price);


					 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)
					 {
						 tr.Highest=mindata.price;
						 tr.StopPrice = tr.Highest -this->GetStopRange();


						 printf("	%s 持仓中   更新最高价   -------  %.3f   止损价  %.3f \n", mindata.MinuteTime,tr.Highest,tr.StopPrice );
						 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
								 pinstrumentid,mindata.price,tr.StopPrice,0);

					 }
					 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)
					{
						 tr.Lowest=mindata.price;
						 tr.StopPrice = tr.Lowest + this->GetStopRange();
						 printf("	%s 持仓中   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

						 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
					}


					 if (tr.Direction ==0  )
					 {

						 if (mindata.price < tr.StopPrice )
						 {

							 Close(tr,mindata,"stop_hold");
						  }
						 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 )
						 {
							 Close(tr,mindata,"stop_opi_less");
						 }
						 else if (mindata.price < ema50)
						 {
							 Close(tr,mindata,"stop_ema50");

						 }
//						 else if (mindata.price < oma)
//							 {
//								 Close(tr,mindata,"stop_oma");
//
//							 }
//						 else if ( mindata.price < tr.WinPrice)
//						 {
//								 printf("	持仓止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//									 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//
//						 }
//						 else if (mindata.price < today.Open*0.995 && today.Open < predata.AveragePrice *0.9975)
//						 {
//								 printf("	持仓行情反转   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//									 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//
//						 }

					 }
					 else if (tr.Direction ==1  )
					 {

						 if (mindata.price > tr.StopPrice)
						 {

							 Close(tr,mindata,"stop_hold");

						 }
						 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2)
						 {
							 Close(tr,mindata,"stop_opi_less");

						 }
						 else if (mindata.price > ema50)
						 {

							 Close(tr,mindata,"stop_ema50");

						 }
//						 else if (mindata.price > oma)
//						 {
//
//							 Close(tr,mindata,"stop_oma");
//
//						 }
//						 else if (mindata.price > tr.WinPrice)
//						 {

//								 printf("	持仓止盈   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//									 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//
//
//						 }
//						 else if (mindata.price > today.Open*1.005 && today.Open > predata.AveragePrice *1.0025)
//						 {
//
//
//								 printf("	持仓行情反转   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
//									 mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
//
//
//						 }

					 }


				 }//exist 存在持仓单



			 }// for
		}
		else
		{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(productid);
		}


	}
	else
	{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);
	}
//	printf ("check tablename : %s \n",tablename.c_str());




	vector<MinuteLineData>().swap(min_list);
}
void TrendTradeTest::UpdateOpenReference_EMA(const char *pinstrumentid,const char* mintime,EMAData emadata)
{

	MysqlTestRecord mysqltestrecord;
	TestRecordData tr;
	strcpy(tr.OpenDateTime,mintime);
	strcpy(tr.InstrumentID,pinstrumentid);

	tr.EMA50 = emadata.EMA50;
	tr.EMA9 = emadata.EMA9;
	tr.EMA4 = emadata.EMA4;

	tr.EMA50_X = emadata.EMA50_x;
	tr.EMA9_X = emadata.EMA9_x;
	tr.EMA4_X = emadata.EMA4_x;

	if (this->GetOpenFlag())
	{
		mysqltestrecord.UpdateRef(this->GetTableRecord().c_str(),tr);
	}

}

int  TrendTradeTest::Strategy_MA10_20_50_Pre()
{
	int ret=0;
	MysqlDayLine mysqldayline;
	double pre_ma50_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9,1);

	double pre_ma20 =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
	double pre_ma10 =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),10);

	bool pre_direction_long_20 =false;
	bool pre_direction_short_20 =false;
	if (pre_ma10 > pre_ma20 )
	{
		pre_direction_long_20=true;
		ret =1;
		printf("%s------[%s]pre_direction_long   ---> true  \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	}
	else if (pre_ma10 < pre_ma20 )
	{
		pre_direction_short_20 =true;
		ret =-1;
		printf("%s------[%s]pre_direction_short   ---> true  \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	}

return ret;
}

int  TrendTradeTest::Strategy_MA4_9_50_Pre()
{
	int ret=0;
	MysqlDayLine mysqldayline;
	double pre_ma50_offset =mysqldayline.Get_PreHisMADatabyOffset(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9,1);

	double pre_ma9 =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),9);
	double pre_ma4 =mysqldayline.Get_PreHisMAData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),4);

	bool pre_direction_long =false;
	bool pre_direction_short =false;
	if (pre_ma4 > pre_ma9 )
	{
		pre_direction_long=true;
		ret =1;
		printf("%s------[%s]pre_direction_long   ---> true  \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	}
	else if (pre_ma4 < pre_ma9 )
	{
		pre_direction_short =true;
		ret =-1;
		printf("%s------[%s]pre_direction_short   ---> true  \n",this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	}

return ret;
}
void TrendTradeTest::Create_TestReportXlsFile(std::string pathfile)
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
		ws->colwidth(i++,256*15);
		ws->colwidth(i++,256*15);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);
		ws->colwidth(i++,256*10);
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

		ws->label(1,i++,"多头交易数",xf);
		ws->label(1,i++,"多头盈利数",xf);
		ws->label(1,i++,"空头交易数",xf);
		ws->label(1,i++,"空头盈利数",xf);

		ws->label(1,i++,"平均盈利",xf);
		ws->label(1,i++,"平均亏损",xf);
		ws->label(1,i++,"盈亏比",xf);
		ws->label(1,i++,"胜率",xf);
		ws->label(1,i++,"期望值",xf);
		ws->label(1,i++,"错误数据天数",xf);
		ws->label(1,i++,"佣金",xf);
		ws->label(1,i++,"最终收益",xf);
		ws->label(1,i++,"Section",xf);

//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);

		range * _range;

		_range = ws->rangegroup(1,1,1,20);
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

int  total_tradecount_long =0;
int  total_tradecount_short=0;
int  total_wincount_long=0;
int  total_wincount_short=0;

double totalprofit=0;
double totalcommission=0;
int row;
		for (auto &item:list)
		{

		  row=y+2;


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
		  tradecount+=item.TradeCount;
//		  sprintf(buf,"%d",item.WinCount);
		  ws->number(row,7,item.WinCount,xf);
		  wincount+=item.WinCount;

		  ws->number(row,8,item.LongCount,xf);
		  ws->number(row,9,item.WinCount_Long,xf);
		  ws->number(row,10,item.ShortCount,xf);
		  ws->number(row,11,item.WinCount_Short,xf);

		  total_tradecount_long+=item.LongCount;
		  total_wincount_long+=item.WinCount_Long;

		  total_tradecount_short+=item.ShortCount;
		  total_wincount_short+=item.WinCount_Short;

//		  sprintf(buf,"%.2f",item.AverageWinProfit);
		  ws->number(row,12,item.AverageWinProfit,xf);


//		  sprintf(buf,"%.2f",item.AverageLoseProfit);
		  ws->number(row,13,item.AverageLoseProfit,xf);

//		  sprintf(buf,"%.2f",item.WinLoseRate);
		  ws->number(row,14,item.WinLoseRate,xf);

//		  sprintf(buf,"%.2f%%",item.WinRate*100);
		  ws->number(row,15,item.WinRate,xf);

//		  sprintf(buf,"%.2f",item.Expectation);
		  ws->number(row,16,item.Expectation,xf);

//		  sprintf(buf,"%d",item.ErrorMinuteData);
		  ws->number(row,17,item.ErrorMinuteData,xf);

//		  sprintf(buf,"%.2f",item.Asset);
		  ws->number(row,18,item.Commission,xf);
		  totalcommission+=item.Commission;
		  ws->number(row,19,item.Asset,xf);
		  totalprofit+=item.Asset;
		  ws->label(row,20,item.Section,xf);

		  if(item.Expectation <0 )
			{
					range * range_r;

					range_r = ws->rangegroup(row,1,row,20);
					range_r->cellcolor(CLR_GOLD);

//				_range = ws->rangegroup(row,1,row,18);
//				_range->cellcolor(CLR_GOLD);
//				printf("    %s 品种期望值为负值  \n",item.Product);
			}

			y++;
		}
		y++;
		row=y+2;
		ws->label(row,2,"总盈利",xf);
		ws->label(row,3,"限保证金总盈利",xf);
		ws->label(row,4,"总交易次数",xf);
		ws->label(row,5,"盈利次数",xf);
		ws->label(row,6,"总胜率",xf);
		ws->label(row,7,"多头胜率",xf);
		ws->label(row,8,"空头胜率",xf);

		ws->label(row,9,"历史最高",xf);
		ws->label(row,10,"最大回撤",xf);
		ws->label(row,11,"佣金",xf);
		ws->label(row,12,"最多持仓合约",xf);
		ws->label(row,13,"最高保证金",xf);
		y++;

		row=y+2;
		tradecount = mysqltestasset.CountbyAll(totaltablename.c_str());
		 wincount = mysqltestasset.CountbyWinProfit(totaltablename.c_str());
		 ws->number(row,2,totalprofit,xf);
		 ws->number(row,3,mysqltestasset.Get_SumbyProfit(totaltablename.c_str()),xf);
		 ws->number(row,4,tradecount,xf);
		 ws->number(row,5,wincount,xf);

		  char buf[20];
		  sprintf(buf,"%.2f%%",(double)wincount/(double)tradecount*100.0);
		  ws->label(row,6,buf,xf);


		  sprintf(buf,"%.2f%%",(double)total_wincount_long/(double)total_tradecount_long*100.0);
		  ws->label(row,7,buf,xf);


		  sprintf(buf,"%.2f%%",(double)total_wincount_short/(double)total_tradecount_short*100.0);
		  ws->label(row,8,buf,xf);

		 ws->number(row,9,mysqltestasset.Get_MaxbyHighestAsset(totaltablename.c_str()),xf);
		 ws->number(row,10,mysqltestasset.Get_MaxbyBackDownRate(totaltablename.c_str()),xf);
		 ws->number(row,11,mysqltestasset.Get_SumbyCommission(totaltablename.c_str()),xf);


		 TestAssetData testassetdata=MaxHoldRecord();

		 ws->number(row,12,testassetdata.SameTimeHold,xf);
		 ws->number(row,13,testassetdata.MaxMarginRate,xf);


//		  ws->number(row,12,mysqltestasset.Get_MaxbySameTimeHold(totaltablename.c_str()),xf);
//		 ws->number(row,13,mysqltestasset.Get_MaxbyMarginRate(totaltablename.c_str()),xf);

		wb.Dump(pathfile);
		vector<TestReportData> ().swap(list);
//						printf("Create_TestReportXlsFile  end! ...\n");

		printf("最终汇总表   -------%s   end! \n ",this->GetTableReport().c_str());

}



void TrendTradeTest::Create_TestRecordXlsFile(std::string pathfile)
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

			ws->label(1,24,"EMA50_X",xf);
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
		MysqlMaincontract mysqlmaincontract;
		mysqlmaincontract.Find_AllList(list);
		int i=0;
		for (auto &item:list)	{


			vector<TestRecordData>  ins_list;
			MysqlTestRecord mysqltestrecord;
			std::string tablename =this->GetTable_Record_Prefix();
			tablename.append(item.ProductID);
			if (mysqltestrecord.Exist_Table(tablename.c_str()))		{
				mysqltestrecord.Find_AllLists(ins_list,tablename.c_str());


				for (auto &record:ins_list)	{
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
					  ws->number(row,24,record.Close_AddOPIRate,xf);
					  ws->number(row,25,record.Close_VolOpiRate,xf);
					  ws->number(row,26,record.EMA9_X,xf);
					  ws->number(row,27,record.EMA4_X,xf);
					  ws->number(row,28,record.EMA50,xf);
					  ws->number(row,29,record.EMA9,xf);
					  ws->number(row,30,record.EMA4,xf);

				  if(record.Profit <0 )
					{
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



