#include "mysqlmaincontract.hpp"

MysqlMaincontract::MysqlMaincontract() {
	std::string database = "baseinfo";
	std::string tablename = "maincontract";

	if (IsAliveConn()) {

	} else {
		if (!Connect()) {

			return;
		}

	}

	this->Set_Database(database);
	this->Set_Table(tablename);

}

void MysqlMaincontract::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("

				"ProductID CHAR(30) NOT NULL  PRIMARY KEY,"
				"InstrumentName CHAR(40),"
				"InstrumentID CHAR(40),"
				"TradingDay CHAR(20),"
				"ActionDay CHAR(20),"

				"UpdateTime CHAR(20),"
				"ExchangeID CHAR(10),"
				"Section CHAR(20),"
				"DayNight CHAR(20),"
				"VolumeMultiple INT(11),"

				"PriceTick DOUBLE,"
				"UpperLimitPrice DOUBLE,"
				"LowerLimitPrice DOUBLE,"
				"LongMarginRatio DOUBLE,"
				"ShortMarginRatio DOUBLE,"

				"MaxOpenInterest INT,"
				"OpenInterest INT,"
				"Volume INT,"
				"Sedimentary DOUBLE,"
				"LastPrice DOUBLE,"
				"DaylineCount INT,"

				"InstrumentID_next CHAR(40),"
				"OpenInterest_next INT,"
				"UpperLimitPrice_next DOUBLE,"
				"LowerLimitPrice_next DOUBLE,"
				"Sedimentary_next DOUBLE,"

				"LastPrice_next DOUBLE,"
				"Volume_next INT,"
				"DaylineCount_next INT,"
				"ErrorDaylineCount INT,"
				"ErrorDaylineCount_next INT,"

				"InstrumentID_trade CHAR(40),"
				"LocalUpdateTime DATETIME)  default character set utf8",
				this->getTablename().c_str());
		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlMaincontract::Insert(MainContractData data) {

	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (ProductID,InstrumentName,InstrumentID,TradingDay,ActionDay,"
					"UpdateTime,ExchangeID,Section,DayNight,VolumeMultiple,"
					"PriceTick,UpperLimitPrice,LowerLimitPrice,LongMarginRatio,ShortMarginRatio,"
					"MaxOpenInterest,OpenInterest,Volume,Sedimentary,LastPrice,DaylineCount,"
					"InstrumentID_next,OpenInterest_next,UpperLimitPrice_next,LowerLimitPrice_next,Sedimentary_next,"
					"LastPrice_next,Volume_next,DaylineCount_next,InstrumentID_trade,ErrorDaylineCount,"
					"ErrorDaylineCount_next,LocalUpdateTime)"
					"VALUES ('%s', '%s','%s','%s','%s',"
					"'%s','%s','%s','%s','%d',"
					"'%.3f','%.3f','%.3f','%.3f','%.3f',"
					"'%d','%d','%d','%.3f','%.3f','%d',"
					"'%s','%d','%.3f','%.3f','%.3f',"
					"'%.3f','%d','%d','%s','%d',"
					"'%d','%s')", tablename.c_str(), data.ProductID,
			data.InstrumentName, data.InstrumentID, data.TradingDay,
			data.ActionDay, data.UpdateTime, data.ExchangeID, data.Section,
			data.DayNight, data.VolumeMultiple, data.PriceTick,
			data.UpperLimitPrice, data.LowerLimitPrice, data.LongMarginRatio,
			data.ShortMarginRatio, data.MaxOpenInterest, data.OpenInterest,
			data.Volume, data.Sedimentary, data.LastPrice, data.DaylineCount,
			data.InstrumentID_next, data.OpenInterest_next,
			data.UpperLimitPrice_next, data.LowerLimitPrice_next,
			data.Sedimentary_next, data.LastPrice_next, data.Volume_next,
			data.DaylineCount_next, data.InstrumentID_trade,
			data.ErrorDaylineCount, data.ErrorDaylineCount_next, time.c_str());
	str_sql = buf;

	this->Execute(str_sql);

}

