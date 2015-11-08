

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



#include <pgen/pgen-arptbl.h>




template<typename T>
static void remove(std::vector<T>& vector, unsigned int index) {
    vector.erase(vector.begin() + index);
}



arptable::arptable(){
	return;	
}



arptable::arptable(pgen_t* h){
	sethandle(h);
	return;
}



void arptable::sethandle(pgen_t* h){
	handle = h;
	return;	
}





int arptable::add(ipaddr ip, macaddr mac){
	for(int i=0; i<entry.size(); i++){
		if(entry[i].ip == ip){
			entry[i].mac = mac;
			return 1;
		}
	}
	arpent_t buf;
	buf.ip = ip;
	buf.mac = mac;
	entry.push_back(buf);
	return 1;
}





int arptable::del(ipaddr ip){
	for(int i=0; i<entry.size(); i++){
		if(ip == entry[i].ip){
			remove(entry, i);
			return 1;
		}
	}
	return -1;	
}





void arptable::show(){
	for(int i=0; i<this->entry.size(); i++){
		printf("%s %s \n", entry[i].ip.c_str(), entry[i].mac.c_str());	
	}
	printf("%d entries \n", (int)entry.size());
}



//#include "debug.h"

int arptable::get(ipaddr ip){
	struct _packet{
		ethernet_header eth;
		arp_packet      arp;
	};
	struct _packet  send_pack;
	struct _packet* packet;
	ipaddr myip;
	macaddr mymac;
	macaddr results_mac;

	int len = 0;
	u_char data[1000];
	memset(data, 0, sizeof data);
	struct timeval tv;
	tv.tv_sec  = 1;
	tv.tv_usec = 0;

	myip.setipbydev(handle->online.ifname);
	mymac.setmacbydev(handle->online.ifname);
	
	for(int i=0; i<6; i++){
		send_pack.eth.ether_shost[i] = mymac.getOctet(i);
		send_pack.eth.ether_dhost[i] = 0xff;
	}
	send_pack.eth.ether_type = htons(0x0806);
	send_pack.arp.ea_hdr.ar_hrd = htons(0x0001);
	send_pack.arp.ea_hdr.ar_pro = htons(0x0800);
	send_pack.arp.ea_hdr.ar_hln = 6;
	send_pack.arp.ea_hdr.ar_pln = 4;
	send_pack.arp.ea_hdr.ar_op = htons(0x001);
	for(int i=0; i<6; i++){
		send_pack.arp.arp_sha[i] = mymac.getOctet(i);
		send_pack.arp.arp_tha[i] = 0xff;
	}
	for(int i=0; i<4; i++){
		send_pack.arp.arp_spa[i] = myip.getOctet(i);
		send_pack.arp.arp_tpa[i] = ip.getOctet(i);
	}
	
	pgen_send_to_netif(handle->fd, &send_pack, sizeof(send_pack));
	
	int timeout_flag = 1;
	for(int i=0; i<3; i++){
		len = pgen_recv_from_netif_to(handle->fd, data, sizeof(data), tv);
		if(len == 0){
			timeout_flag = 1;
			continue;
		}else if(len < 0){
			timeout_flag = 0;
			pgen_perror("pgen_recv_from_netif");
			return -1;
		}
		if(len < 14){
			timeout_flag = 0;
			continue;
		}

		timeout_flag = 0;
		packet = (struct _packet*)data;
		if(htons(packet->eth.ether_type) == 0x0806){
			if(packet->arp.ea_hdr.ar_op == htons(0x002)){
				results_mac.setmacbyarry(packet->arp.arp_sha);		
				this->add(ip, results_mac);
				return 1;
			}
		}
	}
	if(timeout_flag == 1){
		pgen_errno2 = PG_ERRNO_ARPERR;		
	}

	return -1;
}





macaddr arptable::find(ipaddr ip){
	macaddr zero;
	zero = "00:00:00:00:00:00";

	for(int i=0; i<entry.size(); i++){
		if(ip == entry[i].ip)
			return entry[i].mac;
	}
	if(this->get(ip) < 0){
		pgen_perror("arptable::find");
		return zero;
	}

	for(int i=0; i<entry.size(); i++){
		if(ip == entry[i].ip)
			return entry[i].mac;
	}

	return zero;
}




