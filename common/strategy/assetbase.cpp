#include "assetbase.hpp"

AssetBase::AssetBase() {

//	SetInitialMoney(1000000.0);
//	SetRiskRate(0.015);
//	SetMaxMargin(0.3);

//	printf("AssetBase 构造函数  \n");
}

AssetBase::~AssetBase() {

}
void AssetBase::SetAvalible(double ava)
{
	this->_avalible=ava;
}
double  AssetBase::GetAvalible()
{
	return this->_avalible;
}

//void AssetBase::SetMaxMargin(double max)
//{
//	this->_maxmargin=max;
//}
//double  AssetBase::GetMaxMargin()
//{
//	return this->_maxmargin;
//}

double AssetBase::GetAssetAvaliblebyStrategyName(const char* strategynam)
{

	MysqlUser mysqluser;
	User userinfo=mysqluser.Find_ActiveDatabyStrategyName(strategynam);

	MysqlAsset mysqlasset(userinfo.UserID);
	AssetData asset;

	asset=mysqlasset.Find_LastDayAsset();


	return asset.Available;
}

void AssetBase::SetRiskRate(double risk)
{
	this->_riskrate=risk;
}

double AssetBase::GetRiskRate()
{
return this->_riskrate;
}

void AssetBase::SetStopRate(double stoprate)
{

	this->_stoprate=stoprate;
}
double AssetBase::GetStopRate()
{
return this->_stoprate;
}


void AssetBase::SetWinRate(double winrate)
{

	this->_winrate=winrate;
}
double AssetBase::GetWinRate()
{
return this->_winrate;
}

void AssetBase::SetBigVolumeRate(double rate){

	this->_bigvolumerate=rate;
}
double AssetBase::GetBigVolumeRate(){
return this->_bigvolumerate;
}

void AssetBase::SetSingleMaxMargin(double margin){

	this->_singleins_maxmargin=margin;
}
double AssetBase::GetSingleMaxMargin(){
return this->_singleins_maxmargin;
}

void AssetBase::SetTotalMaxMargin(double margin){

	this->_total_maxmargin=margin;
}
double AssetBase::GetTotalMaxMargin(){
return this->_total_maxmargin;
}
void AssetBase::SetInitialMoney(double initial){
	this->_initialmoney=initial;
}

double AssetBase::GetInitialMoney(){
	return this->_initialmoney;
}

void AssetBase::SetMaxSedimentary(double num){
	this->_maxsedimentary=num;
}

double AssetBase::GetMaxSedimentary(){
	return this->_maxsedimentary;
}


void AssetBase::SetHoldPositionRate(double rate)
{
	this->_holdpositionrate=rate;
}
double AssetBase::GetHoldPositionRate()
{
	return this->_holdpositionrate;
}

void AssetBase::SetTotalHolding(int total)
{
	this->_totalholding=total;
}
int AssetBase::GetTotalHolding()
{
	return this->_totalholding;
}

void AssetBase::SetOpenSignalCount(int count)
{
	this->_open_signalcount=count;
}
int AssetBase::GetOpenSignalCount()
{
	return this->_open_signalcount;
}

void AssetBase::SetCloseSignalCount(int count)
{
	this->_close_signalcount=count;
}
int AssetBase::GetCloseSignalCount()
{
	return this->_close_signalcount;
}

void AssetBase::SetMaxCancelOrderCount(int count)
{
	this->_max_cancel_ordercount=count;
}
int AssetBase::GetMaxCancelOrderCount()
{
	return this->_max_cancel_ordercount;
}

void AssetBase::SetMaxOpenErrorCount(int count)
{
	this->_max_openerror_count=count;
}
int AssetBase::GetMaxOpenErrorCount()
{
	return this->_max_openerror_count;
}
