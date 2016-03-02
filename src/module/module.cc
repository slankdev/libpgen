

#include <pgen2/core/packet.h>
#include <pgen2/io/stream.h>
#include <string.h>
#include <stdio.h>



namespace pgen {
namespace module {



int check(pgen::packet* pack, const char* filename, 
                    void (*callback)(const void*,size_t)) {

    pgen::pcap_stream handle;
    handle.open(filename, pgen::open_mode::pcap_read);

    int count = 0; 
    while (1) {
        if (handle.eof())
            break;

        uint8_t data[10000];
        size_t datalen = handle.recv(data, sizeof(data));

        pack->analyze(data, datalen);
        pack->compile();
    
        if (datalen != pack->length()) {
            printf("binary operation havs length error \n");
            if (callback) callback(data, datalen);
        }else if (memcmp(pack->raw(), data, datalen) == 0) {
            printf("binary operation is successed\n");
        } else {
            printf("binary operation havs binary error \n");
            if (callback) callback(data, datalen);
        }
        count++;
    }
    return count;
}

 


} /* namespace module */
} /* namespace pgen */
