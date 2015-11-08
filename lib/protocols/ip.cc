

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


//#include "packet.h"
//#include "address.h"
#include "pgen/pgen-error.h"
#include "pgen/pgen-io.h"
#include "pgen/protocols/ip.h"

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





pgen_ip::pgen_ip(){
	clear();	
}



pgen_ip::pgen_ip(const void* packet, int len){
	clear();	
	cast(packet, len);
}



void pgen_ip::clear(){
	pgen_eth::clear();
	ETH.type = 0x0800;

	this->IP.hlen = 5;
	this->IP.tos = 0;
	this->IP.tot_len = 20;
	this->IP.id = 1;
	this->IP.frag_off = 0;
	this->IP.ttl = 64;
	this->IP.protocol = IPPROTO_IP;
	this->IP.src = 0;
	this->IP.dst = "127.0.0.1";
	memset(&IP.option, 0, sizeof IP.option);
}



void pgen_ip::compile(){

	u_char buf[1000];
	int buflen;

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->data;
	
	buflen = pgen_eth::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_ip::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;

	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	this->len = p - this->data;
}




void pgen_ip::cast(const void* data, int l){
	const u_char* p = (u_char*)data;
	int buflen;

	buflen = pgen_eth::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_ip::read_bin(p, l);
	p += buflen;
	l -= buflen;

	add_data(p, l);
}






int  pgen_ip::write_bin(void* buf, int buflen){

	if(buflen < 20){
		fprintf(stderr, "pgen_ip::write_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct ip_header ip_head;
	memset(&ip_head, 0, sizeof(ip_head));

	ip_head.ihl = this->IP.hlen;
	ip_head.version = 4;
	ip_head.tos = this->IP.tos; //no useing world now
	ip_head.tot_len = htons(this->IP.tot_len);
	ip_head.frag_off = htons(this->IP.frag_off); // ?????
	ip_head.id = htons(this->IP.id);
	ip_head.ttl = this->IP.ttl;
	ip_head.protocol = this->IP.protocol;
	ip_head.saddr = this->IP.src._addr;
	ip_head.daddr = this->IP.dst._addr;
	ip_head.check = htons(this->IP.check);
	memcpy(ip_head.option, IP.option, IP.hlen*4-20);

	memcpy(buf, &ip_head, IP.hlen*4);
	return IP.hlen*4;
}




int  pgen_ip::read_bin(const void* buf, int buflen){
	if(buflen < 20){
		fprintf(stderr, "pgen_ip::read_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct ip_header* ip_head = (struct ip_header*)buf;
	
	this->IP.hlen = ip_head->ihl;
	this->IP.tos = ip_head->tos;
	this->IP.tot_len = ntohs(ip_head->tot_len);
	this->IP.id = ntohs(ip_head->id);
	this->IP.frag_off = ntohs(ip_head->frag_off);
	this->IP.ttl = ip_head->ttl;
	this->IP.protocol = ip_head->protocol;
	this->IP.check = ntohs(ip_head->check);
	this->IP.src._addr = ip_head->saddr;
	this->IP.dst._addr = ip_head->daddr;
	memcpy(IP.option, ip_head->option, ip_head->ihl*4-20);

	return IP.hlen*4;
}


unsigned short pgen_ip::calc_checksum(){
	this->IP.check = 0;
		
	struct ip_header ip_head;
	pgen_ip::write_bin(&ip_head, sizeof(ip_head));

	return ntohs(checksum((unsigned short*)&ip_head, IP.hlen*4));
}



void pgen_ip::send_L3(const char* ifname){
	compile();	
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = this->IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, this->data+ETH_HDR_LEN, this->len-ETH_HDR_LEN,
				(struct sockaddr*)&addr) < 0){
		pgen_perror("send_L3");
	}
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



void pgen_ip::help(){
	printf("IP Packet CLass------------------------------------------------\n");
	printf("hlen     : header length     :  8 bit field \n");
	printf("tos      : type of service   :  8 bit field \n");
	printf("tot_len  : total length      : 16 bit field \n");
	printf("id       : identification    : 16 bit field \n");
	printf("frag_off : fragment offset   : 16 bit field \n"); // fix soon
	printf("ttl      : time to leave     :  8 bit field \n");
	printf("protocol : L4 protocol       :  8 bit field \n");
	printf("src      : source ip address : 32 bit field \n");
	printf("dst      : dest ip address   : 32 bit field \n");
	printf("----------------------------------------------------------------\n");
}
