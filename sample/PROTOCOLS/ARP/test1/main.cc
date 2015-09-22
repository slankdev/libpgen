#include <stdio.h>
#include <pgen.h>

const char* dev = "en0";

int main(int argc, char** argv){
	pgen_arp pack;
	
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "a2:12:42:22:17:d8";
	printf("test\n");
	pack.ARP.srcEth.setmacbydev(dev);
	pack.ARP.srcIp.setipbydev(dev);
	pack.ARP.dstEth = "a2:12:42:22:17:d8";
	pack.ARP.dstIp = "255.254.1.123";
	pack.ARP.operation = 2;


	pack.summary();
	pack.info();
	pack.hex();
}

