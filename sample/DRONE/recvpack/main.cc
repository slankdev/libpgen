#include "pgen.h"
#include <stdio.h>
#include <pcap.h>
#include <thread>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

const char* dev = "wlan0";

void capture();


int main(int argc,char** argv){
	capture();
}


void callback(u_char* data, const struct pcap_pkthdr *header,
									const u_char* packet){
	struct ether_header* eth;
	struct ip* ip;
	struct udphdr* udp;
	char buf[64];

	eth = (struct ether_header*)packet;
	packet += sizeof(struct ether_header);
	ip  = (struct ip*)packet;
	packet += sizeof(struct ip);
	udp = (struct udphdr*)packet;
	packet += sizeof(struct udphdr);


	if(ip->ip_p!=17 || ntohs(udp->uh_sport)!=5556 
			|| ntohs(udp->uh_dport)!=5556){
		//printf("other packet!\n");
		return ;
	}

	printf("%s\n", packet);
}

void capture(){
	pcap_t* handle;
	char errbuf[PCAP_ERRBUF_SIZE];

	handle = pcap_open_live(dev, 66536, 1,10,errbuf);
	if(handle == NULL){
		fprintf(stderr, "%s\n", errbuf);
		return;
	}
	
	if(pcap_loop(handle, 0, callback, NULL) < 0){
		fprintf(stderr, "%s\n", pcap_geterr(handle));
		pcap_close(handle);
		return;
	}

	pcap_close(handle);
	return;
}
