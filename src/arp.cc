#include "packet.h"
#include "address.h"
#include "mytypes.h"

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
	clear();
}



pgen_arp::pgen_arp(const u_char* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_arp::clear(){
	pgen_eth::clear();
	ARP.srcIp = 0;
	ARP.dstIp = 0;
	ARP.srcEth = 0;
	ARP.dstEth = 0;
	ARP.operation = 1; // arp request
}   


void pgen_arp::send(const char* ifname){
	compile();		
	if(pgen_sendpacket_L2(ifname, data, len) < 0)
		exit(-1);	
}



void pgen_arp::compile(){
	pgen_eth::compile();
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
		arp.arp_spa[i] = ARP.srcIp.getOctet(i);
		arp.arp_tpa[i] = ARP.dstIp.getOctet(i);
	}
		
	u_char* p = data;
	memcpy(p, &eth, sizeof(eth));
	p += sizeof(eth);
	memcpy(p, &arp, sizeof(arp));
	p += sizeof(arp);
	len = p - data;

	_addData_WRAP();
}



void pgen_arp::cast(const u_char* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "arp packet length not support (%d)\n", len);
		return;
	}
	
	
	pgen_eth::cast(data, len);

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







void pgen_arp::summary(){
	if(ARP.operation == 1){
		printf("who has %s tell %s \n", ARP.dstIp.c_str(), ARP.srcEth.c_str());
	}else if(ARP.operation == 2){
		printf("%s is at %s\n", ARP.srcIp.c_str(), ARP.srcEth.c_str());	
	}else{
		printf("other arp operation!! \n");	
	}
}


void pgen_arp::info(){
	compile();
	pgen_eth::info();	

	std::map<int, const char*> _arpopcode;
	_arpopcode[1] = "ARP Request";
	_arpopcode[2] = "ARP Reply";
	_arpopcode[3] = "RARP Request";
	_arpopcode[4] = "RARP Reply";
	_arpopcode[-1] = "Not Set";

	printf(" * Address Resolution Protocol \n");
	printf("    - Opcode          :  %s (%d) \n", 
			_arpopcode[ARP.operation], ARP.operation);
	printf("    - Sender Mac      :  %s (%s) \n", 
			ARP.srcEth.c_str(), ARP.srcEth.bender());
	printf("    - Sender IP       :  %s  \n", 
			ARP.srcIp.c_str() );
	printf("    - Target Mac      :  %s (%s) \n", 
			ARP.dstEth.c_str(), ARP.dstEth.bender());
	printf("    - Target IP       :  %s  \n", ARP.dstIp.c_str() );
}
