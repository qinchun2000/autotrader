

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string.h>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/find.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <json/json.h>
#include "dateutil.hpp"
#include "mongoproductclass.hpp"






#ifndef MONGOINSTRUMENT_HPP_
#define MONGOINSTRUMENT_HPP_

using namespace mongocxx;

using mongocxx::database;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;


	class  InstrumentInfo
	{

		public :

			 char InstrumentID[9];

			  ///交易所代码 TThostFtdcExchangeIDType char[9]
			  char ExchangeID[27];

			  ///合约名称 TThostFtdcInstrumentNameType char[21]
			  char InstrumentName[21];

			  ///合约在交易所的代码 TThostFtdcExchangeInstIDType char[31]
			  char ExchangeInstID[31];

			  ///产品代码 TThostFtdcInstrumentIDType char[31]
			  char ProductID[31];

			  ///产品类型 TThostFtdcProductClassType char
			  //// THOST_FTDC_PC_Futures '1' 期货
			  //// THOST_FTDC_PC_Options '2' 期权
			  //// THOST_FTDC_PC_Combination '3' 组合
			  //// THOST_FTDC_PC_Spot '4' 即期
			  //// THOST_FTDC_PC_EFP '5' 期转现
			  int ProductClass ;
			  ///交割年份 TThostFtdcYearType int
			  int DeliveryYear ;
			  ///交割月 TThostFtdcMonthType int
			  int DeliveryMonth ;
			  ///市价单最大下单量 TThostFtdcVolumeType int
			  int MaxMarketOrderVolume ;
			  ///市价单最小下单量 TThostFtdcVolumeType int
			  int MinMarketOrderVolume ;
			  ///限价单最大下单量 TThostFtdcVolumeType int
			  int MaxLimitOrderVolume ;
			  ///限价单最小下单量 TThostFtdcVolumeType int
			  int MinLimitOrderVolume ;
			  ///合约数量乘数 TThostFtdcVolumeMultipleType int
			  int VolumeMultiple ;
			  ///最小变动价位 TThostFtdcPriceType double
			  double PriceTick ;
			  ///创建日 TThostFtdcDateType char[9]
			  char CreateDate[27];

			  ///上市日 TThostFtdcDateType char[9]
			  char OpenDate[27];

			  ///到期日 TThostFtdcDateType char[9]
			  char ExpireDate[27];

			  ///开始交割日 TThostFtdcDateType char[9]
			  char StartDelivDate[27];

			  ///结束交割日 TThostFtdcDateType char[9]
			  char EndDelivDate[27];

			  ///合约生命周期状态 TThostFtdcInstLifePhaseType char
			  //// THOST_FTDC_IP_NotStart '0' 未上市
			  //// THOST_FTDC_IP_Started '1' 上市
			  //// THOST_FTDC_IP_Pause '2' 停牌
			  //// THOST_FTDC_IP_Expired '3' 到期
			  int InstLifePhase ;
			  ///当前是否交易 TThostFtdcBoolType int
			  int IsTrading ;
			  ///持仓类型 TThostFtdcPositionTypeType char
			  //// THOST_FTDC_PT_Net '1' 净持仓
			  //// THOST_FTDC_PT_Gross '2' 综合持仓
			  char PositionType ;
			  ///持仓日期类型 TThostFtdcPositionDateTypeType char
			  //// THOST_FTDC_PDT_UseHistory '1' 使用历史持仓
			  //// THOST_FTDC_PDT_NoUseHistory '2' 不使用历史持仓
			  char PositionDateType ;
			  ///多头保证金率 TThostFtdcRatioType double
			  double LongMarginRatio;
			  ///空头保证金率 TThostFtdcRatioType double
			  double ShortMarginRatio ;
			  ///是否使用大额单边保证金算法 TThostFtdcMaxMarginSideAlgorithmType char
			  //// THOST_FTDC_MMSA_NO '0' 不使用大额单边保证金算法
			  //// THOST_FTDC_MMSA_YES '1' 使用大额单边保证金算法
			  char MaxMarginSideAlgorithm ;

			  char UpdateTime[27];
			  char LocalUpdateTime[27];

		private :

	};

	class MongoInstrument
		{

		public :

		MongoInstrument();

			void Insert(InstrumentInfo instrumentinfo);
			void DeleteAll();
			void Check_Instrument(InstrumentInfo instrumentinfo);
			InstrumentInfo Find_InstrumentID(InstrumentInfo instrumentinfo);
			InstrumentInfo Findby_InstrumentID(char * pinstrumentid);
			bool Exist_InstrumentID(char * pinstrumentid);
			std::string Find_LastUpdateTime();
			void Check_ProductClass();
			int Count();

		private :

			char  _mongodb_url[30];
			char  _mongodb_dbname[20];
			char  _mongodb_collenction[20];
			mongocxx::database _dbname;
			mongocxx::collection _collection;


		};


#endif /* MONGOINSTRUMENT_HPP_ */
