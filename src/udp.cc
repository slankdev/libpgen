
/* include must!!! */
#include "packet.h"
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"
#include "pgen-variable.h"

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
	pgen_ip::clear();
	clear();
}

void pgen_udp::clear(){
	udp_srcPort = 53;
	udp_dstPort = 53;
}

void pgen_udp::sendPack(const char* ifname){
	wrap(ifname);		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_dstIp._addr;

	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(-1);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_udp::wrap(const char* ifname){
	pgen_ip::wrap(ifname);
	packetType = PGEN_PACKETTYPE_UDP;
	ip.protocol = IPPROTO_UDP;
	ip.tot_len = htons(sizeof(ip) + sizeof(udp));

	memset(&udp, 0, sizeof udp);
	udp.source = htons(udp_srcPort);
	udp.dest   = htons(udp_dstPort);
	udp.len    = htons(sizeof(udp));
	udp.check  = 0;
	
	u_char* p = data;
	//memcpy(p, &eth, sizeof eth);
	//p += sizeof(struct ether_header);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct udphdr);
	len = p-data;
}



void pgen_udp::info(){
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("   - Source Port      :  %d \n", udp_srcPort);
	printf("   - Destination Port :  %d \n", udp_dstPort);
}




void pgen_udp::setData(const u_char *p, int len){}

