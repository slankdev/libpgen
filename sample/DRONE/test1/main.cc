#include <pgen.h>

const char* file = "file.pcap";

pgen_t* whandle;

bool func(const u_char* p,  int len){
	static int count=0;
	printf("%06d  ", count);
	count++;
	pgen_ardrone pack(p, len);
	pack.summary();
	//pack.send_handle(whandle);

	return true;
}


int main(){
	/*
	whandle = pgen_open_offline("write.pcap", 1);
	if(whandle == NULL){
		return -1;	
	}
	*/
	pgen_t* rhandle;
	rhandle = pgen_open_offline(file, 0);
	if(rhandle == NULL){
		return -1;	
	}

	sniff(rhandle, func);


	pgen_close(rhandle);
	//pgen_close(whandle);
}
