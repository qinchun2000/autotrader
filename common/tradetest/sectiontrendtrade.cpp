#include "sectiontrendtrade.hpp"

SectionTrendTradeTest::SectionTrendTradeTest(const char* pstrategy,const char* year)
{

//	_section_strategy=pstrategy;
//
//	SetSectionYear(year);


	SetStrategy(pstrategy);
	SetYear(year);
	SetTable_Year_DateTime(year);


	SetTable_Report();

	SetTable_Record_Prefix();

	MysqlDateTime mysqldatetime;

	if(!mysqldatetime.Exist_Table(GetTable_Year_DateTime().c_str()))
		{
		CheckDateTimeSequence();
		}

}
void SectionTrendTradeTest::SetSectionYear(const char* year)
{
	_section_year=year;
}

std::string SectionTrendTradeTest::GetSectionYear()
{
	return this->_section_year;
}


// trademinute    --> 生成板块的分钟时间序列,按照对应的板块 进行回测
void SectionTrendTradeTest::Check_SectionContractMinuteTime(const char* section)
{
	MysqlMaincontract  mysqlmaincontract;
	MysqlDayLine mysqldayline;
	MysqlMinuteTime mysqlminutetime;
	MysqlHisMinData mysqlhismindata;

	std::string table_year = this->GetTable_Year_DateTime();


	MysqlDateTime mysqldatetime;
	vector<MainContractData>  contract_list;
	mysqlmaincontract.Find_ListsbySection(contract_list,section);


	for (auto &contract:contract_list)
	{

		printf("   contract   --> %s \n",contract.ProductID);


		vector<DateTime>  date_list;
		mysqldatetime.Find_AllLists(date_list,table_year.c_str());
		for (auto &dt:date_list)
		{

			printf("  %s  dayline ---> %s \n ",contract.ProductID,dt.Time);
			vector<MinuteLineData>  min_list;


			vector<std::string> instrument_list;
			GetInstrumentList(instrument_list,this->GetYear().c_str(),contract);

			HxDayLineData main=this->Collect_MainContractbyDateTime(instrument_list,dt.Time);


			std::string min_database;
			min_database = "min_";
			min_database.append(contract.ProductID);
			mysqlhismindata.Set_Database(min_database);

			std::string tmp = dt.Time;
			char date[20];
			sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

			std::string min_tablename;
			min_tablename=main.InstrumentID;
			min_tablename.append("_");
			min_tablename.append(date);

			std::string miutetime_tablename=section;
			miutetime_tablename.append("_");
			miutetime_tablename.append(date);


			if (mysqlhismindata.Exist_Table(min_tablename.c_str()) )
			{

				mysqlminutetime.CreateTable(miutetime_tablename.c_str());

				printf("database --%s    table---> %s \n",min_database.c_str(),min_tablename.c_str());
				mysqlhismindata.Find_AllList(min_list,min_tablename.c_str());

				for (auto &mindata:min_list)
				 {
					printf("   mindata ---> %s \n ",mindata.MinuteTime);
					if (!mysqlminutetime.Exist_MinuteTime(miutetime_tablename.c_str(),mindata.MinuteTime))
					{
						MinuteTimeData  minutetimedata;
						strcpy(minutetimedata.MinuteTime,mindata.MinuteTime);
						mysqlminutetime.Insert(miutetime_tablename.c_str(),minutetimedata);
						printf("insert minutetime %s \n",minutetimedata.MinuteTime);
					}
				 }
			}





			vector<MinuteLineData>().swap(min_list);
			vector<std::string>().swap(instrument_list);
		}


		vector<DateTime>().swap(date_list);
	}

	vector<MainContractData>().swap(contract_list);
}


