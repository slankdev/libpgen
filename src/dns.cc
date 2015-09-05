

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



void debug(const char* p){
	for(int i=0; ; i++){
		if(p[i] == 0) break;

		if(65 <= p[i] && p[i] <= 122){
			printf("%c", p[i]);	
		}else{
			printf("(%d)", p[i]);
		}
	}
	printf("%%\n");
}



static bool is_dns_name_charcter(char );
static char* get_dns_name(const char* );
static void dns_print_record_data(const bit8* data, int len, int type);





pgen_dns::pgen_dns(){
	clear();
}



pgen_dns::pgen_dns(const void* packet, int len){
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
	clear_auth();
	clear_addition();
}	


void pgen_dns::clear_query(){
	for(int i=0; i<MAX_QUERY; i++){
		DNS.query[i].name  = "example.com";
		DNS.query[i].type  = 0x0001;
		DNS.query[i].cls   = 0x0001;
	}
}



void pgen_dns::clear_answer(){
	for(int i=0; i<MAX_ANSWER; i++){
		DNS.answer[i].name = 0xc00c;
		DNS.answer[i].type = 0x0001;
		DNS.answer[i].cls  = 0x0001;
		DNS.answer[i].ttl  = 0x00000b44;
		DNS.answer[i].len  = 0x0004;
		//DNS.answer[i].addr = "127.0.0.1";
		memset(DNS.answer[i].data, 0, sizeof(DNS.answer[i].data));
	}
}



void pgen_dns::clear_auth(){
	for(int i=0; i<MAX_AUTH; i++){
		DNS.auth[i].name = 0xc00c;
		DNS.auth[i].type = 0x0001;
		DNS.auth[i].cls  = 0x0001;
		DNS.auth[i].ttl  = 0x00000b44;
		DNS.auth[i].len  = 0x0004;
		memset(DNS.auth[i].data, 0, sizeof(DNS.auth[i].data));
	}
}



void pgen_dns::clear_addition(){
	for(int i=0; i<MAX_ADD; i++){
		DNS.addition[i].name = 0xc00c;
		DNS.addition[i].type = 0x0001;
		DNS.addition[i].cls  = 0x0001;
		DNS.addition[i].ttl  = 0x00000b44;
		DNS.addition[i].len  = 0x0004;
		memset(DNS.addition[i].data, 0, sizeof(DNS.addition[i].data));
	}
}



void pgen_dns::compile_query(){
	memset(query_data, 0, sizeof(query_data));
	query_data_len = 0;
	
	char* name;
	struct{
		bit16 type;
		bit16 cls;
	}q_data;

	bit8* p = query_data;
	for(int j=0; j<DNS.qdcnt; j++){
		name = get_dns_name(DNS.query[j].name.c_str());
		q_data.type = htons(DNS.query[j].type);
		q_data.cls  = htons(DNS.query[j].cls);
		
		memcpy(p, name, strlen(name)+1);
		p += strlen(name)+1;
		memcpy(p, &q_data, sizeof(q_data));
		p += sizeof(q_data);
	}

	query_data_len = p - query_data;
}


/*//[[[
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


	bit8* p = answer_data;
	for(int j=0; j<DNS.ancnt; j++){

		a_data1.name = htons(DNS.answer[j].name);
		a_data1.type = htons(DNS.answer[j].type);
		a_data1.cls  = htons(DNS.answer[j].cls);
		ttl  = htonl(DNS.answer[j].ttl);
		a_data2.len  = htons(DNS.answer[j].len);
		for(int i=0; i<4; i++)
			a_data2.addr[i] = DNS.answer[j].addr.getOctet(i);

		memcpy(p, &a_data1, sizeof(a_data1));
		p += sizeof(a_data1);
		memcpy(p, &ttl, sizeof(ttl));
		p += sizeof(ttl);
		memcpy(p, &a_data2, sizeof(a_data2));
		p += sizeof(a_data2);
	}
	answer_data_len = p - answer_data;
}
*///]]]


