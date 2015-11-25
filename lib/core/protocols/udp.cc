

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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <pgen/core/protocols/udp.h>



pgen_udp::pgen_udp(){
	clear();
}


pgen_udp::pgen_udp(const void* packet, int len){
	clear();
	cast(packet, len);
}


void pgen_udp::clear(){
	pgen_ip::clear();
	this->IP.protocol = 17;
	this->IP.tot_len = 20+8;

	UDP.src = 53;
	UDP.dst = 53;
	UDP.len = 8;
	UDP.check = 0;
}




void pgen_udp::compile(){
	u_char buf[100000];
	int buflen;

	memset(this->__data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->__data;

	buflen = pgen_eth::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_ip::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_udp::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	
	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	__len = p - this->__data;
}



void pgen_udp::cast(const void* data, int l){
	const u_char* p = (u_char*)data;
	int buflen;

	buflen = pgen_eth::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_ip::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_udp::read_bin(p, l);
	p += buflen;
	l -= buflen;

	this->__len = p - (u_char*)__data;
	add_data(p, l);
}



int  pgen_udp::write_bin(void* buf, int buflen){
	if(buflen < sizeof(struct udp_header)){
		fprintf(stderr, "pgen_udp::write_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}
	
	struct udp_header udp_head;
	memset(&udp_head, 0, sizeof(udp_head));

	udp_head.source = htons(this->UDP.src);
	udp_head.dest   = htons(this->UDP.dst);
	udp_head.len    = htons(this->UDP.len);
	udp_head.check  = htons(this->UDP.check);
	
	memcpy(buf, &udp_head, sizeof(udp_head));
	return sizeof(udp_head);
}


int  pgen_udp::read_bin(const void* buf, int buflen){
	if(buflen < sizeof(struct udp_header)){
		fprintf(stderr, "pgen_udp::read_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct udp_header *udp_head = (struct udp_header*)buf;

	this->UDP.src   = ntohs(udp_head->source);
	this->UDP.dst   = ntohs(udp_head->dest);
	this->UDP.len   = ntohs(udp_head->len);
	this->UDP.check = ntohs(udp_head->check);

	return sizeof(udp_head);
}



unsigned short pgen_udp::calc_checksum(){
	this->UDP.check = 0;
	
	struct ip_header ip_head;
	pgen_ip::write_bin(&ip_head, sizeof(ip_head));
	struct udp_header udp_head;
	pgen_udp::write_bin(&udp_head, sizeof(udp_head));
	
	return ntohs(checksumUdp(ip_head, udp_head, _additional_data,IP.tot_len-IP.hlen*4));
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
	printf("    - Checksum        :  %d \n", UDP.check);
}




void pgen_udp::help(){
	printf("UDP Packet CLass---------------------------------------\n");
	printf("src  : source port : 16 bit field \n");
	printf("dst  : dest port   : 16 bit field \n");
	printf("len  : length      : 16 bit field \n");
	printf("check: checksum    : 16 bit field \n");
	printf("-------------------------------------------------------\n");
}
