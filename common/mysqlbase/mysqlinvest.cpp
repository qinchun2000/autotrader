#include "mysqlinvest.hpp"

MysqlInvestPosition::MysqlInvestPosition(const char* userid){
	printf("mysql begin ");
	std::string database = "account";
	std::string tablename = "invest_";
	tablename.append(userid);

	if (IsAliveConn()) {

	} else {
		if (!Connect()) {
			return;
		}
	}

	this->Set_Database(database);
	this->Set_Table(tablename);
	printf("mysql end");
}
void MysqlInvestPosition::CreateTable() {
	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];
		sprintf(buf, "CREATE TABLE IF NOT EXISTS %s ("
				"ID INT UNSIGNED AUTO_INCREMENT NOT NULL PRIMARY KEY, "
				"InstrumentID CHAR(40),"
				"YdPosition INT(11),"
				"Position INT(11),"
				"TodayPosition INT(11),"
				"PosiDirection INT(11),"
				"HedgeFlag INT(11),"
				"OpenVolume INT(11),"
				"CloseVolume INT(11),"
				"OpenAmount DOUBLE(15,3),"
				"CloseAmount DOUBLE(15,3),"
				"CloseProfit DOUBLE(15,3),"
				"PositionProfit DOUBLE(15,3),"
				"Commission DOUBLE(15,3),"
				"PreSettlementPrice DOUBLE(15,3),"
				"SettlementPrice DOUBLE(15,3),"
				"OpenPriceAverage DOUBLE(15,3),"
				"UseMargin DOUBLE(15,3),"
				"TradingDay CHAR(20),"
				"LocalUpdateTime DATETIME) default character set utf8",
				this->getTablename().c_str());

		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlInvestPosition::Insert(InvestPositionData data) {

	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (InstrumentID,YdPosition,Position,TodayPosition,CloseProfit,PositionProfit,"
					"PosiDirection,OpenPriceAverage,Commission,UseMargin,HedgeFlag ,PreSettlementPrice,SettlementPrice,"
					"OpenVolume,CloseVolume,OpenAmount,CloseAmount,TradingDay,LocalUpdateTime)"
					"VALUES ('%s','%d','%d','%d','%.3f','%.3f',"
					"'%d','%.3f','%.3f','%.3f','%d','%.3f','%.3f',"
					"'%d','%d','%.3f','%.3f','%s','%s')", tablename.c_str(),
			data.InstrumentID, data.YdPosition, data.Position,
			data.TodayPosition, data.CloseProfit, data.PositionProfit,
			data.PosiDirection, data.OpenPriceAverage, data.Commission,
			data.UseMargin, data.HedgeFlag, data.PreSettlementPrice,
			data.SettlementPrice, data.OpenVolume, data.CloseVolume,
			data.OpenAmount, data.CloseAmount, data.TradingDay, time.c_str());

	str_sql = buf;

	this->Execute(str_sql);

}

bool MysqlInvestPosition::Exist_DatabyInstrumentID(const char *pinstrumentid) {

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

int MysqlInvestPosition::Count_DetailList(const char* pinstrumentid,
		const char *tradingday) {

	int count = 0;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s  WHERE InstrumentID='%s' AND TradingDay='%s'  ",
			this->getTablename().c_str(), pinstrumentid, tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		count = this->getRowCount();

	}
	this->Empty();
	return count;
}

