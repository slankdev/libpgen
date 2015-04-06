

/* include must!!! */
#include "pgen-packet.h"
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"
#include "pgen-variable.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>		/* for struct tcp			*/



pgen_tcp::pgen_tcp(){
	clear();		
}



void pgen_tcp::info(){
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port      :  %d\n", tcp_srcPort);
	printf("    - Destination Port :  %d \n", tcp_dstPort);
}







void pgen_tcp::wrap(const char* ifname){
	pgen_ip::wrap(ifname);
	ip.protocol = IPPROTO_TCP;
	
	memset(&data, 0, sizeof data);
	memset(&tcp, 0, sizeof tcp);


	if((sock=socket(AF_PACKET, SOCK_PACKET, htons(IPPROTO_TCP))) < 0){
		perror("arp::wrap bind()");
		exit(PGEN_ERROR);
	}

	memset(&addr, 0, sizeof addr);
	addr.sa_family = AF_PACKET;
	snprintf(addr.sa_data, sizeof(addr.sa_data), "%s", ifname);
	if(bind(sock, &addr, sizeof(addr)) < 0){
		perror("arp::wrap bind()");
		exit(PGEN_ERROR);
	}
}





void pgen_tcp::clear(){
	tcp_srcPort = 0;
	tcp_dstPort = 0;
}
