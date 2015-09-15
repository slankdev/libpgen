#include <stdio.h>
#include <pgen.h>

const char* dev = "en0";
macaddr macsrc;
macaddr macdst = "ff:ff:ff:ff:ff:ff";
ipaddr ipsrc;
ipaddr ipdst = "192.168.179.1";
int portsrc = 23445;
int portdst = 7;

void test_eth();
void test_arp();
void test_ip();
void test_icmp();
void test_tcp();
void test_udp();
void test_dns();
void test_ardrone();

int main(int argc, char** argv){
	macsrc.setmacbydev(dev);
	ipsrc.setipbydev(dev);

	//test_eth();
    //test_arp();
    //test_ip();
    //test_icmp();
    test_tcp();
    //test_udp();
    //test_dns();
    //test_ardrone();
}



void test_eth(){
	pgen_eth p;

	p.ETH.src = macsrc;
	p.ETH.dst = macdst;
	p.ETH.type = 0;
		
	p.info();
	p.send(dev);
}

void test_arp(){
	pgen_arp p;

	p.ETH.src = macsrc;
	p.ETH.dst = "ff:ff:ff:ff:ff:ff";
	p.ARP.srcEth = macsrc;
	p.ARP.srcIp  = ipsrc;
	p.ARP.dstEth = macdst;
	p.ARP.dstIp  = ipdst;
	p.ARP.operation = 1;
		
	p.info();
	p.send(dev);
}

void test_ip(){
	pgen_ip p;
	
	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	
	p.send(dev);
}
void test_icmp(){
	pgen_icmp p;
	
	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.ICMP.type = 8;
	p.ICMP.code   = 0;
	
	p.send(dev);
}
void test_tcp(){
	pgen_tcp p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.TCP.src = portsrc;
	p.TCP.dst = portdst;
	p.TCP.flags.syn = 1;

	p.send(dev);
	p.info();
}
void test_udp(){
	pgen_udp p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.UDP.src = portsrc;
	p.UDP.dst = portdst;

	p.send(dev);
}
void test_dns(){
	pgen_dns p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.UDP.src = portsrc;
	p.UDP.dst = 53;
	
	p.DNS.flags.qr = 0;
	p.DNS.flags.ra = 1;
	p.DNS.ancnt = 1;
	p.DNS.query[0].name  = "slankdev.net";

	p.send(dev);
}
void test_ardrone(){
	pgen_ardrone p;

	p.IP.src = ipsrc;
	p.IP.dst = ipdst;
	p.UDP.src = 5556;
	p.UDP.dst = 5556;

	p.ARDRONE.pcmd.seq = 0;
	p.ARDRONE.pcmd.flag = 0;
	p.ARDRONE.pcmd.roll = 0;
	p.ARDRONE.pcmd.pitch = 0;
	p.ARDRONE.pcmd.gaz = -10000000;
	p.ARDRONE.pcmd.yaw.x = 0;
	p.ARDRONE.pcmd.yaw.y = 0;
	p.ARDRONE.pcmd.yaw.z = 0;
	p.ARDRONE.ref.seq = 0;
	p.ARDRONE.ref.command = 0;

	p.send(dev);
}
