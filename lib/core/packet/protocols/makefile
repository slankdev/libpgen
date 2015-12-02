

include makefile.protocols
export CPP := clang++
export RM  := rm -rf


CPPFLAGS = -Wall -fPIC -std=c++11 
PROTOCOL_OBJ = $(PROTOCOL_SRC:.cc=.o)
INCLUDE_DIR = ../../../../include
	
.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)



all: $(PROTOCOL_OBJ)



clean:
	$(RM) *.o 


