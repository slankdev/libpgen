#sample makefile using libpgen

export CC := gcc
export CPP := g++
export CXX := g++
export RM := rm -rf

OPTION=-L/home/slank/git/libpgen/src/ -I/home/slank/git/libpgen/src/ 
program = a.out


all: program


program: main.cc
	$(CPP) -std=c++11 main.cc $(OPTION) -lpgen -lpthread -lpcap

clean:
	$(RM) *.o *.out 