void pgen_dns::compile_answer(){
	memset(answer_data, 0, sizeof(answer_data));
	answer_data_len = 0;
	
	if(DNS.flags.qr != 1)
		return;
	
	bit16 name;
	bit16 type;
	bit16 cls;
	bit32 ttl;
	bit16 len;
	bit8* data;


	bit8* p = answer_data;
	for(int j=0; j<DNS.ancnt; j++){

		name = htons(DNS.answer[j].name);
		type = htons(DNS.answer[j].type);
		cls  = htons(DNS.answer[j].cls);
		ttl  = htonl(DNS.answer[j].ttl);
		len  = htons(DNS.answer[j].len);
		data = DNS.answer[j].data;

		memcpy(p, &name, sizeof(name));
		p += sizeof(name);
		memcpy(p, &type, sizeof(type));
		p += sizeof(type);
		memcpy(p, &cls, sizeof(cls));
		p += sizeof(cls);
		memcpy(p, &ttl, sizeof(ttl));
		p += sizeof(ttl);
		memcpy(p, &len, sizeof(len));
		p += sizeof(len);
		memcpy(p, data, DNS.answer[j].len);
		p += DNS.answer[j].len;
	}
	answer_data_len = p - answer_data;

}


void pgen_dns::compile_auth(){
	memset(auth_data, 0, sizeof(auth_data));
	auth_data_len = 0;
	
	if(DNS.flags.qr != 1)
		return;
	
	bit16 name;
	bit16 type;
	bit16 cls;
	bit32 ttl;
	bit16 len;
	bit8* data;


	bit8* p = auth_data;
	for(int j=0; j<DNS.nscnt; j++){

		name = htons(DNS.auth[j].name);
		type = htons(DNS.auth[j].type);
		cls  = htons(DNS.auth[j].cls);
		ttl  = htonl(DNS.auth[j].ttl);
		len  = htons(DNS.auth[j].len);
		data = DNS.auth[j].data;

		memcpy(p, &name, sizeof(name));
		p += sizeof(name);
		memcpy(p, &type, sizeof(type));
		p += sizeof(type);
		memcpy(p, &cls, sizeof(cls));
		p += sizeof(cls);
		memcpy(p, &ttl, sizeof(ttl));
		p += sizeof(ttl);
		memcpy(p, &len, sizeof(len));
		p += sizeof(len);
		memcpy(p, data, DNS.auth[j].len);
		p += DNS.auth[j].len;
	}
	auth_data_len = p - auth_data;

}


void pgen_dns::compile_addition(){
	memset(addition_data, 0, sizeof(addition_data));
	addition_data_len = 0;
	
	if(DNS.flags.qr != 1)
		return;
	
	bit16 name;
	bit16 type;
	bit16 cls;
	bit32 ttl;
	bit16 len;
	bit8* data;


	bit8* p = addition_data;
	for(int j=0; j<DNS.arcnt; j++){
		name = htons(DNS.addition[j].name);
		type = htons(DNS.addition[j].type);
		cls  = htons(DNS.addition[j].cls);
		ttl  = htonl(DNS.addition[j].ttl);
		len  = htons(DNS.addition[j].len);
		data = DNS.addition[j].data;

		memcpy(p, &name, sizeof(name));
		p += sizeof(name);
		memcpy(p, &type, sizeof(type));
		p += sizeof(type);
		memcpy(p, &cls, sizeof(cls));
		p += sizeof(cls);
		memcpy(p, &ttl, sizeof(ttl));
		p += sizeof(ttl);
		memcpy(p, &len, sizeof(len));
		p += sizeof(len);
		memcpy(p, data, DNS.addition[j].len);
		p += DNS.addition[j].len;
	}
	addition_data_len = p - addition_data;

}


void pgen_dns::compile(){
	this->len = 0;
	compile_query();
	compile_answer();
	compile_auth();
	compile_addition();

	//UDP.dst = 53;
	UDP.len = UDP_HDR_LEN + DNS_HDR_LEN + query_data_len + answer_data_len + auth_data_len + addition_data_len;
	pgen_udp::compile();

	memset(&dns, 0, sizeof dns);
	dns.id = htons(DNS.id);
	dns.qr = DNS.flags.qr;
	dns.opcode = DNS.flags.opcode;
	dns.aa = DNS.flags.aa;
	dns.tc = DNS.flags.tc;
	dns.rd = DNS.flags.rd;
	dns.ra = DNS.flags.ra;
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
	p += sizeof(struct ip_header);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct udp_header);
	memcpy(p, &dns, sizeof dns);
	p += sizeof(struct dns_header);
	
	memcpy(p, &query_data, query_data_len);
	p += query_data_len;
	memcpy(p, &answer_data, answer_data_len);
	p += answer_data_len;
	memcpy(p, &auth_data, auth_data_len);
	p += auth_data_len;
	memcpy(p, &addition_data, addition_data_len);
	p += addition_data_len;
	
	len = p - data;
}




