

#include <pgen2/io/pcapng_stream.h>
#include <pgen2/exception.h>
#include <pgen2/util.h>
#include <assert.h>



namespace pgen {







struct pcapng_block_head {
    uint32_t type;
    uint32_t total_length;
};

struct pcapng_block_tail {
    uint32_t total_length;
};


static pcapng_blocktype_t detect_pcapng_blocktype(
        const void* buffer, size_t bufferlen) {
    if (bufferlen < sizeof(struct pcapng_block_head)) {
        throw pgen::exception(
            "pgen::detect_pcapng_blocktype: bufferlen is too small");
    }
    const struct pcapng_block_head* p =
        reinterpret_cast<const pcapng_block_head*>(buffer);
    return p->type;
}

pcapng_block::pcapng_block() {
    type = 0;
    total_length = 0;
}


void pcapng_block::read_head(const void* buffer, size_t bufferlen) {
    if (bufferlen < sizeof(pcapng_block_head)) {
        throw pgen::exception(
             "pgen::pcapng_block::read: bufferlen is too small");
    }

    const struct pcapng_block_head* pcapng_head = 
        reinterpret_cast<const pcapng_block_head*>(buffer);
    type = pcapng_head->type;
    total_length = pcapng_head->total_length;
}


void pcapng_block::read_option(const void* buffer, size_t bufferlen) {
    size_t optlen = total_length - sizeof(pcapng_block_head) 
        - body_length() - sizeof(pcapng_block_tail);

    if (bufferlen < optlen) {
        throw pgen::exception(
                "pgen::pcapng_block::read: bufferlen is too small");
    }

    option.resize(optlen);
    memcpy(option.data(), buffer, option.size());
}


void pcapng_block::read_tail(const void* buffer, size_t bufferlen) {
    if (bufferlen < sizeof(pcapng_block_tail)) {
        throw pgen::exception(
                "pgen::pcapng_block::read: bufferlen is too small");
    }

    const struct pcapng_block_tail* pcapng_tail = 
        reinterpret_cast<const pcapng_block_tail*>(buffer);
    uint32_t total_length2   = pcapng_tail->total_length;
    if (total_length2 != total_length) {
        throw pgen::exception(
                "pgen::pcapng_block::read: totlen1 != totlen2");
    }
}


void pcapng_block::read(const void* buffer, size_t bufferlen) {

    const uint8_t* buffer_pointer = reinterpret_cast<const uint8_t*>(buffer);
    size_t buffer_length = bufferlen;
    
    read_head(buffer_pointer, buffer_length);
    buffer_pointer += sizeof(pcapng_block_head);
    buffer_length -= sizeof(pcapng_block_head);

    read_body(buffer_pointer, buffer_length);
    buffer_pointer += body_length();
    buffer_length -= body_length();

    read_option(buffer_pointer, buffer_length);
    buffer_pointer += option.size();
    buffer_length -= option.size();

    read_tail(buffer_pointer, buffer_length);
    buffer_pointer += sizeof(struct pcapng_block_tail);
    buffer_length -= sizeof(struct pcapng_block_tail);

    if (bufferlen - buffer_length != total_length) 
        throw pgen::exception("pgen::pcapng_block::read: length error");
}




void pcapng_block::write_head(void* buffer, size_t bufferlen) const {
    if (bufferlen < sizeof(pcapng_block_head)) {
        throw pgen::exception(
                "pgen::pcapng_block::write_head: bufferlen is too small");
    }

    struct pcapng_block_head* pcapng_head = 
        reinterpret_cast<pcapng_block_head*>(buffer);
    pcapng_head->type = type;
    pcapng_head->total_length = total_length;
}


void pcapng_block::write_option(void* buffer, size_t bufferlen) const {
    if (bufferlen < option.size()) {
        throw pgen::exception(
                "pgen::pcapng_block::write_option: bufferlen is too small");
    }

    memcpy(buffer, option.data(), option.size());
}


void pcapng_block::write_tail(void* buffer, size_t bufferlen) const {
    if (bufferlen < sizeof(pcapng_block_tail)) {
        throw pgen::exception(
                "pgen::pcapng_block::write_tail: bufferlen is too small");
    }

    struct pcapng_block_tail* pcapng_tail = 
        reinterpret_cast<pcapng_block_tail*>(buffer);
    pcapng_tail->total_length   = total_length;
}


void pcapng_block::write(void* buffer, size_t bufferlen) const {
    if (bufferlen < total_length) {
        throw pgen::exception(
                "pgen::pcapng_SHB::write: buffer length is too small");
    }

    uint8_t* buffer_pointer = reinterpret_cast<uint8_t*>(buffer);
    size_t buffer_length = bufferlen;

    write_head(buffer_pointer, buffer_length);
    buffer_pointer += sizeof(pcapng_block_head);
    buffer_length -= sizeof(pcapng_block_head);

    write_body(buffer_pointer, buffer_length);
    buffer_pointer += body_length();
    buffer_length -= body_length();

    write_option(buffer_pointer, buffer_length);
    buffer_pointer += option.size();
    buffer_length -= option.size();

    write_tail(buffer_pointer, buffer_length);
    buffer_pointer += sizeof(pcapng_block_tail);
    buffer_length -= sizeof(pcapng_block_tail);

    if (bufferlen - buffer_length != total_length) {
        throw pgen::exception("pgen::pcapng_block::write: length error");
    }
}









struct shb_struct {
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    uint32_t section_length[2];
};

pcapng_SHB::pcapng_SHB() {
    type = pgen::pcapng_type::SHB;
    total_length = 
        sizeof(struct pcapng_block_head) +
        sizeof(struct shb_struct) + 
        option.size() + 
        sizeof(struct pcapng_block_tail);
    magic = 0x1a2b3c4d;
    version_major = 0x0001;
    version_minor = 0x0000;
    section_length[0] = 0xffffffff;
    section_length[1] = 0xffffffff;
}

void pcapng_SHB::summary(bool moreinfo) const {
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

size_t pcapng_SHB::body_length() const {
    return sizeof(struct shb_struct);
}

void pcapng_SHB::read_body(const void* buffer, size_t bufferlen) {
    if (type != pgen::pcapng_type::SHB) {
        throw pgen::exception(
                "pgen::pcapng_SHB::read_body: this is not SHB");
    }
    if (bufferlen < body_length()) {
        throw pgen::exception(
                "pgen::pcapng_SHB::read_body: bufferlen is too small");
    }

    const struct shb_struct* shb = 
        reinterpret_cast<const shb_struct*>(buffer);
    magic             = shb->magic         ;
    version_major     = shb->version_major ;
    version_minor     = shb->version_minor ;
    section_length[0] = shb->section_length[0];
    section_length[1] = shb->section_length[1];
}


void pcapng_SHB::write_body(void* buffer, size_t bufferlen) const {
    if (bufferlen < body_length()) {
        throw pgen::exception(
                "pgen::pcapng_SHB::write_body: bufferlen is too small");
    }

    struct shb_struct* shb = reinterpret_cast<shb_struct*>(buffer);
    shb->magic             = magic;
    shb->version_major     = version_major;
    shb->version_minor     = version_minor;
    shb->section_length[0] = section_length[0];
    shb->section_length[1] = section_length[1];
}










struct idb_struct {
    uint16_t link_type;
    uint16_t reserved;
    uint32_t snap_length;
};



pcapng_IDB::pcapng_IDB() {
    type = pgen::pcapng_type::IDB;
    total_length = 
        sizeof(pcapng_block_head) +
        sizeof(idb_struct)        + 
        option.size()             + 
        sizeof(pcapng_block_tail);
    link_type = pgen::pcapng_IDB::link_types::ethernet;
    reserved = 0;
    snap_length = 0;
}



size_t pcapng_IDB::body_length() const {
    return sizeof(struct idb_struct);
}



void pcapng_IDB::summary(bool moreinfo) const {
    printf("Interface Description Block \n");
    if (moreinfo) {
        printf("link type: %02x \n", link_type);
        printf("reserved : %02x \n", reserved);
        printf("snap length: %04x \n", snap_length);
        printf("option: \n");
        pgen::hex(option.data(), option.size());
    }
}



void pcapng_IDB::read_body(const void* buffer, size_t bufferlen) {
    if (type != pgen::pcapng_type::IDB) {
        throw pgen::exception("pgen::pcapng_IDB::read: this is not IDB");
    }
    if (bufferlen < body_length()) {
        throw pgen::exception(
                "pgen::pcapng_IDB::read_body: bufferlen is too small");
    }

    const struct idb_struct* idb =
        reinterpret_cast<const idb_struct*>(buffer);
    link_type    = idb->link_type;
    reserved     = idb->reserved;
    snap_length  = idb->snap_length;
}



void pcapng_IDB::write_body(void* buffer, size_t bufferlen) const {
    if (bufferlen < body_length()) {
        throw pgen::exception(
                "pgen::pcapng_IDB::write_body: bufferlen is too small");
    }

    struct idb_struct* idb = reinterpret_cast<idb_struct*>(buffer);
    idb->link_type    = link_type   ;
    idb->reserved     = reserved    ;
    idb->snap_length  = snap_length ;
}






struct epb_struct {
    uint32_t interface_id;
    uint32_t timestamp_high;
    uint32_t timestamp_low;
    uint32_t capture_length;
    uint32_t packet_length;
};


pcapng_EPB::pcapng_EPB() {
    type = pgen::pcapng_type::EPB;
    total_length = 
        sizeof(pcapng_block_head) +
        sizeof(epb_struct)        +
        option.size()             +
        sizeof(pcapng_block_tail);
    interface_id   = 0;
    timestamp_high = 0;
    timestamp_low  = 0;
    capture_length = 0;
    packet_length  = 0;
}



size_t pcapng_EPB::body_length() const {
    size_t bodysize = sizeof(struct epb_struct) + capture_length;
    // [OR] return sizeof(struct epb_struct) + packet_length;
   
    int a = bodysize % sizeof(uint32_t);
    if (a != 0)
        bodysize += sizeof(uint32_t)-a;

    return bodysize;
}



void pcapng_EPB::summary(bool moreinfo) const {
    printf("Enhanced Packet Block \n");
    if (moreinfo) {
        printf("Not implemented yet \n");
    }
}



// TODO need test
void pcapng_EPB::read_body(const void* buffer, size_t bufferlen) {
    if (type != pgen::pcapng_type::EPB) {
        printf("0x%04x \n", type);
        throw pgen::exception("pgen::pcapng_EPB::read: this is not EPB");
    }
    if (bufferlen < sizeof(epb_struct)) {
        throw pgen::exception(
                "pgen::pcapng_IDB::read_body: bufferlen is too small");
    }

    const struct epb_struct* epb =
        reinterpret_cast<const epb_struct*>(buffer);
    interface_id   = epb->interface_id  ;
    timestamp_high = epb->timestamp_high;
    timestamp_low  = epb->timestamp_low ;
    capture_length = epb->capture_length;
    packet_length  = epb->packet_length ;

    if (bufferlen < body_length()) {
        throw pgen::exception(
                "pgen::pcapng_IDB::read_body: bufferlen is too small");
    }
    
    packet_data_pointer = 
        reinterpret_cast<const uint8_t*>(buffer) + sizeof(epb_struct);
}

void pcapng_EPB::write_body(void* buffer, size_t bufferlen) const {
    if (bufferlen < body_length()) {
       throw pgen::exception(
               "pgen::pcapng_IDB::write_body: bufferlen is too small");
    }

    struct epb_struct* epb = reinterpret_cast<epb_struct*>(buffer);
    epb->interface_id   = interface_id;
    epb->timestamp_high = timestamp_high;
    epb->timestamp_low  = timestamp_low;
    epb->capture_length = capture_length;
    epb->packet_length  = packet_length;

    memcpy(reinterpret_cast<uint8_t*>(buffer)+sizeof(epb_struct), 
            packet_data_pointer, capture_length);
}

void pcapng_EPB::set_packet(const void* packet, size_t packetlen) {
    packet_data_pointer = reinterpret_cast<const uint8_t*>(packet);

    capture_length = packetlen;
    packet_length  = packetlen;

    total_length += packetlen;
    int a = packetlen % sizeof(uint32_t);
    if (a != 0)
        total_length += (sizeof(uint32_t)-a);
}





pcapng_stream::pcapng_stream() {}
pcapng_stream::pcapng_stream(const char* name, pgen::open_mode mode) {
    open(name, mode);
}

void pcapng_stream::open(const char* name, pgen::open_mode mode) {
    switch (mode) {
        case pgen::open_mode::pcapng_write:
        {
            uint8_t tmp_buffer[1000];
            fopen(name, "wb");
            pcapng_SHB shb;
            shb.write(tmp_buffer, sizeof tmp_buffer);
            write(tmp_buffer, shb.total_length);
            pcapng_IDB idb;
            idb.write(tmp_buffer, sizeof tmp_buffer);
            write(tmp_buffer, idb.total_length);
            break;
        }
        case pgen::open_mode::pcapng_read:
        {
            fopen(name, "rb");
            //,,,
            break;
        }
        default:
        {
            throw pgen::exception(
                    "pgen::pcapng_stream::open: unknown mode \n");
            break;
        }
    }
}



void pcapng_stream::send(const void* buffer, size_t bufferlen) {
    pcapng_EPB epb;
    epb.set_packet(buffer, bufferlen);
    write_block(epb);
}

size_t pcapng_stream::recv(void* buffer, size_t bufferlen) {
    while (1) {
        if (feof()) {
            throw pgen::exception(
                    "pgen::pcapng_stream::recv: is end of file");
        }
        
        uint8_t tmp_buffer[1000];
        size_t blocklen = read_block(tmp_buffer, sizeof(tmp_buffer));
        pcapng_blocktype_t type = detect_pcapng_blocktype(tmp_buffer, blocklen);
        if (type == pcapng_type::EPB) {
            pcapng_EPB epb;
            epb.read(tmp_buffer, blocklen);
            if (bufferlen < epb.packet_length)
                epb.packet_length = bufferlen;
            memcpy(buffer, epb.packet_data_pointer, epb.packet_length);
            return epb.packet_length;
    
        } else if (type == pcapng_type::SPB) {
            throw pgen::exception(
                    "pgen::pcapng_stream::recv: not implement");
        } else {
            continue;
        }
    }
}

void pcapng_stream::write_block(pcapng_block& block) {
    std::vector<uint8_t> raw_data;
    raw_data.resize(block.total_length);
    block.write(raw_data.data(), raw_data.size());
    write(raw_data.data(), raw_data.size());
}

size_t pcapng_stream::read_block(void* buffer, size_t bufferlen) {
    uint8_t* buffer_pointer = reinterpret_cast<uint8_t*>(buffer);

    struct pcapng_block_head* head = 
        reinterpret_cast<pcapng_block_head*>(buffer_pointer);
    read(head, sizeof(pcapng_block_head));
    buffer_pointer += sizeof(pcapng_block_head);
    bufferlen -= sizeof(pcapng_block_head);

    uint8_t* body = reinterpret_cast<uint8_t*>(buffer_pointer);
    size_t bodylen = head->total_length 
        - sizeof(pcapng_block_head) - sizeof(pcapng_block_tail);
    read(body, bodylen);
    buffer_pointer += bodylen;
    bufferlen      -= bodylen;
    
    struct pcapng_block_tail* tail =
        reinterpret_cast<pcapng_block_tail*>(buffer_pointer);
    read(tail, sizeof(pcapng_block_tail));
    buffer_pointer += sizeof(tail);
    bufferlen -= sizeof(tail);

    if (head->total_length != tail->total_length) {
        pgen::exception(
                "pgen::pcapng_stream::read_block: read error");
    }
    return head->total_length;
}





} /* namespace pgen */
