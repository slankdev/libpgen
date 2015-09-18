#include <pgen.h>

const char* dev = "en0";

bool func(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	buf.summary();
	return true;
}



int main(){
	pgen_t* h = pgen_open(dev, NULL);
	if(h == NULL){
		printf("error\n");
		return -1;
	}

	sniff(h, func);
}
