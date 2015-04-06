#include "pgen.h"



int main(){
	pgen_eth eth;
	pgen_arp arp;
	pgen_ip ip;
	pgen_icmp icmp;
	pgen_udp udp;

	
	printf("ARP========================================\n");
	arp.eth_srcEth = pgen_getMAC("eth0");
	arp.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	arp.arp_srcEth = pgen_getMAC("eth0");
	arp.arp_srcIp = pgen_getIP("eth0");
	arp.arp_dstEth = 0;
	arp.arp_dstIp = "192.168.179.1";
	arp.arp_option = ARPOP_REQUEST;
	arp.wrap("eth0");
	arp.info();
	arp.send();
	printf("===========================================\n");
	
	
	printf("ICMP========================================\n");
	icmp.ip_dstIp = "192.168.179.1";
	icmp.icmp_option = PGEN_ICMPOP_ECHO;
	icmp.icmp_code 	 = PGEN_ICMPCODE_NET_UNREACH;
	icmp.wrapLite("eth0");
	icmp.info();
	icmp.send();
	printf("===========================================\n");

	printf("IP=========================================\n");
	ip.ip_dstIp = "192.168.179.1";
	ip.wrapLite("eth0");
	ip.info();
	ip.send();
	printf("===========================================\n");

	
	printf("UDP========================================\n");
	udp.ip_dstIp = "192.168.179.1";
	//udp.udp_srcPort = 11111;
	udp.udp_srcPort = 123;
	//udp.udp_dstPort = 22222;
	udp.udp_dstPort = 123;
	udp.wrapLite("eth0");
	udp.info();
	udp.send();
	printf("===========================================\n");
}
