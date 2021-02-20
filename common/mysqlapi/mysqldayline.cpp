#include "mysqldayline.hpp"



MysqlDayLine::MysqlDayLine()
{
	std::string database="dayline";

	if (!IsAliveConn())	{
		if(!Connect())	{

			return;
		}
	}
	this->Set_Database(database);
}

void MysqlDayLine::CreateTable(const char* tablename)
{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
						"Time DATETIME NOT NULL PRIMARY KEY, "
						"Open DOUBLE,"
						"High DOUBLE, "
						"Low DOUBLE, "
						"Close DOUBLE, "
						"LastClose DOUBLE, "
						"PreSettlementPrice DOUBLE, "
						"SettlementPrice DOUBLE, "
						"OpenInterest INT, "
						"Amount DOUBLE, "
						"Volume DOUBLE) default character set utf8 ",tablename);

		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}

void MysqlDayLine::Insert(const char* tablename,HxDayLineData data)
{

	char buf[2048];
	std::string str_sql;

	std::string sqltime=data.Time;
	std::string datetime;

	if(sqltime.length()==8){

		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str());
		datetime=tem;
	}
	else if(sqltime.length()==14){
		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str()	);
		datetime=tem;
	}
	else if(sqltime.length()==19){
		datetime=sqltime;
	}


	if(datetime.length()==19){
		sprintf(buf,"INSERT INTO %s(Time, Open,High,Low,Close,LastClose,Amount,Volume) "
				"VALUES ('%s', '%.3f','%.3f','%.3f','%.3f','%.3f','%.3f','%.3f')",
				tablename,datetime.c_str(),data.Open,data.High,data.Low,data.Close,data.LastClose,data.Amount,data.Volume);

		str_sql=buf;

		this->Execute(str_sql);
	}


}

void MysqlDayLine::Update(const char* tablename,HxDayLineData data)
{


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"UPDATE %s SET LastClose='%.3f' WHERE Time='%s' ",tablename,data.LastClose,data.Time);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlDayLine::UpdateAllPrice(const char* tablename,HxDayLineData data)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"UPDATE %s SET Open='%.3f',High='%.3f',Low='%.3f',Close='%.3f',LastClose='%.3f' WHERE Time='%s' ",
			tablename,data.Open,data.High,data.Low,data.Close,data.LastClose,data.Time);

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
void MysqlDayLine::InsertDepthmarket(const char* tablename,HxDayLineData data)
{

	char buf[2048];
	std::string str_sql;

	std::string sqltime=data.Time;
	std::string datetime;

	if(sqltime.length()==8){

		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str());
		datetime=tem;
	}
	else if(sqltime.length()==14){
		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str()	);
		datetime=tem;
	}
	else if(sqltime.length()==19){
		datetime=sqltime;
	}


	if(datetime.length()==19){
		sprintf(buf,"INSERT INTO %s(Time, Open,High,Low,Close,"
				"LastClose,PreSettlementPrice,SettlementPrice,OpenInterest,Amount,Volume) "
				"VALUES ('%s', '%.3f','%.3f','%.3f','%.3f',"
				"'%.3f','%.3f','%.3f','%d','%.3f','%.3f')",
				tablename,datetime.c_str(),data.Open,data.High,data.Low,data.Close,
				data.LastClose,data.PreSettlementPrice,data.SettlementPrice,data.OpenInterest,data.Amount,data.Volume);

		str_sql=buf;

		this->Execute(str_sql);
	}

}
void MysqlDayLine::UpdateDepthmarket(const char* tablename,HxDayLineData data)
{
	char buf[2048];
	std::string str_sql;

	std::string sqltime=data.Time;
	std::string datetime;

	if(sqltime.length()==8){

		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str());
		datetime=tem;
	}
	else if(sqltime.length()==14){
		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str()	);
		datetime=tem;
	}
	else if(sqltime.length()==19){
		datetime=sqltime;
	}

	sprintf(buf,"UPDATE %s SET PreSettlementPrice='%.3f',SettlementPrice='%.3f',OpenInterest='%d' WHERE Time='%s' ",
			tablename,
			data.PreSettlementPrice,data.SettlementPrice,data.OpenInterest,
			datetime.c_str());

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlDayLine::UpdateAllPriceDepthmarket(const char* tablename,HxDayLineData data){

	char buf[2048];
	std::string str_sql;

	std::string sqltime=data.Time;
	std::string datetime;

	if(sqltime.length()==8){

		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str());
		datetime=tem;
	}
	else if(sqltime.length()==14){
		char tem[20];
		sprintf(tem,"%s-%s-%s 00:00:00",sqltime.substr(0,4).c_str(),sqltime.substr(4,2).c_str(),sqltime.substr(6,2).c_str()	);
		datetime=tem;
	}
	else if(sqltime.length()==19){
		datetime=sqltime;
	}

	sprintf(buf,"UPDATE %s SET Open='%.3f',High='%.3f',Low='%.3f',Close='%.3f',LastClose='%.3f',"
			"PreSettlementPrice='%.3f',SettlementPrice='%.3f',OpenInterest='%d',Amount='%.3f',Volume='%.3f' WHERE Time='%s' ",
			tablename,
			data.Open,data.High,data.Low,data.Close,data.LastClose,
			data.PreSettlementPrice,data.SettlementPrice,data.OpenInterest,data.Amount,data.Volume,
			datetime.c_str());

	str_sql=buf;
	this->ExecuteUpdate(str_sql);

}
std::string MysqlDayLine::GetLastDateTime(const char *tablename)
{

	std::string  info;
	char buf[2048];
	std::string str_sql;

//	info="1997-01-01 00:00:00";

	sprintf(buf,"SELECT Time FROM %s ORDER BY Time desc limit 1 ",tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

			if(this->getRowCount()==1)	{
					this->getFirst();
					 info =  getValueString("Time");
				}
		}
	this->Empty();
	  return info;

}


