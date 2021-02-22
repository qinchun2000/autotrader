#include "mysqlposdetail.hpp"

MysqlPosDetail::MysqlPosDetail(const char * userid) {
	std::string database = "account";
	std::string tablename = "posdetail_";
	tablename.append(userid);
	if (IsAliveConn()) {

	} else {
		if (!Connect()) {
			return;
		}

	}

	this->Set_Database(database);
	this->Set_Table(tablename);
}

void MysqlPosDetail::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
				"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
				"InstrumentID CHAR(40),"
				"ExchangeID CHAR(10),"
				"Direction INT(11),"
				"Volume INT(11),"
				"TradeID CHAR(21),"
//				"FrontID INT(11),"
//				"SessionID INT(11),"
//				"OrderSysID CHAR(21),"
//				"ErrorId INT(11),"
				"OpenPrice DOUBLE,"
				"Margin DOUBLE,"
				"TradingDay CHAR(20),"
				"OpenDate CHAR(20),"
				"CloseVolume INT(11),"
				"CloseAmount DOUBLE,"
				"LocalUpdateTime DATETIME) ENGINE=MEMORY  default character set utf8  ",
				this->getTablename().c_str());

		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}
void MysqlPosDetail::Insert(PositionDetailData data) {
//	printf("MysqlPosDetail Insert begin\n");
	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (InstrumentID,Margin,Direction,"
					"ExchangeID,TradeID,OpenPrice, "
					"Volume,TradingDay,OpenDate,CloseVolume,CloseAmount,LocalUpdateTime)"
					"VALUES ('%s','%.3f','%d',"
					"'%s','%s','%.3f',"
					"'%d','%s','%s','%d','%.3f','%s')", tablename.c_str(),
			data.InstrumentID, data.Margin, data.Direction, data.ExchangeID,
			data.TradeID, data.OpenPrice, data.Volume, data.TradingDay,
			data.OpenDate, data.CloseVolume, data.CloseAmount, time.c_str());

	str_sql = buf;

	this->Execute(str_sql);

//	printf("MysqlPosDetail Insert end \n");
}

bool MysqlPosDetail::Exist_DatabyInstrumentid(const char* pinstrumentid) {
//	printf("Exist_PositionbyInstrumentid begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE  InstrumentID='%s'  ",
			this->getTablename().c_str(), pinstrumentid);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			ret = true;
		}
	}

	/* retrieve the row count in the result set */

	this->Empty();
	return ret;
}

bool MysqlPosDetail::Exist_PositionbyInstrumentid(const char* pinstrumentid,
		const char *tradeid, const char *opendate) {
//	printf("Exist_PositionbyInstrumentid begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE  InstrumentID='%s' AND TradeID='%s'  AND OpenDate='%s' ",
			this->getTablename().c_str(), pinstrumentid, tradeid, opendate);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			ret = true;
		}

	}

	/* retrieve the row count in the result set */

	this->Empty();
	return ret;
}

bool MysqlPosDetail::Exist_HoldbyInstrumentid(HoldData holddata) {
//	printf("Exist_PositionbyInstrumentid begin\n");
	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE  InstrumentID='%s' AND OpenDate='%s'  AND Direction='%d' AND Volume>0  ",
			this->getTablename().c_str(), holddata.InstrumentID,
			holddata.OpenDate, holddata.Direction);
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			ret = true;
		}

	}

	/* retrieve the row count in the result set */

	this->Empty();
	return ret;
}

int MysqlPosDetail::Count_DetailList(const char* pinstrumentid,
		const char *tradingday) {

	int count = 0;

	char buf[500];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s WHERE InstrumentID='%s' AND TradingDay='%s'  ",
			this->getTablename().c_str(), pinstrumentid, tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		count = this->getRowCount();

	}
	this->Empty();
	return count;
}

void MysqlPosDetail::Find_AllList(vector<PositionDetailData> &temp) {
	PositionDetailData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(data.OpenDate, getValueString("OpenDate").c_str());
			strcpy(data.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(data.TradeID, getValueString("TradeID").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.OpenPrice = getValueDouble("OpenPrice");
			data.Margin = getValueDouble("Margin");
			data.Volume = getValueInt("Volume");
			data.Direction = getValueInt("Direction");
			data.CloseVolume = getValueInt("Volume");
			data.CloseAmount = getValueDouble("CloseAmount");

			temp.push_back(data);
			this->getNext();
		}
	}
	this->Empty();

}

