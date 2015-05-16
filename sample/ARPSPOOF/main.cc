#include "pgen.h"

const char* dev = "wlan0";

ipaddr target = "192.168.0.1";

int main(){
	pgen_arp pack;

	pack.ETH.src = pgen_getMAC(dev);
	pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
	pack.ARP.srcEth = pgen_getMAC(dev);
	pack.ARP.srcIp  = pgen_getIP(dev);
	pack.ARP.dstEth = "ff:ff:ff:ff:ff:ff";
	pack.ARP.dstIp  = target;
	pack.ARP.option = PGEN_ARPOP_REQEST;
		
	pack.info();
	pack.hex();

	printf("\n");
	for(int i=0; i<10; i++){
		printf("%d: target is %s\n", i, target.c_str());
		//printf("%d: %s\n", i, pack.summary());
		pack.sendPack(dev);
		sleep(1);
	}

}

