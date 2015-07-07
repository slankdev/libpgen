#include <pgen.h>
#include <pcap.h>
#include <sniff.h>
#include <string.h>

const char* dev = "wlan0";

bool callback(const bit8* packet, int len){
	pgen_dns recv;
	debug("begin function");
	recv.CAST(packet, len);
	debug("fin   function");

	if(recv.IP.protocol != 17)		return true;
	if(!( recv.UDP.dst == 53 || recv.UDP.src == 53))return true;
	
	

	recv.SUMMARY();
	return true;

	ipaddr tmp;
	tmp = recv.IP.src;
	recv.IP.src  = recv.IP.dst;
	recv.IP.dst  = tmp;

	recv.UDP.src = recv.UDP.dst;
	recv.UDP.dst = recv.UDP.src;
	
	recv.DNS.flags.qr = 1;
	recv.DNS.flags.ra = 1;
	recv.DNS.ancnt    = 1;
	recv.DNS.query.name = recv.DNS.query.name;
	recv.DNS.answer.type = 1;
	recv.DNS.answer.cls  = 1;
	recv.DNS.answer.ttl  = 1;
	recv.DNS.answer.len  = 1;
	recv.DNS.answer.addr = "141.0.174.35";

	recv.SUMMARY();

	for(int i=0; i<1000; i++){
		//recv.SEND(dev);
	}
	
	return true;
}


int main(int argc, char** argv){
	sniff(dev, callback);
}
