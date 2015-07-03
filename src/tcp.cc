/* include must!!! */
#include "pgen.h"
#include "packet.h"
#include "packconf.h"
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
	TCP.src = 20;
	TCP.dst = 80;
	TCP.flag.fin = 0;
	TCP.flag.syn = 0;
	TCP.flag.rst = 0;
	TCP.flag.psh = 0;
	TCP.flag.ack = 0;
	TCP.flag.urg = 0;
	TCP.window = 8192;
	TCP.seq = 0;
	TCP.ack = 0;
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
	tcp.source = htons(TCP.src);
	tcp.dest   = htons(TCP.dst);
	tcp.seq    = htonl(TCP.seq);
	tcp.ack_seq = htonl(TCP.ack);
	tcp.doff = 20 >> 2;  // 4で割った値を入れる
	tcp.window = htons(TCP.window);	//OK
	tcp.check  = 0;
	if(TCP.flag.fin == 1)	tcp.fin = 1;
	if(TCP.flag.syn == 1)	tcp.syn = 1;
	if(TCP.flag.rst == 1)	tcp.rst = 1;
	if(TCP.flag.psh == 1)	tcp.psh = 1;
	if(TCP.flag.ack == 1)	tcp.ack = 1;
	if(TCP.flag.urg == 1)	tcp.urg = 1;
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





void pgen_tcp::CAST(bit8* data){
	struct MYTCP buf;
	memcpy(&buf, data, sizeof(buf));

	TCP.src = ntohs(buf.source);
	TCP.dst = ntohs(buf.dest);
	TCP.seq = ntohl(buf.seq);
	TCP.ack = ntohl(buf.ack_seq);
	TCP.window = ntohs(buf.window);
	TCP.flag.fin = buf.fin;
    TCP.flag.syn = buf.syn;
    TCP.flag.rst = buf.rst;
    TCP.flag.psh = buf.psh;
    TCP.flag.ack = buf.ack;
	TCP.flag.urg = buf.urg;
	
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
