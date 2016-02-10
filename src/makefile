
include ../config.make

IO_DIR   = io
CORE_DIR = core

INCLUDE_DIR = ../include




build: build-io build-core build-main

build-io:
	$(MAKE) -C io build

build-core:
	$(MAKE) -C core build

build-main:
	$(CPP) -I$(INCLUDE_DIR) \
		$(IO_DIR)/*.o \
		$(CORE_DIR)/*.o \
		main.cc
	
	
clean:
	$(MAKE) -C $(IO_DIR) clean
	$(MAKE) -C $(CORE_DIR) clean
	$(RM) *.o *.out


