#include <pgen.h>
const char* dev="en0";

int main(){
	ipaddr addr;
	addr.setipbydev(dev);
	printf("%s: %s\n", dev, addr.c_str());
	
	macaddr haddr;
	haddr.setmacbydev(dev);
	printf("%s: %s\n", dev, haddr.c_str());
}
