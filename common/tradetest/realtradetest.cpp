#include "realtradetest.hpp"

RealTradeTest::RealTradeTest()
{
printf("RealTradeTest --> 构造函数   \n");
}

RealTradeTest::RealTradeTest(const char* pstrategy,const char* year)
{
	printf("RealTradeTest --> 带参数 构造函数  pstrategy: %s   year: %s \n",pstrategy,year);
	this->SetInitialMoney(1000000.00);
	this->SetRiskRate(0.02);
	this->SetStrategy(pstrategy);
	this->SetActiveCount(3);

	SetYear(year);
	SetTable_Year_DateTime(year);

	this->SetTable_Report();
	SetTable_Record_Prefix();
	MysqlDateTime mysqldatetime;

	if(!mysqldatetime.Exist_Table(GetTable_Year_DateTime().c_str()))
	{
		CheckDateTimeSequence();
	}

	InitialAssessTable();

	this->SetTableRecord("all");
}

void RealTradeTest::InitialAssessTable()
{
	MysqlTestSecAssess mysqltestsecassess;
	if(mysqltestsecassess.Exist_Table()){
			mysqltestsecassess.DropTable();
		}

	if(!mysqltestsecassess.Exist_Table()){
		mysqltestsecassess.CreateTable();
	}

	if(mysqltestsecassess.Count_AllLists()==0){

		printf("RealTradeTest:InitialAssessTable  不存在评估,写入初始评估表!!! \n");
//		Config config("test");
//		std::string path=config.GetConfigPath();
		std::string path="./";
		std::string filename ="assess.csv";

		std::string path_filename =path;
		path_filename.append(filename);

		CSVFile CSVRead;

		if (!CSVRead.Open(true, path.c_str(), filename.c_str()))	{
			printf("RealTradeTest:InitialAssessTable ---->File not found!\n");
		}
		else	{
			printf("RealTradeTest:InitialAssessTable --->File found!\n");
		}

		while (CSVRead.CSVReadNextRow())	{

			std::string strSection;
			std::string strTotalProductNumber;
			std::string strThreshHoldNumber;
			std::string strProductRiskNumber;
			CSVRead.CSVRead("Section", strSection);
			CSVRead.CSVRead("TotalProductNumber", strTotalProductNumber);
			CSVRead.CSVRead("ThreshHoldNumber", strThreshHoldNumber);
			CSVRead.CSVRead("ProductRiskNumber", strProductRiskNumber);
			int totalpruductnumber = atoi(strTotalProductNumber.c_str());
			int productrisknumber = atoi(strProductRiskNumber.c_str());
			int threshholdnumber = atoi(strThreshHoldNumber.c_str());

			SectionAssessData data;
			strcpy(data.Section,strSection.c_str());

			data.TotalProductNumber=totalpruductnumber;
			data.ProductRiskNumber=productrisknumber;
			data.ThreshHoldNumber = threshholdnumber;
			data.ProductHoldNumber=0;
			data.PD_Long_SignalNumber=0;
			data.PD_Short_SignalNumber=0;
			data.InsHoldNumber=0;
			DateUtil dateutil;
			strcpy(data.UpdateTime,dateutil.ConvertCurrentTimetoSqlTime().c_str());
			printf("RealTradeTest:InitialAssessTable 原始数据：%s %d %d %d   \n",strSection.c_str(),totalpruductnumber,
					threshholdnumber,productrisknumber);

			if(!mysqltestsecassess.Exist_DataBySection(strSection.c_str()))	{
				mysqltestsecassess.Insert(data);
				printf("RealTradeTest:InitialAssessTable 插入数据 ----> %s %d %d %d \n",data.Section,
						data.TotalProductNumber,data.ThreshHoldNumber,data.ProductRiskNumber);
			}
		}

	}
	else	{
		printf("RealTradeTest:InitialAssessTable  存在开仓评估表!!! \n");
	}

}

