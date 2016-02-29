
#pragma once

#include <pgen2/core/packet.h>
#include <pgen2/core/header/header.h>



namespace pgen {


class ethernet : public packet {
    private:
    public:
        pgen::ethernet_header ETH;

        ethernet();
        ethernet(const void* buffer, size_t bufferlen);

        size_t header_length() const override;
        void clear() override;
        void compile() override;
        void analyze(const void* buffer, size_t bufferlen) override;
        void summary(bool moreinfo=false) const override;
};


} /* namespace pgen */

