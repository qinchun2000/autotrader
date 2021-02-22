#include "report.hpp"


Report::Report(DataWrapper * datawrapper,const char* staname) {
	m_pdatawrapper=datawrapper;
	m_strategyname=staname;
//	this->SetStrategy(strategy);
	Config config(m_strategyname.c_str());
	m_userid=config.GetUserID().c_str();
	m_tradingday=m_pdatawrapper->GetTradingDay();

	DateUtil dateutil;
	std::string date = dateutil.GetTodayString();

	if(access(config.GetHisXlsPath().c_str(),0)==-1){
		printf("Report--->>>>不存在文件夹 %s \n",config.GetHisXlsPath().c_str());
		if (mkdir(config.GetHisXlsPath().c_str(),0777)){
			 printf("Report:开始启动程序  --->>>>创建文件夹 %s Error!!!  \n",config.GetHisXlsPath().c_str());
		}
		else{
			 printf("Report:开始启动程序  --->>>>创建文件夹 %s 完成!  \n",config.GetHisXlsPath().c_str());
		}
	}


	this->m_maincontract_path  =config.GetHisXlsPath();
	this->m_maincontract_path.append("main-");
	this->m_maincontract_path.append(date);
	this->m_maincontract_path.append(".xls");


	std::string logname="report_";
	logname.append(staname);
	logname.append("_");
	logname.append(m_userid);

	std::string logfilename="report_";
	logfilename.append(staname);
	logfilename.append("_");
	logfilename.append(m_userid);
	logfilename.append(".log");
	this->SetLogName(logname.c_str());
	this->SetLogPathFileName(logfilename.c_str());

	this->NewLogUtil(this->GetLogName(),this->GetLogPathFileName());

	std::string mail_to = "<12795227@qq.com>";
//	std::string mail_cc = "<g1q2@sohu.com>";
	std::string mail_cc = "<21009491@qq.com>";
	m_smtputil = new SmtpUtil(staname,mail_to.c_str(),mail_cc.c_str());

//	this->GetLogUtil()= new LogUtil(this->GetLogName(),this->GetLogPathFileName());
}


