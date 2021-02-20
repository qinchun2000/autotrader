#include "mysqlexchange.hpp"

MysqlExchange::MysqlExchange() {

	std::string database = "baseinfo";
	std::string tablename = "exchange";

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

//	printf("connet to database [%s]OK\n",_DATABASE.c_str());

}

void MysqlExchange::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf,
				"CREATE TABLE IF NOT EXISTS %s ("
				//					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
						"ExchangeID CHAR(10) NOT NULL  PRIMARY KEY,"
						"ExchangeName CHAR(20),"
						"ExchangeProperty INT(11),"
						"ExchangeP INT(11)) default character set utf8 ",
				this->getTablename().c_str());

		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlExchange::Insert(ExchangeData data) {
//	printf("MysqlExchange Insert begin\n");
	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (ExchangeID,ExchangeName,ExchangeProperty,ExchangeP) "
					"VALUES ('%s', '%s','%d','%d') ", tablename.c_str(),
			data.ExchangeID, data.ExchangeName, data.ExchangeProperty,
			data.ExchangeP);

	str_sql = buf;

	this->Execute(str_sql);

//	printf("MysqlExchange Insert end \n");
}

bool MysqlExchange::Exist_DatabyExchangeID(const char *exchangeid) {
//	printf("MysqlExchange-->Exist_DatabyExchangeID begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT * FROM %s WHERE ExchangeID='%s' ",
			this->getTablename().c_str(), exchangeid);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	/* retrieve the row count in the result set */

//	printf("MysqlExchange--->Exist_DatabyExchangeID end\n");
	return ret;
}

ExchangeData MysqlExchange::Find_DatabyExchangeID(const char *exchangeid) {
//	printf("MysqlExchange -->Find_DatabyExchangeID begin\n");

	ExchangeData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE ExchangeID='%s' ",
			this->getTablename().c_str(), exchangeid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();
			//		 cout << res -> getString("Time") << endl;

			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ExchangeName, getValueString("ExchangeName").c_str());
			info.ExchangeProperty = getValueInt("ExchangeProperty");

		}
	}

//		printf("MysqlExchange -->Find_DatabyExchangeID End  %s\n",info.TradingDay);
	this->Empty();
	return info;

}

void MysqlExchange::Find_AllList(vector<ExchangeData> &temp) {

//		printf("Find_AllList begin\n");
	ExchangeData info;

	char buf[500];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			//			 cout << res -> getValueString("ExchangeID") << endl;
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ExchangeName, getValueString("ExchangeName").c_str());
			info.ExchangeProperty = getValueInt("ExchangeProperty");

			temp.push_back(info);
			this->getNext();
		}
	}

	this->Empty();

}

