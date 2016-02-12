
include config.mk

INCLUDE_DIR = include 

SRC_DIR   = src

# origin source
IO_DIR    = $(SRC_DIR)/io
CORE_DIR  = $(SRC_DIR)/core
TYPES_DIR = $(SRC_DIR)/types
PROTCOL_DIR = $(CORE_DIR)/protocol



OBJ = \
	$(IO_DIR)/*.o \
	$(CORE_DIR)/*.o \
	$(TYPES_DIR)/*.o \
	$(PROTCOL_DIR)/*.o



all: build build-main


build-main: 
	$(CPP) -I$(INCLUDE_DIR) $(OBJ)	main.cc


build: 
	$(MAKE) -C $(SRC_DIR) build


clean:
	$(RM) *.o *.out
	$(MAKE) -C $(SRC_DIR) clean