Report::~Report(){
//	printf("Report 析构函数开始\n");
//	this->EmptyLogUtil();

//	if(this->GetLogUtil()==nullptr){
//		delete this->GetLogUtil();
//	}
//	m_pdatawrapper=nullptr;
//	printf("Report 析构函数结束\n");
}
void Report::WriteGlobalViewReport(std::string * content){

	char buf[4096];
	printf("Report::WriteGlobalViewReport    --------整体行情汇总-------------  \n");

	GlobalViewData view ;
	memset(&view,0,sizeof(view));

	m_pdatawrapper->OutputGlobalViewData(&view);

	if(view.TotalCapital >0.1 ){

		int allcount = view.Up_Count +view.Down_Count ;
		double globalview_longweight	;
		double  globalview_shortweight	 ;
		if (allcount>0){
			globalview_longweight= (double )view.Up_Count /(double )allcount ;
			globalview_shortweight = (double )view.Down_Count /(double )allcount ;
		}
		else {
			globalview_longweight=0.0;
			globalview_shortweight=0.0;
		}

		if(view.Status==0){
			printf("整体市场多头  ---> 上涨合约 [%d]   ----> 下跌合约[%d]  上涨多头占比 %.2f%%  \n",
					view.Up_Count,view.Down_Count,globalview_longweight*100);

			content->append("--------整体行情  <<多头>>---------\n");

			sprintf(buf,"上涨合约[%d]---->下跌合约[%d]  上涨多头占比 %.2f%%  \n",
					view.Up_Count,view.Down_Count,globalview_longweight*100);
			content->append(buf);
		}
		else if(view.Status==1){
			printf("整体市场空头  ---> 下跌合约 [%d]   ----> 上涨合约[%d]  下跌空头占比 %.2f%%  \n",
					view.Down_Count,view.Up_Count,globalview_shortweight*100);
			content->append("--------整体行情  <<空头>>---------\n");
			sprintf(buf,"下跌合约 [%d]---->上涨合约[%d]  下跌空头占比 %.2f%%  \n",
					view.Down_Count,view.Up_Count,globalview_shortweight*100);

			content->append(buf);
		}
		else{
			printf("整体市场震荡  ---> 上涨合约 [%d]   ----> 下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
					view.Up_Count,view.Down_Count,globalview_longweight*100,globalview_shortweight*100);
			content->append("--------整体行情  <<震荡>>---------\n");
			sprintf(buf,"上涨合约 [%d]---->下跌合约[%d]   多头占比 %.2f%%  空头占比 %.2f%% \n",
					view.Up_Count,view.Down_Count,globalview_longweight*100,globalview_shortweight*100);

			content->append(buf);
		}

//		MysqlGlobalView mysqlglobalview(m_userid.c_str());
//		vector<GlobalViewData>  list;
//		mysqlglobalview.Find_ListsbyTradingDayReason(list,m_pdatawrapper->GetTradingDay().c_str(),1);
//		sprintf(buf,"[%s]行情变动次数--->  %ld  \n",m_pdatawrapper->GetTradingDay().c_str(),list.size());
//						content->append(buf);
//		for (auto &item:list){
//
//			int all = item.Up_Count +item.Down_Count ;
//			double gv_longweight	;
//			double  gv_shortweight	 ;
//			if (all>0){
//				gv_longweight= (double )item.Up_Count /(double )all ;
//				gv_shortweight = (double )item.Down_Count /(double )all ;
//			}
//			else {
//				gv_longweight=0.0;
//				gv_shortweight=0.0;
//			}
//
//			if(item.Status==0){
//				sprintf(buf,"多头 多占比 %.2f%%  %s\n",gv_longweight*100,item.Time);
//				content->append(buf);
//			}
//			else if (item.Status==1){
//				sprintf(buf,"空头 空占比 %.2f%% %s\n",gv_shortweight*100,item.Time);
//				content->append(buf);
//			}
//			else if (item.Status==2){
//				sprintf(buf,"震荡 多占比 %.2f%% %s\n",gv_longweight*100,item.Time);
//				content->append(buf);
//			}
//		}
//
//		sprintf(buf,"	*********************************************  \n");
//		content->append(buf);
//		vector<GlobalViewData>().swap(list);
//
//
//		vector<GlobalViewData>  absmax_list;
//		mysqlglobalview.Find_ListsbyTradingDayReason(absmax_list,m_pdatawrapper->GetTradingDay().c_str(),3);
//		sprintf(buf,"[%s]涨跌幅最大合约变化次数--->  %ld  \n",m_pdatawrapper->GetTradingDay().c_str(),absmax_list.size());
//						content->append(buf);
//		for (auto &item:absmax_list){
//			sprintf(buf,"合约 ---> %s  [%.2f] >>%s\n",item.AbsMaxUpDownIns,item.AbsMaxUpDown,item.Time);
//			content->append(buf);
//		}
//		sprintf(buf,"	*********************************************  \n");
//		content->append(buf);
//		vector<GlobalViewData>().swap(absmax_list);


		printf("市场整体总资金 [%.2f]-->流入流出资金[%.2f]\n",view.TotalCapital,view.TotalSedimentaryIO);
		sprintf(buf,"市场总资金 [%.2f]->流入流出资金[%.2f]\n",view.TotalCapital,view.TotalSedimentaryIO);
		content->append(buf);

		printf("总流入资金---->  [%.2f]亿 \n",view.TotalSedimentaryIn);
		sprintf(buf,"总流入资金---->  [%.2f]亿 \n",view.TotalSedimentaryIn);
		content->append(buf);

		printf("总流出资金---->  [%.2f]亿\n",view.TotalSedimentaryOut);
		sprintf(buf,"总流出资金---->  [%.2f]亿\n",view.TotalSedimentaryOut);
		content->append(buf);

		printf("板块资金流最多%s---->  [%.2f]亿 \n",view.MaxSedIO_SectionName,view.MaxSedimentaryIO_Section);
		sprintf(buf,"板块资金流最多%s---->  [%.2f]亿\n",view.MaxSedIO_SectionName,view.MaxSedimentaryIO_Section);
		content->append(buf);

		printf("板块资金流最小%s---->  [%.2f]亿 \n",view.MinSedIO_SectionName,view.MinSedimentaryIO_Section);
		sprintf(buf,"板块资金流最小%s---->  [%.2f]亿 \n",view.MinSedIO_SectionName,view.MinSedimentaryIO_Section);
		content->append(buf);

		printf("板块资金流最多%s---->   幅度[%.2f%%]\n",view.MaxSedIORate_SectionName,view.MaxSedimentaryIORate_Section*100);
		sprintf(buf,"板块资金流最多%s---->  幅度[%.2f%%]\n",view.MaxSedIORate_SectionName,view.MaxSedimentaryIORate_Section*100);
		content->append(buf);

		printf("板块资金流最小%s---->  幅度[%.2f%%]\n",view.MinSedIORate_SectionName,view.MinSedimentaryIORate_Section*100);
		sprintf(buf,"板块资金流最小%s---->  幅度[%.2f%%]\n",view.MinSedIORate_SectionName,view.MinSedimentaryIORate_Section*100);
		content->append(buf);

		printf("资金流入最多合约%s---->  [%.2f]亿\n",view.MaxSedimentaryIOIns,view.MaxSedimentaryIO);
		sprintf(buf,"资金流入最多合约%s---->  [%.2f]亿\n",view.MaxSedimentaryIOIns,view.MaxSedimentaryIO);
		content->append(buf);

		printf("资金流出最多合约%s---->  [%.2f]亿\n",view.MinSedimentaryIOIns,view.MinSedimentaryIO);
		sprintf(buf,"资金流出最多合约%s---->  [%.2f]亿\n",view.MinSedimentaryIOIns,view.MinSedimentaryIO);
		content->append(buf);

		printf("资金流动比最高合约%s---->  [%.2f%%]\n",view.MaxSedimentaryIORateIns,view.MaxSedimentaryIORate*100);
		sprintf(buf,"资金流动比最高合约%s---->  [%.2f%%]\n",view.MaxSedimentaryIORateIns,view.MaxSedimentaryIORate*100);
		content->append(buf);

		printf("资金流动比最低合约%s---->  [%.2f%%]\n",view.MinSedimentaryIORateIns,view.MinSedimentaryIORate*100);
		sprintf(buf,"资金流动比最低合约%s---->  [%.2f%%]\n",view.MinSedimentaryIORateIns,view.MinSedimentaryIORate*100);
		content->append(buf);

		printf("上涨[%d]->总幅[%.2f%%]->均幅[%.2f%%]\n",view.Up_Count,view.Total_Up*100,view.Average_Up);
		sprintf(buf,"上涨[%d]->总幅[%.2f%%]->均幅[%.2f%%]\n",view.Up_Count,view.Total_Up*100,view.Average_Up);
		content->append(buf);

		printf("下跌数[%d]->总跌幅[%.2f%%]-->平均跌幅[%.2f%%]\n",view.Down_Count,view.Total_Down*100,view.Average_Down);
		sprintf(buf,"下跌[%d]->总幅[%.2f%%]->均幅[%.2f%%]\n",view.Down_Count,view.Total_Down*100,view.Average_Down);
		content->append(buf);

		printf("涨跌幅最高合约[%s]-->涨跌幅[%.2f%%]\n",view.MaxUpDownIns,view.MaxUpDown*100);
		sprintf(buf,"涨跌幅最高合约[%s]-->涨跌幅[%.2f%%]\n",view.MaxUpDownIns,view.MaxUpDown*100);
		content->append(buf);

		printf("涨跌幅最低合约[%s]-->涨跌幅[%.2f%%]\n",view.MinUpDownIns,view.MinUpDown*100);
		sprintf(buf,"涨跌幅最低合约[%s]-->涨跌幅[%.2f%%]\n",view.MinUpDownIns,view.MinUpDown*100);
		content->append(buf);
		content->append("\n");
	}
	else {
		content->append("--------整体行情汇总-------------\n");
		sprintf(buf,"整体行情汇总-->无数据!!!! \n");
		content->append(buf);
		content->append("\n");
	}



}
void Report::WriteSectionViewReport(std::string * content){
	char buf[4096];
	printf("Report::WriteSectionViewReport    --------版块行情汇总-------------  \n");
	content->append("--------版块行情汇总-------------\n");

	int sec_count=0;
	MysqlSection mysqlsection;
	vector<std::string>  sectionname_lists;
	vector<SectionInfo>  section_list;
	mysqlsection.Find_SectionLists(sectionname_lists);
	for (auto &item:sectionname_lists){

		if(m_redissection.Exists(item)){

			printf("Report::WriteSectionViewReport    --------版块行-----%s  \n",item.c_str());
			SectionInfo section;
			section=m_redissection.GetSection(item);

			if(section.CountUprate>0.7&& section.CountUprate<=1){
				printf("%s  [%s]板块整体上涨----------[%.2f%%]\n",m_tradingday.c_str(),
										section.SectionName,section.CountUprate*100);
				sprintf(buf,"[%s]整体上涨----------[%.2f%%]\n",
						section.SectionName,section.CountUprate*100);
				content->append(buf);
				printf("[%s]  %s涨幅最高----------[%.2f%%]\n",section.SectionName,section.MaxInstrumentID,section.Maxrate*100);
				sprintf(buf,"[%s]  涨幅最高%s-->[%.2f%%]\n",section.SectionName,section.MaxInstrumentID,section.Maxrate*100);
				content->append(buf);
				sec_count++;
			}
			else if(section.CountDownrate>0.7&& section.CountDownrate<=1)	{
				printf("%s [%s]板块整体下跌----------[%.2f%%]\n",m_tradingday.c_str(),
										section.SectionName,section.CountDownrate*100);
				sprintf(buf,"[%s]整体下跌----------[%.2f%%]\n",section.SectionName,section.CountDownrate*100);
				content->append(buf);
				printf("[%s]板块  %s跌幅最低----------[%.2f%%]\n",section.SectionName,section.MinInstrumentID,section.Minrate*100);
				sprintf(buf,"[%s]  跌幅最低%s-->[%.2f%%]\n",section.SectionName,section.MinInstrumentID,section.Minrate*100);
				content->append(buf);
				sec_count++;
			}
			else{
				printf("%s [%s]板块整体不明晰----------[%.2f%%]   [%.2f%%] \n",m_tradingday.c_str(),
				section.SectionName,section.CountUprate*100,section.CountDownrate*100);
				sprintf(buf,"[%s]板块整体不明晰-->[%.2f%%]   [%.2f%%] \n",
								section.SectionName,section.CountUprate*100,section.CountDownrate*100);
				content->append(buf);
			}
		}
	}

	if(sec_count==0){
		content->append("   当日无板块出现联动行情! \n");
			content->append("\n");
	}

	vector<std::string>().swap(sectionname_lists);
	vector<SectionInfo>().swap(section_list);
	content->append("\n");

}
void Report::WriteSectionSedmentaryViewReport(std::string * content){
	char buf[4096];
	printf("Report::WriteSectionSedmentaryViewReport    --------版块资金流汇总-------------  \n");
	content->append("--------版块资金流汇总-------------\n");

	int sec_count=0;
	MysqlSection mysqlsection;
	vector<std::string>  sectionname_lists;
	vector<SectionInfo>  section_list;
	mysqlsection.Find_SectionLists(sectionname_lists);
	for (auto &item:sectionname_lists){

		if(m_redissection.Exists(item)){

			printf("Report::WriteSectionSedmentaryViewReport    --------版块行-----%s  \n",item.c_str());
			SectionInfo section;
			section=m_redissection.GetSection(item);

			printf("[%s]  %.2f亿-->比例[%.2f%%]",section.SectionName,section.SedimentaryIO,section.SedimentaryIORate*100);
			sprintf(buf,"[%s]  %.2f亿-->比例[%.2f%%]\n",section.SectionName,section.SedimentaryIO,section.SedimentaryIORate*100);
			content->append(buf);
			sec_count++;


//			if(section.CountUprate>0.7&& section.CountUprate<=1){
//				printf("%s  [%s]板块整体上涨----------[%.2f%%]\n",m_tradingday.c_str(),
//										section.SectionName,section.CountUprate*100);
//				sprintf(buf,"[%s]整体上涨----------[%.2f%%]\n",
//						section.SectionName,section.CountUprate*100);
//				content->append(buf);
//				printf("[%s]  %s涨幅最高----------[%.2f%%]\n",section.SectionName,section.MaxInstrumentID,section.Maxrate*100);
//				sprintf(buf,"[%s]  涨幅最高%s-->[%.2f%%]\n",section.SectionName,section.MaxInstrumentID,section.Maxrate*100);
//				content->append(buf);
//				sec_count++;
//			}
//			else if(section.CountDownrate>0.7&& section.CountDownrate<=1)	{
//				printf("%s [%s]板块整体下跌----------[%.2f%%]\n",m_tradingday.c_str(),
//										section.SectionName,section.CountDownrate*100);
//				sprintf(buf,"[%s]整体下跌----------[%.2f%%]\n",section.SectionName,section.CountDownrate*100);
//				content->append(buf);
//				printf("[%s]板块  %s跌幅最低----------[%.2f%%]\n",section.SectionName,section.MinInstrumentID,section.Minrate*100);
//				sprintf(buf,"[%s]  跌幅最低%s-->[%.2f%%]\n",section.SectionName,section.MinInstrumentID,section.Minrate*100);
//				content->append(buf);
//				sec_count++;
//			}
//			else{
//				printf("%s [%s]板块整体不明晰----------[%.2f%%]   [%.2f%%] \n",m_tradingday.c_str(),
//				section.SectionName,section.CountUprate*100,section.CountDownrate*100);
//				sprintf(buf,"[%s]板块整体不明晰-->[%.2f%%]   [%.2f%%] \n",
//								section.SectionName,section.CountUprate*100,section.CountDownrate*100);
//				content->append(buf);
//			}
		}
	}

//	if(sec_count==0){
//		content->append("   当日无板块出现联动行情! \n");
//			content->append("\n");
//	}

	vector<std::string>().swap(sectionname_lists);
	vector<SectionInfo>().swap(section_list);
	content->append("\n");

}

