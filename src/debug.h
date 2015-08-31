#ifndef DEBUG_H
#define DEBUG_H

#include <sys/types.h>

void debugprint(int flag, const char* str);
void hexdump(const u_char* data, int len);


#endif /* DEBUG_H */
