#include "tradetest.hpp"

TradeTest::TradeTest()
{
	SetTable_Whole_DateTime("alltime");

	printf("TradeTest --> 构造函数   \n");
}
TradeTest::TradeTest(const char* pstrategy,const char* year)
{
	printf("TradeTest --> 带参数构造函数   \n");
	SetStrategy(pstrategy);
	SetYear(year);
	SetTable_Year_DateTime(year);
	SetTable_Whole_DateTime("alltime");

	SetTable_Report();
	SetTable_Record_Prefix();
	MysqlDateTime mysqldatetime;

	if(!mysqldatetime.Exist_Table(GetTable_Year_DateTime().c_str()))
	{
		this->CheckDateTimeSequence();
	}
}

void TradeTest::HisDataCheck(const char* year)
{


	MysqlUtil mysqlutil("test");
	vector<MainContractData>  list;
	MysqlMaincontract mysqlmaincontract;

	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)	{

		vector<std::string> instrument_list;
		GetInstrumentList(instrument_list,year,item);
//		if ( strcmp(item.ExchangeID,"CFFEX")!=0 && strcmp(item.Section,"Metal")!=0 ){
		if(strcmp(item.ProductID,"ag")==0)		{
//		if(strcmp(item.ProductID,"rb")==0 || strcmp(item.ProductID,"ag"))		{
			for (auto &ins:instrument_list)	{
				printf(  " 合约列表 ---> %s \n",ins.c_str())	;
//				mysqlutil.CheckAllHisDataByInstrument(ins.c_str());

				mysqlutil.CheckHttpDayLineByInstrument(ins.c_str());
				mysqlutil.CorrectDayLineByInstrument(ins.c_str());
//				mysqlutil.CheckCycleDataByInstrument(ins.c_str());
//				mysqlutil.CheckMADataByInstrument(ins.c_str());
//				mysqlutil.CheckEMADataByInstrument(ins.c_str());
//				mysqlutil.CheckHisEMADataByInstrument(ins.c_str());
//				mysqlutil.CheckHisMADataByInstrument(ins.c_str());
				mysqlutil.CheckHttpMinuteLineByInstrument(ins.c_str());
//				mysqlutil.CheckAveragePriceByInstrument(ins.c_str());
//				mysqlutil.CheckHisATRDataByInstrument(ins.c_str());

			}

		}


		vector<std::string>().swap(instrument_list);

	}

	vector<MainContractData>().swap(list);
}

void TradeTest::HisCsvDataCheck(const char* year)
{


	MysqlUtil mysqlutil("test");
	vector<MainContractData>  list;

	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;
	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)	{

		vector<std::string> instrument_list;

		GetInstrumentList(instrument_list,year,item);

		if(strcmp(item.ProductID,"i")==0 || strcmp(item.ProductID,"j")==0 || strcmp(item.ProductID,"jm")==0)	{



					MysqlAvePrice mysqlaveprice;
					MysqlHisMA mysqlhisma;
					MysqlMA mysqlma;
					MysqlHisEMA mysqlhisema;
					MysqlEMA mysqlema;
					MysqlHisATR mysqlhisatr;
					MysqlCycle mysqlcycle;

					for (auto &ins:instrument_list)		{
							if(mysqldayline.Exist_Table(ins.c_str()))
							{
								mysqldayline.DropbyTableName(ins.c_str());
							}

							if(mysqlaveprice.Exist_Table(ins.c_str()))
							{
								mysqlaveprice.DropbyTableName(ins.c_str());
							}
							if(mysqlhisma.Exist_Table(ins.c_str()))
							{
								mysqlhisma.DropbyTableName(ins.c_str());
							}
							if(mysqlma.Exist_Table(ins.c_str()))
							{
								mysqlma.DropbyTableName(ins.c_str());
							}
							if(mysqlhisema.Exist_Table(ins.c_str()))
							{
								mysqlhisema.DropbyTableName(ins.c_str());
							}
							if(mysqlema.Exist_Table(ins.c_str()))
							{
								mysqlema.DropbyTableName(ins.c_str());
							}
							if(mysqlhisatr.Exist_Table(ins.c_str()))
							{
								mysqlhisatr.DropbyTableName(ins.c_str());
							}


						}


				}

		if(strcmp(item.ProductID,"i")==0 || strcmp(item.ProductID,"j")==0 || strcmp(item.ProductID,"jm")==0)
		{
			for (auto &ins:instrument_list)
			{
				printf(  " 合约列表 ---> %s \n",ins.c_str())	;
				mysqlutil.CheckAllCsvDataByInstrument(ins.c_str());

			}

		}


		vector<std::string>().swap(instrument_list);

	}

	vector<MainContractData>().swap(list);
}

// year    ----- 2017


void TradeTest::GetInstrumentList(vector<std::string> &instrument_list ,const char* year,MainContractData item)
{
	std::string str_year=year;
	char year_3[10];
	char year_4[10];
	std::string year_34;

	char before_year_4[10];
	char next_year_4[10];
	char before_year_34[10];
	char next_year_34[10];

	int i_year_3;
	int i_year_4;
	int i_year_34;

	strcpy(year_3,str_year.substr(2,1).c_str());
	strcpy(year_4,str_year.substr(3,1).c_str());
	year_34=str_year.substr(2,2).c_str();

	i_year_3 = atoi(year_3);
	i_year_4 = atoi(year_4);

	i_year_34 =atoi (year_34.c_str());

	sprintf(before_year_4,"%d",abs(i_year_4 -1)%10);
	sprintf(next_year_4,"%d",abs(i_year_4 +1)%10);

	sprintf(before_year_34,"%d",i_year_34-1);
	sprintf(next_year_34,"%d",i_year_34+1);

	if (strcmp(item.ExchangeID,"CZCE")==0)	{

		for (int i=0;i<6;i++)	{
			std::string instrumentid;
			instrumentid=item.ProductID;

			if (i==0)	{
				instrumentid.append(year_4);
				instrumentid.append("01");
			}
			else if (i==1)	{
				instrumentid.append(year_4);
				instrumentid.append("05");
			}
			else if (i==2)	{
				instrumentid.append(year_4);
				instrumentid.append("09");
			}
			else if (i==3)	{
				instrumentid.append(year_4);
				instrumentid.append("10");
			}
			else if (i==4)	{
				instrumentid.append(next_year_4);
				instrumentid.append("01");
			}
			else if (i==5)	{
				instrumentid.append(next_year_4);
				instrumentid.append("05");
			}

			instrument_list.push_back(instrumentid.c_str());
		}


	}
	else if(strcmp(item.ProductID,"bu")==0)	{

		for (int i=0;i<5;i++)	{
			std::string instrumentid;
			instrumentid=item.ProductID;
			if (i==0)	{
				instrumentid.append(before_year_34);
				instrumentid.append("12");
			}
			else if (i==1)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("06");
			}
			else if (i==2)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("09");
			}
			else if (i==3)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("12");
			}
			else if (i==4)	{
				instrumentid.append(next_year_34);
				instrumentid.append("06");
			}

			instrument_list.push_back(instrumentid.c_str());
		}

	}
	else if(strcmp(item.Section,"Metal")==0)	{

		for (int i=0;i<15;i++)	{
			std::string instrumentid;
			instrumentid=item.ProductID;
			if (i==0)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("01");
			}
			else if (i==1)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("02");
			}
			else if (i==2)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("03");
			}
			else if (i==3)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("04");
			}
			else if (i==4)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("05");
			}
			else if (i==5)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("06");
			}
			else if (i==6)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("07");
			}
			else if (i==7)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("08");
			}
			else if (i==8)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("09");
			}
			else if (i==9)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("10");
			}
			else if (i==10)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("11");
			}
			else if (i==11)
			{
				instrumentid.append(year_34.c_str());
				instrumentid.append("12");
			}
			else if (i==12)	{
				instrumentid.append(next_year_34);
				instrumentid.append("01");
			}
			else if (i==13)	{
				instrumentid.append(next_year_34);
				instrumentid.append("02");
			}
			else if (i==14)	{
				instrumentid.append(next_year_34);
				instrumentid.append("03");
			}
			instrument_list.push_back(instrumentid.c_str());
		}

	}

	else if(strcmp(item.Section,"Precious")==0)			{

		for (int i=0;i<3;i++)	{
			std::string instrumentid;
			instrumentid=item.ProductID;
			if (i==0)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("06");
			}
			else if (i==1)	{
				instrumentid.append(year_34.c_str());
				instrumentid.append("12");
			}

			else if (i==2)	{
				instrumentid.append(next_year_34);
				instrumentid.append("06");
			}

			instrument_list.push_back(instrumentid.c_str());
		}

	}
	else{
		for (int i=0;i<6;i++)	{
			std::string instrumentid;
			instrumentid=item.ProductID;
			if (i==0)		{
				instrumentid.append(year_34.c_str());
				instrumentid.append("01");
			}
			else if (i==1)		{
				instrumentid.append(year_34.c_str());
				instrumentid.append("05");
			}
			else if (i==2)		{
				instrumentid.append(year_34.c_str());
				instrumentid.append("09");
			}
			else if (i==3)		{
				instrumentid.append(year_34.c_str());
				instrumentid.append("10");
			}
			else if (i==4)	{
				instrumentid.append(next_year_34);
				instrumentid.append("01");
			}
			else if (i==5)	{
				instrumentid.append(next_year_34);
				instrumentid.append("05");
			}
			instrument_list.push_back(instrumentid.c_str());
		}


	}
//				printf(  " 插入合约列表 ---> %s \n",instrumentid.c_str())	;


}

HxDayLineData TradeTest::Get_CurrentMaincontractbyTime(vector<std::string> &array,const char* sqltime)
{
	HxDayLineData main_data;
	main_data.Open=0.0;
	strcpy(main_data.InstrumentID,"");
	MysqlDayLine mysqldayline;

	long int ccl=0;
	long int maxvol=0;
	for (auto &item:array)	{
		if(mysqldayline.Exist_Table(item.c_str()))		{
			if(mysqldayline.Exist_DatabySqlTime(item.c_str(),sqltime))		{
					//根据前一日 计算主力合约
					HxDayLineData data=mysqldayline.Find_PreDataBySqlTime(item.c_str(),sqltime);
					if (data.Volume > maxvol)	{
						maxvol=data.Volume;
						main_data.Volume =data.Volume;
						main_data.Amount = data.Amount;
						strcpy(main_data.InstrumentID,item.c_str());
						main_data.Close = data.Close;
						main_data.Open = data.Open;
						main_data.High = data.High;
						main_data.Low = data.Low;
						main_data.LastClose = data.LastClose;
//							printf("Get_CurrentMaincontractbyTime: max  data:  %s  ------- main.volume --> %.3f \n",main_data.InstrumentID,main_data.Volume);
					}
					else		{
//						printf("Get_CurrentMaincontractbyTime:  item   数据非主力--> %s   --> %.f \n",item.c_str(),data.Volume);
					}

			}
			else	{
//				printf("Get_CurrentMaincontractbyTime:  item   无%s当天日线数据--> %s \n",sqltime,item.c_str());
			}

		}
		else{
//			printf("Get_CurrentMaincontractbyTime:  item   无数据表--> %s \n",item.c_str());
		}
//			printf("  item   --> %s \n",item.c_str());

	}

	return main_data;
}

