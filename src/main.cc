#include "pgen.h"


int main(){
	pgen_udp p;
	p.ip_srcIp = pgen_getIP("eth0");
	p.ip_srcIp = "192.168.179.34";
	p.ip_dstIp = "192.168.179.1";
	p.udp_srcPort = 50;
	p.udp_dstPort = 10;
	p.wrap("eth0");
	p.info();
	p.send();
}



int main0(){
	pgen_eth eth;
	pgen_arp arp;
	pgen_ip ip;
	pgen_icmp icmp;
	pgen_udp udp;
	pgen_tcp tcp;

	
	arp.eth_srcEth = pgen_getMAC("eth0");
	arp.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	arp.arp_srcEth = pgen_getMAC("eth0");
	arp.arp_srcIp = pgen_getIP("eth0");
	arp.arp_dstEth = 0;
	arp.arp_dstIp = "192.168.179.1";
	arp.arp_option = ARPOP_REQUEST;
	arp.wrap("eth0");
	printf("ARP========================================\n");
	arp.info();
	printf("===========================================\n");
	arp.send();
	
	
	icmp.ip_dstIp = "192.168.179.1";
	icmp.icmp_option = PGEN_ICMPOP_ECHO;
	icmp.icmp_code 	 = PGEN_ICMPCODE_NET_UNREACH;
	icmp.wrapLite("eth0");
	printf("ICMP========================================\n");
	icmp.info();
	printf("===========================================\n");
	icmp.send();

	ip.ip_dstIp = "192.168.179.1";
	ip.wrapLite("eth0");
	printf("IP=========================================\n");
	ip.info();
	printf("===========================================\n");
	ip.send();

	
	udp.ip_dstIp = "192.168.179.1";
	udp.udp_srcPort = 123;
	udp.udp_dstPort = 123;
	udp.wrapLite("eth0");
	printf("UDP========================================\n");
	udp.info();
	printf("===========================================\n");
	udp.send();
	
	
	tcp.ip_dstIp = "192.168.179.1";
	tcp.tcp_srcPort = 11111;
	tcp.tcp_dstPort = 22222;
	tcp.wrapLite("eth0");
	printf("TCP========================================\n");
	tcp.info();
	printf("===========================================\n");
	tcp.send();
}
