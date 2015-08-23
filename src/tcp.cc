/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "address.h"

#include "netutil.h"

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



pgen_tcp::pgen_tcp(const bit8* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_tcp::clear(){
	pgen_ip::clear();
	TCP.src = 20;
	TCP.dst = 80;
	TCP.flags.fin = 0;
	TCP.flags.syn = 0;
	TCP.flags.rst = 0;
	TCP.flags.psh = 0;
	TCP.flags.ack = 0;
	TCP.flags.urg = 0;
	TCP.window = 8192;
	TCP.seq = 0;
	TCP.ack = 0;
}


void pgen_tcp::send(const char* ifname){
	compile();		
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, data, len, (struct sockaddr*)&addr) < 0)
		exit(-1);
}



void pgen_tcp::compile(){
	pgen_ip::compile();
	memset(data, 0, sizeof data);
	ip.protocol = MT_IPPROTO_TCP;
	//ip.tot_len = htons(sizeof(ip) + sizeof(tcp));
	ip.tot_len = htons(sizeof(ip) + 20);
	u_char buf[1000];
	u_char *bp;
	memset(buf, 0, sizeof buf);

	memset(&tcp, 0, sizeof tcp);
	tcp.source = htons(TCP.src);
	tcp.dest   = htons(TCP.dst);
	tcp.seq    = htonl(TCP.seq);
	tcp.ack_seq = htonl(TCP.ack);
	tcp.doff = 20 >> 2;  // 4で割った値を入れる
	tcp.window = htons(TCP.window);	//OK
	tcp.check  = 0;
	if(TCP.flags.fin == 1)	tcp.fin = 1;
	if(TCP.flags.syn == 1)	tcp.syn = 1;
	if(TCP.flags.rst == 1)	tcp.rst = 1;
	if(TCP.flags.psh == 1)	tcp.psh = 1;
	if(TCP.flags.ack == 1)	tcp.ack = 1;
	if(TCP.flags.urg == 1)	tcp.urg = 1;
	bp = buf;
	memcpy(bp, &ip, sizeof(ip));
	bp += sizeof(ip);
	memcpy(bp, &tcp, sizeof(tcp));
	bp += sizeof(tcp);
	tcp.check = checksumTcp(buf, bp-buf);

	u_char* p = data;
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &tcp, sizeof(tcp));
	p += sizeof(tcp);
	len = p - data;
	
	_addData_WRAP();
}





void pgen_tcp::cast(const bit8* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "tcp packet length not support (%d)\n", len);
		return;
	}
	
	
	pgen_ip::cast(data, len);

	struct MYTCP buf;
	memcpy(&buf, data+sizeof(struct MYETH)+sizeof(struct MYIP),
			sizeof(buf));

	TCP.src = ntohs(buf.source);
	TCP.dst = ntohs(buf.dest);
	TCP.seq = ntohl(buf.seq);
	TCP.ack = ntohl(buf.ack_seq);
	TCP.window = ntohs(buf.window);
	TCP.flags.fin = buf.fin;
    TCP.flags.syn = buf.syn;
    TCP.flags.rst = buf.rst;
    TCP.flags.psh = buf.psh;
    TCP.flags.ack = buf.ack;
	TCP.flags.urg = buf.urg;
	
}



void pgen_tcp::SUMMARY(){
	int seq = tcp.seq;
	int win = tcp.window;
	int len = tcp.doff;
	int sport = ntohs(tcp.source);
	int dport = ntohs(tcp.dest);
	
	std::string flag;
	if(tcp.fin == 1)	flag+= "FIN";
	if(tcp.syn == 1)	flag+= "SYN";
	if(tcp.rst == 1)	flag+= "RST";
	if(tcp.psh == 1)	flag+= "PSH";
	if(tcp.ack == 1)	flag+= "ACK";
	if(tcp.urg == 1)	flag+= "URG";



	
	printf("%d > %d [%s] seq=%d win=%d len=%d\n",
			sport, dport, flag.c_str(), seq, win, len);
}

void pgen_tcp::info(){
	compile();
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %d (%s) \n",
			ntohs(tcp.source), pgen_port2service(ntohs(tcp.source), 1));
	printf("    - Dest Port       :  %d (%s) \n", 
			ntohs(tcp.dest), pgen_port2service(ntohs(tcp.dest), 1));
	printf("    - Frags           :  ");
	if(tcp.fin == 1)	printf("F");
	if(tcp.syn == 1)	printf("S");
	if(tcp.rst == 1)	printf("R");
	if(tcp.psh == 1)	printf("P");
	if(tcp.ack == 1)	printf("A");
	if(tcp.urg == 1)	printf("U");
	printf("\n");
	printf("    - Window size     :  %d \n", ntohs(tcp.window));
	printf("    - Checksum        :  0x%04x \n", ntohs(tcp.check));
	printf("    - sequence        :  %u \n", ntohl(tcp.seq));
	printf("    - acknowledge     :  %u \n", ntohl(tcp.ack_seq));

}


