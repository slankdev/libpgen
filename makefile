

include config.mk
INCLUDE_DIR = include 

IO_HDR = \
	stream.h \
	util.h

IO_SRC = \
	src/io/stream.cc \
	src/io/util.cc

TYPES_SRC    = \
	src/data_container.cc

ARCH_SRC = \
	src/arch/arch.cc

CORE_SRC = \
	src/core/address.cc \
	src/core/header.cc \
	src/core/packet.cc \
	src/core/protocol/ethernet.cc \
	src/core/protocol/ip.cc
	
ALL_SRC = $(IO_SRC) $(TYPES_SRC) $(ARCH_SRC) $(CORE_SRC) 
OBJ = $(ALL_SRC:.cc=.o)



all: libpgen2.a

libpgen2.a: $(OBJ)
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

clean:
	$(RM) libpgen2.a a.out
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
	./a.out

