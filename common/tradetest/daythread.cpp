#include "daythread.hpp"

void DayThread::run()
{
    printf("run  \n");
    Test(this->GetYear().c_str());
}
//void DayThread::Test(const char * year)
//{
//	std:: string str_year=year;
//    printf("test --->%s \n",year);
//}

void DayThread::SetYear(const char * year)
{
	this->_year=year;
    printf("set year %s \n",_year.c_str());
}
std::string  DayThread::GetYear()
{
	return this->_year;

}
void DayThread::Test(const char * year)
{
	 printf("Test %s \n",year);
//		CheckContractTime();
		std:: string str_year=year;
		DateUtil dateutil;
		std::string date = dateutil.GetDateTimeing();

		std::string director="短线回测";

		std::string section = "Black";
		std::string stratagename = "daytrade_";


		std::string table_testreport=stratagename;
		table_testreport.append(str_year);

		std::string table_testrecord_prefix;
		table_testrecord_prefix=year;
		table_testrecord_prefix.append("_");
		table_testrecord_prefix.append(stratagename);

		std::string record_path ="/home/vnpy/";
		record_path.append(director);
		record_path.append("/");
		record_path.append(date);
		record_path.append("-record-");
		record_path.append(str_year);
		record_path.append(".xls");


		std::string report_path ="/home/vnpy/";
		report_path.append(director);
		report_path.append("/");
		report_path.append(date);
		report_path.append("-testreport-");
		report_path.append(str_year);
		report_path.append(".xls");

		MysqlTestReport mysqltestreport;
		if(mysqltestreport.Exist_Table(table_testreport.c_str()))
		{
			mysqltestreport.DeleteAllbyTableName(table_testreport.c_str());
		}

		MysqlTestRecord mysqltestrecord;
		if (mysqltestrecord.CountTables()>0)
		{
			mysqltestrecord.DropPrefixTables(table_testrecord_prefix.c_str());
		}

		//计算板块联动用的分钟时间轴
	//	CheckContractMinuteTime(section.c_str());

	//MysqlUtil mysqlutil;
	////mysqlutil.CorrectDayLineByInstrument("hc1801");
	//mysqlutil.CheckEMADataByInstrument("hc1801");

		////全部合约测试


		DayTradeTest daytradetest(stratagename.c_str(),str_year.c_str());
//		daytradetest.HisDataCheck(str_year.c_str());
		daytradetest.AllMainContract_DayTradeTest(str_year.c_str());
	//////
	//////
	//	//生成回测汇总数据表
		daytradetest.TestReport();
		daytradetest.Create_TestRecordXlsFile(record_path);
		daytradetest.Create_TestReportXlsFile(report_path);

}
