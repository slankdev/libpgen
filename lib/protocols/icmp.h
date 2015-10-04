
#ifndef ICMP_H
#define ICMP_H

#include "ip.h"

class pgen_icmp : public pgen_ip {
	protected:
		struct icmp_header icmp;
		bit8  icmp_data[256];
		bit32 icmp_data_len;
		bit8  icmp_ext_data[256];
		bit32 icmp_ext_data_len;
	public:
		static const int minLen = pgen_ip::minLen+ICMP_HDR_LEN;
		static const int maxLen = PGEN_MAX_PACKET_LEN;
		struct{	
			int type;
			int code;
			
			struct{
				int id;
				int seq;
			}echo;
			struct{
				ipaddr gw_addr;	
			}redirect;
			struct{
				bit8 len;
				bit16 next_mtu;
			}destination_unreach;
			struct{
				bit8 len;
			}time_exceeded;


		}ICMP;
		
		pgen_icmp();
		pgen_icmp(const void*, int);
		void clear();
		void compile();
		void cast(const void*, int);
		void send(const char* ifname){send_L3(ifname);}
		void summary();
		void info();
		void help();

		void icmp_addData(const void*, int);
};


#endif
