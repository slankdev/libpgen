

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


#include <pgen/io/pgen-error.h>
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
	int errstrlen = 255;
	char str[errstrlen];


	switch(pgen_errno2){
		case PG_ERRNO_SUCSS:
			strncpy(str, "success", errstrlen);
			break;
		case PG_ERRNO_BIND:
			strncpy(str, "bind", errstrlen);
			break;
		case PG_ERRNO_HDRINC: 
			strncpy(str, "setsockopt hdrinc", errstrlen);
			break;
		case PG_ERRNO_PROMISC:
			strncpy(str, "set promisc", errstrlen);
			break;
		case PG_ERRNO_WONLY:
			strncpy(str, "handle is write only", errstrlen);
			break;
		case PG_ERRNO_RONLY: 
			strncpy(str, "handle is read only", errstrlen);
			break;
		case PG_ERRNO_FWRITE:
			strncpy(str, "fwrite", errstrlen);
			break;
		case PG_ERRNO_FREAD:
			strncpy(str, "fread", errstrlen);
			break;
		case PG_ERRNO_WRITE:
			strncpy(str, "write", errstrlen);
			break;
		case PG_ERRNO_READ:
			strncpy(str, "read", errstrlen);
			break;
		case PG_ERRNO_SENDTO:
			strncpy(str, "sendto", errstrlen);
			break;
		case PG_ERRNO_FOPEN:
			strncpy(str, "fopen", errstrlen);
			break;
		case PG_ERRNO_IOCTL:
			strncpy(str, "ioctl", errstrlen);
			break;
		case PG_ERRNO_SOCKET:
			strncpy(str, "socket", errstrlen);
			break;
		case PG_ERRNO_SELECT:
			strncpy(str, "select", errstrlen);
			break;
		case PG_ERRNO_TIMEOUT:
			strncpy(str, "timeout", errstrlen);
			break;
		case PG_ERRNO_ARPERR:
			strncpy(str, "arp table error", errstrlen);
			break;
		case PG_ERRNO_NOSUPPORT:
			strncpy(str, "not support", errstrlen);
			break;

#ifndef __linux // for bsd
		case PG_ERRNOBSD_OPENBPF:
			strncpy(str, "open bpf", errstrlen);
			break;
		case PG_ERRNOBSD_SETBUF:
			strncpy(str, "set buf", errstrlen);
			break;
		case PG_ERRNOBSD_BIND:
			strncpy(str, "bind", errstrlen);
			break;
		case PG_ERRNOBSD_PROMISC:
			strncpy(str, "set promisc", errstrlen);
			break;
		case PG_ERRNOBSD_IMDAT:
			strncpy(str, "set immediate", errstrlen);
			break;
		case PG_ERRNOBSD_RCVALL: 
			strncpy(str, "set rcv all", errstrlen);
			break;
		case PG_ERRNOBSD_FLUSH:
			strncpy(str, "buf flush", errstrlen);
			break;
		case PG_ERRNOBSD_NCMPMAC:
			strncpy(str, "no cmpl mac", errstrlen);
			break;
#endif

		default:
			strncpy(str, "unknown error!", errstrlen);
			break;
			
	}


	fprintf(stderr, "%s(%s): %s \n", msg, str, pgen_strerror(pgen_errno));	
}
