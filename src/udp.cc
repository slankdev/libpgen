
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
	IP.protocol = 17;
	IP.tot_len = (sizeof(ip) + UDP.len);
	pgen_ip::compile();

	memset(&udp, 0, sizeof udp);
	udp.source = htons(UDP.src);
	udp.dest   = htons(UDP.dst);
	udp.len    = htons(UDP.len);
	udp.check  = 0;
	
	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct MYIP);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	len = p-data;
	
	compile_addData();
}



void pgen_udp::cast(const u_char* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "udp packet length not support (%d)\n", len);
		return;
	}
	
	
	pgen_ip::cast(data, len);
	


	struct MYUDP buf;
	memcpy(&buf, data+sizeof(struct MYETH)+sizeof(struct MYIP),
			sizeof(buf));
	
	UDP.src = ntohs(buf.source);
	UDP.dst = ntohs(buf.dest);

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
}



