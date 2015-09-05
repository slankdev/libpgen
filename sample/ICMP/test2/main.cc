
#include <pgen.h>
const char* file = "file.pcap";
const char* dev  = "wlan0";

int main(int argc, char** argv){
	char data[] = "0123456789";

	pgen_icmp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "192.168.4.1";
	pack.ICMP.type = 8;
	pack.ICMP.code   = 0;
	
	pack.icmp_addData(data, strlen(data));

	pack.summary();
	pack.hex();
	pack.send_L3(dev);
}
