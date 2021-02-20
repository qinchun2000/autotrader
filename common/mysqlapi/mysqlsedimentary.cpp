#include "mysqlsedimentary.hpp"



MysqlSedimentary::MysqlSedimentary()
{
	std::string database="sedimentary";


	if (IsAliveConn())
		{
	//		printf("MysqlMdLogin IsAliveConn !!!! %s \n",database.c_str());

		}else
		{

	//		printf("MysqlMdLogin IsAliveConn not connect !!!! %s \n",database.c_str());
			if(!Connect())
			{

				return;
			}

		}

	this->Set_Database(database);


}

void MysqlSedimentary::CreateTable(const char* tablename)
{

	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"DateTime DATETIME NOT NULL PRIMARY KEY, "
						"TotalSedimentary DOUBLE,"
						"Sedimentary_Black DOUBLE,"
						"Sedimentary_Oil DOUBLE,"
						"Sedimentary_Metal DOUBLE,"
						"Sedimentary_Stock DOUBLE,"
						"Sedimentary_Corn DOUBLE,"
						"Sedimentary_Single DOUBLE,"
						"Sedimentary_Finance DOUBLE,"
						"Sedimentary_Feed DOUBLE,"
						"Sedimentary_Precious DOUBLE,"
						"Sedimentary_CookOil DOUBLE) default character set utf8 ",tablename);

	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);
}


void MysqlSedimentary::Insert(const char* tablename,SedimentaryData data)
{
	printf("MysqlMdLogin Insert begin\n");

	char buf[2048];
	std::string str_sql;

	DateUtil dateutil;
	std::string time =dateutil.ConvertCurrentTimetoSqlTime();


	sprintf(buf,"INSERT INTO %s (DateTime,TotalSedimentary,Sedimentary_Black,Sedimentary_Oil,Sedimentary_Metal,"
			"Sedimentary_Stock,Sedimentary_Corn,Sedimentary_Single,Sedimentary_Finance,Sedimentary_Feed,"
			"Sedimentary_Precious,Sedimentary_CookOil) "
			"VALUES ('%s','%.2f', '%.2f','%.2f','%.2f',"
			"'%.2f','%.2f','%.2f','%.2f','%.2f',"
			"'%.2f','%.2f') ",tablename,
			data.DateTime,data.TotalSedimentary,data.Sedimentary_Black,data.Sedimentary_Oil,data.Sedimentary_Metal,
			data.Sedimentary_Stock,data.Sedimentary_Corn,data.Sedimentary_Single,data.Sedimentary_Finance,data.Sedimentary_Feed,
			data.Sedimentary_Precious,data.Sedimentary_CookOil);

	str_sql=buf;

	this->Execute(str_sql);

	printf("MysqlMdLogin Insert end \n");
}



