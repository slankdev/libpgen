
#pragma once


#include <stddef.h>
#include <pgen2/io/stream.h>


namespace pgen {



struct pcapng_type {
    static const uint32_t SHB = 0x0a0d0d0a;/**< Section Header block        */
    static const uint32_t IDB = 0x00000001;/**< interface description block */
    static const uint32_t SPB = 0x00000003;/**< Simple Packet block         */
    static const uint32_t NRB = 0x00000004;/**< Name Resorution Block       */
    static const uint32_t ISB = 0x00000005;/**< Interface Statistic block   */
    static const uint32_t EPB = 0x00000006;/**< Enhanced Packet block       */
};



/**
 * pcapng_block abstract class provide PcapNg Blocks.
 * Developers can implement class of new Block types
 * when need new block class.
 **/
class pcapng_block {
    private:
        void read_head(const void* buffer, size_t bufferlen);
        void read_option(const void* buffer, size_t bufferlen);
        void read_tail(const void* buffer, size_t bufferlen);
        void write_head(void* buffer, size_t bufferlen) const;
        void write_option(void* buffer, size_t bufferlen) const;
        void write_tail(void* buffer, size_t bufferlen) const;

    protected:
        virtual size_t impl_length() const = 0;
        virtual void read_body(const void* buffer, size_t bufferlen) = 0;
        virtual void write_body(void* buffer, size_t bufferlen) const = 0;

    public:
        uint32_t type;
        uint32_t total_length;
        std::vector<uint8_t> option;

        void read(const void* buffer, size_t bufferlen);
        void write(void* buffer, size_t bufferlen) const;
        virtual void summary(bool moreinfo=true) const = 0;
};



class pcapng_SHB : public pcapng_block {
    protected:
        size_t impl_length() const override;
        void read_body(const void* buffer, size_t bufferlen) override;
        void write_body(void* buffer, size_t bufferlen) const override;

    public:
        uint32_t magic;
        uint16_t version_major;
        uint16_t version_minor;
        uint32_t section_length[2];

        pcapng_SHB();
        void summary(bool moreinfo=true) const override;
};



class pcapng_IDB : public pcapng_block {
    protected:
        size_t impl_length() const override;
        void read_body(const void* buffer, size_t bufferlen) override;
        void write_body(void* buffer, size_t bufferlen) const override;

    public:
        uint16_t link_type;
        uint16_t reserved;
        uint32_t snap_length;

        pcapng_IDB();
        void summary(bool moreinfo=true) const override;
};



class pcapng_EPB : public pcapng_block {
    protected:
        size_t impl_length() const override;
        void read_body(const void* buffer, size_t bufferlen) override;
        void write_body(void* buffer, size_t bufferlen) const override;

    public:
        uint32_t interface_id;
        uint32_t timestamp_high;
        uint32_t timestamp_low;
        uint32_t capture_length;
        uint32_t packet_length;

        const uint8_t* packet_data_pointer;
        void set_packet(const void* packet, size_t packetlen);
    
        pcapng_EPB();
        void summary(bool moreinfo=true) const override;
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
