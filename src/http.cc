
#include "pgen.h"
#include "packet.h"
#include "address.h"
#include "netutil.h"

#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>


pgen_http::pgen_http(){
	clear();	
}

pgen_http::pgen_http(const void* packet, int len){
	clear();
	cast(packet, len);
}


void pgen_http::clear(){}

void pgen_http::compile(){}

void pgen_http::cast(const void* packet, int len){}

void pgen_http::summary(){}
void pgen_http::info(){}
