CC=g++
STD =--std=c++11
SOFLAGS = -lboost_regex -lmongoapi -ltradespi -ldl
LBOOSTDIR=/usr/lib/x86_64-linux-gnu
IBOOSTDIR=/usr/include/boost

CFLAGS=-pthread -I$(IBOOSTDIR) -L$(LBOOSTDIR)
SFLAGS=-fPIC -shared

all : clean so copy

so : 
	$(CC) $(STD) mysqlmindata.cpp $(CFLAGS) $(SFLAGS)   -lboost_system  -ldl -o libmysqlapi.so
	


copy :
	cp libmysqlapi.so ../../lib
	cp mysqlmindata.hpp ../../include	
	
clean :
	rm -f *.so 
