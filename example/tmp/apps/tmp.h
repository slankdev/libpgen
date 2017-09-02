

#include <stdio.h>
#include <stdint.h>

#define SERV_PORT 8888

struct tmp_header {
    uint32_t id;
    uint16_t seq;
    uint16_t msg_len;
};
