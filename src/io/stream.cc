
#include <pgen2/io/stream.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



namespace pgen {


pgen::open_mode base_stream::mode() const {
    return _mode;
}


void base_stream::print_mode() const {
    switch (_mode) {
        case pgen::open_mode::pcap_read: {
            printf("pcap_read mode \n");
            break;
        } case pgen::open_mode::pcap_write: {
            printf("pcap_write mode \n");
            break;
        } case pgen::open_mode::pcapng_read: { 
            printf("pcapng_read mode \n");
            break;
        } case pgen::open_mode::pcapng_write: { 
            printf("pcapng_write mode \n");
            break;
        } case pgen::open_mode::netif: {
            printf("netif mode \n");
            break;
        } defult: {
            printf("unknown mode \n");
            break;
        }
    }
}







} /* namespace pgen */
