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
	$(CPP) $(CPPFLAGS) -c  $<  -o $@  -I./protocols

PROGNAME = libpgen.a
CPPFLAGS = -Wall -fPIC -std=c++11 
INSTALL_LIB_DIR = /usr/local/lib
INSTALL_HDR_DIR = /usr/local/include
PROTOCOLS_DIR = protocols


NETUTILS_SRC = \
	pgen-io.cc \
	pgen-netutil.cc \
	pgen-util.cc

OTHER_SRC = \
	packet.cc \
	unknown.cc \
	pgen-error.cc \
	debug.cc

HDR = pgen.h \
	address.h \
	packet.h \
	pgen-types.h \
	pgen-endian.h \
	pgen-error.h \
	pgen-io.h \
	pgen-netutil.h \
	pgen-util.h \
	release.h \
	debug.h 

OBJ = $(NETUTILS_SRC:.cc=.o) $(OTHER_SRC:.cc=.o) 



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

install: $(HDR) $(PROGNAME) mac_code.list
	$(MAKE) -C $(PROTOCOLS_DIR) install
	$(CP) $(PROGNAME) $(INSTALL_LIB_DIR)
	for i in $(HDR); do \
	  $(CP) $$i $(INSTALL_HDR_DIR); done
	$(CP) mac_code.list /usr/local/etc/mac_code.list

uninstall: 
	$(MAKE) -C $(PROTOCOLS_DIR) uninstall
	$(RM) $(INSTALL_LIB_DIR)/$(PROGNAME)
	for i in $(HDR); do \
	  $(RM) $(INSTALL_HDR_DIR)/$$i ; done
	$(RM) /usr/local/etc/mac_code.list


