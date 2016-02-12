
#pragma once


// #include <assert.h>
// #include <pgen2/io/util.h>
#include <pgen2/core/packet.h>
#include <pgen2/core/header.h>
#include <pgen2/types/data_container.h>


namespace pgen {
namespace core {



class ethernet : packet {
    private:
        void compile() override;
        
    public:
        pgen::core::ethernet_header ETH;

        ethernet();
        void clear() override;

        size_t header_length() const override;

        void analyze(const void* buffer, size_t buffer_len) override;
        void summary(bool moreinfo=false) const override;
        void help() const override;
};


} /* namespace core */
} /* namespace pgen */

