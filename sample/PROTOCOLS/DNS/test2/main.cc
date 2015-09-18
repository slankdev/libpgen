#include <stdio.h>
#include <pgen.h>
const char* file = "file.pcap";


bool func(const u_char* packet, int len){
	pgen_dns buf(packet, len);

	buf.summary();
	
	return true;
}


int main(int argc, char** argv){
	pgen_t* handle = pgen_open_offline(file, 0);
	if(handle == NULL){
		return -1;	
	}
	sniff(handle, func);
}
