#include "mysqlutil.hpp"

MysqlUtil::MysqlUtil(const char* staname){
	_strategyname=staname;
	this->SetLogPathFileName();
}
MysqlUtil::~MysqlUtil(){

}
void MysqlUtil::SetLogPathFileName(){

	Config config("config");
	std::string baselog=config.GetLogPath().c_str();

	baselog.append(_strategyname);
	baselog.append(".log");

	this->_log_pathfilename=baselog;
}


void MysqlUtil::run(){

	printf("	线程中运行日线数据检测-------------------->  %s   \n",this->GetInstrumentID().c_str());

}

std::string MysqlUtil::GetLogPathFileName(){

	return this->_log_pathfilename;
}


void MysqlUtil::CheckAveragePriceData()
{
	DateUtil dateutil;

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlAvePrice mysqlaveprice;

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckAveragePriceData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());
	for (auto &item:list)	{
		printf("<<<<<<CheckAveragePriceData--------   %s \n",item.c_str());

		CheckAveragePriceByInstrument(item.c_str());

	}
	vector<std::string>().swap(list);
}


void MysqlUtil::CheckAveragePriceByInstrument(const char * pinstrumentid)
{
	DateUtil dateutil;

	MysqlDayLine mysqldayline;
	MysqlHisMinData mysqlhismindata;
	MysqlAvePrice mysqlaveprice;

	printf("<<<<<<CheckAveragePriceByInstrument--------   %s \n",pinstrumentid);


	 int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);

	 char ProductID[31];
	CodeUtil codeutil;
	strcpy(ProductID,codeutil.GetProductID(pinstrumentid).c_str());

	std::string min_database;
	min_database = "min_";
	min_database.append(ProductID);
	mysqlhismindata.Set_Database(min_database);

	  int minuteline_count =mysqlhismindata.Count_MinuteTables(pinstrumentid);

	  if(!mysqlaveprice.Exist_Table(pinstrumentid))
	  {
		  mysqlaveprice.CreateTable(pinstrumentid);
	  }

	  int averageprice_count =mysqlaveprice.Count_AllListsbyTableName(pinstrumentid);


	  if(minuteline_count==dayline_count && dayline_count ==averageprice_count )
	  {
//			  printf("合约 AveragePriceData 已经更新完成 --->>>>[%s]\n",item.c_str());
	  }
	  else
	  {
		  vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);

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
			time_t t_closetime =dateutil.ConvertStringtoTimet (closetime.c_str());

			std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
			time_t local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());



			std::string lastdate=mysqlaveprice.GetLastDateTime(pinstrumentid);
			lastdate.append("150000");
			time_t t_lastdate=dateutil.ConvertStringtoTimet(lastdate.c_str());

	  //					printf("CheckAveragePriceData  --->   %s  lastdate[%s]--->closetime--->%ld     local-->%ld\n",
	  //							closetime.c_str(),lastdate.c_str(),t_closetime,local_t);

			  if(mysqlaveprice.Exist_DateTime(pinstrumentid,dayline.Time))
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
					tablename=pinstrumentid;
					tablename.append("_");
					tablename.append(closetime.substr(0,8).c_str());
					if(mysqlhismindata.Exist_Table(tablename.c_str()))
					{
						SqlAveragePriceData averagepricedata;
						strcpy(averagepricedata.Time,dayline.Time);
						averagepricedata.StartOpenInterest = mysqlhismindata.GetStartOpenInterest(tablename.c_str());
						averagepricedata.OpenInterest = mysqlhismindata.GetLastOpenInterest(tablename.c_str());
						averagepricedata.AveragePrice = mysqlhismindata.GetAveragePrice(tablename.c_str());

					printf (" ------->  合约%s : [%s] 分钟数据 计算中！！ %.3f \n",pinstrumentid,closetime.c_str(),averagepricedata.AveragePrice);
					if(!mysqlaveprice.Exist_Table(pinstrumentid))
					{
						mysqlaveprice.CreateTable(pinstrumentid);
					}

					if(!mysqlaveprice.Exist_DateTime(pinstrumentid,dayline.Time))
					{
						mysqlaveprice.Insert(pinstrumentid,averagepricedata);
					}

					}
					else
					{
						printf (" ------->  %s 不存在: [%s] 分钟数据 averagepricedata 缺失！！ \n",pinstrumentid,closetime.c_str());
					}

				}

			}

			vector<HxDayLineData>().swap(dayline_list);

	  }

}



