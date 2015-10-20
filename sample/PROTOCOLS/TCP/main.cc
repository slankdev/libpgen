#include <pgen.h>

const char* dev = "en0";

int main(){
	pgen_t* handle = pgen_open("en0", NULL);
	if(handle == NULL){
		pgen_perror("oops");
		return -1;
	}
	

	pgen_tcp pack;
	pack.ETH.src.setmacbydev("en0");
	pack.ETH.dst = "a2:12:42:17:d8:8f";
	pack.IP.src = "192.168.179.2";
	pack.IP.dst = "192.168.179.1";
	pack.IP.tot_len = 44;
	pack.IP.protocol = 6;
	pack.TCP.src = 48529;
	pack.TCP.dst = 80;
	pack.TCP.window = 1024;
	pack.TCP.flags.syn = 1;
	
	pack.add_data("1234", 4);
	pack.hex();

	pack.send_handle(handle);

}
