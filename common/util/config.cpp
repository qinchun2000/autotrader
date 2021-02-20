#include "config.hpp"

//Config::Config()
//{
//
////	printf("config()   begin     %s \n",this->GetConfigFileName().c_str());
//	SetConfigFileName("config");
//	Initial();
//}
Config::Config(const char* configfilename)
{
//	printf("构造函数 config  ---> %s  \n",configfilename);
	std::string name =configfilename;

	if(name.length()==0){
		name.append("config.txt");
	}
	else {
		name.append(".txt");
	}

	SetConfigFileName(name.c_str());
//	printf("构造函数 config  ---> name  %s [%ld] \n",name.c_str(),name.length());

	std::string work_directory;
	work_directory=this->GetWorkDirectory();
//	printf("config   initial 000  \n");
	std::string config_directory=work_directory;
	config_directory.append("config/");


	SetConfigPath(config_directory.c_str());

	SetStrategyName();

	Initial();
//	printf("构造函数 config  --->end  %s  \n",configfilename);

}


std::string Config::GetWorkDirectory()
{
//	printf("config   GetWorkDirectory begin  \n");
//	int PATH_MAX=1024;
 //   char dir[PATH_MAX] ;
//    char syspath[PATH_MAX] ;
 //   memset(dir,0,PATH_MAX*sizeof(int));
  //  memset(syspath,0,PATH_MAX*sizeof(int));
//	int PATH_MAX=1024;
    char dir[1024] ={0};
    char syspath[1024]={0} ;
//    int n = readlink("/proc/self/exe", dir, PATH_MAX);

    std::string s2;
	if(!getcwd(dir, sizeof(dir)))
	{
		s2="";
		return  s2;
	}
//	printf("config   GetWorkDirectory 000  \n");
    const char *ptr;
    ptr = strrchr(dir, '/');

    int length = strlen(dir) - strlen(ptr);
    std::string s1(dir);
//    printf("config   GetWorkDirectory 1111  \n");
    s2 = s1.substr(0, length+1);
//    memcpy(syspath, (char*)s2.data(), PATH_MAX);

//    ptr = strrchr(syspath, '/');
//    length = strlen(syspath) - strlen(ptr);
//    s2 = s2.substr(0, length);

//    cout << "dir     : " << dir << endl;
//   cout << "syspath : " << syspath << endl;
//    cout << "s2      : " << s2 << endl;

    return s2;
}

