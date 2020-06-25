#include "CuTest.h"

#include <ustring.h>

#define copy_string(dest, str, len) for (int i = 0; i < len; i++) dest[i] = str[i]

void test_utf8_decode(CuTest *tc) {
    rune actual;
    size_t size;
    byte buf[5];

    copy_string(buf, "", 1);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0xfffd, actual);
    CuAssertIntEquals(tc, 1, size);

    copy_string(buf, "\xe2", 2);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0xfffd, actual);
    CuAssertIntEquals(tc, 2, size);

    copy_string(buf, "\xe2\x98", 3);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0xfffd, actual);
    CuAssertIntEquals(tc, 3, size);

    copy_string(buf, "\x61", 2);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0x61, actual);
    CuAssertIntEquals(tc, 1, size);

    copy_string(buf, "\xe2\x98\xa2", 4);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0x2622, actual);
    CuAssertIntEquals(tc, 3, size);

    copy_string(buf, "\xf0\x9f\x8e\x87", 5);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0x1f387, actual);
    CuAssertIntEquals(tc, 4, size);
}

void test_utf8_len(CuTest *tc) {
    CuAssertIntEquals(tc, 0, utf8_len('\x80'));
    CuAssertIntEquals(tc, 1, utf8_len('\x61'));
    CuAssertIntEquals(tc, 2, utf8_len('\xc6'));
    CuAssertIntEquals(tc, 3, utf8_len('\xe2'));
    CuAssertIntEquals(tc, 4, utf8_len('\xf0'));
}

CuSuite* utf8_suite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_utf8_decode);
    SUITE_ADD_TEST(suite, test_utf8_len);
    return suite;
}
