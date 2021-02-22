#include <getopt.h>
#include "baseinfo.hpp"
using std::cout;
using std::endl;


int main(){

	std::string sta = "turtle";
	ThreadTrade trader(sta.c_str());
	trader.Run();
}

