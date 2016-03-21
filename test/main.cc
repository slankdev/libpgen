


#include <iostream>
#include <pgen2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
const char* dev = "en0";
const char* out = "out.pcap";

using std::cout;
using std::endl;



int main() {
    try {
        FILE *fp = fopen("out.pcapng", "wb");
        uint8_t buf[1000];

        pgen::pcapng_SHB h;
        h.write(buf, sizeof buf);
        fwrite(buf, h.total_length, 1, fp );
        pgen::hex(buf, h.total_length);
    
        pgen::pcapng_IDB i;
        i.write(buf, sizeof buf);
        fwrite(buf, i.total_length, 1, fp );
        pgen::hex(buf, i.total_length);

        pgen::arp pack;
        pack.compile();

        pgen::pcapng_EPB e;
        e.set_packet(pack.raw(), pack.length());
        e.write(buf, sizeof buf);
        fwrite(buf, e.total_length, 1, fp );
        pgen::hex(buf, e.total_length);



        fclose(fp);

    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}



