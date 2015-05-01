
#include <map>
#include <string>
#include <iostream>

/* include must!!! */
#include "packet.h"
#include "pgen-variable.h"
#include "pgen-macro.h"
#include "pgen-opcode.h"
#include "pgen-funcs.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//#include <net/ethernet.h> 
//#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "netutil.h"



pgen_icmp::pgen_icmp(){
	pgen_ip::clear();
	clear();
}
void pgen_icmp::clear(){
	pgen_packet::clear();
	icmp_option -1;
	icmp_code = -1;
}

void pgen_icmp::send(const char* ifname){
	int sock;
	int n;
	
	/*
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip_dstIp._addr;
	*/

	wrap(ifname);		
	
	
	if((sock=initRawSocket(ifname, 1, 0)) < 0){
		exit(PGEN_ERROR);
	}
	if((n=write(sock, data, len)) < 0){
//	if((n=sendto(sock, data, len, 0, (struct sockaddr*)&addr, sizeof addr)) < 0){
		perror("ip::send sendto()");
		exit(PGEN_ERROR);
	}

	close(sock);
		
}



void pgen_icmp::wrap(const char* ifname){
	packetType = PGEN_PACKETTYPE_ICMP;
	pgen_ip::wrap(ifname);
	memset(data, 0, sizeof(data));
	ip.protocol = IPPROTO_ICMP;
	ip.tot_len = sizeof(ip) + sizeof(icmp) + 100;


	memset(&icmp, 0, sizeof icmp);
	icmp.type = icmp_option;
	icmp.code = icmp_code;
	icmp.checksum = 0;
	icmp.un.echo.id = 0;
	icmp.un.echo.sequence = 0;
	icmp.checksum = checksum(&icmp, sizeof icmp);


	u_char* p = data;
	memcpy(p, &eth, sizeof(eth));
	p += sizeof(eth);
	memcpy(p, &ip, sizeof(ip));
	p += sizeof(ip);
	memcpy(p, &icmp, sizeof(icmp));
	len = p-data;

}



/*
void pgen_icmp::wrapLite(const char* ifname){//[[[
	int sock;
	packetType = PGEN_PACKETTYPE_ICMP;
	memset(data, 0, sizeof(data));

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ip_dstIp._addr;
	memcpy(&addr, &sin, sizeof(sin));
	if((sock=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0){
		perror("icmp::wrapLite bind()");
		exit(PGEN_ERROR);
	}

	memset(&icmp, 0, sizeof icmp);
	icmp.type = icmp_option;
	icmp.code = icmp_code;
	icmp.checksum = 0;
	icmp.un.echo.id = 0;
	icmp.un.echo.sequence = 0;
	icmp.checksum = checksum(&icmp, sizeof icmp);
	
	u_char* p = data;
	memcpy(p, &icmp, sizeof(icmp));
	p += sizeof(icmp);
	len = p-data;
}//]]]
*/



void pgen_icmp::info(){
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
	printf("    - Type        :  %s (%d)\n", _icmpoption[icmp.type] , icmp.type);
	printf("    - Code        :  %s (%d)\n",  _icmpcode[icmp.code], icmp.code);
}



