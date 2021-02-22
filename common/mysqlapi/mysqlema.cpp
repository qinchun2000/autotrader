#include "mysqlema.hpp"



MysqlEMA::MysqlEMA()
{
	std::string database="ema";

	if (!IsAliveConn())
	{
		if(!Connect())
			{

				return;
			}
	}

	this->Set_Database(database);

}


void MysqlEMA::CreateTable(const char *tablename)
{
		char buf[2048];

		sprintf(buf,"CREATE TABLE IF NOT EXISTS %s("
							"Time DATETIME NOT NULL PRIMARY KEY, "
							"EMA49 DOUBLE(15,3),"
							"EMA8 DOUBLE(15,3),"
							"EMA3 DOUBLE(15,3)) default character set utf8 ",tablename);
		std::string str_sql;
		str_sql=buf;

		this->Execute(str_sql);


}


void MysqlEMA::Insert(const char *pinstrumentid,EMAData data)
{
	printf("insert begin\n");

	char buf[1024];
	std::string str_sql;

	sprintf(buf,"INSERT INTO %s(Time,EMA49,EMA8,EMA3) VALUES ('%s', '%.3f','%.3f','%.3f')",
			pinstrumentid,data.Time,data.EMA49,data.EMA8,data.EMA3);

	str_sql=buf;
	this->Execute(str_sql);
//	conn->commit();
}


std::string MysqlEMA::GetLastDateTime(const char *tablename)
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




bool MysqlEMA::Exist_DateTime(const char *tablename,std::string datetime)
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


EMAData  MysqlEMA::Find_LastEMAData(const char *tablename)
{

	EMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s ORDER BY Time desc limit 1 ",tablename);
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{
			if(this->getRowCount()==1)
				{
					this->getFirst();
					 strcpy(data.Time,getValueString("Time").c_str());

					 data.EMA49=getValueDouble("EMA49");
					 data.EMA8 =getValueDouble("EMA8");
					 data.EMA3 =getValueDouble("EMA3");
				}
		}
	this->Empty();
	  return data;

}


EMAData  MysqlEMA::Find_EMADatabyTime(const char *tablename,std::string datetime)
{

	EMAData data;
	char buf[2048];
	std::string str_sql;

	sprintf(buf,"SELECT * FROM %s WHERE Time='%s' ",tablename,datetime.c_str());
	str_sql=buf;
	if(this->ExecuteQuery(str_sql))
		{

			if(this->getRowCount()==1)
				{
					this->getFirst();

					 strcpy(data.Time,getValueString("Time").c_str());
					 data.EMA49=getValueDouble("EMA49");
					 data.EMA8 =getValueDouble("EMA8");
					 data.EMA3 =getValueDouble("EMA3");

				}
		}
	this->Empty();
	  return data;

}

void MysqlEMA::Find_ListsbyDate(vector<EMAData> &temp,const char *tablename,const char* date,int count)
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
				EMAData data;
				 strcpy(data.Time,getValueString("Time").c_str());
				 data.EMA49=getValueDouble("EMA49");
				 data.EMA8 =getValueDouble("EMA8");
				 data.EMA3 =getValueDouble("EMA3");

				temp.push_back(data);
				this->getNext();
			}
	}
	reverse(temp.begin(),temp.end());
	this->Empty();

}
double  MysqlEMA::EMA(const char* pinstrumentid,const char* sqltime, int N)
{
	std::vector<HxDayLineData> X;
	std::vector<HisEMAData> Y;

	MysqlDayLine mysqldayline;
	mysqldayline.Find_ListsbyDate(X,pinstrumentid,sqltime,N);

	MysqlHisEMA mysqlhiseam;


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
        		vec[i] = k* X[i].Close + vec[i - 1]*(1-k);
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
