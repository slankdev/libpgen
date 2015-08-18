#sample makefile using libpgen

export CP := cp -rf
export CC := gcc
export CPP := g++
export CXX := g++
export RM := rm -rf

program = mitm2


all: program


program: main.cc
	$(CPP)    main.cc -std=c++11 $(OPTION) -lpgen -o $(program)

clean:
	$(RM) *.o *.out $(program)


install:
	$(CP) $(program)  /usr/local/bin/
	
uninstall:
	$(RM) /usr/local/$(program)



