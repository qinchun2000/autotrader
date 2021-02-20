INCS =../include/
LOCAL_INCS =./
LOG4PLUSE_INCS =/usr/local/include/log4cplus
LIBS =../lib/

CC =g++
STD =--std=c++11
SFLAGS= -shared
CFLAGS =-I$(INCS)  -L$(LIBS) 
LINKFLAGS = -lboost_regex -lboost_system -llog4cplus -ljson  -lmysqlcppconn -lhiredis 

CC = g++ --std=c++11
GCC_SO = g++ --std=c++11 -fPIC -c
LINK = g++

#期望得到的执行文件或动态库.so
TARGET_CODECONVERT=libcode_convert.so
TARGET_MYUTIL=libmyutil.so
TARGET_HTTP=libmyhttp.so
TARGET_LOCK=liblock.so
TARGET_MSBASE=libmysqlbase.so
TARGET_MSAPI=libmysqlapi.so
TARGET_MSTEST=libmysqltest.so
TARGET_REDIS=libredisapi.so
TARGET_MD=libmdspi.so
TARGET_TD=libtradespi.so
TARGET_REPORT=libreport.so
TARGET_STA=libstrategy.so
TARGET_TRATEST=libtradetest.so
TARGET_ZEROMQUTIL=libzeromqutil.so
TARGET_EVENTENGINE=libeventengine.so
TARGET_GATEWAY=libgateway.so
TARGET_CTAMANAGER=libcta.so

CODECONVERT_SRCS = code_convert/code_convert.cpp
CODECONVERT_OBJS = $(CODECONVERT_SRCS:.cpp=.o) 

MYUTIL_SRCS = util/code_convert.cpp util/codeutil.cpp util/dateutil.cpp util/logutil.cpp util/csvfile.cpp util/ticktoklineutil.cpp util/config.cpp util/lock.cpp util/threadbase.cpp util/logbase.cpp
MYUTIL_OBJS = $(MYUTIL_SRCS:.cpp=.o) 

EVENTENGINE_SRCS = engine/eventengine.cpp
EVENTENGINE_OBJS =$(EVENTENGINE_SRCS:.cpp=.o)

GATEWAY_SRCS = mgateway/gatewaybase.cpp mgateway/tdspi.cpp mgateway/ctpgateway.cpp  mgateway/gatewaymanager.cpp
GATEWAY_OBJS =$(GATEWAY_SRCS:.cpp=.o)

DEPS = mysqlbase/mysqldata.hpp
MSBASE_SRCS = mysqlbase/mysqlbase.cpp mysqlbase/mysqlinstrument.cpp mysqlbase/mysqldepthmarket.cpp mysqlbase/mysqlproduct.cpp mysqlbase/mysqlmaincontract.cpp \
	mysqlbase/mysqlexchange.cpp mysqlbase/mysqlasset.cpp  mysqlbase/mysqltradingday.cpp mysqlbase/mysqlinvest.cpp  mysqlbase/mysqlposdetail.cpp mysqlbase/mysqluser.cpp \
	mysqlbase/mysqlsubscribe.cpp mysqlbase/mysqlsection.cpp mysqlbase/mysqlglobalview.cpp mysqlbase/mysqlsignal.cpp
MSBASE_OBJS = $(MSBASE_SRCS:.cpp=.o) 

MSAPI_SRCS = mysqlapi/mysqlmindata.cpp mysqlapi/mysqldayline.cpp mysqlapi/mysqlcycle.cpp mysqlapi/mysqlma.cpp mysqlapi/mysqlorder.cpp  \
	mysqlapi/mysqlhold.cpp mysqlapi/mysqlhisma.cpp  mysqlapi/mysqlhisema.cpp mysqlapi/mysqlmdlogin.cpp mysqlapi/mysqlhisatr.cpp \
	mysqlapi/mysqlhismindata.cpp mysqlapi/mysqlaveprice.cpp mysqlapi/mysqlrecord.cpp mysqlapi/mysqlutil.cpp mysqlapi/mysqlema.cpp \
	mysqlapi/mysqlmarket.cpp mysqlapi/thread.cpp mysqlapi/mysqlsecassess.cpp
MSAPI_OBJS = $(MSAPI_SRCS:.cpp=.o) 

