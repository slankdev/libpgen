

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


/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "address.h"
#include "pgen-io.h"
#include "udp.h"

#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/udp.h>		// for struct udp		

pgen_udp::pgen_udp(){
	clear();
}


pgen_udp::pgen_udp(const void* packet, int len){
	clear();
	cast(packet, len);
}


void pgen_udp::clear(){
	pgen_ip::clear();
	UDP.src = 53;
	UDP.dst = 53;
	UDP.len = 8;
}





void pgen_udp::compile(){
	this->IP.protocol = 17;
	this->IP.tot_len = IP_HDR_LEN + this->UDP.len;
	pgen_ip::compile();

	memset(&this->udp, 0, UDP_HDR_LEN);
	this->udp.source = htons(this->UDP.src);
	this->udp.dest   = htons(this->UDP.dst);
	this->udp.len    = htons(this->UDP.len);
	this->udp.check  = 0;
	
	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	memcpy(p, &this->udp, UDP_HDR_LEN);
	p += UDP_HDR_LEN;
	len = p - this->data;
	
	compile_addData();
}



void pgen_udp::cast(const void* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_udp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_ip::cast(data, len);

	const u_char* p = (u_char*)data;
	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;

	struct udp_header *buf = (struct udp_header*)p;
	p += UDP_HDR_LEN;

	this->UDP.src = ntohs(buf->source);
	this->UDP.dst = ntohs(buf->dest);
	this->UDP.len = ntohs(buf->len);
	
	this->len = p - (u_char*)data;
	addData(p, len- this->len);
}



void pgen_udp::summary(){
	printf("UDP(%d -> %d) \n", UDP.src, UDP.dst);
}



void pgen_udp::info(){
	compile();
	pgen_ip::info();
	char buf[32];

	printf(" * User Datagram Protocol \n");
	printf("    - Source Port     :  %d (%s)\n", 
			UDP.src, pgen_port2service(UDP.src, "tcp", buf));
	printf("    - Destination Port:  %d (%s)\n", 
			UDP.dst, pgen_port2service(UDP.dst, "buf", buf));
	printf("    - Length          :  %d \n", UDP.len);
}



void pgen_udp::help(){
	printf("UDP Packet CLass---------------------------------------\n");
	printf("src : source port : 16 bit field \n");
	printf("dst : dest port   : 16 bit field \n");
	printf("len : length      : 16 bit field \n");
	printf("-------------------------------------------------------\n");
}
