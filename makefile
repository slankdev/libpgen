




export CPP             := g++
# export CPP             := clang
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




TYPES_SRC    = \
	src/data_container.cc

ARCH_SRC = \
	src/arch/arch.cc

UTIL_SRC = \
	src/util.cc

IO_SRC = \
	src/io/stream.cc \
	src/io/file_stream.cc \
	src/io/pcap_stream.cc \
	src/io/pcapng_stream.cc \
	src/io/net_stream.cc

CORE_SRC = \
	src/core/macaddress.cc \
	src/core/ipaddress.cc \
	src/core/packet.cc \
	src/core/protocol/ethernet.cc \
	src/core/protocol/arp.cc \
	src/core/protocol/ipv6.cc \
	src/core/protocol/ip.cc \
	src/core/protocol/zundoko.cc \
	src/core/protocol/udp.cc


MODULE_SRC = \
	src/module/module.cc


SRC = $(IO_SRC) $(TYPES_SRC) $(ARCH_SRC) $(CORE_SRC) $(MODULE_SRC) $(UTIL_SRC) 
OBJ = $(SRC:.cc=.o)




all: libpgen2.a test-code

test-code:
	$(MAKE) -C test

libpgen2.a: $(OBJ)
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@



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


