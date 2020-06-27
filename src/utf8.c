#include <ustring.h>

#define UNICODE_REPLACEMENT 0xFFFD

rune utf8_decode(byte *buf, size_t *size) {
    rune code;
    int i;
    byte b;

    if (*buf == 0) {
        *size = 1;
        return 0;
    }

    *size = utf8_len(*buf);
    if (*size == 1) {
        return (rune)*buf;
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

size_t utf8_len(byte b) {
    // The first 0 bit indicates the length:
    // 
    // 0b0xxxxxxx is 1 byte
    // 0b10xxxxxx is invalid
    // 0b110xxxxx is 2 bytes
    // 0b1110xxxx is 3 bytes
    // 0b11110xxx is 4 bytes
    byte mask = 0x80;
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
