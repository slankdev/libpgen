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
	IP.tos = 0;
	IP.tot_len = 0;
	IP.id = 1;
	IP.frag_off = 0;
	IP.ttl = 64;
	IP.protocol = IPPROTO_IP;
	IP.src = 0;
	IP.dst = "127.0.0.1";
}



void pgen_ip::send_L3(const char* ifname){
	compile();	
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, data+sizeof(struct MYETH), len-sizeof(struct MYETH),
				(struct sockaddr*)&addr) < 0)
		exit(-1);
}



void pgen_ip::send(const char* ifname){
	compile();		
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, data, len, (struct sockaddr*)&addr) < 0)
		exit(-1);
}



void pgen_ip::compile(){
	ETH.type = MT_ETHERTYPE_IP;
	pgen_eth::compile();
	memset(data, 0, sizeof data);

	memset(&ip, 0, sizeof ip);
	ip.ihl = sizeof(ip) / 4;
	ip.version = 4;
	ip.tos = IP.tos; //no useing world now
	ip.tot_len = htons(IP.tot_len);
	ip.id = htons(IP.id);
	ip.frag_off = htons(IP.frag_off); // ?????
	ip.ttl = IP.ttl;
	ip.protocol = IP.protocol;
	ip.saddr = IP.src._addr;
	ip.daddr = IP.dst._addr;
	ip.check = 0;
	ip.check = checksum((unsigned short*)&ip, sizeof(ip));

	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(ip);
	len = p - data;

	compile_addData();
}




void pgen_ip::cast(const bit8* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "ip packet length not support (%d)\n", len);
		return;
	}
	
	pgen_eth::cast(data, len);
	struct MYIP *buf;
	const u_char* p = data;
	p += sizeof(struct MYETH);
	buf = (struct MYIP*)p;
	p += sizeof(struct MYIP);
	
	IP.tos = buf->tos;
	IP.tot_len = ntohs(buf->tot_len);
	IP.id = ntohs(buf->id);
	IP.frag_off = ntohs(buf->frag_off);
	IP.ttl = buf->ttl;
	IP.protocol = buf->protocol;
	IP.src._addr = buf->saddr;
	IP.dst._addr = buf->daddr;

	addData(p, len-(p-data));
}


void pgen_ip::summary(){
	compile();
	printf("IP %s(%s) -> %s(%s) \n", IP.src.c_str(), ETH.src.c_str(),
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
