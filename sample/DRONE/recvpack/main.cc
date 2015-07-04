#include <pgen.h>
#include <stdio.h>
#include <sniff.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

const char* dev = "lo";



bool callback(const u_char* packet, int len){
	char buf[64];
	pgen_ardrone p;
	
	p.CAST(packet, len);
	packet += sizeof(struct ether_header);
	len -= sizeof(struct ether_header);
	packet += sizeof(struct ip);
	len -= sizeof(struct ip);
	packet += sizeof(struct udphdr);
	len -= sizeof(struct udphdr);
	

	// other packet!!
	if(p.IP.protocol != 17)	return true;
	if(p.UDP.src != 5556)	return true;
	if(p.UDP.dst != 5556)	return true;


	p.SUMMARY();
	return true;
}



int main(int argc,char** argv){
	sniff(dev, callback);
}
