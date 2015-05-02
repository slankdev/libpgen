#include "pgen.h"

const char* dev = "wlan0";

pgent_mac  macsrc = pgen_getMAC(dev);
pgent_mac  macdst = "ff:ff:ff:ff:ff:ff";
pgent_ip ipsrc = pgen_getIP(dev);
pgent_ip ipdst = "192.168.179.18";
int sport = 8888;
int dport = 9999;



void udp(){//[[[
	pgen_udp p;

	p.ip_srcIp = ipsrc;
	p.ip_dstIp = ipdst;
	p.udp_srcPort = sport;
	p.udp_dstPort = dport;

	p.sendPack(dev);
	p.info();
	p.hex();
}//]]]

void tcp(){
	pgen_tcp p;

	p.ip_srcIp = ipsrc;
	p.ip_dstIp = ipdst;
	p.tcp_srcPort = sport;
	p.tcp_dstPort = dport;
	p.tcp_frag.syn = 1;
	p.tcp_frag.ack = 1;

	p.sendPack(dev);
	p.info();
	p.hex();
}

void ip(){//[[[
	pgen_ip p;

	p.ip_srcIp = ipsrc;
	p.ip_dstIp = ipdst;

	p.sendPack(dev);
	p.info();
	p.hex();
}//]]]

void arp(){//[[[
	pgen_arp p;

	p.eth_srcEth = macsrc;
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_srcEth = macsrc;
	p.arp_srcIp  = ipsrc;
	p.arp_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_dstIp  = ipdst;
	p.arp_option = PGEN_ARPOP_REQEST;
		
	p.sendPack(dev);
	p.info();
	p.hex();
}//]]]

void icmp(){//[[[ 
	pgen_icmp p;
	
	p.ip_srcIp = ipsrc;
	p.ip_dstIp = ipdst;
	p.icmp_option = PGEN_ICMPOP_ECHO;
	p.icmp_code   = PGEN_ICMPCODE_NET_UNREACH;

	p.sendPack(dev);
	p.info();
	p.hex();
}//]]]

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
	//udp();
}

