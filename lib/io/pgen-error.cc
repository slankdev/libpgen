

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

int pgen_errno_native=0;
int pgen_errno=0;
char pgen_errstr[PGEN_ERRSTR_LEN]={0};





// char* old_pgen_strerror(int errno){
// 	strncpy(pgen_errstr, strerror(errno), PGEN_ERRSTR_LEN);
// 	return pgen_errstr;	
// }



char* pgen_strerror(int errno){
	int errstrlen = sizeof(pgen_errstr) -1;
	

	switch(pgen_errno){
		case PG_NERRNO_WRITE:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[write]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_READ:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[read]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_FOPEN:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[fopen]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_FWRITE:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[fwrite]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_FREAD:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[fread]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_SOCKET:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[socket]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_SETSOCKOPT:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[setsockopt]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_IOCTL:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[ioctl]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_SELECT:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[select]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_BIND:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[bind]:%s", strerror(pgen_errno));
			break;
		case PG_NERRNO_SENDTO:
			snprintf(pgen_errstr, sizeof(pgen_errstr), 
					"[sendto]:%s", strerror(pgen_errno));
			break;


		case PG_ERRNO_UNDEFINED:
			snprintf(pgen_errstr, sizeof(pgen_errstr), "This error number is undefined yet \n");
			break;
		case PG_ERRNO_NOWRITE:        
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Descriptor isn't writeable \n");
			break;
		case PG_ERRNO_NOREAD:
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Descriptor isn't readable \n");
			break;
		case PG_ERRNO_TIMEOUT:
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Timeout error \n");
			break;
		case PG_ERRNO_OPENBPF :       
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Can't open bpf \n");
			break;
		case PG_ERRNO_GETHARDADDR_BSD:
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Get hardware address on bsd \n");
			break;
		case PG_ERRNO_NOSUPPORT:
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Not support \n");
			break;
		case PG_ERRNO_BUFLEN_FAIL  :
			snprintf(pgen_errstr, sizeof(pgen_errstr), "Buffer length is failed \n");
			break;


		default:
			strncpy(pgen_errstr, "unknown error! We didn't use this error number", errstrlen);
			break;
	}

	return pgen_errstr;
}



