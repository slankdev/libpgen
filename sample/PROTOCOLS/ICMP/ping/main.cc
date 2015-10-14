#include <pgen.h>
const char* dev = "en0";

bool func(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(!buf.isICMP) return true;

	pgen_icmp pack(packet, len);
	if(!(pack.ICMP.type == 0 && pack.ICMP.code == 0))
		return true;
	pack.summary();
	return false;
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s ipaddr \n", argv[0]);
		return -1;
	}

	pgen_t* handle = pgen_open(dev, NULL);
	if(handle == NULL){
		pgen_perror("test");
		return -1;	
	}
	arptable at(handle);

	pgen_icmp pack;
	pack.ETH.src.setmacbydev(dev);
	printf("test\n");
	pack.ETH.dst = at.find(argv[1]);
	printf("test\n");
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = argv[1];
	pack.ICMP.echo.id = 0x1234;
	

	for(int i=0; i<10; i++){
		pack.send_handle(handle);
		sniff(handle, func);
		sleep(1);
		pack.ICMP.echo.seq++;
	}
}
