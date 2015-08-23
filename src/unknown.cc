
#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"

#include "debug.h"
#define PACKET_MINLEN 14



pgen_unknown::pgen_unknown(){
	CLEAR();
}


pgen_unknown::pgen_unknown(const bit8* packet, int len){
	CLEAR();
	CAST(packet, len);
}


void pgen_unknown::CLEAR(){
	ETH.src = 0;
	ETH.dst = 0;
	IP.src = 0;
	IP.dst = 0;
	TCP.src = 0;
	TCP.dst = 0;
	UDP.src = 0;
	UDP.dst = 0;
	_isETH = false;
	_isARP = false;
	_isIP  = false;
	_isICMP= false;
	_isTCP = false;
	_isUDP = false;
}


void pgen_unknown::SUMMARY(){
	printf("unknown(packet=");
	if(isTCP())			
		printf("TCP %s:%d > %s:%d", IP.src.c_str(), TCP.src, IP.dst.c_str(), TCP.dst);
	else if(isUDP())	
		printf("UDP %s:%d > %s:%d", IP.src.c_str(), UDP.src, IP.dst.c_str(), UDP.dst);
	else if(isICMP())	
		printf("ICMP %s > %s", IP.src.c_str(), IP.dst.c_str());
	else if(isIP())		
		printf("IP   %s > %s", IP.src.c_str(), IP.dst.c_str()); 
	else if(isARP())	
		printf("ARP  %s > %s", ETH.src.c_str(), ETH.dst.c_str()); 
	else if(isETH())	
		printf("ETH  %s > %s", ETH.src.c_str(), ETH.dst.c_str());
	else			
		printf("no support");
	printf(" len=%d\n", len);
}


bool pgen_unknown::CAST(const bit8* packet, int len){
	CLEAR();
	if(!(14 < len && len < PGEN_PACKLEN)){
		fprintf(stderr, "recv packet length is not support (len=%d)\n", len);
		return false;
	}
	this->len = len;

	struct MYETH*  eth;
	struct MYARP*  arp;
	struct MYIP*   ip;
	struct MYICMP* icmp;
	struct MYTCP*  tcp;
	struct MYUDP*  udp;
		
	const bit8* p = packet;
	_isETH = true;
	eth = (struct MYETH*)p;
	p += sizeof(struct MYETH);
	ETH.src.setmacbyarry(eth->ether_shost);
	ETH.dst.setmacbyarry(eth->ether_dhost);

	if(ntohs(eth->ether_type) == MT_ETHERTYPE_IP){
		_isIP = true;
		ip = (struct MYIP*)p;
		p += sizeof(struct MYIP);
		IP.src._addr = ip->saddr;
		IP.dst._addr = ip->daddr;

		if(ip->protocol == MT_IPPROTO_ICMP){
			_isICMP = true;
			icmp = (struct MYICMP*)p;
			p += sizeof(struct MYICMP);
		}
		else if(ip->protocol == MT_IPPROTO_TCP){
			_isTCP = true;
			tcp = (struct MYTCP*)p;
			p += sizeof(struct MYTCP);
			TCP.src = ntohs(tcp->source);
			TCP.dst = ntohs(tcp->dest);
		}
		else if(ip->protocol == MT_IPPROTO_UDP){
			_isUDP = true;
			udp = (struct MYUDP*)p;
			p += sizeof(struct MYUDP);
			UDP.src = ntohs(udp->source);
			UDP.dst = ntohs(udp->dest);
		}
		else{
			//fprintf(stderr, "unknown L4 protocol 0x%04x\n", ip->protocol);
			return false;
		}
	}
	
	else if(ntohs(eth->ether_type) == MT_ETHERTYPE_ARP){
		_isARP = true;
	}
	else{
		//fprintf(stderr, "unknown L3 protocol 0x%04x\n", ntohs(eth->ether_type));
		return false;
	}
	
	return true;
}


bool pgen_unknown::isETH(){
	return _isETH;
}
bool pgen_unknown::isARP(){
	return _isARP;
}
bool pgen_unknown::isIP(){
	return _isIP;
}
bool pgen_unknown::isICMP(){
	return _isICMP;
}
bool pgen_unknown::isTCP(){
	return _isTCP;
}
bool pgen_unknown::isUDP(){
	return _isUDP;
}
