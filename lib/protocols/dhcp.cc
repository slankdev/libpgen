

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


#include "pgen.h"
#include "packet.h"
#include "address.h"
#include "netutils.h"

#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>



pgen_dhcp::pgen_dhcp(){
	clear();
}





pgen_dhcp::pgen_dhcp(const void* packet, int len){
	clear();
	cast(packet, len);
}





void pgen_dhcp::clear(){
	pgen_udp::clear();

	this->DHCP.op     = 1;
	this->DHCP.htype  = 0x01;
	this->DHCP.hlen   = 6;
	this->DHCP.hops   = 0;
	this->DHCP.xid    = 0;
	this->DHCP.secs   = 0;
	this->DHCP.flags  = 0;
	this->DHCP.ciaddr = 0;
	this->DHCP.yiaddr = 0;
	this->DHCP.siaddr = 0;
	this->DHCP.giaddr = 0;
	this->DHCP.chaddr = 0;
	memset(this->DHCP.sname, 0, sizeof(this->DHCP.sname));
	memset(this->DHCP.file, 0, sizeof(this->DHCP.file));
	
	memset(this->dhcp_option, 0, sizeof(this->dhcp_option));
	dhcp_option_len = 0;

	this->DHCP.option_len = 0;
	memset(this->DHCP.option, 0, sizeof(struct dhcp_option) * DHCP_MAX_OPT);

}





void pgen_dhcp::compile(){
	
	memset(&this->dhcp, 0, DHCP_HDR_LEN);
	this->dhcp.op = this->DHCP.op;
	this->dhcp.htype  = this->DHCP.htype;
	this->dhcp.hlen   = this->DHCP.hlen;
	this->dhcp.hops   = this->DHCP.hops;
	this->dhcp.xid    = htonl(this->DHCP.xid);
	this->dhcp.secs   = htons(this->DHCP.secs);
	this->dhcp.flags  = htons(this->DHCP.flags);
	this->dhcp.ciaddr = this->DHCP.ciaddr._addr;
	this->dhcp.yiaddr = this->DHCP.yiaddr._addr;
	this->dhcp.siaddr = this->DHCP.siaddr._addr;
	this->dhcp.giaddr = this->DHCP.giaddr._addr;
	for(int i=0; i<this->DHCP.hlen; i++){
		this->dhcp.chaddr[i] = this->DHCP.chaddr._addr[i];		
	}
	memcpy(this->dhcp.sname, this->DHCP.sname, sizeof(this->dhcp.sname));
	memcpy(this->dhcp.file, this->DHCP.file, sizeof(this->dhcp.file));
	this->dhcp.magic[0] = 0x63; 	
	this->dhcp.magic[1] = 0x82; 	
	this->dhcp.magic[2] = 0x53; 	
	this->dhcp.magic[3] = 0x63; 	
	
	bit8* p0 = dhcp_option;
	for(int i=0; i<(int)this->DHCP.option_len; i++){
		memcpy(p0, &DHCP.option[i], DHCP.option[i].len+2);	
		p0 += DHCP.option[i].len+2;
	}
	dhcp_option_len = p0 - dhcp_option;

	UDP.len = UDP_HDR_LEN + DHCP_HDR_LEN + dhcp_option_len;
	pgen_udp::compile();

	bit8* p = data;
	memcpy(p, &eth, ETH_HDR_LEN);
	p += ETH_HDR_LEN;
	memcpy(p, &ip, IP_HDR_LEN);
	p += IP_HDR_LEN;
	memcpy(p, &udp, UDP_HDR_LEN);
	p += UDP_HDR_LEN;
	memcpy(p, &dhcp, DHCP_HDR_LEN);
	p += DHCP_HDR_LEN;
	memcpy(p, dhcp_option, dhcp_option_len);
	p += dhcp_option_len;

	this->len = p - data;
}




