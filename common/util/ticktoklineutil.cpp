#include "ticktoklineutil.hpp"

TickToKlineUtil::TickToKlineUtil()
{
	m_priceVec.clear();
	m_volumeVec.clear();
}
void TickToKlineUtil::WriteSingleMinuteline(const char* pinstrumentid)
{
	char productid[10];
	CodeUtil codeutil;
	strcpy(productid,codeutil.GetProductID(pinstrumentid).c_str());

	char number[10];
	strcpy(number,codeutil.GetNumber(pinstrumentid).c_str());

	std::string year ="20";
	year.append(number);




}

void TickToKlineUtil::ConvertTick_Minuteline(const char* productid,const char* year,const char*tradingday,const char* preday,const char* contract_month)
{
	char buf[100];
	sprintf(buf,"/home/vnpy/hisdata/%s/%s_1/%s/%s/",year,year,productid,tradingday);

	std::string path=buf;
	char f_buf[30];
	sprintf(f_buf,"%s%s_%s.csv",productid,contract_month,tradingday);
	std::string filename =f_buf;

	printf(" path: %s      filename: %s   \n",path.c_str(),filename.c_str());
//	ReadTickData_CSV(path.c_str(),filename.c_str(),preday);
}

void TickToKlineUtil::ReadTickData_CSV(vector<KLineDataType> &temp,const char* path,const char*filename,const char* preday)
{
//	std::string path="/home/vnpy/hisdata/2015/2015_1/rb/20150105/";
//	std::string filename ="rb01_20150105.csv";

//	std::string path_filename =path;
//	path_filename.append(filename);

	bool MnKlinesig;

	CSVFile CSVRead;

	if (!CSVRead.Open(true, path, filename))
	{
		printf("File not found!\n");
	}
	else
	{
		printf("File found!\n");
	}

	while (CSVRead.CSVReadNextRow())
	{

	    std::string strDate;
	    std::string strTime;
	    std::string strMs;
	    std::string strPrice;
	    std::string strVolume;
	    std::string strOPI;

	    CSVRead.CSVRead("日期", strDate);
	    CSVRead.CSVRead("时间", strTime);
	    CSVRead.CSVRead("毫秒", strMs);

	    CSVRead.CSVRead("最新价", strPrice);
	    CSVRead.CSVRead("成交量", strVolume);
	    CSVRead.CSVRead("持仓量", strOPI);

	    int hours = atoi(strTime.substr(0,2).c_str());
		int minutes = atoi(strTime.substr(3,2).c_str());
		int seconds = atoi(strTime.substr(6,2).c_str());
		int Millisecs = atoi(strMs.c_str());

		double lastprice = atof(strPrice.c_str());
		int volume = atoi(strVolume.c_str());
		int opi = atoi(strOPI.c_str());

		double Q_BarTime	= (1/10e1)*hours + (1/10e3)*minutes + (1/10e5)*seconds;

//	    printf("  [%s %s  %s]   ------>[%.6f]  ---> lastprice %.3f   volume  %d opi  %d \n",
//	    		strDate.c_str(),strTime.c_str(),strMs.c_str(),Q_BarTime,lastprice,volume,opi);


	    bool Timemore0=Q_BarTime!=0.0859 ;
		bool Timemore1=Q_BarTime!=0.1029 && Q_BarTime!=0.1329 ;
		bool Timemore2=Q_BarTime!=0.2059 ;

		bool Timemore_morning1 = (Q_BarTime>=0.0859 && Q_BarTime<0.1131);
//			bool Timemore_morning2 = (Q_BarTime_2>=0.1030 && Q_BarTime_2<0.1131);
		bool Timemore_afternoon = (Q_BarTime>=0.1300 && Q_BarTime<0.1501);    // IC IF IH T TF  13:00 开盘
		bool Timemore_night = (Q_BarTime>=0.205900 && Q_BarTime<0.240000);
		bool Timemore_night1 = (Q_BarTime>=0.000000 && Q_BarTime<0.023100);
		bool Timemore3=( Timemore_morning1  || Timemore_afternoon || Timemore_night || Timemore_night1);


		KLineDataType contractflag;

		if (   Timemore3 && seconds>=0 && seconds<40 && Millisecs >400 && MnKlinesig==false )
		{
			if(Q_BarTime==0.085900||Q_BarTime==0.090000 ||Q_BarTime==0.103000 ||Q_BarTime==0.133000 ||Q_BarTime==0.205900||Q_BarTime==0.210000)
			{
				MnKlinesig=true;
				m_priceVec.clear();
				m_volumeVec.clear();

				 m_priceVec.push_back(lastprice);
				 m_volumeVec.push_back(volume);
				 m_opiVec.push_back(opi);
			}
			else
			{
				 m_priceVec.push_back(lastprice);
				 m_volumeVec.push_back(volume);
				 m_opiVec.push_back(opi);

				KLineDataType k_line_data;
				k_line_data.open_price = m_priceVec.front();
				k_line_data.high_price = *std::max_element(m_priceVec.begin(), m_priceVec.end());
				k_line_data.low_price = *std::min_element(m_priceVec.begin(), m_priceVec.end());
				k_line_data.close_price = m_priceVec.back();
				// 成交量的真实的算法是当前区间最后一个成交量减去上去一个区间最后一个成交量
				k_line_data.volume = m_volumeVec.back() - m_volumeVec.front();
				//m_KLineDataArray.push_back(k_line_data); // 此处可以存到内存
				k_line_data.opi = m_opiVec.back() ;

				if (Q_BarTime>0.2100)
				{
					std::string temp = preday;
					std::string predate = temp.substr(0,10);
					sprintf(k_line_data.Time,"%s %s",predate.c_str(),strTime.c_str());
				}
				else
				{
					sprintf(k_line_data.Time,"%s %s",strDate.c_str(),strTime.c_str());
				}


//				printf("[%s]分钟线  open %.3f    high %.3f    low %.3f   close  %.3f   volume %d  opi %d \n",k_line_data.Time,k_line_data.open_price ,
//					k_line_data.high_price,k_line_data.low_price , k_line_data.close_price , k_line_data.volume,k_line_data.opi );

				temp.push_back(k_line_data);
				m_priceVec.clear();
				m_volumeVec.clear();


				MnKlinesig=true;
			}


		}
		else
		{

			 m_priceVec.push_back(lastprice);
			 m_volumeVec.push_back(volume);
			 m_opiVec.push_back(opi);
		}

		if (seconds>45 && seconds<55 && MnKlinesig==true)
		{
			MnKlinesig=false;
		}
	}// while loop end
}


