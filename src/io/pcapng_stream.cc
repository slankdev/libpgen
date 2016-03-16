

#include <pgen2/io/stream.h>
#include <pgen2/io/pcapng_stream.h>
#include <pgen2/exception.h>
#include <pgen2/util.h>



namespace pgen {


void pcapng_SHB::summary(bool moreinfo) {
    printf("Section Header Block \n");
    if (moreinfo) {
        printf("magic : %04x \n", magic);
        printf("version_minor: %04x \n", version_minor);
        printf("version_major: %04x \n", version_major);
        printf("section_length: %04x \n", section_length);
        printf("option: \n");
        pgen::hex(option.data(), option.size());
    }
}






} /* namespace pgen */
