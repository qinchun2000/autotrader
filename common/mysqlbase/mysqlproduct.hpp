#ifndef _MYSQLPRODUCT_HPP
#define _MYSQLPRODUCT_HPP

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <string>
#include <memory>
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
#include "mysqlbase.hpp"

#include <json/json.h>
//#include "mongoproductclass.hpp"

using namespace std;
using namespace sql;

class MysqlProduct: public MysqlBase {

public:
	MysqlProduct();

	void CreateTable();
	void Insert(ProductData data);
	bool Exist_DatabyProductID(const char *productid);
	ProductData Find_DatabyProductID(const char *productid);

	void Find_DatabyProductID(ProductData &info, const char *productid);
	void Find_AllList(vector<ProductData> &temp);

};

#endif

