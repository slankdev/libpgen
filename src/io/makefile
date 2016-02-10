
include ../../config.make

INCLUDE_DIR = ../../include



.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)


SRC = arch.cc util.cc
OBJ = $(SRC:.cc=.o)

build: $(OBJ)
	
clean:
	$(RM) *.o
