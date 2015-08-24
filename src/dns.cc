

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



pgen_dns::pgen_dns(){
	clear();
}



pgen_dns::pgen_dns(const u_char* packet, int len){
	clear();
	cast(packet, len);
}




void pgen_dns::clear(){
	pgen_udp::clear();

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
	DNS.query.name  = "example.com";
	DNS.query.type  = 0x0001;
	DNS.query.cls   = 0x0001;

	DNS.answer.name = 0;
	DNS.answer.type = 0;
	DNS.answer.cls  = 0;
	DNS.answer.ttl  = 0;
	DNS.answer.len  = 0;
	DNS.answer.addr = 0;
}






void pgen_dns::send(const char* ifname){
	compile();		
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;
	
	if(pgen_sendpacket_L3(ifname, data, len, (struct sockaddr*)&addr) < 0)
		exit(-1);
}



void pgen_dns::cast(const u_char* packet, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "dns packet length not support (%d)\n", len);
		return;
	}
	
	
	pgen_udp::cast(packet, len);	
	

	const bit8* dnsPoint = packet + sizeof(struct MYETH)
			+ sizeof(struct MYIP) + sizeof(struct MYUDP);
	struct MYDNS buf;
	memcpy(&buf, dnsPoint, sizeof(struct MYDNS));


	// query infomation
	bit8* queryPoint;
	char url[256];
	struct query_typecls{
		bit16 type;
		bit16 cls;
	};
	struct query_typecls* tc;
	bit32 queryLen;

	// answer infomation
	bit8* answerPoint;
	struct answer{
		bit16 type:16;
		bit16 cls:16;
		bit32 ttl:32;
		bit16 len:16;
		bit8  addr[4];
	};
	struct answer ans;
	bit32 answerLen;

	queryPoint = (bit8*)(dnsPoint+sizeof(struct MYDNS));

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


	for(queryLen=0; *(queryPoint+queryLen)!=0; queryLen++){
		if(queryLen==0)	continue;
		if(queryLen>= 50){
			printf("stack abunai!!!\n")	;
			printf("queryLen is too large(%d)\n", queryLen);
			return;

		}


		if(('a' <=  *(queryPoint+queryLen) && *(queryPoint+queryLen) <= 'z') ||
				('A' <=  *(queryPoint+queryLen) && *(queryPoint+queryLen) <= 'Z') || 
				('0' <=  *(queryPoint+queryLen) && *(queryPoint+queryLen) <= '9')){
			url[queryLen-1] = *(queryPoint+queryLen);
		}else{
			url[queryLen-1] = '.';
		}
	}queryLen += 4 + 1;




	tc = (struct query_typecls*)(queryPoint + queryLen - 4);
	

	DNS.query.name = url;
	DNS.query.type = ntohs(tc->type);
	DNS.query.cls  = ntohs(tc->cls);



	answerPoint = queryPoint + queryLen;
	memcpy(&ans, answerPoint+2, 16);

	char ansaddrstr[16];
	snprintf(ansaddrstr, sizeof(ansaddrstr), "%d.%d.%d.%d", 
			ans.addr[0], ans.addr[1], ans.addr[2], ans.addr[3]);

	//DNS.answer.name = htons(answerPoint);
	DNS.answer.name = 0;
	DNS.answer.type = htons(ans.type);
	DNS.answer.cls  = htons(ans.cls );
	DNS.answer.ttl  = htonl(ans.ttl );
	DNS.answer.len  = htons(ans.len );
	DNS.answer.addr = ansaddrstr;
}



void pgen_dns::compile(){
	
	char name[DNS.query.name.length()+2];
	int count = 0;
	struct{
		u_int16_t type;
		u_int16_t cls;
	}query;
	
	if(DNS.flags.qr == 1)
		_wrap_answer();

	UDP.dest = (53);
	UDP.len = UDP.len + sizeof(struct MYDNS) + sizeof(query) + sizeof(name) + answer_len;
	pgen_udp::compile();


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
	strncpy(buf, DNS.query.name.c_str(), sizeof(buf)-1);
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

	query.type = htons(DNS.query.type);
	query.cls  = htons(DNS.query.cls);


	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct MYIP);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	memcpy(p, &dns, sizeof dns);
	p += sizeof(struct MYDNS);
	memcpy(p, name, count);
	p += count;
	memcpy(p, &query, sizeof(query));
	p += sizeof(query);

	if(dns.qr == 1){
		memcpy(p, &answer, sizeof(answer));
		p += answer_len;
	}
	len = p-data;
	
	compile_addData();
}

