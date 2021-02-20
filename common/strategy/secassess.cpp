#include "secassess.hpp"


SecAssess::SecAssess() {


}
void SecAssess::SetSignalLabel(const char* label)
{
	this->_signallabel=label;
}

std::string SecAssess::GetSignalLabel()
{
	return this->_signallabel;
}
void SecAssess::SetStrategyName(const char* strategyname)
{
	this->_strategyName=strategyname;
}

std::string SecAssess::GetStrategyName()
{
	return this->_strategyName;
}
void SecAssess::SetTradingDay(const char* date)
{
	this->_tradingday=date;
}
std::string SecAssess::GetTradingDay()
{
	return this->_tradingday;
}
void SecAssess::SetSectionAcessessLists(const char* user){

	std::string logname="secassess";
	logname.append("_");
	logname.append(user);


	std::string logfilename="secassess";
	logfilename.append("_");
	logfilename.append(user);
	logfilename.append(".log");
	this->SetLogName(logname.c_str());
	this->SetLogPathFileName(logfilename.c_str());


	DateUtil dateutil;
	MysqlSecAssess mysqlsecasses(user);
	std::cout<<"user ------>"<<user<<endl;
	mysqlsecasses.Find_AllLists(_sectionlists);

	if(this->_sectionlists.size()>0)	{
		printf("SecAssess::SetSectionAcessessLists----->初始化待评估板块数[%ld] ----> %s\n",this->_sectionlists.size(),
				dateutil.GetCurrentSqlTimeString().c_str());
	}
	else{
		printf("SecAssess::SetSectionAcessessLists----->待评估板块数[0]   \n");
	}

}
vector<SectionAssessData>* SecAssess::GetSectionAcessessLists(){
	return &this->_sectionlists;
}
SectionAssessData SecAssess::GetSecAssessbySection(const char* section){

	SectionAssessData data;

	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			strcpy(data.Section,item->Section);
			data.ProductRiskNumber =item->ProductRiskNumber ;
			data.ProductHoldNumber =item->ProductHoldNumber ;
			data.PD_Long_Inst_UpdownRate =item->PD_Long_Inst_UpdownRate ;
			strcpy(data.PD_Long_InstrumentId,item->PD_Long_InstrumentId);
			data.PD_Long_SignalNumber =item->PD_Long_SignalNumber ;
			data.PD_Short_Inst_UpdownRate =item->PD_Short_Inst_UpdownRate ;
			strcpy(data.PD_Short_InstrumentId,item->PD_Short_InstrumentId);
			data.PD_Short_SignalNumber =item->PD_Short_SignalNumber ;

			return data;
		}
	}
	return data;
}
void SecAssess::GetSecAssessbySection(const char* section,SectionAssessData *data){

//	SectionAssessData data;
	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			strcpy(data->Section,item->Section);
			data->ProductRiskNumber =item->ProductRiskNumber ;
			data->ProductHoldNumber =item->ProductHoldNumber ;
			data->PD_Long_Inst_UpdownRate =item->PD_Long_Inst_UpdownRate ;
			strcpy(data->PD_Long_InstrumentId,item->PD_Long_InstrumentId);
			data->PD_Long_SignalNumber =item->PD_Long_SignalNumber ;
			data->PD_Short_Inst_UpdownRate =item->PD_Short_Inst_UpdownRate ;
			strcpy(data->PD_Short_InstrumentId,item->PD_Short_InstrumentId);
			data->PD_Short_SignalNumber =item->PD_Short_SignalNumber ;

			if(data->PD_Long_SignalNumber>0){
				data->Result_Direction=0;
				strcpy(data->Result_InstrumentId,item->PD_Long_InstrumentId);
				printf("	GetSecAssessbySection   按照指定板块更新推荐多头合约%s    \n",data->PD_Long_InstrumentId);
			}
			else if(data->PD_Short_SignalNumber>0){
				data->Result_Direction=1;
				strcpy(data->Result_InstrumentId,item->PD_Short_InstrumentId);
				printf("	GetSecAssessbySection   按照指定板块更新推荐空头合约%s  \n",data->PD_Short_InstrumentId);
			}

		}
	}

}

void SecAssess::GetSecAssessbyInstrumentID(const char* instrumentid,SectionAssessData *data,Strategy * strategy){

//	SectionAssessData data;
	char section[20];
	for (vector<MainContractData>::iterator item = strategy->GetContractLists()->begin();
				item != strategy->GetContractLists()->end(); item++){
		if(strcmp(item->InstrumentID,instrumentid)==0){
			strcpy(section,item->Section);
			printf("	GetSecAssessbyInstrumentID   发现合约%s所在板块 %s    \n",instrumentid,section);
		}
	}

	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			strcpy(data->Section,item->Section);
			data->ProductRiskNumber =item->ProductRiskNumber ;
			data->ProductHoldNumber =item->ProductHoldNumber ;
			data->PD_Long_Inst_UpdownRate =item->PD_Long_Inst_UpdownRate ;
			strcpy(data->PD_Long_InstrumentId,item->PD_Long_InstrumentId);
			data->PD_Long_SignalNumber =item->PD_Long_SignalNumber ;
			data->PD_Short_Inst_UpdownRate =item->PD_Short_Inst_UpdownRate ;
			strcpy(data->PD_Short_InstrumentId,item->PD_Short_InstrumentId);
			data->PD_Short_SignalNumber =item->PD_Short_SignalNumber ;

			if(strategy->GetGlobalViewData()->LongWeight > strategy->GetGlobalViewData()->ShortWeight){
				data->Result_Direction=0;
				strcpy(data->Result_InstrumentId,instrumentid);
				printf("	GetSecAssessbyInstrumentID   按照指定合约更新推荐多头合约%s    \n",data->Result_InstrumentId);
			}
			else if(strategy->GetGlobalViewData()->LongWeight < strategy->GetGlobalViewData()->ShortWeight){
				data->Result_Direction=1;
				strcpy(data->Result_InstrumentId,instrumentid);
				printf("	GetSecAssessbyInstrumentID   按照指定合约更新推荐空头合约%s  \n",data->Result_InstrumentId);
			}

		}
	}

}
SectionAssessData SecAssess::GetSecAssessbyPriority(vector<SectionAssessData> &list){

	SectionAssessData data;
	int priority=11;


	for (auto &item:list){

		if(item.Priority<priority){

			priority = item.Priority;
			strcpy(data.Section,item.Section);
			data.ProductRiskNumber =item.ProductRiskNumber ;
			data.ProductHoldNumber =item.ProductHoldNumber ;
			data.PD_Long_Inst_UpdownRate =item.PD_Long_Inst_UpdownRate ;
			strcpy(data.PD_Long_InstrumentId,item.PD_Long_InstrumentId);
			data.PD_Long_SignalNumber =item.PD_Long_SignalNumber ;
			data.PD_Short_Inst_UpdownRate =item.PD_Short_Inst_UpdownRate ;
			strcpy(data.PD_Short_InstrumentId,item.PD_Short_InstrumentId);
			data.PD_Short_SignalNumber =item.PD_Short_SignalNumber ;
		}

	}
	return data;
}

