

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
#include <string>
#include <iostream>

#include <pgen/protocols/icmp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>




pgen_icmp::pgen_icmp(){
	clear();
}


pgen_icmp::pgen_icmp(const void* packet, int len){
	clear();
	cast(packet, len);
}


void pgen_icmp::clear(){
	pgen_ip::clear();
	this->IP.protocol = 1;

	this->ICMP.type = 8;
	this->ICMP.code = 0;

	this->ICMP.echo.id = 0;
	this->ICMP.echo.seq = 0;
	this->ICMP.redirect.gw_addr = 0;

	memset(icmp_data, 0, sizeof(icmp_data));
	icmp_data_len = 0;
	memset(icmp_ext_data, 0, sizeof(icmp_ext_data));
	icmp_ext_data_len = 0;
} 

void pgen_icmp::compile(){

	u_char buf[1000];
	int buflen;

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->data;

	buflen = pgen_eth::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_ip::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_icmp::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;

	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	len = p-this->data;
} 





void pgen_icmp::cast(const void* data, int l){
	const u_char* p = (u_char*)data;
	int buflen;

	buflen = pgen_eth::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_ip::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_icmp::read_bin(p, l);
	p += buflen;
	l -= buflen;

	add_data(p, l);
} 



int  pgen_icmp::write_bin(void* buf, int buflen){
	if(buflen < sizeof(struct icmp_header)){
		fprintf(stderr, "pgen_icmp::write_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}
		
	struct icmp_header icmp_head;
	u_char icmp_data[4000];
	int icmp_datalen = 0;
	memset(&icmp_head, 0, sizeof(icmp_head));
	memset(&icmp_data, 0, sizeof(icmp_data));

	icmp_head.icmp_type = this->ICMP.type;
	icmp_head.icmp_code = this->ICMP.code;
	icmp_head.icmp_cksum = htons(this->ICMP.check);

	if(ICMP.type==8 || ICMP.type==0){  // Echo or Echo Relay
		struct icmp_echo_header ie;
		memset(&ie, 0, sizeof(ie));
		ie.id = htons(this->ICMP.echo.id);
		ie.seq = htons(this->ICMP.echo.seq);
		memcpy(icmp_data, &ie, sizeof(ie));
		icmp_datalen = sizeof(ie);

	}else if(ICMP.type==3){				// Destination Unreachable
		struct icmp_destination_unreach idu;
		memset(&idu, 0, sizeof(idu));
		idu.nouse = 0;
		idu.len   = this->ICMP.destination_unreach.len;
		idu.next_mtu = htons(this->ICMP.destination_unreach.next_mtu);
		memcpy(icmp_data, &idu, sizeof(idu));
		icmp_datalen = sizeof(idu);
		
	}else if(ICMP.type==11){			// Time Exceeded
		struct icmp_time_exceeded ite;
		memset(&ite, 0, sizeof(ite));
		ite.nouse1 = 0;
		ite.len    = this->ICMP.time_exceeded.len;  // kokoyare
		ite.nouse2 = htons(0);
		memcpy(icmp_data, &ite, sizeof(ite));
		icmp_datalen = sizeof(ite);

	}else if(ICMP.type==5){		// Redirect
		struct icmp_redirect ir;
		memset(&ir, 0, sizeof(ir));
		ir.gw_addr = this->ICMP.redirect.gw_addr._addr;
		memcpy(icmp_data, &ir, sizeof(ir));
		icmp_datalen = sizeof(ir);

	}else if(ICMP.type==9 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::write_bin: router advertisement not implement  yet\n");
	}else if(ICMP.type==10 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::write_bin: router solicitation not implement  yet\n");
	}else if(ICMP.type==13 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::write_bin: Timestamp request not implement yet \n");
	}else{
		fprintf(stderr, "pgen_icmp::write_bin: icmp type & code is not support yet(%d,%d)\n",
				ICMP.type, ICMP.code);
	}

	u_char* p = (u_char*)buf;
	memcpy(p, &icmp_head, sizeof(icmp_head));
	p += sizeof(icmp_head);
	memcpy(p, icmp_data, icmp_datalen);
	p += icmp_datalen;
	memcpy(p, icmp_ext_data, icmp_ext_data_len);
	p += icmp_ext_data_len;

	this->icmp.icmp_cksum = (checksum((unsigned short*)buf, (unsigned short)(p-(u_char*)buf)));

	p = (u_char*)buf;
	memcpy(p, &icmp_head, sizeof(icmp_head));
	p += sizeof(icmp_head);
	memcpy(p, icmp_data, icmp_datalen);
	p += icmp_datalen;
	memcpy(p, icmp_ext_data, icmp_ext_data_len);
	p += icmp_ext_data_len;

	return p - (u_char*)buf;
}




int  pgen_icmp::read_bin(const void* buf, int buflen){
	if(buflen < sizeof(struct icmp_header)){
		fprintf(stderr, "pgen_icmp::read_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	u_char* p = (u_char*)buf;
	struct icmp_header *icmp_head = (struct icmp_header*)p;
	p += sizeof(struct icmp_header);

	this->ICMP.type = icmp_head->icmp_type;
	this->ICMP.code = icmp_head->icmp_code;
	this->ICMP.check = ntohs(icmp_head->icmp_cksum);

	if(ICMP.type==8 || ICMP.type==0){  // Echo or Echo Relay
		struct icmp_echo_header* ie = (struct icmp_echo_header*)p;
		this->ICMP.echo.id = ntohs(ie->id);
		this->ICMP.echo.seq = ntohs(ie->seq);
		p += sizeof(struct icmp_echo_header);
	}else if(ICMP.type==3){				// Destination Unreachable
		struct icmp_destination_unreach* idu = (struct icmp_destination_unreach*)p;
		this->ICMP.destination_unreach.len = (idu->len);
		this->ICMP.destination_unreach.next_mtu = ntohs(idu->next_mtu);
		p += sizeof(struct icmp_destination_unreach);
	}else if(ICMP.type==11){			// Time Exceeded
		struct icmp_time_exceeded* ite = (struct icmp_time_exceeded*)p;
		this->ICMP.time_exceeded.len = ite->len;  // kokoyare
		p += sizeof(struct icmp_time_exceeded);
	}else if(ICMP.type==5){
		struct icmp_redirect* ir = (struct icmp_redirect*)p;
		this->ICMP.redirect.gw_addr._addr = ir->gw_addr;
		p += sizeof(struct icmp_redirect);
	}else if(ICMP.type==9 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::read_bin: router advertisement not implement  yet\n");
		return p - (u_char*)buf;
	}else if(ICMP.type==10 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::read_bin: router solicitation not implement  yet\n");
		return p - (u_char*)buf;
	}else if(ICMP.type==13 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::read_bin: Timestamp request not implement yet \n");
		return p - (u_char*)buf;
	}else{
		fprintf(stderr, "pgen_icmp::read_bin: icmp type & code is not support yet(%d,%d)\n",
				ICMP.type, ICMP.code);
		return p - (u_char*)buf;
	}

	return p - (u_char*)buf;
}



unsigned short pgen_icmp::calc_checksum(){
	this->ICMP.check = 0;

	u_char buf[10000];
	int buflen;
	u_char* p = buf;
	
	buflen = pgen_icmp::write_bin(buf, sizeof(buf));
	memcpy(p, buf, buflen);
	p += buflen;
	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	return ntohs(checksum((unsigned short*)buf, p-buf));
}



void pgen_icmp::summary(){
	compile();
	printf("ICMP{ ");

	if(ICMP.type==8){
		printf("Echo Request id=0x%04x seq=%d ttl=%d",ICMP.echo.id, ICMP.echo.seq, IP.ttl );
	}else if(ICMP.type==0){ 
		printf("Echo Reply id=0x%04x seq=%d ttl=%d",ICMP.echo.id, ICMP.echo.seq, IP.ttl );
	}else if(ICMP.type==3){
		printf("Destination Unreachable code=%d", ICMP.code);
	}else if(ICMP.type==11){
		printf("Time Exceeded");
	}else if(ICMP.type==5){
		printf("Redirect gw_addr=%s", ICMP.redirect.gw_addr.c_str());
	}else if(ICMP.type==9 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::summary: router advertisement not implement  yet\n");
	}else if(ICMP.type==10 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::summary: router solicitation not implement  yet\n");
	}else{
		fprintf(stderr, "pgen_icmp::summary: icmp type & code is not support yet\n");
	}

	printf(" }\n");
}



void pgen_icmp::info(){
	compile();
	pgen_ip::info();

	std::map<int, const char*> _icmpcode;
	_icmpcode[0]  = "Net Unreachable";
	_icmpcode[1]  = "Host Unreachable";
	_icmpcode[2]  = "Protocol Unreachable";
	_icmpcode[3]  = "Port Unreachable";
	_icmpcode[255]  = "Not Set";

	printf(" * Internet Control Message Protocol \n");
	printf("    - Type            :  %d \n" , ICMP.type);
	printf("    - Code            :  %d \n", ICMP.code);
	printf("    - Header Checksum :  0x%x \n", icmp.icmp_cksum);


	if(ICMP.type==8 || ICMP.type==0){
		printf("    - Identifier      :  0x%04x \n", ICMP.echo.id);
		printf("    - Sequence Number :  %d \n", ICMP.echo.seq);
	}else if(ICMP.type==3){
		printf("    - Dest Unreach    :  %s \n", _icmpcode[ICMP.code]);
	}else if(ICMP.type==11){
		printf("    - Time Exceeded   :  This is Time Exceeded \n");
	}else if(ICMP.type==5){
		printf("    - Redirect GW     :  %s \n", ICMP.redirect.gw_addr.c_str()); 
	}else if(ICMP.type==9 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::info: router advertisement not implement  yet\n");
	}else if(ICMP.type==10 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::info: router solicitation not implement  yet\n");
	}else{
		fprintf(stderr, "pgen_icmp::info: icmp type & code is not support yet\n");
	}
}




void pgen_icmp::icmp_addData(const void* data, int len){
	memcpy(icmp_ext_data, data, len);
	icmp_ext_data_len = len;
}


void pgen_icmp::help(){
	printf("ICMP Packet CLass--------------------------------------------------\n");
	printf("type \n");
	printf("code \n");
	printf("echo.id \n");
	printf("echo.seq \n");
	printf("redirect.gw_addr \n");
	printf("destination_unreach.len \n");
	printf("destination_unreach.next_mtu \n");
	printf("time_exceeded.len \n");
	printf("type \n");
	printf("-------------------------------------------------------------------\n");
}
