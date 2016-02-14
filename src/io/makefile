
include ../../config.mk
INCLUDE_DIR = ../../include


ARCH_DIR = arch
UTIL_DIR = util


SRC = stream.cc
OBJ = $(SRC:.cc=.o)


build: build-sub $(OBJ)
	
build-sub: 
	$(MAKE) -C $(ARCH_DIR) build
	$(MAKE) -C $(UTIL_DIR) build

clean:
	$(MAKE) -C $(ARCH_DIR) clean
	$(MAKE) -C $(UTIL_DIR) clean
	$(RM) *.o
	
