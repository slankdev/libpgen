

#include <pgen2/core/header.h>
#include <pgen2/core/ipaddress.h>
#include <pgen2/core/protocol/ethernet.h>
#include <pgen2/core/packet.h>

namespace pgen {
    


class ipv6_header : public header {
    public:
        static const size_t min_length = 40;
        static const size_t max_length = 40;

    public:

        /**
         * Member Values.
         **/
        uint32_t version:4;
        uint32_t trafic_class:8;
        uint32_t flow_level:20; //can't use yet
        uint16_t payload_len;
        uint8_t  next_header;
        uint8_t  hop_limit;
        ipv6address src;
        ipv6address dst;

        
        void clear() override;
        void summary(bool moreinfo=false) const override;
        void write(void* buffer, size_t bufferlen) const override;
        void read(const void* buffer, size_t bufferlen) override;
        size_t length() const override;

};

class ipv6 : public packet {
    private:
        void init_headers() override;
    public:
        struct protocol {
            static const uint8_t icmp = 1;
            static const uint8_t tcp  = 6;
            static const uint8_t udp  = 17;
        };

        pgen::ethernet_header ETH;
        pgen::ipv6_header IP;

        ipv6();
        ipv6(const void* buffer, size_t bufferlen);
        ipv6(const pgen::ipv6& rhs);
        // pgen::ipv6& operator =(const pgen::ipv6& rhs); //ERASE

        void clear() override;
};


} /* namespace pgen */
