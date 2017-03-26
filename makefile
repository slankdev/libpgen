

CXX             := g++
CXXFLAGS        := -std=c++11 -Wextra -g3 -O0 -Iinclude
MAKEFLAGS += --no-print-directory

INSTALL_LIB_DIR := /usr/local/lib
INSTALL_HDR_DIR := /usr/local/include


.SUFFIXES: .out .c .cc .o .h
.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@



CORE_SRC = \
	src/core/protocol/ethernet.cc \
	src/core/protocol/arp.cc \
	src/core/protocol/ipv6.cc \
	src/core/protocol/ip.cc \
	src/core/protocol/zundoko.cc \
	src/core/protocol/udp.cc
CORE_OBJ = $(CORE_SRC:.cc=.o)


OBJ = $(CORE_OBJ)


all: libpgen2.a
build-core:   $(CORE_OBJ)


libpgen2.a: build-core
	@rm -f $@
	ar rc $@ $(OBJ)
	ranlib $@

re: clean all

clean:
	rm -rf libpgen2.a a.out
	rm -rf $(OBJ)


install:
	cp -rf libpgen2.a      $(INSTALL_LIB_DIR)
	cp -rf include/pgen2.h $(INSTALL_HDR_DIR)
	cp -rf include/pgen2   $(INSTALL_HDR_DIR)


uninstall:
	rm -rf $(INSTALL_LIB_DIR)/libpgen2.a
	rm -rf $(INSTALL_HDR_DIR)/pgen2.h
	rm -rf $(INSTALL_HDR_DIR)/pgen2


# .PHONY: depend
# depend:
# 	@echo "Create Dependfile"
# 	$(CXX) -MM -MG $(SRC) $(CXXFLAGS) -I$(INCLUDE_DIR) > depend.mk
#
# -include depend.mk