MSTEST_SRCS =  mysqltest/mysqltestrecord.cpp mysqltest/mysqldatetime.cpp mysqltest/mysqlminutetime.cpp  mysqltest/mysqltestreport.cpp mysqltest/mysqlhissection.cpp \
	mysqltest/mysqlcommission.cpp mysqltest/mysqltestasset.cpp mysqltest/mysqltestsignal.cpp mysqltest/mysqltestsecassess.cpp mysqltest/mysqlprostatis.cpp 
MSTEST_OBJS = $(MSTEST_SRCS:.cpp=.o)

REDIS_SRCS =redisapi/redisbase.cpp redisapi/redisminflag.cpp redisapi/redismd.cpp redisapi/redisstatus.cpp redisapi/redissignal.cpp redisapi/redissection.cpp \
	redisapi/redismindata.cpp
REDIS_OBJS = $(REDIS_SRCS:.cpp=.o)

MD_SRCS = mdapi/mdsem.cpp mdapi/mdspi.cpp mdapi/marketbase.cpp mdapi/market.cpp
MD_OBJS = $(MD_SRCS:.cpp=.o)

TD_SRCS =  tradeapi/sembase.cpp tradeapi/sysbase.cpp tradeapi/tradespi.cpp  tradeapi/mytdspi.cpp tradeapi/daytrader.cpp tradeapi/tradebase.cpp  tradeapi/threadtrade.cpp \
	tradeapi/trader.cpp 
TD_OBJS = $(TD_SRCS:.cpp=.o) 

CTAMANAGER_SRCS = cta/ctaapibase.cpp cta/ctamanager.cpp  
CTAMANAGER_OBJS =$(CTAMANAGER_SRCS:.cpp=.o)

REPORT_SRCS = report/report.cpp
REPORT_OBJS =$(REPORT_SRCS:.cpp=.o)

STA_SRCS =  strategy/tech.cpp strategy/assetbase.cpp strategy/strategybase.cpp strategy/strategy.cpp  \
	strategy/secassess.cpp  strategy/datawrapper.cpp strategy/statest.cpp strategy/turtle.cpp 
STA_OBJS =$(STA_SRCS:.cpp=.o) 

TRATEST_SRCS = tradetest/testbase.cpp tradetest/tradetest.cpp tradetest/daytradetest.cpp  tradetest/trendtradetest.cpp  tradetest/sectiontrendtrade.cpp tradetest/hisdata.cpp \
	tradetest/daythread.cpp tradetest/realtradetest.cpp
TRATEST_OBJS =$(TRATEST_SRCS:.cpp=.o)

TARGETS=$(TARGET_MYUTIL)  $(TARGET_REDIS) $(TARGET_MSBASE) $(TARGET_MSAPI) $(TARGET_MSTEST) \
	 $(TARGET_EVENTENGINE) $(TARGET_GATEWAY) $(TARGET_STA)  $(TARGET_MD) $(TARGET_CTAMANAGER)  $(TARGET_TD)  
OBJS=$(MYUTIL_OBJS) $(REDIS_OBJS) $(MSBASE_OBJS) $(MSAPI_OBJS) $(MSTEST_OBJS) \
	 $(MD_OBJS) $(STA_OBJS) $(TD_OBJS) $(TRATEST_OBJS) $(EVENTENGINE_OBJS) $(GATEWAY_OBJS) $(CTAMANAGER_OBJS)

LIB_6_3_15 = ../thirdparty/v6.3.15_20190220_api_tradeapi_se_linux64
LIB_6_3_11 = ../thirdparty/v6.3.11_20180109_api_tradeapi_linux64

all: copyhpp_base $(TARGETS)  copy
rebuild: clean all
ct: copyctlib6315 $(TARGETS)  copy 
sim: copyctlib6311 $(TARGETS)  copy 

copy: copyhpp copylib 
copyhpp_base:
	cp mysqlbase/depthmarket.hpp ../include
	cp mysqlbase/mysqldata.hpp ../include
	cp mdapi/marketbase.hpp ../include
	cp mdapi/mdspi.hpp ../include
	cp mdapi/mdsem.hpp ../include
	cp mgateway/gatewaymanager.hpp ../include
	cp mgateway/ctpgateway.hpp ../include
	cp mgateway/gatewaybase.hpp ../include
	cp mgateway/tdspi.hpp ../include
	cp tradeapi/tradespi.hpp ../include