void RealTradeTest::AllTimeIndex_TrendTradeTest()
{
	// 设置止损价 atr* 倍数
	this->SetStopRatio(1.0);

	//定义开平仓时间  0, 盘中开仓   1,尾盘开仓
	this->SetOpenTimeMode(1);
	this->SetCloseTimeMode(0);

	std::string year_tablename =this->GetTable_Year_DateTime().c_str();
	MysqlTestRecord mysqltestrecord;
	if(!mysqltestrecord.Exist_Table(this->GetTableRecord().c_str())){
		mysqltestrecord.CreateTable(this->GetTableRecord().c_str());
	}

	MysqlMaincontract mysqlmaincontract;

	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;

	mysqldatetime.Find_AllLists(list,year_tablename.c_str());
//	mysqldatetime.Find_AllListsbyStartSqlTime(list,year_tablename.c_str(),"2018-03-05 00:00:00");


	DateTime enddatetime =mysqldatetime.Find_LastDateTime(year_tablename.c_str());

	 MysqlTestSignal mysqltestsignal;
	 if(!mysqltestsignal.Exist_Table()){
		 mysqltestsignal.CreateTable();
	 }
	 else{
		 mysqltestsignal.DeleteAll();
		 printf("全景测试  清空信号表 \n");
	 }

	for (auto &dt:list)	{

		this->SetCusor_DaySqlTime(dt.Time);
		vector<MinuteTimeData>  min_list;

		DateUtil dateutil;
		std::string date = dateutil.ConvertSqlTimetoDate(dt.Time);

		std::string m_tablename;
		m_tablename="au";
		m_tablename.append("_");
		m_tablename.append(date);
		printf("全景测试  当前日期------> sqltime: %s  date:%s \n",dt.Time,date.c_str());

		MysqlMinuteTime mysqlminutetime;

		if (mysqlminutetime.Exist_Table(m_tablename.c_str()))	{
			mysqlminutetime.Find_AllLists(min_list,m_tablename.c_str());
		}
		else{
			printf("不存在分钟数据   ---> %s    \n",m_tablename.c_str());

		}

		 for (auto &mindata:min_list)	 {

			 this->SetCusor_MinuteTime(mindata.MinuteTime);

			 std::string tmp = dt.Time;
			 char endtime[20];
			sprintf(endtime,"%s-%s-%s 14:55:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
			time_t t_signal_endtime = dateutil.ConvertSqlTimetoTimet(endtime);

			char operate_time[20];
			sprintf(operate_time,"%s-%s-%s 14:59:00",tmp.substr(0,4).c_str(),tmp.substr(5,2).c_str(),tmp.substr(8,2).c_str());
			time_t t_operate_time = dateutil.ConvertSqlTimetoTimet(operate_time);

			time_t t_current = dateutil.ConvertSqlTimetoTimet(mindata.MinuteTime);

//			printf(" %s   \n",this->GetTableRecord().c_str());
			if(mysqltestrecord.Count_RecordByStatus(this->GetTableRecord().c_str(),0)>0){

				vector<TestRecordData>  record_list;

				mysqltestrecord.Find_HoldLists(record_list,this->GetTableRecord().c_str());
				for(auto &record:record_list){

//					printf("	持仓合约 ----->  %s \n",record.InstrumentID);

					Dynamic_Hold(record.InstrumentID);
				}
				vector<TestRecordData>().swap(record_list);
			}

			if(t_current>=t_signal_endtime && t_current < t_operate_time){

				vector<MainContractData>  list;
				mysqlmaincontract.Find_AllList(list);
				for (auto &item:list){
//						printf("	全景测试[%s]  当前合约品种------> %s   \n",mindata.MinuteTime,item.ProductID);
					vector<std::string> instrument_list;
					GetInstrumentList(instrument_list,this->GetYear().c_str(),item);

					bool not_ap = strcmp(item.ProductID,"AP")!=0;
					bool not_stock = strcmp(item.Section,"Stock")!=0;
					bool not_finance = strcmp(item.Section,"Finance")!=0;
					bool not_metal = strcmp(item.Section,"Metal")!=0;
					bool not_precious = strcmp(item.Section,"Precious")!=0;
					bool black =strcmp(item.Section,"Black")==0;

//					if ( not_stock  &&  not_finance  && not_metal && not_precious && not_oil && not_corn ){
					if(black){

						this->SetProductID(item.ProductID);
						HxDayLineData main=	this->Get_CurrentMaincontractbyTime(instrument_list,dt.Time);
//						printf("	全景测试 [%s]--------- [%s]主力合约 :  %s   open: %.3f  \n",
//								mindata.MinuteTime,item.ProductID,main.InstrumentID,main.Open);
						std::string maininstrument=main.InstrumentID;

						if (maininstrument.length()>2)	{

							this->SetMainInstrumentID(main.InstrumentID);

							OpenSignal_MinuteTime_DynamicCycle(mindata.MinuteTime);


						}

					}
					vector<std::string>().swap(instrument_list);
				}
				 vector<MainContractData>().swap(list);

			}
			else if(t_current >= t_operate_time){
				printf(" 决策时间  %s \n",mindata.MinuteTime);
				Operate();
			}




		}
		vector<MinuteTimeData>().swap(min_list);
	}
	vector<DateTime>().swap(list);


}

void RealTradeTest::AllTimeIndex_DayTradeTest()
{
	// 设置止损价 atr* 倍数
	this->SetStopRatio(1.0);

	//定义开平仓时间  0, 盘中开仓   1,尾盘开仓
	this->SetOpenTimeMode(0);
	this->SetCloseTimeMode(0);

	std::string year_tablename =this->GetTable_Year_DateTime().c_str();
	MysqlTestRecord mysqltestrecord;
	if(!mysqltestrecord.Exist_Table(this->GetTableRecord().c_str())){
		mysqltestrecord.CreateTable(this->GetTableRecord().c_str());
	}


	MysqlMaincontract mysqlmaincontract;

	vector<DateTime>  list;
	MysqlDateTime mysqldatetime;

//	mysqldatetime.Find_AllLists(list,year_tablename.c_str());
	mysqldatetime.Find_AllListsbyStartSqlTime(list,year_tablename.c_str(),"2019-01-20 00:00:00");


	DateTime enddatetime =mysqldatetime.Find_LastDateTime(year_tablename.c_str());

	 MysqlTestSignal mysqltestsignal;

	 //	MysqlTestSignal mysqltestsignal;
	 	if(mysqltestsignal.Exist_Table()){
	 		mysqltestsignal.DropTable();
	 	}

	 if(!mysqltestsignal.Exist_Table()){
		 mysqltestsignal.CreateTable();
	 }
//	 else{
//		 mysqltestsignal.DropTable();
////		 mysqltestsignal.DeleteAll();
//		 printf("全景测试  清空信号表 \n");
//	 }

	for (auto &dt:list)	{

		this->SetCusor_DaySqlTime(dt.Time);
		vector<MinuteTimeData>  min_list;

		DateUtil dateutil;
		std::string date = dateutil.ConvertSqlTimetoDate(dt.Time);

		std::string m_tablename;
		m_tablename="au";
		m_tablename.append("_");
		m_tablename.append(date);
		printf("全景测试  当前日期------> sqltime: %s  date:%s \n",dt.Time,date.c_str());

		MysqlMinuteTime mysqlminutetime;

		if (mysqlminutetime.Exist_Table(m_tablename.c_str()))	{
			mysqlminutetime.Find_AllLists(min_list,m_tablename.c_str());
		}
		else{
			printf("AllTimeIndex_DayTradeTest:不存在分钟序列数据   ---> %s    \n",m_tablename.c_str());

		}

		 for (auto &mindata:min_list)	 {

			 this->SetCusor_MinuteTime(mindata.MinuteTime);

//			printf(" %s   \n",this->GetTableRecord().c_str());

			int holdcount= mysqltestrecord.Count_RecordByStatus(this->GetTableRecord().c_str(),0);
			bool valid_opentime = this->condition_OpenTimebyMinTimeData(&mindata,0);

			if(valid_opentime && holdcount==0){

				vector<MainContractData>  list;
//				mysqlmaincontract.Find_AllList(list);

				mysqlmaincontract.Find_ListsbySection(list,"Black");
				for (auto &item:list){
//						printf("	全景测试[%s]  当前合约品种------> %s   \n",mindata.MinuteTime,item.ProductID);
					vector<std::string> instrument_list;
					GetInstrumentList(instrument_list,this->GetYear().c_str(),item);

					bool not_ap = strcmp(item.ProductID,"AP")!=0;
					bool not_stock = strcmp(item.Section,"Stock")!=0;
					bool not_finance = strcmp(item.Section,"Finance")!=0;
					bool not_metal = strcmp(item.Section,"Metal")!=0;
					bool not_precious = strcmp(item.Section,"Precious")!=0;

					bool not_oil = strcmp(item.Section,"Oil")!=0;
					bool not_corn = strcmp(item.Section,"Corn")!=0;

					bool black =strcmp(item.Section,"Black")==0;

//					if ( not_stock  &&  not_finance  && not_metal && not_precious && not_oil && not_corn ){
					if(black){

						this->SetProductID(item.ProductID);

						HxDayLineData main=	this->Get_CurrentMaincontractbyTime(instrument_list,dt.Time);
//						printf("	全景测试 [%s]--------- [%s]主力合约 :  %s   open: %.3f  \n",
//								mindata.MinuteTime,item.ProductID,main.InstrumentID,main.Open);
						std::string maininstrument=main.InstrumentID;

						if (maininstrument.length()>2)	{

							this->SetMainInstrumentID(main.InstrumentID);

//							OpenSignal_MinuteTime_DynamicCycle(mindata.MinuteTime);

							OpenSignal_MinuteTime_SameUpDown(mindata.MinuteTime);

						}

					}
					vector<std::string>().swap(instrument_list);
				}
				 vector<MainContractData>().swap(list);

//				 printf(" 决策时间  %s \n",mindata.MinuteTime);
				 Operate();
			}

			if(holdcount>0){

				vector<TestRecordData>  record_list;

				mysqltestrecord.Find_HoldLists(record_list,this->GetTableRecord().c_str());
				for(auto &record:record_list){

//					printf("	持仓合约 ----->  %s \n",record.InstrumentID);

					Dynamic_Hold(record.InstrumentID);

					bool valid_endtime = this->condition_EndTimebyMinTimeData(&mindata);

					if(valid_endtime){
						printf("	收盘平仓:持仓合约 ----->  %s \n",record.InstrumentID);
						EndTimeClose(record.InstrumentID);
					}
				}
				vector<TestRecordData>().swap(record_list);
			}

		}
		vector<MinuteTimeData>().swap(min_list);
	}
	vector<DateTime>().swap(list);


}
// 周期价差的 周期是动态变化的   变化 参考   20日atr/10日atr
void RealTradeTest::OpenSignal_MinuteTime_DynamicCycle(const char *current_mt)
{
//	printf ("		OpenSignal_MinuteTime_DynamicCycle begin----> %s \n",this->GetCusor_DaySqlTime().c_str());
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;
	MysqlTestSignal mysqltestsignal;
	MysqlProduct mysqlproduct;

	DateUtil dateutil;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());

	ProductData productdata;
	productdata =mysqlproduct.Find_DatabyProductID(productid);

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	std::string str_current_date = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());

