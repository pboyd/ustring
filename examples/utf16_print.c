#include <stdio.h>
#include <ustring.h>

enum byte_order read_bom(FILE *in, uint8_t *raw);
uint32_t read_rune(FILE *in, enum byte_order bo);

int main(int argc, char **argv) {
    uint32_t r;
    uint8_t start[2];
    enum byte_order bo;
   
    bo = read_bom(stdin, &start[0]);
    if (bo == -1) {
        // Not a byte order mark. Default to little endian and print the non-BOM.
        bo = little_endian;
        printf("U+%04x\n", utf16_decode(&start[0], bo, NULL));
    }

    for (;;) {
        r = read_rune(stdin, little_endian);
        if (r == 0) {
            break;
        }

        printf("U+%04x\n", r);
    }

    return 0;
}

enum byte_order read_bom(FILE *in, uint8_t *raw) {
    size_t len = fread(&raw[0], 1, 2, in);
    if (len < 2) {
        return little_endian;
    }

    return utf16_decode_bom(&raw[0]);
}

uint32_t read_rune(FILE *in, enum byte_order bo) {
    uint8_t buf[4];
    size_t len;

    len = fread(&buf[0], 1, 2, in);
    if (len < 2) {
        return 0;
    }

    if (utf16_surrogate(&buf[0], bo) == 1) {
        len = fread(&buf[2], 1, 2, in);
        if (len < 2) {
            return UNICODE_REPLACEMENT;
        }
    }

    return utf16_decode(&buf[0], bo, NULL);
}
