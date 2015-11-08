
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





export CPP := clang++
export AR  := ar
export CP  := cp -rf
export RM  := rm -rf
export RANLIB := ranlib

.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  $(INCLUDE)
 

INCLUDE = -I../include 
PROGNAME = libpgen.a
CPPFLAGS = -Wall -fPIC -std=c++11 
INSTALL_LIB_DIR = /usr/local/lib
INSTALL_HDR_DIR = /usr/local/include
PROTOCOLS_DIR = protocols


LOWLAYER_SRC = \
	pgen-netutil.cc \
	pgen-util.cc 

OTHER_SRC = \
	pgen-error.cc \
	debug.cc

PGEN_IO_SRC = \
	$(LOWLAYER_SRC) \
	pgen-io.cc 

PGEN_PACKET_SRC = \
	packet.cc \
	unknown.cc \

PGEN_ARPTBL_SRC = \
	pgen-arptbl.cc 


OTHER_OBJ       = $(OTHER_SRC:.cc=.o)
PGEN_IO_OBJ     = $(PGEN_IO_SRC:.cc=.o)
PGEN_PACKET_OBJ = $(PGEN_PACKET_SRC:.cc=.o)
PGEN_ARPTBL_OBJ = $(PGEN_ARPTBL_SRC:.cc=.o)


OBJ = $(LOWLAYER_OBJ) $(OTHER_OBJ) \
	$(PGEN_IO_OBJ) $(PGEN_PACKET_OBJ) $(PGEN_ARPTBL_OBJ)




all: libpgen.a


libpgen.a: $(OBJ) makeprotocol
	@rm -f $@
	$(AR) rc $@ *.o $(PROTOCOLS_DIR)/*.o
	$(RANLIB) $@

makeprotocol: 
	$(MAKE) -C $(PROTOCOLS_DIR) 



clean:
	$(MAKE) -C $(PROTOCOLS_DIR) clean
	$(RM) *.o *.out *.a *.so


install:
	$(CP) libpgen.a          $(INSTALL_LIB_DIR)
	$(CP) ../include/pgen.h  $(INSTALL_HDR_DIR)
	$(CP) ../include/pgen   $(INSTALL_HDR_DIR)
	$(CP) mac_code.list /usr/local/etc/mac_code.list
	

uninstall:
	$(RM) $(INSTALL_LIB_DIR)/libpgen.a
	$(RM) $(INSTALL_HDR_DIR)/pgen.h
	$(RM) $(INSTALL_HDR_DIR)/pgen 
	$(RM) /usr/local/etc/mac_code.list






