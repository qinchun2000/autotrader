#include "mysqlglobalview.hpp"

MysqlGlobalView::MysqlGlobalView(const char* userid) {
	std::string database = "account";
	std::string tablename = "globalview_";
	tablename.append(userid);

	if (this->IsCloseConn()) {
		if (!Connect()) {
			return;
		}
	} else {
		if (IsAliveConn()) {
		} else {
			if (!Connect()) {
				return;
			}
		}
	}

	this->Set_Database(database);
	this->Set_Table(tablename);
}

void MysqlGlobalView::CreateTable() {

	char buf[2048];
	sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
			"Time  DATETIME NOT NULL PRIMARY KEY, "
			"TradingDay CHAR(20), "
			"Status INT, "
			"TotalCapital DOUBLE,"
			"TotalSedimentaryIO DOUBLE, "
			"TotalSedimentaryIn DOUBLE, "
			"TotalSedimentaryOut DOUBLE, "
			"TotalSedimentaryIORate DOUBLE,"

			"MaxSedimentaryIO DOUBLE, "
			"MaxSedimentaryIOIns CHAR(40), "
			"MinSedimentaryIO DOUBLE, "
			"MinSedimentaryIOIns CHAR(40), "

			"MaxSedimentaryIORate DOUBLE, "
			"MaxSedimentaryIORateIns CHAR(40), "
			"MinSedimentaryIORate DOUBLE, "
			"MinSedimentaryIORateIns CHAR(40), "

			"MaxSedimentaryIO_Section DOUBLE, "//资金流入流出最大板块的资金量
			"MaxSedIO_SectionName CHAR(40), "//资金流入流入最大板块名
			"MinSedimentaryIO_Section DOUBLE, "//资金流入流出最小板块的资金量
			"MinSedIO_SectionName CHAR(40), "//资金流入流入最小板块名

			"MaxSedimentaryIORate_Section DOUBLE, " //板块资金流入流出最大幅度
			"MaxSedIORate_SectionName CHAR(40), "//资金流入流出比最大板块名
			"MinSedimentaryIORate_Section DOUBLE, "//板块资金流入流出比最小幅度
			"MinSedIORate_SectionName CHAR(40), "//资金流入流入比最小板块名

			"Up_Count INT, "
			"Down_Count INT, "
			"Total_Up DOUBLE, "
			"Total_Down DOUBLE, "
			"Average_Up DOUBLE, "
			"Average_Down DOUBLE, "

			"MaxUpDown DOUBLE, "
			"MinUpDown DOUBLE, "
			"AbsMaxUpDownIns CHAR(40),"
			"AbsMaxUpDown DOUBLE, "
			"StatusChangeReason INT,"
			"MaxUpDownIns CHAR(40), "
			"MinUpDownIns CHAR(40)) default character set utf8 ",
			this->getTablename().c_str());
	std::string str_sql;
	str_sql = buf;
	this->Execute(str_sql);
}
void MysqlGlobalView::Insert(GlobalViewData data) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"INSERT INTO %s (Time,TradingDay,Status,TotalCapital,TotalSedimentaryIO,TotalSedimentaryIn,TotalSedimentaryOut,TotalSedimentaryIORate,"
					"MaxSedimentaryIO,MaxSedimentaryIOIns,MinSedimentaryIO,MinSedimentaryIOIns,"
			"MaxSedimentaryIORate,MaxSedimentaryIORateIns,MinSedimentaryIORate,MinSedimentaryIORateIns,"
			"MaxSedimentaryIO_Section,MaxSedIO_SectionName,MinSedimentaryIO_Section,MinSedIO_SectionName,"
			"MaxSedimentaryIORate_Section,MaxSedIORate_SectionName,MinSedimentaryIORate_Section,MinSedIORate_SectionName,"
					"Up_Count,Down_Count,Total_Up,Total_Down,Average_Up,Average_Down, "
					"MaxUpDown,MinUpDown,MaxUpDownIns,MinUpDownIns,AbsMaxUpDownIns,AbsMaxUpDown,StatusChangeReason)  "
					"VALUES ('%s','%s','%d','%.2f','%.2f','%.2f','%.2f','%.2f',"
					"'%.2f','%s','%.2f','%s',"
					"'%.4f','%s','%.4f','%s',"
					"'%.2f','%s','%.2f','%s',"
					"'%.4f','%s','%.4f','%s',"
					"'%d','%d','%.2f','%.2f','%.2f','%.2f',"
					"'%.2f','%.2f','%s','%s','%s','%.2f','%d')", this->getTablename().c_str(),
			data.Time, data.TradingDay, data.Status, data.TotalCapital,
			data.TotalSedimentaryIO, data.TotalSedimentaryIn,data.TotalSedimentaryOut,data.TotalSedimentaryIORate,
			 data.MaxSedimentaryIO,data.MaxSedimentaryIOIns, data.MinSedimentaryIO,data.MinSedimentaryIOIns,
			 data.MaxSedimentaryIORate,data.MaxSedimentaryIORateIns, data.MinSedimentaryIORate,data.MinSedimentaryIORateIns,
			 data.MaxSedimentaryIO_Section,data.MaxSedIO_SectionName, data.MinSedimentaryIO_Section,data.MinSedIO_SectionName,
			 data.MaxSedimentaryIORate_Section,data.MaxSedIORate_SectionName, data.MinSedimentaryIORate_Section,data.MinSedIORate_SectionName,
			 data.Up_Count, data.Down_Count,data.Total_Up, data.Total_Down, data.Average_Up, data.Average_Down,
			data.MaxUpDown, data.MinUpDown, data.MaxUpDownIns,data.MinUpDownIns,data.AbsMaxUpDownIns,data.AbsMaxUpDown,data.StatusChangeReason);

	str_sql = buf;
	this->Execute(str_sql);
}

