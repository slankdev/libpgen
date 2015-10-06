#include <pgen.h>

arptable at;

const char* dev = "en0";


bool func(const u_char* p, int len){
	at.learn(p, len);
	at.show();
	return true;
}


int main(){
	pgen_t* h = pgen_open(dev, NULL);
	if(h == NULL){
		pgen_perror("open");
		return -1;
	}
	at.sethandle(h);
	sniff(h, func);
}
