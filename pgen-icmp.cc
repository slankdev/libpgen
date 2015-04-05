#include "pgen-packet.h"
#include "pgen-variable.h"

/* include must!!! */
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"
#include "util.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <net/ethernet.h> 
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>



pgen_icmp::pgen_icmp(){
	clear();
}


void pgen_icmp::info(){
	printf("icmp::info()\n");	
}


void pgen_icmp::clear(){
	pgen_packet::clear();
	icmp_option -1;
	icmp_code = -1;
}



void pgen_icmp::compile(const char* ifname){
	pgen_packet::compile(ifname);
	packetType = PGEN_PACKETTYPE_ICMP;
	memset(data, 0, sizeof(data));
	memset(&eth, 0, sizeof eth);
	memset(&ip, 0, sizeof ip);		
	memset(&icmp, 0, sizeof icmp);
		
	eth.ether_type = htons(ETHERTYPE_IP);
	for(int i=0; i< 6; i++){
		eth.ether_shost[i] = eth_srcEth._addr[i];	
		eth.ether_dhost[i] = eth_dstEth._addr[i];	
	}
	ip.ihl = sizeof(ip) / 4;
	ip.version = 4;
	ip.tos = 0; //no useing world now
	ip.tot_len = sizeof(ip) + sizeof(icmp);
	ip.id = random(); // ??????
	ip.frag_off = 0; // ?????
	ip.ttl = PGEN_DEFAULT_TTL;
	ip.protocol = IPPROTO_ICMP;
	ip.saddr = ip_srcIp._addr;
	ip.daddr = ip_dstIp._addr;
	ip.check = checksum(&ip, sizeof(ip));
	icmp.type = icmp_option;
	icmp.code = icmp_code;
	icmp.checksum = checksum(&icmp, sizeof icmp);
	
	u_char* p = data;
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &icmp, sizeof(icmp));
	len = p-data;
	/*
	u_char* p = data;
	memcpy(p, &eth, sizeof(eth));
	p += sizeof(eth);
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &icmp, sizeof(icmp));
	len = p-data;
	*/
}





