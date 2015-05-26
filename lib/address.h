#ifndef PGEN_VARIABLE
#define PGEN_VARIABLE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "pgen.h"
#include "pgen-funcs.h"

#define FILEPATH "/home/slank/git/libpgen/lib/"


static const char* getbender(const char* mac);




class ipaddr{
	private:
		
	public:
		u_int32_t _addr;		
		ipaddr(){}
		ipaddr(const char* str){
			_addr = inet_addr(str);
		}
		ipaddr(const ipaddr &i){
			_addr = i._addr;
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
};




class macaddr{
	private:
	public:
		u_char _addr[6];
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
		char* c_str(){
			char* str = (char*)malloc(sizeof(char)*19);
			snprintf(str,sizeof(char[18]),"%02x:%02x:%02x:%02x:%02x:%02x",
				_addr[0], _addr[1], _addr[2], _addr[3], _addr[4], _addr[5]);
			return str;
		}
		char* bender(){
			FILE *fp;
			const char* filename = FILEPATH;
			unsigned int  mac[3];
			char buf[256];
			char* bender = (char*)malloc(sizeof(char) * 256);
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
					return bender;
				}
				memset(mac, 0, sizeof(mac));
				memset(bender, 0, sizeof(bender));
				memset(buf, 0, sizeof(buf));
			}fclose(fp);
			strcpy(bender, "not-found");
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
		bool operator==(const ipaddr iaddr){
			for(int i=0; i<6; i++)
				if(_addr[i] != _addr[i])	return false;
			return true;
		}
};





#endif
