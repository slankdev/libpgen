
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




# export CPP := clang++
# export RM  := rm -rf
# CPPFLAGS = -Wall -fPIC -std=c++11 
# 
.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  $(INCLUDE)

include ../../makefile.config
INCLUDE = -I../../include 
PGEN_ADDRESS_DIR= address
PGEN_PACKET_DIR= packet


all: makeaddress makepacket 


makeaddress:
	$(MAKE) -C $(PGEN_ADDRESS_DIR)

makepacket:
	$(MAKE) -C $(PGEN_PACKET_DIR)



clean:
	$(MAKE) -C $(PGEN_ADDRESS_DIR) clean
	$(MAKE) -C $(PGEN_PACKET_DIR) clean