void SectionTrendTradeTest::Check_SectionUpDownRate(const char* section)
{
	MysqlHisMinData mysqlhismindata;
	MysqlMinuteTime mysqlminutetime;

	std::string tablename_year =this->GetTable_Year_DateTime();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;

	MysqlMaincontract  mysqlmaincontract;
//	std::string section = "Black";

	MysqlHisSection mysqlhissection;

	MysqlAvePrice mysqlaveprice;

	mysqldatetime.Find_AllLists(list,tablename_year.c_str());
	for (auto &dt:list)
	{
		//收集当天的板块主力合约列表
		vector<std::string> section_list;
		Collect_MainContractsbySection(section_list,section,dt.Time);

		for (auto &ins:section_list)
		{
			printf("Check_SectionUpDownRate  %s --->%s     \n",dt.Time,ins.c_str());
		}

		//收集当天的分钟线 分钟数据 ,按分钟进行轮询
		std::string tmp = dt.Time;
		char date[20];
		sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

		std::string miutetime_tablename=section;
		miutetime_tablename.append("_");
		miutetime_tablename.append(date);


		std::string hissection_table=section;
		hissection_table.append("_");
		hissection_table.append(date);

		mysqlhissection.CreateTable(hissection_table.c_str());

		vector<MinuteTimeData>  minute_list;
		mysqlminutetime.Find_AllLists(minute_list,miutetime_tablename.c_str());


		for (auto &minute:minute_list)
		{
			HisSectionData hissectiondata;

			strcpy(hissectiondata.MinuteTime,minute.MinuteTime);
			hissectiondata.DownCount=0;
			hissectiondata.UpCount=0;
			hissectiondata.ZeroCount=0;
			hissectiondata.CountUprate=0;
			hissectiondata.CountDownrate=0;
			hissectiondata.Maxrate =0;
			hissectiondata.Minrate =1;
			hissectiondata.TotalUprate =0;
			hissectiondata.TotalDownrate=0;

			for (auto &ins:section_list)
			{

				SqlAveragePriceData predata = mysqlaveprice.Find_YesterdayDatabyDateTime(ins.c_str(),dt.Time);

				printf("%s   -->  %.3f  \n",ins.c_str(),predata.AveragePrice);
				char productid[10];
				CodeUtil codeutil;
				strcpy(productid,codeutil.GetProductID(ins.c_str()).c_str());

				std::string min_database;
				min_database = "min_";
				min_database.append(productid);
				mysqlhismindata.Set_Database(min_database);

				std::string min_tablename;
				min_tablename=ins.c_str();
				min_tablename.append("_");
				min_tablename.append(date);

				if (mysqlhismindata.Exist_Table(min_tablename.c_str()))
					{
						if (mysqlhismindata.Exist_DataBySqlTime(min_tablename.c_str(),minute.MinuteTime))
						{
							MinuteLineData mindata=mysqlhismindata.Find_DatabyMinuteTime(min_tablename.c_str(),minute.MinuteTime);
							if (mindata.price > predata.AveragePrice && predata.AveragePrice >0 )
							{
								hissectiondata.UpCount++;
								double uprate = (mindata.price-predata.AveragePrice)/predata.AveragePrice;
								printf("  %s   uprate ---> %.2f%%   mindata.price %.2f   predata.AveragePrice %.2f \n",ins.c_str(),uprate*100,mindata.price,predata.AveragePrice);
								hissectiondata.TotalUprate +=uprate;
								if (uprate > hissectiondata.Maxrate)
								{
									strcpy(hissectiondata.MaxInstrumentID,ins.c_str());
									hissectiondata.Maxrate =uprate;
								}
							}
							else if (mindata.price < predata.AveragePrice && predata.AveragePrice >0 )
							{
								hissectiondata.DownCount++;
								double downrate = (mindata.price-predata.AveragePrice)/predata.AveragePrice;
								printf("  %s   downrate ---> %.2f%%   mindata.price %.2f   predata.AveragePrice %.2f \n",ins.c_str(),downrate*100,mindata.price,predata.AveragePrice);
								hissectiondata.TotalDownrate +=downrate;
								if(downrate< hissectiondata.Minrate)
								{
									strcpy(hissectiondata.MinInstrumentID,ins.c_str());
									hissectiondata.Minrate = downrate;
								}
							}
							else
							{
								hissectiondata.ZeroCount++;
							}
						}

					}


			}


			double totalcount =(double) hissectiondata.UpCount + (double) hissectiondata.DownCount + (double) hissectiondata.ZeroCount;
			if (totalcount>0.0)
			{
				hissectiondata.CountUprate = (double) hissectiondata.UpCount/totalcount;
				hissectiondata.CountDownrate = (double) hissectiondata.DownCount/totalcount;
				hissectiondata.AverageUpDownrate = (hissectiondata.TotalDownrate + hissectiondata.TotalUprate)/totalcount;
			}

			if(!mysqlhissection.Exist_DatabyMinuteTime(hissection_table.c_str(),minute.MinuteTime) &&(hissectiondata.CountUprate>0 ||hissectiondata.CountDownrate>0 ))
			{
				printf("	[%s]-----inser-----CountUprate %.2f   CountDownrate %.2f   AverageUpDownrate %.2f \n",
						hissectiondata.MinuteTime,hissectiondata.CountUprate,hissectiondata.CountDownrate,hissectiondata.AverageUpDownrate);
				mysqlhissection.Insert(hissection_table.c_str(),hissectiondata);
			}
			else
			{
				printf("Check_SectionUpDownRate   --->  数据不合规!!!	\n");
			}


		}

		vector<MinuteTimeData>().swap(minute_list);
		vector<std::string>().swap(section_list);
	}


	vector<DateTime>().swap(list);
}
HxDayLineData SectionTrendTradeTest::Collect_MainContractbyDateTime(vector<std::string> &array,const char* datetime)
{
//	std::string tablename =table_year;

//		printf("Collect_MainContractbyDateTime [%s]---------\n",datetime);
	MysqlDayLine mysqldayline;
	HxDayLineData main;
	long int ccl=0;
	long int maxvol=0;
	for (auto &item:array)
	{
//			printf("  item   --> %s \n",item.c_str());
		if(mysqldayline.Exist_DatabySqlTime(item.c_str(),datetime))
		{
			HxDayLineData data=mysqldayline.Find_PreDataBySqlTime(item.c_str(),datetime);
			if (data.Volume > maxvol)
			{
				maxvol=data.Volume;
				main.Volume =data.Volume;
				main.Amount = data.Amount;
				strcpy(main.InstrumentID,item.c_str());
				main.Close = data.Close;
				main.Open = data.Open;
				main.High = data.High;
				main.Low = data.Low;
				main.LastClose = data.LastClose;
//					printf(" max  data:  %s  ------- main.volume --> %.3f \n",main.InstrumentID,main.Volume);
			}


		}
	}

	return main;
}
void  SectionTrendTradeTest::Collect_MainContractsbySection(vector<std::string> &array,const char * section,const char* datetime)
{

	MysqlMaincontract  mysqlmaincontract;

	vector<MainContractData>  product_list;
	mysqlmaincontract.Find_ListsbySection(product_list,section);


	for (auto &contract:product_list)
	{
		vector<std::string> instrument_list;

//		printf("product   -> %s \n",contract.ProductID);

		this->GetInstrumentList(instrument_list,this->GetYear().c_str(),contract);

		HxDayLineData main=Collect_MainContractbyDateTime(instrument_list,datetime);

//		printf("array -----> %s    \n",main.InstrumentID);
		array.push_back(main.InstrumentID);

		vector<std::string>().swap(instrument_list);

	}
	vector<MainContractData>().swap(product_list);
}

