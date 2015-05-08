#include "pgen.h"

const char* dev = "wlan0";

void tcp2();
void tcp();
void udp();
void ip();
void arp();
void icmp();

macaddr macsrc = pgen_getMAC(dev);
macaddr macdst = "ff:ff:ff:ff:ff:ff";
ipaddr ipsrc = pgen_getIP(dev);
ipaddr ipdst = "10.128.6.24";
int portsrc = 23445;
int portdst = 7;



int main(){

	//arp();
	//ip();
	//icmp();
	tcp();
	//udp();
}



void tcp(){
	pgen_tcp p;

	p.ip_srcIp = ipsrc;
	p.ip_dstIp = ipdst;
	p.TCP.srcPort = portsrc;
	p.TCP.dstPort = portdst;
	p.TCP.frag.syn = 1;

	p.sendPack(dev);
	p.info();
	p.hex();
}

void udp(){
	pgen_udp p;

	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";
	p.udp_srcPort = 8888;
	p.udp_dstPort = 7;
	
	p.sendPack(dev);
	p.info();
	p.hex();
 }

void ip(){
	pgen_ip p;

	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";

	p.sendPack(dev);
	p.info();
	p.hex();
}

void arp(){
	pgen_arp p;

	p.eth_srcEth = pgen_getMAC(dev);
	p.eth_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_srcEth = pgen_getMAC(dev);
	p.arp_srcIp  = pgen_getIP(dev);
	p.arp_dstEth = "ff:ff:ff:ff:ff:ff";
	p.arp_dstIp  = "192.168.0.1";
	p.arp_option = PGEN_ARPOP_REQEST;
		
	p.sendPack(dev);
	p.info();
	p.hex();
}

void icmp(){ 
	pgen_icmp p;
	
	p.ip_srcIp = pgen_getIP(dev);
	p.ip_dstIp = "192.168.0.1";
	p.icmp_option = PGEN_ICMPOP_ECHO;
	p.icmp_code   = PGEN_ICMPCODE_NET_UNREACH;
	
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
