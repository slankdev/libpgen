
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



#include <pgen/packet/address/address.h>



macaddr::macaddr(){ 
	clear();
}


macaddr::macaddr(const macaddr &m){
	for(int i=0; i<6; i++)
		this->_addr[i] = m._addr[i];
}



macaddr::macaddr(const char* str){
	unsigned int buf[6];
	int n = sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
			&buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
	if(n != 6){
		fprintf(stderr, "macaddr::macaddr(): reading error\n");
		exit(-1);
	}
	for(int i=0; i<6; i++)	this->_addr[i] = (u_char)buf[i];
}



int macaddr::setmacbyarry(const u_char* array){
	for(int i=0; i<6; i++){
		this->_addr[i] = array[i];
	}
	return 1;
}



char* macaddr::c_str(){
	snprintf(_str,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
			this->_addr[0], this->_addr[1], this->_addr[2], 
			this->_addr[3], this->_addr[4], this->_addr[5]);
	return _str;
}



void macaddr::clear(){
	memset(this->_addr, 0, sizeof(char[6]));	
}



int macaddr::setOctet(int n, int num){
	if(n>=6){
		fprintf(stderr, "macaddr::setOctet(): index is not support\n");
		return -1;
	}else{
		this->_addr[n] = num;
	}
	return 1;
}



unsigned char macaddr::getOctet(int n){
	if(n>=6){
		fprintf(stderr, "ipaddr::operator[]: index is not support\n");
		exit(-1);
	}else{
		return this->_addr[n];		
	}
}



int macaddr::setmacbydev(const char* ifname){
	char buf[256];
	if(pgen_getmacbydev(ifname, buf) < 0){
		printf("error \n");
		return -1;
	}
	*this = buf;
	return 1;
}




int macaddr::setmacbroadcast(){
	for(int i=0; i<6; i++){
		_addr[i] = 0xff;
	}
	return 1;
}



char* macaddr::bender(){
	unsigned int mac[3];
	char  buf[64];
	FILE* fp;
	if((fp=fopen(FILEPATH, "r")) == NULL){
		perror("macaddr::bender()");
		strncpy(_bender, "error", sizeof(_bender)-1);
		return _bender;
	}
	while(fgets(buf, sizeof(buf), fp) != NULL){
		sscanf(buf, "%2x%2x%2x\t%s", &mac[0],&mac[1],&mac[2],buf);
		if( mac[0]==this->_addr[0] &&
				mac[1]==this->_addr[1] && 
				mac[2]==this->_addr[2]){
			snprintf(_bender, sizeof(_bender), "%s", buf);
			fclose(fp);
			return _bender;
		}
		memset(mac, 0, sizeof(mac));
		memset(buf, 0, sizeof(buf));
		memset(_bender, 0, sizeof(_bender));
	}
	strncpy(_bender, "not-found", sizeof(_bender)-1);
	fclose(fp);
	return _bender;
}



macaddr& macaddr::operator=(int n){
	for(int i=0; i<6; i++)  this->_addr[i] = n;
	return *this;
}




macaddr& macaddr::operator=(const macaddr m){
	for(int i=0; i<6; i++)	this->_addr[i] = m._addr[i];
	return *this;
}



macaddr& macaddr::operator=(const char* str){
	unsigned int buf[6];
	int n = sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
			&buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
	if(n!=6){
		fprintf(stderr, "macaddr::operator=: reading error\n");
		exit(-1);
	}
	for(int i=0; i<6; i++)	this->_addr[i] = (u_char)buf[i];
	return *this;
}



macaddr& macaddr::operator=(std::string str){
	*this = str.c_str();
	return *this;
}



bool macaddr::operator<(const macaddr iaddr){
	for(int i=0; i<6; i++){
		if(this->_addr[i] == iaddr._addr[i])	continue;
		else return this->_addr[i] < iaddr._addr[i];
	}
	return false;
}



bool macaddr::operator>(const macaddr iaddr){
	for(int i=0; i<6; i++){
		if(this->_addr[i] == iaddr._addr[i])	continue;
		else return this->_addr[i] > iaddr._addr[i];
	}
	return false;
}



bool macaddr::operator<=(const macaddr iaddr){
	for(int i=0; i<6; i++){
		if(this->_addr[i] == iaddr._addr[i])	continue;
		else return this->_addr[i] <= iaddr._addr[i];
	}
	return false;
}



bool macaddr::operator>=(const macaddr iaddr){
	for(int i=0; i<6; i++){
		if(this->_addr[i] == iaddr._addr[i])	continue;
		else	return this->_addr[i] >= iaddr._addr[i];
	}
	return false;
}




bool macaddr::operator==(const macaddr iaddr){
	for(int i=0; i<6; i++){
		if(this->_addr[i] != iaddr._addr[i])	return false;
	}
	return true;
}



bool macaddr::operator!=(const macaddr iaddr){
	for(int i=0; i<6; i++){
		if(this->_addr[i] != iaddr._addr[i])	return true;
	}
	return false;
}

