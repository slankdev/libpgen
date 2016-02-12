

#include <pgen2.h>

void show(const pgen::core::header::ethernet& e) {
    printf("Ethernet \n");
    printf(" - source      : %s \n", e.src().get_str().c_str());
    printf(" - destination : %s \n", e.dst().get_str().c_str());
    printf(" - type        : 0x%04x \n", e.type());
}

int main() {

    uint8_t buf[1000];
    size_t buflen;

    pgen::core::header::ethernet e;
    e.src() = "11:22:33:44:55:66";
    e.dst() = "aa:bb:cc:dd:ee:ff";
    e.type() = 0x1234;

    buflen = e.write_header(buf, sizeof(buf));
    pgen::hex(buf, buflen);
    show(e);

    buflen = e.read_header(buf, buflen);
    show(e);

}