void MysqlPosDetail::Find_InstrumentIDLists(
		vector<string> &lists_instrumentid) {
	PositionDetailData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  distinct   InstrumentID FROM %s ",
			this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(data.OpenDate, getValueString("OpenDate").c_str());
			strcpy(data.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(data.TradeID, getValueString("TradeID").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.OpenPrice = getValueDouble("OpenPrice");
			data.Margin = getValueDouble("Margin");
			data.Volume = getValueInt("Volume");
			data.Direction = getValueInt("Direction");
			data.CloseVolume = getValueInt("Volume");
			data.CloseAmount = getValueDouble("CloseAmount");

			lists_instrumentid.push_back(data.InstrumentID);
			this->getNext();
		}
	}
	this->Empty();

}
void MysqlPosDetail::Find_CloseLists(vector<PositionDetailData> &temp) {
	PositionDetailData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s Where CloseVolume>0 ",
			this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(data.OpenDate, getValueString("OpenDate").c_str());
			strcpy(data.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(data.TradeID, getValueString("TradeID").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.OpenPrice = getValueDouble("OpenPrice");
			data.Margin = getValueDouble("Margin");
			data.Volume = getValueInt("Volume");
			data.Direction = getValueInt("Direction");
			data.CloseVolume = getValueInt("CloseVolume");
			data.CloseAmount = getValueDouble("CloseAmount");

			temp.push_back(data);
			this->getNext();
		}
	}
	this->Empty();

}

HoldData MysqlPosDetail::CollectHoldPositionbyOpenDate(
		const char * pinstrumentid, const char *opendate, int direction) {
	HoldData data;
	data.Volume = 0;
	data.OpenPriceAverage = 0.0;
	data.Margin = 0;
//		printf("CollectHoldPosition begin\n");

	char buf[500];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE InstrumentID ='%s' AND OpenDate ='%s' AND Direction='%d'",
			this->getTablename().c_str(), pinstrumentid, opendate, direction);

	str_sql = buf;

	double totalamount = 0;

	strcpy(data.InstrumentID, pinstrumentid);
	strcpy(data.OpenDate, opendate);
	data.Direction = direction;

	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.Margin += getValueDouble("Margin");
			if (getValueInt("Volume") > 0) {
				data.Volume += getValueInt("Volume");
				totalamount += getValueDouble("OpenPrice")
						* (double) getValueInt("Volume");
			}

			this->getNext();
		}
	}

	if (data.Volume > 0) {
		data.OpenPriceAverage = totalamount / (double) data.Volume;
	}

	this->Empty();

	return data;

}

HoldData MysqlPosDetail::CollectHoldPosition(const char * pinstrumentid,
		int direction) {
	HoldData data;
	data.Volume = 0;
	data.OpenPriceAverage = 0.0;
	data.Margin = 0;
//		printf("CollectHoldPosition begin\n");

	char buf[500];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE InstrumentID ='%s' AND  AND Direction='%d'",
			this->getTablename().c_str(), pinstrumentid, direction);

	str_sql = buf;

	double totalamount = 0;

	strcpy(data.InstrumentID, pinstrumentid);

	data.Direction = direction;

	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(data.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());
			data.Margin += getValueDouble("Margin");
			if (getValueInt("Volume") > 0) {
				data.Volume += getValueInt("Volume");
				totalamount += getValueDouble("OpenPrice")
						* (double) getValueInt("Volume");
			}

			this->getNext();
		}
	}

	if (data.Volume > 0) {
		data.OpenPriceAverage = totalamount / (double) data.Volume;
	}

	this->Empty();

	return data;

}
double MysqlPosDetail::CollectOpenAveragePrice(const char * pinstrumentid) {

	double OpenPriceAverage = 0.0;

//		printf("CollectOpenAveragePrice begin\n");
	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE InstrumentID ='%s' ",
			this->getTablename().c_str(), pinstrumentid);

	str_sql = buf;

	double totalamount = 0;
	int totalvolume = 0;

	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		for (int i = 0; i < this->getRowCount(); i++) {
			//			 cout << res -> getString("Time") << endl;

			int volume = getValueInt("Volume");
			int close_volume = getValueInt("CloseVolume");
			double openprice = getValueDouble("OpenPrice");
			if (volume > 0) {
				totalvolume += volume;
				totalamount += openprice * (double) volume;
			} else if (close_volume > 0) {
				totalvolume += close_volume;
				totalamount += openprice * (double) close_volume;
			}

			this->getNext();
		}
	}

	if (totalvolume > 0) {
		OpenPriceAverage = totalamount / (double) totalvolume;
	}

	this->Empty();

	return OpenPriceAverage;

}