void MysqlMaincontract::UpdateErrorDaylineCount(int count,
		const char* instrumentid) {
//	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "UPDATE %s SET ErrorDaylineCount ='%d',LocalUpdateTime='%s'  "
			"WHERE InstrumentID='%s' ", this->getTablename().c_str(), count,
			time.c_str(), instrumentid);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

void MysqlMaincontract::UpdateErrorDaylineCountNext(int count,
		const char* instrumentid) {
//	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"UPDATE %s SET ErrorDaylineCount_next ='%d',LocalUpdateTime='%s'  "
					"WHERE InstrumentID_next='%s' ",
			this->getTablename().c_str(), count, time.c_str(), instrumentid);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);
//	conn->commit();
}

std::string MysqlMaincontract::GetLastLocalUpdateTime() {

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

std::string MysqlMaincontract::GetOldestTradingDay() {

	std::string info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s ORDER BY TradingDay ASC limit 1 ",
			this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();
			info = getValueString("TradingDay");
		}
	}
	this->Empty();
	return info;

}

bool MysqlMaincontract::Exist_DatabyProductID(const char *productid) {

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

bool MysqlMaincontract::Exist_DatabyInstrumentID(const char *pinstrumentid) {

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

bool MysqlMaincontract::Exist_DatabyInstrumentIDNext(
		const char *pinstrumentid) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE InstrumentID_next='%s' ",
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
MainContractData MysqlMaincontract::Find_DatabyInstrumentID(
		const char *pinstrumentid) {

	MainContractData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE InstrumentID='%s' ",
			this->getTablename().c_str(), pinstrumentid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
		}
	}
	this->Empty();
	return info;

}

MainContractData MysqlMaincontract::Find_DatabyInstrumentIDNext(
		const char *pinstrumentid) {

	MainContractData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE InstrumentID_next='%s' ",
			this->getTablename().c_str(), pinstrumentid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
		}
	}
	this->Empty();
	return info;

}

MainContractData MysqlMaincontract::Find_DatabyProductID(
		const char *productid) {

	MainContractData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE ProductID='%s' ",
			this->getTablename().c_str(), productid);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());
		}
	}
	this->Empty();
	return info;

}

void MysqlMaincontract::Find_AllList(vector<MainContractData> &temp) {

	MainContractData info;

	char buf[500];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			if (strcmp(info.ExchangeID, "INE") != 0
					&& strcmp(info.ProductID, "T") != 0
					&& strcmp(info.ProductID, "TF") != 0) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}

void MysqlMaincontract::Find_TradeSectionList(vector<MainContractData> &temp) {
	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			MainContractData info;
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");

			if (strcmp(info.InstrumentID_trade, info.InstrumentID) == 0) {
				info.Sedimentary = getValueDouble("Sedimentary");
			} else if (strcmp(info.InstrumentID_trade, info.InstrumentID_next)
					== 0) {
				info.Sedimentary = getValueDouble("Sedimentary_next");
			}

			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");

			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;
			bool is_black = strcmp(info.Section, "Black") == 0;

//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
			if (not_ine && not_cffex) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}

void MysqlMaincontract::Find_CycleNList(vector<MainContractData> &temp) {

	MainContractData info;

	char buf[500];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");

			if (strcmp(info.InstrumentID_trade, info.InstrumentID) == 0) {
				info.Sedimentary = getValueDouble("Sedimentary");
			} else if (strcmp(info.InstrumentID_trade, info.InstrumentID_next)
					== 0) {
				info.Sedimentary = getValueDouble("Sedimentary_next");
			}

			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");

			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;
			bool is_black = strcmp(info.Section, "Black") == 0;
			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
			if (not_ine && not_cffex && not_metal && not_precious) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}
void MysqlMaincontract::Find_CycleNList_Ndays(vector<MainContractData> &temp,
		int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE DaylineCount > %d   ",
			this->getTablename().c_str(), n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");

			if (strcmp(info.InstrumentID_trade, info.InstrumentID) == 0) {
				info.Sedimentary = getValueDouble("Sedimentary");
			} else if (strcmp(info.InstrumentID_trade, info.InstrumentID_next)
					== 0) {
				info.Sedimentary = getValueDouble("Sedimentary_next");
			}

			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");

			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;
			bool is_black = strcmp(info.Section, "Black") == 0;
			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
			if (not_ine && not_cffex && not_metal && not_precious) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}