void MysqlUtil::CheckCycleData()
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

				  int dayline_count = mysqldayline.Count_AllListsbyTableName(item.c_str());
				  int count =mysqlcycle.Count_AllListsbyTableName(item.c_str());
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
								  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());



								  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
								  time_t local_t;
								  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

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

void MysqlUtil::CheckCycleDataByInstrument(const char* pinstrumentid)
{
	int cyclelength=20;
	MysqlDayLine mysqldayline;
	MysqlCycle mysqlcycle;
	DateUtil dateutil;



//				printf ("------周期价差数据更新检测 ------->  %s \n",item.c_str());
	if(!mysqlcycle.Exist_Table(pinstrumentid))
	{
		mysqlcycle.CreateTable(pinstrumentid);
	}

	  int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	  int count =mysqlcycle.Count_AllListsbyTableName(pinstrumentid);
	  std::string lasttime = mysqlcycle.GetLastDateTime(pinstrumentid);

	  if(count>=dayline_count-cyclelength-1 && dayline_count >cyclelength)
	  {
//					  printf("交易日周期价差数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
	  }
	  else if( dayline_count >cyclelength)
	  {

		  vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);
	//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());


			int i=0;
			for (auto &dayline:dayline_list)
			{
				if(i>cyclelength)
				{
						mysqlcycle.CreateTable(pinstrumentid);
						DateUtil dateutil;

					  std::string date = dayline.Time;
					  std::string closeTime ;
					  char buf[20];
					  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
					  closeTime=buf;
					  closeTime.append("150000");
					  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());

					  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
					  time_t local_t;
					  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

	//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
	//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


						  if(mysqlcycle.Exist_DateTime(pinstrumentid,dayline.Time))
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
								cycledata=mysqldayline.Collect_CycleData(pinstrumentid,dayline.Time);
								printf("[%s]不存在该数据,插入新周期价差数据[%s] --->>>>high20[%.3f]  low20[%.3f]:\n",
										pinstrumentid,cycledata.Time,cycledata.High20,cycledata.Low20);
								mysqlcycle.Insert(pinstrumentid,cycledata);
							}


				}
				i++;
			}



		vector<HxDayLineData>().swap(dayline_list);


	  }
  else
	{
	  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",pinstrumentid,cyclelength,dayline_count);
	}






}
void MysqlUtil::CheckHisMAData()
{
	int malength=50;
	MysqlDayLine mysqldayline;
	MysqlHisMA mysqlhisma;
	DateUtil dateutil;


	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;

	mysqlmaincontract.GetTradeInstrumentLists (list);

	printf("CheckHisMAData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
	{
//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());
		printf("CheckHisMAData----->list size [%ld] ----> %s\n",list.size(),item.c_str());
		if(!mysqlhisma.Exist_Table(item.c_str()))
		{
			mysqlhisma.CreateTable(item.c_str());
		}

		 int dayline_count = mysqldayline.Count_AllListsbyTableName(item.c_str());
		  int count =mysqlhisma.Count_AllListsbyTableName(item.c_str());
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
						  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());



						  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
						  time_t local_t;
						  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

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
void MysqlUtil::CheckHisMADataByInstrument(const char* pinstrumentid)
{
	int malength=50;
	MysqlDayLine mysqldayline;
	MysqlHisMA mysqlhisma;
	DateUtil dateutil;


	printf("CheckHisMAData----->----> %s\n",dateutil.GetDateTimeString().c_str());

//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());
	if(!mysqlhisma.Exist_Table(pinstrumentid))
	{
		mysqlhisma.CreateTable(pinstrumentid);
	}

	 int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	  int count =mysqlhisma.Count_AllListsbyTableName(pinstrumentid);
	  std::string lasttime = mysqlhisma.GetLastDateTime(pinstrumentid);

	  if(count>=dayline_count-malength-1 && dayline_count >malength)
	  {
//					  printf("历史MA数据更数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
	  }
	  else if( dayline_count >malength)
	  {

			vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);
	//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());
			int i=0;
			for (auto &dayline:dayline_list)
			{
				if(i>malength)
				{
					mysqlhisma.CreateTable(pinstrumentid);
						DateUtil dateutil;

					  std::string date = dayline.Time;
					  std::string closeTime ;
					  char buf[20];
					  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
					  closeTime=buf;
					  closeTime.append("150000");
					  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());



					  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
					  time_t local_t;
					  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

	//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
	//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


					  if(mysqlhisma.Exist_DateTime(pinstrumentid,dayline.Time))
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
							data=mysqldayline.Collect_TotalHisMAData(pinstrumentid,dayline.Time);
							printf("[%s]不存在该数据,插入新HisMA数据[%s] --->>>>MA50[%.3f]  MA20[%.3f]  MA10[%.3f]:\n",
									pinstrumentid,data.Time,data.MA50,data.MA20,data.MA10);
							mysqlhisma.Insert(pinstrumentid,data);
						}
				}
				i++;
			}
			vector<HxDayLineData>().swap(dayline_list);
	  }
	  else
	  {
		  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",pinstrumentid,malength,dayline_count);
	  }


}

