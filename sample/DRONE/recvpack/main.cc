#include <pgen.h>
#include <stdio.h>
#include <sniff.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

const char* dev = "wlan0";



bool callback(const u_char* packet, int len){
	char buf[64];
	pgen_ardrone p;

	printf("start ");
	p.CAST(packet, len);
	printf(" end\n");

	// other packet!!
	if(p.IP.protocol != 17)	return true;
	if(p.UDP.src != 5556)	return true;
	if(p.UDP.dst != 5556)	return true;

	p.DSUMMARY();
	return true;
}




int main(int argc,char** argv){
	sniff(dev, callback);
}
