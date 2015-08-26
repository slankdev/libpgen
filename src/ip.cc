#include "packet.h"
#include "pgen.h"
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
#include <netinet/ip.h>

#include "netutil.h"




pgen_ip::pgen_ip(){
	clear();	
}



pgen_ip::pgen_ip(const u_char* packet, int len){
	clear();	
	cast(packet, len);
}



void pgen_ip::clear(){
	pgen_eth::clear();
	this->IP.tos = 0;
	this->IP.tot_len = 0;
	this->IP.id = 1;
	this->IP.frag_off = 0;
	this->IP.ttl = 64;
	this->IP.protocol = IPPROTO_IP;
	this->IP.src = 0;
	this->IP.dst = "127.0.0.1";
}



void pgen_ip::compile(){
	ETH.type = MT_ETHERTYPE_IP;
	pgen_eth::compile();
	
	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	memset(&this->ip, 0, IP_HDR_LEN);
	this->ip.ihl = IP_HDR_LEN / 4;
	this->ip.version = 4;
	this->ip.tos = this->IP.tos; //no useing world now
	this->ip.tot_len = htons(this->IP.tot_len);
	this->ip.id = htons(this->IP.id);
	this->ip.frag_off = htons(this->IP.frag_off); // ?????
	this->ip.ttl = this->IP.ttl;
	this->ip.protocol = this->IP.protocol;
	this->ip.saddr = this->IP.src._addr;
	this->ip.daddr = this->IP.dst._addr;
	this->ip.check = 0;
	this->ip.check = checksum((unsigned short*)&this->ip, IP_HDR_LEN);

	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	this->len = p - this->data;

	compile_addData();
}



void pgen_ip::cast(const bit8* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_ip::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_eth::cast(data, len);

	const u_char* p = data;
	p += ETH_HDR_LEN;
	struct MYIP* buf = (struct MYIP*)p;
	p += IP_HDR_LEN;
	
	this->IP.tos = buf->tos;
	this->IP.tot_len = ntohs(buf->tot_len);
	this->IP.id = ntohs(buf->id);
	this->IP.frag_off = ntohs(buf->frag_off);
	this->IP.ttl = buf->ttl;
	this->IP.protocol = buf->protocol;
	this->IP.src._addr = buf->saddr;
	this->IP.dst._addr = buf->daddr;
	
	len = p - data;
	addData(p, len-(p-data));
}



void pgen_ip::send_L3(const char* ifname){
	compile();	
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = this->IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, this->data+ETH_HDR_LEN, this->len-ETH_HDR_LEN,
				(struct sockaddr*)&addr) < 0)
		exit(-1);
}



void pgen_ip::summary(){
	compile();
	printf("IP{ %s(%s) -> %s(%s) }\n", IP.src.c_str(), ETH.src.c_str(),
			IP.dst.c_str(), ETH.dst.c_str());
}



void pgen_ip::info(){
	compile();
	pgen_eth::info();
	
	std::map<int, const char*> _ipprot;
	_ipprot[0]  = "Not Set. Empty IP Packet";
	_ipprot[1]  = "ICMP";
	_ipprot[2]  = "IGMP";
	_ipprot[4]  = "IPv4 on IP";
	_ipprot[6]  = "TCP";
	_ipprot[17] = "UDP";
	_ipprot[41] = "IPv6 on IP";

	printf(" * Internet Protocol version 4\n");
	printf("    - Source          :  %s \n", IP.src.c_str());
	printf("    - Destination     :  %s \n", IP.dst.c_str());
	printf("    - Protocol        :  %s (%u) \n", 
			_ipprot[IP.protocol],  IP.protocol);
	printf("    - Time to Leave   :  %d \n", IP.ttl);
	printf("    - Total Length    :  %d \n", IP.tot_len);
	printf("    - Identification  :  %d \n", IP.id);
}
