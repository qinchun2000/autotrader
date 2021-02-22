INCS =../../include/
BOOST_INCS =/usr/include/boost
LOG4PLUSE_INCS =/usr/local/include/log4cplus
XLS_INCS =/usr/local/include/xlslib/

LIBS =../../lib/
LOCAL_LIBS =/usr/local/lib
BOOST_LIBS =/usr/lib/x86_64-linux-gnu


CC =g++
STD =--std=c++11
SFLAGS=-fPIC -shared
CFLAGS =-I$(INCS)  -I$(BOOST_INCS) -I$(LOG4PLUSE_INCS) -I$(XLS_INCS) -L$(LIBS) -L$(BOOST_LIBS) -L$(LOCAL_LIBS) 


CC = g++ --std=c++11
GCC_SO = g++ --std=c++11 -fPIC -c
LINK = g++ -shared

#期望得到的执行文件或动态库.so
TARGET=libmyutil.so 

C++FILES = $(wildcard *.cpp ./*.cpp)

SRCS = codeutil.cpp dateutil.cpp smtputil.cpp logutil.cpp csvfile.cpp ticktoklineutil.cpp config.cpp threadbase.cpp logbase.cpp
OBJFILE = $(SRCS:.cpp=.o)
 
all:$(TARGET)
 
$(TARGET): $(OBJFILE)
	$(LINK) $^ $(CFLAGS) $(SFLAGS) -lboost_regex -lcurl -lmimetic -llog4cplus -lxls -ldl -o $@
 
%.o:%.cpp
	$(GCC_SO) -o $@ $< $(CFLAGS) 
clean:
	rm -rf $(TARGET)
	rm -rf $(OBJFILE)

