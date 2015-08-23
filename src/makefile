#makefile

export CC  := gcc
export CPP := g++ 
export CXX := g++ 
export MKDIR := mkdir -p
export AR  := ar
export CP  := cp -rf
export RM  := rm -rf



.SUFFIXES: .out .c .cc .o .h 

.cc.o: 
	$(CPP) -std=c++11 -c  $<  -o $@  


PACKET_CLASS_SRC = \
	  unknown.cc \
	  packet.cc \
	  eth.cc \
	  arp.cc \
	  ip.cc \
	  icmp.cc \
	  tcp.cc \
	  udp.cc \
	  dns.cc \
	  ardrone.cc \
	  debug.cc


NETUTIL_SRC = \
	  netutil.cc

SRC = $(PACKET_CLASS_SRC) $(SNIFF_SRC) $(NETUTIL_SRC)


HDR = pgen.h \
	  packconf.h \
	  address.h \
	  packet.h \
	  mytypes.h \
	  debug.h


OBJ = $(SRC:.cc=.o)

CFLAGS = -Wall -fPIC


all: libpgen.a

libpgen.a: $(OBJ)
	$(RM) $@
	$(AR) rc $@ $(OBJ)


clean:
	$(RM) *.o *.out *.a *.so


install: copylib copyheader copymaclist

uninstall: deletelib deleteheader detelemaclist


copylib:
	$(CP) libpgen.a     /usr/local/lib/libpgen.a

copyheader:
	$(CP) address.h     /usr/local/include/address.h
	$(CP) mytypes.h     /usr/local/include/mytypes.h
	$(CP) netutil.h     /usr/local/include/netutil.h
	$(CP) packconf.h    /usr/local/include/packconf.h
	$(CP) packet.h      /usr/local/include/packet.h
	$(CP) pgen.h        /usr/local/include/pgen.h
	$(CP) release.h     /usr/local/include/release.h

copymaclist:
	$(CP) mac_code.list /usr/local/etc/mac_code.list


deletelib:
	$(RM) /usr/local/lib/libpgen.a

deleteheader:
	$(RM) /usr/local/include/address.h
	$(RM) /usr/local/include/mytypes.h
	$(RM) /usr/local/include/netutil.h
	$(RM) /usr/local/include/packconf.h
	$(RM) /usr/local/include/packet.h
	$(RM) /usr/local/include/pgen.h
	$(RM) /usr/local/include/release.h

detelemaclist:
	$(RM) /usr/local/etc/mac_code.list