void SectionTrendTradeTest::TrendTradebySection(const char* section)
{

	MysqlMaincontract  mysqlmaincontract;
	vector<MainContractData>  product_list;

	mysqlmaincontract.Find_ListsbySection(product_list,section);
	for (auto &item:product_list)
	{

		vector<std::string> instrument_list;

			printf("TrendTradebySection ----->  %s  \n",item.ProductID);

		this->GetInstrumentList(instrument_list,this->GetYear().c_str(),item);
		printf("TrendTradebySection -----> GetInstrumentList ok !!! %s  \n",item.ProductID);

		if (strcmp(item.ProductID,"AP")!=0  &&  strcmp(item.ExchangeID,"CFFEX")!=0 && strcmp(item.Section,"Metal")!=0)
		//			if(strcmp(item.ProductID,"MA")==0)
		{
			MysqlTestRecord mysqltestrecord;

			this->SetTableRecord(item.ProductID);
			if (!mysqltestrecord.Exist_Table(GetTableRecord().c_str()))
			{
				SectionSingleIns_Trade(section,instrument_list);
			}
			else
			{
				printf(  " [%s] 品种已经完成测试 !!! \n",item.ProductID)	;
			}


		}




	vector<std::string>().swap(instrument_list);

	}

	vector<MainContractData>().swap(product_list);
}