void MysqlUtil::CheckHisEMAData()
{

	DateUtil dateutil;
	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);

	printf("CheckHisEMAData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
	{
		printf ("------历史HisEMA 数据更新检测 ------->  %s \n",item.c_str());
		CheckHisEMADataByInstrument(item.c_str());
	}

	vector<std::string>().swap(list);
}
void MysqlUtil::CheckHisEMADataByInstrument(const char* pinstrumentid)
{
	int malength=50;
	MysqlDayLine mysqldayline;
	MysqlHisEMA mysqlhisema;
	DateUtil dateutil;


	printf("CheckHisEMAData----->----> %s\n",dateutil.GetDateTimeString().c_str());

//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());
	if(!mysqlhisema.Exist_Table(pinstrumentid))
	{
		mysqlhisema.CreateTable(pinstrumentid);
	}

	 int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	  int count =mysqlhisema.Count_AllListsbyTableName(pinstrumentid);
	  std::string lasttime = mysqlhisema.GetLastDateTime(pinstrumentid);

	  if(count>=dayline_count-malength-1 && dayline_count >malength)
	  {
//					  printf("历史MA数据更数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
	  }
	  else if( dayline_count >malength)
	  {

			vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);
	//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());
			int i=0;
			for (auto &dayline:dayline_list)
			{
				if(i>malength)
				{
					mysqlhisema.CreateTable(pinstrumentid);
					DateUtil dateutil;

				  std::string date = dayline.Time;
				  std::string closeTime ;
				  char buf[20];
				  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
				  closeTime=buf;
				  closeTime.append("150000");
				  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());



				  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
				  time_t local_t;
				  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

	//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
	//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


					  if(mysqlhisema.Exist_DateTime(pinstrumentid,dayline.Time))
						{
	//								printf("exist!!!  --->>>>:\n");
						}
					  else if(local_t<t_closetime)
						  {
	//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
						  }
						else
						{

							HisEMAData data;
							data=mysqldayline.Collect_HisEMAData(pinstrumentid,dayline.Time);
							printf("[%s]不存在该数据,插入新HisEMA数据[%s] --->>>>EMA50[%.3f]  EMA20[%.3f] EMA9[%.3f]  EMA4[%.3f]:\n",
									pinstrumentid,data.Time,data.EMA50,data.EMA20,data.EMA9,data.EMA4);
							mysqlhisema.Insert(pinstrumentid,data);
						}
				}
				i++;
			}
			vector<HxDayLineData>().swap(dayline_list);
	  }
	  else
	  {
		  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",pinstrumentid,malength,dayline_count);
	  }


}
void MysqlUtil::CheckMADataByInstrument(const char* pinstrumentid)
{
	int malength=50;
	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;
	DateUtil dateutil;

//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());
	if(!mysqlma.Exist_Table(pinstrumentid))
	{
		mysqlma.CreateTable(pinstrumentid);
	}

	 int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	  int count =mysqlma.Count_AllListsbyTableName(pinstrumentid);
	  std::string lasttime = mysqlma.GetLastDateTime(pinstrumentid);

	  if(count>=dayline_count-malength-1 && dayline_count >malength)
	  {
//					  printf("历史MA数据更数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
	  }
	  else if( dayline_count >malength)
	  {

			vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);
	//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());
			int i=0;
			for (auto &dayline:dayline_list)
			{
				if(i>malength)
				{
					mysqlma.CreateTable(pinstrumentid);
						DateUtil dateutil;

					  std::string date = dayline.Time;
					  std::string closeTime ;
					  char buf[20];
					  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
					  closeTime=buf;
					  closeTime.append("150000");
					  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());



					  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
					  time_t local_t;
					  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

	//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
	//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


					  if(mysqlma.Exist_DateTime(pinstrumentid,dayline.Time))
						{
	//								printf("exist!!!  --->>>>:\n");
						}
					  else if(local_t<t_closetime)
						  {
	//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
						  }
						else
						{

							MAData madata;
							strcpy(madata.Time,dayline.Time);
							madata.MA59=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,59);
							madata.MA49=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,49);
							madata.MA19=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,19);
							madata.MA9=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,9);
							madata.MA17=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,17);
							madata.MA8=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,8);
							madata.MA3=mysqldayline.Collect_TotalMA_NData(pinstrumentid,dayline.Time,3);
							printf("[%s]不存在该数据,插入当日交易日MA新数据[%s] --->>>>MA59[%.3f]  MA49[%.3f]:\n",
									pinstrumentid,madata.Time,madata.MA59,madata.MA49);
							mysqlma.Insert(pinstrumentid,madata);

						}
				}
				i++;
			}
			vector<HxDayLineData>().swap(dayline_list);
	  }
	  else
	  {
		  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",pinstrumentid,malength,dayline_count);
	  }


}

