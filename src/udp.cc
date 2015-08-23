
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
}


void pgen_udp::send(const char* ifname){
	compile();		
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, data, len, (struct sockaddr*)&addr) < 0)
		exit(-1);
}




void pgen_udp::compile(){
	IP.protocol = 17;
	pgen_ip::compile();
	ip.tot_len = htons(sizeof(ip) + sizeof(udp));

	memset(&udp, 0, sizeof udp);
	udp.source = htons(UDP.src);
	udp.dest   = htons(UDP.dst);
	udp.len    = htons(sizeof(udp));
	udp.check  = 0;
	
	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct MYIP);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	len = p-data;
	
	_addData_WRAP();
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
			ntohs(UDP.src), pgen_port2service(UDP.src, 2));
	printf("    - Destination Port:  %d (%s)\n", 
			ntohs(UDP.dst), pgen_port2service(UDP.dst, 2));
}



