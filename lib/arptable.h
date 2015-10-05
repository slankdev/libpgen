

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
		pgen_t* handle;
	public:
		arptable();
		arptable(pgen_t*);
		void sethandle(pgen_t*);
		int  add(ipaddr, macaddr);
		int  del(ipaddr);
		int  get(ipaddr);
		int  learn(const void*, int len);
		void show();
		macaddr find(ipaddr);
};



#endif

