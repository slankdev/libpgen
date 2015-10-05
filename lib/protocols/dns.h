

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

#ifndef DNS_H
#define DNS_H


#include "udp.h"


#define MAX_QUERY  16
#define MAX_ANSWER 16
#define MAX_AUTH   16
#define MAX_ADD    16
class pgen_dns :public pgen_udp {
	protected:
		struct dns_header dns;
		bit8  query_data[256];
		bit32 query_data_len;
		bit8  answer_data[256];
		bit32 answer_data_len;
		bit8  auth_data[256];
		bit32 auth_data_len;
		bit8  addition_data[256];
		bit32 addition_data_len;
	public:
		static const int minLen = pgen_udp::minLen+DNS_HDR_LEN;
		static const int maxLen = PGEN_MAX_PACKET_LEN; 
		struct{
			u_int16_t id;
			struct{
				bit8 qr:1;
				bit8 opcode:4;
				bit8 aa:1;
				bit8 tc:1;
				bit8 rd:1;
				bit8 ra:1;
				bit8 nouse:3;
				bit8 rcode:4;
			}flags;
			u_int16_t qdcnt;
			u_int16_t ancnt;
			u_int16_t nscnt;
			u_int16_t arcnt;
			struct{
				std::string name;
				u_int16_t type;
				u_int16_t cls;
			}query[MAX_QUERY];
			struct{
				bit16  name;
				bit16  type;
				bit16  cls;
				bit32  ttl;
				bit16  len;
				
				//ipaddr addr;
				//std::string url;
				bit8 data[32];
			}answer[MAX_ANSWER];
			struct{
				bit16 name;
				bit16 type;
				bit16 cls;
				bit16 ttl;
				bit16 len;
				
				//ipaddr addr;
				//std::string url;
				bit8  data[32];
			}auth[MAX_AUTH];
			struct{
				bit16 name;
				bit16 type;
				bit16 cls;
				bit16 ttl;
				bit16 len;
				
				//ipaddr addr;
				//std::string url;
				bit8  data[32];
			}addition[MAX_ADD];
		}DNS;

		pgen_dns();
		pgen_dns(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void debug();
		void help();
		
		void clear_query();
		void clear_answer();
		void clear_auth();
		void clear_addition();
		void compile_query();
		void compile_answer();
		void compile_auth();
		void compile_addition();
		int  cast_query(const char*, int);
		int  cast_answer(const char*, int);
		int  cast_auth(const char* , int);
		int  cast_addition(const char*, int);

};


#endif
