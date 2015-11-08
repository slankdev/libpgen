
/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */



#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <pgen/pgen-types.h>
#include <pgen/pgen-io.h>
#include <pgen/pgen-error.h>



#define FILEPATH "/usr/local/etc/libpgen/mac_code.list"


union lc{
	unsigned int l;
	unsigned char c[4];
};


class ipaddr{
	private:
		
	public:
		char _c_str[32];
		bit32 _addr;		
		
		ipaddr();
		ipaddr(const char* str);
		ipaddr(const ipaddr &i);
		ipaddr(std::string str);
		char* c_str();
		void clear();
		int setOctet(int n, int num);
		unsigned char getOctet(int n);
		int setipbydev(const char* ifname);
		int setmaskbydev(const char* ifname);
		int setnetbydev(const char* ifname);
		int setnetbyaddr(ipaddr ip, ipaddr mask);
		bool isSameSegment(ipaddr addr, ipaddr mask);
		ipaddr& operator=(ipaddr i);
		ipaddr& operator=(const char* str);
		ipaddr& operator=(int num);	// FUGUAI ARUKAMO
		ipaddr& operator=(std::string str);
		bool operator<(const ipaddr iaddr);
		bool operator>(const ipaddr iaddr);
		bool operator<=(const ipaddr iaddr);
		bool operator>=(const ipaddr iaddr);
		bool operator==(const ipaddr iaddr);
		bool operator!=(const ipaddr iaddr);
		ipaddr  operator++(int);
		
};




class macaddr{
	private:
	public:
		char _str[256];
		char _bender[32];
		bit8 _addr[6];
	
		macaddr();
		macaddr(const macaddr &m);
		macaddr(const char* str);
		int setmacbyarry(const u_char* array);
		char* c_str();
		void clear();
		int setOctet(int n, int num);
		unsigned char getOctet(int n);
		int setmacbydev(const char* ifname);
		int setmacbroadcast();
		char* bender();
		macaddr& operator=(int n);
		macaddr& operator=(const macaddr m);
		macaddr& operator=(const char* str);
		macaddr& operator=(std::string str);
		bool operator<(const macaddr iaddr);
		bool operator>(const macaddr iaddr);
		bool operator<=(const macaddr iaddr);
		bool operator>=(const macaddr iaddr);
		bool operator==(const macaddr iaddr);
		bool operator!=(const macaddr iaddr);
};





#endif /* ADDRESS_H */
