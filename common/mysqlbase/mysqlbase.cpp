#include "mysqlbase.hpp"

MysqlBase::MysqlBase() {
//	printf("MysqlBase connect begin \n");
	this->_DBHOST = "tcp://127.0.0.1:3306";
	this->_USER = "root";
	this->_PASSWORD = "111111";

	if (!Connect()) {
		printf("ERROR: MysqlBase Create database [%s] error !!! \n",
				_DATABASE.c_str());
		return;
	}

	//turn off the autocommit

//	conn->setAutoCommit(0);
//	this->commitflag=0;
//	conn->setSchema(_DATABASE);

//	printf("MysqlBase connet to database [%s]OK\n",_DATABASE.c_str());

}

MysqlBase::~MysqlBase() {

//	printf("MysqlBase    - delete \n");

	if (res) {
		delete res;
		res = nullptr;
	}
	if (savept) {
		delete savept;
		savept = nullptr;
	}
// if(res_meta)	    {
//		delete res_meta;
//		res_meta = NULL;
//	}
	if (state) {
		delete state;
		state = nullptr;
	}
	if (prep_stmt) {
		delete prep_stmt;
		prep_stmt = nullptr;
	}
	if (conn) {
//	    	printf("MysqlBase    - conn close \n");
		conn->close();
		delete conn;
		conn = nullptr;
	}

}

bool MysqlBase::Exist_Database(const char *database) {
//	printf("Exist_Database begin\n");
	bool ret = false;
	if (!state) {
		state = conn->createStatement();
	}
	char buf[500];
	std::string str_sql;

	sprintf(buf, "SHOW DATABASES LIKE '%s' ", database);

	str_sql = buf;
	res = state->executeQuery(str_sql);

	/* retrieve the row count in the result set */

	if (res->rowsCount() > 0) {
		ret = true;
	}

	this->Empty();

//	printf("Exist_Database End\n");
	return ret;
}

void MysqlBase::Create_DataBase(const char *database) {

	char buf[500];
	sprintf(buf, "CREATE DATABASE IF NOT EXISTS %s ", database);
	std::string str_sql;
	str_sql = buf;
	this->Execute(str_sql);

}

void MysqlBase::Set_Database(const string& database) {
	if (ConnectCheck()) {
		if (!state) {
			state = conn->createStatement();
		}

		if (!this->Exist_Database(database.c_str())) {
			this->Create_DataBase(database.c_str());
		}

		this->_DATABASE = database;
		conn->setSchema(_DATABASE);
	}

}

void MysqlBase::Set_Table(const string& tablename) {

	this->_TABLENAME = tablename;

}

bool MysqlBase::Exist_Table() {
//	printf("Exist_DateTime begin\n");
	bool ret = false;
	if (!state) {
		state = conn->createStatement();
	}
	char buf[500];
	std::string str_sql;

	sprintf(buf, "SHOW TABLES LIKE '%s' ", this->getTablename().c_str());

	str_sql = buf;
	res = state->executeQuery(str_sql);

	/* retrieve the row count in the result set */

	if (res->rowsCount() > 0) {
		ret = true;
	}

	this->Empty();
	return ret;
}

bool MysqlBase::Exist_Table(const char * tablename) {
//	printf("Exist_DateTime begin\n");
	bool ret = false;
	if (!state) {
		state = conn->createStatement();
	}
	char buf[500];
	std::string str_sql;

	sprintf(buf, "SHOW TABLES LIKE '%s' ", tablename);

	str_sql = buf;
	res = state->executeQuery(str_sql);

	/* retrieve the row count in the result set */

	if (res->rowsCount() > 0) {
		ret = true;
	}

	this->Empty();
	return ret;
}
void MysqlBase::DeleteAll() {
	//	printf("DeleteAll begin\n");
	if (!state) {
		state = conn->createStatement();
	}
	char buf[200];
	std::string str_sql;

	sprintf(buf, "DELETE FROM %s ", this->getTablename().c_str());

	str_sql = buf;
	state->execute(str_sql);

	this->Commit();
}

void MysqlBase::DeleteAllbyTableName(const char* tablename) {
	//	printf("DeleteAll begin\n");
	if (!state) {
		state = conn->createStatement();
	}
	char buf[200];
	std::string str_sql;

	sprintf(buf, "DELETE FROM %s", tablename);

	str_sql = buf;
	state->execute(str_sql);
	this->Commit();
}

