

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


#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pgen/protocols/eth.h>
#include <pgen/pgen-error.h>
#include <pgen/pgen-io.h>




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
	u_char buf[1000];
	int buflen;

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->data;

	buflen = pgen_eth::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	
	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	len = p - this->data;
}





void pgen_eth::cast(const void* data, int l){
	u_char* p = (u_char*)data;
	int buflen;

	buflen = pgen_eth::read_bin(p, l);
	p += buflen;
	l -= buflen;

	add_data(p, l);
}




int pgen_eth::write_bin(void* buf, int buflen){
	if(buflen < sizeof(struct ethernet_header)){
		fprintf(stderr, "pgen_eth::write_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct ethernet_header eth;
	memset(&eth, 0, sizeof eth);

	for(int i=0; i< 6; i++){
		eth.ether_dhost[i] = this->ETH.dst._addr[i];	
		eth.ether_shost[i] = this->ETH.src._addr[i];	
	}
	eth.ether_type = htons(this->ETH.type);
	
	memcpy(buf, &eth, sizeof(struct ethernet_header));
	return sizeof(struct ethernet_header);	
}




int pgen_eth::read_bin(const void* buf, int buflen){
	if(buflen < sizeof(struct ethernet_header)){
		fprintf(stderr, "pgen_eth::read_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	const u_char* p = (u_char*)buf;
	struct ethernet_header* eth = (struct ethernet_header*)p;

	for(int i=0; i<6; i++){
		this->ETH.dst._addr[i] = eth->ether_dhost[i];
		this->ETH.src._addr[i] = eth->ether_shost[i];
	}
	this->ETH.type = ntohs(eth->ether_type);

	return sizeof(struct ethernet_header);
}




void pgen_eth::send_L2(const char* ifname){
	compile();		
	if(pgen_sendpacket_L2(ifname, this->data, this->len) < 0){
		pgen_perror("send_L2");		
	}
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
	printf("Ethernet Packet Class-------------------------------------------------\n");
	printf("dst   : Destination Hardware Address : 48 bit field \n");
	printf("src   : source      Hardware Address : 48 bit field \n");
	printf("type  : ethernet type                : 16 bit field \n");
	printf("----------------------------------------------------------------------\n");
}


