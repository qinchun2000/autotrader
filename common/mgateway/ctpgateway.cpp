/*
 * ctpgateway.cpp
 *
 *  Created on: 2020年2月22日
 *      Author: qc
 */

#include "ctpgateway.hpp"

CTPGateway::CTPGateway(EventEngine *eventengine, std::string gatewayname) :GatewayBase(eventengine, "CTP")
{
	m_eventengine = eventengine;
	m_gatewayname = gatewayname;

	//创建行情和交易的回调对象
//	m_MDSPI = new CTPMD(this, gatewayname);
	m_TDSPI = new CTPTD(this, gatewayname);

	ctpmdconnected = false;
	ctptdconnected = false;
	m_qryEnabled = false;
	m_qrycount = 0;
	m_eventengine->RegEvent(EVENT_TIMER, std::bind(&CTPGateway::query, this));//注册到事件驱动引擎，循环查询

//	std::cout<<"CTPGateway-------------------->"<<endl;
}

CTPGateway::~CTPGateway()
{
//	if (m_MDSPI != nullptr)
//	{
//		delete m_MDSPI;
//	}
	if (m_TDSPI != nullptr)
	{
		delete m_TDSPI;
	}
}

void CTPGateway::connect()
{
//	连接行情和交易
//	先扫描文档下的账号密码配置文件
//	if (_access("./CTPGateway", 0) != -1)
//	{
//		std::fstream f;
//		f.open("./CTPGateway/CTP_connect");
//		if (!f.is_open())
//		{
//			//如果打不开文件
//			std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
//			e->msg = "无法读取本地配置文件";
//			e->gatewayname = m_gatewayname;
//			this->onLog(e);
//			return;
//		}
//		std::string line;
//		std::map<std::string, std::string>configmap;
//		while (!f.eof())
//		{
//			getline(f, line);
//			std::string::size_type pos = line.find("=");//按照等号分隔
//			configmap.insert(std::make_pair(line.substr(0, pos), line.substr(pos + 1, line.size() - 1)));
//		}
////		m_MDSPI->connect(configmap["userid"], configmap["password"], configmap["brokerid"], configmap["mdaddress"]);
//		m_TDSPI->connect(configmap["userid"], configmap["password"], configmap["brokerid"], configmap["tdaddress"]);
//	}
//	else
//	{
//		std::shared_ptr<Event_Log>e = std::make_shared<Event_Log>();
//		e->msg = "无法读取本地配置文件";
//		e->gatewayname = m_gatewayname;
//		this->onLog(e);
//	}

	m_TDSPI->connect("081759", "qin1978", "9999", "tcp://180.168.146.187:10130");
}

void CTPGateway::subscribe(SubscribeReq& subscribeReq)
{
//	m_MDSPI->subscribe(subscribeReq);
}

std::string CTPGateway::sendOrder(OrderReq & req)
{
	return m_TDSPI->sendOrder(req);
}

void CTPGateway::cancelOrder(CancelOrderReq & req)
{
	m_TDSPI->cancelOrder(req);
}

void CTPGateway::qryAccount()
{

	m_TDSPI->qryAccount();
}

void CTPGateway::qryPosition()
{
	m_TDSPI->qryPosition();
}

//std::shared_ptr<Event_Order>CTPGateway::getorder(std::string orderID)
//{
//	std::unique_lock<std::mutex>lck(m_ordermapmtx);
//	if (m_ordermap.find(orderID) != m_ordermap.end())
//	{
//		return m_ordermap[orderID];
//	}
//	else
//	{
//		return nullptr;
//	}
//}

void CTPGateway::close()
{
	m_qryEnabled = false;
//	m_MDSPI->close();
	m_TDSPI->close();

}

void CTPGateway::initQuery()
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	m_qryEnabled = true;

	std::cout<<"iiiiiiiiiiiiiiiiiiiiiiiiii"<<endl;
}

void CTPGateway::query()
{

	std::cout<<"query ----------------->runing"<<endl;
	if (m_qryEnabled == false)
	{
		std::cout<<"1111111111111111"<<endl;
		return;
	}
	m_qrycount++;
	if (m_qrycount == 2)
	{
		std::cout<<"2222222222222222222222"<<endl;
		qryAccount();
	}
	else if (m_qrycount == 4)
	{
		std::cout<<"44444444444444444444444444444"<<endl;
		qryPosition();
	}
	else if (m_qrycount > m_maxqry)
	{
		m_qrycount = 0;
	}
}



