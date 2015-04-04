#include "pgen.h"



int main(){
	pgent_mac mac;
	pgent_ip ip;
	mac	= pgen_getMAC("eth0");
	ip  = pgen_getIP("eth0");

	printf("%s %s\n", mac.c_str(), mac.bender());
	printf("%s \n", ip.c_str());
	printf("ip[2]: %u\n", ip[1]);
	
}
