

/*
 * copyright (C) <2015>  <Slank Hiroki Shirokura>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */


#include <stdio.h>
#include "debug.h"
//#include "pgen.h"




void debugprint(int flag, const char* str){
	if(flag)
		printf("%s", str);
}




void hexdump(const u_char* data, int len){
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





