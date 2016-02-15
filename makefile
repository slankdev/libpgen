
include config.mk
INCLUDE_DIR = include 

SRC_DIR   = src
OBJ = `find . -name "*.o"`



all: libpgen2.a

# all:
# 	g++ -std=c++11 `find . -name "*.cc" | xargs`  -Iinclude


libpgen2.a: build
	@rm -f $@
	$(AR) rc $@ $(OBJ)
	$(RANLIB) $@

build: 
	$(MAKE) -C $(SRC_DIR) build

clean:
	$(RM) *.o *.out *.a
	$(MAKE) -C $(SRC_DIR) clean


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