// 20180121093031
bool MysqlDayLine::Exist_DateTime(const char *tablename,std::string datetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;




	char tem[20];
	sprintf(tem,"%s-%s-%s %s:%s:%s",datetime.substr(0,4).c_str(),datetime.substr(4,2).c_str(),datetime.substr(6,2).c_str(),
	datetime.substr(8,2).c_str(),datetime.substr(10,2).c_str(),datetime.substr(12,2).c_str());
	sprintf(buf,"SELECT Time FROM %s WHERE Time='%s' ",tablename,tem);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		if(this->getRowCount()>0)
		{
			ret=true;
		}


	}
	this->Empty();
	return ret;
}

bool MysqlDayLine::Exist_DatabySqlTime(const char *tablename,const char * sqltime)
{
	bool ret=false;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT Time FROM %s WHERE Time='%s' ",tablename,sqltime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		if(this->getRowCount()>0)	{
			ret=true;
		}
	}
	this->Empty();
	return ret;
}

void MysqlDayLine::CopyData(HxDayLineData *data,const char *tablename)
{
	printf("aaaaa\n");
	int columecount =this->Count_ColumnsbyTableName(tablename);

	if(this->getRowCount()>0 && columecount==11)		{
				printf("22222222\n");
		strcpy(data->Time,getValueString("Time").c_str());
		data->Close=getValueDouble("Close");
		data->LastClose=getValueDouble("LastClose");
		data->Open = getValueDouble("Open");
		  data->High = getValueDouble("High");
		  data->Low = getValueDouble("Low");
		  data->PreSettlementPrice=getValueDouble("PreSettlementPrice");
		  data->SettlementPrice=getValueDouble("SettlementPrice");
		  data->OpenInterest=getValueInt("OpenInterest");
		  data->Volume =getValueDouble("Volume");
		  data->Amount = getValueDouble("Amount");
	}
	else if (this->getRowCount()>0 && columecount==8){

			printf("4444444444444\n");
		strcpy(data->Time,getValueString("Time").c_str());
		data->Close=getValueDouble("Close");
		data->LastClose=getValueDouble("LastClose");
		data->Open = getValueDouble("Open");
	  data->High = getValueDouble("High");
	  data->Low = getValueDouble("Low");
	  data->Volume =getValueDouble("Volume");
	  data->Amount = getValueDouble("Amount");
	}
	else{
		strcpy(data->Time,getValueString("Time").c_str());
		data->Close=getValueDouble("Close");
		data->LastClose=getValueDouble("LastClose");
		data->Open = getValueDouble("Open");
	  data->High = getValueDouble("High");
	  data->Low = getValueDouble("Low");
	  data->Volume =getValueDouble("Volume");
	  data->Amount = getValueDouble("Amount");

	}
}

void MysqlDayLine::Find_UpdateDateList(vector<HxDayLineData> &temp,const char *tablename)
{

	HxDayLineData data;


	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT Time FROM %s ORDER BY Time DESC limit 80 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{

			strcpy(data.Time,getValueString("Time").c_str());

			  temp.push_back(data);
			  this->getNext();
			}
	}
	this->Empty();
	reverse(temp.begin(), temp.end());
}


void MysqlDayLine::Find_AllList(vector<HxDayLineData> &temp,const char *tablename)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time ASC ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
				HxDayLineData data;
				strcpy(data.Time,getValueString("Time").c_str());
				data.Close=getValueDouble("Close");
				data.LastClose=getValueDouble("LastClose");
				data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
			  data.LastClose=getValueDouble("LastClose");
				temp.push_back(data);
				this->getNext();
			}
	}
	this->Empty();

}

void MysqlDayLine::Find_FiveDayLineDataLists(vector<FiveDayLineData> &temp,const char *tablename,int count)
{
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time DESC LIMIT %d ",tablename,count);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++){
			FiveDayLineData data;
				strcpy(data.Time,getValueString("Time").c_str());
				data.Close=getValueDouble("Close");
//				data.LastClose=getValueDouble("LastClose");
				data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
				temp.push_back(data);
				this->getNext();
			}
	}

	reverse(temp.begin(),temp.end());
	this->Empty();

}

void MysqlDayLine::Find_FiveDayLineDataListsbyDate(vector<FiveDayLineData> &temp,const char *tablename,const char* date,int count)
{
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time<'%s' ORDER BY Time DESC LIMIT %d ",tablename,date,count);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++){
			FiveDayLineData data;

//				printf("Find_ListsbyDate \n");
//			strcpy(data.InstrumentID,tablename);
				strcpy(data.Time,getValueString("Time").c_str());
				data.Close=getValueDouble("Close");
//				data.LastClose=getValueDouble("LastClose");
				data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");

//			  CopyData(&data,tablename);
				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());

	int n=1;
	for (auto &data:temp){
		data.Number=n;
		n++;
//		printf("Find_FiveDayLineDataListsbyDate  number:%d   instrumentid:%s    time:%s close:%.3f  \n",data.Number,tablename,
//				data.Time,data.Close);

	}

	this->Empty();

}

