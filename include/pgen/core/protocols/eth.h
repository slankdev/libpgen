

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


#ifndef ETH_H
#define ETH_H


#include <pgen/core/packet.h>
#include <pgen/core/address/address.h>


class pgen_eth : public pgen_packet {
	protected:
	public:
		static const int minLen = sizeof(struct ethernet_header);
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{
			macaddr dst;
			macaddr src;
			bit16 type;
		}ETH;
		
		pgen_eth();
		pgen_eth(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void summary();
		void info();
		void help();

		int write_bin(void*, int);
		int read_bin(const void*, int);
};


#endif /* ETH_H */