void Report::WriteSignalViewReport(std::string * content){

	char buf[4096];
	printf("Report::WriteSignalViewReport    --------交易日信号汇总-------------  \n");
	content->append("--------交易日信号汇总-------------\n");

	MysqlSignal mysqlsignal(m_userid.c_str());

	if(!mysqlsignal.Exist_Table() ){
		mysqlsignal.CreateTable();
		printf("	Report::WriteSignalViewReport 创建数据表  MysqlSignal !!!\n");
	}


	vector<SignalAssessData>  lists;
	mysqlsignal.Find_AllListbyTradingDay(lists,m_tradingday.c_str());
	for (auto &item:lists){
			printf("Time[%s]  有信号板块数:%d   总信号数:%d  多头信号数:%d  空头信号数:%d  \n",item.Time,
					item.Total_SingleDirection_SectionNumber,item.Total_SingleDirection_SignalNumber,
					item.Single_Long_SignalNumber,item.Single_Short_SignalNumber);

			if(item.Single_Long_SignalNumber>0 && item.Single_Short_SignalNumber==0){
				sprintf(buf,"Time[%s]  有信号板块数:%d   总信号数:%d  多头信号数:%d  \n",item.Time,
								item.Total_SingleDirection_SectionNumber,item.Total_SingleDirection_SignalNumber,
				item.Single_Long_SignalNumber);
				content->append(buf);
				sprintf(buf,"          多头信号最多[%d]板块:%s   信号最多板块合约:%s \n",item.High_Long_SignalNumber,
						item.High_Long_Section,item.High_Long_InstrumentID);
				content->append(buf);

			}
			else if(item.Single_Long_SignalNumber==0 && item.Single_Short_SignalNumber>0){
				sprintf(buf,"Time[%s]  有信号板块数:%d   总信号数:%d  空头信号数:%d  \n",item.Time,
									item.Total_SingleDirection_SectionNumber,item.Total_SingleDirection_SignalNumber,
									item.Single_Short_SignalNumber);
				content->append(buf);
				sprintf(buf,"          空头信号最多[%d]板块:%s   信号最多板块合约:%s \n",item.High_Long_SignalNumber,
						item.High_Short_Section,item.High_Short_InstrumentID);
				content->append(buf);

			}
			else{
				sprintf(buf,"Time[%s]  有信号板块数:%d   总信号数:%d  多头信号数:%d  空头信号数:%d  \n",item.Time,
									item.Total_SingleDirection_SectionNumber,item.Total_SingleDirection_SignalNumber,
									item.Single_Long_SignalNumber,item.Single_Short_SignalNumber);
				content->append(buf);
				sprintf(buf,"          多头信号最多[%d]板块:%s   信号最多板块合约:%s \n",item.High_Long_SignalNumber,
						item.High_Long_Section,item.High_Long_InstrumentID);
				content->append(buf);
				sprintf(buf,"          空头信号最多[%d]板块:%s   信号最多板块合约:%s \n",item.High_Short_SignalNumber,
						item.High_Short_Section,item.High_Short_InstrumentID);
				content->append(buf);
			}


			sprintf(buf,"          推荐信号:%s   推荐方向:%d \n",item.RecommandInstrumentID,item.RecommandDirection);
			content->append(buf);
	}
	vector<SignalAssessData>().swap(lists);
	content->append("\n");
}
void Report::WriteHoldOrderViewReport(std::string  &content,std::string &title)
{
	char buf[4096];
	content.append("\n");
	content.append("------------------盘后持仓列表------------------------\n");
	content.append("合约|方向|手数|开仓价|现价|止损价|浮盈|日期|时间\n");

	vector<InvestPositionData>  investlist;
	m_pdatawrapper->GetHoldingInvestList(investlist);
	double total_profit=0;
	for (auto &item:investlist)	{

		char d[10];
		memset(&d,0,10);

		HoldData holddata = this->m_pdatawrapper->FindHoldData(item.InstrumentID,item.PosiDirection);

		DepthMarket depthmarket;
		depthmarket = m_redisdepthmarket.GetDepthMarket(item.InstrumentID);

		if (item.PosiDirection==0){
			strcpy(d,"多");
			if(item.PositionProfit<0)	{	content.append(">>>");	}
			else {	content.append("+++");	}
			sprintf(buf,"%s|%s|%d|%.2f|%.2f|%.2f|￥%.2f|%s|-\n",item.InstrumentID,d,item.Position,item.OpenPriceAverage,
					depthmarket.LastPrice,holddata.StopPrice,item.PositionProfit,holddata.OpenDate);
			content.append(buf);
		}
		else{
			strcpy(d,"空");
			if(item.PositionProfit<0)	{	content.append(">>>");	}
			else {	content.append("+++");	}
			sprintf(buf,"%s|%s|%d|%.2f|%.2f|%.2f|￥%.2f|%s|-\n",item.InstrumentID,d,item.Position,item.OpenPriceAverage,
					depthmarket.LastPrice,holddata.StopPrice,item.PositionProfit,holddata.OpenDate);
			content.append(buf);
		}
	}


	content.append("\n");
	vector<InvestPositionData> ().swap(investlist);

}
void Report::WriteCloseOrderViewReport(std::string &content,std::string &title){
	char buf[4096];
	content.append("当日平仓合约-----------------------------------------\n");
	content.append("平仓合约名称|方向|手数|开仓价|   |平仓价|浮盈|日期|开仓时间|平仓时间\n");

	vector<InvestPositionData>  investlist;
	m_pdatawrapper->GetCloseInvestList(investlist);
	double total_profit=0;
	for (auto &item:investlist)	{

		char ProductID[31];
		CodeUtil codeutil;
		strcpy(ProductID,codeutil.GetProductID(item.InstrumentID).c_str());
		std::shared_ptr<Commission>  commssion=m_pdatawrapper->FindCommissionData(ProductID);
		double m=(double)commssion->VolumeMultiple*(double)item.CloseVolume;
		double close =item.CloseAmount/m;

		if (item.PosiDirection==2){
			if(item.CloseProfit<0)	{	content.append(">>>");	}
			else {	content.append("+++");	}
			sprintf(buf,"%s|%s|%d|%.2f|-|%.2f|￥%.2f|-|-\n",item.InstrumentID,"多",item.Position,item.OpenPriceAverage,
					close,item.CloseProfit);
			content.append(buf);
		}
		else{
			if(item.CloseProfit<0)	{	content.append(">>>");	}
			else {	content.append("+++");	}
			sprintf(buf,"%s|%s|%d|%.2f|-|%.2f|￥%.2f|-|-\n",item.InstrumentID,"空",item.Position,item.OpenPriceAverage,
					close,item.CloseProfit);
			content.append(buf);
		}
	}

	content.append("\n");
	vector<InvestPositionData> ().swap(investlist);





}
void Report::WriteIpTailPart(std::string * content){
	char buf[4096];

	std::string hostName;
	std::string Ip;
	std::string tradername;
	CodeUtil codeutil;
	bool ret = codeutil.GetHostInfo(hostName, Ip);
	if (true == ret) {
//		std::cout << "hostname: " << hostName << std::endl;
//		std::cout << "Ip: " << Ip << std::endl;
		tradername = hostName;
		tradername.append("  \n");
		content->append(tradername);

	}
	else{
		content->append("autotrader   \n");
	}
	DateUtil dateutil;
	sprintf(buf,"%s \n",dateutil.GetDateTimeString().c_str());
	content->append(buf);
}
void Report::WriteReportStaticsPart(int n,std::string * content)
{
	DateUtil dateutil;
	char buf[4096];


	std::string today_sqltime = dateutil.ConvertDatetoSqlTime(dateutil.GetCurrentDayString().c_str());
	time_t tm_today = dateutil.ConvertSqlTimetoTimet(today_sqltime.c_str());
	time_t tm_lastweek =tm_today -60*60*24*n;
	std::string lastweek_sqltime;
	lastweek_sqltime =dateutil.ConvertTimetoSqlString(tm_lastweek);
	std::string str_lastweek = dateutil.GetDateStringByTimet(&tm_lastweek);

	printf("Report::SendNDaysReport   ---------->报告起止日期  %s   ---> %s \n",lastweek_sqltime.c_str(),str_lastweek.c_str());


	Config config(m_strategyname.c_str());
	MysqlOrder mysqlopenorder("opentrade",m_userid.c_str());
	MysqlOrder mysqlcloseorder("closetrade",m_userid.c_str());
	vector<OrderData>  sendlists;

	mysqlopenorder.Find_ListsByDate(sendlists,str_lastweek.c_str());  // 有发送的开仓单
	MysqlInstrument mysqlinstrument;
	double totalprofit =0.0;
	double totalprofit_win =0.0;
	double totalprofit_lost =0.0;
	int tradecount=0;
	int win_tradecount=0;
	int lost_tradecount=0;

	for (auto &item:sendlists)  {  // 逐笔比对

		printf("Report  ----------> %s   ---> %s  %s   %s  \n",item.InstrumentID,item.TradeDate,item.TradingDay,item.TradeTime);
		sprintf(buf,"开:%s->%d  %d [%.2f]  %s  %s   %s\n",item.InstrumentID,item.Direction,item.Volume,item.Price,
				item.TradeDate,item.TradeTime,item.TradingDay);

		content->append(buf);

		int ret=mysqlcloseorder.Exist_CloseOrderbyInsTradingDay(item.InstrumentID,item.TradingDay);
		if(ret>0){
			printf("Report  ----------> %s  --> 存在平仓单\n",item.InstrumentID);

			OrderData closedata = mysqlcloseorder.Find_CloseOrderbyInsTradingDay(item.InstrumentID,item.TradingDay);
			InstrumentData instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
			double profit ;
			if(item.Direction==0){
				profit = (closedata.Price  - item.Price) *(double)instrumentdata.VolumeMultiple*(double)item.Volume;
			}
			else if (item.Direction==1){
				profit = (item.Price-closedata.Price) *(double)instrumentdata.VolumeMultiple*(double)item.Volume;
			}
			else {

			}
			tradecount++;
			if(profit>0){
				win_tradecount++;
				totalprofit_win+=profit;
			}
			else{
				lost_tradecount++;
				totalprofit_lost+=profit;
			}
			totalprofit+=profit;
			sprintf(buf,"平:%s->%d %d [%.2f]  %s %s   %.2f\n",item.InstrumentID,closedata.Direction,closedata.Volume,closedata.Price,
					closedata.TradeDate,closedata.TradeTime,profit);
			content->append(buf);
		}
		else{
			sprintf(buf,"持:%s-> [...] %s %s   %s\n",item.InstrumentID,item.TradeDate,item.TradeTime,item.TradingDay);
			content->append(buf);
		}

	}
	content->append(" \n");

	sprintf(buf,"总:------->总交易笔数:%d  胜率:%.2f%%  总收益： [%.2f]\n",
			tradecount,(double)win_tradecount/(double)tradecount*100,totalprofit);
	content->append(buf);

	sprintf(buf,"总:------->总盈利笔数:%d  总亏损笔数:%d  总盈利金额:%.2f 总亏损金额:%.2f \n",
				win_tradecount,lost_tradecount,totalprofit_win,totalprofit_lost);
	content->append(buf);

	double ave_win = totalprofit_win/win_tradecount;
	double ave_lost = totalprofit_lost/lost_tradecount;
	sprintf(buf,"总:------->平均单笔盈利金额:%.2f 平均单笔亏损金额:%.2f  平均盈亏比：%.2f \n",
			ave_win,ave_lost,ave_win/ave_lost);
	content->append(buf);

	content->append(" \n");

	vector<OrderData>().swap(sendlists);
}
void Report::SendEndTimeReport()
{
	printf("Report::SendEndReport   ---------->Begin   \n");

	this->GetLogUtil()->WriteLog("SendEndReport-----------------发送持仓报告邮件!!!!!!");

//	MysqlDepthMarket mysqldepthmarket;

	char buf[4096];
	DateUtil dateutil;
	std::string title =m_userid.c_str();
	title.append("_");
	title.append(dateutil.GetTodayString());
	title.append("_");

	std::string content ="hi: \n";

	this->WriteGlobalViewReport(&content);
//	this->WriteSectionViewReport(&content);
//	this->WriteSectionSedmentaryViewReport(&content);
//	this->WriteSignalViewReport(&content);

	std::shared_ptr<AssetData> data_asset = this->m_pdatawrapper->FindAssetData(m_userid.c_str());

	int total_openvolume = this->m_pdatawrapper->GetDetailPosition();
	int total_closevolume = this->m_pdatawrapper->CollectDetailCloseVolume_Total();

	if( total_openvolume==0&& total_closevolume==0){
		title.append("报告(空仓)");
	}
	else if(total_openvolume==0&& total_closevolume>0){
		title.append("报告(全平)");
	}
	else if(total_openvolume>0&& total_closevolume==0){
		title.append("报告(持仓,无平仓)");
	}
	else if(total_openvolume>0&& total_closevolume>0){
		title.append("报告(持仓,有平仓)");
	}
	else{
		title.append("报告(???)");
	}

	if(total_openvolume>0){
		this->WriteHoldOrderViewReport(content,title);

		if(data_asset!=nullptr){
			sprintf(buf,"当前持仓盈利      ----------  [￥%.2f]\n",data_asset->PositionProfit);
			content.append(buf);
			sprintf(buf,"当日占用保证金占用 ----------  [￥%.2f]\n",data_asset->CurrMargin);
			content.append(buf);
			sprintf(buf,"当日持仓仓位百分比 ----------  [%.2f%%]\n",(data_asset->CurrMargin/data_asset->DynamicEquity)*100);
			content.append(buf);
		}
	}

	if(total_closevolume>0){
		// 计算平仓操作报告
		this->WriteCloseOrderViewReport(content,title);
		if(data_asset!=nullptr){
			sprintf(buf,"当日平仓盈利      ----------  [￥%.2f]\n",data_asset->CloseProfit);
			content.append(buf);
			sprintf(buf,"当日佣金总计      ----------  [￥%.2f]\n",data_asset->Commission);
			content.append(buf);
		}

	}
	if(data_asset!=nullptr){
		sprintf(buf,"%s 动态权益----------[￥%.2f]\n",m_tradingday.c_str(),data_asset->DynamicEquity);
		content.append(buf);
	}


	content.append("\n");
	content.append("注:  >>>   标示的合约为亏损合约,请关注!!! \n");

	this->WriteIpTailPart(&content);


	this->m_smtputil->SendMail_Content(title,content);
	printf("Report::SendEndTimeReport   ---------->End   \n");

}
void Report::SendEndReport()
{
	printf("Report::SendEndTimeReport   ---------->Begin   \n");

//	this->GetLogUtil()->WriteLog("SendEndTimeReport-----------------发送持仓报告邮件!!!!!!");

//	this->GetTrader()->ReqQryInvestorPosition();   // 后期加入时间判断 减少请求次数
	MysqlDepthMarket mysqldepthmarket;
//	MysqlTradingDay mysqltradingday;
//	TradingDayData tradingdaydata=this->GetTrader()->GetTradingDayData();
//	tradingdaydata= mysqltradingday.Find_LastTradingDay();

	MysqlAsset mysqlasset(m_userid.c_str());
	AssetData assetdata;
	assetdata= mysqlasset.Find_LastDayAsset();

	double staticequity = assetdata.PreBalance;
	double dynamicequity ;
	double holdpositionrate;

	char buf[4096];
	DateUtil dateutil;
	std::string title =m_userid.c_str();
	title.append("_");
	title.append(dateutil.GetTodayString());
	title.append("_");

	std::string content ="hi: \n";

	this->WriteGlobalViewReport(&content);
	this->WriteSectionViewReport(&content);
	this->WriteSectionSedmentaryViewReport(&content);
	this->WriteSignalViewReport(&content);

	content.append("\n");
	content.append("盘后的持仓列表----------------------------------------\n");

	sprintf(buf,"%s 静态权益----------[￥%.2f]\n",m_tradingday.c_str(),staticequity);
		content.append(buf);
	content.append("------------------------------------------------------\n");
	content.append("合约|方向|手数|开仓价|现价|止损价|浮盈|日期|时间\n");


	vector<HoldData>  list;

	m_pdatawrapper->GetHoldList(list);
	double total_profit=0;
	for (auto &item:list)	{

		char d[10];
		memset(&d,0,10);

		DepthMarket depthmarket;
		depthmarket = m_redisdepthmarket.GetDepthMarket(item.InstrumentID);
//			DepthMarketData depthmarket;
//			depthmarket =mysqldepthmarket.Find_DatabyInstrumentID(item.InstrumentID);

		Tech tech;
		if(tech.ValidCheck_DepthMarket(&depthmarket)){

			Config config(m_strategyname.c_str());
			MysqlOrder mysqlopenorder(config.GetTable_Open().c_str(),m_userid.c_str());

			OrderData orderdata=mysqlopenorder.Find_OrderbyInsTradingday(item.InstrumentID,m_tradingday.c_str());
			double profit;

			if (item.Direction==0){
				strcpy(d,"多");
				if(depthmarket.LastPrice<item.OpenPriceAverage)	{
					content.append(">>>");
				}
				else if(depthmarket.LastPrice>item.OpenPriceAverage && item.StopPrice > item.OpenPriceAverage)	{
					content.append("+++");
				}

				if (depthmarket.LastPrice >0.1  && depthmarket.LastPrice<100000000.0){
					profit = (depthmarket.LastPrice - item.OpenPriceAverage) *
											(double)depthmarket.VolumeMultiple*	(double)item.Volume;
					total_profit+=profit;
					sprintf(buf,"%s|%s|%d|%.2f|%.2f|%.2f|￥%.2f|%s|%s\n",item.InstrumentID,d,item.Volume,item.OpenPriceAverage,
									depthmarket.LastPrice,item.StopPrice,profit,item.OpenDate,orderdata.TradeTime);
					content.append(buf);
				}
				else{
					profit=0.0;
					sprintf(buf,"%s|%s|%d|%.2f|???|%.2f|￥???|%s -- %s\n",item.InstrumentID,d,item.Volume,item.OpenPriceAverage,
								item.StopPrice,item.OpenDate,item.LocalUpdateTime);

					content.append(buf);
				}
			}
			else{
				strcpy(d,"空");
				if(depthmarket.LastPrice>item.OpenPriceAverage)	{
					content.append(">>>");
				}
				if(depthmarket.LastPrice<item.OpenPriceAverage && item.StopPrice < item.OpenPriceAverage ){
					content.append("+++");
				}

				if (depthmarket.LastPrice >0.1  && depthmarket.LastPrice <100000000.0)	{
					profit = ( item.OpenPriceAverage-depthmarket.LastPrice) *
											(double)depthmarket.VolumeMultiple*	(double)item.Volume;
					total_profit+=profit;

					sprintf(buf,"%s|%s|%d|%.2f|%.2f|%.2f|￥%.2f|%s|%s\n",item.InstrumentID,d,item.Volume,item.OpenPriceAverage,
						depthmarket.LastPrice,item.StopPrice,profit,item.OpenDate,orderdata.TradeTime);
					content.append(buf);
				}
				else{
					profit=0.0;
					sprintf(buf,"%s|%s|%d|%.2f|???|%.2f|￥???|%s\n",item.InstrumentID,d,item.Volume,item.OpenPriceAverage,
								item.StopPrice,item.OpenDate);
					content.append(buf);
				}

			}

		}// depthmarket.LastPrice >0
		else{
			sprintf(buf,"      行情数据为空!!!     \n");
		}
	}

	sprintf(buf,"当日持仓历史最高浮盈----------[￥%.2f]\n",assetdata.HighestProfit);
	content.append(buf);

	MysqlInvestPosition mysqlinvestposition(m_userid.c_str());
	vector<InvestPositionData>  investlist;
	double totalusemargin=0;
	double totalholdprofit=0;
	double totalcommission=0;
	mysqlinvestposition.Find_AllLists(investlist);

	MysqlCommission mysqlcommission;
	CodeUtil codeutil;
	for (auto &item:investlist)	{
		char productid[31];
		strcpy(productid,codeutil.GetProductID(item.InstrumentID).c_str());
		Commission commission=mysqlcommission.Find_DatabyProductID(productid);
//		InstrumentData instrumentdata;
//		MysqlInstrument mysqlinstrument;
//
//		//检测对应的合约信息是否存在,不存在则请求下载
//		if(!mysqlinstrument.Exist_DatabyInstrumentID(item.InstrumentID))	{
//			this->GetTrader()->ReqQrySingleInstrument(item.InstrumentID);
//		}

//		instrumentdata = mysqlinstrument.Find_DatabyInstrumentID(item.InstrumentID);
		totalcommission+=item.Commission;
	//				printf("%s保证金占用   %.2f  \n",item.InstrumentID,item.UseMargin);
	//				totalusemargin+=item.UseMargin;

		DepthMarket depthmarket;
		depthmarket = m_redisdepthmarket.GetDepthMarket(item.InstrumentID);

		totalusemargin+=item.OpenPriceAverage * item.Position *depthmarket.VolumeMultiple*commission.Margin;
		totalholdprofit+=item.PositionProfit;


	}

	sprintf(buf,"当日持仓合约持仓浮盈 --------  [￥%.2f]\n",totalholdprofit);
	content.append(buf);

	sprintf(buf,"当前浮盈总计      ----------[￥%.2f]\n",total_profit);
	content.append(buf);

	holdpositionrate =totalusemargin/staticequity;

	if(holdpositionrate>0.5){
		sprintf(buf,"当前持仓仓位过半,请注意! 需要减仓 控制风险!\n");
		content.append(buf);
	}
	content.append("\n");


// 计算平仓操作报告
//	double total_closeprofit = this->WriteCloseOrderViewReport(content,title,(int)list.size());
//
//	dynamicequity=staticequity+totalholdprofit-totalcommission+total_closeprofit;

	std::shared_ptr<AssetData> data_asset = this->m_pdatawrapper->FindAssetData(m_userid.c_str());

	sprintf(buf,"当日占用保证金占用 ----------  [￥%.2f]\n",data_asset->CurrMargin);
	content.append(buf);
	sprintf(buf,"当日持仓仓位百分比 ----------  [%.2f%%]\n",(data_asset->CurrMargin/data_asset->DynamicEquity)*100);
	content.append(buf);
	sprintf(buf,"当日佣金总计      ----------  [￥%.2f]\n",data_asset->Commission);
	content.append(buf);


	sprintf(buf,"%s 动态权益----------[￥%.2f]\n",m_tradingday.c_str(),data_asset->DynamicEquity);
	content.append(buf);

	content.append("\n");
	content.append("注:  >>>   标示的合约为亏损合约,请关注!!! \n");

	this->WriteIpTailPart(&content);



	vector<HoldData> ().swap(list);
	vector<InvestPositionData> ().swap(investlist);

	this->m_smtputil->SendMail_Content(title,content);
	printf("Report::SendEndTimeReport   ---------->End   \n");

}