void Config::Initial()
{
//	printf("config   initial begin  \n");
//	printf("config   initial 111  \n");
	SetHolidayPath(this->GetConfigPath().c_str());
	SetHolidayFileName("holiday.csv");


	std::string log_directory=this->GetWorkDirectory();
	log_directory.append("log/");

	SetLogPath(log_directory.c_str());
//	printf("Config::SetLogPath ---> [%s]   \n",log_directory.c_str());

//	std::string log_pathfilename=log_directory;
//	log_pathfilename.append("baseinfo.log");
//	printf("config   initial 222  \n");


//	printf("Config::Initial ---> log directory  [%s]   \n",log_directory.c_str());
	this->_hisxls_path = this->GetWorkDirectory();
	this->_hisxls_path.append("hisxls/");

	std::string nightpathfile =this->GetConfigPath();
	nightpathfile.append("night.txt");
	SetNightPorductPathFile(nightpathfile.c_str());

	this->SetBrokerID();
	this->SetUserID();
//	SetUserID("qinchun");
	this->SetPasswrod();
	this->SetAppID();
	this->SetFrontAddress();
	this->SetMDAddress();
	this->SetProductInfo();
	this->SetAuthCode();


	SetEmailAddress("qinchun@sina.com");
	SetEmailPasswrod("Qin19780216");
	SetSmtpAddress("smtp://smtp.sina.com:25");

	this->SetInitialMoney();
	this->SetRiskRate();
	this->SetStopRate();
	this->SetWinRate();
	this->SetBigVolumeRate();
	this->SetMaxSedimentary();
	this->SetSingleMaxMargin();
	this->SetTotalMaxMargin();
	this->SetOpenSignalCount();
	this->SetCloseSignalCount();
	this->SetMaxCancelOrderCount();
	this->SetMaxOpenErrorCount();
	this->SetMaxCloseErrorCount();

	char  YINHE_TDADDR_A[] = "tcp://58.247.171.147:51205";
	char  YINHE_TDADDR_B[] = "tcp://27.115.78.35:52205";
	char  YINHE_TDADDR_C[] = "tcp://58.247.171.147:53205";

	char  FRONT_ADDR_ZLQH_TEST[] = "tcp://27.115.56.212:41205";
		char  MD_ADDR_ZLQH_TEST[] = "tcp://27.115.56.212:41213";

	char  FRONT_ADDR_ZLQH[] = "tcp://140.206.103.245:41205";
	char  MD_ADDR_ZLQH[] = "tcp://140.206.103.245:41213";

//	char  FRONT_ADDR_ZLQH_ct[] = "tcp://140.206.103.243:41204";
//	char  MD_ADDR_ZLQH_ct[] = "tcp://140.206.103.243:41212";

	char  FRONT_ADDR_ZLQH_B[] = "tcp://140.206.103.243:41205";
	char  MD_ADDR_ZLQH_B[] = "tcp://140.206.103.243:41213";
//	SetFrontAddressLists(FRONT_ADDR_3C);

//	SetFrontAddressLists(YINHE_TDADDR_A);
//	SetFrontAddressLists(YINHE_TDADDR_B);
//	SetFrontAddressLists(YINHE_TDADDR_C);
	char  FRONT_ADDR_3A[] = "tcp://180.168.146.187:10000";
	char  FRONT_ADDR_3B[] = "tcp://180.168.146.187:10001";
	char  FRONT_ADDR_3C[] = "tcp://218.202.237.33:10002";

	char  md_add_a[] = "tcp://180.168.146.187:10010";
	char  md_add_b[] = "tcp://180.168.146.187:10011";
	char  md_add_c[] = "tcp://218.202.237.33:10012";

	if (strcmp(this->GetStrategyName().c_str(),"CalendarSpreadTrade_zlqh")==0){
//		printf("Config::Initial ---> 策略名:%s \n",this->GetStrategyName().c_str());
//		printf("Config::Initial ---> 用户名:%s \n",this->GetUserID().c_str());
//		printf("Config::Initial ---> 交易网关:%s \n",FRONT_ADDR_ZLQH);
//		printf("Config::Initial ---> 行情网关:%s \n",MD_ADDR_ZLQH);
		SetFrontAddressLists(FRONT_ADDR_ZLQH);
		SetMdFrontAddressLists(MD_ADDR_ZLQH);
	}
	else if(strcmp(this->GetStrategyName().c_str(),"ZLQH_TEST")==0){
		SetFrontAddressLists(FRONT_ADDR_ZLQH_TEST);
		SetMdFrontAddressLists(MD_ADDR_ZLQH_TEST);
	}
	else if(strcmp(this->GetStrategyName().c_str(),"SpreadTrade_zlqh")==0){
//		SetFrontAddressLists(FRONT_ADDR_ZLQH_ct);
//		SetMdFrontAddressLists(MD_ADDR_ZLQH_ct);

		SetFrontAddressLists(this->GetFrontAddress().c_str());
		SetMdFrontAddressLists(this->GetMDAddress().c_str());
	}
	else {
//		printf("Config::Initial ---> 策略名:%s \n",this->GetStrategyName().c_str());
//		printf("Config::Initial ---> 用户名:%s \n",this->GetUserID().c_str());
//		printf("Config::Initial ---> 交易网关:%s \n",FRONT_ADDR_3A);
//		printf("Config::Initial ---> 行情网关:%s \n",md_add_a);

////		SetFrontAddressLists(FRONT_ADDR_3A);
//		SetFrontAddressLists(FRONT_ADDR_3B);
//		SetFrontAddressLists(FRONT_ADDR_3C);
//
////		SetMdFrontAddressLists(md_add_a);
//		SetMdFrontAddressLists(md_add_b);
//
//
//		SetMdFrontAddressLists(md_add_c);


		SetFrontAddressLists(this->GetFrontAddress().c_str());
		SetMdFrontAddressLists(this->GetMDAddress().c_str());
	}


	SetTable_Open("open");
	SetTable_Close("close");
	SetTable_OrderList("orderlist");

}
void Config::SetConfigPath(const char* path)
{

	this->_config_path=path;
}
std::string Config::GetConfigPath()
{

	return this->_config_path;
}

void Config::SetConfigFileName(const char* filename)
{
//	printf("Config::SetConfigFileName   %s  \n",filename);
	this->_config_filename=filename;


}
std::string Config::GetConfigFileName()
{

	return this->_config_filename;
}

std::string Config::GetConfigPathFile()
{
	std::string pathfilename =this->_config_path;
	pathfilename.append(GetConfigFileName());

	return pathfilename;
}

