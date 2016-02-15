
#pragma once


// #include <pgen2/io/util.h>
#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/data_container.h>


namespace pgen {



class ethernet : public packet {
    private:
        
    public:
        void compile() override;
        pgen::ethernet_header ETH;

        ethernet();
        void clear() override;

        size_t header_length() const override;

        void analyze(const void* buffer, size_t buffer_len) override;
        void summary(bool moreinfo=false) const override;
};


} /* namespace pgen */

