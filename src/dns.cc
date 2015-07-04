
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

pgen_dns::pgen_dns(){
	CLEAR();
}

void pgen_dns::CLEAR(){
	pgen_udp::CLEAR();

	DNS.id	   = 0x0000;
	
	DNS.flags.qr = 0;
	DNS.flags.opcode = 0;
	DNS.flags.aa = 0;
	DNS.flags.tc = 0;
	DNS.flags.rd = 1;
	DNS.flags.ra = 0;
	DNS.flags.nouse = 0;
	DNS.flags.rcode = 0;

	DNS.qdcnt = 0x0001;
	DNS.ancnt = 0x0000;
	DNS.nscnt = 0x0000;
	DNS.arcnt = 0x0000;
	DNS.name  = "example.com";
	DNS.type  = 0x0001;
	DNS.cls   = 0x0001;
}



void pgen_dns::CAST(const bit8* packet, int len){
	pgen_udp::CAST(packet, len);	
	
	struct MYDNS buf;
	int dnslen = len - sizeof(struct MYETH)-sizeof(struct MYIP)-
			sizeof(struct MYUDP);
	memcpy(&buf, packet + len - dnslen, sizeof(buf));

	char str[128];
	memset(str, 'A', sizeof(str));
	int namelen = dnslen - 12 - 4;
	int type;
	int cls;
	const bit8* queryPoint = packet + len - dnslen + 12;

	DNS.id	  = ntohs(buf.id);
	DNS.flags.qr = buf.qr;
	DNS.flags.opcode = buf.opcode;
	DNS.flags.aa = buf.aa;
	DNS.flags.tc = buf.tc;
	DNS.flags.rd = buf.rd;
	DNS.flags.ra = buf.ra;
	DNS.flags.nouse = buf.nouse;
	DNS.flags.rcode = buf.rcode;
	DNS.qdcnt = ntohs(buf.qdcnt);
	DNS.ancnt = ntohs(buf.ancnt);
	DNS.nscnt = ntohs(buf.nscnt);
	DNS.arcnt = ntohs(buf.arcnt);
	
	queryPoint++;
	for(int i=0; i<namelen; i++){
		if(  !(('A' <= *(queryPoint+i)&&*(queryPoint+i) <= 'Z') || 
				('a' <= *(queryPoint+i)&&*(queryPoint+i) <= 'z') ||
					('0'<=*(queryPoint+i)&&*(queryPoint+i) <= '9')  ))
			str[i] = '.';
		else
			str[i] = *(queryPoint+i);
	}str[namelen-2] = '\0';
	

	printf("len    : %d \n",len);
	printf("dnslen : %d \n", dnslen);
	printf("namelen: %d \n", namelen);

	DNS.name  = str;
	DNS.type  = (bit16)*(queryPoint+namelen);
	DNS.cls   = (bit16)*(queryPoint+namelen+2);
}




void pgen_dns::WRAP(){
	pgen_udp::WRAP();
	udp.dest = htons(53);

	char name[DNS.name.length()+2];
	int count = 0;
	struct{
		u_int16_t type;
		u_int16_t cls;
	}query;
	
	udp.len = htons(ntohs(udp.len)+sizeof(struct MYDNS)+
			sizeof(query)+sizeof(name));


	memset(&dns, 0, sizeof dns);
	dns.id = htons(DNS.id);
	
	if(DNS.flags.qr == 1)	dns.qr = 1;
	dns.opcode = DNS.flags.opcode;
	if(DNS.flags.aa == 1)	dns.aa = 1;
	if(DNS.flags.tc == 1)	dns.tc = 1;
	if(DNS.flags.rd == 1)	dns.rd = 1;
	if(DNS.flags.ra == 1)	dns.ra = 1;
	dns.nouse = DNS.flags.nouse;
	dns.rcode = DNS.flags.rcode;

	dns.qdcnt = htons(DNS.qdcnt);
	dns.ancnt = htons(DNS.ancnt);
	dns.nscnt = htons(DNS.nscnt);
	dns.arcnt = htons(DNS.arcnt);

	char *str;
	char buf[256];
	strcpy(buf, DNS.name.c_str());
	str = strtok(buf, ".");
	name[count] = (char)strlen(str);
	count++;
	for(int i=0; i<strlen(str); i++){
		name[count] = str[i];
		count++;
	}
	while((str=strtok(NULL, ".")) != NULL){
		name[count] = (char)strlen(str);
		count++;
		for(int i=0; i<strlen(str); i++){
			name[count] = str[i];
			count++;
		}
	}
	name[count] = 0x00;
	count++;

	query.type = htons(DNS.type);
	query.cls  = htons(DNS.cls);


	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	memcpy(p, &dns, sizeof dns);
	p += sizeof(struct MYDNSHDR);
	memcpy(p, name, count);
	p += count;
	memcpy(p, &query, sizeof(query));
	p += sizeof(query);
	
	len = p-data;
}


void pgen_dns::SEND(const char* ifname){
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

void pgen_dns::SUMMARY(){
	WRAP();
//	if(ntohs(dns.flags) == 0x0100){
		printf("Standard query 0x%04x %s\n", ntohs(dns.id),
				DNS.name.c_str());
//	}else{
//		printf("not supported\n");	
//	}
}

void pgen_dns::INFO(){
	WRAP();
	pgen_udp::INFO();

	printf(" * Domain Name System \n");
	printf("    - Identification  : 0x%04x\n", ntohs(dns.id));
	printf("    - Flags           : 0x%04x\n", ntohs(dns.flags));
	printf("         - qr         : %d\n", dns.qr);
	printf("         - opcode     : %d\n", dns.opcode);
	printf("         - aa         : %d\n", dns.aa);
	printf("         - tc         : %d\n", dns.tc);
	printf("         - rd         : %d\n", dns.rd);
	printf("         - ra         : %d\n", dns.ra);
	printf("         - nouse      : %d\n", dns.nouse);
	printf("         - rcode      : %d\n", dns.rcode);
	
	printf("    - Question        : 0x%04x\n", ntohs(dns.qdcnt));
	printf("    - Answer RRs      : 0x%04x\n", ntohs(dns.ancnt));
	printf("    - Authority RRs   : 0x%04x\n", ntohs(dns.nscnt));
	printf("    - Additional RRs  : 0x%04x\n", ntohs(dns.arcnt));

	printf("    - Queries \n");
	printf("         - name       : %s \n", DNS.name.c_str());
	printf("         - type       : 0x%04x \n", DNS.type);
	printf("         - class      : 0x%04x \n", DNS.cls);
}
