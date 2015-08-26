
/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "address.h"

#include "netutil.h"

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


pgen_udp::pgen_udp(const u_char* packet, int len){
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



void pgen_udp::cast(const u_char* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_tcp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_ip::cast(data, len);

	const u_char* p = data;
	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;

	struct MYUDP *buf = (struct MYUDP*)p;
	p += UDP_HDR_LEN;

	this->UDP.src = ntohs(buf->source);
	this->UDP.dst = ntohs(buf->dest);
	this->UDP.len = ntohs(buf->len);
	
	this->len = p - data;
	addData(p, len-(p-data));
}



void pgen_udp::summary(){
	printf("UDP(%d -> %d) \n", UDP.src, UDP.dst);
}



void pgen_udp::info(){
	compile();
	pgen_ip::info();

	printf(" * User Datagram Protocol \n");
	printf("    - Source Port     :  %d (%s)\n", 
			UDP.src, pgen_port2service(UDP.src, 2));
	printf("    - Destination Port:  %d (%s)\n", 
			UDP.dst, pgen_port2service(UDP.dst, 2));
	printf("    - Length          :  %d \n", UDP.len);
}