void Report::SendWeekEndReport()
{
	printf("Report::SendWeekEndReport   ---------->Begin   \n");

//	this->GetLogUtil()->WriteLog("SendWeekEndReport-----------------发送持仓报告邮件!!!!!!");

	char buf[4096];
	DateUtil dateutil;

	char temp[200];
	sprintf(temp,"%s-%s-周报", m_userid.c_str(),dateutil.GetTodayString().c_str());
	std::string title ;
	title=temp;

	std::string content ="hi: \n";
	this->WriteReportStaticsPart(7,&content);
	this->WriteIpTailPart(&content);

	this->m_smtputil->SendMail_Content(title,content);
	printf("Report::SendWeekEndReport   ---------->End   \n");

}
void Report::SendMonthEndReport()
{
	printf("Report::SendMonthEndReport   ---------->Begin   \n");

//	this->GetLogUtil()->WriteLog("SendMonthEndReport-----------------发送持仓报告邮件!!!!!!");
	char temp[200];
	DateUtil dateutil;
	sprintf(temp,"%s-%s-月报", m_userid.c_str(),dateutil.GetTodayString().c_str());
	std::string title ;
	title=temp;
	std::string content ="hi: \n";

	this->WriteReportStaticsPart(30,&content);
	this->WriteIpTailPart(&content);


	this->m_smtputil->SendMail_Content(title,content);
	printf("Report::SendMonthEndReport   ---------->End   \n");

}

