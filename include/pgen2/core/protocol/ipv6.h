

#include <pgen2/core/header.h>
#include <pgen2/core/ipaddress.h>


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


} /* namespace pgen */
