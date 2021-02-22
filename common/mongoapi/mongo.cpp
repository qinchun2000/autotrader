#include "mongo.hpp"

Mongo::Mongo()
		{

		}


void Mongo::CheckAveragePriceData()
{
	DateUtil dateutil;

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlAvePrice mysqlaveprice;

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckAveragePriceData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());
	for (auto &item:list)
	{
		printf("<<<<<<CheckAveragePriceData--------   %s \n",item.c_str());


		 int dayline_count = mysqldayline.Count_DayLine(item.c_str());
		  int minuteline_count =mysqlhismindata.Count_MinuteTables(item.c_str());

		  if(!mysqlaveprice.Exist_Table(item.c_str()))
		  {
			  mysqlaveprice.CreateTable(item.c_str());
		  }

		  int averageprice_count =mysqlaveprice.Count_AllLists(item.c_str());


		  if(minuteline_count==dayline_count && dayline_count ==averageprice_count )
		  {
//			  printf("合约 AveragePriceData 已经更新完成 --->>>>[%s]\n",item.c_str());
		  }
		  else
		  {
			  vector<HxDayLineData>  dayline_list;
				mysqldayline.Find_AllList(dayline_list,item.c_str());

				for (auto &dayline:dayline_list)
				{
					DateUtil dateutil;


				 std::string date = dayline.Time;
				  std::string closetime ;
				  char buf[20];
				  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
				  closetime=buf;
				  closetime.append("150000");
		  //				  printf("<<<<<<CheckAveragePriceData--------   %s--->closetime[%s] \n",item.InstrumentID,closetime.c_str());
				time_t t_closetime =dateutil.ConvertStringtoTime (closetime.c_str());

				std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				time_t local_t = dateutil.ConvertStringtoTime(str_localupdate.c_str());



				std::string lastdate=mysqlaveprice.GetLastDateTime(item.c_str());
				lastdate.append("150000");
				time_t t_lastdate=dateutil.ConvertStringtoTime(lastdate.c_str());

		  //					printf("CheckAveragePriceData  --->   %s  lastdate[%s]--->closetime--->%ld     local-->%ld\n",
		  //							closetime.c_str(),lastdate.c_str(),t_closetime,local_t);

				  if(mysqlaveprice.Exist_DateTime(item.c_str(),dayline.Time))
					{
		  //								printf("exist!!!  --->>>>:\n");
					}
				  else if(local_t<t_closetime)
					  {
		  //								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
					  }
					else
					{
		  //						printf (" ------->  averagepricedata: %s 数据收集开始！！ \n",closetime.c_str());
						std::string tablename;
						tablename=item;
						tablename.append("_");
						tablename.append(closetime.substr(0,8).c_str());
						if(mysqlhismindata.Exist_Table(tablename.c_str()))
						{
							SqlAveragePriceData averagepricedata;
							strcpy(averagepricedata.Time,dayline.Time);
							averagepricedata.StartOpenInterest = mysqlhismindata.GetStartOpenInterest(tablename.c_str());
							averagepricedata.OpenInterest = mysqlhismindata.GetLastOpenInterest(tablename.c_str());
							averagepricedata.AveragePrice = mysqlhismindata.GetAveragePrice(tablename.c_str());

		  				printf (" ------->  合约%s : [%s] 分钟数据 计算中！！ %.3f \n",item.c_str(),closetime.c_str(),averagepricedata.AveragePrice);
							mysqlaveprice.Insert(item.c_str(),averagepricedata);
						}
						else
						{
							printf (" ------->  %s 不存在: [%s] 分钟数据 averagepricedata 缺失！！ \n",item.c_str(),closetime.c_str());
						}

					}

				}

				vector<HxDayLineData>().swap(dayline_list);

		  }

	}




	vector<std::string>().swap(list);
}


