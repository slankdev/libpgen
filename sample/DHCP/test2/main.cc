#include <pgen.h>
const char* file  = "file.pcap";
const char* wfile = "write.pcap";
pgen_t* whandle;

bool func(const u_char* packet, int len){
	pgen_dhcp pack(packet, len);
	pack.compile();

	pack.summary();
	pack.send_handle(whandle);
	return true;	
}


int main(int argc, char** argv){
	pgen_t* rhandle = pgen_open_offline(file, 0);
	if(rhandle == NULL){
		printf("pgen_open_offline: \n");
		return -1;
	}
	whandle = pgen_open_offline(wfile, 1);
	if(whandle == NULL){
		printf("pgen_open_offline: \n");
		return -1;
	}
	sniff(rhandle, func);
	pgen_close(rhandle);
	pgen_close(whandle);
}
