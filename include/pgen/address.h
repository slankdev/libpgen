
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



#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "pgen/pgen-types.h"
#include "pgen/pgen-io.h"
#include "pgen/pgen-error.h"

#ifndef __linux
//#include <ifaddrs.h>
//#include <net/if_dl.h>
//#include <net/if_types.h>
#endif


#define FILEPATH "/usr/local/etc/pgen/mac_code.list"


union lc{
	unsigned int l;
	unsigned char c[4];
};


class ipaddr{
	private:
		
	public:
		char _c_str[32];
		bit32 _addr;		
		
		ipaddr(){ clear(); }
		ipaddr(const char* str){
			int n = inet_pton(AF_INET, str, &this->_addr);
			if(n == 0){
				fprintf(stderr, "ipaddr(const char*): string error \n");
				exit(-1);
			}else if(n == -1){
				perror("ipaddr(const char*)");
				exit(-1);
			}
		}
		ipaddr(const ipaddr &i){
			this->_addr = i._addr;
		}
		ipaddr(std::string str){
			*this = str.c_str();		
		}
		char* c_str(){
			union lc lc;
			lc.l = (unsigned int)this->_addr;
			snprintf(_c_str, sizeof(_c_str)-1,"%u.%u.%u.%u",
							lc.c[0],lc.c[1],lc.c[2],lc.c[3]);
			return _c_str;
		}
		void clear(){
			this->_addr = 0;	
		}
		int setOctet(int n, int num){
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
		unsigned char getOctet(int n){
			union lc lc;
			lc.l = this->_addr;
			if(n>=4){
				fprintf(stderr, "ipaddr::operator[]: index is not support\n");
				exit(-1);
			}else{
				return lc.c[n];		
			}
		}
		int setipbydev(const char* ifname){
			char buf[256];
			if(pgen_getipbydev(ifname, buf) < 0){
				pgen_perror("ipaddr::setipbydev");
				return -1;
			}
			*this = buf;
			return 1;
		}
		int setmaskbydev(const char* ifname){
			char buf[256];
			if(pgen_getmaskbydev(ifname, buf) < 0){
				pgen_perror("ipaddr::setmaskbydev");
				return -1;
			}
			*this = buf;
			return 1;
		}
		int setnetbydev(const char* ifname){
			ipaddr ip, mask;
			ip.setipbydev(ifname);
			mask.setmaskbydev(ifname);
			this->_addr = ip._addr & mask._addr;
			return 1;
		}
		int setnetbyaddr(ipaddr ip, ipaddr mask){
			this->_addr = ip._addr & mask._addr;
			return 1;
		}
		bool isSameSegment(ipaddr addr, ipaddr mask){
			return ((_addr&mask._addr) == (addr._addr&mask._addr));
		}
		ipaddr& operator=(ipaddr i){
			this->_addr = i._addr;
			return *this;
		}
		ipaddr& operator=(const char* str){
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
		ipaddr& operator=(int num){	// FUGUAI ARUKAMO
			this->_addr = num;
			return *this;
		}
		ipaddr& operator=(std::string str){
			*this = str.c_str();
			return *this;
		}
		bool operator<(const ipaddr iaddr){
			union lc lc, ilc;
			lc.l = (unsigned int)this->_addr;
			ilc.l = (unsigned int)iaddr._addr;
			
			for(int i=0; i<4; i++){
				if(lc.c[i] == ilc.c[i])	continue;
				else					return lc.c[i] < ilc.c[i];
			}
			return false;
		}
		bool operator>(const ipaddr iaddr){
			union lc lc, ilc;
			lc.l = (unsigned int)this->_addr;
			ilc.l = (unsigned int)iaddr._addr;
			for(int i=0; i<4; i++){
				if(lc.c[i] == ilc.c[i])	continue;
				else					return lc.c[i] > ilc.c[i];
			}
			return false;
		}
		bool operator<=(const ipaddr iaddr){
			union lc lc, ilc;
			lc.l = (unsigned int)this->_addr;
			ilc.l = (unsigned int)iaddr._addr;
			
			for(int i=0; i<4; i++){
				if(lc.c[i] == ilc.c[i])	continue;
				else					return lc.c[i] <= ilc.c[i];
			}
			return false;
		}
		bool operator>=(const ipaddr iaddr){
			union lc lc, ilc;
			lc.l = (unsigned int)this->_addr;
			ilc.l = (unsigned int)iaddr._addr;
			for(int i=0; i<4; i++){
				if(lc.c[i] == ilc.c[i])	continue;
				else					return lc.c[i] >= ilc.c[i];
			}
			return false;
		}
		bool operator==(const ipaddr iaddr){
			return this->_addr == iaddr._addr;
		}
		bool operator!=(const ipaddr iaddr){
			return this->_addr != iaddr._addr;
		}
		ipaddr  operator++(int){
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

		
};




class macaddr{
	private:
	public:
		char _str[256];
		char _bender[32];
		bit8 _addr[6];
	
		macaddr(){ clear(); }
		macaddr(const macaddr &m){
			for(int i=0; i<6; i++)
				this->_addr[i] = m._addr[i];
		}
		macaddr(const char* str){
			unsigned int buf[6];
			int n = sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
				&buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
			if(n != 6){
				fprintf(stderr, "macaddr::macaddr(): reading error\n");
				exit(-1);
			}
			for(int i=0; i<6; i++)	this->_addr[i] = (u_char)buf[i];
		}
		int setmacbyarry(const u_char* array){
			for(int i=0; i<6; i++){
				this->_addr[i] = array[i];
			}
			return 1;
		}
		char* c_str(){
			snprintf(_str,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
				this->_addr[0], this->_addr[1], this->_addr[2], 
				this->_addr[3], this->_addr[4], this->_addr[5]);
			return _str;
		}
		void clear(){
			memset(this->_addr, 0, sizeof(char[6]));	
		}
		int setOctet(int n, int num){
			if(n>=6){
				fprintf(stderr, "macaddr::setOctet(): index is not support\n");
				return -1;
			}else{
				this->_addr[n] = num;
			}
			return 1;
		}
		unsigned char getOctet(int n){
			if(n>=6){
				fprintf(stderr, "ipaddr::operator[]: index is not support\n");
				exit(-1);
			}else{
				return this->_addr[n];		
			}
		}
		int setmacbydev(const char* ifname){
			char buf[256];
			if(pgen_getmacbydev(ifname, buf) < 0){
				printf("error \n");
				return -1;
			}
			*this = buf;
			return 1;
		}
		int setmacbroadcast(){
			for(int i=0; i<6; i++){
				_addr[i] = 0xff;
			}
			return 1;
		}
		char* bender(){
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
		macaddr& operator=(int n){
			for(int i=0; i<6; i++)  this->_addr[i] = n;
			return *this;
		}
		macaddr& operator=(const macaddr m){
			for(int i=0; i<6; i++)	this->_addr[i] = m._addr[i];
			return *this;
		}
		macaddr& operator=(const char* str){
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
		macaddr& operator=(std::string str){
			*this = str.c_str();
			return *this;
		}
		bool operator<(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(this->_addr[i] == iaddr._addr[i])	continue;
				else return this->_addr[i] < iaddr._addr[i];
			}
			return false;
		}
		bool operator>(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(this->_addr[i] == iaddr._addr[i])	continue;
				else return this->_addr[i] > iaddr._addr[i];
			}
			return false;
		}
		bool operator<=(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(this->_addr[i] == iaddr._addr[i])	continue;
				else return this->_addr[i] <= iaddr._addr[i];
			}
			return false;
		}
		bool operator>=(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(this->_addr[i] == iaddr._addr[i])	continue;
				else	return this->_addr[i] >= iaddr._addr[i];
			}
			return false;
		}
		bool operator==(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(this->_addr[i] != iaddr._addr[i])	return false;
			}
			return true;
		}
		bool operator!=(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(this->_addr[i] != iaddr._addr[i])	return true;
			}
			return false;
		}
};





#endif /* ADDRESS_H */
