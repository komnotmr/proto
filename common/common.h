#pragma once

/*
|byte order| message_len | fc       |len field_name  |field_name|len field_value|field_value|
|  1 or 0  |    4 bytes  |  4 bytes |4 bytes       | X bytes  |   4 bytes     |    x Bytes|
*/

#include <cstdint.h>
#include <stdlib.h>

typedef int socket_t;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define BYTE_ORDER 0
#else
    #define BYTE_ORDER 1
#endif

typedef struct buf {
    uint8_t *buf;
    int32_t len;
} buffer_t;

