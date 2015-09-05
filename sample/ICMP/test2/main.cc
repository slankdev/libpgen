
#include <pgen.h>
const char* file = "file.pcap";

int main(int argc, char** argv){
	pgen_icmp pack;
	pack.IP.src = "192.168.100.1";
	pack.IP.dst = "192.168.100.2";
	pack.ICMP.option = 8;
	pack.ICMP.code   = 0;


	pack.summary();
	pack.hex();
}
