#include "mysqlproduct.hpp"

MysqlProduct::MysqlProduct() {

	std::string database = "baseinfo";
	std::string tablename = "product";

	if (IsAliveConn()) {

	} else {

		if (!Connect()) {

			return;
		}

	}

	this->Set_Database(database);
	this->Set_Table(tablename);

}

void MysqlProduct::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf,
				"CREATE TABLE IF NOT EXISTS %s ("
				//					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
						"ProductID CHAR(30) NOT NULL  PRIMARY KEY,"
						"ExchangeID CHAR(20),"
						"Section CHAR(20),"
						"DayNight CHAR(20)) default character set utf8 ",
				this->getTablename().c_str());
		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlProduct::Insert(ProductData data) {

	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "INSERT INTO %s (ProductID,ExchangeID,Section,DayNight) "
			"VALUES ('%s', '%s','%s','%s') ", tablename.c_str(), data.ProductID,
			data.ExchangeID, data.Section, data.DayNight);

	str_sql = buf;

	this->Execute(str_sql);

}

bool MysqlProduct::Exist_DatabyProductID(const char *productid) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE ProductID='%s' ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}

ProductData MysqlProduct::Find_DatabyProductID(const char *productid) {

	ProductData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE ProductID='%s' ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());

		}
	}
	this->Empty();
	return info;

}

void MysqlProduct::Find_DatabyProductID(ProductData &info,
		const char *productid) {

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE ProductID='%s' ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());

		}
	}

	this->Empty();

}

void MysqlProduct::Find_AllList(vector<ProductData> &temp) {

	ProductData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());

			if (strcmp(info.Section, "Nothing") != 0) {
				temp.push_back(info);
			}
			this->getNext();
		}
	}

	this->Empty();

}

