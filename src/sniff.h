#ifndef SNIFF_H
#define SNIFF_H

struct sniff_filter{
	struct{
		bool ipsrc;
		bool ipdst;
		bool ethsrc;
		bool ethdst;
	}flag;
	ipaddr ipsrc;
	ipaddr ipdst;
	macaddr ethsrc;
	macaddr ethdst;
};


//void sniff(const char* dev, bool (*callback)(const u_char*, int));
void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc);
void sniff_with_filter(const char* dev, bool (*callback)(const u_char*, int), int promisc, struct sniff_filter* filter);

#endif
