

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

#ifndef IP_H
#define IP_H

#include "eth.h"


class pgen_ip : public pgen_eth {
	protected:
		struct ip_header		ip;
	public:
		static const int minLen = pgen_eth::minLen+sizeof(struct ip_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			bit8  hlen:4;
			bit8  tos;
			bit16 tot_len;
			bit16 id;
			bit16 frag_off;
			bit8  ttl; 
			bit8  protocol;
			ipaddr src;
			ipaddr dst;
		}IP;


		pgen_ip();
		pgen_ip(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void send(const char* ifname){send_L3(ifname);}
		void send_L3(const char* ifname);
		void summary();
		void info();
		void help();
};




#endif 
