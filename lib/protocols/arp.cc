

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

#include <pgen/protocols/arp.h>





pgen_arp::pgen_arp(){
	clear();
}



pgen_arp::pgen_arp(const void* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_arp::clear(){
	pgen_eth::clear();
	this->ETH.type = 0x0806;

	this->ARP.srcIp = 0;
	this->ARP.dstIp = 0;
	this->ARP.srcEth = 0;
	this->ARP.dstEth = 0;
	this->ARP.operation = 1; // arp request
}   


void pgen_arp::compile(){
	u_char buf[1000];
	int buflen;

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->data;

	buflen = pgen_eth::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_arp::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;

	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	len = p - this->data;
}


void pgen_arp::cast(const void* data, int l){
	u_char* p = (u_char*)data;
	int buflen;

	buflen = pgen_eth::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_arp::read_bin(p, l);
	p += buflen;
	l -= buflen;
	
	add_data(p, l);
	len = p - (u_char*)data;
}



int  pgen_arp::write_bin(void* buf, int buflen){
	if(buflen < sizeof(struct arp_packet)){
		fprintf(stderr, "pgen_arp::write_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct arp_packet arp_pack;
	memset(&arp_pack, 0, sizeof arp_pack);
	
	arp_pack.ea_hdr.ar_hrd = htons(1);
	arp_pack.ea_hdr.ar_pro = htons(0x0800);
	arp_pack.ea_hdr.ar_hln = 6;
	arp_pack.ea_hdr.ar_pln = 4;
	arp_pack.ea_hdr.ar_op  = htons(this->ARP.operation);
	for(int i=0; i<6; i++){
		arp_pack.arp_sha[i] = this->ARP.srcEth._addr[i];
		arp_pack.arp_tha[i] = this->ARP.dstEth._addr[i];
	}
	for(int i=0; i<4; i++){
		arp_pack.arp_spa[i] = this->ARP.srcIp.getOctet(i);
		arp_pack.arp_tpa[i] = this->ARP.dstIp.getOctet(i);
	}
		
	memcpy(buf, &arp_pack, sizeof(arp_pack));
	return sizeof(struct arp_packet);	
}


int  pgen_arp::read_bin(const void* buf, int buflen){
	if(buflen < sizeof(struct arp_packet)){
		fprintf(stderr, "pgen_arp::read_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct arp_packet* arp_pack = (struct arp_packet*)buf;
	union lc{
		bit32 l;
		bit8 c[4];
	};
	lc slc, dlc;

	this->ARP.operation = ntohs(arp_pack->ea_hdr.ar_op);
	for(int i=0; i<6; i++){
		this->ARP.srcEth._addr[i] = arp_pack->arp_sha[i];
		this->ARP.dstEth._addr[i] = arp_pack->arp_tha[i];
	}
	for(int i=0; i<4; i++){
		slc.c[i] = arp_pack->arp_spa[i];
		dlc.c[i] = arp_pack->arp_tpa[i];
	}
	this->ARP.srcIp = slc.l;
	this->ARP.dstIp = dlc.l;

	return sizeof(struct arp_packet);	
}


void pgen_arp::summary(){
	printf("ARP{ ");
	if(ARP.operation == 1){
		printf("who has %s tell %s }\n", ARP.dstIp.c_str(), ARP.srcEth.c_str());
	}else if(ARP.operation == 2){
		printf("%s is at %s }\n", ARP.srcIp.c_str(), ARP.srcEth.c_str());	
	}else{
		printf("other arp operation!! }\n");	
	}
}



void pgen_arp::info(){
	compile();
	pgen_eth::info();	

	std::map<int, const char*> _arpopcode;
	_arpopcode[1] = "ARP Request";
	_arpopcode[2] = "ARP Reply";
	_arpopcode[3] = "RARP Request";
	_arpopcode[4] = "RARP Reply";
	_arpopcode[-1] = "Not Set";

	printf(" * Address Resolution Protocol \n");
	printf("    - Opcode          :  %s (%d) \n", 
			_arpopcode[ARP.operation], ARP.operation);
	printf("    - Sender Mac      :  %s (%s) \n", 
			ARP.srcEth.c_str(), ARP.srcEth.bender());
	printf("    - Sender IP       :  %s  \n", 
			ARP.srcIp.c_str() );
	printf("    - Target Mac      :  %s (%s) \n", 
			ARP.dstEth.c_str(), ARP.dstEth.bender());
	printf("    - Target IP       :  %s  \n", ARP.dstIp.c_str() );
}



void pgen_arp::help(){
	printf("ARP Packet CLass----------------------------------------------------\n");
	printf("operation : ARP operation code      : 16 bit field \n");
	printf("srcEth    : sender hardware address : 48 bit field \n");
	printf("srcIp     : sender protocol address : 32 bit field \n");
	printf("dstEth    : target hardware address : 48 bit field \n");
	printf("dstIp     : Target protocol address : 32 bit field \n");
	printf("--------------------------------------------------------------------\n");
}
