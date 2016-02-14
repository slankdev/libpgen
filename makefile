
include config.mk

INCLUDE_DIR = include 

SRC_DIR   = src

OBJ = `find . -name "*.o"`

# # origin source
# IO_DIR      = $(SRC_DIR)/io
#             
# CORE_DIR    = $(SRC_DIR)/core
# PROTCOL_DIR = $(CORE_DIR)/protocol
#
# TYPES_DIR   = $(SRC_DIR)/types
# UTIL_DIR    = $(SRC_DIR)/util
# ARCH_DIR    = $(SRC_DIR)/arch

# OBJ = \
# 	$(IO_DIR)/*.o \
# 	$(UTIL_DIR)/*.o \
# 	$(CORE_DIR)/*.o \
# 	$(TYPES_DIR)/*.o \
# 	$(PROTCOL_DIR)/*.o \
# 	$(ARCH_DIR)/*.o 



all: build build-main


build-main:  build
	$(CPP) $(CPPFLAGS) -I$(INCLUDE_DIR) $(OBJ)	main.cc


build: 
	$(MAKE) -C $(SRC_DIR) build


clean:
	$(RM) *.o *.out
	$(MAKE) -C $(SRC_DIR) clean