void TradeTest::MainContract_MinuteLineHold(const char *pinstrumentid,
		const char *datetime,
		SqlAveragePriceData predata,
		HxDayLineData today)
{
//	printf ("MinuteLine_Hold begin \n");
	MysqlDayLine mysqldayline;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisATR mysqlhisatr;
	MysqlEMA mysqlema;
	MysqlHisEMA mysqlhisema;

	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	MysqlHisMinData mysqlhismindata;
	mysqlhismindata.Set_Database(min_database);

	std::string tmp = datetime;
	char date[20];
	sprintf(date,"%s%s%s",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(date);
	MysqlTestRecord mysqltestrecord;
	if(mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )		{

			ATRData atr_data =mysqlhisatr.FindPreDatabyDateTime(pinstrumentid,datetime);
			EMAData emadata;
			emadata=mysqlema.Find_EMADatabyTime(pinstrumentid,datetime);



			 for (auto &mindata:min_list)			 {
		//		 printf ("check %s mindata : %s \n",tablename.c_str(),mindata.MinuteTime);

				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) )		 {

		//			 printf("MinuteLine_Hold 持仓中   -------%s  \n",pinstrumentid);

					 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
					 char open[20];
					std::string str_open=tr.OpenDateTime;
					sprintf(open,"%s 00:00:00",str_open.substr(0,10).c_str());
					int holddays= mysqldayline.Count_HoldDays(pinstrumentid,open,datetime);

					int y;
					if (50-y >10)		{
						y=50-y;
					}
					else	{
						y=10;
					}
					double oma=mysqldayline.Get_MADatabyMinPrice(pinstrumentid,datetime,y,mindata.price);


					 double ema50,ema9,ema4;
					ema50 = (2*mindata.price + 49*emadata.EMA49)/51;
					ema9 = (2*mindata.price + 8*emadata.EMA8)/10;
					ema4 = (2*mindata.price + 3*emadata.EMA3)/5;


					 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice)	 {
						 tr.Highest=mindata.price;
						 tr.StopPrice = tr.Highest -this->GetStopRange();


						 printf("	%s 持仓中   更新最高价   -------  %.3f   止损价  %.3f \n", mindata.MinuteTime,tr.Highest,tr.StopPrice );
						 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
								 pinstrumentid,mindata.price,tr.StopPrice,0);

					 }
					 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)		{
						 tr.Lowest=mindata.price;
						 tr.StopPrice = tr.Lowest + this->GetStopRange();
						 printf("	%s 持仓中   更新最低价   -------%.3f    止损价  %.3f\n",mindata.MinuteTime,tr.Lowest,tr.StopPrice );

						 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
					}


					 if (tr.Direction ==0  )	 {

						 if (mindata.price < tr.StopPrice )	 {

							 Close(tr,mindata,"stop_hold");
						  }
						 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 )	 {
							 Close(tr,mindata,"stop_opi_less");
						 }
						 else if (mindata.price < ema50)	 {
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
					 else if (tr.Direction ==1  )		 {

						 if (mindata.price > tr.StopPrice)		 {

							 Close(tr,mindata,"stop_hold");

						 }
						 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2)		 {
							 Close(tr,mindata,"stop_opi_less");

						 }
						 else if (mindata.price > ema50)		 {

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
		else{
			printf(" 当日无分钟数据   ---> %s    \n",tablename.c_str());
			CheckErrorData(productid);
		}
	}
	else{
		printf(" 不存在分钟数据   ---> %s    \n",tablename.c_str());
		CheckErrorData(productid);
	}
//	printf ("check tablename : %s \n",tablename.c_str());

	vector<MinuteLineData>().swap(min_list);
}


void TradeTest::EndDay(const char* pinstrumentid,double endday_close,const char* endday)
{
	MysqlMaincontract mysqlmaincontract;
	char ProductID[10];
	CodeUtil codeutil;
	strcpy(ProductID,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(ProductID);
	MysqlTestRecord mysqltestrecord;
	if (mysqltestrecord.Exist_RecordByStatus(this->GetTableRecord().c_str(),0))	{
			TestRecordData tr =mysqltestrecord.Find_RecordByStatus(this->GetTableRecord().c_str(),0);

			 tr.ClosePrice = endday_close;
			 strcpy(tr.CloseDateTime,endday);
			 tr.Status =1;
			 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) ;
			 if(mysqltestrecord.Exist_RecordByStatus(this->GetTableRecord().c_str(),0))	 {
				 mysqltestrecord.Update(this->GetTableRecord().c_str(),tr);
			 }

			 printf("	合约交割平仓   -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
					 endday,endday_close,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
		}
}


void TradeTest::EndDayClose(const char* pinstrumentid,HxDayLineData lastdata)
{


	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	DateUtil dateutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	std::string min_database;
	min_database = "min_";
	min_database.append(this->GetProductID().c_str());
//	printf ("check min_database : %s \n",min_database.c_str());
	MysqlHisMinData mysqlhismindata;
	mysqlhismindata.Set_Database(min_database);

	std::string str_date = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());
	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(str_date);
	MysqlTestRecord mysqltestrecord;
	int startopi = mysqlhismindata.GetStartOpenInterest(tablename.c_str());

	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0))	{

		 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
		 if (tr.Direction ==0  )	 {

//					 SqlAveragePriceData sap_data=mysqlaveprice.Find_YesterdayDatabyDateTime(pinstrumentid,lastdata.Time);


			 int closevolume = tr.Volume - tr.CloseVolume;
				double closeaveprice = (tr.ClosePrice * tr.CloseVolume + lastdata.Close*closevolume)/(tr.Volume);

				tr.ClosePrice = closeaveprice;
				 tr.CloseVolume=tr.Volume;


			 tr.CloseOPI = startopi;
				 tr.ClosePrice = closeaveprice;
				 strcpy(tr.CloseDateTime,lastdata.Time);
				 tr.Status =1;

				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.Volume;
				 strcpy(tr.CloseSignal,"end_contract");
				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0))	 {
					 mysqltestrecord.UpdatebyInstrument(this->GetTableRecord().c_str(),pinstrumentid,tr);
				 }

				 printf("	多头合约交割平仓 平仓手数:[%d]  -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
						 closevolume,lastdata.Time,lastdata.Close,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);


		 }
		 else if (tr.Direction ==1  )	 {

				int closevolume = tr.Volume - tr.CloseVolume;
				double closeaveprice = (tr.ClosePrice * tr.CloseVolume + lastdata.Close*closevolume)/(tr.Volume);

				tr.ClosePrice = closeaveprice;
				 tr.CloseVolume=tr.Volume;

				 tr.CloseOPI = startopi;

				 tr.ClosePrice = closeaveprice;
				 strcpy(tr.CloseDateTime,lastdata.Time);
				 tr.Status =1;

				 tr.Profit = (double)maincontractdata.VolumeMultiple  *( tr.OpenPrice -tr.ClosePrice )*tr.Volume ;
				 strcpy(tr.CloseSignal,"end_contract");
				 if(mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0))	 {
					 mysqltestrecord.UpdatebyInstrument(this->GetTableRecord().c_str(),pinstrumentid,tr);
					 printf("	空头合约交割平仓  平仓手数:[%d] -------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",
							 closevolume,lastdata.Time,lastdata.Close,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
				 }

		 }

	 }
}

double TradeTest::GetSedimentary(const char * pinstrumentid,int opi,double price)
{

	double sedimentary;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	sedimentary=opi * price * (double)maincontractdata.VolumeMultiple * commission.Margin /100000000.0;

	return sedimentary;
}
int TradeTest::GetPositionbyATR(const char * pinstrumentid)
{
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);

	return p_stop;
}

int TradeTest::GetMinPosition(const char * pinstrumentid,double price)
{
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);
	int p_margin = (this->GetInitialMoney()*0.08)/( price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
	return position;
}

void TradeTest::UpdateStopPrice(TestRecordData *tr,MinuteLineData *mindata){
//	DateUtil dateutil;
//	time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata->MinuteTime);
	//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
	// 更新持仓后的历史新低
	MysqlTestRecord mysqltestrecord;
	if (tr->Direction==0 && mindata->price> tr->Highest && mindata->price > tr->OpenPrice)		 {
		 tr->Highest=mindata->price;
		 tr->StopPrice = tr->Highest -this->GetStopRange();

		 printf("	%s  开仓日   更新最高价   -------  %.3f   stoprange[%.2f]   止损价  %.3f \n",mindata->MinuteTime, tr->Highest,this->GetStopRange(),tr->StopPrice );
		 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),mindata->price,tr->StopPrice,0);

	}
	else if (tr->Direction==1 && mindata->price < tr->Lowest && mindata->price < tr->OpenPrice)		{
		 tr->Lowest=mindata->price;
		 tr->StopPrice = tr->Lowest + this->GetStopRange();
		 printf("	%s 开仓日   更新最低价   -------%.3f     stoprange[%.2f]   止损价  %.3f\n",mindata->MinuteTime,tr->Lowest,this->GetStopRange(),tr->StopPrice );

		 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),mindata->price,tr->StopPrice,0);
	}
}

void TradeTest::UpdateStopWinPrice(TestRecordData *tr,MinuteLineData *mindata){
//	DateUtil dateutil;
//	time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata->MinuteTime);
	//			 printf("	当日有开仓,持仓中   ------- 最高价: %.3f \n",tr.Highest);
	// 更新持仓后的历史新低
	MysqlTestRecord mysqltestrecord;
	if (tr->Direction==1 && mindata->price> tr->Highest && mindata->price > tr->OpenPrice)		 {
		 tr->Highest=mindata->price;
		 tr->WinPrice = tr->Highest -this->GetStopRange();

		 printf("	%s  开仓日   更新最高价   -------  %.3f   stoprange[%.2f]   止盈价  %.3f \n",mindata->MinuteTime, tr->Highest,this->GetStopRange(),tr->WinPrice );
		 mysqltestrecord.UpdateHighStopWinPriceByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),mindata->price,tr->WinPrice,0);

	}
	else if (tr->Direction==0 && mindata->price < tr->Lowest && mindata->price < tr->OpenPrice)		{
		 tr->Lowest=mindata->price;
		 tr->WinPrice = tr->Lowest + this->GetStopRange();
		 printf("	%s 开仓日   更新最低价   -------%.3f     stoprange[%.2f]   止盈价  %.3f\n",mindata->MinuteTime,tr->Lowest,this->GetStopRange(),tr->WinPrice );

		 mysqltestrecord.UpdateLowStopWinPriceByInstrumentStatus(this->GetTableRecord().c_str(),
				 this->GetMainInstrumentID().c_str(),mindata->price,tr->WinPrice,0);
	}
}
void TradeTest::BuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal)
{

	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);

	tr.OpenPrice = mindata.price ;
	tr.Direction =0;
	tr.Status =0;

tr.Sedimentary = sed;
	tr.OpenOPI = mindata.OpenInterest;

	printf("open----> openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
			tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);


	tr.StopPrice = mindata.price  - this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
//	int position = min(p_stop,p_margin) ;
	int position = p_margin ;
	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission= tr.Volume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount;
	}
	else{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();


	if (tr.Volume >0 && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s ------position[%d]  openprice %.3f   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,tr.Volume,mindata.price, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else	{
		if(tr.Volume >0)	{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else
		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}

	}
}

void TradeTest::SellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal)
{

	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
//	tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
//	tr.pATR = atr_data.ATR/mindata.price;
//	tr.EMA50_X =ema_x50;
//	tr.EMA20_X =ema_x20;
//	tr.EMA9_X =ema_x9;
//	tr.EMA4_X =ema_x4;
//	tr.Margin=maincontractdata.LongMarginRatio;
//	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio;
//	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.OpenPrice = mindata.price ;
	tr.Direction =1;
	tr.Status =0;

//	tr.PreSettlePrice = predata.AveragePrice;
//	tr.DayOpenPrice = openprice;
	tr.OpenOPI = mindata.OpenInterest;
	tr.Sedimentary = sed;
//					printf("open----> openopi   %d    min.opi   %d    sed %.3f \n",tr.OpenOPI ,mindata.OpenInterest,sed);
	printf("open----> openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
			tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);

	tr.StopPrice = mindata.price  + this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price - this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);


	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;

	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
//	int position = min(p_stop,p_margin) ;
	int position = p_margin ;
	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)
	{
		tr.Commission=tr.Volume* mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount;
	}
	else
	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	if (tr.Volume >0  && !this->GetOpenFlag())
	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);
		printf("	%s:开空[%s]   -------%s  ------position[%d]   openprice %.3f   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,tr.Volume,mindata.price, tr.StopPrice,tr.Commission,sed);
//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else
	{
		if(tr.Volume >0)
		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else
		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}
	}
}
void TradeTest::BuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data)
{

	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);

	tr.OpenPrice = mindata.price ;
	tr.Direction =0;
	tr.Status =0;

