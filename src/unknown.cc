
#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"

#include "debug.h"


#define PACKET_MINLEN 14





pgen_unknown::pgen_unknown(){
	clear();
}




pgen_unknown::pgen_unknown(const bit8* packet, int len){
	clear();
	cast(packet, len);
}




void pgen_unknown::clear(){
	len = 0;
	isETH = false;
	isARP = false;
	isIP  = false;
	isICMP= false;
	isTCP = false;
	isUDP = false;
	ETH.src = 0;
	ETH.dst = 0;
	IP.src = 0;
	IP.dst = 0;
	TCP.src = 0;
	TCP.dst = 0;
	UDP.src = 0;
	UDP.dst = 0;
	memset(data, 0 , PGEN_MAX_PACKET_LEN);
}



void pgen_unknown::send_handle(pgen_t* handle){
	pgen_sendpacket_handle(handle, this->data, this->len);
}




bool pgen_unknown::cast(const bit8* packet, int len){
	clear();
	if(!(14 < len && len < PGEN_MAX_PACKET_LEN)){
		fprintf(stderr, "recv packet length is not support (len=%d)\n", len);
		return false;
	}
	this->len = len;
	memcpy(this->data, packet, len);

	struct MYETH*  eth;
	struct MYIP*   ip;
	struct MYTCP*  tcp;
	struct MYUDP*  udp;
		
	const bit8* p = packet;
	isETH = true;
	eth = (struct MYETH*)p;
	p += sizeof(struct MYETH);
	this->ETH.src.setmacbyarry(eth->ether_shost);
	this->ETH.dst.setmacbyarry(eth->ether_dhost);

	if(ntohs(eth->ether_type) == 0x0800){
		this->isIP = true;
		ip = (struct MYIP*)p;
		p += sizeof(struct MYIP);
		this->IP.src._addr = ip->saddr;
		this->IP.dst._addr = ip->daddr;

		if(ip->protocol == 1){
			this->isICMP = true;
			p += sizeof(struct MYICMP);
		}
		else if(ip->protocol == 6){
			this->isTCP = true;
			tcp = (struct MYTCP*)p;
			p += sizeof(struct MYTCP);
			this->TCP.src = ntohs(tcp->source);
			this->TCP.dst = ntohs(tcp->dest);
		}
		else if(ip->protocol == 17){
			this->isUDP = true;
			udp = (struct MYUDP*)p;
			p += sizeof(struct MYUDP);
			this->UDP.src = ntohs(udp->source);
			this->UDP.dst = ntohs(udp->dest);
		}
		else{
			// other L4 protocol
			return false;
		}
	}
	
	else if(ntohs(eth->ether_type) == 0x0806){
		this->isARP = true;
	}
	else{
		// other L3 protocol
		return false;
	}
	return true;
}






void pgen_unknown::summary(){
	printf("unknown(packet=[");
	if(isTCP) printf("TCP|");
	if(isUDP) printf("UDP|");
	if(isICMP) printf("ICMP|");
	if(isIP) printf("IP|");
	if(isARP) printf("ARP|");
	if(isETH) printf("ETH]  ");


	if(isTCP)			
		printf("%s:%d > %s:%d", IP.src.c_str(), TCP.src, IP.dst.c_str(), TCP.dst);
	else if(isUDP)	
		printf("%s:%d > %s:%d", IP.src.c_str(), UDP.src, IP.dst.c_str(), UDP.dst);
	else if(isICMP)	
		printf("%s > %s ", IP.src.c_str(), IP.dst.c_str());
	else if(isIP)		
		printf("%s > %s", IP.src.c_str(), IP.dst.c_str()); 
	else if(isARP)	
		printf("%s > %s ", ETH.src.c_str(), ETH.dst.c_str()); 
	else if(isETH)	
		printf("%s > %s", ETH.src.c_str(), ETH.dst.c_str());
	else			
		printf("no support");
	printf(" len=%d\n", len);
}





bool pgen_unknown::ipaddris(ipaddr addr){
	if(!isIP) return false;
	return (addr==IP.src || addr==IP.dst);
}




bool pgen_unknown::macaddris(macaddr addr){
	if(!isETH) return false;
	return (addr==ETH.src || addr==ETH.dst);
}




bool pgen_unknown::portis(unsigned short port){
	if(!(isUDP || isTCP)) return false;
	if(isTCP){
		return (port==TCP.src || port==TCP.dst);	
	}else{
		return (port==UDP.src || port==UDP.dst);	
	}
}



void pgen_unknown::hex(){
	printf("hexdump len: %d \n", len);

	int row=0;
	int column=0;
	for(row=0; (row+column)<16+len; row+=16){
		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf("   ");
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
			}else{
				if((row+column)%16 == 0) printf("%04x:    ", row+column);
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
				printf("%02x ", data[row+column]);
			}
		}

		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf(" ");
			}else{
				if((row+column)%16 == 0) 
					printf("  ");
				if((row+column)%8 == 0 && (row+column)%16!=0) 
					printf("  ");
				
				if(0x20<=data[row+column] && data[row+column]<=0x7E)
					printf("%c", data[row+column]);
				else
					printf(".");
				
				if((row+column+1)%16 == 0)	
					printf("\n");
			}
		}
	}
	printf("\n");
}
