#include "mysqlsignal.hpp"

MysqlSignal::MysqlSignal(const char* userid) {

	std::string database = "account";
	std::string tablename = "signal_";
	tablename.append(userid);

	if (IsAliveConn()) {
		//		printf("MysqlExchange IsAliveConn !!!! %s \n",database.c_str());

	} else {

//		printf("MysqlExchange IsAliveConn not connect !!!! %s \n",database.c_str());
		if (!Connect()) {

			return;
		}

	}

	this->Set_Database(database);
	this->Set_Table(tablename);

//	printf("connet to database [%s]OK\n",_DATABASE.c_str());

}

void MysqlSignal::CreateTable() {
	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];
		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
				"Time CHAR(30) NOT NULL PRIMARY KEY, "
				"TradingDay CHAR(20), "
				"Total_SingleDirection_SignalNumber INT,"
				"Total_SingleDirection_SectionNumber INT,"
				"SingleShort_SectionNumber INT,"
				"SingleLong_SectionNumber INT,"
				"Single_Long_SignalNumber INT,"
				"Single_Short_SignalNumber INT,"

				"High_Long_SignalNumber INT,"
				"High_Short_SignalNumber INT,"
				"High_Long_Section CHAR(40),"
				"High_Short_Section CHAR(40),"
				"High_Long_InstrumentID CHAR(40),"
				"High_Short_InstrumentID CHAR(40),"

				"RecommandInstrumentID CHAR(40),"
				"RecommandDirection INT,"
				"LocalUpdateTime CHAR(20)) default character set utf8 ",
				this->getTablename().c_str());

		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}
void MysqlSignal::Insert(SignalAssessData data) {
	printf("MysqlSignal Insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (Time,TradingDay,Total_SingleDirection_SignalNumber,Total_SingleDirection_SectionNumber,Single_Long_SignalNumber,"
					"Single_Short_SignalNumber,SingleLong_SectionNumber,SingleShort_SectionNumber,"
					"High_Long_SignalNumber,High_Short_SignalNumber,High_Long_Section,High_Short_Section,"
					"High_Long_InstrumentID,High_Short_InstrumentID,"
					"RecommandInstrumentID,RecommandDirection,"
					"LocalUpdateTime) "
					"VALUES ('%s','%s','%d','%d','%d',"
					"'%d','%d','%d',"
					"'%d','%d','%s','%s',"
					"'%s','%s',"
					"'%s','%d',"
					"'%s') ", this->getTablename().c_str(), data.Time,
			data.TradingDay, data.Total_SingleDirection_SignalNumber,
			data.Total_SingleDirection_SectionNumber,
			data.Single_Long_SignalNumber, data.Single_Short_SignalNumber,
			data.SingleLong_SectionNumber, data.SingleShort_SectionNumber,
			data.High_Long_SignalNumber, data.High_Short_SignalNumber,data.High_Long_Section, data.High_Short_Section,
			data.High_Long_InstrumentID,data.High_Short_InstrumentID,
			data.RecommandInstrumentID, data.RecommandDirection, time.c_str());

	str_sql = buf;

	this->Execute(str_sql);

	printf("MysqlSignal Insert end \n");
}

void MysqlSignal::Update(SignalAssessData data) {
//	printf("Update begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"UPDATE %s SET Total_SingleDirection_SignalNumber='%d',LocalUpdateTime='%s' WHERE TradingDay='%s' ",
			this->getTablename().c_str(),
			data.Total_SingleDirection_SignalNumber, time.c_str(),
			data.TradingDay);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);

}
bool MysqlSignal::Exist_DatabyTradingDay(const char* date) {
//	printf("MysqlTradingDay-->Exist_TradingDay begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s WHERE TradingDay='%s' ",
			this->getTablename().c_str(), date);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() >= 1) {
			ret = true;
		}

	}

	this->Empty();
	/* retrieve the row count in the result set */

