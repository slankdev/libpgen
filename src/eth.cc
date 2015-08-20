
#include "pgen.h"
#include "packet.h"
#include "address.h"

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

#include "netutil.h"



pgen_eth::pgen_eth(){
	CLEAR();	
}


pgen_eth::pgen_eth(const bit8* packet, int len){
	CLEAR();
	CAST(packet, len);
}


void pgen_eth::CLEAR(){
	ETH.src = 0;
	ETH.dst = 0;
	ETH.type = htons(0);
}



void pgen_eth::SEND(const char* ifname){
	WRAP();		
	if(pgen_sendpacket_L2(ifname, data, len) < 0)
		exit(-1);	
}



void pgen_eth::WRAP(){
	memset(data, 0, sizeof data);
	eth.ether_type = htons(ETH.type);;

	for(int i=0; i< 6; i++){
		eth.ether_shost[i] = ETH.src._addr[i];	
		eth.ether_dhost[i] = ETH.dst._addr[i];	
	}

	u_char* p = data;
	memcpy(data, &eth, sizeof eth);
	p += sizeof(eth);
	len = p - data;
	
	_addData_WRAP();
}




void pgen_eth::CAST(const bit8* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "eth packet length not support (%d)\n", len);
		return;
	}
	
	
	struct MYETH* buf;
	buf = (struct MYETH*)data;

	ETH.type = ntohs(buf->ether_type);
	for(int i=0; i<6; i++){
		ETH.src._addr[i] = buf->ether_shost[i];
		ETH.dst._addr[i] = buf->ether_dhost[i];
	}
}



void pgen_eth::SUMMARY(){
	printf("Ethernet(0x%04x) %s -> %s \n", 
			ETH.type, ETH.src.c_str(), ETH.dst.c_str());
		
}


void pgen_eth::INFO(){
	WRAP();
	std::map<int , const char*> _ethtype;
	_ethtype[0x0800] = "IPv4";
	_ethtype[0x0806] = "ARP";
	_ethtype[0x8035] = "RARP";
	_ethtype[0x8191] = "NetBios";
	_ethtype[0x86dd] = "IPv6";

	printf(" * Ethernet  %s -> %s \n", ETH.src.bender(), ETH.dst.bender());
	printf("    - Destination     :  %s (%s)   \n", 
			ETH.dst.c_str(), ETH.dst.bender());
	printf("    - Source          :  %s (%s)   \n" , 
			ETH.src.c_str(), ETH.src.bender());
	printf("    - Type            :  %s  (0x%04x)   \n", 
			_ethtype[htons(eth.ether_type)] ,htons(eth.ether_type));
}


