#ifndef HTTP_H
#define HTTP_H


#include <tcp.h>



class pgen_http : public pgen_tcp {
	protected:
		
	public:
		static const int minLen = pgen_tcp::minLen;
		static const int macLen = PGEN_MAX_PACKET_LEN;
	struct{
		int a;
		struct{
			int method;
			char header[2][256]; 
		}request;
	}HTTP;

	pgen_http();
	pgen_http(const void*, int);
	void clear();
	void compile();
	void cast(const void*, int);
	void send(const char* ifname){send_L3(ifname);}
	void summary();
	void info();
	void help();
	
};




#endif
