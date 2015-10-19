#include <stdio.h>
#include <pgen.h>

const char* dev  = "en0";

int main(int argc, char** argv){
	pgen_t* w = pgen_open(dev, NULL);
	if(w == NULL){
		pgen_perror("Oops");
		return -1;
	}

	pgen_arp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "a2:12:42:17:d8:8f";
	pack.ARP.srcEth.setmacbydev(dev);
	pack.ARP.srcIp.setipbydev(dev);
	pack.ARP.dstEth = "a2:12:42:17:d8:8f";
	pack.ARP.dstIp  = "192.168.179.1";
	pack.ARP.operation = 1;

	pack.summary();
	pack.info();
	pack.hex();
	pack.send_handle(w);

	pgen_close(w);
}

