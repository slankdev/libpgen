#include <map>
#include <string>
#include <iostream>

/* include must!!! */
#include "packet.h"
#include "address.h"
#include "packconf.h"
#include "pgen-funcs.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <netinet/ip_icmp.h>
#include "netutil.h"



pgen_icmp::pgen_icmp(){
	pgen_ip::CLEAR();
	CLEAR();
}

void pgen_icmp::CLEAR(){
	pgen_packet::CLEAR();
	ICMP.option = 8;
	ICMP.code = 0;
	ICMP.id = 1;
	ICMP.seq = 1;
}

void pgen_icmp::SEND(const char* ifname){
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



void pgen_icmp::WRAP(){
	pgen_ip::WRAP();
	packetType = PGEN_PACKETTYPE_ICMP;
	memset(data, 0, sizeof(data));
	ip.protocol = MT_IPPROTO_ICMP;
	ip.tot_len = htons(sizeof(ip) + sizeof(icmp)) ;

	memset(&icmp, 0, sizeof icmp);
	icmp.icmp_type = ICMP.option;
	icmp.icmp_code = ICMP.code;
	icmp.icmp_cksum = 0;
	icmp.icmp_void = 0;
	icmp.icmp_hun.ih_idseq.icd_id = htons(ICMP.id);
	icmp.icmp_hun.ih_idseq.icd_seq = htons(ICMP.seq);
	icmp.icmp_cksum = checksum(&icmp, sizeof icmp);

	u_char* p = data;
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &icmp, sizeof(icmp));
	p += sizeof(icmp);
	len = p-data;
} 



void pgen_icmp::SUMMARY(){
	INFO();

	if(ICMP.option == PGEN_ICMPOP_ECHO && ICMP.code == 0){
		printf("Echo Request id=%d seq=%d ttl=%d \n", 
				ntohs(icmp.icmp_hun.ih_idseq.icd_id),
				ntohs(icmp.icmp_hun.ih_idseq.icd_seq),
				ip.ttl
				);
	}else if(ICMP.option == PGEN_ICMPOP_ECHOREPLY && ICMP.code == 0){
		printf("Echo Relay   id=%d seq=%d ttl=%d\n", 
				ntohs(icmp.icmp_hun.ih_idseq.icd_id),
				ntohs(icmp.icmp_hun.ih_idseq.icd_seq),
				ip.ttl
			  );
	}else{
		printf("other icmp type\n");	
	}
}


void pgen_icmp::INFO(){
	WRAP();
	pgen_ip::INFO();

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
	printf("    - Type            :  %s (%d)\n", 
			_icmpoption[icmp.icmp_type] , icmp.icmp_type);
	printf("    - Code            :  %s (%d)\n",  
			_icmpcode[icmp.icmp_code], icmp.icmp_code);
	printf("    - id(BE/LE)       :  %d/%d \n", 
			htons(icmp.icmp_hun.ih_idseq.icd_seq), icmp.icmp_hun.ih_idseq.icd_id);
	printf("    - seq num(BE/LE)  :  %d/%d \n", 
			htons(icmp.icmp_hun.ih_idseq.icd_seq), icmp.icmp_hun.ih_idseq.icd_seq);
	printf("    - Header Checksum :  0x%x \n", icmp.icmp_cksum);
}



void pgen_icmp::setData(const u_char *p, int len){}
