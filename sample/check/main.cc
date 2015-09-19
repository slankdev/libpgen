#include <pgen.h>
const char* dev = "en0";
const char* file = "out.bin";
const char* src_mac = "66:77:88:99:aa:bb";
const char* dst_mac = "00:11:22:33:44:55";
const char* src_ip  = "";
const char* dst_ip  = "";

int test_eth();
int test_arp();
int test_ip();
int test_icmp();
int test_tcp();
int test_udp();
int test_dns();
int test_dhcp();
int test_ardrone();



int main(int argc, char** argv){
	test_eth();
}


int test_eth(){
	pgen_eth pack;
	pack.ETH.src  = src_mac;
	pack.ETH.dst  = dst_mac;
	pack.ETH.type = 0xccdd;
	
	pack.hex();
	pack.cat(file);

	return 1;
}