void Report::SendNDaysReport(const char* startday)
{
	printf("Report::SendNDaysReport   ---------->Begin   \n");

//	this->GetLogUtil()->WriteLog("SendNDaysReport-----------------发送开平仓报表邮件!!!!!!");

	char buf[4096];
	DateUtil dateutil;
	string startday_sqltime=dateutil.ConvertDatetoSqlTime(startday);
	time_t tm_startday = dateutil.ConvertSqlTimetoTimet(startday_sqltime.c_str());

	std::string today_sqltime = dateutil.ConvertDatetoSqlTime(dateutil.GetCurrentDayString().c_str());
	time_t tm_today = dateutil.ConvertSqlTimetoTimet(today_sqltime.c_str());
	int n = (tm_today - tm_startday)/(60*60*24);

	char temp[200];
	sprintf(temp,"%s-%s-报表", m_userid.c_str(),dateutil.GetTodayString().c_str());
	std::string title ;
	title=temp;

	std::string content ="hi: \n";

	this->WriteReportStaticsPart(n,&content);
	this->WriteIpTailPart(&content);


	this->m_smtputil->SendMail_Content(title,content);
	printf("Report::SendNDaysReport   ---------->End   \n");

}
void Report::SendLastNightReport()
{

//	this->GetLogUtil()->WriteLog("Report::SendLastNightReport-----------------发送持仓报告邮件!!!!!!");

	char buf[4096];
	DateUtil dateutil;
	std::string title =m_userid.c_str();
	title.append("_");
	title.append(dateutil.GetTodayString());
	title.append("_");

	std::string content ="hi: \n";

	this->WriteGlobalViewReport(&content);
//	this->WriteSectionViewReport(&content);
//	this->WriteSectionSedmentaryViewReport(&content);
//	this->WriteSignalViewReport(&content);

	this->WriteHoldOrderViewReport(content,title);
	// 计算平仓操作报告
	this->WriteCloseOrderViewReport(content,title);

	std::shared_ptr<AssetData> data_asset = this->m_pdatawrapper->FindAssetData(m_userid.c_str());

	sprintf(buf,"当日占用保证金占用 ----------  [￥%.2f]\n",data_asset->CurrMargin);
	content.append(buf);
	sprintf(buf,"当日持仓仓位百分比 ----------  [%.2f%%]\n",(data_asset->CurrMargin/data_asset->DynamicEquity)*100);
	content.append(buf);
	sprintf(buf,"当日佣金总计      ----------  [￥%.2f]\n",data_asset->Commission);
	content.append(buf);


	sprintf(buf,"%s 动态权益----------[￥%.2f]\n",m_tradingday.c_str(),data_asset->DynamicEquity);
	content.append(buf);

	content.append("\n");
	content.append("注:  >>>   标示的合约为亏损合约,请关注!!! \n");

	this->WriteIpTailPart(&content);

	this->m_smtputil->SendMail_Content(title,content);

	printf("Report::SendLastNightReport ----->End!   \n");

}
void Report::SendSelfCheckReport()
{

//	TradingDayData tradingdaydata=this->GetTrader()->GetTradingDayData();
	DateUtil dateutil;
	char buf[200];
	std::string title =dateutil.GetTodayString();
	title.append("_");
	title.append(m_userid.c_str());
	title.append("_");

	std::string content ="hi: \n";
	content.append(" \n");

	MysqlInstrument mysqlinstrument;
	std::string instrument_update = mysqlinstrument.GetLastLocalUpdateTime();

	MysqlDepthMarket mysqldepthmarket;
	std::string depthmarket_update  = mysqldepthmarket.GetLastLocalUpdateTime();

	MysqlMaincontract mysqlmaincontract;
	std::string maincontract_localupdate = mysqlmaincontract.GetLastLocalUpdateTime();

	sprintf(buf,"1.Instrument 更新日期 [%s] \n",instrument_update.c_str());
	content.append(buf);


	sprintf(buf,"2.Instrument 条目数 [%d] \n",mysqlinstrument.Count_AllLists());
	content.append(buf);

	sprintf(buf,"3.Depthmarket 更新日期 [%s] \n",depthmarket_update.c_str());
	content.append(buf);

	sprintf(buf,"4.Depthmarket 条目数 [%d] \n",mysqldepthmarket.Count_AllLists());
	content.append(buf);

	sprintf(buf,"5.MainContract 更新日期  [%s] \n",maincontract_localupdate.c_str());
	content.append(buf);

	sprintf(buf,"6.MainContract 更新日期 条目数  [%d] \n",mysqlmaincontract.Count_AllLists());
	content.append(buf);

	sprintf(buf,"7.交易日  [%s] \n",m_tradingday.c_str());
	content.append(buf);

	std::string tradingsqltime ;
	DateUtil datetutil;
	tradingsqltime =dateutil.ConvertDatetoSqlTime(m_tradingday.c_str());
	time_t t_tradingday = dateutil.ConvertSqlTimetoTimet(tradingsqltime.c_str());
	time_t t_current = time(NULL);

	if(t_current < t_tradingday)	{
		title.append("-盘前自检报告(交易日更新)");
	}
	else	{
		title.append("-盘前自检故障(交易日不正确 )");
	}

	content.append(" \n");
	this->WriteIpTailPart(&content);

	this->m_smtputil->SendMail_Content(title,content);

}

