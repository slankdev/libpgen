
#include <pgen.h>
#include <thread>

pgen_t* handle;
ipaddr  target;
const char* dev = "en0";


void synscan(){
	arptable ap;

	pgen_tcp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "a2:12:42:17:d8:8f";
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = target;
	pack.TCP.flags.syn = 1;
	
	
	pack.TCP.src = (rand() % 50000) + 10000;
	pack.TCP.dst = 80;
	pack.send_handle(handle);
	
	/*
	pack.TCP.src = (rand() % 50000) + 10000;
	pack.TCP.dst = 53;
	pack.send_handle(handle);
	*/
}

bool capture(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(buf.isTCP && buf.ipaddris(target)){
		pgen_tcp pack(packet, len);
		if(pack.TCP.flags.ack==1){
			printf(" %d open \n", pack.TCP.src);
		}
	}
	return true;
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("usage: %s target \n", argv[0]);
		return -1;
	}
	target = argv[1];

	handle = pgen_open(dev, NULL);
	if(handle == NULL){
		pgen_perror("oops");
		return -1;
	}

	std::thread scan(synscan);
	sniff(handle, capture);
	scan.join();

	pgen_close(handle);
}