void MysqlGlobalView::Insert(GlobalViewData *data) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"INSERT INTO %s (Time,TradingDay,Status,TotalCapital,TotalSedimentaryIO,TotalSedimentaryIn,TotalSedimentaryOut,TotalSedimentaryIORate,"
					"MaxSedimentaryIO,MaxSedimentaryIOIns,MinSedimentaryIO,MinSedimentaryIOIns,"
			"MaxSedimentaryIORate,MaxSedimentaryIORateIns,MinSedimentaryIORate,MinSedimentaryIORateIns,"
			"MaxSedimentaryIO_Section,MaxSedIO_SectionName,MinSedimentaryIO_Section,MinSedIO_SectionName,"
			"MaxSedimentaryIORate_Section,MaxSedIORate_SectionName,MinSedimentaryIORate_Section,MinSedIORate_SectionName,"
					"Up_Count,Down_Count,Total_Up,Total_Down,Average_Up,Average_Down, "
					"MaxUpDown,MinUpDown,MaxUpDownIns,MinUpDownIns,AbsMaxUpDownIns,AbsMaxUpDown,StatusChangeReason)  "
					"VALUES ('%s','%s','%d','%.2f','%.2f','%.2f','%.2f','%.2f',"
					"'%.2f','%s','%.2f','%s',"
					"'%.4f','%s','%.4f','%s',"
					"'%.2f','%s','%.2f','%s',"
					"'%.4f','%s','%.4f','%s',"
					"'%d','%d','%.2f','%.2f','%.2f','%.2f',"
					"'%.2f','%.2f','%s','%s','%s','%.2f','%d')", this->getTablename().c_str(),
			data->Time, data->TradingDay, data->Status, data->TotalCapital,
			data->TotalSedimentaryIO, data->TotalSedimentaryIn,data->TotalSedimentaryOut,data->TotalSedimentaryIORate,
			 data->MaxSedimentaryIO,data->MaxSedimentaryIOIns, data->MinSedimentaryIO,data->MinSedimentaryIOIns,
			 data->MaxSedimentaryIORate,data->MaxSedimentaryIORateIns, data->MinSedimentaryIORate,data->MinSedimentaryIORateIns,
			 data->MaxSedimentaryIO_Section,data->MaxSedIO_SectionName, data->MinSedimentaryIO_Section,data->MinSedIO_SectionName,
			 data->MaxSedimentaryIORate_Section,data->MaxSedIORate_SectionName, data->MinSedimentaryIORate_Section,data->MinSedIORate_SectionName,
			 data->Up_Count, data->Down_Count,data->Total_Up, data->Total_Down, data->Average_Up, data->Average_Down,
			data->MaxUpDown, data->MinUpDown, data->MaxUpDownIns,data->MinUpDownIns,data->AbsMaxUpDownIns,data->AbsMaxUpDown,data->StatusChangeReason);

	str_sql = buf;
	this->Execute(str_sql);
}
void MysqlGlobalView::Update(GlobalViewData data) {

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "UPDATE %s SET MaxUpDownIns='%s' WHERE Time='%s' ",
			this->getTablename().c_str(), data.MaxUpDownIns, data.Time);
	str_sql = buf;
	this->ExecuteUpdate(str_sql);
}