//	printf("MysqlTradingDay--->Exist_TradingDay end\n");
	return ret;
}
void MysqlSignal::Find_AllList(vector<SignalAssessData> &temp) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			SignalAssessData info;
			strcpy(info.Time, getValueString("Time").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.Total_SingleDirection_SignalNumber = getValueInt(
					"Total_SingleDirection_SignalNumber");
			info.Total_SingleDirection_SectionNumber = getValueInt(
					"Total_SingleDirection_SectionNumber");
			info.SingleLong_SectionNumber = getValueInt(
					"SingleLong_SectionNumber");
			info.SingleShort_SectionNumber = getValueInt(
					"SingleShort_SectionNumber");
			info.Single_Long_SignalNumber = getValueInt(
					"Single_Long_SignalNumber");
			info.Single_Short_SignalNumber = getValueInt(
					"Single_Short_SignalNumber");

			info.High_Long_SignalNumber = getValueInt("High_Long_SignalNumber");
			info.High_Short_SignalNumber = getValueInt(
					"High_Short_SignalNumber");
			strcpy(info.High_Long_Section,
					getValueString("High_Long_Section").c_str());
			strcpy(info.High_Short_Section,
					getValueString("High_Short_Section").c_str());

			strcpy(info.High_Long_InstrumentID,
					getValueString("High_Long_InstrumentID").c_str());
			strcpy(info.High_Short_InstrumentID,
					getValueString("High_Short_InstrumentID").c_str());


			strcpy(info.RecommandInstrumentID,
					getValueString("RecommandInstrumentID").c_str());
			info.RecommandDirection = getValueInt("RecommandDirection");

			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlSignal::Find_AllListbyTradingDay(vector<SignalAssessData> &temp,const char * tradingday) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE TradingDay='%s' ", this->getTablename().c_str(),tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			SignalAssessData info;
			strcpy(info.Time, getValueString("Time").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.Total_SingleDirection_SignalNumber = getValueInt(
					"Total_SingleDirection_SignalNumber");
			info.Total_SingleDirection_SectionNumber = getValueInt(
					"Total_SingleDirection_SectionNumber");
			info.SingleLong_SectionNumber = getValueInt(
					"SingleLong_SectionNumber");
			info.SingleShort_SectionNumber = getValueInt(
					"SingleShort_SectionNumber");
			info.Single_Long_SignalNumber = getValueInt(
					"Single_Long_SignalNumber");
			info.Single_Short_SignalNumber = getValueInt(
					"Single_Short_SignalNumber");

			info.High_Long_SignalNumber = getValueInt("High_Long_SignalNumber");
			info.High_Short_SignalNumber = getValueInt(
					"High_Short_SignalNumber");
			strcpy(info.High_Long_Section,
					getValueString("High_Long_Section").c_str());
			strcpy(info.High_Short_Section,
					getValueString("High_Short_Section").c_str());

			strcpy(info.High_Long_InstrumentID,
					getValueString("High_Long_InstrumentID").c_str());
			strcpy(info.High_Short_InstrumentID,
					getValueString("High_Short_InstrumentID").c_str());


			strcpy(info.RecommandInstrumentID,
					getValueString("RecommandInstrumentID").c_str());
			info.RecommandDirection = getValueInt("RecommandDirection");

			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

SignalAssessData MysqlSignal::Find_LastDatabyTradingDay(
		const char* tradingday) {
	SignalAssessData info;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,
			"SELECT * FROM %s  WHERE TradingDay='%s' ORDER BY Time DESC LIMIT 1 ",
			this->getTablename().c_str(), tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.Time, getValueString("Time").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.Total_SingleDirection_SignalNumber = getValueInt(
					"Total_SingleDirection_SignalNumber");
			info.Total_SingleDirection_SectionNumber = getValueInt(
					"Total_SingleDirection_SectionNumber");
			info.SingleLong_SectionNumber = getValueInt(
					"SingleLong_SectionNumber");
			info.SingleShort_SectionNumber = getValueInt(
					"SingleShort_SectionNumber");
			info.Single_Long_SignalNumber = getValueInt(
					"Single_Long_SignalNumber");
			info.Single_Short_SignalNumber = getValueInt(
					"Single_Short_SignalNumber");

			info.High_Long_SignalNumber = getValueInt("High_Long_SignalNumber");
			info.High_Short_SignalNumber = getValueInt(
					"High_Short_SignalNumber");
			strcpy(info.High_Long_Section,
					getValueString("High_Long_Section").c_str());
			strcpy(info.High_Short_Section,
					getValueString("High_Short_Section").c_str());

			strcpy(info.High_Long_InstrumentID,
					getValueString("High_Long_InstrumentID").c_str());
			strcpy(info.High_Short_InstrumentID,
					getValueString("High_Short_InstrumentID").c_str());


			strcpy(info.RecommandInstrumentID,
					getValueString("RecommandInstrumentID").c_str());
			info.RecommandDirection = getValueInt("RecommandDirection");
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
		}
	}

	this->Empty();
	return info;

}
SignalAssessData MysqlSignal::Find_MaxSignalNumberbyTradingDay(
		const char* tradingday) {
	SignalAssessData info;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,
			"SELECT * FROM %s  WHERE TradingDay='%s' ORDER BY Total_SingleDirection_SignalNumber DESC LIMIT 1 ",
			this->getTablename().c_str(), tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.Time, getValueString("Time").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.Total_SingleDirection_SignalNumber = getValueInt(
					"Total_SingleDirection_SignalNumber");
			info.Total_SingleDirection_SectionNumber = getValueInt(
					"Total_SingleDirection_SectionNumber");
			info.SingleLong_SectionNumber = getValueInt(
					"SingleLong_SectionNumber");
			info.SingleShort_SectionNumber = getValueInt(
					"SingleShort_SectionNumber");
			info.Single_Long_SignalNumber = getValueInt(
					"Single_Long_SignalNumber");
			info.Single_Short_SignalNumber = getValueInt(
					"Single_Short_SignalNumber");

			info.High_Long_SignalNumber = getValueInt("High_Long_SignalNumber");
			info.High_Short_SignalNumber = getValueInt(
					"High_Short_SignalNumber");
			strcpy(info.High_Long_Section,
					getValueString("High_Long_Section").c_str());
			strcpy(info.High_Short_Section,
					getValueString("High_Short_Section").c_str());

			strcpy(info.High_Long_InstrumentID,
					getValueString("High_Long_InstrumentID").c_str());
			strcpy(info.High_Short_InstrumentID,
					getValueString("High_Short_InstrumentID").c_str());


			strcpy(info.RecommandInstrumentID,
					getValueString("RecommandInstrumentID").c_str());
			info.RecommandDirection = getValueInt("RecommandDirection");
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
		}
	}

	this->Empty();
	return info;

}
