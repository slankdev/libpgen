#include <pgen.h>

int main(int argc, char** argv){
	if(argc != 2){
		printf("Usage: %s interface \n", argv[0]);
		return -1;
	}
	pgen_t* handle = pgen_open(argv[1], NULL);
	if(handle == NULL){
		pgen_perror("oops");
		return -1;
	}

	arptable at(handle);
	at.find("10.128.4.1");
	at.show();
}