void SecAssess::GetSecAssessbyPriority(vector<SectionAssessData> &list,SectionAssessData * data){

//	SectionAssessData data;
	int priority=11;
	RedisSignal redissignal;
	Config config(this->GetStrategyName().c_str());

	vector<SectionAssessData> temp_list;
	for (auto &item:list){
		SectionAssessData sadata;
		strcpy(sadata.Section,item.Section);
		sadata.ProductRiskNumber =item.ProductRiskNumber ;
		sadata.ProductHoldNumber =item.ProductHoldNumber ;
		sadata.PD_Long_Inst_UpdownRate =item.PD_Long_Inst_UpdownRate ;
		strcpy(sadata.PD_Long_InstrumentId,item.PD_Long_InstrumentId);
		sadata.PD_Long_SignalNumber =item.PD_Long_SignalNumber ;
		sadata.PD_Short_Inst_UpdownRate =item.PD_Short_Inst_UpdownRate ;
		strcpy(sadata.PD_Short_InstrumentId,item.PD_Short_InstrumentId);
		sadata.PD_Short_SignalNumber =item.PD_Short_SignalNumber ;
		sadata.Priority = item.Priority;

		if(sadata.PD_Long_SignalNumber>0){
			std::string signalname;
			signalname=sadata.PD_Long_InstrumentId;
			signalname.append(this->GetSignalLabel());
			signalname.append("_");
			signalname.append(config.GetUserID().c_str());
			if (redissignal.Exists(signalname)> 0){
				Signal signal;
				signal=redissignal.GetInsSignal(signalname);
				if (signal.Volume>0){
					temp_list.push_back(sadata);
				}
				else{
					printf("	GetSecAssessbyPriority   多头合约%s  运行开仓数量不足 不参与板块优先级优选\n",sadata.PD_Long_InstrumentId);
				}
			}
		}
		else if(sadata.PD_Short_SignalNumber>0){
			std::string signalname;
			signalname=sadata.PD_Short_InstrumentId;
			signalname.append(this->GetSignalLabel());
			signalname.append("_");
			signalname.append(config.GetUserID().c_str());
			if (redissignal.Exists(signalname)> 0){
				Signal signal;
				signal=redissignal.GetInsSignal(signalname);
				if (signal.Volume>0){
					temp_list.push_back(sadata);
				}
				else{
					printf("	GetSecAssessbyPriority   空头合约%s  运行开仓数量不足 不参与板块优先级优选\n",sadata.PD_Short_InstrumentId);
				}
			}
		}


	}


	for (auto &item:temp_list){

		if(item.Priority<priority){

			priority = item.Priority;
			strcpy(data->Section,item.Section);
			data->ProductRiskNumber =item.ProductRiskNumber ;
			data->ProductHoldNumber =item.ProductHoldNumber ;
			data->PD_Long_Inst_UpdownRate =item.PD_Long_Inst_UpdownRate ;
			strcpy(data->PD_Long_InstrumentId,item.PD_Long_InstrumentId);
			data->PD_Long_SignalNumber =item.PD_Long_SignalNumber ;
			data->PD_Short_Inst_UpdownRate =item.PD_Short_Inst_UpdownRate ;
			strcpy(data->PD_Short_InstrumentId,item.PD_Short_InstrumentId);
			data->PD_Short_SignalNumber =item.PD_Short_SignalNumber ;

			if(data->PD_Long_SignalNumber>0){
				data->Result_Direction=0;
				strcpy(data->Result_InstrumentId,item.PD_Long_InstrumentId);
				printf("	GetSecAssessbyPriority   按照优先级更新推荐多头合约%s   优先级别%d \n",data->PD_Long_InstrumentId,item.Priority);
			}
			else if(data->PD_Short_SignalNumber>0){
				data->Result_Direction=1;
				strcpy(data->Result_InstrumentId,item.PD_Short_InstrumentId);
				printf("	GetSecAssessbyPriority   按照优先级更新推荐空头合约%s  优先级别%d \n",data->PD_Short_InstrumentId,item.Priority);
			}

		}

	}
	vector<SectionAssessData>().swap(temp_list);
}
void SecAssess::GetSecAssessbySedimentaryIORate(vector<SectionAssessData> &list,SectionAssessData * data){

	RedisSection redisection;
	double sedimentaryiorate=0.0;

	vector<SectionAssessData> temp_list;
	for (auto &item:list){
		SectionAssessData sadata;
		strcpy(sadata.Section,item.Section);
		sadata.ProductRiskNumber =item.ProductRiskNumber ;
		sadata.ProductHoldNumber =item.ProductHoldNumber ;
		sadata.PD_Long_Inst_UpdownRate =item.PD_Long_Inst_UpdownRate ;
		strcpy(sadata.PD_Long_InstrumentId,item.PD_Long_InstrumentId);
		sadata.PD_Long_SignalNumber =item.PD_Long_SignalNumber ;
		sadata.PD_Short_Inst_UpdownRate =item.PD_Short_Inst_UpdownRate ;
		strcpy(sadata.PD_Short_InstrumentId,item.PD_Short_InstrumentId);
		sadata.PD_Short_SignalNumber =item.PD_Short_SignalNumber ;
		sadata.Priority = item.Priority;

		if(sadata.PD_Long_SignalNumber>0){

			if (redisection.Exists(item.Section)> 0){
				SectionInfo sectioninfo;
				sectioninfo=redisection.GetSection(item.Section);
				if (sectioninfo.Sedimentary>0){
					sadata.Sedimentary = sectioninfo.Sedimentary;
					sadata.SedimentaryIO = sectioninfo.SedimentaryIO;
					sadata.SedimentaryIORate = sectioninfo.SedimentaryIORate;

					temp_list.push_back(sadata);
				}
				else{
					printf("	GetSecAssessbySedimentaryIORate   多头板块%s  沉淀资金[%.2f]不足 不符合筛选要求\n",item.Section,sectioninfo.Sedimentary);
				}
			}
		}
		else if(sadata.PD_Short_SignalNumber>0){

			if (redisection.Exists(item.Section)> 0){
				SectionInfo sectioninfo;
				sectioninfo=redisection.GetSection(item.Section);
				if (sectioninfo.Sedimentary>0){
					sadata.Sedimentary = sectioninfo.Sedimentary;
					sadata.SedimentaryIO = sectioninfo.SedimentaryIO;
					sadata.SedimentaryIORate = sectioninfo.SedimentaryIORate;
					temp_list.push_back(sadata);
				}
				else{
					printf("	GetSecAssessbySedimentaryIORate   空头板块%s  沉淀资金[%.2f]不足 不符合筛选要求\n",item.Section,sectioninfo.Sedimentary);
				}
			}
		}


	}


	for (auto &item:temp_list){

		if(sedimentaryiorate==0.0){

			sedimentaryiorate = item.SedimentaryIORate;
			strcpy(data->Section,item.Section);
			data->ProductRiskNumber =item.ProductRiskNumber ;
			data->ProductHoldNumber =item.ProductHoldNumber ;
			data->PD_Long_Inst_UpdownRate =item.PD_Long_Inst_UpdownRate ;
			strcpy(data->PD_Long_InstrumentId,item.PD_Long_InstrumentId);
			data->PD_Long_SignalNumber =item.PD_Long_SignalNumber ;
			data->PD_Short_Inst_UpdownRate =item.PD_Short_Inst_UpdownRate ;
			strcpy(data->PD_Short_InstrumentId,item.PD_Short_InstrumentId);
			data->PD_Short_SignalNumber =item.PD_Short_SignalNumber ;

			data->Sedimentary = item.Sedimentary;
			data->SedimentaryIO = item.SedimentaryIO;
			data->SedimentaryIORate = item.SedimentaryIORate;

			if(data->PD_Long_SignalNumber>0){
				data->Result_Direction=0;
				strcpy(data->Result_InstrumentId,item.PD_Long_InstrumentId);
				printf("	GetSecAssessbySedimentaryIORate   按照资金流入流出比例更新推荐多头合约%s   资金流入流出比例: %.2f%% \n",data->PD_Long_InstrumentId,item.SedimentaryIORate*100);
			}
			else if(data->PD_Short_SignalNumber>0){
				data->Result_Direction=1;
				strcpy(data->Result_InstrumentId,item.PD_Short_InstrumentId);
				printf("	GetSecAssessbySedimentaryIORate   按照资金流入流出比例更新推荐空头合约%s  资金流入流出比例: %.2f%% \n",data->PD_Short_InstrumentId,item.SedimentaryIORate*100);
			}
		}
		else if(sedimentaryiorate < fabs(item.SedimentaryIORate)){

			sedimentaryiorate = fabs(item.SedimentaryIORate);
			strcpy(data->Section,item.Section);
			data->ProductRiskNumber =item.ProductRiskNumber ;
			data->ProductHoldNumber =item.ProductHoldNumber ;
			data->PD_Long_Inst_UpdownRate =item.PD_Long_Inst_UpdownRate ;
			strcpy(data->PD_Long_InstrumentId,item.PD_Long_InstrumentId);
			data->PD_Long_SignalNumber =item.PD_Long_SignalNumber ;
			data->PD_Short_Inst_UpdownRate =item.PD_Short_Inst_UpdownRate ;
			strcpy(data->PD_Short_InstrumentId,item.PD_Short_InstrumentId);
			data->PD_Short_SignalNumber =item.PD_Short_SignalNumber ;
			data->Sedimentary = item.Sedimentary;
			data->SedimentaryIO = item.SedimentaryIO;
			data->SedimentaryIORate = item.SedimentaryIORate;

			if(data->PD_Long_SignalNumber>0){
				data->Result_Direction=0;
				strcpy(data->Result_InstrumentId,item.PD_Long_InstrumentId);
				printf("	GetSecAssessbySedimentaryIORate   按照资金流入流出比例更新推荐多头合约%s   资金流入流出比例: %.2f%% \n",data->PD_Long_InstrumentId,item.SedimentaryIORate*100);
			}
			else if(data->PD_Short_SignalNumber>0){
				data->Result_Direction=1;
				strcpy(data->Result_InstrumentId,item.PD_Short_InstrumentId);
				printf("	GetSecAssessbySedimentaryIORate   按照资金流入流出比例更新推荐空头合约%s  资金流入流出比例: %.2f%% \n",data->PD_Short_InstrumentId,item.SedimentaryIORate*100);
			}

		}

	}
	vector<SectionAssessData>().swap(temp_list);
}
void SecAssess::UpdateSection_ProductHoldNumber(const char* section,int num){
	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			item->ProductHoldNumber = num;
		}
	}
}
void SecAssess::UpdateSection_PD_Long(const char* section,SectionAssessData data){
	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			item->PD_Long_SignalNumber = data.PD_Long_SignalNumber;
			item->PD_Long_Inst_UpdownRate = data.PD_Long_Inst_UpdownRate;
			strcpy(item->PD_Long_InstrumentId,data.PD_Long_InstrumentId);
		}
	}
}
void SecAssess::UpdateSection_PD_Short(const char* section,SectionAssessData data){
	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			item->PD_Short_SignalNumber = data.PD_Short_SignalNumber;
			item->PD_Short_Inst_UpdownRate = data.PD_Short_Inst_UpdownRate;
			strcpy(item->PD_Short_InstrumentId,data.PD_Short_InstrumentId);
		}
	}
}
void SecAssess::UpdateSection_PD_Long_SignalNumber(const char* section,int num){
	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			item->PD_Long_SignalNumber = num;
		}
	}
}
void SecAssess::UpdateSection_PD_Short_SignalNumber(const char* section,int num){
	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){
		if(strcmp(item->Section,section)==0){
			item->PD_Short_SignalNumber = num;
		}
	}
}
void SecAssess::Reset(){

	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
						item != this->GetSectionAcessessLists()->end(); item++){

		if(item->ProductHoldNumber>0 ||item->PD_Long_SignalNumber>0 || item->PD_Short_SignalNumber>0)
			printf("	SecAssess::Reset 前----->板块%s 持仓品种数  %d 多头信号  %d  空头信号  %d \n",item->Section,
					item->ProductHoldNumber,item->PD_Long_SignalNumber,item->PD_Short_SignalNumber);

		}

	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
				item != this->GetSectionAcessessLists()->end(); item++){
		if(item->ProductHoldNumber>0){
			item->ProductHoldNumber=0;
			printf("	SecAssess::Reset----->板块%s 清空持仓品种数  %d \n",item->Section,item->ProductHoldNumber);
		}

		if(item->PD_Long_SignalNumber>0){
			item->PD_Long_SignalNumber=0;
			printf("	SecAssess::Reset----->板块%s 清空多头信号  %d \n",item->Section,item->PD_Long_SignalNumber);
		}

		if(item->PD_Short_SignalNumber>0){
			item->PD_Short_SignalNumber=0;
			printf("	SecAssess::Reset----->板块%s 清空空头信号  %d \n",item->Section,item->PD_Short_SignalNumber);
		}
	}

	for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
					item != this->GetSectionAcessessLists()->end(); item++){

		if(item->ProductHoldNumber>0 ||item->PD_Long_SignalNumber>0 || item->PD_Short_SignalNumber>0)		{
			printf("	SecAssess::Reset 后----->板块%s 持仓品种数  %d 多头信号  %d  空头信号  %d \n",item->Section,
							item->ProductHoldNumber,item->PD_Long_SignalNumber,item->PD_Long_SignalNumber);
		}


	}
}
void SecAssess::SectionAssess(Strategy * strategy,const char* opensignal)
{
	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());
	printf("SecAssess::SectionAssess----->   板块级别信号评估 begin!!!  \n");

	// 多空信号数 归零

	this->Reset();

	//更新当日的信号统计数目
	DateUtil dateutil;
	printf("	SecAssess::SectionAssess----->板块级别信号评估 主力合约品种数[%ld] ----> %s\n",strategy->GetContractLists()->size(),
			dateutil.ConvertSHFETimetoSqlTime(strategy->GetDifSec()).c_str());

	for (vector<MainContractData>::iterator item = strategy->GetContractLists()->begin();
			item != strategy->GetContractLists()->end(); item++){

		std::string signalname;
		signalname=item->InstrumentID;
		signalname.append(opensignal);
		signalname.append("_");
		signalname.append(strategy->GetUserID().c_str());
		bool existsignal = strategy->GetRedisSignal()->Exists(signalname)> 0;

		if(existsignal){
//			printf("Strategy::SectionAssess----->  板块%s  :存在信号   [%s]   \n",item->Section,signalname.c_str());
			Signal signal;
			signal=strategy->GetRedisSignal()->GetInsSignal(signalname);
			if (signal.SingalActive ){
				printf("	SecAssess::SectionAssess----->板块%s  :存在激活信号   [%s]   \n",item->Section,signalname.c_str());
				SectionAssessData data=this->GetSecAssessbySection(item->Section);

				if(signal.Direction==0){
					data.PD_Long_SignalNumber++;
					this->UpdateSection_PD_Long_SignalNumber(item->Section,data.PD_Long_SignalNumber);
					//该板块第一个多头信号 无须比对涨幅最高合约 直接录入即可
					if(data.PD_Long_SignalNumber==1){
						strcpy(data.PD_Long_InstrumentId,signal.InstrumentID);
						DepthMarket depthmarket;
						if( strategy->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket = strategy->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}
						data.PD_Long_Inst_UpdownRate=depthmarket.UpdownRate;
						this->UpdateSection_PD_Long(item->Section,data);
						printf("	SecAssess::SectionAssess----->板块%s 存在激活多头信号  %s \n",item->Section,signal.SingalName);
						sprintf(logbuf,"	SecAssess::SectionAssess----->板块%s 存在激活多头信号  %s \n",item->Section,signal.SingalName);
						logutil.WriteLog(logbuf);
					}
					else{   // 该板块出现多个多头信号  则需要比对再更新涨跌幅最高合约

						DepthMarket depthmarket_last;
						if( strategy->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket_last = strategy->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}
						sprintf(logbuf,"	SecAssess::SectionAssess----->板块%s 多个激活多头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
									data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
						logutil.WriteLog(logbuf);
						if(depthmarket_last.UpdownRate > data.PD_Long_Inst_UpdownRate){

							printf("	SecAssess::SectionAssess----->板块%s 更新优选激活多头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
									data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
							sprintf(logbuf,"	SecAssess::SectionAssess----->板块%s 更新优选激活多头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
										data.PD_Long_InstrumentId,data.PD_Long_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
							logutil.WriteLog(logbuf);
							strcpy(data.PD_Long_InstrumentId,signal.InstrumentID);
							data.PD_Long_Inst_UpdownRate=depthmarket_last.UpdownRate;
							this->UpdateSection_PD_Long(item->Section,data);
						}
					}

				}

				if(signal.Direction==1){
					data.PD_Short_SignalNumber++;
					this->UpdateSection_PD_Long_SignalNumber(item->Section,data.PD_Long_SignalNumber);
					if(data.PD_Short_SignalNumber==1){
						strcpy(data.PD_Short_InstrumentId,signal.InstrumentID);
						DepthMarket depthmarket;
						if( strategy->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket = strategy->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}
						data.PD_Short_Inst_UpdownRate=depthmarket.UpdownRate;
						this->UpdateSection_PD_Short(item->Section,data);
						printf("	SecAssess::SectionAssess----->板块%s 存在激活空头信号>>> %s \n",item->Section,signal.SingalName);
						sprintf(logbuf,"	SecAssess::SectionAssess----->板块%s 存在激活空头信号>>> %s \n",item->Section,signal.SingalName);
						logutil.WriteLog(logbuf);
					}
					else{
						DepthMarket depthmarket_last;
						if( strategy->GetRedisDepthMarket()->Exists(signal.InstrumentID)> 0 ){
							depthmarket_last = strategy->GetRedisDepthMarket()->GetDepthMarket(signal.InstrumentID);
						}

						sprintf(logbuf,"	SecAssess::SectionAssess----->板块%s 多个激活空头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
									data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
						logutil.WriteLog(logbuf);
						if(depthmarket_last.UpdownRate < data.PD_Short_Inst_UpdownRate){

							printf("	SecAssess::SectionAssess----->板块%s 更新优选激活空头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
									data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
							sprintf(logbuf,"	SecAssess::SectionAssess----->板块%s 更新优选激活空头信号>>>  %s[%.3f] --> %s[%.3f]\n",item->Section,
										data.PD_Short_InstrumentId,data.PD_Short_Inst_UpdownRate,signal.InstrumentID,depthmarket_last.UpdownRate);
							logutil.WriteLog(logbuf);
							strcpy(data.PD_Short_InstrumentId,signal.InstrumentID);
							data.PD_Short_Inst_UpdownRate=depthmarket_last.UpdownRate;
							this->UpdateSection_PD_Short(item->Section,data);
						}
					}
				}

			}
			else{
//				if(signal.Direction==0){
//					printf("SecAssess::SectionAssess----->  板块%s  :存在未激活多头信号   [%s]   \n",item->Section,signalname.c_str());
//				}
//				if(signal.Direction==1){
//					printf("SecAssess::SectionAssess----->  板块%s  :存在未激活空头信号   [%s]   \n",item->Section,signalname.c_str());
//				}
			}
		}
		else{
//			printf("SecAssess::SectionAssess----->  无此信号   [%s]   \n",signalname.c_str());
		}


	}

	//评估多头信号列表  多头信号数超过阈值

	//评估空头信号列表 空头信号数超过阈值


	MysqlHold mysqlhold(strategy->GetUserID().c_str());
	vector<HoldData>  holdlist;
	mysqlhold.Find_AllTradingDayList(holdlist,strategy->GetTradingDay().c_str());
	if(holdlist.size()>0)	{
		printf("	SecAssess::SectionAssess-----> ----->当日持仓批次[%ld] %s\n",holdlist.size(),
				dateutil.ConvertSHFETimetoSqlTime(strategy->GetDifSec()).c_str());
	}

	CodeUtil codeutil;
	for (auto &item:holdlist)	{

		std::string str_section = codeutil.GetSectionByInstrumentid(item.InstrumentID);

		SectionAssessData data=this->GetSecAssessbySection(str_section.c_str());
		data.ProductHoldNumber++;
		this->UpdateSection_ProductHoldNumber(str_section.c_str(),data.ProductHoldNumber);
		printf("	SecAssess::SectionAssess----->  板块%s  :更新持仓合约数 %s   [%d]   \n",str_section.c_str(),
				item.InstrumentID,data.ProductHoldNumber);
	}

	vector<HoldData>().swap(holdlist);

}
void SecAssess::FilterSectionList(vector<SectionAssessData>  &seclists){

	printf("	SecAssess::FilterSectionList  ---> begin!!!  SectionAcessessLists  %ld \n",this->GetSectionAcessessLists()->size());

		// 对板块评估表进行初步过滤   >>>> 插入临时评估表 seclists
		for (vector<SectionAssessData>::iterator item = this->GetSectionAcessessLists()->begin();
							item != this->GetSectionAcessessLists()->end(); item++){

			bool long_con=item->PD_Long_SignalNumber >0  && item->PD_Short_SignalNumber==0;
			bool short_con=item->PD_Long_SignalNumber ==0  && item->PD_Short_SignalNumber>0;
			bool risknum = item->ProductHoldNumber < item->ProductRiskNumber &&item->ProductRiskNumber>0;  //允许开仓的板块

			if(  risknum ){

				if(long_con||short_con){
					printf("	SecAssess::FilterSectionList  ---> 过滤方向明确板块   %s \n",item->Section);
					SectionAssessData data;
					strcpy(data.Section,item->Section);
					data.ProductRiskNumber =item->ProductRiskNumber ;
					data.ProductHoldNumber =item->ProductHoldNumber ;
					data.PD_Long_Inst_UpdownRate =item->PD_Long_Inst_UpdownRate ;
					strcpy(data.PD_Long_InstrumentId,item->PD_Long_InstrumentId);
					data.PD_Long_SignalNumber =item->PD_Long_SignalNumber ;
					data.PD_Short_Inst_UpdownRate =item->PD_Short_Inst_UpdownRate ;
					strcpy(data.PD_Short_InstrumentId,item->PD_Short_InstrumentId);
					data.PD_Short_SignalNumber =item->PD_Short_SignalNumber ;
					data.Priority = item->Priority;
					//符合信号要求的版块插入临时评估表中
					seclists.push_back(data);
				}
				else if(item->PD_Long_SignalNumber >0  && item->PD_Short_SignalNumber>0){
					printf("	SecAssess::FilterSectionList  ---> 板块内部方向分歧  %s \n",item->Section);
				}
				else if( item->PD_Long_SignalNumber ==0  && item->PD_Short_SignalNumber==0){
	//				printf("	SecAssess::SectionRecommand  ---> 板块无信号  %s \n",item->Section);
				}
				else{
					printf("	SecAssess::FilterSectionList  ---> 未知  %s 最大开仓品种限制[%d]  已经持仓品种数[%d] \n",item->Section,
							item->ProductRiskNumber,item->ProductHoldNumber);
				}

			}
			else if(!risknum){

				if(item->ProductRiskNumber==0){
	//				printf("   SecAssess::SectionRecommand  ---> 板块不允许开单!!!  %s    最大开仓品种限制[%d]  已经持仓品种数[%d]\n",
	//									item->Section,item->ProductRiskNumber,item->ProductHoldNumber);
				}
				else if(item->ProductHoldNumber >= item->ProductRiskNumber ){
					printf("	SecAssess::FilterSectionList  ---> 板块已存在持仓单 不允许开单  %s    最大开仓品种限制[%d]  已经持仓品种数[%d]\n",
										item->Section,item->ProductRiskNumber,item->ProductHoldNumber);
				}

			}

		}

		printf("	SecAssess::FilterSectionList  >>>>>> 完成有效板块过滤   %ld \n",seclists.size());
}

void SecAssess::StaticsSectionList(vector<SectionAssessData>  &seclists,SignalAssessData *data){
	//统计有单一方向的板块 信号数据
		for (auto &item:seclists){
			data->Total_SingleDirection_SectionNumber++;
			if(item.PD_Long_SignalNumber >0 ){

				data->Single_Long_SignalNumber+=item.PD_Long_SignalNumber;
				data->SingleLong_SectionNumber++;

				if(item.PD_Long_SignalNumber>data->High_Long_SignalNumber){
					data->High_Long_SignalNumber=item.PD_Long_SignalNumber;

					strcpy(data->High_Long_Section,item.Section);
					strcpy(data->High_Long_InstrumentID,item.PD_Long_InstrumentId);

				}
			}

			if(item.PD_Short_SignalNumber >0 ){
				data->Single_Short_SignalNumber+=item.PD_Short_SignalNumber;
				data->SingleShort_SectionNumber++;

				if(item.PD_Short_SignalNumber>data->High_Short_SignalNumber){
					data->High_Short_SignalNumber=item.PD_Short_SignalNumber;
					strcpy(data->High_Short_Section,item.Section);
					strcpy(data->High_Short_InstrumentID,item.PD_Short_InstrumentId);
				}
			}
		}
		data->Total_SingleDirection_SignalNumber = data->Single_Long_SignalNumber +data->Single_Long_SignalNumber;

		printf("	SecAssess::StaticsSectionList  >>>>>> 分析有信号的板块总数   %d \n",data->Total_SingleDirection_SectionNumber);
		if(data->Total_SingleDirection_SectionNumber>0 ){
			printf("	SecAssess::StaticsSectionList  >>>>>>多头板块数: %d  空头板块数: %d \n",data->SingleLong_SectionNumber,data->SingleShort_SectionNumber);
			printf("	SecAssess::StaticsSectionList  >>>>>>多头板块最多信号数: %d  空头板块最多信号数: %d \n",data->High_Long_SignalNumber,data->High_Short_SignalNumber);
			printf("	SecAssess::StaticsSectionList  >>>>>>多头信号总数: %d  空头信号总数: %d \n",data->Single_Long_SignalNumber,data->Single_Short_SignalNumber);
		}

}
void SecAssess::UpdateMysqlSignal(SignalAssessData* data){

	Config config(this->GetStrategyName().c_str());
	MysqlSignal mysqlsignal(config.GetUserID().c_str());
	if(mysqlsignal.Exist_Table() && mysqlsignal.Count_ColumnsbyTableName()<17){
		mysqlsignal.DropTable();
			printf("	SecAssess::UpdateMysqlSignal 清空数据表  MysqlSignal !!!\n");
		}
	else{
		mysqlsignal.CreateTable();
	}

	SignalAssessData signalassessdata;
	DateUtil dateutil;
	std::string current_sqltime =dateutil.ConvertCurrentTimetoSqlTime();
	strcpy(signalassessdata.Time,current_sqltime.c_str());
	strcpy(signalassessdata.TradingDay,this->GetTradingDay().c_str());
	signalassessdata.Single_Long_SignalNumber = data->Single_Long_SignalNumber;
	signalassessdata.Single_Short_SignalNumber = data->Single_Short_SignalNumber;
	signalassessdata.Total_SingleDirection_SignalNumber = data->Single_Long_SignalNumber+data->Single_Short_SignalNumber;

	signalassessdata.SingleLong_SectionNumber = data->SingleLong_SectionNumber;
	signalassessdata.SingleShort_SectionNumber = data->SingleShort_SectionNumber;
	signalassessdata.Total_SingleDirection_SectionNumber= data->Total_SingleDirection_SectionNumber;

	strcpy(signalassessdata.High_Long_Section,data->High_Long_Section);
	strcpy(signalassessdata.High_Long_InstrumentID,data->High_Long_InstrumentID);
	signalassessdata.High_Long_SignalNumber = data->High_Long_SignalNumber;

	strcpy(signalassessdata.High_Short_Section,data->High_Short_Section);
	strcpy(signalassessdata.High_Short_InstrumentID,data->High_Long_InstrumentID);
	signalassessdata.High_Short_SignalNumber = data->High_Short_SignalNumber;

	signalassessdata.RecommandDirection = data->RecommandDirection;
	strcpy(signalassessdata.RecommandInstrumentID , data->RecommandInstrumentID);


	if(!mysqlsignal.Exist_DatabyTradingDay(this->GetTradingDay().c_str())){
		mysqlsignal.Insert(signalassessdata);
		printf("	SecAssess::UpdateMysqlSignal  不存在数据 新交易日首次插入新数据%s  !!!\n",signalassessdata.TradingDay);
	}
	else{
		SignalAssessData max_signalassessdata;
		max_signalassessdata = mysqlsignal.Find_MaxSignalNumberbyTradingDay(this->GetTradingDay().c_str());

		SignalAssessData last = mysqlsignal.Find_LastDatabyTradingDay(this->GetTradingDay().c_str());

		bool count_change = max_signalassessdata.Total_SingleDirection_SignalNumber < data->Single_Long_SignalNumber + data->Single_Short_SignalNumber;
		bool recommand_change = strcmp(last.RecommandInstrumentID,signalassessdata.RecommandInstrumentID)!=0;
		if(count_change || recommand_change){
			mysqlsignal.Insert(signalassessdata);
			printf("	SecAssess::UpdateMysqlSignal 信号数增加  插入新数据%s   max:%d   ---> %d !!!\n",signalassessdata.TradingDay,
					max_signalassessdata.Total_SingleDirection_SignalNumber,data->Single_Long_SignalNumber);
		}
	}
}
bool SecAssess::SectionRecommand(SectionAssessData * ret_data){

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

	bool ret=false;
	vector<SectionAssessData>  seclists;

	SignalAssessData initialdata;
	memset(&initialdata,0,sizeof(initialdata));

	//过滤掉已有持仓的板块\无信号的板块\信号冲突的板块
	this->FilterSectionList(seclists);

	//根据过滤后的板块 进行信号统计
	this->StaticsSectionList(seclists,&initialdata);

	if(initialdata.Total_SingleDirection_SectionNumber==0){
		ret=false;
	}
	else if(initialdata.Total_SingleDirection_SectionNumber==1){

		if(initialdata.Single_Long_SignalNumber>0){
			if(initialdata.Single_Long_SignalNumber==1){
				//1个板块 1个信号
				this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
				ret=true;
			}
			else{
				//该板块有多个信号，需要对板块内的信号进行甄别
				// 选择板块内涨幅最高的合约
				this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
				ret=true;
			}

		}
		else if(initialdata.Single_Short_SignalNumber>0){

			if(initialdata.Single_Short_SignalNumber==1){
				//1个板块 1个信号
				this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
				ret=true;
			}
			else{
				//该板块有多个信号，需要对板块内的信号进行甄别
				// 选择板块内跌幅最低的合约
				this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
				ret=true;
			}
		}
	}
	else{
		printf("    SecAssess::SectionRecommand   多个板块出现信号\n");
		//多个板块出现信号


		if(initialdata.Single_Long_SignalNumber==0){
			//全是多头信号
			printf("    SecAssess::SectionRecommand   多个板块, 全是全是多头信号\n");
			if(initialdata.High_Long_SignalNumber==initialdata.Total_SingleDirection_SignalNumber/initialdata.Total_SingleDirection_SectionNumber){
				//多个板块, 每个板块信号数相同
				printf("    SecAssess::SectionRecommand   多个板块, 每个板块信号数相同\n");
				this->GetSecAssessbyPriority(seclists,ret_data);
				ret=true;
				printf("    SecAssess::SectionRecommand  板块[%d]  多头信号[%d]--->板块[%s]优先级[%d]   板块信号数: %d  \n",
						initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Long_SignalNumber,ret_data->Section,ret_data->Priority,ret_data->PD_Long_SignalNumber);
			}
			else{
				//多个板块, 存在板块间信号数不同
				printf("    SecAssess::SectionRecommand   多个板块, 存在板块间信号数不同\n");

					printf("    SecAssess::SectionRecommand  板块[%d]  多头信号[%d]--->板块[%s]出现最高多头信号数: %d  \n",
							initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Long_SignalNumber,initialdata.High_Long_Section,initialdata.High_Long_SignalNumber);
					this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
					ret=true;

			}
		}
		else if(initialdata.Single_Long_SignalNumber==0){
			//全是空头信号
			printf("    SecAssess::SectionRecommand   多个板块, 全是空头信号\n");
			if(initialdata.High_Short_SignalNumber==initialdata.Total_SingleDirection_SignalNumber/initialdata.Total_SingleDirection_SectionNumber){
				//多个板块, 每个板块信号数相同
				printf("    SecAssess::SectionRecommand   多个板块, 每个板块信号数相同\n");
				this->GetSecAssessbyPriority(seclists,ret_data);
				ret=true;
				printf("    SecAssess::SectionRecommand  板块[%d]  空头信号[%d]--->板块[%s]优先级[%d]   板块信号数: %d  \n",
						initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Long_SignalNumber,ret_data->Section,ret_data->Priority,ret_data->PD_Short_SignalNumber);
			}
			else{
				//多个板块, 存在板块间信号数不同
				printf("    SecAssess::SectionRecommand   多个板块, 存在板块间信号数不同\n");
				printf("    SecAssess::SectionRecommand  板块[%d]  空头信号[%d]--->板块[%s]出现最高空头信号数: %d  \n",
						initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Short_SignalNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);

				this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
				 ret=true;

			}
		}
		else{
			//多头 空头信号并存
//			printf("    SecAssess::SectionRecommand   多个板块,多头 空头信号并存\n");
			printf("    SecAssess::SectionRecommand  多个板块,多头 空头信号并存 有信号的板块数[%d]  总多头信号[%d] 总空头信号[%d]  \n",
					initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Long_SignalNumber,initialdata.Single_Short_SignalNumber);
			printf("    SecAssess::SectionRecommand  --->多头板块数[%d]   多头信号最多板块[%s]出现最高多头信号数: %d  \n",
					initialdata.SingleLong_SectionNumber,initialdata.High_Long_Section,initialdata.High_Long_SignalNumber);
			printf("    SecAssess::SectionRecommand  --->多头板块数[%d]   空头信号最多板块[%s]出现最高空头信号数: %d  \n",
					initialdata.SingleShort_SectionNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);

			if(initialdata.Single_Long_SignalNumber>initialdata.Single_Short_SignalNumber){
				//多头板块 信号多
				this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
				 ret=true;
			}
			else if (initialdata.Single_Long_SignalNumber <initialdata.Single_Short_SignalNumber){
				//空头板块 信号多
				this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
				 ret=true;
			}
			else{
				//多空板块 信号数相同
				this->GetSecAssessbySedimentaryIORate(seclists,ret_data);
				ret=true;
			}


//			if(initialdata.High_Long_SignalNumber>initialdata.High_Short_SignalNumber){
//				//多头板块 信号多
//				this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
//				 ret=true;
//			}
//			else if(initialdata.High_Long_SignalNumber<initialdata.High_Short_SignalNumber)	{
//				//空头板块 信号多
//				this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
//				 ret=true;
//			}
//			else{
//				//多空板块 信号数相同
//				this->GetSecAssessbyPriority(seclists,ret_data);
//				ret=true;
//			}
		}
	}




	if(ret){
		printf("	SecAssess::SectionRecommand  >>>>>> 存在信号的板块数:%d  信号分析----推荐合约  %s  推荐方向: %d \n",
				initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);

		if(initialdata.Single_Long_SignalNumber>0 && initialdata.Single_Short_SignalNumber==0){
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>整体多头行情--->板块数: %d  多头信号总数: %d  多头板块[%s]最多信号数: %d  ",initialdata.SingleLong_SectionNumber,
					initialdata.Single_Long_SignalNumber,initialdata.High_Long_Section,initialdata.High_Long_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>优选成功----> 存在信号的板块数:%d  信号分析推荐合约:%s  推荐方向:%d \n",
					initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);
			logutil.WriteLog(logbuf);
		}
		else if (initialdata.Single_Long_SignalNumber==0 && initialdata.Single_Short_SignalNumber>0){
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>整体空头行情--->板块数: %d  空头信号总数: %d  空头板块[%s]最多信号数: %d  ",initialdata.SingleShort_SectionNumber,
					initialdata.Single_Long_SignalNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>优选成功----> 存在信号的板块数:%d  信号分析推荐合约:%s  推荐方向:%d \n",
					initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);
			logutil.WriteLog(logbuf);
		}
		else{

			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----多头板块数: %d  空头板块数: %d ",initialdata.SingleLong_SectionNumber,initialdata.SingleShort_SectionNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----多头板块[%s]最多信号数: %d  空头板块[%s]最多信号数: %d ",initialdata.High_Long_Section,
					initialdata.High_Long_SignalNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----多头信号总数: %d  空头信号总数: %d ",initialdata.Single_Long_SignalNumber,initialdata.Single_Short_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----优选成功----> 存在信号的板块数:%d  信号分析推荐合约:%s  推荐方向:%d \n",
					initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);
			logutil.WriteLog(logbuf);
		}

		initialdata.RecommandDirection = ret_data->Result_Direction;
		strcpy(initialdata.RecommandInstrumentID , ret_data->Result_InstrumentId);
		this->UpdateMysqlSignal(&initialdata);
	}

	vector<SectionAssessData>().swap(seclists);
	return ret;
}

bool SecAssess::SectionRecommandbyMoreSignal(SectionAssessData * ret_data){

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

	bool ret=false;
	vector<SectionAssessData>  seclists;
	SignalAssessData initialdata;
	memset(&initialdata,0,sizeof(initialdata));
	//过滤掉已有持仓的板块\无信号的板块\信号冲突的板块
	this->FilterSectionList(seclists);
	//根据过滤后的板块 进行信号统计
	this->StaticsSectionList(seclists,&initialdata);

	if(initialdata.High_Long_SignalNumber<2 && initialdata.High_Short_SignalNumber<2){
		printf("    SecAssess::SectionRecommand   信号数量不足,直接退出推荐分析环节!!! \n");	//多个板块出现信号
		return false;
	}

	//全局市场的情况

	if(initialdata.High_Long_SignalNumber>=2 || initialdata.High_Short_SignalNumber>=2){
		printf("    SecAssess::SectionRecommand   多个板块出现信号\n");	//多个板块出现信号
		if(initialdata.Single_Short_SignalNumber==0){
			//全是多头信号
			printf("    SecAssess::SectionRecommand   多个板块, 全是全是多头信号\n");

			printf("    SecAssess::SectionRecommand  板块[%d]  多头信号[%d]--->板块[%s]出现最高多头信号数: %d  \n",
					initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Long_SignalNumber,initialdata.High_Long_Section,initialdata.High_Long_SignalNumber);
			this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
			ret=true;
		}
		else if(initialdata.Single_Long_SignalNumber==0){
			//全是空头信号
			printf("    SecAssess::SectionRecommand   多个板块, 全是空头信号\n");
			//多个板块, 存在板块间信号数不同
			printf("    SecAssess::SectionRecommand   多个板块, 存在板块间信号数不同\n");
			printf("    SecAssess::SectionRecommand  板块[%d]  空头信号[%d]--->板块[%s]出现最高空头信号数: %d  \n",
					initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Short_SignalNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);

			this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
			 ret=true;
		}
		else{
			//多头 空头信号并存
//			printf("    SecAssess::SectionRecommand   多个板块,多头 空头信号并存\n");
			printf("    SecAssess::SectionRecommand  多个板块,多头 空头信号并存 有信号的板块数[%d]  总多头信号[%d] 总空头信号[%d]  \n",
					initialdata.Total_SingleDirection_SectionNumber,initialdata.Single_Long_SignalNumber,initialdata.Single_Short_SignalNumber);
			printf("    SecAssess::SectionRecommand  --->多头板块数[%d]   多头信号最多板块[%s]出现最高多头信号数: %d  \n",
					initialdata.SingleLong_SectionNumber,initialdata.High_Long_Section,initialdata.High_Long_SignalNumber);
			printf("    SecAssess::SectionRecommand  --->多头板块数[%d]   空头信号最多板块[%s]出现最高空头信号数: %d  \n",
					initialdata.SingleShort_SectionNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);

			if(initialdata.Single_Long_SignalNumber>initialdata.Single_Short_SignalNumber){
				//多头板块 信号多
				this->GetSecAssessbySection(initialdata.High_Long_Section,ret_data);
				 ret=true;
			}
			else if (initialdata.Single_Long_SignalNumber <initialdata.Single_Short_SignalNumber){
				//空头板块 信号多
				this->GetSecAssessbySection(initialdata.High_Short_Section,ret_data);
				 ret=true;
			}
			else{
				//多空板块 信号数相同
				this->GetSecAssessbySedimentaryIORate(seclists,ret_data);
				ret=true;
			}

		}
	}



	if(ret){
		printf("	SecAssess::SectionRecommand  >>>>>> 存在信号的板块数:%d  信号分析----推荐合约  %s  推荐方向: %d \n",
				initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);

		if(initialdata.Single_Long_SignalNumber>0 && initialdata.Single_Short_SignalNumber==0){
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>整体多头行情--->板块数: %d  多头信号总数: %d  多头板块[%s]最多信号数: %d  ",initialdata.SingleLong_SectionNumber,
					initialdata.Single_Long_SignalNumber,initialdata.High_Long_Section,initialdata.High_Long_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>优选成功----> 存在信号的板块数:%d  信号分析推荐合约:%s  推荐方向:%d \n",
					initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);
			logutil.WriteLog(logbuf);
		}
		else if (initialdata.Single_Long_SignalNumber==0 && initialdata.Single_Short_SignalNumber>0){
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>整体空头行情--->板块数: %d  空头信号总数: %d  空头板块[%s]最多信号数: %d  ",initialdata.SingleShort_SectionNumber,
					initialdata.Single_Long_SignalNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>优选成功----> 存在信号的板块数:%d  信号分析推荐合约:%s  推荐方向:%d \n",
					initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);
			logutil.WriteLog(logbuf);
		}
		else{

			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----多头板块数: %d  空头板块数: %d ",initialdata.SingleLong_SectionNumber,initialdata.SingleShort_SectionNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----多头板块[%s]最多信号数: %d  空头板块[%s]最多信号数: %d ",initialdata.High_Long_Section,
					initialdata.High_Long_SignalNumber,initialdata.High_Short_Section,initialdata.High_Short_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----多头信号总数: %d  空头信号总数: %d ",initialdata.Single_Long_SignalNumber,initialdata.Single_Short_SignalNumber);
			logutil.WriteLog(logbuf);
			sprintf(logbuf,"	SecAssess::SectionRecommand  >>>>>>震荡-----优选成功----> 存在信号的板块数:%d  信号分析推荐合约:%s  推荐方向:%d \n",
					initialdata.Total_SingleDirection_SectionNumber,ret_data->Result_InstrumentId,ret_data->Result_Direction);
			logutil.WriteLog(logbuf);
		}

		initialdata.RecommandDirection = ret_data->Result_Direction;
		strcpy(initialdata.RecommandInstrumentID , ret_data->Result_InstrumentId);
		this->UpdateMysqlSignal(&initialdata);
	}

	vector<SectionAssessData>().swap(seclists);
	return ret;
}
bool SecAssess::SectionRecommandbyGlobalView(SectionAssessData * ret_data,Strategy * strategy){

	char logbuf[1024];
	LogUtil logutil(this->GetLogName(),this->GetLogPathFileName().c_str());

	bool ret=false;
	vector<SectionAssessData>  seclists;
	SignalAssessData initialdata;
	memset(&initialdata,0,sizeof(initialdata));

	GlobalViewData viewdata;
	memset(&viewdata,0,sizeof(viewdata));

	strategy->OutputGlobalViewData(&viewdata);
	GlobalViewData *view=&viewdata;

	//过滤掉已有持仓的板块\无信号的板块\信号冲突的板块
	this->FilterSectionList(seclists);
	//根据过滤后的板块 进行信号统计
	this->StaticsSectionList(seclists,&initialdata);


	bool max_rateup_ins=strcmp(view->MaxSedimentaryIORateIns,view->MaxUpDownIns)==0;
	bool max_ins=strcmp(view->MaxSedimentaryIORateIns,view->MaxSedimentaryIOIns)==0;
	bool max_ratelevel=view->MaxSedimentaryIORate>0.1;   //资金流入比超过10%
	bool max_sedio=view->MaxSedimentaryIO>0.5;


	bool min_rateup_ins=strcmp(view->MinSedimentaryIORateIns,view->MinUpDownIns)==0;
	bool min_ins=strcmp(view->MinSedimentaryIORateIns,view->MinSedimentaryIOIns)==0;
	bool min_ratelevel=view->MinSedimentaryIORate<-0.1;
	bool min_sedio=view->MinSedimentaryIO<-0.5;

	if(max_rateup_ins&&max_ratelevel&&max_sedio && view->LongWeight > view->ShortWeight){
		printf("	SectionRecommandbyGlobalView: 多头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MaxSedimentaryIORateIns,view->MaxSedimentaryIO,
		view->MaxSedimentaryIORate*100,view->MaxUpDown*100);
		sprintf(logbuf,"	SectionRecommandbyGlobalView: 多头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MaxSedimentaryIORateIns,view->MaxSedimentaryIO,
				view->MaxSedimentaryIORate*100,view->MaxUpDown*100);
		logutil.WriteLog(logbuf);

		this->GetSecAssessbyInstrumentID(view->MaxSedimentaryIORateIns,ret_data,strategy);

		if(strcmp(ret_data->PD_Long_InstrumentId,ret_data->Result_InstrumentId)==0){
			ret=true;
		}
		else{
			printf("	SectionRecommandbyGlobalView: 板块合约%s 最终合约不同:%s \n",ret_data->PD_Long_InstrumentId,ret_data->Result_InstrumentId);
		}

	}
	else if(min_rateup_ins&&min_ratelevel&&min_sedio&& view->LongWeight < view->ShortWeight){
		printf("	SectionRecommandbyGlobalView: 空头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MinSedimentaryIORateIns,view->MinSedimentaryIO,
		view->MinSedimentaryIORate*100,view->MinUpDown*100);
		sprintf(logbuf,"	SectionRecommandbyGlobalView: 空头合约 %s请关注  资金流入:%.2f  资金流比:%.2f%% 涨幅:%.2f%%  \n",view->MinSedimentaryIORateIns,view->MinSedimentaryIO,
				view->MinSedimentaryIORate*100,view->MinUpDown*100);
		logutil.WriteLog(logbuf);
		this->GetSecAssessbyInstrumentID(view->MinSedimentaryIORateIns,ret_data,strategy);

		if(strcmp(ret_data->PD_Short_InstrumentId,ret_data->Result_InstrumentId)==0){
			ret=true;
		}
		else{
			printf("	SectionRecommandbyGlobalView: 板块合约%s 最终合约不同:%s \n",ret_data->PD_Short_InstrumentId,ret_data->Result_InstrumentId);
		}
	}


	//全局市场的情况



	vector<SectionAssessData>().swap(seclists);
	return ret;
}
