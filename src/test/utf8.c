#include "CuTest.h"

#include <ustring.h>

#define copy_string(dest, str, len) for (int i = 0; i < len; i++) dest[i] = str[i]

void test_utf8_decode(CuTest *tc) {
    rune actual;
    size_t size;
    byte buf[5];

    copy_string(buf, "", 1);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0, actual);
    CuAssertIntEquals(tc, 1, size);

    copy_string(buf, "\xe2", 2);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0xfffd, actual);
    CuAssertIntEquals(tc, 2, size);

    copy_string(buf, "\xc2\xa1", 2);
    actual = utf8_decode(buf, &size);
    CuAssertIntEquals(tc, 0xa1, actual);
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

    actual = utf8_decode(buf, NULL);
    CuAssertIntEquals(tc, 0x1f387, actual);
}

void test_utf8_decode_string(CuTest *tc) {
    rune actual[13];
    rune expected[13] = {'s', 'o', 'm', 'e', ' ', 's', 't', 'r', 'i', 'n', 'g', 0x1F387, 0};

    byte string[] = {"some string\xf0\x9f\x8e\x87"};
    byte *p = &string[0];

    size_t size;
    for (int i = 0; ; i++) {
        actual[i] = utf8_decode(p, &size);
        p += size;

        if (actual[i] == 0) {
            break;
        }
    }

    for (int i = 0; i < 13; i++) {
        CuAssertIntEquals(tc, expected[i], actual[i]);
    }
}

void test_utf8_len(CuTest *tc) {
    CuAssertIntEquals(tc, 0, utf8_len(0x80));
    CuAssertIntEquals(tc, 1, utf8_len(0x61));
    CuAssertIntEquals(tc, 2, utf8_len(0xc6));
    CuAssertIntEquals(tc, 3, utf8_len(0xe2));
    CuAssertIntEquals(tc, 4, utf8_len(0xf0));
    CuAssertIntEquals(tc, 0, utf8_len(0xf8));
}

void test_utf8_rune_len(CuTest *tc) {
    CuAssertIntEquals(tc, 1, utf8_rune_len(0x61));
    CuAssertIntEquals(tc, 2, utf8_rune_len(0xa1));
    CuAssertIntEquals(tc, 3, utf8_rune_len(0x2622));
    CuAssertIntEquals(tc, 4, utf8_rune_len(0x1f387));

    CuAssertIntEquals(tc, 0, utf8_rune_len(-1));
    CuAssertIntEquals(tc, 0, utf8_rune_len(0x110000));
}

void test_utf8_encode(CuTest *tc) {
    byte buf[5];
    char str[5];
    size_t size;

    size = utf8_encode(&buf[0], 0x61);
    CuAssertIntEquals(tc, 1, size);
    buf[size] = 0;
    copy_string(str, buf, 2);
    CuAssertStrEquals(tc, "\x61", str);

    size = utf8_encode(&buf[0], 0xa1);
    CuAssertIntEquals(tc, 2, size);
    buf[size] = 0;
    copy_string(str, buf, 3);
    CuAssertStrEquals(tc, "\xc2\xa1", str);

    size = utf8_encode(&buf[0], 0x2622);
    CuAssertIntEquals(tc, 3, size);
    buf[size] = 0;
    copy_string(str, buf, 4);
    CuAssertStrEquals(tc, "\xe2\x98\xa2", str);

    size = utf8_encode(&buf[0], 0x1f387);
    CuAssertIntEquals(tc, 4, size);
    buf[size] = 0;
    copy_string(str, buf, 5);
    CuAssertStrEquals(tc, "\xf0\x9f\x8e\x87", str);

    CuAssertIntEquals(tc, 0, utf8_encode(&buf[0], -1));
    CuAssertIntEquals(tc, 0, utf8_encode(&buf[0], 0x110000));
}

CuSuite* utf8_suite() {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_utf8_decode);
    SUITE_ADD_TEST(suite, test_utf8_decode_string);
    SUITE_ADD_TEST(suite, test_utf8_len);
    SUITE_ADD_TEST(suite, test_utf8_rune_len);
    SUITE_ADD_TEST(suite, test_utf8_encode);
    return suite;
}
