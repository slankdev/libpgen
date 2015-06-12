
#include <stdio.h>
#include <pgen.h>
const char* dev = "wlan0";

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "usage: %s targetIP\n", argv[0]);
		return -1;
	}

	pgen_icmp pack;
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = argv[1];
	pack.ICMP.option = 8;
	pack.ICMP.code = 0;
	pack.ICMP.id = 10;
	pack.ICMP.seq = 1;

	for(int i=0; i<5; i++){
		pack.SEND(dev);
		pack.ICMP.id--;
	//	pack.ICMP.seq++;
	}
	pack.INFO();
}
