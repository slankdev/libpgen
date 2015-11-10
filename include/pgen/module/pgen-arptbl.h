

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



#ifndef PGEN_ARPTBL
#define PGEN_ARPTBL


#include <pgen/packet/address/address.h>
#include <vector>

typedef struct arpent arpent_t;

struct arpent{
	ipaddr ip;
	macaddr mac;
};



class arptable{
	private:
		std::vector<arpent_t> entry;
		pgen_t* handle;
	public:
		arptable();
		arptable(pgen_t*);
		void sethandle(pgen_t*);
		int  add(ipaddr, macaddr);
		int  del(ipaddr);
		int  get(ipaddr);
		int  learn(const void*, int len);
		void show();
		macaddr find(ipaddr);
};



#endif /* PGEN_ARPTBL */