void MysqlUtil::CheckEMAData()
{

	DateUtil dateutil;


	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);

	printf("CheckEMAData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
	{
//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());

		CheckEMADataByInstrument(item.c_str());
	}



	vector<std::string>().swap(list);
}
void MysqlUtil::CheckEMADataByInstrument(const char* pinstrumentid)
{
	int malength=50;
	MysqlDayLine mysqldayline;
	MysqlEMA mysqlema;
	DateUtil dateutil;

//				printf ("------历史MA数据更新检测 ------->  %s \n",item.c_str());
	if(!mysqlema.Exist_Table(pinstrumentid))
	{
		mysqlema.CreateTable(pinstrumentid);
	}

	 int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	  int count =mysqlema.Count_AllListsbyTableName(pinstrumentid);
	  std::string lasttime = mysqlema.GetLastDateTime(pinstrumentid);

	  if(count>=dayline_count-malength-1 && dayline_count >malength)
	  {
//					  printf("历史MA数据更数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
	  }
	  else if( dayline_count >malength)
	  {

			vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);
	//					printf("<<<<<<Find_UpdateDateList----    ----->list size[%ld]\n",dayline_list.size());
			int i=0;
			for (auto &dayline:dayline_list)
			{
				if(i>malength)
				{
					mysqlema.CreateTable(pinstrumentid);
						DateUtil dateutil;

					  std::string date = dayline.Time;
					  std::string closeTime ;
					  char buf[20];
					  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
					  closeTime=buf;
					  closeTime.append("150000");
					  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());



					  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
					  time_t local_t;
					  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

	//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
	//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


					  if(mysqlema.Exist_DateTime(pinstrumentid,dayline.Time))
						{
	//								printf("exist!!!  --->>>>:\n");
						}
					  else if(local_t<t_closetime)
						  {
	//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
						  }
						else
						{

							EMAData emadata;
							strcpy(emadata.Time,dayline.Time);
							emadata.EMA49=mysqldayline.Collect_EMA_NData(pinstrumentid,dayline.Time,49);
							emadata.EMA8=mysqldayline.Collect_EMA_NData(pinstrumentid,dayline.Time,8);
							emadata.EMA3=mysqldayline.Collect_EMA_NData(pinstrumentid,dayline.Time,3);

							printf("[%s]不存在该数据,插入当日交易日EMA新数据[%s] --->>>>EMA49[%.3f]  EMA8[%.3f]  EMA3[%.3f]\n",
									pinstrumentid,emadata.Time,emadata.EMA49,emadata.EMA8,emadata.EMA3);
							mysqlema.Insert(pinstrumentid,emadata);

						}
				}
				i++;
			}
			vector<HxDayLineData>().swap(dayline_list);
	  }
	  else
	  {
		  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",pinstrumentid,malength,dayline_count);
	  }


}