void MysqlDayLine::Find_ListsLimit(vector<HxDayLineData> &temp,const char *tablename,int count)
{
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time DESC LIMIT %d ",tablename,count);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++){
				HxDayLineData data;
				strcpy(data.Time,getValueString("Time").c_str());
				data.Close=getValueDouble("Close");
				data.LastClose=getValueDouble("LastClose");
				data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
				temp.push_back(data);
				this->getNext();
			}
	}
	this->Empty();

}

void MysqlDayLine::Find_ListsbyDate(vector<HxDayLineData> &temp,const char *tablename,const char* date,int count)
{
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time<'%s' ORDER BY Time DESC LIMIT %d ",tablename,date,count);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++){
				HxDayLineData data;

//				printf("Find_ListsbyDate \n");
				strcpy(data.Time,getValueString("Time").c_str());
				data.Close=getValueDouble("Close");
				data.LastClose=getValueDouble("LastClose");
				data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");

//			  CopyData(&data,tablename);
				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}

void MysqlDayLine::Find_ListsbyOpenDate(vector<HxDayLineData> &temp,const char *tablename,const char* opendate)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time>='%s' ORDER BY Time ASC ",tablename,opendate);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++){
				HxDayLineData data;
				strcpy(data.Time,getValueString("Time").c_str());
				data.Close=getValueDouble("Close");
				data.LastClose=getValueDouble("LastClose");
				data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
				temp.push_back(data);
				this->getNext();
			}
	}

	this->Empty();

}
HxDayLineData MysqlDayLine::Find_DataBySqlTime(const char *tablename,const char* sqltime)
{
	HxDayLineData data;
//printf("1111111\n");
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time='%s' ",tablename,sqltime);
int columecount =this->Count_ColumnsbyTableName(tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		if(this->getRowCount()>0 && columecount==11)		{
//			printf("22222222\n");
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
			  data.PreSettlementPrice=getValueDouble("PreSettlementPrice");
			  data.SettlementPrice=getValueDouble("SettlementPrice");
			  data.OpenInterest=getValueInt("OpenInterest");
			  data.Volume =getValueDouble("Volume");
			  data.Amount = getValueDouble("Amount");
		}
		else if (this->getRowCount()>0 && columecount==8){

//			printf("4444444444444\n");
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
		  data.High = getValueDouble("High");
		  data.Low = getValueDouble("Low");
		  data.Volume =getValueDouble("Volume");
		  data.Amount = getValueDouble("Amount");
		}
		else{
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
		  data.High = getValueDouble("High");
		  data.Low = getValueDouble("Low");
		  data.Volume =getValueDouble("Volume");
		  data.Amount = getValueDouble("Amount");

		}


	}

//	printf("333333333333333\n");
	this->Empty();
	return data;
}

HxDayLineData MysqlDayLine::Find_PreDataBySqlTime(const char *tablename,const char* sqltime)
{
	HxDayLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time<'%s'  Order by Time Desc Limit 1 ",tablename,sqltime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		if(this->getRowCount()>0)		{
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
			  data.Volume =getValueDouble("Volume");
			  data.Amount = getValueDouble("Amount");
		}


	}
	this->Empty();
	return data;
}
HxDayLineData MysqlDayLine::Find_PreNDataBySqlTime(const char *tablename,const char* sqltime,int n)
{
	HxDayLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time<'%s'  Order by Time desc Limit 1 offset %d ",tablename,sqltime,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		if(this->getRowCount()>0)		{
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
			  data.Volume =getValueDouble("Volume");
			  data.Amount = getValueDouble("Amount");
		}


	}
	this->Empty();
	return data;
}
HxDayLineData MysqlDayLine::Find_LastDayLine(const char *tablename)
{
	HxDayLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time desc limit 1 ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		if(this->getRowCount()>0)		{
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
			  data.Volume =getValueDouble("Volume");
			  data.Amount = getValueDouble("Amount");
		}


	}
	this->Empty();
	return data;
}

HxDayLineData MysqlDayLine::Find_LastDayLineOffset(const char *tablename,int offset)
{
	HxDayLineData data;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time desc limit 1 offset %d ",tablename,offset);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		if(this->getRowCount()>0)		{
			strcpy(data.Time,getValueString("Time").c_str());
			data.Close=getValueDouble("Close");
			data.LastClose=getValueDouble("LastClose");
			data.Open = getValueDouble("Open");
			  data.High = getValueDouble("High");
			  data.Low = getValueDouble("Low");
			  data.Volume =getValueDouble("Volume");
			  data.Amount = getValueDouble("Amount");
		}


	}
	this->Empty();
	return data;
}
void MysqlDayLine::Update_CycleData(const char *tablename)
{
	MysqlCycle mysqlcycle;
	char pinstrumentid[10];
	strcpy(pinstrumentid,tablename);
	vector<HxDayLineData>  dayline_list;
	this->Find_UpdateDateList(dayline_list,pinstrumentid);
//	printf("<<<<<<Update_CycleData----    ----->list size[%ld]\n",dayline_list.size());
	int i=0;
	for (auto &dayline:dayline_list)	{
		if(i>20)		{
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


			  if(mysqlcycle.Exist_DateTime(pinstrumentid,dayline.Time))				{
//								printf("exist!!!  --->>>>:\n");
				}
			  else if(local_t<t_closetime)				  {
//								  printf("当日未收盘数据,不插入!!!  --->>>>:\n");
				  }
				else		{

					CycleData cycledata;
					cycledata=this->Collect_CycleData(pinstrumentid,dayline.Time);
					printf("不存在该数据,插入新数据[%s] --->>>>high20[%.3f]  low20[%.3f]:\n",
							cycledata.Time,cycledata.High20,cycledata.Low20);
					mysqlcycle.Insert(pinstrumentid,cycledata);
				}
		}
		i++;
	}
	vector<HxDayLineData>().swap(dayline_list);

}

