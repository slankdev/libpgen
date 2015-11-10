# PGEN-Core

PGEN-Core is the most important component of LibPGEN.
This component has **address-class** and **packet-class**.   
Address-class gives users easy interface about some protocols address operations.
Packet-class gives users easy interface too. This class is easily extensible for you.



## Address class
LibPGEN support IP address and Mac address.   
Address class has some utility operator for size comparison, increment and more.

### Sample Code

	#include <pgen.h>

	int main(){
		ipaddr addr = "192.168.0.1";
		ipaddr mask = "255.255.252.0";
		ipaddr net;
		
		net.setnetbyaddr(addr, mask);
		for(ipaddr a=addr; a.isSameSegment(addr,mask); a++)
			printf("%s \n", a.c_str());
	}



## Packet class
We support Ethernet, ARP, IP, ICMP, TCP and UDP by default.
You can extension new protocols easily. If you want extend protocols, see http://libpgen.org .

### Sample Code


	#include <pgen.h>

	int main(){
		pgen_t* handle = pgen_open("wlan0", NULL);
		if(handle == NULL){
			pgen_perror("oops");
			return -1;
		}

		pgen_arp packet;
		packet.ETH.src       = "00:11:22:33:44:55";
		packet.ETH.dst       = "aa:bb:cc:dd:ee:ff";
		packet.ETH.type      = 0x0806;
		packet.ARP.operation = 2;
		packet.ARP.srcEth    = "77:77:77:77:77:77";
		packet.ARP.srcIp     = "192.168.10.11";
		packet.ARP.dstEth    = "aa:bb:cc:dd:ee:ff";
		packet.ARP.dstIp     = "192.168.10.1";
		
		packet.send_handle(handle);
		pgen_close(handle); 
	}


