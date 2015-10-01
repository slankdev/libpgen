#include <pgen.h>

const char* dev = "en0";
const char* fil = "test.pcap";

int main(){
	pgen_t* h = pgen_open(dev, NULL);
	if(h == NULL){
		pgen_perror("Oops");
		return -1;
	}
	pgen_t* w = pgen_open_offline(fil, 1);
	if(w == NULL){
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
	pgen_send_to_netif(h->fd, buf, res);
}
