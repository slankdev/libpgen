#include "pgen.h"

const char* dev = "wlan0";

int main(){
	pgen_arp p;
	
	p.eth_srcEth = pgen_getMAC(dev);
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_srcEth = pgen_getMAC(dev);
	p.arp_srcIp  = pgen_getIP(dev);
	p.arp_dstEth = 0;
	p.arp_dstIp  = "192.168.179.1";

	p.info();
	p.send(dev);
}


