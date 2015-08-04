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
	CLEAR();	
}



pgen_ip::pgen_ip(const bit8* packet, int len){
	CLEAR();	
	CAST(packet, len);
}




void pgen_ip::CLEAR(){
	pgen_eth::CLEAR();
	IP.src = 0;
	IP.dst = "127.0.0.1";
	IP.protocol = IPPROTO_IP;
	IP.tos = 0;
	IP.id = 1;
	IP.ttl = 64;
}


void pgen_ip::SEND(const char* ifname){
	WRAP();		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;
	
	if((sock=initRawSocket(ifname, 0, 1)) < 0)
		exit(PGEN_ERROR);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_ip::WRAP(){
	packetType = PGEN_PACKETTYPE_IP;
	pgen_eth::WRAP();
	eth.ether_type = htons(MT_ETHERTYPE_IP);
	memset(data, 0, sizeof data);

	memset(&ip, 0, sizeof ip);
	ip.ihl = sizeof(ip) / 4;
	ip.version = 4;
	ip.tos = IP.tos; //no useing world now
	ip.tot_len = sizeof(ip);
	ip.id = htons(IP.id);
	ip.frag_off = 0; // ?????
	ip.ttl = IP.ttl;
	ip.protocol = IP.protocol;
	ip.saddr = IP.src._addr;
	ip.daddr = IP.dst._addr;
	ip.check = htons(checksum(&ip, sizeof(ip)));

	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(ip);
	len = p - data;
}




void pgen_ip::CAST(const bit8* data, int len){
	pgen_eth::CAST(data, len);

	struct MYIP buf;
	memcpy(&buf, data+sizeof(struct MYETH), sizeof(buf));
	
	IP.protocol = buf.protocol;
	IP.src._addr = buf.saddr;
	IP.dst._addr = buf.daddr;
	IP.tos = buf.tos;
	IP.id = ntohs(buf.id);
	IP.ttl = buf.ttl;
}


void pgen_ip::SUMMARY(){
	WRAP();

	printf("IP %s(%s) -> %s(%s) \n", IP.src.c_str(), ETH.src.c_str(),
			IP.dst.c_str(), ETH.dst.c_str());
}



void pgen_ip::INFO(){
	WRAP();
	pgen_eth::INFO();
	
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
			_ipprot[ip.protocol],  ip.protocol);
	printf("    - Time to Leave   :  %d \n", ip.ttl);
	printf("    - Total Length    :  %d \n", ntohs(ip.tot_len));
	printf("    - Identification  :  %d \n", htons(ip.id));
}
