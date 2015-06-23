#include <pgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* dev = "wlan0";


int main(){
	pgen_icmp pack;
	
	pack.IP.src = "192.168.179.123";
	pack.IP.dst = "192.168.179.1";
	pack.ICMP.option = 0;
	pack.ICMP.code = 0;
	
	pack.INFO();
	pack.SUMMARY();
}
