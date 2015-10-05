#include <pgen.h>

int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s interface \n", argv[0]);
		return -1;
	}

	arptable at(argv[1]);
	at.get("10.128.4.1");
	at.show();
}
