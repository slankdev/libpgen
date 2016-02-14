
include ../../config.mk
INCLUDE_DIR = ../../include


UTIL_DIR = util


SRC = stream.cc
OBJ = $(SRC:.cc=.o)


build: build-sub $(OBJ)
	
build-sub: 
	$(MAKE) -C $(UTIL_DIR) build

clean:
	$(MAKE) -C $(UTIL_DIR) clean
	$(RM) *.o
	
