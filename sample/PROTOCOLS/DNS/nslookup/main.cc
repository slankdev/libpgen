#include <pgen.h>
#include <stdio.h>

const char *dev = "en0";
//const char* dnsserver = "133.25.243.11";
const char* dnsserver = "192.168.179.1";

bool func(const u_char* p, int len){
	pgen_unknown buf(p, len);
	if(buf.portis(53)){
		pgen_dns pack(p, len);
		if(pack.DNS.flags.qr == 1){
			pack.summary();
			return false;
		}
	}
	return true;
}



int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s name \n", argv[0]);
		return -1;
	}
	
	pgen_t* h = pgen_open(dev, NULL);
	if(h == NULL){
		pgen_perror("Oops");
		return -1;
	}
	arptable at(h);

	pgen_dns pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = at.find(dnsserver);
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = dnsserver;
	pack.UDP.src = 53;
	pack.UDP.dst = 53;
	pack.DNS.id = 0x95de;
	pack.DNS.query[0].name = argv[1];
	
	pack.summary();
	pack.send_handle(h);
	sniff(h, func);
}
