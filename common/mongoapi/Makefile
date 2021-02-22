MONGOCXX_INCLUDEDIR =/usr/local/include/mongocxx/v_noabi
BSCONCXX_INCLUDEDIR =/usr/local/include/bsoncxx/v_noabi
MONGOCXX_LIBS =/usr/local/lib
LIBS =../../lib/
STD =--std=c++11
LBOOSTDIR=/usr/lib/x86_64-linux-gnu
IBOOSTDIR=/usr/include/boost

CC=g++
CFLAGS=-I$(MONGOCXX_INCLUDEDIR) -I$(BSCONCXX_INCLUDEDIR) -I$(IBOOSTDIR) -L$(LBOOSTDIR) -L$(MONGOCXX_LIBS) -L$(LIBS)
SFLAGS=-fPIC -shared

all: clean api copy

compiler:
	g++ --std=c++11 mongoexchangeinfo.cpp test.cpp -o test \
    -I/usr/local/include/mongocxx/v_noabi \
    -I/usr/local/include/bsoncxx/v_noabi \
    -L/usr/local/lib -lmongocxx -lbsoncxx

main:
	$(CC) $(STD) main.cpp -o main $(CFLAGS) -lmongocxx -lbsoncxx -lmongoapi -ldl
	
api:
	$(CC) $(STD) mongoproductclass.cpp mongoexchangeinfo.cpp mongoinstrument.cpp  mongodepthmarket.cpp \
	mongomaincontract.cpp  mongotickdata.cpp mongomindata.cpp mongohxdayline.cpp  mongohxminuteline.cpp mongoaverageprice.cpp \
	$(CFLAGS) $(SFLAGS) -lboost_regex -lboost_system -lhttp -lmongocxx -lbsoncxx -ljson -o libmongoapi.so
	
copy :
	cp libmongoapi.so ../../lib
	cp *.hpp ../../include
	
    
clean:
	rm -f *.o insert test
