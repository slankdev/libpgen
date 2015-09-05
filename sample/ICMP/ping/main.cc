#include <pgen.h>
const char* dev = "wlan1";

bool func(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(!buf.isICMP) return true;

	pgen_icmp pack(packet, len);
	if(!(pack.ICMP.option == 0 && pack.ICMP.code == 0))
		return true;
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

	pgen_icmp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst.setmacbroadcast();
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = argv[1];

	for(int i=0; i<10; i++){
		pack.send_handle(handle);
		sniff(handle, func);
		sleep(1);
		pack.ICMP.seq++;
	}
}
