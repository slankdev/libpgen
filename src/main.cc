#include "pgen.h"

int main(){
	pgen_tcp p;
	p.eth_srcEth = pgen_getMAC("eth0");
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.ip_srcIp = pgen_getIP("eth0");
	p.ip_dstIp = "255.255.255.255";
	p.tcp_srcPort = 1111;
	p.tcp_dstPort = 2222;
	p.wrap("eth0");
	p.info();
	p.sendTest();
}


/*
int main(){
	pgen_eth eth;
	pgen_arp arp;
	pgen_ip ip;
	pgen_icmp icmp;


	printf("ETH========================================\n");
	eth.eth_srcEth = pgen_getMAC("eth0");
	eth.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	eth.wrap("eth0");
	eth.info();
	eth.sendTest();
	printf("===========================================\n");
	
	
	
	printf("ARP========================================\n");
	arp.eth_srcEth = pgen_getMAC("eth0");
	arp.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	arp.arp_srcIp = pgen_getIP("eth0");
	arp.arp_dstIp = "172.16.234.1";
	arp.arp_option = ARPOP_REQUEST;
	arp.wrap("eth0");
	arp.info();
	arp.sendTest();
	printf("===========================================\n");
	
	
	
	printf("IP=========================================\n");
	ip.eth_srcEth = pgen_getMAC("eth0");
	ip.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	ip.ip_srcIp = pgen_getIP("eth0");
	ip.ip_dstIp = "255.255.255.255";
	ip.wrap("eth0");
	ip.info();
	ip.sendTest();
	printf("===========================================\n");
	
	
	
	printf("ICMP========================================\n");
	icmp.eth_srcEth = pgen_getMAC("eth0");
	icmp.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	icmp.ip_srcIp = pgen_getIP("eth0");
	icmp.ip_dstIp = "172.16.234.1";
	icmp.icmp_option = PGEN_ICMPOP_ECHO;
	icmp.icmp_code   = PGEN_ICMPCODE_NET_UNREACH;
	icmp.wrap("eth0");
	icmp.info();
	icmp.sendTest();
	printf("===========================================\n");


	
}*/
