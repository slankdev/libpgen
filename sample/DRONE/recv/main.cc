#include <pgen.h>
#include <stdio.h>
#include <sniff.h>

const char* dev = "wlan0";


bool callback(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	pgen_udp udp;
	pgen_ardrone pack;

	if(buf.isUDP()){
		udp.CAST(packet, len);
		if(udp.UDP.src == 5556){
			pack.CAST(packet, len);	
			pack.DSUMMARY();
		}
	}
	return true;
}

int main(int argc,char** argv){
	sniff(dev, callback);
}
