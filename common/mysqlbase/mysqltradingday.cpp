#include "mysqltradingday.hpp"

MysqlTradingDay::MysqlTradingDay(const char * table) {

	std::string database = "tdlogin";
	std::string tablename = "user_";
	tablename.append(table);

	if (IsAliveConn()) {
//		printf("MysqlTradingDay IsAliveConn !!!! %s \n",database.c_str());

	} else {

//		printf("MysqlTradingDay IsAliveConn not connect !!!! %s \n",database.c_str());
		if (!Connect()) {

			return;
		}

	}

	this->Set_Database(database);
	this->Set_Table(tablename);

//	printf("MysqlTradingDay connet to database [%s]OK\n",database.c_str());

}

void MysqlTradingDay::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];
		sprintf(buf,
				"CREATE TABLE IF NOT EXISTS %s ("
//					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
						"TradingDateTime DATETIME NOT NULL  PRIMARY KEY,"
						"TradingDay CHAR(30),"
						"FrontID INT(11),"
						"SessionID INT(11),"
						"MaxOrderRef CHAR(13),"
						"LoginTime CHAR(9),"

						"DCETime CHAR(9),"
						"CZCETime CHAR(9),"
						"SHFETime CHAR(9),"
						"INETime CHAR(9),"
						"DifSeconds INT(11),"
						"SettleInfoConfirmStatus BOOL,"
						"DataReadyStatus BOOL,"
						"ConfirmDate CHAR(30),"
						"ConfirmTime CHAR(30),"
						"LocalCreateTime DATETIME,"
						"LocalUpdatetime DATETIME) default character set utf8 ",
				this->getTablename().c_str());

		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlTradingDay::Insert(TradingDayData data) {
//	printf("MysqlTradingDay Insert begin\n");
	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (TradingDateTime,TradingDay,FrontID,SessionID,MaxOrderRef,LoginTime,"
					"DCETime,CZCETime,SHFETime,INETime,DifSeconds,"
					"SettleInfoConfirmStatus,DataReadyStatus,LocalCreateTime,LocalUpdatetime) "
					"VALUES ('%s','%s', '%d','%d','%s','%s',"
					"'%s','%s','%s','%s','%d',"
					"'%d','%d','%s','%s') ", tablename.c_str(),
			data.TradingDateTime, data.TradingDay, data.FrontID, data.SessionID,
			data.MaxOrderRef, data.LoginTime, data.DCETime, data.CZCETime,
			data.SHFETime, data.INETime, data.DifSeconds,
			data.SettleInfoConfirmStatus, data.DataReadyStatus, time.c_str(),
			time.c_str());

	str_sql = buf;

	this->Execute(str_sql);

//	printf("MysqlTradingDay Insert end \n");
}

void MysqlTradingDay::Update(TradingDayData data) {
//	printf("insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "UPDATE %s SET LocalUpdateTime='%s' "
			"WHERE TradingDateTime='%s' ", this->getTablename().c_str(),
			time.c_str(), data.TradingDateTime);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

void MysqlTradingDay::UpdateMaxOrderRefbyDate(const char *maxorderref,
		const char* datetime) {
//	printf("insert begin\n");

	char buf[500];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "UPDATE %s SET MaxOrderRef ='%s',LocalUpdateTime='%s' "
			"WHERE TradingDateTime='%s' ", this->getTablename().c_str(),
			maxorderref, time.c_str(), datetime);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

void MysqlTradingDay::UpdateDifSecbyDate(int difsec, const char* date) {
//	printf("insert begin\n");

	char buf[500];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "UPDATE %s SET DifSeconds ='%d',LocalUpdateTime='%s' "
			"WHERE TradingDay='%s' ", this->getTablename().c_str(), difsec,
			time.c_str(), date);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

void MysqlTradingDay::UpdateSettleInfoConfirmStatusbyDate(
		TradingDayData *data) {
//	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"UPDATE %s SET SettleInfoConfirmStatus ='%d',ConfirmDate='%s',ConfirmTime='%s',LocalUpdateTime='%s' "
					"WHERE TradingDay='%s' ", this->getTablename().c_str(),
			data->SettleInfoConfirmStatus, data->ConfirmDate, data->ConfirmTime,
			time.c_str(), data->TradingDay);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}
void MysqlTradingDay::UpdateDataReadyStatusbyDate(TradingDayData *data) {
//	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "UPDATE %s SET DataReadyStatus ='%d',LocalUpdateTime='%s' "
			"WHERE TradingDay='%s' ", this->getTablename().c_str(),
			data->DataReadyStatus, time.c_str(), data->TradingDay);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}
void MysqlTradingDay::ResetSettleInfoConfirmStatusbyDate(bool status,
		const char* date) {
//	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"UPDATE %s SET SettleInfoConfirmStatus ='%d',LocalUpdateTime='%s' WHERE TradingDay='%s' ",
			this->getTablename().c_str(), status, time.c_str(), date);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}
bool MysqlTradingDay::Exist_DateTime(const char* datetime) {
//	printf("MysqlTradingDay-->Exist_DateTime begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT TradingDateTime FROM %s WHERE TradingDateTime='%s' ",
			this->getTablename().c_str(), datetime);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			ret = true;
		}
	}

	this->Empty();
	/* retrieve the row count in the result set */

//	printf("MysqlTradingDay--->Exist_DateTime end\n");
	return ret;
}

