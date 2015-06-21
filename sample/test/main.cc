#include <pgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* dev = "wlan0";


int main(){
	pgen_tcp pack;
	
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = "10.128.7.167";
	pack.TCP.srcPort = 23456;
	pack.TCP.dstPort = 8080;
	pack.TCP.frag.syn = 1;
	pack.TCP.frag.ack = 1;

	pack.INFO();
	pack.SEND(dev);
}
