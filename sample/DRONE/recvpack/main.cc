#include <pgen.h>
#include <stdio.h>
#include <sniff.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <net/ethernet.h>

const char* dev = "lo";



bool callback(const u_char* packet, int len){
	struct ether_header* eth;
	struct ip* ip;
	struct udphdr* udp;
	char buf[64];
	pgen_ardrone p;
	
	p.ETH.CAST(packet);
	packet += sizeof(struct ether_header);
	len -= sizeof(struct ether_header);
	p.IP.CAST(packet);
	packet += sizeof(struct ip);
	len -= sizeof(struct ip);
	p.UDP.CAST(packet);
	packet += sizeof(struct udphdr);
	len -= sizeof(struct udphdr);
	

	// other jpacket!!
	if(p.ETH.type != ETHERTYPE_IP)	return true;
	if(p.IP.type != IPPROTO_UDP)	return true;
	if(p.UDP.src != 5556)			return true;
	if(p.UDP.dst != 5556)			return true;

		
	for(int i=0; i<len; i++){
		if(packet[i] == 0x0d)	buf[i] = '.';
		else					buf[i] = packet[i];
	}buf[len] = '\0';


	sscanf(buf, "AT*PCMD_MAG=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld.AT*REF=%ld,%ld.",
			&p.ARDRONE.pcmd.seq, &p.ARDRONE.pcmd.flag, &p.ARDRONE.pcmd.roll,
			&p.ARDRONE.pcmd.pitch, &p.ARDRONE.pcmd.gaz, &p.ARDRONE.pcmd.yaw.x,
			&p.ARDRONE.pcmd.yaw.y, &p.ARDRONE.pcmd.yaw.z,
			&p.ARDRONE.ref.seq, &p.ARDRONE.ref.command );

	p.SUMMARY();
	
	return true;
}



int main(int argc,char** argv){
	sniff(dev, callback);
}
