#include "mysqluser.hpp"

MysqlUser::MysqlUser() {
	std::string database = "baseinfo";
	std::string tablename = "user";
//	printf("MysqlUser   ---> begin !\n");
	if (IsAliveConn()) {

//		printf("MysqlUser   ---> connect alive ok!\n");

	} else {

//			printf("MysqlUser   ---> connect not alive!\n");
		if (!Connect()) {

			return;
		}

	}

	this->Set_Database(database);
	this->Set_Table(tablename);

}

void MysqlUser::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
				"UserID CHAR(20) NOT NULL  PRIMARY KEY,"
				"BrokerID CHAR(20),"
				"InvestorID CHAR(20),"
				"Password CHAR(20),"
				"StrategyName CHAR(50),"
				"Actived BOOL ) default character set utf8 ",
				this->getTablename().c_str());
		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}
void MysqlUser::Insert(User data) {

	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"INSERT INTO %s (UserID,BrokerID,InvestorID,Password,StrategyName,Actived) "
					"VALUES ('%s', '%s','%s','%s','%s','%d') ",
			tablename.c_str(), data.UserID, data.BrokerID, data.InvestorID,
			data.Password, data.StrategyName, data.Actived);
	str_sql = buf;

	this->Execute(str_sql);

}

void MysqlUser::Update(User data) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"UPDATE %s SET Password='%s',Actived='%d',StrategyName='%s' WHERE UserID='%s' ",
			this->getTablename().c_str(), data.Password, data.Actived,
			data.StrategyName, data.UserID);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);

}

bool MysqlUser::Exist_DatabyUserID(const char *user) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE UserID='%s' ",
			this->getTablename().c_str(), user);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}

bool MysqlUser::Exist_ActiveDataByUserID(const char *user) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE Actived='%d'  AND  UserID='%s'  ",
			this->getTablename().c_str(), true, user);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}

bool MysqlUser::Exist_ActiveDataByStrategyName(const char *strategyname) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE Actived='%d'  AND  StrategyName='%s'  ",
			this->getTablename().c_str(), true, strategyname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}

bool MysqlUser::Exist_DataByStrategyName(const char *strategyname) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE StrategyName='%s'  ",
			this->getTablename().c_str(), strategyname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}
bool MysqlUser::Exist_ActiveData() {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE Actived='%d' ",
			this->getTablename().c_str(), true);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}

User MysqlUser::Find_DatabyUserID(const char *user) {

	User info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE UserID='%s'  ",
			this->getTablename().c_str(), user);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.BrokerID, getValueString("BrokerID").c_str());
			strcpy(info.InvestorID, getValueString("InvestorID").c_str());
			strcpy(info.Password, getValueString("Password").c_str());
			strcpy(info.StrategyName, getValueString("StrategyName").c_str());
			info.Actived = getValueInt("Actived");

		}
	}
	this->Empty();
	return info;

}

User MysqlUser::Find_ActiveData() {

	User info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE Actived='%d'  ",
			this->getTablename().c_str(), true);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.BrokerID, getValueString("BrokerID").c_str());
			strcpy(info.InvestorID, getValueString("InvestorID").c_str());
			strcpy(info.Password, getValueString("Password").c_str());
			strcpy(info.StrategyName, getValueString("StrategyName").c_str());
			info.Actived = getValueInt("Actived");

		}
	}
	this->Empty();
	return info;

}

User MysqlUser::Find_ActiveDatabyStrategyName(const char* strategyname) {

	User info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE Actived='%d' AND  StrategyName='%s'   ",
			this->getTablename().c_str(), true, strategyname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.BrokerID, getValueString("BrokerID").c_str());
			strcpy(info.InvestorID, getValueString("InvestorID").c_str());
			strcpy(info.Password, getValueString("Password").c_str());
			strcpy(info.StrategyName, getValueString("StrategyName").c_str());
			info.Actived = getValueInt("Actived");

		}
	}
	this->Empty();
	return info;

}

User MysqlUser::Find_DatabyStrategyName(const char* strategyname) {

	User info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE StrategyName='%s'   ",
			this->getTablename().c_str(), strategyname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.BrokerID, getValueString("BrokerID").c_str());
			strcpy(info.InvestorID, getValueString("InvestorID").c_str());
			strcpy(info.Password, getValueString("Password").c_str());
			strcpy(info.StrategyName, getValueString("StrategyName").c_str());
			info.Actived = getValueInt("Actived");

		}
	}
	this->Empty();
	return info;

}
void MysqlUser::Find_AllList(vector<User> &temp) {

	User info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.BrokerID, getValueString("BrokerID").c_str());
			strcpy(info.InvestorID, getValueString("InvestorID").c_str());
			strcpy(info.Password, getValueString("Password").c_str());
			strcpy(info.StrategyName, getValueString("StrategyName").c_str());
			info.Actived = getValueInt("Actived");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}
void MysqlUser::Find_AllActiveListbyStrategyName(vector<User> &temp) {

	User info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE Actived='%d' ",
			this->getTablename().c_str(), true);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.UserID, getValueString("UserID").c_str());
			strcpy(info.BrokerID, getValueString("BrokerID").c_str());
			strcpy(info.InvestorID, getValueString("InvestorID").c_str());
			strcpy(info.Password, getValueString("Password").c_str());
			strcpy(info.StrategyName, getValueString("StrategyName").c_str());
			info.Actived = getValueInt("Actived");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

