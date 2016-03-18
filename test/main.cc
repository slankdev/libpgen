


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
        uint8_t buf[1000];

        pgen::pcapng_SHB h;
        cout << h.option.size() << endl;
        h.write(buf, sizeof buf);
    

        pgen::hex(buf, h.total_length);
        
        FILE *fp = fopen("out.pcapng", "wb");
        fwrite(buf, h.total_length, 1, fp );
        fclose(fp);




    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}



