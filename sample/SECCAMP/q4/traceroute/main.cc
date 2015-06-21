#include <pgen.h>
#include <pcap.h>
#include <thread>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>


const char* dev = "wlan0";
const int targetPort = 80;
const int minTTL = 1;
const int maxTTL = 30;
char targetIP[256];

std::vector<int> vec;


void callback(u_char* data, const struct pcap_pkthdr *hdr, const u_char* pack){
	struct ether_header* eth;
	struct ip* ip;
	struct tcphdr* tcp;
	char str[256];

	eth = (struct ether_header*)pack;
	pack += sizeof(struct ether_header);
	ip = (struct ip*)pack;
	pack += sizeof(struct ip);
	tcp = (struct tcphdr*)pack;

	if(ntohs(eth->ether_type) != ETHERTYPE_IP || ip->ip_p != IPPROTO_TCP)
		return;
	if(strcmp(inet_ntoa(ip->ip_src), targetIP) != 0)
		return;


	if(tcp->syn && tcp->ack){
		printf("Return [SYN,ACK] from %15s ttl=%3d hop>=%2d \n", 
				inet_ntoa(ip->ip_src),  (ip->ip_ttl), ntohs(tcp->th_dport)/1000);
		vec.push_back(ntohs(tcp->th_dport)/1000);
	}
}


void scan(){
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* handle;

	handle = pcap_open_live(dev, 0, 0, 1000, errbuf);
	if(handle == NULL){
		perror("pcap open live");
		exit(-1);
	}
	pcap_loop(handle, 0, callback, NULL);
	pcap_close(handle);
}

void sendpack(const char *ip){
	pgen_tcp pack;
	pack.IP.src.setipbydev(dev);
	pack.IP.dst = ip;
	pack.TCP.frag.syn = 1;
	pack.TCP.dstPort = targetPort;
	
	for(int i=minTTL; i<=maxTTL; i++){
		pack.TCP.srcPort = i*1000;
		pack.IP.ttl = i;
		pack.SEND(dev);
	}
}

int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "usage: %s targetIP \n", argv[0]);
		return -1;
	}
	strcpy(targetIP, argv[1]);
	
	printf("SP Traceroute to %s, %d hosp max \n", targetIP, maxTTL);
	printf("This program coded with libpgen(slankdev.net/libpgen/)\n");
	
	std::thread s(scan);
	sleep(1);
	sendpack(targetIP);
	sleep(10);
	pthread_cancel(s.native_handle());
	s.join();
	
	int min = *min_element(vec.begin(), vec.end());
	printf("traceroute: %d\n", min);
}

