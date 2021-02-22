CC=g++
STD =--std=c++11
SOFLAGS = -lboost_regex -lmongoapi -ltradespi -ldl
LBOOSTDIR=/usr/lib/x86_64-linux-gnu
IBOOSTDIR=/usr/include/boost

CFLAGS=-pthread -I$(IBOOSTDIR) -L$(LBOOSTDIR)
SFLAGS=-fPIC -shared

all : clean so copy

so : 
	$(CC) $(STD) httpbase.cpp $(CFLAGS) $(SFLAGS)   -lboost_system  -ldl -o libhttp.so
	
http :  
	$(CC)  $(STD)  httpbase.cpp main.cpp $(CFLAGS) -lboost_system -o http 

copy :
	cp libhttp.so ../../lib
	cp httpbase.hpp ../../include	
	
clean :
	rm -f *.o  http