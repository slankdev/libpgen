

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
			u_char buf[1000];
			int buflen;

			handle->offline.fd = fopen(filename, "rb");
			if(handle->offline.fd == NULL){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FOPEN;
				free(handle);
				handle = NULL;
				return NULL;
			}

			buflen = pgen_readblock_from_pcapng(handle->offline.fd, buf, sizeof(buf));
			if(buflen < 0){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FOPEN;
				pgen_close(handle);
				handle = NULL;
			}
			break;



		case PCAPNG_WRITE:

			handle->offline.fd = fopen(filename, "wb");
			if(handle->offline.fd == NULL){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FOPEN;
				pgen_close(handle);
				handle = NULL;
				break;
			}
			unsigned int tot_len_tail;
			
			// write section header block
			struct __pcapng_SHB filehdrng;
			filehdrng.type    = 0x0a0d0d0a;
			filehdrng.tot_len = 0x00000020;
			filehdrng.byteorder_magic = 0x1a2b3c4d;
			filehdrng.major_version = 0x0001;
			filehdrng.minor_version = 0x0000;
			filehdrng.section_length[0] = 0xffffffff;
			filehdrng.section_length[1] = 0xffffffff;
			filehdrng.section_length[2] = 0x000000ff;
			tot_len_tail = filehdrng.tot_len;
			if(fwrite(&filehdrng,
						sizeof(struct __pcapng_SHB),1,handle->offline.fd) != 1){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FWRITE;
				pgen_close(handle);
				handle = NULL;
				break;
			}
			if(fwrite(&tot_len_tail,
						sizeof(tot_len_tail),1,handle->offline.fd) != 1){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FWRITE;
				pgen_close(handle);
				handle = NULL;
				break;
			}

			// write interface description block
			struct __pcapng_IDB idb;
			idb.type    = 0x00000001;
			idb.tot_len = 0x00000014;
			idb.link_type = 0x0001;
			idb.reserved = 0x0000;
			idb.snap_length = 0x0000;
			tot_len_tail = idb.tot_len;
			if(fwrite(&idb,
						sizeof(struct __pcapng_IDB),1,handle->offline.fd) != 1){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FWRITE;
				pgen_close(handle);
				handle = NULL;
				break;
			}
			if(fwrite(&tot_len_tail,
						sizeof(tot_len_tail),1,handle->offline.fd) != 1){
				pgen_errno_native = errno;
				pgen_errno = PG_NERRNO_FWRITE;
				pgen_close(handle);
				handle = NULL;
				break;
			}
			break;



		default: // mode not found
			pgen_errno_native = -1;
			pgen_errno = PG_ERRNO_MODENFOUND;
			pgen_close(handle);
			handle = NULL;
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
	

	if(p->mode == NETIF){
		printf("pgen_sendpacket_handle NETIF \n");
		sendlen = pgen_send_to_netif(p->online.fd, packet, len);
	}else if(p->mode == PCAP_WRITE){
		printf("pgen_sendpacket_handle PCAP \n");
		sendlen = pgen_send_to_pcap(p->offline.fd, packet, len);
	}else if(p->mode == PCAPNG_WRITE){
		printf("pgen_sendpacket_handle PCAPNG \n");
		sendlen = pgen_send_to_pcapng(p->offline.fd, packet, len);
	}else{
		printf("pgen_sendpacket_handle OTHER \n");
		pgen_errno_native = -1;
		pgen_errno = PG_ERRNO_MODENFOUND;
		sendlen = -1;
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
	
	if(p->mode == NETIF){
		sendlen = pgen_recv_from_netif(p->online.fd, packet, len);
	}else if(p->mode == PCAP_READ){
		sendlen = pgen_recv_from_pcap(p->offline.fd, packet, len);
	}else if(p->mode == PCAPNG_READ){
		sendlen = pgen_recv_from_pcapng(p->offline.fd, packet, len);
	}else{
		pgen_errno_native = -1;
		pgen_errno = PG_ERRNO_MODENFOUND;
		sendlen = -1;
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
	if(pgen_errno_native < 0){
		printf("%s: %s \n", msg, pgen_strerror(pgen_errno));
	}else{
		printf("%s: %s \n" , msg, pgen_strerror(pgen_errno));
	}
}






