

# Pgen-Module

This component has some functions and classes that were coded with Pgen-IO and Pgen-Core.  
Users can add a new module to Pgen-Module easily.  
For example, sniff was coded by slank(@slankdev) with pgen_arp class and Pgen-IO's functions.  

## sniff
This is function that recv binary with PGEN-discripter, and calls callback function. 

### Sample Code

	#include <pgen.h>

	bool callback(const u_char* p, int len){
		pgen_hex(p, len);
		return true;
	}

	int main(){
		pgen_t* handle = pgen_open_offline("test.pcap", 0);
		if(handle == NULL){
			pgen_perror("pgen_open_offline");
			return -1;
		}
		sniff(handle, callback);
	}


## pgen_arpbtl
This is a ARP-table class that coded with pgen_arp and Pgen-IO. 


### Sample Code

	#include <pgen.h>

	int main(int argc, char** argv){
		pgen_t* handle = pgen_open("wlan0", NULL);
		if(handle == NULL){
			pgen_perror("oops");
			return -1;
		}

		arptable at(handle);
		at.find("192.168.179.1");
		at.show();
	}