//void TickToKlineUtil::KLineFromLocalData(TickDataField)
//{
//    // 先清理残留数据
//    m_priceVec.clear();
//    m_volumeVec.clear();
//    m_KLineDataArray.clear();
//
//    std::cout << "开始转换tick到k线..." << std::endl;
//    // 默认读取的tick数据表有4个字段：合约代码、更新时间、最新价、成交量
//    std::ifstream srcInFile;
//    std::ofstream dstOutFile;
//    srcInFile.open(sFilePath, std::ios::in);
//    dstOutFile.open(dFilePath, std::ios::out);
//    dstOutFile << "开盘价" << ','
//        << "最高价" << ','
//        << "最低价" << ','
//        << "收盘价" << ','
//        << "成交量" << std::endl;
//
//    // 一遍解析文件一边计算k线数据，1分钟k线每次读取60 * 2 = 120行数据
//    std::string lineStr;
//    bool isFirstLine = true;
//    while (std::getline(srcInFile, lineStr))
//    {
//        if (isFirstLine)
//        {
//            // 跳过第一行表头
//            isFirstLine = false;
//            continue;
//        }
//        std::istringstream ss(lineStr);
//        std::string fieldStr;
//        int count = 4;
//        while (std::getline(ss, fieldStr, ','))
//        {
//            count--;
//            if (count == 1)
//                m_priceVec.push_back(std::atof(fieldStr.c_str()));
//            else if (count == 0)
//            {
//                m_volumeVec.push_back(std::atoi(fieldStr.c_str()));
//                break;
//            }
//        }
//
//        // 计算k线
//
//        if (m_priceVec.size() == kDataLineNum)
//        {
//            KLineDataType k_line_data;
//            k_line_data.open_price = m_priceVec.front();
//            k_line_data.high_price = *std::max_element(m_priceVec.cbegin(), m_priceVec.cend());
//            k_line_data.low_price = *std::min_element(m_priceVec.cbegin(), m_priceVec.cend());
//            k_line_data.close_price = m_priceVec.back();
//            // 成交量的真实的算法是当前区间最后一个成交量减去上去一个区间最后一个成交量
//            k_line_data.volume = m_volumeVec.back() - m_volumeVec.front();
//            //m_KLineDataArray.push_back(k_line_data); // 此处可以存到内存
//
//            dstOutFile << k_line_data.open_price << ','
//                << k_line_data.high_price << ','
//                << k_line_data.low_price << ','
//                << k_line_data.close_price << ','
//                << k_line_data.volume << std::endl;
//
//            m_priceVec.clear();
//            m_volumeVec.clear();
//        }
//    }
//
//    srcInFile.close();
//    dstOutFile.close();
//
//    std::cout << "k线生成成功" << std::endl;
//}
//
//void TickToKlineUtil::KLineFromRealtimeData(CThostFtdcDepthMarketDataField *pDepthMarketData)
//{
//    m_priceVec.push_back(pDepthMarketData->LastPrice);
//    m_volumeVec.push_back(pDepthMarketData->Volume);
//    if (m_priceVec.size() == kDataLineNum)
//    {
//        KLineDataType k_line_data;
//        k_line_data.open_price = m_priceVec.front();
//        k_line_data.high_price = *std::max_element(m_priceVec.cbegin(), m_priceVec.cend());
//        k_line_data.low_price = *std::min_element(m_priceVec.cbegin(), m_priceVec.cend());
//        k_line_data.close_price = m_priceVec.back();
//        // 成交量的真实的算法是当前区间最后一个成交量减去上去一个区间最后一个成交量
//        k_line_data.volume = m_volumeVec.back() - m_volumeVec.front();
//        m_KLineDataArray.push_back(k_line_data); // 此处可以存到内存
//
//        m_priceVec.clear();
//        m_volumeVec.clear();
//    }
//}
