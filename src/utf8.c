#include <ustring.h>

rune utf8_decode(byte *buf, size_t *size) {
    rune code;
    int i;
    byte b;

    *size = utf8_len(*buf);
    if (*size == 0) {
        *size = 1;
        return 0xfffd;
    }

    if (*size == 1) {
        return (rune)*buf;
    }

    code = (*buf & 0x7f >> *size);

    for (i = 1; i < *size; i++) {
        b = *(buf+i);

        // Make sure the two high bits are 1 and 0 respectively.
        if ((b>>6) != 2) {
            *size = i+1;
            return 0xfffd;
        }

        // There are 6 bits of the code point in this byte. So shift
        // everything to make room.
        code <<= 6;

        // Put the low 6 bits of b on the low 6 bits of char.
        code |= (b & 0xff >> 2);
    }

    return code;
}

size_t utf8_len(byte b) {
    if (b == 0) {
        return 0;
    }

    // Find the first 0 bit
    // 
    // 0b0xxxxxxx is 1 byte
    // 0b10xxxxxx is invalid
    // 0b110xxxxx is 2 bytes
    // 0b1110xxxx is 3 bytes
    // 0b11110xxx is 4 bytes
    for (size_t i = 0; i <= 4; i++) {
        if ((b&0x80>>i) == 0) {
            switch (i) {
            case 0:
                return 1;
            case 1:
                return 0;
            default:
                return i;
            }
        }
    }

    return 0;
}
