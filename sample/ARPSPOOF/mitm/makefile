#sample makefile using libpgen

export CC := gcc
export CPP := g++
export CXX := g++
export RM := rn -rf

OPTION=-L/home/slank/git/libpgen/lib/ -I/home/slank/git/libpgen/lib/ 
program = a.out


all: program


program: main.cc
	$(CPP)   main.cc $(OPTION) -lpgen

clean:
	$(RM) *.o *.out 




