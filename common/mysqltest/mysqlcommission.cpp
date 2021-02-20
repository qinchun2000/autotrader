#include "mysqlcommission.hpp"

MysqlCommission::MysqlCommission()
{
	std::string database="commission";

	std::string tablename="simnow";

	if(this->IsCloseConn())
	{

		if(!Connect())
		{

			return;
		}

	}
	else
	{
		if (IsAliveConn())
		{

		}
		else
		{

			if(!Connect())
			{

				return;
			}
		}
	}


	this->Set_Database(database);

	this->Set_Table(tablename);

}


void MysqlCommission::CreateTable()
{

	char buf[2048];

	sprintf(buf,"CREATE TABLE IF NOT EXISTS %s ("
						"ProductID CHAR(10) NOT NULL PRIMARY KEY, "
						"ExchangeId CHAR(10),"
						"Mode CHAR(10),"
						"OpenAccount DOUBLE,"
						"OpenNumber DOUBLE,"
						"CloseAccount DOUBLE,"
						"CloseNumber DOUBLE,"
						"VolumeMultiple INT,"
						"Margin DOUBLE) default character set utf8 ",this->getTablename().c_str());
	std::string str_sql;
	str_sql=buf;

	this->Execute(str_sql);


}



void MysqlCommission::Insert(Commission data)
{

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"INSERT INTO %s (ProductID,ExchangeID,Mode,OpenAccount,OpenNumber,"
			"CloseAccount,CloseNumber,Margin,VolumeMultiple) VALUES ('%s','%s','%s','%f','%f','%f','%f','%f' ,'%d')",this->getTablename().c_str(),
				data.ProductID,data.ExchangeID,data.Mode,data.OpenAccount,data.OpenNumber,
				data.CloseAccount,data.CloseNumber,data.Margin,data.VolumeMultiple);

	str_sql=buf;

	this->Execute(str_sql);


}
bool MysqlCommission::Exist_DatabyProductID(const char *productid)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE ProductID='%s' ",this->getTablename().c_str(),productid);


	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		if(this->getRowCount()>0)
		{
			ret=true;
		}


	}
	this->Empty();
	return ret;
}



Commission MysqlCommission::Find_DatabyProductID(const char *productid)
{

	Commission info;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE ProductID='%s' ",this->getTablename().c_str(),productid);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{

		if(this->getRowCount()==1)	{
				this->getFirst();
				 strcpy(info.ProductID,getValueString("ProductID").c_str());
				 strcpy(info.ExchangeID,getValueString("ExchangeID").c_str());
				 strcpy(info.Mode,getValueString("Mode").c_str());
				 info.OpenAccount=getValueDouble("OpenAccount");
				 info.OpenNumber=getValueDouble("OpenNumber");
				 info.CloseAccount=getValueDouble("CloseAccount");
				 info.CloseNumber=getValueDouble("CloseNumber");
				 info.Margin=getValueDouble("Margin");
				 info.VolumeMultiple=getValueInt("VolumeMultiple");
			}
		}
	this->Empty();
	return info;

}
void MysqlCommission::Find_AllList(vector<Commission> &temp)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s  ",this->getTablename().c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)	{
			Commission info;
			 strcpy(info.ProductID,getValueString("ProductID").c_str());
			 strcpy(info.ExchangeID,getValueString("ExchangeID").c_str());
			 strcpy(info.Mode,getValueString("Mode").c_str());
			 info.OpenAccount=getValueDouble("OpenAccount");
			 info.OpenNumber=getValueDouble("OpenNumber");
			 info.CloseAccount=getValueDouble("CloseAccount");
			 info.CloseNumber=getValueDouble("CloseNumber");
			 info.Margin=getValueDouble("Margin");
			 info.VolumeMultiple=getValueInt("VolumeMultiple");
			temp.push_back(info);
			this->getNext();
		}
	}
	this->Empty();

}

