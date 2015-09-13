#include <pgen.h>

static int c =0;
const char* writefile = "write.pcap";
pgen_t* w;

bool func(const u_char* buf, int len){
	c++;
	pgen_unknown pack(buf, len);
	pack.summary();
	pack.send_handle(w);
	return true;
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s filename \n", argv[0]);
		return -1;
	}

	w = pgen_open_offline(writefile, 1);
	if(w == NULL){
		printf("w error\n");
		return -1;
	}
	pgen_t* h = pgen_open_offline(argv[1], 0);
	if(h == NULL){
		printf("h error\n");
		return -1;
	}
	sniff(h, func);
	pgen_close(h);

	printf("%d packets \n", c);

}
