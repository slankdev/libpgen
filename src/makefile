#makefile

export CC  := gcc
export CPP := g++ 
export CXX := g++ 
export RM  := rm -rf
export MKDIR := mkdir -p


.SUFFIXES: .out .c .cc .o .h 


program = a.out
SRC = main.cc \
	  pgen-funcs.cc \
	  packet.cc \
	  eth.cc \
	  arp.cc \
	  ip.cc \
	  icmp.cc \
	  tcp.cc 
HDR = pgen.h \
	  pgen-opcode.h \
	  pgen-macro.h \
	  pgen-variable.h \
	  pgen-funcs.h \
	  packet.h 
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall 


all: $(program)


$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)


.cc.o: 
	$(CPP) -std=c++11 -c  $<  -o $@  



#----------------------------------OTHERS--------------------------------------#

clean:
	$(RM) *.o *.ouat



