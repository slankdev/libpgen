
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
#include "pgen/pgen-io.h"
#include "pgen/protocols/tcp.h"

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
	this->IP.protocol = 6;
	this->IP.tot_len = 20+20;

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
	memset(&TCP.option, 0, sizeof TCP.option);
}



void pgen_tcp::compile(){

	u_char buf[100000];
	int buflen;

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);
	u_char* p = this->data;

	buflen = pgen_eth::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_ip::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	buflen = pgen_tcp::write_bin(buf, sizeof(buf));
    memcpy(p, buf, buflen);
	p += buflen;
	
	memcpy(p, _additional_data, _additional_len);
	p += _additional_len;

	len = p - this->data;
}


void pgen_tcp::cast(const void* data, int l){
	const u_char* p = (u_char*)data;
	int buflen;

	buflen = pgen_eth::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_ip::read_bin(p, l);
	p += buflen;
	l -= buflen;
	buflen = pgen_tcp::read_bin(p, l);
	p += buflen;
	l -= buflen;
	
	add_data(p, l);
}







int  pgen_tcp::write_bin(void* buf, int buflen){

	if(buflen < sizeof(struct tcp_header)){
		fprintf(stderr, "pgen_tcp::write_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}
	

	struct tcp_header tcp_head;
	memset(&tcp_head, 0, sizeof(tcp_head));
	tcp_head.source = htons(this->TCP.src);
	tcp_head.dest   = htons(this->TCP.dst);
	tcp_head.seq    = htonl(this->TCP.seq);
	tcp_head.ack_seq = htonl(this->TCP.ack);
	tcp_head.doff = this->TCP.doff;  // header length
	tcp_head.window = htons(this->TCP.window);
	tcp_head.check  = 0;
	tcp_head.fin = this->TCP.flags.fin;
	tcp_head.syn = this->TCP.flags.syn;
	tcp_head.rst = this->TCP.flags.rst;
	tcp_head.psh = this->TCP.flags.psh;
	tcp_head.ack = this->TCP.flags.ack;
	tcp_head.urg = this->TCP.flags.urg;
	memcpy(tcp_head.option, TCP.option, TCP.doff*4-20);
	tcp_head.check = htons(this->TCP.check);

	memcpy(buf, &tcp_head, TCP.doff*4);
	return TCP.doff*4;	
}


int  pgen_tcp::read_bin(const void* buf, int buflen){
	if(buflen < 20){
		fprintf(stderr, "pgen_tcp::read_bin: binary length is not support (%d)\n", buflen);
		return -1;
	}

	struct tcp_header* tcp_head = (struct tcp_header*)buf;

	this->TCP.src = ntohs(tcp_head->source);
	this->TCP.dst = ntohs(tcp_head->dest);
	this->TCP.seq = ntohl(tcp_head->seq);
	this->TCP.ack = ntohl(tcp_head->ack_seq);
	this->TCP.doff = tcp_head->doff;
	this->TCP.flags.fin = tcp_head->fin;
    this->TCP.flags.syn = tcp_head->syn;
    this->TCP.flags.rst = tcp_head->rst;
    this->TCP.flags.psh = tcp_head->psh;
    this->TCP.flags.ack = tcp_head->ack;
	this->TCP.flags.urg = tcp_head->urg;
	this->TCP.window = ntohs(tcp_head->window);
	this->TCP.check  = ntohs(tcp_head->check);
	memcpy(TCP.option, tcp_head->option, TCP.doff*4-20);
		
	return TCP.doff*4;	
}




unsigned short pgen_tcp::calc_checksum(){
	this->TCP.check = 0;
		
	struct ip_header ip_head;
	pgen_ip::write_bin(&ip_head, sizeof(ip_head));
	struct tcp_header tcp_head;
	pgen_tcp::write_bin(&tcp_head, sizeof(tcp_head));
	
	return ntohs(checksumTcp(ip_head, tcp_head, _additional_data,IP.tot_len-IP.hlen*4));
}




void pgen_tcp::summary(){
	compile();
	printf("TCP{ ");
	int len = TCP.doff;
	std::string flag;
	if(TCP.flags.fin == 1)	flag+= "FIN";
	if(TCP.flags.syn == 1)	flag+= "SYN";
	if(TCP.flags.rst == 1)	flag+= "RST";
	if(TCP.flags.psh == 1)	flag+= "PSH";
	if(TCP.flags.ack == 1)	flag+= "ACK";
	if(TCP.flags.urg == 1)	flag+= "URG";
	printf("%u > %u [%s] seq=%u win=%u len=%u }\n",
			TCP.src, TCP.dst, flag.c_str(), TCP.seq, TCP.window, len);
}





void pgen_tcp::info(){
	compile();
	pgen_ip::info();
	char buf[32];

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %u (%s) \n",
			TCP.src, pgen_port2service(TCP.src, "tcp", buf));
	printf("    - Dest Port       :  %u (%s) \n", 
			TCP.dst, pgen_port2service(TCP.dst, "tcp", buf));
	printf("    - Frags           :  ");
	if(TCP.flags.fin == 1)	printf("F");
	if(TCP.flags.syn == 1)	printf("S");
	if(TCP.flags.rst == 1)	printf("R");
	if(TCP.flags.psh == 1)	printf("P");
	if(TCP.flags.ack == 1)	printf("A");
	if(TCP.flags.urg == 1)	printf("U");
	printf("\n");
	printf("    - Window size     :  %u \n", TCP.window);
	printf("    - Checksum        :  0x%04x \n", TCP.check);
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
