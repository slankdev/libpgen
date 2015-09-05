#include <map>
#include <string>
#include <iostream>

#include "packet.h"
#include "address.h"
#include "netutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>








pgen_icmp::pgen_icmp(){
	clear();
}



pgen_icmp::pgen_icmp(const u_char* packet, int len){
	clear();
	cast(packet, len);
}



void pgen_icmp::clear(){
	this->ICMP.option = 8;
	this->ICMP.code = 0;
	this->ICMP.id = 0;
	this->ICMP.seq = 0;
} 





void pgen_icmp::compile(){
	this->IP.tot_len = IP_HDR_LEN + ICMP_HDR_LEN;
	this->IP.protocol = 1;
	pgen_ip::compile();

	memset(this->data, 0, PGEN_MAX_PACKET_LEN);

	memset(&this->icmp, 0, ICMP_HDR_LEN);
	this->icmp.icmp_type = this->ICMP.option;
	this->icmp.icmp_code = this->ICMP.code;
	this->icmp.icmp_cksum = 0;
	this->icmp.icmp_void = 0;
	this->icmp.icmp_hun.ih_idseq.icd_id = htons(this->ICMP.id);
	this->icmp.icmp_hun.ih_idseq.icd_seq = htons(this->ICMP.seq);
	this->icmp.icmp_cksum = checksum((unsigned short*)&this->icmp, ICMP_HDR_LEN);

	u_char* p = this->data;
	memcpy(p, &this->eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &this->ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	memcpy(p, &this->icmp, ICMP_HDR_LEN);
	p += ICMP_HDR_LEN;
	len = p-this->data;

	compile_addData();
} 




void pgen_icmp::cast(const u_char* data, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_icmp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}
	
	pgen_ip::cast(data, len);

	const u_char* p = data;
	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;
	
	struct MYICMP *buf = (struct MYICMP*)p;
	p += ICMP_HDR_LEN;

	this->ICMP.option = buf->icmp_type;
	this->ICMP.code = buf->icmp_code;
	this->ICMP.id = ntohs(buf->icmp_hun.ih_idseq.icd_id);
	this->ICMP.seq = ntohs(buf->icmp_hun.ih_idseq.icd_seq);

	this->len = p - data;
	addData(p, len-(p-data));
} 




void pgen_icmp::summary(){
	compile();
	printf("ICMP{ ");
	if(ICMP.option == 8 && ICMP.code == 0){
		printf("Echo Request id=%d seq=%d ttl=%d }\n", 
				ICMP.id, ICMP.seq, IP.ttl );
	}else if(ICMP.option == 0 && ICMP.code == 0){
		printf("Echo Relay   id=%d seq=%d ttl=%d }\n", 
				ICMP.id, ICMP.seq, IP.ttl );
	}else{
		printf("other icmp type }\n");	
	}
}



void pgen_icmp::info(){
	compile();
	pgen_ip::info();

	std::map<int, const char*>  _icmpoption;
	_icmpoption[0] = "Echo Reply";
	_icmpoption[5] = "REdirect"; 
	_icmpoption[8] = "Echo"; 
	std::map<int, const char*> _icmpcode;
	_icmpcode[0]  = "Net Unreachable";
	_icmpcode[1]  = "Host Unreachable";
	_icmpcode[2]  = "Protocol Unreachable";
	_icmpcode[3]  = "Port Unreachable";
	_icmpcode[255]  = "Not Set";

	printf(" * Internet Control Message Protocol \n");
	printf("    - Option          :  %s (%d)\n", 
			_icmpoption[ICMP.option] , ICMP.option);
	printf("    - Code            :  %s (%d)\n",  
			_icmpcode[ICMP.code], ICMP.code);
	printf("    - id(BE/LE)       :  %d/%d \n", 
			htons(icmp.icmp_hun.ih_idseq.icd_id), icmp.icmp_hun.ih_idseq.icd_id);
	printf("    - seq num(BE/LE)  :  %d/%d \n", 
			htons(icmp.icmp_hun.ih_idseq.icd_seq), icmp.icmp_hun.ih_idseq.icd_seq);
	printf("    - Header Checksum :  0x%x \n", icmp.icmp_cksum);
}