void SectionTrendTradeTest::SectionSingleIns_Trade(const char* section,vector<std::string> &array)
{
	std::string tablename =GetTable_Year_DateTime();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;
	MysqlCycle mysqlcycle;
	MysqlTestRecord mysqltestrecord;
	MysqlAvePrice mysqlaveprice;
	MysqlMaincontract mysqlmaincontract;

	mysqldatetime.Find_AllLists(list,tablename.c_str());

	DateTime enddatetime =mysqldatetime.Find_LastDateTime(tablename.c_str());
	for (auto &dt:list)
	{
//		printf("[%s]---------\n",dt.Time);
		MysqlDayLine mysqldayline;

		HxDayLineData main=this->Collect_MainContractbyDateTime(array,dt.Time);
//		printf("[%s]--------- 主力合约 :  %s   open: %.3f  \n",dt.Time,main.InstrumentID,main.Open);


		CycleData cycledata = mysqlcycle.Find_DatabyDateTime(main.InstrumentID,dt.Time);
		CycleData highdata = mysqlcycle.GetHighestbyHighCount(main.InstrumentID,dt.Time,20);
		CycleData lowdata = mysqlcycle.GetLowestbyLowCount(main.InstrumentID,dt.Time,20);


		SqlAveragePriceData predata = mysqlaveprice.Find_YesterdayDatabyDateTime(main.InstrumentID,dt.Time);

		char productid[10];
		CodeUtil codeutil;
		strcpy(productid,codeutil.GetProductID(main.InstrumentID).c_str());


		if(!mysqltestrecord.Exist_Table(GetTableRecord().c_str()))
		{
			mysqltestrecord.CreateTable(GetTableRecord().c_str());
		}


		int count  =mysqltestrecord.Count_RecordByStatus(GetTableRecord().c_str(),0);



		if(count==0)    // 空仓
		{
			// 计算 当日开盘价
			HxDayLineData today=mysqldayline.Find_DataBySqlTime(main.InstrumentID,dt.Time);
//				printf("---空仓 %s  -------    presettle: %.3f    -------%s  \n",
//						main.InstrumentID,predata.AveragePrice,dt.Time);
			//趋势交易
			SectionMainContract_MinuteLineOpen(section,main.InstrumentID,dt.Time,predata,today.Open,highdata,lowdata,cycledata);



		}
		else
		{


			vector<TestRecordData>  record_list;

			 mysqltestrecord.Find_HoldLists(record_list,GetTableRecord().c_str());
//			 printf("	持仓合约数-------> %ld   \n" ,record_list.size());
			 for (auto &record:record_list)
			 {


				 if (strcmp(record.InstrumentID,main.InstrumentID)==0)
				 {
					 if(mysqldayline.Exist_DatabySqlTime(record.InstrumentID,dt.Time))
					 {
						 SqlAveragePriceData predata = mysqlaveprice.Find_YesterdayDatabyDateTime(record.InstrumentID,dt.Time);
						 HxDayLineData holddata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
						 if (holddata.Volume <50000 )
						 {
							 printf("	主力合约  %s -------  成交量变小 平仓  -------%s  \n",record.InstrumentID,dt.Time);

							 EndDayClose(record.InstrumentID,holddata);
						 }
						 else
						 {
	 //						 printf("	主力合约未改变 持有  %s -------    -------%s  \n",record.InstrumentID,dt.Time);
							 MainContract_MinuteLineHold(record.InstrumentID,dt.Time,predata,holddata);
						 }


					 }
					 else
					 {
						 printf("	主力合约未改变 持有无日线数据   %s -------    -------%s  \n",record.InstrumentID,dt.Time);
						 HxDayLineData lastdata=mysqldayline.Find_LastDayLineOffset(record.InstrumentID,5);

						 EndDayClose(record.InstrumentID,lastdata);
					 }


				 }
				 else
				 {

//					 printf("	非主力合约持仓  %s -------    -------%s  \n",record.InstrumentID,dt.Time);
					 if(mysqldayline.Exist_DatabySqlTime(record.InstrumentID,dt.Time))
						 {
						 	 SqlAveragePriceData predata = mysqlaveprice.Find_YesterdayDatabyDateTime(record.InstrumentID,dt.Time);
						 	 HxDayLineData holddata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
							 if (holddata.Volume <50000 )
							 {
								 printf("	非主力合约  %s -------  成交量变小 平仓  -------%s  \n",record.InstrumentID,dt.Time);
								 EndDayClose(record.InstrumentID,holddata);
							 }
							 else
							 {
								 MainContract_MinuteLineHold(record.InstrumentID,dt.Time,predata,holddata);
							 }

						 }
					 else
					 {
						 printf("	非主力合约持仓 持有无日线数据 %s -------    -------%s  \n",record.InstrumentID,dt.Time);
						 HxDayLineData lastdata=mysqldayline.Find_LastDayLineOffset(record.InstrumentID,1);

						 EndDayClose(record.InstrumentID,lastdata);

					 }




				 }


				 if (strcmp(dt.Time,enddatetime.Time)==0)
				 {
					 printf("	合约 截止年度最后交易日   %s -------    -------%s  \n",record.InstrumentID,dt.Time);
					 HxDayLineData lastdata=mysqldayline.Find_DataBySqlTime(record.InstrumentID,dt.Time);
					 EndDayClose(record.InstrumentID,lastdata);
				 }

			 }

//			MinuteLine_Hold(main.InstrumentID,dt.Time);
			 vector<TestRecordData>().swap(record_list);
		}
	}
	vector<DateTime>().swap(list);


}


