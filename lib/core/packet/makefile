
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
export RM  := rm -rf

.SUFFIXES: .out .c .cc .o .h 
.cc.o: 
	$(CPP) $(CPPFLAGS) -c $< -o $@  $(INCLUDE)
 

INCLUDE = -I../../../include 
CPPFLAGS = -Wall -fPIC -std=c++11 
PROTOCOLS_DIR = protocols


all:  packet.o unknown.o makeprotocol 


makeprotocol: 
	$(MAKE) -C $(PROTOCOLS_DIR) 



clean:
	$(MAKE) -C $(PROTOCOLS_DIR) clean
	$(RM) *.o 


