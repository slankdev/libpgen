BASE = ..

export CPP := g++ 

.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(BASE)

CPPFLAGS = -Wall -fPIC -std=c++11 
PROTOCOL_OBJ = $(PROTOCOL_SRC:.cc=.o)
	
PROTOCOL_SRC = \
	eth.cc \
	arp.cc \
	ip.cc \
	icmp.cc \
	tcp.cc \
	udp.cc \
	dns.cc \
	ardrone.cc \
	http.cc \
	dhcp.cc 


all: $(PROTOCOL_OBJ)

clean:
	@echo clean PROTOCOL_OBJ
	$(RM) *.o 
