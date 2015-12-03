

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#include <pgen/pgen-types.h>

#include <pgen/module/debug.h>
#include <pgen/core/packet/packet.h>
#include <pgen/core/packet/unknown.h>
#include <pgen/core/address/address.h>
#include <pgen/io/pgen-io.h>

#include <stdio.h>

#define PACKET_MINLEN 14





pgen_unknown::pgen_unknown(){
	clear();
}




pgen_unknown::pgen_unknown(const void* packet, int len){
	clear();
	cast(packet, len);
}




void pgen_unknown::clear(){
	len = 0;
	__isETH = false;
	__isARP = false;
	__isIP  = false;
	__isICMP= false;
	__isTCP = false;
	__isUDP = false;
	ETH._addr.src = 0;
	ETH._addr.dst = 0;
	IP._addr.src = 0;
	IP._addr.dst = 0;
	TCP._port.src = 0;
	TCP._port.dst = 0;
	UDP._port.src = 0;
	UDP._port.dst = 0;
	memset(data, 0 , PGEN_MAX_PACKET_LEN);
}



void pgen_unknown::send_handle(pgen_t* handle){
	pgen_sendpacket_handle(handle, this->data, this->len);
}




int pgen_unknown::cast(const void* packet, int len){
	clear();
	if(!(14 <= len && len <= PGEN_MAX_PACKET_LEN)){
		fprintf(stderr, "pgen_unknown::cast: packet len isn't support (len=%d)\n", len);
		return -1;
	}
	this->len = len;
	memcpy(this->data, packet, len);

	struct ethernet_header*  eth;
	struct ip_header*   ip;
	struct tcp_header*  tcp;
	struct udp_header*  udp;
		
	const bit8* p = (bit8*)packet;
	__isETH = true;
	eth = (struct ethernet_header*)p;
	p += 14;
	this->ETH._addr.src.setmacbyarry(eth->ether_shost);
	this->ETH._addr.dst.setmacbyarry(eth->ether_dhost);

	if(ntohs(eth->ether_type) == 0x0800){
		this->__isIP = true;
		ip = (struct ip_header*)p;
		p += ip->ihl*4;
		this->IP._addr.src._addr = ip->saddr;
		this->IP._addr.dst._addr = ip->daddr;

		if(ip->protocol == 1){
			this->__isICMP = true;
			p += sizeof(struct icmp_header);
		}
		else if(ip->protocol == 6){
			this->__isTCP = true;
			tcp = (struct tcp_header*)p;
			p += sizeof(struct tcp_header);
			this->TCP._port.src = ntohs(tcp->source);
			this->TCP._port.dst = ntohs(tcp->dest);
		}
		else if(ip->protocol == 17){
			this->__isUDP = true;
			udp = (struct udp_header*)p;
			p += sizeof(struct udp_header);
			this->UDP._port.src = ntohs(udp->source);
			this->UDP._port.dst = ntohs(udp->dest);
		}
		else{
			// other L4 protocol
			return -1;
		}
	}
	
	else if(ntohs(eth->ether_type) == 0x0806){
		this->__isARP = true;
	}
	else{
		// other L3 protocol
		return -1;
	}
	return 1;
}






void pgen_unknown::summary(){
	printf("unknown(packet=[");
	if(isTCP()) printf("TCP|");
	if(isUDP()) printf("UDP|");
	if(isICMP()) printf("ICMP|");
	if(isIP()) printf("IP|");
	if(isARP()) printf("ARP|");
	if(isETH()) printf("ETH]  ");


	if(isTCP()){			
		printf("%s:%d > %s:%d",IP._addr.src.c_str(),
				TCP._port.src,IP._addr.dst.c_str(),TCP._port.dst);
	}else if(isUDP()){
		printf("%s:%d > %s:%d",IP._addr.src.c_str(),
				UDP._port.src,IP._addr.dst.c_str(),UDP._port.dst);
	}else if(isICMP()){	
		printf("%s > %s ", IP._addr.src.c_str(), IP._addr.dst.c_str());
	}else if(isIP()){	
		printf("%s > %s", IP._addr.src.c_str(), IP._addr.dst.c_str()); 
	}else if(isARP()){	
		printf("%s > %s ", ETH._addr.src.c_str(), ETH._addr.dst.c_str()); 
	}else if(isETH()){	
		printf("%s > %s", ETH._addr.src.c_str(), ETH._addr.dst.c_str());
	}else{
		printf("no support");
	}
	printf(" len=%d\n", len);
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


bool pgen_unknown::isETH(){return __isETH;}
bool pgen_unknown::isARP(){return __isARP;}
bool pgen_unknown::isIP(){return __isIP;}
bool pgen_unknown::isICMP(){return __isICMP;}
bool pgen_unknown::isTCP(){return __isTCP;}
bool pgen_unknown::isUDP(){return __isUDP;}


