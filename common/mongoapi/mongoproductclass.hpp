/*
 * mongoproductclass.hpp
 *
 *  Created on: 2017-8-9
 *      Author: vnpy
 */
#include <vector>
#include <cstdint>
#include <cstdio>
#include <iostream>

#include <string.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <json/json.h>


#ifndef MONGOPRODUCTCLASS_HPP_
#define MONGOPRODUCTCLASS_HPP_

using namespace mongocxx;
using namespace std;

using mongocxx::database;
using bsoncxx::stdx::optional;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


//extern char *pProductID[60];


class ProductClassInfo
	{
	public:
		char ProductID[10];
		char ExchangeID[9];
		char Section[30];
		char DayNight[30];


	};


class MongoProductClass
	{
	public :

		MongoProductClass();

		void DeleteAll();
		void Insert(ProductClassInfo productclassinfo);
		ProductClassInfo Findby_ProductID(char * productid);
		void Check_ProductClass(ProductClassInfo productclassinfo);
//		char** Find_All();

		void FindAll_ProductClass(vector<ProductClassInfo> &temp);
		int Count();

	private :

		char  _mongodb_url[30];
		char  _mongodb_dbname[20];
		char  _mongodb_collenction[20];
		mongocxx::database _dbname;
		mongocxx::collection _collection;
	};

#endif /* MONGOPRODUCTCLASS_HPP_ */
