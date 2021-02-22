#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


//#include <cstdint>
//#include <cstdio>
//#include <iostream>
//#include <vector>
#include <string.h>
#include <string>
//#include <math.h>
//#include <vector>
//#include <regex.h>
//
//#include <dirent.h>
//
//
//#include <stdio.h>
//#include <unistd.h>
//#include <dirent.h>
//#include <stdlib.h>
//#include <sys/stat.h>
//#include <string.h>

#include "threadbase.hpp"

//#include <log4cplus/logger.h>
//#include <log4cplus/loggingmacros.h>
//#include <log4cplus/configurator.h>
//#include <iomanip>
//#include <log4cplus/fileappender.h>
//#include <log4cplus/consoleappender.h>
//#include <log4cplus/layout.h>
//#include <float.h>
//
//#include "mysqldata.hpp"
//#include "mysqldayline.hpp"
#include "mysqltestrecord.hpp"
//#include "mysqlcycle.hpp"
//#include "mysqlinstrument.hpp"
//#include "mysqlproduct.hpp"
//#include "mysqlhismindata.hpp"
//#include "mysqlutil.hpp"
//#include "mysqlhisatr.hpp"
//#include "mysqldatetime.hpp"
//#include "mysqlmaincontract.hpp"
#include "mysqltestreport.hpp"
//#include "mysqlhissection.hpp"
//#include "mysqlminutetime.hpp"
//#include "mysqlmindata.hpp"
#include "daytradetest.hpp"
//#include "trendtradetest.hpp"
//#include "sectiontrendtrade.hpp"
//
//#include "ticktoklineutil.hpp"
//#include "hisdata.hpp"
//
//#include <xlslib/xlslib.h>
//#include <xls.h>
//#include "csvfile.hpp"
//#include "daythread.hpp"

class DayThread:public ThreadBase
{
    public:
        void run();
        void Test(const char * year);
//        void Test(const char * year);
        void SetYear(const char * year);
        std::string  GetYear();
    private:
        std::string _year;
};
