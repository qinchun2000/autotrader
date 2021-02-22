#include "hisdata.hpp"

HisData::HisData()
{

}

void HisData::SetDatabase(const char* database){
	this->_database_hisminute=database;
}
std::string HisData::GetDatabase(){
	return this->_database_hisminute;
}
void HisData::DropEmpty_MinuteTable(const char* year)
{

	MysqlMaincontract  mysqlmaincontract;
	MysqlUtil mysqlutil("test");
	vector<MainContractData>  list;


	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)
	{

		vector<std::string> instrument_list;

		TradeTest tradetest;
		tradetest.GetInstrumentList(instrument_list,year,item);


		for (auto &ins:instrument_list)
		{
			printf(  " 合约列表 ---> %s \n",ins.c_str());
			DropInstrument_MinutelineTable(ins.c_str());
		}


		vector<std::string>().swap(instrument_list);

	}

	vector<MainContractData>().swap(list);
}

void HisData::CheckAllCsvData(const char* year)
{

	MysqlMaincontract  mysqlmaincontract;
	MysqlUtil mysqlutil("test");
	vector<MainContractData>  list;


	mysqlmaincontract.Find_AllList(list);
	for (auto &item:list)
	{

		vector<std::string> instrument_list;

		TradeTest tradetest;
		tradetest.GetInstrumentList(instrument_list,year,item);


		for (auto &ins:instrument_list)
		{
			printf(  " 合约列表 ---> %s \n",ins.c_str())	;

			if(strcmp(item.ProductID,"a")==0)
					{
//					mysqlutil.CheckHttpDayLineByInstrument(ins.c_str());

//					DropInstrument_MinutelineTable(ins.c_str());

					CheckInstrument_HisMinuteline(ins.c_str());
					}


//			if(strcmp(item.ProductID,"SR")==0 || strcmp(item.ProductID,"a")==0 || strcmp(item.ProductID,"WH")==0 || strcmp(item.ProductID,"bu")==0)
//			{
//
//				mysqlutil.CheckHttpDayLineByInstrument(ins.c_str());
//					CheckInstrument_Minuteline(ins.c_str());
//
//
//			}
//
//			else
//			{
////				if(strcmp(item.ProductID,"rb")==0)
//				{
//
//
//				}
////				else
//				{
//
//				}
//
//			}


		}


		vector<std::string>().swap(instrument_list);

	}

	vector<MainContractData>().swap(list);
}
void HisData::CheckInstrument_Minuteline(const char* pinstrumentid)
{
	MysqlDayLine mysqldayline;

	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());


	vector<HxDayLineData>  dayline_list;
	mysqldayline.Find_AllList(dayline_list,pinstrumentid);

	double endday_close;
	std::string endday;
	int i=0;
	for (auto &dayline:dayline_list)
	{

			std::string str_current = dayline.Time;

			char tradingday[10];
			sprintf(tradingday,"%s%s%s",str_current.substr(0,4).c_str(),str_current.substr(5,2).c_str(),str_current.substr(8,2).c_str());

			printf("tradingday   ---> %s    \n",tradingday);

			std::string min_database;
			min_database = "min_";
			min_database.append(productid);
			mysqlhismindata.Set_Database(min_database);

			std::string tablename;
			tablename=pinstrumentid;
			tablename.append("_");
			tablename.append(tradingday);

			std::string path = GetPath(pinstrumentid,dayline.Time);
			std::string filename = GetFileName(pinstrumentid,dayline.Time);

			if(!mysqlhismindata.Exist_Table(tablename.c_str()) )
				{
					  ConvertDayTicks_Minuteline(path.c_str(),filename.c_str(),pinstrumentid,dayline.Time);

				}
			else
			{

//				if (mysqlhismindata.Count_AllListsbyTableName(tablename.c_str()) ==0 )
				{
					 printf ("CheckInstrument_Minuteline ------->  [%s]合约 [%s]分钟数据为空，重新补充！！ \n",pinstrumentid,tradingday);
					 ConvertDayTicks_Minuteline(path.c_str(),filename.c_str(),pinstrumentid,dayline.Time);
				}
			}



//		printf()
	}
	vector<HxDayLineData>().swap(dayline_list);
}

