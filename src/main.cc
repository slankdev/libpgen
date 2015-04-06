#include "pgen.h"


int main55(){
	u_int32_t myad = inet_addr("192.168.179.1");
	pgent_ip  ip;
	ip = "192.168.179.1";
	if(ip._addr == myad)	printf("same\n");
	else 	printf("no same\n");
}

int main(){
	pgen_eth eth;
	pgen_arp arp;
	pgen_ip ip;
	pgen_icmp icmp;

/*
	printf("ETH========================================\n");
	eth.eth_srcEth = pgen_getMAC("eth0");
	eth.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	eth.wrap("eth0");
	eth.info();
	eth.send();
	printf("===========================================\n");
*/	
	
	
	printf("ARP========================================\n");
	arp.eth_srcEth = pgen_getMAC("eth0");
	arp.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	arp.arp_srcIp = pgen_getIP("eth0");
	arp.arp_dstIp = "192.168.179.1";
	arp.arp_option = ARPOP_REQUEST;
	arp.wrap("eth0");
	arp.info();
	arp.send();
	printf("===========================================\n");
	
	
/*	
	printf("IP=========================================\n");
	ip.eth_srcEth = pgen_getMAC("eth0");
	ip.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	ip.ip_srcIp = pgen_getIP("eth0");
	ip.ip_dstIp = "255.255.255.255";
	ip.wrap("eth0");
	ip.info();
	ip.send();
	printf("===========================================\n");
*/	
	
	
	printf("ICMP========================================\n");
	icmp.ip_dstIp = "192.168.179.1";
	icmp.icmp_option = PGEN_ICMPOP_ECHO;
	icmp.icmp_code 	 = PGEN_ICMPCODE_NET_UNREACH;
	icmp.wrapLite("eth0");
	icmp.info();
	icmp.send();
	printf("===========================================\n");


}
