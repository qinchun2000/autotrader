#include "mysqlhisema.hpp"



MysqlHisEMA::MysqlHisEMA()
{
	std::string database="hisema";

	if (!IsAliveConn())
	{
		if(!Connect())
			{

				return;
			}
	}

	this->Set_Database(database);

}


void MysqlHisEMA::CreateTable(const char *tablename)
{
		char buf[2048];
		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
						"Time DATETIME NOT NULL PRIMARY KEY, "
						"EMA50 DOUBLE(15,3),"
						"EMA20 DOUBLE(15,3),"
						"EMA9 DOUBLE(15,3),"
						"EMA4 DOUBLE(15,3),"
						"EMA50Angel DOUBLE(15,3),"
						"TrendStatus CHAR(10),"
						"LongCount INT(11),"
						"ShortCount INT(11),"
						"LongStartTime DATETIME,"
						"ShortStartTime DATETIME) default character set utf8 ",tablename);

		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}


void MysqlHisEMA::Insert(const char *pinstrumentid,HisEMAData data)
{


	char buf[1024];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s(Time, EMA50,EMA20,EMA9,EMA4) "
				"VALUES ('%s', '%.3f','%.3f','%.3f','%.3f')",
				pinstrumentid,data.Time,data.EMA50,data.EMA20,data.EMA9,data.EMA4);

	str_sql=buf;
	this->Execute(str_sql);

}


std::string MysqlHisEMA::GetLastDateTime(const char *tablename)
{

	std::string  info;
	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT Time FROM %s ORDER BY Time desc limit 1 ",tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();
					 info =  getValueString("Time");
				}
		}
	this->Empty();
	  return info;

}


bool MysqlHisEMA::Exist_DateTime(const char *tablename,std::string datetime)
{

	bool ret=false;

	char buf[2048];
	std::string str_sql;


	sprintf(buf,"SELECT * FROM %s WHERE Time='%s' ",tablename,datetime.c_str());

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{

		if(this->getRowCount()==1)
		{
			this->getFirst();
			 if(strcmp(getValueString("Time").c_str(),datetime.c_str())==0)
			 {
				 ret=true;
			 }

		}


	}
	this->Empty();
	return ret;
}


HisEMAData  MysqlHisEMA::Find_LastHisEMADatabyOffset(const char *tablename,int offset)
{

	HisEMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time desc limit 1 OFFSET %d",tablename,offset);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.EMA50=getValueDouble("EMA50");
					 data.EMA20=getValueDouble("EMA20");
					 data.EMA9=getValueDouble("EMA9");
					 data.EMA4 =getValueDouble("EMA4");
				}
		}
	this->Empty();
	  return data;

}

HisEMAData  MysqlHisEMA::Find_HisEMADatabyDateTime_Offset(const char *tablename,const char* datetime,int offset)
{

	HisEMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time desc limit 1 OFFSET %d",tablename,datetime,offset);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.EMA50=getValueDouble("EMA50");
					 data.EMA20=getValueDouble("EMA20");
					 data.EMA9=getValueDouble("EMA9");
					 data.EMA4 =getValueDouble("EMA4");
				}
		}
	this->Empty();
	  return data;

}

HisEMAData  MysqlHisEMA::Find_HisEMADatabyDateTime(const char *tablename,const char* datetime)
{

	HisEMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time ='%s' ",tablename,datetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.EMA50=getValueDouble("EMA50");
					 data.EMA20=getValueDouble("EMA20");
					 data.EMA9=getValueDouble("EMA9");
					 data.EMA4 =getValueDouble("EMA4");
				}
		}
	this->Empty();
	  return data;

}
HisEMAData  MysqlHisEMA::Find_PreHisEMADatabyDateTime(const char *tablename,const char* datetime)
{

	HisEMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Where Time <'%s' ORDER BY Time desc limit 1 ",tablename,datetime);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.EMA50=getValueDouble("EMA50");
					 data.EMA20=getValueDouble("EMA20");
					 data.EMA9=getValueDouble("EMA9");
					 data.EMA4 =getValueDouble("EMA4");
				}
		}
	this->Empty();
	  return data;

}

void MysqlHisEMA::Find_ListsbyDate(vector<HisEMAData> &temp,const char *tablename,const char* date,int count)
{

	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s Time<'%s' ORDER BY Time DESC LIMIT %d ",tablename,date,count);

	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
	{
		this->getFirst();
		/* retrieve the row count in the result set */
		for (int i=0; i<this->getRowCount();i++)
			{
				HisEMAData data;
				strcpy(data.Time,getValueString("Time").c_str());

				 data.EMA50=getValueDouble("EMA50");
				 data.EMA20=getValueDouble("EMA20");
				 data.EMA9=getValueDouble("EMA9");
				 data.EMA4 =getValueDouble("EMA4");

				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}

double  MysqlHisEMA::EMA(const char* pinstrumentid,const char* sqltime, int N)
{
	std::vector<HxDayLineData> X;
	std::vector<HisEMAData> Y;

	MysqlDayLine mysqldayline;
	mysqldayline.Find_ListsbyDate(X,pinstrumentid,sqltime,N);

	MysqlHisEMA mysqlhisema;


    std::vector<double> vec;
    double ret;
    int nLen = X.size();
    if(nLen >= 1)
    {
        if(N > nLen) N = nLen;
        vec.resize(nLen);

        double k =2.0/((double)N+1.0);

//        printf ("k   --->   %f \n",k);
        for(int i = 0; i < nLen; i++)
        {
//            vec[i] = (2 * X[i] + (N - 1) * vec[i - 1]) / (N + 1);
//        	int n=i+1;
        	if (i==0)
        	{
        		 vec[i] = X[i].Close;
        	}
        	else
        	{
        		if(mysqlhisema.Exist_DateTime(pinstrumentid,X[i].Time))
        		{
        			HisEMAData yy=mysqlhisema.Find_HisEMADatabyDateTime(pinstrumentid,X[i].Time);
        			vec[i] = k* X[i].Close + vec[i - 1]*(1-k);
        		}
        		else
        		{
        			vec[i] = k* X[i].Close + vec[i - 1]*(1-k);
        		}

        	}


            ret =vec[i];
//            printf("EMA  --------  i :%d  close: %f   vec[i-1]  %f  vec[i]: %f    ret: %f   \n",i,X[i],vec[i - 1], vec[i],ret);
        }
    }

    vector<double>().swap(vec);
    std::vector<HxDayLineData>().swap(X);
    std::vector<HisEMAData>().swap(Y);
    return ret;
}
