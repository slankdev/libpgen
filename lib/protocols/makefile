BASE = ..



include makefile.protocols
export CPP := clang++
export CP  := cp -rf
export RM  := rm -rf

.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@ -I$(BASE)

CPPFLAGS = -Wall -fPIC -std=c++11 
PROTOCOL_OBJ = $(PROTOCOL_SRC:.cc=.o)
INSTALL_HDR_DIR = /usr/local/include
	


all: $(PROTOCOL_OBJ)

clean:
	$(RM) *.o 

install:
	for i in $(PROTOCOL_HDR); do \
	  $(CP) $$i $(INSTALL_HDR_DIR); done


uninstall:
	for i in $(PROTOCOL_HDR); do \
	  $(RM) $(INSTALL_HDR_DIR)/$$i ; done