void Mongo::CheckHttpDayLine()
{
	DateUtil dateutil;
	MysqlDayLine mysqldayline;

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckHttpDayLine----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

	MysqlInstrument mysqlinstrument;


	for (auto &item:list)
			{
				mysqldayline.CreateTable(item.c_str());
				InstrumentData instrumentdata;
				instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.c_str());


				DateUtil dateutil;
				time_t lastclosetime_t = dateutil.GetLastCloseTime();
				std::string updatetime = mysqldayline.GetLastDateTime(item.c_str());
				time_t t_localupdate =dateutil.ConvertSqlTimetoCloseTimet (updatetime.c_str());

				if(!mysqldayline.Exist_Table(item.c_str()))
				{
					mysqldayline.CreateTable(item.c_str());
				}


				int count = mysqldayline.Count_DayLine(item.c_str());
				if ( count==0)
				{
					printf("CheckHttpDayLine[%s]   日线无数据,需要更新     %ld\n",item.c_str(),lastclosetime_t);
					mysqldayline.CheckHttpDayLine(item.c_str(),instrumentdata.ExchangeID);
				}
				else if(t_localupdate < lastclosetime_t   )
				{
					printf("CheckHttpDayLine[%s]: dayline localupdatetime [%s]   %ld\n",item.c_str(),updatetime.c_str(),t_localupdate);
					printf("CheckHttpDayLine      GetLastCloseTime             %ld\n",lastclosetime_t);
					mysqldayline.CheckHttpDayLine(item.c_str(),instrumentdata.ExchangeID);
				}
				else
				{
//						printf("CheckHttpDayLine[%s]已经完成更新: dayline localupdatetime [%s]   %ld\n",item.c_str(),updatetime.c_str(),t_localupdate);
//						printf("CheckHttpDayLine已经完成更新        GetLastCloseTime             %ld\n",lastclosetime_t);
				}


			}

	vector<std::string>().swap(list);
}


