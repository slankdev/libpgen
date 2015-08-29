#include <stdio.h>
#include <pgen.h>

const char* dev = "wlan0";

bool func(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(buf.isARP == false) return true;

	pgen_arp pack(packet, len);
	pack.summary();
	return false;
}


int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s ipaddr \n", argv[0]);
		return -1;
	}

	pgen_t* handle = pgen_open(dev, 1, NULL);
	if(handle == NULL){
		return -1;	
	}

	pgen_arp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
	pack.ARP.srcIp.setipbydev(dev);
	pack.ARP.srcEth.setmacbydev(dev);
	pack.ARP.dstIp = argv[1];
	pack.ARP.dstEth = 0;

	pack.summary();
	pack.send_handle(handle);
	sniff(handle, func);

}

