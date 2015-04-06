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
#include <netinet/tcp.h>		// for struct tcp		


pgen_tcp::pgen_tcp(){
	clear();
}



void pgen_tcp::wrapLite(const char* ifname){
	memset(data, 0, sizeof data);
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip_dstIp._addr;
	sin.sin_port = htons(tcp_dstPort);
	memcpy(&addr, &sin, sizeof(sin));

	if((sock=socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0){
		perror("tcp::wrapLite socket()");
		exit(PGEN_ERROR);
	}

	memset(&tcp, 0, sizeof tcp);
	tcp.source = htons(tcp_srcPort);
	tcp.dest   = htons(tcp_dstPort);
	tcp.seq    = 0;
	tcp.ack_seq = 0;
	tcp.doff = sizeof(tcp);
	tcp.window = 1500;
	tcp.check  = 0;

	u_char* p = data;
	memcpy(p, &tcp, sizeof(icmp));
	p += sizeof(tcp);
	len = p - data;
}



void pgen_tcp::wrap(const char* ifname){}




void pgen_tcp::info(){
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("   - Source Port      :  %d \n", tcp_srcPort);
	printf("   - Destination Port :  %d \n", tcp_dstPort);
}





void pgen_tcp::clear(){
	tcp_srcPort = 0;
	tcp_dstPort = 0;
}