tr.Sedimentary = sed;
	tr.OpenOPI = mindata.OpenInterest;

	printf("open----> openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
			tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);


	tr.StopPrice = mindata.price  - this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;
	tr.pATR = data->pATR;
	tr.Open_VolOpiRate=data->Open_VolOpiRate;
		tr.Open_AddOPIRate=data->Open_AddOPIRate;
		tr.DayOpenPrice = data->DayOpenPrice;

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
//	int position = min(p_stop,p_margin) ;
	int position = p_margin ;
	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission= tr.Volume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount;
	}
	else{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();


	if (tr.Volume >0 && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s ------position[%d]  openprice %.3f   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,tr.Volume,mindata.price, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else	{
		if(tr.Volume >0)	{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else
		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}

	}
}

void TradeTest::SellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data)
{

	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
//	tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
//	tr.pATR = atr_data.ATR/mindata.price;
//	tr.EMA50_X =ema_x50;
//	tr.EMA20_X =ema_x20;
//	tr.EMA9_X =ema_x9;
//	tr.EMA4_X =ema_x4;
//	tr.Margin=maincontractdata.LongMarginRatio;
//	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio;
//	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.OpenPrice = mindata.price ;
	tr.Direction =1;
	tr.Status =0;

//	tr.PreSettlePrice = predata.AveragePrice;
//	tr.DayOpenPrice = openprice;
	tr.OpenOPI = mindata.OpenInterest;
	tr.Sedimentary = sed;
//					printf("open----> openopi   %d    min.opi   %d    sed %.3f \n",tr.OpenOPI ,mindata.OpenInterest,sed);
	printf("open----> openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
			tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);

	tr.StopPrice = mindata.price  + this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price - this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);


	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;

	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;
	tr.pATR = data->pATR;
	tr.Open_VolOpiRate=data->Open_VolOpiRate;
	tr.Open_AddOPIRate=data->Open_AddOPIRate;
	tr.DayOpenPrice = data->DayOpenPrice;
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
//	int position = min(p_stop,p_margin) ;
	int position = p_margin ;
	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)
	{
		tr.Commission=tr.Volume* mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount;
	}
	else
	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	if (tr.Volume >0  && !this->GetOpenFlag())
	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);
		printf("	%s:开空[%s]   -------%s  ------position[%d]   openprice %.3f   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,tr.Volume,mindata.price, tr.StopPrice,tr.Commission,sed);
//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else
	{
		if(tr.Volume >0)
		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else
		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}
	}
}
void TradeTest::DayBuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
	tr.OpenPrice = mindata.price ;
	tr.Direction =0;
	tr.Status =0;

	tr.OpenOPI = mindata.OpenInterest;

	printf("open----> openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
					tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);


	tr.StopPrice = mindata.price  -this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;

	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission= tr.Volume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();


	if (tr.Volume >0 && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price,tr.Volume, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else
	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}

	}
}

void TradeTest::DaySellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);
	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
//	tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
//	tr.pATR = atr_data.ATR/mindata.price;
//	tr.EMA50_X =ema_x50;
//	tr.EMA20_X =ema_x20;
//	tr.EMA9_X =ema_x9;
//	tr.EMA4_X =ema_x4;
//	tr.Margin=maincontractdata.LongMarginRatio;
//	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio;
//	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.OpenPrice = mindata.price ;
	tr.Direction =1;
	tr.Status =0;

//	tr.PreSettlePrice = predata.AveragePrice;
//	tr.DayOpenPrice = openprice;
	tr.OpenOPI = mindata.OpenInterest;

//					printf("open----> openopi   %d    min.opi   %d    sed %.3f \n",tr.OpenOPI ,mindata.OpenInterest,sed);


	tr.StopPrice = mindata.price  +this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price -  this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);


	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;

	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;
	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission=tr.Volume* mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	if (tr.Volume >0  && !this->GetOpenFlag())	{

		printf("	%s:开空[%s]   %s-------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,tr.Section,opensignal,mindata.MinuteTime,mindata.price, tr.Volume,tr.StopPrice,tr.Commission,sed);

		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);
//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}
	}
}
void TradeTest::Close(TestRecordData tr,MinuteLineData mindata,const char* closesignal)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(tr.InstrumentID).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	int closevolume = tr.Volume - tr.CloseVolume;
	double closeaveprice = (tr.ClosePrice * tr.CloseVolume + mindata.price*closevolume)/(tr.Volume);

	tr.ClosePrice = closeaveprice;
	tr.CloseVolume= tr.Volume;


	 MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);
	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission+= closevolume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.CloseAccount;
	}
	else	{
		tr.Commission +=closevolume*commission.CloseNumber;
	}

	 strcpy(tr.CloseDateTime,mindata.MinuteTime);

//	 printf("close   %s  %s \n",tr.CloseDateTime,mindata.MinuteTime);
	 tr.Status =1;
	 tr.CloseOPI = mindata.OpenInterest;
	 if (tr.Direction ==0)	 {
		 double p = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *(double)tr.Volume;
		 tr.Profit = p-tr.Commission;
		 printf("	profit   交易单位:%d volume:%d  close:%.2f  open:%.2f  p:%.2f   commission:%.2f CloseAccount:%.6f  profit:%.2f    \n",
				 maincontractdata.VolumeMultiple,tr.Volume,tr.ClosePrice,tr.OpenPrice,p,tr.Commission,commission.CloseAccount,tr.Profit);
	 }
	 else if (tr.Direction ==1)	 {
		 double p = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - tr.ClosePrice ) *(double)tr.Volume;
		 tr.Profit = p-tr.Commission;
		 printf("	profit   交易单位:%d volume:%d open:%.2f close:%.2f   p:%.2f   commission:%.2f  CloseAccount:%.6f   profit:%.2f    \n",
				 maincontractdata.VolumeMultiple,tr.Volume,tr.OpenPrice,tr.ClosePrice,p,tr.Commission,commission.CloseAccount,tr.Profit);
	 }

	 strcpy(tr.CloseSignal,closesignal);
	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(GetTableRecord().c_str(),tr.InstrumentID,0))	 {

		 printf("close	%s:平仓单[%s] [%d]  -------平仓日期:%s  平仓价 %.3f  平仓均价: %.3f unit: %d   margin: %.3f profit: %.3f \n",tr.InstrumentID,closesignal,
				 closevolume,tr.CloseDateTime,mindata.price,tr.ClosePrice,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
		 mysqltestrecord.UpdatebyCloseInstrument(GetTableRecord().c_str(),tr.InstrumentID,&tr);
		 this->SetCloseFlag(true);
	 }
}

void TradeTest::bvDayBuyOpen(MinuteLineData mindata,const char * pinstrumentid,
		const char* opensignal,TestRecordData *data,double openprice)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
	tr.OpenPrice = openprice;
	tr.Direction =0;
	tr.Status =0;

	tr.OpenOPI = data->OpenOPI;
//	tr.OpenOPI = mindata.OpenInterest;



	tr.StopPrice = mindata.price  -this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;
	tr.DayOpenPrice = data->DayOpenPrice;
	tr.pATR = data->pATR;
		tr.Open_VolOpiRate=data->Open_VolOpiRate;
		tr.Open_AddOPIRate=data->Open_AddOPIRate;
		 tr.Close_AddOPIRate= data->Close_AddOPIRate;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;
strcpy(tr.FiveDayLineType,data->FiveDayLineType);
tr.PreSettlePrice = data->PreSettlePrice;
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;

	printf("	%s:  止损价开仓手数:%d    ---> 初始资金:%.2f   RiskRate:%.2f   止损价差:%.2f  交易单位:%d   \n",
						tr.InstrumentID,p_stop,this->GetInitialMoney(),this->GetRiskRate(),this->GetStopRange(),maincontractdata.VolumeMultiple);
	printf("	%s:  单手保证金限制开仓手数:%d  \n",
					tr.InstrumentID,p_margin);
	printf("open---->%s openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  Margin %.3f sed %.3f \n",mindata.MinuteTime,
						tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , commission.Margin,sed);

	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission= tr.Volume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();


	if (tr.Volume >0 && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price,tr.Volume, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else
	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}

	}
}

void TradeTest::bvDaySellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data,double openprice)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);
	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
//	tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
//	tr.pATR = atr_data.ATR/mindata.price;
//	tr.EMA50_X =ema_x50;
//	tr.EMA20_X =ema_x20;
//	tr.EMA9_X =ema_x9;
//	tr.EMA4_X =ema_x4;
//	tr.Margin=maincontractdata.LongMarginRatio;
//	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio;
//	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.OpenPrice = openprice ;
	tr.Direction =1;
	tr.Status =0;

//	tr.PreSettlePrice = predata.AveragePrice;
//	tr.DayOpenPrice = openprice;
	tr.OpenOPI = data->OpenOPI;

//	printf("open----> openopi   %d    min.opi   %d    sed %.3f \n",tr.OpenOPI ,mindata.OpenInterest,sed);


	tr.StopPrice = mindata.price  + this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price -  this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);


	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;
	tr.DayOpenPrice = data->DayOpenPrice;
	tr.pATR = data->pATR;
	tr.Open_VolOpiRate=data->Open_VolOpiRate;
	tr.Open_AddOPIRate=data->Open_AddOPIRate;
	 tr.Close_AddOPIRate= data->Close_AddOPIRate;
	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;

	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;
	strcpy(tr.FiveDayLineType,data->FiveDayLineType);
	tr.PreSettlePrice = data->PreSettlePrice;
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;
	tr.Volume =position;

	printf("	%s:  止损价开仓手数:%d    ---> 初始资金:%.2f   RiskRate:%.2f   止损价差:%.2f  交易单位:%d   \n",
						tr.InstrumentID,p_stop,this->GetInitialMoney(),this->GetRiskRate(),this->GetStopRange(),maincontractdata.VolumeMultiple);
	printf("	%s:  单手保证金限制开仓手数:%d  \n",
					tr.InstrumentID,p_margin);
	printf("open---->%s openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  Margin %.3f sed %.3f \n",mindata.MinuteTime,
						tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , commission.Margin,sed);

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission=tr.Volume* mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	if (tr.Volume >0  && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);
		printf("	%s:开空[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price, tr.Volume,tr.StopPrice,tr.Commission,sed);
//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}
	}
}
void TradeTest::bvClose(TestRecordData tr,MinuteLineData mindata,const char* closesignal,double closeprice)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(tr.InstrumentID).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double bv_closeprice=closeprice;
	int closevolume = tr.Volume - tr.CloseVolume;
	double closeaveprice = (tr.ClosePrice * tr.CloseVolume +bv_closeprice*closevolume)/(tr.Volume);

	tr.ClosePrice = closeaveprice;
	tr.CloseVolume= tr.Volume;


	 MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);
	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission+= closevolume*closeaveprice * (double)maincontractdata.VolumeMultiple * commission.CloseAccount;
	}
	else	{
		tr.Commission +=closevolume*commission.CloseNumber;
	}

	 strcpy(tr.CloseDateTime,mindata.MinuteTime);

