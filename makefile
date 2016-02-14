
include config.mk

INCLUDE_DIR = include 

SRC_DIR   = src

OBJ = `find . -name "*.o"`

all: build-main

build-main:  build
	$(CPP) $(CPPFLAGS) -I$(INCLUDE_DIR) $(OBJ)	main.cc

build: 
	$(MAKE) -C $(SRC_DIR) build


clean:
	$(RM) *.o *.out
	$(MAKE) -C $(SRC_DIR) clean