void pgen_dns::_wrap_query(){}


void pgen_dns::_wrap_answer(){
	

	bit16 name;
	struct{
		//bit16 name;
		bit16 type;
		bit16 cls ;
		bit32 ttl ;
		bit16 len ;
		bit8  addr[4];
	}buf;
	
	name = htons(0xc00c);
	buf.type = htons(DNS.answer.type);
	buf.cls  = htons(DNS.answer.cls );
	buf.ttl  = htons(DNS.answer.ttl );
	buf.len  = htons(DNS.answer.len );
	      
	buf.addr[0] = DNS.answer.addr.getOctet(0);
	buf.addr[1] = DNS.answer.addr.getOctet(1);
	buf.addr[2] = DNS.answer.addr.getOctet(2);
	buf.addr[3] = DNS.answer.addr.getOctet(3);
	
	memcpy(answer, &name, sizeof(name));
	memcpy(answer+2, &buf, sizeof(buf));
	answer_len = sizeof(buf)+2;
	
}





// not coding now
void pgen_dns::summary(){
	compile();
	if(dns.qr == 1){
  		printf("Query response 0x%04x %s %s\n", ntohs(dns.id), DNS.query.name.c_str(),
				DNS.answer.addr.c_str());
	}else{
		printf("Query 0x%04x %s\n", ntohs(dns.id), DNS.query.name.c_str());	
	}
}





void pgen_dns::DSUMMARY(){
	compile();

	printf("%s -> %s ", IP.src.c_str(), IP.dst.c_str());

	if(dns.qr == 1){
  		printf("(Query response 0x%04x %s %s)\n", ntohs(dns.id), DNS.query.name.c_str(),
				DNS.answer.addr.c_str());
	}else{
		printf("(Query 0x%04x %s)\n", ntohs(dns.id), DNS.query.name.c_str());	
	}
}




void pgen_dns::info(){
	compile();
	pgen_udp::info();

	printf(" * Domain Name System \n");
	printf("    - Identification  : 0x%04x\n", ntohs(dns.id));
	printf("    - Flags           : 0x%04x\n", ntohs(dns.flags));
	printf("         - qr         : %d", dns.qr);
	if(dns.qr == 0)				printf("   (query) \n");
	else						printf("   (response)\n");
	printf("         - opcode     : %d", dns.opcode);
	if(dns.opcode == 0)			printf("   (standard query) \n");
	else if(dns.opcode == 1)	printf("   (inverse query) \n");
	else if(dns.opcode == 2)	printf("   (server status request) \n");
	else						printf("   (malformed)\n");
	printf("         - aa         : %d", dns.aa);
	if(dns.aa == 1)				printf("   (have authority) \n");
	else						printf("   (no authority)\n");
	printf("         - tc         : %d", dns.tc);
	if(dns.tc == 1)				printf("   (caption) \n");
	else						printf("   (no caption)\n");
	printf("         - rd         : %d", dns.rd);
	if(dns.rd == 1)				printf("   (recursion desired) \n");
	else						printf("   (no recursion)\n");
	printf("         - ra         : %d", dns.ra);
	if(dns.ra == 1)				printf("   (recursion available) \n");
	else						printf("   (recursion unavailable)\n");

	printf("         - nouse      : %d\n", dns.nouse);
	printf("         - rcode      : %d\n", dns.rcode);
	
	printf("    - Question        : 0x%04x\n", ntohs(dns.qdcnt));
	printf("    - Answer RRs      : 0x%04x\n", ntohs(dns.ancnt));
	printf("    - Authority RRs   : 0x%04x\n", ntohs(dns.nscnt));
	printf("    - Additional RRs  : 0x%04x\n", ntohs(dns.arcnt));

	printf("    - Queries \n");
	printf("         - name       : %s \n", DNS.query.name.c_str());
	printf("         - type       : 0x%04x \n", DNS.query.type);
	printf("         - class      : 0x%04x \n", DNS.query.cls);


	if(dns.qr == 1){
		printf("    - Answer  \n");
		printf("         - name       : 0x%04x (kaihatutyu)\n", DNS.answer.name);
		printf("         - type       : 0x%04x \n", DNS.answer.type);
		printf("         - class      : 0x%04x \n", DNS.answer.cls);
		printf("         - ttl        : 0x%08x \n", DNS.answer.ttl);
		printf("         - data len   : 0x%04x \n", DNS.answer.len);
		printf("         - address    : %s \n", DNS.answer.addr.c_str());
	}
}
