#include <stdio.h>
#include <ustring.h>

uint32_t read_rune(FILE *);

int main(int argc, char **argv) {
    uint32_t r;

    for (;;) {
        r = read_rune(stdin);
        if (r == 0) {
            break;
        }

        printf("U+%04x\n", r);
    }

    return 0;
}

uint32_t read_rune(FILE *in) {
    uint8_t buf[4];
    size_t len, rlen;
    int c;

    c = fgetc(in);
    if (c == EOF) {
        return 0;
    }

    len = utf8_len(c);
    if (len < 2) {
        return (uint32_t)c;
    }

    buf[0] = c;
    rlen = fread(&buf[1], 1, len, in);
    if (rlen < len) {
        return UNICODE_REPLACEMENT;
    }

    return utf8_decode(buf, NULL);
}
