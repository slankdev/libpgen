#include "packet.h"
#include "pgen.h"
#include "pgen-variable.h"

/* include must!!! */
#include "pgen-opcode.h"
#include "pgen-funcs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <netinet/in.h>		/* for struct sockaddr_in */
		

pgen_packet::pgen_packet(){
	clear();
}

void pgen_packet::clear(){
	len = 0;
}


void pgen_packet::hex(){
	unsigned char buf[256];
	memcpy(buf, data, len);

	printf("\nHexDump len: %d\n", len);
	for(int address=0; address<len; address+=0x10){
		printf("%08x     ", address);
		for(int addr=address, i=0; i<0x10; i++, addr++){
			if(addr%8==0 && addr%16!=0)	printf(" ");
			if(address+i > len)
				printf("   ");
			else{
				printf("%02x ", buf[addr] & 0xff);
			}
		}
		printf("\n");
	}printf("\n");
}


void pgen_packet::hexFull(){
	unsigned char buf[256];
	memcpy(buf, data, len);
	
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

