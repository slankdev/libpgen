


_SRC_DIR   = src
_IO_DIR    = $(SRC_DIR)/io
_CORE_DIR  = $(SRC_DIR)/core
_TYPES_DIR = $(SRC_DIR)/types

export CPP := clang++
export AR  := ar
export CP  := cp -rf
export RM  := rm -rf
export MKDIR  := mkdir -p
export RANLIB := ranlib
export MAKE := make

CPPFLAGS = -Wall -fPIC -std=c++11 -Wextra


.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)