//	printf ("		当前时间: %s 当前合约: %s \n",current_mt,this->GetMainInstrumentID().c_str());
	int n =20;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	CycleData precycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),preday.Time,n);

	double presettleprice =mysqlhismindata.GetAveragePriceByTradingDay(this->GetMainInstrumentID().c_str(),str_current_date.c_str());

	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(min_database);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(str_current_date);

	if (mysqlhismindata.Exist_DataBySqlTime(tablename.c_str(),current_mt)){
		MinuteLineData mindata = mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),current_mt);

		CycleData cycledata = mysqldayline.Collect_CycleNData(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str(),n);

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		bool condition_long =   mindata.price > cycledata.High20   && preday.Close < precycledata.High20 ;
		bool condition_short = mindata.price <  cycledata.Low20  && preday.Close > precycledata.Low20 ;
		bool condition_kline_long = mindata.price > today.Open  ;
		bool condition_kline_short= mindata.price < today.Open ;


		 Signal signal;
		 std::string signalname=str_current_date;
		 signalname.append("_realcycle_");
		 signalname.append(this->GetMainInstrumentID().c_str());
		 strcpy(signal.SingalName,signalname.c_str());

		 if (condition_long && condition_kline_long    && sed>2)	{

			 strcpy(signal.InstrumentID,this->GetMainInstrumentID().c_str());
			 strcpy(signal.ProductID,productid);
			 strcpy(signal.Section,productdata.Section);
			 signal.Direction=0;
			 signal.Offset=0;
			 signal.Price = mindata.price;
			 signal.StopRange = mindata.price *0.02;
			 signal.StopPrice = signal.Price-signal.StopRange;

			 int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( signal.StopRange* maincontractdata.VolumeMultiple);

			 signal.Volume=p_stop;
			 signal.SignalCount=1;
			 signal.SingalActive =true;

			 signal.Sedimentary =sed;

			 signal.OpenPrice = today.Open;
			 signal.CloseOpenRate = (mindata.price-today.Open) /today.Open;
			 signal.PreSettlePrice = presettleprice;
			 signal.PreSettleOpenRate=(mindata.price- signal.PreSettlePrice) / signal.PreSettlePrice;

			 strcpy(signal.TradingDay,str_current_date.c_str());
			 strcpy(signal.SingalStartTime,mindata.MinuteTime);
			 strcpy(signal.SingalEndTime,mindata.MinuteTime);


				if(!mysqltestsignal.Exist_DataBySignalName(signal.SingalName)){
//					printf("precycledata     preday.Time %s    最高价 %.3f    最低价  %.3f   \n",
//							preday.Time,precycledata.High20,precycledata.Low20);
					printf ("		信号检测:多头信号----------> cycledata  %s  signalname: %s n:  %d   time[%s]   最高价: %.3f  现价: %.3f \n",
										 tablename.c_str(),signal.SingalName,n,current_mt,cycledata.High20,mindata.price );
					 mysqltestsignal.Insert(signal);
//					 printf("		插入signal 信号 \n");
				}
				else{

					if(mysqltestsignal.Exist_Long_ActiveDataBySignalName(signal.SingalName)){
						Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,0);
						db_signal.SignalCount++;
						db_signal.SingalActive =true;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);
//						printf("		存在激活signal多头 信号 %s 更新 SignalCount------> %d   %s !!!  \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					}
					else if(mysqltestsignal.Exist_Long_UnActiveDataBySignalNameTradingDay(signal.SingalName,str_current_date.c_str())){

						Signal db_signal = mysqltestsignal.Find_UnActiveDataBySignalNameDirectionTradingDay(signal.SingalName,0,str_current_date.c_str());
						db_signal.SignalCount++;
						db_signal.SingalActive =true;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);

						printf("		存在未激活signal多头 信号 %s 更新 SignalCount------> %d   %s !!!  \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					}
				}


	//				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_n_open");
		}
		 else {

			 if(mysqltestsignal.Exist_Long_ActiveDataBySignalName(signal.SingalName)){
				 Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,0);
				 int count = db_signal.SignalCount;
				 db_signal.SignalCount=0;
				db_signal.SingalActive=false;
				 strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
				mysqltestsignal.Update(db_signal);

				printf("		信号消失:多头 %s  SignalCount[%d]------> %d    时间: %s!!!  \n",
						db_signal.SingalName,count,db_signal.SignalCount,db_signal.SingalEndTime);

			 }

		 }

		 if (condition_short && condition_kline_short   && sed>2)	{

			 strcpy(signal.InstrumentID,this->GetMainInstrumentID().c_str());
			 strcpy(signal.ProductID,productid);
			 strcpy(signal.Section,productdata.Section);
			 signal.Direction=1;
			 signal.Offset=0;
			 signal.Price = mindata.price;
			 signal.StopRange = mindata.price *0.02;
			 signal.StopPrice = signal.Price+signal.StopRange;

			 int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( signal.StopRange* maincontractdata.VolumeMultiple);

			 signal.Volume=p_stop;
			 signal.SignalCount=1;
			 signal.SingalActive =true;

			 signal.Sedimentary =sed;
			 signal.OpenPrice = today.Open;
			 signal.CloseOpenRate = (mindata.price-today.Open) /today.Open;

			 signal.PreSettlePrice = presettleprice;
			 signal.PreSettleOpenRate=(mindata.price- signal.PreSettlePrice) / signal.PreSettlePrice;

			 strcpy(signal.TradingDay,str_current_date.c_str());
			 strcpy(signal.SingalStartTime,mindata.MinuteTime);
			 strcpy(signal.SingalEndTime,mindata.MinuteTime);

				if(!mysqltestsignal.Exist_DataBySignalName(signal.SingalName)){
//					printf("precycledata     preday.Time %s    最高价 %.3f    最低价  %.3f   \n",
//												preday.Time,precycledata.High20,precycledata.Low20);
					 printf ("		信号检测:空头信号---------->  cycledata  %s signalname: %s  n:  %d   time[%s]   最低价: %.3f  现价: %.3f \n",
										 tablename.c_str(),signal.SingalName,n,current_mt, cycledata.Low20,mindata.price);
					 mysqltestsignal.Insert(signal);
//					 printf("		插入signal 信号 \n");
				}
				else{
//					 printf("		存在signal 信号%s   !!!     \n",signal.SingalName);
					 if(mysqltestsignal.Exist_Short_ActiveDataBySignalName(signal.SingalName)){
						 Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,1);
						db_signal.SignalCount++;
						db_signal.SingalActive =true;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);
//						printf("		存在激活signal空头 信号%s  更新 SignalCount-------->%d  时间: %s \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					 }
					 else if(mysqltestsignal.Exist_Short_UnActiveDataBySignalNameTradingDay(signal.SingalName,str_current_date.c_str())){

						Signal db_signal = mysqltestsignal.Find_UnActiveDataBySignalNameDirectionTradingDay(signal.SingalName,1,str_current_date.c_str());
						db_signal.SignalCount++;
						db_signal.SingalActive =true;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);
						printf("		存在未激活signal空头 信号 %s 重新激活 SignalCount------> %d   %s !!!  \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					}
					 else {
//						 printf("		不存在激活signal空头 信号%s  !!!  \n",signal.SingalName);
					 }
				}
	//				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_n_open");

		}
		 else {

				 if(mysqltestsignal.Exist_Short_ActiveDataBySignalName(signal.SingalName)){
					 Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,1);

					 int count = db_signal.SignalCount;
					db_signal.SignalCount=0;
					db_signal.SingalActive =false;
					 strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
					mysqltestsignal.Update(db_signal);
					printf("		信号消失:空头 %s  SignalCount[%d]------> %d    时间: %s!!!  \n",
										 							db_signal.SingalName,count,db_signal.SignalCount,db_signal.SingalEndTime);


				 }

		 }
	}
	else{
//		printf ("		当前时间: %s 当前合约: %s  不存在分钟数据  !!! \n",current_mt,this->GetMainInstrumentID().c_str());
	}


//	 printf ("		OpenSignal_MinuteTime_DynamicCycle end \n");

}
// 涨跌共振 参考   20日atr/10日atr
void RealTradeTest::OpenSignal_MinuteTime_SameUpDown(const char *current_mt)
{
//	printf ("		OpenSignal_MinuteTime_DynamicCycle begin----> %s \n",this->GetCusor_DaySqlTime().c_str());
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;
	MysqlTestSignal mysqltestsignal;
	MysqlProduct mysqlproduct;

	DateUtil dateutil;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());

	ProductData productdata;
	productdata =mysqlproduct.Find_DatabyProductID(productid);

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	std::string str_current_date = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());

