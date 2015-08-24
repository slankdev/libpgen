
#include "packet.h"
#include "pgen.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>






pgen_packet::pgen_packet(){
	len = 0;
	ext_data_len =0;
	memset(data, 0, sizeof(data));
	memset(ext_data, 0, sizeof(ext_data));
}




void pgen_packet::compile_addData(){
	if(ext_data_len == 0) return;
	if(ext_data_len+len >= PGEN_MAX_PACKET_LEN){
		fprintf(stderr, "pgen_packet::compile_addData(): packet length is too large\n");
	}

	u_char* p = this->data + this->len;
	memcpy(p, ext_data, ext_data_len);
	this->len += ext_data_len;
}




void pgen_packet::addData(const u_char* byte, int len){
	if(len > PGEN_MAX_EXT_DATA_LEN){
		fprintf(stderr, "pgen_packet::addData(): Extension Data is too long\n");
		return;
	}
	
	memcpy(ext_data, byte, len);
	ext_data_len = len;
	
	return;	
}




void pgen_packet::hex(){
	compile();
	printf("hexdump len: %d \n", len);

	int row=0;
	int column=0;
	for(row=0; (row+column)<16+len; row+=16){
		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf("   ");
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
			}else{
				if((row+column)%16 == 0) printf("%04x:    ", row+column);
				if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
				printf("%02x ", data[row+column]);
			}
		}

		for(column=0; column<=15; column++){
			if(!(row+column < len)){
				printf(" ");
			}else{
				if((row+column)%16 == 0) 
					printf("  ");
				if((row+column)%8 == 0 && (row+column)%16!=0) 
					printf("  ");
				
				if(0x20<=data[row+column] && data[row+column]<=0x7E)
					printf("%c", data[row+column]);
				else
					printf(".");
				
				if((row+column+1)%16 == 0)	
					printf("\n");
			}
		}
	}
	printf("\n");
}




