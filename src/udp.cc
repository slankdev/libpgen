
/* include must!!! */
#include "packet.h"
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"
#include "pgen-variable.h"


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



void pgen_udp::wrapLite(const char* ifname){
	memset(data, 0, sizeof data);
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip_dstIp._addr;
	sin.sin_port = htons(udp_dstPort);
	memcpy(&addr, &sin, sizeof(sin));

	if((sock=socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
		perror("udp::wrapLite socket()");
		exit(PGEN_ERROR);
	}

	memset(&udp, 0, sizeof udp);
	udp.uh_sport = htons(udp_srcPort);
	udp.uh_dport = htons(udp_dstPort);
	udp.uh_ulen = sizeof(udp);
	udp.uh_sum = 0;
	


	u_char* p = data;
	memcpy(p, &udp, sizeof(icmp));
	p += sizeof(udp);
	len = p - data;
}



void pgen_udp::wrap(const char* ifname){}




void pgen_udp::info(){
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("   - Source Port      :  %d \n", udp_srcPort);
	printf("   - Destination Port :  %d \n", udp_dstPort);
}





void pgen_udp::clear(){
	udp_srcPort = 0;
	udp_dstPort = 0;
}
