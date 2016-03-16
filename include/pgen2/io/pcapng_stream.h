
#pragma once


#include <stddef.h>
#include <pgen2/io/stream.h>


namespace pgen {



struct pcapng_type {
    static const uint32_t SHB = 0x0a0d0d0a;  /**< Section Header block        */
    static const uint32_t IDB = 0x00000001;  /**< interface description block */
    static const uint32_t SPB = 0x00000003;  /**< Simple Packet block         */
    static const uint32_t NRB = 0x00000004;  /**< Name Resorution Block       */
    static const uint32_t ISB = 0x00000005;  /**< Interface Statistic block   */
    static const uint32_t EPB = 0x00000006;  /**< Enhanced Packet block       */
};



/**
 * pcapng_block abstract class provide PcapNg Blocks.
 * Developers can implement class of new Block types
 * when need new block class.
 **/
class pcapng_block {
    public:
        uint32_t type;
        uint32_t total_length;

        virtual void summary(bool moreinfo=true) = 0;
        virtual void read(const void* buffer, size_t bufferlen) = 0;
        virtual void write(void* buffer, size_t bufferlen) = 0;
};




class pcapng_SHB : public pcapng_block{
    public:
        uint32_t magic;
        uint16_t version_major;
        uint16_t version_minor;
        uint32_t section_length;
        std::vector<uint8_t> option;

        void summary(bool moreinfo=true);
        void read(const void* buffer, size_t bufferlen);
        void write(void* buffer, size_t bufferlen);
};




class pcapng_stream : public file_stream {
    public:
        pcapng_stream();
        pcapng_stream(const char* name, pgen::open_mode mode);

        void open(const char* name, pgen::open_mode _mode);
        void close() noexcept;
        void send(const void* buf, size_t buflen);
        size_t recv(void* buf, size_t buflen);

        void write_block(const void* buffer, size_t bufferlen);
        void read_block(void* buffer, size_t bufferlen);
};


} /* namespace pgen */
