#include "mysqlasset.hpp"

MysqlAsset::MysqlAsset() {
	std::string database = "account";

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

}

MysqlAsset::MysqlAsset(const char* userid) {
	std::string database = "account";
	std::string tablename = "asset_";
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

void MysqlAsset::CreateTable() {

	if (!this->Exist_Table(this->getTablename().c_str())) {
		char buf[2048];

		sprintf(buf,
				"CREATE TABLE IF NOT EXISTS %s ("
						"TradingDay CHAR(10) NOT NULL  PRIMARY KEY,"
						"AccountID CHAR(20),"
						"PreBalance DOUBLE,"
						"Available DOUBLE,"   //可用资金
						"CurrMargin DOUBLE,"

						"Commission DOUBLE,"
						"StaticEquity DOUBLE,"//静态权益
						"DynamicEquity DOUBLE,"//动态权益
						"HighestProfit DOUBLE,"
						"LowestProfit DOUBLE,"
						"Withdraw DOUBLE,"
						"Deposit DOUBLE,"
						"TotalCaptial DOUBLE,"
						"Profit DOUBLE,"
						"HoldProfit DOUBLE,"
						"LocalUpdatetime CHAR(20)) default character set utf8 ",
				this->getTablename().c_str());
		std::string str_sql;
		str_sql = buf;

		this->Execute(str_sql);
	}

}

void MysqlAsset::Insert(AssetData data) {

	std::string tablename = this->getTablename();
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"INSERT INTO %s (TradingDay,AccountID,PreBalance,Available,CurrMargin,"
					"Commission,StaticEquity,DynamicEquity,HighestProfit,LowestProfit,"
					"Withdraw,Deposit,TotalCaptial,Profit,HoldProfit,LocalUpdatetime) "
					"VALUES ('%s', '%s','%.2f','%.2f','%.2f',"
					"'%.2f','%.2f','%.2f','%.2f','%.2f',"
					"'%.2f','%.2f','%.2f','%.2f','%.2f','%s') ",
			tablename.c_str(), data.TradingDay, data.AccountID, data.PreBalance,
			data.Available, data.CurrMargin, data.Commission, data.StaticEquity,
			data.DynamicEquity, data.HighestProfit, data.LowestProfit,
			data.Withdraw, data.Deposit, data.TotalCaptial, data.Profit,
			data.HoldProfit, time.c_str());
	str_sql = buf;

	this->Execute(str_sql);

}

void MysqlAsset::Update(AssetData data) {

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf,
			"UPDATE %s SET DynamicEquity='%.2f',HighestProfit='%.2f',LowestProfit='%.2f',Profit='%.2f',"
					"HoldProfit='%.2f',LocalUpdateTime='%s' "
					"WHERE TradingDay='%s' ", this->getTablename().c_str(),
			data.DynamicEquity, data.HighestProfit, data.LowestProfit,
			data.Profit, data.HoldProfit, time.c_str(), data.TradingDay);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);

}

void MysqlAsset::UpdateTotalCapital(double totalcapital,
		const char *tradingday) {
	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time = dateutil.ConvertCurrentTimetoSqlTime();

	sprintf(buf, "UPDATE %s SET TotalCaptial='%.2f' WHERE TradingDay='%s' ",
			this->getTablename().c_str(), totalcapital, tradingday);

	str_sql = buf;
	this->ExecuteUpdate(str_sql);

}

bool MysqlAsset::Exist_DatabyDate(const char *date) {

	bool ret = false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE TradingDay='%s' ",
			this->getTablename().c_str(), date);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() > 0) {
			ret = true;
		}

	}
	this->Empty();
	return ret;
}

AssetData MysqlAsset::Find_DatabyDate(const char *date) {

	AssetData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s WHERE TradingDay='%s'  ",
			this->getTablename().c_str(), date);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.AccountID, getValueString("AccountID").c_str());
			info.Available = getValueDouble("Available");
			info.PreBalance = getValueDouble("PreBalance");
			info.Withdraw = getValueDouble("Withdraw");
			info.Deposit = getValueDouble("Deposit");
			info.Commission = getValueDouble("Commission");
			info.CurrMargin = getValueDouble("CurrMargin");
			info.StaticEquity = getValueDouble("StaticEquity");
			info.HighestProfit = getValueDouble("HighestProfit");
			info.LowestProfit = getValueDouble("LowestProfit");
			info.Profit = getValueDouble("Profit");
			info.HoldProfit = getValueDouble("HoldProfit");
			info.DynamicEquity = getValueDouble("DynamicEquity");
			info.TotalCaptial = getValueDouble("TotalCaptial");

		}
	}
	this->Empty();
	return info;

}

AssetData MysqlAsset::Find_LastDayAsset() {

	AssetData info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s ORDER BY TradingDay DESC LIMIT 1  ",
			this->getTablename().c_str());
	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		if (this->getRowCount() == 1) {
			this->getFirst();

			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.AccountID, getValueString("AccountID").c_str());
			info.Available = getValueDouble("Available");
			info.PreBalance = getValueDouble("PreBalance");
			info.Withdraw = getValueDouble("Withdraw");
			info.Deposit = getValueDouble("Deposit");
			info.Commission = getValueDouble("Commission");
			info.CurrMargin = getValueDouble("CurrMargin");
			info.StaticEquity = getValueDouble("StaticEquity");
			info.HighestProfit = getValueDouble("HighestProfit");
			info.LowestProfit = getValueDouble("LowestProfit");
			info.Profit = getValueDouble("Profit");
			info.HoldProfit = getValueDouble("HoldProfit");
			info.DynamicEquity = getValueDouble("DynamicEquity");
			info.TotalCaptial = getValueDouble("TotalCaptial");

		}
	}
	this->Empty();

	return info;

}

void MysqlAsset::Find_AllList(vector<AssetData> &temp) {

	AssetData info;

	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT  * FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i = 0; i < this->getRowCount(); i++) {

			strcpy(info.TradingDay, getValueString("TradingDay").c_str());
			strcpy(info.AccountID, getValueString("AccountID").c_str());
			info.Available = getValueDouble("Available");
			info.PreBalance = getValueDouble("PreBalance");
			info.Withdraw = getValueDouble("Withdraw");
			info.Deposit = getValueDouble("Deposit");
			info.Commission = getValueDouble("Commission");
			info.CurrMargin = getValueDouble("CurrMargin");
			info.StaticEquity = getValueDouble("StaticEquity");
			info.HighestProfit = getValueDouble("HighestProfit");
			info.LowestProfit = getValueDouble("LowestProfit");
			info.Profit = getValueDouble("Profit");
			info.HoldProfit = getValueDouble("HoldProfit");
			info.DynamicEquity = getValueDouble("DynamicEquity");
			info.TotalCaptial = getValueDouble("TotalCaptial");

			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

