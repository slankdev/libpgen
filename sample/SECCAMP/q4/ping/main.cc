

#include <pgen.h>
#include <pcap.h>
#include <thread>
#include <stdio.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <net/ethernet.h>


const char* dev="wlan0";
const int   sendCount = 5;
int recvCount = 0;


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

	if(tcp->syn && tcp->ack){
		printf("Return SYN,ACK packet from %s:%d ttl=%d\n", 
				inet_ntoa(ip->ip_src), ntohs(tcp->th_sport), (ip->ip_ttl));
		recvCount++;

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


void sendSyn(const char* ipstr){
	pgen_tcp pack;
	pack.IP.dst = ipstr;
	pack.IP.src.setipbydev(dev);
	pack.TCP.frag.syn = 1;
	pack.TCP.srcPort = 7777;
	pack.TCP.dstPort = 80;
	
	for(int i=0; i<sendCount; i++){
		pack.SEND(dev);
		sleep(1);
	}
}



int main(int argc, char** argv){
	if(argc != 2){
		fprintf(stderr, "usage: %s targetIP \n", argv[0]);
		return -1;
	}

	printf("Special PING: Send %d TCP-SYN packets to %s \n", sendCount, argv[1]);


	std::thread s(scan);
	sleep(1);
	sendSyn(argv[1]);
	sleep(2);
	pthread_cancel(s.native_handle());
	s.join();


	printf("---ping statistics---\n");
	printf("%d packet trancemited, %d packet returned, %f%% packet loss\n", 
			sendCount, recvCount, 100*(1.0-((float)recvCount/(float)sendCount)));
}


