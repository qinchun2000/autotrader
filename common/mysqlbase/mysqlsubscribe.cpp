#include "mysqlsubscribe.hpp"

MysqlSubscribe::MysqlSubscribe(const char* userid) {

	std::string database = "account";
	std::string tablename = "subscribe_";
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

void MysqlSubscribe::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];
		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
				"InstrumentID CHAR(20) NOT NULL PRIMARY KEY, "
				"Status BOOL,"
				"UserID CHAR(20),"
				"Processor CHAR(20),"
				"LocalUpdateTime CHAR(20)) default character set utf8 ",
				this->getTablename().c_str());

		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}
void MysqlSubscribe::Insert(SubscribeData data) {
	printf("MysqlSubscribe Insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (InstrumentID,Status,UserID,Processor,LocalUpdateTime) "
					"VALUES ('%s','%d','%s','%s','%s') ",
			this->getTablename().c_str(), data.InstrumentID, data.Status,
			data.UserID, data.Processor, time.c_str());

	str_sql = buf;

	this->Execute(str_sql);

	printf("MysqlSubscribe Insert end \n");
}

void MysqlSubscribe::Update(SubscribeData data) {
//	printf("Update begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"UPDATE %s SET Status='%d',LocalUpdateTime='%s' WHERE InstrumentID='%s' ",
			this->getTablename().c_str(), data.Status, time.c_str(),
			data.InstrumentID);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlSubscribe::DeleteAllbyProcessor(const char* processor) {
	//	printf("DeleteAll begin\n");

	char buf[200];
	std::string str_sql;

	sprintf(buf, "DELETE FROM %s  WHERE Processor='%s' ",
			this->getTablename().c_str(), processor);

	str_sql = buf;
	this->Execute(str_sql);

	this->Empty();
}

bool MysqlSubscribe::Exist_DatabyInstrumentID(const char *pinstrumentid) {
	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE InstrumentID='%s' ",
			this->getTablename().c_str(), pinstrumentid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}
	}
	this->Empty();
	return ret;
}
void MysqlSubscribe::Find_AllList(vector<SubscribeData> &temp) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			SubscribeData info;
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			info.Status = getValueInt("Status");
			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.Processor, getValueString("Processor").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlSubscribe::Find_AllListbyProcessor(vector<SubscribeData> &temp,
		const char* processor) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  WHERE Processor='%s' ",
			this->getTablename().c_str(), processor);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			SubscribeData info;
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			info.Status = getValueInt("Status");
			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.Processor, getValueString("Processor").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlSubscribe::Find_AllListbyUser(vector<SubscribeData> &temp,
		const char* user) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  WHERE UserID='%s' ",
			this->getTablename().c_str(), user);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			SubscribeData info;
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			info.Status = getValueInt("Status");
			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.Processor, getValueString("Processor").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}