void MysqlMaincontract::Find_Night_CycleNList_Ndays(
		vector<MainContractData> &temp, int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE DayNight='DayNight' AND DaylineCount > %d   ",
			this->getTablename().c_str(), n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");

			if (strcmp(info.InstrumentID_trade, info.InstrumentID) == 0) {
				info.Sedimentary = getValueDouble("Sedimentary");
			} else if (strcmp(info.InstrumentID_trade, info.InstrumentID_next)
					== 0) {
				info.Sedimentary = getValueDouble("Sedimentary_next");
			}

			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");

			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;
			bool is_black = strcmp(info.Section, "Black") == 0;
			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
			if (not_ine && not_cffex && not_metal && not_precious) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}

void MysqlMaincontract::Find_ListsbySection(vector<MainContractData> &temp,
		const char* sectionname) {

	MainContractData info;

	char buf[500];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  WHERE Section ='%s' ",
			this->getTablename().c_str(), sectionname);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			if (strcmp(info.ExchangeID, "INE") != 0) {
				temp.push_back(info);
			}
			this->getNext();
		}
	}

	this->Empty();

}

void MysqlMaincontract::Find_AllListorderbySed(vector<MainContractData> &temp) {

	MainContractData info;

	char buf[500];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  ORDER BY Sedimentary DESC",
			this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			if (strcmp(info.ExchangeID, "INE") != 0
					&& strcmp(info.ProductID, "T") != 0
					&& strcmp(info.ProductID, "TF") != 0) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}

void MysqlMaincontract::Find_AllListorderbySedN20(
		vector<MainContractData> &temp) {

	MainContractData info;

	char buf[500];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s  WHERE DaylineCount > 20  AND DaylineCount_next >20 ORDER BY Sedimentary DESC",
			this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			if (strcmp(info.ExchangeID, "INE") != 0
					&& strcmp(info.ProductID, "T") != 0
					&& strcmp(info.ProductID, "TF") != 0) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}
