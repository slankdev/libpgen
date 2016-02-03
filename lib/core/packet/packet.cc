

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




#include <pgen/module/debug.h>
#include <pgen/core/packet/packet.h>
#include <pgen/io/pgen-io.h>
#include <pgen/io/pgen-error.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>




void pgen_packet::DATA_set(const void* buf, int l) {
	memcpy(_additional_data, buf, l);
	_additional_len = l;
}

void pgen_packet::add_data(const void* buf, int l) {
	DATA_set(buf, l);
}

int pgen_packet::DATA_len() {
	this->compile();
	return _additional_len;
}
void* pgen_packet::DATA_get() {
	this->compile();
	return _additional_data;
}




pgen_packet::pgen_packet(){
	__len = 0;
	memset(__data, 0, sizeof(__data));
	_additional_len = 0;
	memset(_additional_data, 0, sizeof _additional_data);

	_D.pack = this;
}





void pgen_packet::hex(){
	compile();
	pgen_hex(this->__data, this->__len);
}



int pgen_packet::length(){
	compile();
	return this->__len;
}





u_char* pgen_packet::byte(){
	compile();
	return this->__data;
}



void pgen_packet::send_dev(const char* ifname){
	compile();		
	if(pgen_sendpacket_L2(ifname, this->__data, this->__len) < 0){
		pgen_perror("send_L2");		
	}
}




void pgen_packet::send(pgen_t* handle){
	compile();
	int r = pgen_send(handle, this->__data, this->__len);
	if(r < 0){
		pgen_perror("send_handle");	
	}
}