void Config::SetNightPorductPathFile(const char* pathfile)
{

	this->_nightproduct_pathfile=pathfile;

}

std::string Config::GetNightPorductPathFile()
{
	return this->_nightproduct_pathfile;
}

void Config::SetFrontAddressLists(const char* address)
{
	this->_front_list.push_back(address);
}

void Config::GetFrontAddressLists(vector<std::string> &list)
{
	for(auto &item:_front_list)
	{
		list.push_back(item);
	}
}

void Config::SetMdFrontAddressLists(const char* address)
{
	this->_md_front_list.push_back(address);
}

void Config::GetMdFrontAddressLists(vector<std::string> &list)
{
	for(auto &item:_md_front_list)
	{
		list.push_back(item);
	}
}

void Config::SetBrokerID(const char* brokerid)
{
this->_brokerid=brokerid;
}
void Config::SetBrokerID()
{
	std::string key="brokerid";
		std::string value;
		readConfigFile(GetConfigPathFile().c_str(),key,value);
	//	printf(" key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

		this->_brokerid=value;
}
std::string Config::GetBrokerID()
{
return this->_brokerid;
}

void Config::SetUserID()
{

	std::string key="userid";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf(" key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_userid=value;
}

void Config::SetUserID(const char* userid)
{
this->_userid=userid;
}

std::string Config::GetUserID()
{
return this->_userid;
}
void Config::SetPasswrod()
{
	std::string key="password";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf(" key   %s     ---> value   %s   \n",key.c_str(),value.c_str());
	this->_password=value;
}
void Config::SetPasswrod(const char* password)
{
this->_password=password;
}

std::string Config::GetPasswrod()
{
return this->_password;
}

void Config::SetStrategyName()
{
	std::string key="strategyname";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_strategyname=value;
}
std::string Config::GetStrategyName()
{
return this->_strategyname;
}
void Config::SetFrontAddress()
{
	std::string key="front_address";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_frontaddress=value;
}
std::string Config::GetFrontAddress()
{
return this->_frontaddress;
}
void Config::SetMDAddress()
{
	std::string key="md_address";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_mdaddress=value;
}
std::string Config::GetMDAddress()
{
return this->_mdaddress;
}
void Config::SetAppID()
{
	std::string key="appid";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_appid=value;
}
std::string Config::GetAppID()
{
return this->_appid;
}

void Config::SetProductInfo()
{
	std::string key="productinfo";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_productinfo=value;
}
std::string Config::GetProductInfo()
{
return this->_productinfo;
}

void Config::SetAuthCode()
{
	std::string key="authcode";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);
//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_authcode=value;
}
std::string Config::GetAuthCode()
{
return this->_authcode;
}


void Config::SetEmailAddress(const char* address)
{
this->_email_address=address;
}

std::string Config::GetEmailAddress()
{
return this->_email_address;
}


void Config::SetEmailPasswrod(const char* password)
{
this->_email_password=password;
}

std::string Config::GetEmailPasswrod()
{
return this->_email_password;
}


void Config::SetSmtpAddress(const char* address)
{
this->_smtp_address=address;
}

std::string Config::GetSmtpAddress()
{
return this->_smtp_address;
}

void Config::SetInitialMoney()
{
	std::string key="initialmoney";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_initialmoney=atof(value.c_str());
}
double Config::GetInitialMoney()
{
return this->_initialmoney;
}


void Config::SetRiskRate()
{
	std::string key="riskrate";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_riskrate=atof(value.c_str());
}
double Config::GetRiskRate()
{
return this->_riskrate;
}

void Config::SetStopRate()
{
	std::string key="stoprate";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_stoprate=atof(value.c_str());
}
double Config::GetStopRate()
{
return this->_stoprate;
}


void Config::SetWinRate()
{
	std::string key="winrate";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_winrate=atof(value.c_str());
}
double Config::GetWinRate()
{
return this->_winrate;
}


void Config::SetBigVolumeRate()
{
	std::string key="bigvolume_rate";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_bigvolumerate=atof(value.c_str());
}
double Config::GetBigVolumeRate()
{
return this->_bigvolumerate;
}
void Config::SetMaxSedimentary()
{
	std::string key="maxsedimentary";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_maxsedimentary=atof(value.c_str());
}
double Config::GetMaxSedimentary()
{
return this->_maxsedimentary;
}
void Config::SetSingleMaxMargin()
{
	std::string key="singleins_maxmargin";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_singleins_maxmargin=atof(value.c_str());
}
double Config::GetSingleMaxMargin()
{
return this->_singleins_maxmargin;
}