void Report::run(){

	printf("	线程中运行--------------------> SendMainContractCheckReport  \n");

	this->SendMainContractCheckReport();
}

void Report::SendMainContractCheckReport()
{

	DateUtil dateutil;
	std::string date = dateutil.GetTodayString();

	Create_MaincontractXlsFile(this->m_maincontract_path);

//	MysqlTradingDay mysqltradingday;
//	TradingDayData tradingdaydata;
//	tradingdaydata= mysqltradingday.Find_LastTradingDay();

//	TradingDayData tradingdaydata=this->GetTrader()->GetTradingDayData();

	std::string title =m_userid.c_str();
//	title.append("-主力合约统计及盘前自检表");
	title.append("_");
	title.append(date);
	title.append("_");

//	std::string content ="hi: \n   这是当天的主力合约迁移追踪图 ，请查收\n    autotrader";
	char buf[200];

	std::string content ="hi: \n";

	content.append(" \n");



	MysqlInstrument mysqlinstrument;
	std::string instrument_update = mysqlinstrument.GetLastLocalUpdateTime();
	InstrumentData insdata=mysqlinstrument.FindLastExpireDateInstrument();

	MysqlDepthMarket mysqldepthmarket;
	std::string depthmarket_update  = mysqldepthmarket.GetLastLocalUpdateTime();

	MysqlMaincontract mysqlmaincontract;
	std::string maincontract_localupdate = mysqlmaincontract.GetLastLocalUpdateTime();

	MysqlAsset mysqlasset(m_userid.c_str());
	AssetData assetdata=mysqlasset.Find_LastDayAsset();
	sprintf(buf,"1.Instrument 更新日期 [%s] \n",instrument_update.c_str());
	content.append(buf);

	sprintf(buf,"2.Instrument 条目数 [%d] \n",mysqlinstrument.Count_AllLists());
	content.append(buf);

	sprintf(buf,"3.Instrument 最快到期合约%s---> 到期时间[%s] \n",insdata.InstrumentID,insdata.ExpireDate);
	content.append(buf);

	sprintf(buf,"4.Depthmarket 更新日期 [%s] \n",depthmarket_update.c_str());
	content.append(buf);

	sprintf(buf,"5.Depthmarket 条目数 [%d] \n",mysqldepthmarket.Count_AllLists());
	content.append(buf);

	sprintf(buf,"6.MainContract 更新日期  [%s] \n",maincontract_localupdate.c_str());
	content.append(buf);

	sprintf(buf,"7.MainContract 更新日期 条目数  [%d] \n",mysqlmaincontract.Count_AllLists());
	content.append(buf);

	sprintf(buf,"8.MainContract 主力合约日线不足合约  [%d] \n",mysqlmaincontract.CountbyMainDayLine());
	content.append(buf);
	if (mysqlmaincontract.CountbyMainDayLine()>0){
		vector<MainContractData>  list;
		mysqlmaincontract.Find_DayLineWrongLists(list,20);

		for (auto &item:list){

			sprintf(buf,"	主力不足合约--->  %s    [%d]\n",item.InstrumentID,item.DaylineCount);
			content.append(buf);
		}


		vector<MainContractData>().swap(list);
	}

	sprintf(buf,"9.MainContract 远月合约日线不足合约  [%d] \n",mysqlmaincontract.CountbyNextDayLine());
	content.append(buf);

	if (mysqlmaincontract.CountbyNextDayLine()>0){
		vector<MainContractData>  list;
		mysqlmaincontract.Find_NextDayLineWrongLists(list,20);

		for (auto &item:list){


			sprintf(buf,"	远月不足合约--->  %s    [%d]\n",item.InstrumentID_next,item.DaylineCount_next);
			content.append(buf);
		}


		vector<MainContractData>().swap(list);
	}

	sprintf(buf,"10.MainContract主力合约截止日期检查: \n");
		content.append(buf);

	MysqlDayLine mysqldayline;

	if (mysqlmaincontract.Count_AllLists()>0){
		vector<MainContractData>  list;
		mysqlmaincontract.Find_AllList(list);

		for (auto &item:list){

			bool valid_dayline= m_pdatawrapper->ValidCheck_DayLineNdays(&mysqldayline,item.InstrumentID,
					m_tradingday.c_str(),item.ExchangeID,20);
//				bool valid_dayline_next = this->GetStrategy()->ValidCheck_DayLineNdays(item.InstrumentID,
//						m_tradingday.c_str(),item.ExchangeID,20);
			if(!valid_dayline){

				MysqlDayLine mysqldayline;
				if(mysqldayline.CountbyAll(item.InstrumentID)>0){
					HxDayLineData daydata=mysqldayline.Find_LastDayLine(item.InstrumentID);
					sprintf(buf,"	%s Error->%s\n",item.InstrumentID,daydata.Time);
					content.append(buf);
				}
				else{
					sprintf(buf,"	%s 无日线数据!!!\n",item.InstrumentID);
					content.append(buf);
				}

			}
//				else{
//
//					MysqlDayLine mysqldayline;
//					HxDayLineData daydata=mysqldayline.Find_LastDayLine(item.InstrumentID);
//					sprintf(buf,"	%s OK->%s\n",item.InstrumentID,daydata.Time);
//					content.append(buf);
//				}

		}


		vector<MainContractData>().swap(list);
	}
	sprintf(buf,"11.MainContract远月合约截止日期检查: \n");
	content.append(buf);


	if (mysqlmaincontract.Count_AllLists()>0){
		vector<MainContractData>  list;
		mysqlmaincontract.Find_AllList(list);

		for (auto &item:list){


			bool valid_dayline= m_pdatawrapper->ValidCheck_DayLineNdays(&mysqldayline,item.InstrumentID_next,
					m_tradingday.c_str(),item.ExchangeID,20);
//				bool valid_dayline_next = this->GetStrategy()->ValidCheck_DayLineNdays(item.InstrumentID,
//						m_tradingday.c_str(),item.ExchangeID,20);
			if(!valid_dayline){

				MysqlDayLine mysqldayline;
				if(mysqldayline.CountbyAll(item.InstrumentID_next)>0){
					HxDayLineData daydata=mysqldayline.Find_LastDayLine(item.InstrumentID_next);
					sprintf(buf,"	%s Error->%s\n",item.InstrumentID_next,daydata.Time);
					content.append(buf);
				}
				else{
					sprintf(buf,"	%s 无日线数据!!!\n",item.InstrumentID_next);
					content.append(buf);
				}
			}
//				else{
//
//					MysqlDayLine mysqldayline;
//					HxDayLineData daydata=mysqldayline.Find_LastDayLine(item.InstrumentID_next);
//					sprintf(buf,"	%s OK->%s\n",item.InstrumentID,daydata.Time);
//					content.append(buf);
//				}

		}


		vector<MainContractData>().swap(list);
	}
	sprintf(buf,"12.TradingDay 交易日  [%s]  \n",m_tradingday.c_str());
	content.append(buf);

	sprintf(buf,"13.Asset [%s] \n",assetdata.TradingDay);
	content.append(buf);

	MysqlTradingDay mysqltradingday(m_userid.c_str());

	if(mysqltradingday.Exist_Table()){

		if(mysqltradingday.Exist_TradingDay(m_tradingday.c_str())){

			TradingDayData last;
			last= mysqltradingday.Find_DatabyTradingDay(m_tradingday.c_str());

			if (last.SettleInfoConfirmStatus){
				sprintf(buf,"14.交易日[%s]结算完成确认,时间[%s]  \n",last.ConfirmDate,last.ConfirmTime);
						content.append(buf);
			}
			else{
				sprintf(buf,"14.交易日[%s]结算未确认!!!  \n",last.ConfirmDate);
				content.append(buf);
			}
		}
		else{
			sprintf(buf,"14.交易日[%s]结算未确认-->不存在交易日数据  \n",m_tradingday.c_str());
			content.append(buf);
		}
	}
	else{
		sprintf(buf,"14.交易日[%s]结算未确认-->不存在MysqlTradingDay数据表  \n",m_tradingday.c_str());
		content.append(buf);
	}

	sprintf(buf,"15.上次结算准备金: %.2f  \n",assetdata.PreBalance);
	content.append(buf);


	sprintf(buf,"16.板块限制开仓数--------  \n");
	content.append(buf);


	MysqlSecAssess mysqlsecassess(m_userid.c_str());
	vector<SectionAssessData>  sec_list;
	mysqlsecassess.Find_OpenLists(sec_list);
	for (auto &item:sec_list){

		sprintf(buf,"	板块[%s]---> 允许开仓的合约品种数  [%d]\n",item.Section,item.ProductRiskNumber);
		content.append(buf);
	}
	vector<SectionAssessData>().swap(sec_list);


	std::string tradingsqltime ;
	DateUtil datetutil;
	tradingsqltime =dateutil.ConvertDatetoSqlTime(m_tradingday.c_str());
	time_t t_tradingday = dateutil.ConvertSqlTimetoTimet(tradingsqltime.c_str());
	time_t t_current = time(NULL);


	double Q_BarTime_2 = dateutil.GetCurrentBarTime();

	time_t current_t = time(NULL);//这句返回的只是一个时间
	tm* t= localtime(&current_t);

	if (t->tm_wday <=5 && t->tm_wday >=1)	{
		if( Q_BarTime_2>0.160000){

			if(t_current < t_tradingday)	{
					title.append("-主力合约统计,盘前自检(交易日更新)");
				}
				else	{
					title.append("-主力合约统计,盘前自检(交易日不正确 )");
				}
		}
		else{
			if(t_current > t_tradingday)	{
				title.append("-主力合约统计,盘前自检(交易日更新)");
			}
			else	{
				title.append("-主力合约统计,盘前自检(交易日不正确 )");
			}
		}
	}
	else{

		if(t_current < t_tradingday)	{
			title.append("-主力合约统计,盘前自检(交易日更新)");
		}
		else	{
			title.append("-主力合约统计,盘前自检(交易日未更新)");
		}
	}




	content.append(" \n");

	std::string hostName;
	std::string Ip;
	std::string tradername;
	CodeUtil codeutil;
	bool ret = codeutil.GetHostInfo(hostName, Ip);
	if (true == ret) {
//		std::cout << "hostname: " << hostName << std::endl;
//		std::cout << "Ip: " << Ip << std::endl;
		tradername = hostName;
		tradername.append("  \n");
		content.append(tradername);

	}
	else{
		content.append("autotrader   \n");
	}


	sprintf(buf,"%s \n",dateutil.GetDateTimeString().c_str());
	content.append(buf);


	this->m_smtputil->SendMail(title,content,this->m_maincontract_path);
}

