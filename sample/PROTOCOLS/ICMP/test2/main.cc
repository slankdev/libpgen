
#include <pgen.h>
const char* file = "file.pcap";
const char* dev  = "wlan0";

int main(int argc, char** argv){
	pgen_dns _pack;
	char data[] = "0123456789";

	pgen_icmp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst = "ff:ff:ff:ff:ff:ff";
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "192.168.4.1";
	pack.ICMP.type = 5;
	pack.ICMP.code   = 0;
	pack.ICMP.redirect.gw_addr = "123.123.123.123";
	
	_pack.compile();
	pack.icmp_addData(_pack.data+ETH_HDR_LEN, _pack.len-ETH_HDR_LEN);

	pack.summary();
	pack.hex();
	pack.send_L3(dev);
}