void MysqlMaincontract::Find_LimitListbySed_Ndays(
		vector<MainContractData> &temp, int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,"SELECT  * FROM %s  WHERE DaylineCount > %d  ORDER BY Sedimentary DESC",
			this->getTablename().c_str(),n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.ErrorDaylineCount = getValueInt("ErrorDaylineCount");
			info.ErrorDaylineCount_next = getValueInt("ErrorDaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;

			bool is_black = strcmp(info.Section, "Black") == 0;
			bool is_feed = strcmp(info.Section, "Feed") == 0;
			bool is_oil = strcmp(info.Section, "Oil") == 0;
			bool is_cookoil = strcmp(info.Section, "CookOil") == 0;

			bool is_productid_rb = strcmp(info.ProductID, "rb") == 0;
			bool is_productid_ag = strcmp(info.ProductID, "ag") == 0;

			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
//				 if( not_ine  && not_cffex && not_metal && not_precious ){
//			if (is_black || is_feed || is_oil || is_cookoil) {
			if (is_productid_rb || is_productid_ag ) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}
void MysqlMaincontract::Find_SpreadListorderbySed_Ndays(
		vector<MainContractData> &temp, int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s  WHERE DaylineCount > %d  AND DaylineCount_next >%d  "
					" ORDER BY Sedimentary DESC", this->getTablename().c_str(),
			n, n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.ErrorDaylineCount = getValueInt("ErrorDaylineCount");
			info.ErrorDaylineCount_next = getValueInt("ErrorDaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;

			bool is_black = strcmp(info.Section, "Black") == 0;
			bool is_feed = strcmp(info.Section, "Feed") == 0;
			bool is_oil = strcmp(info.Section, "Oil") == 0;
			bool is_cookoil = strcmp(info.Section, "CookOil") == 0;

			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
//				 if( not_ine  && not_cffex && not_metal && not_precious ){
			if (is_black || is_feed || is_oil || is_cookoil) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}
void MysqlMaincontract::Find_Night_SpreadListorderbySed_Ndays(
		vector<MainContractData> &temp, int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s  WHERE DayNight='DayNight' AND DaylineCount > %d  AND DaylineCount_next >%d "
					"ORDER BY Sedimentary DESC ", this->getTablename().c_str(),
			n, n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.ErrorDaylineCount = getValueInt("ErrorDaylineCount");
			info.ErrorDaylineCount_next = getValueInt("ErrorDaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;

			bool is_black = strcmp(info.Section, "Black") == 0;
			bool is_feed = strcmp(info.Section, "Feed") == 0;
			bool is_oil = strcmp(info.Section, "Oil") == 0;
			bool is_cookoil = strcmp(info.Section, "CookOil") == 0;

			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
//				 if( not_ine  && not_cffex && not_metal && not_precious ){
			if (is_black || is_feed || is_oil || is_cookoil) {
				temp.push_back(info);
			}

			this->getNext();
		}
	}

	this->Empty();

}
void MysqlMaincontract::GetTradeInstrumentLists(vector<std::string> &temp) {

	printf("--->>> GetTradeInstrumentLists   temp---->[%ld] \n", temp.size());

	vector<MainContractData> list;
	this->Find_AllListorderbySed(list);

	printf(
			"--->>> GetTradeInstrumentLists    Find_AllListorderbySed -->主力合约数---->[%ld] \n",
			list.size());

	vector<User> userlist;
	MysqlUser mysqluser;
	mysqluser.Find_AllActiveListbyStrategyName(userlist);
	for (auto &user : userlist) {
		MysqlInvestPosition mysqlinvest(user.UserID);

		vector<InvestPositionData> investinfo;
		mysqlinvest.Find_AllLists(investinfo);
		printf("user : %s --->>> GetTradeInstrumentLists    持仓列表数：%ld \n",
				user.UserID, investinfo.size());
		for (auto &invest : investinfo) {
			bool exists = false;
			for (auto &item : list) {
				if (strcmp(item.InstrumentID_trade, invest.InstrumentID) == 0) {
					printf("-------> 主力合约已经存在 %s  ！！！！ 合约[%s]无需加入订阅列表 \n",
							item.InstrumentID_trade, invest.InstrumentID);
					exists = true;
					break;
				}
//					else if(strcmp(item.InstrumentID_next,invest.InstrumentID)==0)
//					{
//						exists=true;
//						break;
//					}
			}

			if (!exists) {
				printf("-------> 不存在   %s  将合约加入行情订阅表 \n", invest.InstrumentID);
				std::string str;
				str = invest.InstrumentID;

				temp.push_back(str);

			}
//				else
//				{
//					printf("-------> 主力合约已经存在   ！！！！ 合约[%s]无需加入订阅列表 \n",invest.InstrumentID);
//				}

		}
		vector<InvestPositionData>().swap(investinfo);

	}

	printf("--->>> GetTradeInstrumentLists    持仓合约加入订阅合约的条数为---->[%ld] \n",
			temp.size());
	for (auto &item : list) {
//		printf("ppInstrumentID -->  %s   %s\n",item.InstrumentID,item.InstrumentID_next);
		std::string str;
		str = item.InstrumentID_trade;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str);
		}

//		std::string str_next;
//		str_next=item.InstrumentID_next;
//
//		if(strcmp(item.ExchangeID,"INE")!=0)
//		{
//			temp.push_back(str_next);
//		}

	}

	printf("--->>> GetTradeInstrumentLists    加入当前主力合约，最终订阅合约数---->[%ld] \n",
			temp.size());
	vector<User>().swap(userlist);
	vector<MainContractData>().swap(list);

}

