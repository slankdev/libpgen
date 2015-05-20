#include "pgen.h"

const char* dev = "wlan0";

void tcp();//[[[
void udp();
void ip();
void arp();
void icmp();

macaddr macsrc = pgen_getMAC(dev);
macaddr macdst = "ff:ff:ff:ff:ff:ff";
ipaddr ipsrc = pgen_getIP(dev);
ipaddr ipdst = "192.168.179.1";
int portsrc = 23445;
int portdst = 7;//]]]


int main(){
	arp();
	//ip();
	//icmp();
	//tcp();
	//udp();
}


void arp(){
	pgen_arp p;

	p.ETH.src = macsrc;
	p.ETH.dst = "ff:ff:ff:ff:ff:ff";
	p.ARP.srcEth = pgen_getMAC(dev);
	p.ARP.srcIp  = pgen_getIP(dev);
	p.ARP.srcIp  = "10.128.5.189";
	p.ARP.dstEth = "ff:ff:ff:ff:ff:ff";
	p.ARP.dstIp  = "10.128.7.242";
	p.ARP.option = PGEN_ARPOP_REQEST;
		
	p.SEND(dev);
	p.INFO();
	p.hex();
}


void icmp(){ 
	pgen_icmp p;
	
	p.IP.src = pgen_getIP(dev);
	p.IP.dst = ipdst;
	p.ICMP.option = PGEN_ICMPOP_ECHO;
	p.ICMP.code   = PGEN_ICMPCODE_NET_UNREACH;
	
	p.INFO();
	p.hex();
	for(int i=0; ;i++){
		p.SEND(dev);
		printf("%d: send !!\n",i);
		sleep(0.1);
	}
}





void tcp(){
	pgen_tcp p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.TCP.srcPort = portsrc;
	p.TCP.dstPort = portdst;
	p.TCP.frag.syn = 1;

	p.SEND(dev);
	p.INFO();
	p.hex();
}

void udp(){
	pgen_udp p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.UDP.srcPort = portsrc;
	p.UDP.dstPort = portdst;

	p.SEND(dev);
	p.INFO();
	p.hex();
 }


void ip(){
	pgen_ip p;

	p.IP.src = pgen_getIP(dev);
	p.IP.dst = "192.168.0.1";

	p.SEND(dev);
	p.INFO();
	p.hex();
}


void checkEndian(){
	/*
	printf("BE: %d\n",BIG_ENDIAN);
	printf("LE: %d\n",LITTLE_ENDIAN);
	printf("%d\n",__BYTE_ORDER);
	*/
}
