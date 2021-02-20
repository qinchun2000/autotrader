#include "mysqldepthmarket.hpp"

MysqlDepthMarket::MysqlDepthMarket() {
//	printf("MysqlDepthMarket()  构造函数 begin!!! \n");
	std::string database = "baseinfo";
	std::string tablename = "depthmarket";

	if (IsAliveConn()) {

	} else {
		if (!Connect()) {

			return;
		}
	}

	this->Set_Database(database);
	this->Set_Table(tablename);
//	printf("MysqlDepthMarket()  构造函数 end!!! \n");
}

MysqlDepthMarket::MysqlDepthMarket(const char* tablename) {
//	printf("MysqlDepthMarket(const char* tablename)  构造函数 begin!!!  \n");
	std::string database = "baseinfo";
//	std::string tablename="depthmarket";

	if (IsAliveConn()) {

	} else {
		if (!Connect()) {

			return;
		}
	}

	this->Set_Database(database);
	this->Set_Table(tablename);
//	printf("MysqlDepthMarket(const char* tablename)  构造函数 end!!!  \n");
}
void MysqlDepthMarket::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];
		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
				"InstrumentID CHAR(30) NOT NULL  PRIMARY KEY,"
				"ExchangeID CHAR(10),"
				"ProductID CHAR(10),"
				"OpenInterest INT(11),"
				"Volume INT(11),"
				"Turnover DOUBLE,"
				"LastPrice DOUBLE,"
				"SettlementPrice DOUBLE,"
				"PreSettlementPrice DOUBLE,"
				"OpenPrice DOUBLE,"
				"HighestPrice DOUBLE,"
				"LowestPrice DOUBLE,"
				"ClosePrice DOUBLE,"
				"PreClosePrice DOUBLE,"
				"LowerLimitPrice DOUBLE,"
				"UpperLimitPrice DOUBLE,"
				"ActionDay CHAR(20),"
				"UpdateTime CHAR(20),"
				"TradingDay CHAR(20),"
				"LocalUpdateTime DATETIME) default character set utf8 ",
				this->getTablename().c_str());
		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlDepthMarket::Insert(DepthMarketData data) {

	std::string tablename = this->getTablename();
	char buf[5000];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (InstrumentID,ExchangeID,ProductID,OpenInterest,Volume,"
					"Turnover,LastPrice,SettlementPrice,PreSettlementPrice,LowerLimitPrice,"
					"OpenPrice,HighestPrice,LowestPrice,ClosePrice,PreClosePrice,"
					"UpperLimitPrice,ActionDay,UpdateTime,TradingDay,LocalUpdateTime)"
					"VALUES ('%s', '%s','%s','%d','%d',"
					"'%.3f','%.3f','%.3f','%.3f','%.3f',"
					"'%.3f','%.3f','%.3f','%.3f','%.3f',"
					"'%.3f','%s','%s','%s','%s')", tablename.c_str(),
			data.InstrumentID, data.ExchangeID, data.ProductID,
			data.OpenInterest, data.Volume, data.Turnover, data.LastPrice,
			data.SettlementPrice, data.PreSettlementPrice, data.LowerLimitPrice,
			data.OpenPrice, data.HighestPrice, data.LowestPrice,
			data.ClosePrice, data.PreClosePrice, data.UpperLimitPrice,
			data.ActionDay, data.UpdateTime, data.TradingDay, time.c_str());

	str_sql = buf;

	this->Execute(str_sql);

}

std::string MysqlDepthMarket::GetLastLocalUpdateTime() {

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

std::string MysqlDepthMarket::GetOldestTradingDay() {

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

bool MysqlDepthMarket::Exist_DatabyInstrumentID(const char *pinstrumentid) {

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

DepthMarketData MysqlDepthMarket::Find_DatabyInstrumentID(
		const char *pinstrumentid) {

	DepthMarketData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE InstrumentID='%s' ",
			this->getTablename().c_str(), pinstrumentid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");
		}
	}
	this->Empty();
	return info;

}

DepthMarketData MysqlDepthMarket::Find_DatabyMaxOPI(const char *productid) {

	DepthMarketData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE ProductID='%s' ORDER BY OpenInterest DESC LIMIT 1 ",
			this->getTablename().c_str(), productid);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");
		}
	}
	this->Empty();
	return info;

}

DepthMarketData MysqlDepthMarket::Find_DatabyMaxVolume(const char *productid) {

	DepthMarketData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE ProductID='%s' ORDER BY Volume DESC LIMIT 1 ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");
		}
	}
	this->Empty();
	return info;

}

