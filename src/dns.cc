

#include "pgen.h"
#include "packet.h"
#include "address.h"
#include "netutil.h"

#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>


static char* get_dns_name(const char* iurl){
	int result_len = 64;
	char* result = (char*)malloc(result_len);
	char url[result_len];
	std::vector<std::string> vec;
	
	memcpy(url, iurl, sizeof(url));
	memset(result, 0, result_len);
	
	for(char *tp=strtok(url, "."); tp!=NULL; tp=strtok(NULL, ".")){
		if(tp != NULL)
			vec.push_back(tp);
	}

	for(int i=0; i<vec.size(); i++){
		char buf[100];
		snprintf(buf, sizeof(buf), ".%s", vec[i].c_str());
		buf[0] = (int)strlen(vec[i].c_str());
		buf[strlen(vec[i].c_str())+1] = '\0';
		
		strncat(result, buf, result_len-strlen(result));
	}
	return result;
}


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
	
	clear_query();
	clear_answer();
}	


void pgen_dns::clear_query(){
	DNS.query.name  = "example.com";
	DNS.query.type  = 0x0001;
	DNS.query.cls   = 0x0001;
}

void pgen_dns::clear_answer(){
	DNS.answer.name = 0xc00c;
	DNS.answer.type = 0x0001;
	DNS.answer.cls  = 0x0001;
	DNS.answer.ttl  = 0x00000b44;
	DNS.answer.len  = 0x0004;
	DNS.answer.addr = "127.0.0.1";
}



void pgen_dns::compile_query(){
	memset(query_data, 0, sizeof(query_data));
	query_data_len = 0;
	
	char* name;
	struct{
		bit16 type;
		bit16 cls;
	}q_data;

	name = get_dns_name(DNS.query.name.c_str());
	q_data.type = htons(DNS.query.type);
	q_data.cls  = htons(DNS.query.cls);
	
	bit8* p = query_data;
	memcpy(p, name, strlen(name)+1);
	p += strlen(name)+1;
	memcpy(p, &q_data, sizeof(q_data));
	p += sizeof(q_data);

	query_data_len = p - query_data;
}



void pgen_dns::compile_answer(){
	memset(answer_data, 0, sizeof(answer_data));
	answer_data_len = 0;
	
	if(DNS.flags.qr != 1)
		return;

	struct{
		bit16 name;
		bit16 type;
		bit16 cls;
	}a_data1;
	bit32 ttl;
	struct{
		bit16 len;
		bit8 addr[4];
			
	}a_data2;

	a_data1.name = htons(DNS.answer.name);
	a_data1.type = htons(DNS.answer.type);
	a_data1.cls  = htons(DNS.answer.cls);
	ttl  = htonl(DNS.answer.ttl);
	a_data2.len  = htons(DNS.answer.len);
	for(int i=0; i<4; i++)
		a_data2.addr[i] = DNS.answer.addr.getOctet(i);

	bit8* p = answer_data;
	memcpy(p, &a_data1, sizeof(a_data1));
	p += sizeof(a_data1);
	memcpy(p, &ttl, sizeof(ttl));
	p += sizeof(ttl);
	memcpy(p, &a_data2, sizeof(a_data2));
	p += sizeof(a_data2);
	
	answer_data_len = p - answer_data;
	

}




void pgen_dns::compile(){
	compile_query();
	compile_answer();

	UDP.dst = 53;
	UDP.len = UDP_HDR_LEN + DNS_HDR_LEN + query_data_len + answer_data_len;
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

	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct MYIP);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	memcpy(p, &dns, sizeof dns);
	p += sizeof(struct MYDNS);
	
	memcpy(p, &query_data, query_data_len);
	p += query_data_len;
	memcpy(p, &answer_data, answer_data_len);
	p += answer_data_len;
	
	len = p - data;
}




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
	
	memcpy(answer_data, &name, sizeof(name));
	memcpy(answer_data+2, &buf, sizeof(buf));
	answer_data_len = sizeof(buf)+2;
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
