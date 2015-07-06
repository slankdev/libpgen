#include <pgen.h>
const char* dev = "eth0";



int main(int argc, char** argv){
	pgen_dns pack;
	
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "127.0.0.1";
	pack.IP.dst = "192.168.117.2";
	pack.UDP.src = 56112;
	pack.UDP.dst = 53;
	
	pack.DNS.flags.qr = 0;
	pack.DNS.flags.ra = 1;
	pack.DNS.query.name  = "slankdev.net";

	pack.INFO();
	for(int i=0;; i++){
		pack.SEND(dev);
		sleep(1);
	}
}
