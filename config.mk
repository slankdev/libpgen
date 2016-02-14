


export CPP := clang++
export AR  := ar
export CP  := cp -rf
export RM  := rm -rf
export MKDIR  := mkdir -p
export RANLIB := ranlib
export MAKE := make

INSTALL_LIB_DIR = /usr/local/lib
INSTALL_HDR_DIR = /usr/local/include


CPPFLAGS = -fPIC -std=c++11 -Wextra


.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)


