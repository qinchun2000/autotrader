/*
 * main.cpp
 *
 *  Created on: 2017-8-8
 *      Author: vnpy
 */

// 标准C库文件
#include <stdio.h>
#include <string.h>
#include <cstdlib>

#include "mongoexchangeinfo.hpp"
#include "mongoinstrument.hpp"
#include "mongodepthmarket.hpp"
#include "mongomaincontract.hpp"


int main(int argc, char** args)
{

	printf("main 入口 ...\n");
	ExchangeInfo exchangeInfo;
	strcpy(exchangeInfo.ExchangeID,"1");
	strcpy(exchangeInfo.ExchangeName,"CZCE");
	exchangeInfo.ExchangeProperty='1';

	InstrumentInfo instrumentinfo;
	strcpy(instrumentinfo.InstrumentID,"rb1710");

	MongoExchangeInfo mongoExchangeInfo;
	//mongoExchangeInfo.Insert(exchangeInfo);
	MongoInstrument mongoinstrument;
	mongoinstrument.Find_InstrumentID(instrumentinfo);
	return 0;
}
