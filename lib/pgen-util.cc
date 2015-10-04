
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




#include "pgen-util.h"
#include "pgen-types.h"
#include "pgen-error.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#ifndef __linux
#include <net/bpf.h>
#endif







int pgen_send_to_pcap(FILE* fp, const void* buf, int len){
	int sendlen = 0;
	struct timeval ts_now;
	struct pcap_pkthdr pkthdr;
	gettimeofday(&ts_now, NULL);
	
	pkthdr.ts.tv_sec = (unsigned int)(ts_now.tv_sec);
	pkthdr.ts.tv_usec = (unsigned int)(ts_now.tv_usec);
	pkthdr.caplen = len;
	pkthdr.len    = len;

	if(fwrite(&pkthdr, sizeof(struct pcap_pkthdr), 1, fp) != 1){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_FWRITE;
		sendlen = -1;
	}else{
		if(fwrite(buf, len, 1, fp) < 1){
			pgen_errno = errno;
			pgen_errno2 = PG_ERRNO_FWRITE;
			sendlen = -1;
		}
		sendlen = len;
	}
	
	return sendlen;	
}






int pgen_recv_from_pcap(FILE* fp, void* buf, int len){
	struct pcap_pkthdr hdr;
	if(fread(&hdr, sizeof(struct pcap_pkthdr), 1, fp) < 1){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_FREAD;
		return -1;
	}
	if(fread(buf, hdr.len, 1, fp) != 1){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_FREAD;
		return -1;
	}

	return hdr.len;
}