void MysqlMaincontract::GetTradeInstrumentListsbyMain(
		vector<std::string> &temp) {

	printf("--->>> GetTradeInstrumentListsbyMain   temp---->[%ld] \n",
			temp.size());

	vector<MainContractData> list;
	this->Find_AllListorderbySed(list);

	printf(
			"--->>> GetTradeInstrumentListsbyMain    Find_AllListorderbySed -->主力合约数---->[%ld] \n",
			list.size());

	vector<User> userlist;
	MysqlUser mysqluser;
	mysqluser.Find_AllActiveListbyStrategyName(userlist);
	for (auto &user : userlist) {
		MysqlInvestPosition mysqlinvest(user.UserID);

		vector<InvestPositionData> investinfo;
		mysqlinvest.Find_AllLists(investinfo);
		printf("user : %s --->>> GetTradeInstrumentListsbyMain    持仓列表数：%ld \n",
				user.UserID, investinfo.size());
		for (auto &invest : investinfo) {
			bool exists = false;
			for (auto &item : list) {
				if (strcmp(item.InstrumentID, invest.InstrumentID) == 0) {
					printf(
							"GetTradeInstrumentListsbyMain-------> 主力合约已经存在 %s  ！！！！ 合约[%s]无需加入订阅列表 \n",
							item.InstrumentID_trade, invest.InstrumentID);
					exists = true;
					break;
				}
//					else if(strcmp(item.InstrumentID_next,invest.InstrumentID)==0)
//					{
//						exists=true;
//						break;
//					}
			}

			if (!exists) {
				printf(
						"GetTradeInstrumentListsbyMain-------> 不存在   %s  将合约加入行情订阅表 \n",
						invest.InstrumentID);
				std::string str;
				str = invest.InstrumentID;

				temp.push_back(str);

			}
//				else
//				{
//					printf("-------> 主力合约已经存在   ！！！！ 合约[%s]无需加入订阅列表 \n",invest.InstrumentID);
//				}

		}
		vector<InvestPositionData>().swap(investinfo);

	}

	printf(
			"--->>> GetTradeInstrumentListsbyMain    持仓合约加入订阅合约的条数为---->[%ld] \n",
			temp.size());
	for (auto &item : list) {
//		printf("ppInstrumentID -->  %s   %s\n",item.InstrumentID,item.InstrumentID_next);
		std::string str;
		str = item.InstrumentID;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str);
		}

//		std::string str_next;
//		str_next=item.InstrumentID_next;
//
//		if(strcmp(item.ExchangeID,"INE")!=0)
//		{
//			temp.push_back(str_next);
//		}

	}
	printf(
			"--->>> GetTradeInstrumentListsbyMain    加入当前主力合约，最终订阅合约数---->[%ld] \n",
			temp.size());
	vector<User>().swap(userlist);
	vector<MainContractData>().swap(list);

}

void MysqlMaincontract::GetAllInstrumentLists(vector<std::string> &temp) {

	printf("--->>> GetAllInstrumentLists   temp---->[%ld] \n", temp.size());

	vector<MainContractData> list;
	this->Find_AllListorderbySed(list);

	printf(
			"--->>> GetAllInstrumentLists    Find_AllListorderbySed -->主力合约数---->[%ld] \n",
			list.size());

	for (auto &item : list) {

//		printf("ppInstrumentID -->  %s   %s\n",item.InstrumentID,item.InstrumentID_next);
		std::string str;
		str = item.InstrumentID;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str);
		}

		std::string str_next;
		str_next = item.InstrumentID_next;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str_next);
		}

	}
	printf("--->>> GetAllInstrumentLists    加入当前主力合约，最终订阅合约数---->[%ld] \n",
			temp.size());

	vector<MainContractData>().swap(list);

}

