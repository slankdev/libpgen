#include <pgen.h>

int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s interface \n", argv[0]);
		return -1;
	}

	arptable at(argv[1]);
	ht.find("192.168.179.1");
	at.show();
}