CycleData MysqlDayLine::Collect_CycleData(const char *tablename,const char * date)
		{

			CycleData cycledata;
			strcpy(cycledata.InstrumentID,tablename);
			strcpy(cycledata.Time,date);


//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit 20 ",tablename,date);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))	{
					this->getFirst();
					/* retrieve the row count in the result set */
					for (int i=0; i<this->getRowCount();i++){

		//	 			 cout << res -> getString("Time") << endl;
		//	 			cout << res -> getDouble("Open") << endl;
		//	 			cout << res -> getDouble("Close") << endl;

						  std::string str_date;
						  str_date= getValueString("Time");
						  double open = getValueDouble("Open");
						  double close = getValueDouble("Close");

						  double today_max;
						  double today_min;

						  if (i==0)	  {
							  cycledata.High20=open;
							  cycledata.Low20=open;
							  strcpy(cycledata.HighDate,str_date.c_str());
							  strcpy(cycledata.LowDate,str_date.c_str());
							  today_max=open;
							  today_min=open;
						  }


						  if (open >close )	  {
							  today_max =open;
							  if(today_max > cycledata.High20)
							  {
								  cycledata.High20 = today_max;
								  strcpy(cycledata.HighDate,str_date.c_str());
							  }
						  }
						  else
						  {
							  today_max =close;
							  if(today_max > cycledata.High20)	  {
								  cycledata.High20 = today_max;
								  strcpy(cycledata.HighDate,str_date.c_str());
							  }
						  }

						  if (open <close )	  {
							  today_min =open;
							  if(today_min < cycledata.Low20)  {
								  cycledata.Low20 = today_min;
								  strcpy(cycledata.LowDate,str_date.c_str());
							  }
						  }
						  else  {
							  today_min =close;
							  if(today_min < cycledata.Low20) {
								  cycledata.Low20 = today_min;
								  strcpy(cycledata.LowDate,str_date.c_str());
							  }
						  }
						  this->getNext();
					}

				}

			MysqlCycle mcycle;
			cycledata.HighCount = mcycle.Check_Highcount(tablename,date,cycledata.High20);
			cycledata.LowCount = mcycle.Check_Lowcount(tablename,date,cycledata.Low20);

			this->Empty();
			return cycledata;


		}

CycleData MysqlDayLine::Collect_CycleNData(const char *tablename,const char * date,int n)
{

	CycleData cycledata;
	strcpy(cycledata.InstrumentID,tablename);
	strcpy(cycledata.Time,date);


//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d  ",tablename,date,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)	{

//	 			 cout << res -> getString("Time") << endl;
//	 			cout << res -> getDouble("Open") << endl;
//	 			cout << res -> getDouble("Close") << endl;

				  std::string str_date;
				  str_date= getValueString("Time");
				  double open = getValueDouble("Open");
				  double close = getValueDouble("Close");
				  double high = getValueDouble("High");
				  double low = getValueDouble("Low");

				  double today_max;
				  double today_min;


				  if (i==0)	  {
					  cycledata.High20=open;
					  cycledata.Low20=open;



					  strcpy(cycledata.HighDate,str_date.c_str());
					  strcpy(cycledata.LowDate,str_date.c_str());
					  today_max=open;
					  today_min=open;

					  cycledata.Highest=high;
					  cycledata.Lowest=low;

				  }


				  if (open >close )	  {
					  today_max =open;
					  if(today_max > cycledata.High20)  {
						  cycledata.High20 = today_max;
						  strcpy(cycledata.HighDate,str_date.c_str());
					  }
				  }
				  else
				  {
					  today_max =close;
					  if(today_max > cycledata.High20)  {
						  cycledata.High20 = today_max;
						  strcpy(cycledata.HighDate,str_date.c_str());
					  }
				  }

				  if (open <close )	  {
					  today_min =open;
					  if(today_min < cycledata.Low20)
					  {
						  cycledata.Low20 = today_min;
						  strcpy(cycledata.LowDate,str_date.c_str());
					  }
				  }
				  else  {
					  today_min =close;
					  if(today_min < cycledata.Low20)  {
						  cycledata.Low20 = today_min;
						  strcpy(cycledata.LowDate,str_date.c_str());
					  }
				  }


				  if(high > cycledata.Highest)  {
					  cycledata.Highest = high;

				  }



				  if(low < cycledata.Lowest)  {
					  cycledata.Lowest = low;

				  }


				  this->getNext();
			}

		}



	this->Empty();
	return cycledata;


}