//	printf ("		当前时间: %s 当前合约: %s \n",current_mt,this->GetMainInstrumentID().c_str());
	int n =20;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());
	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());


	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(min_database);

	std::string tablename;
	tablename=this->GetMainInstrumentID().c_str();
	tablename.append("_");
	tablename.append(str_current_date);

	mysqlhismindata.Set_Table(tablename.c_str());

	if(!mysqlhismindata.Exist_Table(tablename.c_str())){
		return;
	}
	double presettleprice =mysqlhismindata.GetAveragePriceByTradingDay(this->GetMainInstrumentID().c_str(),str_current_date.c_str());


	if (mysqlhismindata.Exist_DataBySqlTime(tablename.c_str(),current_mt)){
		MinuteLineData mindata = mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),current_mt);

		double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

		double updownrate =( mindata.price - presettleprice )/presettleprice;

		double closeopenrate = (mindata.price-today.Open) /today.Open;

		bool condition_long =   updownrate > 0.01 &&  updownrate <0.1 ;
		bool condition_short = updownrate < -0.01  &&  updownrate >-0.1  ;
		bool condition_kline_long = closeopenrate > 0.005 ;
		bool condition_kline_short= closeopenrate < -0.005 ;


		 Signal signal;
		 std::string signalname=str_current_date;
		 signalname.append("_sameupdown_");
		 signalname.append(this->GetMainInstrumentID().c_str());
		 strcpy(signal.SingalName,signalname.c_str());

		 if (condition_long && condition_kline_long    && sed>2)	{

				if(!mysqltestsignal.Exist_DataBySignalName(signal.SingalName)){
//					printf("precycledata     preday.Time %s    最高价 %.3f    最低价  %.3f   \n",
//							preday.Time,precycledata.High20,precycledata.Low20);

					 strcpy(signal.InstrumentID,this->GetMainInstrumentID().c_str());
					 strcpy(signal.ProductID,productid);
					 strcpy(signal.Section,productdata.Section);
					 signal.Direction=0;
					 signal.Offset=0;
					 signal.Price = mindata.price;
					 signal.StopRange = mindata.price *0.02;
					 signal.StopPrice = signal.Price+signal.StopRange;

					 int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( signal.StopRange* maincontractdata.VolumeMultiple);

					 signal.Volume=p_stop;

		//			 signal.SingalActive =true;

					 signal.Sedimentary =sed;
					 signal.OpenPrice = today.Open;
					 signal.CloseOpenRate = (mindata.price-today.Open) /today.Open;

					 signal.PreSettlePrice = presettleprice;
					 signal.PreSettleOpenRate=updownrate;

					 strcpy(signal.TradingDay,str_current_date.c_str());

					 strcpy(signal.SingalEndTime,mindata.MinuteTime);

					 signal.SignalCount=1;
					 signal.SingalActive =false;
					 strcpy(signal.SingalStartTime,mindata.MinuteTime);
					printf ("		>>>信号检测:多头信号----------> cycledata  %s  signalname: %s n:  %d   time[%s]   涨跌幅: %.3f%%  现价: %.3f \n",
										 tablename.c_str(),signal.SingalName,n,current_mt,updownrate*100,mindata.price );
					 mysqltestsignal.Insert(signal);
//					 printf("		插入signal 信号 \n");
				}
				else{

					if(mysqltestsignal.Exist_Long_ActiveDataBySignalName(signal.SingalName)){
						Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,0);
						db_signal.SignalCount++;
						db_signal.SingalActive =true;
						db_signal.CloseOpenRate = signal.CloseOpenRate;
						db_signal.PreSettleOpenRate = signal.PreSettleOpenRate;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);
//						printf("		存在激活signal多头 信号 %s 更新 SignalCount------> %d   %s !!!  \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					}
					else if(mysqltestsignal.Exist_Long_UnActiveDataBySignalNameTradingDay(signal.SingalName,str_current_date.c_str())){

						Signal db_signal = mysqltestsignal.Find_UnActiveDataBySignalNameDirectionTradingDay(signal.SingalName,0,str_current_date.c_str());
						db_signal.SignalCount++;
						if(db_signal.SignalCount>this->GetActiveCount() && !db_signal.SingalActive){
							printf("		--->激活多头信号 %s 更新 SignalCount------> %d   %s !!!  \n",
									db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);
							db_signal.SingalActive =true;
						}

						db_signal.CloseOpenRate = signal.CloseOpenRate;
						db_signal.PreSettleOpenRate = signal.PreSettleOpenRate;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);


					}
				}


	//				 BuyOpen(mindata,this->GetMainInstrumentID().c_str(),"cycle_n_open");
		}
		 else {

			 if(mysqltestsignal.Exist_Long_ActiveDataBySignalName(signal.SingalName)){
				 Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,0);
				 int count = db_signal.SignalCount;
				 db_signal.SignalCount=0;
				db_signal.SingalActive=false;
				 strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
				mysqltestsignal.Update(db_signal);

				printf("		<<<信号消失:多头 %s  SignalCount[%d]------> %d    时间: %s!!!  \n",
						db_signal.SingalName,count,db_signal.SignalCount,db_signal.SingalEndTime);

			 }

		 }

		 if (condition_short && condition_kline_short   && sed>2)	{



				if(!mysqltestsignal.Exist_DataBySignalName(signal.SingalName)){
//					printf("precycledata     preday.Time %s    最高价 %.3f    最低价  %.3f   \n",
//												preday.Time,precycledata.High20,precycledata.Low20);

					 strcpy(signal.InstrumentID,this->GetMainInstrumentID().c_str());
					 strcpy(signal.ProductID,productid);
					 strcpy(signal.Section,productdata.Section);
					 signal.Direction=1;
					 signal.Offset=0;
					 signal.Price = mindata.price;
					 signal.StopRange = mindata.price *0.02;
					 signal.StopPrice = signal.Price+signal.StopRange;

					 int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( signal.StopRange* maincontractdata.VolumeMultiple);

					 signal.Volume=p_stop;


					 signal.Sedimentary =sed;
					 signal.OpenPrice = today.Open;
					 signal.CloseOpenRate = (mindata.price-today.Open) /today.Open;

					 signal.PreSettlePrice = presettleprice;
					 signal.PreSettleOpenRate=updownrate;

					 strcpy(signal.TradingDay,str_current_date.c_str());
					 strcpy(signal.SingalEndTime,mindata.MinuteTime);

					 signal.SignalCount=1;
					 signal.SingalActive =false;
					 strcpy(signal.SingalStartTime,mindata.MinuteTime);
					 printf ("		>>>信号检测:空头信号---------->  cycledata  %s signalname: %s  n:  %d   time[%s]   涨跌幅: %.3f%%  现价: %.3f \n",
										 tablename.c_str(),signal.SingalName,n,current_mt, updownrate*100,mindata.price);
					 mysqltestsignal.Insert(signal);
//					 printf("		插入signal 信号 \n");
				}
				else{
//					 printf("		存在signal 信号%s   !!!     \n",signal.SingalName);
					 if(mysqltestsignal.Exist_Short_ActiveDataBySignalName(signal.SingalName)){
						 Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,1);
						db_signal.SignalCount++;
//						db_signal.SingalActive =true;

						db_signal.CloseOpenRate = signal.CloseOpenRate;
						db_signal.PreSettleOpenRate = signal.PreSettleOpenRate;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);
//						printf("		存在激活signal空头 信号%s  更新 SignalCount-------->%d  时间: %s \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					 }
					 else if(mysqltestsignal.Exist_Short_UnActiveDataBySignalNameTradingDay(signal.SingalName,str_current_date.c_str())){

						Signal db_signal = mysqltestsignal.Find_UnActiveDataBySignalNameDirectionTradingDay(signal.SingalName,1,str_current_date.c_str());
						db_signal.SignalCount++;
						if(db_signal.SignalCount>this->GetActiveCount() && !db_signal.SingalActive){
							printf("		--->激活空头信号 %s 更新 SignalCount------> %d   %s !!!  \n",
									db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);
							db_signal.SingalActive =true;
						}
						db_signal.CloseOpenRate = signal.CloseOpenRate;
						db_signal.PreSettleOpenRate = signal.PreSettleOpenRate;
						strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
						mysqltestsignal.Update(db_signal);
//						printf("		存在未激活signal空头 信号 %s 重新激活 SignalCount------> %d   %s !!!  \n",db_signal.SingalName,db_signal.SignalCount,db_signal.SingalEndTime);

					}
					 else {
//						 printf("		不存在激活signal空头 信号%s  !!!  \n",signal.SingalName);
					 }
				}
	//				 	 SellOpen(mindata, this->GetMainInstrumentID().c_str(),"cycle_n_open");

		}
		 else {

				 if(mysqltestsignal.Exist_Short_ActiveDataBySignalName(signal.SingalName)){
					 Signal db_signal = mysqltestsignal.Find_ActiveDataBySignalNameDirection(signal.SingalName,1);

					 int count = db_signal.SignalCount;
					db_signal.SignalCount=0;
					db_signal.SingalActive =false;
					 strcpy(db_signal.SingalEndTime,mindata.MinuteTime);
					mysqltestsignal.Update(db_signal);
					printf("		<<<信号消失:空头 %s  SignalCount[%d]------> %d    时间: %s!!!  \n",
							db_signal.SingalName,count,db_signal.SignalCount,db_signal.SingalEndTime);


				 }

		 }
	}
	else{
//		printf ("		当前时间: %s 当前合约: %s  不存在分钟数据  !!! \n",current_mt,this->GetMainInstrumentID().c_str());
	}


