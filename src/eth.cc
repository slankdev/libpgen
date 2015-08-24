
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
	clear();	
}



pgen_eth::pgen_eth(const u_char* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_eth::clear(){
	this->ETH.src = 0;
	this->ETH.dst = 0;
	this->ETH.type = htons(0);
}



void pgen_eth::compile(){
	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	for(int i=0; i< 6; i++){
		this->eth.ether_dhost[i] = this->ETH.dst._addr[i];	
		this->eth.ether_shost[i] = this->ETH.src._addr[i];	
	}
	this->eth.ether_type = htons(this->ETH.type);

	u_char* p = this->data;
	memcpy(p, &(this->eth), ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	len = p - this->data;
	
	compile_addData();
}



void pgen_eth::cast(const u_char* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_eth::cast(): packet len isn`t support (len=%d)\n", this->len);
		return;
	}

	const u_char* p = data;
	struct MYETH* buf;
	buf = (struct MYETH*)p;
	p += ETH_HDR_LEN;
	
	for(int i=0; i<6; i++){
		this->ETH.dst._addr[i] = buf->ether_dhost[i];
		this->ETH.src._addr[i] = buf->ether_shost[i];
	}
	this->ETH.type = ntohs(buf->ether_type);

	addData(p, len-(p-data));
}



void pgen_eth::send_L2(const char* ifname){
	compile();		
	if(pgen_sendpacket_L2(ifname, this->data, this->len) < 0)
		exit(-1);	
}



void pgen_eth::summary(){
	printf("Ethernet{%s -> %s type=0x%04x}\n", 
			ETH.src.c_str(), ETH.dst.c_str(), ETH.type);
}



void pgen_eth::info(){
	compile();
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
			_ethtype[ETH.type] ,ETH.type);
}



