MONGOCXX_INCLUDEDIR =/usr/local/include/mongocxx/v_noabi
BSCONCXX_INCLUDEDIR =/usr/local/include/bsoncxx/v_noabi
MONGOCXX_LIBS =/usr/local/lib
MONGOFLAGS = -I$(MONGOCXX_INCLUDEDIR) -I$(BSCONCXX_INCLUDEDIR) -L$(MONGOCXX_LIBS) -lmongocxx -lbsoncxx
IDIR =../../include
LIBS =../../lib/
CC=g++
CFLAGS=-pthread -I$(IDIR) -L$(LIBS)
SFLAGS=-fPIC -shared
STD =--std=c++11


all : clean so copy
so : 
	$(CC) $(STD) mdspi.cpp minflag.cpp redismd.cpp $(CFLAGS) $(SFLAGS)  $(MONGOFLAGS) -lmysqlbase -lmysqlapi -lmongoapi -lhiredis -ljson -ldl -o libmdspi.so


copy :
	cp libmdspi.so ../../lib
	cp *.hpp ../../include
		
clean :
	rm -f *.so