double MysqlDayLine::Collect_TotalMA_NData(const char *tablename,const char * date,int n)
		{

			double total_ma=0;

//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();
				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					  double close = getValueDouble("Close");


					  total_ma+=close;


					  this->getNext();
				}
			}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
			this->Empty();
			return total_ma;


		}

//double  MysqlDayLine::EMA(std::vector<double> &X, int N)
//{
//    std::vector<double> vec;
//    double ret;
//    int nLen = X.size();
//    if(nLen >= 1)
//    {
//        if(N > nLen) N = nLen;
//
//        vec.resize(nLen);
//        //vec.reserve(nLen);
////        vec[0] = X[0];
//
//        for(int i = 0; i < nLen; i++)
//        {
////            vec[i] = (2 * X[i] + (N - 1) * vec[i - 1]) / (N + 1);
//        	int n=i+1;
//        	if (i==0)
//        	{
//        		 vec[i] = X[i];
//        	}
//        	else
//        	{
//        		vec[i] = ((double)2.0 * X[i] + (double)(n - 1) * vec[i - 1]) / (double)(n + 1);
//        	}
//
//
//            ret =vec[i];
//            printf("EMA  --------  n :%d   vec[%d]: %.3f    ret: %.3f   \n",n,i,X[i], ret);
//        }
//    }
//
//    vector<double>().swap(vec);
//    return ret;
//}

double  MysqlDayLine::EMA(std::vector<double> &X, int N)
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

double MysqlDayLine::Collect_EMA_NData(const char *tablename,const char * date,int n)
		{

			double ema_n=0;

			vector<double> x;
//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();
				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					  double close = getValueDouble("Close");


					 x.push_back(close);


					  this->getNext();
				}
			}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
			this->Empty();

			reverse(x.begin(),x.end());

			ema_n = EMA(x,n);
			 vector<double>().swap(x);

			return ema_n;


		}

EMAData MysqlDayLine::Collect_EMAData(const char *tablename,const char * date)
		{

			EMAData total_ma;


			strcpy(total_ma.Time,date);
			total_ma.EMA49=Collect_EMA_NData(tablename,date,49);
			total_ma.EMA19=Collect_EMA_NData(tablename,date,19);
			total_ma.EMA8=Collect_EMA_NData(tablename,date,8);
			total_ma.EMA3=Collect_EMA_NData(tablename,date,3);


//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
			return total_ma;


		}

double MysqlDayLine::Get_EMADatabyMinPrice(const char *tablename,const char * date,int n,double minprice)
		{

			double ema_n=0;

			vector<double> x;
//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n-1);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();
				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					  double close = getValueDouble("Close");


					 x.push_back(close);


					  this->getNext();
				}
			}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
			this->Empty();

			reverse(x.begin(),x.end());

			x.push_back(minprice);

			ema_n = EMA(x,n);
			 vector<double>().swap(x);

			return ema_n;


		}


double MysqlDayLine::Get_MADatabyMinPrice(const char *tablename,const char * date,int n,double minprice)
		{

		double total_ma=0;
		double ma=0;

	//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n-1);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)
			{
				char buf[20];
				  double close = getValueDouble("Close");
				  strcpy(buf,getValueString("Time").c_str());

				  total_ma+=close;
//printf("%s  close %.3f total_ma %.3f    %d --> i %d   \n",buf,close,total_ma,this->getRowCount(),i);

				  this->getNext();
			}
		}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
		this->Empty();

		total_ma+=minprice;
		ma=total_ma/n;
		return ma;


		}


double MysqlDayLine::Get_ATRDatabyMinPrice(const char *tablename,const char * date,int n,double today_tr)
		{


			ATRData atrdata;
//		  printf ("Get_ATRDatabyMinPrice ---->  %s    %s ---->  n: %d    today_tr: %.2f \n",tablename,date,n,today_tr);
			double total_tr=0.0;
			double tr=0.0;
			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n-1);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))	{
				this->getFirst();

				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)	{
					HxDayLineData today;

					today.High =  getValueDouble("High");
					today.Low =  getValueDouble("Low");
					today.LastClose =  getValueDouble("LastClose");

					double hl = abs(today.High -today.Low);
					double ll= abs(today.LastClose - today.Low);
					double lh = abs(today.LastClose - today.High);

					tr = max(hl,max(ll,lh));

					if (i==0)	{
						atrdata.TR =tr;
						strcpy(atrdata.Time,date);
					}
//					printf("MysqlDayLine max %.2f    today.LastClose %.2f   today.High %.2f today.Low %.2f \n",tr,today.LastClose,today.High,today.Low );
					total_tr +=tr;
					this->getNext();

				}
			}

			total_tr +=today_tr;
			atrdata.ATR = total_tr/(double)n;
//			printf("Get_ATRDatabyMinPrice Collect_HisATR_NData  total_tr: %.2f   atr[%d]: %.2f \n",total_tr, n,atrdata.ATR);
			this->Empty();
			return atrdata.ATR;

		}
