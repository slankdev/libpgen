

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
#include <pgen/pgen-types.h>

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




/* This is pgen-syscall */
pgen_t* pgen_open_offline(const char* filename, int mode){

	struct pcap_fhdr filehdr;
	pgen_t* handle = (pgen_t*)malloc(sizeof(pgen_t));
	handle->mode = mode;

	switch(mode){
		case PCAP_READ: // pcap read mode
			handle->offline.fd = fopen(filename, "rb");
			if(handle->offline.fd == NULL){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FOPEN;
				free(handle);
				handle = NULL;
				return NULL;
			}

			if(fread(&filehdr,
						sizeof(struct pcap_fhdr),1,handle->offline.fd)<1){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FREAD;
				pgen_close(handle);
				handle = NULL;
			}
			break;
		
		case PCAP_WRITE: // pcap write mode 
			handle->offline.fd = fopen(filename, "wb");
			if(handle->offline.fd == NULL){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FOPEN;
				pgen_close(handle);
				handle = NULL;
			}
			filehdr.magic = 0xa1b2c3d4;
			filehdr.version_major = 0x0002;
			filehdr.version_minor = 0x0004;
			filehdr.timezone = 0x0000;
			filehdr.sigfigs  = 0x0000;
			filehdr.snaplen  = 0x0000ffff;
			filehdr.linktype = 0x00000001;
			if(fwrite(&filehdr,
						sizeof(struct pcap_fhdr),1,handle->offline.fd)<1){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FWRITE;
				pgen_close(handle);
				handle = NULL;
			}
			break;
		
		case PCAPNG_READ:
			printf("pgen_open_offline: this mode is not implement yet coming soon \n");
			break;
		case PCAPNG_WRITE:
			printf("pgen_open_offline: this mode is not implement yet coming soon \n");
			break;

		default: // mode not found
			fprintf(stderr, "pgen_open_offline: mode not found\n");
			pgen_close(handle);
			break;
	}

	return handle;	
}





/* This is pgen-syscall */
pgen_t* pgen_open(const char* dev, void* nouseyet){
	pgen_t* handle = (pgen_t*)malloc(sizeof(pgen_t));
	strncpy(handle->online.ifname, dev, sizeof(handle->online.ifname)-1);
	
	handle->mode = NETIF;

	handle->online.fd = initRawSocket(dev, 1, 0);
	if(handle->online.fd < 0){
		handle =  NULL;
	}
	
	return handle;
}




/* This is pgen-syscall */
void pgen_close(pgen_t* p){
	if(pgen_descriptor_is_offline(p)){
		fclose(p->offline.fd);
	}else {
		close(p->online.fd);
	}

	free(p);
	return ;	
}







/* This is pgen-syscall */
int pgen_sendpacket_handle(pgen_t* p, const void* packet, int len){
	if(pgen_descriptor_is_writeable(p) == false){
		pgen_errno_native = -1;
		pgen_errno = PG_ERRNO_NOWRITE;
		return -1;
	}
	int sendlen;
	
	if(pgen_descriptor_is_offline(p)){
		sendlen = pgen_send_to_pcap(p->offline.fd, packet, len);
	}else{
		sendlen = pgen_send_to_netif(p->online.fd, packet, len);
	}
	
	return sendlen;
}






/* This is pgen-syscall */
int pgen_sendpacket_L2(const char* dev, const void* packet, int len){
	int sock;
	int sendlen;

	if((sock=initRawSocket(dev, 0, 0)) < 0){
		return -1;
	}
	sendlen = pgen_send_to_netif(sock, packet, len);
	if(sendlen < 0){
		return -1;	
	}

	close(sock);
	return sendlen;
}



/* This is pgen-syscall */
int pgen_recvpacket_L2(const char* dev, void* packet, int len){
	int sock;
	int sendlen;

	if((sock=initRawSocket(dev, 0, 0)) < 0){
		return -1;
	}


	sendlen = pgen_recv_from_netif(sock, packet, len);

	close(sock);
	return sendlen;
}



/* This is pgen-syscall */
int pgen_recvpacket_handle(pgen_t* p, void* packet, int len){
	if(pgen_descriptor_is_readable(p) == false){
		pgen_errno_native = -1;
		pgen_errno = PG_ERRNO_NOREAD;
		return -1;
	}
	int sendlen;
	
	if(pgen_descriptor_is_offline(p)){
		sendlen = pgen_recv_from_pcap(p->offline.fd, packet, len);
	}else{
		sendlen = pgen_recv_from_netif(p->online.fd, packet, len);
	}
	
	return sendlen;
}






