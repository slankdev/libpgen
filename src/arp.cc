#include "packet.h"
#include "address.h"

/* include must!!! */
#include "packconf.h"

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
	CLEAR();
}



pgen_arp::pgen_arp(const bit8* packet, int len){
	CLEAR();
	CAST(packet, len);
}



void pgen_arp::CLEAR(){
	pgen_eth::CLEAR();
	ARP.srcIp = 0;
	ARP.dstIp = 0;
	ARP.srcEth = 0;
	ARP.dstEth = 0;
	ARP.operation = PGEN_ARPOP_REQEST;
}   


void pgen_arp::SEND(const char* ifname){
	WRAP();		
	int sock;
	
	if((sock=initRawSocket(ifname, 0, 0)) < 0)
		exit(PGEN_ERROR);
	if(sendRawPacket(sock, data, len, 2, NULL) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_arp::WRAP(){
	pgen_eth::WRAP();
	packetType = PGEN_PACKETTYPE_ARP;
	memset(data, 0, sizeof data);
	eth.ether_type = htons(MT_ETHERTYPE_ARP);

	memset(&arp, 0, sizeof arp);
	arp.arp_hrd = htons(MT_ARPHRD_ETHER);
	arp.arp_pro = htons(MT_ETHERTYPE_IP);
	arp.arp_hln = 6;
	arp.arp_pln = 4;
	arp.arp_op  = htons(ARP.operation);
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


void pgen_arp::CAST(const bit8* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "arp packet length not support (%d)\n", len);
		return;
	}
	
	
	pgen_eth::CAST(data, len);

	struct MYARP buf;
	memcpy(&buf, data+sizeof(struct MYETH), sizeof(buf));;
	
	union lc{
		bit32 l;
		bit8 c[4];
	};
	lc slc, dlc;

	ARP.operation = ntohs(buf.arp_op);
	for(int i=0; i<6; i++){
		ARP.srcEth._addr[i] = buf.arp_sha[i];
		ARP.dstEth._addr[i] = buf.arp_tha[i];
	}
	for(int i=0; i<4; i++){
		slc.c[i] = buf.arp_spa[i];
		dlc.c[i] = buf.arp_tpa[i];
	}
	ARP.srcIp = slc.l;
	ARP.dstIp = dlc.l;

}







void pgen_arp::SUMMARY(){
	if(ARP.operation == 2){
		printf("who has %s tell %s \n", ARP.dstIp.c_str(), ARP.srcEth.c_str());
	}else if(ARP.operation == 1){
		printf("%s is at %s\n", ARP.srcIp.c_str(), ARP.srcEth.c_str());	
	}else{
		printf("other arp operation!! \n");	
	}
}


void pgen_arp::INFO(){
	WRAP();
	pgen_eth::INFO();	

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
