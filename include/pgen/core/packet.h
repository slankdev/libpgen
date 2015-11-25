

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



#ifndef PACKET_H
#define PACKET_H



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pgen/pgen-types.h>

#define PGEN_MAX_PACKET_LEN 10000
#define PGEN_MAX_EXT_DATA_LEN 98000


class pgen_packet{
	protected:
		int len;
		u_char data[PGEN_MAX_PACKET_LEN];
	public:

		u_char  _additional_data[10000];
		int     _additional_len;
		
		pgen_packet();
		virtual void clear()=0;
		virtual void compile()=0;
		virtual void cast(const void*, int)=0;
		virtual void send(const char* ifname)=0;
		virtual void summary()=0;
		virtual void info()=0;	
		virtual void help()=0;
		virtual int  write_bin(void*, int)=0;
		virtual int  read_bin(const void*, int)=0;



		void hex();
		void send_handle(pgen_t*);
		void add_data(const void*, int);
		
		int  length();
		u_char* byte();
};






#endif /* PACKET_H */
