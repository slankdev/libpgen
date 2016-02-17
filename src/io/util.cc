

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pgen2/io/util.h>
#include <pgen2/arch/arch.h>
#include <pgen2/exception.h>
#include <pgen2/types.h>




namespace pgen {



namespace util {


class file_fd {
    private:
        FILE* fd;
        pgen::open_mode mode;
    public:
        file_fd() : fd(NULL) {}
        FILE* get_fd() const {
            return fd;
        }
        void open(const char* filename, pgen::open_mode m) {
            mode = m;
            if (mode == pgen::open_mode::pcap_read) { 
                fd = fopen(filename, "rb");
            } else if (mode == pgen::open_mode::pcap_write) { 
                fd = fopen(filename, "wb");
            } else {
                throw pgen::exception("pgen::util::file_fd::open: unknown mode");
            }
            if (fd == NULL) {
                throw pgen::exception("pgen::util::file_fd ");
            }
        }
        size_t read(void* buffer, size_t bufferlen) {
            size_t read_num = fread(buffer, bufferlen, 1, fd);
            if (read_num != 1) {
                throw pgen::exception("pgen::util::file_fd::read: ");
            }
            return bufferlen;
        }
        size_t write(const void* buffer, size_t bufferlen) {
            size_t write_num = fwrite(buffer, bufferlen, 1, fd);
            if (write_num != 1) {
                throw pgen::exception("pgen::util::file_fd::write: ");
            }
            return bufferlen;
        }
        void flush() {
            fflush(fd);
        }
};


FILE*  open_pcap(const char* filename, pgen::open_mode mode) {
    file_fd Fd;
    Fd.open(filename, mode);

    if (mode == pgen::open_mode::pcap_read) {
        struct pcap_fhdr fh;
        Fd.read(&fh, sizeof(pcap_fhdr));
    
    } else if (mode == pgen::open_mode::pcap_write) {
        struct pcap_fhdr fh;
        fh.magic = 0xa1b2c3d4;
        fh.version_major = 0x0002;
        fh.version_minor = 0x0004;
        fh.timezone = 0x0000;
        fh.sigfigs  = 0x0000;
        fh.snaplen  = 0x0000ffff;
        fh.linktype = 0x00000001;

        Fd.write(&fh, sizeof(pcap_fhdr));
        Fd.flush();
        
    } else {
        throw pgen::exception("pgen::util::open_pcap: unknown mode");
    }
    
    return Fd.get_fd();
}




size_t send_to_pcap(FILE* fd, const void* buffer, size_t bufferlen) {

    
}










size_t send_to_netif(int fd, const void* buffer, size_t bufferlen) {
    int sent_len = write(fd, buffer, bufferlen);
    if (sent_len < 0) {
        throw pgen::exception("pgen::util::send_to_netif:write: ");
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
        throw pgen::exception("pgen::util::recv_from_netif:read: ")<<recved_len;
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