//	 printf("close   %s  %s \n",tr.CloseDateTime,mindata.MinuteTime);
	 tr.Status =1;
	 tr.CloseOPI = mindata.OpenInterest;
	 if (tr.Direction ==0)	 {
		 double p = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *(double)tr.Volume;
		 tr.Profit = p-tr.Commission;
		 printf("	profit   交易单位:%d volume:%d  close:%.2f  open:%.2f  p:%.2f   commission:%.2f CloseAccount:%.6f  profit:%.2f    \n",
				 maincontractdata.VolumeMultiple,tr.Volume,tr.ClosePrice,tr.OpenPrice,p,tr.Commission,commission.CloseAccount,tr.Profit);
	 }
	 else if (tr.Direction ==1)	 {
		 double p = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - tr.ClosePrice ) *(double)tr.Volume;
		 tr.Profit = p-tr.Commission;
		 printf("	profit   交易单位:%d volume:%d open:%.2f close:%.2f   p:%.2f   commission:%.2f  CloseAccount:%.6f   profit:%.2f    \n",
				 maincontractdata.VolumeMultiple,tr.Volume,tr.OpenPrice,tr.ClosePrice,p,tr.Commission,commission.CloseAccount,tr.Profit);
	 }

	 strcpy(tr.CloseSignal,closesignal);
	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(GetTableRecord().c_str(),tr.InstrumentID,0))	 {

		 printf("close	%s:平仓单[%s] [%d]  -------平仓日期:%s  平仓价 %.3f  平仓均价: %.3f unit: %d   margin: %.3f profit: %.3f \n",tr.InstrumentID,closesignal,
				 closevolume,tr.CloseDateTime,mindata.price,tr.ClosePrice,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
		 mysqltestrecord.UpdatebyCloseInstrument(GetTableRecord().c_str(),tr.InstrumentID,&tr);
		 this->SetCloseFlag(true);
	 }
}
void TradeTest::DayBuyOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
	tr.OpenPrice = mindata.price ;
	tr.Direction =0;
	tr.Status =0;

	tr.OpenOPI = data->OpenOPI;
//	tr.OpenOPI = mindata.OpenInterest;



	tr.StopPrice = mindata.price  -this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;
	tr.DayOpenPrice = data->DayOpenPrice;
	tr.pATR = data->pATR;
		tr.Open_VolOpiRate=data->Open_VolOpiRate;
		tr.Open_AddOPIRate=data->Open_AddOPIRate;
		 tr.Close_AddOPIRate= data->Close_AddOPIRate;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;
strcpy(tr.FiveDayLineType,data->FiveDayLineType);

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;

	printf("	%s:  止损价开仓手数:%d    ---> 初始资金:%.2f   RiskRate:%.2f   止损价差:%.2f  交易单位:%d   \n",
						tr.InstrumentID,p_stop,this->GetInitialMoney(),this->GetRiskRate(),this->GetStopRange(),maincontractdata.VolumeMultiple);
	printf("	%s:  单手保证金限制开仓手数:%d  \n",
					tr.InstrumentID,p_margin);
	printf("open---->%s openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  Margin %.3f sed %.3f \n",mindata.MinuteTime,
						tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , commission.Margin,sed);

	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission= tr.Volume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();


	if (tr.Volume >0 && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price,tr.Volume, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else
	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}

	}
}
void TradeTest::DaySellOpen(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);
	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
//	tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
//	tr.pATR = atr_data.ATR/mindata.price;
//	tr.EMA50_X =ema_x50;
//	tr.EMA20_X =ema_x20;
//	tr.EMA9_X =ema_x9;
//	tr.EMA4_X =ema_x4;
//	tr.Margin=maincontractdata.LongMarginRatio;
//	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio;
//	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.OpenPrice = mindata.price ;
	tr.Direction =1;
	tr.Status =0;

//	tr.PreSettlePrice = predata.AveragePrice;
//	tr.DayOpenPrice = openprice;
	tr.OpenOPI = data->OpenOPI;

//	printf("open----> openopi   %d    min.opi   %d    sed %.3f \n",tr.OpenOPI ,mindata.OpenInterest,sed);


	tr.StopPrice = mindata.price  + this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price -  this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);


	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;
	tr.DayOpenPrice = data->DayOpenPrice;
	tr.pATR = data->pATR;
	tr.Open_VolOpiRate=data->Open_VolOpiRate;
	tr.Open_AddOPIRate=data->Open_AddOPIRate;
	 tr.Close_AddOPIRate= data->Close_AddOPIRate;
	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;

	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;
	strcpy(tr.FiveDayLineType,data->FiveDayLineType);
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;
	tr.Volume =position;

	printf("	%s:  止损价开仓手数:%d    ---> 初始资金:%.2f   RiskRate:%.2f   止损价差:%.2f  交易单位:%d   \n",
						tr.InstrumentID,p_stop,this->GetInitialMoney(),this->GetRiskRate(),this->GetStopRange(),maincontractdata.VolumeMultiple);
	printf("	%s:  单手保证金限制开仓手数:%d  \n",
					tr.InstrumentID,p_margin);
	printf("open---->%s openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  Margin %.3f sed %.3f \n",mindata.MinuteTime,
						tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , commission.Margin,sed);

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission=tr.Volume* mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	if (tr.Volume >0  && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);
		printf("	%s:开空[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price, tr.Volume,tr.StopPrice,tr.Commission,sed);
//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}
	}
}
void TradeTest::Buy(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);

	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
	tr.OpenPrice = mindata.price ;
	tr.Direction =0;
	tr.Status =0;

	tr.OpenOPI = data->OpenOPI;
//	tr.OpenOPI = mindata.OpenInterest;



	tr.StopPrice = mindata.price  -this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;
	tr.DayOpenPrice = data->DayOpenPrice;
	tr.pATR = data->pATR;
		tr.Open_VolOpiRate=data->Open_VolOpiRate;
		tr.Open_AddOPIRate=data->Open_AddOPIRate;
		 tr.Close_AddOPIRate= data->Close_AddOPIRate;

	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;
strcpy(tr.FiveDayLineType,data->FiveDayLineType);

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
//	int position = min(p_stop,p_margin) ;
	int position = p_stop ;

	printf("	%s:  止损价开仓手数:%d    ---> 初始资金:%.2f   RiskRate:%.2f   止损价差:%.2f  交易单位:%d   \n",
						tr.InstrumentID,p_stop,this->GetInitialMoney(),this->GetRiskRate(),this->GetStopRange(),maincontractdata.VolumeMultiple);
	printf("	%s:  单手保证金限制开仓手数:%d  \n",
					tr.InstrumentID,p_margin);
	printf("open---->%s openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  Margin %.3f sed %.3f \n",mindata.MinuteTime,
						tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , commission.Margin,sed);

	tr.Volume =position;

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission= tr.Volume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();


	if (tr.Volume >0 && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price,tr.Volume, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else
	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}

	}
}
void TradeTest::Sell(MinuteLineData mindata,const char * pinstrumentid,const char* opensignal,TestRecordData *data)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	strcpy(tr.Section,codeutil.GetSectionByInstrumentid(pinstrumentid).c_str());
	std::string opendate;
	opendate = mindata.MinuteTime;
	char buf[20];
	sprintf(buf,"%s%s%s",opendate.substr(0,4).c_str(),opendate.substr(5,2).c_str(),opendate.substr(8,2).c_str());
	strcpy(tr.OpenDate,buf);

	strcpy(tr.InstrumentID,pinstrumentid);
	strcpy(tr.InstrumentID,pinstrumentid);
	char id[60];
	sprintf(id,"%s_%s",tr.OpenDateTime,tr.InstrumentID);
strcpy(tr.ID,id);
//	tr.AverageUpDownrate =hissectiondata.AverageUpDownrate;
//	tr.pATR = atr_data.ATR/mindata.price;
//	tr.EMA50_X =ema_x50;
//	tr.EMA20_X =ema_x20;
//	tr.EMA9_X =ema_x9;
//	tr.EMA4_X =ema_x4;
//	tr.Margin=maincontractdata.LongMarginRatio;
//	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *maincontractdata.LongMarginRatio;
//	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.OpenPrice = mindata.price ;
	tr.Direction =1;
	tr.Status =0;

//	tr.PreSettlePrice = predata.AveragePrice;
//	tr.DayOpenPrice = openprice;
	tr.OpenOPI = data->OpenOPI;

//	printf("open----> openopi   %d    min.opi   %d    sed %.3f \n",tr.OpenOPI ,mindata.OpenInterest,sed);


	tr.StopPrice = mindata.price  + this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price -  this->GetWinRatio()*this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);


	tr.EMA50=0;
	tr.EMA20=0;
	tr.EMA9=0;
	tr.EMA4=0;
	tr.DayOpenPrice = data->DayOpenPrice;
	tr.pATR = data->pATR;
	tr.Open_VolOpiRate=data->Open_VolOpiRate;
	tr.Open_AddOPIRate=data->Open_AddOPIRate;
	 tr.Close_AddOPIRate= data->Close_AddOPIRate;
	tr.EMA50_X=0;
	tr.EMA20_X=0;
	tr.EMA9_X=0;
	tr.EMA4_X=0;

	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;
	strcpy(tr.FiveDayLineType,data->FiveDayLineType);
	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*this->GetSingleMarginRate())/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
//	int position = min(p_stop,p_margin) ;
	int position = p_stop ;
	tr.Volume =position;

	printf("	%s:  止损价开仓手数:%d    ---> 初始资金:%.2f   RiskRate:%.2f   止损价差:%.2f  交易单位:%d   \n",
						tr.InstrumentID,p_stop,this->GetInitialMoney(),this->GetRiskRate(),this->GetStopRange(),maincontractdata.VolumeMultiple);
	printf("	%s:  单手保证金限制开仓手数:%d  \n",
					tr.InstrumentID,p_margin);
	printf("open---->%s openopi   %d    min.opi   %d   price  %.3f  VolumeMultiple %d  Margin %.3f sed %.3f \n",mindata.MinuteTime,
						tr.OpenOPI ,mindata.OpenInterest,mindata.price,maincontractdata.VolumeMultiple , commission.Margin,sed);

	if(strcmp(commission.Mode,"account")==0)	{
		tr.Commission=tr.Volume* mindata.price * (double)maincontractdata.VolumeMultiple * commission.OpenAccount/10;
	}
	else	{
		tr.Commission =tr.Volume*commission.OpenNumber;
	}



	tr.Margin=commission.Margin;
	tr.UseMargin = position*mindata.price * (double)maincontractdata.VolumeMultiple *commission.Margin;
	tr.UseMarginRate = tr.UseMargin/ this->GetInitialMoney();

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	if (tr.Volume >0  && !this->GetOpenFlag())	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);
		printf("	%s:开空[%s]   -------%s  openprice %.3f   手数:%d   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,mindata.price, tr.Volume,tr.StopPrice,tr.Commission,sed);
//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);
		this->SetOpenFlag(true);
	}
	else	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d <=0   沉淀资金  %.2f(持仓量 %d)  \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d   !!! \n",pinstrumentid,tr.Volume);
		}
	}
}
void TradeTest::CloseAdjust(TestRecordData tr,MinuteLineData mindata,const char* closesignal,int closevolume)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(tr.InstrumentID).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double closeaveprice = (tr.ClosePrice * tr.CloseVolume + mindata.price*closevolume)/(tr.CloseVolume+closevolume);

	tr.ClosePrice = closeaveprice;
	 tr.CloseVolume=tr.CloseVolume+closevolume;

	 strcpy(tr.CloseDateTime,mindata.MinuteTime);
	 if(tr.CloseVolume < tr.Volume)	 {
		 tr.Status =0;
	 }
	 else {
		 tr.Status =1;
	 }

	 tr.CloseOPI = mindata.OpenInterest;

	 MysqlCommission mysqlcommission;
		Commission commission=mysqlcommission.Find_DatabyProductID(productid);
		if(strcmp(commission.Mode,"account")==0)	{
			tr.Commission+= closevolume*mindata.price * (double)maincontractdata.VolumeMultiple * commission.CloseAccount;
		}
		else	{
			tr.Commission +=closevolume*commission.CloseNumber;
		}


	 if (tr.Direction ==0) {
		 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.CloseVolume;
	 }
	 else if (tr.Direction ==1) {
		 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - tr.ClosePrice ) *tr.CloseVolume;
	 }

	 strcpy(tr.CloseSignal,closesignal);
	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(GetTableRecord().c_str(),tr.InstrumentID,0))	 {
		 mysqltestrecord.UpdatebyInstrument(GetTableRecord().c_str(),tr.InstrumentID,tr);
		 printf("	%s:调仓信号[%s]   本次减仓目标手数[%d]-------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",tr.InstrumentID,closesignal,
				 closevolume, mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
		 this->SetCloseFlag(true);
	 }
}

