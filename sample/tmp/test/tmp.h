
#include <pgen2.h>
#include <string>
#include <arpa/inet.h>


struct tmp_s {
    uint32_t id;
    uint16_t seq;
    uint16_t msglen;
};


class tmp_header : public pgen::header {
    public:
        static const size_t min_length = sizeof(uint8_t)*4;
        static const size_t max_length = min_length + 1000;

        uint32_t id;
        uint16_t seq;
        uint16_t msglen;
        std::string msg;

    public:
        void clear() override
        {
            id     = 0;
            seq    = 0;
            msglen = 0;
            msg    = "";
        }

        void summary(bool moreinfo=false) const override
        {
            printf("TMP[id=%d, seq=%d, msg=%s]", 
                    id, seq, msg);

            if (moreinfo) {
                printf(" - id    : %d \n", id);
                printf(" - seq   : %d \n", seq);
                printf(" - msglen: %d \n", msglen);
                printf(" - msg   : %s \n", msg.c_str());
            }
        }

        void write(void* buffer, size_t bufferlen) const override
        {
            if (bufferlen < min_length) {
                throw pgen::exception("pgen::tmp_header::write: buflen is too small");
            }

            uint8_t* buffer_point = reinterpret_cast<uint8_t*>(buffer);

            struct tmp_s* tmp_s = reinterpret_cast<struct tmp_s*>(buffer_point);
            tmp_s->id     = htonl(id);
            tmp_s->seq    = htons(seq);
            tmp_s->msglen = htons(msglen);
            buffer_point += sizeof(struct tmp_s);
            bufferlen    -= sizeof(struct tmp_s);

            if (bufferlen < msglen) {
                throw pgen::exception("pgen::tmp_header::write: buflen is too small");
            }
            memcpy(buffer_point, msg.c_str(), msg.size());
        }

        void read(const void* buffer, size_t bufferlen) override
        {
            if (bufferlen < min_length) {
                throw pgen::exception("pgen::tmp_header::read: buflen is too small");
            }

            const uint8_t* buffer_point = reinterpret_cast<const uint8_t*>(buffer);

            const struct tmp_s* tmp_s = reinterpret_cast<const struct tmp_s*>(buffer_point);
            id     = htonl(tmp_s->id    );
            seq    = htons(tmp_s->seq   );
            msglen = htons(tmp_s->msglen);
            buffer_point += sizeof(struct tmp_s);
            bufferlen    -= sizeof(struct tmp_s);

            if (bufferlen < msglen) {
                throw pgen::exception("pgen::tmp_header::write: buflen is too small");
            }

            char message[msglen+1];
            memset(message, 0, sizeof message);
            memcpy(message, buffer_point, msglen);
            msg = message;
        }

        size_t length() const override
        {
            return min_length + msglen;
        }
};



class tmp : public pgen::packet {
    private:
        void init_headers() override
        {
            headers = {&ETH, &IP, &UDP, &TMP};
        }

    public:

        tmp_header TMP;
        pgen::udp_header UDP;
        pgen::ipv4_header IP;
        pgen::ethernet_header ETH;

        tmp()
        {
            clear();
            init_headers();
        }

        tmp(const void* buffer, size_t bufferlen) : tmp()
        {
            analyze(buffer, bufferlen);
        }

        tmp(const tmp& rhs) : tmp()
        {
            TMP = rhs.TMP;
            UDP = rhs.UDP;
            IP  = rhs.IP ;
            ETH = rhs.ETH;
            init_headers();
        }

        void clear() override
        {
            ETH.clear();
            ETH.type = pgen::ethernet::type::ip;
            IP.clear();
            IP.protocol = pgen::ipv4::protocol::udp;
            IP.tot_len = IP.length() + UDP.length() + TMP.length();
            UDP.clear();
            UDP.src = 8888;
            UDP.dst = 8888;
            UDP.len = UDP.length() + TMP.length();
        }
};



