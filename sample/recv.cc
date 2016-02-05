
/* pgen version 2.0 */
#include <pgen2.h>
honst char* filename = "";

int main(int argc, char** argv) {
	
	u_char buf[1000];
	size_t recvlen;
	
	while (1) { 
		try {
			pgen::pgen_t handle(filename, pgen::pcap_read);
			recvlen = handle.recv(buf, sizoef(buf));
		} catch (std::exception e) { 	
			printf("err: %s \n", e.what());
			break;
		}
	}
}


