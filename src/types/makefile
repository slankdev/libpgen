
include ../../config.make

INCLUDE_DIR = ../../include



.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)


TYPES_SRC = data_container.cc
TYPES_OBJ = $(TYPES_SRC:.cc=.o)

build: $(TYPES_OBJ)
	
clean:
	$(RM) *.o