bool pgen_descriptor_is_offline(pgen_t* handle){
	return !pgen_descriptor_is_online(handle);
}


bool pgen_descriptor_is_online(pgen_t* handle){
	int mode = handle->mode;
	if(mode == NETIF)
		return true;
	else
		return false;
}


bool pgen_descriptor_is_readable(pgen_t* handle){
	int mode = handle->mode;
	if(mode==NETIF || mode==PCAP_READ || mode==PCAPNG_READ)
		return true;
	else
		return false;
}
bool pgen_descriptor_is_writeable(pgen_t* handle){
	int mode = handle->mode;
	if(mode==NETIF || mode==PCAP_WRITE || mode==PCAPNG_WRITE)
		return true;
	else
		return false;
		
}




void pgen_perror(const char* msg){
	const int errstrlen = 255;
	char str[errstrlen];

// 	switch(pgen_errno){
// 		case PG_ERRNO_SUCSS:
// 			strncpy(str, "success", errstrlen);
// 			break;
// 		case PG_ERRNO_BIND:
// 			strncpy(str, "bind", errstrlen);
// 			break;
// 		case PG_ERRNO_HDRINC: 
// 			strncpy(str, "setsockopt hdrinc", errstrlen);
// 			break;
// 		case PG_ERRNO_PROMISC:
// 			strncpy(str, "set promisc", errstrlen);
// 			break;
// 		case PG_ERRNO_WONLY:
// 			strncpy(str, "handle is write only", errstrlen);
// 			break;
// 		case PG_ERRNO_RONLY: 
// 			strncpy(str, "handle is read only", errstrlen);
// 			break;
// 		case PG_ERRNO_FWRITE:
// 			strncpy(str, "fwrite", errstrlen);
// 			break;
// 		case PG_ERRNO_FREAD:
// 			strncpy(str, "fread", errstrlen);
// 			break;
// 		case PG_ERRNO_WRITE:
// 			strncpy(str, "write", errstrlen);
// 			break;
// 		case PG_ERRNO_READ:
// 			strncpy(str, "read", errstrlen);
// 			break;
// 		case PG_ERRNO_SENDTO:
// 			strncpy(str, "sendto", errstrlen);
// 			break;
// 		case PG_ERRNO_FOPEN:
// 			strncpy(str, "fopen", errstrlen);
// 			break;
// 		case PG_ERRNO_IOCTL:
// 			strncpy(str, "ioctl", errstrlen);
// 			break;
// 		case PG_ERRNO_SOCKET:
// 			strncpy(str, "socket", errstrlen);
// 			break;
// 		case PG_ERRNO_SELECT:
// 			strncpy(str, "select", errstrlen);
// 			break;
// 		case PG_ERRNO_TIMEOUT:
// 			strncpy(str, "timeout", errstrlen);
// 			break;
// 		case PG_ERRNO_ARPERR:
// 			strncpy(str, "arp table error", errstrlen);
// 			break;
// 		// case PG_ERRNO_NOSUPPORT:
// 		// 	strncpy(str, "not support", errstrlen);
// 		// 	break;
// 		case PG_ERRNO_UNDEFINED:
// 			strncpy(str, "this error wasn't implemented yet", errstrlen);
// 			break;
//
// #ifndef __linux // for bsd
// 		case PG_ERRNOBSD_OPENBPF:
// 			strncpy(str, "open bpf", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_SETBUF:
// 			strncpy(str, "set buf", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_BIND:
// 			strncpy(str, "bind", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_PROMISC:
// 			strncpy(str, "set promisc", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_IMDAT:
// 			strncpy(str, "set immediate", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_RCVALL: 
// 			strncpy(str, "set rcv all", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_FLUSH:
// 			strncpy(str, "buf flush", errstrlen);
// 			break;
// 		case PG_ERRNOBSD_NCMPMAC:
// 			strncpy(str, "no cmpl mac", errstrlen);
// 			break;
// #endif
//
// 		default:
// 			strncpy(str, "unknown error! We didn't use this error number", errstrlen);
// 			break;
// 		
// }


	// fprintf(stderr, "%s(%s): %s \n", msg, str, pgen_strerror(pgen_errno_native));	
}



