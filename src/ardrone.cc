#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"


pgen_ardrone::pgen_ardrone(){
	CLEAR();
}


void pgen_ardrone::CLEAR(){
	pgen_udp::CLEAR();

	ARDRONE.pcmd_mag.seq   = 0;
	ARDRONE.pcmd_mag.flag  = 0;
	ARDRONE.pcmd_mag.roll  = 0;
	ARDRONE.pcmd_mag.pitch = 0;
	ARDRONE.pcmd_mag.gaz   = 0;
	ARDRONE.pcmd_mag.yaw.x = 0;
	ARDRONE.pcmd_mag.yaw.y = 0;
	ARDRONE.pcmd_mag.yaw.z = 0;
	ARDRONE.ref.seq        = 0;
	ARDRONE.ref.command    = 0;
}



void pgen_ardrone::SEND(const char *ifname){
	WRAP();		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;

	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(-1);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}


void pgen_ardrone::WRAP(){
	pgen_udp::WRAP();

	char cmd1[64];
	char cmd2[64];
	char *p1;
	char spliter = 0x0d;

	sprintf(cmd1, "AT*PCMD_MAG=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld", 
			ARDRONE.pcmd_mag.seq, ARDRONE.pcmd_mag.flag, 
			ARDRONE.pcmd_mag.roll, ARDRONE.pcmd_mag.pitch, 
			ARDRONE.pcmd_mag.gaz, ARDRONE.pcmd_mag.yaw.x, 
			ARDRONE.pcmd_mag.yaw.y, ARDRONE.pcmd_mag.yaw.z);
	
	sprintf(cmd2, "AT*REF=%ld,%ld", 
			ARDRONE.ref.seq, ARDRONE.ref.command);

	p1 = cmd;
	memcpy(p1, cmd1, strlen(cmd1));
	p1 += strlen(cmd1);
	memcpy(p1, &spliter, 1);
	p1 += 1;
	memcpy(p1, cmd2, strlen(cmd2));
	p1 += strlen(cmd2);
	memcpy(p1, &spliter, 1);
	p1 += 1;
	clen = p1 - cmd;

	udp.len = htons(ntohs(udp.len) + clen);

	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	memcpy(p, cmd, clen);
	p += clen;
	len = p-data;
}


void pgen_ardrone::INFO(){
	WRAP();
	pgen_udp::INFO();
	
	printf(" * AR Drone packet\n");
	printf("    - PCMD MAG\n");
	printf("         Sequence Num : %ld \n", ARDRONE.pcmd_mag.seq);
	printf("         Flag         : %ld \n", ARDRONE.pcmd_mag.flag);
	printf("         Roll         : %ld \n", ARDRONE.pcmd_mag.roll);
	printf("         Pitch        : %ld \n", ARDRONE.pcmd_mag.pitch);
	printf("         Gaz          : %ld \n", ARDRONE.pcmd_mag.gaz);
	printf("         Yaw(x,y,z)   : (%ld,%ld,%ld)  \n", 
			ARDRONE.pcmd_mag.yaw.x, ARDRONE.pcmd_mag.yaw.y,
			ARDRONE.pcmd_mag.yaw.z);
	printf("    - REF\n");
	printf("         Sequence Num : %ld \n", ARDRONE.ref.seq);
	printf("         Command      : %ld \n", ARDRONE.ref.command);
	
}





void pgen_ardrone::SUMMARY(){
	WRAP();
	printf("AR Drone packet seq=%ld roll=%ld pitch=%ld gaz=%ld yaw=(%ld,%ld,%ld)\n", 
			ARDRONE.pcmd_mag.seq, ARDRONE.pcmd_mag.roll, 
			ARDRONE.pcmd_mag.pitch, ARDRONE.pcmd_mag.gaz,
			ARDRONE.pcmd_mag.yaw.x, ARDRONE.pcmd_mag.yaw.y,
			ARDRONE.pcmd_mag.yaw.x);
}


void pgen_ardrone::_printdata(){
	for(int i=0; i<clen; i++){
		if(cmd[i] == 0x0d)
			printf(".");
		else
			printf("%c", cmd[i]);
	}printf("\n");
}
