



#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <iostream>
#include <pgen2/util.h>




namespace pgen {


/* Thanks @herumi */
static uint16_t read_as_little_endian(const void* data)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(data);  
    return uint16_t(p[0]) | (uint16_t(p[1]) << 8);
}

/* Thanks @herumi, Isn't used yet */
static uint16_t read_as_big_endian(const void* data)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(data);  
    return uint16_t(p[1]) | (uint16_t(p[2]) << 8);
}



// TODO This function work currectry 
// only when architecture is little endian.
// add code about endian differences
uint16_t checksum(const void* data, size_t len) noexcept 
{
    uint32_t sum;
    const uint8_t* data_pointer = reinterpret_cast<const uint8_t*>(data);
    
    for (; len > 1; len-=2, data_pointer+=2) {
        uint16_t tmp = read_as_little_endian(data_pointer);
        sum += tmp;
    }
    
    // jinnsei_overflow...
    // interop mousikomi... 1000chars....orz
    if (len == 1) {
        fprintf(stderr, "pgen::checksum: byte length isn't even num\n");
        uint8_t tmp = *data_pointer;
        sum += tmp;
    }
    
    uint16_t overflowd = sum >> 16;
    sum = sum & 0x0000ffff;
    sum = sum + overflowd;
    
    return ~sum;
}


// uint16_t checksum(const void *data, size_t len) noexcept 
// {
//     uint32_t sum = 0; 
//     const uint16_t* _data = reinterpret_cast<const uint16_t*>(data);
//  
//     for (; len > 1; len -= 2) {
//         sum += *_data++;
//         if (sum & 0x80000000) 
//           sum = (sum & 0xffff) + (sum >> 16);
//     }
//  
//     if (len == 1) {
//         uint16_t i = 0;
//         *(uint8_t*)(&i) = *(uint8_t*)_data; // TODO malformed codes
//         sum += i;
//     }
//  
//     while (sum >> 16)
//         sum = (sum & 0xffff) + (sum >> 16);
//        
//     return ~sum;
// }





/* 
 * So smart code. Thank you @herumi 
 * I'll do my best more.
 */
void hex(const void *buffer, size_t bufferlen) noexcept {
    // printf("hexdump len: %zu \n", bufferlen); // ERASE
    const uint8_t *data = reinterpret_cast<const uint8_t*>(buffer);
    size_t row = 0;
    while (bufferlen > 0) {
        printf("%04zx:   ", row);
        size_t n = std::min<size_t>(bufferlen, 16);
        for (size_t i = 0; i < n; i++) {
            if (i == 8) printf(" ");
            printf(" %02x", data[i]);
        }
        for (size_t i = n; i < 16; i++) {
            printf("   ");
        }
        printf("   ");
        for (size_t i = 0; i < n; i++) {
            if (i == 8) printf("  ");
            uint8_t c = data[i];
            if (!(0x20 <= c && c <= 0x7e)) c = '.';
            printf("%c", c);
        }
        printf("\n");
        bufferlen -= n;
        data += n;
    }
}





void bin(const void* buffer, size_t bufferlen) noexcept {
    struct bits {
        uint8_t b0:1;
        uint8_t b1:1;
        uint8_t b2:1;
        uint8_t b3:1;
        uint8_t b4:1;
        uint8_t b5:1;
        uint8_t b6:1;
        uint8_t b7:1;
    };
    const uint8_t* p0 = reinterpret_cast<const uint8_t*>(buffer);
    for (size_t i=0; i<bufferlen; i++) {
        bits* b = (bits*)p0;
        printf("%d%d%d%d%d%d%d%d ", 
                b->b0, b->b1, b->b2, b->b3,
                b->b4, b->b5, b->b6, b->b7);
        p0++;
        if ((i+1) % 8 == 0) printf("\n");
    }
}




} /* namespace pgen */




