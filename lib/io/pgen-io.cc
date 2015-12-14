

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



#include <pgen/io/pgen-io.h>
#include <pgen/io/pgen-netutil.h>
#include <pgen/io/pgen-util.h>
#include <pgen/io/pgen-error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <netdb.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>

#ifndef __linux
#include <net/if_dl.h>
#include <net/bpf.h>
#include <fcntl.h>
#include <ifaddrs.h>
#else
#include <netpacket/packet.h>
#endif





// mode 0: read , 1: write
pgen_t* pgen_open_offline(const char* filename, int mode){

	pgen_t* handle = (pgen_t*)malloc(sizeof(pgen_t));
	handle->is_offline = 1;
	
	switch(mode){
		case 0: // read mode
			handle->is_write = 0;
			handle->is_read  = 1;
			handle->offline.fd = fopen(filename, "rb");
			if(handle->offline.fd == NULL){
				pgen_errno = errno;
				pgen_errno2 = PG_ERRNO_FWRITE;
				free(handle);
				handle = NULL;
				return NULL;
			}
			if(fread(&handle->offline.filehdr,
						sizeof(struct pcap_fhdr),1,handle->offline.fd)<1){
				pgen_errno = errno;
				pgen_errno2 = PG_ERRNO_FREAD;
				pgen_close(handle);
				handle = NULL;
			}
			break;
		
		case 1: // write mode 
			handle->is_write = 1;
			handle->is_read  = 0;
			handle->offline.fd = fopen(filename, "wb");
			if(handle->offline.fd == NULL){
				pgen_errno = errno;
				pgen_errno2 = PG_ERRNO_FOPEN;
				pgen_close(handle);
				handle = NULL;
			}
			handle->offline.filehdr.magic = 0xa1b2c3d4;
			handle->offline.filehdr.version_major = 0x0002;
			handle->offline.filehdr.version_minor = 0x0004;
			handle->offline.filehdr.timezone = 0x0000;
			handle->offline.filehdr.sigfigs  = 0x0000;
			handle->offline.filehdr.snaplen  = 0x0000ffff;
			handle->offline.filehdr.linktype = 0x00000001;
			if(fwrite(&handle->offline.filehdr,
						sizeof(struct pcap_fhdr),1,handle->offline.fd)<1){
				pgen_errno = errno;
				pgen_errno2 = PG_ERRNO_FWRITE;
				pgen_close(handle);
				handle = NULL;
			}
			break;
		
		default: // mode not found
			fprintf(stderr, "pgen_open_offline: mode not found\n");
			pgen_close(handle);
			break;
	}

	return handle;	
}





pgen_t* pgen_open(const char* dev, void* nouseyet){
	pgen_t* handle = (pgen_t*)malloc(sizeof(pgen_t));
	strncpy(handle->online.ifname, dev, sizeof(handle->online.ifname)-1);
	
	handle->is_write = 1;
	handle->is_read  = 1;

	handle->fd = initRawSocket(dev, 1, 0);
	if(handle->fd < 0){
		handle =  NULL;
	}
	
	return handle;
}




void pgen_close(pgen_t* p){
	if(p->is_offline == 1){
		fclose(p->offline.fd);
	}else{
		close(p->fd);
	}

	free(p);
	return ;	
}







// send packet in handle
int pgen_sendpacket_handle(pgen_t* p, const void* packet, int len){
	if(p->is_write == 0){
		pgen_errno2 = PG_ERRNO_RONLY;
		return -1;
	}
	int sendlen;
	
	if(p->is_offline == 1){
		sendlen = pgen_send_to_pcap(p->offline.fd, packet, len);
	}else{
		sendlen = pgen_send_to_netif(p->fd, packet, len);
	}
	
	return sendlen;
}



int pgen_sendpacket_L3(const char* dev,const void* packet,int len,struct sockaddr* sa){
#ifndef __linux
	pgen_errno2 = PG_ERRNO_NOSUPPORT;
	return -1;
#endif

	int sock;
	int sendlen;

	if((sock=initRawSocket(dev, 0, 1)) < 0){
		return -1;
	}

	sendlen = sendto(sock, packet, len, 0, sa, sizeof(struct sockaddr));
	if(sendlen < 0){
		pgen_errno = errno;
		pgen_errno2 = PG_ERRNO_SENDTO;
	}

	close(sock);
	return sendlen;
}




int pgen_sendpacket_L2(const char* dev, const void* packet, int len){
	int sock;
	int sendlen;

	if((sock=initRawSocket(dev, 0, 0)) < 0){
		return -1;
	}
	sendlen = pgen_send_to_netif(sock, packet, len);

	close(sock);
	return sendlen;
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

