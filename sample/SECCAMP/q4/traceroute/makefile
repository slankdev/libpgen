#sample makefile using libpgen

export CC := gcc
export CPP := g++
export CXX := g++
export RM := rm -rf

OPTION=-L/home/slank/git/libpgen/lib/ -I/home/slank/git/libpgen/lib/ 
program = a.out


all: program


program: main.cc
	$(CPP) -std=c++11 -o sp-traceroute  main.cc  $(OPTION) -lpgen -lpthread -lpcap

clean:
	$(RM) *.o *.out 




