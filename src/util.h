

#include <stdio.h>
#include <stdint.h>
#include <vector>



char* getbenderbymac(const u_char data[6]);
void printIpaddr(unsigned int* addr);
void hexdump(const void* p, int len);
unsigned short checksum(const void* data, int len);
char* gettimestr();