void MysqlMaincontract::GetNextInstrumentLists(vector<std::string> &temp) {

	printf("--->>> GetNextInstrumentLists   temp---->[%ld] \n", temp.size());

	vector<MainContractData> list;
	this->Find_AllListorderbySed(list);

	printf(
			"--->>> GetNextInstrumentLists    Find_AllListorderbySed -->主力合约数---->[%ld] \n",
			list.size());

	for (auto &item : list) {
//		printf("ppInstrumentID -->  %s   %s\n",item.InstrumentID,item.InstrumentID_next);
		std::string str;
		str = item.InstrumentID_next;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str);
		}

	}
	printf("--->>> GetNextInstrumentLists    加入当前主力合约，最终订阅合约数---->[%ld] \n",
			temp.size());

	vector<MainContractData>().swap(list);

}
void MysqlMaincontract::GetTradeInstrumentListsbyStrategy(
		vector<std::string> &temp, const char * strategy) {

	printf("--->>> GetTradeInstrumentLists   temp---->[%ld] \n", temp.size());

	vector<MainContractData> list;
	this->Find_AllListorderbySed(list);

//	printf ("--->>> GetTradeInstrumentLists    主力合约数---->[%ld] \n",list.size());
	MysqlUser mysqluser;
	User userinfo = mysqluser.Find_ActiveDatabyStrategyName(strategy);
	MysqlInvestPosition mysqlinvest(userinfo.UserID);

	vector<InvestPositionData> investinfo;
	mysqlinvest.Find_AllLists(investinfo);
//	printf ("--->>> GetTradeInstrumentLists    持仓合约数---->[%ld] \n",investinfo.size());
	for (auto &ip : investinfo) {
		bool exists = false;
		for (auto &item : list) {
			if (strcmp(item.InstrumentID, ip.InstrumentID) == 0) {
				exists = true;
				break;
			} else if (strcmp(item.InstrumentID_next, ip.InstrumentID) == 0) {
				exists = true;
				break;
			}
		}

		if (!exists) {
//				printf("-------> 不存在   %s \n",ip.InstrumentID);
			std::string str;
			str = ip.InstrumentID;

			temp.push_back(str);

		}

	}

	for (auto &item : list) {
//		printf("ppInstrumentID -->  %s   %s\n",item.InstrumentID,item.InstrumentID_next);
		std::string str;
		str = item.InstrumentID_trade;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str);
		}

//		std::string str_next;
//		str_next=item.InstrumentID_next;
//
//		if(strcmp(item.ExchangeID,"INE")!=0)
//		{
//			temp.push_back(str_next);
//		}

	}
	printf("--->>> GetTradeInstrumentLists    订阅合约数---->[%ld] \n", temp.size());
	vector<MainContractData>().swap(list);
	vector<InvestPositionData>().swap(investinfo);
}

void MysqlMaincontract::GetAllInstrumentListsbyStrategy(
		vector<std::string> &temp, const char * strategy) {

	printf("--->>> GetAllInstrumentListsbyStrategy   temp---->[%ld] \n",
			temp.size());

	vector<MainContractData> list;
	this->Find_AllListorderbySed(list);

//	printf ("--->>> GetAllInstrumentListsbyStrategy    主力合约数---->[%ld] \n",list.size());
	MysqlUser mysqluser;
	User userinfo = mysqluser.Find_ActiveDatabyStrategyName(strategy);
	MysqlInvestPosition mysqlinvest(userinfo.UserID);

	vector<InvestPositionData> investinfo;
	mysqlinvest.Find_AllLists(investinfo);
//	printf ("--->>> GetAllInstrumentListsbyStrategy    持仓合约数---->[%ld] \n",investinfo.size());
	for (auto &ip : investinfo) {
		bool exists = false;
		for (auto &item : list) {
			if (strcmp(item.InstrumentID, ip.InstrumentID) == 0) {
				exists = true;
				break;
			} else if (strcmp(item.InstrumentID_next, ip.InstrumentID) == 0) {
				exists = true;
				break;
			}
		}

		if (!exists) {
//				printf("-------> 不存在   %s \n",ip.InstrumentID);
			std::string str;
			str = ip.InstrumentID;

			temp.push_back(str);

		}

	}

	for (auto &item : list) {
//		printf("ppInstrumentID -->  %s   %s\n",item.InstrumentID,item.InstrumentID_next);
		std::string str;
		str = item.InstrumentID_trade;

		if (strcmp(item.ExchangeID, "INE") != 0
				&& strcmp(item.ExchangeID, "CFFEX") != 0) {
			temp.push_back(str);
		}

		std::string str_next;
		str_next = item.InstrumentID_next;

		if (strcmp(item.ExchangeID, "INE") != 0) {
			temp.push_back(str_next);
		}

	}
	printf("--->>> GetAllInstrumentListsbyStrategy    订阅合约数---->[%ld] \n",
			temp.size());
	vector<MainContractData>().swap(list);
	vector<InvestPositionData>().swap(investinfo);
}

