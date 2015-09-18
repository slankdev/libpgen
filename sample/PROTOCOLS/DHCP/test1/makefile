#sample makefile using libpgen

export CP := cp -rf
export CPP := clang++
export RM := rm -rf

program = a.out

all: program


program: main.cc
	$(CPP) main.cc $(OPTION) -lpgen -o $(program)

clean:
	$(RM) *.o *.out  $(program)


install:
	$(CP) $(program) /usr/local/bin/ 
	
uninstall:
	$(RM) /usr/local/$(program)

