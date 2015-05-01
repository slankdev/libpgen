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
	ip_dstIp = 0;
}


void pgen_ip::send(const char* ifname){
	int sock;
	int n;
	
	/*
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_dstIp._addr;
	*/

	wrap(ifname);		
	
	
	if((sock=initRawSocket(ifname, 1, 0)) < 0){
		exit(PGEN_ERROR);
	}
	if((n=write(sock, data, len)) < 0){
//	if((n=sendto(sock, data, len, 0, (struct sockaddr*)&addr, sizeof addr)) < 0){
		perror("ip::send sendto()");
		exit(PGEN_ERROR);
	}

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
	ip.tos = 0; //no useing world now
	ip.tot_len = sizeof(ip);
	ip.id = random(); // ??????
	ip.frag_off = 0; // ?????
	ip.ttl = PGEN_DEFAULT_TTL;
	ip.protocol = IPPROTO_IP;
	ip.saddr = ip_srcIp._addr;
	ip.daddr = ip_dstIp._addr;
	ip.check = checksum(&ip, sizeof(ip));

	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(ip);
	len = p - data;
}
/*
void pgen_ip::wrapLite(const char* ifname){
	int sock;
	packetType = PGEN_PACKETTYPE_IP;
	memset(data, 0, sizeof data);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip_dstIp._addr;
	if((sock=socket(AF_INET, SOCK_RAW, htons(ETH_P_IP))) < 0){
		perror("ip::wrapLite socket()");
		exit(PGEN_ERROR);
	}

	memset(&ip, 0, sizeof ip);
	ip.ihl = sizeof(ip) / 4;
	ip.version = 4;
	ip.tos = 0; //no useing world now
	ip.tot_len = sizeof(ip);
	ip.id = random(); // ??????
	ip.frag_off = 0; // ?????
	ip.ttl = PGEN_DEFAULT_TTL;
	ip.protocol = IPPROTO_IP;
	ip.saddr = ip_srcIp._addr;
	ip.daddr = ip_dstIp._addr;
	ip.check = checksum(&ip, sizeof(ip));

	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(ip);
	len = p - data;
}
*/


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
}
