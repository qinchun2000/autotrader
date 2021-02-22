INCS =../include
LOG4PLUSE_INCS=/usr/local/include/log4cplus
LIBS =../lib
LOCAL_LIBS =/usr/local/lib
TRADE_LIBS =/root/autotrader/lib

CC=g++
STD =--std=c++11
SFLAGS=-fPIC -shared
#CFLAGS=-pthread -I$(INCS)  -I$(BOOST_INCS)  -I$(MYSQL_INCS) -L$(LIBS)   -L$(LOCAL_LIBS) -L$(BOOST_LIBS) 
CFLAGS=-pthread -I$(INCS)  -L$(LIBS) 
LOGFLAGS=-I$(LOG4PLUSE_INCS)
BASEFLAGS = -lredisapi -lmyutil -lmysqlbase -lmysqlapi -lmysqltest -lmdspi -ltradespi  -lstrategy 
SOFLAGS = -lboost_regex -lboost_system  -llog4cplus  -lmysqlcppconn -ljson  -lthosttraderapi  -lthostmduserapi 
MDSOFLAGS = -lthosttraderapi -lthostmduserapi -lboost_regex -lboost_system -lmyutil -llog4cplus \
        -lhiredis -ljson -lredisapi -lmysqlcppconn -lmysqlbase -lmysqlapi -lmysqltest -lstrategy -lmdspi -leventengine -lgateway -lcta -ltradespi -ldl
        
TESTFLAGS=-pthread -I$(IDIR) -L$(LIBS)


TARGET_BASE = baseinfo
TARGET_CYCLE = cycle
TARGET_TREND = trend
TARGET_MDSERVICE = mdservice
TARGET_MDNEXT = mdnext

TARGET_TEST = ctptest
TARGET_DATA = data


BASE_SRCS = baseinfo.cpp baseinfo.hpp
BASE_OBJS =$(BASE_SRCS:.cpp=.o)

CYCLE_SRCS = trendcyclen.cpp
CYCLE_OBJS =$(CYCLE_SRCS:.cpp=.o)

TREND_SRCS = trend.cpp
TREND_OBJS =$(TREND_SRCS:.cpp=.o)

TEST_SRCS = ctptest.cpp
TEST_OBJS =$(TEST_SRCS:.cpp=.o)

MDSERVICE_SRCS = mdservice.cpp
MDSERVICE_OBJS =$(MDSERVICE_SRCS:.cpp=.o)

MDNEXT_SRCS = msnext.cpp
MDNEXT_OBJS =$(MDNEXT_SRCS:.cpp=.o)

DATA_SRCS = data.cpp
DATA_OBJS =$(DATA_SRCS:.cpp=.o)

ALL_TARGETS = $(TARGET_BASE) $(TARGET_MDSERVICE) 

%.o:%.cpp
	$(CC) $(STD) -o $@ -c $<   $(CFLAGS) $(XLSFLAGS) $(LOGFLAGS) $(SOFLAGS)  $(BASEFLAGS) 
	
all : $(ALL_TARGETS) copy

rebuild: clean all
t : 
	rm -f test	
	$(CC) $(STD)  check.cpp   $(CFLAGS) $(XLSFLAGS) $(LOGFLAGS)  $(SOFLAGS)   $(BASEFLAGS)  -ldl  -o test 	
zlqhtest :  ctptest.cpp ctptest.hpp
	$(CC)  $(STD) ctptest.cpp $(CFLAGS) $(XLSFLAGS) $(LOGFLAGS) $(SOFLAGS)  $(BASEFLAGS) -ldl -o  ctptest  
	@echo "hello"
	
$(TARGET_BASE): $(BASE_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(LOGFLAGS) $(SOFLAGS)  $(BASEFLAGS) -leventengine -lgateway -lcta -ldl  -o  $@	
	@echo "-------------------->TARGET_BASE compile ok!"
	@echo " "

$(TARGET_CYCLE): $(CYCLE_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(XLSFLAGS) $(LOGFLAGS) $(SOFLAGS)  $(BASEFLAGS)  -leventengine -lgateway -lcta  -ldl  -o  $@
	@echo "-------------------->TARGET_CYCLE compile ok!"
	@echo " "
	
$(TARGET_TREND): $(TREND_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(XLSFLAGS) $(LOGFLAGS) $(SOFLAGS)  $(BASEFLAGS)  -leventengine -lgateway -lcta -ldl  -o  $@
	@echo "-------------------->TARGET_TREND compile ok!"
	@echo " "
	
$(TARGET_TEST): $(TEST_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(XLSFLAGS) $(LOGFLAGS) $(SOFLAGS)  $(BASEFLAGS)  -leventengine -lgateway -lcta -ldl  -o  $@
	@echo "-------------------->TARGET_TEST compile ok!"
	@echo " "
	
$(TARGET_MDSERVICE): $(MDSERVICE_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(MDSOFLAGS)  -ldl  -o  $@
	@echo "-------------------->TARGET_MDSERVICE compile ok!"
	@echo " "
	
$(TARGET_MDNEXT): $(MDNEXT_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(XLSFLAGS) $(MDSOFLAGS)  -ldl  -o  $@
	@echo "-------------------->TARGET_MDNEXT compile ok!"
	@echo " "	
	
$(TARGET_DATA): $(DATA_OBJS)
	$(CC)  $(STD)  $^  $(CFLAGS) $(XLSFLAGS) $(MDSOFLAGS)  -ldl  -o  $@
	@echo "-------------------->TARGET_DATA compile ok!"
	@echo " "		

mailtest : 
	$(CC)  smtp.cpp  $(TESTFLAGS) -lcurl -lmimetic  -o mailtest 

nettest : test.cpp
	rm -f nettest
	$(CC)  test.cpp  $(TESTFLAGS) -lcurl -o nettest 
cleantest:
	rm -f test
copy : 
	cp  $(TARGET_BASE) ../bin
	cp  $(TARGET_MDSERVICE) ../bin
clean :
	rm -f *.o    $(ALL_TARGETS) 