void MysqlUtil::CheckHisATRData()
{

	DateUtil dateutil;
	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);

	printf("CheckHisATRData----->主力合约品种数[%ld] ----> %s\n",list.size(),dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
	{
		printf ("------历史HisATR 数据更新检测 ------->  %s \n",item.c_str());
		CheckHisATRDataByInstrument(item.c_str());
	}

	vector<std::string>().swap(list);
}
void MysqlUtil::CheckHisATRDataByInstrument(const char* pinstrumentid)
{
	int datalength=20;
	MysqlDayLine mysqldayline;
	MysqlHisATR mysqlhisatr;
	DateUtil dateutil;



//				printf ("------ATR数据更新检测 ------->  %s \n",item.c_str());
	if(!mysqlhisatr.Exist_Table(pinstrumentid))
	{
		mysqlhisatr.CreateTable(pinstrumentid);
	}

	  int dayline_count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);
	  int count =mysqlhisatr.Count_AllListsbyTableName(pinstrumentid);
	  std::string lasttime = mysqlhisatr.GetLastDateTime(pinstrumentid);

	  if(count>=dayline_count-datalength && dayline_count >datalength)
	  {
//					  printf("交易日ATR数据已经更新[%s]  --->>>>[%s]\n",item.c_str(),lasttime.c_str());
	  }
	  else if( dayline_count >datalength)
	  {

		  vector<HxDayLineData>  dayline_list;
			mysqldayline.Find_AllList(dayline_list,pinstrumentid);
	//					printf("<<<<<<Find_AllList----    ----->list size[%ld]\n",dayline_list.size());


			int i=0;
			for (auto &dayline:dayline_list)
			{
				if(i>datalength)
				{
					mysqlhisatr.CreateTable(pinstrumentid);
						DateUtil dateutil;

					  std::string date = dayline.Time;
					  std::string closeTime ;
					  char buf[20];
					  sprintf(buf,"%s%s%s",date.substr(0,4).c_str(),date.substr(5,2).c_str(),date.substr(8,2).c_str());
					  closeTime=buf;
					  closeTime.append("150000");
					  time_t t_closetime =dateutil.ConvertStringtoTimet (closeTime.c_str());

					  std::string str_localupdate = dateutil.ConvertCurrentTimetoString();
					  time_t local_t;
					  local_t = dateutil.ConvertStringtoTimet(str_localupdate.c_str());

	//				  printf ("%s ----t_closetime[%s] %ld   local_t--> %ld     \n",
	//							  item.InstrumentID,closeTime.c_str(),t_closetime ,local_t);


						  if(mysqlhisatr.Exist_DateTime(pinstrumentid,dayline.Time))
							{
		//								printf("exist!!!  --->>>>:\n");
							}
						  else if(local_t<t_closetime)
							  {
		//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
							  }
							else
							{

								ATRData atrdata;
								atrdata=mysqldayline.Collect_HisATR_NData(pinstrumentid,dayline.Time,20);
								printf("[%s]不存在该数据,插入新ATR数据[%s] --->>>>TR[%.3f]  ATR[%.3f]:\n",
										pinstrumentid,atrdata.Time,atrdata.TR,atrdata.ATR);
								mysqlhisatr.Insert(pinstrumentid,atrdata);
							}


				}
				i++;
			}



		vector<HxDayLineData>().swap(dayline_list);


	  }
  else
	{
	  printf("合约 [%s]日线数据不足%d天   --->>>>[%d]\n",pinstrumentid,datalength,dayline_count);
	}






}