void Config::SetTotalMaxMargin()
{
	std::string key="total_maxmargin";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_total_maxmargin=atof(value.c_str());
}
double Config::GetTotalMaxMargin()
{
return this->_total_maxmargin;
}

void Config::SetOpenSignalCount()
{
	std::string key="open_signalcount";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_open_signalcount=atoi(value.c_str());
}
int Config::GetOpenSignalCount()
{
return this->_open_signalcount;
}

void Config::SetCloseSignalCount()
{
	std::string key="close_signalcount";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_close_signalcount=atoi(value.c_str());
}
int Config::GetCloseSignalCount()
{
return this->_close_signalcount;
}


void Config::SetMaxCancelOrderCount()
{
	std::string key="max_cancel_ordercount";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_max_cancel_ordercount=atoi(value.c_str());
}
int Config::GetMaxCancelOrderCount()
{
return this->_max_cancel_ordercount;
}

void Config::SetMaxOpenErrorCount()
{
	std::string key="max_openerror_count";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_max_openerror_count=atoi(value.c_str());
}
int Config::GetMaxOpenErrorCount()
{
return this->_max_openerror_count;
}

void Config::SetMaxCloseErrorCount()
{
	std::string key="max_closeerror_count";
	std::string value;
	readConfigFile(GetConfigPathFile().c_str(),key,value);

//	printf("Config::SetStrategyName---> key   %s     ---> value   %s   \n",key.c_str(),value.c_str());

	this->_max_closeerror_count=atoi(value.c_str());
}
int Config::GetMaxCloseErrorCount()
{
return this->_max_closeerror_count;
}
void Config::SetLogPath(const char* path)
{
this->_log_path=path;
}

std::string Config::GetLogPath()
{
return this->_log_path;
}

//void Config::SetLogPath_Base(const char* path)
//{
//this->_logpath_baseinfo=path;
//}
//
//std::string Config::GetLogPath_Base()
//{
//return this->_logpath_baseinfo;
//}


void Config::SetHolidayPath(const char* path)
{
this->_holiday_path=path;
}

std::string Config::GetHolidayPath()
{
return this->_holiday_path;
}
std::string Config::GetHisXlsPath()
{
return this->_hisxls_path;
}


void Config::SetHolidayFileName(const char* filename)
{
this->_holiday_filename=filename;
}

std::string Config::GetHolidayFileName()
{
return this->_holiday_filename;
}


void Config::SetTable_Open(const char* open)
{
this->_table_open=open;
}

std::string Config::GetTable_Open()
{
return this->_table_open;
}


void Config::SetTable_Close(const char* close)
{
this->_table_close=close;
}

std::string Config::GetTable_Close()
{
return this->_table_close;
}

void Config::SetTable_OrderList(const char* orderlist)
{
this->_table_orderlist=orderlist;
}

std::string Config::GetTable_OrderList()
{
return this->_table_orderlist;
}



bool Config::readConfigFile(const char * cfgfilepath, const string & key, string & value)
{
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//打开文件
    if( ! cfgFile.is_open())
    {
//        cout<<"can not open cfg file!"<<endl;
        printf("Config::readConfigFile 无法打开配置文件  -->  %s   \n",cfgfilepath);
        return false;
    }
    char tmp[1000];
    while(!cfgFile.eof())//循环读取每一行
    {
        cfgFile.getline(tmp,1000);//每行读取前1000个字符，1000个应该足够了
        string line(tmp);
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
        if(pos==string::npos) return false;
        string tmpKey = line.substr(0,pos);//取=号之前
        if(key==tmpKey)
        {
            value = line.substr(pos+1);//取=号之后
            cfgFile.close();
            return true;
        }
    }
    return false;
}

bool Config::IsNightProduct(const char* product)
{

	printf("Config::IsNightProduct ----%s     \n",product);
	bool ret=true;

	std::string key,value;
	key=product;
	bool response=readConfigFile(this->GetNightPorductPathFile().c_str(),key,value);

	printf("value   --> %s   \n ",value.c_str());

	if (strcmp(value.c_str(),"day")==0 && response )	{
//		printf("day   --> %s   \n ",product);
		ret=false;
	}
	else if (strcmp(value.c_str(),"night")==0 && response)	{
//		printf("night   --> %s   \n ",product);
		ret=true;
	}

	return ret;
}
