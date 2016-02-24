

#include <pgen2.h>



void print(pgen::ipv4_header& ip) {
    printf("Internet Protocol \n");
    printf(" - header len     : %d \n", (int)ip.hlen()    );
    printf(" - type of service: %d \n", (int)ip.tos()     );
    printf(" - total length   : %d \n", (int)ip.tot_len() );
    printf(" - identifer      : %d \n", (int)ip.id()      );
    printf(" - fragment offset: %d \n", (int)ip.frag_off());
    printf(" - time to leave  : %d \n", (int)ip.ttl()     );
    printf(" - protocol       : %d \n", (int)ip.protocol());
    printf(" - checksum       : %d \n", (int)ip.check()   );
    printf(" - source         : %s \n", ip.src().get_str().c_str());
    printf(" - destination    : %s \n", ip.dst().get_str().c_str());
}



int main() {
    try {
        char data[] = "1234";

        pgen::ipv4_header ip;
        ip.src("1.2.3.4");
        ip.dst("5.6.7.9");
        ip.hlen(6);
        ip.option(data, strlen(data));
        print(ip);


        uint8_t buf[1000];
        ip.write(buf, sizeof(buf));
        pgen::hex(buf, ip.length());

        pgen::ipv4_header test;
        test.read(buf, ip.length());
        
        memset(buf, 0, sizeof(buf));
        test.write(buf, sizeof(buf));
        pgen::hex(buf, test.length());


    
    } catch (std::exception& e) {
        printf("%s \n", e.what());
    }
}
