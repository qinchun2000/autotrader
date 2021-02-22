#include "zeromqutil.hpp"

ZeroMqUtil::ZeroMqUtil(DataWrapper * datawrapper,const char* staname){

	m_datawrapper = datawrapper;
	this->SetStrategyName(staname);

}
ZeroMqUtil::~ZeroMqUtil(){

}

void ZeroMqUtil::SetStrategyName(const char* strategyname){
	this->m_strategyname=strategyname;

}

std::string ZeroMqUtil::GetStrategyName(){
	return this->m_strategyname;
}

std::string ZeroMqUtil::RouterPort(){
	std::string port;
	if(this->GetStrategyName()=="md"){
		port="5888";
	}
	else{
		port="5559";
	}

	return port;
}
void ZeroMqUtil::run(){


	 // Prepare our context and socket
	    zmq::context_t context (1);
	    zmq::socket_t socket (context, ZMQ_REP);
	    std::string address="tcp://*:"+this->RouterPort();
	    socket.bind (address);
//	    socket.bind ("tcp://*:5555");

	    while (true) {
//	    	std::cout<<"zmq>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   port:"<<address<<std::endl;
	        zmq::message_t request;

	        // Wait for next request from client
	        socket.recv (&request);

	        std::string req = std::string(static_cast<char*>(request.data()), request.size());
	        std::cout << "Received --->" <<req << std::endl;

	        int req_code = this->GetReqCode(req.c_str());
	        std::cout<<"req code:"<<req_code<<std::endl;

	        switch (req_code){
	        case 1:
	        	{
	        	  std::string str_ins=ProcessQueryHoldPosition(req.c_str());
	        	  std::cout <<"sending--->"<< str_ins << std::endl;
				 // Send reply back to client
					zmq::message_t reply (str_ins.length());
					memcpy ((void *) reply.data (), str_ins.c_str(), str_ins.length());
					socket.send (reply);
	        	}

				break;
	        case 201:

	        	break;
	        default:
	        	std::cout << "no answer!"<<std::endl;
	        	std::string str_error=PackError(1002);

	        	zmq::message_t reply (str_error.length());
				memcpy ((void *) reply.data (), str_error.c_str(), str_error.length());
				socket.send (reply);

	        }


	        // Do some 'work'
	        sleep (1);



	    }
	    return ;

}

int ZeroMqUtil::GetReqCode(const char* req)
{
	std::string str_req=req;
	int reqcode=0;
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str_req, root))	{
		 if (root["ReqCode"].isInt()){
			 reqcode=root["ReqCode"].asInt();
		 }

	}

	return reqcode;
}
std::string ZeroMqUtil::ProcessQueryHoldPosition(const char* req)
{
	std::string str_req=req;
	std::string userid;
	std::string ret;
	Json::Reader reader;
	Json::Value root;
	Json::Value data;
	if (reader.parse(str_req, root))	{

		 if (root["data"]["userid"].isString()){
			 userid = root["data"]["userid"].asString();
			 ret=PackHoldPosition(userid.c_str());
		 }
		 else{
			 ret=PackError(1001); //请求数据非法
		 }

	}
	else{
		ret=PackError(1001);//请求数据非法
	}


	return ret;
}
std::string ZeroMqUtil::PackHoldPosition(const char* userid){

	std::string ret;

	// 构建json数组
	Json::Value array;
	Json::Value root;
	Json::Value holddata;

	Json::FastWriter writer;

	vector<HoldData> list;
	MysqlHold mysqlhold(userid);
	mysqlhold.Find_AllList(list);
	for (auto &item:list){
//		ret+=item.InstrumentID;

		 holddata["InstrumentID"] = item.InstrumentID;
		holddata["Direction"] = item.Direction;
		holddata["OpenPriceAverage"] = item.OpenPriceAverage;
		root.append(holddata);
	}
	array["replycode"]=0;
	array["array"] = Json::Value(root);

	ret = writer.write(array);
//	std::cout << "pack:"<<ret<<std::endl;
	vector<HoldData>().swap(list);
	return ret;
}

std::string ZeroMqUtil::PackError(int errorcode){

	std::string ret;
	Json::Value root;
	Json::FastWriter writer;
	root["replycode"]=errorcode;
	ret = writer.write(root);
	std::cout << "pack error:"<<ret<<std::endl;
	return ret;
}
