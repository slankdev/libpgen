

include config.mk
INCLUDE_DIR = include 


SRC_DIR = src
IO_DIR       = $(SRC_DIR)/io
TYPES_DIR    = $(SRC_DIR)/types
UTIL_DIR     = $(SRC_DIR)/util
ARCH_DIR     = $(SRC_DIR)/arch
CORE_DIR     = $(SRC_DIR)/core
PROTOCOL_DIR = $(CORE_DIR)/protocol

IO_SRC       = $(IO_DIR)/*.cc
TYPES_SRC    = $(TYPES_DIR)/*.cc
UTIL_SRC     = $(UTIL_DIR)/*.cc
ARCH_SRC     = $(ARCH_DIR)/*.cc
CORE_SRC     = $(CORE_DIR)/*.cc
PROTOCOL_SRC = $(PROTOCOL_DIR)/*.cc

SRC = $(IO_SRC) $(TYPES_SRC) $(UTIL_SRC) $(ARCH_SRC) $(CORE_SRC) $(PROTOCOL_SRC)
OBJ = $(SRC:.c=.o)



all: libpgen2.a

libpgen2.a: build
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@


build: $(OBJ)







clean:
	$(RM) $(OBJ)









install:
	$(CP) libpgen2.a      $(INSTALL_LIB_DIR)
	$(CP) include/pgen2.h $(INSTALL_HDR_DIR)
	$(CP) include/pgen2   $(INSTALL_HDR_DIR)

uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libpgen2.a
	$(RM) $(INSTALL_HDR_DIR)/pgen2.h
	$(RM) $(INSTALL_HDR_DIR)/pgen2 

main:
	g++ -std=c++11 main.cc -lpgen2 -Iinclude -L./ -lpgen2
