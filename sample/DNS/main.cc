#include <pgen.h>
const char* dev = "wlan0";

int main(int argc, char** argv){
	pgen_dns pack;
	
	pack.IP.src = pgen_getIP(dev);
	pack.IP.dst = "192.168.179.1";
	pack.UDP.srcPort = 56112;
	pack.UDP.dstPort = 53;
	pack.DNS.id	   = 0x62cc;
	pack.DNS.flags = 0x0100;
	pack.DNS.qdcnt = 0x0001;
	pack.DNS.ancnt = 0x0000;
	pack.DNS.nscnt = 0x0000;
	pack.DNS.arcnt = 0x0000;
	pack.DNS.name  = "www.slankdev.net";
	pack.DNS.type  = 0x0001;
	pack.DNS.cls   = 0x0001;


	pack.SEND(dev);
	pack.hex();
}