void HisData::CheckInstrument_HisMinuteline(const char* pinstrumentid)
{
	MysqlDayLine mysqldayline;

	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());


	vector<HxDayLineData>  dayline_list;
	mysqldayline.Find_AllList(dayline_list,pinstrumentid);

	double endday_close;
	std::string endday;
	int i=0;
	for (auto &dayline:dayline_list)
	{

			std::string str_current = dayline.Time;

			char tradingday[10];
			sprintf(tradingday,"%s%s%s",str_current.substr(0,4).c_str(),str_current.substr(5,2).c_str(),str_current.substr(8,2).c_str());

			printf("tradingday   ---> %s    \n",tradingday);

			std::string min_database;
			min_database = "hismin_";
			min_database.append(pinstrumentid);
			this->SetDatabase(min_database.c_str());
			mysqlhismindata.Set_Database(this->GetDatabase().c_str());

			std::string tablename;
			tablename=pinstrumentid;
			tablename.append("_");
			tablename.append(tradingday);

			std::string path = GetPath(pinstrumentid,dayline.Time);
			std::string filename = GetFileName(pinstrumentid,dayline.Time);

			if(!mysqlhismindata.Exist_Table(tablename.c_str()) )
				{
					  ConvertDayTicks_Minuteline(path.c_str(),filename.c_str(),pinstrumentid,dayline.Time);

				}
			else
			{

//				if (mysqlhismindata.Count_AllListsbyTableName(tablename.c_str()) ==0 )
				{
					 printf ("CheckInstrument_Minuteline ------->  [%s]合约 [%s]分钟数据为空，重新补充！！ \n",pinstrumentid,tradingday);
					 ConvertDayTicks_Minuteline(path.c_str(),filename.c_str(),pinstrumentid,dayline.Time);
				}
			}



//		printf()
	}
	vector<HxDayLineData>().swap(dayline_list);
}

