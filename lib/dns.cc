
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


#include <vector>
#include <string>
void pgen_dns::WRAP(){
	pgen_udp::WRAP();
	udp.dest = htons(53);


	udp.len = htons(42);
	


	char *name;	
	int count = 0;
	name = (char*)malloc(DNS.name.length()+2);
	
	struct{
		u_int16_t type;
		u_int16_t cls;
	}_query;


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

	printf("\ncount: %d \n", count);
	for(int i=0; i<count; i++){
		printf("%02x ", name[i]);	
	}




	memset(&dns, 0, sizeof dns);
	dns.id = htons(DNS.id);
	dns.flags = htons(DNS.flags);
	dns.qdcnt = htons(DNS.qdcnt);
	dns.ancnt = htons(DNS.ancnt);
	dns.nscnt = htons(DNS.nscnt);
	dns.arcnt = htons(DNS.arcnt);


	_query.type = htons(DNS.type);
	_query.cls  = htons(DNS.cls);


	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);

	memcpy(p, &dns, sizeof dns);
	p += sizeof(struct MYDNSHDR);

	memcpy(p, name, count);
	p += count;

	memcpy(p, &_query, sizeof(_query));
	p += sizeof(_query);
	
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