void MysqlBase::DropTable() {
	//	printf("DeleteAll begin\n");
	if (!state) {
		state = conn->createStatement();
	}
	char buf[200];
	std::string str_sql;

	sprintf(buf, "DROP TABLE %s", this->getTablename().c_str());

	str_sql = buf;
	state->execute(str_sql);
	this->Commit();
}

void MysqlBase::DropbyTableName(const char* tablename) {
	//	printf("DeleteAll begin\n");
	if (!state) {
		state = conn->createStatement();
	}
	char buf[200];
	std::string str_sql;

	sprintf(buf, "DROP TABLE %s", tablename);

	str_sql = buf;
	state->execute(str_sql);
	this->Commit();
}

int MysqlBase::Count_AllLists() {
//	printf("Count_AllLists begin\n");

	int count = 0;

	if (!state) {
		state = conn->createStatement();
	}
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT COUNT(*) FROM  %s ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			this->getFirst();
//				count = res -> rowsCount();
			count = getValueInt("COUNT(*)");
		}
	}

	/* retrieve the row count in the result set */
//	count =res -> rowsCount();
	this->Empty();
	return count;
}

int MysqlBase::Count_AllListsbyTableName(const char *tablename) {

//		printf("Count_AllLists begin\n");

	int count = 0;

	if (!state) {
		state = conn->createStatement();
	}
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT COUNT(*) FROM  %s ", tablename);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			this->getFirst();
			//				count = res -> rowsCount();
			count = getValueInt("COUNT(*)");
		}
	}

	/* retrieve the row count in the result set */
	//	count =res -> rowsCount();
	this->Empty();
	return count;

}
int MysqlBase::Count_ColumnsbyTableName(const char *tablename) {
	int count = 0;

	if (!state) {
		state = conn->createStatement();
	}
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT COUNT(*) FROM information_schema. COLUMNS	"
			"WHERE table_schema = '%s'	AND table_name = '%s'",
			this->getDatabase().c_str(), tablename);

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			this->getFirst();
//				count = res -> rowsCount();
			count = getValueInt("COUNT(*)");
		}
	}

	/* retrieve the row count in the result set */