//	 printf ("		OpenSignal_MinuteTime_DynamicCycle end \n");

}
void RealTradeTest::InitialSignal( Signal *signal,MinuteLineData *mindata)
{

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(this->GetMainInstrumentID().c_str()).c_str());

//	ProductData productdata;
//	productdata =mysqlproduct.Find_DatabyProductID(productid);
	MysqlMaincontract mysqlmaincontract;
	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	 strcpy(signal->InstrumentID,this->GetMainInstrumentID().c_str());
	 strcpy(signal->ProductID,productid);
	 strcpy(signal->Section,codeutil.GetSectionByInstrumentid(this->GetMainInstrumentID().c_str()).c_str());
	 signal->Direction=0;
	 signal->Offset=0;
	 signal->Price = mindata->price;
	 signal->StopRange = mindata->price *0.02;
	 signal->StopPrice = signal->Price-signal->StopRange;

	 int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( signal->StopRange* maincontractdata.VolumeMultiple);

	 signal->Volume=p_stop;
	 signal->SignalCount=1;
	 signal->SingalActive =true;

	 signal->Sedimentary =this->GetSedimentary(this->GetMainInstrumentID().c_str(),mindata->OpenInterest,mindata->price);

//	 signal->OpenPrice = today.Open;
//	 signal->CloseOpenRate = (mindata->price-today.Open) /today.Open;
//	 signal->PreSettlePrice = presettleprice;
//	 signal->PreSettleOpenRate=updownrate;
//
//	 strcpy(signal->TradingDay,str_current_date.c_str());
	 strcpy(signal->SingalStartTime,mindata->MinuteTime);
	 strcpy(signal->SingalEndTime,mindata->MinuteTime);

}
void RealTradeTest::Operate(){

	MysqlTestSignal mysqltestsignal;
	MysqlTestSecAssess mysqltestsecasses;
	MysqlTestRecord mysqltestrecord;

	vector<Signal>  signal_list;
	DateUtil dateutil;
	std::string str_current_date = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());
	vector<SectionAssessData>  assess_list;
	mysqltestsecasses.Find_AllLists(assess_list);

	// 清空上一周期的信号统计数据
	for (auto &assess:assess_list){
		SectionAssessData data=mysqltestsecasses.Find_DataBySection(assess.Section);
//		printf("   清空信号统计数----> data.ProductDaySignalNumber %d    %s \n",data.ProductDaySignalNumber,data.Section);
		if(data.PD_Long_SignalNumber>0){
			mysqltestsecasses.UpdateLongProductDaySignalNumber(0,assess.Section);
		}

		if(data.PD_Short_SignalNumber>0){
			mysqltestsecasses.UpdateShortProductDaySignalNumber(0,assess.Section);
		}

	}

	//更新当日的信号统计数据
	mysqltestsignal.Find_AllListsActivebyTradingDay(signal_list,str_current_date.c_str());
	if(signal_list.size()<2){
		return;
	}
	else{

		printf("信号评估  %s----> signalcount %ld   \n",this->GetCusor_MinuteTime().c_str(),signal_list.size());
	}

	for (auto &signal:signal_list){

		SectionAssessData data=mysqltestsecasses.Find_DataBySection(signal.Section);
		if(signal.Direction==0){
			data.PD_Long_SignalNumber++;
			mysqltestsecasses.UpdateLongProductDaySignalNumber(data.PD_Long_SignalNumber,signal.Section);
			printf("	评估多头信号: %s ------->Signal   %s   section %s   ProductHoldNumber %d \n",str_current_date.c_str(),signal.SingalName,
					signal.Section,data.ProductHoldNumber);
		}
		else if(signal.Direction==1){
			data.PD_Short_SignalNumber++;
			mysqltestsecasses.UpdateShortProductDaySignalNumber(data.PD_Short_SignalNumber,signal.Section);
			printf("	评估空头信号: %s ------->Signal   %s   section %s   ProductHoldNumber %d \n",str_current_date.c_str(),signal.SingalName,
					signal.Section,data.ProductHoldNumber);
		}
	}


	vector<SectionAssessData>().swap(assess_list);

	vector<SectionAssessData>  assess_long_list;

	mysqltestsecasses.Find_Long_AllListsBySingalNumber(assess_long_list);
	printf ("	待评估多头信号列表:信号数---------->   %ld \n",assess_long_list.size());
	//
	for (auto &assess:assess_long_list){

		if(assess.PD_Long_SignalNumber >= assess.ThreshHoldNumber && assess.PD_Short_SignalNumber==0){
			vector<Signal>  section_signal_list;
			mysqltestsignal.Find_CurrentDay_Long_AllListsActivebySection(section_signal_list,str_current_date.c_str(),assess.Section);
			printf (" 多头评估中--->%s   %d  %s  \n",assess.Section,assess.PD_Short_SignalNumber,assess.PD_Short_InstrumentId);
				for (auto &signal:section_signal_list){
					printf ("	待评估多头信号列表:信号---------->   signalname: %s    信号截止时间[%s]   方向: %d  状态: %d  涨跌幅 : %.3f%% \n",
									 signal.SingalName,signal.SingalEndTime,signal.Direction,signal.SingalActive,signal.PreSettleOpenRate*100 );
				}

			vector<Signal>().swap(section_signal_list);
//			Signal data = mysqltestsignal.Find_Long_BigSedSignalbySection(str_current_date.c_str(),assess.Section);
			Signal data = mysqltestsignal.Find_Long_BigUpRateSignalbySection(str_current_date.c_str(),assess.Section);
// 编译错,暂时忽略
//double sumrate = mysqltestsignal.Sum_ActiveUpRatebySection();

			int section_holdcount = mysqltestrecord.Count_RecordByTradingDaySection(this->GetTableRecord().c_str(),data.Section,str_current_date.c_str());
			bool isExist = mysqltestrecord.Exist_RecordByOpenDateInstrument(this->GetTableRecord().c_str(),
					str_current_date.c_str(),data.InstrumentID);
			if (!isExist && section_holdcount < assess.ProductRiskNumber) {
				printf("	多头评估:  %s	assess.PD_Long_SignalNumber %d   门限信号数  %d  %s \n",assess.Section,
									assess.PD_Long_SignalNumber,assess.ThreshHoldNumber,assess.Section);
				printf("	多头评估:不存在当日开平仓合约 	沉淀资金: %.3f   涨跌幅: %.3f%%   %s    板块%s  允许持仓  %d ------>现持仓%d  \n",
						data.Sedimentary,  data.PreSettleOpenRate*100,data.InstrumentID,assess.Section,assess.ProductRiskNumber,section_holdcount);

				if(data.Direction==0){
					this->SetStopRange(data.StopRange);
					BuyOpenSignal(assess.Section,data.InstrumentID,"realcycle_open");
				}
			}
			else{
							if(isExist){
			//					printf("	评估:存在开平仓合约 	沉淀资金: %.3f     %s \n", data.Sedimentary, data.InstrumentID);
							}
							if(section_holdcount >= assess.ProductRiskNumber){
			//					printf("	评估:板块%s  已存在定量合约 不再开仓  %d ------>现持仓%d  \n",
			//							assess.Section,assess.ProductRiskNumber,section_holdcount);
							}
						}

			}

	}

	vector<SectionAssessData>  assess_short_list;


	mysqltestsecasses.Find_Short_AllListsBySingalNumber(assess_short_list);
	printf ("	待评估空头信号列表:信号数---------->   %ld \n",assess_short_list.size());

	//
	for (auto &assess:assess_short_list){
		printf (" 空头评估中--->%s   PD_Short_SignalNumber:%d  ThreshHoldNumber:%d  PD_Long_SignalNumber:%d %s  \n",assess.Section,assess.PD_Short_SignalNumber,assess.ThreshHoldNumber,assess.PD_Long_SignalNumber,assess.PD_Short_InstrumentId);
		 if(assess.PD_Short_SignalNumber >= assess.ThreshHoldNumber && assess.PD_Long_SignalNumber==0 ){
			vector<Signal>  section_signal_list;
			mysqltestsignal.Find_CurrentDay_Short_AllListsActivebySection(section_signal_list,str_current_date.c_str(),assess.Section);

				for (auto &signal:section_signal_list){
					printf ("	待评估空头信号列表:信号---------->   signalname: %s    信号截止时间[%s]   方向: %d  状态: %d  涨跌幅: %.3f%%\n",
									 signal.SingalName,signal.SingalEndTime,signal.Direction,signal.SingalActive,signal.PreSettleOpenRate*100 );
				}

				vector<Signal>().swap(section_signal_list);
//			Signal data = mysqltestsignal.Find_Short_BigSedSignalbySection(str_current_date.c_str(),assess.Section);
			Signal data = mysqltestsignal.Find_Short_BigUpRateSignalbySection(str_current_date.c_str(),assess.Section);

			int section_holdcount = mysqltestrecord.Count_RecordByTradingDaySection(this->GetTableRecord().c_str(),data.Section,str_current_date.c_str());

			bool isExist = mysqltestrecord.Exist_RecordByOpenDateInstrument(this->GetTableRecord().c_str(),
					str_current_date.c_str(),data.InstrumentID);
			if (!isExist && section_holdcount < assess.ProductRiskNumber) {
				printf("	空头评估:  %s	assess.PD_Short_SignalNumber %d   门限信号数  %d  %s \n",assess.Section,
									assess.PD_Short_SignalNumber,assess.ThreshHoldNumber,assess.Section);
				printf("	空头评估:不存在当日开平仓合约 	沉淀资金: %.3f    涨跌幅: %.3f%%   %s    板块%s  允许持仓  %d ------>现持仓%d  \n",
						data.Sedimentary, data.PreSettleOpenRate*100,data.InstrumentID,assess.Section,assess.ProductRiskNumber,section_holdcount);

				if(data.Direction==1){
					this->SetStopRange(data.StopRange);
					SellOpenSignal(assess.Section,data.InstrumentID,"realcycle_open");
				}
			}
			else{
					if(isExist){
	//					printf("	评估:存在开平仓合约 	沉淀资金: %.3f     %s \n", data.Sedimentary, data.InstrumentID);
					}
					if(section_holdcount >= assess.ProductRiskNumber){
	//					printf("	评估:板块%s  已存在定量合约 不再开仓  %d ------>现持仓%d  \n",
	//							assess.Section,assess.ProductRiskNumber,section_holdcount);
					}

			}
		}

	}

	vector<SectionAssessData>().swap(assess_long_list);
	vector<SectionAssessData>().swap(assess_short_list);
	vector<Signal>().swap(signal_list);

}
void RealTradeTest::BuyOpenSignal(const char* section,const char * pinstrumentid,const char* opensignal)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;
	std::string str_tradingday = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	MysqlDayLine mysqldayline;
	HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(min_database);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(str_tradingday);

	MinuteLineData mindata;
	if(mysqlhismindata.Exist_MinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str())){

		mindata= mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str());
	}
	else{
		mindata= mysqlhismindata.Find_ExistDatabyMinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);

	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);

	std::string opendate;
	opendate = dateutil.ConvertSqlTimetoDate(mindata.MinuteTime);

	printf ("check min_database : %s  table: %s  cusor_mt: %s   mindata.MinuteTime: %s   \n",min_database.c_str(),tablename.c_str(),
			this->GetCusor_MinuteTime().c_str(),mindata.MinuteTime);

	strcpy(tr.OpenDate,opendate.c_str());
	std::string id =opendate;
	id.append("_");
	id.append(pinstrumentid);
	strcpy(tr.ID,id.c_str());
	strcpy(tr.InstrumentID,pinstrumentid);
	strcpy(tr.Section,section);

	tr.OpenPrice = mindata.price ;
	tr.Direction =0;
	tr.Status =0;

	tr.Sedimentary = sed;
	tr.OpenOPI = mindata.OpenInterest;

	printf("open----> openopi   %d    min.opi   %d   price  %.3f  stoprange : %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
			tr.OpenOPI ,mindata.OpenInterest,mindata.price,this->GetStopRange(),maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);


	tr.StopPrice = mindata.price  - this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price + this->GetStopRange();

	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);

	tr.DayOpenPrice=today.Open;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;

	tr.ClosePrice=0;
	tr.CloseVolume=0;


	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*0.05)/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;
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


	if (tr.Volume >0 )	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开多[%s]   -------%s ------position[%d]  openprice %.3f   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,tr.Volume,mindata.price, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

	}
	else	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d >0   沉淀资金  %.2f(持仓量 %d)    未开仓  !!! \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d    p_stop: %d   p_margin: %d  !!! \n",pinstrumentid,tr.Volume,p_stop,p_margin);
		}

	}
}
void RealTradeTest::SellOpenSignal(const char* section,const char * pinstrumentid,const char* opensignal)
{
	MysqlTestRecord mysqltestrecord;
	MysqlMaincontract mysqlmaincontract;
	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;
	std::string str_tradingday = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	MysqlDayLine mysqldayline;
		HxDayLineData today=mysqldayline.Find_DataBySqlTime(this->GetMainInstrumentID().c_str(),this->GetCusor_DaySqlTime().c_str());

	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(min_database);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(str_tradingday);

	MinuteLineData mindata;
	if(mysqlhismindata.Exist_MinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str())){

		mindata= mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str());
	}
	else{
			mindata= mysqlhismindata.Find_ExistDatabyMinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str());
	}

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);
	double sed=mindata.OpenInterest * mindata.price * (double)maincontractdata.VolumeMultiple * maincontractdata.LongMarginRatio /100000000.0;

	TestRecordData tr;
	strcpy(tr.OpenDateTime,mindata.MinuteTime);
	std::string opendate;
	opendate = dateutil.ConvertSqlTimetoDate(mindata.MinuteTime);

	printf ("check min_database : %s   cusor_mt: %s   mindata.MinuteTime: %s   \n",min_database.c_str(),this->GetCusor_MinuteTime().c_str(),mindata.MinuteTime);

	strcpy(tr.OpenDate,opendate.c_str());
	std::string id =opendate;
	id.append("_");
	id.append(pinstrumentid);
	strcpy(tr.ID,id.c_str());
	strcpy(tr.InstrumentID,pinstrumentid);
	strcpy(tr.Section,section);

	tr.OpenPrice = mindata.price ;
	tr.Direction =1;
	tr.Status =0;
	tr.Sedimentary = sed;
	tr.OpenOPI = mindata.OpenInterest;

	printf("open----> openopi   %d    min.opi   %d   price  %.3f  stoprange : %.3f  VolumeMultiple %d  LongMarginRatio %.3f sed %.3f \n",
			tr.OpenOPI ,mindata.OpenInterest,mindata.price,this->GetStopRange(),maincontractdata.VolumeMultiple , maincontractdata.LongMarginRatio,sed);


	tr.StopPrice = mindata.price  + this->GetStopRange();
