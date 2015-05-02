#include "pgen.h"

const char* dev = "wlan0";

void tcp();
void udp();
void ip();
void arp();
void icmp();


int main(){
	/*
	printf("BE: %d\n",BIG_ENDIAN);
	printf("LE: %d\n",LITTLE_ENDIAN);
	printf("%d\n",__BYTE_ORDER);
	*/

	//arp();
	//ip();
	//icmp();
	//tcp();
	udp();
}


void tcp(){//[[[
	pgen_tcp p;

	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";
	p.tcp_srcPort = 65432;
	p.tcp_dstPort = 80;
	p.tcp_doff = 20;
	p.tcp_window = 8192;
	p.tcp_frag.syn = 1;
	//p.tcp_frag.ack = 1;


	p.sendPack(dev);
	p.info();
	p.hex();
}//]]]

void udp(){
	pgen_udp p;

	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";
	p.udp_srcPort = 8888;
	p.udp_dstPort = 7;

	p.sendPack(dev);
	p.info();
	p.hex();
 }

void ip(){
	pgen_ip p;

	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";

	p.sendPack(dev);
	p.info();
	p.hex();
}

void arp(){
	pgen_arp p;

	p.eth_srcEth = pgen_getMAC(dev);
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_srcEth = pgen_getMAC(dev);
	p.arp_srcIp  = pgen_getIP(dev);
	p.arp_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_dstIp  = "192.168.0.1";
	p.arp_option = PGEN_ARPOP_REQEST;
		
	p.sendPack(dev);
	p.info();
	p.hex();
}

void icmp(){ 
	pgen_icmp p;
	
	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";
	p.icmp_option = PGEN_ICMPOP_ECHO;
	p.icmp_code   = PGEN_ICMPCODE_NET_UNREACH;
	
	p.sendPack(dev);
	p.info();
	p.hex();
}


