
/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "packconf.h"
#include "pgen-funcs.h"
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
	CLEAR();
}

void pgen_udp::CLEAR(){
	pgen_ip::CLEAR();
	UDP.srcPort = 53;
	UDP.dstPort = 53;
}

void pgen_udp::SEND(const char* ifname){
	WRAP();		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;

	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(-1);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_udp::WRAP(){
	pgen_ip::WRAP();
	ip.protocol = MT_IPPROTO_UDP;
	ip.tot_len = htons(sizeof(ip) + sizeof(udp));

	memset(&udp, 0, sizeof udp);
	udp.source = htons(UDP.srcPort);
	udp.dest   = htons(UDP.dstPort);
	udp.len    = htons(sizeof(udp));
	udp.check  = 0;
	
	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct udphdr);
	len = p-data;
}


void pgen_udp::INFO(){
	WRAP();
	pgen_ip::INFO();

	printf(" * User Datagram Protocol \n");
	printf("   - Source Port      :  %d (%s)\n", 
			ntohs(udp.source), port2service(ntohs(udp.source), 2));
	printf("   - Destination Port :  %d (%s)\n", 
			ntohs(udp.dest), port2service(ntohs(udp.dest), 2));
}




void pgen_udp::setData(const u_char *p, int len){}

