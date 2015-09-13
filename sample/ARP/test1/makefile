#sample makefile using libpgen

export CP := cp -rf
export CPP := g++-4.9
export CXX := g++
export RM := rm -rf

program = a.out

all: program


program: main.cc
	$(CPP) -g  main.cc $(OPTION) -lpgen -o $(program) -I/usr/local/include -L/usr/local/lib

clean:
	$(RM) *.o *.out  $(program)


install:
	$(CP) $(program) /usr/local/bin/ 
	
uninstall:
	$(RM) /usr/local/$(program)