double MysqlDayLine::Get_TRndaysbySqlTime(const char *tablename,const char * date,int n)
{


	ATRData atrdata;
//		  printf ("Get_ATRDatabyMinPrice ---->  %s    %s ---->  n: %d    today_tr: %.2f \n",tablename,date,n,today_tr);
	double total_tr=0.0;
	double tr=0.0;
	char buf[1024];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();

		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			HxDayLineData today;

			today.High =  getValueDouble("High");
			today.Low =  getValueDouble("Low");
			today.LastClose =  getValueDouble("LastClose");

			double hl = abs(today.High -today.Low);
			double ll= abs(today.LastClose - today.Low);
			double lh = abs(today.LastClose - today.High);

			tr = max(hl,max(ll,lh));

			if (i==0)	{
				atrdata.TR =tr;
				strcpy(atrdata.Time,date);
			}
//					printf("MysqlDayLine max %.2f    today.LastClose %.2f   today.High %.2f today.Low %.2f \n",tr,today.LastClose,today.High,today.Low );
			total_tr +=tr;
			this->getNext();

		}
	}



//			printf("Get_ATRDatabyMinPrice Collect_HisATR_NData  total_tr: %.2f   atr[%d]: %.2f \n",total_tr, n,atrdata.ATR);
	this->Empty();
	return total_tr;

}
double MysqlDayLine::Get_OpenClosePercent(const char *tablename,const char * date,int n)
{



//		  printf ("Get_ATRDatabyMinPrice ---->  %s    %s ---->  n: %d    today_tr: %.2f \n",tablename,date,n,today_tr);
	double oc=0.0;
	double hl=0.0;
	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();

		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			HxDayLineData today;

			today.Open =  getValueDouble("Open");
			today.Close =  getValueDouble("Close");
			today.High =  getValueDouble("High");
			today.Low =  getValueDouble("Low");
			today.LastClose =  getValueDouble("LastClose");

			oc+= fabs(today.Open -today.Close);
			hl+= abs(today.High - today.Low);
			this->getNext();

		}
	}
	double percent;
	if(hl==0.0){
		percent = 0.0;
	}
	else{
		percent = oc/hl;
	}

//			printf("Get_ATRDatabyMinPrice Collect_HisATR_NData  total_tr: %.2f   atr[%d]: %.2f \n",total_tr, n,atrdata.ATR);
	this->Empty();
	return percent;

}

double MysqlDayLine::Get_HisMAData(const char *tablename,const char * date,int n)
		{

		double total_ma=0;
		double ma=0;

	//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s WHERE Time <='%s' ORDER BY Time DESC limit %d ",tablename,date,n);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)
			{
				  double close = getValueDouble("Close");


				  total_ma+=close;


				  this->getNext();
			}
		}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
		this->Empty();


		ma=total_ma/n;
		return ma;


		}


double MysqlDayLine::Get_PreHisMADatabyOffset(const char *tablename,const char * date,int n,int offset)
		{

		double total_ma=0;
		double ma=0;

	//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d offset %d ",tablename,date,n,offset);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)
			{
				  double close = getValueDouble("Close");


				  total_ma+=close;


				  this->getNext();
			}
		}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
		this->Empty();


		ma=total_ma/n;
		return ma;


		}
double MysqlDayLine::Get_PreHisMAData(const char *tablename,const char * date,int n)
		{

		double total_ma=0;
		double ma=0;

	//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

		char buf[500];
		std::string str_sql;

		sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))
		{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)
			{
				  double close = getValueDouble("Close");


				  total_ma+=close;


				  this->getNext();
			}
		}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
		this->Empty();


		ma=total_ma/n;
		return ma;


		}

double MysqlDayLine::Get_HisEMAData(const char *tablename,const char * date,int n)
		{

			double ema_n=0;

				vector<double> x;
	//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

				char buf[500];
				std::string str_sql;

				sprintf(buf,"SELECT * FROM %s WHERE Time <='%s' ORDER BY Time DESC limit %d ",tablename,date,n);

				str_sql=buf;
				if(this->ExecuteQuery(str_sql))
				{
					this->getFirst();
					/* retrieve the row count in the result set */
					for (int i=0; i<this->getRowCount();i++)
					{
						  double close = getValueDouble("Close");
						  std:: string t =getValueString("Time");

//						  printf(" [%s]   %d    ---> %s    ->   %.3f   \n",tablename,n,t.c_str(),close);
						 x.push_back(close);


						  this->getNext();
					}
				}
	//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
				this->Empty();

				reverse(x.begin(),x.end());
//				for (auto &item:x)
//				{
//					 printf("reverse  [%s]   %d    --->    ->   %.3f   \n",tablename,n,item);
//				}

				ema_n = EMA(x,n);
				 vector<double>().swap(x);

				return ema_n;


		}

double MysqlDayLine::Collect_TotalHisMA_NData(const char *tablename,const char * date,int n)
		{

			double total_ma=0;

//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <='%s' ORDER BY Time DESC limit %d ",tablename,date,n);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();
				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					  double close = getValueDouble("Close");


					  total_ma+=close;


					  this->getNext();
				}
			}
//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
			this->Empty();
			return total_ma;


		}