//					tr.StopPrice = mindata.price*0.99;
	tr.WinPrice = mindata.price - this->GetStopRange();
	tr.Highest = mindata.price ;
	tr.Lowest = mindata.price;
	strcpy(tr.OpenSignal,opensignal);
	tr.DayOpenPrice=today.Open;
	tr.AverageUpDownrate =0.0 ;
	tr.HalfStop=false;
	tr.ClosePrice=0;
	tr.CloseVolume=0;

	MysqlCommission mysqlcommission;
	Commission commission=mysqlcommission.Find_DatabyProductID(productid);

	int p_stop= (this->GetInitialMoney()*this->GetRiskRate())/( this->GetStopRange()* maincontractdata.VolumeMultiple);
	int p_margin = (this->GetInitialMoney()*0.05)/( mindata.price* maincontractdata.VolumeMultiple*commission.Margin);
	int position = min(p_stop,p_margin) ;
//	int position = p_stop ;
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


	if (tr.Volume >0 )	{
		mysqltestrecord.Insert(this->GetTableRecord().c_str(),tr);

		printf("	%s:开空[%s]   -------%s ------position[%d]  openprice %.3f   止损价 %.3f   佣金: %.3f 沉淀资金 %.3f \n",
				tr.InstrumentID,opensignal,mindata.MinuteTime,tr.Volume,mindata.price, tr.StopPrice,tr.Commission,sed);
	//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

//		printf("	开多   -------%s 板块上涨: %.2f 昨结:%.3f 今开:%.3f   openprice %.3f   止损价 %.3f  止盈价  %.3f highest %.3f   highdata.High20 %.3f cycledata.high20 %.3f HighCount--> %d  StopPrice  ---> %.3f \n",
//				mindata.MinuteTime,hissectiondata.CountUprate,predata.AveragePrice, openprice,mindata.price, tr.StopPrice,tr.WinPrice,highest,highdata.High20,cycledata.High20,cycledata.HighCount,tr.StopPrice);

	}
	else	{
		if(tr.Volume >0)		{
			printf("  [%s ] position  %d >0   沉淀资金  %.2f(持仓量 %d)  未开仓!!! \n",pinstrumentid,tr.Volume,sed, mindata.OpenInterest);
		}
		else		{
			printf("   [%s ] 可开仓位不足  %d    p_stop: %d   p_margin: %d  !!! \n",pinstrumentid,tr.Volume,p_stop,p_margin);
		}

	}
}

