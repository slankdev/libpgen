#sample makefile using libpgen

export CC := gcc
export CPP := g++
export CXX := g++
export RM := rm -rf



OPTION=-L/home/slank/git/libpgen/src/ -I/home/slank/git/libpgen/src/ 
program = dronemitm


all: program


program: main.cc
	$(CPP) -o $(program)  main.cc -std=c++11 $(OPTION) -lpgen -lpcap -lpthread

clean:
	$(RM) *.o *.out 


install: program
	cp $(program) /usr/local/bin/



