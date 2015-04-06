#include "packet.h"
#include "pgen-variable.h"

/* include must!!! */
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"

#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <net/ethernet.h> 
#include <netinet/if_ether.h>


pgen_arp::pgen_arp(){
	clear();
}





void pgen_arp::wrap(const char* ifname){
	pgen_eth::wrap(ifname);

	packetType = PGEN_PACKETTYPE_ARP;
	eth.ether_type = htons(ETHERTYPE_ARP);
	memset(data, 0, sizeof data);
	memset(&arp, 0, sizeof arp);
	
	arp.arp_hrd = htons(ARPHRD_ETHER);
	arp.arp_pro = htons(ETHERTYPE_IP);
	arp.arp_hln = 6;
	arp.arp_pln = 4;
	arp.arp_op  = htons(arp_option);
	for(int i=0; i<6; i++){
		arp.arp_sha[i] = arp_srcEth._addr[i];
		arp.arp_tha[i] = arp_dstEth._addr[i];
	}
	for(int i=0; i<4; i++){
		arp.arp_spa[i] = arp_srcIp[i];
		arp.arp_tpa[i] = arp_dstIp[i];
	}
		
	u_char* p = data;
	memcpy(p, &eth, sizeof(eth));
	p += sizeof(eth);
	memcpy(p, &arp, sizeof(arp));
	p += sizeof(arp);
	len = p - data;


	if((sock=socket(AF_PACKET, SOCK_PACKET, htons(ETH_P_ARP))) < 0){
		perror("arp::wrap bind()");
		exit(PGEN_ERROR);
	}

	memset(&addr, 0, sizeof addr);
	addr.sa_family = AF_PACKET;
	snprintf(addr.sa_data, sizeof(addr.sa_data), "%s", ifname);
}







void pgen_arp::clear(){
	arp_srcIp = 0;
	arp_dstIp = 0;
	arp_srcEth = 0;
	arp_dstEth = 0;
	arp_option = -1;
}





void pgen_arp::info(){
	pgen_eth::info();	

	std::map<int, const char*> _arpopcode;
	_arpopcode[1] = "ARP Request";
	_arpopcode[2] = "ARP Reply";
	_arpopcode[3] = "RARP Request";
	_arpopcode[4] = "RARP Reply";
	_arpopcode[-1] = "Not Set";

	printf(" * Address Resolution Protocol \n");
	printf("    - Opcode          :  %s (%d) \n", _arpopcode[arp_option], arp_option);
	printf("    - Sender Mac      :  %s (%s) \n", arp_srcEth.c_str(), arp_srcEth.bender());
	printf("    - Sender IP       :  %s  \n", arp_srcIp.c_str() );
	printf("    - Target Mac      :  %s (%s) \n", arp_dstEth.c_str(), arp_dstEth.bender());
	printf("    - Target IP       :  %s  \n", arp_dstIp.c_str() );
}
