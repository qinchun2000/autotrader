#ifndef _MYSQLBASE_HPP
#define _MYSQLBASE_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <string>
#include <memory>

#include <stdlib.h>

#include <sstream>
#include <stdexcept>

#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/driver.h"
#include "cppconn/statement.h"
#include "cppconn/prepared_statement.h"
#include "cppconn/metadata.h"
#include "cppconn/exception.h"

#include "codeutil.hpp"
#include "dateutil.hpp"
#include <boost/regex.hpp>
#include "mysqldata.hpp"

using namespace std;
using namespace sql;

class MysqlBase {

public:
	MysqlBase();
	virtual ~MysqlBase();

	void Set_Table(const string& tablename);

	bool Exist_Table();
	bool Exist_Table(const char * tablename);

	bool Exist_Database(const char *database);
	void Create_DataBase(const char *database);
	void Set_Database(const string& database);
	void DeleteAll();
	void DeleteAllbyTableName(const char* tablename);
	void DropbyTableName(const char* tablename);
	void DropTable();

	int Count_AllLists();
	int Count_AllListsbyTableName(const char *tablename);
	int Count_ColumnsbyTableName(const char *tablename);
	int Count_ColumnsbyTableName();

	bool Connect();
	void Ping();
	bool IsAliveConn();
	bool IsCloseConn();
	void Execute(const SQLString& sql);
	void Empty();
	bool ExecuteQuery(const SQLString& sql);
	void ExecuteUpdate(const SQLString& sql);
	bool ExecuteProcedure(const SQLString& sql);
	void RollBack(Savepoint * savepoint = NULL);
	void Commit();
	bool ReConnect();
	bool ConnectCheck();

//	int getColumnCount();
	bool getNext();
	bool getFirst();
	int getRowCount();
//	string getColumnName(int index);
	bool isValueNull(int columnindex);
	bool isValueNull(const string& columnname);
	int getValueInt(const string& columnname);
	double getValueDouble(const string& columnname);
	string getValueString(const string& columnname);
	string getValueString(int columindex);

	std::string getDatabase();
	std::string getTablename();
	std::string getDBHost();
	int getCommitFlag();

private:
	//sql::Driver *driver = nullptr;
	sql::mysql::MySQL_Driver *driver=nullptr;
	sql::Connection *conn = nullptr;
	sql::Statement *state = nullptr;
	ResultSet *res = nullptr;
//	DatabaseMetaData  *dbcon_meta=nullptr;
//	ResultSetMetaData *res_meta=nullptr;
	sql::PreparedStatement *prep_stmt = nullptr;
	Savepoint *savept = nullptr;

	int commitflag = 1;
	std::string _DBHOST; // "tcp://127.0.0.1:3306"
	std::string _USER; // "root"
	std::string _PASSWORD; // "root"

	std::string _DATABASE; // "test"
	std::string _TABLENAME; // "test"

};

#endif

