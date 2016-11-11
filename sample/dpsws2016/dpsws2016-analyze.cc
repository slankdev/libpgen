
#include <pgen2.h>
#include <time.h>
#include "dpsws2016.h"

static const char* getnow()
{
  static char str[32];
  time_t now = time(NULL);
  struct tm* p = localtime(&now);
  sprintf(str, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);
  return str;
}

int main(int argc, char** argv)
{
  if (argc != 2) {
    fprintf(stderr, "Usage: %s pcapfile \n", argv[0]);
    return -1;
  }

  pgen::pcap_stream packet_stream(argv[1], pgen::open_mode::pcap_read);

  uint8_t buf[10000];
  size_t recvlen;
  try {
    recvlen = packet_stream.recv(buf, sizeof buf);
  } catch (std::exception& e) {
    return 0;
  }

  if ( is_dpsws2016_packet(buf, recvlen) ) {
    dpsws2016 pack(buf, recvlen);
    printf("[%s] 0x%04x: \n", getnow());
    printf("%s\n", (pack.DPSWS2016.print_str()).c_str() );
  }

  return 0;
}

