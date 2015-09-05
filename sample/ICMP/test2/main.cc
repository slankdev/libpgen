
#include <pgen.h>
const char* file = "file.pcap";

int main(int argc, char** argv){
	char data[] = "0123456789";

	pgen_icmp pack;
	pack.IP.src = "192.168.100.1";
	pack.IP.dst = "192.168.100.2";
	pack.ICMP.option = 8;
	pack.ICMP.code   = 0;
	pack.addData(data, sizeof(data));

	pack.summary();
	pack.hex();

}