//	count =res -> rowsCount();
	this->Empty();
	return count;
}
int MysqlBase::Count_ColumnsbyTableName() {
	int count = 0;

	if (!state) {
		state = conn->createStatement();
	}
	char buf[2048];
	std::string str_sql;

	sprintf(buf, "SELECT COUNT(*) FROM information_schema. COLUMNS	"
			"WHERE table_schema = '%s'	AND table_name = '%s'",
			this->getDatabase().c_str(), this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {
		if (this->getRowCount() > 0) {
			this->getFirst();
//				count = res -> rowsCount();
			count = getValueInt("COUNT(*)");
		}
	}

	/* retrieve the row count in the result set */
//	count =res -> rowsCount();
	this->Empty();
	return count;
}

bool MysqlBase::Connect() {

	//cout << "MysqlBase Connect get driver begin \n";
	try {
		//driver = sql::mysql::get_driver_instance();
    	driver = sql::mysql::get_mysql_driver_instance();
		if (driver == NULL) {
			cout << "MysqlBase::Connect get_mysql_driver_instance err!!!\n";
			return false;
		} else {

	//	    cout << "get driver ok"<< endl;
	//	    cout << "host:"<<_DBHOST<< endl;
	//	    cout << "user:"<<_USER << endl;
	//	    cout << "password:"<<_PASSWORD << endl;

			conn = driver->connect(_DBHOST, _USER, _PASSWORD);
	//		conn = driver->connect("tcp://localhost:3306", "root", "111111");
	//	    cout << "mysql connect ok\n";

			state = conn->createStatement();
	//	    cout << "createStatement ok...\n";

			if (!conn || !state) {

				if (!conn) {
					cout << "MysqlBase::driver->connect err!!!\n";
				}

				if (!state) {
					cout << "MysqlBase::conn->createStatement err!!!\n";
				}

				return false;
			} else {
				return true;
			}
		}

//    }
//    catch (sql::SQLException &e)
//    {
//        cout << "MysqlBase::connect 出现异常\n" << endl;
//    }

	} catch (SQLException &e) {
//			cout << ", SQL: " << sql.c_str() << " ;" << endl;
//
//			cout << "ERROR: SQLException in " << __FILE__;
//			cout << " (" << __func__<< ") on line " << __LINE__ << endl;
//
//			cout << "ERROR: " << e.what();
//			cout << " (MySQL error code: " << e.getErrorCode();
//			cout << ", SQLState: " << e.getSQLState() << ")" << endl;
//	//		RollBack(savept);
//			return false;

		/*
		 The JDBC API throws three different exceptions:
		 - sql::MethodNotImplementedException (derived from sql::SQLException)
		 - sql::InvalidArgumentException (derived from sql::SQLException)
		 - sql::SQLException (derived from std::runtime_error)
		 */
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << "EXAMPLE_FUNCTION" << ") on line " << __LINE__ << endl;
		/* Use what() (derived from std::runtime_error) to fetch the error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return EXIT_FAILURE;

	}

	// cout << "get driver ok1111\n";

	return true;
}
void MysqlBase::Ping() {
	char buf[500];
	std::string str_sql;

	sprintf(buf, "SELECT * FROM %s   ", this->getTablename().c_str());

	str_sql = buf;
	if (this->ExecuteQuery(str_sql)) {

		int count = this->getRowCount();

	}
	this->Empty();

}
bool MysqlBase::IsAliveConn() {
	return conn->isValid() ? true : false;
}

bool MysqlBase::IsCloseConn() {
	return conn->isClosed() ? true : false;
}

void MysqlBase::Execute(const SQLString& sql) {

	try {
		if (ConnectCheck()) {
			if (!state) {
				state = conn->createStatement();
				//		        printf("createStatement 重新链接\n");
			}

			state->execute(sql);
			Commit();
		} else {
			printf("Execute--->Connect Check False !\n");

		}

	} catch (SQLException &e) {
		cout << ", SQL: " << sql.c_str() << " ;" << endl;
		cout << "ERROR: SQLException in " << __FILE__;
		cout << " (" << __func__ << ") on line " << __LINE__ << endl;
		cout << "ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << ")" << endl;
//		RollBack(savept);
	}
}

bool MysqlBase::ExecuteProcedure(const SQLString& sql) {
	bool ret = 0;
	return ret;
}

//execute select sql and return resultset
bool MysqlBase::ExecuteQuery(const SQLString& sql) {
//	printf("ExecuteQuery--->begin \n");

	try {

		if (ConnectCheck()) {

			if (!state) {
				state = conn->createStatement();
			}

			bool flag = false;
			if (this->commitflag == 0) {
				conn->setAutoCommit(1);
				commitflag = 1;
				flag = true;
			}

			res = state->executeQuery(sql);

			if (this->commitflag == 1 && flag) {
				conn->setAutoCommit(0);
				commitflag = 0;
			}
			//    	    printf("ExecuteQuery--->222222222222222\n");
			if (res) {
				//    	    	printf("ExecuteQuery--->true\n");
				return true;
			}

			else {
				//    	    	printf("ExecuteQuery--->false\n");
				return false;
			}
		} else {
			printf("ExecuteQuery--->Connect Check False !\n");
			return false;
		}

	} catch (SQLException &e) {
		cout << ", SQL: " << sql.c_str() << " ;" << endl;

		cout << "ERROR: SQLException in " << __FILE__;
		cout << " (" << __func__ << ") on line " << __LINE__ << endl;

		cout << "ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << ")" << endl;
		//		RollBack(savept);
		return false;
	}
}

void MysqlBase::Empty() {
	if (res) {
		delete res;
		res = NULL;
	}
}

//execute insert,update,delete sql
void MysqlBase::ExecuteUpdate(const SQLString& sql) {

	try {

		if (ConnectCheck()) {
			bool flag = false;
			if (commitflag == 1) {
				conn->setAutoCommit(0);
				this->commitflag = 0;
				flag = true;
			}

			savept = conn->setSavepoint("savept");

			conn->setTransactionIsolation(TRANSACTION_SERIALIZABLE); //when change the data,we can't read the data
			prep_stmt = conn->prepareStatement(sql);
			int updatecount = prep_stmt->executeUpdate();

			if (updatecount == 0) {
				printf("MysqlBase::ExecuteUpdate -->no rows update:%s\n",
						sql.c_str());
				return;
			} else {
				//        	 printf(" %d rows update\n",updatecount);
			}

			Commit();

			if (savept) {
				delete savept;
				savept = NULL;
			}

			if (prep_stmt) {
				delete prep_stmt;
				prep_stmt = NULL;
			}

			if (commitflag == 0 && flag) {
				conn->setAutoCommit(1);
				this->commitflag = 1;
			}
		}

	} catch (SQLException &e) {
		cout << ", SQL: " << sql.c_str() << " ;" << endl;
		cout << "ERROR: SQLException in " << __FILE__;
		cout << " (" << __func__ << ") on line " << __LINE__ << endl;
		cout << "ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << ")" << endl;
		RollBack(savept);
	}
}

void MysqlBase::Commit() {
	if (commitflag == 0) {
		conn->commit();
	}

}

void MysqlBase::RollBack(Savepoint * savepoint) {
	conn->rollback(savepoint);
	conn->releaseSavepoint(savepoint);
}

bool MysqlBase::ReConnect() {

//	 if(res)	    {
//			delete res;
//			res = nullptr;
//		}
//		if(savept)	    {
//			delete savept;
//			savept = nullptr;
//		}
//// if(res_meta)	    {
////		delete res_meta;
////		res_meta = NULL;
////	}
//	if(state)	    {
//		delete state;
//		state = nullptr;
//	}
//	if(prep_stmt)	    {
//		delete prep_stmt;
//		prep_stmt = nullptr;
//	}
//
//	if(conn)	    {
//	//	    	printf("MysqlBase    - conn close \n");
//			conn->close();
//			delete conn;
//			conn = nullptr;
//		}
//
//	conn = driver->connect(_DBHOST, _USER, _PASSWORD);
//
//
//	state = conn->createStatement();
//
//	 conn->setSchema(_DATABASE);
//
//	if(!conn || !state){
//
//		if(!conn){
//					cout << "MysqlBase::ReConnect driver->connect err!!!\n";
//		}
//
//		if(!state){
//					cout << "MysqlBase::ReConnect conn->createStatement err!!!\n";
//		}
//
//		return false;
//	}
//	else{
//		return true;
//	}

	return conn->reconnect();

}

bool MysqlBase::ConnectCheck() {
	bool ret = false;

	if (this->IsCloseConn()) {
		printf(
				"MysqlBase::ConnectCheck ----->Connect Closed !  database:%s   tablename:%s\n",
				this->getDatabase().c_str(), this->getTablename().c_str());
		return conn->reconnect();
	} else {
		if (IsAliveConn()) {
			ret = true;
			return ret;
		} else {
			printf(
					"MysqlBase::ConnectCheck  --->Connect Alive False!   database:%s   tablename:%s \n",
					this->getDatabase().c_str(), this->getTablename().c_str());
			return ReConnect();
//    		    		return conn->reconnect();
		}
	}

}

//string MysqlBase::getColumnName(int index)
//{
//    if(res_meta == NULL)
//    {
//    	res_meta = res->getMetaData();
//    }
//    string columnname = res_meta->getColumnLabel(index+1);
//    return columnname;
//}

bool MysqlBase::getNext() {
	return res->next();
}

bool MysqlBase::getFirst() {
	return res->first();
}

int MysqlBase::getRowCount() {
	return res->rowsCount();
}

bool MysqlBase::isValueNull(int columnindex) {
	return res->isNull(columnindex) ? true : false;
}

bool MysqlBase::isValueNull(const string& columnname) {
	return res->isNull(columnname) ? true : false;
}

double MysqlBase::getValueDouble(const string& columnname) {
	double value = res->getDouble(columnname);
	return value;
}

int MysqlBase::getValueInt(const string& columnname) {
	int value = res->getInt(columnname);
	return value;
}

string MysqlBase::getValueString(const string& columnname) {
	string value = res->getString(columnname);
	return value;
}

string MysqlBase::getValueString(int columindex) {
	string value = res->getString(columindex);
	return value;
}

std::string MysqlBase::getDatabase() {
	return this->_DATABASE;
}

std::string MysqlBase::getTablename() {
	return this->_TABLENAME;
}

std::string MysqlBase::getDBHost() {
	return this->_DBHOST;
}

int MysqlBase::getCommitFlag() {
	return this->commitflag;
}

