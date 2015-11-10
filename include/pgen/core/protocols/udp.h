

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

#ifndef UDP_H
#define UDP_H


#include <pgen/core/protocols/ip.h>

class pgen_udp : public pgen_ip {
	protected:
	public:
		static const int minLen = pgen_ip::minLen+sizeof(struct udp_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			bit16 src;
			bit16 dst;
			bit16 len;
			bit16 check;
		}UDP;

		pgen_udp();
		pgen_udp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, const int len);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void help();

		int  write_bin(void*, int);
		int  read_bin(const void*, int);

		unsigned short calc_checksum();
};




#endif
