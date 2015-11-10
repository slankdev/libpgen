

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


#ifndef ICMP_H
#define ICMP_H

#include <pgen/core/protocols/ip.h>

class pgen_icmp : public pgen_ip {
	protected:
		struct icmp_header icmp;
		bit8  icmp_data[256];
		bit32 icmp_data_len;
		bit8  icmp_ext_data[256];
		bit32 icmp_ext_data_len;
	public:
		static const int minLen = pgen_ip::minLen+ICMP_HDR_LEN;
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{	
			bit8  type;
			bit8  code;
			bit16 check;
			
			struct{
				int id;
				int seq;
			}echo;
			struct{
				ipaddr gw_addr;	
			}redirect;
			struct{
				bit8 len;
				bit16 next_mtu;
			}destination_unreach;
			struct{
				bit8 len;
			}time_exceeded;


		}ICMP;
		
		pgen_icmp();
		pgen_icmp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void help();

		int  write_bin(void*, int);
		int  read_bin(const void*, int);

		void icmp_addData(const void*, int);
		unsigned short calc_checksum();
};


#endif