void TradeTest::HalfCloseAdjust(TestRecordData tr,MinuteLineData mindata,const char* closesignal,int closevolume)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(tr.InstrumentID).c_str());
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double closeaveprice = (tr.ClosePrice * tr.CloseVolume + mindata.price*closevolume)/(tr.CloseVolume+closevolume);

	tr.ClosePrice = closeaveprice;
	 tr.CloseVolume=tr.CloseVolume+closevolume;
	 tr.HalfStop=true;

	 strcpy(tr.CloseDateTime,mindata.MinuteTime);
	 if(tr.CloseVolume < tr.Volume)	 {
		 tr.Status =0;
	 }
	 else	 {
		 tr.Status =1;
	 }

	 tr.CloseOPI = mindata.OpenInterest;
	 if (tr.Direction ==0)	 {
		 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.ClosePrice - tr.OpenPrice) *tr.CloseVolume;
	 }
	 else if (tr.Direction ==1)	 {
		 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - tr.ClosePrice ) *tr.CloseVolume;
	 }

	 strcpy(tr.CloseSignal,closesignal);
	 if(mysqltestrecord.Exist_RecordByInstrumentStatus(GetTableRecord().c_str(),tr.InstrumentID,0))	 {
		 mysqltestrecord.UpdatebyInstrument(GetTableRecord().c_str(),tr.InstrumentID,tr);
		 printf("	%s:接近止损价半价调仓[%s]   本次减仓目标手数[%d]-------%s  ClosePrice %.3f unit: %d   margin: %.3f profit: %.3f \n",tr.InstrumentID,closesignal,
				 closevolume, mindata.MinuteTime,mindata.price,maincontractdata.VolumeMultiple,maincontractdata.LongMarginRatio,tr.Profit);
		 this->SetCloseFlag(true);
	 }
}
void TradeTest::SingleTestReport(const char* tablename,const char* table_report)
{
	MysqlTestRecord mysqltestrecord;
	MysqlTestReport mysqltestreport;
	MysqlMaincontract mysqlmaincontract;
	TestReportData testreport;
	std::string temp = tablename;

	std::string prefix=this->GetYear();
	prefix.append("_");
	prefix.append(this->GetStrategy());


	strcpy(testreport.Product,this->GetProductID().c_str());

//	std::string table_report ="v1";
	if(!mysqltestreport.Exist_Table(table_report))		{
			mysqltestreport.CreateTable(table_report);
		}


	vector<TestRecordData>  list;
	if (mysqltestrecord.Exist_Table(tablename))	{

		printf("SingleTestReport 汇总[%s]单合约交易数据---->  %s \n",tablename,testreport.Product);
		mysqltestrecord.Find_AllLists(list,tablename);
			double totalprofit=0;
			double totalcommission=0;
			double winprofit=0;
			double loseprofit=0;
			double avewin=0;
			double avelost=0;
			double winrate=0;
			double winlostrate=0;

			int tradecount=0;
			int wincount=0;
			int longcount=0;
			int shortcount=0;
			int wincount_long=0;
			int wincount_short=0;

			double exp=0;
			for (auto &item:list)	{

				if(item.Direction==0 && item.Status==1)		{
					longcount++;
					if(item.Profit>0)		{
						wincount_long++;
					}
				}
				else if(item.Direction==1 && item.Status==1)	{
					shortcount++;
					if(item.Profit>0)	{
						wincount_short++;
					}
				}
				totalprofit+=item.Profit;
				totalcommission+=item.Commission;

				if(item.Status==1)	{
					tradecount++;
					if(item.Profit>0 )	{
						wincount++;
						winprofit+=item.Profit;
					}
					else	{
						loseprofit+=item.Profit;
					}
				}

			}

			if (wincount >0){
				avewin = winprofit/(double)wincount;
			}
			else{
				avewin =0;
			}

			if ((double)(tradecount-wincount) >0)	{
				avelost = loseprofit/(double)(tradecount-wincount);
			}
			else	{
				avelost=0;
			}

			if (tradecount>0)	{
				winrate=(double)wincount/(double)tradecount;
			}
			if (abs(avelost)>0)	{
				winlostrate =avewin/abs(avelost);
			}
			else	{
				winlostrate =0;
			}


			exp=avewin*winrate +avelost*(1-winrate);
			printf("合约[%s]测试汇总 -->交易次数: %d 总亏损:%.2f 总盈利:%.2f 平均亏损:%.2f 平均盈利:%.2f 佣金:%.2f 最终收益: %.2f  胜率:%.2f%%  盈亏比: %.2f  期望值: %.2f\n",
					tablename,tradecount,loseprofit,winprofit,avelost,avewin,totalcommission,totalprofit,winrate*100,winlostrate,exp);

			testreport.TotalWinProfit = winprofit;
			testreport.TotalLoseProfit = loseprofit;
			testreport.TradeCount =tradecount;
			testreport.WinCount = wincount;

			testreport.LongCount=longcount;
			testreport.ShortCount=shortcount;
			testreport.WinCount_Long=wincount_long;
			testreport.WinCount_Short=wincount_short;

			testreport.AverageWinProfit = avewin;
			testreport.AverageLoseProfit = avelost;
			testreport.WinRate = winrate;
			testreport.Expectation = exp;

			if (abs(avelost)>0)		{
				testreport.WinLoseRate = winlostrate;
			}
			else	{
				testreport.WinLoseRate = 0;
			}

			testreport.Asset = totalprofit;
			testreport.Commission =totalcommission;
			testreport.InitialMoney =this->GetInitialMoney() ;

			sprintf(testreport.StartTestTime,"%s-01-01 00:00:00",this->GetYear().c_str());
			sprintf(testreport.EndTestTime,"%s-12-30 00:00:00",this->GetYear().c_str());
			strcpy(testreport.Strategy,"cycle_ema");


			MainContractData mcdata = mysqlmaincontract.Find_DatabyProductID(testreport.Product);
			strcpy(testreport.Section,mcdata.Section);

			if (!mysqltestreport.Exist_DatabyProductID(table_report,testreport.Product))	{
				printf("不存在测试报告  插入测试汇总数据    testreport %s   mc:%s\n",testreport.Section,mcdata.Section);
				mysqltestreport.Insert(table_report,testreport);
				testreport.ErrorMinuteData =0;
				mysqltestreport.UpdateErrorData(table_report,testreport);
			}
			else	{

				mysqltestreport.Update(table_report,testreport);
				printf("已经存在测试报告  更新测试报告数据  testreport %s   mc:%s\n",testreport.Section,mcdata.Section);
			}
	}

	vector<TestRecordData>().swap(list);
}
void TradeTest::TestReport()
{

	vector<MainContractData>  list;
	MysqlMaincontract mysqlmaincontract;
	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)	{

		std::string table_testrecord;
		table_testrecord=this->GetYear();
		table_testrecord.append("_");
		table_testrecord.append(this->GetStrategy());
		table_testrecord.append(item.ProductID);
		this->SetProductID(item.ProductID);
			printf("TradeTest --->TestReport    %s\n",item.ProductID);
		SingleTestReport(table_testrecord.c_str(),this->GetTableReport().c_str());
	}

	vector<MainContractData>().swap(list);
}
int TradeTest::process_GetPosition(){

	MysqlDayLine mysqldayline;

	int vm =0;
	if ( this->GetVolumeMultiple()==0){
			MysqlMaincontract mysqlmaincontract;

			char productid[10];
			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
			MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
			vm = maincontractdata.VolumeMultiple;
	}
	else{
		vm =  this->GetVolumeMultiple();
	}
	ATRData atr20_data = mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),20);
	int p= (this->GetInitialMoney()*this->GetRiskRate())/(atr20_data.ATR *this->GetStopRatio() * vm);
	return p;
}
void TradeTest::update_HighLow(MinuteLineData *mindata){

	 if(mindata->price>this->GetTodayHigh())		 {
		 this->SetTodayHigh(mindata->price);
//			 printf("   [%s]   highest  新高 %.2f    \n",mindata.MinuteTime,highest);
	 }

	 if(mindata->price<this->GetTodayLow())		 {
		 this->SetTodayLow(mindata->price);;
//			 printf("   [%s]   lowest  新低 %.2f    \n",mindata.MinuteTime,lowest);
	 }

}

bool TradeTest::condition_OpenTimebyMinTimeData(MinuteTimeData *mindata,int seconds)
{

	 std::string tmp_time = mindata->MinuteTime;
	char time[20];
	sprintf(time,"%s",tmp_time.substr(11,8).c_str());
	DateUtil dateutil;
	double Q_BarTime_2 =  dateutil.ConvertMinuteTimetoBarTime(time);
	bool night ;

	bool night2 =  Q_BarTime_2 > 0.000000 && Q_BarTime_2 < 0.023000;
	bool endtime = Q_BarTime_2 > 0.145500 && Q_BarTime_2 < 0.150000;
	bool day ;

	bool valid_opentime =false;
		CodeUtil codeutil;

	if(this->GetOpenTimeMode()==0){
		if (seconds==0){
				night=  Q_BarTime_2 > 0.210000 && Q_BarTime_2 <=0.235959;
				day= Q_BarTime_2 > 0.090000 && Q_BarTime_2 < 0.145500;
			}
			else if (seconds==5){
				night=  Q_BarTime_2 > 0.210500 && Q_BarTime_2 <=0.235959;
				day= Q_BarTime_2 > 0.090500 && Q_BarTime_2 < 0.145500;
			}
			else if (seconds==10){
					night=  Q_BarTime_2 > 0.211000 && Q_BarTime_2 <=0.235959;
					day= Q_BarTime_2 > 0.091000 && Q_BarTime_2 < 0.145500;
				}
			else if (seconds==20){
						night=  Q_BarTime_2 > 0.212000 && Q_BarTime_2 <=0.235959;
						day= Q_BarTime_2 > 0.092000 && Q_BarTime_2 < 0.145500;
					}
			else{
				night=  Q_BarTime_2 > 0.210000 && Q_BarTime_2 <=0.235959;
				day= Q_BarTime_2 > 0.090000 && Q_BarTime_2 < 0.145500;
			}

		if(codeutil.IsNightByProductid(this->GetProductID().c_str())  ){


			if ( night || night2 || day){
	//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
				valid_opentime=true;
			}
			else{
	//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
				valid_opentime=false;
			}
		}
		else if(!codeutil.IsNightByProductid(this->GetProductID().c_str())  ){
			if (day){
	//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
				valid_opentime=true;
			}
			else{
	//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
				valid_opentime=false;
			}
		}
	}
	else if(this->GetOpenTimeMode()==1){

		if(endtime){
			valid_opentime=true;
		}
		else{
			valid_opentime=false;
		}
	}

	return valid_opentime;
}
bool TradeTest::condition_EndTimebyMinTimeData(MinuteTimeData *mindata)
{
	 std::string tmp_time = mindata->MinuteTime;
	char time[20];
	sprintf(time,"%s",tmp_time.substr(11,8).c_str());
	DateUtil dateutil;
	double Q_BarTime_2 =  dateutil.ConvertMinuteTimetoBarTime(time);
	bool endtime = Q_BarTime_2 > 0.145500 && Q_BarTime_2 < 0.150000;
	bool valid_endtime;
	if(endtime){
		valid_endtime=true;
	}
	else{
		valid_endtime=false;
	}
	return valid_endtime;
}
bool TradeTest::condition_OpenTime(MinuteLineData *mindata,int seconds)
{

	 std::string tmp_time = mindata->MinuteTime;
	char time[20];
	sprintf(time,"%s",tmp_time.substr(11,8).c_str());
	DateUtil dateutil;
	double Q_BarTime_2 =  dateutil.ConvertMinuteTimetoBarTime(time);
	bool night ;

	bool night2 =  Q_BarTime_2 > 0.000000 && Q_BarTime_2 < 0.023000;
	bool endtime = Q_BarTime_2 > 0.145500 && Q_BarTime_2 < 0.150000;
	bool day ;

	bool valid_opentime =false;
		CodeUtil codeutil;

	if(this->GetOpenTimeMode()==0){
		if (seconds==0){
				night=  Q_BarTime_2 > 0.210000 && Q_BarTime_2 <=0.235959;
				day= Q_BarTime_2 > 0.090000 && Q_BarTime_2 < 0.150000;
			}
			else if (seconds==5){
				night=  Q_BarTime_2 > 0.210500 && Q_BarTime_2 <=0.235959;
				day= Q_BarTime_2 > 0.090500 && Q_BarTime_2 < 0.150000;
			}
			else if (seconds==10){
					night=  Q_BarTime_2 > 0.211000 && Q_BarTime_2 <=0.235959;
					day= Q_BarTime_2 > 0.091000 && Q_BarTime_2 < 0.150000;
				}
			else if (seconds==20){
						night=  Q_BarTime_2 > 0.212000 && Q_BarTime_2 <=0.235959;
						day= Q_BarTime_2 > 0.092000 && Q_BarTime_2 < 0.150000;
					}
			else{
				night=  Q_BarTime_2 > 0.210000 && Q_BarTime_2 <=0.235959;
				day= Q_BarTime_2 > 0.090000 && Q_BarTime_2 < 0.150000;
			}

		if(codeutil.IsNightByProductid(this->GetProductID().c_str())  ){


			if ( night || night2 || day){
	//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
				valid_opentime=true;
			}
			else{
	//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
				valid_opentime=false;
			}
		}
		else if(!codeutil.IsNightByProductid(this->GetProductID().c_str())  ){
			if (day){
	//						printf(" current_time %s   --> %s valid_opentime >>> true \n",mindata.MinuteTime,time);
				valid_opentime=true;
			}
			else{
	//						printf(" current_time %s   --> %s valid_opentime >>> false \n",mindata.MinuteTime,time);
				valid_opentime=false;
			}
		}
	}
	else if(this->GetOpenTimeMode()==1){

		if(endtime){
			valid_opentime=true;
		}
		else{
			valid_opentime=false;
		}
	}

	return valid_opentime;
}

