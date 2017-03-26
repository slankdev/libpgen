
#include <pgen/core.h>
#include <vector>
#include <slankdev/util.h>

int main()
{
  pgen::ethernet_packet<std::vector<uint8_t>> pack;
  pack.ETH.set_src("00:00:00:00:00:00");
  pack.ETH.set_dst("00:00:00:00:00:11");
  pack.ETH.set_type(0x0800);
  pack.ETH[100] = 0xff;

  slankdev::hexdump("", pack.data(), pack.length());
}

int main()
{

  /*
   * Craft Packet
   */
  std::vector<uint8_t> raw;
  pgen::packet<std::vector<uint8_t>> pack(raw);
  pack.push_netstack(new pgen::ethernet);
  pack.push_netstack(new pgen::ipv4);
  pack.push_netstack(new pgen::udp);

  enum {
    ETH = 1,
    IP  = 2,
    UDP = 3,
  };

  pack.header[ETH]->set_src("11:11:11:11:11:11");
  pack.header[UDP]->set_sport(53);


  /*
   * Analyze Packet
   */
  uint8_t buf[10000];
  size_t len = pack_recv(buf, sizeof(buf));

  pgen::raw_container cont(buf, len);
  pgen::packet<pgen::raw_container> pack(cont);

  proto p = pack.get_layer(1);
}



class ether_header {
  ether_addr dst;
  ether_addr src;
  uint16_t type;
 publi:
  void set_src(...) {}
  void set_dst(...) {}
  void set_type(...) {}
  ether_addr get_src() {}
  ether_addr get_src() {}
  uint16_t   get_type() {}
  virtual size_t len() override { return 14; }
};

class ip_header {
  ...;
  ...;
  uint8_t proto;
  ip_addr src;
  ip_addr dst;
 public:
  void get_dddd();
  ...;
};

class packet {
  ether_header *eh;
  ipv4_header     *ih;
  udp_header      *uh;
};