bool MysqlGlobalView::Exist_DataByTime(const char* time) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s WHERE  Time='%s'  ",
			this->getTablename().c_str(), time);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
//		printf("MysqlSection::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if (this->getRowCount() > 0) {
//			printf("MysqlSection::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			ret = true;
		}

	}

	this->Empty();

	return ret;
}

bool MysqlGlobalView::Exist_DataByTradingDay(const char* tradingday) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s WHERE  TradingDay='%s'  ",
			this->getTablename().c_str(), tradingday);

//	printf("SELECT * FROM %s WHERE  SingalName='%s'   \n",this->getTablename().c_str(),signalname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
//		printf("MysqlSection::Exist_DataBySignalName getRowCount [%d] ! \n",this->getRowCount());

		if (this->getRowCount() > 0) {
//			printf("MysqlSection::Exist_DataBySignalName [%d] ---> true ! \n",this->getRowCount());
			ret = true;
		}

	}

	this->Empty();

	return ret;
}
GlobalViewData MysqlGlobalView::Find_LastData() {
//	printf(" Find_DataBySignalName begin  \n ");
	GlobalViewData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s  ORDER BY Time DESC LIMIT 1  ",
			this->getTablename().c_str());
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
//		printf(" count %d \n ",this->getRowCount());

		if (this->getRowCount() > 0) {
			this->getFirst();
			strcpy(data.Time, getValueString("Time").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.Status = getValueInt("Status");
			data.TotalCapital = getValueDouble("TotalCapital");
			data.TotalSedimentaryIO = getValueDouble("TotalSedimentaryIO");
			data.TotalSedimentaryIORate = getValueDouble("TotalSedimentaryIORate");
			data.TotalSedimentaryIn = getValueDouble("TotalSedimentaryIn");
			data.TotalSedimentaryOut = getValueDouble("TotalSedimentaryOut");

			data.MaxSedimentaryIO = getValueDouble("MaxSedimentaryIO");
			strcpy(data.MaxSedimentaryIOIns,getValueString("MaxSedimentaryIOIns").c_str());
			data.MinSedimentaryIO = getValueDouble("MinSedimentaryIO");
			strcpy(data.MinSedimentaryIOIns,getValueString("MinSedimentaryIOIns").c_str());

			data.MaxSedimentaryIORate = getValueDouble("MaxSedimentaryIORate");
			strcpy(data.MaxSedimentaryIORateIns,getValueString("MaxSedimentaryIORateIns").c_str());
			data.MinSedimentaryIORate = getValueDouble("MinSedimentaryIORate");
			strcpy(data.MinSedimentaryIORateIns,getValueString("MinSedimentaryIORateIns").c_str());

			data.MaxSedimentaryIO_Section = getValueDouble("MaxSedimentaryIO_Section");
			strcpy(data.MaxSedIO_SectionName,getValueString("MaxSedIO_SectionName").c_str());
			data.MinSedimentaryIO_Section = getValueDouble("MinSedimentaryIO_Section");
			strcpy(data.MinSedIO_SectionName,getValueString("MinSedIO_SectionName").c_str());

			data.MaxSedimentaryIORate_Section = getValueDouble("MaxSedimentaryIORate_Section");
			strcpy(data.MaxSedIORate_SectionName,getValueString("MaxSedIORate_SectionName").c_str());
			data.MinSedimentaryIORate_Section = getValueDouble("MinSedimentaryIORate_Section");
			strcpy(data.MinSedIORate_SectionName,getValueString("MinSedIORate_SectionName").c_str());

			data.Up_Count = getValueInt("Up_Count");
			data.Down_Count = getValueInt("Down_Count");
			data.Total_Up = getValueDouble("Total_Up");
			data.Total_Down = getValueDouble("Total_Down");
			data.Average_Up = getValueDouble("Average_Up");
			data.Average_Down = getValueDouble("Average_Down");

			data.MaxUpDown = getValueDouble("MaxUpDown");
			data.MinUpDown = getValueDouble("MinUpDown");
			data.StatusChangeReason = getValueInt("StatusChangeReason");
			strcpy(data.MaxUpDownIns, getValueString("MaxUpDownIns").c_str());
			strcpy(data.MinUpDownIns, getValueString("MinUpDownIns").c_str());
			strcpy(data.AbsMaxUpDownIns, getValueString("AbsMaxUpDownIns").c_str());
			data.AbsMaxUpDown = getValueDouble("AbsMaxUpDown");
		}
	}

	this->Empty();
	return data;
}
GlobalViewData MysqlGlobalView::Find_LastDatabyTradingDay(const char * tradingday) {
//	printf(" Find_DataBySignalName begin  \n ");
	GlobalViewData data;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s  WHERE TradingDay='%s' ORDER BY Time DESC LIMIT 1  ",
			this->getTablename().c_str(),tradingday);