void pgen_dhcp::cast(const void* packet, int len){
	if(!(this->minLen<=len && len<=this->maxLen)){
		fprintf(stderr, "pgen_dhcp::cast(): packet len isn`t support (%d)\n", len);
		return;
	}

	pgen_udp::cast(packet, len);
	const u_char* p = (u_char*)packet;

	p += ETH_HDR_LEN;
	p += IP_HDR_LEN;
	p += UDP_HDR_LEN;

	struct dhcp_header* buf = (struct dhcp_header*)p;
	p += DHCP_HDR_LEN;

	this->DHCP.op = buf->op;
	this->DHCP.htype = buf->htype;
	this->DHCP.hlen = buf->hlen;
	this->DHCP.hops = buf->hops;
	this->DHCP.xid  = ntohl(buf->xid);
	this->DHCP.secs = ntohs(buf->secs);
	this->DHCP.flags = ntohs(buf->flags);
	this->DHCP.ciaddr._addr = buf->ciaddr;
	this->DHCP.yiaddr._addr = buf->yiaddr;
	this->DHCP.siaddr._addr = buf->siaddr;
	this->DHCP.giaddr._addr = buf->giaddr;
	for(int i=0; i<DHCP.hlen; i++)
		this->DHCP.chaddr._addr[i] = buf->chaddr[i];
	memcpy(this->DHCP.sname, buf->sname, 64);
	memcpy(this->DHCP.file , buf->file , 128);

	for(this->DHCP.option_len=0; p-(u_char*)packet<len; this->DHCP.option_len++){
		if(*p == 255){
			DHCP.option[DHCP.option_len].type = 255;
			DHCP.option[DHCP.option_len].len  = 0;
			memcpy(DHCP.option[DHCP.option_len].data, p+1, len - (p-(u_char*)packet) - 1);
			DHCP.option_len++;
			break;	
		}
		
		dhcp_get_option(p, &DHCP.option[DHCP.option_len]);
		p += this->DHCP.option[this->DHCP.option_len].len + 2;

	}
}






void pgen_dhcp::summary(){
	compile();

	printf("DHCP{ ");
	
	if(this->DHCP.option[0].data[0] == 1){
		printf("Discover");
	}else if(this->DHCP.option[0].data[0] == 2){
		printf("Offer");
	}else if(this->DHCP.option[0].data[0] == 3){
		printf("Request");
	}else if(this->DHCP.option[0].data[0] == 4){
		printf("Decline");
	}else if(this->DHCP.option[0].data[0] == 5){
		printf("ACK");
	}else if(this->DHCP.option[0].data[0] == 6){
		printf("NAK");
	}else if(this->DHCP.option[0].data[0] == 7){
		printf("Release");
	}else if(this->DHCP.option[0].data[0] == 8){
		printf("Inform");
	}else if(this->DHCP.option[0].data[0] == 9){
		printf("Forcerenew");
	}else if(this->DHCP.option[0].data[0] == 10){
		printf("Lease Query");
	}else if(this->DHCP.option[0].data[0] == 11){
		printf("Lease Unassigned");
	}else if(this->DHCP.option[0].data[0] == 12){
		printf("Lease Unknown");
	}else if(this->DHCP.option[0].data[0] == 13){
		printf("Lease Active");
	}else{
		fprintf(stderr, "}\npgen_dhcp::summary: DHCP Message type not found\n");
	}

	printf(" Transaction ID 0x%04x }\n", DHCP.xid);
}





void pgen_dhcp::info(){
	printf(" * Dynamic Host Configuration Protocol \n");
	printf("    - Message Tpye    : %d \n", DHCP.op);
	printf("    - Hardware Type   : 0x%02x \n", DHCP.htype);
	printf("    - Hardware Len    : %d \n", DHCP.hlen);
	printf("    - Hops            : %d \n", DHCP.hops);
	printf("    - Transaction ID  : 0x%08x \n", DHCP.xid);
	printf("    - Seconds Elapsed : %d \n", DHCP.secs);
	printf("    - Flags           : 0x%04x \n", DHCP.flags);
	printf("    - Client IP       : %s \n", DHCP.ciaddr.c_str());
	printf("    - Your IP         : %s \n", DHCP.yiaddr.c_str());
	printf("    - Next Server IP  : %s \n", DHCP.siaddr.c_str());
	printf("    - Relay Agent IP  : %s \n", DHCP.giaddr.c_str());
	printf("    - Client Mac Addr : %s \n", DHCP.chaddr.c_str());
	printf("    - Server Hostname : %s \n", DHCP.sname);
	printf("    - Boot Filename   : %s \n", DHCP.file);

	for(int i=0; i<(int)DHCP.option_len; i++){
		printf("    - Option          : (%d) \n", DHCP.option[i].type);
	}
}




void pgen_dhcp::dhcp_set_option(int index, int type, int len, void* data){
	this->DHCP.option[index].type = type;
	this->DHCP.option[index].len  = len;
	memcpy(this->DHCP.option[index].data, data, len);
}




void pgen_dhcp::dhcp_get_option(const void* p, struct dhcp_option* buf){
	struct dhcp_option* b = (struct dhcp_option*)p;
	buf->type = b->type;
	buf->len  = b->len;
	memcpy(buf->data, b->data, buf->len);
}