double MysqlDayLine::Collect_HisEMA_NData(const char *tablename,const char * date,int n)
		{

			double ema_n=0;

				vector<double> x;
	//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

				char buf[500];
				std::string str_sql;

				sprintf(buf,"SELECT * FROM %s WHERE Time <='%s' ORDER BY Time DESC limit %d ",tablename,date,n);

				str_sql=buf;
				if(this->ExecuteQuery(str_sql))
				{
					this->getFirst();
					/* retrieve the row count in the result set */
					for (int i=0; i<this->getRowCount();i++)
					{
						  double close = getValueDouble("Close");
						  std:: string t =getValueString("Time");

//						  printf(" [%s]   %d    ---> %s    ->   %.3f   \n",tablename,n,t.c_str(),close);
						 x.push_back(close);


						  this->getNext();
					}
				}
	//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
				this->Empty();

				reverse(x.begin(),x.end());
//				for (auto &item:x)
//				{
//					 printf("reverse  [%s]   %d    --->    ->   %.3f   \n",tablename,n,item);
//				}

				ema_n = EMA(x,n);
				 vector<double>().swap(x);

				return ema_n;


		}


HisEMAData MysqlDayLine::Collect_HisEMAData(const char *tablename,const char * date){

	HisEMAData data;


	strcpy(data.Time,date);
	data.EMA50=Collect_HisEMA_NData(tablename,date,50);
	data.EMA20=Collect_HisEMA_NData(tablename,date,20);
	data.EMA9=Collect_HisEMA_NData(tablename,date,9);
	data.EMA4=Collect_HisEMA_NData(tablename,date,4);


//			printf("MysqlDayLine Collect_TotalHisEMAData end! ...\n");
	return data;


}

MAData MysqlDayLine::Collect_TotalMAData(const char *tablename,const char * date)
		{

			MAData total_ma;


			strcpy(total_ma.Time,date);
			total_ma.MA59=Collect_TotalMA_NData(tablename,date,59);
			total_ma.MA49=Collect_TotalMA_NData(tablename,date,49);
			total_ma.MA19=Collect_TotalMA_NData(tablename,date,19);
			total_ma.MA9=Collect_TotalMA_NData(tablename,date,9);
			total_ma.MA17=Collect_TotalMA_NData(tablename,date,17);
			total_ma.MA8=Collect_TotalMA_NData(tablename,date,8);
			total_ma.MA3=Collect_TotalMA_NData(tablename,date,3);

//			printf("MongoHxDayLine Collect_TotalMA_NData end! ...\n");
			return total_ma;


		}



HisMAData MysqlDayLine::Collect_TotalHisMAData(const char *tablename,const char * date)
		{

			HisMAData hismadata;


			strcpy(hismadata.Time,date);
			hismadata.MA50=Collect_TotalHisMA_NData(tablename,date,50)/50.0;
			hismadata.MA20=Collect_TotalHisMA_NData(tablename,date,20)/20.0;
			hismadata.MA10=Collect_TotalHisMA_NData(tablename,date,10)/10.0;


//			printf("MysqlDayLine Collect_TotalMA_NData end! ...\n");
			return hismadata;


		}

ATRData MysqlDayLine::Collect_HisATR_NData(const char *tablename,const char * date,int n)
		{

			double total_ma=0;
			ATRData atrdata;
//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);
			double total_tr;
			double tr;
			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <='%s' ORDER BY Time DESC limit %d ",tablename,date,n);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();

				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					HxDayLineData today;

					today.High =  getValueDouble("High");
					today.Low =  getValueDouble("Low");
					today.LastClose =  getValueDouble("LastClose");

					double hl = abs(today.High -today.Low);
					double ll= abs(today.LastClose - today.Low);
					double lh = abs(today.LastClose - today.High);

					tr = max(hl,max(ll,lh));

					if (i==0)
					{
						atrdata.TR =tr;
						strcpy(atrdata.Time,date);
					}
//					printf("MysqlDayLine max %.2f    today.LastClose %.2f   today.High %.2f today.Low %.2f \n",tr,today.LastClose,today.High,today.Low );
					total_tr +=tr;
					this->getNext();

				}
			}


			atrdata.ATR = total_tr/(double)n;
//			printf("MysqlDayLine Collect_HisATR_NData %.2f    %.2f \n",tr, atrdata.TR);
			this->Empty();
			return atrdata;

		}

ATRData MysqlDayLine::Collect_PreHisATR_NData(const char *tablename,const char * date,int n)
		{


			ATRData atrdata;
//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);
			double total_tr=0.0;
			double tr;
			char buf[500];
			std::string str_sql;

			sprintf(buf,"SELECT * FROM %s WHERE Time <'%s' ORDER BY Time DESC limit %d ",tablename,date,n);

			str_sql=buf;
			if(this->ExecuteQuery(str_sql))
			{
				this->getFirst();

				/* retrieve the row count in the result set */
				for (int i=0; i<this->getRowCount();i++)
				{
					HxDayLineData today;

					today.High =  getValueDouble("High");
					today.Low =  getValueDouble("Low");
					today.LastClose =  getValueDouble("LastClose");

					double hl = abs(today.High -today.Low);
					double ll= abs(today.LastClose - today.Low);
					double lh = abs(today.LastClose - today.High);

					tr = max(hl,max(ll,lh));

					if (i==0)
					{
						atrdata.TR =tr;
						strcpy(atrdata.Time,date);
					}

					total_tr +=tr;

//					printf("MysqlDayLine  total: %.3f max %.2f    today.LastClose %.2f   today.High %.2f today.Low %.2f \n",total_tr,tr,today.LastClose,today.High,today.Low );

					this->getNext();

				}
			}


			atrdata.ATR = total_tr/(double)n;
//			printf("MysqlDayLine Collect_HisATR_NData[%d] total_tr: %.2f   atrdata.TR:  %.2f   atr: %.3f \n",n,total_tr, atrdata.TR,atrdata.ATR);
			this->Empty();
			return atrdata;

		}
