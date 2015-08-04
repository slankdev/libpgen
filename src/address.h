#ifndef PGEN_VARIABLE
#define PGEN_VARIABLE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>

#include "pgen.h"
#include "mytypes.h"

#define FILEPATH "/usr/local/etc/mac_code.list"


static const char* getbender(const char* mac);



class ipaddr{
	private:
		
	public:
		bit32 _addr;		
		
		ipaddr(){}
		ipaddr(const char* str){
			_addr = inet_addr(str);
		}
		ipaddr(const ipaddr &i){
			_addr = i._addr;
		}
		bit32 getbit(){
			return _addr;	
		}
		bool nslookup(const char* host){
			

			
			return true;
		}
		bool setipbydev(const char* ifname){
			int sockd;
			struct ifreq ifr;
			struct sockaddr_in *sa;
			char* ipstr;

			if ((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
				perror("socket()!");
				return false;
			}
			ifr.ifr_addr.sa_family = AF_INET;
			strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
			ioctl(sockd, SIOCGIFADDR, &ifr);
			close(sockd);
			sa = (struct sockaddr_in*)&ifr.ifr_addr;
			ipstr = inet_ntoa(sa->sin_addr);

			_addr = inet_addr(ipstr);
			return true;
		}
		bool setmaskbydev(const char* ifname){
			int sockd;
			struct ifreq ifr;
			struct sockaddr_in *sa;
			char* maskstr;

			if((sockd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
				perror("socket");
				return false;
			}
			ifr.ifr_addr.sa_family = AF_INET;
			strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
			ioctl(sockd, SIOCGIFNETMASK, &ifr);
			close(sockd);
			sa = (struct sockaddr_in*)&ifr.ifr_addr;
			maskstr = inet_ntoa(sa->sin_addr);

			_addr = inet_addr(maskstr);
			return true;
		}
		char* c_str(){
			char* str = (char*)malloc(sizeof(char)*16);
			union lc{
				unsigned int l;
				unsigned char c[4];
			};
			union lc lc;
			lc.l = (unsigned int)_addr;
			snprintf(str,sizeof(char[16]),"%u.%u.%u.%u",
							lc.c[0],lc.c[1],lc.c[2],lc.c[3]);
			return str;
		}
		bool isEmpty(){
			if(_addr == 0)	return true;
			else			return false;
		}

		ipaddr& operator=(const ipaddr i){
			//_addr = i._addr;
			_addr = i._addr;
			return *this;
		}
		ipaddr& operator=(const char* str){
			_addr = inet_addr(str);
			return *this;
		}
		ipaddr& operator=(const int num){
			_addr = num;
			return *this;
		}
		ipaddr& operator=(const bit8 num[4]){
			union lc{
				unsigned int l;
				unsigned char c[4];
			};
			union lc lc;
			
			for(int i=0; i<4; i++)
				lc.c[i] = num[i];

			_addr = lc.l;
			return *this;
		}

		u_char operator[](const int num){
			union lc{
				unsigned int l;
				unsigned char c[4];
			};
			union lc lc;
			lc.l = _addr;
			switch(num){
				case 0:
					return lc.c[0];
					break;
				case 1:
					return lc.c[1];
					break;
				case 2:
					return lc.c[2];
					break;
				case 3:
					return lc.c[3];
					break;
				default:
					fprintf(stderr, "pgen_ipaddr_t: contents not\n");
					break;
			}
		}
		bool operator<(const ipaddr iaddr){
			union lc{
				unsigned int l;
				unsigned char c[4];
			};
			union lc lc, ilc;
			lc.l = (unsigned int)_addr;
			ilc.l = (unsigned int)iaddr._addr;
			
			for(int i=0; i<4; i++){
				if(lc.c[i] == ilc.c[i])	continue;
				else					return lc.c[i] < ilc.c[i];
			}return false;
		}
		bool operator>(const ipaddr iaddr){
			union lc{
				unsigned int l;
				unsigned char c[4];
			};
			union lc lc, ilc;
			lc.l = (unsigned int)_addr;
			ilc.l = (unsigned int)iaddr._addr;
			for(int i=0; i<4; i++){
				if(lc.c[i] == ilc.c[i])	continue;
				else					return lc.c[i] > ilc.c[i];
			}return false;
		}
		bool operator==(const ipaddr iaddr){
			if(_addr == iaddr._addr)	return true;
			else						return false;
		}
		bool operator!=(const ipaddr iaddr){
			if(_addr != iaddr._addr)	return true;
			else						return false;
		}
};




class macaddr{
	private:
	public:
		bit8 _addr[6];
		macaddr(){}
		macaddr(const char* str){
			unsigned int buf[6];
			sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
				&buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
			for(int i=0; i<6; i++)	_addr[i] = (u_char)buf[i];
		}
		macaddr(const macaddr &m){
			for(int i=0; i<6; i++)
				_addr[i] = m._addr[i];
		}
		bool setmacbydev(const char* ifname){
			int sockd;
			struct ifreq ifr;
			char* macstr;
			u_char addr[6];

			if ((sockd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
				perror("socket()!");
				return false;
			}
			macstr = (char*)malloc(sizeof(char)*19);
			ifr.ifr_addr.sa_family = AF_INET;
			strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
			ioctl(sockd, SIOCGIFHWADDR, &ifr);
			close(sockd);
			for(int i=0; i<6; i++)
				addr[i] = (unsigned char)ifr.ifr_hwaddr.sa_data[i];
			snprintf(macstr ,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
				*addr, *(addr+1), *(addr+2), *(addr+3), *(addr+4), *(addr+5));

			*this = macstr;
			return true;
				
		}
		char* c_str(){
			char* str = (char*)malloc(sizeof(char)*19);
			snprintf(str,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
				_addr[0], _addr[1], _addr[2], _addr[3], _addr[4], _addr[5]);
			return str;
		}
		char* bender(){
			FILE *fp;
			const char* filename = FILEPATH;
			int strsize = 256;
			unsigned int  mac[3];
			char buf[strsize];
			char* bender = (char*)malloc(sizeof(char) * strsize);
			u_char mymac[3];
			if((fp=fopen(filename, "r")) == NULL){
				perror("getbenderbymac fopen");
				strcpy(bender, "error");
				return bender;
			}
			for(int i=0; i<3; i++)	mymac[i] = _addr[i];
			while(fgets(buf, sizeof(buf), fp) != NULL){
				sscanf(buf, "%2x%2x%2x\t%s", &mac[0],&mac[1],&mac[2],buf);
				if(mac[0]==mymac[0]&&mac[1]==mymac[1]&&mac[2]==mymac[2]){
					sprintf(bender, "%s", buf);
					fclose(fp);
					return bender;
				}
				memset(mac, 0, sizeof(mac));
				memset(bender, 0, strsize);
				memset(buf, 0, strsize);
			}
			strcpy(bender, "not-found");
			fclose(fp);
			return bender;
		}
		bool isEmpty(){
			for(int i=0; i<6; i++)
				if(_addr[i] != 0)	return false;
			return true;
		}
		macaddr& operator=(const macaddr m){
			for(int i=0; i<6; i++)	_addr[i] = m._addr[i];
			return *this;
		}
		macaddr& operator=(const char* str){
			unsigned int buf[6];
			sscanf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
				&buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]);
			for(int i=0; i<6; i++)	_addr[i] = (u_char)buf[i];
			return *this;
		}
		macaddr& operator=(const int num){
			for(int i=0; i<6; i++)	_addr[i] = (u_char)num;
			return *this;
		}
		unsigned char operator[](const int num){
			switch(num){
				case 0:
					return _addr[0];
					break;
				case 1:
					return _addr[1];
					break;
				case 2:
					return _addr[2];
					break;
				case 3:
					return _addr[3];
					break;
				case 4:
					return _addr[4];
					break;
				case 5:
					return _addr[5];
					break;
				default:
					fprintf(stderr, "pgen_ipaddr_t: contents not\n");
					break;
			}
		}
		bool operator<(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(_addr[i] == iaddr._addr[i])	continue;
				else							return _addr[i] < _addr[i];
			}return false;
		}
		bool operator>(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(_addr[i] == iaddr._addr[i])	continue;
				else							return _addr[i] > _addr[i];
			}return false;
		}
		bool operator==(const macaddr iaddr){
			for(int i=0; i<6; i++){
				if(_addr[i] != iaddr._addr[i]){
					return false;
				}
			}
			return true;
		}
		bool operator!=(const macaddr iaddr){
			for(int i=0; i<6; i++)
				if(_addr[i] == iaddr._addr[i])	return false;
			return true;
		}
};





#endif