//	printf("SELECT * FROM %s WHERE  Status='%d'  \n",tablename,status);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
//		printf(" count %d \n ",this->getRowCount());

		if (this->getRowCount() > 0) {
			this->getFirst();
			strcpy(data.Time, getValueString("Time").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.Status = getValueInt("Status");
			data.TotalCapital = getValueDouble("TotalCapital");
			data.TotalSedimentaryIO = getValueDouble("TotalSedimentaryIO");
			data.TotalSedimentaryIORate = getValueDouble("TotalSedimentaryIORate");
			data.TotalSedimentaryIn = getValueDouble("TotalSedimentaryIn");
			data.TotalSedimentaryOut = getValueDouble("TotalSedimentaryOut");

			data.MaxSedimentaryIO = getValueDouble("MaxSedimentaryIO");
			strcpy(data.MaxSedimentaryIOIns,getValueString("MaxSedimentaryIOIns").c_str());
			data.MinSedimentaryIO = getValueDouble("MinSedimentaryIO");
			strcpy(data.MinSedimentaryIOIns,getValueString("MinSedimentaryIOIns").c_str());

			data.MaxSedimentaryIORate = getValueDouble("MaxSedimentaryIORate");
			strcpy(data.MaxSedimentaryIORateIns,getValueString("MaxSedimentaryIORateIns").c_str());
			data.MinSedimentaryIORate = getValueDouble("MinSedimentaryIORate");
			strcpy(data.MinSedimentaryIORateIns,getValueString("MinSedimentaryIORateIns").c_str());

			data.MaxSedimentaryIO_Section = getValueDouble("MaxSedimentaryIO_Section");
			strcpy(data.MaxSedIO_SectionName,getValueString("MaxSedIO_SectionName").c_str());
			data.MinSedimentaryIO_Section = getValueDouble("MinSedimentaryIO_Section");
			strcpy(data.MinSedIO_SectionName,getValueString("MinSedIO_SectionName").c_str());

			data.MaxSedimentaryIORate_Section = getValueDouble("MaxSedimentaryIORate_Section");
			strcpy(data.MaxSedIORate_SectionName,getValueString("MaxSedIORate_SectionName").c_str());
			data.MinSedimentaryIORate_Section = getValueDouble("MinSedimentaryIORate_Section");
			strcpy(data.MinSedIORate_SectionName,getValueString("MinSedIORate_SectionName").c_str());

			data.Up_Count = getValueInt("Up_Count");
			data.Down_Count = getValueInt("Down_Count");
			data.Total_Up = getValueDouble("Total_Up");
			data.Total_Down = getValueDouble("Total_Down");
			data.Average_Up = getValueDouble("Average_Up");
			data.Average_Down = getValueDouble("Average_Down");

			data.MaxUpDown = getValueDouble("MaxUpDown");
			data.MinUpDown = getValueDouble("MinUpDown");
			data.StatusChangeReason = getValueInt("StatusChangeReason");
			strcpy(data.MaxUpDownIns, getValueString("MaxUpDownIns").c_str());
			strcpy(data.MinUpDownIns, getValueString("MinUpDownIns").c_str());
			strcpy(data.AbsMaxUpDownIns, getValueString("AbsMaxUpDownIns").c_str());
			data.AbsMaxUpDown = getValueDouble("AbsMaxUpDown");
		}
	}

	this->Empty();
	return data;
}
void MysqlGlobalView::Find_ListsbyTradingDay(vector<GlobalViewData> &temp,
		const char * tradingday) {
	GlobalViewData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE TradingDay='%s' ",
			this->getTablename().c_str(), tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.Time, getValueString("Time").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.Status = getValueInt("Status");
			data.TotalCapital = getValueDouble("TotalCapital");
			data.TotalSedimentaryIO = getValueDouble("TotalSedimentaryIO");
			data.TotalSedimentaryIORate = getValueDouble("TotalSedimentaryIORate");
			data.TotalSedimentaryIn = getValueDouble("TotalSedimentaryIn");
			data.TotalSedimentaryOut = getValueDouble("TotalSedimentaryOut");

			data.MaxSedimentaryIO = getValueDouble("MaxSedimentaryIO");
			strcpy(data.MaxSedimentaryIOIns,getValueString("MaxSedimentaryIOIns").c_str());
			data.MinSedimentaryIO = getValueDouble("MinSedimentaryIO");
			strcpy(data.MinSedimentaryIOIns,getValueString("MinSedimentaryIOIns").c_str());

			data.MaxSedimentaryIORate = getValueDouble("MaxSedimentaryIORate");
			strcpy(data.MaxSedimentaryIORateIns,getValueString("MaxSedimentaryIORateIns").c_str());
			data.MinSedimentaryIORate = getValueDouble("MinSedimentaryIORate");
			strcpy(data.MinSedimentaryIORateIns,getValueString("MinSedimentaryIORateIns").c_str());

			data.MaxSedimentaryIO_Section = getValueDouble("MaxSedimentaryIO_Section");
			strcpy(data.MaxSedIO_SectionName,getValueString("MaxSedIO_SectionName").c_str());
			data.MinSedimentaryIO_Section = getValueDouble("MinSedimentaryIO_Section");
			strcpy(data.MinSedIO_SectionName,getValueString("MinSedIO_SectionName").c_str());

			data.MaxSedimentaryIORate_Section = getValueDouble("MaxSedimentaryIORate_Section");
			strcpy(data.MaxSedIORate_SectionName,getValueString("MaxSedIORate_SectionName").c_str());
			data.MinSedimentaryIORate_Section = getValueDouble("MinSedimentaryIORate_Section");
			strcpy(data.MinSedIORate_SectionName,getValueString("MinSedIORate_SectionName").c_str());

			data.Up_Count = getValueInt("Up_Count");
			data.Down_Count = getValueInt("Down_Count");
			data.Total_Up = getValueDouble("Total_Up");
			data.Total_Down = getValueDouble("Total_Down");
			data.Average_Up = getValueDouble("Average_Up");
			data.Average_Down = getValueDouble("Average_Down");

			data.MaxUpDown = getValueDouble("MaxUpDown");
			data.MinUpDown = getValueDouble("MinUpDown");
			data.StatusChangeReason = getValueInt("StatusChangeReason");
			strcpy(data.MaxUpDownIns, getValueString("MaxUpDownIns").c_str());
			strcpy(data.MinUpDownIns, getValueString("MinUpDownIns").c_str());
			strcpy(data.AbsMaxUpDownIns, getValueString("AbsMaxUpDownIns").c_str());
			data.AbsMaxUpDown = getValueDouble("AbsMaxUpDown");
			temp.push_back(data);
			this->getNext();
		}
	}
	this->Empty();

}
void MysqlGlobalView::Find_ListsbyTradingDayReason(vector<GlobalViewData> &temp,
		const char * tradingday,int reason) {
	GlobalViewData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE TradingDay='%s' AND StatusChangeReason='%d'  ",
			this->getTablename().c_str(), tradingday,reason);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.Time, getValueString("Time").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.Status = getValueInt("Status");
			data.TotalCapital = getValueDouble("TotalCapital");
			data.TotalSedimentaryIO = getValueDouble("TotalSedimentaryIO");
			data.TotalSedimentaryIORate = getValueDouble("TotalSedimentaryIORate");
			data.TotalSedimentaryIn = getValueDouble("TotalSedimentaryIn");
			data.TotalSedimentaryOut = getValueDouble("TotalSedimentaryOut");

			data.MaxSedimentaryIO = getValueDouble("MaxSedimentaryIO");
			strcpy(data.MaxSedimentaryIOIns,getValueString("MaxSedimentaryIOIns").c_str());
			data.MinSedimentaryIO = getValueDouble("MinSedimentaryIO");
			strcpy(data.MinSedimentaryIOIns,getValueString("MinSedimentaryIOIns").c_str());

			data.MaxSedimentaryIORate = getValueDouble("MaxSedimentaryIORate");
			strcpy(data.MaxSedimentaryIORateIns,getValueString("MaxSedimentaryIORateIns").c_str());
			data.MinSedimentaryIORate = getValueDouble("MinSedimentaryIORate");
			strcpy(data.MinSedimentaryIORateIns,getValueString("MinSedimentaryIORateIns").c_str());

			data.MaxSedimentaryIO_Section = getValueDouble("MaxSedimentaryIO_Section");
			strcpy(data.MaxSedIO_SectionName,getValueString("MaxSedIO_SectionName").c_str());
			data.MinSedimentaryIO_Section = getValueDouble("MinSedimentaryIO_Section");
			strcpy(data.MinSedIO_SectionName,getValueString("MinSedIO_SectionName").c_str());

			data.MaxSedimentaryIORate_Section = getValueDouble("MaxSedimentaryIORate_Section");
			strcpy(data.MaxSedIORate_SectionName,getValueString("MaxSedIORate_SectionName").c_str());
			data.MinSedimentaryIORate_Section = getValueDouble("MinSedimentaryIORate_Section");
			strcpy(data.MinSedIORate_SectionName,getValueString("MinSedIORate_SectionName").c_str());

			data.Up_Count = getValueInt("Up_Count");
			data.Down_Count = getValueInt("Down_Count");
			data.Total_Up = getValueDouble("Total_Up");
			data.Total_Down = getValueDouble("Total_Down");
			data.Average_Up = getValueDouble("Average_Up");
			data.Average_Down = getValueDouble("Average_Down");

			data.MaxUpDown = getValueDouble("MaxUpDown");
			data.MinUpDown = getValueDouble("MinUpDown");
			data.StatusChangeReason = getValueInt("StatusChangeReason");
			strcpy(data.MaxUpDownIns, getValueString("MaxUpDownIns").c_str());
			strcpy(data.MinUpDownIns, getValueString("MinUpDownIns").c_str());
			strcpy(data.AbsMaxUpDownIns, getValueString("AbsMaxUpDownIns").c_str());
			data.AbsMaxUpDown = getValueDouble("AbsMaxUpDown");
			temp.push_back(data);
			this->getNext();
		}
	}
	this->Empty();

}
