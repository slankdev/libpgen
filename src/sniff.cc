#include "pgen.h"
#include "netutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void sniff(const char* dev, bool (*callback)(const u_char*, int)){
	u_char packet[20000];
	bool result = true;
	int len;
	int sock;
	
	if((sock=initRawSocket(dev, 2))<0){
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



void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc){
	u_char packet[20000];
	bool result = true;
	int len;
	int sock;
	
	if((sock=initRawSocket_test(dev, promisc))<0){
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
