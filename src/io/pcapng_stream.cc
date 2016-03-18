

#include <pgen2/io/stream.h>
#include <pgen2/io/pcapng_stream.h>
#include <pgen2/exception.h>
#include <pgen2/util.h>
#include <assert.h>



namespace pgen {


struct shb_struct1 {
    uint32_t type;
    uint32_t total_length;
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    uint32_t section_length[2];
};


struct shb_struct2 {
    uint32_t total_length;
};


pcapng_SHB::pcapng_SHB() {
    type = pgen::pcapng_type::SHB;
    total_length = option.size() + sizeof(struct shb_struct1) + sizeof(struct shb_struct2);
    magic = 0xa1b2c3d4;
    version_major = 0x0001;
    version_minor = 0x0000;
    section_length[0] = 0xffffffff;
    section_length[1] = 0xffffffff;
}



void pcapng_SHB::summary(bool moreinfo) {
    printf("Section Header Block \n");
    if (moreinfo) {
        printf("magic : %04x \n", magic);
        printf("version_minor: %04x \n", version_minor);
        printf("version_major: %04x \n", version_major);
        printf("section_length: 0x%04x%04x \n", 
                section_length[0],section_length[1]);
        printf("option: \n");
        pgen::hex(option.data(), option.size());
    }
}



void pcapng_SHB::read(const void* buffer, size_t bufferlen) {
    if (bufferlen < sizeof(struct shb_struct1)+sizeof(struct shb_struct2)) 
        throw pgen::exception("pgen::pcapng_SHB::read: buffer length is too small");

    const uint8_t* buffer_pointer = reinterpret_cast<const uint8_t*>(buffer);

    const struct shb_struct1* shb1 = reinterpret_cast<const shb_struct1*>(buffer_pointer);
    type           = shb1->type          ;
    if (type != pgen::pcapng_type::SHB)
        throw pgen::exception("pgen::pcapng_SHB::read: this is not SHB");

    total_length      = shb1->total_length  ;
    magic             = shb1->magic         ;
    version_major     = shb1->version_major ;
    version_minor     = shb1->version_minor ;
    section_length[0] = shb1->section_length[0];
    section_length[1] = shb1->section_length[1];
    buffer_pointer += sizeof(struct shb_struct1);

    option.resize(total_length);
    assert(total_length == option.size()); // TODO is this needed?
    memcpy(option.data(), buffer_pointer, total_length);
    buffer_pointer += option.size();

    const struct shb_struct2* shb2 = reinterpret_cast<const shb_struct2*>(buffer_pointer);
    total_length   = shb2->total_length;
    buffer_pointer += sizeof(struct shb_struct2);

    if (buffer_pointer - reinterpret_cast<const uint8_t*>(buffer) != total_length) 
        throw pgen::exception("pgen::pcapng_SHB::read: length error");
}



void pcapng_SHB::write(void* buffer, size_t bufferlen) const {
    if (bufferlen < sizeof(struct shb_struct1)+sizeof(struct shb_struct2)) 
        throw pgen::exception("pgen::pcapng_SHB::write: buffer length is too small");

    uint8_t* buffer_pointer = reinterpret_cast<uint8_t*>(buffer);

    struct shb_struct1* shb1 = reinterpret_cast<shb_struct1*>(buffer_pointer);
    shb1->type              = type;
    shb1->total_length      = total_length;
    shb1->magic             = magic;
    shb1->version_major     = version_major;
    shb1->version_minor     = version_minor;
    shb1->section_length[0] = section_length[0];
    shb1->section_length[1] = section_length[1];
    buffer_pointer += sizeof(struct shb_struct1);

    memcpy(buffer_pointer, option.data(), option.size());
    buffer_pointer += option.size();

    struct shb_struct2* shb2 = reinterpret_cast<shb_struct2*>(buffer_pointer);
    shb2->total_length   = total_length;
    buffer_pointer += sizeof(struct shb_struct2);

    if (buffer_pointer - reinterpret_cast<uint8_t*>(buffer) != total_length) {
        // ERASE
        // printf("bp - p:  %zd \n", buffer_pointer - reinterpret_cast<uint8_t*>(buffer));
        // printf("total_length:  %zd \n", total_length);
        throw pgen::exception("pgen::pcapng_SHB::write: length error");
    }
}






void pcapng_IDB::summary(bool moreinfo) {
    printf("Interface Description Block \n");
    if (moreinfo) {
        printf("link type: %02x \n", link_type);
        printf("reserved : %02x \n", reserved);
        printf("snap length: %04x \n", snap_length);
        printf("option: \n");
        pgen::hex(option.data(), option.size());
    }
}


struct idb_struct1 {
    uint16_t link_type;
    uint16_t reserved;
    uint32_t snap_length;
};



struct idb_struct2 {
    uint32_t total_length;
};










} /* namespace pgen */