void Mongo::CheckHttpMinuteLine()
{
	printf("<<<<<<CheckHttpMinuteLine---- begin \n");

	char buf[100];
	LogUtil logutil("baselog","/home/vnpy/autotrader/baseinfo/baseinfo.log");

	MysqlInstrument mysqlinstrument;
	MysqlDayLine mysqldayline;

	MysqlHisMinData mysqlhismindata;
	DateUtil dateutil;
	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckHttpMinuteLine----->主力合约品种数[%ld] ----> %s\n",list.size()*2,dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
		{
				printf ("------分钟数据更新检测 ------->  %s \n",item.c_str());

				InstrumentData instrumentdata;
				instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.c_str());

				 int dayline_count = mysqldayline.Count_DayLine(item.c_str());
				  int minutetable_count =mysqlhismindata.Count_MinuteTables(item.c_str());


				  if(minutetable_count==dayline_count)
				  {
//					  printf("合约分钟线数据已经更新  --->>>>[%s]\n",item.c_str());
				  }
				  else
				  {
					  vector<HxDayLineData>  dayline_list;
						mysqldayline.Find_AllList(dayline_list,item.c_str());
	  //					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());

						for (auto &info:dayline_list)
						{
	  //					  printf("<<<<<<Find_UpdateDateList---- %s \n",info.Time);
						  DateUtil dateutil;

						  std::string date = info.Time;
						  std::string closeTime ;
						  std::string urldate ;
						  char buf[20];
						  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
						  urldate=buf;
						  closeTime=buf;
						  closeTime.append("150000");


						  time_t t_closetime =dateutil.ConvertStringtoTime (closeTime.c_str());


						  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
							time_t local_t;
							local_t = dateutil.ConvertStringtoTime(str_localupdate.c_str());
	  //						  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld   t_lastclosetime  %ld    \n",
	  //										  item.InstrumentID,CloseTime.c_str(),t_closetime ,local_t,t_lastclosetime);

	  //					  if(t_closetime<local_t && t_closetime>t_lastclosetime)
	  //						{
	  //
	  //
	  //
	  //						}

							std::string tablename;
							tablename=item;
							tablename.append("_");
							tablename.append(urldate);

						  if(mysqlhismindata.Exist_Table(tablename.c_str()))
							{
		//								printf("exist!!!  --->>>>:\n");
							}
						  else if(local_t<t_closetime)
							  {
		//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
							  }
							else
							{
								 tm tm_;
								 time_t t_;

								  strptime(closeTime.c_str(), "%Y%m%d%H%M%S", &tm_); //将字符串转换为tm时间
								  tm_.tm_isdst = -1;
								  t_  = mktime(&tm_); //将tm时间转换为秒时间


								  time_t tt = time(NULL);//这句返回的只是一个时间
								  tm* lastyear= localtime(&tt);
								  lastyear->tm_year  = lastyear->tm_year -1;

								  time_t  lastyear_t =mktime(lastyear);


	  //							  printf ("%s ----t_closetime %ld   local_t %ld   t_lastclosetime  %ld    \n",
	  //									  item.InstrumentID,t_closetime ,local_t,t_lastclosetime);
								 // 只下载一年内的分钟数据，如果请求的时当日未收盘的盘中分钟数据，则忽略
								  if (t_ < tt && t_ > lastyear_t)
								  {
	  								  printf (" ------->  不存在[%s]合约当日分钟数据，插入: %s 数据！！ \n",item.c_str(),closeTime.c_str());
	  								  mysqlhismindata.CreateTable(tablename.c_str());
	  								mysqlhismindata.Check_HxMinuteLineData(item.c_str() ,instrumentdata.ExchangeID,urldate.c_str());
								  }
							}
						}
						vector<HxDayLineData>().swap(dayline_list);

				  }



		}
	vector<std::string>().swap(list);

	printf("-------CheckHttpMinuteLine---- end   >>>>>>>>>>>>\n");
}
void Mongo::CheckCycleData()
{
	int cyclelength=20;
	MysqlDayLine mysqldayline;
	MysqlCycle mysqlcycle;
	DateUtil dateutil;

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckCycleData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
			{
//				printf ("------周期价差数据更新检测 ------->  %s \n",item.c_str());
				if(!mysqlcycle.Exist_Table(item.c_str()))
				{
					mysqlcycle.CreateTable(item.c_str());
				}

				  int dayline_count = mysqldayline.Count_DayLine(item.c_str());
				  int count =mysqlcycle.Count_AllLists(item.c_str());
				  std::string lasttime = mysqlcycle.GetLastDateTime(item.c_str());

				  if(count>=dayline_count-cyclelength-1 && dayline_count >cyclelength)
				  {
//					  printf("交易日周期价差数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
				  }
				  else if( dayline_count >cyclelength)
				  {

					  vector<HxDayLineData>  dayline_list;
						mysqldayline.Find_AllList(dayline_list,item.c_str());
				//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());


						int i=0;
						for (auto &dayline:dayline_list)
						{
							if(i>cyclelength)
							{
									mysqlcycle.CreateTable(item.c_str());
									DateUtil dateutil;

								  std::string date = dayline.Time;
								  std::string closeTime ;
								  char buf[20];
								  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
								  closeTime=buf;
								  closeTime.append("150000");
								  time_t t_closetime =dateutil.ConvertStringtoTime (closeTime.c_str());



								  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
								  time_t local_t;
								  local_t = dateutil.ConvertStringtoTime(str_localupdate.c_str());

				//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
				//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


									  if(mysqlcycle.Exist_DateTime(item.c_str(),dayline.Time))
										{
					//								printf("exist!!!  --->>>>:\n");
										}
									  else if(local_t<t_closetime)
										  {
					//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
										  }
										else
										{

											CycleData cycledata;
											cycledata=mysqldayline.Collect_CycleData(item.c_str(),dayline.Time);
											printf("[%s]不存在该数据,插入新周期价差数据[%s] --->>>>high20[%.3f]  low20[%.3f]:\n",
													item.c_str(),cycledata.Time,cycledata.High20,cycledata.Low20);
											mysqlcycle.Insert(item.c_str(),cycledata);
										}


							}
							i++;
						}



					vector<HxDayLineData>().swap(dayline_list);


				  }
			  else
				{
				  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",item.c_str(),cyclelength,dayline_count);
				}

			}



	vector<std::string>().swap(list);
}


