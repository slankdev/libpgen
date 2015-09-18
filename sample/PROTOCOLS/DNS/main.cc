#include <stdio.h>
#include <pgen.h>

const char* dev = "wlan0";

bool func(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(!(buf.isUDP && buf.portis(53))) return true;
	
	pgen_dns pack(packet, len);
	pack.summary();
	return false;
}


int main(int argc, char** argv){
	if(argc < 2){
		printf("Usage: %s host \n", argv[0]);	
		return -1;
	}

	pgen_t* handle = pgen_open(dev, 1, NULL);
	if(handle == NULL){
		return -1;	
	}

	pgen_dns pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "192.168.222.1";
	pack.UDP.src = 53;
	pack.UDP.dst = 53;
	pack.DNS.id = 0x1234;
	pack.DNS.query[0].name = argv[1];
	
	pack.summary();
	pack.send_handle(handle);
	sniff(handle, func);
}

