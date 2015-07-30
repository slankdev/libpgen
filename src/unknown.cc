
#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"





pgen_unknown::pgen_unknown(){
	L3.src = 0;
	L3.dst = 0;
}


pgen_unknown::pgen_unknown(const bit8* packet, int len){
	L3.src = 0;
	L3.dst = 0;
	CAST(packet, len);
}

bool pgen_unknown::CAST(const bit8* packet, int len){
	if(len > PGEN_PACKLEN){
		fprintf(stderr, "recv packet is too large!\n");
		return false;
	}
	bit8 data[PGEN_PACKLEN];
	memcpy(data, packet, len);

	struct MYETH*  eth;
	struct MYARP*  arp;
	struct MYIP*   ip;
	struct MYICMP* icmp;
	struct MYTCP*  tcp;
	struct MYUDP*  udp;
	_isETH = false;
	_isARP = false;
	_isIP  = false;
	_isICMP= false;
	_isTCP = false;
	_isUDP = false;
		
	const bit8* p = data;
	_isETH = true;
	eth = (struct MYETH*)p;
	p += sizeof(struct MYETH);
	


	if(ntohs(eth->ether_type) == MT_ETHERTYPE_IP){
		_isIP = true;
		ip = (struct MYIP*)p;
		p += sizeof(struct MYIP);
		L3.src._addr = ip->saddr;
		L3.dst._addr = ip->daddr;


		if(ip->protocol == MT_IPPROTO_ICMP){
			_isICMP = true;
			p += sizeof(struct MYICMP);
		}
		else if(ip->protocol == MT_IPPROTO_TCP){
			_isTCP = true;
			p += sizeof(struct MYTCP);
		}
		else if(ip->protocol == MT_IPPROTO_UDP){
			_isUDP = true;
			p += sizeof(struct MYUDP);
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