void Report::Create_MaincontractXlsFile(std::string pathfile)
	{

		workbook wb;
		xf_t* xf = wb.xformat();
		xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
		xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
		xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
		xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

		worksheet* ws;
		ws = wb.sheet("maincontract");


		ws->colwidth(1,256*5);
		ws->colwidth(2,256*10);
		ws->colwidth(3,256*10);
		ws->colwidth(4,256*18);
		ws->colwidth(5,256*7);
		ws->colwidth(6,256*10);
		ws->colwidth(7,256*15);
		ws->colwidth(8,256*15);
		ws->colwidth(9,256*8);
		ws->colwidth(10,256*12);
		ws->colwidth(11,256*10);
		ws->colwidth(12,256*15);
		ws->colwidth(13,256*10);
		ws->colwidth(14,256*15);
		ws->colwidth(15,256*15);
		ws->colwidth(16,256*10);
		ws->colwidth(17,256*10);
		ws->colwidth(18,256*10);
		ws->colwidth(19,256*10);
		ws->colwidth(20,256*10);



		ws->label(1,1,"序号",xf);
		ws->label(1,2,"主力合约ID",xf);
		ws->label(1,3,"交易日期",xf);
		ws->label(1,4,"主力合约名称",xf);
		ws->label(1,5,"沉淀资金",xf);
		ws->label(1,6,"主力持仓量",xf);
		ws->label(1,7,"主力成交量",xf);
		ws->label(1,8,"主力最高持仓量",xf);
		ws->label(1,9,"交易所",xf);
		ws->label(1,10,"板块",xf);
		ws->label(1,11,"夜盘",xf);
		ws->label(1,12,"次月主力合约",xf);
		ws->label(1,13,"次月主力沉淀资金",xf);
		ws->label(1,14,"次月主力持仓量",xf);
		ws->label(1,15,"次月主力成交量",xf);
		ws->label(1,16,"更新日期",xf);
		ws->label(1,17,"交易合约",xf);
		ws->label(1,18,"多头保证金",xf);
		ws->label(1,19,"交易单位",xf);
		ws->label(1,20,"单手保证金",xf);

//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);

		range * _range;

		_range = ws->rangegroup(1,1,1,20);
		_range->cellcolor(CLR_GOLD);

		MysqlMaincontract mysqlmaincontract;
		vector<MainContractData>  list;
		mysqlmaincontract.Find_AllListorderbySed(list);
		int i=0;
		for (auto &item:list)
		{

		  int row=i+2;


//		  char number[10];
//		  sprintf(number,"%d",i+1);
		  ws->number(row,1,i+1,xf);
		  ws->label(row,2,item.InstrumentID,xf);
		  ws->label(row,3,item.TradingDay,xf);
		  ws->label(row,4,item.InstrumentName,xf);

		  char sedimentary[20];
		  sprintf(sedimentary,"%.1f",item.Sedimentary);
		  ws->label(row,5,sedimentary,xf);

		  char OpenInterest[20];
		  sprintf(OpenInterest,"%d",item.OpenInterest);
		  ws->label(row,6,OpenInterest,xf);

		  char Volume[20];
		  sprintf(Volume,"%d",item.Volume);
		  ws->label(row,7,Volume,xf);

		  char MaxOpenInterest[20];
		  sprintf(MaxOpenInterest,"%d",item.MaxOpenInterest);
		  ws->label(row,8,MaxOpenInterest,xf);


		  ws->label(row,9,item.ExchangeID,xf);
		  ws->label(row,10,item.Section,xf);
		  ws->label(row,11,item.DayNight,xf);
		  ws->label(row,12,item.InstrumentID_next,xf);

		  char Sedimentary_next[20];
		  sprintf(Sedimentary_next,"%.1f",item.Sedimentary_next);
		  ws->label(row,13,Sedimentary_next,xf);

		  char OpenInterest_next[20];
		  sprintf(OpenInterest_next,"%d",item.OpenInterest_next);
		  ws->label(row,14,OpenInterest_next,xf);

		  char Volume_next[20];
		  sprintf(Volume_next,"%d",item.Volume_next);
		  ws->label(row,15,Volume_next,xf);

		  ws->label(row,16,item.UpdateTime,xf);
		  ws->label(row,17,item.InstrumentID_trade,xf);

			MysqlCommission mysqlcommission;
			Commission commission=mysqlcommission.Find_DatabyProductID(item.ProductID);

		  ws->number(row,18,commission.Margin,xf);
//		  ws->number(row,19,item.ShortMarginRatio,xf);
		  ws->number(row,19,item.VolumeMultiple,xf);

		  double single_margin = 1*item.LastPrice*commission.Margin*item.VolumeMultiple;
		  ws->number(row,20,single_margin,xf);

//		  MysqlCycle mycycle;
//		  std::string s_cycledate=mycycle.GetLastDateTime(item.InstrumentID_trade);
//		  ws->label(row,18,s_cycledate.substr(0,10),xf);

		  if(strcmp(item.InstrumentID,item.InstrumentID_trade)!=0 )	{

			  	  range * range_r;

				range_r = ws->rangegroup(row,1,row,20);
				range_r->cellcolor(CLR_GOLD);   // 专用颜色标注 非主力合约
//				_range = ws->rangegroup(row,1,row,18);
//				_range->cellcolor(CLR_GOLD);
				printf("Report-Create_MaincontractXlsFile:   	!!!主力合约:%s--->远月合约[交易合约]:%s \n",
						item.InstrumentID,item.InstrumentID_trade);
			}
		  else{
			  printf("Report-Create_MaincontractXlsFile:    主力合约[交易合约]:%s--->远月合约:%s  \n",
					  item.InstrumentID,item.InstrumentID_next);
		  }



			i++;
		}


		wb.Dump(pathfile);
		vector<MainContractData> ().swap(list);
printf("Create_MaincontractXlsFile  end! ...\n");




}


