#include "mysqlinstrument.hpp"

MysqlInstrument::MysqlInstrument() {

	std::string database = "baseinfo";
	std::string tablename = "instrument";

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

}

void MysqlInstrument::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf,
				"CREATE TABLE IF NOT EXISTS %s ("
				//					"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
						"InstrumentID CHAR(40) NOT NULL  PRIMARY KEY,"
						"ExchangeID CHAR(27),"
						"InstrumentName CHAR(40),"
						"ExchangeInstID CHAR(30),"
						"ProductID CHAR(30),"

						"ProductClass INT(11),"
						"DeliveryYear INT(11),"
						"DeliveryMonth INT(11),"
						"VolumeMultiple INT(11),"
						"PriceTick DOUBLE,"

						"CreateDate CHAR(27),"
						"OpenDate CHAR(27),"
						"ExpireDate CHAR(27),"
						"StartDelivDate CHAR(27),"
						"EndDelivDate CHAR(27),"

						"InstLifePhase INT(11),"
						"LongMarginRatio DOUBLE,"
						"ShortMarginRatio DOUBLE,"
						"LocalUpdateTime DATETIME) default character set utf8; ",
				this->getTablename().c_str());
		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlInstrument::Insert(InstrumentData data) {

	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (InstrumentID,ExchangeID,InstrumentName,ExchangeInstID,ProductID,"
					"ProductClass,DeliveryYear,DeliveryMonth,VolumeMultiple,PriceTick,"
					"CreateDate,OpenDate,ExpireDate,StartDelivDate,EndDelivDate,"
					"InstLifePhase,LongMarginRatio,ShortMarginRatio,LocalUpdateTime)"
					"VALUES ('%s', '%s','%s','%s','%s',"
					"'%d','%d','%d','%d','%.3f',"
					"'%s','%s','%s','%s','%s',"
					"'%d','%.3f','%.3f','%s')", tablename.c_str(),
			data.InstrumentID, data.ExchangeID, data.InstrumentName,
			data.ExchangeInstID, data.ProductID, data.ProductClass,
			data.DeliveryYear, data.DeliveryMonth, data.VolumeMultiple,
			data.PriceTick, data.CreateDate, data.OpenDate, data.ExpireDate,
			data.StartDelivDate, data.EndDelivDate, data.InstLifePhase,
			data.LongMarginRatio, data.ShortMarginRatio, time.c_str());
	str_sql = buf;

	this->Execute(str_sql);

}

std::string MysqlInstrument::GetLastLocalUpdateTime() {

	std::string info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s ORDER BY LocalUpdateTime desc limit 1 ",
			this->getTablename().c_str());
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();
			info = getValueString("LocalUpdateTime");
		}
	}
	this->Empty();
	return info;

}

InstrumentData MysqlInstrument::FindLastExpireDateInstrument() {

	InstrumentData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s  WHERE ProductClass='%d' ORDER BY ExpireDate asc limit 1 ",
			this->getTablename().c_str(), 1);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() >= 1) {
			this->getFirst();

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.ExchangeInstID,
					getValueString("ExchangeInstID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());

			info.ProductClass = getValueInt("ProductClass");
			info.DeliveryYear = getValueInt("DeliveryYear");
			info.DeliveryMonth = getValueInt("DeliveryMonth");
			info.VolumeMultiple = getValueInt("VolumeMultiple");
			info.PriceTick = getValueDouble("PriceTick");

			strcpy(info.CreateDate, getValueString("CreateDate").c_str());
			strcpy(info.OpenDate, getValueString("OpenDate").c_str());
			strcpy(info.ExpireDate, getValueString("ExpireDate").c_str());
			strcpy(info.StartDelivDate,
					getValueString("StartDelivDate").c_str());
			strcpy(info.EndDelivDate, getValueString("EndDelivDate").c_str());

			info.InstLifePhase = getValueInt("InstLifePhase");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

		}
	}
	this->Empty();
	return info;

}

bool MysqlInstrument::Exist_DatabyInstrumentID(const char *pinstrumentid) {

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

InstrumentData MysqlInstrument::Find_DatabyInstrumentID(
		const char *pinstrumentid) {

	InstrumentData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE InstrumentID='%s' ",
			this->getTablename().c_str(), pinstrumentid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.ExchangeInstID,
					getValueString("ExchangeInstID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());

			info.ProductClass = getValueInt("ProductClass");
			info.DeliveryYear = getValueInt("DeliveryYear");
			info.DeliveryMonth = getValueInt("DeliveryMonth");
			info.VolumeMultiple = getValueInt("VolumeMultiple");
			info.PriceTick = getValueDouble("PriceTick");

			strcpy(info.CreateDate, getValueString("CreateDate").c_str());
			strcpy(info.OpenDate, getValueString("OpenDate").c_str());
			strcpy(info.ExpireDate, getValueString("ExpireDate").c_str());
			strcpy(info.StartDelivDate,
					getValueString("StartDelivDate").c_str());
			strcpy(info.EndDelivDate, getValueString("EndDelivDate").c_str());

			info.InstLifePhase = getValueInt("InstLifePhase");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
		}
	}
	this->Empty();
	return info;

}

void MysqlInstrument::Find_AllList(vector<InstrumentData> &temp) {

	InstrumentData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.ExchangeInstID,
					getValueString("ExchangeInstID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());

			info.ProductClass = getValueInt("ProductClass");
			info.DeliveryYear = getValueInt("DeliveryYear");
			info.DeliveryMonth = getValueInt("DeliveryMonth");
			info.VolumeMultiple = getValueInt("VolumeMultiple");
			info.PriceTick = getValueDouble("PriceTick");

			strcpy(info.CreateDate, getValueString("CreateDate").c_str());
			strcpy(info.OpenDate, getValueString("OpenDate").c_str());
			strcpy(info.ExpireDate, getValueString("ExpireDate").c_str());
			strcpy(info.StartDelivDate,
					getValueString("StartDelivDate").c_str());
			strcpy(info.EndDelivDate, getValueString("EndDelivDate").c_str());

			info.InstLifePhase = getValueInt("InstLifePhase");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlInstrument::Find_ListsbyProductID(vector<InstrumentData> &temp,
		const char * productid) {
	InstrumentData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE ProductID='%s' ",
			this->getTablename().c_str(), productid);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.ExchangeInstID,
					getValueString("ExchangeInstID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());

			info.ProductClass = getValueInt("ProductClass");
			info.DeliveryYear = getValueInt("DeliveryYear");
			info.DeliveryMonth = getValueInt("DeliveryMonth");
			info.VolumeMultiple = getValueInt("VolumeMultiple");
			info.PriceTick = getValueDouble("PriceTick");

			strcpy(info.CreateDate, getValueString("CreateDate").c_str());
			strcpy(info.OpenDate, getValueString("OpenDate").c_str());
			strcpy(info.ExpireDate, getValueString("ExpireDate").c_str());
			strcpy(info.StartDelivDate,
					getValueString("StartDelivDate").c_str());
			strcpy(info.EndDelivDate, getValueString("EndDelivDate").c_str());

			info.InstLifePhase = getValueInt("InstLifePhase");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