void MysqlDayLine::Update_HisData(const char *tablename)
{
	MysqlHisMA mysqlhisma;
	char pinstrumentid[10];
	strcpy(pinstrumentid,tablename);
	vector<HxDayLineData>  dayline_list;
	this->Find_UpdateDateList(dayline_list,pinstrumentid);
//	printf("<<<<<<Update_CycleData----    ----->list size[%ld]\n",dayline_list.size());
	int i=0;
	for (auto &dayline:dayline_list)
	{
		if(i>50)
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
					data=Collect_TotalHisMAData(pinstrumentid,dayline.Time);
					printf("不存在该数据,插入新数据[%s] --->>>>MA50[%.3f]  MA20[%.3f]  MA10[%.3f]:\n",
							data.Time,data.MA50,data.MA20,data.MA10);
					mysqlhisma.Insert(pinstrumentid,data);
				}
		}
		i++;
	}
	vector<HxDayLineData>().swap(dayline_list);

}
HighLowest MysqlDayLine::Collect_CycleDatabyOpenDate(const char *tablename,const char * date)
{
	HighLowest info;
//		  printf ("query Time ---->  %s     ---->  long : %ld \n",date,t_);

		char buf[500];
		std::string str_sql;


		sprintf(buf,"SELECT * FROM %s WHERE Time >='%s' ORDER BY Time ASC ",tablename,date);

		str_sql=buf;
		if(this->ExecuteQuery(str_sql))		{
			this->getFirst();
			/* retrieve the row count in the result set */
			for (int i=0; i<this->getRowCount();i++)		{

//	 			 cout << res -> getString("Time") << endl;
//	 			cout << res -> getDouble("Open") << endl;
//	 			cout << res -> getDouble("Close") << endl;

				  std::string str_date;
				  str_date= getValueString("Time");
				  double high = getValueDouble("High");
				  double low = getValueDouble("Low");

				  if(i==0)				  {
					  info.highest=high;
					  info.lowest=low;
				  }
				  else		  {
						if(high>info.highest)	{
							info.highest=high;
						}

						if(low<info.lowest)	{
							info.lowest=low;
						}
				  }

				  this->getNext();
			}
		}
	this->Empty();
	return info;

}

int  MysqlDayLine::Count_HoldDays(const char* tablename,const char* opendate,const char *holddate)
{

	int count=0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE  Time>='%s' AND Time<='%s'  ",tablename,
			opendate,holddate);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		count=this->getRowCount();

	}
	this->Empty();
	return count;
}

int  MysqlDayLine::CountbyAll(const char* tablename)
{

	int count=0;

	char buf[2048];
	std::string str_sql;
	sprintf(buf,"SELECT count(Time) FROM %s ",tablename);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		if(this->getRowCount()>0)
		{
			this->getFirst();

				count = getValueInt("count(Time)");

		}


	}

	this->Empty();

	return count;
}
void MysqlDayLine::CorrectDayLineByInstrument(const char * pinstrumentid)
{
	DateUtil dateutil;

	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());


	vector<HxDayLineData>  dayline_list;
	this->Find_AllList(dayline_list,pinstrumentid);

	double endday_close;
	std::string endday;
	int i=0;
	for (auto &dayline:dayline_list)	{

		if (strcmp(productid,"i")==0 )	{

			if (dayline.Close <100)		{
				HxDayLineData data;
				strcpy(data.Time,dayline.Time);
				data.Close = dayline.Close*10.0;
				data.Open = dayline.Open*10.0;
				data.High = dayline.High*10.0;
				data.Low = dayline.Low*10.0;
				data.LastClose = dayline.LastClose*10.0;
				data.Volume = dayline.Volume;
				data.Amount = dayline.Amount;
				this->UpdateAllPrice(pinstrumentid,data);
			}
		}

		if (strcmp(productid,"j")==0|| strcmp(productid,"jm")==0)	{
			if (dayline.Close <500)	{
				HxDayLineData data;
				strcpy(data.Time,dayline.Time);
				data.Close = dayline.Close*10.0;
				data.Open = dayline.Open*10.0;
				data.High = dayline.High*10.0;
				data.Low = dayline.Low*10.0;
				data.LastClose = dayline.LastClose*10.0;
				data.Volume = dayline.Volume;
				data.Amount = dayline.Amount;
				this->UpdateAllPrice(pinstrumentid,data);
			}
		}

		if (i!=0){
			HxDayLineData predata = this->Find_PreDataBySqlTime(pinstrumentid,dayline.Time);
			if (predata.Close != dayline.LastClose)		{
				HxDayLineData data;
				strcpy(data.Time,dayline.Time);
				data.LastClose = predata.Close;

				printf("<<<<<<CorrectDayLineByInstrument[%s]---- Time %s :  predata.Close %.3f dayline.LastClose%.3f ---> LastClose %.3f  \n",pinstrumentid,data.Time,predata.Close,dayline.LastClose,data.LastClose);
				this->Update(pinstrumentid,data);
			}
		}

		i++;
	}

	vector<HxDayLineData>().swap(dayline_list);
}