std::string MysqlUtil::GetTradingDay(const char* userid)
{

	 MysqlTradingDay mysqltradingday(userid);
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string str=tradingdaydata.TradingDay;
	return str;
}
TradingDayData  MysqlUtil::GetTradingDayData(const char* userid)
{

	MysqlTradingDay mysqltradingday(userid);
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	return tradingdaydata;
}
std::string MysqlUtil::GetTradingDateTime(const char* userid)
{

	 MysqlTradingDay mysqltradingday(userid);
	TradingDayData tradingdaydata;
	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	std::string str=tradingdaydata.TradingDateTime;
	return str;
}

void MysqlUtil::CheckTradingDayMAData(const char* userid)
{

	char buf[200];
	LogUtil logutil("baselog",this->GetLogPathFileName());
	logutil.PrintLog("CheckTradingDayMAData ---------Begin!!!");
	MysqlDayLine mysqldayline;
	MysqlMA mysqlma;

	DateUtil dateutil;

//	MysqlTradingDay mysqltradingday;
//	TradingDayData tradingdaydata;
//	tradingdaydata= mysqltradingday.Find_LastTradingDay();
	TradingDayData tradingdaydata=this->GetTradingDayData(userid);
	std::string tradingday ;
	tradingday = GetTradingDay(userid);

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckTradingDayMAData----->主力合约品种数[%ld] ----> %s\n",list.size()*2,dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
			{
				printf ("CheckTradingDayMAData-%s-MA数据更新检测  ---------    %s \n",tradingday.c_str(),item.c_str());

				std::string dayline_lastdate =mysqldayline.GetLastDateTime(item.c_str());
				time_t tm_lastdayline= dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());
				time_t tm_enddayline =GetHisLastDate_DayLine(tradingday.c_str(),0);

				if(tm_lastdayline >= tm_enddayline)
				{

					mysqlma.CreateTable(item.c_str());
					if(mysqlma.Exist_DateTime(item.c_str(),tradingday.c_str()))
					{
			//								printf("exist!!!  --->>>>:\n");
					}
					else  if(mysqldayline.Count_AllListsbyTableName(item.c_str())>59)
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
				else
				{
					printf ("CheckTradingDayMAData-%s-   日线数据不合规 ---------    %s \n",tradingday.c_str(),item.c_str());
				}

			}



	vector<std::string>().swap(list);
	logutil.PrintLog("CheckTradingDayMAData ---------End!!!");
}