int MysqlInvestPosition::Count_OpenNumber(const char *tradingday) {

	int count = 0;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s  WHERE OpenVolume>'%d' AND TradingDay='%s'  ",
			this->getTablename().c_str(), 0, tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		count = this->getRowCount();

	}
	this->Empty();
	return count;
}
// 统计当天开仓和平仓的合约，但不包括撤单的合约
int MysqlInvestPosition::Count_OperatedNumber(const char *tradingday) {

	int count = 0;

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT * FROM %s  WHERE TradingDay='%s'  AND (OpenAmount>0 OR CloseAmount>0) ",
			this->getTablename().c_str(), tradingday);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		count = this->getRowCount();

	}
	this->Empty();
	return count;
}
void MysqlInvestPosition::Find_AllLists(vector<InvestPositionData> &temp) {

	InvestPositionData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.OpenPriceAverage = getValueDouble("OpenPriceAverage");
			info.UseMargin = getValueDouble("UseMargin");
			info.PositionProfit = getValueDouble("PositionProfit");
			info.CloseProfit = getValueDouble("CloseProfit");
			info.Position = getValueInt("Position");
			info.YdPosition = getValueInt("YdPosition");
			info.TodayPosition = getValueInt("TodayPosition");
			info.PosiDirection = getValueInt("PosiDirection");
			info.OpenVolume = getValueInt("OpenVolume");
			info.CloseVolume = getValueInt("CloseVolume");
			info.OpenAmount = getValueDouble("OpenAmount");
			info.CloseAmount = getValueDouble("CloseAmount");
			info.Commission = getValueDouble("Commission");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

HoldData MysqlInvestPosition::CollectHoldPosition(const char * pinstrumentid,
		const char *opendate, int direction) {
	HoldData data;
	data.Volume = 0;
	data.OpenPriceAverage = 0.0;
	data.Margin = 0;
//		printf("CollectHoldPosition begin\n");

	char buf[1024];
	std::string str_sql;

	sprintf(buf,
			"SELECT  * FROM %s WHERE InstrumentID ='%s' AND OpenDate ='%s' AND Direction='%d'",
			this->getTablename().c_str(), pinstrumentid, opendate, direction);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		double totalamount = 0;

		strcpy(data.InstrumentID, pinstrumentid);
		strcpy(data.OpenDate, opendate);
		data.Direction = direction;

		/* retrieve the row count in the result set */
		this->getFirst();
		for (int i = 0; i < this->getRowCount(); i++) {
			//			 cout << res -> getString("Time") << endl;

			strcpy(data.ExchangeID, getValueString("ExchangeID").c_str());
			strcpy(data.TradingDay, getValueString("TradingDay").c_str());

			data.Margin += getValueDouble("Margin");

			data.Volume += getValueInt("Position");
			totalamount += getValueDouble("OpenPrice")
					* (double) getValueInt("Volume");
			this->getNext();
		}

		if (data.Volume > 0) {
			data.OpenPriceAverage = totalamount / (double) data.Volume;
		}

	}

	this->Empty();

	return data;

}

int MysqlInvestPosition::GetPositionbyIns(const char * pinstrumentid) {
	int volume = 0;
//		printf("CollectHoldPosition begin\n");
	InvestPositionData info;
	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE InstrumentID ='%s' ",
			this->getTablename().c_str(), pinstrumentid);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		/* retrieve the row count in the result set */
		this->getFirst();
		for (int i = 0; i < this->getRowCount(); i++) {
			//			 cout << res -> getString("Time") << endl;

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.OpenPriceAverage = getValueDouble("OpenPriceAverage");
			info.UseMargin = getValueDouble("UseMargin");
			info.PositionProfit = getValueDouble("PositionProfit");
			info.CloseProfit = getValueDouble("CloseProfit");
			info.Position = getValueInt("Position");
			info.YdPosition = getValueInt("YdPosition");
			info.TodayPosition = getValueInt("TodayPosition");
			info.PosiDirection = getValueInt("PosiDirection");
			info.OpenVolume = getValueInt("OpenVolume");
			info.CloseVolume = getValueInt("CloseVolume");
			info.OpenAmount = getValueDouble("OpenAmount");
			info.CloseAmount = getValueDouble("CloseAmount");
			info.Commission = getValueDouble("Commission");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			volume += info.Position;

			this->getNext();
		}
	}

	this->Empty();
	return volume;

}

int MysqlInvestPosition::GetPositionbyInsDirection(const char * pinstrumentid,const int direction) {
	int volume = 0;
//		printf("CollectHoldPosition begin\n");
	InvestPositionData info;
	char buf[1024];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s WHERE InstrumentID ='%s' AND PosiDirection ='%d'  ",
			this->getTablename().c_str(), pinstrumentid,direction);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		/* retrieve the row count in the result set */
		this->getFirst();
		if(this->getRowCount()>0 ){
			//			 cout << res -> getString("Time") << endl;

			strcpy(info.InstrumentID, getValueString("InstrumentID").c_str());
			strcpy(info.TradingDay, getValueString("TradingDay").c_str());

			info.OpenPriceAverage = getValueDouble("OpenPriceAverage");
			info.UseMargin = getValueDouble("UseMargin");
			info.PositionProfit = getValueDouble("PositionProfit");
			info.CloseProfit = getValueDouble("CloseProfit");
			info.Position = getValueInt("Position");
			info.YdPosition = getValueInt("YdPosition");
			info.TodayPosition = getValueInt("TodayPosition");
			info.PosiDirection = getValueInt("PosiDirection");
			info.OpenVolume = getValueInt("OpenVolume");
			info.CloseVolume = getValueInt("CloseVolume");
			info.OpenAmount = getValueDouble("OpenAmount");
			info.CloseAmount = getValueDouble("CloseAmount");
			info.Commission = getValueDouble("Commission");
			info.PreSettlementPrice = getValueDouble("PreSettlementPrice");
			info.SettlementPrice = getValueDouble("SettlementPrice");

			volume = info.Position;

//			this->getNext();
		}
	}

	this->Empty();
	return volume;

}
