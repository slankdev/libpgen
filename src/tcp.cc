/* include must!!! */
#include "packet.h"
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"
#include "pgen-variable.h"

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
	pgen_ip::clear();
	clear();
}

void pgen_tcp::clear(){
	tcp_srcPort = 0;
	tcp_dstPort = 0;
	tcp_frag.fin = 0;
	tcp_frag.syn = 0;
	tcp_frag.rst = 0;
	tcp_frag.psh = 0;
	tcp_frag.ack = 0;
	tcp_frag.urg = 0;
}

void pgen_tcp::sendPack(const char* ifname){
	wrap(ifname);		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_dstIp._addr;

	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(-1);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_tcp::wrap(const char* ifname){
	pgen_ip::wrap(ifname);
	packetType = PGEN_PACKETTYPE_TCP;
	memset(data, 0, sizeof data);
	ip.protocol = IPPROTO_TCP;
	//ip.tot_len = htons(sizeof(ip) + sizeof(tcp));
	ip.tot_len = htons(sizeof(ip) + tcp_doff);
	u_char buf[1000];
	u_char *bp;
	memset(buf, 0, sizeof buf);

	memset(&tcp, 0, sizeof tcp);
	tcp.source = htons(tcp_srcPort);
	tcp.dest   = htons(tcp_dstPort);
	tcp.seq    = htonl(0);
	tcp.ack_seq = htonl(0);
	tcp.doff = tcp_doff >> 2;  // ５で割った値を入れる 
	tcp.window = htons(tcp_window);	//OK
	tcp.check  = 0;
	if(tcp_frag.fin == 1)	tcp.fin = 1;
	if(tcp_frag.syn == 1)	tcp.syn = 1;
	if(tcp_frag.rst == 1)	tcp.rst = 1;
	if(tcp_frag.psh == 1)	tcp.psh = 1;
	if(tcp_frag.ack == 1)	tcp.ack = 1;
	if(tcp_frag.urg == 1)	tcp.urg = 1;
	bp = buf;
	memcpy(bp, &ip, sizeof(ip));
	bp += sizeof(ip);
	memcpy(bp, &tcp, sizeof(icmp));
	bp += sizeof(tcp);
	tcp.check = checksumTcp(buf, bp-buf);

	u_char* p = data;
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &tcp, sizeof(icmp));
	p += sizeof(tcp);
	len = p - data;
}



void pgen_tcp::info(){
	pgen_ip::info();

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %d \n", tcp_srcPort);
	printf("    - Dest Port       :  %d \n", tcp_dstPort);
	printf("    - Frags           :  ");
	if(tcp_frag.fin == 1)	printf("F");
	if(tcp_frag.syn == 1)	printf("S");
	if(tcp_frag.rst == 1)	printf("R");
	if(tcp_frag.psh == 1)	printf("P");
	if(tcp_frag.ack == 1)	printf("A");
	if(tcp_frag.urg == 1)	printf("U");
	printf("\n");
	printf("    - Checksum        :  0x%04x \n", ntohs(tcp.check));
}


void pgen_tcp::setData(const u_char *p, int len){}


