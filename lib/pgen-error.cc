

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


#include "pgen-error.h"
#include <stdio.h>
#include <string.h>


#define PGEN_ERRSTR_LEN 256

int pgen_errno=0;
int pgen_errno2=0;
char pgen_errstr[PGEN_ERRSTR_LEN]={0};





char* pgen_strerror(int errno){
	strncpy(pgen_errstr, strerror(errno), PGEN_ERRSTR_LEN);

	return pgen_errstr;	
}




void pgen_perror(const char* msg){
	char str[256];

	switch(pgen_errno2){
		case PG_ERRNO_SOCKET:
			strncpy(str, "socket", 255);
			break;
		case PG_ERRNO_BIND:
			strncpy(str, "bind", 255);
			break;
		case PG_ERRNO_HDRINC: 
			strncpy(str, "setsockopt hdrinc", 255);
			break;
		case PG_ERRNO_PROMISC:
			strncpy(str, "set promisc", 255);
			break;

#ifndef __linux
		case PG_ERRNOBSD_OPENBPF:
			strncpy(str, "open bpf", 255);
			break;
		case PG_ERRNOBSD_SETBUF:
			strncpy(str, "set buf", 255);
			break;
		case PG_ERRNOBSD_BIND:
			strncpy(str, "bind", 255);
			break;
		case PG_ERRNOBSD_PROMISC:
			strncpy(str, "set promisc", 255);
			break;
		case PG_ERRNOBSD_IMDAT:
			strncpy(str, "set immediate", 255);
			break;
		case PG_ERRNOBSD_RCVALL: 
			strncpy(str, "set rcv all", 255);
			break;
		case PG_ERRNOBSD_FLUSH:
			strncpy(str, "buf flush", 255);
			break;
		case PG_ERRNOBSD_NCMPMAC:
			strncpy(str, "no cmpl mac", 255);
			break;
#endif

		default:
			strncpy(str, "unknown error!", 255);
			break;
			
	}



	fprintf(stderr, "%s(%s): %s \n", msg, str, pgen_strerror(pgen_errno));	
}
