P=$(shell pwd)
CXX=gcc
SHFLAGS=-w -fPIC -shared
LDFLAGS=-ldl
CFLAGS=-w

all:libio.so main

libio.so: iotrace.c
	$(CXX) $(SHFLAGS) $< -o $@ $(LDFLAGS)

main: main.c
	$(CXX) $(CFLAGS) $< -o $@

run:
	LD_PRELOAD=$(P)/libio.so ./main

clean:
	rm -f *.o *.so ./main

