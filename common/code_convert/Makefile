all : compile copy clean

compile : code_convert.cpp 
	g++ -c code_convert.cpp  -fPIC -shared -o libcode_convert.so

copy : 
	cp libcode_convert.so ../../lib
	cp code_convert.h ../../include

clean :
	rm -f *.so *.out
