#include <pgen.h>
const char* dev = "lo";



int main(int argc, char** argv){
	pgen_dns pack;
	
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "127.0.0.1";
	pack.UDP.src = 56112;
	pack.UDP.dst = 53;
	
	pack.DNS.flags.qr = 1;
	pack.DNS.flags.ra = 1;
	pack.DNS.query.name  = "slankdev.net";

	pack.SEND(dev);
	pack.INFO();
}
