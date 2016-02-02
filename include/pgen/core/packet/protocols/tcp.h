

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

#ifndef TCP_H
#define TCP_H

#include <pgen/core/packet/protocols/ip.h>

class pgen_tcp : public pgen_ip {
	protected:
	public:
		static const int minLen = pgen_ip::minLen+20;
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			bit16 src;
			bit16 dst;
			bit32 seq;
			bit32 ack;
			u_char doff:4;
			struct{
				u_char fin:1;
				u_char syn:1;
				u_char rst:1;
				u_char psh:1;
				u_char ack:1;
				u_char urg:1;
			}flags;
			bit16 window;
			bit16 check;

			bit8 option[1000];
		}TCP;

		pgen_tcp();
		pgen_tcp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, const int len);
		void summary();
		void info();
		void help();

		int  write_bin(void*, int);
		int  read_bin(const void*, int);

		unsigned short calc_tcp_checksum();
};




#endif