void Mongo::CheckHisMAData()
{
	int malength=50;
	MysqlDayLine mysqldayline;
	MysqlHisMA mysqlhisma;
	DateUtil dateutil;


	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);

	printf("CheckHisMAData----->主力合约品种数[%ld] ----> %s\n",list.size()*2,dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
			{
//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());
				if(!mysqlhisma.Exist_Table(item.c_str()))
				{
					mysqlhisma.CreateTable(item.c_str());
				}

				 int dayline_count = mysqldayline.Count_DayLine(item.c_str());
				  int count =mysqlhisma.Count_AllLists(item.c_str());
				  std::string lasttime = mysqlhisma.GetLastDateTime(item.c_str());

				  if(count>=dayline_count-malength-1 && dayline_count >malength)
				  {
//					  printf("历史MA数据更数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
				  }
				  else if( dayline_count >malength)
				  {

						vector<HxDayLineData>  dayline_list;
						mysqldayline.Find_AllList(dayline_list,item.c_str());
				//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());
						int i=0;
						for (auto &dayline:dayline_list)
						{
							if(i>malength)
							{
								mysqlhisma.CreateTable(item.c_str());
									DateUtil dateutil;

								  std::string date = dayline.Time;
								  std::string closeTime ;
								  char buf[20];
								  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
								  closeTime=buf;
								  closeTime.append("150000");
								  time_t t_closetime =dateutil.ConvertStringtoTime (closeTime.c_str());



								  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
								  time_t local_t;
								  local_t = dateutil.ConvertStringtoTime(str_localupdate.c_str());

				//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
				//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


								  if(mysqlhisma.Exist_DateTime(item.c_str(),dayline.Time))
									{
				//								printf("exist!!!  --->>>>:\n");
									}
								  else if(local_t<t_closetime)
									  {
				//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
									  }
									else
									{

										HisMAData data;
										data=mysqldayline.Collect_TotalHisMAData(item.c_str(),dayline.Time);
										printf("[%s]不存在该数据,插入新HisMA数据[%s] --->>>>MA50[%.3f]  MA20[%.3f]  MA10[%.3f]:\n",
												item.c_str(),data.Time,data.MA50,data.MA20,data.MA10);
										mysqlhisma.Insert(item.c_str(),data);
									}
							}
							i++;
						}
						vector<HxDayLineData>().swap(dayline_list);
				  }
				  else
				  {
					  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",item.c_str(),malength,dayline_count);
				  }

			}



	vector<std::string>().swap(list);
}

void Mongo::CheckTradingDayMAData()
{

	char buf[200];
	LogUtil logutil("baselog","/home/vnpy/autotrader/baseinfo/mongodb.log");
	logutil.PrintLog("CheckTradingDayMAData ---------Begin!!!");
	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;

	DateUtil dateutil;
	MongoTradingDay mongotradingday;
	std::string tradingday ;
	TradingDayInfo tradingdayinfo;
	tradingdayinfo= mongotradingday.Find_LastTradingDay();
	std::string str_tradingdate=tradingdayinfo.TradingDay;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
			str_tradingdate.substr(6,2).c_str());
	tradingday = time;

	MysqlMaincontract mysqlmaincontract;
		vector<std::string>  list;
		mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckTradingDayMAData----->主力合约品种数[%ld] ----> %s\n",list.size()*2,dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
			{
				printf ("CheckTradingDayMAData-%s-MA数据更新检测  ---------    %s \n",tradingday.c_str(),item.c_str());

				mysqlma.CreateTable(item.c_str());
				if(mysqlma.Exist_DateTime(item.c_str(),tradingday.c_str()))
				{
		//								printf("exist!!!  --->>>>:\n");
				}
				else  if(mysqldayline.Count_DayLine(item.c_str())>59)
				{

					MAData madata;
					strcpy(madata.Time,tradingday.c_str());
					madata.MA59=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),59);
					madata.MA49=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),49);
					madata.MA19=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),19);
					madata.MA9=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),9);
					madata.MA17=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),17);
					madata.MA8=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),8);
					madata.MA3=mysqldayline.Collect_TotalMA_NData(item.c_str(),tradingday.c_str(),3);
					printf("[%s]不存在该数据,插入当日交易日MA新数据[%s] --->>>>MA59[%.3f]  MA49[%.3f]:\n",
							item.c_str(),madata.Time,madata.MA59,madata.MA49);
					mysqlma.Insert(item.c_str(),madata);
					sprintf(buf,"[%s] 不存在该数据,插入当日交易日MA新数据[%s] --->>>>MA59[%.3f]  MA49[%.3f]:\n",
							item.c_str(),madata.Time,madata.MA59,madata.MA49);
					logutil.PrintLog(buf);
				}
			}



	vector<std::string>().swap(list);
	logutil.PrintLog("CheckTradingDayMAData ---------End!!!");
}