void RealTradeTest::Dynamic_Hold(const char* pinstrumentid)
{

//	printf ("Dynamic_Hold begin  当前日期: %s    ---> %s \n", this->GetCusor_DaySqlTime().c_str(),this->GetCusor_MinuteTime().c_str());

	DateUtil dateutil;
	std::string str_tradingday = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	MysqlTestRecord mysqltestrecord;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);



	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(min_database);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(str_tradingday);

	if(mysqlhismindata.Exist_DataBySqlTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str())){
//		printf ("check min_database : %s    tablename: %s \n",min_database.c_str(),tablename.c_str());
		MinuteLineData mindata = mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str());

		double stoprange = mindata.price *0.02;
	//	int p= (this->GetInitialMoney()*this->GetRiskRate())/(stoprange* maincontractdata.VolumeMultiple);
	//	int position = p ;
//		 printf ("check %s mindata : %s     --> price % .3f \n",tablename.c_str(),mindata.MinuteTime,mindata.price);

		if (mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0))	{
			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);

			 if (tr.Direction==0 && mindata.price> tr.Highest && mindata.price > tr.OpenPrice) {
				 tr.Highest=mindata.price;

				 tr.StopPrice = tr.Highest -stoprange;
				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(mindata.price- tr.OpenPrice) *tr.Volume-tr.Commission;

				 printf("	%s  %s 持仓中[%d]   更新最高价   -------  %.3f   开仓价 %.3f 止损价  %.3f  交易单位 %d profit:  %.2f 佣金: %.2f \n",tr.InstrumentID,
						 mindata.MinuteTime,tr.Volume,tr.Highest,tr.OpenPrice,tr.StopPrice,maincontractdata.VolumeMultiple ,tr.Profit,tr.Commission );
	//						 mysqltestrecord.UpdateHighStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),
	//								 pinstrumentid,mindata.price,tr.StopPrice,0);
				 mysqltestrecord.UpdateProfit(this->GetTableRecord().c_str(),tr);
			 }
			 else if (tr.Direction==1 && mindata.price < tr.Lowest && mindata.price < tr.OpenPrice)	{
				 tr.Lowest=mindata.price;
				 tr.StopPrice = tr.Lowest + stoprange;
				 tr.Profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - mindata.price ) *tr.Volume-tr.Commission;
				 printf("	%s  %s 持仓中[%d]   更新最低价   -------  %.3f   开仓价 %.3f 止损价  %.3f  交易单位 %d profit:  %.2f  佣金: %.2f \n",tr.InstrumentID,
				 						 mindata.MinuteTime,tr.Volume,tr.Lowest,tr.OpenPrice,tr.StopPrice,maincontractdata.VolumeMultiple ,tr.Profit,tr.Commission );

	//						 mysqltestrecord.UpdateLowStopPriceByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,mindata.price,tr.StopPrice,0);
				 mysqltestrecord.UpdateProfit(this->GetTableRecord().c_str(),tr);
			}
		}

		 if( mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) ) {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
			 double profit;

			 if (tr.Direction ==0)	 {
				 profit = (double)maincontractdata.VolumeMultiple  *(mindata.price- tr.OpenPrice) *tr.Volume-tr.Commission;
			 }
			 else if (tr.Direction ==1)	 {
				 profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - mindata.price ) *tr.Volume-tr.Commission;
			 }

			 if (tr.Direction ==0  )	 {

				 if (mindata.price < tr.StopPrice )		{
					 Close(tr,mindata,"stop_hold");
				  }
				 else if ( mindata.price < tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2 ) {
					 Close(tr,mindata,"stop_opi_less");
				 }
			 }
			 else if (tr.Direction ==1  ) {


				 if (mindata.price > tr.StopPrice)	 {
					 Close(tr,mindata,"stop_hold");
				 }
				 else if (mindata.price > tr.OpenPrice && mindata.OpenInterest < tr.OpenOPI/2) {
					 Close(tr,mindata,"stop_opi_less");

				 }
			 }


		 }//exist 存在持仓单

	}
	else{
//		printf ("无分钟数据 -----> check min_database : %s    tablename: %s \n",min_database.c_str(),tablename.c_str());
	}


//	printf ("check tablename : %s \n",tablename.c_str());


}


void RealTradeTest::EndTimeClose(const char* pinstrumentid)
{

//	printf ("EndTimeClose begin  当前日期: %s    ---> %s \n", this->GetCusor_DaySqlTime().c_str(),this->GetCusor_MinuteTime().c_str());

	DateUtil dateutil;
	std::string str_tradingday = dateutil.ConvertSqlTimetoDate(this->GetCusor_DaySqlTime().c_str());

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	MysqlTestRecord mysqltestrecord;
	MysqlHisMinData mysqlhismindata;
	MysqlMaincontract mysqlmaincontract;
	MysqlDayLine mysqldayline;

	HxDayLineData today=mysqldayline.Find_DataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	HxDayLineData preday =mysqldayline.Find_PreDataBySqlTime(pinstrumentid,this->GetCusor_DaySqlTime().c_str());

	MainContractData maincontractdata = mysqlmaincontract.Find_DatabyProductID(productid);



	std::string min_database;
	min_database = "min_";
	min_database.append(productid);
//	printf ("check min_database : %s \n",min_database.c_str());
	mysqlhismindata.Set_Database(min_database);

	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(str_tradingday);

	if(mysqlhismindata.Exist_DataBySqlTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str())){
//		printf ("check min_database : %s    tablename: %s \n",min_database.c_str(),tablename.c_str());
		MinuteLineData mindata = mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),this->GetCusor_MinuteTime().c_str());

//		double stoprange = mindata.price *0.02;
	//	int p= (this->GetInitialMoney()*this->GetRiskRate())/(stoprange* maincontractdata.VolumeMultiple);
	//	int position = p ;
