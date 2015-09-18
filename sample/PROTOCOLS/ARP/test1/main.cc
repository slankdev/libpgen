#include <stdio.h>
#include <pgen.h>

const char* dev = "en0";

int main(int argc, char** argv){
	pgen_arp pack;
	
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst.setmacbroadcast();
	pack.ARP.srcEth.setmacbydev(dev);
	pack.ARP.srcIp.setipbydev(dev);
	pack.ARP.dstEth = 0;
	pack.ARP.dstIp = "192.168.1.123";
	/*
	*/

	pack.summary();
	pack.info();
	pack.hex();
}

