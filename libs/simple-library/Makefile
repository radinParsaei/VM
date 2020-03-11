all: libstring

libstring:
	g++ -c ../../VM.cpp -std=c++17 -ldl -fPIC
	g++ -shared -fPIC -o libstring.so simpleStringLib.cpp VM.o -std=c++17