void Mongo::CheckTradingDayCycleData()
{
	char buf[200];
	LogUtil logutil("baselog","/home/vnpy/autotrader/baseinfo/mongodb.log");
	logutil.PrintLog("CheckTradingDayCycleData ---------Begin!!!");
	DateUtil dateutil;
	MongoTradingDay mongotradingday;
	std::string tradingday ;
	TradingDayInfo tradingdayinfo;
	tradingdayinfo= mongotradingday.Find_LastTradingDay();
	std::string str_tradingdate=tradingdayinfo.TradingDay;
	char time[20];
	sprintf(time,"%s-%s-%s 00:00:00",str_tradingdate.substr(0,4).c_str(),str_tradingdate.substr(4,2).c_str(),
			str_tradingdate.substr(6,2).c_str());
	tradingday = time;

	MysqlDayLine mysqldayline;
	MysqlCycle mysqlcycle;

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckTradingDayCycleData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());
	sprintf(buf,"CheckTradingDayCycleData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());
	logutil.PrintLog(buf);
	for (auto &item:list)
		{
			printf ("Check-%s-交易日周期价差  ---------    %s \n",tradingday.c_str(),item.c_str());
			sprintf (buf,"Check-%s-交易日周期价差  ---------    %s \n",tradingday.c_str(),item.c_str());
			logutil.PrintLog(buf);
			mysqlcycle.CreateTable(item.c_str());

			int count =mysqldayline.Count_DayLine(item.c_str());
			if(mysqlcycle.Exist_DateTime(item.c_str(),tradingday.c_str()))
			{
	//								printf("exist!!!  --->>>>:\n");
			}
			else if(mysqldayline.Count_DayLine(item.c_str())>20)
			{

				CycleData cycledata;
				cycledata=mysqldayline.Collect_CycleData(item.c_str(),tradingday.c_str());
				printf("[%s]不存在该数据,插入当日交易日周期价差新数据[%s] ---count[%d]>>>>high20[%.3f]  low20[%.3f] HighDate[%s] LowDate[%s]\n",
						item.c_str(),cycledata.Time,count,cycledata.High20,cycledata.Low20,cycledata.HighDate,cycledata.LowDate);
				mysqlcycle.Insert(item.c_str(),cycledata);
				sprintf(buf,"[%s]----不存在该数据,插入当日交易日周期价差新数据[%s] --->>>>high20[%.3f]  low20[%.3f]  HighDate[%s] LowDate[%s]\n",
						item.c_str(),cycledata.Time,cycledata.High20,cycledata.Low20,cycledata.HighDate,cycledata.LowDate);
				logutil.PrintLog(buf);
			}


//			printf ("CheckTradingDayCycleData  ---------    %s \n",item.InstrumentID_next);


		}



	vector<std::string>().swap(list);
	logutil.PrintLog("CheckTradingDayCycleData ---------End!!!");
}