bool TradeTest::condition_EndTime(MinuteLineData *mindata)
{
	 std::string tmp_time = mindata->MinuteTime;
	char time[20];
	sprintf(time,"%s",tmp_time.substr(11,8).c_str());
	DateUtil dateutil;
	double Q_BarTime_2 =  dateutil.ConvertMinuteTimetoBarTime(time);
	bool endtime = Q_BarTime_2 > 0.145500 && Q_BarTime_2 < 0.150000;
	bool valid_endtime;
	if(endtime){
		valid_endtime=true;
	}
	else{
		valid_endtime=false;
	}
	return valid_endtime;
}
double  TradeTest::zb_TR(double h,double l,double preclose)
{
	 double tr_highlow=abs(h-l);
	 double tr_highclose=abs(h-preclose);
	 double tr_lowclose=abs(l-preclose);

	double  tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
	return tr;
}

double  TradeTest::zb_TR(double preclose)
{
	 double tr_highlow=abs(this->GetTodayHigh()-this->GetTodayLow());
	 double tr_highclose=abs(this->GetTodayHigh()-preclose);
	 double tr_lowclose=abs(this->GetTodayLow()-preclose);

	double  tr=max(tr_highlow,max(tr_highclose,tr_lowclose));
	return tr;
}
double  TradeTest::zb_Sedimentary(MinuteLineData *mindata)
{

	 double sed=mindata->OpenInterest * mindata->price * (double) this->GetVolumeMultiple()* this->GetMargin() /100000000.0;
	 return sed;
}
double  TradeTest::zb_PreSettlePrice(const char* time)
{
	MysqlHisMinData mysqlhismindata;
//	MysqlMaincontract mysqlmaincontract;
//	char productid[10];
//	CodeUtil codeutil;
//	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
//	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);


	std::string last_tmp = time;
	char last_date[20];
	sprintf(last_date,"%s%s%s",last_tmp.substr(0,4).c_str(),last_tmp.substr(5,2).c_str(),last_tmp.substr(8,2).c_str());

//	std::cout<<"   last_date "<<last_date<<endl;
	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string last_tablename;
	last_tablename=this->GetMainInstrumentID().c_str();
	last_tablename.append("_");
	last_tablename.append(last_date);
	double presettleprice;
	if(!mysqlhismindata.Exist_Table(last_tablename.c_str())){
		printf("	--->[%s]   preday      no exist \n",time);
		presettleprice=0;
	}
	else{
		presettleprice = mysqlhismindata.GetSettlePrice(last_tablename.c_str(), this->GetVolumeMultiple());
	}

	return presettleprice;
}

int  TradeTest::zb_PreVolume(const char* time)
{
	MysqlHisMinData mysqlhismindata;
//	MysqlMaincontract mysqlmaincontract;
//	char productid[10];
//	CodeUtil codeutil;
//	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
//	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	std::string last_tmp = time;
	char last_date[20];
	sprintf(last_date,"%s%s%s",last_tmp.substr(0,4).c_str(),last_tmp.substr(5,2).c_str(),last_tmp.substr(8,2).c_str());

	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string last_tablename;
	last_tablename=this->GetMainInstrumentID().c_str();
	last_tablename.append("_");
	last_tablename.append(last_date);

	int total_volume;
	if(!mysqlhismindata.Exist_Table(last_tablename.c_str())){
		printf("	--->[%s]   preday      no exist \n",time);
		total_volume=0;
	}
	else{
		total_volume = mysqlhismindata.GetVolumeSum(last_tablename.c_str());
	}

	return total_volume;
}
int  TradeTest::zb_PreOpi(const char* time)
{
	MysqlHisMinData mysqlhismindata;
//	MysqlMaincontract mysqlmaincontract;
//	char productid[10];
//	CodeUtil codeutil;
//	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());
//	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	std::string last_tmp = time;
	char last_date[20];
	sprintf(last_date,"%s%s%s",last_tmp.substr(0,4).c_str(),last_tmp.substr(5,2).c_str(),last_tmp.substr(8,2).c_str());

	mysqlhismindata.Set_Database(this->GetDataBase_HisMindata());

	std::string last_tablename;
	last_tablename=this->GetMainInstrumentID().c_str();
	last_tablename.append("_");
	last_tablename.append(last_date);

	int total_opi;
	if(!mysqlhismindata.Exist_Table(last_tablename.c_str())){
		printf("	--->[%s]   preday      no exist \n",time);
		total_opi=0;
	}
	else{
		total_opi = mysqlhismindata.GetLastOpenInterest(last_tablename.c_str());
	}

	return total_opi;
}
HxDayLineData TradeTest::data_PreDay()
{
	MysqlDayLine mysqldayline;
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	return preday;
}
CycleData TradeTest::data_PreDynamicCycleData(const char* daytime)
{
	MysqlDayLine mysqldayline;
	int long_n =20;
	int short_n =10;
	ATRData preatr20_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),long_n);
	ATRData preatr10_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),short_n);
//	ATRData preatr5_data =mysqldayline.Collect_PreHisATR_NData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),5);
	int pre_n ;
	double pre_k ;
	if(preatr10_data.ATR >1.0 && preatr20_data.ATR >1.0){
		pre_k = preatr20_data.ATR/preatr10_data.ATR;
	}
	else	{
		pre_k=1.0;
	}
	pre_n=floor(this->GetN()*pre_k);
	CycleData precycledata =mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),daytime,pre_n);
	return precycledata;
}
double  TradeTest::type_FiveDays(vector<HxDayLineData>  &fivedays_list)
{

//	printf("	type_FiveDays begin \n");
double ret;
vector<HxDayLineData>  temp;


double total_up_price;
double total_down_price;
double updown_price;
double highest_oc;
double lowest_oc;
int i=0;

double five_open;
double five_close;
for (auto &data:fivedays_list)	{

	if(data.Open<data.Close){
		total_up_price+=data.Close-data.Open;
	}

	else if(data.Open>data.Close){
		total_down_price+=data.Close-data.Open;
	}

	if(i==0){
		five_open=data.Open;
	}

	five_close=data.Close;
//	if(i==0){
//
//		double daymax= max(data.Open,data.Close);
//		double daymin=min(data.Open,data.Close);
//		highest_oc=daymax;
//		lowest_oc=daymin;
//	}
//	else{
//
//		double daymax= max(data.Open,data.Close);
//		double daymin=min(data.Open,data.Close);
//		if(daymax>highest_oc){
//			highest_oc=daymax;
//		}
//
//		if(daymin<lowest_oc){
//			lowest_oc=daymin;
//		}
//	}

}
updown_price= five_close-five_open;

double dif = fabs(total_up_price+total_down_price);

double weight ;

if (updown_price!=0){
	weight= dif/fabs(updown_price);
}
else{
	weight=0;
}

printf("	5日涨跌幅:%.2f  5日内阳线长度:%.2f  5日内阴线长度:%.2f   weight:%.2f  \n",updown_price,total_up_price,total_down_price,weight);

vector<HxDayLineData>().swap(temp);
return weight;
}