int MysqlMaincontract::CountbyMainDayLine() {
	int count;
	char buf[2048];
	std::string str_sql;
	sprintf(buf, "SELECT count(DaylineCount) FROM %s WHERE DaylineCount < %d  ",
			this->getTablename().c_str(), 20);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			this->getFirst();
			count = getValueInt("count(DaylineCount)");
		}
	}

	this->Empty();

	return count;
}

int MysqlMaincontract::CountbyNextDayLine() {
	int count;
	char buf[2048];
	std::string str_sql;
	sprintf(buf,
			"SELECT count(DaylineCount_next) FROM %s WHERE DaylineCount_next < %d  ",
			this->getTablename().c_str(), 20);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			this->getFirst();
			count = getValueInt("count(DaylineCount_next)");
		}
	}

	this->Empty();

	return count;
}
void MysqlMaincontract::Find_DayLineWrongLists(vector<MainContractData> &temp,
		int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  WHERE  DaylineCount < %d ",
			this->getTablename().c_str(), n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.ErrorDaylineCount = getValueInt("ErrorDaylineCount");
			info.ErrorDaylineCount_next = getValueInt("ErrorDaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;
			bool is_black = strcmp(info.Section, "Black") == 0;
			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
			if (not_ine && not_cffex) {
				temp.push_back(info);
			}
			this->getNext();
		}
	}

	this->Empty();

}
void MysqlMaincontract::Find_NextDayLineWrongLists(
		vector<MainContractData> &temp, int n) {
	MainContractData info;

	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s  WHERE  DaylineCount_next < %d ",
			this->getTablename().c_str(), n);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.InstrumentName,
					getValueString("InstrumentName").c_str());
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());

			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.Section, getValueString("Section").c_str());
			strcpy(info.DayNight, getValueString("DayNight").c_str());
			info.VolumeMultiple = getValueInt("VolumeMultiple");

			info.PriceTick = getValueDouble("PriceTick");
			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");
			info.LongMarginRatio = getValueDouble("LongMarginRatio");
			info.ShortMarginRatio = getValueDouble("ShortMarginRatio");

			info.MaxOpenInterest = getValueInt("MaxOpenInterest");
			info.OpenInterest = getValueInt("OpenInterest");
			info.Volume = getValueInt("Volume");
			info.Sedimentary = getValueDouble("Sedimentary");
			info.LastPrice = getValueDouble("LastPrice");
			info.DaylineCount = getValueInt("DaylineCount");

			strcpy(info.InstrumentID_next,
					getValueString("InstrumentID_next").c_str());
			info.OpenInterest_next = getValueInt("OpenInterest_next");
			info.UpperLimitPrice_next = getValueDouble("UpperLimitPrice_next");
			info.LowerLimitPrice_next = getValueDouble("LowerLimitPrice_next");
			info.Sedimentary_next = getValueDouble("Sedimentary_next");
			info.DaylineCount_next = getValueInt("DaylineCount_next");

			info.ErrorDaylineCount = getValueInt("ErrorDaylineCount");
			info.ErrorDaylineCount_next = getValueInt("ErrorDaylineCount_next");

			info.LastPrice_next = getValueDouble("LastPrice_next");
			info.Volume_next = getValueInt("Volume_next");
			strcpy(info.InstrumentID_trade,
					getValueString("InstrumentID_trade").c_str());
			strcpy(info.LocalUpdateTime,
					getValueString("LocalUpdateTime").c_str());

			bool not_ine = strcmp(info.ExchangeID, "INE") != 0;
			bool not_precious = strcmp(info.Section, "Precious") != 0;
			bool not_metal = strcmp(info.Section, "Metal") != 0;
			bool not_cffex = strcmp(info.ExchangeID, "CFFEX") != 0;

			bool not_productid_fg = strcmp(info.ProductID, "FG") != 0;
			bool is_black = strcmp(info.Section, "Black") == 0;
			bool not_productid_fu = strcmp(info.ProductID, "fu") != 0;
//					 if( not_ine && not_precious && not_metal && not_cffex)
//					 if(is_black && not_productid_fg)
			if (not_ine && not_cffex) {
				temp.push_back(info);
			}
			this->getNext();
		}
	}

	this->Empty();

}
