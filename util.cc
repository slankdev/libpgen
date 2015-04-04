#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <vector>
#include <sys/types.h>
#include <unistd.h>


char* getbenderbymac(const u_char data[6]){
	FILE *fp;
	const char* filename = "mac_code.list";
	unsigned int  mac[3];
	char* bender;
	char buf[256];
	bender = (char*)malloc(sizeof(char) * (strlen(buf)+1));
	u_char dev_mac[3] = {data[0],data[1],data[2]};

	if((fp=fopen(filename, "r")) == NULL){
		perror("getbenderbymac fopen");
		strcpy(bender, "error");
		return bender;
	}
	while(fgets(buf, sizeof(buf), fp) != NULL){
		sscanf(buf, "%2x%2x%2x\t%s", &mac[0],&mac[1],&mac[2],buf);
		if(mac[0]==dev_mac[0]&&mac[1]==dev_mac[1]&&mac[2]==dev_mac[2]){
			snprintf(bender, strlen(buf), "%s", buf);
			return bender;
		}
		memset(mac, 0, sizeof(mac));
		memset(bender, 0, sizeof(bender));
		memset(buf, 0, sizeof(buf));
	}
	fclose(fp);
	
	strcpy(bender, "not-found");
	return bender;
}




void printIpaddr( unsigned int* addr){
	union lc{
		unsigned int l;
		unsigned char c[4];
	};
	union lc *lc = (union lc*)addr;
	for(int i=0; i<4; i++){
		printf("%d", lc->c[i]);
		if(i<3)		printf(".");
		else		printf("\n");
	}
}





void hexdump(const void* p, int len){
	unsigned char buf[256];
	memcpy(buf, p, len);
	
	printf("\nHexDump len: %d\n", len);
	for(int address=0; address<len; address+=0x10){
		printf("%08x     ", address);
		for(int addr=address, i=0; i<0x10; i++, addr++){
			if(addr%8==0 && addr%16!=0)	printf(" ");
			if(address+i > len)
				printf("   ");
			else
				printf("%02x ", buf[addr] & 0xff);
		}
		printf("   ");
		for(int addr=address, i=0; i<0x10; i++, addr++){
			if(addr%0X08==0 && addr%0x10!=0)	printf(" ");
			if(address+i > len)
				printf(" ");
			else{
				if(buf[addr] < 0x20 || 0x7e < buf[addr])
					buf[addr] = '.';
				printf("%c ", buf[addr]);
			}
		}printf("\n");
	}printf("\n");
}



unsigned short checksum(const void* data, int len){
	unsigned long sum = 0;
	unsigned short* buf = (unsigned short*)data;

	while (len > 1) {
		sum += *buf;
		buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;

	sum = (sum & 0xffff) + (sum >> 16);
	sum = (sum & 0xffff) + (sum >> 16);
	return ~sum;
}






char* gettimestr(){
	char* str;
	time_t timer = time(NULL);
	struct tm *time;

	str = (char*)malloc(sizeof(char) * 256);
	memset(str, 0, sizeof str);
	time = localtime(&timer);
	strftime(str, 255, "%H:%M:%S", time);
	return str;
}





