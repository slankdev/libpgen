

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pgen2/io/util.h>
#include <pgen2/arch/arch.h>
#include <pgen2/exception.h>




namespace pgen {



namespace util {

size_t send_to_netif(int fd, const void* buffer, size_t bufferlen) {
    int sent_len = write(fd, buffer, bufferlen);
    if (sent_len < 0) {
        std::string errmsg = "pgen::util::send_to_netif:write: ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }
    return (size_t)sent_len;   
}




/*
 * HELP
 * read system call returns -1, 3rd argument, bufferlen is not 4096.
 * It was hard-coded in pgen::util::open_bpf's set buffer.
 * I think that this hard-coding is reason... but MENDOKUSAI...
 *
 * BUG
 * This function can't work correct. find and fix bug.
 */
size_t recv_from_netif(int fd, void* buffer, size_t bufferlen) {
    int recved_len = read(fd, buffer, bufferlen);
    if (recved_len < 0) {
        printf("recved_len is %d \n", recved_len);
        std::string errmsg = "pgen::util::recv_from_netif:read: ";
        errmsg += strerror(errno);
        throw pgen::exception(errmsg);
    }
    return (size_t)recved_len;
}





int open_netif(const char* ifname) {
#if defined(__PGEN_OSX)
    return pgen::arch::open_bpf(ifname);
#elif defined(__PGEN_LINUX)
    return pgen::arch::open_rawsock(ifname);
#else
#error "pgen::arch: unknown architecture"
#endif
}




void close_netif(int fd) {
    close(fd);   
}


} /* namespace util */


void hex(const void* buffer, size_t bufferlen) { 

    printf("hexdump len: %lu \n", bufferlen);

    const uint8_t* data = (const uint8_t*)buffer;

    size_t row=0;
    size_t column=0;
    for(row=0; (row+column)<16+bufferlen; row+=16){
        for(column=0; column<=15; column++){
            if(!(row+column < bufferlen)){
                printf("   ");
                if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
            }else{
                if((row+column)%16 == 0) printf("%04x:    ", (unsigned int)(row+column));
                if((row+column)%8  == 0 && (row+column)%16 != 0) printf(" ");
                printf("%02x ", data[row+column]);
            }
        }

        for(column=0; column<=15; column++){
            if(!(row+column < bufferlen)){
                printf(" ");
            }else{
                if((row+column)%16 == 0) 
                    printf("  ");
                if((row+column)%8 == 0 && (row+column)%16!=0) 
                    printf("  ");
                
                if(0x20<=data[row+column] && data[row+column]<=0x7E)
                    printf("%c", data[row+column]);
                else
                    printf(".");
                
                if((row+column+1)%16 == 0)  
                    printf("\n");
            }
        }
    }
    printf("\n");
}



} /* namespace pgen */




