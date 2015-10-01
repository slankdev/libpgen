#include <pgen.h>

const char* dev = "en0";

int main(){
	pgen_t* h = pgen_open(dev, NULL);
	if(h == NULL){
		pgen_perror("Oops");
		return -1;
	}

	u_char buf[10000];
	int res = pgen_recv_from_netif(h->fd, buf, sizeof(buf));
	if(res < 0){
		printf("error\n");
		return -1;
	}

	hexdump(buf, res);

}
