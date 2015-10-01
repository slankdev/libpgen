

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
#include "pgen.h"
#include "address.h"
#include "pgen-error.h"

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
#include <netinet/ip.h>

#include "pgen-io.h"




pgen_ip::pgen_ip(){
	clear();	
}



pgen_ip::pgen_ip(const void* packet, int len){
	clear();	
	cast(packet, len);
}



void pgen_ip::clear(){
	pgen_eth::clear();
	this->IP.tos = 0;
	this->IP.tot_len = 20;
	this->IP.id = 1;
	this->IP.frag_off = 0;
	this->IP.ttl = 64;
	this->IP.protocol = IPPROTO_IP;
	this->IP.src = 0;
	this->IP.dst = "127.0.0.1";
}



void pgen_ip::compile(){
	ETH.type = 0x0800;
	pgen_eth::compile();
	
	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	memset(&this->ip, 0, IP_HDR_LEN);
	this->ip.ihl = IP_HDR_LEN / 4;
	this->ip.version = 4;
	this->ip.tos = this->IP.tos; //no useing world now
	
	this->ip.tot_len = htons(this->IP.tot_len);
	this->ip.frag_off = htons(this->IP.frag_off); // ?????

/*
#ifndef __linux	
	this->ip.tot_len  = this->IP.tot_len;
	this->ip.frag_off = this->IP.frag_off; // ?????
#endif
*/
	this->ip.id = htons(this->IP.id);
	this->ip.ttl = this->IP.ttl;
	this->ip.protocol = this->IP.protocol;
	this->ip.saddr = this->IP.src._addr;
	this->ip.daddr = this->IP.dst._addr;
	this->ip.check = 0;
	this->ip.check = checksum((unsigned short*)&this->ip, IP_HDR_LEN);

	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	this->len = p - this->data;

	compile_addData();
}



void pgen_ip::cast(const void* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_ip::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_eth::cast(data, len);

	const u_char* p = (u_char*)data;
	p += ETH_HDR_LEN;
	struct ip_header* buf = (struct ip_header*)p;
	p += IP_HDR_LEN;
	
	this->IP.tos = buf->tos;
	this->IP.tot_len = ntohs(buf->tot_len);
	this->IP.id = ntohs(buf->id);
	this->IP.frag_off = ntohs(buf->frag_off);
	this->IP.ttl = buf->ttl;
	this->IP.protocol = buf->protocol;
	this->IP.src._addr = buf->saddr;
	this->IP.dst._addr = buf->daddr;
	
	this->len = p - (u_char*)data;
	addData(p, len-(this->len));
}



void pgen_ip::send_L3(const char* ifname){
	compile();	
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = this->IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, this->data+ETH_HDR_LEN, this->len-ETH_HDR_LEN,
				(struct sockaddr*)&addr) < 0){
		pgen_perror("send_L3");
	}
}



void pgen_ip::summary(){
	compile();
	printf("IP{ %s(%s) -> %s(%s) }\n", IP.src.c_str(), ETH.src.c_str(),
			IP.dst.c_str(), ETH.dst.c_str());
}



void pgen_ip::info(){
	compile();
	pgen_eth::info();
	
	std::map<int, const char*> _ipprot;
	_ipprot[0]  = "Not Set. Empty IP Packet";
	_ipprot[1]  = "ICMP";
	_ipprot[2]  = "IGMP";
	_ipprot[4]  = "IPv4 on IP";
	_ipprot[6]  = "TCP";
	_ipprot[17] = "UDP";
	_ipprot[41] = "IPv6 on IP";

	printf(" * Internet Protocol version 4\n");
	printf("    - Source          :  %s \n", IP.src.c_str());
	printf("    - Destination     :  %s \n", IP.dst.c_str());
	printf("    - Protocol        :  %s (%u) \n", 
			_ipprot[IP.protocol],  IP.protocol);
	printf("    - Time to Leave   :  %d \n", IP.ttl);
	printf("    - Total Length    :  %d \n", IP.tot_len);
	printf("    - Identification  :  %d \n", IP.id);
}



void pgen_ip::help(){
	printf("IP Packet CLass------------------------------------------------\n");
	printf("tos      : type of service   :  8 bit field \n");
	printf("tot_len  : total length      : 16 bit field \n");
	printf("id       : identification    : 16 bit field \n");
	printf("frag_off : fragment offset   : 16 bit field \n"); // fix soon
	printf("ttl      : time to leave     :  8 bit field \n");
	printf("protocol : L4 protocol       :  8 bit field \n");
	printf("src      : source ip address : 32 bit field \n");
	printf("dst      : dest ip address   : 32 bit field \n");
	printf("----------------------------------------------------------------\n");
}
