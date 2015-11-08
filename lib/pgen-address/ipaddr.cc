
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



#include <pgen/address.h>



ipaddr::ipaddr(){
	clear();	
}


ipaddr::ipaddr(const char* str){
	int n = inet_pton(AF_INET, str, &this->_addr);
	if(n == 0){
		fprintf(stderr, "ipaddr(const char*): string error \n");
		exit(-1);
	}else if(n == -1){
		perror("ipaddr(const char*)");
		exit(-1);
	}

}




ipaddr::ipaddr(const ipaddr &i){
	this->_addr = i._addr;
}





ipaddr::ipaddr(std::string str){
	*this = str.c_str();		
}


char* ipaddr::c_str(){
	union lc lc;
	lc.l = (unsigned int)this->_addr;
	snprintf(_c_str, sizeof(_c_str)-1,"%u.%u.%u.%u",
			lc.c[0],lc.c[1],lc.c[2],lc.c[3]);
	return _c_str;
}


void ipaddr::clear(){
	this->_addr = 0;	
}



int ipaddr::setOctet(int n, int num){
	union lc lc;
	if(n>=4){
		fprintf(stderr, "ipaddr::setOctet(): index is not support\n");
		return -1;
	}else{
		lc.c[n] = num;
		this->_addr = lc.l;
	}
	return 1;
}



unsigned char ipaddr::getOctet(int n){
	union lc lc;
	lc.l = this->_addr;
	if(n>=4){
		fprintf(stderr, "ipaddr::operator[]: index is not support\n");
		exit(-1);
	}else{
		return lc.c[n];		
	}
}



int ipaddr::setipbydev(const char* ifname){
	char buf[256];
	if(pgen_getipbydev(ifname, buf) < 0){
		pgen_perror("ipaddr::setipbydev");
		return -1;
	}
	*this = buf;
	return 1;
}



int ipaddr::setmaskbydev(const char* ifname){
	char buf[256];
	if(pgen_getmaskbydev(ifname, buf) < 0){
		pgen_perror("ipaddr::setmaskbydev");
		return -1;
	}
	*this = buf;
	return 1;
}



int ipaddr::setnetbydev(const char* ifname){
	ipaddr ip, mask;
	ip.setipbydev(ifname);
	mask.setmaskbydev(ifname);
	this->_addr = ip._addr & mask._addr;
	return 1;
}



int ipaddr::setnetbyaddr(ipaddr ip, ipaddr mask){
	this->_addr = ip._addr & mask._addr;
	return 1;
}



bool ipaddr::isSameSegment(ipaddr addr, ipaddr mask){
	return ((_addr&mask._addr) == (addr._addr&mask._addr));
}



ipaddr& ipaddr::operator=(ipaddr i){
	this->_addr = i._addr;
	return *this;
}


ipaddr& ipaddr::operator=(const char* str){
	int n = inet_pton(AF_INET, str, &_addr);
	if(n == 0){
		fprintf(stderr, "ipaddr(const char*): string error \n");
		exit(-1);
	}else if(n == -1){
		perror("ipaddr(const char*)");
		exit(-1);
	}
	return *this;
}




ipaddr& ipaddr::operator=(int num){	// FUGUAI ARUKAMO
	this->_addr = num;
	return *this;
}



ipaddr& ipaddr::operator=(std::string str){
	*this = str.c_str();
	return *this;
}



bool ipaddr::operator<(const ipaddr iaddr){
	union lc lc, ilc;
	lc.l = (unsigned int)this->_addr;
	ilc.l = (unsigned int)iaddr._addr;

	for(int i=0; i<4; i++){
		if(lc.c[i] == ilc.c[i])	continue;
		else					return lc.c[i] < ilc.c[i];
	}
	return false;
}



bool ipaddr::operator>(const ipaddr iaddr){
	union lc lc, ilc;
	lc.l = (unsigned int)this->_addr;
	ilc.l = (unsigned int)iaddr._addr;
	for(int i=0; i<4; i++){
		if(lc.c[i] == ilc.c[i])	continue;
		else					return lc.c[i] > ilc.c[i];
	}
	return false;
}



bool ipaddr::operator<=(const ipaddr iaddr){
	union lc lc, ilc;
	lc.l = (unsigned int)this->_addr;
	ilc.l = (unsigned int)iaddr._addr;

	for(int i=0; i<4; i++){
		if(lc.c[i] == ilc.c[i])	continue;
		else					return lc.c[i] <= ilc.c[i];
	}
	return false;
}



bool ipaddr::operator>=(const ipaddr iaddr){
	union lc lc, ilc;
	lc.l = (unsigned int)this->_addr;
	ilc.l = (unsigned int)iaddr._addr;
	for(int i=0; i<4; i++){
		if(lc.c[i] == ilc.c[i])	continue;
		else					return lc.c[i] >= ilc.c[i];
	}
	return false;
}


bool ipaddr::operator==(const ipaddr iaddr){
	return this->_addr == iaddr._addr;
}



bool ipaddr::operator!=(const ipaddr iaddr){
	return this->_addr != iaddr._addr;
}



ipaddr  ipaddr::operator++(int){
	ipaddr newaddr = *this;
	union lc lc;
	lc.l = this->_addr;
	for(int i=3; i>=0; i--){
		if(lc.c[i] < 255){	
			lc.c[i]++;
			break;
		}
	}
	this->_addr = lc.l;
	return newaddr;
}


