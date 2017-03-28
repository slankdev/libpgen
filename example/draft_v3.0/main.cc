
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <slankdev/mbuf.h>
#include <slankdev/net_header.h>
#include <slankdev/hexdump.h>
#include <slankdev/util.h>


void craft_packet()
{
  const char* str = "slankdev";
  slankdev::mbuf m;

  m.prepend(strlen(str));
  uint8_t* d = m.mtod();
  memcpy(d, str, strlen(str));

  m.prepend(sizeof(slankdev::udp));
  slankdev::udp* uh = reinterpret_cast<slankdev::udp*>(m.mtod());
  uh->src = htons(80);
  uh->dst = htons(53);
  uh->len = m.dlen();
  uh->cksum = htons(0x0000);

  m.prepend(sizeof(slankdev::ip));
  slankdev::ip* ih = reinterpret_cast<slankdev::ip*>(m.mtod());
  ih->ver   = 4;
  ih->ihl   = 5;
  ih->len   = htons(m.dlen());
  ih->proto = 0x11;
  ih->sum   = htons(0x0000);
  ih->src.s_addr = htonl(0xc0a8000a);
  ih->dst.s_addr = htonl(0xc0a80001);

  m.prepend(sizeof(slankdev::ether));
  slankdev::ether* eh = reinterpret_cast<slankdev::ether*>(m.mtod());
  eh->type = htons(0x0800);

  m.dump(stdout);
  printf("\n");
  eh->print();
  ih->print();
  uh->print();
}

void analyze_packet(const void* ptr, size_t len)
{
  UNUSED(ptr);
  UNUSED(len);
}


int main()
{
  craft_packet();
}
