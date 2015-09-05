#include <map>
#include <string>
#include <iostream>

#include "packet.h"
#include "address.h"
#include "netutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>




pgen_icmp::pgen_icmp(){
	clear();
}


pgen_icmp::pgen_icmp(const void* packet, int len){
	clear();
	cast(packet, len);
}


void pgen_icmp::clear(){
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



// support icmp packet
//  - Echo,Echo Relay
//	- Destination Unreachable
//	- Time Exceeded
//	- Redirect (not yet)
//	- Router Solicitation (not yes)

void pgen_icmp::compile(){

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	memset(&this->icmp, 0, ICMP_HDR_LEN);
	this->icmp.icmp_type = this->ICMP.type;
	this->icmp.icmp_code = this->ICMP.code;
	this->icmp.icmp_cksum = 0;

	if(ICMP.type==8 || ICMP.type==0){  // Echo or Echo Relay
		struct icmp_echo_header ie;
		ie.id = htons(this->ICMP.echo.id);
		ie.seq = htons(this->ICMP.echo.seq);
		memcpy(icmp_data, &ie, sizeof(ie));
		icmp_data_len = sizeof(ie);
	}else if(ICMP.type==3){				// Destination Unreachable
		struct icmp_destination_unreach idu;
		idu.nouse = 0;
		idu.len   = 0;
		idu.next_mtu = htons(0);
		memcpy(icmp_data, &idu, sizeof(idu));
		icmp_data_len = sizeof(idu);
	}else if(ICMP.type==11){			// Time Exceeded
		struct icmp_time_exceeded ite;
		ite.nouse1 = 0;
		ite.len    = 0;  // kokoyare
		ite.nouse2 = htons(0);
		memcpy(icmp_data, &ite, sizeof(ite));
		icmp_data_len = sizeof(ite);
	}else if(ICMP.type==5){
		struct icmp_redirect ir;
		ir.gw_addr = this->ICMP.redirect.gw_addr._addr;
		memcpy(icmp_data, &ir, sizeof(ir));
		icmp_data_len = sizeof(ir);
	}else if(ICMP.type==9 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::compile: router advertisement not implement  yet\n");
	}else if(ICMP.type==10 && ICMP.code==0){
		fprintf(stderr, "pgen_icmp::compile: router solicitation not implement  yet\n");
	}else{
		fprintf(stderr, "pgen_icmp::compile: icmp type & code is not support yet\n");
	}

	this->IP.tot_len = IP_HDR_LEN + ICMP_HDR_LEN + icmp_data_len + icmp_ext_data_len;
	this->IP.protocol = 1;
	pgen_ip::compile();
	char buffer[256];

	char* p0 = buffer;
	memcpy(p0, &this->icmp, ICMP_HDR_LEN);
	p0 += ICMP_HDR_LEN;
	memcpy(p0, icmp_data, icmp_data_len);
	p0 += icmp_data_len;
	memcpy(p0, icmp_ext_data, icmp_ext_data_len);
	p0 += icmp_ext_data_len;
	this->icmp.icmp_cksum = (checksum((unsigned short*)buffer, (unsigned short)(p0-buffer)));


	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	memcpy(p, &this->icmp, ICMP_HDR_LEN);
	p += ICMP_HDR_LEN;
	memcpy(p, icmp_data, icmp_data_len);
	p += icmp_data_len;
	memcpy(p, icmp_ext_data, icmp_ext_data_len);
	p += icmp_ext_data_len;

	len = p-this->data;
} 




void pgen_icmp::cast(const void* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_icmp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_ip::cast(data, len);

	const u_char* p = (u_char*)data;
	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;
	
	struct MYICMP *buf = (struct MYICMP*)p;
	p += ICMP_HDR_LEN;

	this->ICMP.type = buf->icmp_type;
	this->ICMP.code = buf->icmp_code;

	memcpy(icmp_data, p, len-(p-(u_char*)data));
	icmp_data_len = len - (p-(u_char*)data);
	p += icmp_data_len;

	this->len = p - (u_char*)data;
} 



void pgen_icmp::summary(){
	compile();
	printf("ICMP{ ");
	if(ICMP.type == 8 && ICMP.code == 0){
		printf("Echo Request ttl=%d }\n",IP.ttl );
	}else if(ICMP.type == 0 && ICMP.code == 0){
		printf("Echo Relay ttl=%d }\n", IP.ttl );
	}else{
		printf("other icmp type }\n");	
	}
}



void pgen_icmp::info(){
	compile();
	pgen_ip::info();

	std::map<int, const char*>  _icmptype;
	_icmptype[0] = "Echo Reply";
	_icmptype[5] = "REdirect"; 
	_icmptype[8] = "Echo"; 
	std::map<int, const char*> _icmpcode;
	_icmpcode[0]  = "Net Unreachable";
	_icmpcode[1]  = "Host Unreachable";
	_icmpcode[2]  = "Protocol Unreachable";
	_icmpcode[3]  = "Port Unreachable";
	_icmpcode[255]  = "Not Set";

	printf(" * Internet Control Message Protocol \n");
	printf("    - Type          :  %s (%d)\n", 
			_icmptype[ICMP.type] , ICMP.type);
	printf("    - Code            :  %s (%d)\n",  
			_icmpcode[ICMP.code], ICMP.code);
	printf("    - Header Checksum :  0x%x \n", icmp.icmp_cksum);
}




void pgen_icmp::icmp_addData(const void* data, int len){
	memcpy(icmp_ext_data, data, len);
	icmp_ext_data_len = len;
}
