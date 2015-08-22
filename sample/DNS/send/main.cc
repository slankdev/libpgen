#include <pgen.h>
const char* dev = "wlan0";



int main(int argc, char** argv){
	pgen_dns p;

	p.IP.src = "192.168.179.1";;
	p.IP.dst = "192.168.179.7";
	p.UDP.src = 56112;
	p.UDP.dst = 53;
	
	p.DNS.flags.qr = 1;
	p.DNS.flags.ra = 1;
	p.DNS.ancnt = 1;
	p.DNS.query.name  = "slankdev.net";
	
	p.DNS.answer.type = 1;
	p.DNS.answer.cls  = 1;
	p.DNS.answer.ttl  = 5;
	p.DNS.answer.len  = 0x04;
	p.DNS.answer.addr = "123.123.123.123";


	p.INFO();
	for(;;){
		p.SEND(dev);
		sleep(1);
	}
}