bool MysqlTradingDay::Exist_TradingDay(const char* date) {
//	printf("MysqlTradingDay-->Exist_TradingDay begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT TradingDay FROM %s WHERE TradingDay='%s' ",
			this->getTablename().c_str(), date);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() == 1) {
			ret = true;
		}

	}

	this->Empty();
	/* retrieve the row count in the result set */

//	printf("MysqlTradingDay--->Exist_TradingDay end\n");
	return ret;
}

TradingDayData MysqlTradingDay::Find_DatabyDate(const char *datetime) {
//	printf("Find_LastTradingDay begin\n");

	TradingDayData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE TradingDateTime='%s'  ",
			this->getTablename().c_str(), datetime);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();
			//		 cout << res -> getString("Time") << endl;

			strcpy(info.TradingDateTime,
					this->getValueString("TradingDateTime").c_str());
			strcpy(info.TradingDay, this->getValueString("TradingDay").c_str());

			info.FrontID = this->getValueInt("FrontID");
			info.SessionID = this->getValueInt("SessionID");
			strcpy(info.MaxOrderRef, getValueString("MaxOrderRef").c_str());
			strcpy(info.LoginTime, getValueString("LoginTime").c_str());

			strcpy(info.CZCETime, getValueString("CZCETime").c_str());
			strcpy(info.DCETime, getValueString("DCETime").c_str());
			strcpy(info.SHFETime, getValueString("SHFETime").c_str());
			strcpy(info.INETime, getValueString("INETime").c_str());
			info.DifSeconds = getValueInt("DifSeconds");
			info.SettleInfoConfirmStatus = getValueInt(
					"SettleInfoConfirmStatus");
			info.DataReadyStatus = getValueInt("DataReadyStatus");
			strcpy(info.ConfirmDate, getValueString("ConfirmDate").c_str());
			strcpy(info.ConfirmTime, getValueString("ConfirmTime").c_str());
			strcpy(info.LocalCreateTime,
					getValueString("LocalCreateTime").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

		}

	}
	this->Empty();
	return info;

}
TradingDayData MysqlTradingDay::Find_DatabyTradingDay(const char *date) {
//	printf("Find_LastTradingDay begin\n");

	TradingDayData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE TradingDay='%s'  ",
			this->getTablename().c_str(), date);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();
			//		 cout << res -> getString("Time") << endl;

			strcpy(info.TradingDateTime,
					this->getValueString("TradingDateTime").c_str());
			strcpy(info.TradingDay, this->getValueString("TradingDay").c_str());

			info.FrontID = this->getValueInt("FrontID");
			info.SessionID = this->getValueInt("SessionID");
			strcpy(info.MaxOrderRef, getValueString("MaxOrderRef").c_str());
			strcpy(info.LoginTime, getValueString("LoginTime").c_str());

			strcpy(info.CZCETime, getValueString("CZCETime").c_str());
			strcpy(info.DCETime, getValueString("DCETime").c_str());
			strcpy(info.SHFETime, getValueString("SHFETime").c_str());
			strcpy(info.INETime, getValueString("INETime").c_str());
			info.DifSeconds = getValueInt("DifSeconds");
			info.SettleInfoConfirmStatus = getValueInt(
					"SettleInfoConfirmStatus");
			info.DataReadyStatus = getValueInt("DataReadyStatus");
			strcpy(info.ConfirmDate, getValueString("ConfirmDate").c_str());
			strcpy(info.ConfirmTime, getValueString("ConfirmTime").c_str());
			strcpy(info.LocalCreateTime,
					getValueString("LocalCreateTime").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

		}

	}
	this->Empty();
	return info;

}
TradingDayData MysqlTradingDay::Find_LastTradingDay() {
//	printf("MysqlTradingDay -->Find_LastTradingDay begin\n");

	TradingDayData info;
	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s  ORDER BY TradingDateTime DESC LIMIT 1 ",
			this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();
			//		 cout << res -> getString("Time") << endl;

			strcpy(info.TradingDateTime,
					this->getValueString("TradingDateTime").c_str());
			strcpy(info.TradingDay, this->getValueString("TradingDay").c_str());

			info.FrontID = this->getValueInt("FrontID");
			info.SessionID = this->getValueInt("SessionID");
			strcpy(info.MaxOrderRef, getValueString("MaxOrderRef").c_str());
			strcpy(info.LoginTime, getValueString("LoginTime").c_str());

			strcpy(info.CZCETime, getValueString("CZCETime").c_str());
			strcpy(info.DCETime, getValueString("DCETime").c_str());
			strcpy(info.SHFETime, getValueString("SHFETime").c_str());
			strcpy(info.INETime, getValueString("INETime").c_str());
			info.DifSeconds = getValueInt("DifSeconds");
			info.SettleInfoConfirmStatus = getValueInt(
					"SettleInfoConfirmStatus");
			info.DataReadyStatus = getValueInt("DataReadyStatus");
			strcpy(info.ConfirmDate, getValueString("ConfirmDate").c_str());
			strcpy(info.ConfirmTime, getValueString("ConfirmTime").c_str());
			strcpy(info.LocalCreateTime,
					getValueString("LocalCreateTime").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

		}

	}

	this->Empty();

//		printf("MysqlTradingDay -->Find_LastTradingDay End  %s\n",info.TradingDay);

	return info;

}
