
#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"



pgen_unknown::pgen_unknown(){
	
}


bool pgen_unknown::isETH(){}
bool pgen_unknown::isARP(){}
bool pgen_unknown::isIP(){}
bool pgen_unknown::isICMP(){}
bool pgen_unknown::isTCP(){}
bool pgen_unknown::isUDP(){}
