
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

pgen_dns::pgen_dns(){
	CLEAR();
}

void pgen_dns::CLEAR(){
	pgen_udp::CLEAR();
}

void pgen_dns::WRAP(){
	pgen_udp::WRAP();
	udp.dest = htons(53);
	udp.len = htons(38);
	char *name;
	u_int16_t type;
	u_int16_t cls;

	memset(&dns, 0, sizeof dns);
	//setting dns Structure
	dns.id = htons(DNS.id);
	dns.flags = htons(DNS.flags);
	dns.qdcnt = htons(DNS.qdcnt);
	dns.ancnt = htons(DNS.ancnt);
	dns.nscnt = htons(DNS.nscnt);
	dns.arcnt = htons(DNS.arcnt);

	name = (char*)malloc(DNS.name.length());
	//strcpy(name, DNS.name.c_str());
	sprintf(name,  "%c%s", 0x08, DNS.name.c_str());
	sprintf(name,  "%cslankdev%cnet", 0x08, 0x03);
	type = htons(DNS.type);
	cls  = htons(DNS.cls);


	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);

	memcpy(p, &dns, sizeof dns);
	p += sizeof(struct MYDNS);
	memcpy(p, name, strlen(name)+1);
	p += strlen(name) + 1;
	memcpy(p, &(type), sizeof(u_int16_t));
	p += sizeof(u_int16_t);
	memcpy(p, &(cls), sizeof(u_int16_t));
	p += sizeof(u_int16_t);
	
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


void pgen_dns::INFO(){
	WRAP();
}
