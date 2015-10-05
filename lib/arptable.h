

#ifndef PGEN_ARPTBL
#define PGEN_ARPTBL


#include "address.h"
#include <vector>
#include <thread>

typedef struct arpent arpent_t;

struct arpent{
	ipaddr ip;
	macaddr mac;
};



class arptable{
	private:
		std::vector<arpent_t> entry;
		char ifname[64];
	public:
		arptable(const char*);
		int  add(ipaddr, macaddr);
		int  del(ipaddr);
		int  get(ipaddr);
		macaddr find(ipaddr);
		void show();
};



#endif

