#include "CuTest.h"

#include <ustring.h>

#include <strings.h>

#define copy_string(dest, str, len) for (int i = 0; i < len; i++) dest[i] = str[i]

void test_utf16_decode(CuTest *tc) {
    rune actual;
    size_t size;
    byte buf[6];

    copy_string(buf, "\x00\x61", 2);
    actual = utf16_decode(buf, big_endian, &size);
    CuAssertIntEquals(tc, 0x61, actual);
    CuAssertIntEquals(tc, 2, size);

    copy_string(buf, "\x61\x00", 2);
    actual = utf16_decode(buf, little_endian, &size);
    CuAssertIntEquals(tc, 0x61, actual);
    CuAssertIntEquals(tc, 2, size);

    copy_string(buf, "\x26\x15", 2);
    actual = utf16_decode(buf, big_endian, &size);
    CuAssertIntEquals(tc, 0x2615, actual);
    CuAssertIntEquals(tc, 2, size);

    copy_string(buf, "\x15\x26", 2);
    actual = utf16_decode(buf, little_endian, &size);
    CuAssertIntEquals(tc, 0x2615, actual);
    CuAssertIntEquals(tc, 2, size);

    copy_string(buf, "\xd8\x3c\xdf\x0e", 4);
    actual = utf16_decode(buf, big_endian, &size);
    CuAssertIntEquals(tc, 0x1f30e, actual);
    CuAssertIntEquals(tc, 4, size);

    copy_string(buf, "\x3c\xd8\x0e\xdf", 4);
    actual = utf16_decode(buf, little_endian, &size);
    CuAssertIntEquals(tc, 0x1f30e, actual);
    CuAssertIntEquals(tc, 4, size);

    copy_string(buf, "\xdf\x0e", 2);
    actual = utf16_decode(buf, big_endian, &size);
    CuAssertIntEquals(tc, 0xfffd, actual);
    CuAssertIntEquals(tc, 2, size);
}

void test_utf16_rune_len(CuTest *tc) {
    CuAssertIntEquals(tc, 2, utf16_rune_len(0xff));
    CuAssertIntEquals(tc, 4, utf16_rune_len(0x10000));

    CuAssertIntEquals(tc, 0, utf16_rune_len(-1));
    CuAssertIntEquals(tc, 0, utf16_rune_len(0x110000));
}

void test_utf16_encode(CuTest *tc) {
    byte buf[4];
    byte expected[4];
    size_t size;

    size = utf16_encode(&buf[0], big_endian, 0x61);
    CuAssertIntEquals(tc, 2, size);
    copy_string(expected, "\x00\x61", 2);
    CuAssertIntEquals(tc, 0, utf16_cmp(expected, buf, big_endian, 2));

    size = utf16_encode(&buf[0], little_endian, 0x61);
    CuAssertIntEquals(tc, 2, size);
    copy_string(expected, "\x61\x00", 2);
    CuAssertIntEquals(tc, 0, utf16_cmp(expected, buf, little_endian, 2));

    size = utf16_encode(&buf[0], big_endian, 0x2615);
    CuAssertIntEquals(tc, 2, size);
    copy_string(expected, "\x26\x15", 2);
    CuAssertIntEquals(tc, 0, utf16_cmp(expected, buf, big_endian, 2));

    size = utf16_encode(&buf[0], little_endian, 0x2615);
    CuAssertIntEquals(tc, 2, size);
    copy_string(expected, "\x15\x26", 2);
    CuAssertIntEquals(tc, 0, utf16_cmp(expected, buf, little_endian, 2));

    size = utf16_encode(&buf[0], big_endian, 0x1f30e);
    CuAssertIntEquals(tc, 4, size);
    copy_string(expected, "\xd8\x3c\xdf\x0e", 4);
    CuAssertIntEquals(tc, 0, utf16_cmp(expected, buf, big_endian, 4));

    size = utf16_encode(&buf[0], little_endian, 0x1f30e);
    CuAssertIntEquals(tc, 4, size);
    copy_string(expected, "\x3c\xd8\x0e\xdf", 4);
    CuAssertIntEquals(tc, 0, utf16_cmp(expected, buf, big_endian, 4));
}

CuSuite* utf16_suite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_utf16_decode);
    SUITE_ADD_TEST(suite, test_utf16_rune_len);
    SUITE_ADD_TEST(suite, test_utf16_encode);
    return suite;
}
