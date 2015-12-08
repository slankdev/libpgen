
include ../../../../makefile.config
include makefile.protocols

# CPPFLAGS = -Wall -fPIC -std=c++11 
# export CPP := clang++
# export RM  := rm -rf

.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)



INCLUDE_DIR = ../../../../include
PROTOCOL_OBJ = $(PROTOCOL_SRC:.cc=.o)
	
all: $(PROTOCOL_OBJ)



clean:
	$(RM) *.o 


