#ifndef USTRING_H
#define USTRING_H 1

#include <stddef.h>

/** \brief rune is a type large enough to hold any Unicode code point */
typedef int rune;

/** \brief byte is a type with 8 bits */
typedef unsigned char byte;

/**
 * \brief utf_decode finds and returns the value of the first UTF-8 code point in buf.
 *
 * utf_decode will stop at the first invalid byte and return the Unicode replace character (U+FFFD).
 *
 * To prevent an overrun, ensure that buf ends with a NULL byte.
 *
 * \param buf the buffer to read a code point from
 * \param size if not NULL, the number of bytes used by the code point will be returned.
 */
rune utf8_decode(byte *buf, size_t *size);

/**
 * \brief utf8_len returns the number of bytes in a UTF-8 encoded code point according to the first byte of the encoded text.
 *
 * If the byte is not valid a length of 0 is returned.
 *
 * \param b the first byte of the encoded text.
 */
size_t utf8_len(byte b);

/**
 * \brief utf8_rune_len returns the number of bytes needed to store the rune in UTF-8.
 *
 * If the rune is not a valid Unicode code point value (i.e. less than zero or
 * greater than or equal to 0x10ffff), a 0 length is returned.
 */
size_t utf8_rune_len(rune r);

/**
 * \brief utf8_encode encodes the rune r in UTF-8 and copies the value to buf.
 *
 * buf should have enough space to hold the encoded text, which is at most 4
 * bytes. Use \ref utf8_rune_len for a precise size.
 *
 * Returns the number of bytes set in the buffer.
 */
size_t utf8_encode(byte *buf, rune r);

#endif
