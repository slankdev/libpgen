#include <pgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* dev = "wlan0";


int main(){
	pgen_tcp pack;
	
	pack.IP.src = "192.168.179.123";
	pack.IP.dst = "192.168.179.1";
	
	pack.TCP.srcPort = 12345;
	pack.TCP.dstPort   = 54321;
	pack.TCP.frag.syn =1;
	pack.TCP.frag.ack =1;
	
	pack.INFO();
	pack.SUMMARY();
}
