BASE = ..

include makefile.protocols
export CPP := g++ 

.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(BASE)

CPPFLAGS = -Wall -fPIC -std=c++11 
PROTOCOL_OBJ = $(PROTOCOL_SRC:.cc=.o)
	


all: $(PROTOCOL_OBJ)

clean:
	$(RM) *.o 
