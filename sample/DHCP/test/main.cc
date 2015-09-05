#include <pgen.h>
const char* file = "file.pcap";
const char* dev  = "wlan0";

int main(int argc, char** argv){
	pgen_t* handle = pgen_open_offline(file, 1);
	if(handle == NULL){
		printf("pgen_open_offline: \n");
		return -1;
	}

	pgen_dhcp pack;
	pack.ETH.src.setmacbydev(dev);
	pack.ETH.dst.setmacbroadcast();
	pack.IP.src = 0;
	pack.IP.dst = "255.255.255.255";
	pack.UDP.src = 68;
	pack.UDP.dst = 67;
	pack.DHCP.chaddr.setmacbydev(dev);
	
	pack.DHCP.option_len = 7;

	char buf0[] = {0x01};
	char buf1[] = {0xff,0xff,0xff,0x00};
	char buf2[] = {0x00,0x00,0x07,0x08};
	char buf3[] = {0x00,0x00,0x0c,0x4e};
	char buf4[] = {0x00,0x00,0x0e,0x10};
	char buf5[] = {0xc0,0xa8,0x00,0x01};
	char buf6[] = {0x00,0x00,0x00,0x00};
	pack.dhcp_set_option(0, 53, 1, buf0);
	pack.dhcp_set_option(1, 1, 4, buf1);
	pack.dhcp_set_option(2, 58, 4, buf2);
	pack.dhcp_set_option(3, 59, 4, buf3);
	pack.dhcp_set_option(4, 51, 4, buf4);
	pack.dhcp_set_option(5, 54, 4, buf5);
	pack.dhcp_set_option(6, 255, 4, buf6);

	pack.summary();
	pack.hex();
	pack.send_handle(handle);
	pgen_close(handle);

}
