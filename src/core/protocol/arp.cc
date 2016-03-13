


#include <pgen2/core/protocol/arp.h>
#include <pgen2/exception.h>
#include <string>
#include <arpa/inet.h>


namespace pgen {




struct arp_struct {
    uint16_t hwtype;
    uint16_t ptype;
    uint8_t  hwlen;
    uint8_t  plen;
    uint16_t operation;
    uint8_t  hwsrc[6];
    uint8_t  psrc[4];
    uint8_t  hwdst[6];
    uint8_t  pdst[4];
};


void arp_header::clear() {
    hwtype    = 0x0001;
    ptype     = 0x0800;
    hwlen     = 6;
    plen      = 4;
    operation = pgen::arp::operation::request;
    hwsrc.clear();
    psrc.clear();
    hwdst = "ff:ff:ff:ff:ff:ff";
    pdst.clear();
}

void arp_header::summary(bool moreinfo) const {
    printf("ARP [");
    if (operation == 1) {
        printf("Who has %s? Tell %s", pdst.str().c_str(), hwsrc.str().c_str());
    } else if (operation == 2) {
        printf("%s is at %s", psrc.str().c_str(), hwsrc.str().c_str());
    } else {
        printf("unknown arp operation");
    }
    printf("]\n");

    if (moreinfo) {
        printf(" - hardware type : 0x%04x \n", hwtype);
        printf(" - protocol type : 0x%04x \n", ptype);
        printf(" - hardware len  : %d \n", hwlen);
        printf(" - protocol len  : %d \n", plen);
        printf(" - operation     : ");
        if (operation == pgen::arp::operation::request) 
            printf("request(%d)\n",    operation);
        else if (operation == pgen::arp::operation::reply) 
            printf("reply(%d)\n", operation);
        else printf("unknown(%d)\n", operation);
        printf(" - sender hw addr: %s \n", hwsrc.str().c_str());
        printf(" - sender p  addr: %s \n",  psrc.str().c_str());
        printf(" - target hw addr: %s \n", hwdst.str().c_str());
        printf(" - target p  addr: %s \n",  pdst.str().c_str());
    }
}

void arp_header::write(void* buffer, size_t bufferlen) const {
    if (bufferlen < min_length) {
        throw pgen::exception("pgen::arp_header::write: bufferlen is too small");
    }

    struct arp_struct* p = (arp_struct*)buffer;
    p->hwtype = htons(hwtype);
    p->ptype  = htons(ptype);
    p->hwlen  = hwlen;
    p->plen   = plen;
    p->operation = htons(operation);
    hwsrc.copytoarray(p->hwsrc);
    psrc.copytoarray(p->psrc);
    hwdst.copytoarray(p->hwdst);
    pdst.copytoarray(p->pdst);
}

void arp_header::read(const void* buffer, size_t bufferlen) {
    if (bufferlen < min_length) {
        throw pgen::exception("pgen::arp_header::read: bufferlen is too small");
    }

    struct arp_struct* p = (arp_struct*)buffer;
    hwtype = ntohs(p->hwtype);
    ptype  = ntohs(p->ptype);
    hwlen  = p->hwlen;
    plen   = p->plen;
    operation = ntohs(p->operation);
    hwsrc.setbyarray(p->hwsrc);
    psrc.setbyarray(p->psrc);
    hwdst.setbyarray(p->hwdst);
    pdst.setbyarray(p->pdst);
}

size_t arp_header::length() const {
    /* 
     * TODO hard coding about pgen::arp_header::length()
     * This function is hard coded. if not ethernet and ip's ARP,
     * packet length is not same to usualy ARP packet.
     */
    return min_length;
}






arp::arp() {
    clear();
    init_headers();
}


arp::arp(const pgen::arp& rhs) : arp() {
    ETH = rhs.ETH;
    ARP = rhs.ARP;
    init_headers();
}


arp::arp(const void* buffer, size_t bufferlen) : arp() {
    analyze(buffer, bufferlen);
}




void arp::clear() {
    ETH.clear();
    ETH.type = pgen::ethernet::type::arp;
    ARP.clear();
}

void arp::init_headers() {
    headers = {&ETH, &ARP};
}



} /* namespace pgen */

