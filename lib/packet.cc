

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




#include "packet.h"
#include "pgen-io.h"
#include "pgen-error.h"
#include "debug.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>










pgen_packet::pgen_packet(){
	len = 0;
	memset(data, 0, sizeof(data));
	_additional_len = 0;
	memset(_additional_data, 0, sizeof _additional_data);
}




void pgen_packet::add_data(const void* buf, int l) {
	memcpy(_additional_data, buf, l);
	_additional_len = l;

	return ;	
}




void pgen_packet::hex(){
	compile();
	
	pgen_hex(this->data, this->len);
}



int pgen_packet::length(){
	compile();
	return this->len;
}





u_char* pgen_packet::byte(){
	compile();
	return this->data;
}







void pgen_packet::send_handle(pgen_t* handle){
	compile();
	int r = pgen_sendpacket_handle(handle, this->data, this->len);
	if(r < 0){
		pgen_perror("send_handle");	
	}
}