void Report::Create_TestReportXlsFile(std::string pathfile)
	{

		workbook wb;
		xf_t* xf = wb.xformat();
		xf->SetBorderStyle(BORDER_BOTTOM,BORDER_THIN);
		xf->SetBorderStyle(BORDER_TOP,BORDER_THIN);
		xf->SetBorderStyle(BORDER_LEFT,BORDER_THIN);
		xf->SetBorderStyle(BORDER_RIGHT,BORDER_THIN);

		worksheet* ws;
		ws = wb.sheet("testreport");

		ws->colwidth(1,256*10);
		ws->colwidth(2,256*10);
		ws->colwidth(3,256*10);
		ws->colwidth(4,256*18);
		ws->colwidth(5,256*10);
		ws->colwidth(6,256*10);
		ws->colwidth(7,256*15);
		ws->colwidth(8,256*15);
		ws->colwidth(9,256*10);
		ws->colwidth(10,256*12);
		ws->colwidth(11,256*10);
		ws->colwidth(12,256*15);
		ws->colwidth(13,256*10);
		ws->colwidth(14,256*15);
		ws->colwidth(15,256*15);
		ws->colwidth(16,256*10);
		ws->colwidth(17,256*10);
		ws->colwidth(18,256*15);

		ws->label(1,1,"序号",xf);
		ws->label(1,2,"品种",xf);
		ws->label(1,3,"初始资金",xf);
		ws->label(1,4,"总盈利额",xf);
		ws->label(1,5,"总亏损额",xf);
		ws->label(1,6,"总交易次数",xf);
		ws->label(1,7,"盈利次数",xf);
		ws->label(1,8,"平均盈利",xf);
		ws->label(1,9,"平均亏损",xf);
		ws->label(1,10,"盈亏比",xf);
		ws->label(1,11,"胜率",xf);
		ws->label(1,12,"期望值",xf);
		ws->label(1,13,"错误数据天数",xf);

//				cell_t * cell;
//				cell = ws->label(0,0,"world");
//
//				cell->fillfgcolor(CLR_RED);
//				cell->borderstyle(0,1);

		range * _range;

		_range = ws->rangegroup(1,1,1,18);
		_range->cellcolor(CLR_GOLD);

		MysqlTestReport mysqltestreport;
		vector<TestReportData>  list;
		mysqltestreport.Find_ListsOrderbyExp(list,"v1");
		int i=0;
		for (auto &item:list)
		{

		  int row=i+2;


		  char number[10];
		  sprintf(number,"%d",i+1);
		  ws->label(row,1,number,xf);
		  ws->label(row,2,item.Product,xf);

		  char buf[20];
		  sprintf(buf,"%.2f",item.InitialMoney);
		  ws->label(row,3,buf,xf);


		  sprintf(buf,"%.2f",item.TotalWinProfit);
		  ws->label(row,4,buf,xf);

		  sprintf(buf,"%.2f",item.TotalLoseProfit);
		  ws->label(row,5,buf,xf);

		  sprintf(buf,"%d",item.TradeCount);
		  ws->label(row,6,buf,xf);

		  sprintf(buf,"%d",item.WinCount);
		  ws->label(row,7,buf,xf);

		  sprintf(buf,"%.2f",item.AverageWinProfit);
		  ws->label(row,8,buf,xf);


		  sprintf(buf,"%.2f",item.AverageLoseProfit);
		  ws->label(row,9,buf,xf);

		  sprintf(buf,"%.2f",item.WinLoseRate);
		  ws->label(row,10,buf,xf);

		  sprintf(buf,"%.2f%%",item.WinRate*100);
		  ws->label(row,11,buf,xf);

		  sprintf(buf,"%.2f",item.Expectation);
		  ws->label(row,12,buf,xf);

		  sprintf(buf,"%d",item.ErrorMinuteData);
		  ws->label(row,13,buf,xf);

		  if(item.Expectation <0 )
			{
					range * range_r;

					range_r = ws->rangegroup(row,1,row,18);
					range_r->cellcolor(CLR_GOLD);

//				_range = ws->rangegroup(row,1,row,18);
//				_range->cellcolor(CLR_GOLD);
				printf("    %s 品种期望值为负值  \n",item.Product);
			}

			i++;
		}


		wb.Dump(pathfile);
		vector<TestReportData> ().swap(list);
//						printf("MongoMainContract Find_All  end! ...\n");




}
