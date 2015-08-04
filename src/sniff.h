#ifndef SNIFF_H
#define SNIFF_H

void sniff(const char* dev, bool (*callback)(const u_char*, int));
void sniff(const char* dev, bool (*callback)(const u_char*, int), int promisc);

#endif