void SectionTrendTradeTest::SectionMainContract_MinuteLineOpen(const char*section,
		const char *pinstrumentid,
		const char *datetime,
		SqlAveragePriceData predata,
		double openprice,
		CycleData highdata,
		CycleData lowdata,
		CycleData cycledata)
{
//	printf ("MinuteLine_OpenTest begin \n");

	this->SetOpenFlag(false);
	this->SetCloseFlag(false);

	int count=2;

	MysqlTestRecord mysqltestrecord;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlMA mysqlma;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;
	MysqlHisMA mysqlhisma;
	MysqlAvePrice mysqlaveprice;
	MysqlDayLine mysqldayline;

	MysqlHisSection mysqlhissection;
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
	if (mysqlhismindata.Exist_Table(tablename.c_str()))
	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());
	}
	else
	{
		printf(" 空仓 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);

	}


	double uplimit = openprice *1.01;
	double downlimit = openprice *0.99;

	MAData madata;
	madata=mysqlma.Find_MADatabyTime(pinstrumentid,datetime);

	EMAData emadata;
	emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,datetime);

	HisMAData hismadata;
	hismadata = mysqlhisma.Find_HisMADatabyDateTime_Offset(pinstrumentid,datetime,1);

	HisEMAData hisemadata;
	hisemadata = mysqlhisema.Find_PreHisEMADatabyDateTime(pinstrumentid,datetime);


	bool pre_direction_long =false;
	bool pre_direction_short =false;
	if (hisemadata.EMA4 > hisemadata.EMA9 )
	{
		pre_direction_long=true;
	}
	else if (hisemadata.EMA4 < hisemadata.EMA9 )
	{
		pre_direction_short =true;
	}


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
	hisemadata_x = mysqlhisema.Find_HisEMADatabyDateTime_Offset(pinstrumentid,datetime,1);


	ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);

	this->SetStopRange(atr_data.ATR *this->GetStopRatio());

	 for (auto &mindata:min_list)
	 {
//		 printf ("check %s date[%s]    ---->   mindata : %s \n",tablename.c_str(),datetime,mindata.MinuteTime  );

		 if(!mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) && !this->GetOpenFlag() && !this->GetCloseFlag())
		 {

			double ma50;
			ma50=(madata.MA49+mindata.price)/50;
			double ma20;
			ma20=(madata.MA19+mindata.price)/20;
			double ma10;
			ma10=(madata.MA9+mindata.price)/10;

			double ema50,ema9,ema4;
			double ema20;
			ema50 = (2*mindata.price + 49*emadata.EMA49)/51;
			ema20 = mysqldayline.Get_EMADatabyMinPrice(pinstrumentid,datetime,20,mindata.price);
			ema9 = (2*mindata.price + 8*emadata.EMA8)/10;
			ema4 = (2*mindata.price + 3*emadata.EMA3)/5;




			double ma50_last =hismadata.MA50;
			double ma20_last =hismadata.MA20;
			double ma10_last =hismadata.MA10;
			double x50 = atan((ma50/ma50_last-1)*100)*180/M_PI;
			double x20 = atan((ma20/ma20_last-1)*100)*180/M_PI;
			double x10 = atan((ma10/ma10_last-1)*100)*180/M_PI;

			double ema_x50 = atan((ema50/hisemadata_x.EMA50-1)*100)*180/M_PI;
			double ema_x20 = atan((ema20/hisemadata_x.EMA20-1)*100)*180/M_PI;
			double ema_x9 = atan((ema9/hisemadata_x.EMA9-1)*100)*180/M_PI;
			double ema_x4 = atan((ema4/hisemadata_x.EMA4-1)*100)*180/M_PI;


			if (strcmp(section,"none")!=0)
			{

				hissectiondata = mysqlhissection.Find_DatabyMinuteTime(hissection_table.c_str(),mindata.MinuteTime);
			}





//			printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f   下跌品种:%.2f   平均涨跌幅 : %.2f \n",hissection_table.c_str(),
//					mindata.MinuteTime,hissectiondata.CountUprate,hissectiondata.CountDownrate,hissectiondata.AverageUpDownrate);
			bool condition_long1  = ma50<ma20 && ma20< ma10 ;
			bool condition_long6 = x50 >0 && x20 >0 && x10>0;

			bool condition_long_ema = ema50< ema9 && ema9<ema4  && ema4< mindata.price  && mindata.price > openprice;
			bool condition_long_ema50_20_4 = ema50< ema20 && ema20<ema9  && ema9< mindata.price  && mindata.price > openprice;
			bool condition_long_ema_x = ema_x50>0  && ema_x50!=90 ;




			bool condition_long2 = mindata.price >openprice*1.005 && openprice > predata.AveragePrice*1.0025;
			bool condition_long4 = atrcount2x ==0;
			bool condition_long3 =false;

			bool condition_long5_a =false;
			bool condition_long5_b =false;
			bool condition_long5_c =false;

			if (strcmp(section,"none")!=0)
			{
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
			bool condition_short6 = x50 < 0 && x20 <0 && x10<0;

			bool condition_short_ema = ema50> ema9 && ema9>ema4 && ema4 > mindata.price  && mindata.price < openprice ;
			bool condition_short_ema50_20_4 = ema50> ema20 && ema20>ema9 && ema9 > mindata.price  && mindata.price < openprice;
			bool condition_short_ema_x = ema_x50<0   && ema_x50!=90 ;



			bool condition_short2 = mindata.price < openprice*0.995 && openprice < predata.AveragePrice*0.9975;
			bool condition_short3=false ;
			bool condition_short4 = atrcount2x ==0;

			bool condition_short5_a=false;
			bool condition_short5_b=false;
			bool condition_short5_c=false;

			if (strcmp(section,"none")!=0)
			{
				condition_short5_b =hissectiondata.CountDownrate >0.7;
				condition_short5_a =(mindata.price  -predata.AveragePrice )/predata.AveragePrice < hissectiondata.AverageUpDownrate;
				condition_short5_c = strcmp(hissectiondata.MinInstrumentID,pinstrumentid)==0;

			}
			bool condition_short_section =condition_short5_a && condition_short5_b ;

			bool condition_short7 = x10 < x20 && x20 <x50 && x50<0;
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

//			bool condition_long_trend = condition_long1 && condition_long6;
//			bool condition_long = condition_long_trend    && condition_long_pattern;
//
//			bool condition_short_trend = condition_short1 && condition_short6;
//			bool condition_short = condition_short_trend  && condition_short_pattern;


			/* EMA 开仓法*/
			// 开多总条件
			bool condition_long_trend = condition_long_ema  && condition_long_ema_x;
			bool condition_long = condition_long_trend    && !pre_direction_long && condition_long_section;

			//开空总条件
			bool condition_short_trend = condition_short_ema && condition_short_ema_x;
			bool condition_short = condition_short_trend  && !pre_direction_short && condition_short_section;



			 if (condition_long  )
				{

				 int atrcount = mysqlhisatr.Count_ListsByTime_N(pinstrumentid,datetime,20);
				 int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(pinstrumentid,datetime,20);
				 double atr_indicator = mysqlhisatr.Indicator_ListsByTime_N(pinstrumentid,datetime,20);
				 printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f%%   下跌品种:%.2f%%   平均涨跌幅 : %.2f%% MaxInstrumentID %s \n",hissection_table.c_str(),
								mindata.MinuteTime,hissectiondata.CountUprate*100,hissectiondata.CountDownrate*100,100*hissectiondata.AverageUpDownrate,hissectiondata.MaxInstrumentID);


				 BuyOpen(mindata, pinstrumentid,"ema_open");

				}

			 if (condition_short )
				{
				 int atrcount = mysqlhisatr.Count_ListsByTime_N(pinstrumentid,datetime,20);
					int atrcount2x = mysqlhisatr.Count2x_ListsByTime_N(pinstrumentid,datetime,20);
					double atr_indicator = mysqlhisatr.Indicator_ListsByTime_N(pinstrumentid,datetime,20);
					ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);
//				 	printf("ema50 %.3f  ema9 %.3f ema4 %.3f   \n",ema50,ema9,ema4);
//				 	printf("  %s-%s   atrcount -----[%d]    atrcount2x -----[%d]   atr_indicator  ---> %.2f ATR  ---> %.3f \n ",
//				 			pinstrumentid,datetime,atrcount,atrcount2x,atr_indicator, atr_data.ATR);
//				 	printf("  %s-%s   x50 -----[%.2f]    x20 -----[%.2f]   x10  ---> [%.2f] \n ",
//				 													pinstrumentid,datetime,x50,x20,x10);
				 	printf("table:%s  hissectiondata  [%s] --->    上涨品种:%.2f%%   下跌品种:%.2f%%   平均涨跌幅 : %.2f%% MinInstrumentID %s \n",hissection_table.c_str(),
				 						mindata.MinuteTime,hissectiondata.CountUprate*100,hissectiondata.CountDownrate*100,100*hissectiondata.AverageUpDownrate,hissectiondata.MinInstrumentID);
//				 	int p= 20000.00/( mindata.price* maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio);

				 	 SellOpen(mindata, pinstrumentid,"ema_open");


				}
		 }

		 else if (mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) && this->GetOpenFlag() && !this->GetOpenFlag())
		 {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);

			 time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
			 // 更新持仓后的历史新低
			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)
			 {
				 tr.Highest=mindata.price;
				 tr.StopPrice = tr.Highest -this->GetStopRange();


				 printf("	%s  开仓日   更新最高价   -------  %.3f   止损价  %.3f \n",mindata.MinuteTime, tr.Highest,tr.StopPrice );
				 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
						 pinstrumentid,mindata.price,tr.StopPrice,0);

			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)
			{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + this->GetStopRange();
				 printf("	%s 开仓日   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

				 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
			}


			 if (tr.Direction ==0 && mindata.price < tr.StopPrice)
			 {

				 Close(tr,mindata,"stop_openday");

				 this->SetCloseFlag(true);
			 }
			 else if (tr.Direction ==1 && mindata.price > tr.StopPrice)
			 {

				 Close(tr,mindata,"stop_openday");

				 this->SetCloseFlag(true);
			 }


		 }



	 }

	vector<MinuteLineData>().swap(min_list);
}


