

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
#include "address.h"
#include "mytypes.h"

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
#include <netinet/if_ether.h>

#include "netutils.h"




pgen_arp::pgen_arp(){
	clear();
}



pgen_arp::pgen_arp(const void* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_arp::clear(){
	pgen_eth::clear();
	this->ARP.srcIp = 0;
	this->ARP.dstIp = 0;
	this->ARP.srcEth = 0;
	this->ARP.dstEth = 0;
	this->ARP.operation = 1; // arp request
}   



void pgen_arp::compile(){
	this->ETH.type = 0x0806;
	pgen_eth::compile();
	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	memset(&(this->arp), 0, ARP_HDR_LEN);
	
	this->arp.arp_hrd = htons(1);
	this->arp.arp_pro = htons(0x0800);
	this->arp.arp_hln = 6;
	this->arp.arp_pln = 4;
	this->arp.arp_op  = htons(this->ARP.operation);
	for(int i=0; i<6; i++){
		this->arp.arp_sha[i] = this->ARP.srcEth._addr[i];
		this->arp.arp_tha[i] = this->ARP.dstEth._addr[i];
	}
	for(int i=0; i<4; i++){
		this->arp.arp_spa[i] = this->ARP.srcIp.getOctet(i);
		this->arp.arp_tpa[i] = this->ARP.dstIp.getOctet(i);
	}
		
	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->arp, ARP_HDR_LEN);
	p += ARP_HDR_LEN;
	len = p - this->data;

	compile_addData();
}



void pgen_arp::cast(const void* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_arp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_eth::cast(data, len);

	struct arp_packet* buf;
	const u_char* p = (u_char*)data;
	p += ETH_HDR_LEN;

	buf = (struct arp_packet*)p;
	p += ARP_HDR_LEN;
	
	union lc{
		bit32 l;
		bit8 c[4];
	};
	lc slc, dlc;

	this->ARP.operation = ntohs(buf->arp_op);
	for(int i=0; i<6; i++){
		this->ARP.srcEth._addr[i] = buf->arp_sha[i];
		this->ARP.dstEth._addr[i] = buf->arp_tha[i];
	}
	for(int i=0; i<4; i++){
		slc.c[i] = buf->arp_spa[i];
		dlc.c[i] = buf->arp_tpa[i];
	}
	this->ARP.srcIp = slc.l;
	this->ARP.dstIp = dlc.l;
	
	len = p - (u_char*)data;
	addData(p, len-(p-(u_char*)data));
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
	printf("operation \n");
	printf("srcEth \n");
	printf("srcIp \n");
	printf("dstEth \n");
	printf("dstIp \n");
	printf("--------------------------------------------------------------------\n");
}
