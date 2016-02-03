
#
# copyright (C) <2015>  <Slank Hiroki Shirokura>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
#





include ../makefile.config

# export CPP := clang++
# export AR  := ar
# export CP  := cp -rf
# export RM  := rm -rf
# export MKDIR  := mkdir -p
# export RANLIB := ranlib
# CPPFLAGS = -Wall -fPIC -std=c++11 
.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  $(INCLUDE)
 


INCLUDE = -I../include 
PROGNAME = libpgen.a
INSTALL_LIB_DIR = /usr/local/lib
INSTALL_HDR_DIR = /usr/local/include


PGEN_IO_DIR     = io
PGEN_MODULE_DIR = module
PGEN_CORE_DIR = core
ADDRESS_DIR     = $(PGEN_CORE_DIR)/address
PACKET_DIR      = $(PGEN_CORE_DIR)/packet
PROTOCOLS_DIR   = $(PACKET_DIR)/protocols




all: libpgen.a



libpgen.a: make-pgen-io make-pgen-module make-pgen-core
	@rm -f $@
	$(AR) rc $@  \
		$(PGEN_IO_DIR)/*.o \
		$(ADDRESS_DIR)/*.o \
		$(PACKET_DIR)/*.o \
		$(PROTOCOLS_DIR)/*.o    \
		$(PGEN_MODULE_DIR)/*.o
	$(RANLIB) $@

make-pgen-core: 
	$(MAKE) -C $(PGEN_CORE_DIR) 

make-pgen-io:
	$(MAKE) -C $(PGEN_IO_DIR) 

make-pgen-module:
	$(MAKE) -C $(PGEN_MODULE_DIR) 



clean:
	$(MAKE) -C $(PGEN_CORE_DIR) clean
	$(MAKE) -C $(PGEN_IO_DIR)     clean
	$(MAKE) -C $(PGEN_MODULE_DIR)     clean
	$(RM) *.o *.out *.a *.so



install:
	$(CP) libpgen.a          $(INSTALL_LIB_DIR)
	$(CP) ../include/pgen.h  $(INSTALL_HDR_DIR)
	$(CP) ../include/pgen   $(INSTALL_HDR_DIR)
	$(MKDIR) /usr/local/etc/libpgen
	$(CP) ../etc/mac_code.list /usr/local/etc/libpgen/mac_code.list

uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libpgen.a
	$(RM) $(INSTALL_HDR_DIR)/pgen.h
	$(RM) $(INSTALL_HDR_DIR)/pgen 
	$(RM) /usr/local/etc/mac_code.list






