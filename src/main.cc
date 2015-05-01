#include "pgen.h"

const char* dev = "wlan0";

int main(){
	pgen_icmp p;
	
	p.eth_srcEth = pgen_getMAC(dev);
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.179.1";
	p.icmp_option = PGEN_ICMPOP_ECHO;
	p.icmp_code   = PGEN_ICMPCODE_NET_UNREACH;

	p.send(dev);
	p.info();
	p.hex();


	/*//[[[
	p.eth_srcEth = pgen_getMAC(dev);
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_srcEth = pgen_getMAC(dev);
	p.arp_srcIp  = pgen_getIP(dev);
	p.arp_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_dstIp  = "192.168.179.1";
	p.arp_option = PGEN_ARPOP_REQEST;
	*///]]]
}


