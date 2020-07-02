#include <ustring.h>

uint32_t utf8_decode(uint8_t *buf, size_t *size) {
    uint32_t code;
    int i;
    uint8_t b;

    if (size == NULL) {
        size_t dummy;
        size = &dummy;
    }

    if (*buf == 0) {
        *size = 1;
        return 0;
    }

    *size = utf8_len(*buf);
    if (*size == 1) {
        return (uint32_t)*buf;
    }

    code = (*buf & 0x7f >> *size);

    for (i = 1; i < *size; i++) {
        b = *(buf+i);

        // Make sure the two high bits are 1 and 0 respectively.
        if ((b>>6) != 2) {
            *size = i+1;
            return UNICODE_REPLACEMENT;
        }

        // There are 6 bits of the code point in this byte. So shift
        // everything to make room.
        code <<= 6;

        // Copy the lower six bits of the byte.
        code |= (b & 0xff >> 2);
    }

    return code;
}

size_t utf8_len(uint8_t b) {
    // The first 0 bit indicates the length:
    // 
    // 0b0xxxxxxx is 1 byte
    // 0b10xxxxxx is invalid
    // 0b110xxxxx is 2 bytes
    // 0b1110xxxx is 3 bytes
    // 0b11110xxx is 4 bytes
    uint8_t mask = 0x80;
    if ((b&mask) == 0) {
        return 1;
    }

    mask >>= 1;
    if ((b&mask) == 0) {
        // The byte is 0b10xxxxxx, which is invalid for the first byte.
        return 0;
    }

    for (size_t i = 2; i <= 4; i++) {
        mask >>= 1;
        if ((b&mask) == 0) {
            return i;
        }
    }

    return 0;
}

size_t utf8_rune_len(uint32_t r) {
    if (r < 0x80) {
        return 1;
    }

    if (r < 0x800) {
        return 2;
    }

    if (r < 0xffff) {
        return 3;
    }

    if (r < UNICODE_MAX) {
        return 4;
    }

    return 0;
}

size_t utf8_encode(uint8_t *buf, uint32_t r) {
    if (r < 0x80) {
        buf[0] = (uint8_t)r;
        return 1;
    }

    if (r < 0x800) {
        buf[0] = 0x80 | 0x40 | (uint8_t)(r>>6);
        buf[1] = 0x80 | (uint8_t)(r&0x3f);
        return 2;
    }

    if (r < 0xffff) {
        buf[0] = 0x80 | 0x40 | 0x20 | (uint8_t)(r>>12);
        buf[1] = 0x80 | (uint8_t)(r>>6&0x3f);
        buf[2] = 0x80 | (uint8_t)(r&0x3f);
        return 3;
    }

    if (r < UNICODE_MAX) {
        buf[0] = 0x80 | 0x40 | 0x20 | 0x10 | (uint8_t)(r>>18);
        buf[1] = 0x80 | (uint8_t)(r>>12&0x3f);
        buf[2] = 0x80 | (uint8_t)(r>>6&0x3f);
        buf[3] = 0x80 | (uint8_t)(r&0x3f);
        return 4;
    }

    return 0;
}