void MysqlUtil::CheckTradingDayEMAData(const char* userid)
{

	char buf[200];
	LogUtil logutil("baselog",this->GetLogPathFileName());
	logutil.PrintLog("CheckTradingDayEMAData ---------Begin!!!");
	MysqlDayLine mysqldayline;
	MysqlEMA mysqlema;

	DateUtil dateutil;

	TradingDayData tradingdaydata=this->GetTradingDayData(userid);

	std::string tradingdatetime ;

	tradingdatetime = tradingdaydata.TradingDateTime;

	MysqlMaincontract mysqlmaincontract;
	vector<std::string>  list;
	mysqlmaincontract.GetTradeInstrumentLists (list);
	printf("CheckTradingDayEMAData----->主力合约品种数[%ld] ----> %s\n",list.size()*2,dateutil.GetDateTimeString().c_str());

	for (auto &item:list)
			{
				printf ("CheckTradingDayEMAData-%s-MA数据更新检测  ---------    %s \n",tradingdatetime.c_str(),item.c_str());

				std::string dayline_lastdate =mysqldayline.GetLastDateTime(item.c_str());
				time_t tm_lastdayline= dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());


				time_t tm_enddayline =GetHisLastDate_DayLine(tradingdaydata.TradingDay,0);

				if(tm_lastdayline >= tm_enddayline)
				{

					mysqlema.CreateTable(item.c_str());
					if(mysqlema.Exist_DateTime(item.c_str(),tradingdatetime.c_str()))
					{
			//								printf("exist!!!  --->>>>:\n");
					}
					else  if(mysqldayline.Count_AllListsbyTableName(item.c_str())>59)
					{

						EMAData emadata;
						emadata=mysqldayline.Collect_EMAData(item.c_str(),tradingdatetime.c_str());
						mysqlema.Insert(item.c_str(),emadata);

						logutil.PrintLog(buf);
					}

				}
				else
				{
					printf ("CheckTradingDayEMAData-%s-   日线数据不合规 ---------    %s \n",tradingdatetime.c_str(),item.c_str());
				}

			}



	vector<std::string>().swap(list);
	logutil.PrintLog("CheckTradingDayMAData ---------End!!!");
}
void MysqlUtil::CheckTradingDayCycleData(const char* userid)
{
	char buf[200];
	LogUtil logutil("baselog",this->GetLogPathFileName());
	logutil.PrintLog("CheckTradingDayCycleData ---------Begin!!!");
	DateUtil dateutil;

	TradingDayData tradingdaydata=this->GetTradingDayData(userid);

	std::string tradingday ;
	tradingday = tradingdaydata.TradingDateTime;

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

			std::string dayline_lastdate =mysqldayline.GetLastDateTime(item.c_str());
			time_t tm_lastdayline= dateutil.ConvertSqlTimetoTimet(dayline_lastdate.c_str());
			time_t tm_enddayline =GetHisLastDate_DayLine(tradingdaydata.TradingDay,0);

			if(tm_lastdayline >= tm_enddayline)
			{
				mysqlcycle.CreateTable(item.c_str());

				int count =mysqldayline.Count_AllListsbyTableName(item.c_str());
				if(mysqlcycle.Exist_DateTime(item.c_str(),tradingday.c_str()))
				{
		//								printf("exist!!!  --->>>>:\n");
				}
				else if(count > 20)
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


			}
			else
			{
				printf ("CheckTradingDayCycleData  --------- 日线历史数据不合规   %s \n",item.c_str());
			}


//			printf ("CheckTradingDayCycleData  ---------    %s \n",item.InstrumentID_next);


		}



	vector<std::string>().swap(list);
	logutil.PrintLog("CheckTradingDayCycleData ---------End!!!");
}

