#include <ustring.h>

#define HIGH_SURROGATE 0xd800
#define LOW_SURROGATE 0xdc00
#define SURROGATE_MASK 0x3f<<10

#define IS_HIGH_SURROGATE(value) ((value & SURROGATE_MASK) == HIGH_SURROGATE)
#define IS_LOW_SURROGATE(value) ((value & SURROGATE_MASK) == LOW_SURROGATE)

uint32_t read_word(uint8_t *buf, enum byte_order byte_order) {
    if (byte_order == big_endian) {
        return (uint32_t)((buf[0] << 8) | buf[1]);
    }

    return (uint32_t)((buf[1] << 8) | buf[0]);
}

void write_word(uint8_t *buf, enum byte_order byte_order, uint32_t r) {
    if (byte_order == big_endian) {
        buf[0] = r >> 8;
        buf[1] = r;
    } else {
        buf[0] = r;
        buf[1] = r >> 8;
    }
}

uint32_t utf16_decode(uint8_t *buf,  enum byte_order byte_order, size_t *size) {
    uint32_t w1 = read_word(buf, byte_order);

    if (size != NULL) *size = 2;

    if (IS_LOW_SURROGATE(w1)) {
        return UNICODE_REPLACEMENT;
    }

    if (!IS_HIGH_SURROGATE(w1)) {
        return w1;
    }

    uint32_t w2 = read_word(buf+2, byte_order);
    if (size != NULL) *size = 4;

    if (!IS_LOW_SURROGATE(w2)) {
        return UNICODE_REPLACEMENT;
    }

    uint32_t value = (w1&0x3ff) << 10;
    value |= (w2&0x3ff);
    value |= 0x10000;

    return value;
}

int utf16_surrogate(uint8_t *buf, enum byte_order byte_order) {
    uint32_t w = read_word(buf, byte_order);

    if (IS_HIGH_SURROGATE(w) || IS_LOW_SURROGATE(w)) {
        return 1;
    }

    return 0;
}

size_t utf16_rune_len(uint32_t r) {
    if (r > UNICODE_MAX) {
        return 0;
    }

    return r < 0x10000 ? 2 : 4;
}

int utf16_cmp(uint8_t *s1, uint8_t *s2, enum byte_order byte_order, size_t n) {
    int i = 0;
    uint32_t r1, r2;
    size_t step;

    if (n % 2 != 0) {
        n--;
    }

    do {
        r1 = utf16_decode(&s1[i], byte_order, &step);
        r2 = utf16_decode(&s2[i], byte_order, NULL);

        if (r1 < r2) {
            return -1;
        } else if (r1 > r2) {
            return 1;
        }

        if (r1 == 0 && r2 == 0) {
            // Two null words, stop looking at the string.
            break;
        } else if (r1 == 0) {
            // s1 has a null word, but s2 does not.
            return -1;
        } else if (r2 == 0) {
            // s2 has a null word, but s1 does not.
            return 1;
        }

        i += step;
    } while (n < 0 || i < n);

    return 0;
}

size_t utf16_encode(uint8_t *buf, enum byte_order byte_order, uint32_t r) {
    if (r > UNICODE_MAX) {
        r = UNICODE_REPLACEMENT;
    }

    if (r < 0x10000) {
        write_word(buf, byte_order, r);
        return 2;
    }

    r ^= 0x10000;
    write_word(&buf[0], byte_order, HIGH_SURROGATE | (r >> 10));
    write_word(&buf[2], byte_order, LOW_SURROGATE | (r & 0x3ff));
    return 4;
}

enum byte_order utf16_decode_bom(uint8_t *buf) {
    if (buf[0] == 0xff && buf[1] == 0xfe) {
        return little_endian;
    }
    if (buf[0] == 0xfe && buf[1] == 0xff) {
        return big_endian;
    }
    return -1;
}
