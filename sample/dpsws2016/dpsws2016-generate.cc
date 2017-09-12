
#include <pgen2.h>
#include "dpsws2016.h"

int main(int argc, char** argv)
{

  if (argc != 2) {
    fprintf(stderr, "Usage: %s pcapfile \n", argv[0]);
    return -1;
  }

  try {
    dpsws2016 pack;
    pack.compile();
    pgen::pcap_stream pcap(argv[1], pgen::open_mode::pcap_write);
    pcap << pack;
  } catch (std::exception& e) {
     printf("%s \n", e.what());
  }

  return 0;
}

