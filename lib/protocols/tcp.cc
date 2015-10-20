
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


#include "packet.h"
#include "address.h"
#include "pgen-io.h"
#include "tcp.h"

#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>		// for struct tcp		



pgen_tcp::pgen_tcp(){
	clear();
}



pgen_tcp::pgen_tcp(const void* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_tcp::clear(){
	pgen_ip::clear();
	this->TCP.src = 20;
	this->TCP.dst = 80;
	this->TCP.seq = 0;
	this->TCP.ack = 0;
	this->TCP.doff = 5;
	this->TCP.flags.fin = 0;
	this->TCP.flags.syn = 0;
	this->TCP.flags.rst = 0;
	this->TCP.flags.psh = 0;
	this->TCP.flags.ack = 0;
	this->TCP.flags.urg = 0;
	this->TCP.window = 8192;
}





void pgen_tcp::compile(){
	this->IP.tot_len = IP.hlen*4 + 20 + _additional_len;
	this->IP.protocol = 6;
	pgen_ip::compile();

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char buf[1000];
	u_char *bp;
	memset(buf, 0, sizeof buf);

	memset(&this->tcp, 0, sizeof this->tcp);
	this->tcp.source = htons(this->TCP.src);
	this->tcp.dest   = htons(this->TCP.dst);
	this->tcp.seq    = htonl(this->TCP.seq);
	this->tcp.ack_seq = htonl(this->TCP.ack);
	this->tcp.doff = this->TCP.doff;  // header length
	this->tcp.window = htons(this->TCP.window);
	this->tcp.check  = 0;

	this->tcp.fin = this->TCP.flags.fin;
	this->tcp.syn = this->TCP.flags.syn;
	this->tcp.rst = this->TCP.flags.rst;
	this->tcp.psh = this->TCP.flags.psh;
	this->tcp.ack = this->TCP.flags.ack;
	this->tcp.urg = this->TCP.flags.urg;


	bp = buf;
	memcpy(bp, &this->ip, 20);
	bp += 20;
	memcpy(bp, &this->tcp, tcp.doff*4);
	bp += tcp.doff*4;
	this->tcp.check = checksumTcp(buf, bp-buf);

	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP.hlen*4);
	p += IP.hlen*4;
	memcpy(p, &this->tcp, tcp.doff*4);
	p += tcp.doff*4;

	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	len = p - this->data;
	
}





void pgen_tcp::cast(const void* data, int l){
	if(!(this->minLen<=l && l<=this->maxLen)){
		fprintf(stderr, "pgen_tcp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_ip::cast(data, l);

	const u_char* p = (u_char*)data;
	p += ETH_HDR_LEN;
	l -= ETH_HDR_LEN;
	p += IP.hlen*4;
	l -= IP.hlen*4;

	struct tcp_header* buf = (struct tcp_header*)p;
	p += tcp.doff*4;
	l -= tcp.doff*4;

	this->TCP.src = ntohs(buf->source);
	this->TCP.dst = ntohs(buf->dest);
	this->TCP.seq = ntohl(buf->seq);
	this->TCP.ack = ntohl(buf->ack_seq);
	this->TCP.doff = buf->doff;
	this->TCP.window = ntohs(buf->window);
	this->TCP.flags.fin = buf->fin;
    this->TCP.flags.syn = buf->syn;
    this->TCP.flags.rst = buf->rst;
    this->TCP.flags.psh = buf->psh;
    this->TCP.flags.ack = buf->ack;
	this->TCP.flags.urg = buf->urg;

	add_data(p, l);
}



void pgen_tcp::summary(){
	compile();
	printf("TCP{ ");
	int len = tcp.doff;
	std::string flag;
	if(TCP.flags.fin == 1)	flag+= "FIN";
	if(TCP.flags.syn == 1)	flag+= "SYN";
	if(TCP.flags.rst == 1)	flag+= "RST";
	if(TCP.flags.psh == 1)	flag+= "PSH";
	if(TCP.flags.ack == 1)	flag+= "ACK";
	if(TCP.flags.urg == 1)	flag+= "URG";
	printf("%d > %d [%s] seq=%d win=%d len=%d }\n",
			TCP.src, TCP.dst, flag.c_str(), TCP.seq, TCP.window, len);
}





void pgen_tcp::info(){
	compile();
	pgen_ip::info();
	char buf[32];

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %d (%s) \n",
			TCP.src, pgen_port2service(TCP.src, "tcp", buf));
	printf("    - Dest Port       :  %d (%s) \n", 
			TCP.dst, pgen_port2service(TCP.dst, "tcp", buf));
	printf("    - Frags           :  ");
	if(tcp.fin == 1)	printf("F");
	if(tcp.syn == 1)	printf("S");
	if(tcp.rst == 1)	printf("R");
	if(tcp.psh == 1)	printf("P");
	if(tcp.ack == 1)	printf("A");
	if(tcp.urg == 1)	printf("U");
	printf("\n");
	printf("    - Window size     :  %d \n", TCP.window);
	printf("    - Checksum        :  0x%04x \n", ntohs(tcp.check));
	printf("    - sequence        :  %u \n", TCP.seq);
	printf("    - acknowledge     :  %u \n", TCP.ack);

}




void pgen_tcp::help(){
	printf("TCP Packet CLass---------------------------------------- \n");
	printf("src       : source port        : 16 bit field \n");
	printf("dst       : dest port          : 16 bit field \n");
	printf("seq       : sequence number    : 32 bit field \n");
	printf("ack       : acknowledge number : 32 bit field \n");
	printf("flags.fin : fin flag           :  1 bit field \n");
	printf("flags.syn : syn flag           :  1 bit field \n");
	printf("flags.rst : rst flag           :  1 bit field \n");
	printf("flags.psh : psh flag           :  1 bit field \n");
	printf("flags.ack : ack flag           :  1 bit field \n");
	printf("flags.urg : urg flag           :  1 bit field \n");
	printf("window    : window size        : 16 bit field \n");
	printf("-------------------------------------------------------- \n");
}
