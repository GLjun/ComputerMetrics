CXX=g++
CXXFLAGS= -std=c++11
LDFLAGS=-L. -lprof -lunwind -lunwind-x86_64
SHAREFLAGS=-shared -fPIC

all:libprof.so main

libprof.so : GZProf.cpp 
	$(CXX) $(CXXFLAGS) $(SHAREFLAGS) $^ -o $@

main.o : main.cpp
	$(CXX) $(CXXFLAGS)  -c $< -o $@
main : main.o
	$(CXX) $(CXXFLAGS)  $< -o $@ $(LDFLAGS)

clean :
	rm -f ./libprof.so *.o ./test
