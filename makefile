
include config.make

INCLUDE_DIR = include 

SRC_DIR   = src

IO_DIR    = $(SRC_DIR)/io
CORE_DIR  = $(SRC_DIR)/core
TYPES_DIR = $(SRC_DIR)/types


OBJ = \
	$(IO_DIR)/*.o \
	$(CORE_DIR)/*.o \
	$(TYPES_DIR)/*.o 


.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  $(INCLUDE)

all: build build-main


build-main: 
	$(CPP) -I$(INCLUDE_DIR) $(OBJ)	main.cc


build: 
	$(MAKE) -C $(SRC_DIR) build


clean:
	$(MAKE) -C $(SRC_DIR) clean
