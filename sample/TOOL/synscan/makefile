#sample makefile using libpgen

export CP := cp -rf
export CC := gcc
export CPP := g++
export CXX := g++
export RM := rm -rf

program = a.out

all: program


program: main.cc
	$(CPP) -std=c++11  main.cc $(OPTION) -lpgen -o $(program) -lpthread

clean:
	$(RM) *.o *.out  $(program)


install:
	$(CP) $(program) /usr/local/bin/ 
	
uninstall:
	$(RM) /usr/local/$(program)

