

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

#ifndef HTTP_H
#define HTTP_H


#include "tcp.h"



class pgen_http : public pgen_tcp {
	protected:
		
	public:
		static const int minLen = pgen_tcp::minLen;
		static const int macLen = PGEN_MAX_PACKET_LEN;
	struct{
		int a;
		struct{
			int method;
			char header[2][256]; 
		}request;
	}HTTP;

	pgen_http();
	pgen_http(const void*, int);
	void clear();
	void compile();
	void cast(const void*, int);
	void send(const char* ifname){send_L3(ifname);}
	void summary();
	void info();
	void help();
	
};




#endif
