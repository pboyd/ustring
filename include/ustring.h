#ifndef USTRING_H
#define USTRING_H 1

#include <stddef.h>

#define UNICODE_MAX 0x10FFFF
#define UNICODE_REPLACEMENT 0xFFFD

/** rune is a type large enough to hold any Unicode code point */
typedef int rune;

/** byte is a type with 8 bits */
typedef unsigned char byte;

/** word is a type with 16 bits */
typedef unsigned short int word;

enum byte_order {
    big_endian,
    little_endian,
};

/**
 * utf8_decode returns the value of the first UTF-8 code point in buf.
 *
 * utf8_decode will stop at the first invalid byte and return the Unicode
 * replace character (U+FFFD).
 *
 * To prevent an overrun, ensure that buf ends with a NULL byte.
 *
 * \param buf the buffer to read from
 * \param size if not NULL, the number of bytes used by the code point will be returned.
 */
rune utf8_decode(byte *buf, size_t *size);

/**
 * utf8_len returns the number of bytes in a UTF-8 encoded code point according to the first byte of the encoded text.
 *
 * If the byte is not valid a length of 0 is returned.
 *
 * \param b the first byte of the encoded text.
 */
size_t utf8_len(byte b);

/**
 * utf8_rune_len returns the number of bytes needed to store the rune in UTF-8.
 *
 * If the rune is not a valid Unicode code point value (i.e. less than zero or
 * greater than or equal to 0x10ffff), a 0 length is returned.
 */
size_t utf8_rune_len(rune r);

/**
 * utf8_encode encodes the rune r in UTF-8 and copies the value to buf.
 *
 * buf should have enough space to hold the encoded text, which is at most 4
 * bytes. Use \ref utf8_rune_len for a precise size.
 *
 * Returns the number of bytes set in the buffer.
 */
size_t utf8_encode(byte *buf, rune r);

/**
 * utf16_decode returns the value of the first UTF-16 code point in buf.
 *
 * utf16_decode will stop at the first invalid byte and return the Unicode
 * replace character (U+FFFD).
 *
 * \param buf the buffer to read from
 * \param byte_order either big_endian or little_endian
 * \param size if not NULL, the number of bytes used by the code point will be returned.
 */
rune utf16_decode(byte *buf, enum byte_order byte_order, size_t *size);

/**
 * utf16_rune_len returns the number of bytes needed to store the rune in UTF-16.
 *
 * If the rune is not a valid Unicode code point value (i.e. less than zero or
 * greater than or equal to 0x10ffff), a 0 length is returned.
 */
size_t utf16_rune_len(rune r);

/**
 * utf16_encode encodes the rune r in UTF-16 and copies the value to buf.
 *
 * buf should have enough space to hold the encoded text, which is at most 4
 * bytes. Use \ref utf16_rune_len for a precise size.
 *
 * Returns the number of bytes set in the buffer.
 */
size_t utf16_encode(byte *buf, enum byte_order byte_order, rune r);

/** utf16_cmp compares two UTF-16 byte strings.
 *
 * Returns 0 if the strings are equal, non-zero if the strings differ. If s1 is
 * less than s2 a negative values is returned. If s2 is less than s1 a positive
 * value is returned. The comparison is done by code point value and should not
 * be used for sorting.
 *
 * The comparison stops when a NULL word is found (U+0000) in either string, or
 * n bytes are read.
 *
 * \param s1 pointer to the beginning of the first UTF-16 string
 * \param s2 pointer to the beginning of the second UTF-16 string
 * \param byte_order either big_endian or little_endian
 * \param n maximum number of bytes (not code points, not words) to examine. If n < 0 it will scan until a NULL word.
 */
int utf16_cmp(byte *s1, byte *s2, enum byte_order byte_order, size_t n);

#endif
