#include "pgen.h"

const char* dev = "wlan0";

void tcp();
void udp();
void ip();
void arp();
void icmp();

macaddr macsrc = pgen_getMAC(dev);
macaddr macdst = "ff:ff:ff:ff:ff:ff";
ipaddr ipsrc = pgen_getIP(dev);
ipaddr ipdst = "192.168.101.27";
int portsrc = 23445;
int portdst = 7;


int main(){

	//arp();
	//ip();
	//icmp();
	//tcp();
	udp();
}

void icmp(){ 
	pgen_icmp p;
	
	p.IP.src = pgen_getIP(dev);
	p.IP.dst = ipdst;
	p.ICMP.option = PGEN_ICMPOP_ECHO;
	p.ICMP.code   = PGEN_ICMPCODE_NET_UNREACH;
	
	p.sendPack(dev);
	p.info();
	p.hex();
}


void arp(){
	pgen_arp p;

	p.ETH.src = macsrc;
	p.ETH.dst = "ff:ff:ff:ff:ff:ff";
	p.ARP.srcEth = macsrc;
	p.ARP.srcIp  = ipsrc;
	p.ARP.dstEth = "ff:ff:ff:ff:ff:ff";
	p.ARP.dstIp  = ipdst;
	p.ARP.option = PGEN_ARPOP_REQEST;
		
	p.sendPack(dev);
	p.info();
	p.hex();
}


void tcp(){
	pgen_tcp p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.TCP.srcPort = portsrc;
	p.TCP.dstPort = portdst;
	p.TCP.frag.syn = 1;

	p.sendPack(dev);
	p.info();
	p.hex();
}

void udp(){
	pgen_udp p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.UDP.srcPort = portsrc;
	p.UDP.dstPort = portdst;

	p.sendPack(dev);
	p.info();
	p.hex();
 }


void ip(){
	pgen_ip p;

	p.IP.src = pgen_getIP(dev);
	p.IP.dst = "192.168.0.1";

	p.sendPack(dev);
	p.info();
	p.hex();
}


void checkEndian(){
	/*
	printf("BE: %d\n",BIG_ENDIAN);
	printf("LE: %d\n",LITTLE_ENDIAN);
	printf("%d\n",__BYTE_ORDER);
	*/
}
