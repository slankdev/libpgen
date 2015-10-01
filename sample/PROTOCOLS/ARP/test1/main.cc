#include <stdio.h>
#include <pgen.h>

const char* dev  = "en0";
const char* file = "test.pcap";

int main(int argc, char** argv){
	pgen_t* h = pgen_open_offline(file, 1);
	if(h == NULL){
		pgen_perror("Oops");
		return -1;
	}
	pgen_t* w = pgen_open(dev, NULL);
	if(w == NULL){
		pgen_perror("Oops");
		return -1;
	}

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
	pack.send_handle(h);
	pack.send_handle(w);

	pgen_close(h);
}

