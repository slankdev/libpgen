#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <map>


const char* fname = "mac_code.list";
const char* valname = "macDictionary";


// std::map<const char*, const char*> m;



int main(){
	FILE *fp;
	char buf[256];
	char mac[64];
	char bender[64];

	if((fp=fopen(fname, "r"))==NULL){
		exit(-1);	
	}

	while(fgets(buf, sizeof(buf), fp) != NULL){
		memset(mac, 0, sizeof mac);
		memset(bender, 0, sizeof bender);

		sscanf(buf, "%s\t%s", mac, bender);
		printf("%s[\"%s\"] = \"%s\"; \n", valname, mac, bender);
	}

		

	fclose(fp);		
}
