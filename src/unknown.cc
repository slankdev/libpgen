
#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"







pgen_unknown::pgen_unknown(){
	
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

	if(eth->ether_type == MT_ETHERTYPE_IP){
		_isIP = true;
		ip = (struct MYIP*)p;
		p += sizeof(struct MYIP);

		if(ip->protocol == MT_IPPROTO_ICMP){
			_isICMP = true;		
		}
		else if(ip->protocol == MT_IPPROTO_TCP){
			_isTCP = true;
		}
		else if(ip->protocol == MT_IPPROTO_UDP){
			_isUDP = true;
		}
	}
	
	else if(eth->ether_type == MT_ETHERTYPE_ARP){
		_isARP = true;
	}
	else{
		fprintf(stderr, "unknown L3 protocol\n");
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