//		 printf ("check %s mindata : %s     --> price % .3f \n",tablename.c_str(),mindata.MinuteTime,mindata.price);


		 if( mysqltestrecord.Exist_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0) ) {

			 TestRecordData tr =mysqltestrecord.Find_RecordByInstrumentStatus(this->GetTableRecord().c_str(),pinstrumentid,0);
			 double profit;

			 if (tr.Direction ==0)	 {
				 profit = (double)maincontractdata.VolumeMultiple  *(mindata.price- tr.OpenPrice) *tr.Volume-tr.Commission;
			 }
			 else if (tr.Direction ==1)	 {
				 profit = (double)maincontractdata.VolumeMultiple  *(tr.OpenPrice - mindata.price ) *tr.Volume-tr.Commission;
			 }

			 if (tr.Direction ==0  )	 {

					 Close(tr,mindata,"stop_end_time");

			 }
			 else if (tr.Direction ==1  ) {

					 Close(tr,mindata,"stop_end_time");

			 }


		 }//exist 存在持仓单

	}
	else{
//		printf ("无分钟数据 -----> check min_database : %s    tablename: %s \n",min_database.c_str(),tablename.c_str());
	}







//	printf ("check tablename : %s \n",tablename.c_str());


}
void RealTradeTest::Create_SectionTestReportXlsFile(std::string pathfile)	{

		workbook wb;
		xlslib_core::xf_t* xf = wb.xformat();

		xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
		xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
		xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
		xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

		worksheet* ws;
		ws = wb.sheet("section_testreport");

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
		ws->colwidth(i++,256*10);

		i = start;

		ws->label(1,i++,"序号",xf);
		ws->label(1,i++,"版块",xf);
		ws->label(1,i++,"总交易次数",xf);
		ws->label(1,i++,"总盈利额",xf);
		ws->label(1,i++,"总亏损额",xf);
		ws->label(1,i++,"总盈利次数",xf);

		ws->label(1,i++,"多头交易数",xf);
		ws->label(1,i++,"多头盈利数",xf);
		ws->label(1,i++,"空头交易数",xf);
		ws->label(1,i++,"空头盈利数",xf);
		ws->label(1,i++,"平均盈利",xf);
		ws->label(1,i++,"平均亏损",xf);

		ws->label(1,i++,"盈亏比",xf);
		ws->label(1,i++,"胜率",xf);
		ws->label(1,i++,"期望值",xf);
		ws->label(1,i++,"佣金",xf);

//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);

		range * _range;

		_range = ws->rangegroup(1,1,1,i-1);
		_range->cellcolor(CLR_GOLD);


		MysqlTestAsset mysqltestasset;
		MysqlTestSecAssess mysqltestsecasses;
		std::string totaltablename;
		totaltablename.append(this->GetStrategy().c_str());
		totaltablename.append(this->GetYear().c_str());

		vector<SectionAssessData>  assess_list;
		mysqltestsecasses.Find_AllLists(assess_list);

		printf("最终汇总表   -------%s   \n ",this->GetTableReport().c_str());
		int y=0;

		int row;

		for (auto &item:assess_list)	{

		  row=y+2;
		  ws->number(row,1,y+1,xf);
		  ws->label(row,2,item.Section,xf);

		  MysqlTestRecord mysqltestrecord;
		  int count =mysqltestrecord.Count_RecordBySection(this->GetTableRecord().c_str(),item.Section);
		  char buf[20];
		  ws->number(row,3,count,xf);


		  double profit =mysqltestrecord.SumProfit_RecordBySection(this->GetTableRecord().c_str(),item.Section);
		  ws->number(row,4,profit,xf);
//
////		  sprintf(buf,"%.2f",item.TotalLoseProfit);
//		  ws->number(row,5,item.TotalLoseProfit,xf);
//
////		  sprintf(buf,"%d",item.TradeCount);
		  int win_count = mysqltestrecord.Count_WinRecordBySection(this->GetTableRecord().c_str(),item.Section);
		  ws->number(row,6,win_count,xf);
//		  tradecount+=item.TradeCount;
////		  sprintf(buf,"%d",item.WinCount);
//		  ws->number(row,7,item.WinCount,xf);
//		  wincount+=item.WinCount;
//
//		  ws->number(row,8,item.LongCount,xf);
//		  ws->number(row,9,item.WinCount_Long,xf);
//		  ws->number(row,10,item.ShortCount,xf);
//		  ws->number(row,11,item.WinCount_Short,xf);
//
//		  total_tradecount_long+=item.LongCount;
//		  total_wincount_long+=item.WinCount_Long;
//
//		  total_tradecount_short+=item.ShortCount;
//		  total_wincount_short+=item.WinCount_Short;
//
////		  sprintf(buf,"%.2f",item.AverageWinProfit);
//		  ws->number(row,12,item.AverageWinProfit,xf);
//
//
////		  sprintf(buf,"%.2f",item.AverageLoseProfit);
//		  ws->number(row,13,item.AverageLoseProfit,xf);
//
////		  sprintf(buf,"%.2f",item.WinLoseRate);
		  double winrate ;
		  if(count>0){
			  winrate = (double)win_count/(double)count;
		  }
		  else {
			  winrate=0.0;
		  }
		  ws->number(row,14,winrate,xf);
//
////		  sprintf(buf,"%.2f%%",item.WinRate*100);
//		  ws->number(row,15,item.WinRate,xf);
//
////		  sprintf(buf,"%.2f",item.Expectation);
//		  ws->number(row,16,item.Expectation,xf);
//
////		  sprintf(buf,"%d",item.ErrorMinuteData);
//		  ws->number(row,17,item.ErrorMinuteData,xf);
//
////		  sprintf(buf,"%.2f",item.Asset);
//		  ws->number(row,18,item.Commission,xf);
//		  totalcommission+=item.Commission;
//		  ws->number(row,19,item.Asset,xf);
//		  totalprofit+=item.Asset;
//		  ws->label(row,20,item.Section,xf);
//
//		  if(item.Expectation <0 )	{
//			range * range_r;
//
//			range_r = ws->rangegroup(row,1,row,20);
//			range_r->cellcolor(CLR_GOLD);
//
////				_range = ws->rangegroup(row,1,row,18);
////				_range->cellcolor(CLR_GOLD);
////				printf("    %s 品种期望值为负值  \n",item.Product);
//			}

			y++;
		}

//		  ws->number(row,12,mysqltestasset.Get_MaxbySameTimeHold(totaltablename.c_str()),xf);
//		 ws->number(row,13,mysqltestasset.Get_MaxbyMarginRate(totaltablename.c_str()),xf);

		wb.Dump(pathfile);


		vector<SectionAssessData>().swap(assess_list);
//						printf("Create_TestReportXlsFile  end! ...\n");

}
void RealTradeTest::Create_TestReportXlsFile(std::string pathfile)	{

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

}

void RealTradeTest::Create_TestRecordXlsFile(std::string pathfile)
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
			ws->label(1,22,"OpenOPI",xf);
			ws->label(1,23,"CloseOPI",xf);

			int colume_size =23;

		range * _range;

		_range = ws->rangegroup(1,1,1,colume_size);
		_range->cellcolor(CLR_GOLD);


		MysqlTestRecord mysqltestrecord;

		if (mysqltestrecord.Exist_Table(this->GetTableRecord().c_str()))	{

			vector<TestRecordData>  list;
			mysqltestrecord.Find_AllLists(list,this->GetTableRecord().c_str());

			int i=0;
			for (auto &record:list)		{

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

			  ws->number(row,9,(record.OpenPrice-record.DayOpenPrice)/record.DayOpenPrice,xf);

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


			  if(record.Profit <0 )	{
					range * range_r;

					range_r = ws->rangegroup(row,1,row,colume_size);
					range_r->cellcolor(CLR_GOLD);

		//				_range = ws->rangegroup(row,1,row,18);
		//				_range->cellcolor(CLR_GOLD);
		//				printf("    %s 品种期望值为负值  \n",item.Product);
				}

			i++;

			}
			vector<TestRecordData> ().swap(list);

		}

		wb.Dump(pathfile);


}