double  TradeTest::type_FiveDaysATR(vector<HxDayLineData>  &fivedays_list,double atr)
{

	//	printf("	type_FiveDays begin \n");
	double ret;
	vector<HxDayLineData>  temp;
	double total_up_price;
	double total_down_price;
	double updown_price;
	double highest_oc;
	double lowest_oc;
	int i=0;

	double five_open;
	double five_close;
	for (auto &data:fivedays_list)	{

		if(data.Open<data.Close){
			total_up_price+=data.Close-data.Open;
		}

		else if(data.Open>data.Close){
			total_down_price+=data.Close-data.Open;
		}

		if(i==0){
			five_open=data.Open;
		}

		five_close=data.Close;
		if(i==0){

			double daymax= max(data.Open,data.Close);
			double daymin=min(data.Open,data.Close);
			highest_oc=daymax;
			lowest_oc=daymin;
		}
		else{

			double daymax= max(data.Open,data.Close);
			double daymin=min(data.Open,data.Close);
			if(daymax>highest_oc){
				highest_oc=daymax;
			}

			if(daymin<lowest_oc){
				lowest_oc=daymin;
			}
		}
		i++;

	}
	updown_price= five_close-five_open;

	//double dif = fabs(total_up_price+total_down_price);

	double weight ;

	if (updown_price!=0){
		weight= fabs(highest_oc-lowest_oc)/atr;

	//	weight= fabs(updown_price)/atr;
	}
	else{
		weight=0;
	}
	printf("	atr: %.2f  5日涨跌幅:%.2f  最高:%.2f  最低:%.2f   %.2f ----> weight:%.2f  \n",atr,updown_price,highest_oc,lowest_oc,highest_oc-lowest_oc,weight);

	//printf("	atr: %.2f  5日涨跌幅:%.2f  5日内阳线长度:%.2f  5日内阴线长度:%.2f   weight:%.2f  \n",atr,updown_price,total_up_price,total_down_price,weight);

	vector<HxDayLineData>().swap(temp);
	return weight;
}
double  TradeTest::zb_CCI(vector<HxDayLineData>  &list,double current_tp)
{
	//	printf("	zb_CCI begin \n");

	vector<double>  tplists;

	for (auto &day:list)	{
		double t = (day.High+day.Low+day.Close)/3.0;
		tplists.push_back(t);
	}

	tplists.push_back(current_tp);
	double sum=std::accumulate(tplists.begin(), tplists.end(), 0);
	double mean = sum/tplists.size();

	double accum  = 0.0;
	std::for_each (tplists.begin(), tplists.end(), [&](const double d) {
	    accum  += (d-mean)*(d-mean);
	});

	double stdev = sqrt(accum/(tplists.size()-1)); //方差
	double cci = (current_tp -mean)/(0.015*stdev);

//	printf("	cci: %.2f  current_tp: %.2f  mean:%.2f  stdev:%.2f   \n",cci,current_tp,mean,stdev);


	vector<double>().swap(tplists);
	return cci;
}
void TradeTest::StaticTestRecord()
{
	MysqlTestAsset mysqltestasset;
	std::string totaltablename;
	totaltablename.append(this->GetStrategy().c_str());
	totaltablename.append(this->GetYear().c_str());
	mysqltestasset.CreateTable(totaltablename.c_str());
	MysqlMaincontract mysqlmaincontract;
	MysqlTestRecord mysqltestrecord;
	vector<MainContractData>  list;

	mysqlmaincontract.Find_AllList(list);
	int i=0;
	for (auto &item:list)	{

		vector<TestRecordData>  ins_list;
		std::string tablename =this->GetTable_Record_Prefix();
		tablename.append(item.ProductID);
		if (mysqltestrecord.Exist_Table(tablename.c_str()))		{
			mysqltestrecord.Find_AllListsbyStatus(ins_list,tablename.c_str());

			for (auto &record:ins_list)			{
				TestAssetData data;
				std:string id=record.OpenDateTime;
				id.append("_");
				id.append(record.InstrumentID);

				strcpy(data.ID,id.c_str());
				strcpy(data.OpenDateTime,record.OpenDateTime);
				strcpy(data.CloseDateTime,record.CloseDateTime);
				strcpy(data.InstrumentID,record.InstrumentID);
				data.Direction=record.Direction;
				data.Status = record.Status;
				data.Profit = record.Profit;
				data.UseMarginRate = record.UseMarginRate;
				data.Commission = record.Commission;
				data.HighestAsset=0;
				data.BackDownRate=0;

				if(!mysqltestasset.Exist_DatabyID(totaltablename.c_str(),data.ID))			{
					mysqltestasset.Insert(totaltablename.c_str(),data);
				}
			}
		}

			vector<TestRecordData> ().swap(ins_list);
//		printf("MongoMainContract Find_All  end! ...\n");

	}

	vector<TestAssetData>  total_list;
	mysqltestasset.Find_AllListsbyCloseTime(total_list,totaltablename.c_str());
	for (auto &record:total_list)	{

		vector<TestAssetData>  sametime_list;
		mysqltestasset.Find_SameTimeHoldListbyTime(sametime_list,totaltablename.c_str(),record.OpenDateTime,record.CloseDateTime);
		double totalmarginrate=0;
		int i=0;
		int maxhold=5;
		for (auto &samehold_item:sametime_list)		{
//			printf("	开仓时间: %s    平仓时间: %s    同时开仓合约：%s----> [%s   %s   %.3f]\n",record.OpenDateTime,record.CloseDateTime,
//					samehold_item.InstrumentID,samehold_item.OpenDateTime,samehold_item.CloseDateTime,samehold_item.UseMarginRate);
			if(i<maxhold-1)			{
				printf("	编号: %d 同时开仓合约：%s----> [%s   %s   %.3f]\n",	i,samehold_item.InstrumentID,samehold_item.OpenDateTime,samehold_item.CloseDateTime,samehold_item.UseMarginRate);
				totalmarginrate+=samehold_item.UseMarginRate;
			}
			else			{
				printf("	编号: %d 删除合约：%s----> [%s   %s   %.3f]\n",	i,samehold_item.InstrumentID,samehold_item.OpenDateTime,samehold_item.CloseDateTime,samehold_item.UseMarginRate);

				mysqltestasset.Delete_byID(totaltablename.c_str(),record.ID);
			}

			i++;
		}
		record.MaxMarginRate = totalmarginrate+record.UseMarginRate;
//		record.SameTimeHold=sametime_list.size()+1;

		if(sametime_list.size()<=maxhold-1)		{
			record.SameTimeHold=i+1;
		}
		else		{
			record.SameTimeHold=maxhold;
		}

		printf("%s 开仓时间: %s    平仓时间: %s    同时持仓 [%d]合约占用保证金比例：%.2f%%  \n",record.InstrumentID,record.OpenDateTime,
				record.CloseDateTime,record.SameTimeHold,record.MaxMarginRate*100);
		vector<TestAssetData> ().swap(sametime_list);

		mysqltestasset.UpdateMaxMargin(totaltablename.c_str(),record);
	}


	vector<TestAssetData>  total_second_list;
	mysqltestasset.Find_AllListsbyCloseTime(total_second_list,totaltablename.c_str());
		double initialmoney=1000000.00;
		double totalasset=0;
		double highestasset=0;
		double maxback=0;
		double maxmaringrate=0;

	for (auto &record:total_second_list)		{
			totalasset+=record.Profit;
			record.LastAsset = initialmoney+totalasset;
	//		printf("更新收益: %s   %s   %.2f   \n ",record.InstrumentID,record.CloseDateTime,record.LastAsset);
			if(record.LastAsset>highestasset)		{
				record.HighestAsset=record.LastAsset;
				highestasset=record.LastAsset;
	//			printf("更新最高收益: %s   %s   %.2f   \n ",record.InstrumentID,record.CloseDateTime,record.HighestAsset);

			}

			if(record.LastAsset<highestasset)		{
				double back = (highestasset-record.LastAsset)/highestasset;
				if(back>maxback)			{
				record.BackDownRate=back;
				maxback=back;

	//			printf("更新最大回撤: %s   %s   %.2f   \n ",record.InstrumentID,record.CloseDateTime,record.BackDownRate);
				}
			}


			mysqltestasset.Update(totaltablename.c_str(),record);
		}
	vector<TestAssetData> ().swap(total_list);
	vector<TestAssetData> ().swap(total_second_list);
		vector<MainContractData> ().swap(list);


}

void TradeTest::StaticTestRecord(int maxhold)
{
	MysqlTestAsset mysqltestasset;
	MysqlTestRecord mysqltestrecord;
	std::string totaltablename;
	totaltablename.append(this->GetStrategy().c_str());
	totaltablename.append(this->GetYear().c_str());
	mysqltestasset.CreateTable(totaltablename.c_str());

	vector<MainContractData>  list;
	MysqlMaincontract mysqlmaincontract;
	mysqlmaincontract.Find_AllList(list);
	int i=0;
	for (auto &item:list)	{

		vector<TestRecordData>  ins_list;
		std::string tablename =this->GetTable_Record_Prefix();
		tablename.append(item.ProductID);
		if (mysqltestrecord.Exist_Table(tablename.c_str()))		{
			mysqltestrecord.Find_AllListsbyStatus(ins_list,tablename.c_str());

			for (auto &record:ins_list)		{

				TestAssetData data;
				std:string id=record.OpenDateTime;
				id.append("_");
				id.append(record.InstrumentID);

				strcpy(data.ID,id.c_str());
				strcpy(data.OpenDateTime,record.OpenDateTime);
				strcpy(data.CloseDateTime,record.CloseDateTime);
				strcpy(data.InstrumentID,record.InstrumentID);
				data.Direction=record.Direction;
				data.Status = record.Status;
				data.Profit = record.Profit;
				data.UseMarginRate = record.UseMarginRate;
				data.Commission = record.Commission;
				data.HighestAsset=0;
				data.BackDownRate=0;

				if(!mysqltestasset.Exist_DatabyID(totaltablename.c_str(),data.ID))				{
					mysqltestasset.Insert(totaltablename.c_str(),data);
				}
			}
		}



			vector<TestRecordData> ().swap(ins_list);
//		printf("MongoMainContract Find_All  end! ...\n");

	}

//	vector<TestAssetData>  total_list;


//	mysqltestasset.Find_AllListsbyCloseTime(total_list,totaltablename.c_str());
//	for (auto &record:total_list)
//	{
//
//		vector<TestAssetData>  sametime_list;
//		mysqltestasset.Find_SameTimeHoldListbyTime(sametime_list,totaltablename.c_str(),record.OpenDateTime,record.CloseDateTime);
//		double totalmarginrate=0;
//		int i=0;
//
//		for (auto &samehold_item:sametime_list)
//		{
////			printf("	开仓时间: %s    平仓时间: %s    同时开仓合约：%s----> [%s   %s   %.3f]\n",record.OpenDateTime,record.CloseDateTime,
////					samehold_item.InstrumentID,samehold_item.OpenDateTime,samehold_item.CloseDateTime,samehold_item.UseMarginRate);
//			if(i<maxhold-1)
//			{
//				printf("	编号: %d 同时开仓合约：%s----> [%s   %s   %.3f]\n",	i,samehold_item.InstrumentID,samehold_item.OpenDateTime,samehold_item.CloseDateTime,samehold_item.UseMarginRate);
//				totalmarginrate+=samehold_item.UseMarginRate;
//			}
//			else
//			{
//				printf("	编号: %d 删除合约：%s----> [%s   %s   %.3f]\n",	i,samehold_item.InstrumentID,samehold_item.OpenDateTime,samehold_item.CloseDateTime,samehold_item.UseMarginRate);
//
//				mysqltestasset.Delete_byID(totaltablename.c_str(),record.ID);
//			}
//
//			i++;
//		}
//		record.MaxMarginRate = totalmarginrate+record.UseMarginRate;
////		record.SameTimeHold=sametime_list.size()+1;
//
//		if(sametime_list.size()<=maxhold-1)
//		{
//			record.SameTimeHold=i+1;
//		}
//		else
//		{
//			record.SameTimeHold=maxhold;
//		}
//
//		printf("%s 开仓时间: %s    平仓时间: %s    同时持仓 [%d]合约占用保证金比例：%.2f%%  \n",record.InstrumentID,record.OpenDateTime,
//				record.CloseDateTime,record.SameTimeHold,record.MaxMarginRate*100);
//		vector<TestAssetData> ().swap(sametime_list);
//
//		mysqltestasset.UpdateMaxMargin(totaltablename.c_str(),record);
//	}


//	TestAssetData temp_assetdata = MaxHoldRecord();

	vector<TestAssetData>  total_second_list;
	mysqltestasset.Find_AllListsbyCloseTime(total_second_list,totaltablename.c_str());
		double initialmoney=1000000.00;
		double totalasset=0;
		double highestasset=0;
		double maxback=0;
		double maxmaringrate=0;

	for (auto &record:total_second_list)	{
			totalasset+=record.Profit;
			record.LastAsset = initialmoney+totalasset;
	//		printf("更新收益: %s   %s   %.2f   \n ",record.InstrumentID,record.CloseDateTime,record.LastAsset);
			if(record.LastAsset>highestasset)		{
				record.HighestAsset=record.LastAsset;
				highestasset=record.LastAsset;
	//			printf("更新最高收益: %s   %s   %.2f   \n ",record.InstrumentID,record.CloseDateTime,record.HighestAsset);

			}

			if(record.LastAsset<highestasset)			{
				double back = (highestasset-record.LastAsset)/highestasset;
				if(back>maxback)				{
				record.BackDownRate=back;
				maxback=back;

	//			printf("更新最大回撤: %s   %s   %.2f   \n ",record.InstrumentID,record.CloseDateTime,record.BackDownRate);
				}
			}


			mysqltestasset.Update(totaltablename.c_str(),record);
		}
//	vector<TestAssetData> ().swap(total_list);
	vector<TestAssetData> ().swap(total_second_list);
		vector<MainContractData> ().swap(list);


}

