#include "pgen.h"
#include "netutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void sniff_with_filter(const char* dev, bool (*callback)(const u_char*, int), 
										int promisc, struct sniff_filter* filter){
	u_char packet[20000];
	bool result = true;
	int len;
	int sock;
	pgen_unknown buf;
	
	if((sock=initRawSocket(dev, promisc, 0))<0){
		perror("sniff");
		return;
	}

	for(;result;){
		if((len = read(sock, packet, sizeof(packet))) < 0){
			perror("read");
			return;
		}

		buf.CAST(packet, len);

		// read filter
		if(filter->flag.ipsrc){
			if(buf.IP.src != filter->ipsrc)	return;
		}
		if(filter->flag.ipdst){
			if(buf.IP.dst != filter->ipdst)	return;
		}
		if(filter->flag.ethsrc){
			if(buf.ETH.src != filter->ethsrc)	return;
		}
		if(filter->flag.ipdst){
			if(buf.ETH.dst != filter->ethdst)	return;
		}

		result = (*callback)(packet, len);
	}
}




void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc){
	u_char packet[20000];
	bool result = true;
	int len;
	int sock;
	
	if((sock=initRawSocket(dev, promisc, 0))<0){
		perror("sniff");
		return;
	}

	for(;result;){
		if((len = read(sock, packet, sizeof(packet))) < 0){
			perror("read");
			return;
		}
		result = (*callback)(packet, len);
	}
}
