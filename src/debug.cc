#include <stdio.h>


void debugprint(int flag, const char* str){
	if(flag)
		printf("%s", str);
}
