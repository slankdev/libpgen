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

#include "netutil.h"


pgen_arp::pgen_arp(){
	pgen_eth::clear();
	clear();
}

void pgen_arp::clear(){
	ARP.srcIp = 0;
	ARP.dstIp = 0;
	ARP.srcEth = 0;
	ARP.dstEth = 0;
	ARP.option = PGEN_ARPOP_REQEST;
}   


void pgen_arp::sendPack(const char* ifname){
	wrap(ifname);		
	int sock;
	
	if((sock=initRawSocket(ifname, 2)) < 0)
		exit(PGEN_ERROR);
	if(sendRawPacket(sock, data, len, 2, NULL) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_arp::wrap(const char* ifname){
	pgen_eth::wrap(ifname);
	packetType = PGEN_PACKETTYPE_ARP;
	memset(data, 0, sizeof data);
	eth.ether_type = htons(ETHERTYPE_ARP);

	memset(&arp, 0, sizeof arp);
	arp.arp_hrd = htons(ARPHRD_ETHER);
	arp.arp_pro = htons(ETHERTYPE_IP);
	arp.arp_hln = 6;
	arp.arp_pln = 4;
	arp.arp_op  = htons(ARP.option);
	for(int i=0; i<6; i++){
		arp.arp_sha[i] = ARP.srcEth._addr[i];
		arp.arp_tha[i] = ARP.dstEth._addr[i];
	}
	for(int i=0; i<4; i++){
		arp.arp_spa[i] = ARP.srcIp[i];
		arp.arp_tpa[i] = ARP.dstIp[i];
	}
		
	u_char* p = data;
	memcpy(p, &eth, sizeof(eth));
	p += sizeof(eth);
	memcpy(p, &arp, sizeof(arp));
	p += sizeof(arp);
	len = p - data;
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
	printf("    - Opcode          :  %s (%d) \n", 
			_arpopcode[ntohs(arp.arp_op)], ntohs(arp.arp_op));
	printf("    - Sender Mac      :  %s (%s) \n", 
			ARP.srcEth.c_str(), ARP.srcEth.bender());
	printf("    - Sender IP       :  %s  \n", 
			ARP.srcIp.c_str() );
	printf("    - Target Mac      :  %s (%s) \n", 
			ARP.dstEth.c_str(), ARP.dstEth.bender());
	printf("    - Target IP       :  %s  \n", ARP.dstIp.c_str() );
}
