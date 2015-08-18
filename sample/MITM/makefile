#sample makefile using libpgen

export CC := gcc
export CPP := g++
export CXX := g++
export RM := rm -rf

OPTION=-L/home/slank/git/libpgen/src/ -I/home/slank/git/libpgen/src/ 
program = a.out


all: a.out


program: main.cc
	$(CPP)    main.cc -std=c++11 $(OPTION) -lpgen -lpcap -lpthread -o $(program)

clean:
	$(RM) *.o *.out 


install:
	$(CP) $(program) /usr/local/bin/ 
	
uninstall:
	rm /usr/local/$(program)



