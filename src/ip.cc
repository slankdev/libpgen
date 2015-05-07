#include "packet.h"
#include "pgen-variable.h"

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
	pgen_eth::clear();
	clear();	
}
void pgen_ip::clear(){
	ip_srcIp = 0;
	ip_dstIp = "127.0.0.1";
	ip_type = IPPROTO_IP;
	ip_tos = 0;
	ip_id = 1;
	ip_ttl = 64;
}


void pgen_ip::sendPack(const char* ifname){
	wrap(ifname);		
	int sock;
	int n;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_dstIp._addr;

	
	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(PGEN_ERROR);

	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_ip::wrap(const char* ifname){
	packetType = PGEN_PACKETTYPE_IP;
	pgen_eth::wrap(ifname);
	eth.ether_type = htons(ETHERTYPE_IP);
	memset(data, 0, sizeof data);

	memset(&ip, 0, sizeof ip);
	ip.ihl = sizeof(ip) / 4;
	ip.version = 4;
	ip.tos = ip_tos; //no useing world now
	ip.tot_len = sizeof(ip);
	ip.id = htons(ip_id);
	ip.frag_off = 0; // ?????
	ip.ttl = ip_ttl;
	ip.protocol = ip_type;
	ip.saddr = ip_srcIp._addr;
	ip.daddr = ip_dstIp._addr;
	ip.check = htons(checksum(&ip, sizeof(ip)));

	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(ip);
	len = p - data;
}


void pgen_ip::info(){
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
	printf("    - Source          :  %s \n", ip_srcIp.c_str());
	printf("    - Destination     :  %s \n", ip_dstIp.c_str());
	printf("    - Protocol        :  %s (%u) \n", _ipprot[ip.protocol],  ip.protocol);
	printf("    - Time to Leave   :  %d \n", ip.ttl);
	printf("    - Total Length    :  %d \n", ntohs(ip.tot_len));
	printf("    - Identification  :  %d \n", htons(ip.id));
}
