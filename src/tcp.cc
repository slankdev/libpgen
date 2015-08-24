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



pgen_tcp::pgen_tcp(const u_char* packet, int len){
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





void pgen_tcp::compile(){
	IP.tot_len = (sizeof(ip) + 20);
	IP.protocol = MT_IPPROTO_TCP;
	pgen_ip::compile();
	memset(data, 0, sizeof data);
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
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &tcp, sizeof(tcp));
	p += sizeof(tcp);
	len = p - data;
	
	compile_addData();
}





void pgen_tcp::cast(const u_char* data, int len){
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



void pgen_tcp::summary(){
	compile();
	int len = tcp.doff;
	std::string flag;
	if(tcp.fin == 1)	flag+= "FIN";
	if(tcp.syn == 1)	flag+= "SYN";
	if(tcp.rst == 1)	flag+= "RST";
	if(tcp.psh == 1)	flag+= "PSH";
	if(tcp.ack == 1)	flag+= "ACK";
	if(tcp.urg == 1)	flag+= "URG";
	
	printf("%d > %d [%s] seq=%d win=%d len=%d\n",
			TCP.src, TCP.dst, flag.c_str(), TCP.seq, TCP.window, len);
}

void pgen_tcp::info(){
	compile();
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %d (%s) \n",
			TCP.src, pgen_port2service(TCP.src, 1));
	printf("    - Dest Port       :  %d (%s) \n", 
			TCP.dst, pgen_port2service(TCP.dst, 1));
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


