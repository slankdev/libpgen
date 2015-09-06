
#include <pgen.h>
#include <thread>


pgen_t* handle;
const char* dev = "wlan0";


void synscan(const char* targetIP){
	pgen_tcp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = targetIP;
	pack.TCP.flags.syn = 1;
	pack.TCP.src = 12345;
	
	printf("tcp quick scan. port range 1-100 ....  \n");
	for(pack.TCP.dst=1; pack.TCP.dst<100; pack.TCP.dst++){
		pack.send_handle(handle);
	}
	sleep(10);
	exit(1);
}

bool capture(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(buf.isTCP == false) return true;
	pgen_tcp pack(packet, len);

	if(pack.TCP.flags.ack==1){
		printf(" %d open \n", pack.TCP.src);
	}
	return true;
}

int main(int argc, char** argv){
	if((handle = pgen_open(dev, NULL)) == NULL){
		return -1;
	}

	std::thread scan(synscan, argv[1]);
	sniff(handle, capture);
	scan.join();
}
