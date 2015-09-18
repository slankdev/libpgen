

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


#include "pgen.h"
#include "packet.h"
#include "address.h"

#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/ethernet.h> 

#include "netutils.h"





pgen_eth::pgen_eth(){
	clear();	
}



pgen_eth::pgen_eth(const void* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_eth::clear(){
	this->ETH.src = 0;
	this->ETH.dst = 0;
	this->ETH.type = htons(0);
}



void pgen_eth::compile(){
	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	for(int i=0; i< 6; i++){
		this->eth.ether_dhost[i] = this->ETH.dst._addr[i];	
		this->eth.ether_shost[i] = this->ETH.src._addr[i];	
	}
	this->eth.ether_type = htons(this->ETH.type);

	u_char* p = this->data;
	memcpy(p, &(this->eth), ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	len = p - this->data;
	
	compile_addData();
}



void pgen_eth::cast(const void* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_eth::cast(): packet len isn`t support (len=%d)\n", this->len);
		return;
	}

	const u_char* p = (u_char*)data;
	struct ethernet_header* buf;
	buf = (struct ethernet_header*)p;
	p += ETH_HDR_LEN;
	
	for(int i=0; i<6; i++){
		this->ETH.dst._addr[i] = buf->ether_dhost[i];
		this->ETH.src._addr[i] = buf->ether_shost[i];
	}
	this->ETH.type = ntohs(buf->ether_type);
	
	this->len = p - (u_char*)data;
	addData(p, len-(this->len));
}



void pgen_eth::send_L2(const char* ifname){
	compile();		
	if(pgen_sendpacket_L2(ifname, this->data, this->len) < 0)
		exit(-1);	
}



void pgen_eth::summary(){
	printf("Ethernet{%s -> %s type=0x%04x}\n", 
			ETH.src.c_str(), ETH.dst.c_str(), ETH.type);
}



void pgen_eth::info(){
	compile();
	std::map<int , const char*> _ethtype;
	_ethtype[0x0800] = "IPv4";
	_ethtype[0x0806] = "ARP";
	_ethtype[0x8035] = "RARP";
	_ethtype[0x8191] = "NetBios";
	_ethtype[0x86dd] = "IPv6";

	printf(" * Ethernet  %s -> %s \n", ETH.src.bender(), ETH.dst.bender());
	printf("    - Destination     :  %s (%s)   \n", 
			ETH.dst.c_str(), ETH.dst.bender());
	printf("    - Source          :  %s (%s)   \n" , 
			ETH.src.c_str(), ETH.src.bender());
	printf("    - Type            :  %s  (0x%04x)   \n", 
			_ethtype[ETH.type] ,ETH.type);
}



void pgen_eth::help(){
	printf("help ethernet\n");
	printf("dst : Destination Hardware Address : 48 bit field \n");
	printf("src : source      Hardware Address : 48 bit field \n");
	printf("type : ethernet type  : 16 bit field \n");
}

