

#include "arptable.h"
#include "protocols.h"
#include "unknown.h"

#include <thread>
#include <iostream>



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




int arptable::learn(const void* p, int len){
	pgen_unknown buf;
	pgen_arp     pack;
	
	buf.cast(p, len);
	if(buf.isARP){
		pack.cast(p, len);	
		this->add(pack.ARP.srcIp, pack.ARP.srcEth);
		return 1;
	}
	return 0;	
}





void arptable::show(){
	for(int i=0; i<this->entry.size(); i++){
		printf("%s %s \n", entry[i].ip.c_str(), entry[i].mac.c_str());	
	}
	printf("%d entries \n", (int)entry.size());
}






int arptable::get(ipaddr ip){
	int len = 0;
	u_char data[1000];
	pgen_arp pack;
	pgen_unknown unknown;
	struct timeval tv;
	
	tv.tv_sec  = 1;
	tv.tv_usec = 0;

	pack.ETH.src.setmacbydev(handle->online.ifname);
	pack.ETH.dst.setmacbroadcast();
	pack.ARP.operation = 1;
	pack.ARP.srcEth.setmacbydev(handle->online.ifname);
	pack.ARP.srcIp.setipbydev(handle->online.ifname);
	pack.ARP.dstEth.setmacbroadcast();
	pack.ARP.dstIp = ip;
	pack.compile();

	pgen_send_to_netif(handle->fd, pack.data, pack.len);
	
	for(int i=0; i<3; i++){
		len = pgen_recv_from_netif_to(handle->fd, data, sizeof(data), tv);
		if(len == 0){
			continue;
		}else if(len < 0){
			pgen_perror("pgen_recv_from_netif");
			return -1;
		}
		unknown.cast(data, len);
		if(unknown.isARP){
			pack.cast(data, len);
			if(pack.ARP.operation==2 && pack.ARP.srcIp==ip){
				this->add(ip, pack.ARP.srcEth);
				return 1;
			}
		}
	}
	return -1;
}





macaddr arptable::find(ipaddr ip){
	for(int i=0; i<entry.size(); i++){
		if(ip == entry[i].ip)
			return entry[i].mac;
	}
	this->get(ip);
	for(int i=0; i<entry.size(); i++){
		if(ip == entry[i].ip)
			return entry[i].mac;
	}
	return NULL;
}