copyhpp:
	cp engine/*.hpp ../include
	cp mgateway/*.hpp ../include
	cp cta/*.hpp ../include
copylib:
	cp lib*.so ../lib
copyctlib6315:
	cp $(LIB_6_3_15)/lib*.so ../lib
	cp $(LIB_6_3_15)/*.h ../include
	cp $(LIB_6_3_15)/*.xml ../include
	cp $(LIB_6_3_15)/*.dtd ../include
copyctlib6311:
	cp $(LIB_6_3_11)/lib*.so ../lib
	cp $(LIB_6_3_11)/*.h ../include
	cp $(LIB_6_3_11)/*.xml ../include
	cp $(LIB_6_3_11)/*.dtd ../include

$(TARGET_CODECONVERT): $(CODECONVERT_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -ldl -o $@
	cp code_convert/*.h ../include
	cp $@ ../lib
	@echo "-------------------->TARGET_CODECONBERT lib compile ok!"
	@echo " "
	 
$(TARGET_MYUTIL): $(MYUTIL_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -ldl -o $@
	cp util/*.hpp ../include
	cp $@ ../lib
	@echo "-------------------->TARGET_MYUTIL lib compile ok!"
	@echo " "

$(TARGET_HTTP): $(HTTP_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -ldl -o $@
	cp httpboost/httpbase.hpp  httpboost/cping.hpp ../include
	cp $@ ../lib
	@echo "-------------------->TARGET_HTTP lib compile ok!"
	@echo " "

$(TARGET_REDIS): $(REDIS_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -ldl -ljson -o $@
	cp redisapi/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_REDIS lib compile ok!"
	@echo " "

$(TARGET_MSBASE): $(MSBASE_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmyutil -ldl -o $@
	cp mysqlbase/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_MSBASE lib compile ok!"
	@echo " "
$(TARGET_MSAPI): $(MSAPI_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmyutil -lmysqlbase  -ldl -o $@
	cp mysqlapi/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_MSAPI lib compile ok!"
	@echo " "
$(TARGET_MSTEST): $(MSTEST_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmyutil -lmysqlbase  -ldl -o $@
	cp mysqltest/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_MSTEST lib compile ok!"
	@echo " "
$(TARGET_MD): $(MD_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmysqlbase -lmysqlapi -lmysqltest -lmyutil -llog4cplus -lthostmduserapi -lredisapi -lstrategy  -ldl -o $@
	cp mdapi/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_MD lib compile ok!"
	@echo " "
$(TARGET_TD): $(TD_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmysqlbase -lmysqlapi -lthosttraderapi   -lthostmduserapi -lmdspi  -leventengine -lgateway -lcta -ldl -o $@
	cp tradeapi/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_TD lib compile ok!"
	@echo " "

$(TARGET_STA): $(STA_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmyutil -lmysqlbase -lmysqlapi -lmysqltest -lredisapi -ldl -o $@
	cp strategy/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_STA lib compile ok!"
	@echo " "
$(TARGET_ZEROMQUTIL): $(ZEROMQUTIL_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmyutil -lmysqlbase -lmysqlapi -lmysqltest  -ldl -o $@
	cp zeromq/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_ZEROMQUTIL lib compile ok!"
	@echo " "

$(TARGET_EVENTENGINE): $(EVENTENGINE_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -ldl -o $@
	cp engine/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_EVENTENGINE lib compile ok!"
	@echo " "

$(TARGET_GATEWAY): $(GATEWAY_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lthosttraderapi  -leventengine -ldl -o $@
	cp engine/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_GATEWAY lib compile ok!"
	@echo " "

$(TARGET_CTAMANAGER): $(CTAMANAGER_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -leventengine -lgateway -ldl -o $@
	cp cta/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_CTAMANAGER lib compile ok!"
	@echo " "

$(TARGET_TRATEST): $(TRATEST_OBJS)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) $(LINKFLAGS) -lmyutil -lmysqlbase -lmysqlapi -lmysqltest  -lredisapi -ldl -o $@
	cp tradetest/*.hpp ../include
	cp $@ ../lib
	@echo "------------------>TARGET_TRATEST lib compile ok!"
	@echo " "

%.o:%.cpp $(DEPS)
	$(GCC_SO) -o $@ $< $(CFLAGS) 
clean:
	rm -rf $(TARGETS) 
	rm -rf $(OBJS)

