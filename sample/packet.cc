
#include <pgen2.h>


int main() {

	pgen::arp pack;

	pack.ARP.operation = pgen::arp::reply;
	pack.ARP.hwsrc = "22:22:22:22:22:22";
	pack.ARP.psrc  = "192.168.0.2";
	pack.ARP.hwdst = "11:11:11:11:11:11";
	pack.ARP.pdst  = "192.168.0.1";
	pack.ETH.src   = "22:22:22:22:22:22";
	pack.ETH.dst   = "11:11:11:11:11:11";
	
	pack.summary();
	pack.hex();

	try {
		pgen::pgen_t handle(filename, PCAP_WRITE);
		size_t sendlen = handle.send(pack);
	} catch (std::exception e) { 	
		printf("err: %s \n", e.what());
	}
}