// madamada koko kakinaoshite hoshii
int pgen_dns::cast_query(const char* bpacket, int len){
	u_char* packet = (u_char*)malloc(len);
	memcpy(packet, (u_char*)bpacket, len);
	
	char* name;
	struct q_data{
		bit16 type;
		bit16 cls;
	};
	const struct q_data* buf;

	packet += ETH_HDR_LEN;
	packet += IP_HDR_LEN;
	packet += UDP_HDR_LEN;
	packet += DNS_HDR_LEN;
	const u_char* p = packet;
	

	for(int i=0; i<DNS.qdcnt; i++){
		name = (char*)(p+1);
		for(int j=0; name[j]!='\0'; j++){
			if(!is_dns_name_charcter(name[j])){
				name[j] = '.';
			}
		}
		p += strlen(name) + 2;
		buf = (struct q_data*)p;
		p += sizeof(struct q_data);

		DNS.query[i].name = name;
		DNS.query[i].type = ntohs(buf->type);
		DNS.query[i].cls  = ntohs(buf->cls);
	}
	
	return p - packet;
}



int pgen_dns::cast_answer(const char* packet, int blen){
	
	bit16* name;
	bit16* type;
	bit16* cls;
	bit32* ttl;
	bit16* len;
	

	packet += ETH_HDR_LEN;
	packet += IP_HDR_LEN;
	packet += UDP_HDR_LEN;
	packet += DNS_HDR_LEN;
	packet += query_data_len;
	const u_char* p = (u_char*)packet;
	
	for(int i=0; i<DNS.ancnt; i++){
		if(*p == 0x00){ // soa record
			bit16 zero = 0;
			name = &zero;
			p += 1;
		}else if(*p == 0xc0){
			name = (bit16*)p;
			p += 2;
		}else{
			fprintf(stderr, "pgen_dns::cast_answer: type maybe PTR not support yet\n");
			return 0;
		}
		
		type = (bit16*)p;
		p += 2;
		cls  = (bit16*)p;
		p += 2;
		ttl  = (bit32*)p;
		p += 4;
		len  = (bit16*)p;
		p += 2;
		
		DNS.answer[i].name = ntohs(*name);
		DNS.answer[i].type = ntohs(*type);
		DNS.answer[i].cls  = ntohs(*cls);
		DNS.answer[i].ttl  = ntohl(*ttl);
		DNS.answer[i].len  = ntohs(*len);
		memcpy(DNS.answer[i].data, p, DNS.answer[i].len);
		p += DNS.answer[i].len;
		
		
	}

	return p - (u_char*)packet;
}



int pgen_dns::cast_auth(const char* packet, int blen){
	
	bit16* name;
	bit16* type;
	bit16* cls;
	bit32* ttl;
	bit16* len;
	

	packet += ETH_HDR_LEN;
	packet += IP_HDR_LEN;
	packet += UDP_HDR_LEN;
	packet += DNS_HDR_LEN;
	packet += query_data_len;
	packet += answer_data_len;
	const u_char* p = (u_char*)packet;
	
	for(int i=0; i<DNS.nscnt; i++){
		if(*p == 0x00){ // soa record
			bit16 zero = 0;
			name = &zero;
			p += 1;
		}else if(*p == 0xc0){
			name = (bit16*)p;
			p += 2;
		}else{
			fprintf(stderr, "pgen_dns::cast_auth: type maybe PTR not support yet\n");
			return 0;
		}

		type = (bit16*)p;
		p += 2;
		cls  = (bit16*)p;
		p += 2;
		ttl  = (bit32*)p;
		p += 4;
		len  = (bit16*)p;
		p += 2;

		DNS.auth[i].name = ntohs(*name);
		DNS.auth[i].type = ntohs(*type);
		DNS.auth[i].cls  = ntohs(*cls);
		DNS.auth[i].ttl  = ntohl(*ttl);
		DNS.auth[i].len  = ntohs(*len);
		memcpy(DNS.auth[i].data, p, DNS.auth[i].len);
		p += DNS.auth[i].len;
		
	}

	return p - (u_char*)packet;
}


