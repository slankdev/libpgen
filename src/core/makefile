
include ../../config.make

INCLUDE_DIR = ../../include


.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)

CORE_SRC = address.cc header.cc
CORE_OBJ = $(CORE_SRC:.cc=.o)

build: $(CORE_OBJ) 
	
clean:
	$(RM) *.o
