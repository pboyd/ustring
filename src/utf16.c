#include <ustring.h>

#define HIGH_SURROGATE 0xd800
#define LOW_SURROGATE 0xdc00
#define SURROGATE_MASK 0x3f<<10

#define IS_HIGH_SURROGATE(value) ((value & SURROGATE_MASK) == HIGH_SURROGATE)
#define IS_LOW_SURROGATE(value) ((value & SURROGATE_MASK) == LOW_SURROGATE)

rune read_word(byte *buf, enum byte_order byte_order) {
    if (byte_order == big_endian) {
        return (rune)((buf[0] << 8) | buf[1]);
    }

    return (rune)((buf[1] << 8) | buf[0]);
}

void write_word(byte *buf, enum byte_order byte_order, rune r) {
    if (byte_order == big_endian) {
        buf[0] = r >> 8;
        buf[1] = r;
    } else {
        buf[0] = r;
        buf[1] = r >> 8;
    }
}

rune utf16_decode(byte *buf,  enum byte_order byte_order, size_t *size) {
    rune w1 = read_word(buf, byte_order);

    if (size != NULL) *size = 2;

    if (IS_LOW_SURROGATE(w1)) {
        return UNICODE_REPLACEMENT;
    }

    if (!IS_HIGH_SURROGATE(w1)) {
        return w1;
    }

    rune w2 = read_word(buf+2, byte_order);
    if (size != NULL) *size = 4;

    if (!IS_LOW_SURROGATE(w2)) {
        return UNICODE_REPLACEMENT;
    }

    rune value = (w1&0x3ff) << 10;
    value |= (w2&0x3ff);
    value |= 0x10000;

    return value;
}

size_t utf16_rune_len(rune r) {
    if (r < 0 || r > UNICODE_MAX) {
        return 0;
    }

    return r < 0x10000 ? 2 : 4;
}

int utf16_cmp(byte *s1, byte *s2, enum byte_order byte_order, size_t n) {
    int i = 0;
    rune r1, r2;
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

size_t utf16_encode(byte *buf, enum byte_order byte_order, rune r) {
    if (r < 0 || r > UNICODE_MAX) {
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
