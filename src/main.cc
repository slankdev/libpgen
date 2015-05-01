#include "pgen.h"

const char* dev = "wlan0";

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
	
	//p.eth_srcEth = pgen_getMAC(dev);
	//p.eth_dstEth = "00:10:18:de:ad:05";
	//p.eth_dstEth = "80:e6:50:17:18:46";
	p.ip_srcIp = pgen_getIP(dev);
	//p.ip_srcIp = "192.168.0.1";
	p.ip_dstIp = "192.168.0.18";
	p.icmp_option = PGEN_ICMPOP_ECHO;
	p.icmp_code   = PGEN_ICMPCODE_NET_UNREACH;

	p.sendPack(dev);
	p.info();
	p.hex();


}

int main(){
//	arp();
	icmp();
}