void MysqlDepthMarket::Find_AllList(vector<DepthMarketData> &temp) {

	DepthMarketData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlDepthMarket::Find_ListsbyProductID(vector<DepthMarketData> &temp,
		const char * productid) {
	DepthMarketData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE ProductID='%s' ORDER BY OpenInterest DESC ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}
void MysqlDepthMarket::Find_ListsbyProductIDVolume(
		vector<DepthMarketData> &temp, const char * productid) {

	char buf[2048];
	std::string str_sql;
	sprintf(buf,
			"SELECT  * FROM %s WHERE ProductID='%s' AND Volume>0 ORDER BY Volume DESC ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {
			DepthMarketData info;
			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");
			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}
void MysqlDepthMarket::Find_MainNextListsbyProductID(
		vector<DepthMarketData> &temp, const char * productid) {
	DepthMarketData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE ProductID='%s' ORDER BY OpenInterest DESC limit 2 ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.ProductID, getValueString("ProductID").c_str());
			strcpy(info.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(info.ActionDay, getValueString("ActionDay").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.UpdateTime, getValueString("UpdateTime").c_str());

			info.OpenInterest = getValueInt("OpenInterest");
			info.LastPrice = getValueDouble("LastPrice");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			info.OpenPrice = getValueDouble("OpenPrice");
			info.HighestPrice = getValueDouble("HighestPrice");
			info.LowestPrice = getValueDouble("LowestPrice");
			info.ClosePrice = getValueDouble("ClosePrice");
			info.PreClosePrice = getValueDouble("PreClosePrice");

			info.UpperLimitPrice = getValueDouble("UpperLimitPrice");
			info.LowerLimitPrice = getValueDouble("LowerLimitPrice");

			info.Turnover = getValueDouble("Turnover");
			info.Volume = getValueInt("Volume");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlDepthMarket::Find_MainNextItembyProductID(DepthMarketData &a,
		DepthMarketData &b, const char * productid) {
	DepthMarketData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE ProductID='%s' ORDER BY OpenInterest DESC limit 2 ",
			this->getTablename().c_str(), productid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			if (i == 0) {
				strcpy(a.InstrumentID, getValueString("InstrumentID").c_str());
				strcpy(a.ProductID, getValueString("ProductID").c_str());
				strcpy(a.ExchangeID, getValueString("ExchangeID").c_str());
				strcpy(a.ActionDay, getValueString("ActionDay").c_str());
				strcpy(a.TradingDay, getValueString("TradingDay").c_str());
				strcpy(a.UpdateTime, getValueString("UpdateTime").c_str());

				a.OpenInterest = getValueInt("OpenInterest");
				a.LastPrice = getValueDouble("LastPrice");
				a.PreSettlementPrice = getValueDouble("PreSettlementPrice");
				a.SettlementPrice = getValueDouble("SettlementPrice");

				a.OpenPrice = getValueDouble("OpenPrice");
				a.HighestPrice = getValueDouble("HighestPrice");
				a.LowestPrice = getValueDouble("LowestPrice");
				a.ClosePrice = getValueDouble("ClosePrice");
				a.PreClosePrice = getValueDouble("PreClosePrice");

				a.UpperLimitPrice = getValueDouble("UpperLimitPrice");
				a.LowerLimitPrice = getValueDouble("LowerLimitPrice");

				a.Turnover = getValueDouble("Turnover");
				a.Volume = getValueInt("Volume");
			} else if (i == 1) {
				strcpy(b.InstrumentID, getValueString("InstrumentID").c_str());
				strcpy(b.ProductID, getValueString("ProductID").c_str());
				strcpy(b.ExchangeID, getValueString("ExchangeID").c_str());
				strcpy(b.ActionDay, getValueString("ActionDay").c_str());
				strcpy(b.TradingDay, getValueString("TradingDay").c_str());
				strcpy(b.UpdateTime, getValueString("UpdateTime").c_str());

				b.OpenInterest = getValueInt("OpenInterest");
				b.LastPrice = getValueDouble("LastPrice");
				b.PreSettlementPrice = getValueDouble("PreSettlementPrice");
				b.SettlementPrice = getValueDouble("SettlementPrice");

				b.OpenPrice = getValueDouble("OpenPrice");
				b.HighestPrice = getValueDouble("HighestPrice");
				b.LowestPrice = getValueDouble("LowestPrice");
				b.ClosePrice = getValueDouble("ClosePrice");
				b.PreClosePrice = getValueDouble("PreClosePrice");

				b.UpperLimitPrice = getValueDouble("UpperLimitPrice");
				b.LowerLimitPrice = getValueDouble("LowerLimitPrice");

				b.Turnover = getValueDouble("Turnover");
				b.Volume = getValueInt("Volume");
			}

			this->getNext();
		}
	}
	this->Empty();

}
