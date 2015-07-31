#include "pgen.h"
#include "netutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netpacket/packet.h>
#include <netinet/if_ether.h>




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