int pgen_dns::cast_addition(const char* packet, int blen){
	
	bit16* name;
	bit16* type;
	bit16* cls;
	bit32* ttl;
	bit16* len;
	

	packet += ETH_HDR_LEN;
	packet += IP_HDR_LEN;
	packet += UDP_HDR_LEN;
	packet += DNS_HDR_LEN;
	packet += query_data_len;
	packet += answer_data_len;
	packet += auth_data_len;
	const u_char* p = (u_char*)packet;
	
	for(int i=0; i<DNS.arcnt; i++){
		if(*p == 0x00){ // soa record
			bit16 zero = 0;
			name = &zero;
			p += 1;
		}else if(*p == 0xc0){
			name = (bit16*)p;
			p += 2;
		}else{
			fprintf(stderr, "pgen_dns::cast_addition: type maybe PTR not support yet\n");
			return 0;
		}

		type = (bit16*)p;
		p += 2;
		cls  = (bit16*)p;
		p += 2;
		ttl  = (bit32*)p;
		p += 4;
		len  = (bit16*)p;
		p += 2;
		
		DNS.addition[i].name = ntohs(*name);
		DNS.addition[i].type = ntohs(*type);
		DNS.addition[i].cls  = ntohs(*cls);
		DNS.addition[i].ttl  = ntohl(*ttl);
		DNS.addition[i].len  = ntohs(*len);
		memcpy(DNS.addition[i].data, p, DNS.addition[i].len);
		p += DNS.addition[i].len;
		
		/*
		printf("addition[%d].name = 0x%x \n", i, DNS.addition[i].name);
		printf("addition[%d].type = %d \n", i, DNS.addition[i].type);
		printf("addition[%d].cls  = %d \n", i, DNS.addition[i].cls );
		printf("addition[%d].ttl  = %d \n", i, DNS.addition[i].ttl );
		printf("addition[%d].len  = %d \n", i, DNS.addition[i].len );
		printf("addition[%d].data = ", i);
		for(int k=0; k<DNS.addition[i].len; k++){
			printf("%d.", *(DNS.addition[i].data+k));
		}printf("\n");
		*/
	}

	return p - (u_char*)packet;
}



void pgen_dns::cast(const void* packet, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_tcp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}

	pgen_udp::cast(packet, len);	
	const u_char* p = (u_char*)packet;

	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;
	p += UDP_HDR_LEN;
	
	struct dns_header* buf = (struct dns_header*)p;
	p += DNS_HDR_LEN;

	this->DNS.id = ntohs(buf->id);
	this->DNS.flags.qr = buf->qr;
	this->DNS.flags.opcode = buf->opcode;
	this->DNS.flags.aa = buf->aa;
	this->DNS.flags.tc = buf->tc;
	this->DNS.flags.rd = buf->rd;
	this->DNS.flags.ra = buf->ra;
	this->DNS.flags.nouse = buf->nouse;
	this->DNS.flags.rcode = buf->rcode;
	this->DNS.qdcnt = ntohs(buf->qdcnt);
	this->DNS.ancnt = ntohs(buf->ancnt);
	this->DNS.nscnt = ntohs(buf->nscnt);
	this->DNS.arcnt = ntohs(buf->arcnt);
	
	query_data_len = cast_query((char*)packet, len);	
	p += query_data_len;
	answer_data_len = cast_answer((char*)packet, len);
	p += answer_data_len;
	auth_data_len = cast_auth((char*)packet, len);
	p += auth_data_len;
	addition_data_len = cast_addition((char*)packet, len);
	p += addition_data_len;
	
	
	this->len = p - (u_char*)packet;
}





// not coding now
void pgen_dns::summary(){
	compile();
	printf("DNS{ ");
	if(dns.qr == 1){
  		printf("Query response 0x%04x %s type=%d ", ntohs(dns.id), DNS.query[0].name.c_str(),
				DNS.answer[0].type);
		dns_print_record_data(DNS.answer[0].data, DNS.answer[0].len, DNS.answer[0].type);
		printf(" }\n");
	}else{
		printf("Query 0x%04x %s type=%d }\n", ntohs(dns.id), DNS.query[0].name.c_str(),
				DNS.query[0].type);	
	}
}




