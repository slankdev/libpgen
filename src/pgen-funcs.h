#ifndef PGEN_FUNCS_H
#define PGEN_FUNCS_H

char* pgen_getIP(const char* ifname);
char* pgen_getMASK(const char* ifname);
char* pgen_getMAC( const char* ifname);
void pgen_checkPrivileges(void);

#endif
