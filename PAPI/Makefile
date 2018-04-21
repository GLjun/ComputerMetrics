CC=gcc
ROOT=/home/zhenyanjie/gaowei_zhenbao/install/papiinstall
CFLAGS=  -std=c99 -I$(ROOT)/include
LDFLAGS= -lpapi -L$(ROOT)/lib

all: ipc l1 l2 tlb br

ipc: prime_sieve-64.c
	$(CC) $(CFLAGS) -DIPC $< -o $@ $(LDFLAGS)

l1: prime_sieve-64.c
	$(CC) $(CFLAGS) -DL1 $< -o $@ $(LDFLAGS)

l2: prime_sieve-64.c
	$(CC) $(CFLAGS) -DL2 $< -o $@ $(LDFLAGS)

tlb: prime_sieve-64.c
	$(CC) $(CFLAGS) -DTLB $< -o $@ $(LDFLAGS)

br: prime_sieve-64.c
	$(CC) $(CFLAGS) -DBR $< -o $@ $(LDFLAGS)

run:
	./ipc
	./l1
	./l2
	./tlb
	./br

clean:
	rm -f ./ipc ./l1 ./l2 ./tlb ./br

