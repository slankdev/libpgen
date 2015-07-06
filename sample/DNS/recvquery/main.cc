#include <pgen.h>
#include <pcap.h>
#include <sniff.h>
#include <string.h>

const char* dev = "eth0";


bool callback(const bit8* packet, int len){

	pgen_dns p;
	p.CAST(packet, len);

	if(p.IP.protocol != 17)		return true;
	if(!(p.UDP.src == 53))		return true;


	p.INFO();
	return true;

}


int main(int argc, char** argv){
	sniff(dev, callback);
}
