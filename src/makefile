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
	  tcp.cc \
	  udp.cc \
	  netutil.cc
HDR = pgen.h \
	  pgen-opcode.h \
	  pgen-macro.h \
	  address.h \
	  pgen-funcs.h \
	  packet.h \
	  types.h
OBJ = $(SRC:.cc=.o)
CFLAGS = -Wall 


all: $(program)


$(program): $(OBJ)
	$(CPP) $(OBJ) -o $(program) $(CFLAGS)
#	$(RM) *.o


.cc.o: 
	$(CPP) -std=c++11 -c  $<  -o $@  



#----------------------------------OTHERS--------------------------------------#

clean:
	$(RM) *.o *.out



