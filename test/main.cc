
#include <pgen2.h>



void print(pgen::ip_header& ip) {
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



void test_eth() {
    pgen::ethernet pack;
    pack.ETH.src() = "11:22:33:44:55:66";
    pack.ETH.dst() = "11:22:33:44:55:66";
    pack.ETH.type() = 0xffff;
    pack.compile();

    pack.hex();
}



int main() {
    test_eth();


    return 0;
    pgen::ip_header ip;
    print(ip);
    printf("---------------------------------------\n");

    ip.hlen(0);
    ip.tos(0);
    ip.tot_len(0);
    ip.id(0);     
    ip.frag_off(0);
    ip.ttl(0);    
    ip.protocol(0);
    ip.check(0);
    ip.src("192.168.1.1");
    ip.dst("192.168.1.3");



    print(ip);

}










 
 

        //
        // uint8_t    
        // uint8_t    
        // uint16_t   
        // uint16_t   
        // uint16_t   
        // uint8_t    
        // uint8_t    
        // uint16_t   
        // ipaddress  
        // ipaddress  