void  SectionTrendTradeTest::Create_TestRecordXlsFilebySection(std::string pathfile,const char*section)
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

		ws->label(1,1,"序号",xf);
		ws->label(1,2,"合约",xf);
		ws->label(1,3,"开仓时间",xf);
		ws->label(1,4,"方向",xf);
		ws->label(1,5,"板块平均涨跌幅",xf);
		ws->label(1,6,"昨日结算价",xf);
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
		ws->label(1,22,"OpenOPI",xf);
		ws->label(1,23,"CloseOPI",xf);
//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);
		int colume_size =23;

		range * _range;

		_range = ws->rangegroup(1,1,1,colume_size);
		_range->cellcolor(CLR_GOLD);

		MysqlMaincontract  mysqlmaincontract;
		vector<MainContractData>  list;

		mysqlmaincontract.Find_ListsbySection(list,section);;
		int i=0;
		for (auto &item:list)
		{

			MysqlTestRecord mysqltestrecord;
			vector<TestRecordData>  ins_list;


			std::string tablename =this->GetTable_Record_Prefix();
				tablename.append(item.ProductID);

			if (mysqltestrecord.Exist_Table(tablename.c_str()))
			{
				mysqltestrecord.Find_AllLists(ins_list,tablename.c_str());


				for (auto &record:ins_list)
				{

				  int row=i+2;



				  ws->number(row,1,i+1,xf);

				  char buf[100];
				  sprintf(buf,"%s",record.InstrumentID);
				  ws->label(row,2,buf,xf);


				  sprintf(buf,"%s",record.OpenDateTime);
				  ws->label(row,3,buf,xf);

				  ws->number(row,4,record.Direction,xf);
				  ws->number(row,5,record.AverageUpDownrate,xf);
				  ws->number(row,6,record.PreSettlePrice,xf);
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

				  ws->number(row,22,record.OpenOPI,xf);

				  ws->number(row,23,record.CloseOPI,xf);

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
