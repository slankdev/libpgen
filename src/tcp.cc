/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "packconf.h"
#include "pgen-funcs.h"
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
	CLEAR();
}

void pgen_tcp::CLEAR(){
	pgen_ip::CLEAR();
	TCP.srcPort = 20;
	TCP.dstPort = 80;
	TCP.frag.fin = 0;
	TCP.frag.syn = 0;
	TCP.frag.rst = 0;
	TCP.frag.psh = 0;
	TCP.frag.ack = 0;
	TCP.frag.urg = 0;
	TCP.window = 8192;
	TCP.seqNum = 0;
	TCP.ackNum = 0;
}


void pgen_tcp::SEND(const char* ifname){
	WRAP();		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;

	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(-1);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}

void pgen_tcp::WRAP(){
	pgen_ip::WRAP();
	packetType = PGEN_PACKETTYPE_TCP;
	memset(data, 0, sizeof data);
	ip.protocol = MT_IPPROTO_TCP;
	//ip.tot_len = htons(sizeof(ip) + sizeof(tcp));
	ip.tot_len = htons(sizeof(ip) + 20);
	u_char buf[1000];
	u_char *bp;
	memset(buf, 0, sizeof buf);

	memset(&tcp, 0, sizeof tcp);
	tcp.source = htons(TCP.srcPort);
	tcp.dest   = htons(TCP.dstPort);
	tcp.seq    = htonl(TCP.seqNum);
	tcp.ack_seq = htonl(TCP.ackNum);
	tcp.doff = 20 >> 2;  // 4で割った値を入れる
	tcp.window = htons(TCP.window);	//OK
	tcp.check  = 0;
	if(TCP.frag.fin == 1)	tcp.fin = 1;
	if(TCP.frag.syn == 1)	tcp.syn = 1;
	if(TCP.frag.rst == 1)	tcp.rst = 1;
	if(TCP.frag.psh == 1)	tcp.psh = 1;
	if(TCP.frag.ack == 1)	tcp.ack = 1;
	if(TCP.frag.urg == 1)	tcp.urg = 1;
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

void pgen_tcp::SUMMARY(){
	int seq = tcp.seq;
	int win = tcp.window;
	int len = tcp.doff;
	int sport = tcp.source;
	int dport = tcp.dest;
	
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

void pgen_tcp::INFO(){
	WRAP();
	pgen_ip::INFO();

	printf(" * Transmission Control Protocol \n");
	printf("    - Source Port     :  %d (%s) \n",
			ntohs(tcp.source), port2service(ntohs(tcp.source), 1));
	printf("    - Dest Port       :  %d (%s) \n", 
			ntohs(tcp.dest), port2service(ntohs(tcp.dest), 1));
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


void pgen_tcp::setData(const u_char *p, int len){}