void HisData::DropInstrument_MinutelineTable(const char* pinstrumentid)
{
	MysqlDayLine mysqldayline;

	MysqlHisMinData mysqlhismindata;

	DateUtil dateutil;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());


	vector<HxDayLineData>  dayline_list;
	mysqldayline.Find_AllList(dayline_list,pinstrumentid);

	double endday_close;
	std::string endday;
	int i=0;
	for (auto &dayline:dayline_list)
	{

			std::string str_current = dayline.Time;

			char tradingday[10];
			sprintf(tradingday,"%s%s%s",str_current.substr(0,4).c_str(),str_current.substr(5,2).c_str(),str_current.substr(8,2).c_str());

			printf("tradingday   ---> %s    \n",tradingday);

			std::string min_database;
			min_database = "min_";
			min_database.append(productid);
			mysqlhismindata.Set_Database(min_database);

			std::string tablename;
			tablename=pinstrumentid;
			tablename.append("_");
			tablename.append(tradingday);



			std::string year=str_current.substr(0,4);
			int y=atoi(year.c_str());
			if (  y<=2015 )
			{
				if(mysqlhismindata.Exist_Table(tablename.c_str()) && mysqlhismindata.Count_AllListsbyTableName(tablename.c_str())==0)
					{

					printf("drop table ----> %s  \n",tablename.c_str());
					mysqlhismindata.DropbyTableName(tablename.c_str());
					}
			}






//		printf()
	}
	vector<HxDayLineData>().swap(dayline_list);
}
void HisData::ConvertDayTicks_Minuteline(const char* path,const char*filename,const char* pinstrumentid,
		const char* sqltime)
{
	MysqlHisMinData mysqlhismindata;
	MysqlDateTime mysqldatetime;

	DateUtil dateutil;
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	printf("----database    %s    \n",this->GetDatabase().c_str());
	mysqlhismindata.Set_Database(this->GetDatabase().c_str());

	DateTime predata= mysqldatetime.Find_PreDateTime("alltime",sqltime);

	printf("%s[%s]   path: %s      filename: %s   \n",pinstrumentid,sqltime,path,filename);

	std::string str_current = sqltime;
	char tradingday[10];
	sprintf(tradingday,"%s%s%s",str_current.substr(0,4).c_str(),str_current.substr(5,2).c_str(),str_current.substr(8,2).c_str());

	TickToKlineUtil util;

	vector<KLineDataType>  minuteline_list;

	util.ReadTickData_CSV(minuteline_list,path,filename,predata.Time);
	std::string tablename;
	tablename=pinstrumentid;
	tablename.append("_");
	tablename.append(tradingday);

	if(minuteline_list.size()>0)
	{
		 mysqlhismindata.CreateTable(tablename.c_str());
	}
	for (auto &data:minuteline_list)
		{

//			printf("%s 分钟线 [%s]  open %.3f    high %.3f    low %.3f   close  %.3f   volume %d  opi %d \n",pinstrumentid,data.Time,data.open_price ,
//					data.high_price,data.low_price , data.close_price , data.volume,data.opi );

			MinuteLineData info;
			strcpy(info.MinuteTime,data.Time);
			info.price = data.close_price;
			info.Volume = data.volume;
			info.OpenInterest = data.opi;
			info.AvePrice =0;
			info.Amount =0;

			if(!mysqlhismindata.Exist_DataBySqlTime(tablename.c_str(),info.MinuteTime))
				{
					printf("插入分钟数据[%s]  close  %.3f   volume %d  opi %d  \n",info.MinuteTime,info.price,info.Volume,info.OpenInterest);
					mysqlhismindata.Insert(tablename.c_str(),info);
				}
			else
			{
				MinuteLineData temp=mysqlhismindata.Find_DatabyMinuteTime(tablename.c_str(),info.MinuteTime);
				if (temp.price != temp.price   || temp.Volume != info.Volume  || temp.OpenInterest != info.OpenInterest)
				{
					printf("原分钟数据  [%s]  close  %.3f   volume %d  opi %d  \n",temp.MinuteTime,temp.price,temp.Volume,temp.OpenInterest);
					printf("更新分钟数据[%s]  close  %.3f   volume %d  opi %d  \n",info.MinuteTime,info.price,info.Volume,info.OpenInterest);
					mysqlhismindata.Update(tablename.c_str(),info);
				}
				else
				{
					printf("已有分钟数据---完全相同[%s]  close  %.3f   volume %d  opi %d  \n",info.MinuteTime,info.price,info.Volume,info.OpenInterest);
				}
			}

		}
	vector<KLineDataType>().swap(minuteline_list);
}
std::string HisData::GetPath(const char*pinstrumentid,const char* time)
{
	std::string path;
	DateUtil dateutil;


	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());


	std::string start_2014_1 = "2014-01-01 00:00:00";
	std::string end_2014_1 = "2014-02-28 00:00:00";

	std::string start_2014_2 = "2014-03-01 00:00:00";
	std::string end_2014_2 = "2014-05-30 00:00:00";

	std::string start_2014_3 = "2014-06-01 00:00:00";
	std::string end_2014_3 = "2014-08-31 00:00:00";


	std::string start_2014_4 = "2014-09-01 00:00:00";
	std::string end_2014_4 = "2014-12-31 00:00:00";


	std::string start_2015_1 = "2015-01-05 00:00:00";
	std::string end_2015_1 = "2015-03-31 00:00:00";

	std::string start_2015_2 = "2015-04-01 00:00:00";
	std::string end_2015_2 = "2015-06-30 00:00:00";

	std::string start_2015_3 = "2015-07-01 00:00:00";
	std::string end_2015_3 = "2015-09-30 00:00:00";

	std::string start_2015_4 = "2015-10-01 00:00:00";
	std::string end_2015_4 = "2015-12-31 00:00:00";


	std::string str_current = time;
	printf("str_current   ---> %s   \n",time);

	std::string year=str_current.substr(0,4);

	char tradingday[10];
	sprintf(tradingday,"%s%s%s",str_current.substr(0,4).c_str(),str_current.substr(5,2).c_str(),str_current.substr(8,2).c_str());

	time_t t_start_2014_1 =dateutil.ConvertSqlTimetoTimet(start_2014_1.c_str());
	time_t t_end_2014_1 =dateutil.ConvertSqlTimetoTimet(end_2014_1.c_str());

	time_t t_start_2014_2 =dateutil.ConvertSqlTimetoTimet(start_2014_2.c_str());
	time_t t_end_2014_2 =dateutil.ConvertSqlTimetoTimet(end_2014_2.c_str());

	time_t t_start_2014_3 =dateutil.ConvertSqlTimetoTimet(start_2014_3.c_str());
	time_t t_end_2014_3 =dateutil.ConvertSqlTimetoTimet(end_2014_3.c_str());

	time_t t_start_2014_4 =dateutil.ConvertSqlTimetoTimet(start_2014_4.c_str());
	time_t t_end_2014_4 =dateutil.ConvertSqlTimetoTimet(end_2014_4.c_str());


	time_t t_start_2015_1 =dateutil.ConvertSqlTimetoTimet(start_2015_1.c_str());
	time_t t_end_2015_1 =dateutil.ConvertSqlTimetoTimet(end_2015_1.c_str());

	time_t t_start_2015_2 =dateutil.ConvertSqlTimetoTimet(start_2015_2.c_str());
	time_t t_end_2015_2 =dateutil.ConvertSqlTimetoTimet(end_2015_2.c_str());

	time_t t_start_2015_3 =dateutil.ConvertSqlTimetoTimet(start_2015_3.c_str());
	time_t t_end_2015_3 =dateutil.ConvertSqlTimetoTimet(end_2015_3.c_str());

	time_t t_start_2015_4 =dateutil.ConvertSqlTimetoTimet(start_2015_4.c_str());
	time_t t_end_2015_4 =dateutil.ConvertSqlTimetoTimet(end_2015_4.c_str());

	time_t t_current = dateutil.ConvertSqlTimetoTimet(time);

	std::string prefix ="/home/qc/hisdata/";
	if (t_current >= t_start_2014_1 && t_current <=t_end_2014_1)
			{
				char buf[100];
					sprintf(buf,"%s%s/%s_1/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

					path=buf;
			}
	if (t_current >= t_start_2014_2 && t_current <=t_end_2014_2)
			{
				char buf[100];
					sprintf(buf,"%s%s/%s_2/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

					path=buf;
			}
	if (t_current >= t_start_2014_3 && t_current <=t_end_2014_3)
			{
				char buf[100];
					sprintf(buf,"%s%s/%s_3/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

					path=buf;
			}
	if (t_current >= t_start_2014_4 && t_current <=t_end_2014_4)
		{
			char buf[100];
				sprintf(buf,"%s%s/%s_4/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

				path=buf;
		}

	if (t_current >= t_start_2015_1 && t_current <=t_end_2015_1)
	{
		char buf[100];
			sprintf(buf,"%s%s/%s_1/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

			path=buf;
	}
	if (t_current >= t_start_2015_2 && t_current <=t_end_2015_2)
		{
			char buf[100];
				sprintf(buf,"%s%s/%s_2/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

				path=buf;
		}
	if (t_current >= t_start_2015_3 && t_current <=t_end_2015_3)
		{
			char buf[100];
				sprintf(buf,"%s%s/%s_3/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

				path=buf;
		}
	if (t_current >= t_start_2015_4 && t_current <=t_end_2015_4)
		{
			char buf[100];
				sprintf(buf,"%s%s/%s_4/%s/%s/",prefix.c_str(),year.c_str(),year.c_str(),productid,tradingday);

				path=buf;
		}
	return path;
}

std::string HisData::GetFileName(const char* pinstrumentid ,const char* sqltime)
{
	std::string filename;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	char number[10];
	strcpy(number,codeutil.GetNumber(pinstrumentid).c_str());
	std::string str_number =number;
	std::string contract_year ;

	if(str_number.size()==3)
	{
		contract_year=str_number.substr(0,1);
	}
	else if (str_number.size()==4)
	{
		contract_year=str_number.substr(0,2);
	}


	std::string contract_month =str_number.substr(str_number.size()-2,2);
	printf("number ---> %s  contract_month --->%s \n",number,contract_month.c_str());

	std::string str_current = sqltime;
	char tradingday[10];
	sprintf(tradingday,"%s%s%s",str_current.substr(0,4).c_str(),str_current.substr(5,2).c_str(),str_current.substr(8,2).c_str());


	printf("str_current   ---> %s   \n",sqltime);

	std::string year=str_current.substr(0,4);

	int m = atoi(contract_year.c_str())%2;

	bool iSR = strcmp(productid,"SR")==0;
	bool iWH = strcmp(productid,"WH")==0;
	bool ia = strcmp(productid,"a")==0;
	bool ibu = strcmp(productid,"bu")==0;
	if (iSR || iWH || ia || ibu)
	{

		if (m==0)// 偶数年
		{
			char buf[50];
			sprintf(buf,"%sX%s_%s.csv",productid,contract_month.c_str(),tradingday);
			filename=buf;
		}
		else
		{
			char buf[50];
			sprintf(buf,"%sY%s_%s.csv",productid,contract_month.c_str(),tradingday);
			filename=buf;
		}
	}else
	{
		char buf[50];
		sprintf(buf,"%s%s_%s.csv",productid,contract_month.c_str(),tradingday);
		filename=buf;
	}



	return filename;
}
