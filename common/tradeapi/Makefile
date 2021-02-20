MONGOCXX_INCLUDEDIR =/usr/local/include/mongocxx/v_noabi
BSCONCXX_INCLUDEDIR =/usr/local/include/bsoncxx/v_noabi
MONGOCXX_LIBS =/usr/local/lib
MONGOFLAGS = -I$(IBOOSTDIR) -L$(LBOOSTDIR) -I$(MONGOCXX_INCLUDEDIR) -I$(BSCONCXX_INCLUDEDIR) -L$(MONGOCXX_LIBS) -lmongocxx -lbsoncxx
IDIR =../../include
LIBS =../../lib/
LBOOSTDIR=/usr/lib/x86_64-linux-gnu
IBOOSTDIR=/usr/include/boost

CC=g++
CFLAGS=-pthread -I$(IDIR) -L$(LIBS) 
SFLAGS=-fPIC -shared
STD =--std=c++11


all : clean so copy
so : 
	$(CC) $(STD) tradespi.cpp  tradespi.cpp $(CFLAGS) $(SFLAGS)  $(MONGOFLAGS) -lboost_regex -lmongoapi -ljson -ldl -o libtradespi.so


copy :
	cp libtradespi.so ../../lib
	cp tradespi.hpp ../../include
		
clean :
	rm -f *.so