void MysqlUtil::CheckRecordReport(const char* userid)
{
	MysqlUser mysqluser;
	User userinfo=mysqluser.Find_ActiveData();

	Config config("config");
	MysqlOrder mysqlcloseorder(config.GetTable_Close().c_str(),userinfo.UserID);
	MysqlRecord mysqlrecord;

	mysqlrecord.CreateTable();

	vector<OrderData>  closelists;

	mysqlcloseorder.Find_AllOrderListsbyStatus(closelists,2);  // 所有完成的交易

	MysqlDepthMarket mysqldepthmarket;


	for (auto &item:closelists)    // 逐笔比对
	{
		RecordData record;
		strcpy(record.CloseSignal,item.SignalName);
		strcpy(record.InstrumentID,item.InstrumentID);
		strcpy(record.CloseDateTime,item.LocalUpdateTime);
		strcpy(record.ExchangeID,item.ExchangeID);
		record.ClosePrice = item.Price;
		record.Volume = item.Volume;

		printf ("[%s]平仓单 [%s]  匹配中  平仓价: %.3f \n",item.InstrumentID,item.ExchangeID,record.ClosePrice);
		MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),userinfo.UserID);
		vector<OrderData>  openlists;
		mysqlopenorder.Find_AllOrderListsbyDateTimeStatus(openlists,item.InstrumentID,record.CloseDateTime,2);

		if(openlists.size()==1)
		{
			for (auto &item_open:openlists)    // 逐笔比对
				{

					if(item_open.Volume == item.Volume)
					{
						record.OpenPrice = item_open.Price;
						strcpy(record.OpenDateTime,item_open.LocalUpdateTime);
						strcpy(record.OpenSignal,item_open.SignalName);
						record.Direction = item_open.Direction;

						DepthMarketData data=mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);
						record.Unit = data.VolumeMultiple;

						if(item_open.Direction==0)
						{
							record.Margin = data.LongMarginRatio;

							record.Profit = (record.ClosePrice - record.OpenPrice) *(double) record.Unit *record.Margin;
						}
						else
						{
							record.Margin = data.ShortMarginRatio;

							record.Profit = (record.OpenPrice - record.ClosePrice) * (double)record.Unit *record.Margin;
						}

						if(record.Profit >0 )
						{
							strcpy(record.WinLoseStatus,"win");
						}
						else
						{
							strcpy(record.WinLoseStatus,"lose");
						}

						if(!mysqlrecord.Exist_RecordByOpenDateTime(record.InstrumentID,record.OpenDateTime))
						{
							printf ("[%s]平仓单 [%s]  找到对应开仓数据 %.3f \n",item.InstrumentID,item.LocalUpdateTime,record.Profit);
							mysqlrecord.Insert(record);
						}

					}
				}
		}
		else
		{
			printf ("[%s]平仓单 [%s]  没有对应开仓数据 \n",item.InstrumentID,item.LocalUpdateTime);
		}

		vector<OrderData>().swap(openlists);
	}
	vector<OrderData>().swap(closelists);
}



void MysqlUtil::CheckAllCsvDataByInstrument(const char * pinstrumentid)
{
	MysqlDayLine mysqldayline;

	MysqlProduct mysqlproduct;
	MysqlInstrument mysqlinstrument;

	InstrumentData instrumentdata;
	instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(pinstrumentid);

	char ProductID[10];
	CodeUtil codeutil;
	strcpy(ProductID,codeutil.GetProductID(pinstrumentid).c_str());
	ProductData productdata=mysqlproduct.Find_DatabyProductID(ProductID);


	if(!mysqldayline.Exist_Table(pinstrumentid))
	{
		mysqldayline.CreateTable(pinstrumentid);
	}

	int count = mysqldayline.Count_AllListsbyTableName(pinstrumentid);



}

time_t MysqlUtil::GetHisLastDate_DayLine(const char* date,int difsec)
{
	DateUtil dateutil;

	Config config("config");
	vector<std::string>  list;
	dateutil.ReadHoliday_CSV(list,config.GetHolidayPath().c_str(),config.GetHolidayFileName().c_str());

//	for (auto &item:list)
//		{
//			printf("   %s    \n",item.c_str());
//		}

	time_t tm_enddayline =  dateutil.CheckHisLastDayLine(list,date,difsec);

	vector<std::string>().swap(list);
	return tm_enddayline;

}