TestAssetData TradeTest::MaxHoldRecord()
{
	TestAssetData testassetdata;
	std::string tablename =this->GetTable_Year_DateTime().c_str();
	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;
	MysqlTestAsset mysqltestasset;

	std::string totaltablename;
	totaltablename.append(this->GetStrategy().c_str());
	totaltablename.append(this->GetYear().c_str());

	mysqldatetime.Find_AllLists(list,tablename.c_str());

	DateTime enddatetime =mysqldatetime.Find_LastDateTime(tablename.c_str());

	int maxhold=0;
	double maxmarginused =0.0;

	for (auto &dt:list)	{

		std::string day_sqltime =dt.Time;

		char buf[20];
		sprintf(buf,"%s 21:30:00",day_sqltime.substr(0,10).c_str());
		std::string day_night_sqltime =buf;


		sprintf(buf,"%s 09:30:00",day_sqltime.substr(0,10).c_str());
		std::string day_morning_sqltime =buf;

		sprintf(buf,"%s 14:30:00",day_sqltime.substr(0,10).c_str());
		std::string day_afternoon_sqltime =buf;

		int night_hold=mysqltestasset.CountbySqlTime(totaltablename.c_str(),day_night_sqltime.c_str());
		int morning_hold=mysqltestasset.CountbySqlTime(totaltablename.c_str(),day_morning_sqltime.c_str());
		int afternoon_hold=mysqltestasset.CountbySqlTime(totaltablename.c_str(),day_afternoon_sqltime.c_str());

		int tempmax = max(night_hold,max(morning_hold,afternoon_hold));

		if(tempmax>maxhold)		{
			maxhold=tempmax;
		}


		double night_margin=mysqltestasset.Get_SumUseMarginRatebySqlTime(totaltablename.c_str(),day_night_sqltime.c_str());
		double morning_margin=mysqltestasset.Get_SumUseMarginRatebySqlTime(totaltablename.c_str(),day_morning_sqltime.c_str());
		double afternoon_margin=mysqltestasset.Get_SumUseMarginRatebySqlTime(totaltablename.c_str(),day_afternoon_sqltime.c_str());

		double max_margin = max(night_margin,max(morning_margin,afternoon_margin));

		if(max_margin>maxmarginused){
			maxmarginused=max_margin;
		}
	}
	testassetdata.SameTimeHold = maxhold;
	testassetdata.MaxMarginRate = maxmarginused;
	return testassetdata;

}
void TradeTest::CheckMinuteTimeSequence()
{
	printf("CheckMinuteTimeSequence   begin   \n");
	int count =2;
	MysqlUtil mysqlutil("test");
	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
//	std::string au[count]={"au1506","au1512","au1606","au1612","au1706","au1712","au1806","au1812"};
	std::string au[2]={"au2006","au2012"};

//	std::string au[count]={"rb1601","rb1605","rb1610","rb1701","rb1705","rb1710","rb1801","rb1805"};
//	std::string au[count]={"rb1801","rb1805","rb1810","rb1901"};
	vector<std::string> au_list;

	for (int i=0;i<count;i++)		{

		printf("  %d    %s    \n",i,au[i].c_str());
		au_list.push_back(au[i].c_str());
		mysqlutil.CheckAllHisDataByInstrument(au[i].c_str());
	}

	for (auto &item:au_list)	{

		printf("CheckMinuteTimeSequence   合约  %s    \n",item.c_str());

		vector<HxDayLineData>  dayline_list;
		mysqldayline.Find_AllList(dayline_list,item.c_str());

		double endday_close;
		std::string endday;
		int i=0;
		for (auto &dayline:dayline_list)	{
			printf("	CheckMinuteTimeSequence   合约  %s   日期 %s  \n",item.c_str(),dayline.Time);

			vector<MinuteLineData>  min_list;

			char productid[10];
			CodeUtil codeutil;
			strcpy(productid,codeutil.GetProductID(item.c_str()).c_str());

			std::string min_database;
			min_database = "min_";
			min_database.append(productid);
		//	printf ("check min_database : %s \n",min_database.c_str());

			mysqlhismindata.Set_Database(min_database);

			DateUtil dateutil;
			std::string date = dateutil.ConvertSqlTimetoDate(dayline.Time);

			std::string tablename;
			tablename=item.c_str();
			tablename.append("_");
			tablename.append(date);

			if (mysqlhismindata.Exist_Table(tablename.c_str()))	{
				mysqlhismindata.Find_AllList(min_list,tablename.c_str());
			}
			else{
				printf("	不存在分钟数据   ---> %s    \n",tablename.c_str());

			}

			std::string m_tablename;
			m_tablename="au";
			m_tablename.append("_");
			m_tablename.append(date);


			MysqlMinuteTime mysqlminutetime;
			if(!mysqlminutetime.Exist_Table(m_tablename.c_str())){
				mysqlminutetime.CreateTable(m_tablename.c_str());
			}

			int minutetime_count = mysqlminutetime.CountbyAll(m_tablename.c_str());
			if(min_list.size() > minutetime_count){
				printf("	分钟数据不完整 轮询当日分钟数据  更新分钟数据表  历史分钟数据表: %ld   minutetime table: %d \n",min_list.size(),minutetime_count);

				 for (auto &mindata:min_list)	 {

					 MinuteTimeData data ;
					 strcpy(data.MinuteTime,mindata.MinuteTime);
					 if(!mysqlminutetime.Exist_MinuteTime(m_tablename.c_str(),data.MinuteTime)){
						 printf("	不存在分钟数据   --->  插入数据表  %s    MinuteTIme: %s  \n",date.c_str(),data.MinuteTime);
						 mysqlminutetime.Insert(m_tablename.c_str(),data);
					 }
				 }
			}
//			else{
//				printf("	分钟数据不够  无法轮询当日分钟数据表   \n");
//			}



				 vector<MinuteLineData>().swap(min_list);
		}
		vector<HxDayLineData>().swap(dayline_list);
	}



	vector<std::string>().swap(au_list);


}
void TradeTest::CheckTestInsLists(vector<std::string> &list,const char* year)
{
	int count=5;
	std::string rb[count];
	std::string str_year=year;
	std::string tail[5]={"01","05","10","01","05"};
	char buf[20];
	sprintf(buf,"%d",atoi(year)+1);
	std::string str_nextyear=buf;

	std::cout<< "next year  "<<str_nextyear << std::endl;
	for (int i=0;i<count;i++){
		rb[i]="rb";
		if(i>2){
			rb[i]+=str_nextyear.substr(2,2);
		}
		else{
			rb[i]+=str_year.substr(2,2);
		}

		rb[i]+=tail[i];
		list.push_back(rb[i]);
		std::cout<< "rb[] "<<rb[i]<<std::endl;
	}

}
void  TradeTest::CheckTradeDaySequence(const char* year)
{
	int count =5;
	MysqlUtil mysqlutil("test");
	MysqlDayLine mysqldayline;

	vector<std::string> rb_list;
	this->CheckTestInsLists(rb_list,year);

	for(auto &ins:rb_list){
		mysqlutil.CheckHttpDayLineByInstrument(ins.c_str());
	}

	for (auto &item:rb_list){

		vector<HxDayLineData>  dayline_list;
		mysqldayline.Find_AllList(dayline_list,item.c_str());

		double endday_close;
		std::string endday;
		int i=0;
		for (auto &dayline:dayline_list)
		{
				MysqlDateTime mysqldatetime;
				DateTime datetime;
				std::string temp =dayline.Time;
				std::string tablename ;
				tablename="y";
				tablename.append( temp.substr(0,4));
				strcpy(datetime.Time,dayline.Time);
				mysqldatetime.CreateTable(tablename.c_str());

				if (!mysqldatetime.Exist_DateTime(tablename.c_str(),dayline.Time))
				{
					printf("插入新日期   --------------%s  \n",dayline.Time);
					mysqldatetime.Insert(tablename.c_str(),datetime);
				}
		}
		vector<HxDayLineData>().swap(dayline_list);
	}

//	CheckMainContract("Black",rb_list);

	vector<std::string>().swap(rb_list);
}

void  TradeTest::CheckWholeTradeDaySequence()
{
	int count =3;
	MysqlUtil mysqlutil("test");
	MysqlDayLine mysqldayline;
	std::string rb14[3]={"rb1401","rb1405","rb1410"};
	std::string rb15[3]={"rb1501","rb1505","rb1510"};
	std::string rb16[3]={"rb1601","rb1605","rb1610"};
	std::string rb17[3]={"rb1701","rb1705","rb1710"};
	std::string rb18[3]={"rb1801","rb1805","rb1810"};
	vector<std::string> rb_list;


	for (int i=0;i<count;i++)
			{
				rb_list.push_back(rb14[i].c_str());
				mysqlutil.CheckHttpDayLineByInstrument(rb14[i].c_str());
			}

	for (int i=0;i<count;i++)
			{
				rb_list.push_back(rb15[i].c_str());
				mysqlutil.CheckHttpDayLineByInstrument(rb15[i].c_str());
			}

	for (int i=0;i<count;i++)
				{
					rb_list.push_back(rb16[i].c_str());
					mysqlutil.CheckHttpDayLineByInstrument(rb16[i].c_str());
				}

	for (int i=0;i<count;i++)
			{
				rb_list.push_back(rb17[i].c_str());
				mysqlutil.CheckHttpDayLineByInstrument(rb17[i].c_str());
			}


	for (auto &item:rb_list)
	{

		vector<HxDayLineData>  dayline_list;
		mysqldayline.Find_AllList(dayline_list,item.c_str());

		double endday_close;
		std::string endday;
		int i=0;
		for (auto &dayline:dayline_list)
		{
				MysqlDateTime mysqldatetime;
				DateTime datetime;
				std::string temp =dayline.Time;

				strcpy(datetime.Time,dayline.Time);
				mysqldatetime.CreateTable(this->GetTable_Whole_DateTime().c_str());

				if (!mysqldatetime.Exist_DateTime(this->GetTable_Whole_DateTime().c_str(),dayline.Time))
				{
					printf("插入新日期   --------------%s  \n",dayline.Time);
					mysqldatetime.Insert(this->GetTable_Whole_DateTime().c_str(),datetime);
				}
		}
		vector<HxDayLineData>().swap(dayline_list);
	}

//	CheckMainContract("Black",rb_list);

	vector<std::string>().swap(rb_list);
}
void  TradeTest::CheckErrorData(const char* productid)
{
//	printf("CheckErrorData %s  \n",productid);
	MysqlTestReport mysqltestreport;
	if (!mysqltestreport.Exist_Table(this->GetTableReport().c_str()))
	{
		mysqltestreport.CreateTable(this->GetTableReport().c_str());
	}


	if (!mysqltestreport.Exist_DatabyProductID(this->GetTableReport().c_str(),productid))
	{

		TestReportData testreport;
		strcpy(testreport.Product,productid);
		testreport.ErrorMinuteData=1;
//		printf("CheckErrorData  insert \n");
		mysqltestreport.InsertErrorData(this->GetTableReport().c_str(),testreport);

	}
	else
	{

		TestReportData testreport=mysqltestreport.Find_DatabyProductID(this->GetTableReport().c_str(),productid);


		strcpy(testreport.Product,productid);
		testreport.ErrorMinuteData++;
//		printf("CheckErrorData  update \n");
		mysqltestreport.UpdateErrorData(this->GetTableReport().c_str(),testreport);

	}
}

void  TradeTest::CheckErrorData(const char* pinstrumentid,const char* date)
{
	DateUtil dateutil;
	std::string sql_starttime;
	std::string sql_startdate;
	MysqlHisMinData mysqlhismindata;
	MysqlDayLine mysqldayline;
	HxDayLineData preday=mysqldayline.Find_PreDataBySqlTime(pinstrumentid,date);
	sql_startdate=preday.Time;
	char buf[20];
	sprintf(buf,"%s 21:00:00",sql_startdate.substr(0,10).c_str());
//	printf("CheckErrorData  %s   %s  \n",pinstrumentid,buf);
	sql_starttime=buf;

	time_t t_start = dateutil.ConvertSqlTimetoTimet(sql_starttime.c_str());
	 vector<std::string>  datelist;
	vector<MinuteLineData>  min_list;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());

	mysqlhismindata.Set_Database(min_database);


	std::string str_date=dateutil.ConvertSqlTimetoDate(date);
	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(str_date);
//		printf ("CheckErrorData min_database : %s   tablename: %s \n",min_database.c_str(),tablename.c_str());
	if(mysqlhismindata.Exist_Table(tablename.c_str()))	{
		mysqlhismindata.Find_AllList(min_list,tablename.c_str());

		if(min_list.size()>0 )	{

			 for (auto &mindata:min_list)			 {
				 time_t t_current=dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);
				 if(t_current<t_start)	 {
					 printf("CheckErrorData ---> 当日[%s] 起始时间:%s    当前时间:%s  数据非当日数据  需要删除!!!  \n",
							 str_date.c_str(),sql_starttime.c_str(),mindata.MinuteTime);
					 mysqlhismindata.DeletebyMinuteTime(tablename.c_str(),mindata.MinuteTime);
				 }
			 }

		}
	}


	vector<std::string>().swap(datelist);
	vector<MinuteLineData>().swap(min_list);

}
