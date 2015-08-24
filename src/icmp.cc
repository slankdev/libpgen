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
	ICMP.option = 8;
	ICMP.code = 0;
	ICMP.id = 0;
	ICMP.seq = 0;
} 




void pgen_icmp::cast(const bit8* data, int len){
	if(!( minLen<=len && len<=maxLen )){
		fprintf(stderr, "icmp packet length not support (%d)\n", len);
		return;
	}
	
	
	pgen_ip::cast(data, len);


	struct MYICMP buf;
	memcpy(&buf, data+sizeof(struct MYETH)+sizeof(struct MYIP), 
			sizeof(buf));

	ICMP.option = buf.icmp_type;
	ICMP.code = buf.icmp_code;
	ICMP.id = ntohs(buf.icmp_hun.ih_idseq.icd_id);
	ICMP.seq = ntohs(buf.icmp_hun.ih_idseq.icd_seq);

} 




void pgen_icmp::compile(){
	IP.tot_len = (sizeof(ip) + sizeof(icmp)) ;
	IP.protocol = MT_IPPROTO_ICMP;
	pgen_ip::compile();
	memset(data, 0, sizeof(data));

	memset(&icmp, 0, sizeof icmp);
	icmp.icmp_type = ICMP.option;
	icmp.icmp_code = ICMP.code;
	icmp.icmp_cksum = 0;
	icmp.icmp_void = 0;
	icmp.icmp_hun.ih_idseq.icd_id = htons(ICMP.id);
	icmp.icmp_hun.ih_idseq.icd_seq = htons(ICMP.seq);
	icmp.icmp_cksum = checksum((unsigned short*)&icmp, sizeof icmp);

	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &icmp, sizeof(icmp));
	p += sizeof(icmp);
	len = p-data;

	compile_addData();
} 



void pgen_icmp::summary(){
	compile();

	if(ICMP.option == PGEN_ICMPOP_ECHO && ICMP.code == 0){
		printf("Echo Request id=%d seq=%d ttl=%d \n", 
				ICMP.id, ICMP.seq, IP.ttl );
	}else if(ICMP.option == PGEN_ICMPOP_ECHOREPLY && ICMP.code == 0){
		printf("Echo Relay   id=%d seq=%d ttl=%d\n", 
				ICMP.id, ICMP.seq, IP.ttl );
	}else{
		printf("other icmp type\n");	
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



