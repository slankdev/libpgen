#include <pgen.h>
const char* dev = "wlan0";



int main(int argc, char** argv){
	pgen_dns pack;
	
	



	pack.IP.src = "192.168.179.1";;
	pack.IP.dst = "192.168.179.7";
	pack.UDP.src = 56112;
	pack.UDP.dst = 53;
	
	pack.DNS.flags.qr = 1;
	pack.DNS.flags.ra = 1;
	pack.DNS.ancnt = 1;
	pack.DNS.query.name  = "slankdev.net";
	
	pack.DNS.answer.type = 1;
	pack.DNS.answer.cls  = 1;
	pack.DNS.answer.ttl  = 5;
	pack.DNS.answer.len  = 0x04;
	pack.DNS.answer.addr = "123.123.123.123";


	pack.INFO();
	for(;;){
		pack.SEND(dev);
		sleep(1);
	}
}
