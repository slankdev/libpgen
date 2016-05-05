



export CPP             := g++
export CPPFLAGS        := -std=c++11 -Wextra 
export INCLUDE_DIR     := include 
export AR              := ar 
export RANLIB          := ranlib
export MAKE            := make
export CP              := cp -rf
export RM              := rm -rf
export MKDIR           := mkdir -p
export INSTALL_LIB_DIR := /usr/local/lib
export INSTALL_HDR_DIR := /usr/local/include


.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  -I$(INCLUDE_DIR)







UTIL_SRC = \
	src/util.cc
UTIL_OBJ = $(UTIL_SRC:.cc=.o)



IO_SRC = \
	src/io/stream.cc \
	src/io/file_stream.cc \
	src/io/pcap_stream.cc \
	src/io/pcapng_stream.cc \
	src/io/net_stream.cc
IO_OBJ = $(IO_SRC:.cc=.o)


CORE_SRC = \
	src/core/data_container.cc \
	src/core/macaddress.cc \
	src/core/ipaddress.cc \
	src/core/packet.cc \
	src/core/protocol/ethernet.cc \
	src/core/protocol/arp.cc \
	src/core/protocol/ipv6.cc \
	src/core/protocol/ip.cc \
	src/core/protocol/zundoko.cc \
	src/core/protocol/udp.cc
CORE_OBJ = $(CORE_SRC:.cc=.o)


MODULE_SRC = \
	src/module/module.cc
MODULE_OBJ = $(MODULE_SRC:.cc=.o)


SRC = $(IO_SRC) $(CORE_SRC) $(MODULE_SRC) $(UTIL_SRC) 
OBJ = $(IO_OBJ) $(CORE_OBJ) $(MODULE_OBJ) $(UTIL_OBJ) 





all: libpgen2.a test-code

build-base:   $(UTIL_OBJ)
build-core:   $(CORE_OBJ)
build-io:     $(IO_OBJ)
build-module: $(MODULE_OBJ)


# libpgen2.a: $(OBJ)
libpgen2.a: build-base build-io build-core build-module
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@



test-code:
	$(MAKE) -C test


.PHONY: clean
clean:
	$(RM) libpgen2.a a.out
	$(RM) $(OBJ) 



.PHONY: depend
depend:
	@echo "Create Dependfile"
	$(CPP) -MM -MG $(SRC) $(CPPFLAGS) -I$(INCLUDE_DIR) > depend.mk

-include depend.mk
	

.PHONY: install
install:
	$(CP) libpgen2.a      $(INSTALL_LIB_DIR)
	$(CP) include/pgen2.h $(INSTALL_HDR_DIR)
	$(CP) include/pgen2   $(INSTALL_HDR_DIR)



.PHONY: uninstall
uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libpgen2.a
	$(RM) $(INSTALL_HDR_DIR)/pgen2.h
	$(RM) $(INSTALL_HDR_DIR)/pgen2 



doc:
	$(MAKE) -C docs


