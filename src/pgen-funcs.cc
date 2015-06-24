
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <time.h>

#include "pgen.h"
#include "pgen-funcs.h"






void pgen_checkPrivileges(void){
	if(0 != getuid()){
		fprintf(stderr, "pgen : requirs root privileges\n");
		exit(-1);
	}
}


char* gettimestr(){
	char* str;
	time_t timer = time(NULL);
	struct tm *time;

	str = (char*)malloc(sizeof(char) * 256);
	memset(str, 0, sizeof str);
	time = localtime(&timer);
	strftime(str, 255, "%H:%M:%S", time);
	return str;
}
