


#include <pgen.h>

int c = 0;


int check(const char* file, pgen_packet* pack){
	int count  = 0;
	int result = 0;
	int len;
	u_char buf[10000];

	pgen_t* h = pgen_open_offline(file, 0);
	if(h == NULL){
		pgen_perror("pgen_open_offline");
		return -1;
	}
	
	while((len = pgen_recv_from_pcap(h->offline.fd, buf, sizeof(buf))) >= 0){
		pack->cast(buf, len);
		pack->compile();

		if(len != pack->len){
			printf("length isn't same \n");	
			result++;
		}else if((memcmp(pack->data, buf, len)!=0)){
			printf("binary isn't same \n");
			pack->hex();
			pgen_hex(buf, len);
			result++;
		}else{
			printf("OK \n");

		}
		count++;
	}

	printf("%d/%d is false \n", result, count);
	pgen_close(h);
	return result;
}




int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s filename \n", argv[0]);
		return -1;
	}
	pgen_arp pack;
	check(argv[1], &pack);
}