void pgen_dns::debug(){
	compile();
		
	printf(" **Domain Name System *******************************\n");
	printf("    - Identification  : 0x%04x\n", ntohs(dns.id));
	printf("    - Question        : 0x%04x\n", ntohs(dns.qdcnt));
	printf("    - Answer RRs      : 0x%04x\n", ntohs(dns.ancnt));
	printf("    - Authority RRs   : 0x%04x\n", ntohs(dns.nscnt));
	printf("    - Additional RRs  : 0x%04x\n", ntohs(dns.arcnt));

	for(int i=0; i<DNS.qdcnt; i++){
		printf("    - Queries[%d] \n", i);
		printf("         - name       : %s \n", DNS.query[i].name.c_str());
		printf("         - type       : 0x%04x \n", DNS.query[i].type);
		printf("         - class      : 0x%04x \n", DNS.query[i].cls);
	}
	
	for(int i=0; i<DNS.ancnt; i++){
		printf("    - Answer[%d]  \n", i);
		printf("         - name       : 0x%04x (kaihatutyu)\n", DNS.answer[i].name);
		printf("         - type       : 0x%04x \n", DNS.answer[i].type);
		printf("         - class      : 0x%04x \n", DNS.answer[i].cls);
		printf("         - ttl        : 0x%08x \n", DNS.answer[i].ttl);
		printf("         - data len   : 0x%04x \n", DNS.answer[i].len);
		printf("         - address    : %s \n", DNS.answer[i].data);
	}
	printf(" ****************************************************\n");
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

	for(int i=0; i<DNS.qdcnt; i++){
		printf("    - Queries[%d] \n", i);
		printf("         - name       : %s \n", DNS.query[i].name.c_str());
		printf("         - type       : 0x%04x \n", DNS.query[i].type);
		printf("         - class      : 0x%04x \n", DNS.query[i].cls);
	}
	
	for(int i=0; i<DNS.ancnt; i++){
		printf("    - Answers[%d]  \n", i);
		printf("         - name       : 0x%04x (kaihatutyu)\n", DNS.answer[i].name);
		printf("         - type       : 0x%04x \n", DNS.answer[i].type);
		printf("         - class      : 0x%04x \n", DNS.answer[i].cls);
		printf("         - ttl        : 0x%08x \n", DNS.answer[i].ttl);
		printf("         - data len   : 0x%04x \n", DNS.answer[i].len);
		printf("         - data       : ");
		dns_print_record_data(DNS.answer[i].data, DNS.answer[i].len, DNS.answer[i].type);
		printf("\n");
	}

	for(int i=0; i<DNS.nscnt; i++){
		printf("    - Authoritative nameservers[%d]  \n", i);
		printf("         - name       : 0x%04x (kaihatutyu)\n", DNS.auth[i].name);
		printf("         - type       : 0x%04x \n", DNS.auth[i].type);
		printf("         - class      : 0x%04x \n", DNS.auth[i].cls);
		printf("         - ttl        : 0x%08x \n", DNS.auth[i].ttl);
		printf("         - data len   : 0x%04x \n", DNS.auth[i].len);
		printf("         - data       : ");
		dns_print_record_data(DNS.auth[i].data, DNS.auth[i].len, DNS.auth[i].type);
		printf("\n");

	}

	for(int i=0; i<DNS.arcnt; i++){
		printf("    - Additional records[%d]  \n", i);
		printf("         - name       : 0x%04x (kaihatutyu)\n", DNS.addition[i].name);
		printf("         - type       : 0x%04x \n", DNS.addition[i].type);
		printf("         - class      : 0x%04x \n", DNS.addition[i].cls);
		printf("         - ttl        : 0x%08x \n", DNS.addition[i].ttl);
		printf("         - data len   : 0x%04x \n", DNS.addition[i].len);
		printf("         - data       : ");
		dns_print_record_data(DNS.addition[i].data, DNS.addition[i].len, DNS.addition[i].type);
		printf("\n");
	}
}




static void dns_print_record_data(const bit8* data, int len, int type){
	switch(type){
		case 1: // A record
			printf("%d.%d.%d.%d", data[0], data[1], data[2], data[3]);
			break;
		case 2: // NS record
		case 5: // CNAME record
			char name[32];
			memcpy(name, data+1, sizeof(name));
			for(int i=0; name[i]!='\0'; i++){
				if(is_dns_name_charcter(name[i]) == false) name[i]='.';	
			}
			printf("%s", name);
			break;
		case 28: // AAAA record
			for(int i=0; i<8; i++){
				printf("%04x", (bit16)*(data+(i*2)));
				if(i<7) printf(":");
			}
			break;
		default:
			fprintf(stderr, "print_data: type not support\n");
			break;
	}
}






static bool is_dns_name_charcter(char c){
	return (('a'<=c && c<='z') || ('A'<=c && c<='Z') || ('0'<=c && c<='9')|| c=='-');
}



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

	for(int i=0; i<(int)vec.size(); i++){
		char buf[100];
		snprintf(buf, sizeof(buf), ".%s", vec[i].c_str());
		buf[0] = (int)strlen(vec[i].c_str());
		buf[strlen(vec[i].c_str())+1] = '\0';
		
		strncat(result, buf, result_len-strlen(result));
	}
	return result;
}

